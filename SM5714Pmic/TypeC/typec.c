#include "..\Common\registers.h"
#include "..\Common\spbhelper.h"
#include "typec.h"

static ULONG DebugLevel = 100;
static ULONG DebugCatagories = DBG_INIT || DBG_PNP || DBG_IOCTL;

void udelay(ULONG usec) {
	LARGE_INTEGER Interval;
	Interval.QuadPart = -10 * (LONGLONG)usec;
	KeDelayExecutionThread(KernelMode, false, &Interval);
}

void msleep(ULONG msec) {
	udelay(msec * 1000);
}

// Work in progress