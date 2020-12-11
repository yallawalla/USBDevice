#include <string.h>
#include "msc_if.h"

__weak	HAL_StatusTypeDef	FLASH_Program(uint32_t Address, uint32_t Data) {
			HAL_StatusTypeDef status;
			__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR );
			if(*(uint32_t *)Address !=  Data) {
				HAL_FLASH_Unlock();
				do
					status=HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,Data);
				while(status == HAL_BUSY);
				HAL_FLASH_Lock();
			}	
			return status;
}

typedef  unsigned char BYTE;
typedef  unsigned int	 UINT;
typedef  unsigned int	 DWORD;

#define FATFS_SECTOR		FLASH_Sector_6
#define	PAGE_SIZE				0x20000
#define	PAGE_COUNT			5
#define	SECTOR_SIZE			512
#define	CLUSTER_SIZE		4096
#define	SECTOR_COUNT		(int)(PAGE_SIZE*PAGE_COUNT/(SECTOR_SIZE+4))
#define FATFS_ADDRESS		0x8040000

__weak	USBD_ReturnType USER_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
	int i,*p,*q=NULL;
	
	for(p=(int *)FATFS_ADDRESS; (int)p < FATFS_ADDRESS + PAGE_SIZE*PAGE_COUNT &&  p[SECTOR_SIZE/4]!=-1; p=&p[SECTOR_SIZE/4+1]) {
		if(p[SECTOR_SIZE/4] == sector)
			q=p;
	}
	if((int)p >= FATFS_ADDRESS + PAGE_SIZE*PAGE_COUNT)
		return USBD_E_ERROR;
	if(q)
		p=q;
	q=(int *)buff;
	for(i=0;i<SECTOR_SIZE/4; ++i)
		*q++=~(*p++);	
	if(--count)
		return USER_read (pdrv, (uint8_t *)q, ++sector, count);
	return USBD_E_OK;
  /* USER CODE END READ */
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
__weak	USBD_ReturnType USER_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{
  /* USER CODE BEGIN WRITE */
  /* USER CODE HERE */
	int i,*p,*q=NULL;
		
	for(p=(int *)FATFS_ADDRESS; p[SECTOR_SIZE/4]!=-1; p=&p[SECTOR_SIZE/4+1])
		if((int)p >= FATFS_ADDRESS + PAGE_SIZE*PAGE_COUNT)
			return USBD_E_ERROR;

	q=(int *)buff;
	for(i=0; i<SECTOR_SIZE/4; ++i)	
		if(*q++)
			break;

	if(i<SECTOR_SIZE/4) {												// all zeroes ???
		q=(int *)buff;
		for(i=0; i<SECTOR_SIZE/4; ++i,++p,++q)
			FLASH_Program((int)p,~(*q));
		FLASH_Program((int)p,sector);
	}
	
	if(--count)
		return USER_write (pdrv, (uint8_t *)q, ++sector, count);
	return USBD_E_OK;
  /* USER CODE END WRITE */
}
/*
 * msc_if.c
 *
 *  Created on: Jan 25, 2019
 *      Author: vazso
 */


USBD_ReturnType Read_dev (uint8_t lun, uint8_t *dest, uint32_t blockAddr, uint16_t blockLen);
USBD_ReturnType Write_dev (uint8_t lun, uint8_t *src, uint32_t blockAddr, uint16_t blockLen);
void Init_dev(uint8_t lun);
void DeInit_dev(uint8_t lun);

USBD_MSC_LUStatusType Status_dev =  {
        .BlockCount=SECTOR_COUNT,
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
        .ProductId = "Flash Storage",
        .VersionId = "1.01"
	},
	{
        .PeriphType = SCSI_PERIPH_SBC_2,
        .RMB = 1,
        .Version = 2,
        .RespDataFormat = 2,
        .AddLength = sizeof(USBD_SCSI_StdInquiryType) - 4,
        .VendorId = "L405",
        .ProductId = "RAM Storage",
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
		.Status=&Status_dev,
		.Inquiry=&Inquiry_dev[1],
		.Init=Init_dev,
		.Deinit=DeInit_dev
	}
};

USBD_MSC_IfHandleType hmsc_if = {
	.Base.AltCount = 1,
        .LUs=msc_lu,
}, *const msc_if = &hmsc_if;

USBD_ReturnType Read_dev (uint8_t lun, uint8_t *dest, uint32_t blockAddr, uint16_t blockLen){/*!< Read media block */
	return USER_read(lun,dest,blockAddr,blockLen);
}

USBD_ReturnType Write_dev (uint8_t lun, uint8_t *src, uint32_t blockAddr, uint16_t blockLen){/*!< Write media block */
	return USER_write(lun,src,blockAddr,blockLen);
}

void Init_dev(uint8_t lun){
	msc_if->LUs[lun].Status->Ready=1;
	msc_if->LUs[lun].Status->Writable=1;
}

void DeInit_dev(uint8_t lun){
}
