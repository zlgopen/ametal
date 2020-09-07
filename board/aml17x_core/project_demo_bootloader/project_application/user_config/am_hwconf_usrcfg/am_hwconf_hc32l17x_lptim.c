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
  \brief HC32 LPTIM 定时功能用户配置文件
  \sa am_hwconf_hc32l17x_lptim_timing.c
 *
 *
  \internal
  \par Modification history
  - 1.00 19-09-27  zp, first implementation
  \endinternal
 */

#include "ametal.h"
#include "am_clk.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "am_hc32_clk.h"
#include "am_hc32_lptim_timing.h"

/**
  \addtogroup am_if_src_hwconf_hc32l17x_lptim_timing
  \copydoc am_hwconf_hc32l17x_lptim_timing.c
  @{
 */

/*******************************************************************************
  lptim1 配置
*******************************************************************************/

/** \brief LPTIM0 平台初始化 */
void __hc32l17x_plfm_lptim0_timing_init (void)
{
    /* 配置GATE引脚，以实现门控功能 */
    am_gpio_pin_cfg(PIOB_3, PIOB_3_INPUT_FLOAT | PIOB_3_LPTIM0_GATE);

    /* 配置ETR引脚，以外部时钟作为计数时钟功能 */
    am_gpio_pin_cfg(PIOB_4, PIOB_4_INPUT_FLOAT | PIOB_4_LPTIM0_ETR);

    /* 配置TOG、TOGN引脚，输出相关电平 */
    am_gpio_pin_cfg(PIOC_1, PIOC_1_OUT_PP | PIOC_1_LPTIM0_TOG);
    am_gpio_pin_cfg(PIOC_2, PIOC_2_OUT_PP | PIOC_2_LPTIM0_TOGN);

    am_clk_enable(CLK_LPTIM0);
}

/** \brief 解除 LPTIM0 平台初始化 */
void __hc32l17x_plfm_lptim0_timing_deinit (void)
{
    am_gpio_pin_cfg(PIOB_3, PIOB_3_INPUT_PU | PIOB_3_GPIO);
    am_gpio_pin_cfg(PIOB_4, PIOB_4_INPUT_PU | PIOB_4_GPIO);
    am_gpio_pin_cfg(PIOC_1, PIOC_1_INPUT_PU | PIOC_1_GPIO);
    am_gpio_pin_cfg(PIOC_2, PIOC_2_INPUT_PU | PIOC_2_GPIO);

    am_clk_disable(CLK_LPTIM0);
}

