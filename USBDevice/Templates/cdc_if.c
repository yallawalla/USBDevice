/*
 * cdc_if.c
 *
 * 
 * 
 */

#include <string.h>
#include "cdc_if.h"

uint8_t buffer[128];
void console_if_out_cmplt(void* itf, uint8_t * pbuf, uint16_t length) {
	USBD_CDC_Receive(itf,buffer, length);
	USBD_CDC_Transmit(itf,buffer, length);
}

static const USBD_CDC_AppType console_app =
{
    .Name           = "standard I/O",
//    .Open           = console_if_open,
    .Received       = console_if_out_cmplt
//    .Transmitted    = console_if_in_cmplt,
};


USBD_CDC_IfHandleType hcdc_if = {
    .App = &console_app,
    .Base.AltCount = 1
}, *const cdc_if = &hcdc_if;
