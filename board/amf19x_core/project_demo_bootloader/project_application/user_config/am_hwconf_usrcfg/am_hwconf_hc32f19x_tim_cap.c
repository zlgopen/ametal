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
 * \sa am_hwconf_hc32f19x_tim_cap.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-12  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32.h"
#include "am_hc32_tim_cap.h"
#include "am_hc32_adtim_cap.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_hc32_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f19x_tim_cap
 * \copydoc am_hwconf_hc32f19x_tim_cap.c
 * @{
 */
/*******************************************************************************
  TIM0 配置
*******************************************************************************/

/** \brief TIM0用于捕获功能的平台初始化 */
void __hc32f19x_plfm_tim0_cap_init (void)
{
    am_clk_enable(CLK_TIM012);
}

/** \brief 解除TIM0平台初始化 */
void __hc32f19x_plfm_tim0_cap_deinit (void)
{
    am_clk_disable(CLK_TIM012);
}

/** \brief TIM0用于捕获功能的引脚配置信息列表 */
am_hc32_tim_cap_ioinfo_t __g_tim0_cap_ioinfo_list[] = {
    /**< \brief 通道0 */
    {
        AM_HC32_TIM_CAP_CH0A,
        PIOA_0,
        PIOA_0_TIM0_CHA  | PIOA_0_INPUT_FLOAT,
        PIOA_0_GPIO  | PIOA_0_INPUT_PU
    },

    /**< \brief 通道1 */
    {
        AM_HC32_TIM_CAP_CH0B,
        PIOA_1,
        PIOA_1_TIM0_CHB  | PIOA_1_INPUT_FLOAT,
        PIOA_1_GPIO  | PIOA_1_INPUT_PU
    },
};

/** \brief TIM0用于捕获功能的设备信息 */
const am_hc32_tim_cap_devinfo_t  __g_tim0_cap_devinfo = {
    HC32_TIM0_BASE,                   /**< \brief TIM0寄存器块的基地址 */
    INUM_TIM0,                          /**< \brief TIM0中断编号 */
    AMHW_HC32_TIM_CLK_DIV1,           /**< \brief 时钟分频系数 */
    2,                                  /**< \brief 2个捕获通道 */
    AMHW_HC32_TIM_TYPE_TIM0,          /**< \brief 定时器选择 */

    &__g_tim0_cap_ioinfo_list[0],
    __hc32f19x_plfm_tim0_cap_init,        /**< \brief 平台初始化函数 */
    __hc32f19x_plfm_tim0_cap_deinit       /**< \brief 平台解初始化函数 */
};

/** \brief TIM0用于捕获功能的设备定义 */
am_hc32_tim_cap_dev_t  __g_tim0_cap_dev;

/** \brief tim0 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32_tim0_cap_inst_init (void)
{
    return am_hc32_tim_cap_init(&__g_tim0_cap_dev,
                                  &__g_tim0_cap_devinfo);
}

/** \brief tim0 cap实例解初始化 */
void am_hc32_tim0_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32_tim_cap_deinit(handle);
}

/*******************************************************************************
  TIM1 配置
*******************************************************************************/

/** \brief TIM1用于捕获功能的平台初始化 */
void __hc32f19x_plfm_tim1_cap_init (void)
{
    am_clk_enable(CLK_TIM012);
}

/** \brief 解除TIM1平台初始化 */
void __hc32f19x_plfm_tim1_cap_deinit (void)
{
    am_clk_disable(CLK_TIM012);
}

/** \brief TIM1用于捕获功能的引脚配置信息列表 */
am_hc32_tim_cap_ioinfo_t __g_tim1_cap_ioinfo_list[] = {
    /**< \brief 通道0 */
    {
        AM_HC32_TIM_CAP_CH0A,
        PIOA_0,
        PIOA_0_TIM1_CHA  | PIOA_0_INPUT_FLOAT,
        PIOA_0_GPIO  | PIOA_0_INPUT_PU
    },

    /**< \brief 通道1 */
    {
        AM_HC32_TIM_CAP_CH0B,
        PIOA_1,
        PIOA_1_TIM1_CHB  | PIOA_1_INPUT_FLOAT,
        PIOA_1_GPIO  | PIOA_1_INPUT_PU
    },
};

/** \brief TIM1用于捕获功能的设备信息 */
const am_hc32_tim_cap_devinfo_t  __g_tim1_cap_devinfo = {
    HC32_TIM1_BASE,                   /**< \brief TIM1寄存器块的基地址 */
    INUM_TIM1,                          /**< \brief TIM1中断编号 */
    AMHW_HC32_TIM_CLK_DIV1,           /**< \brief 时钟分频系数 */
    2,                                  /**< \brief 2个捕获通道 */
    AMHW_HC32_TIM_TYPE_TIM1,          /**< \brief 定时器选择 */

    &__g_tim1_cap_ioinfo_list[0],
    __hc32f19x_plfm_tim1_cap_init,        /**< \brief 平台初始化函数 */
    __hc32f19x_plfm_tim1_cap_deinit       /**< \brief 平台解初始化函数 */
};

