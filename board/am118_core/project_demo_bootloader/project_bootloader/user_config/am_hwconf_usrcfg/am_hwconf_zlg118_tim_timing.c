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
  \brief ZLG118 TIM 定时功能用户配置文件
  \sa am_hwconf_zlg118_tim_timing.c
 *
 *
  \internal
  \par Modification history
  - 1.00 19-09-11  zp, first implementation
  \endinternal
 */

#include "ametal.h"
#include "am_clk.h"
#include "am_zlg118.h"
#include "am_gpio.h"
#include "am_zlg118_clk.h"
#include "am_zlg118_tim_timing.h"
#include "am_zlg118_adtim_timing.h"

/**
  \addtogroup am_if_src_hwconf_zlg118_tim_timing
  \copydoc am_hwconf_zlg118_tim_timing.c
  @{
 */

/*******************************************************************************
  TIM0 配置
*******************************************************************************/

/** \brief TIM0 平台初始化 */
void __zlg118_plfm_tim0_timing_init (void)
{
//    /* 配置GATE引脚，以实现门控功能 */
//    am_gpio_pin_cfg(PIOB_8, PIOB_8_INPUT_PD | PIOB_8_TIM0_GATE);
//
//    /* 配置ETR引脚，以外部时钟作为计数时钟功能 */
//    am_gpio_pin_cfg(PIOA_0, PIOA_0_INPUT_PD | PIOA_0_TIM0_ETR);

    am_clk_enable(CLK_TIM012);
}

/** \brief 解除 TIM0 平台初始化 */
void __zlg118_plfm_tim0_timing_deinit (void)
{
//    am_gpio_pin_cfg(PIOB_8, PIOB_8_INPUT_PU | PIOB_8_GPIO);
//
//    am_gpio_pin_cfg(PIOA_0, PIOA_0_INPUT_PU | PIOA_0_GPIO);

    am_clk_disable(CLK_TIM012);
}

