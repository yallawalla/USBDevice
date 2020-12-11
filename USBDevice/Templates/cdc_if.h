/*
 * cdc_if.h
 *
 *
 *
 */

#ifndef CDC_IF_H_
#define CDC_IF_H_

#include <usbd_cdc.h>

extern		USBD_CDC_IfHandleType *const cdc_if;

void			VCP_Init(void), VCP_DeInit(void);
void			VCP_Receive(uint8_t *, uint16_t);
void 			VCP_Transmit(uint8_t *, uint16_t);
	
#endif /* MSC_IF_H_ */
