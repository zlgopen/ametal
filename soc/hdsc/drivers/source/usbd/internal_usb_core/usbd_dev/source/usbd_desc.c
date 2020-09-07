/******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */

/******************************************************************************/
/** \file usbd_desc.c
 **
 ** A detailed description is available at
 ** @link
        This file provides the USBD descriptors and string formating method.
    @endlink
 **
 **   - 2019-05-15  1.0  Zhangxl First version for USB MSC device demo.
 **
 ******************************************************************************/
#include <am_usb_common_config.h>
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"
#include "usbd_conf.h"
#include "usb_otg_regs.h"

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#ifdef  USB_DEVICE_CLASS_MSC_ON
#define USBD_VID                        0x2E88
#define USBD_PID                        0x4601

#define USBD_LANGID_STRING              0x409
#define USBD_MANUFACTURER_STRING        "HDSC_mcu"


#define USBD_PRODUCT_HS_STRING          "Mass Storage in HS Mode"
#define USBD_SERIALNUMBER_HS_STRING     "00000000001A"
#define USBD_PRODUCT_FS_STRING          "HDSC USB Card Reader"
#define USBD_SERIALNUMBER_FS_STRING     "00000000001B"
#define USBD_CONFIGURATION_HS_STRING    "MSC Config"
#define USBD_INTERFACE_HS_STRING        "MSC Interface"
#define USBD_CONFIGURATION_FS_STRING    "MSC Config"
#define USBD_INTERFACE_FS_STRING        "MSC Interface"
#endif

#ifdef  USB_DEVICE_CLASS_HID_MOUSE
#define USBD_VID                     0x2E88
#define USBD_PID                     0x4602

#define USBD_LANGID_STRING            0x409
#define USBD_MANUFACTURER_STRING      "HDSC_mcu"

#define USBD_PRODUCT_HS_STRING        "Joystick in HS mode"
#define USBD_SERIALNUMBER_HS_STRING   "00000000011B"

#define USBD_PRODUCT_FS_STRING        "Joystick in HS mode in FS Mode"
#define USBD_SERIALNUMBER_FS_STRING   "00000000011C"

#define USBD_CONFIGURATION_HS_STRING  "HID Config"
#define USBD_INTERFACE_HS_STRING      "HID Interface"

#define USBD_CONFIGURATION_FS_STRING  "HID Config"
#define USBD_INTERFACE_FS_STRING      "HID Interface"
#endif

#ifdef  USB_DEVICE_CLASS_HID_CUSTOM
#define USBD_VID                     0x2E88
#define USBD_PID                     0x4606

#define USBD_LANGID_STRING            0x409
#define USBD_MANUFACTURER_STRING      "HDSC_mcu"

#define USBD_PRODUCT_HS_STRING        "HID in HS mode"
#define USBD_SERIALNUMBER_HS_STRING   "00000000011B"

#define USBD_PRODUCT_FS_STRING        "HID in FS mode"
#define USBD_SERIALNUMBER_FS_STRING   "00000000011C"

#define USBD_CONFIGURATION_HS_STRING  "HID Config"
#define USBD_INTERFACE_HS_STRING      "HID Interface"

