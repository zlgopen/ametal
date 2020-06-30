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
 * \sa am_hwconf_hc32f460_adtim_pwm.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-16  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32f460.h"
#include "am_gpio.h"
#include "am_hc32f460_adtim_pwm.h"
#include "am_clk.h"
#include "am_hc32f460_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_adtim_pwm
 * \copydoc am_hwconf_hc32f460_adtim_pwm.c
 * @{
 */
 
/*******************************************************************************
  ADTIM61 配置
*******************************************************************************/

/** \brief ADTIM61用于PWM输出 平台初始化 */
void __hc32f460_plfm_adtim61_pwm_init (void)
{
    am_clk_enable(CLK_TIMER6_1);
}

/** \brief 解除ADTIM61平台初始化 */
void __hc32f460_plfm_adtim61_pwm_deinit (void)
{
    am_clk_disable(CLK_TIMER6_1);
}

/** \brief ADTIM61用于PWM功能的引脚配置信息列表 */
am_hc32f460_adtim_pwm_chaninfo_t __g_adtim61_pwm_chaninfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        AM_HC32F460_ADTIM_PWM_CHA,
        PIOE_9,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),        
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        AM_HC32F460_ADTIM_PWM_CHB,
        PIOE_8,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),        
        AM_GPIO_INPUT
    },
};

/** \brief ADTIM6用于PWM设备信息 */
const am_hc32f460_adtim_pwm_devinfo_t  __g_adtim61_pwm_devinfo = {
    HC32F460_TMR61_BASE,                           /**< \brief ADTIM6寄存器块的基地址 */
    AM_NELEMENTS(__g_adtim61_pwm_chaninfo_list),   /**< \brief 配置输出通道个数 */
    1,                                             /**< \brief 互补PWM选择
                                                    *          1：互补PWM
                                                    *          0：独立PWM
                                                    */
    AM_HC32F460_ADTIM_PWM_OCPOLARITY_HIGH,         /**< \brief 脉宽极性 */
    &__g_adtim61_pwm_chaninfo_list[0],             /**< \brief 通道配置信息列表 */
    __hc32f460_plfm_adtim61_pwm_init,              /**< \brief 平台初始化函数 */
    __hc32f460_plfm_adtim61_pwm_deinit             /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM6用于PWM设备定义 */
am_hc32f460_adtim_pwm_dev_t __g_adtim61_pwm_dev;

/** \brief adtim6 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_hc32f460_adtim61_pwm_inst_init (void)
{
    return am_hc32f460_adtim_pwm_init(&__g_adtim61_pwm_dev,
                                      &__g_adtim61_pwm_devinfo);
}

/** \brief adtim6 pwm 实例解初始化 */
void am_hc32f460_adtim61_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_hc32f460_adtim_pwm_deinit(handle);
}

/*******************************************************************************
  ADTIM62 配置
*******************************************************************************/

/** \brief ADTIM62用于PWM输出 平台初始化 */
void __hc32f460_plfm_adtim62_pwm_init (void)
{
    am_clk_enable(CLK_TIMER6_2);
}

/** \brief 解除ADTIM62平台初始化 */
void __hc32f460_plfm_adtim62_pwm_deinit (void)
{
    am_clk_disable(CLK_TIMER6_2);
}

/** \brief ADTIM61用于PWM功能的引脚配置信息列表 */
am_hc32f460_adtim_pwm_chaninfo_t __g_adtim62_pwm_chaninfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        AM_HC32F460_ADTIM_PWM_CHA,
        PIOE_11,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),        
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        AM_HC32F460_ADTIM_PWM_CHB,
        PIOE_10,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),        
        AM_GPIO_INPUT
    },
};

/** \brief ADTIM6用于PWM设备信息 */
const am_hc32f460_adtim_pwm_devinfo_t  __g_adtim62_pwm_devinfo = {
    HC32F460_TMR62_BASE,                           /**< \brief ADTIM6寄存器块的基地址 */
    AM_NELEMENTS(__g_adtim62_pwm_chaninfo_list),   /**< \brief 配置输出通道个数 */
    1,                                             /**< \brief 互补PWM选择
                                                    *          1：互补PWM
                                                    *          0：独立PWM
                                                    */
    AM_HC32F460_ADTIM_PWM_OCPOLARITY_HIGH,         /**< \brief 脉宽极性 */
    &__g_adtim62_pwm_chaninfo_list[0],             /**< \brief 通道配置信息列表 */
    __hc32f460_plfm_adtim62_pwm_init,              /**< \brief 平台初始化函数 */
    __hc32f460_plfm_adtim62_pwm_deinit             /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM6用于PWM设备定义 */
am_hc32f460_adtim_pwm_dev_t __g_adtim62_pwm_dev;

/** \brief adtim6 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_hc32f460_adtim62_pwm_inst_init (void)
{
    return am_hc32f460_adtim_pwm_init(&__g_adtim62_pwm_dev,
                                      &__g_adtim62_pwm_devinfo);
}

/** \brief adtim6 pwm 实例解初始化 */
void am_hc32f460_adtim62_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_hc32f460_adtim_pwm_deinit(handle);
}

/*******************************************************************************
  ADTIM63 配置
*******************************************************************************/

/** \brief ADTIM63用于PWM输出 平台初始化 */
void __hc32f460_plfm_adtim63_pwm_init (void)
{
    am_clk_enable(CLK_TIMER6_3);
}

/** \brief 解除ADTIM63平台初始化 */
void __hc32f460_plfm_adtim63_pwm_deinit (void)
{
    am_clk_disable(CLK_TIMER6_3);
}

/** \brief ADTIM63用于PWM功能的引脚配置信息列表 */
am_hc32f460_adtim_pwm_chaninfo_t __g_adtim63_pwm_chaninfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        AM_HC32F460_ADTIM_PWM_CHA,
        PIOE_13,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),        
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        AM_HC32F460_ADTIM_PWM_CHB,
        PIOE_12,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),        
        AM_GPIO_INPUT
    },
};

/** \brief ADTIM6用于PWM设备信息 */
const am_hc32f460_adtim_pwm_devinfo_t  __g_adtim63_pwm_devinfo = {
    HC32F460_TMR63_BASE,                           /**< \brief ADTIM6寄存器块的基地址 */
    AM_NELEMENTS(__g_adtim63_pwm_chaninfo_list),   /**< \brief 配置输出通道个数 */
    1,                                             /**< \brief 互补PWM选择
                                                    *          1：互补PWM
                                                    *          0：独立PWM
                                                    */
    AM_HC32F460_ADTIM_PWM_OCPOLARITY_HIGH,         /**< \brief 脉宽极性 */
    &__g_adtim63_pwm_chaninfo_list[0],             /**< \brief 通道配置信息列表 */
    __hc32f460_plfm_adtim63_pwm_init,              /**< \brief 平台初始化函数 */
    __hc32f460_plfm_adtim63_pwm_deinit             /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM6用于PWM设备定义 */
am_hc32f460_adtim_pwm_dev_t __g_adtim63_pwm_dev;

/** \brief adtim6 pwm 实例初始化，获得pwm标准服务句柄 */
am_pwm_handle_t am_hc32f460_adtim63_pwm_inst_init (void)
{
    return am_hc32f460_adtim_pwm_init(&__g_adtim63_pwm_dev,
                                      &__g_adtim63_pwm_devinfo);
}

/** \brief adtim6 pwm 实例解初始化 */
void am_hc32f460_adtim63_pwm_inst_deinit (am_pwm_handle_t handle)
{
    am_hc32f460_adtim_pwm_deinit(handle);
}

/**
 * @}
 */

/* end of file */
