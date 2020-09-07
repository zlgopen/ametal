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
 * \brief HC32F460 GPIO 用户配置文件。
 * \sa am_hwconf_hc32f460_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-12  zcb, first implementation
 * \endinternal
 */

#include <am_hc32f460_gpio.h>
#include <hc32f460_pin.h>
#include "am_gpio.h"
#include "am_clk.h"
#include "hc32f460_regbase.h"
#include "hc32f460_inum.h"


/**
 * \addtogroup am_if_src_hwconf_hc32f460_gpio
 * \copydoc am_hwconf_hc32f460_gpio.c
 * @{
 */

/** \brief GPIO平台初始化 */
void __hc32f460_plfm_gpio_init (void)
{

}

/** \brief GPIO平台解初始化 */
void __hc32f460_plfm_gpio_deinit (void)
{

}

/** \brief 引脚触发信息内存 */
static struct am_hc32f460_gpio_trigger_info __g_gpio_triginfos[PIN_INT_MAX];

/** \brief 引脚触发信息映射 */
static uint8_t __g_gpio_infomap[PIN_INT_MAX];

/** \brief GPIO设备信息 */
const am_hc32f460_gpio_devinfo_t __g_gpio_devinfo = {
    HC32F460_GPIO_BASE,   /**< \brief GPIO控制器寄存器块基址 */
    0,                    /**< \brief EXTI控制器寄存器块基址 */
    0,                    /**< \brief AFIO控制器寄存器块基址 */

    {
        INUM_PORT_EIRQ0,
        INUM_PORT_EIRQ1,
        INUM_PORT_EIRQ2,
        INUM_PORT_EIRQ3,
        INUM_PORT_EIRQ4,
        INUM_PORT_EIRQ5,
        INUM_PORT_EIRQ6,
        INUM_PORT_EIRQ7,
        INUM_PORT_EIRQ8,
        INUM_PORT_EIRQ9,
        INUM_PORT_EIRQ10,
        INUM_PORT_EIRQ11,
        INUM_PORT_EIRQ12,
        INUM_PORT_EIRQ13,
        INUM_PORT_EIRQ14,
        INUM_PORT_EIRQ15,
    },

    PIN_NUM,                       /**< \brief GPIO PIN数量 */
    PIN_INT_MAX,                   /**< \brief GPIO使用的最大外部中断线编号+1 */

    &__g_gpio_infomap[0],          /**< \brief GPIO 引脚外部事件信息 */
    &__g_gpio_triginfos[0],        /**< \brief GPIO PIN触发信息 */

    __hc32f460_plfm_gpio_init,     /**< \brief GPIO 平台初始化 */
    __hc32f460_plfm_gpio_deinit    /**< \brief GPIO 平台去初始化 */
};

/** \brief GPIO设备实例 */
am_hc32f460_gpio_dev_t __g_gpio_dev;

/** \brief GPIO 实例初始化 */
int am_hc32f460_gpio_inst_init (void)
{
    return am_hc32f460_gpio_init(&__g_gpio_dev, &__g_gpio_devinfo);
}

/** \brief GPIO 实例解初始化 */
void am_hc32f460_gpio_inst_deinit (void)
{
    am_hc32f460_gpio_deinit();
}

/**
 * @}
 */

/* end of file */
