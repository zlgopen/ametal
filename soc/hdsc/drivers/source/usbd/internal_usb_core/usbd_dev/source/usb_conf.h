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
/** \file usb_conf.h
 **
 ** A detailed description is available at
 ** @link General low level driver configuration @endlink
 **
 **   - 2019-05-15  1.0  Zhangxl First version for USB MSC device demo.
 **
 ******************************************************************************/
#ifndef __USB_CONF_H__
#define __USB_CONF_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/

/* USB Core and PHY interface configuration.
   Tip: To avoid modifying these defines each time you need to change the USB
        configuration, you can declare the needed define in your toolchain
        compiler preprocessor.
   */
/****************** USB OTG FS PHY CONFIGURATION *******************************
*  The USB OTG FS Core supports one on-chip Full Speed PHY.
*
*  The USE_EMBEDDED_PHY symbol is defined in the project compiler preprocessor
*  when FS core is used.
*******************************************************************************/
#ifndef USE_USB_OTG_FS
 #define USE_USB_OTG_FS
#endif /* USE_USB_OTG_FS */

#ifdef USE_USB_OTG_FS
 #define USB_OTG_FS_CORE
#endif

/****************** USB OTG HS PHY CONFIGURATION *******************************
*  The USB OTG HS Core supports two PHY interfaces:
*   (i)  An ULPI interface for the external High Speed PHY: the USB HS Core will
*        operate in High speed mode
*   (ii) An on-chip Full Speed PHY: the USB HS Core will operate in Full speed mode
*
*  You can select the PHY to be used using one of these two defines:
*   (i)  USE_ULPI_PHY: if the USB OTG HS Core is to be used in High speed mode
*   (ii) USE_EMBEDDED_PHY: if the USB OTG HS Core is to be used in Full speed mode
*
*  Notes:
*   - The USE_ULPI_PHY symbol is defined in the project compiler preprocessor as
*     default PHY when HS core is used.
*******************************************************************************/
#ifndef USE_USB_OTG_HS
//#define USE_USB_OTG_HS
#endif /* USE_USB_OTG_HS */

#ifndef USE_ULPI_PHY
//#define USE_ULPI_PHY
#endif /* USE_ULPI_PHY */

#ifndef USE_EMBEDDED_PHY
    #define USE_EMBEDDED_PHY
#endif /* USE_EMBEDDED_PHY */

#ifdef USE_USB_OTG_HS
 #define USB_OTG_HS_CORE
#endif

/*******************************************************************************
*                      FIFO Size Configuration in Device mode
*
*  (i) Receive data FIFO size = RAM for setup packets +
*                   OUT endpoint control information +
*                   data OUT packets + miscellaneous
*      Space = ONE 32-bits words
*     --> RAM for setup packets = 10 spaces
*        (n is the nbr of CTRL EPs the device core supports)
*     --> OUT EP CTRL info      = 1 space
*        (one space for status information written to the FIFO along with each
*        received packet)
*     --> data OUT packets      = (Largest Packet Size / 4) + 1 spaces
*        (MINIMUM to receive packets)
*     --> OR data OUT packets  = at least 2*(Largest Packet Size / 4) + 1 spaces
*        (if high-bandwidth EP is enabled or multiple isochronous EPs)
*     --> miscellaneous = 1 space per OUT EP
*        (one space for transfer complete status information also pushed to the
*        FIFO with each endpoint's last packet)
*
*  (ii)MINIMUM RAM space required for each IN EP Tx FIFO = MAX packet size for
*       that particular IN EP. More space allocated in the IN EP Tx FIFO results
*       in a better performance on the USB and can hide latencies on the AHB.
*
*  (iii) TXn min size = 16 words. (n  : Transmit FIFO index)
*   (iv) When a TxFIFO is not used, the Configuration should be as follows:
*       case 1 :  n > m    and Txn is not used    (n,m  : Transmit FIFO indexes)
*       --> Txm can use the space allocated for Txn.
*       case2  :  n < m    and Txn is not used    (n,m  : Transmit FIFO indexes)
*       --> Txn should be configured with the minimum space of 16 words
*  (v) The FIFO is used optimally when used TxFIFOs are allocated in the top
*       of the FIFO.Ex: use EP1 and EP2 as IN instead of EP1 and EP3 as IN ones.
*   (vi) In HS case12 FIFO locations should be reserved for internal DMA registers
*        so total FIFO size should be 1012 Only instead of 1024
*******************************************************************************/

/****************** USB Device Endpoint config ********************************/
#define MSC_IN_EP                         0x81u
//#define MSC_OUT_EP                        0x01u
#define MSC_OUT_EP                        0x02u
#define HID_IN_EP                       0x81u
#define HID_OUT_EP                      0x01u

