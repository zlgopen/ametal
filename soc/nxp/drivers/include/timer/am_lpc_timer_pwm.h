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
 * \brief PWM driver implementation for standard timer
 *
 * 1. The standard timer can provide 3 service as follows,This driver implement 
 *    the PWM function.
 *     - Timing
 *     - PWM
 *     - CAP
 * 2. In the driver,The four match register used as follows:
 *        - The match 3 to generate all PWMs period.
 *        - The match 0 to generate PWM channel 0 duty.
 *        - The match 1 to generate PWM channel 1 duty.
 *        - The match 2 to generate PWM channel 2 duty.
 *
 * \note One timer output PWMs shared one period time,in other word,the PWMs
 *       frequency are same. 
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_TIMER_PWM_H
#define __AM_LPC_TIMER_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_pwm.h"
#include "hw/amhw_lpc_timer.h"

/**
 * \addtogroup am_lpc_if_timer_pwm
 * \copydoc am_lpc_timer_pwm.h
 * @{
 */

/**
 * \brief LPC5410x GPIO information for PWM 
 */
typedef struct am_lpc_timer_pwm_ioinfo {
    uint32_t gpio;                          /**< \brief PWM IO                                  */
    uint32_t func;                          /**< \brief The IO function for PWM                 */
    uint32_t dfunc;                         /**< \brief The default function after disable PWM  */
} am_lpc_timer_pwm_ioinfo_t;

/**
 * \brief LPC5410x timer for PWM initialize parameter structure
 */
typedef struct am_lpc_timer_pwm_devinfo {
    uint32_t                    timer_regbase; /**< \brief The standard timer register baseaddr */
    uint32_t                    clk_id;        /**< \brief Timer clk id                           */
    uint8_t                     channels_num;  /**< \brief Total support channels                 */
    am_lpc_timer_pwm_ioinfo_t  *p_ioinfo;      /**< \brief Pointer to the GPIO information        */

    void (*pfn_plfm_init)(void);               /**< \brief plfm init  fuc                         */
    void (*pfn_plfm_deinit)(void);             /**< \brief plfm deinit fuc                        */
} am_lpc_timer_pwm_devinfo_t;
 
/**
 * \brief LPC5410x timer for PWM device structure
 */
typedef struct am_lpc_timer_pwm_dev {
    
    /** \brief for PWM service */
    am_pwm_serv_t                    pwm_serv;
    
    /** \brief Pointer to the device information */
    const am_lpc_timer_pwm_devinfo_t  *p_devinfo; 

} am_lpc_timer_pwm_dev_t;

/**
 * \brief Initialize the TIMER for PWM
 *
 * \param[in] p_dev   : Pointer to the TIMER for PWM device
 * \param[in] p_devinfo : Pointer to the TIMER for PWM device information to initialize
 *
 * \return The handle for standard PWM port.if NULL, Indicate that initialize failed
 */
am_pwm_handle_t am_lpc_timer_pwm_init(am_lpc_timer_pwm_dev_t           *p_dev,
                                    const am_lpc_timer_pwm_devinfo_t *p_devinfo);

/**
 * \brief De-initialize the TIMER for PWM
 * \param[in] p_dev : Pointer to the TIMER for PWM device
 * \return None
 */
void am_lpc_timer_pwm_deinit(am_lpc_timer_pwm_dev_t *p_dev);

/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_TIMER_PWM_H */

/* end of file */
