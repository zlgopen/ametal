/*****************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
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
/** \file usbd_desc.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_desc.c @endlink
 **
 **   - 2019-05-15  1.0  Zhangxl First version for USB MSC device demo.
 **
 ******************************************************************************/
#ifndef __USB_DESC_H__
#define __USB_DESC_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbd_def.h"

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05
#define USB_SIZ_DEVICE_DESC                     18
#define USB_SIZ_STRING_LANGID                   4

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern uint8_t USBD_DeviceDesc  [USB_SIZ_DEVICE_DESC];
extern uint8_t USBD_StrDesc[USB_MAX_STR_DESC_SIZ];
extern uint8_t USBD_OtherSpeedCfgDesc[USB_LEN_CFG_DESC];
extern uint8_t USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC];
extern uint8_t USBD_LangIDDesc[USB_SIZ_STRING_LANGID];
extern USBD_DEVICE USR_desc;

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
uint8_t *     USBD_USR_DeviceDescriptor(uint8_t speed, uint16_t *length);
uint8_t *     USBD_USR_LangIDStrDescriptor(uint8_t speed, uint16_t *length);
uint8_t *     USBD_USR_ManufacturerStrDescriptor(uint8_t speed, uint16_t *length);
uint8_t *     USBD_USR_ProductStrDescriptor(uint8_t speed, uint16_t *length);
uint8_t *     USBD_USR_SerialStrDescriptor(uint8_t speed, uint16_t *length);
uint8_t *     USBD_USR_ConfigStrDescriptor(uint8_t speed, uint16_t *length);
uint8_t *     USBD_USR_InterfaceStrDescriptor(uint8_t speed, uint16_t *length);

#ifdef USB_SUPPORT_USER_STRING_DESC
    uint8_t *     USBD_USR_USRStringDesc(uint8_t speed, uint8_t idx, uint16_t *length);
#endif /* USB_SUPPORT_USER_STRING_DESC */

#endif /* __USBD_DESC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
