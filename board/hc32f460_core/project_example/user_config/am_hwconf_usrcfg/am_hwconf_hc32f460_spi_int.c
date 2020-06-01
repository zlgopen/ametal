/*******************************************************************************

                                AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg118.cn/
*******************************************************************************/

/**
 * \file
 * \brief HC32F460 SPI INT 用户配置文件
 * \sa am_hwconf_hc32f460_spi.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-25  cds, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32f460_spi_int.h"
#include "am_gpio.h"
#include "hw/amhw_hc32f460_spi.h"
#include "am_clk.h"
#include "am_hc32f460.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_spi_int
 * \copydoc am_hwconf_hc32f460_spi_int.c
 * @{
 */
/** \brief SPI3 平台初始化 */
static void __hc32f460_plfm_spi3_int_init (void)
{
    am_gpio_pin_cfg(PIOE_0,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_SCK));
    am_gpio_pin_cfg(PIOE_2,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_MOSI));
    am_gpio_pin_cfg(PIOE_3,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_MISO));

    am_clk_enable(CLK_SPI3);
}

/** \brief 解除SPI3 平台初始化 */
static void __hc32f460_plfm_spi3_int_deinit (void)
{
    /* 释放引脚为输入模式 */
    am_gpio_pin_cfg(PIOE_0,  GPIO_AFIO(AM_GPIO_INPUT));
    am_gpio_pin_cfg(PIOE_2,  GPIO_AFIO(AM_GPIO_INPUT));
    am_gpio_pin_cfg(PIOE_3,  GPIO_AFIO(AM_GPIO_INPUT));

    am_clk_disable(CLK_SPI3);
}

/** \brief SPI0 设备信息 */
const  struct am_hc32f460_spi_int_devinfo  __g_spi3_int_devinfo = {
    HC32F460_SPI3_BASE,                        /**< \brief SPI3寄存器指针 */
    CLK_SPI3,                                  /**< \brief 时钟ID号 */
    INUM_SPI3,                                 /**< \brief SPI3中断号 */
    GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_MOSI),   /**< \brief SPI3配置标识 */
    3,                                         /**< \brief SPI3设备ID */
    PIOE_2,                                    /**< \brief MOSI引脚号 */
    __hc32f460_plfm_spi3_int_init,             /**< \brief SPI3平台初始化函数 */
    __hc32f460_plfm_spi3_int_deinit            /**< \brief SPI3平台解初始化函数 */
};

/** \brief SPI3 设备实例 */
am_hc32f460_spi_int_dev_t __g_spi3_int_dev;

/** \brief SPI3 实例初始化，获得SPI标准服务句柄 */
am_spi_handle_t am_hc32f460_spi3_int_inst_init (void)
{
    return am_hc32f460_spi_int_init(&__g_spi3_int_dev, &__g_spi3_int_devinfo);
}

/** \brief SPI3 实例解初始化 */
void am_hcc32f460_spi3_int_inst_deinit (am_spi_handle_t handle)
{
    am_hc32f460_spi_int_deinit(handle);
}

am_hc32f460_spi_int_dev_t __g_spi1_int_dev;
am_hc32f460_spi_int_dev_t __g_spi2_int_dev;
am_hc32f460_spi_int_dev_t __g_spi4_int_dev;
/**
 * @}
 */

/* end of file */
