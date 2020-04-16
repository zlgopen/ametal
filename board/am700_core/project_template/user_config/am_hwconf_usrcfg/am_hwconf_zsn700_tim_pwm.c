/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zsn700.cn/
*******************************************************************************/

/**
 * \file
 * \brief TIM用于PWM输出 用户配置文件
 * \sa am_hwconf_zsn700_tim_pwm.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-16  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_zsn700.h"
#include "am_gpio.h"
#include "am_zsn700_tim_pwm.h"
#include "am_zsn700_adtim_pwm.h"
#include "am_clk.h"
#include "am_zsn700_clk.h"

/**
 * \addtogroup am_if_src_hwconf_zsn700_tim_pwm
 * \copydoc am_hwconf_zsn700_tim_pwm.c
 * @{
 */
/*******************************************************************************
  TIM0 配置
*******************************************************************************/

/** \brief TIM0用于PWM输出 平台初始化 */
void __zsn700_plfm_tim0_pwm_init (void)
{
    am_clk_enable(CLK_TIM012);
}

/** \brief 解除TIM0平台初始化 */
void __zsn700_plfm_tim0_pwm_deinit (void)
{
    am_clk_disable(CLK_TIM012);
}

/** \brief TIM0用于PWM功能的引脚配置信息列表 */
am_zsn700_tim_pwm_chaninfo_t __g_tim0_pwm_chaninfo_list[] = {

    /** \brief 通道0引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH0A,
        PIOA_2,
        PIOA_2_TIM0_CHA | PIOA_2_OUT_PP,
        PIOA_2_GPIO  | PIOA_2_INPUT_PU
    },

    /** \brief 通道1引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH0B,
        PIOB_3,
        PIOB_3_TIM0_CHB | PIOB_3_OUT_PP,
        PIOB_3_GPIO  | PIOB_3_INPUT_PU
    },
};

/** \brief TIM0用于PWM设备信息 */
const am_zsn700_tim_pwm_devinfo_t  __g_tim0_pwm_devinfo = {
    ZSN700_TIM0_BASE,                           /**< \brief TIM0寄存器块的基地址 */
    AM_NELEMENTS(__g_tim0_pwm_chaninfo_list),   /**< \brief 配置输出通道个数 */
    0,                                          /**< \brief 互补PWM选择
                                                 *          1：互补PWM
                                                 *          0：独立PWM
                                                 */
    AM_ZSN700_TIM_PWM_OCPOLARITY_HIGH,          /**< \brief 脉宽极性 */
    &__g_tim0_pwm_chaninfo_list[0],             /**< \brief 通道配置信息列表        */
    AMHW_ZSN700_TIM_TYPE_TIM0,                  /**< \brief 定时器类型 */
    __zsn700_plfm_tim0_pwm_init,                /**< \brief 平台初始化函数 */
    __zsn700_plfm_tim0_pwm_deinit               /**< \brief 平台解初始化函数 */
};

/** \brief TIM0用于PWM设备定义 */
am_zsn700_tim_pwm_dev_t __g_tim0_pwm_dev;

/** \brief tim0 PWM 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_zsn700_tim0_pwm_inst_init (void)
{
    return am_zsn700_tim_pwm_init(&__g_tim0_pwm_dev,
                               &__g_tim0_pwm_devinfo);
}

/** \brief tim0 PWM 实例解初始化 */
void am_zsn700_tim0_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_zsn700_tim_pwm_deinit(handle);
}

/*******************************************************************************
  TIM1 配置
*******************************************************************************/

/** \brief TIM1用于PWM输出 平台初始化 */
void __zsn700_plfm_tim1_pwm_init (void)
{
    am_clk_enable(CLK_TIM012);
}

/** \brief 解除TIM1平台初始化 */
void __zsn700_plfm_tim1_pwm_deinit (void)
{
    am_clk_disable(CLK_TIM012);
}

