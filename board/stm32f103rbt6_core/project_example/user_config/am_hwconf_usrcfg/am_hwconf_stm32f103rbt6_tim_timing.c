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
  \brief STM32F103RBT6 TIM 定时功能用户配置文件
  \sa am_hwconf_stm32f103rbt6_tim_timing.c
 *
 *
  \internal
  \par Modification history
  - 1.00 17-08-25  zcb, first implementation
  \endinternal
 */

#include "am_stm32f103rbt6.h"
#include "ametal.h"
#include "am_clk.h"
#include "am_stm32f103rbt6_clk.h"
#include "am_stm32f103rbt6_tim_timing.h"

/**
  \addtogroup am_if_src_hwconf_stm32f103rbt6_tim_timing
  \copydoc am_hwconf_stm32f103rbt6_tim_timing.c
  @{
 */

/*******************************************************************************
  TIM1 配置
*******************************************************************************/

/** \brief TIM1 平台初始化 */
void __stm32f103rbt6_plfm_tim1_timing_init (void)
{
    am_clk_enable(CLK_TIM1);
    am_stm32f103rbt6_clk_reset(CLK_TIM1);
}

/** \brief 解除 TIM1 平台初始化 */
void __stm32f103rbt6_plfm_tim1_timing_deinit (void)
{
    am_stm32f103rbt6_clk_reset(CLK_TIM1);
    am_clk_disable(CLK_TIM1);
}

