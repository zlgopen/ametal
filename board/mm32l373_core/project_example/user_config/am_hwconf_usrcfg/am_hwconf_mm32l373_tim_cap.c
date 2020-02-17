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
 * \brief TIM 用于捕获功能的用户配置文件
 * \sa am_hwconf_mm32l373_tim_cap.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-28  zcb, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_mm32l373.h"
#include "am_mm32_tim_cap.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_mm32l373_clk.h"

/**
 * \addtogroup am_if_src_hwconf_mm32l373_tim_cap
 * \copydoc am_hwconf_mm32l373_tim_cap.c
 * @{
 */

/*******************************************************************************
  TIM1 配置
*******************************************************************************/

/** \brief TIM1用于捕获功能的平台初始化 */
void __mm32l373_plfm_tim1_cap_init (void)
{
    am_clk_enable(CLK_TIM1);
    am_mm32l373_clk_reset(CLK_TIM1);
}

/** \brief 解除TIM1平台初始化 */
void __mm32l373_plfm_tim1_cap_deinit (void)
{
    am_mm32l373_clk_reset(CLK_TIM1);
    am_clk_disable(CLK_TIM1);
}

/** \brief TIM1用于捕获功能的引脚配置信息列表 */
am_mm32_tim_cap_ioinfo_t __g_tim1_cap_ioinfo_list[] = {
    {PIOA_8,  PIOA_8_TIM1_CH1_REMAP0  | PIOA_8_INPUT_FLOAT,  PIOA_8_GPIO  | PIOA_8_INPUT_FLOAT},   /**< \brief 通道1 */
    {PIOA_9,  PIOA_9_TIM1_CH2_REMAP0  | PIOA_9_INPUT_FLOAT,  PIOA_9_GPIO  | PIOA_9_INPUT_FLOAT},   /**< \brief 通道2 */
    {PIOA_10, PIOA_10_TIM1_CH3_REMAP0 | PIOA_10_INPUT_FLOAT, PIOA_10_GPIO | PIOA_10_INPUT_FLOAT},  /**< \brief 通道3 */
    {PIOA_11, PIOA_11_TIM1_CH4_REMAP0 | PIOA_11_INPUT_FLOAT, PIOA_11_GPIO | PIOA_11_INPUT_FLOAT},  /**< \brief 通道4 */
};

/** \brief TIM1用于捕获功能的设备信息 */
const am_mm32_tim_cap_devinfo_t  __g_tim1_cap_devinfo = {
    MM32L373_TIM1_BASE,                   /**< \brief TIM1寄存器块的基地址 */
    INUM_TIM1_CC,                       /**< \brief TIM1中断编号 */
    CLK_TIM1,                           /**< \brief TIM1时钟ID */
    4,                                  /**< \brief 4个捕获通道 */
    &__g_tim1_cap_ioinfo_list[0],
    AMHW_MM32_TIM_TYPE0,              /**< \brief 定时器类型 */
    __mm32l373_plfm_tim1_cap_init,        /**< \brief 平台初始化函数 */
    __mm32l373_plfm_tim1_cap_deinit       /**< \brief 平台解初始化函数 */
};

/** \brief TIM1用于捕获功能的设备定义 */
am_mm32_tim_cap_dev_t  __g_tim1_cap_dev;

/** \brief tim1 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_mm32l373_tim1_cap_inst_init (void)
{
    return am_mm32_tim_cap_init(&__g_tim1_cap_dev,
                                  &__g_tim1_cap_devinfo);
}

/** \brief tim1 cap实例解初始化 */
void am_mm32l373_tim1_cap_inst_deinit (am_cap_handle_t handle)
{
    am_mm32_tim_cap_deinit(handle);
}

/*******************************************************************************
  TIM2 配置
*******************************************************************************/

/** \brief TIM2用于捕获功能的平台初始化 */
void __mm32l373_plfm_tim2_cap_init (void)
{
    am_clk_enable(CLK_TIM2);
    am_mm32l373_clk_reset(CLK_TIM2);
}

/** \brief 解除TIM2平台初始化 */
void __mm32l373_plfm_tim2_cap_deinit (void)
{
    am_mm32l373_clk_reset(CLK_TIM2);
    am_clk_disable(CLK_TIM2);
}

