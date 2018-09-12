/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief MRT drivers for Standard Timer drivers 
 *
 * This file implement the MRT timing function.
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_MRT_H
#define __AM_LPC_MRT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "hw/amhw_lpc_mrt.h"

/**
 * \addtogroup am_lpc_if_mrt
 * \copydoc am_lpc_mrt.h
 * @{
 */

/**
 * \brief LPC5410x MRT device information structure
 */
typedef struct am_lpc_mrt_devinfo {
    uint32_t             mrt_regbase;   /**< \brief 指向MRT寄存器块的指针    */
    uint8_t              inum;          /**< \brief The MRT interrupt number                */
    uint32_t             clk_id;        /**< \brief MRT clk id                              */
    uint8_t              channel_nums;  /**< \brief The channel numbers, maximum value is 4 */

    void (*pfn_plfm_init)(void);        /**< \brief plfm init  fuc  */

    void (*pfn_plfm_deinit)(void);      /**< \brief plfm deinit fuc */

} am_lpc_mrt_devinfo_t;

/**
 * \brief LPC5410x MRT device structure
 */
typedef struct am_lpc_mrt_dev {

    am_timer_serv_t   timer_serv;      /**< \brief for timer service                    */

    struct {
        void (*pfn_callback)(void *);  /**< \brief pointer to callback function        */
        void *p_arg;                   /**< \brief argument for the callback function  */
    } callback_info[AMHW_LPC_MRT_CHANNELS_NUM]; /**< \brief The callback information array */
        
    /** \brief Pointer to the MRT device information */
    const am_lpc_mrt_devinfo_t  *p_devinfo; 

} am_lpc_mrt_dev_t;

/**
 * \brief Initialize the MRT for timer
 *
 * \param[in] p_dev     : Pointer to the MRT device
 * \param[in] p_devinfo : Pointer to the MRT device information to initialize
 *
 * \return The handle for standard TIMER port.if NULL,Indicate that initialize failed
 */
am_timer_handle_t am_lpc_mrt_init(am_lpc_mrt_dev_t            *p_dev,
                                  const am_lpc_mrt_devinfo_t  *p_devinfo);

/**
 * \brief De-initialize the MRT
 * \param[in] p_dev   : Pointer to the MRT device
 * \return None
 */
void am_lpc_mrt_deinit(am_lpc_mrt_dev_t *p_dev);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_MRT_H */

/* end of file */
