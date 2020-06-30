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
 * \sa am_hwconf_hc32f460_tim_cap.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-12  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32f460.h"
#include "am_hc32f460_adtim_cap.h"
#include "am_hc32f460_adtim_cap.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_hc32f460_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_tim_cap
 * \copydoc am_hwconf_hc32f460_tim_cap.c
 * @{
 */
/*******************************************************************************
  ADTIM61 配置
*******************************************************************************/
/** \brief ADTIM61用于捕获功能的平台初始化 */
void __hc32f460_plfm_adtim61_cap_init (void)
{
    am_clk_enable(CLK_TIMER6_1);
}

/** \brief 解除ADTIM61平台初始化 */
void __hc32f460_plfm_adtim61_cap_deinit (void)
{
    am_clk_disable(CLK_TIMER6_1);
}

/** \brief ADTIM6用于捕获功能的引脚配置信息列表 */
am_hc32f460_adtim_cap_ioinfo_t __g_adtim61_cap_ioinfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        AM_HC32F460_ADTIM_CAP_CHA,
        PIOE_9,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */    
    {
        AM_HC32F460_ADTIM_CAP_CHB,
        PIOE_8,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),
        AM_GPIO_INPUT
    },
};

/** \brief ADTIM6用于捕获功能的设备信息 */
const am_hc32f460_adtim_cap_devinfo_t  __g_adtim61_cap_devinfo = {
    HC32F460_TMR61_BASE,                   /**< \brief ADTIM6寄存器块的基地址 */
    INUM_TIMER6_1,                         /**< \brief ADTIM6中断编号 */
    AMHW_HC32F460_ADTIM_PCLK_DIV1024,      /**< \brief 时钟分频系数 */
    2,                                     /**< \brief 2个捕获通道 */

    &__g_adtim61_cap_ioinfo_list[0],       /**< \brief 引脚配置信息列表 */
    __hc32f460_plfm_adtim61_cap_init,      /**< \brief 平台初始化函数 */
    __hc32f460_plfm_adtim61_cap_deinit     /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM6用于捕获功能的设备定义 */
am_hc32f460_adtim_cap_dev_t  __g_adtim61_cap_dev;

/** \brief adtim61 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32f460_adtim61_cap_inst_init (void)
{
    return am_hc32f460_adtim_cap_init(&__g_adtim61_cap_dev,
                                      &__g_adtim61_cap_devinfo);
}

/** \brief adtim61 cap实例解初始化 */
void am_hc32f460_adtim61_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32f460_adtim_cap_deinit(handle);
}

/*******************************************************************************
  ADTIM62 配置
*******************************************************************************/
/** \brief ADTIM62用于捕获功能的平台初始化 */
void __hc32f460_plfm_adtim62_cap_init (void)
{
    am_clk_enable(CLK_TIMER6_2);
}

/** \brief 解除ADTIM62平台初始化 */
void __hc32f460_plfm_adtim62_cap_deinit (void)
{
    am_clk_disable(CLK_TIMER6_2);
}

/** \brief ADTIM62用于捕获功能的引脚配置信息列表 */
am_hc32f460_adtim_cap_ioinfo_t __g_adtim62_cap_ioinfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        AM_HC32F460_ADTIM_CAP_CHA,
        PIOE_11,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),        
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        AM_HC32F460_ADTIM_CAP_CHB,
        PIOE_10,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),        
        AM_GPIO_INPUT
    },

};

/** \brief ADTIM6用于捕获功能的设备信息 */
const am_hc32f460_adtim_cap_devinfo_t  __g_adtim62_cap_devinfo = {
    HC32F460_TMR62_BASE,                   /**< \brief ADTIM6寄存器块的基地址 */
    INUM_TIMER6_2,                         /**< \brief ADTIM6中断编号 */
    AMHW_HC32F460_ADTIM_PCLK_DIV16,        /**< \brief 时钟分频系数 */
    2,                                     /**< \brief 2个捕获通道 */

    &__g_adtim62_cap_ioinfo_list[0],       /**< \brief 引脚配置信息列表 */
    __hc32f460_plfm_adtim62_cap_init,      /**< \brief 平台初始化函数 */
    __hc32f460_plfm_adtim62_cap_deinit     /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM6用于捕获功能的设备定义 */
am_hc32f460_adtim_cap_dev_t  __g_adtim62_cap_dev;

/** \brief adtim62 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32f460_adtim62_cap_inst_init (void)
{
    return am_hc32f460_adtim_cap_init(&__g_adtim62_cap_dev,
                                      &__g_adtim62_cap_devinfo);
}

/** \brief adtim61 cap实例解初始化 */
void am_hc32f460_adtim62_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32f460_adtim_cap_deinit(handle);
}

/*******************************************************************************
  ADTIM63 配置
*******************************************************************************/
/** \brief ADTIM63用于捕获功能的平台初始化 */
void __hc32f460_plfm_adtim63_cap_init (void)
{
    am_clk_enable(CLK_TIMER6_3);
}

/** \brief 解除ADTIM63平台初始化 */
void __hc32f460_plfm_adtim63_cap_deinit (void)
{
    am_clk_disable(CLK_TIMER6_3);
}

/** \brief ADTIM62用于捕获功能的引脚配置信息列表 */
am_hc32f460_adtim_cap_ioinfo_t __g_adtim63_cap_ioinfo_list[] = {
    /** \brief 通道1引脚配置 */
    {
        AM_HC32F460_ADTIM_CAP_CHA,
        PIOE_13,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),        
        AM_GPIO_INPUT
    },
    /** \brief 通道2引脚配置 */
    {
        AM_HC32F460_ADTIM_CAP_CHB,
        PIOE_12,
        GPIO_AFIO(AMHW_HC32F460_AFIO_TIM6),        
        AM_GPIO_INPUT
    },
};

/** \brief ADTIM6用于捕获功能的设备信息 */
const am_hc32f460_adtim_cap_devinfo_t  __g_adtim63_cap_devinfo = {
    HC32F460_TMR63_BASE,                   /**< \brief ADTIM6寄存器块的基地址 */
    INUM_TIMER6_3,                         /**< \brief ADTIM6中断编号 */
    AMHW_HC32F460_ADTIM_PCLK_DIV16,        /**< \brief 时钟分频系数 */
    2,                                     /**< \brief 2个捕获通道 */

    &__g_adtim63_cap_ioinfo_list[0],       /**< \brief 引脚配置信息列表 */
    __hc32f460_plfm_adtim63_cap_init,      /**< \brief 平台初始化函数 */
    __hc32f460_plfm_adtim63_cap_deinit     /**< \brief 平台解初始化函数 */
};

/** \brief ADTIM63用于捕获功能的设备定义 */
am_hc32f460_adtim_cap_dev_t  __g_adtim63_cap_dev;

/** \brief adtim63 cap实例初始化，获得cap标准服务句柄 */
am_cap_handle_t am_hc32f460_adtim63_cap_inst_init (void)
{
    return am_hc32f460_adtim_cap_init(&__g_adtim63_cap_dev,
                                      &__g_adtim63_cap_devinfo);
}

/** \brief adtim63 cap实例解初始化 */
void am_hc32f460_adtim63_cap_inst_deinit (am_cap_handle_t handle)
{
    am_hc32f460_adtim_cap_deinit(handle);
}

/**
 * @}
 */

/* end of file */