/** \brief LPTIM0 设备信息 */
const am_hc32_lptim_timing_devinfo_t  __g_lptim0_timing_devinfo = {
    HC32_LPTIM0_BASE,                  /**< \brief LPTIM0寄存器块的基地址 */
    INUM_LPTIM0_1,                       /**< \brief LPTIM0中断编号 */
    AMHW_HC32_LPTIM_CLK_SRC_PCLK,      /**< \brief 时钟源选择 */
    AMHW_HC32_LPTIM_FUNCTION_TIMER,    /**< \brief 定时器/计数器功能选择
                                          *
                                          *（计数器功能需要初始化相关ETR引脚）
                                          *
                                          */
    AMHW_HC32_LPTIM_MODE_RELOAD,       /**< \brief 自动重载模式 */
    AM_FALSE,                            /**< \brief 门控状态（默认关闭） */
    AM_TRUE,                             /**< \brief 门控信号为真（1）有效 */
    AM_TRUE,                             /**< \brief TOG、TOGN输出使能
                                          *
                                          *    AM_FALSE ： TOG,TOGN同时输出0
                                          *    AM_TRUE  ： TOG,TOGN输出相位相反的信号
                                          *
                                          */

    __hc32l17x_plfm_lptim0_timing_init,    /**< \brief 平台初始化函数 */
    __hc32l17x_plfm_lptim0_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief LPTIM0 设备定义 */
am_hc32_lptim_timing_dev_t __g_lptim0_timing_dev;

/** \brief 定时器LPTIM0 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_hc32_lptim0_timing_inst_init (void)
{
    return am_hc32_lptim_timing_init(&__g_lptim0_timing_dev,
                                       &__g_lptim0_timing_devinfo);
}

/** \brief LPTIM0 Timing实例解初始化 */
void am_hc32_lptim0_timing_inst_deinit (am_timer_handle_t handle)
{
    am_hc32_lptim_timing_deinit(handle);
}

/*******************************************************************************
  LPTIM1 配置
*******************************************************************************/

/** \brief LPTIM1 平台初始化 */
void __hc32l17x_plfm_lptim1_timing_init (void)
{
    /* 配置GATE引脚，以实现门控功能 */
    am_gpio_pin_cfg(PIOC_6, PIOC_6_INPUT_FLOAT | PIOC_6_LPTIM1_GATE);

    /* 配置ETR引脚，以外部时钟作为计数时钟功能 */
    am_gpio_pin_cfg(PIOD_6, PIOD_6_INPUT_FLOAT | PIOD_6_LPTIM1_ETR);

    /* 配置TOG、TOGN引脚，输出相关电平 */
    am_gpio_pin_cfg(PIOD_3, PIOD_3_OUT_PP | PIOD_3_LPTIM1_TOG);
    am_gpio_pin_cfg(PIOD_4, PIOD_4_OUT_PP | PIOD_4_LPTIM1_TOGN);

    am_clk_enable(CLK_LPTIM1);
}

/** \brief 解除 LPTIM1 平台初始化 */
void __hc32l17x_plfm_lptim1_timing_deinit (void)
{
    am_gpio_pin_cfg(PIOC_6, PIOC_6_INPUT_PU | PIOC_6_GPIO);
    am_gpio_pin_cfg(PIOD_6, PIOD_6_INPUT_PU | PIOD_6_GPIO);
    am_gpio_pin_cfg(PIOD_3, PIOD_3_INPUT_PU | PIOD_3_GPIO);
    am_gpio_pin_cfg(PIOD_4, PIOD_4_INPUT_PU | PIOD_4_GPIO);

    am_clk_disable(CLK_LPTIM1);
}

/** \brief LPTIM1 设备信息 */
const am_hc32_lptim_timing_devinfo_t  __g_lptim1_timing_devinfo = {
    HC32_LPTIM1_BASE,                  /**< \brief LPTIM1寄存器块的基地址 */
    INUM_LPTIM0_1,                       /**< \brief LPTIM1中断编号 */
    AMHW_HC32_LPTIM_CLK_SRC_PCLK,      /**< \brief 时钟源选择 */
    AMHW_HC32_LPTIM_FUNCTION_COUNTER,  /**< \brief 定时器/计数器功能选择
                                          *
                                          *（计数器功能需要初始化相关ETR引脚）
                                          *
                                          */
    AMHW_HC32_LPTIM_MODE_RELOAD,       /**< \brief 自动重载模式 */
    AM_FALSE,                            /**< \brief 门控状态（默认关闭，开启需要初始化GATE引脚） */
    AM_TRUE,                             /**< \brief 门控信号为真（1）有效 */
    AM_TRUE,                             /**< \brief TOG、TOGN输出使能
                                          *
                                          *    AM_FALSE ： TOG,TOGN同时输出0
                                          *    AM_TRUE  ： TOG,TOGN输出相位相反的信号
                                          *
                                          *    （默认关闭，开启需要初始化TOG、TOGN引脚）
                                          *
                                          */

    __hc32l17x_plfm_lptim1_timing_init,    /**< \brief 平台初始化函数 */
    __hc32l17x_plfm_lptim1_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief LPTIM1 设备定义 */
am_hc32_lptim_timing_dev_t __g_lptim1_timing_dev;

/** \brief 定时器LPTIM1 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_hc32_lptim1_timing_inst_init (void)
{
    return am_hc32_lptim_timing_init(&__g_lptim1_timing_dev,
                                       &__g_lptim1_timing_devinfo);
}

/** \brief LPTIM1 Timing实例解初始化 */
void am_hc32_lptim1_timing_inst_deinit (am_timer_handle_t handle)
{
    am_hc32_lptim_timing_deinit(handle);
}

/**
  @}
 */

/* end of file */
