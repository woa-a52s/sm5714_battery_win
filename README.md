# Siliconmitus SM5714 Battery Fuel Gauge Driver

- Project was based on [windows_hardware_battery_bq27742_src](https://github.com/WOA-Project/windows_hardware_battery_bq27742_src).

This driver is designed for the Siliconmitus SM5714 PMIC fuel gauges found on various Samsung devices. 

This driver enables Windows to get information about the battery pack used in Samsung devices utilizing the SM5714 PMIC. It does not provide charging capabilities.

## ACPI Sample

```asl
Device(BAT)
{
    Name (_HID, "SM5714")
    Name (_UID, 1)
    Name (_DEP, Package() {\_SB_.I2C4})

    Method (_CRS, 0x0, NotSerialized)
    {
        Name (RBUF, ResourceTemplate ()
        {
            I2CSerialBus (0x49, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.I2C4",,,,)

            GpioInt (Edge, ActiveLow, Exclusive, PullNone, 0, "\\_SB.GIO0") {54}
        })
        Return (RBUF)
    }
}
```
## Acknowledgements
* [Gustave Monce](https://github.com/gus33000)
* [WOA-Project](https://github.com/WOA-Project)
* [WOA-Msmnile](https://github.com/woa-msmnile)
* [sunflower2333](https://github.com/sunflower2333)