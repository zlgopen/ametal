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
 * \brief STM32F103RBT6 GPIO 用户配置文件。
 * \sa am_hwconf_stm32f103rbt6_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-12  zcb, first implementation
 * \endinternal
 */

#include "am_stm32f103rbt6.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_stm32f103rbt6_clk.h"
#include "am_stm32f103rbt6_gpio.h"
#include "amhw_stm32f103rbt6_afio.h"
#include "amhw_stm32f103rbt6_gpio.h"

/**
 * \addtogroup am_if_src_hwconf_stm32f103rbt6_gpio
 * \copydoc am_hwconf_stm32f103rbt6_gpio.c
 * @{
 */

/** \brief GPIO平台初始化 */
void __stm32f103rbt6_plfm_gpio_init (void)
{
    am_clk_enable(CLK_IOPA);
    am_clk_enable(CLK_IOPB);
    am_clk_enable(CLK_IOPC);
    am_clk_enable(CLK_IOPD);
    am_clk_enable(CLK_AFIO);

    am_stm32f103rbt6_clk_reset(CLK_IOPA);
    am_stm32f103rbt6_clk_reset(CLK_IOPB);
    am_stm32f103rbt6_clk_reset(CLK_IOPC);
    am_stm32f103rbt6_clk_reset(CLK_IOPD);
    am_stm32f103rbt6_clk_reset(CLK_AFIO);
}

/** \brief GPIO平台解初始化 */
void __stm32f103rbt6_plfm_gpio_deinit (void)
{
    am_stm32f103rbt6_clk_reset(CLK_IOPA);
    am_stm32f103rbt6_clk_reset(CLK_IOPB);
    am_stm32f103rbt6_clk_reset(CLK_IOPC);
    am_stm32f103rbt6_clk_reset(CLK_IOPD);
    am_stm32f103rbt6_clk_reset(CLK_AFIO);

    am_clk_disable(CLK_IOPA);
    am_clk_disable(CLK_IOPB);
    am_clk_disable(CLK_IOPC);
    am_clk_disable(CLK_IOPD);
    am_clk_disable(CLK_AFIO);
}

/** \brief 引脚重映像信息 */
static amhw_stm32f103rbt6_afio_remap_peripheral_t __g_pin_remap[PIN_NUM];

/** \brief 引脚触发信息内存 */
static struct am_stm32f103rbt6_gpio_trigger_info __g_gpio_triginfos[PIN_INT_MAX];

/** \brief 引脚触发信息映射 */
static uint8_t __g_gpio_infomap[PIN_INT_MAX];

/** \brief GPIO设备信息 */
const am_stm32f103rbt6_gpio_devinfo_t __g_gpio_devinfo = {
    STM32F103RBT6_GPIO_BASE,            /**< \brief GPIO控制器寄存器块基址 */
    STM32F103RBT6_EXTI_BASE,            /**< \brief EXTI控制器寄存器块基址 */
    STM32F103RBT6_AFIO_BASE,            /**< \brief AFIO控制器寄存器块基址 */

    {
        INUM_EXTI0,
        INUM_EXTI1,
        INUM_EXTI2,
        INUM_EXTI3,
        INUM_EXTI4,
        INUM_EXTI9_5,
        INUM_EXTI15_10,
    },

    PIN_NUM,                       /**< \brief GPIO PIN数量 */
    PIN_INT_MAX,                   /**< \brief GPIO使用的最大外部中断线编号+1 */

    &__g_gpio_infomap[0],          /**< \brief GPIO 引脚外部事件信息 */
    &__g_pin_remap[0],             /**< \brief GPIO PIN重映像信息 */
    &__g_gpio_triginfos[0],        /**< \brief GPIO PIN触发信息 */

    __stm32f103rbt6_plfm_gpio_init,       /**< \brief GPIO 平台初始化 */
    __stm32f103rbt6_plfm_gpio_deinit      /**< \brief GPIO 平台去初始化 */
};

/** \brief GPIO设备实例 */
am_stm32f103rbt6_gpio_dev_t __g_gpio_dev;

/** \brief GPIO 实例初始化 */
int am_stm32f103rbt6_gpio_inst_init (void)
{
    return am_stm32f103rbt6_gpio_init(&__g_gpio_dev, &__g_gpio_devinfo);
}

/** \brief GPIO 实例解初始化 */
void am_stm32f103rbt6_gpio_inst_deinit (void)
{
    am_stm32f103rbt6_gpio_deinit();
}

/**
 * @}
 */

/* end of file */
