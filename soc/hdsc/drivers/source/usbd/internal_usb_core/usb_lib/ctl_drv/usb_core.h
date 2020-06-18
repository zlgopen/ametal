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
#ifndef __USB_CORE_H__
#define __USB_CORE_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_conf.h"
#include "usb_otg_regs.h"
#include "usb_defines.h"

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define USB_OTG_EP0_IDLE                        (0u)
#define USB_OTG_EP0_SETUP                       (1u)
#define USB_OTG_EP0_DATA_IN                     (2u)
#define USB_OTG_EP0_DATA_OUT                    (3u)
#define USB_OTG_EP0_STATUS_IN                   (4u)
#define USB_OTG_EP0_STATUS_OUT                  (5u)
#define USB_OTG_EP0_STALL                       (6u)

#define USB_OTG_EP_TX_DIS                       (0x0000u)
#define USB_OTG_EP_TX_STALL                     (0x0010u)
#define USB_OTG_EP_TX_NAK                       (0x0020u)
#define USB_OTG_EP_TX_VALID                     (0x0030u)

#define USB_OTG_EP_RX_DIS                       (0x0000u)
#define USB_OTG_EP_RX_STALL                     (0x1000u)
#define USB_OTG_EP_RX_NAK                       (0x2000u)
#define USB_OTG_EP_RX_VALID                     (0x3000u)

#define   MAX_DATA_LENGTH                       (0x200u)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef enum {
    USB_OTG_OK = 0u,
    USB_OTG_FAIL
}USB_OTG_STS;

typedef enum {
    HC_IDLE = 0u,
    HC_XFRC,
    HC_HALTED,
    HC_NAK,
    HC_NYET,
    HC_STALL,
    HC_XACTERR,
    HC_BBLERR,
    HC_DATATGLERR,
}HC_STATUS;

typedef enum {
    URB_IDLE = 0u,
    URB_DONE,
    URB_NOTREADY,
    URB_ERROR,
    URB_STALL
}URB_STATE;

typedef enum {
    CTRL_START = 0u,
    CTRL_XFRC,
    CTRL_HALTED,
    CTRL_NAK,
    CTRL_STALL,
    CTRL_XACTERR,
    CTRL_BBLERR,
    CTRL_DATATGLERR,
    CTRL_FAIL
}CTRL_STATUS;

typedef struct USB_OTG_hc
{
    uint8_t       dev_addr ;
    uint8_t       ep_num;
    uint8_t       ep_is_in;
    uint8_t       speed;
    uint8_t       do_ping;
    uint8_t       ep_type;
    uint16_t      max_packet;
    uint8_t       data_pid;
    uint8_t       *xfer_buff;
    uint32_t      xfer_len;
    uint32_t      xfer_count;
    uint8_t       toggle_in;
    uint8_t       toggle_out;
    uint32_t       dma_addr;
}USB_OTG_HC , *PUSB_OTG_HC;

typedef struct USB_OTG_ep
{
    uint8_t        num;
    uint8_t        is_in;
    uint8_t        is_stall;
    uint8_t        type;
    uint8_t        data_pid_start;
    uint8_t        even_odd_frame;
    uint16_t       tx_fifo_num;
    uint32_t       maxpacket;
    /* transaction level variables*/
    uint8_t        *xfer_buff;
    uint32_t       dma_addr;
    uint32_t       xfer_len;
    uint32_t       xfer_count;
    /* Transfer level variables*/
    uint32_t       rem_data_len;
    uint32_t       total_data_len;
    uint32_t       ctl_data_len;

}USB_OTG_EP , *PUSB_OTG_EP;

typedef struct USB_OTG_core_cfg
{
    uint8_t       host_channels;
    uint8_t       dev_endpoints;
    uint8_t       speed;
    uint8_t       dma_enable;
    uint16_t      mps;
    uint16_t      TotalFifoSize;
    uint8_t       phy_itface;
    uint8_t       Sof_output;
    uint8_t       low_power;
    uint8_t       coreID;
} USB_OTG_CORE_CFGS, *PUSB_OTG_CORE_CFGS;


typedef  struct  usb_setup_req
{
    uint8_t   bmRequest;
    uint8_t   bRequest;
    uint16_t  wValue;
    uint16_t  wIndex;
    uint16_t  wLength;
} USB_SETUP_REQ;