/** \brief TIM1用于PWM功能的引脚配置信息列表 */
am_zsn700_tim_pwm_chaninfo_t __g_tim1_pwm_chaninfo_list[] = {

    /** \brief 通道0引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH0A,
        PIOB_10,
        PIOB_10_TIM1_CHA | PIOB_10_OUT_PP,
        PIOB_10_GPIO  | PIOB_10_INPUT_PU
    },

    /** \brief 通道1引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH0B,
        PIOA_1,
        PIOA_1_TIM1_CHB | PIOA_1_OUT_PP,
        PIOA_1_GPIO  | PIOA_1_INPUT_PU
    },
};

/** \brief TIM1用于PWM设备信息 */
const am_zsn700_tim_pwm_devinfo_t  __g_tim1_pwm_devinfo = {
    ZSN700_TIM1_BASE,                           /**< \brief TIM1寄存器块的基地址 */
    AM_NELEMENTS(__g_tim1_pwm_chaninfo_list),   /**< \brief 配置输出通道个数 */
    0,                                          /**< \brief 互补PWM选择
                                                 *          1：互补PWM
                                                 *          0：独立PWM
                                                 */
    AM_ZSN700_TIM_PWM_OCPOLARITY_HIGH,          /**< \brief 脉宽极性 */
    &__g_tim1_pwm_chaninfo_list[0],             /**< \brief 通道配置信息列表        */
    AMHW_ZSN700_TIM_TYPE_TIM1,                  /**< \brief 定时器类型 */
    __zsn700_plfm_tim1_pwm_init,                /**< \brief 平台初始化函数 */
    __zsn700_plfm_tim1_pwm_deinit               /**< \brief 平台解初始化函数 */
};

/** \brief TIM1用于PWM设备定义 */
am_zsn700_tim_pwm_dev_t __g_tim1_pwm_dev;

/** \brief tim1 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_zsn700_tim1_pwm_inst_init (void)
{
    return am_zsn700_tim_pwm_init(&__g_tim1_pwm_dev,
                               &__g_tim1_pwm_devinfo);
}

/** \brief tim1 pwm 实例解初始化 */
void am_zsn700_tim1_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_zsn700_tim_pwm_deinit(handle);
}

/*******************************************************************************
  tim2 配置
*******************************************************************************/

/** \brief TIM2用于PWM输出 平台初始化 */
void __zsn700_plfm_tim2_pwm_init (void)
{
    am_clk_enable(CLK_TIM012);
}

/** \brief 解除TIM2平台初始化 */
void __zsn700_plfm_tim2_pwm_deinit (void)
{
    am_clk_disable(CLK_TIM012);
}

/** \brief TIM2用于PWM功能的引脚配置信息列表 */
am_zsn700_tim_pwm_chaninfo_t __g_tim2_pwm_chaninfo_list[] = {

    /** \brief 通道0引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH0A,
        PIOA_2,
        PIOA_2_TIM2_CHA | PIOA_2_OUT_PP,
        PIOA_2_GPIO | PIOA_2_INPUT_PU
    },

    /** \brief 通道1引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH0B,
        PIOB_7,
        PIOB_7_TIM2_CHB | PIOB_7_OUT_PP,
        PIOB_7_GPIO | PIOB_7_INPUT_PU
    },
};

/** \brief TIM2用于PWM设备信息 */
const am_zsn700_tim_pwm_devinfo_t  __g_tim2_pwm_devinfo = {
    ZSN700_TIM2_BASE,                           /**< \brief TIM2寄存器块的基地址 */
    AM_NELEMENTS(__g_tim2_pwm_chaninfo_list),   /**< \brief 配置输出通道个数 */
    0,                                          /**< \brief 互补PWM选择
                                                 *          1：互补PWM
                                                 *          0：独立PWM
                                                 */
    AM_ZSN700_TIM_PWM_OCPOLARITY_HIGH,          /**< \brief 脉宽极性 */
    &__g_tim2_pwm_chaninfo_list[0],             /**< \brief 通道配置信息列表        */
    AMHW_ZSN700_TIM_TYPE_TIM2,                  /**< \brief 定时器类型 */
    __zsn700_plfm_tim2_pwm_init,                /**< \brief 平台初始化函数 */
    __zsn700_plfm_tim2_pwm_deinit               /**< \brief 平台解初始化函数 */
};

/** \brief TIM2用于PWM设备定义 */
am_zsn700_tim_pwm_dev_t __g_tim2_pwm_dev;

/** \brief tim2 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_zsn700_tim2_pwm_inst_init (void)
{
    return am_zsn700_tim_pwm_init(&__g_tim2_pwm_dev,
                               &__g_tim2_pwm_devinfo);
}

/** \brief tim2 pwm 实例解初始化 */
void am_zsn700_tim2_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_zsn700_tim_pwm_deinit(handle);
}

