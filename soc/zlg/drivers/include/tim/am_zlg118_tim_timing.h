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

#ifndef __AM_ZLG118_TIM_TIMING_H
#define __AM_ZLG118_TIM_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "hw/amhw_zlg118_tim.h"

/**
 * \addtogroup am_zlg118_if_tim_timing
 * \copydoc am_zlg118_tim_timing.h
 * @{
 */

/**
 * \brief TIM定时功能相关的设备信息
 */
typedef struct am_zlg118_tim_timing_devinfo {

    uint32_t                        tim_regbase;    /**< \brief TIM寄存器块基址 */

    uint8_t                         inum;           /**< \brief TIM中断号 */

    amhw_zlg118_tim_type_t          tim_type;       /**< \brief 定时器类型 */

    /** \brief 16/32位计数    注意：32位计数是不可重装载的，16位计数可重装载*/
    amhw_zlg118_tim_mode0_counter_t counter_type;

    /** \brief 门控使能，电平有效才开启计数（需要配置对应定时器GATE引脚）*/
    amhw_zlg118_tim_gate_t          gate_enable;

    /** \brief 门控电平有效选择    AM_TRUE：高电平              AM_FALSE：低电平 */
    am_bool_t                       gate_state;

    /** \brief 计数时钟选择（内部或外部），若采用外部，需要配置对应定时器ETR引脚 */
    amhw_zlg118_tim_clk_src_t       clk_src;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void                            (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void                            (*pfn_plfm_deinit)(void);

} am_zlg118_tim_timing_devinfo_t;

/**
 * \brief TIM定时功能设备
 */
typedef struct am_zlg118_tim_timing_dev {

    am_timer_serv_t timer_serv;         /**< \brief 标准定时(Timer)服务 */

    void (*pfn_callback)(void *);       /**< \brief 回调函数 */
    void  *p_arg;                       /**< \brief 回调函数的用户参数 */

    uint16_t           prescale_to;     /**< \brief 期望分频值  */
    uint16_t           prescale_in;     /**< \brief 实际分频值  */

    /** \brief 指向TIM(定时功能)设备信息常量的指针 */
    const am_zlg118_tim_timing_devinfo_t  *p_devinfo;

} am_zlg118_tim_timing_dev_t;

/**
 * \brief 初始化TIM为定时功能
 *
 * \param[in] p_dev     : 指向TIM(定时功能)设备的指针
 * \param[in] p_devinfo : 指向TIM(定时功能)设备信息常量的指针
 *
 * \return Timer标准服务操作句柄，值为NULL时表明初始化失败
 */
am_timer_handle_t
am_zlg118_tim_timing_init (am_zlg118_tim_timing_dev_t              *p_dev,
                           const am_zlg118_tim_timing_devinfo_t *p_devinfo);

/**
 * \brief 不使用TIM定时功能时，解初始化TIM定时功能，释放相关资源
 *
 * \param[in] handle : am_zlg118_tim_timing_init() 初始化函数获得的Timer服务句柄
 *
 * \return 无
 */
void am_zlg118_tim_timing_deinit (am_timer_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG118_TIM_TIMING_H */

/* end of file */
