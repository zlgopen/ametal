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
 ** @link Peripheral Device Interface Layer @endlink
 **
 **   - 2018-12-26  1.0  wangmin First version for USB demo.
 **
 ******************************************************************************/
#ifndef __HCD_INT_H__
#define __HCD_INT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_hcd.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct _USBH_HCD_INT
{
    uint8_t (* SOF) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* DevConnected) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* DevDisconnected) (USB_OTG_CORE_HANDLE *pdev);
}USBH_HCD_INT_cb_TypeDef;


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define CLEAR_HC_INT(HC_REGS, intr) \
    {\
    USB_OTG_HCINTn_TypeDef  hcint_clear; \
    *(uint32_t*)&hcint_clear.b = (intr); \
    USB_OTG_WRITE_REG32(&((HC_REGS)->HCINT), hcint_clear.d32);\
    }\

#define MASK_HOST_INT_CHH(hc_num) { USB_OTG_HCINTMSK_TypeDef  INTMSK; \
    INTMSK.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[(hc_num)]->HCINTMSK); \
    INTMSK.b.chhltd = 0u; \
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[(hc_num)]->HCINTMSK, INTMSK.d32);}

#define UNMASK_HOST_INT_CHH(hc_num) { USB_OTG_HCINTMSK_TypeDef  INTMSK; \
    INTMSK.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[(hc_num)]->HCINTMSK); \
    INTMSK.b.chhltd = 1u; \
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[(hc_num)]->HCINTMSK, INTMSK.d32);}

#define MASK_HOST_INT_ACK(hc_num) { USB_OTG_HCINTMSK_TypeDef  INTMSK; \
    INTMSK.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[(hc_num)]->HCINTMSK); \
    INTMSK.b.ack = 0u; \
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[(hc_num)]->HCINTMSK, GINTMSK.d32);}

#define UNMASK_HOST_INT_ACK(hc_num) { USB_OTG_HCGINTMSK_TypeDef  INTMSK; \
    INTMSK.d32 = USB_OTG_READ_REG32(&pdev->regs.HC_REGS[(hc_num)]->HCINTMSK); \
    INTMSK.b.ack = 1u; \
    USB_OTG_WRITE_REG32(&pdev->regs.HC_REGS[(hc_num)]->HCINTMSK, INTMSK.d32);}

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern USBH_HCD_INT_cb_TypeDef *USBH_HCD_INT_fops;

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
void ConnectCallback_Handler(USB_OTG_CORE_HANDLE *pdev);
void Disconnect_Callback_Handler(USB_OTG_CORE_HANDLE *pdev);
void Overcurrent_Callback_Handler(USB_OTG_CORE_HANDLE *pdev);
uint32_t USBH_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);

#endif //__HCD_INT_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


