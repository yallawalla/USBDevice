/*
 * cdc_if.c
 *
 * 
 * 
 */
#include <string.h>
#include "cdc_if.h"

__weak	void	cdcInit(void) {};
__weak	void	cdcDeInit(void) {};
__weak	void	cdcReceive(uint8_t *pbuf, uint16_t length) {
								cdcTransmit(pbuf, length);
							}


bool		cdcReady=false;
static	uint8_t rxbuf[128];

void vcp_data_ready(void* itf, uint8_t * pbuf, uint16_t length) {
		USBD_CDC_Receive(itf, rxbuf, length);
		cdcReceive(rxbuf,length);
}
void vcp_data_sent(void* itf, uint8_t * pbuf, uint16_t length) {
		cdcReady=true;
}
void vcp_open(void* itf, USBD_CDC_LineCodingType *p) {
		cdcInit();
		USBD_CDC_Receive(itf, rxbuf, 128);
		cdcReady=true;
}
void vcp_close(void* itf) {
		cdcDeInit();
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


void cdcTransmit(uint8_t * pbuf, uint16_t length) {
		cdcReady=false;
		USBD_CDC_Transmit(cdc_if,pbuf, length);
}


