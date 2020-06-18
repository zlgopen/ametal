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
 ** @link Host layer Header file @endlink
 **
 **   - 2018-12-26  1.0  wangmin First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HCD_H__
#define __USB_HCD_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_otg_regs.h"
#include "usb_core.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
uint32_t  HCD_Init                 (USB_OTG_CORE_HANDLE *pdev ,
                                    USB_OTG_CORE_ID_TypeDef coreID);
uint32_t  HCD_HC_Init              (USB_OTG_CORE_HANDLE *pdev ,
                                    uint8_t hc_num);
uint32_t  HCD_SubmitRequest        (USB_OTG_CORE_HANDLE *pdev ,
                                    uint8_t hc_num) ;
uint32_t  HCD_GetCurrentSpeed      (USB_OTG_CORE_HANDLE *pdev);
uint32_t  HCD_ResetPort            (USB_OTG_CORE_HANDLE *pdev);
uint32_t  HCD_IsDeviceConnected    (USB_OTG_CORE_HANDLE *pdev);
uint32_t  HCD_GetCurrentFrame      (USB_OTG_CORE_HANDLE *pdev) ;
URB_STATE HCD_GetURB_State         (USB_OTG_CORE_HANDLE *pdev,  uint8_t ch_num);
uint32_t  HCD_GetXferCnt           (USB_OTG_CORE_HANDLE *pdev,  uint8_t ch_num);
HC_STATUS HCD_GetHCState           (USB_OTG_CORE_HANDLE *pdev,  uint8_t ch_num) ;

#endif //__USB_HCD_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