/** \brief TIM1用于捕获功能的设备定义 */
am_hc32_tim_cap_dev_t  __g_tim1_cap_dev;

/** \brief tim1 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32_tim1_cap_inst_init (void)
{
    return am_hc32_tim_cap_init(&__g_tim1_cap_dev,
                                  &__g_tim1_cap_devinfo);
}

/** \brief tim1 cap实例解初始化 */
void am_hc32_tim1_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32_tim_cap_deinit(handle);
}

/*******************************************************************************
  TIM2 配置
*******************************************************************************/

/** \brief TIM2用于捕获功能的平台初始化 */
void __hc32f19x_plfm_tim2_cap_init (void)
{
    am_clk_enable(CLK_TIM012);
}

/** \brief 解除TIM2平台初始化 */
void __hc32f19x_plfm_tim2_cap_deinit (void)
{
    am_clk_disable(CLK_TIM012);
}

/** \brief TIM2用于捕获功能的引脚配置信息列表 */
am_hc32_tim_cap_ioinfo_t __g_tim2_cap_ioinfo_list[] = {
    /**< \brief 通道0 */
    {
        AM_HC32_TIM_CAP_CH0A,
        PIOA_2,
        PIOA_2_TIM2_CHA  | PIOA_2_INPUT_FLOAT,
        PIOA_2_GPIO  | PIOA_2_INPUT_PU
    },

    /**< \brief 通道1 */
    {
        AM_HC32_TIM_CAP_CH0B,
        PIOA_3,
        PIOA_3_TIM2_CHB  | PIOA_3_INPUT_FLOAT,
        PIOA_3_GPIO  | PIOA_3_INPUT_PU
    },
};

/** \brief TIM2用于捕获功能的设备信息 */
const am_hc32_tim_cap_devinfo_t  __g_tim2_cap_devinfo = {
    HC32_TIM2_BASE,                   /**< \brief TIM2寄存器块的基地址 */
    INUM_TIM2,                          /**< \brief TIM2中断编号 */
    AMHW_HC32_TIM_CLK_DIV1,           /**< \brief 时钟分频系数 */
    2,                                  /**< \brief 2个捕获通道 */
    AMHW_HC32_TIM_TYPE_TIM2,          /**< \brief 定时器选择 */

    &__g_tim2_cap_ioinfo_list[0],
    __hc32f19x_plfm_tim2_cap_init,        /**< \brief 平台初始化函数 */
    __hc32f19x_plfm_tim2_cap_deinit       /**< \brief 平台解初始化函数 */
};

/** \brief TIM2用于捕获功能的设备定义 */
am_hc32_tim_cap_dev_t  __g_tim2_cap_dev;

/** \brief tim2 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32_tim2_cap_inst_init (void)
{
    return am_hc32_tim_cap_init(&__g_tim2_cap_dev,
                                  &__g_tim2_cap_devinfo);
}

/** \brief tim2 cap实例解初始化 */
void am_hc32_tim2_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32_tim_cap_deinit(handle);
}

/*******************************************************************************
  TIM3 配置
*******************************************************************************/

/** \brief TIM3用于捕获功能的平台初始化 */
void __hc32f19x_plfm_tim3_cap_init (void)
{
    am_clk_enable(CLK_TIM3);
}

/** \brief 解除TIM3平台初始化 */
void __hc32f19x_plfm_tim3_cap_deinit (void)
{
    am_clk_disable(CLK_TIM3);
}

/** \brief TIM3用于捕获功能的引脚配置信息列表 */
am_hc32_tim_cap_ioinfo_t __g_tim3_cap_ioinfo_list[] = {

    /**< \brief 通道0 */
    {
        AM_HC32_TIM_CAP_CH0A,
        PIOB_3,
        PIOB_3_TIM3_CH0A  | PIOB_3_INPUT_FLOAT ,
        PIOB_3_GPIO  | PIOB_3_INPUT_PU
    },
    /**< \brief 通道1 */
    {
        AM_HC32_TIM_CAP_CH0B,
        PIOB_4,
        PIOB_4_TIM3_CH0B  | PIOB_4_INPUT_FLOAT ,
        PIOB_4_GPIO  | PIOB_4_INPUT_PU
    },
    /**< \brief 通道2 */
    {
        AM_HC32_TIM_CAP_CH1A,
        PIOB_10,
        PIOB_10_TIM3_CH1A | PIOB_10_INPUT_FLOAT,
        PIOB_10_GPIO | PIOB_10_INPUT_PU
    },
    /**< \brief 通道3 */
    {
        AM_HC32_TIM_CAP_CH1B,
        PIOB_14,
        PIOB_14_TIM3_CH1B | PIOB_14_INPUT_FLOAT,
        PIOB_14_GPIO | PIOB_14_INPUT_PU
    },
    /**< \brief 通道4 */
    {
        AM_HC32_TIM_CAP_CH2A,
        PIOE_13,
        PIOE_13_TIM3_CH2A | PIOE_13_INPUT_FLOAT,
        PIOE_13_GPIO | PIOE_13_INPUT_PU
    },
    /**< \brief 通道5 */
    {
        AM_HC32_TIM_CAP_CH2B,
        PIOE_12,
        PIOE_12_TIM3_CH2B | PIOE_12_INPUT_FLOAT,
        PIOE_12_GPIO | PIOE_12_INPUT_PU
    },
};

