/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/
/**
 * \file
 * \brief     USBD标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-27  bob, first implementation.
 * \endinternal
 */

#ifndef __AM_FSL_USBD_KHCI_H__
#define __AM_FSL_USBD_KHCI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "am_usbd_config.h"
#include "am_usbd.h"

#include "hw/amhw_fsl_usb.h"
#include "am_usb_dci.h"

/**
 * \addtogroup am_if_fsl_usbd_khci
 * \copydoc am_fsl_usbd_khci.h
 * @{
 */

/** \brief The maximum value of ISO maximum packet size for FS in USB specification 2.0 */
#define AM_USBD_MAX_FS_ISO_MAX_PACKET_SIZE (1023U)

/** \brief The maximum value of non-ISO maximum packet size for FS in USB specification 2.0 */
#define AM_USBD_MAX_FS_NONE_ISO_MAX_PACKET_SIZE (64U)

/** \brief Set BDT buffer address */
#define AM_USB_KHCI_BDT_SET_ADDRESS(bdt_base, ep, direction, odd, address)                          \
    *((volatile uint32_t *)((bdt_base & 0xfffffe00U) | (((uint32_t)ep & 0x0fU) << 5U) |          \
                            (((uint32_t)direction & 1U) << 4U) | (((uint32_t)odd & 1U) << 3U)) + \
      1U) = address

/** \brief Set BDT control fields*/
#define AM_USB_KHCI_BDT_SET_CONTROL(bdt_base, ep, direction, odd, control)                \
    *(volatile uint32_t *)((bdt_base & 0xfffffe00U) | (((uint32_t)ep & 0x0fU) << 5U) | \
                           (((uint32_t)direction & 1U) << 4U) | (((uint32_t)odd & 1U) << 3U)) = control

/** \brief Get BDT buffer address*/
#define AM_USB_KHCI_BDT_GET_ADDRESS(bdt_base, ep, direction, odd)                                    \
    (*((volatile uint32_t *)((bdt_base & 0xfffffe00U) | (((uint32_t)ep & 0x0fU) << 5U) |          \
                             (((uint32_t)direction & 1U) << 4U) | (((uint32_t)odd & 1U) << 3U)) + \
       1U))

/** \brief Get BDT control fields*/
#define AM_USB_KHCI_BDT_GET_CONTROL(bdt_base, ep, direction, odd)                          \
    (*(volatile uint32_t *)((bdt_base & 0xfffffe00U) | (((uint32_t)ep & 0x0fU) << 5U) | \
                            (((uint32_t)direction & 1U) << 4U) | (((uint32_t)odd & 1U) << 3U)))

/** \brief Endpoint state structure */
typedef struct am_fsl_usbd_khci_ep_stat
{
    uint8_t *p_transfer_buffer; /**< \brief Address of buffer containing the data to be transmitted */
    uint32_t transfer_length; /**< \brief Length of data to transmit. */
    uint32_t transfer_done;   /**< \brief The data length has been transferred*/
    union
    {
        uint32_t state; /**< \brief The state of the endpoint */
        struct
        {
            uint32_t max_packet_size : 10U; /**< \brief The maximum packet size of the endpoint */
            uint32_t stalled : 1U;        /**< \brief The endpoint is stalled or not */
            uint32_t data0 : 1U;          /**< \brief The data toggle of the transaction */
            uint32_t bdt_odd : 1U;         /**< \brief The BDT toggle of the endpoint */
            uint32_t dma_align : 1U;       /**< \brief Whether the transferbuffer is DMA aligned or not */
            uint32_t transferring : 1U;   /**< \brief The endpoint is transferring */
            uint32_t resersed : 1U;       /**< \brief Reversed */
        } state_bit_field;
    } state_union;
} am_fsl_usbd_khci_ep_stat_t;

/** \brief KHCI state structure */
typedef struct am_fsl_usbd_khci_info
{
    amhw_fsl_usbotg_t  *p_hw_usbotg;  /**< \brief usb regbase */
    int                  inum;         /**< \brief int number */

    void   (*p_fsl_plfm_init) (void); /**< \brief usb hardware init */
} am_fsl_usbd_khci_info_t;

/** \brief KHCI state structure */
typedef struct am_fsl_usbd_khci_dev
{
    am_usbd_dev_t  device;    /**< \brief Device handle used to identify the device object belongs to */

    USB_RAM_ADDRESS_ALGINMENT_512 uint8_t bdt[512U]; /**< \brief BDT buffer address */

    amhw_fsl_usbotg_t *p_register_base;            /**< \brief The base address of the register */
    uint8_t setup_packet_buffer[USB_SETUP_PACKET_SIZE * 2]; /**< \brief The setup request buffer */

    /** \brief
     * This buffer is used to fix the transferbuffer or transfer_length does
       not align to 4-bytes when the function USB_DeviceKhciRecv is called.
       The macro USB_DEVICE_CONFIG_KHCI_DMA_ALIGN is used to enable or disable this feature.
       If the feature is enabled, when the transferbuffer or transfer_length does not align to 4-bytes,
       the transfer_length is not more than USB_DEVICE_CONFIG_KHCI_DMA_ALIGN_BUFFER_LENGTH, and
       the flag isDmaAlignBufferInusing is zero, the dmaAlignBuffer is used to receive data
       and the flag isDmaAlignBufferInusing is set to 1.
       When the transfer is done, the received data, kept in dmaAlignBuffer, is copied
       to the transferbuffer, and the flag isDmaAlignBufferInusing is cleared.
     */
    uint8_t dma_align_buffer[(((USB_DEVICE_CONFIG_KHCI_DMA_ALIGN_BUFFER_LENGTH - 1U) >> 2U) + 1U) * 4];

    am_fsl_usbd_khci_ep_stat_t
            endpoint_state[USB_DEVICE_CONFIG_ENDPOINTS * 2]; /**< \brief Endpoint state structures */
    uint8_t is_dma_align_buffer_inusing;                        /**< \brief The dmaAlignBuffer is used or not */
    uint8_t is_resetting;                                    /**< \brief Is doing device reset or not */
    uint8_t controller_id;                                   /**< \brief Controller ID */
    uint8_t setup_buffer_index;                               /**< \brief A valid setup buffer flag */

    am_fsl_usbd_khci_info_t *p_info;
} am_fsl_usbd_khci_dev_t;

#if defined(__cplusplus)
extern "C" {
#endif


am_usbd_handle_t  * am_fsl_usbd_khci_init (am_fsl_usbd_khci_dev_t  *p_dev,
                                             am_fsl_usbd_khci_info_t *p_info);

void  am_fsl_usbd_khci_deinit (am_usbd_handle_t   handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_FSL_USBD_KHCI_H__ */
