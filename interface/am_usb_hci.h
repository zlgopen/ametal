/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#ifndef _USB_HOST_HCI_H_
#define _USB_HOST_HCI_H_

#include "am_usbh.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief USB host lock */
#define USB_HostLock() am_usb_osa_mutex_lock(hostInstance->hostMutex)
/*! @brief USB host unlock */
#define USB_HostUnlock() am_usb_osa_mutex_unlock(hostInstance->hostMutex)

/**
 * \addtogroup am_if_usb_hci
 * \copydoc am_usb_hci.h
 * @{
 */

/*! @brief USB host controller control code */
typedef enum am_usbh_controller_control
{
    AM_USBH_CANCEL_TRANSFER = 1U,          /*!< Cancel transfer code */
    AM_USBH_BUS_CONTROL,                   /*!< Bus control code */
    AM_USBH_GET_FRAME_NUMBER,               /*!< Get frame number code */
    AM_USBH_UPDATE_CONTROL_ENDPOINT_ADDRESS, /*!< Update control endpoint address */
    AM_USBH_UPDATE_CONTROL_PACKET_SIZE,      /*!< Update control endpoint maximum  packet size */
} am_usbh_controller_control_t;

/*! @brief USB host controller bus control code */
typedef enum am_usbh_bus_control
{
    AM_USBH_BUS_RESET = 1U,    /*!< Reset bus */
    AM_USBH_BUS_RESTART,       /*!< Restart bus */
    AM_USBH_BUS_ENABLE_ATTACH,  /*!< Enable attach */
    AM_USBH_BUS_DISABLE_ATTACH, /*!< Disable attach*/
} am_usbh_bus_control_t;

/*! @brief USB host controller interface structure */
typedef struct am_usbh_controller_interface
{
    am_usb_status_t (*controllerCreate)(
        am_usbh_controller_handle_t controllerHandle);
    am_usb_status_t (*controllerDestory)(
        am_usbh_controller_handle_t controllerHandle); /*!< Destroy a controller instance function prototype*/
    am_usb_status_t (*controllerOpenPipe)(am_usbh_controller_handle_t controllerHandle,
                                       am_usbh_pipe_handle_t *pipe_handle,
                                       am_usbh_pipe_init_t *pipeInit); /*!< Open a controller pipe function prototype*/
    am_usb_status_t (*controllerClosePipe)(
        am_usbh_controller_handle_t controllerHandle,
        am_usbh_pipe_handle_t pipe_handle); /*!< Close a controller pipe function prototype*/
    am_usb_status_t (*controllerWritePipe)(am_usbh_controller_handle_t controllerHandle,
                                        am_usbh_pipe_handle_t pipe_handle,
                                        am_usbh_transfer_t *transfer); /*!< Write data to a pipe function prototype*/
    am_usb_status_t (*controllerReadPipe)(am_usbh_controller_handle_t controllerHandle,
                                       am_usbh_pipe_handle_t pipe_handle,
                                       am_usbh_transfer_t *transfer); /*!< Read data from a pipe function prototype*/
    am_usb_status_t (*controllerIoctl)(am_usbh_controller_handle_t controllerHandle,
                                    uint32_t ioctlEvent,
                                    void *ioctlParam); /*!< Control a controller function prototype*/
} am_usbh_controller_interface_t;

/*! @brief USB host instance structure */
typedef struct _usb_host_instance
{
    void *controllerHandle;                                          /*!< The low level controller handle*/
    pfn_usbh_callback_t deviceCallback;                                  /*!< Device attach/detach callback*/
    am_usb_osa_mutex_handle_t hostMutex;                                  /*!< Host layer mutex*/
    am_usbh_transfer_t transferList[USB_HOST_CONFIG_MAX_TRANSFERS]; /*!< Transfer resource*/
    am_usbh_transfer_t *transferHead;                               /*!< Idle transfer head*/
    const am_usbh_controller_interface_t *controllerTable;          /*!< KHCI/EHCI interface*/
    void *deviceList;                                                /*!< Device list*/
    uint8_t addressBitMap[16]; /*!< Used for address allocation. The first bit is the address 1, second bit is the
                                  address 2*/
    uint8_t occupied;          /*!< 0 - the instance is not occupied; 1 - the instance is occupied*/
    uint8_t controllerId;      /*!< The controller ID*/
} am_usbh_instance_t;

/*! @}*/

#endif /* _USB_HOST_HCI_H_ */
