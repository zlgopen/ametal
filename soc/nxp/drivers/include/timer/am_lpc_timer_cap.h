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
 * \brief Capture driver implementation for standard timer
 *
 * 1. The standard timer can provide 3 service as follows,This driver implement 
 *    the capture function.
 *     - Timing
 *     - PWM
 *     - CAP
 * 2. In the driver,The four capture register used as follows:
 *     - The capture register 0 to generate capture channel 0
 *     - The capture register 1 to generate capture channel 1
 *     - The capture register 2 to generate capture channel 2
 *     - The capture register 3 to generate capture channel 3
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_TIMER_CAP_H
#define __AM_LPC_TIMER_CAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_cap.h"
#include "hw/amhw_lpc_timer.h"

/**
 * \addtogroup am_lpc_if_timer_cap
 * \copydoc am_lpc_timer_cap.h
 * @{
 */

/**
 * \brief LPC5410x TIMER IO information for Capture 
 */
typedef struct am_lpc_timer_cap_ioinfo {
    uint32_t gpio;                         /**< \brief capture IO                                  */
    uint32_t func;                         /**< \brief The IO function for Capture                 */
    uint32_t dfunc;                        /**< \brief The default function after disable capture  */
} am_lpc_timer_cap_ioinfo_t;

/**
 * \brief LPC5410x timer for CAP initialize parameter structure
 */
typedef struct am_lpc_timer_cap_devinfo {
    uint32_t                 timer_regbase;     /**< \brief The standard timer register baseaddr */
    uint8_t                  inum;          /**< \brief Timer interrupt number                     */
    uint32_t                 clk_id;        /**< \brief Timer clk id                               */
    uint8_t                  channels_num;  /**< \brief Total used channels,The maximum value is 4 */
    am_lpc_timer_cap_ioinfo_t *p_ioinfo;    /**< \brief Pointer to the GPIO information            */

    void (*pfn_plfm_init)(void);            /**< \brief plfm init  fuc                             */
    void (*pfn_plfm_deinit)(void);          /**< \brief plfm deinit fuc                            */
} am_lpc_timer_cap_devinfo_t;
 
/**
 * \brief LPC5410x timer for Capture device structure
 */
typedef struct am_lpc_timer_cap_dev {
    
    /** \brief For standard capture service              */
    am_cap_serv_t           cap_serv;
    
    /** \brief To store four channels callback functions */
    struct {   
        am_cap_callback_t   callback_func;       /**< \brief callback function                   */
        void               *p_arg;               /**< \brief The parameter for callback function */
    }callback_info[4];                           /**< \brief Most support 4 channel              */
    
    /** \brief Pointer to the device information */
    const am_lpc_timer_cap_devinfo_t  *p_devinfo; 

} am_lpc_timer_cap_dev_t;

/**
 * \brief Initialize the TIMER for CAP
 *
 * \param[in] p_dev     : Pointer to the TIMER for CAP device
 * \param[in] p_devinfo : Pointer to the TIMER for CAP device information to initialize
 *
 * \return The handle for standard CAP port.if NULL,Indicate that initialize failed
 */
am_cap_handle_t am_lpc_timer_cap_init(am_lpc_timer_cap_dev_t           *p_dev,
                                    const am_lpc_timer_cap_devinfo_t *p_devinfo);

/**
 * \brief De-initialize the TIMER for CAP
 * \param[in] p_dev : Pointer to the TIMER for CAP device
 * \return None
 */
void am_lpc_timer_cap_deinit(am_lpc_timer_cap_dev_t *p_dev);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_TIMER_CAP_H */

/* end of file */
