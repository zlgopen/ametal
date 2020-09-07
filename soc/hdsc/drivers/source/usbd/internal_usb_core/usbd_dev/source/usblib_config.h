/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
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
/** \file usblib_config.h
 **
 ** A detailed description is available at
 ** @link UsbLibConfigGroup USB LIB Config description @endlink
 **
 **   - 2018-12-25  1.0  Wangmin First version for Device Driver Library config.
 **
 ******************************************************************************/
#ifndef __USBLIB_CONFIG_H__
#define __USBLIB_CONFIG_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup UsbLibConfigGroup Device Driver Library config(USBLIBCONFIG)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/


/**
 *******************************************************************************
 ** \brief This is the list of modules to be used in the usb lib
 ** Select the modules you need
 **
 ** \note
 ******************************************************************************/
/*! File folder on-off define */
#define USBLIB_DEVICE_CORE                          (DDL_ON)
#define USBLIB_HOST_CORE                            (DDL_OFF)
#define USBLIB_DEVICE_CLASS                         (DDL_ON)
#define USBLIB_HOST_CLASS                           (DDL_OFF)

/*! ctl_drv file on-off define */
#define CTL_DRV_USB_CORE                            (DDL_ON)
#define CTL_DRV_USB_OTG                             (DDL_ON)
#define CTL_DRV_USB_DCD                             (DDL_ON)
#define CTL_DRV_USB_DCD_INT                         (DDL_ON)
#define CTL_DRV_USB_HCD                             (DDL_OFF)
#define CTL_DRV_USB_HCD_INT                         (DDL_OFF)

/*! device class on-off define */
#define DEVICE_CLASS_AUDIO                          (DDL_OFF)
#define DEVICE_CLASS_HID_MOUSE                      (DDL_ON)
#define DEVICE_CLASS_MSC                            (DDL_ON)
#define DEVICE_CLASS_CDC_VCP                        (DDL_OFF)
#define DEVICE_CLASS_HID_CUSTOM                     (DDL_ON)
#define DEVICE_CLASS_HID_CDC_WRAPPER                (DDL_OFF)

/*! host class on-off define */
#define HOST_CLASS_HID                              (DDL_OFF)
#define HOST_CLASS_MSC                              (DDL_OFF)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/

//@} // UsbLibConfigGroup

#ifdef __cplusplus
}
#endif

#endif /* __USBLIB_CONFIG_H__*/

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