#define USBD_CONFIGURATION_FS_STRING  "HID Config"
#define USBD_INTERFACE_FS_STRING      "HID Interface"
#endif

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
USBD_DEVICE USR_desc =
{
    &USBD_USR_DeviceDescriptor,
    &USBD_USR_LangIDStrDescriptor,
    &USBD_USR_ManufacturerStrDescriptor,
    &USBD_USR_ProductStrDescriptor,
    &USBD_USR_SerialStrDescriptor,
    &USBD_USR_ConfigStrDescriptor,
    &USBD_USR_InterfaceStrDescriptor,
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined (__ICCARM__)      /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Standard Device Descriptor */
__USB_ALIGN_BEGIN uint8_t USBD_DeviceDesc[USB_SIZ_DEVICE_DESC] __USB_ALIGN_END =
{
    0x12,                       /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType*/
    0x00,                       /* bcdUSB */
    0x02,
    0x00,                       /* bDeviceClass*/
    0x00,                       /* bDeviceSubClass*/
    0x00,                       /* bDeviceProtocol*/
    USB_OTG_MAX_EP0_SIZE,       /* bMaxPacketSize*/
    LOBYTE(USBD_VID),           /* idVendor*/
    HIBYTE(USBD_VID),           /* idVendor*/
    LOBYTE(USBD_PID),           /* idVendor*/
    HIBYTE(USBD_PID),           /* idVendor*/
    0x00,                       /* bcdDevice rel. 2.00*/
    0x02,
    USBD_IDX_MFC_STR,           /* Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,       /* Index of product string*/
    USBD_IDX_SERIAL_STR,        /* Index of serial number string*/
    USBD_CFG_MAX_NUM            /* bNumConfigurations*/
};                              /* USB_DeviceDescriptor */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined (__ICCARM__)      /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Standard Device Descriptor */
__USB_ALIGN_BEGIN uint8_t USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __USB_ALIGN_END =
{
    USB_LEN_DEV_QUALIFIER_DESC,
    USB_DESC_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined (__ICCARM__)      /*!< IAR Compiler */
        #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Standard Device Descriptor */
__USB_ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_SIZ_STRING_LANGID] __USB_ALIGN_END =
{
    USB_SIZ_STRING_LANGID,
    USB_DESC_TYPE_STRING,
    LOBYTE(USBD_LANGID_STRING),
    HIBYTE(USBD_LANGID_STRING),
};
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief  return the device descriptor
 ** \param  speed : current device speed
 ** \param  length : pointer to data length variable
 ** \retval  pointer to descriptor buffer
 ******************************************************************************/
uint8_t *  USBD_USR_DeviceDescriptor(uint8_t speed, uint16_t *length)
{
    *length = (uint16_t)sizeof(USBD_DeviceDesc);
    return USBD_DeviceDesc;
}

/**
 *******************************************************************************
 ** \brief  return the LangID string descriptor
 ** \param  speed : current device speed
 ** \param  length : pointer to data length variable
 ** \retval  pointer to descriptor buffer
 ******************************************************************************/
uint8_t *  USBD_USR_LangIDStrDescriptor(uint8_t speed, uint16_t *length)
{
    *length = (uint16_t)sizeof(USBD_LangIDDesc);
    return USBD_LangIDDesc;
}

/**
 *******************************************************************************
 ** \brief  return the product string descriptor
 ** \param  speed : current device speed
 ** \param  length : pointer to data length variable
 ** \retval  pointer to descriptor buffer
 ******************************************************************************/
uint8_t *  USBD_USR_ProductStrDescriptor(uint8_t speed, uint16_t *length)
{
    if (speed == 0u)
    {
        USBD_GetString((uint8_t *)(USBD_PRODUCT_HS_STRING), USBD_StrDesc, length);
    }else
    {
        USBD_GetString((uint8_t *)(USBD_PRODUCT_FS_STRING), USBD_StrDesc, length);
    }
    return USBD_StrDesc;
}

/**
 *******************************************************************************
 ** \brief  return the manufacturer string descriptor
 ** \param  speed : current device speed
 ** \param  length : pointer to data length variable
 ** \retval  pointer to descriptor buffer
 ******************************************************************************/
uint8_t *  USBD_USR_ManufacturerStrDescriptor(uint8_t speed, uint16_t *length)
{
    USBD_GetString((uint8_t *)(USBD_MANUFACTURER_STRING), USBD_StrDesc, length);
    return USBD_StrDesc;
}

/**
 *******************************************************************************
 ** \brief  return the serial number string descriptor
 ** \param  speed : current device speed
 ** \param  length : pointer to data length variable
 ** \retval  pointer to descriptor buffer
 ******************************************************************************/
uint8_t *  USBD_USR_SerialStrDescriptor(uint8_t speed, uint16_t *length)
{
    if (speed == USB_OTG_SPEED_HIGH)
    {
        USBD_GetString((uint8_t *)(USBD_SERIALNUMBER_HS_STRING), USBD_StrDesc, length);
    }else
    {
        USBD_GetString((uint8_t *)(USBD_SERIALNUMBER_FS_STRING), USBD_StrDesc, length);
    }
    return USBD_StrDesc;
}

/**
 *******************************************************************************
 ** \brief  return the configuration string descriptor
 ** \param  speed : current device speed
 ** \param  length : pointer to data length variable
 ** \retval  pointer to descriptor buffer
 ******************************************************************************/
uint8_t *  USBD_USR_ConfigStrDescriptor(uint8_t speed, uint16_t *length)
{
    if (speed == USB_OTG_SPEED_HIGH)
    {
        USBD_GetString((uint8_t *)(USBD_CONFIGURATION_HS_STRING), USBD_StrDesc, length);
    }else
    {
        USBD_GetString((uint8_t *)(USBD_CONFIGURATION_FS_STRING), USBD_StrDesc, length);
    }
    return USBD_StrDesc;
}

/**
 *******************************************************************************
 ** \brief  return the interface string descriptor
 ** \param  speed : current device speed
 ** \param  length : pointer to data length variable
 ** \retval  pointer to descriptor buffer
 ******************************************************************************/
uint8_t *  USBD_USR_InterfaceStrDescriptor(uint8_t speed, uint16_t *length)
{
    if (speed == 0u)
    {
        USBD_GetString((uint8_t *)(USBD_INTERFACE_HS_STRING), USBD_StrDesc, length);
    }else
    {
        USBD_GetString((uint8_t *)(USBD_INTERFACE_FS_STRING), USBD_StrDesc, length);
    }
    return USBD_StrDesc;
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
