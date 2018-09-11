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
 * \brief KL26 GPIO 用户配置文件。
 * \sa am_kl26_hwconfig_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-15  nwt, first implementation.
 * \endinternal
 */

/**
 * \addtogroup am_kl26_if_hwconfig_src_gpio
 * \copydoc am_kl26_hwconfig_gpio.c
 * @{
 */

#include "am_gpio.h"
#include "am_kl26_gpio.h"
#include "hw/amhw_kl26_gpio.h"
#include "hw/amhw_kl26_sim.h"
#include "../../../../../soc/freescale/kl26/kl26_periph_map.h"
#include "../../../../../soc/freescale/kl26/kl26_pin.h"

/**
 * \brief 使用的中断通道数量
 *
 *        默认使用所有的中断通道，用户可以根据实际使用通道数，
 *        更改此值，减少内存的占用
 *
 * \note 如果此值为0，将无法使用GPIO中断功能，但是可以使用其他GPIO功能
 */
#define __GPIO_PINT_USE_COUNT   PIN_INT_MAX


/** \brief 引脚触发信息内存 */
static struct am_kl26_gpio_trigger_info g_gpio_triginfos[__GPIO_PINT_USE_COUNT];

/** \brief 引脚触发信息映射 */
static uint8_t g_gpio_infomap[__GPIO_PINT_USE_COUNT];

/** \brief GPIO平台初始化 */
static void __kl26_plfm_gpio_init (void)
{
    /* 使能GPIO相关外设时钟 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_PORTA);
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_PORTB);
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_PORTC);
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_PORTD);
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_PORTE);
}

/** \brief GPIO平台去初始化 */
static void __kl26_plfm_gpio_deinit (void)
{
    /* 禁能GPIO相关外设时钟 */
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_PORTA);
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_PORTB);
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_PORTC);
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_PORTD);
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_PORTE);
}

/** \brief GPIO设备信息     */
static const am_kl26_gpio_devinfo_t __g_gpio_devinfo = {
    KL26_PORT,               /**< \brief 指向GPIO端口寄存器块的指针.*/
    KL26_GPIO,               /**< \brief 指向GPIO寄存器块的指针.    */
    {
        INUM_PORTA,          /**< \brief 端口A中断.                */
        INUM_PORTC_PORTD,    /**< \brief 端口C与端口D中断.          */
    },
    __GPIO_PINT_USE_COUNT,   /**< \brief GPIO支持的引脚中断号数量.  */
    g_gpio_infomap,          /**< \brief 引脚触发信息映射.          */
    g_gpio_triginfos,        /**< \brief 引脚触发信息内存.          */

    __kl26_plfm_gpio_init,   /**< \brief GPIO平台初始化.            */
    __kl26_plfm_gpio_deinit  /**< \brief GPIO平台去初始化.          */
};

/** \brief GPIO设备实例   */
am_kl26_gpio_dev_t __g_gpio_dev;

/** \brief GPIO 实例初始化 */
int am_kl26_gpio_inst_init (void)
{
    return am_kl26_gpio_init(&__g_gpio_dev, &__g_gpio_devinfo);
}

/** \brief GPIO 实例解初始化 */
void am_kl26_gpio_inst_deinit (void)
{
    am_kl26_gpio_deinit();
}


/**
 * @}
 */

/* end of file */
