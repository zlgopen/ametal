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
 * \brief TIM用于PWM输出 用户配置文件
 * \sa am_hwconf_zml165_tim_pwm.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 17-04-24  nwt, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_zml165.h"
#include "am_gpio.h"
#include "am_zlg_tim_pwm.h"
#include "am_clk.h"
#include "am_zml165_clk.h"

/**
 * \addtogroup am_if_src_hwconf_zml165_tim_pwm
 * \copydoc am_hwconf_zml165_tim_pwm.c
 * @{
 */

/*******************************************************************************
 * TIM1 配置
 ******************************************************************************/

/** \brief TIM1用于PWM输出 平台初始化 */
static void __zlg_plfm_tim1_pwm_init (void)
{
    /* 使能高级定时器1时钟 */
    am_clk_enable(CLK_TIM1);

    /* 复位高级定时器1 */
    am_zml165_clk_reset(CLK_TIM1);
}

/** \brief 解除TIM1平台初始化 */
static void __zlg_plfm_tim1_pwm_deinit (void)
{
    /* 复位高级定时器1 */
    am_zml165_clk_reset(CLK_TIM1);

    /* 禁能高级定时器1时钟 */
    am_clk_disable(CLK_TIM1);
}

/** \brief TIM1用于PWM功能的通道配置信息列表 */
static am_zlg_tim_pwm_chaninfo_t __g_tim1_pwm_chaninfo_list[] = {
    /** \brief 通道1引脚配置 */
    {AM_ZLG_TIM_PWM_CH1, PIOB_3, PIOB_3_TIM1_CH1 | PIOB_3_AF_PP, PIOB_3_GPIO | PIOB_3_INPUT_FLOAT},
    /** \brief 通道2引脚配置 */
    {AM_ZLG_TIM_PWM_CH2, PIOB_4, PIOB_4_TIM1_CH2  | PIOB_4_AF_PP,  PIOB_4_GPIO  | PIOB_4_INPUT_FLOAT},
    /** \brief 通道3引脚配置 */
    {AM_ZLG_TIM_PWM_CH3, PIOB_5, PIOB_5_TIM1_CH3 | PIOB_5_AF_PP, PIOB_5_GPIO | PIOB_5_INPUT_FLOAT},
    /** \brief 通道3互补引脚配置 */
    {AM_ZLG_TIM_PWM_CH3N, PIOA_5, PIOA_5_TIM1_CH3N | PIOA_5_AF_PP, PIOA_5_GPIO | PIOA_5_INPUT_FLOAT},
};

/** \brief TIM1用于PWM设备信息 */
static const am_zlg_tim_pwm_devinfo_t  __g_tim1_pwm_devinfo = {
    ZML165_TIM1_BASE,                          /**< \brief TIM1寄存器块的基地址    */
    CLK_TIM1,                                  /**< \brief TIM1时钟ID              */
    AM_NELEMENTS(__g_tim1_pwm_chaninfo_list),  /**< \brief 配置输出通道个数        */
    AMHW_ZLG_TIM_PWM_MODE2,                    /**< \brief PWM输出模式2            */
    0,                                         /**< \brief PWM输出高电平有效       */
    &__g_tim1_pwm_chaninfo_list[0],            /**< \brief 通道配置信息列表        */
    AMHW_ZLG_TIM_TYPE0,                        /**< \brief 定时器类型              */
    __zlg_plfm_tim1_pwm_init,                  /**< \brief 平台初始化函数          */
    __zlg_plfm_tim1_pwm_deinit                 /**< \brief 平台解初始化函数        */
};

/** \brief TIM1用于PWM设备定义 */
static am_zlg_tim_pwm_dev_t  __g_tim1_pwm_dev;
static am_pwm_handle_t       __g_tim1_pwm_handle = NULL;

/** \brief tim1 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_zml165_tim1_pwm_inst_init (void)
{
    if (__g_tim1_pwm_handle == NULL) {
        __g_tim1_pwm_handle = am_zlg_tim_pwm_init(&__g_tim1_pwm_dev,
                                                  &__g_tim1_pwm_devinfo);
    }
    
    return __g_tim1_pwm_handle;
}

/** \brief tim1 pwm 实例解初始化 */
void am_zml165_tim1_pwm_inst_deinit (am_pwm_handle_t handle)
{
    if (__g_tim1_pwm_handle == handle) {
        am_zlg_tim_pwm_deinit(handle);
        __g_tim1_pwm_handle = NULL;
    }
}

/*******************************************************************************
 * TIM2 配置
 ******************************************************************************/

/** \brief TIM2用于PWM输出 平台初始化 */
static void __zlg_plfm_tim2_pwm_init (void)
{
    /* 使能通用定时器2时钟 */
    am_clk_enable(CLK_TIM2);

    /* 复位通用定时器2 */
    am_zml165_clk_reset(CLK_TIM2);
}

