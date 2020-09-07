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
 * \brief 定时器TIM驱动，服务Timer驱动层实现
 *
 * 1. TIM支持提供如下三种标准服务，本驱动提供的是服务Timer标准服务的驱动。
 *     - 定时
 *     - PWM输出
 *     - 捕获
 * 2. 一个TIM模块，只有一个定时通道
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-21  nwt, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_TIMEA_TIMING_H
#define __AM_HC32F460_TIMEA_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "hw/amhw_hc32f460_timea.h"

/**
 * \addtogroup am_hc32f460_if_tim_timing
 * \copydoc am_hc32f460_tim_timing.h
 * @{
 */

/**
 * \brief TIM定时功能相关的设备信息
 */
typedef struct am_hc32f460_timea_timing_devinfo {

    uint32_t                      timea_regbase;  /**< \brief TIM寄存器块基址 */
    uint8_t                       inum;           /**< \brief TIM中断号 */    
    timea_counter_mode_t          timea_counter_mode;     
    timea_count_dir_t             timea_count_dir;
    uint8_t                       sync_startup_en;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void                            (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void                            (*pfn_plfm_deinit)(void);

} am_hc32f460_timea_timing_devinfo_t;

/**
 * \brief TIM定时功能设备
 */
typedef struct am_hc32f460_timea_timing_dev {

    am_timer_serv_t timer_serv;         /**< \brief 标准定时(Timer)服务 */

    void (*pfn_callback)(void *);       /**< \brief 回调函数 */
    void  *p_arg;                       /**< \brief 回调函数的用户参数 */

    uint16_t           prescale_to;     /**< \brief 期望分频值  */
    uint16_t           prescale_in;     /**< \brief 实际分频值  */

    /** \brief 指向TIM(定时功能)设备信息常量的指针 */
    const am_hc32f460_timea_timing_devinfo_t  *p_devinfo;

} am_hc32f460_timea_timing_dev_t;

/**
 * \brief 初始化TIM为定时功能
 *
 * \param[in] p_dev     : 指向TIM(定时功能)设备的指针
 * \param[in] p_devinfo : 指向TIM(定时功能)设备信息常量的指针
 *
 * \return Timer标准服务操作句柄，值为NULL时表明初始化失败
 */
am_timer_handle_t
am_hc32f460_timea_timing_init (am_hc32f460_timea_timing_dev_t           *p_dev,
                               const am_hc32f460_timea_timing_devinfo_t *p_devinfo);

/**
 * \brief 不使用TIM定时功能时，解初始化TIM定时功能，释放相关资源
 *
 * \param[in] handle : am_hc32f460_timea_timing_init() 初始化函数获得的Timer服务句柄
 *
 * \return 无
 */
void am_hc32f460_timea_timing_deinit (am_timer_handle_t handle);


void TimerA1OV_IrqHandler(void *p_arg);
void TimerA1UD_IrqHandler(void *p_arg);
void TimerA1CMP_IrqHandler(void *p_arg);
void TimerA2OV_IrqHandler(void *p_arg);
void TimerA2UD_IrqHandler(void *p_arg);
void TimerA2CMP_IrqHandler(void *p_arg);
void TimerA3OV_IrqHandler(void *p_arg);
void TimerA3UD_IrqHandler(void *p_arg);
void TimerA3CMP_IrqHandler(void *p_arg);
void TimerA4OV_IrqHandler(void *p_arg);
void TimerA4UD_IrqHandler(void *p_arg);
void TimerA4CMP_IrqHandler(void *p_arg);
void TimerA5OV_IrqHandler(void *p_arg);
void TimerA5UD_IrqHandler(void *p_arg);
void TimerA5CMP_IrqHandler(void *p_arg);
void TimerA6OV_IrqHandler(void *p_arg);
void TimerA6UD_IrqHandler(void *p_arg);
void TimerA6CMP_IrqHandler(void *p_arg);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_TIMEA_TIMING_H */

/* end of file */