/** \brief TIM0 设备信息 */
const am_zlg118_tim_timing_devinfo_t  __g_tim0_timing_devinfo = {
    ZLG118_TIM0_BASE,                  /**< \brief TIM0寄存器块的基地址 */
    INUM_TIM0,                         /**< \brief TIM0中断编号 */
    AMHW_ZLG118_TIM_TYPE_TIM0,         /**< \brief 定时器类型 */
    AMHW_ZLG118_TIM_MODE0_COUNTER_16,  /**< \brief 16位重载计数 */
    AMHW_ZLG118_TIM_GATE_DISABLE,      /**< \brief 门控状态（默认关闭） */
    AM_TRUE,                           /**< \brief 门控信号为真（1）有效 */
    AMHW_ZLG118_TIM_CLK_SRC_TCLK,      /**< \brief 计数时钟选择（默认内部） */
    __zlg118_plfm_tim0_timing_init,    /**< \brief 平台初始化函数 */
    __zlg118_plfm_tim0_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief TIM0 设备定义 */
am_zlg118_tim_timing_dev_t __g_tim0_timing_dev;

/** \brief 定时器TIM0 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_zlg118_tim0_timing_inst_init (void)
{
    return am_zlg118_tim_timing_init(&__g_tim0_timing_dev,
                                     &__g_tim0_timing_devinfo);
}

/** \brief TIM0 Timing实例解初始化 */
void am_zlg118_tim0_timing_inst_deinit (am_timer_handle_t handle)
{
    am_zlg118_tim_timing_deinit(handle);
}

/*******************************************************************************
  TIM1 配置
*******************************************************************************/

/** \brief TIM1 平台初始化 */
void __zlg118_plfm_tim1_timing_init (void)
{
//    /* 配置GATE引脚，以实现门控功能 */
//    am_gpio_pin_cfg(PIOB_13, PIOB_13_INPUT_PD | PIOB_13_TIM1_GATE);
//
//    /* 配置ETR引脚，以外部时钟作为计数时钟功能 */
//    am_gpio_pin_cfg(PIOC_9, PIOC_9_INPUT_PD | PIOC_9_TIM1_ETR);

    am_clk_enable(CLK_TIM012);
}

/** \brief 解除 TIM1 平台初始化 */
void __zlg118_plfm_tim1_timing_deinit (void)
{
//    am_gpio_pin_cfg(PIOB_13, PIOB_13_INPUT_PU | PIOB_13_GPIO);
//
//    am_gpio_pin_cfg(PIOC_9, PIOC_9_INPUT_PU | PIOC_9_GPIO);

    am_clk_disable(CLK_TIM012);
}

/** \brief TIM1 设备信息 */
const am_zlg118_tim_timing_devinfo_t  __g_tim1_timing_devinfo = {
    ZLG118_TIM1_BASE,                  /**< \brief TIM1寄存器块的基地址 */
    INUM_TIM1,                         /**< \brief TIM1中断编号 */
    AMHW_ZLG118_TIM_TYPE_TIM1,         /**< \brief 定时器类型 */
    AMHW_ZLG118_TIM_MODE0_COUNTER_16,  /**< \brief 16位重载计数 */
    AMHW_ZLG118_TIM_GATE_DISABLE,      /**< \brief 门控状态（默认关闭） */
    AM_TRUE,                           /**< \brief 门控信号为真（1）有效 */
    AMHW_ZLG118_TIM_CLK_SRC_TCLK,      /**< \brief 计数时钟选择（默认内部） */
    __zlg118_plfm_tim1_timing_init,    /**< \brief 平台初始化函数 */
    __zlg118_plfm_tim1_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief TIM1 设备定义 */
am_zlg118_tim_timing_dev_t __g_tim1_timing_dev;

/** \brief 定时器TIM1 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_zlg118_tim1_timing_inst_init (void)
{
    return am_zlg118_tim_timing_init(&__g_tim1_timing_dev,
                                     &__g_tim1_timing_devinfo);
}

/** \brief TIM1 Timing实例解初始化 */
void am_zlg118_tim1_timing_inst_deinit (am_timer_handle_t handle)
{
    am_zlg118_tim_timing_deinit(handle);
}

/*******************************************************************************
  TIM2 配置
*******************************************************************************/

/** \brief TIM2 平台初始化 */
void __zlg118_plfm_tim2_timing_init (void)
{
//    /* 配置GATE引脚，以实现门控功能 */
//    am_gpio_pin_cfg(PIOA_10, PIOA_10_INPUT_PD | PIOA_10_TIM2_GATE);
//
//    /* 配置ETR引脚，以外部时钟作为计数时钟功能 */
//    am_gpio_pin_cfg(PIOC_4, PIOC_4_INPUT_PD | PIOC_4_TIM2_ETR);

    am_clk_enable(CLK_TIM012);
}

/** \brief 解除 TIM2 平台初始化 */
void __zlg118_plfm_tim2_timing_deinit (void)
{
//    am_gpio_pin_cfg(PIOA_10, PIOA_10_INPUT_PU | PIOA_10_GPIO);
//
//    am_gpio_pin_cfg(PIOC_4, PIOC_4_INPUT_PU | PIOC_4_GPIO);

    am_clk_disable(CLK_TIM012);
}

/** \brief TIM2 设备信息 */
const am_zlg118_tim_timing_devinfo_t  __g_tim2_timing_devinfo = {
    ZLG118_TIM2_BASE,                  /**< \brief TIM2寄存器块的基地址 */
    INUM_TIM2,                         /**< \brief TIM2中断编号 */
    AMHW_ZLG118_TIM_TYPE_TIM2,         /**< \brief 定时器类型 */
    AMHW_ZLG118_TIM_MODE0_COUNTER_16,  /**< \brief 16位重载计数 */
    AMHW_ZLG118_TIM_GATE_DISABLE,      /**< \brief 门控状态（默认关闭） */
    AM_TRUE,                           /**< \brief 门控信号为真（1）有效 */
    AMHW_ZLG118_TIM_CLK_SRC_TCLK,      /**< \brief 计数时钟选择（默认内部） */
    __zlg118_plfm_tim2_timing_init,    /**< \brief 平台初始化函数 */
    __zlg118_plfm_tim2_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief TIM2 设备定义 */
am_zlg118_tim_timing_dev_t __g_tim2_timing_dev;

/** \brief 定时器TIM2 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_zlg118_tim2_timing_inst_init (void)
{
    return am_zlg118_tim_timing_init(&__g_tim2_timing_dev,
                                     &__g_tim2_timing_devinfo);
}

/** \brief TIM2 Timing实例解初始化 */
void am_zlg118_tim2_timing_inst_deinit (am_timer_handle_t handle)
{
    am_zlg118_tim_timing_deinit(handle);
}

/*******************************************************************************
  TIM3 配置
*******************************************************************************/

/** \brief TIM3 平台初始化 */
void __zlg118_plfm_tim3_timing_init (void)
{
    /* 配置GATE引脚，以实现门控功能 */
    am_gpio_pin_cfg(PIOA_6, PIOA_6_INPUT_PD | PIOA_6_TIM3_GATE);

    /* 配置ETR引脚，以外部时钟作为计数时钟功能 */
    am_gpio_pin_cfg(PIOA_11, PIOA_11_INPUT_PD | PIOA_11_TIM3_GATE);

    am_clk_enable(CLK_TIM3);
}

/** \brief 解除 TIM3 平台初始化 */
void __zlg118_plfm_tim3_timing_deinit (void)
{
    am_gpio_pin_cfg(PIOA_6, PIOA_6_INPUT_PU | PIOA_6_GPIO);

    am_gpio_pin_cfg(PIOA_11, PIOA_11_INPUT_PU | PIOA_11_GPIO);

    am_clk_disable(CLK_TIM3);
}

/** \brief TIM3 设备信息 */
const am_zlg118_tim_timing_devinfo_t  __g_tim3_timing_devinfo = {
    ZLG118_TIM3_BASE,                  /**< \brief TIM3寄存器块的基地址 */
    INUM_TIM3,                         /**< \brief TIM3中断编号 */
    AMHW_ZLG118_TIM_TYPE_TIM3,         /**< \brief 定时器类型 */
    AMHW_ZLG118_TIM_MODE0_COUNTER_16,  /**< \brief 16位重载计数 */
    AMHW_ZLG118_TIM_GATE_DISABLE,      /**< \brief 门控状态（默认关闭） */
    AM_TRUE,                           /**< \brief 门控信号为真（1）有效 */
    AMHW_ZLG118_TIM_CLK_SRC_TCLK,      /**< \brief 计数时钟选择（默认内部） */
    __zlg118_plfm_tim3_timing_init,    /**< \brief 平台初始化函数 */
    __zlg118_plfm_tim3_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief TIM3 设备定义 */
am_zlg118_tim_timing_dev_t __g_tim3_timing_dev;

/** \brief 定时器TIM3 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_zlg118_tim3_timing_inst_init (void)
{
    return am_zlg118_tim_timing_init(&__g_tim3_timing_dev,
                                     &__g_tim3_timing_devinfo);
}

/** \brief TIM3 Timing实例解初始化 */
void am_zlg118_tim3_timing_inst_deinit (am_timer_handle_t handle)
{
    am_zlg118_tim_timing_deinit(handle);
}

/*******************************************************************************
  ADTIM4 配置
*******************************************************************************/

/** \brief ADTIM4 平台初始化 */
void __zlg118_plfm_adtim4_timing_init (void)
{
    am_clk_enable(CLK_TIM456);
}

/** \brief 解除 ADTIM4 平台初始化 */
void __zlg118_plfm_adtim4_timing_deinit (void)
{
    am_clk_disable(CLK_TIM456);
}

/** \brief ADTIM4 设备信息 */
const am_zlg118_adtim_timing_devinfo_t  __g_adtim4_timing_devinfo = {
    ZLG118_TIM4_BASE,                    /**< \brief ADTIM4寄存器块的基地址 */
    INUM_TIM4,                           /**< \brief ADTIM4中断编号 */
    __zlg118_plfm_adtim4_timing_init,    /**< \brief 平台初始化函数 */
    __zlg118_plfm_adtim4_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief ADTIM4 设备定义 */
am_zlg118_adtim_timing_dev_t __g_adtim4_timing_dev;

/** \brief 定时器ADTIM4 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_zlg118_tim4_timing_inst_init (void)
{
    return am_zlg118_adtim_timing_init(&__g_adtim4_timing_dev,
                                       &__g_adtim4_timing_devinfo);
}

/** \brief ADTIM4 Timing实例解初始化 */
void am_zlg118_tim4_timing_inst_deinit (am_timer_handle_t handle)
{
    am_zlg118_adtim_timing_deinit(handle);
}

/*******************************************************************************
  ADTIM5 配置
*******************************************************************************/
/** \brief ADTIM5 平台初始化 */
void __zlg118_plfm_adtim5_timing_init (void)
{
    am_clk_enable(CLK_TIM456);
}

/** \brief 解除 ADTIM5 平台初始化 */
void __zlg118_plfm_adtim5_timing_deinit (void)
{
    am_clk_disable(CLK_TIM456);
}

/** \brief ADTIM5 设备信息 */
const am_zlg118_adtim_timing_devinfo_t  __g_adtim5_timing_devinfo = {
    ZLG118_TIM5_BASE,                    /**< \brief ADTIM5寄存器块的基地址 */
    INUM_TIM5,                           /**< \brief ADTIM5中断编号 */
    __zlg118_plfm_adtim5_timing_init,    /**< \brief 平台初始化函数 */
    __zlg118_plfm_adtim5_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief ADTIM5 设备定义 */
am_zlg118_adtim_timing_dev_t __g_adtim5_timing_dev;

/** \brief 定时器ADTIM5 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_zlg118_tim5_timing_inst_init (void)
{
    return am_zlg118_adtim_timing_init(&__g_adtim5_timing_dev,
                                       &__g_adtim5_timing_devinfo);
}

/** \brief ADTIM5 Timing实例解初始化 */
void am_zlg118_tim5_timing_inst_deinit (am_timer_handle_t handle)
{
    am_zlg118_adtim_timing_deinit(handle);
}

/*******************************************************************************
  ADTIM6 配置
*******************************************************************************/

/** \brief ADTIM6 平台初始化 */
void __zlg118_plfm_adtim6_timing_init (void)
{
    am_clk_enable(CLK_TIM456);
}

/** \brief 解除 ADTIM6 平台初始化 */
void __zlg118_plfm_adtim6_timing_deinit (void)
{
    am_clk_disable(CLK_TIM456);
}

/** \brief ADTIM6 设备信息 */
const am_zlg118_adtim_timing_devinfo_t  __g_adtim6_timing_devinfo = {
    ZLG118_TIM6_BASE,                    /**< \brief ADTIM6寄存器块的基地址 */
    INUM_TIM6,                           /**< \brief ADTIM6中断编号 */
    __zlg118_plfm_adtim6_timing_init,    /**< \brief 平台初始化函数 */
    __zlg118_plfm_adtim6_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief ADTIM6 设备定义 */
am_zlg118_adtim_timing_dev_t __g_adtim6_timing_dev;

/** \brief 定时器ADTIM6 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_zlg118_tim6_timing_inst_init (void)
{
    return am_zlg118_adtim_timing_init(&__g_adtim6_timing_dev,
                                       &__g_adtim6_timing_devinfo);
}

/** \brief ADTIM6 Timing实例解初始化 */
void am_zlg118_tim6_timing_inst_deinit (am_timer_handle_t handle)
{
    am_zlg118_adtim_timing_deinit(handle);
}

/**
  @}
 */

/* end of file */
