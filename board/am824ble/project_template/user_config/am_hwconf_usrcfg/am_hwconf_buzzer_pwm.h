/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 蜂鸣器用户配置文件（PWM 驱动）
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-20  pea, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_BUZZER_PWM_H
#define __AM_HWCONF_BUZZER_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_pwm.h"

/**
 * \brief 蜂鸣器实例初始化
 *
 * 当使用蜂鸣器时，默认将使用 SCT 的 OUT1 输出 PWM 波形，因此已经将 SCT 初始化为 PWM
 * 功能，由于 SCT 可以输出六路 PWM，因此可以使用本函数返回的 handle，以便使用其它 5
 * 路 PWM
 *
 * \param 无
 *
 * \return PWM 标准服务句柄，若为 NULL，表明初始化失败
 */
am_pwm_handle_t am_buzzer_pwm_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_HWCONF_BUZZER_PWM_H */

/* end of file */