/*******************************************************************************
  TIM3 配置
*******************************************************************************/

/** \brief TIM3用于PWM输出 平台初始化 */
void __zsn700_plfm_tim3_pwm_init (void)
{
    am_clk_enable(CLK_TIM3);
}

/** \brief 解除TIM3平台初始化 */
void __zsn700_plfm_tim3_pwm_deinit (void)
{
    am_clk_disable(CLK_TIM3);
}

/** \brief TIM3用于PWM功能的引脚配置信息列表 */
am_zsn700_tim_pwm_chaninfo_t __g_tim3_pwm_chaninfo_list[] = {

    /** \brief 通道0引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH0A,
        PIOB_3,
        PIOB_3_TIM3_CH0A | PIOB_3_OUT_PP,
        PIOB_3_GPIO | PIOB_3_INPUT_PU
    },

    /** \brief 通道1引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH0B,
        PIOB_13,
        PIOB_13_TIM3_CH0B | PIOB_13_OUT_PP,
        PIOB_13_GPIO | PIOB_13_INPUT_PU
    },

    /** \brief 通道2引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH1A,
        PIOA_3,
        PIOA_3_TIM3_CH1A | PIOA_3_OUT_PP,
        PIOA_3_GPIO | PIOA_3_INPUT_PU
    },

    /** \brief 通道3引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH1B,
        PIOB_0,
        PIOB_0_TIM3_CH1B | PIOB_0_OUT_PP,
        PIOB_0_GPIO | PIOB_0_INPUT_PU
    },

    /** \brief 通道4引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH2A,
        PIOA_10,
        PIOA_10_TIM3_CH2A | PIOA_10_OUT_PP,
        PIOA_10_GPIO | PIOA_10_INPUT_PU
    },

    /** \brief 通道5引脚配置 */
    {
        AM_ZSN700_TIM_PWM_CH2B,
        PIOA_4,
        PIOA_4_TIM3_CH2B | PIOA_4_OUT_PP,
        PIOA_4_GPIO | PIOA_4_INPUT_PU
    },

};

/** \brief TIM3用于PWM设备信息 */
const am_zsn700_tim_pwm_devinfo_t  __g_tim3_pwm_devinfo = {
    ZSN700_TIM3_BASE,                           /**< \brief TIM3寄存器块的基地址 */
    AM_NELEMENTS(__g_tim3_pwm_chaninfo_list),   /**< \brief 配置输出通道个数 */
    0,                                          /**< \brief 互补PWM选择
                                                 *          1：互补PWM
                                                 *          0：独立PWM
                                                 */
    AM_ZSN700_TIM_PWM_OCPOLARITY_HIGH,          /**< \brief 脉宽极性 */
    &__g_tim3_pwm_chaninfo_list[0],             /**< \brief 通道配置信息列表        */
    AMHW_ZSN700_TIM_TYPE_TIM3,                  /**< \brief 定时器类型 */
    __zsn700_plfm_tim3_pwm_init,                /**< \brief 平台初始化函数 */
    __zsn700_plfm_tim3_pwm_deinit               /**< \brief 平台解初始化函数 */
};

/** \brief TIM3用于PWM设备定义 */
am_zsn700_tim_pwm_dev_t __g_tim3_pwm_dev;

