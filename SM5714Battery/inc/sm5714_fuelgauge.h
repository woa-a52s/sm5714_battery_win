
NTSTATUS
sm5714_Get_CycleCount(
	PSM5714_BATTERY_FDO_DATA DevExt,
	PULONG CycleCount
);

NTSTATUS
sm5714_Get_BatteryTemperature(
	PSM5714_BATTERY_FDO_DATA DevExt,
	PULONG Temperature
);

NTSTATUS
sm5714_Get_BatterySoC(
	PSM5714_BATTERY_FDO_DATA DevExt,
	PULONG Capacity
);

NTSTATUS
sm5714_Get_BatteryVoltage(
	PSM5714_BATTERY_FDO_DATA DevExt,
	PULONG Voltage
);

NTSTATUS
sm5714_Get_BatteryCurrent(
	PSM5714_BATTERY_FDO_DATA DevExt,
	PULONG Current
);
