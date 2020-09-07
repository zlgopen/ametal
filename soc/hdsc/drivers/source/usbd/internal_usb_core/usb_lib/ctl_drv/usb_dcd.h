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
/** \file usbd_desc.h
 **
 ** A detailed description is available at
 ** @link Peripheral Driver Header file @endlink
 **
 **   - 2018-12-26  1.0  wangmin First version for USB demo.
 **
 ******************************************************************************/
#ifndef __DCD_H__
#define __DCD_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_core.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bEndpointAddress;
    uint8_t  bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t  bInterval;
}EP_DESCRIPTOR , *PEP_DESCRIPTOR;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define USB_OTG_EP_CONTROL                       (0u)
#define USB_OTG_EP_ISOC                          (1u)
#define USB_OTG_EP_BULK                          (2u)
#define USB_OTG_EP_INT                           (3u)
#define USB_OTG_EP_MASK                          (3u)

/*  Device Status */
#define USB_OTG_DEFAULT                          (1u)
#define USB_OTG_ADDRESSED                        (2u)
#define USB_OTG_CONFIGURED                       (3u)
#define USB_OTG_SUSPENDED                        (4u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
void       DCD_Init(USB_OTG_CORE_HANDLE *pdev, USB_OTG_CORE_ID_TypeDef coreID);
void        DCD_DevConnect (USB_OTG_CORE_HANDLE *pdev);
void        DCD_DevDisconnect (USB_OTG_CORE_HANDLE *pdev);
void        DCD_EP_SetAddress (USB_OTG_CORE_HANDLE *pdev, uint8_t address);
uint32_t    DCD_EP_Open(USB_OTG_CORE_HANDLE *pdev ,
                        uint8_t ep_addr,
                        uint16_t ep_mps,
                        uint8_t ep_type);
uint32_t    DCD_EP_Close  (USB_OTG_CORE_HANDLE *pdev, uint8_t  ep_addr);
uint32_t   DCD_EP_PrepareRx ( USB_OTG_CORE_HANDLE *pdev,
                                uint8_t   ep_addr,
                                uint8_t *pbuf,
                                uint16_t  buf_len);
uint32_t    DCD_EP_Tx (USB_OTG_CORE_HANDLE *pdev,
                               uint8_t  ep_addr,
                               uint8_t  *pbuf,
                               uint32_t   buf_len);
uint32_t    DCD_EP_Stall (USB_OTG_CORE_HANDLE *pdev, uint8_t   epnum);
uint32_t    DCD_EP_ClrStall (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
uint32_t    DCD_EP_Flush (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
uint32_t    DCD_Handle_ISR(USB_OTG_CORE_HANDLE *pdev);
uint32_t DCD_GetEPStatus(USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
void DCD_SetEPStatus (USB_OTG_CORE_HANDLE *pdev ,
                      uint8_t epnum ,
                      uint32_t Status);

#endif //__DCD_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
