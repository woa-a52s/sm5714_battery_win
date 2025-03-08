# Siliconmitus SM5714 PMIC Driver

- Miniclass driver is based on [windows_hardware_battery_bq27742_src](https://github.com/WOA-Project/windows_hardware_battery_bq27742_src).
- Miniclass reference: [SansungTabS6-TypeC_Pmic](https://github.com/Project-Aloha/SansungTabS6-TypeC_Pmic)

This driver is designed for the Siliconmitus SM5714 PMIC fuel gauges found on various Samsung devices. 

This driver enables Windows to get information about the battery pack used in Samsung devices utilizing the SM5714 PMIC.

## Available features 

- Capacity (State of Charge) in %
- Voltage in mV
- Current mA
- Power state (Charging & Discharging & Critical)
- Charging

## PMIC ACPI Sample

```asl
Device(PM3P)
{
    Name (_HID, "SM5714P")
    Name (_UID, 1)
    Name (_DEP, Package() {\_SB_.I2C4, \_SB_.I2C9})

    Method (_CRS, 0x0, NotSerialized)
    {
        Name (RBUF, ResourceTemplate ()
        {
            // Charger Slave Address
            I2CSerialBus (0x49,, 400000, AddressingMode7Bit, "\\_SB.I2C4",,,,)

            // USBPD Slave Address
            I2CSerialBus (0x33,, 400000, AddressingMode7Bit, "\\_SB.I2C9",,,,)

            // Charger GPIO Interrupt
            GpioInt (Edge, ActiveLow, Shared, PullNone, 0, "\\_SB.GIO0") {54}

            // USBPD GPIO Interrupt
            GpioInt (Edge, ActiveLow, Shared, PullNone, 0, "\\_SB.GIO0") {140}
        })
        Return (RBUF)
    }
}
```

## Miniclass ACPI Sample

```asl
Device(BAT)
{
    Name (_HID, "SM5714F")
    Name (_UID, 1)
    Name (_DEP, Package() {\_SB_.I2C4})

    Method (_CRS, 0x0, NotSerialized)
    {
        Name (RBUF, ResourceTemplate ()
        {
            I2CSerialBus (0x71,, 400000, AddressingMode7Bit, "\\_SB.I2C4",,,,)

            GpioInt (Edge, ActiveLow, Shared, PullNone, 0, "\\_SB.GIO0") {54}
        })
        Return (RBUF)
    }
}
```

## Acknowledgements
* [Gustave Monce](https://github.com/gus33000)
* [map220v](https://github.com/map220v)
* [WOA-Project](https://github.com/WOA-Project)
* [WOA-Msmnile](https://github.com/woa-msmnile)
* [sunflower2333](https://github.com/sunflower2333)
