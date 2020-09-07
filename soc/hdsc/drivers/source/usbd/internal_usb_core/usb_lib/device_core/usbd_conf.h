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
/** \file usbd_conf.h
 **
 ** A detailed description is available at
 ** @link USB Device configuration file @endlink
 **
 **   - 2018-12-26  1.0  wangmin First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_conf.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*---------- -----------*/
#define USBD_MAX_NUM_INTERFACES         (1u)
/*---------- -----------*/
#define USBD_MAX_NUM_CONFIGURATION      (1u)
/*---------- -----------*/
#define USBD_MAX_STR_DESC_SIZ           (512u)
/*---------- -----------*/
#define USBD_SUPPORT_USER_STRING        (0u)
/*---------- -----------*/
#define USBD_DEBUG_LEVEL                (0u)
/*---------- -----------*/
#define USBD_LPM_ENABLED                (0u)
/*---------- -----------*/
#define USBD_AUDIO_FREQ                 (48000ul)

#define USBD_CFG_MAX_NUM                (1u) /* configuration numbers */
//#define USBD_ITF_MAX_NUM                1 /* interface numbers */

#define USB_MAX_STR_DESC_SIZ            (64u)

#define USBD_SELF_POWERED

#define USBD_DYNAMIC_DESCRIPTOR_CHANGE_ENABLED

/**
 *******************************************************************************
 ** \brief USB_ZERO_Class_Layer_Parameter
 **
 ******************************************************************************/
#define ZERO_IN_EP                          (0x81u)
#define ZERO_OUT_EP                         (0x02u)
#ifdef USE_USB_OTG_FS
    #define ZERO_IN_PACKET                  (64u)
    #define ZERO_OUT_PACKET                 (64u)
#else
    #define ZERO_IN_PACKET                  (512u)
    #define ZERO_OUT_PACKET                 (512u)
#endif

#define HID_IN_PACKET                       (4u)
#define HID_OUT_PACKET                      (4u)

#ifdef USE_USB_OTG_HS
    #define CDC_DATA_MAX_PACKET_SIZE    (512u)     /* Endpoint IN & OUT Packet size */
    #define CDC_CMD_PACKET_SZE          (8u)       /* Control Endpoint Packet size */

    #define CDC_IN_FRAME_INTERVAL       (40u)      /* Number of micro-frames between IN transfers */
    #define APP_RX_DATA_SIZE            (2048u)    /* Total size of IN buffer:
                                                  APP_RX_DATA_SIZE*8/MAX_BAUDARATE*1000 should be > CDC_IN_FRAME_INTERVAL*8 */
#else
    #define CDC_DATA_MAX_PACKET_SIZE    (64u)      /* Endpoint IN & OUT Packet size */
    #define CDC_CMD_PACKET_SZE          (8u)       /* Control Endpoint Packet size */

    #define CDC_IN_FRAME_INTERVAL          (5u)    /* Number of frames between IN transfers */
    #define APP_RX_DATA_SIZE               (2048u) /* Total size of IN buffer:
                                                APP_RX_DATA_SIZE*8/MAX_BAUDARATE*1000 should be > CDC_IN_FRAME_INTERVAL */
#endif /* USE_USB_OTG_HS */

#define APP_FOPS                          (VCP_fops)

#define USB_DESC_TYPE_BOS                 (0x0Fu)
#define USB_DEVICE_CAPABITY_TYPE          (0x10u)

#define USBD_ITF_MAX_NUM                  (1u)
#define USB_MAX_STR_DESC_SIZ              (64u)

/* Class Layer Parameter */

#ifdef USE_USB_OTG_HS
    #ifdef USE_ULPI_PHY
        #define MSC_MAX_PACKET            (512u)
    #else
        #define MSC_MAX_PACKET            (64u)
    #endif
#else  /*USE_USB_OTG_FS*/
    #define MSC_MAX_PACKET                (64u)
#endif

#define MSC_MEDIA_PACKET                  (12ul * 1024ul)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/

#endif //__USBD_CONF__H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
