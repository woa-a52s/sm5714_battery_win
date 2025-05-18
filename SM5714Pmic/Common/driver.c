#include "driver.h"
#include "..\Charger\charger.h"
#include "..\TypeC\typec.h"

static ULONG DebugLevel = 100;
static ULONG DebugCatagories = DBG_INIT || DBG_PNP || DBG_IOCTL;

#define GET_INTEGER(_arg_) (*(PULONG UNALIGNED)((_arg_)->Data))

static
NTSTATUS
FetchPmicConfig(
    _In_  WDFDEVICE        Device,
    _Out_ PDEVICE_CONTEXT  DevCtx
)
{
    ACPI_EVAL_INPUT_BUFFER input = { 0 };
    input.Signature = ACPI_EVAL_INPUT_BUFFER_SIGNATURE;
    memcpy(input.MethodName, "PMIC", 4);

    const ULONG outLen =
        sizeof(ACPI_EVAL_OUTPUT_BUFFER) + 8 * sizeof(ACPI_METHOD_ARGUMENT);

    PACPI_EVAL_OUTPUT_BUFFER output =
        (PACPI_EVAL_OUTPUT_BUFFER)ExAllocatePoolZero(
            NonPagedPoolNx, outLen, 'cimP');
    if (!output)
        return STATUS_INSUFFICIENT_RESOURCES;

    WDF_MEMORY_DESCRIPTOR inDesc, outDesc;
    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&inDesc, &input, sizeof(input));
    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&outDesc, output, outLen);

    ULONG_PTR bytesReturned = 0;
    NTSTATUS status = WdfIoTargetSendIoctlSynchronously(
        WdfDeviceGetIoTarget(Device),
        NULL,
        IOCTL_ACPI_EVAL_METHOD,
        &inDesc,
        &outDesc,
        NULL,
        &bytesReturned);

    if (NT_SUCCESS(status) &&
        output->Signature == ACPI_EVAL_OUTPUT_BUFFER_SIGNATURE &&
        output->Count >= 4 &&
        output->Argument[0].Type == ACPI_METHOD_ARGUMENT_INTEGER &&
        output->Argument[1].Type == ACPI_METHOD_ARGUMENT_INTEGER &&
        output->Argument[2].Type == ACPI_METHOD_ARGUMENT_INTEGER &&
        output->Argument[3].Type == ACPI_METHOD_ARGUMENT_INTEGER)
    {
        DevCtx->Autostop = GET_INTEGER(&output->Argument[0]) ? TRUE : FALSE;
        DevCtx->InputCurrentLimit = GET_INTEGER(&output->Argument[1]);
        DevCtx->ChargingCurrent = GET_INTEGER(&output->Argument[2]);
        DevCtx->TopoffCurrent = GET_INTEGER(&output->Argument[3]);
    }
    else
    {
        status = STATUS_ACPI_INVALID_DATA;
    }

    ExFreePool(output);
    return status;
}

NTSTATUS
DriverEntry(
    __in PDRIVER_OBJECT  DriverObject,
    __in PUNICODE_STRING RegistryPath
)
{
    NTSTATUS               status = STATUS_SUCCESS;
    WDF_DRIVER_CONFIG      config;
    WDF_OBJECT_ATTRIBUTES  attributes;

    Print(DEBUG_LEVEL_INFO, DBG_INIT, "Driver Entry\n");

    WDF_DRIVER_CONFIG_INIT(&config, EvtDeviceAdd);

    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);

    //
    // Create a framework driver object to represent our driver.
    //
    status = WdfDriverCreate(DriverObject, RegistryPath, &attributes, &config, WDF_NO_HANDLE);
    if (!NT_SUCCESS(status))
    {
        Print(DEBUG_LEVEL_ERROR, DBG_INIT, "WdfDriverCreate failed with status 0x%x\n", status);
    }
    return status;
}