/** \brief tim3 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_zsn700_tim3_pwm_inst_init (void)
{
    return am_zsn700_tim_pwm_init(&__g_tim3_pwm_dev,
                               &__g_tim3_pwm_devinfo);
}

/** \brief tim3 pwm 实例解初始化 */
void am_zsn700_tim3_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_zsn700_tim_pwm_deinit(handle);
}

/*******************************************************************************
  ADTIM4 配置
*******************************************************************************/
/** \brief ADTIM4用于PWM输出 平台初始化 */
void __zsn700_plfm_adtim4_pwm_init (void)
{
    am_clk_enable(CLK_TIM456);
}

/** \brief 解除ADTIM4平台初始化 */
void __zsn700_plfm_adtim4_pwm_deinit (void)
{
    am_clk_disable(CLK_TIM456);
}

/** \brief ADTIM4用于PWM功能的引脚配置信息列表 */
am_zsn700_adtim_pwm_chaninfo_t __g_adtim4_pwm_chaninfo_list[] = {

    {
        AM_ZSN700_ADTIM_PWM_CHA,
        PIOA_8,
        PIOA_8_TIM4_CHA  | PIOA_8_OUT_PP ,
        PIOA_8_GPIO  | PIOA_8_INPUT_PU
    },

    {
        AM_ZSN700_ADTIM_PWM_CHB,
        PIOA_11,
        PIOA_11_TIM4_CHB | PIOA_11_OUT_PP ,
        PIOA_11_GPIO | PIOA_11_INPUT_PU
    },

};

/** \brief ADTIM4用于PWM设备信息 */
const am_zsn700_adtim_pwm_devinfo_t  __g_adtim4_pwm_devinfo = {
    ZSN700_TIM4_BASE,                           /**< \brief ADTIM4寄存器块的基地址 */
    AM_NELEMENTS(__g_adtim4_pwm_chaninfo_list), /**< \brief 配置输出通道个数 */
    0,                                          /**< \brief 互补PWM选择
                                                 *          1：互补PWM
                                                 *          0：独立PWM
                                                 */
    AM_ZSN700_ADTIM_PWM_OCPOLARITY_HIGH,        /**< \brief 脉宽极性 */
    &__g_adtim4_pwm_chaninfo_list[0],           /**< \brief 通道配置信息列表        */
    __zsn700_plfm_adtim4_pwm_init,              /**< \brief 平台初始化函数 */
    __zsn700_plfm_adtim4_pwm_deinit             /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM4用于PWM设备定义 */
am_zsn700_adtim_pwm_dev_t __g_adtim4_pwm_dev;

/** \brief adtim4 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_zsn700_tim4_pwm_inst_init (void)
{
    return am_zsn700_adtim_pwm_init(&__g_adtim4_pwm_dev,
                                    &__g_adtim4_pwm_devinfo);
}

/** \brief adtim4 pwm 实例解初始化 */
void am_zsn700_tim4_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_zsn700_adtim_pwm_deinit(handle);
}

/*******************************************************************************
  ADTIM5 配置
*******************************************************************************/

/** \brief ADTIM5用于PWM输出 平台初始化 */
void __zsn700_plfm_adtim5_pwm_init (void)
{
    am_clk_enable(CLK_TIM456);
}

/** \brief 解除ADTIM5平台初始化 */
void __zsn700_plfm_adtim5_pwm_deinit (void)
{
    am_clk_disable(CLK_TIM456);
}

/** \brief ADTIM5用于PWM功能的引脚配置信息列表 */
am_zsn700_adtim_pwm_chaninfo_t __g_adtim5_pwm_chaninfo_list[] = {

    {
        AM_ZSN700_ADTIM_PWM_CHA,
        PIOA_3,
        PIOA_3_TIM5_CHA | PIOA_3_OUT_PP ,
        PIOA_3_GPIO | PIOA_3_INPUT_PU
    },

    {
        AM_ZSN700_ADTIM_PWM_CHB,
        PIOC_1,
        PIOC_1_TIM5_CHB | PIOC_1_OUT_PP ,
        PIOC_1_GPIO | PIOC_1_INPUT_PU
    },
};

