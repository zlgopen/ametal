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
 * \brief 定时器TIM驱动，服务PWM标准接口
 *
 * 1. TIM支持提供如下三种标准服务，本驱动提供的是服务PWM标准服务的驱动
 *     - 定时
 *     - PWM输出
 *     - 捕获
 * 2. TIM1、TIM2、TIM3可配置4路PWM通道（边缘或中间对齐模式），
 *    TIM14、TIM16、TIM17可配置1路PWM通道（边缘对齐模式）
 *
 * \note 一个TIM输出的所有PWM共享周期值，也就是说，该TIM输出的
 * 所有PWM周期相同，频率相同
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-16  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_TIM4_PWM_H
#define __AM_HC32F460_TIM4_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_pwm.h"
#include "hw/amhw_hc32f460_tim4.h"

/**
 * \addtogroup am_hc32f460_if_tim_pwm
 * \copydoc am_hc32f460_tim_pwm.h
 * @{
 */

/**
 * \brief TIMPWM输出功能相关的GPIO信息
 */
typedef struct am_hc32f460_tim4_pwm_chaninfo {
    int8_t   channel;                  /**< \brief PWM所使用的通道标识符 */
    int8_t   gpio;                     /**< \brief PWM输出所用的GPIO引脚 */
    uint32_t func;                     /**< \brief PWM功能的GPIO功能设置值 */
    uint32_t dfunc;                    /**< \brief 禁能PWM模式后，默认GPIO功能设置值 */
} am_hc32f460_tim4_pwm_chaninfo_t;

/**
 * \brief TIMPWM输出功能相关的设备信息
 */
typedef struct am_hc32f460_tim4_pwm_devinfo {

    /** \brief TIM寄存器块基址 */
    uint32_t                          tim_regbase;

    /** \brief 使用的通道数，最大为6 */
    uint8_t                           channels_num;

    en_timer4_pwm_ch_t                pwm_ch;

    /** \brief 指向PWM输出通道信息结构体 */
    am_hc32f460_tim4_pwm_chaninfo_t  *p_chaninfo;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void                            (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void                            (*pfn_plfm_deinit)(void);

} am_hc32f460_tim4_pwm_devinfo_t;

/**
 * \brief TIMPWM输出功能设备结构体
 */
typedef struct am_hc32f460_tim4_pwm_dev {

    am_pwm_serv_t                      pwm_serv;   /**< \brief 标准PWM服务 */

    /** \brief 指向TIM(PWM输出功能)设备信息常量的指针 */
    const am_hc32f460_tim4_pwm_devinfo_t *p_devinfo;

    uint8_t                            chan_max;   /**< \brief 有效的最大通道数  */

} am_hc32f460_tim4_pwm_dev_t;

/**
 * \brief 初始化TIM为PWM输出功能
 *
 * \param[in] p_dev     : 指向TIM(PWM输出功能)设备的指针
 * \param[in] p_devinfo : 指向TIM(PWM输出功能)设备信息常量的指针
 *
 * \return PWM标准服务操作句柄，值为NULL时表明初始化失败
 */
am_pwm_handle_t am_hc32f460_tim4_pwm_init(am_hc32f460_tim4_pwm_dev_t              *p_dev,
                                          const am_hc32f460_tim4_pwm_devinfo_t    *p_devinfo);

/**
 * \brief 不使用TIMPWM输出功能时，解初始化TIMPWM输出功能，释放相关资源
 *
 * \param[in] handle : am_hc32f460_tim4_pwm_init() 初始化函数获得的PWM服务句柄
 *
 * \return 无
 */
void am_hc32f460_tim4_pwm_deinit (am_pwm_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_TIM4_PWM_H */

/* end of file */