NTSTATUS
OnPrepareHardware(
    _In_  WDFDEVICE     FxDevice,
    _In_  WDFCMRESLIST  FxResourcesRaw,
    _In_  WDFCMRESLIST  FxResourcesTranslated
)
/*++

Routine Description:

This routine caches the SPB resource connection ID.

Arguments:

FxDevice - a handle to the framework device object
FxResourcesRaw - list of translated hardware resources that
the PnP manager has assigned to the device
FxResourcesTranslated - list of raw hardware resources that
the PnP manager has assigned to the device

Return Value:

Status

--*/
{
    PDEVICE_CONTEXT pDevice = GetDeviceContext(FxDevice);
    NTSTATUS status = STATUS_INSUFFICIENT_RESOURCES;
    pDevice->SpbContextCount = 0;  // Start with zero I�C handles

    UNREFERENCED_PARAMETER(FxResourcesRaw);

    //
    // Parse the peripheral's resources.
    //
    ULONG resourceCount = WdfCmResourceListGetCount(FxResourcesTranslated);

    for (ULONG i = 0; i < resourceCount; i++)
    {
        PCM_PARTIAL_RESOURCE_DESCRIPTOR pDescriptor =
            WdfCmResourceListGetDescriptor(FxResourcesTranslated, i);

        if (pDescriptor == NULL)
        {
            continue;
        }

        if (pDescriptor->Type == CmResourceTypeConnection &&
            pDescriptor->u.Connection.Class == CM_RESOURCE_CONNECTION_CLASS_SERIAL &&
            pDescriptor->u.Connection.Type == CM_RESOURCE_CONNECTION_TYPE_SERIAL_I2C)
        {
            // We found an I�C resource. Use pDevice->SpbContexts[pDevice->SpbContextCount].
            if (pDevice->SpbContextCount >= ARRAYSIZE(pDevice->SpbContexts))
            {
                // We only have space for 2 in this example. You can handle this however you like.
                status = STATUS_BUFFER_TOO_SMALL;
                break;
            }

            SPB_CONTEXT* spbCtx = &pDevice->SpbContexts[pDevice->SpbContextCount];

            // Store the resource IDs
            spbCtx->I2cResHubId.LowPart = pDescriptor->u.Connection.IdLowPart;
            spbCtx->I2cResHubId.HighPart = pDescriptor->u.Connection.IdHighPart;

            // Initialize (open) the SPB target for this address
            status = SpbTargetInitialize(FxDevice, spbCtx);
            if (!NT_SUCCESS(status))
            {
                // If it fails, you can break or keep trying other resources
                break;
            }

            pDevice->SpbContextCount++;
        }
    }

    // If we never found any I�C connections, fail
    if (pDevice->SpbContextCount == 0)
    {
        status = STATUS_NOT_FOUND;
    }



    return status;
}

NTSTATUS
OnReleaseHardware(
    _In_  WDFDEVICE     FxDevice,
    _In_  WDFCMRESLIST  FxResourcesTranslated
)
/*++

Routine Description:

Arguments:

FxDevice - a handle to the framework device object
FxResourcesTranslated - list of raw hardware resources that
the PnP manager has assigned to the device

Return Value:

Status

--*/
{
    PDEVICE_CONTEXT pDevice = GetDeviceContext(FxDevice);
    UNREFERENCED_PARAMETER(FxResourcesTranslated);

    // Deinitialize each SPB_CONTEXT in the array
    for (ULONG i = 0; i < pDevice->SpbContextCount; i++)
    {
        SpbTargetDeinitialize(FxDevice, &pDevice->SpbContexts[i]);
    }

    pDevice->SpbContextCount = 0;

    return STATUS_SUCCESS;
}

NTSTATUS
OnD0Entry(
    _In_  WDFDEVICE               FxDevice,
    _In_  WDF_POWER_DEVICE_STATE  FxPreviousState
)
/*++

Routine Description:

This routine allocates objects needed by the driver.

Arguments:

FxDevice - a handle to the framework device object
FxPreviousState - previous power state

Return Value:

Status

--*/
{
    UNREFERENCED_PARAMETER(FxPreviousState);

    PDEVICE_CONTEXT pDevice = GetDeviceContext(FxDevice);
    NTSTATUS status = STATUS_SUCCESS;
    Print(DEBUG_LEVEL_INFO, DBG_PNP, "OnD0Entry called\n");

    status = FetchPmicConfig(FxDevice, pDevice);
    if (!NT_SUCCESS(status))
    {
        Print(DEBUG_LEVEL_INFO, DBG_INIT, "PMIC ACPI method missing\n");
        return status;
    }

    Print(DEBUG_LEVEL_INFO, DBG_INIT,
        "PMIC cfg: Autostop=%s  ICL=%lu mA  ICHG=%lu mA  TOP=%lu mA\n",
        pDevice->Autostop ? "ON" : "OFF",
        pDevice->InputCurrentLimit,
        pDevice->ChargingCurrent,
        pDevice->TopoffCurrent);

    // Configure charging
    status = charger_probe(pDevice);
    if (!NT_SUCCESS(status))
    {
        Print(DEBUG_LEVEL_ERROR, DBG_IOCTL, "Error configuring charging settings - %!STATUS!", status);
        goto exit;
    }
    else {
        Print(DEBUG_LEVEL_INFO, DBG_INIT, "Charger parameters configured sucessfully!\n");
    }

    // Enable charging
    status = enable_charging(pDevice, true);
    if (!NT_SUCCESS(status))
    {
        Print(DEBUG_LEVEL_ERROR, DBG_IOCTL, "Error enabling charging - %!STATUS!", status);
        goto exit;
    }

    status = WdfWaitLockCreate(WDF_NO_OBJECT_ATTRIBUTES, &pDevice->DataLock);
	if (!NT_SUCCESS(status))
	{
		Print(DEBUG_LEVEL_ERROR, DBG_IOCTL, "Error creating Data Waitlock - %!STATUS!", status);
		goto exit;
	}

exit:
    return status;
}

