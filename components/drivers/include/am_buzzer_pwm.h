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
 * \brief 无源蜂鸣器标准接口
 *
 * 无源蜂鸣器使用PWM驱动 
 *
 * \internal
 * \par Modification history
 * - 1.01 14-12-09  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_BUZZER_PWM_H
#define __AM_BUZZER_PWM_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_common.h"
#include "am_pwm.h"
    
/**
 * \addtogroup am_if_buzzer_pwm
 * \copydoc am_buzzer_pwm.h
 * @{
 */

/**
 * \brief 初始化无源蜂鸣器
  *
 * \param[in] pwm_handle : PWM标准接口操作句柄
 * \param[in] chan       : PWM通道号
 * \param[in] duty_ns    : PWM脉宽时间（单位：纳秒）
 * \param[in] period_ns  : PWM周期时间（单位：纳秒）
 *
 * \return 错误号
 */ 
int am_buzzer_pwm_init (am_pwm_handle_t pwm_handle,
                        int             chan,
                        unsigned int    duty_ns,
                        unsigned int    period_ns);
 
/** 
 * @}  
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_BUZZER_PWM_H */

/* end of file */
