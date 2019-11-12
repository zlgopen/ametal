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
 * \brief 定时器LPTIM驱动，服务Timer驱动层实现
 *
 * 1. LPTIM支持提供如下三种标准服务，本驱动提供的是服务Timer标准服务的驱动。
 *     - 定时
 *     - PWM输出
 *     - 捕获
 * 2. 一个LPTIM模块，只有一个定时通道
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-27  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HC32_LPTIM_LPTIMING_H
#define __AM_HC32_LPTIM_LPTIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "hw/amhw_hc32_lptim.h"

/**
 * \addtogroup am_hc32_if_lptim_timing
 * \copydoc am_hc32_lptim_timing.h
 * @{
 */

/**
 * \brief LPTIM定时功能相关的设备信息
 */
typedef struct am_hc32_lptim_timing_devinfo {

    uint32_t               lptim_regbase;   /**< \brief LPTIM寄存器块基址 */

    uint8_t                inum;            /**< \brief LPTIM中断号 */

    amhw_hc32_lptim_clk_src_t   clk_src;  /**< \brief 计数/计时时钟源选择 */

    /** \brief 计数/计时功能选择.选择计数时需要初始化相关ETR引脚，对外部输入的下降沿进行计数 */
    amhw_hc32_lptim_function_t  fun_sel;

    amhw_hc32_lptim_mode_t      mode;     /**< \brief 重载/不重载模式选择  */

    am_bool_t                     gate_enable; /**< \brief 门控使能控制  */

    am_bool_t                     gate_state;  /**< \brief 门控有效信号设置  */

    /** \brief AM_FALSE:TOG,TOGN同时输出0    AM_TRUE: TOG,TOGN输出相位相反的信号*/
    am_bool_t                     tog_state;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void                 (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void                 (*pfn_plfm_deinit)(void);

} am_hc32_lptim_timing_devinfo_t;

/**
 * \brief LPTIM定时功能设备
 */
typedef struct am_hc32_lptim_timing_dev {

    am_timer_serv_t timer_serv;         /**< \brief 标准定时(Timer)服务 */

    void (*pfn_callback)(void *);       /**< \brief 回调函数 */
    void  *p_arg;                       /**< \brief 回调函数的用户参数 */

    /** \brief 指向LPTIM(定时功能)设备信息常量的指针 */
    const am_hc32_lptim_timing_devinfo_t  *p_devinfo;

    uint32_t  clk_rate;                 /**< \brief 时钟频率 */

    am_timer_info_t  timer_info;        /**< \brief 定时器特性 */
} am_hc32_lptim_timing_dev_t;

/**
 * \brief 初始化LPTIM为定时功能
 *
 * \param[in] p_dev     : 指向LPTIM(定时功能)设备的指针
 * \param[in] p_devinfo : 指向LPTIM(定时功能)设备信息常量的指针
 *
 * \return Timer标准服务操作句柄，值为NULL时表明初始化失败
 */
am_timer_handle_t
am_hc32_lptim_timing_init (am_hc32_lptim_timing_dev_t           *p_dev,
                               const am_hc32_lptim_timing_devinfo_t *p_devinfo);

/**
 * \brief 不使用LPTIM定时功能时，解初始化LPTIM定时功能，释放相关资源
 *
 * \param[in] handle : am_hc32_lptim_timing_init() 初始化函数获得的Timer服务句柄
 *
 * \return 无
 */
void am_hc32_lptim_timing_deinit (am_timer_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32_LPTIM_LPTIMING_H */

/* end of file */