/** \brief 解除TIM2平台初始化 */
static void __zlg_plfm_tim2_pwm_deinit (void)
{
    /* 复位通用定时器2 */
    am_zml165_clk_reset(CLK_TIM2);

    /* 禁能通用定时器2时钟 */
    am_clk_disable(CLK_TIM2);
}

/** \brief TIM2用于PWM功能的通道配置信息列表 */
static am_zlg_tim_pwm_chaninfo_t __g_tim2_pwm_chaninfo_list[] = {
        /** \brief 通道1引脚配置 */
        {AM_ZLG_TIM_PWM_CH1, PIOA_0, PIOA_0_TIM2_CH1_ETR | PIOA_0_AF_PP, PIOA_0_GPIO | PIOA_0_INPUT_FLOAT},
        /** \brief 通道2引脚配置 */
        {AM_ZLG_TIM_PWM_CH2, PIOB_4, PIOB_4_TIM2_CH2 | PIOB_4_AF_PP, PIOB_4_GPIO | PIOB_4_INPUT_FLOAT},
        /** \brief 通道3引脚配置 */
        {AM_ZLG_TIM_PWM_CH3, PIOB_5, PIOB_5_TIM2_CH3 | PIOB_5_AF_PP, PIOB_5_GPIO | PIOB_5_INPUT_FLOAT},
};

/** \brief TIM2用于PWM设备信息 */
static const am_zlg_tim_pwm_devinfo_t  __g_tim2_pwm_devinfo = {
    ZML165_TIM2_BASE,                          /**< \brief TIM2寄存器块的基地址    */
    CLK_TIM2,                                  /**< \brief TIM2时钟ID              */
    AM_NELEMENTS(__g_tim2_pwm_chaninfo_list),  /**< \brief 配置输出通道个数        */
    AMHW_ZLG_TIM_PWM_MODE2,                    /**< \brief PWM输出模式2            */
    0,                                         /**< \brief PWM输出高电平有效       */
    &__g_tim2_pwm_chaninfo_list[0],            /**< \brief 通道配置信息列表        */
    AMHW_ZLG_TIM_TYPE1,                        /**< \brief 定时器类型              */
    __zlg_plfm_tim2_pwm_init,                  /**< \brief 平台初始化函数          */
    __zlg_plfm_tim2_pwm_deinit                 /**< \brief 平台解初始化函数        */
};

/** \brief TIM2 用于PWM设备定义 */
static am_zlg_tim_pwm_dev_t  __g_tim2_pwm_dev;
static am_pwm_handle_t       __g_tim2_pwm_handle = NULL;

/** \brief tim2 pwm实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_zml165_tim2_pwm_inst_init (void)
{
    if (__g_tim2_pwm_handle == NULL) {
        __g_tim2_pwm_handle = am_zlg_tim_pwm_init(&__g_tim2_pwm_dev,
                                                  &__g_tim2_pwm_devinfo);
    }
    
    return __g_tim2_pwm_handle;
}

/** \brief tim2 pwm实例解初始化 */
void am_zml165_tim2_pwm_inst_deinit (am_pwm_handle_t handle)
{
    if (__g_tim2_pwm_handle == handle) {
        am_zlg_tim_pwm_deinit(handle);
        __g_tim2_pwm_handle = NULL;
    }
}

/*******************************************************************************
 * TIM3 配置
 ******************************************************************************/

/** \brief TIM3用于PWM输出 平台初始化 */
static void __zlg_plfm_tim3_pwm_init (void)
{
    /* 使能通用定时器3时钟 */
    am_clk_enable(CLK_TIM3);

    /* 复位通用定时器3 */
    am_zml165_clk_reset(CLK_TIM3);
}

/** \brief 解除TIM3 PWM平台初始化 */
static void __zlg_plfm_tim3_pwm_deinit (void)
{
    /* 复位通用定时器3 */
    am_zml165_clk_reset(CLK_TIM3);

    /* 禁能通用定时器3时钟 */
    am_clk_disable(CLK_TIM3);
}

/** \brief TIM3用于PWM功能的通道配置信息列表 */
static am_zlg_tim_pwm_chaninfo_t __g_tim3_pwm_chaninfo_list[] = {
    /** \brief 通道1引脚配置 */
    {AM_ZLG_TIM_PWM_CH1, PIOB_4,  PIOB_4_TIM3_CH1 | PIOB_4_AF_PP, PIOB_4_GPIO | PIOB_4_INPUT_FLOAT},
    /** \brief 通道2引脚配置 */
    {AM_ZLG_TIM_PWM_CH2, PIOB_5,  PIOB_5_TIM3_CH2 | PIOB_5_AF_PP, PIOB_5_GPIO | PIOB_5_INPUT_FLOAT},
};

