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
 ** @link Header of the Core Layer @endlink
 **
 **   - 2018-12-26  1.0  wangmin First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEF_H__
#define __USB_DEF_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include  "usb_conf.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief USB_DEFINES_Exported_Types
 **
 ******************************************************************************/
typedef enum
{
    USB_OTG_HS_CORE_ID = 0,
    USB_OTG_FS_CORE_ID = 1
}USB_OTG_CORE_ID_TypeDef;

/**
 *******************************************************************************
 ** \brief ENUMERATION TYPE
 **
 ******************************************************************************/
enum USB_OTG_SPEED {
    USB_SPEED_UNKNOWN = 0,
    USB_SPEED_LOW,
    USB_SPEED_FULL,
    USB_SPEED_HIGH
};
/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief _CORE_DEFINES_
 **
 ******************************************************************************/
#define USB_OTG_SPEED_PARAM_HIGH            (0u)
#define USB_OTG_SPEED_PARAM_HIGH_IN_FULL    (1u)
#define USB_OTG_SPEED_PARAM_FULL            (3u)

#define USB_OTG_SPEED_HIGH                  (0u)
#define USB_OTG_SPEED_FULL                  (1u)

#define USB_OTG_ULPI_PHY                    (1u)
#define USB_OTG_EMBEDDED_PHY                (2u)

/**
 *******************************************************************************
 ** \brief _GLOBAL_DEFINES_
 **
 ******************************************************************************/
#define GAHBCFG_TXFEMPTYLVL_EMPTY              (1u)
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          (0u)
#define GAHBCFG_GLBINT_ENABLE                  (1u)
#define GAHBCFG_INT_DMA_BURST_SINGLE           (0u)
#define GAHBCFG_INT_DMA_BURST_INCR             (1u)
#define GAHBCFG_INT_DMA_BURST_INCR4            (3u)
#define GAHBCFG_INT_DMA_BURST_INCR8            (5u)
#define GAHBCFG_INT_DMA_BURST_INCR16           (7u)
#define GAHBCFG_DMAENABLE                      (1u)
#define GAHBCFG_TXFEMPTYLVL_EMPTY              (1u)
#define GAHBCFG_TXFEMPTYLVL_HALFEMPTY          (0u)
#define GRXSTS_PKTSTS_IN                       (2u)
#define GRXSTS_PKTSTS_IN_XFER_COMP             (3u)
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR          (5u)
#define GRXSTS_PKTSTS_CH_HALTED                (7u)

#define SELF_POWERED (1u << 6u)
#define REMOTE_WAKEUP (1u << 5u)

/**
 *******************************************************************************
 ** \brief _OnTheGo_DEFINES_
 **
 ******************************************************************************/
#define MODE_HNP_SRP_CAPABLE                   (0u)
#define MODE_SRP_ONLY_CAPABLE                  (1u)
#define MODE_NO_HNP_SRP_CAPABLE                (2u)
#define MODE_SRP_CAPABLE_DEVICE                (3u)
#define MODE_NO_SRP_CAPABLE_DEVICE             (4u)
#define MODE_SRP_CAPABLE_HOST                  (5u)
#define MODE_NO_SRP_CAPABLE_HOST               (6u)
#define A_HOST                                 (1u)
#define A_SUSPEND                              (2u)
#define A_PERIPHERAL                           (3u)
#define B_PERIPHERAL                           (4u)
#define B_HOST                                 (5u)
#define DEVICE_MODE                            (0u)
#define HOST_MODE                              (1u)
#define OTG_MODE                               (2u)

/**
 *******************************************************************************
 ** \brief __DEVICE_DEFINES_
 **
 ******************************************************************************/
#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ     (0u)
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ     (1u)
#define DSTS_ENUMSPD_LS_PHY_6MHZ               (2u)
#define DSTS_ENUMSPD_FS_PHY_48MHZ              (3u)

#define DCFG_FRAME_INTERVAL_80                 (0u)
#define DCFG_FRAME_INTERVAL_85                 (1u)
#define DCFG_FRAME_INTERVAL_90                 (2u)
#define DCFG_FRAME_INTERVAL_95                 (3u)

#define DEP0CTL_MPS_64                         (0u)
#define DEP0CTL_MPS_32                         (1u)
#define DEP0CTL_MPS_16                         (2u)
#define DEP0CTL_MPS_8                          (3u)

#define EP_SPEED_LOW                           (0u)
#define EP_SPEED_FULL                          (1u)
#define EP_SPEED_HIGH                          (2u)

#define EP_TYPE_CTRL                           (0u)
#define EP_TYPE_ISOC                           (1u)
#define EP_TYPE_BULK                           (2u)
#define EP_TYPE_INTR                           (3u)
#define EP_TYPE_MSK                            (3u)

#define STS_GOUT_NAK                           (1u)
#define STS_DATA_UPDT                          (2u)
#define STS_XFER_COMP                          (3u)
#define STS_SETUP_COMP                         (4u)
#define STS_SETUP_UPDT                         (6u)

/**
 *******************************************************************************
 ** \brief __HOST_DEFINES_
 **
 ******************************************************************************/
#define HC_PID_DATA0                           (0u)
#define HC_PID_DATA2                           (1u)
#define HC_PID_DATA1                           (2u)
#define HC_PID_SETUP                           (3u)

#define HPRT0_PRTSPD_HIGH_SPEED                (0u)
#define HPRT0_PRTSPD_FULL_SPEED                (1u)
#define HPRT0_PRTSPD_LOW_SPEED                 (2u)

#define HCFG_30_60_MHZ                         (0u)
#define HCFG_48_MHZ                            (1u)
#define HCFG_6_MHZ                             (2u)

#define HCCHAR_CTRL                            (0u)
#define HCCHAR_ISOC                            (1u)
#define HCCHAR_BULK                            (2u)
#define HCCHAR_INTR                            (3u)

#ifndef __MIN
#define  __MIN(a, b)      (((a) < (b)) ? (a) : (b))
#endif

/**
 *******************************************************************************
 ** \brief Internal_Macro
 **
 ******************************************************************************/
#define USB_OTG_READ_REG32(reg)  (*(__IO uint32_t *)(reg))
#define USB_OTG_WRITE_REG32(reg,value) (*(__IO uint32_t *)(reg) = (value))
#define USB_OTG_MODIFY_REG32(reg,clear_mask,set_mask) (USB_OTG_WRITE_REG32((reg), (((USB_OTG_READ_REG32((reg))) & ~(clear_mask)) | (set_mask))))
/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/

#endif //__USB_DEFINES__H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