typedef struct _Device_TypeDef
{
    uint8_t  *(*GetDeviceDescriptor)( uint8_t speed , uint16_t *length);
    uint8_t  *(*GetLangIDStrDescriptor)( uint8_t speed , uint16_t *length);
    uint8_t  *(*GetManufacturerStrDescriptor)( uint8_t speed , uint16_t *length);
    uint8_t  *(*GetProductStrDescriptor)( uint8_t speed , uint16_t *length);
    uint8_t  *(*GetSerialStrDescriptor)( uint8_t speed , uint16_t *length);
    uint8_t  *(*GetConfigurationStrDescriptor)( uint8_t speed , uint16_t *length);
    uint8_t  *(*GetInterfaceStrDescriptor)( uint8_t speed , uint16_t *length);
} USBD_DEVICE, *pUSBD_DEVICE;

typedef struct _Device_cb
{
    uint8_t  (*Init)         (void *pdev , uint8_t cfgidx);
    uint8_t  (*DeInit)       (void *pdev , uint8_t cfgidx);
    /* Control Endpoints*/
    uint8_t  (*Setup)        (void *pdev , USB_SETUP_REQ  *req);
    uint8_t  (*EP0_TxSent)   (void *pdev );
    uint8_t  (*EP0_RxReady)  (void *pdev );
    /* Class Specific Endpoints*/
    uint8_t  (*DataIn)       (void *pdev , uint8_t epnum);
    uint8_t  (*DataOut)      (void *pdev , uint8_t epnum);
    uint8_t  (*SOF)          (void *pdev);
    uint8_t  (*IsoINIncomplete)  (void *pdev);
    uint8_t  (*IsoOUTIncomplete)  (void *pdev);

    uint8_t  *(*GetConfigDescriptor)( uint8_t speed , uint16_t *length);
#ifdef USB_OTG_HS_CORE
    uint8_t  *(*GetOtherConfigDescriptor)( uint8_t speed , uint16_t *length);
#endif

#ifdef USB_SUPPORT_USER_STRING_DESC
    uint8_t  *(*GetUsrStrDescriptor)( uint8_t speed ,uint8_t index,  uint16_t *length);
#endif

} USBD_Class_cb_TypeDef;

typedef struct _USBD_USR_PROP
{
    void (*Init)(void);
    void (*DeviceReset)(uint8_t speed);
    void (*DeviceConfigured)(void);
    void (*DeviceSuspended)(void);
    void (*DeviceResumed)(void);

    void (*DeviceConnected)(void);
    void (*DeviceDisconnected)(void);
}USBD_Usr_cb_TypeDef;

typedef struct _DCD
{
    uint8_t        device_config;
    uint8_t        device_state;
    uint8_t        device_status;
    uint8_t        device_old_status;
    uint8_t        device_address;
    uint8_t        connection_status;
    uint8_t        test_mode;
    uint32_t       DevRemoteWakeup;
    USB_OTG_EP     in_ep   [USB_OTG_MAX_TX_FIFOS];
    USB_OTG_EP     out_ep  [USB_OTG_MAX_TX_FIFOS];
    uint8_t        setup_packet [8*3];
    USBD_Class_cb_TypeDef         *class_cb;
    USBD_Usr_cb_TypeDef           *usr_cb;
    USBD_DEVICE                   *usr_device;
    uint8_t        *pConfig_descriptor;
    uint8_t        ep0_state;
 }DCD_DEV , *DCD_PDEV;

typedef struct _HCD
{
    uint8_t                  Rx_Buffer [MAX_DATA_LENGTH];
    __IO uint32_t            ConnSts;
    __IO uint32_t            ErrCnt[USB_OTG_MAX_TX_FIFOS];
    __IO uint32_t            XferCnt[USB_OTG_MAX_TX_FIFOS];
    __IO HC_STATUS           HC_Status[USB_OTG_MAX_TX_FIFOS];
    __IO URB_STATE           URB_State[USB_OTG_MAX_TX_FIFOS];
    USB_OTG_HC               hc [USB_OTG_MAX_TX_FIFOS];
    uint16_t                 channel [USB_OTG_MAX_TX_FIFOS];
    //  USB_OTG_hPort_TypeDef    *port_cb;
}HCD_DEV , *USB_OTG_USBH_PDEV;

typedef struct _OTG
{
    uint8_t    OTG_State;
    uint8_t    OTG_PrevState;
    uint8_t    OTG_Mode;
}OTG_DEV , *USB_OTG_USBO_PDEV;

