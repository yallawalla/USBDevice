/*
 * cdc_if.h
 *
 *
 *
 */

#ifndef CDC_IF_H_
#define CDC_IF_H_

#include <usbd_cdc.h>

extern		bool	cdcReady;
void			cdcInit(void), cdcDeInit(void);
void			cdcReceive(uint8_t *, uint16_t);
void 			cdcTransmit(uint8_t *, uint16_t);
	
#endif /* MSC_IF_H_ */