/** \brief TIM3用于PWM设备信息 */
static const am_zlg_tim_pwm_devinfo_t  __g_tim3_pwm_devinfo = {
    ZML165_TIM3_BASE,                          /**< \brief TIM3寄存器块的基地址    */
    CLK_TIM3,                                  /**< \brief TIM3时钟ID              */
    AM_NELEMENTS(__g_tim3_pwm_chaninfo_list),  /**< \brief 配置输出通道个数        */
    AMHW_ZLG_TIM_PWM_MODE2,                    /**< \brief PWM输出模式2            */
    0,                                         /**< \brief PWM输出高电平有效       */
    &__g_tim3_pwm_chaninfo_list[0],            /**< \brief 通道配置信息列表        */
    AMHW_ZLG_TIM_TYPE1,                        /**< \brief 定时器类型              */
    __zlg_plfm_tim3_pwm_init,                  /**< \brief 平台初始化函数          */
    __zlg_plfm_tim3_pwm_deinit                 /**< \brief 平台解初始化函数        */
};

/** \brief TIM3用于PWM设备定义 */
static am_zlg_tim_pwm_dev_t  __g_tim3_pwm_dev;
static am_pwm_handle_t       __g_tim3_pwm_handle = NULL;

/** \brief tim3 pwm实例初始化，获得PWM标准服务句柄 */
am_pwm_handle_t am_zml165_tim3_pwm_inst_init (void)
{
    if (__g_tim3_pwm_handle == NULL) {
        __g_tim3_pwm_handle = am_zlg_tim_pwm_init(&__g_tim3_pwm_dev,
                                                  &__g_tim3_pwm_devinfo);
    }
    return __g_tim3_pwm_handle;
}

/** \brief tim3 pwm 实例解初始化 */
void am_zml165_tim3_pwm_inst_deinit (am_pwm_handle_t handle)
{
    if (__g_tim3_pwm_handle == handle) {
        am_zlg_tim_pwm_deinit(handle);
        __g_tim3_pwm_handle = NULL;
    }
}

/*******************************************************************************
 * TIM14配置
 ******************************************************************************/

/** \brief TIM14用于PWM输出 平台初始化 */
static void __zlg_plfm_tim14_pwm_init (void)
{
    /* 使能通用定时器14时钟 */
    am_clk_enable(CLK_TIM14);

    /* 复位通用定时器14 */
    am_zml165_clk_reset(CLK_TIM14);
}

/** \brief 解除TIM14 PWM平台初始化 */
static void __zlg_plfm_tim14_pwm_deinit (void)
{
    /* 复位通用定时器14 */
    am_zml165_clk_reset(CLK_TIM14);

    /* 禁能通用定时器14时钟 */
    am_clk_disable(CLK_TIM14);
}

/** \brief TIM14用于PWM功能的通道配置信息列表 */
static am_zlg_tim_pwm_chaninfo_t __g_tim14_pwm_chaninfo_list[] = {
    /** \brief 通道1引脚配置 */
    {AM_ZLG_TIM_PWM_CH1, PIOA_4,  PIOA_4_TIM14_CH1 | PIOA_4_AF_PP, AM_GPIO_INPUT | AM_GPIO_FLOAT},
};

/** \brief TIM14用于PWM设备信息 */
static const am_zlg_tim_pwm_devinfo_t  __g_tim14_pwm_devinfo = {
    ZML165_TIM14_BASE,                          /**< \brief TIM14寄存器块的基地址   */
    CLK_TIM14,                                  /**< \brief TIM14时钟ID             */
    AM_NELEMENTS(__g_tim14_pwm_chaninfo_list) , /**< \brief 配置输出通道个数        */
    AMHW_ZLG_TIM_PWM_MODE2,                     /**< \brief PWM输出模式2            */
    0,                                          /**< \brief PWM输出高电平有效       */
    &__g_tim14_pwm_chaninfo_list[0],            /**< \brief 通道配置信息列表        */
    AMHW_ZLG_TIM_TYPE2,                         /**< \brief 定时器类型              */
    __zlg_plfm_tim14_pwm_init,                  /**< \brief 平台初始化函数          */
    __zlg_plfm_tim14_pwm_deinit                 /**< \brief 平台解初始化函数        */
};

/** \brief TIM14用于PWM设备定义 */
static am_zlg_tim_pwm_dev_t  __g_tim14_pwm_dev;
static am_pwm_handle_t       __g_tim14_pwm_handle = NULL;

/** \brief tim14 pwm实例初始化，获得PWM标准服务句柄 */
am_pwm_handle_t am_zml165_tim14_pwm_inst_init (void)
{
    if (__g_tim14_pwm_handle == NULL) {
        __g_tim14_pwm_handle = am_zlg_tim_pwm_init(&__g_tim14_pwm_dev,
                                                   &__g_tim14_pwm_devinfo);
    }
    return __g_tim14_pwm_handle;
}

/** \brief tim14 pwm实例解初始化 */
void am_zml165_tim14_pwm_inst_deinit (am_pwm_handle_t handle)
{
    if (__g_tim14_pwm_handle == handle) {
        am_zlg_tim_pwm_deinit(handle);
        __g_tim14_pwm_handle = NULL;
    }
}

/**
 * @}
 */

/* end of file */
