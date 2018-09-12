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
 * \brief Timing driver implementation for standard timer
 *
 * 1. The standard timer can provide 3 service as follows,This driver implement 
 *    the timing function.
 *     - Timing
 *     - PWM
 *     - CAP
 * 2. In the driver,The four match register used as follows:
 *     - The match register 0 to generate period timing interrupt
 *     - The match register 1 not use
 *     - The match register 2 not use
 *     - The match register 3 not use
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_TIMER_TIMING_H
#define __AM_LPC_TIMER_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "hw/amhw_lpc_timer.h"

/**
 * \addtogroup am_lpc_if_timer_timing
 * \copydoc am_lpc_timer_timing.h
 * @{
 */

/**
 * \brief LPC5410x TIMER initialize parameter structure
 */
typedef struct am_lpc_timer_timing_devinfo {
    uint32_t             timer_regbase;     /**< \brief The standard timer register baseaddr */
    uint8_t              inum;              /**< \brief The standard timer interrupt number    */
    uint32_t             clk_id;            /**< \brief Timer clk id                           */

    void (*pfn_plfm_init)(void);            /**< \brief plfm init  fuc  */

    void (*pfn_plfm_deinit)(void);          /**< \brief plfm deinit fuc */
} am_lpc_timer_timing_devinfo_t;
    
/**
 * \brief LPC5410x standard timer device structure
 */
typedef struct am_lpc_timer_timing_dev {

    am_timer_serv_t   timer_serv;     /**< \brief for timer service           */
    
    struct {    
        void (*pfn_callback)(void *); /**< \brief pointer to callback function       */
        void *p_arg;                  /**< \brief argument for the callback function */
    } callback_info[1];               /**< \brief Only support one channel           */
    
    /** \brief Pointer to the device information */
    const am_lpc_timer_timing_devinfo_t  *p_devinfo; 

} am_lpc_timer_timing_dev_t;

/**
 * \brief Initialize the Timer for timing function
 *
 * \param[in] p_dev     : Pointer to the Timer for timing device
 * \param[in] p_devinfo : Pointer to the Timer for timing device information to initialize
 *
 * \return The handle for standard timer port.if NULL,Indicate that initialize failed
 */
am_timer_handle_t am_lpc_timer_timing_init(am_lpc_timer_timing_dev_t            *p_dev,
                                         const am_lpc_timer_timing_devinfo_t  *p_devinfo);

/**
 * \brief De-initialize the Timer for timing
 * \param[in] p_dev : Pointer to the Timer for timing device
 * \return None
 */
void am_lpc_timer_timing_deinit(am_lpc_timer_timing_dev_t *p_dev);

/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_TIMER_TIMING_H */

/* end of file */
