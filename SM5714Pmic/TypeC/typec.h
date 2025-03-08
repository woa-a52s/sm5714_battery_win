#ifndef _TYPEC_H_
#define _TYPEC_H_

#include "..\Common\driver.h"

// Function prototypes
int manual_JIGON(_In_ PDEVICE_CONTEXT pDevice);
int TYPE_C_ATTACH_DRP(_In_ PDEVICE_CONTEXT pDevice);
int check_usb_killer(_In_ PDEVICE_CONTEXT pDevice);
int set_enable_pd_function(_In_ PDEVICE_CONTEXT pDevice);
#endif // _TYPEC_H_
