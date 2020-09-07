/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.hc32f460.cn/
*******************************************************************************/

/**
 * \file
 * \brief TIM用于PWM输出 用户配置文件
 * \sa am_hwconf_hc32f460_tim4_pwm.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-16  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32f460.h"
#include "am_gpio.h"
#include "am_hc32f460_tim4_pwm.h"
#include "am_clk.h"
#include "am_hc32f460_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_tim4_pwm
 * \copydoc am_hwconf_hc32f460_tim4_pwm.c
 * @{
 */

/*******************************************************************************
  TIM41 配置
*******************************************************************************/

/** \brief TIM41用于PWM输出 平台初始化 */
void __hc32f460_plfm_tim41_pwm_init (void)
{
    am_clk_enable(CLK_TIMER4_1); 
}

/** \brief 解除TIM41平台初始化 */
void __hc32f460_plfm_tim41_pwm_deinit (void)
{
    am_clk_disable(CLK_TIMER4_1);    
}

/** \brief TIM41用于PWM功能的引脚配置信息列表 */
am_hc32f460_tim4_pwm_chaninfo_t __g_tim41_pwm_chaninfo_list[] = {

    /** \brief 通道1引脚配置 */
    {
        TIMER4_OCO_OUH,
        PIOE_9,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        TIMER4_OCO_OUL,
        PIOE_8,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道3引脚配置 */
    {
        TIMER4_OCO_OVH,
        PIOE_11,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道4引脚配置 */
    {
        TIMER4_OCO_OVL,
        PIOE_10,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
        /** \brief 通道5引脚配置 */
    {
        TIMER4_OCO_OWH,
        PIOE_13,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道6引脚配置 */
    {
        TIMER4_OCO_OWL,
        PIOE_12,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
};

/** \brief TIM41用于PWM设备信息 */
const am_hc32f460_tim4_pwm_devinfo_t  __g_tim41_pwm_devinfo = {
    HC32F460_TMR41_BASE,                           /**< \brief TIM3寄存器块的基地址 */
    AM_NELEMENTS(__g_tim41_pwm_chaninfo_list),     /**< \brief 配置输出通道个数 */
    TIMER4_PWM_U,                                  /**< \brief Timer4PwmU  Timer4PwmV  Timer4PwmW */
    &__g_tim41_pwm_chaninfo_list[0],               /**< \brief 通道配置信息列表        */
    __hc32f460_plfm_tim41_pwm_init,                /**< \brief 平台初始化函数 */
    __hc32f460_plfm_tim41_pwm_deinit               /**< \brief 平台解初始化函数 */
};

/** \brief TIM41用于PWM设备定义 */
am_hc32f460_tim4_pwm_dev_t __g_tim41_pwm_dev;

/** \brief tim41 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_hc32f460_tim41_pwm_inst_init (void)
{
    return am_hc32f460_tim4_pwm_init(&__g_tim41_pwm_dev,
                               &__g_tim41_pwm_devinfo);
}

/** \brief tim41 pwm 实例解初始化 */
void am_hc32f460_tim41_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_hc32f460_tim4_pwm_deinit(handle);
}

/*******************************************************************************
  TIM42 配置
*******************************************************************************/

/** \brief TIM42用于PWM输出 平台初始化 */
void __hc32f460_plfm_tim42_pwm_init (void)
{
    am_clk_enable(CLK_TIMER4_2); 
}

/** \brief 解除TIM41平台初始化 */
void __hc32f460_plfm_tim42_pwm_deinit (void)
{
    am_clk_disable(CLK_TIMER4_2);    
}

/** \brief TIM42用于PWM功能的引脚配置信息列表 */
am_hc32f460_tim4_pwm_chaninfo_t __g_tim42_pwm_chaninfo_list[] = {

    /** \brief 通道1引脚配置 */
    {
        TIMER4_OCO_OUH,
        PIOA_0,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        TIMER4_OCO_OUL,
        PIOA_1,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道3引脚配置 */
    {
        TIMER4_OCO_OVH,
        PIOA_2,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道4引脚配置 */
    {
        TIMER4_OCO_OVL,
        PIOA_3,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
        /** \brief 通道5引脚配置 */
    {
        TIMER4_OCO_OWH,
        PIOA_4,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道6引脚配置 */
    {
        TIMER4_OCO_OWL,
        PIOA_5,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
};

/** \brief TIM42用于PWM设备信息 */
const am_hc32f460_tim4_pwm_devinfo_t  __g_tim42_pwm_devinfo = {
    HC32F460_TMR42_BASE,                           /**< \brief TIM3寄存器块的基地址 */
    AM_NELEMENTS(__g_tim42_pwm_chaninfo_list),     /**< \brief 配置输出通道个数 */
    TIMER4_PWM_U,                                  /**< \brief Timer4PwmU  Timer4PwmV  Timer4PwmW */                                                              
    &__g_tim42_pwm_chaninfo_list[0],               /**< \brief 通道配置信息列表        */
    __hc32f460_plfm_tim42_pwm_init,                /**< \brief 平台初始化函数 */
    __hc32f460_plfm_tim42_pwm_deinit               /**< \brief 平台解初始化函数 */
};

/** \brief TIM42用于PWM设备定义 */
am_hc32f460_tim4_pwm_dev_t __g_tim42_pwm_dev;

/** \brief tim42 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_hc32f460_tim42_pwm_inst_init (void)
{
    return am_hc32f460_tim4_pwm_init(&__g_tim42_pwm_dev,
                               &__g_tim42_pwm_devinfo);
}

/** \brief tim41 pwm 实例解初始化 */
void am_hc32f460_tim42_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_hc32f460_tim4_pwm_deinit(handle);
}

/*******************************************************************************
  TIM43 配置
*******************************************************************************/

/** \brief TIM43用于PWM输出 平台初始化 */
void __hc32f460_plfm_tim43_pwm_init (void)
{
    am_clk_enable(CLK_TIMER4_3); 
}

/** \brief 解除TIM43平台初始化 */
void __hc32f460_plfm_tim43_pwm_deinit (void)
{
    am_clk_disable(CLK_TIMER4_3);    
}

/** \brief TIM43用于PWM功能的引脚配置信息列表 */
am_hc32f460_tim4_pwm_chaninfo_t __g_tim43_pwm_chaninfo_list[] = {

    /** \brief 通道1引脚配置 */
    {
        TIMER4_OCO_OUH,
        PIOC_10,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        TIMER4_OCO_OUL,
        PIOD_8,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道3引脚配置 */
    {
        TIMER4_OCO_OVH,
        PIOC_11,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道4引脚配置 */
    {
        TIMER4_OCO_OVL,
        PIOD_9,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
        /** \brief 通道5引脚配置 */
    {
        TIMER4_OCO_OWH,
        PIOC_12,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
    /** \brief 通道6引脚配置 */
    {
        TIMER4_OCO_OWL,
        PIOD_10,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM4),
        AM_GPIO_INPUT
    },
};

/** \brief TIM43用于PWM设备信息 */
const am_hc32f460_tim4_pwm_devinfo_t  __g_tim43_pwm_devinfo = {
    HC32F460_TMR43_BASE,                           /**< \brief TIM3寄存器块的基地址 */
    AM_NELEMENTS(__g_tim43_pwm_chaninfo_list),     /**< \brief 配置输出通道个数 */
    TIMER4_PWM_U,                                  /**< \brief Timer4PwmU  Timer4PwmV  Timer4PwmW */                                                              
    &__g_tim43_pwm_chaninfo_list[0],               /**< \brief 通道配置信息列表        */
    __hc32f460_plfm_tim43_pwm_init,                /**< \brief 平台初始化函数 */
    __hc32f460_plfm_tim43_pwm_deinit               /**< \brief 平台解初始化函数 */
};

/** \brief TIM43用于PWM设备定义 */
am_hc32f460_tim4_pwm_dev_t __g_tim43_pwm_dev;

/** \brief tim43 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_hc32f460_tim43_pwm_inst_init (void)
{
    return am_hc32f460_tim4_pwm_init(&__g_tim43_pwm_dev,
                               &__g_tim43_pwm_devinfo);
}

/** \brief tim43 pwm 实例解初始化 */
void am_hc32f460_tim43_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_hc32f460_tim4_pwm_deinit(handle);
}

/**
 * @}
 */

/* end of file */
