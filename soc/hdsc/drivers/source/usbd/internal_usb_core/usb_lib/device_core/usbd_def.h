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
/** \file usbd_def.h
 **
 ** A detailed description is available at
 ** @link general defines for the usb device library @endlink
 **
 **   - 2018-12-26  1.0  wangmin First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USBD_DEF_H
#define __USBD_DEF_H

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbd_conf.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#ifndef NULL
#define NULL    0u
#endif

#define  USB_LEN_DEV_QUALIFIER_DESC                     (0x0Au)
#define  USB_LEN_DEV_DESC                               (0x12u)
#define  USB_LEN_CFG_DESC                               (0x09u)
#define  USB_LEN_IF_DESC                                (0x09u)
#define  USB_LEN_EP_DESC                                (0x07u)
#define  USB_LEN_OTG_DESC                               (0x03u)

#define  USBD_IDX_LANGID_STR                            (0x00u)
#define  USBD_IDX_MFC_STR                               (0x01u)
#define  USBD_IDX_PRODUCT_STR                           (0x02u)
#define  USBD_IDX_SERIAL_STR                            (0x03u)
#define  USBD_IDX_CONFIG_STR                            (0x04u)
#define  USBD_IDX_INTERFACE_STR                         (0x05u)

#define  USB_REQ_TYPE_STANDARD                          (0x00u)
#define  USB_REQ_TYPE_CLASS                             (0x20u)
#define  USB_REQ_TYPE_VENDOR                            (0x40u)
#define  USB_REQ_TYPE_MASK                              (0x60u)

#define  USB_REQ_RECIPIENT_DEVICE                       (0x00u)
#define  USB_REQ_RECIPIENT_INTERFACE                    (0x01u)
#define  USB_REQ_RECIPIENT_ENDPOINT                     (0x02u)
#define  USB_REQ_RECIPIENT_MASK                         (0x03u)

#define  USB_REQ_GET_STATUS                             (0x00u)
#define  USB_REQ_CLEAR_FEATURE                          (0x01u)
#define  USB_REQ_SET_FEATURE                            (0x03u)
#define  USB_REQ_SET_ADDRESS                            (0x05u)
#define  USB_REQ_GET_DESCRIPTOR                         (0x06u)
#define  USB_REQ_SET_DESCRIPTOR                         (0x07u)
#define  USB_REQ_GET_CONFIGURATION                      (0x08u)
#define  USB_REQ_SET_CONFIGURATION                      (0x09u)
#define  USB_REQ_GET_INTERFACE                          (0x0Au)
#define  USB_REQ_SET_INTERFACE                          (0x0Bu)
#define  USB_REQ_SYNCH_FRAME                            (0x0Cu)

#define  USB_DESC_TYPE_DEVICE                              (1u)
#define  USB_DESC_TYPE_CONFIGURATION                       (2u)
#define  USB_DESC_TYPE_STRING                              (3u)
#define  USB_DESC_TYPE_INTERFACE                           (4u)
#define  USB_DESC_TYPE_ENDPOINT                            (5u)
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    (6u)
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           (7u)


#define USB_CONFIG_REMOTE_WAKEUP                           (2u)
#define USB_CONFIG_SELF_POWERED                            (1u)

#define USB_FEATURE_EP_HALT                                (0u)
#define USB_FEATURE_REMOTE_WAKEUP                          (1u)
#define USB_FEATURE_TEST_MODE                              (2u)

#define  SWAPBYTE(addr)        (((uint16_t)(*((uint8_t *)(addr)))) + \
                               (uint16_t)(((uint16_t)(*(((uint8_t *)(addr)) + 1u))) << 8u))

#define LOBYTE(x)  ((uint8_t)((uint16_t)(x) & 0x00FFu))
#define HIBYTE(x)  ((uint8_t)(((uint16_t)(x) & 0xFF00u) >>8u))

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/


#endif /* __USBD_DEF_H */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
