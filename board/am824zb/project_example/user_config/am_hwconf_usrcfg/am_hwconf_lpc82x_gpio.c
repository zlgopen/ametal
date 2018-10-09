/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X GPIO 用户配置文件
 * \sa am_hwconf_lpc82x_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-08  mem,modified.
 * - 1.00 15-07-8  zxl, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc82x_gpio.h"
#include "lpc82x_regbase.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"
/**
 * \addtogroup am_if_src_hwconf_lpc82x_gpio
 * \copydoc am_hwconf_lpc82x_gpio.c
 * @{
 */

 /** \brief GPIO平台初始化 */
static void __lpc82x_gpio_plfm_init (void)
{
    /* 复位GPIO相关外设 */
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_GPIO);

    /* 使能GPIO相关外设时钟 */
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_GPIO);
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_IOCON);
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SWM);
}

/** \brief GPIO平台解初始化 */
static void __lpc82x_gpio_plfm_deinit (void)
{
    /* 复位GPIO相关外设 */
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_GPIO);

    /* 禁能GPIO相关外设时钟 */
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_GPIO);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_IOCON);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_SWM);
}

/**
 * \brief 使用的中断通道数量
 *
 *        默认使用所有的中断通道，用户可以根据实际使用通道数，
 *        更改此值，减少内存的占用
 *
 * \note 如果此值为0，将无法使用GPIO中断功能，但是可以使用其他GPIO功能
 */
#define __GPIO_PINT_USE_COUNT   AMHW_LPC82X_PINT_CHAN_NUM

/** \brief 引脚触发信息内存 */
static
struct am_lpc82x_gpio_trigger_info __g_gpio_triginfos[__GPIO_PINT_USE_COUNT];

/** \brief 引脚触发信息映射 */
static uint8_t                     __g_gpio_infomap[__GPIO_PINT_USE_COUNT];

/** \brief GPIO设备信息 */
static const am_lpc82x_gpio_devinfo_t __g_gpio_devinfo = {

    LPC82X_SWM_BASE,           /**< \brief SWM寄存器块基址          */
    LPC82X_GPIO_BASE,          /**< \brief GPIO寄存器块基址         */
    LPC82X_IOCON_BASE,         /**< \brief IOCON寄存器块基址        */
    LPC82X_PINT_BASE,          /**< \brief PINT寄存器块基址         */
    {
        INUM_PIN_INT0,         /**< \brief PINT0中断号              */
        INUM_PIN_INT1,         /**< \brief PINT1中断号              */
        INUM_PIN_INT2,         /**< \brief PINT2中断号              */
        INUM_PIN_INT3,         /**< \brief PINT3中断号              */
        INUM_PIN_INT4,         /**< \brief PINT4中断号              */
        INUM_PIN_INT5,         /**< \brief PINT5中断号              */
        INUM_PIN_INT6,         /**< \brief PINT6中断号              */
        INUM_PIN_INT7,         /**< \brief PINT7中断号              */
    },
    __GPIO_PINT_USE_COUNT,     /**< \brief GPIO支持的引脚中断号数量 */
    &__g_gpio_infomap[0],      /**< \brief 引脚触发信息映射         */
    &__g_gpio_triginfos[0],    /**< \brief 引脚触发信息内存         */

    __lpc82x_gpio_plfm_init,   /**< \brief 平台初始化函数           */
    __lpc82x_gpio_plfm_deinit  /**< \brief 平台解初始化函数         */
};

/** \brief GPIO设备实例 */
static am_lpc82x_gpio_dev_t __g_gpio_dev;

/** \brief GPIO 实例初始化 */
int am_lpc82x_gpio_inst_init (void)
{
    return am_lpc82x_gpio_init(&__g_gpio_dev, &__g_gpio_devinfo);
}

/** \brief GPIO 实例解初始化 */
void am_lpc82x_gpio_inst_deinit (void)
{
    am_lpc82x_gpio_deinit();
}


/**
 * @}
 */

/* end of file */