NTSTATUS
OnD0Exit(
    _In_  WDFDEVICE               FxDevice,
    _In_  WDF_POWER_DEVICE_STATE  FxPreviousState
)
/*++

Routine Description:

This routine destroys objects needed by the driver.

Arguments:

FxDevice - a handle to the framework device object
FxPreviousState - previous power state

Return Value:

Status

--*/
{

    PDEVICE_CONTEXT pDevice = GetDeviceContext(FxDevice);
    NTSTATUS status = STATUS_SUCCESS;

    // Only disable charging if transitioning to OFF state (S5)
    if (FxPreviousState == WdfPowerDeviceD3Final)
    {
        enable_charging(pDevice, false);
    }

    if (pDevice->DataLock != NULL)
    {
        WdfObjectDelete(pDevice->DataLock);
    }
    return status;
}

NTSTATUS
EvtDeviceAdd(
    IN WDFDRIVER       Driver,
    IN PWDFDEVICE_INIT DeviceInit
)
{
    NTSTATUS                      status = STATUS_SUCCESS;
    WDF_IO_QUEUE_CONFIG           queueConfig;
    WDF_OBJECT_ATTRIBUTES         attributes;
    WDFDEVICE                     device;
    WDFQUEUE                      queue;
    PDEVICE_CONTEXT               devContext;

    UNREFERENCED_PARAMETER(Driver);

    PAGED_CODE();

    Print(DEBUG_LEVEL_INFO, DBG_PNP, "EvtDeviceAdd called\n");

    {
        WDF_PNPPOWER_EVENT_CALLBACKS pnpCallbacks;
        WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpCallbacks);

        pnpCallbacks.EvtDevicePrepareHardware = OnPrepareHardware;
        pnpCallbacks.EvtDeviceReleaseHardware = OnReleaseHardware;
        pnpCallbacks.EvtDeviceD0Entry = OnD0Entry;
        pnpCallbacks.EvtDeviceD0Exit = OnD0Exit;

        WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpCallbacks);
    }

    //
    // Setup the device context
    //
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, DEVICE_CONTEXT);

    //
    // Create a framework device object.This call will in turn create
    // a WDM device object, attach to the lower stack, and set the
    // appropriate flags and attributes.
    //
    status = WdfDeviceCreate(&DeviceInit, &attributes, &device);
    if (!NT_SUCCESS(status))
    {
        Print(DEBUG_LEVEL_ERROR, DBG_PNP, "WdfDeviceCreate failed with status code 0x%x\n", status);
        return status;
    }

    {
        WDF_DEVICE_STATE deviceState;
        WDF_DEVICE_STATE_INIT(&deviceState);

        deviceState.NotDisableable = WdfFalse;
        WdfDeviceSetDeviceState(device, &deviceState);
    }

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchParallel);

    queueConfig.EvtIoInternalDeviceControl = EvtInternalDeviceControl;

    status = WdfIoQueueCreate(device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);
    if (!NT_SUCCESS(status))
    {
        Print(DEBUG_LEVEL_ERROR, DBG_PNP, "WdfIoQueueCreate failed 0x%x\n", status);
        return status;
    }

    //
    // Create manual I/O queue to take care of hid report read requests
    //
    devContext = GetDeviceContext(device);
    devContext->FxDevice = device;

    WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);

    queueConfig.PowerManaged = WdfFalse;

    status = WdfIoQueueCreate(device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &devContext->ReportQueue);
    if (!NT_SUCCESS(status))
    {
        Print(DEBUG_LEVEL_ERROR, DBG_PNP, "WdfIoQueueCreate failed 0x%x\n", status);
        return status;
    }
    return status;
}

VOID
EvtInternalDeviceControl(
    IN WDFQUEUE     Queue,
    IN WDFREQUEST   Request,
    IN size_t       OutputBufferLength,
    IN size_t       InputBufferLength,
    IN ULONG        IoControlCode
)
{
    NTSTATUS            status = STATUS_SUCCESS;
    WDFDEVICE           device;
    PDEVICE_CONTEXT     devContext;

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    device = WdfIoQueueGetDevice(Queue);
    devContext = GetDeviceContext(device);

    switch (IoControlCode)
    {
    default:
        status = STATUS_NOT_SUPPORTED;
        break;
    }

    WdfRequestComplete(Request, status);

    return;
}