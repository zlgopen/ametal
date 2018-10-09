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
 * \brief PWM drivers implementation
 *
 * 1. The SCT can provide 3 service as follows,This driver implement 
 *    the PWM function.
 *     - Timing
 *     - PWM
 *     - CAP
 * 2. In the driver,The SCT resource used as follows:
 *    - The match 0 to generate period.associated with event 0.
 *    - The match 1 to generate PWM channel 0 duty. output through SCT_OUT0
 *    - The match 2 to generate PWM channel 1 duty. output through SCT_OUT1
 *    - The match 3 to generate PWM channel 2 duty. output through SCT_OUT2
 *    - The match 4 to generate PWM channel 3 duty. output through SCT_OUT3
 *    - The match 5 to generate PWM channel 4 duty. output through SCT_OUT4
 *    - The match 6 to generate PWM channel 5 duty. output through SCT_OUT5
 *    - The match 7 to generate PWM channel 6 duty. output through SCT_OUT6
 *    - The match 8 to generate PWM channel 7 duty. output through SCT_OUT7
 *
 * \note One SCT output PWMs shared one period time,in other word,the PWMs
 *       frequency is same.
 *
 * \internal
 * \par Modification history
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */


#ifndef __AM_LPC_SCT_PWM_H
#define __AM_LPC_SCT_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_pwm.h"
#include "hw/amhw_lpc_sct.h"

/**
 * \addtogroup am_lpc_if_sct_pwm
 * \copydoc am_lpc_sct_pwm.h
 * @{
 */

/**
 * \brief LPC5410x GPIO information for PWM 
 */
typedef struct am_lpc_sct_pwm_ioinfo {
    uint32_t gpio;                          /**< \brief PWM IO                                  */
    uint32_t func;                          /**< \brief The IO function for PWM                 */
    uint32_t dfunc;                         /**< \brief The default function after disable PWM  */
} am_lpc_sct_pwm_ioinfo_t;

/**
 * \brief LPC5410x SCT for PWM initialize parameter structure
 */
typedef struct am_lpc_sct_pwm_devinfo {
    uint32_t                   sct_regbase;   /**< \brief SCT¼Ä´æÆ÷»ùµØÖ·.                             */
    uint32_t                   clk_id;        /**< \brief SCT clk id                                */
    uint8_t                    channels_num;  /**< \brief Total support channels(maximum value is 8) */
    am_lpc_sct_pwm_ioinfo_t   *p_ioinfo;      /**< \brief Pointer to the GPIO information            */

    void (*pfn_plfm_init)(void);              /**< \brief plfm init  fuc                             */
    void (*pfn_plfm_deinit)(void);            /**< \brief plfm deinit fuc                            */
} am_lpc_sct_pwm_devinfo_t;
 
/**
 * \brief LPC5410x SCT for PWM device structure
 */
typedef struct am_lpc_sct_pwm_dev {
    
    /** \brief for PWM service           */
    am_pwm_serv_t                 pwm_serv;
    
    /** \brief Pointer to the device information */
    const am_lpc_sct_pwm_devinfo_t  *p_devinfo; 

} am_lpc_sct_pwm_dev_t;

/**
 * \brief Initialize the SCT for PWM
 *
 * \param[in] p_dev     : Pointer to the SCT for PWM device
 * \param[in] p_devinfo : Pointer to the SCT for PWM device information to initialize
 *
 * \return The handle for standard PWM port.if NULL,Indicate that initialize failed
 */
am_pwm_handle_t am_lpc_sct_pwm_init(am_lpc_sct_pwm_dev_t           *p_dev,
                                    const am_lpc_sct_pwm_devinfo_t *p_devinfo);

/**
 * \brief De-initialize the SCT for PWM
 * \param[in] p_dev : Pointer to the SCT for PWM device
 * \return None
 */
void am_lpc_sct_pwm_deinit(am_lpc_sct_pwm_dev_t *p_dev);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_SCT_PWM_H */

/* end of file */
