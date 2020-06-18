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
#ifndef USB_DCD_INT_H__
#define USB_DCD_INT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dcd.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct _USBD_DCD_INT
{
    uint8_t (* DataOutStage) (USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
    uint8_t (* DataInStage)  (USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
    uint8_t (* SetupStage) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* SOF) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* Reset) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* Suspend) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* Resume) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* IsoINIncomplete) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* IsoOUTIncomplete) (USB_OTG_CORE_HANDLE *pdev);

    uint8_t (* DevConnected) (USB_OTG_CORE_HANDLE *pdev);
    uint8_t (* DevDisconnected) (USB_OTG_CORE_HANDLE *pdev);
}USBD_DCD_INT_cb_TypeDef;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* C-STAT */
#if 1
#define CLEAR_IN_EP_INTR(epnum,intr) \
    {    \
        *(uint32_t*)&diepint.b = (intr);\
        USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[(epnum)]->DIEPINT,*(uint32_t*)&diepint.b); \
    }

/* C-STAT */
#define CLEAR_OUT_EP_INTR(epnum,intr) \
    {   \
        *(uint32_t*)&doepint.b = (intr);\
        USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[(epnum)]->DOEPINT,*(uint32_t*)&doepint.b); \
    }
#endif
#if 0
#define CLEAR_IN_EP_INTR(epnum,intr) \
    diepint.d32=0; \
    diepint.b.intr = 1; \
    USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT,diepint.d32);

#define CLEAR_OUT_EP_INTR(epnum,intr) \
    doepint.d32=0; \
    doepint.b.intr = 1; \
    USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[epnum]->DOEPINT,doepint.d32);
#endif
/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern USBD_DCD_INT_cb_TypeDef *USBD_DCD_INT_fops;

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/

#endif // USB_DCD_INT_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
