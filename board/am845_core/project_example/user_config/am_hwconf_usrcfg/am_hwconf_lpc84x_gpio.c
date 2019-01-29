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
 * \brief LPC84X GPIO 用户配置文件
 * \sa am_hwconf_lpc84x_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-08  mem,modified
 * - 1.00 15-07-08  zxl, first implementation
 * \endinternal
 */

#include <am_lpc84x.h>
#include "ametal.h"
#include "am_lpc84x_gpio.h"
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "hw/amhw_lpc82x_pint.h"

/**
 * \addtogroup am_if_src_hwconf_lpc84x_gpio
 * \copydoc am_hwconf_lpc84x_gpio.c
 * @{
 */

/**
 * \brief GPIO 平台初始化
 */
am_local void __lpc84x_gpio_plfm_init (void)
{
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_GPIO0);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_GPIOINT);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_GPIO1);

    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_GPIO0);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_RESET_GPIOINT);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_GPIO1);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_IOCON);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SWM);

}

/**
 * \brief GPIO 平台解初始化
 */
am_local void __lpc84x_gpio_plfm_deinit (void)
{
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_GPIO0);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_GPIO1);

    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_GPIO0);
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_GPIO1);
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_IOCON);
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_SWM);
}

/**
 * \brief 使用的中断通道数量
 *
 * 默认使用所有的中断通道，用户可以根据实际使用通道数，更改此值，减少内存的占用
 *
 * \note 如果此值为 0，将无法使用 GPIO 中断功能，但是可以使用其他 GPIO 功能
 */
#define __GPIO_PINT_USE_COUNT    AMHW_LPC82X_PINT_CHAN_NUM

/** \brief 引脚触发信息内存 */
am_local
struct am_lpc84x_gpio_trigger_info __g_gpio_triginfos[__GPIO_PINT_USE_COUNT];

/** \brief 引脚触发信息映射 */
am_local uint8_t                   __g_gpio_infomap[__GPIO_PINT_USE_COUNT];

/** \brief GPIO 设备信息 */
am_local am_const am_lpc84x_gpio_devinfo_t __g_lpc84x_gpio_devinfo = {

    LPC84X_SWM_BASE,             /* SWM 寄存器块基址 */
    LPC84X_GPIO_BASE,            /* GPIO 寄存器块基址 */
    LPC84X_IOCON_BASE,           /* IOCON 寄存器块基址 */
    LPC84X_PINT_BASE,            /* PINT 寄存器块基址 */
    {
        INUM_PIN_INT0,           /* PINT0 中断号 */
        INUM_PIN_INT1,           /* PINT1 中断号 */
        INUM_PIN_INT2,           /* PINT2 中断号 */
        INUM_PIN_INT3,           /* PINT3 中断号 */
        INUM_PIN_INT4,           /* PINT4 中断号 */
        INUM_PIN_INT5,           /* PINT5 中断号 */
        INUM_PIN_INT6,           /* PINT6 中断号 */
        INUM_PIN_INT7,           /* PINT7 中断号 */
    },
    __GPIO_PINT_USE_COUNT,       /* GPIO 支持的引脚中断号数量 */
    &__g_gpio_infomap[0],        /* 引脚触发信息映射 */
    &__g_gpio_triginfos[0],      /* 引脚触发信息内存 */
    __lpc84x_gpio_plfm_init,     /* 平台初始化函数 */
    __lpc84x_gpio_plfm_deinit    /* 平台解初始化函数 */
};

/** \brief GPIO 设备实例 */
am_local am_lpc84x_gpio_dev_t __g_lpc84x_gpio_dev;

/**
 * \brief GPIO 实例初始化
 */
int am_lpc84x_gpio_inst_init (void)
{
    return am_lpc84x_gpio_init(&__g_lpc84x_gpio_dev, &__g_lpc84x_gpio_devinfo);
}

/**
 * \brief GPIO 实例解初始化
 */
void am_lpc84x_gpio_inst_deinit (void)
{
    am_lpc84x_gpio_deinit();
}

/**
 * @}
 */

/* end of file */