/** \brief TIM3用于捕获功能的设备信息 */
const am_hc32_tim_cap_devinfo_t  __g_tim3_cap_devinfo = {
    HC32_TIM3_BASE,                   /**< \brief TIM3寄存器块的基地址 */
    INUM_TIM3,                          /**< \brief TIM3中断编号 */
    AMHW_HC32_TIM_CLK_DIV1,           /**< \brief 时钟分频系数 */
    6,                                  /**< \brief 6个捕获通道 */
    AMHW_HC32_TIM_TYPE_TIM3,          /**< \brief 定时器选择 */

    &__g_tim3_cap_ioinfo_list[0],
    __hc32f19x_plfm_tim3_cap_init,        /**< \brief 平台初始化函数 */
    __hc32f19x_plfm_tim3_cap_deinit       /**< \brief 平台解初始化函数 */
};

/** \brief TIM3用于捕获功能的设备定义 */
am_hc32_tim_cap_dev_t  __g_tim3_cap_dev;

/** \brief tim3 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32_tim3_cap_inst_init (void)
{
    return am_hc32_tim_cap_init(&__g_tim3_cap_dev,
                                  &__g_tim3_cap_devinfo);
}

/** \brief tim3 cap实例解初始化 */
void am_hc32_tim3_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32_tim_cap_deinit(handle);
}

/*******************************************************************************
  ADTIM4 配置
*******************************************************************************/
/** \brief ADTIM4用于捕获功能的平台初始化 */
void __hc32f19x_plfm_adtim4_cap_init (void)
{
    am_clk_enable(CLK_TIM456);
}

/** \brief 解除ADTIM4平台初始化 */
void __hc32f19x_plfm_adtim4_cap_deinit (void)
{
    am_clk_disable(CLK_TIM456);
}

/** \brief ADTIM4用于捕获功能的引脚配置信息列表 */
am_hc32_adtim_cap_ioinfo_t __g_adtim4_cap_ioinfo_list[] = {
    {
        AM_HC32_ADTIM_CAP_CHA,
        PIOB_2,
        PIOB_2_TIM4_CHA  | PIOB_2_INPUT_FLOAT ,
        PIOB_2_GPIO  | PIOB_2_INPUT_PU
    },

    {
        AM_HC32_ADTIM_CAP_CHB,
        PIOA_7,
        PIOA_7_TIM4_CHB  | PIOA_7_INPUT_FLOAT ,
        PIOA_7_GPIO  | PIOA_7_INPUT_PU
    },
};

/** \brief ADTIM4用于捕获功能的设备信息 */
const am_hc32_adtim_cap_devinfo_t  __g_adtim4_cap_devinfo = {
    HC32_TIM4_BASE,                   /**< \brief ADTIM4寄存器块的基地址 */
    INUM_TIM4,                          /**< \brief ADTIM4中断编号 */
    AMHW_HC32_ADTIM_PCLK_DIV16,       /**< \brief 时钟分频系数 */
    2,                                  /**< \brief 2个捕获通道 */

    &__g_adtim4_cap_ioinfo_list[0],     /**< \brief 引脚配置信息列表 */
    __hc32f19x_plfm_adtim4_cap_init,      /**< \brief 平台初始化函数 */
    __hc32f19x_plfm_adtim4_cap_deinit     /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM4用于捕获功能的设备定义 */
am_hc32_adtim_cap_dev_t  __g_adtim4_cap_dev;

/** \brief adtim4 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32_tim4_cap_inst_init (void)
{
    return am_hc32_adtim_cap_init(&__g_adtim4_cap_dev,
                                    &__g_adtim4_cap_devinfo);
}

/** \brief adtim4 cap实例解初始化 */
void am_hc32_tim4_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32_adtim_cap_deinit(handle);
}