/** \brief ADTIM5用于PWM设备信息 */
const am_zsn700_adtim_pwm_devinfo_t  __g_adtim5_pwm_devinfo = {
    ZSN700_TIM5_BASE,                           /**< \brief ADTIM5寄存器块的基地址 */
    AM_NELEMENTS(__g_adtim5_pwm_chaninfo_list), /**< \brief 配置输出通道个数 */
    0,                                          /**< \brief 互补PWM选择
                                                 *          1：互补PWM
                                                 *          0：独立PWM
                                                 */
    AM_ZSN700_ADTIM_PWM_OCPOLARITY_HIGH,        /**< \brief 脉宽极性 */
    &__g_adtim5_pwm_chaninfo_list[0],           /**< \brief 通道配置信息列表        */
    __zsn700_plfm_adtim5_pwm_init,              /**< \brief 平台初始化函数 */
    __zsn700_plfm_adtim5_pwm_deinit             /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM5用于PWM设备定义 */
am_zsn700_adtim_pwm_dev_t __g_adtim5_pwm_dev;

/** \brief adtim5 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_zsn700_tim5_pwm_inst_init (void)
{
    return am_zsn700_adtim_pwm_init(&__g_adtim5_pwm_dev,
                                    &__g_adtim5_pwm_devinfo);
}

/** \brief adtim5 pwm 实例解初始化 */
void am_zsn700_tim5_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_zsn700_adtim_pwm_deinit(handle);
}

/*******************************************************************************
  ADTIM6 配置
*******************************************************************************/

/** \brief ADTIM6用于PWM输出 平台初始化 */
void __zsn700_plfm_adtim6_pwm_init (void)
{
    am_clk_enable(CLK_TIM456);
}

/** \brief 解除ADTIM6平台初始化 */
void __zsn700_plfm_adtim6_pwm_deinit (void)
{
    am_clk_disable(CLK_TIM456);
}

/** \brief ADTIM6用于PWM功能的引脚配置信息列表 */
am_zsn700_adtim_pwm_chaninfo_t __g_adtim6_pwm_chaninfo_list[] = {

    {
        AM_ZSN700_ADTIM_PWM_CHA,
        PIOB_12,
        PIOB_12_TIM6_CHA | PIOB_12_OUT_PP ,
        PIOB_12_GPIO | PIOB_12_INPUT_PU
    },

    {
        AM_ZSN700_ADTIM_PWM_CHB,
        PIOC_5,
        PIOC_5_TIM6_CHB  | PIOC_5_OUT_PP ,
        PIOC_5_GPIO  | PIOC_5_INPUT_PU
    },
};

/** \brief ADTIM6用于PWM设备信息 */
const am_zsn700_adtim_pwm_devinfo_t  __g_adtim6_pwm_devinfo = {
    ZSN700_TIM6_BASE,                           /**< \brief ADTIM6寄存器块的基地址 */
    AM_NELEMENTS(__g_adtim6_pwm_chaninfo_list), /**< \brief 配置输出通道个数 */
    0,                                          /**< \brief 互补PWM选择
                                                 *          1：互补PWM
                                                 *          0：独立PWM
                                                 */
    AM_ZSN700_ADTIM_PWM_OCPOLARITY_HIGH,        /**< \brief 脉宽极性 */
    &__g_adtim6_pwm_chaninfo_list[0],           /**< \brief 通道配置信息列表        */
    __zsn700_plfm_adtim6_pwm_init,              /**< \brief 平台初始化函数 */
    __zsn700_plfm_adtim6_pwm_deinit             /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM6用于PWM设备定义 */
am_zsn700_adtim_pwm_dev_t __g_adtim6_pwm_dev;

/** \brief adtim6 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_zsn700_tim6_pwm_inst_init (void)
{
    return am_zsn700_adtim_pwm_init(&__g_adtim6_pwm_dev,
                                    &__g_adtim6_pwm_devinfo);
}

/** \brief adtim6 pwm 实例解初始化 */
void am_zsn700_tim6_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_zsn700_adtim_pwm_deinit(handle);
}

/**
 * @}
 */

/* end of file */
