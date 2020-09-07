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
/** \file usbd_hid_mouse_core.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_hid_core.c @endlink
 **
 **   - 2018-12-26  1.0  wangmin First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HID_CORE_H_
#define __USB_HID_CORE_H_

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include  "usbd_ioreq.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief USBD_HID_Exported_Defines
 **
 ******************************************************************************/
#define USB_HID_CONFIG_DESC_SIZ       (41u)
#define USB_HID_DESC_SIZ              (9u)
#define HID_MOUSE_REPORT_DESC_SIZE    (74u)

#define HID_DESCRIPTOR_TYPE           (0x21u)
#define HID_REPORT_DESC               (0x22u)


#define HID_REQ_SET_PROTOCOL          (0x0Bu)
#define HID_REQ_GET_PROTOCOL          (0x03u)

#define HID_REQ_SET_IDLE              (0x0Au)
#define HID_REQ_GET_IDLE              (0x02u)

#define HID_REQ_SET_REPORT            (0x09u)
#define HID_REQ_GET_REPORT            (0x01u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern USBD_Class_cb_TypeDef  USBD_HID_cb;
/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
uint8_t USBD_HID_SendReport (USB_OTG_CORE_HANDLE  *pdev,
                                 uint8_t *report,
                                 uint16_t len);

#endif  // __USB_HID_CORE_H_

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

