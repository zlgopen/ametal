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
 * \brief MM32L073 GPIO 用户配置文件。
 * \sa am_hwconf_mm32l073_gpio.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 17-04-12  nwt, first implementation.
 * \endinternal
 */

#include "am_mm32l073.h"
#include "am_gpio.h"
#include "am_mm32l073_gpio.h"
#include "hw/amhw_mm32_gpio.h"
#include "am_mm32l073_clk.h"

/**
 * \addtogroup am_if_src_hwconf_mm32l073_gpio
 * \copydoc am_hwconf_mm32l073_gpio.c
 * @{
 */

/** \brief GPIO平台初始化 */
void __mm32l073_plfm_gpio_init (void)
{

    /* 使能GPIO相关外设时钟 */

    /* 开启GPIO各个端口时钟 */
    am_clk_enable(CLK_GPIOA );
    am_clk_enable(CLK_GPIOB);
    am_clk_enable(CLK_GPIOC);
    am_clk_enable(CLK_GPIOD);

    /* 系统配置时钟使能(等价于AFIO时钟) */
    am_clk_enable(CLK_SYSCFG);

    /* 复位GPIO相关外设 */
    am_mm32l073_clk_reset(CLK_GPIOA);
    am_mm32l073_clk_reset(CLK_GPIOB);
    am_mm32l073_clk_reset(CLK_GPIOC);
    am_mm32l073_clk_reset(CLK_GPIOD);
    am_mm32l073_clk_reset(CLK_SYSCFG);
}

/** \brief GPIO平台去初始化 */
void __mm32l073_plfm_gpio_deinit (void)
{

    /* 复位GPIO相关外设 */
    am_mm32l073_clk_reset(CLK_GPIOA);
    am_mm32l073_clk_reset(CLK_GPIOB);
    am_mm32l073_clk_reset(CLK_GPIOC);
    am_mm32l073_clk_reset(CLK_GPIOD);
    am_mm32l073_clk_reset(CLK_SYSCFG);

    /* 禁能GPIO相关外设时钟 */

    /* 禁能GPIO各个端口时钟 */
    am_clk_disable(CLK_GPIOA);
    am_clk_disable(CLK_GPIOB);
    am_clk_disable(CLK_GPIOC);
    am_clk_disable(CLK_GPIOD);

    /* 系统配置时钟禁能(等价于AFIO时钟) */
    am_clk_disable(CLK_SYSCFG);
}

/** \brief 引脚触发信息内存 */
static struct am_mm32l073_gpio_trigger_info __g_gpio_triginfos[PIN_INT_MAX];

/** \brief 引脚触发信息映射 */
static uint8_t __g_gpio_infomap[PIN_INT_MAX];

/** \brief GPIO设备信息 */
const am_mm32l073_gpio_devinfo_t __g_gpio_devinfo = {
     MM32L073_GPIO_BASE,          /**< \brief GPIO控制器寄存器块基址 */
     MM32L073_SYSCFG_BASE,        /**< \brief SYSCFG配置寄存器块基址 */
     MM32L073_EXTI_BASE,          /**< \brief 外部事件控制器寄存器块基址 */

     {
          INUM_EXTI0_1,         /**< \brief 外部中断线0与线1 */
          INUM_EXTI2_3,         /**< \brief 外部中断线2与线3 */
          INUM_EXTI4_15         /**< \brief 外部中断线4与线15 */

     },

     PIN_INT_MAX,               /**< \brief GPIO支持的引脚中断号数量 */
    __g_gpio_infomap,           /**< \brief 引脚触发信息映射 */
    __g_gpio_triginfos,         /**< \brief 引脚触发信息内存 */
    __mm32l073_plfm_gpio_init,
    __mm32l073_plfm_gpio_deinit
};

/** \brief GPIO设备实例 */
am_mm32l073_gpio_dev_t __g_gpio_dev;

/** \brief GPIO 实例初始化 */
int am_mm32l073_gpio_inst_init (void)
{
    return am_mm32l073_gpio_init(&__g_gpio_dev, &__g_gpio_devinfo);
}

/** \brief GPIO 实例解初始化 */
void am_mm32l073_gpio_inst_deinit (void)
{
    am_mm32l073_gpio_deinit();
}


/**
 * @}
 */

/* end of file */
