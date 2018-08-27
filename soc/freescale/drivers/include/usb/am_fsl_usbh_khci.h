/*
 * Copyright (c) 2015 -2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __USB_HOST_KHCI_H__
#define __USB_HOST_KHCI_H__

#include "hw/amhw_fsl_usb.h"
#include "am_usb_hci.h"

/*******************************************************************************
 * KHCI driver private structures, enumerations, macros, functions
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define USB_KHCI_MAX_SPEED_DETECTION_COUNT 3

/* usb khci atom transaction error results */
#define USB_KHCI_ATOM_TR_PID_ERROR (-1)
#define USB_KHCI_ATOM_TR_EOF_ERROR (-2)
#define USB_KHCI_ATOM_TR_CRC_ERROR (-4)
#define USB_KHCI_ATOM_TR_TO (-16)
#define USB_KHCI_ATOM_TR_DMA_ERROR (-32)
#define USB_KHCI_ATOM_TR_BTS_ERROR (-128)
#define USB_KHCI_ATOM_TR_NAK (-256)
#define USB_KHCI_ATOM_TR_DATA_ERROR (-512)
#define USB_KHCI_ATOM_TR_STALL (-1024)
#define USB_KHCI_ATOM_TR_RESET (-2048)
#define USB_KHCI_ATOM_TR_BUS_TIMEOUT (-4096)
#define USB_KHCI_ATOM_TR_INVALID (-8192)

/* KHCI event bits */
#define USB_KHCI_EVENT_ATTACH 0x01
#define USB_KHCI_EVENT_RESET 0x02
#define USB_KHCI_EVENT_TOK_DONE 0x04
#define USB_KHCI_EVENT_SOF_TOK 0x08
#define USB_KHCI_EVENT_DETACH 0x10
#define USB_KHCI_EVENT_MSG 0x20
#define USB_KHCI_EVENT_ISO_MSG 0x40
#define USB_KHCI_EVENT_NAK_MSG 0x80
#define USB_KHCI_EVENT_MASK 0xff

typedef enum _transfer_status
{
    AM_FSL_TRANSFER_IDLE = 0,
    AM_FSL_TRANSFER_SETUP0,
    AM_FSL_TRANSFER_SETUP1,
    AM_FSL_TRANSFER_SETUP2,
    AM_FSL_TRANSFER_SETUP3,
} am_fsl_usbh_transfer_status_t;

typedef enum _khci_intr_type
{
    AM_FSL_INTR_USBRST = 0x01,
    AM_FSL_INTR_ERROR = 0x02,
    AM_FSL_INTR_SOFTOK = 0x04,
    AM_FSL_INTR_TOKDNE = 0x08,
    AM_FSL_INTR_SLEEP = 0x10,
    AM_FSL_INTR_RESUME = 0x20,
    AM_FSL_INTR_ATTACH = 0x40,
    AM_FSL_INTR_STALL = 0x80,
} am_fsl_khci_intr_type_t;

typedef enum _tr_type
{
    AM_FSL_TR_CTRL = 0,
    AM_FSL_TR_IN,
    AM_FSL_TR_OUT,
    AM_FSL_TR_OUTHNDSK,
    AM_FSL_TR_INHNDSK,
    AM_FSL_TR_UNKNOWN
} am_fsl_tr_type_t;

/* Transaction type */
typedef enum _tr_request_type
{
    AM_FSL_TR_MSG_UNKNOWN = 0, /*Unknown - not used */
    AM_FSL_TR_MSG_SETUP,       /* Set up transaction */
    AM_FSL_TR_MSG_SEND,        /* Send transaction */
    AM_FSL_TR_MSG_RECV         /* Receive transaction */
} am_fsl_tr_request_type_t;

typedef enum tr_request_state
{
    AM_FSL_TR_MSG_IDLE = 0, /*Normal transfer */
    AM_FSL_TR_MSG_NAK,      /* NAK transfer */
    AM_FSL_TR_BUS_TIMEOUT,
} am_fsl_tr_request_state_t;

typedef enum khci_tr_state
{
    AM_FSL_KHCI_TR_GET_MSG = 0,
    AM_FSL_KHCI_ISO_TR_GET_MSG,
    AM_FSL_KHCI_TR_START_TRANSMIT,
    AM_FSL_KHCI_TR_TRANSMITING,
    AM_FSL_KHCI_Tr_TRANSMIT_DONE,
    AM_FSL_KHCI_TR_NONE
} am_fsl_khci_tr_state_t;

/* Defines the USB KHCI time out value from USB specification */
#define USB_TIMEOUT_NODATA (500)
#define USB_TIMEOUT_TOHOST (5000)
#define USB_TIMEOUT_TODEVICE (5000)
#define USB_TIMEOUT_OTHER (10000)
#define USB_TIMEOUT_DEFAULT (500)

#define NAK_RETRY_TIME (1)
#define RETRY_TIME (3)

/* Define USB buffer descriptor operator MACRO definitions; This part is not included in header files  */
#define USB_KHCI_BDT_BASE ((uint32_t *)(&bdt[0]))
#define USB_KHCI_BD_PTR(ep, rxtx, odd) \
    ((((uint32_t)USB_KHCI_BDT_BASE) & 0xfffffe00U) | ((ep & 0x0fu) << 5) | ((rxtx & 1u) << 4) | ((odd & 1u) << 3))