/** \brief TIM2用于捕获功能的引脚配置信息列表 */
am_mm32_tim_cap_ioinfo_t __g_tim2_cap_ioinfo_list[] = {
    {PIOA_15, PIOA_15_TIM2_CH1_ETR_REMAP1 | PIOA_15_INPUT_FLOAT, PIOA_15_GPIO | PIOA_15_INPUT_FLOAT},  /**< \brief 通道1 */
    {PIOB_3, PIOB_3_TIM2_CH2_REMAP1       | PIOB_3_INPUT_FLOAT, PIOB_3_GPIO   | PIOB_3_INPUT_FLOAT},   /**< \brief 通道2 */
    {PIOB_10, PIOB_10_TIM2_CH3_REMAP2     | PIOB_10_INPUT_FLOAT, PIOB_10_GPIO | PIOB_10_INPUT_FLOAT},  /**< \brief 通道3 */
    {PIOB_11, PIOB_11_TIM2_CH4_REMAP2     | PIOB_11_INPUT_FLOAT, PIOB_11_GPIO | PIOB_11_INPUT_FLOAT},  /**< \brief 通道4 */
};

/** \brief TIM2用于捕获功能的设备信息 */
const am_mm32_tim_cap_devinfo_t  __g_tim2_cap_devinfo = {
    MM32L373_TIM2_BASE,                   /**< \brief TIM2寄存器块的基地址 */
    INUM_TIM2,                          /**< \brief TIM2中断编号 */
    CLK_TIM2,                           /**< \brief TIM2时钟ID */
    4,                                  /**< \brief 4个捕获通道 */
    &__g_tim2_cap_ioinfo_list[0],
    AMHW_MM32_TIM_TYPE1,              /**< \brief 定时器类型 */
    __mm32l373_plfm_tim2_cap_init,        /**< \brief 平台初始化函数 */
    __mm32l373_plfm_tim2_cap_deinit       /**< \brief 平台解初始化函数 */

};

/** \brief TIM2用于捕获功能的设备定义 */
am_mm32_tim_cap_dev_t  __g_tim2_cap_dev;

/** \brief tim2 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_mm32l373_tim2_cap_inst_init (void)
{
    return am_mm32_tim_cap_init(&__g_tim2_cap_dev,
                                  &__g_tim2_cap_devinfo);
}

/** \brief tim2 cap实例解初始化 */
void am_mm32l373_tim2_cap_inst_deinit (am_cap_handle_t handle)
{
    am_mm32_tim_cap_deinit(handle);
}

/*******************************************************************************
  TIM3 配置
*******************************************************************************/

/** \brief TIM3用于捕获功能的平台初始化 */
void __mm32l373_plfm_tim3_cap_init (void)
{
    am_clk_enable(CLK_TIM3);
    am_mm32l373_clk_reset(CLK_TIM3);
}

/** \brief 解除TIM3平台初始化 */
void __mm32l373_plfm_tim3_cap_deinit (void)
{
    am_mm32l373_clk_reset(CLK_TIM3);
    am_clk_disable(CLK_TIM3);
}

/** \brief TIM3用于捕获功能的引脚配置信息列表 */
am_mm32_tim_cap_ioinfo_t __g_tim3_cap_ioinfo_list[] = {
    {PIOA_6, PIOA_6_TIM3_CH1_REMAP0 | PIOA_6_INPUT_FLOAT, PIOA_6_GPIO | PIOA_6_INPUT_FLOAT},  /**< \brief 通道1 */
    {PIOA_7, PIOA_7_TIM3_CH2_REMAP0 | PIOA_7_INPUT_FLOAT, PIOA_7_GPIO | PIOA_7_INPUT_FLOAT},  /**< \brief 通道2 */
    {PIOB_0, PIOB_0_TIM3_CH3_REMAP0 | PIOB_0_INPUT_FLOAT, PIOB_0_GPIO | PIOB_0_INPUT_FLOAT},  /**< \brief 通道3 */
    {PIOB_1, PIOB_1_TIM3_CH4_REMAP0 | PIOB_1_INPUT_FLOAT, PIOB_1_GPIO | PIOB_1_INPUT_FLOAT},  /**< \brief 通道4 */
};