/*******************************************************************************
  ADTIM5 配置
*******************************************************************************/
/** \brief ADTIM5用于捕获功能的平台初始化 */
void __hc32f19x_plfm_adtim5_cap_init (void)
{
    am_clk_enable(CLK_TIM456);
}

/** \brief 解除ADTIM5平台初始化 */
void __hc32f19x_plfm_adtim5_cap_deinit (void)
{
    am_clk_disable(CLK_TIM456);
}

/** \brief ADTIM5用于捕获功能的引脚配置信息列表 */
am_hc32_adtim_cap_ioinfo_t __g_adtim5_cap_ioinfo_list[] = {
    {
        AM_HC32_ADTIM_CAP_CHA,
        PIOA_3,
        PIOA_3_TIM5_CHA  | PIOA_3_INPUT_FLOAT ,
        PIOA_3_GPIO  | PIOA_3_INPUT_PU
    },

    {
        AM_HC32_ADTIM_CAP_CHB,
        PIOA_5,
        PIOA_5_TIM5_CHB  | PIOA_5_INPUT_FLOAT ,
        PIOA_5_GPIO  | PIOA_5_INPUT_PU
    },
};

/** \brief ADTIM5用于捕获功能的设备信息 */
const am_hc32_adtim_cap_devinfo_t  __g_adtim5_cap_devinfo = {
    HC32_TIM5_BASE,                   /**< \brief ADTIM5寄存器块的基地址 */
    INUM_TIM5,                          /**< \brief ADTIM5中断编号 */
    AMHW_HC32_ADTIM_PCLK_DIV16,        /**< \brief 时钟分频系数 */
    2,                                  /**< \brief 2个捕获通道 */

    &__g_adtim5_cap_ioinfo_list[0],     /**< \brief 引脚配置信息列表 */
    __hc32f19x_plfm_adtim5_cap_init,      /**< \brief 平台初始化函数 */
    __hc32f19x_plfm_adtim5_cap_deinit     /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM5用于捕获功能的设备定义 */
am_hc32_adtim_cap_dev_t  __g_adtim5_cap_dev;

/** \brief adtim5 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32_tim5_cap_inst_init (void)
{
    return am_hc32_adtim_cap_init(&__g_adtim5_cap_dev,
                                    &__g_adtim5_cap_devinfo);
}

/** \brief adtim5 cap实例解初始化 */
void am_hc32_tim5_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32_adtim_cap_deinit(handle);
}

/*******************************************************************************
  ADTIM6 配置
*******************************************************************************/
/** \brief ADTIM6用于捕获功能的平台初始化 */
void __hc32f19x_plfm_adtim6_cap_init (void)
{
    am_clk_enable(CLK_TIM456);
}

/** \brief 解除ADTIM6平台初始化 */
void __hc32f19x_plfm_adtim6_cap_deinit (void)
{
    am_clk_disable(CLK_TIM456);
}

/** \brief ADTIM6用于捕获功能的引脚配置信息列表 */
am_hc32_adtim_cap_ioinfo_t __g_adtim6_cap_ioinfo_list[] = {
    {
        AM_HC32_ADTIM_CAP_CHA,
        PIOB_11,
        PIOB_11_TIM6_CHA | PIOB_11_INPUT_FLOAT ,
        PIOB_11_GPIO | PIOB_11_INPUT_PU
    },
    {
        AM_HC32_ADTIM_CAP_CHB,
        PIOB_1,
        PIOB_1_TIM6_CHB  | PIOB_1_INPUT_FLOAT ,
        PIOB_1_GPIO  | PIOB_1_INPUT_PU
    },
};

/** \brief ADTIM6用于捕获功能的设备信息 */
const am_hc32_adtim_cap_devinfo_t  __g_adtim6_cap_devinfo = {
    HC32_TIM6_BASE,                   /**< \brief ADTIM6寄存器块的基地址 */
    INUM_TIM6,                          /**< \brief ADTIM6中断编号 */
    AMHW_HC32_ADTIM_PCLK_DIV16,       /**< \brief 时钟分频系数 */
    2,                                  /**< \brief 2个捕获通道 */

    &__g_adtim6_cap_ioinfo_list[0],     /**< \brief 引脚配置信息列表 */
    __hc32f19x_plfm_adtim6_cap_init,      /**< \brief 平台初始化函数 */
    __hc32f19x_plfm_adtim6_cap_deinit     /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM6用于捕获功能的设备定义 */
am_hc32_adtim_cap_dev_t  __g_adtim6_cap_dev;

/** \brief adtim6 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32_tim6_cap_inst_init (void)
{
    return am_hc32_adtim_cap_init(&__g_adtim6_cap_dev,
                                    &__g_adtim6_cap_devinfo);
}

/** \brief adtim6 cap实例解初始化 */
void am_hc32_tim6_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32_adtim_cap_deinit(handle);
}

/**
 * @}
 */

/* end of file */