#define USB_KHCI_BD_CTRL(ep, rxtx, odd) (*((uint32_t *)USB_KHCI_BD_PTR(ep, rxtx, odd)))
#define USB_KHCI_BD_CTRL_RX(ep, odd) (*((uint32_t *)USB_KHCI_BD_PTR(ep, 0, odd)))
#define USB_KHCI_BD_CTRL_TX(ep, odd) (*((uint32_t *)USB_KHCI_BD_PTR(ep, 1, odd)))

#define USB_KHCI_BD_ADDR(ep, rxtx, odd) (*((uint32_t *)USB_KHCI_BD_PTR(ep, rxtx, odd) + 1))
#define USB_KHCI_BD_ADDR_RX(ep, odd) (*((uint32_t *)USB_KHCI_BD_PTR(ep, 0, odd) + 1))
#define USB_KHCI_BD_ADDR_TX(ep, odd) (*((uint32_t *)USB_KHCI_BD_PTR(ep, 1, odd) + 1))

/* Define USB buffer descriptor definitions; This part is not included in header files  */
#define USB_KHCI_BD_BC(n) ((n & 0x3ffu) << 16)
#define USB_KHCI_BD_OWN 0x80u
#define USB_KHCI_BD_DATA01(n) ((n & 1u) << 6)
#define USB_KHCI_BD_DATA0 USB_KHCI_BD_DATA01(0u)
#define USB_KHCI_BD_DATA1 USB_KHCI_BD_DATA01(1u)
#define USB_KHCI_BD_KEEP 0x20u
#define USB_KHCI_BD_NINC 0x10u
#define USB_KHCI_BD_DTS 0x08u
#define USB_KHCI_BD_STALL 0x04u
#define USB_KHCI_BD_PID(n) ((n & 0x0fu) << 2)

#define USB_HostKhciLock() am_usb_osa_mutex_lock(usbHostPointer->khciMutex)
#define USB_HostKhciUnlock() am_usb_osa_mutex_unlock(usbHostPointer->khciMutex)

typedef struct _khci_xfer_sts
{
    uint32_t rxLen;
    uint8_t *rxBuf;
    uint8_t *rxBufOrig;
    uint8_t isDmaAlign;
} am_usb_khci_xfer_sts_t;

/*******************************************************************************
 * KHCI driver public structures, enumerations, macros, functions
 ******************************************************************************/

/*!
 * @addtogroup usb_host_controller_khci
 * @{
 */

/*! @brief  The value programmed into the threshold register must reserve enough time to ensure the worst case
   transaction completes. In general, the worst case transaction is an IN token followed by a data packet from the target
   followed by the response from the host. The actual time required is a function of the maximum packet size on the bus. Set the
   KHCICFG_THSLD_DELAY to 0x65 to meet the worst case.*/
   
#define KHCICFG_THSLD_DELAY 0x65

/** \brief KHCI state structure */
typedef struct am_fsl_usbh_khci_info
{
    uint32_t regbase;                      /**< \brief usb regbase */
    int      inum;                         /**< \brief int number */

    void   (*pfn_fsl_plfm_init) (void);     /**< \brief usb hardware init */
} am_fsl_usbh_khci_info_t;

/*! @brief KHCI controller driver instance structure */
typedef struct am_usbh_khci_dev
{
    am_usbh_instance_t  host_instance;
    volatile amhw_fsl_usbotg_t *usbRegBase;              /*!< The base address of the register */
    am_usbh_pipe_t *pipeDescriptorBasePointer; /*!< Pipe descriptor bas pointer*/
    am_usb_osa_event_handle_t khciEventPointer;      /*!< KHCI event*/
    am_usb_osa_mutex_handle_t khciMutex;             /*!< KHCI mutex*/
    am_usbh_transfer_t
        *periodicListPointer; /*!< KHCI periodic list pointer, which link is an interrupt and an ISO transfer request*/
    am_usbh_transfer_t *asyncListPointer; /*!< KHCI async list pointer, which link controls and bulk transfer request*/
    am_usb_khci_xfer_sts_t sXferSts;              /*!< KHCI transfer status structure for the DAM ALIGN workaround */
    uint8_t *khciSwapBufPointer;           /*!< KHCI swap buffer pointer for the DAM ALIGN workaround*/
    volatile uint32_t trState;             /*!< KHCI transfer state*/
    uint8_t asyncListAvtive;               /*!< KHCI async list is active*/
    uint8_t periodicListAvtive;            /*!< KHCI periodic list is active*/
    uint8_t rxBd;                          /*!< RX buffer descriptor toggle bits*/
    uint8_t txBd;                          /*!< TX buffer descriptor toggle bits*/
    uint8_t deviceSpeed;                   /*!< Device speed*/
    int8_t deviceAttached;                 /*!< Device attach/detach state */

    am_fsl_usbh_khci_info_t *p_info;
} am_fsl_usbh_khci_dev_t, *ptr_usb_host_khci_state_struct_t;

#ifdef __cplusplus
extern "C" {
#endif

am_usbh_handle_t am_fsl_usbh_khci_init(am_fsl_usbh_khci_dev_t   *p_dev,
                                        am_fsl_usbh_khci_info_t       *p_info);

void am_fsl_usbh_khci_deinit(am_usbd_handle_t handle);

/*! @}*/
#ifdef __cplusplus
}
#endif
/*! @}*/
#endif /* __USB_HOST_KHCI_H__ */
