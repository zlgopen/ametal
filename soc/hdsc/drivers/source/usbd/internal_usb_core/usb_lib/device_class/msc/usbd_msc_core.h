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
/** \file usbd_msc_core.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_msc_core.c @endlink
 **
 **   - 2019-05-15  1.0  Zhangxl First version for USB MSC device demo.
 **
 ******************************************************************************/
#ifndef __USB_MSC_CORE_H__
#define __USB_MSC_CORE_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include  "usbd_ioreq.h"

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define BOT_GET_MAX_LUN              (0xFEu)
#define BOT_RESET                    (0xFFu)
#define USB_MSC_CONFIG_DESC_SIZ      (32u)

#define MSC_EPIN_SIZE                (MSC_MAX_PACKET)
#define MSC_EPOUT_SIZE               (MSC_MAX_PACKET)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
uint8_t  USBD_MSC_Init(void  *pdev,
                       uint8_t cfgidx);

uint8_t  USBD_MSC_DeInit(void  *pdev,
                         uint8_t cfgidx);

uint8_t  USBD_MSC_Setup(void  *pdev,
                        USB_SETUP_REQ *req);

uint8_t  USBD_MSC_DataIn(void  *pdev,
                         uint8_t epnum);


uint8_t  USBD_MSC_DataOut(void  *pdev,
                          uint8_t epnum);

uint8_t  *USBD_MSC_GetCfgDesc(uint8_t speed,
                              uint16_t *length);

#ifdef USB_OTG_HS_CORE
uint8_t  *USBD_MSC_GetOtherCfgDesc(uint8_t speed,
                                   uint16_t *length);
#endif

extern uint8_t USBD_MSC_CfgDesc[USB_MSC_CONFIG_DESC_SIZ];

extern USBD_Class_cb_TypeDef USBD_MSC_cb;

#endif  /* __USB_MSC_CORE_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
