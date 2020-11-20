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
  \file
  \brief HC32 TIM 定时功能用户配置文件
  \sa am_hwconf_hc32f460_tim_timing.c
 *
 *
  \internal
  \par Modification history
  - 1.00 
  \endinternal
 */

#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_hc32f460_clk.h"
#include "am_hc32f460_tim_timing.h"
#include "hc32f460_inum.h"

/**
  \addtogroup am_if_src_hwconf_hc32f460_tim_timing
  \copydoc am_hwconf_hc32f460_tim_timing.c
  @{
 */

/*******************************************************************************
  TIM01 配置
*******************************************************************************/

/** \brief TIM01 平台初始化 */
void __hc32f460_plfm_tim01_timing_init (void)
{
    am_clk_enable(CLK_TIMER0_1);
}

/** \brief 解除 TIM01 平台初始化 */
void __hc32f460_plfm_tim01_timing_deinit (void)
{
    am_clk_disable(CLK_TIMER0_1);
}

/** \brief TIM01 设备信息 */
const am_hc32f460_tim_timing_devinfo_t  __g_tim01_timing_devinfo = {
    HC32F460_TIMER0_1_BASE,                /**< \brief TIM0寄存器块的基地址 */
    INUM_TIMER0_1,                         /**< \brief TIM0中断编号 */
    tim0_pclk1,                            /**< \brief Tim0_syncClockSource */
    tim0_XTAL32,                           /**< \brief Tim0_AsyncClockSource */
    tim0_sync,
    TIM0_CHANNEL_B,                        /**< \brief demo 中的Channel_num需要与硬件配置文件am_hwconf_hc32f460_tim_timing.c一致 */
    __hc32f460_plfm_tim01_timing_init,     /**< \brief 平台初始化函数 */
    __hc32f460_plfm_tim01_timing_deinit    /**< \brief 平台解析初始化函数 */
};

/** \brief TIM01 设备定义 */
am_hc32f460_tim_timing_dev_t __g_tim01_timing_dev;

/** \brief 定时器TIM01 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_hc32f460_tim01_timing_inst_init (void)
{
    return am_hc32f460_tim_timing_init(&__g_tim01_timing_dev,
                                       &__g_tim01_timing_devinfo);
}

/** \brief TIM01 Timing实例解初始化 */
void am_hc32f460_tim01_timing_inst_deinit (am_timer_handle_t handle)
{
    am_hc32f460_tim_timing_deinit(handle);
}

/*******************************************************************************
  TIM02 配置
*******************************************************************************/

/** \brief TIM02 平台初始化 */
void __hc32f460_plfm_tim02_timing_init (void)
{
    am_clk_enable(CLK_TIMER0_2);
}

/** \brief 解除 TIM02 平台初始化 */
void __hc32f460_plfm_tim02_timing_deinit (void)
{
    am_clk_disable(CLK_TIMER0_2);
}

/** \brief TIM02 设备信息 */
const am_hc32f460_tim_timing_devinfo_t  __g_tim02_timing_devinfo = {
    HC32F460_TIMER0_2_BASE,                /**< \brief TIM0寄存器块的基地址 */
    INUM_TIMER0_2,                         /**< \brief TIM0中断编号 */
    tim0_pclk1,                            /**< \brief Tim0_syncClockSource */
    tim0_XTAL32,                           /**< \brief Tim0_AsyncClockSource */
    tim0_sync,
    TIM0_CHANNEL_B,                        /**< \brief demo 中的Channel_num需要与硬件配置文件am_hwconf_hc32f460_tim_timing.c一致 */
    __hc32f460_plfm_tim02_timing_init,     /**< \brief 平台初始化函数 */
    __hc32f460_plfm_tim02_timing_deinit    /**< \brief 平台解析初始化函数 */
};

/** \brief TIM02 设备定义 */
am_hc32f460_tim_timing_dev_t __g_tim02_timing_dev;

/** \brief 定时器TIM02 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_hc32f460_tim02_timing_inst_init (void)
{
    return am_hc32f460_tim_timing_init(&__g_tim02_timing_dev,
                                     &__g_tim02_timing_devinfo);
}

/** \brief TIM02 Timing实例解初始化 */
void am_hc32f460_tim02_timing_inst_deinit (am_timer_handle_t handle)
{
    am_hc32f460_tim_timing_deinit(handle);
}


/**
  @}
 */

/* end of file */