/****************** USB OTG HS CONFIGURATION **********************************/
#ifdef USB_OTG_HS_CORE
    #define RX_FIFO_HS_SIZE                         512u
    #define TX0_FIFO_HS_SIZE                        128u
    #define TX1_FIFO_HS_SIZE                        372u
    #define TX2_FIFO_HS_SIZE                          0u
    #define TX3_FIFO_HS_SIZE                          0u
    #define TX4_FIFO_HS_SIZE                          0u
    #define TX5_FIFO_HS_SIZE                          0u
    #define TX6_FIFO_HS_SIZE                          0u
    #define TX7_FIFO_HS_SIZE                          0u
//  #define USB_OTG_HS_LOW_PWR_MGMT_SUPPORT
//  #define USB_OTG_HS_SOF_OUTPUT_ENABLED

#ifdef USE_ULPI_PHY
    #define USB_OTG_ULPI_PHY_ENABLED
#endif
#ifdef USE_EMBEDDED_PHY
    #define USB_OTG_EMBEDDED_PHY_ENABLED
#endif
    #define USB_OTG_HS_INTERNAL_DMA_ENABLED
    #define USB_OTG_HS_DEDICATED_EP1_ENABLED
#endif

/****************** USB OTG FS CONFIGURATION **********************************/
#ifdef USB_OTG_FS_CORE
 #define RX_FIFO_FS_SIZE                          128u
 #define TX0_FIFO_FS_SIZE                          64u
 #define TX1_FIFO_FS_SIZE                          64u
 #define TX2_FIFO_FS_SIZE                          64u
 #define TX3_FIFO_FS_SIZE                           0u
 #define TX4_FIFO_FS_SIZE                           0u
 #define TX5_FIFO_FS_SIZE                           0u
#ifdef USE_ULPI_PHY
    #define USB_OTG_ULPI_PHY_ENABLED
#endif
#ifdef USE_EMBEDDED_PHY
    #define USB_OTG_EMBEDDED_PHY_ENABLED
#endif
//  #define USB_OTG_HS_INTERNAL_DMA_ENABLED

//  #define USB_OTG_FS_LOW_PWR_MGMT_SUPPORT
//  #define USB_OTG_FS_SOF_OUTPUT_ENABLED
#endif

/****************** USB OTG MISC CONFIGURATION ********************************/
#define VBUS_SENSING_ENABLED

/****************** USB OTG MODE CONFIGURATION ********************************/
//#define USE_HOST_MODE
#define USE_DEVICE_MODE
//#define USE_OTG_MODE

#ifndef USB_OTG_FS_CORE
    #ifndef USB_OTG_HS_CORE
        #error  "USB_OTG_HS_CORE or USB_OTG_FS_CORE should be defined"
    #endif
#endif

#ifndef USE_DEVICE_MODE
    #ifndef USE_HOST_MODE
        #error  "USE_DEVICE_MODE or USE_HOST_MODE should be defined"
    #endif
#endif

#ifndef USE_USB_OTG_HS
    #ifndef USE_USB_OTG_FS
        #error  "USE_USB_OTG_HS or USE_USB_OTG_FS should be defined"
    #endif
#else //USE_USB_OTG_HS
    #ifndef USE_ULPI_PHY
        #ifndef USE_EMBEDDED_PHY
            #error  "USE_ULPI_PHY or USE_EMBEDDED_PHY should be defined"
        #endif
    #endif
#endif

/****************** C Compilers dependant keywords ****************************/
/* In HS mode and when the DMA is used, all variables and data structures dealing
   with the DMA during the transaction process should be 4-bytes aligned */
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined   (__GNUC__)        /* GNU Compiler */
        #define __USB_ALIGN_END    __attribute__ ((aligned (4)))
        #define __USB_ALIGN_BEGIN
    #else
        #define __ALIGN_END
        #if defined   (__CC_ARM)      /* ARM Compiler */
            #define __USB_ALIGN_BEGIN    __align(4)
        #elif defined (__ICCARM__)    /* IAR Compiler */
            #define __USB_ALIGN_BEGIN
        #elif defined  (__TASKING__)  /* TASKING Compiler */
            #define __USB_ALIGN_BEGIN    __align(4)
        #endif /* __CC_ARM */
    #endif /* __GNUC__ */
#else
    #define __USB_ALIGN_BEGIN
    #define __USB_ALIGN_END
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* __packed keyword used to decrease the data type alignment to 1-byte */
#if defined (__CC_ARM)         /* ARM Compiler */
    #define __packed    __packed
#elif defined (__ICCARM__)     /* IAR Compiler */
//    #define __packed    __packed
#elif defined   (__GNUC__)     /* GNU Compiler */
//    #define __packed    __attribute__ ((__packed__))
#elif defined   (__TASKING__)  /* TASKING Compiler */
    #define __packed    __unaligned
#endif /* __CC_ARM */

#endif //__USB_CONF_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
