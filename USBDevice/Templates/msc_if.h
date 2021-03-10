/*
 * msc_if.h
 *
 *  Created on: Jan 25, 2019
 *      Author: vazso
 */

#ifndef MSC_IF_H_
#define MSC_IF_H_

#include <usbd_msc.h>

extern USBD_MSC_IfHandleType *const msc_if;

USBD_ReturnType Read_dev (uint8_t, uint8_t *, uint32_t, uint16_t);
USBD_ReturnType Write_dev (uint8_t, uint8_t *, uint32_t, uint16_t);
void Init_dev(uint8_t);
void DeInit_dev(uint8_t);

#define	SECTOR_SIZE		512
#define _RAM					((uint8_t *)0x10000000)

USBD_ReturnType disk_initialize (uint8_t);
USBD_ReturnType disk_read		(uint8_t, uint8_t *, uint32_t, uint16_t);
USBD_ReturnType disk_write	(uint8_t, uint8_t *, uint32_t, uint16_t);
USBD_ReturnType disk_ioctl	(uint8_t, uint8_t , void *);
USBD_ReturnType disk_status (uint8_t);

#endif /* MSC_IF_H_ */
