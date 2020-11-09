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
 * \brief ZSL42x GPIO 用户配置文件。
 * \sa am_hwconf_zsl42x_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-09  zp, first implementation
 * \endinternal
 */

#include "am_gpio.h"
#include "am_clk.h"
#include "am_zsl42x.h"
#include "am_zsl42x_clk.h"
#include "am_zsl42x_gpio.h"
#include "hw/amhw_zsl42x_gpio.h"

/**
 * \addtogroup am_if_src_hwconf_zsl42x_gpio
 * \copydoc am_hwconf_zsl42x_gpio.c
 * @{
 */

/** \brief GPIO平台初始化 */
void __zsl42x_plfm_gpio_init (void)
{
    am_clk_enable(CLK_GPIO);

    am_zsl42x_clk_reset(CLK_GPIO);
}

/** \brief GPIO平台解初始化 */
void __zsl42x_plfm_gpio_deinit (void)
{
    am_zsl42x_clk_reset(CLK_GPIO);

    am_clk_disable(CLK_GPIO);
}

/** \brief 引脚触发信息内存 */
static struct am_zsl42x_gpio_trigger_info __g_gpio_triginfos[PIN_INT_MAX];

/** \brief 引脚触发信息映射 */
static uint8_t __g_gpio_infomap[PIN_INT_MAX];

/** \brief GPIO设备信息 */
const am_zsl42x_gpio_devinfo_t __g_gpio_devinfo = {
    ZSL42x_PORT0_BASE,              /**< \brief GPIO控制器寄存器块基址 */

    {
        INUM_PORTA,
        INUM_PORTB,
        INUM_PORTC_E,
        INUM_PORTD_F,
    },

    PIN_INT_MAX,                   /**< \brief GPIO使用的最大外部中断线编号+1 */

    &__g_gpio_infomap[0],          /**< \brief GPIO 引脚外部事件信息 */
    &__g_gpio_triginfos[0],        /**< \brief GPIO PIN触发信息 */

    __zsl42x_plfm_gpio_init,       /**< \brief GPIO 平台初始化 */
    __zsl42x_plfm_gpio_deinit      /**< \brief GPIO 平台去初始化 */
};

/** \brief GPIO设备实例 */
am_zsl42x_gpio_dev_t __g_gpio_dev;

/** \brief GPIO 实例初始化 */
int am_zsl42x_gpio_inst_init (void)
{
    return am_zsl42x_gpio_init(&__g_gpio_dev, &__g_gpio_devinfo);
}

/** \brief GPIO 实例解初始化 */
void am_zsl42x_gpio_inst_deinit (void)
{
    am_zsl42x_gpio_deinit();
}

/**
 * @}
 */

/* end of file */
