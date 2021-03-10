#include <string.h>
#include "msc_if.h"
#include "ramfmt.c"
/*
 * msc_if.c
 *
 *  Created on: Jan 25, 2019
 *      Author: vazso
 */
USBD_ReturnType Read_dev (uint8_t lun, uint8_t *dest, uint32_t blockAddr, uint16_t blockLen) {
	if(lun)
		return disk_read(lun,dest,blockAddr,blockLen);
	memcpy(dest,&_RAM[blockAddr*SECTOR_SIZE],SECTOR_SIZE*blockLen);
	return USBD_E_OK;
}
USBD_ReturnType Write_dev (uint8_t lun, uint8_t *src, uint32_t blockAddr, uint16_t blockLen) {
	if(lun)
		return disk_write(lun,src,blockAddr,blockLen);
	memcpy(&_RAM[blockAddr*SECTOR_SIZE],src,SECTOR_SIZE*blockLen);
	return USBD_E_OK;

}
void Init_dev(uint8_t lun) {
	msc_if->LUs[lun].Status->Ready=1;
	msc_if->LUs[lun].Status->Writable=1;
	if(lun)
		disk_initialize (lun);
	else
		memcpy(_RAM,ramFmt,sizeof(ramFmt));
}

void DeInit_dev(uint8_t lun) {
}


USBD_MSC_LUStatusType Status_dev =  {					// 64k RAM
        .BlockCount=128,
        .BlockSize=SECTOR_SIZE,
        .Ready=0,
        .Writable=0,
};
USBD_MSC_LUStatusType Status_dev_usr =  {			// 5*128k FLASH, 512 + word
        .BlockCount=1270,
        .BlockSize=SECTOR_SIZE,
        .Ready=0,
        .Writable=0,
};

const USBD_SCSI_StdInquiryType Inquiry_dev[] = {
	{
        .PeriphType = SCSI_PERIPH_SBC_2,
        .RMB = 1,
        .Version = 2,
        .RespDataFormat = 2,
        .AddLength = sizeof(USBD_SCSI_StdInquiryType) - 4,
        .VendorId = "L405",
        .ProductId = "RAM Storage",
        .VersionId = "1.01"
	},
	{
        .PeriphType = SCSI_PERIPH_SBC_2,
        .RMB = 1,
        .Version = 2,
        .RespDataFormat = 2,
        .AddLength = sizeof(USBD_SCSI_StdInquiryType) - 4,
        .VendorId = "L405",
        .ProductId = "Flash Storage",
        .VersionId = "1.02"
			},
};

USBD_MSC_LUType msc_lu[] = {
	{
		.Read=Read_dev,
		.Write=Write_dev,
		.Status=&Status_dev,
		.Inquiry=&Inquiry_dev[0],
		.Init=Init_dev,
		.Deinit=DeInit_dev
	},
	{
		.Read=Read_dev,
		.Write=Write_dev,
		.Status=&Status_dev_usr,
		.Inquiry=&Inquiry_dev[1],
		.Init=Init_dev,
		.Deinit=DeInit_dev
	}
};

USBD_MSC_IfHandleType hmsc_if = {
	.Base.AltCount = 1,
	.LUs=msc_lu,
}, *const msc_if = &hmsc_if;


__weak	USBD_ReturnType disk_initialize (uint8_t lun) {
	Init_dev(0);
	return USBD_E_OK;
}
__weak	USBD_ReturnType disk_read(uint8_t lun, uint8_t *dest, uint32_t blockAddr, uint16_t blockLen) {
	return Read_dev(0,dest,blockAddr,blockLen);
}
__weak	USBD_ReturnType disk_write	(uint8_t lun, uint8_t *dest, uint32_t blockAddr, uint16_t blockLen) {
	return Write_dev(0,dest,blockAddr,blockLen);
}