typedef struct USB_OTG_handle
{
    USB_OTG_CORE_CFGS    cfg;
    USB_OTG_CORE_REGS    regs;
#ifdef USE_DEVICE_MODE
    DCD_DEV     dev;
#endif
#ifdef USE_HOST_MODE
    HCD_DEV     host;
#endif
#ifdef USE_OTG_MODE
    OTG_DEV     otg;
#endif
  void        *pClassData;
  void        *pUserData;
}USB_OTG_CORE_HANDLE , *PUSB_OTG_CORE_HANDLE;

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
USB_OTG_STS  USB_OTG_CoreInit        (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_SelectCore      (USB_OTG_CORE_HANDLE *pdev,
                                      USB_OTG_CORE_ID_TypeDef coreID);
USB_OTG_STS  USB_OTG_EnableGlobalInt (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_DisableGlobalInt(USB_OTG_CORE_HANDLE *pdev);
void*           USB_OTG_ReadPacket   (USB_OTG_CORE_HANDLE *pdev ,
    uint8_t *dest,
    uint16_t len);
USB_OTG_STS  USB_OTG_WritePacket     (USB_OTG_CORE_HANDLE *pdev ,
    uint8_t *src,
    uint8_t ch_ep_num,
    uint16_t len);
USB_OTG_STS  USB_OTG_FlushTxFifo     (USB_OTG_CORE_HANDLE *pdev , uint32_t num);
USB_OTG_STS  USB_OTG_FlushRxFifo     (USB_OTG_CORE_HANDLE *pdev);

uint32_t     USB_OTG_ReadCoreItr     (USB_OTG_CORE_HANDLE *pdev);
uint32_t     USB_OTG_ReadOtgItr      (USB_OTG_CORE_HANDLE *pdev);
uint8_t      USB_OTG_IsHostMode      (USB_OTG_CORE_HANDLE *pdev);
uint8_t      USB_OTG_IsDeviceMode    (USB_OTG_CORE_HANDLE *pdev);
uint32_t     USB_OTG_GetMode         (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_PhyInit         (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_SetCurrentMode  (USB_OTG_CORE_HANDLE *pdev, uint8_t mode);

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/*********************** HOST APIs ********************************************/
#ifdef USE_HOST_MODE
USB_OTG_STS  USB_OTG_CoreInitHost    (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EnableHostInt   (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_HC_Init         (USB_OTG_CORE_HANDLE *pdev, uint8_t hc_num);
USB_OTG_STS  USB_OTG_HC_Halt         (USB_OTG_CORE_HANDLE *pdev, uint8_t hc_num);
USB_OTG_STS  USB_OTG_HC_StartXfer    (USB_OTG_CORE_HANDLE *pdev, uint8_t hc_num);
USB_OTG_STS  USB_OTG_HC_DoPing       (USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num);
uint32_t     USB_OTG_ReadHostAllChannels_intr    (USB_OTG_CORE_HANDLE *pdev);
uint32_t     USB_OTG_ResetPort       (USB_OTG_CORE_HANDLE *pdev);
uint32_t USB_OTG_testmode(USB_OTG_CORE_HANDLE *pdev, int mode);
uint32_t     USB_OTG_ReadHPRT0       (USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_DriveVbus       (USB_OTG_CORE_HANDLE *pdev, uint8_t state);
void         USB_OTG_InitFSLSPClkSel (USB_OTG_CORE_HANDLE *pdev ,uint8_t freq);
uint8_t      USB_OTG_IsEvenFrame     (USB_OTG_CORE_HANDLE *pdev) ;
void         USB_OTG_StopHost        (USB_OTG_CORE_HANDLE *pdev);
#endif
/********************* DEVICE APIs ********************************************/
#ifdef USE_DEVICE_MODE
USB_OTG_STS  USB_OTG_CoreInitDev         (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EnableDevInt        (USB_OTG_CORE_HANDLE *pdev);
uint32_t     USB_OTG_ReadDevAllInEPItr           (USB_OTG_CORE_HANDLE *pdev);
enum USB_OTG_SPEED USB_OTG_GetDeviceSpeed (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EP0Activate (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EPActivate  (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPDeactivate(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPStartXfer (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EP0StartXfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPSetStall          (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPClearStall        (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
uint32_t     USB_OTG_ReadDevAllOutEp_itr (USB_OTG_CORE_HANDLE *pdev);
uint32_t     USB_OTG_ReadDevOutEP_itr    (USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
void         USB_OTG_InitDevSpeed        (USB_OTG_CORE_HANDLE *pdev , uint8_t speed);
uint8_t      USBH_IsEvenFrame (USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_EP0_OutStart(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_ActiveRemoteWakeup(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_UngateClock(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_StopDevice(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_SetEPStatus (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep , uint32_t Status);
uint32_t     USB_OTG_GetEPStatus(USB_OTG_CORE_HANDLE *pdev ,USB_OTG_EP *ep);
#endif

#endif  /* __USB_CORE_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


