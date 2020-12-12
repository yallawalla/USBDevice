/*
 * cdc_if.c
 *
 * 
 * 
 */
#include <string.h>
#include "cdc_if.h"

__weak	void	VCP_Init(void) {};
__weak	void	VCP_DeInit(void) {};
__weak	void	VCP_Receive(uint8_t *pbuf, uint16_t length) {
							VCP_Transmit(pbuf, length);
							}


bool		VCP_Ready=false;
static	uint8_t rxbuf[128];

void vcp_data_ready(void* itf, uint8_t * pbuf, uint16_t length) {
		USBD_CDC_Receive(itf, rxbuf, length);
		VCP_Receive(rxbuf,length);
}
void vcp_data_sent(void* itf, uint8_t * pbuf, uint16_t length) {
		VCP_Ready=true;
}
void vcp_open(void* itf, USBD_CDC_LineCodingType *p) {
		VCP_Init();
		USBD_CDC_Receive(itf, rxbuf, 128);
		VCP_Ready=true;
}
void vcp_close(void* itf) {
		VCP_DeInit();
}

static const USBD_CDC_AppType console_app =
{
		.Name           = "standard I/O",
    .Open           = vcp_open,
    .Close          = vcp_close,
    .Received       = vcp_data_ready,
    .Transmitted    = vcp_data_sent,
};

USBD_CDC_IfHandleType hcdc_if = {
		.App = &console_app,
		.Base.AltCount = 1
},	*const cdc_if = &hcdc_if;


void VCP_Transmit(uint8_t * pbuf, uint16_t length) {
		VCP_Ready=false;
		USBD_CDC_Transmit(cdc_if,pbuf, length);
}