/** \brief TIM1 设备信息 */
const am_stm32f103rbt6_tim_timing_devinfo_t  __g_tim1_timing_devinfo = {
    STM32F103RBT6_TIM1_BASE,                  /**< \brief TIM1寄存器块的基地址 */
    INUM_TIM1_UP,                      /**< \brief TIM1中断编号 */
    CLK_TIM1,                          /**< \brief TIM1时钟ID */
    AMHW_STM32F103RBT6_TIM_TYPE0,             /**< \brief 定时器类型 */
    __stm32f103rbt6_plfm_tim1_timing_init,    /**< \brief 平台初始化函数 */
    __stm32f103rbt6_plfm_tim1_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief TIM1 设备定义 */
am_stm32f103rbt6_tim_timing_dev_t __g_tim1_timing_dev;

/** \brief 定时器TIM1 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_stm32f103rbt6_tim1_timing_inst_init (void)
{
    return am_stm32f103rbt6_tim_timing_init(&__g_tim1_timing_dev,
                                     &__g_tim1_timing_devinfo);
}

/** \brief TIM1 Timing实例解初始化 */
void am_stm32f103rbt6_tim1_timing_inst_deinit (am_timer_handle_t handle)
{
    am_stm32f103rbt6_tim_timing_deinit(handle);
}

/*******************************************************************************
  TIM2 配置
*******************************************************************************/

/** \brief TIM2 平台初始化 */
void __stm32f103rbt6_plfm_tim2_timing_init (void)
{
    am_clk_enable(CLK_TIM2);
    am_stm32f103rbt6_clk_reset(CLK_TIM2);
}

/** \brief 解除 TIM2 平台初始化 */
void __stm32f103rbt6_plfm_tim2_timing_deinit (void)
{
    am_stm32f103rbt6_clk_reset(CLK_TIM2);
    am_clk_disable(CLK_TIM2);
}

/** \brief TIM2 设备信息 */
const am_stm32f103rbt6_tim_timing_devinfo_t  __g_tim2_timing_devinfo = {
    STM32F103RBT6_TIM2_BASE,                  /**< \brief TIM2寄存器块的基地址 */
    INUM_TIM2,                         /**< \brief TIM2中断编号 */
    CLK_TIM2,                          /**< \brief TIM2时钟ID */
    AMHW_STM32F103RBT6_TIM_TYPE1,             /**< \brief 定时器类型 */
    __stm32f103rbt6_plfm_tim2_timing_init,    /**< \brief 平台初始化函数 */
    __stm32f103rbt6_plfm_tim2_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief TIM2 设备定义 */
am_stm32f103rbt6_tim_timing_dev_t  __g_tim2_timing_dev;

/** \brief TIM2 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_stm32f103rbt6_tim2_timing_inst_init (void)
{
    return am_stm32f103rbt6_tim_timing_init(&__g_tim2_timing_dev,
                                     &__g_tim2_timing_devinfo);
}

/** \brief TIM2 Timing实例解初始化 */
void am_stm32f103rbt6_tim2_timing_inst_deinit (am_timer_handle_t handle)
{
    am_stm32f103rbt6_tim_timing_deinit(handle);
}

/*******************************************************************************
  TIM3配置
*******************************************************************************/

/** \brief TIM3平台初始化 */
void __stm32f103rbt6_plfm_tim3_timing_init (void)
{
    am_clk_enable(CLK_TIM3);
    am_stm32f103rbt6_clk_reset(CLK_TIM3);
}

/** \brief 解除TIM3平台初始化 */
void __stm32f103rbt6_plfm_tim3_timing_deinit (void)
{
    am_stm32f103rbt6_clk_reset(CLK_TIM3);
    am_clk_disable(CLK_TIM3);
}

/** \brief TIM3 设备信息 */
const am_stm32f103rbt6_tim_timing_devinfo_t  __g_tim3_timing_devinfo = {
    STM32F103RBT6_TIM3_BASE,                  /**< \brief TIM3寄存器块的基地址 */
    INUM_TIM3,                         /**< \brief TIM3中断编号 */
    CLK_TIM3,                          /**< \brief TIM3时钟ID */
    AMHW_STM32F103RBT6_TIM_TYPE1,             /**< \brief 定时器类型 */
    __stm32f103rbt6_plfm_tim3_timing_init,    /**< \brief 平台初始化函数 */
    __stm32f103rbt6_plfm_tim3_timing_deinit   /**< \brief 平台解初始化函数 */
};

/** \brief TIM3设备定义 */
am_stm32f103rbt6_tim_timing_dev_t  __g_tim3_timing_dev;

/** \brief TIM3 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_stm32f103rbt6_tim3_timing_inst_init (void)
{
    return am_stm32f103rbt6_tim_timing_init(&__g_tim3_timing_dev,
                                     &__g_tim3_timing_devinfo);
}

/** \brief TIM3 Timing实例解初始化 */
void am_stm32f103rbt6_tim3_timing_inst_deinit (am_timer_handle_t handle)
{
    am_stm32f103rbt6_tim_timing_deinit(handle);
}

/*******************************************************************************
  TIM4配置
*******************************************************************************/

/** \brief TIM4平台初始化 */
void __stm32f103rbt6_plfm_tim4_timing_init (void)
{
    am_clk_enable(CLK_TIM4);
    am_stm32f103rbt6_clk_reset(CLK_TIM4);
}

/** \brief 解除TIM4平台初始化 */
void __stm32f103rbt6_plfm_tim4_timing_deinit (void)
{
    am_stm32f103rbt6_clk_reset(CLK_TIM4);
    am_clk_disable(CLK_TIM4);
}

/** \brief TIM4设备信息 */
const am_stm32f103rbt6_tim_timing_devinfo_t  __g_tim4_timing_devinfo = {
    STM32F103RBT6_TIM4_BASE,                  /**< \brief TIM4寄存器块的基地址 */
    INUM_TIM4,                         /**< \brief TIM4中断编号 */
    CLK_TIM4,                          /**< \brief TIM4时钟ID */
    AMHW_STM32F103RBT6_TIM_TYPE1,             /**< \brief 定时器类型 */
    __stm32f103rbt6_plfm_tim4_timing_init,    /**< \brief 平台初始化函数 */
    __stm32f103rbt6_plfm_tim4_timing_deinit   /**< \brief 平台解初始化函数 */
};

/** \brief TIM4设备定义 */
am_stm32f103rbt6_tim_timing_dev_t  __g_tim4_timing_dev;

/** \brief TIM4 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_stm32f103rbt6_tim4_timing_inst_init (void)
{
    return am_stm32f103rbt6_tim_timing_init(&__g_tim4_timing_dev,
                                     &__g_tim4_timing_devinfo);
}

/** \brief TIM4 Timing实例解初始化 */
void am_stm32f103rbt6_tim4_timing_inst_deinit (am_timer_handle_t handle)
{
    am_stm32f103rbt6_tim_timing_deinit(handle);
}

/**
  @}
 */

/* end of file */
