#ifndef SM5714_H
#define SM5714_H

#include <ntddk.h>
#include "driver.h"

NTSTATUS
write_reg(
	PDEVICE_CONTEXT pDevice,
	unsigned long spbIndex,
	unsigned char reg,
	unsigned short data
);

NTSTATUS
read_reg(
	PDEVICE_CONTEXT pDevice,
	unsigned long spbIndex,
	unsigned char reg,
	unsigned short* data
);

NTSTATUS
update_reg(
	PDEVICE_CONTEXT pDevice,
	unsigned long spbIndex,
	unsigned char reg,
	unsigned short mask,
	unsigned short val
);

#endif // SM5714_H
