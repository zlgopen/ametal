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
 * \brief KL16 SPI0 用户配置文件
 * \sa am_kl26_hwconfig_spi0_int.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-23  sdy, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_kl26_spi_int.h"
#include "am_gpio.h"
#include "hw/amhw_fsl_spi.h"
#include "hw/amhw_kl26_sim.h"
#include "../../../../../soc/freescale/kl26/am_kl26.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_spi0_int
 * \copydoc am_kl26_hwconfig_spi0_int.c
 * @{
 */

/** \brief SPI0 平台初始化 */
static void __kl26_plfm_spi0_int_init (void)
{
    am_gpio_pin_cfg(PIOD_1, PIOD_1_SPI0_SCK  | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOD_3, PIOD_3_SPI0_MISO | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOD_2, PIOD_2_SPI0_MOSI | AM_GPIO_PULLUP);

    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_SPI0);
}

/** \brief 解除SPI0 平台初始化 */
static void __kl26_plfm_spi0_int_deinit (void)
{
    /* 释放引脚为输入模式 */
    am_gpio_pin_cfg(PIOD_1, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOD_3, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOD_2, AM_GPIO_INPUT);

    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_SPI0);
}

/**
 * \brief SPI0 设备信息
 */
const  struct am_kl26_spi_int_devinfo  __g_spi0_int_devinfo = {
    KL26_SPI0_BASE,                   /**< \brief SPI0寄存器指针   */
    INUM_SPI0,                        /**< \brief SPI0中断号       */
    AM_KL26_SPI_INT_CFG_WIAT_EN,      /**< \brief SPI 配置标识    */
    __kl26_plfm_spi0_int_init,        /**< \brief SPI0平台初始化函数 */
    __kl26_plfm_spi0_int_deinit       /**< \brief SPI0平台解初始化函数 */
};

/** \brief SPI0 设备实例 */
static am_kl26_spi_int_dev_t __g_spi0_int_dev;

/** \brief SPI0 实例初始化，获得SPI标准服务句柄 */
am_spi_handle_t am_kl26_spi0_int_inst_init (void)
{
    return am_kl26_spi_int_init(&__g_spi0_int_dev, &__g_spi0_int_devinfo);
}

/** \brief SPI0 实例解初始化 */
void am_kl26_spi0_int_inst_deinit (am_spi_handle_t handle)
{
    am_kl26_spi_int_deinit(handle);
}

/**
 * @}
 */

/* end of file */
