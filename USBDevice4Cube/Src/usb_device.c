/**
  ******************************************************************************
  * @file    usb_device.c
  * @author  Benedek Kupper
  * @version 0.1
  * @date    2018-11-03
  * @brief   USB device definition and initialization
  *
  * Copyright (c) 2018 Benedek Kupper
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */
#include <usb_device.h>
#include <usbd_cdc.h>
#include <usbd_msc.h>
/*
#define USBD_VID     1155
#define USBD_LANGID_STRING     1033
#define USBD_MANUFACTURER_STRING     "STMicroelectronics"
#define USBD_PID_FS     22314
#define USBD_PRODUCT_STRING_FS     "STM32 Mass Storage"
#define USBD_CONFIGURATION_STRING_FS     "MSC Config"
#define USBD_INTERFACE_STRING_FS     "MSC Interface"

#define USB_SIZ_BOS_DESC            0x0C
*/
/* USER CODE BEGIN PRIVATE_DEFINES */


/** @brief USB device configuration */
const USBD_DescriptionType hmsc_dev_cfg = {
    .Vendor = {
        .Name           = "Mocnik Matjaz",
        .ID             = 0xffff, //1155,
    },
    .Product = {
        .Name           = "Serial/Mass storage",
        .ID             = 0xffff, //22314,
        .Version.bcd    = 0x0100,
    },
    .Config = {
        .Name           = "Serial/Mass storage config",
        .MaxCurrent_mA  = 500,
        .RemoteWakeup   = 0,
        .SelfPowered    = 0,
    },
}, *const msc_dev_cfg = &hmsc_dev_cfg;

USBD_HandleType hUsbDevice, *const UsbDevice = &hUsbDevice;

extern USBD_CDC_IfHandleType *const cdc_if;
extern USBD_MSC_IfHandleType *const msc_if;

void UsbDevice_Init(void)
{
    msc_if->Config.InEpNum  = 0x81;
    msc_if->Config.OutEpNum = 0x01;
#ifdef _USE_RAMDRIVE	
		msc_if->Config.MaxLUN = 1;
#else
		msc_if->Config.MaxLUN = 0;
#endif
    USBD_MSC_MountInterface(msc_if, UsbDevice);

    cdc_if->Config.InEpNum  = 0x82;
    cdc_if->Config.OutEpNum = 0x02;
    cdc_if->Config.NotEpNum = 0x83;
    USBD_CDC_MountInterface(cdc_if, UsbDevice);


		USBD_Init(UsbDevice, msc_dev_cfg);
    USBD_Connect(UsbDevice);
}

void UsbDevice_DeInit(void)
{
    USBD_Disconnect(UsbDevice);
		USBD_Deinit(UsbDevice);
}