/** \brief TIM3用于捕获功能的设备信息 */
const am_mm32_tim_cap_devinfo_t  __g_tim3_cap_devinfo = {
    MM32L373_TIM3_BASE,                   /**< \brief TIM3寄存器块的基地址 */
    INUM_TIM3,                          /**< \brief TIM3中断编号 */
    CLK_TIM3,                           /**< \brief TIM3时钟ID */
    4,                                  /**< \brief 4个捕获通道 */
    &__g_tim3_cap_ioinfo_list[0],
    AMHW_MM32_TIM_TYPE1,              /**< \brief 定时器类型 */
    __mm32l373_plfm_tim3_cap_init,        /**< \brief 平台初始化函数 */
    __mm32l373_plfm_tim3_cap_deinit       /**< \brief 平台解初始化函数 */

};

/** \brief TIM3用于捕获功能的设备定义 */
am_mm32_tim_cap_dev_t  __g_tim3_cap_dev;

/** \brief tim3 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_mm32l373_tim3_cap_inst_init (void)
{
    return am_mm32_tim_cap_init(&__g_tim3_cap_dev,
                                  &__g_tim3_cap_devinfo);
}

/** \brief tim3 cap实例解初始化 */
void am_mm32l373_tim3_cap_inst_deinit (am_cap_handle_t handle)
{
    am_mm32_tim_cap_deinit(handle);
}

/*******************************************************************************
  TIM4 配置
*******************************************************************************/

/** \brief TIM4用于捕获功能的平台初始化 */
void __mm32l373_plfm_tim4_cap_init (void)
{
    am_clk_enable(CLK_TIM4);
    am_mm32l373_clk_reset(CLK_TIM4);
}

/** \brief 解除TIM4平台初始化 */
void __mm32l373_plfm_tim4_cap_deinit (void)
{
    am_mm32l373_clk_reset(CLK_TIM4);
    am_clk_disable(CLK_TIM4);
}

/** \brief TIM4用于捕获功能的引脚配置信息列表 */
am_mm32_tim_cap_ioinfo_t __g_tim4_cap_ioinfo_list[] = {
    {PIOB_6,  PIOB_6_TIM4_CH1 | PIOB_6_INPUT_FLOAT, PIOB_6_GPIO | PIOB_6_INPUT_FLOAT},  /**< \brief 通道1 */
    {PIOB_7,  PIOB_7_TIM4_CH2 | PIOB_7_INPUT_FLOAT, PIOB_7_GPIO | PIOB_7_INPUT_FLOAT},  /**< \brief 通道2 */
    {PIOB_8,  PIOB_8_TIM4_CH3 | PIOB_8_INPUT_FLOAT, PIOB_8_GPIO | PIOB_8_INPUT_FLOAT},  /**< \brief 通道3 */
    {PIOB_9,  PIOB_9_TIM4_CH4 | PIOB_9_INPUT_FLOAT, PIOB_9_GPIO | PIOB_9_INPUT_FLOAT},  /**< \brief 通道4 */
};

/** \brief TIM4用于捕获功能的设备信息 */
const am_mm32_tim_cap_devinfo_t  __g_tim4_cap_devinfo = {
    MM32L373_TIM4_BASE,                   /**< \brief TIM4寄存器块的基地址 */
    INUM_TIM4,                          /**< \brief TIM4中断编号 */
    CLK_TIM4,                           /**< \brief TIM4时钟ID */
    4,                                  /**< \brief 4个捕获通道 */
    &__g_tim4_cap_ioinfo_list[0],
    AMHW_MM32_TIM_TYPE1,              /**< \brief 定时器类型 */
    __mm32l373_plfm_tim4_cap_init,        /**< \brief 平台初始化函数 */
    __mm32l373_plfm_tim4_cap_deinit       /**< \brief 平台解初始化函数 */

};

/** \brief TIM4用于捕获功能的设备定义 */
am_mm32_tim_cap_dev_t  __g_tim4_cap_dev;

/** \brief tim4 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_mm32l373_tim4_cap_inst_init (void)
{
    return am_mm32_tim_cap_init(&__g_tim4_cap_dev,
                                  &__g_tim4_cap_devinfo);
}

/** \brief tim4 cap实例解初始化 */
void am_mm32l373_tim4_cap_inst_deinit (am_cap_handle_t handle)
{
    am_mm32_tim_cap_deinit(handle);
}

/**
 * @}
 */

/* end of file */
