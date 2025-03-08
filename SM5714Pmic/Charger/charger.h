#ifndef _CHARGER_H_
#define _CHARGER_H_

#include "..\Common\driver.h"

// Function prototypes
int set_autostop(_In_ PDEVICE_CONTEXT pDevice, bool enable);
int set_input_current_limit(_In_ PDEVICE_CONTEXT pDevice, unsigned int mA);
int set_charging_current(_In_ PDEVICE_CONTEXT pDevice, unsigned int mA);
int set_topoff_current(_In_ PDEVICE_CONTEXT pDevice, unsigned int mA);
int charger_probe(_In_ PDEVICE_CONTEXT pDevice);
int enable_charging(_In_ PDEVICE_CONTEXT pDevice, bool enable);

#endif // _CHARGER_H_
