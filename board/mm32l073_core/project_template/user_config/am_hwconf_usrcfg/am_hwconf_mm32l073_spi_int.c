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
 * \brief MM32L073 SPI 用户配置文件
 * \sa am_hwconf_mm32l073_spi_int.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-23  ari, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_mm32l073.h"
#include "am_mm32_spi_int.h"
#include "am_gpio.h"
#include "hw/amhw_mm32_spi.h"
#include "am_clk.h"

/**
 * \addtogroup am_if_src_hwconf_mm32l073_spi_int
 * \copydoc am_hwconf_mm32l073_spi_int.c
 * @{
 */

/** \brief SPI1 平台初始化 */
static void __mm32_plfm_spi1_int_init (void)
{
    am_gpio_pin_cfg(PIOA_5, PIOA_5_SPI1_SCK  | PIOA_5_AF_PP);
    am_gpio_pin_cfg(PIOA_6, PIOA_6_SPI1_MISO | PIOA_6_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOA_7, PIOA_7_SPI1_MOSI | PIOA_7_AF_PP);

    am_clk_enable(CLK_SPI1);
}

/** \brief 解除SPI1 平台初始化 */
static void __mm32_plfm_spi1_int_deinit (void)
{

    /* 释放引脚为输入模式 */
    am_gpio_pin_cfg(PIOA_5, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_6, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_7, AM_GPIO_INPUT);

    am_clk_disable(CLK_SPI1);
}

/**
 * \brief SPI1 设备信息
 */
const  struct am_mm32_spi_int_devinfo  __g_spi1_int_devinfo = {
    MM32L073_SPI1_BASE,                   /**< \brief SPI1寄存器指针 */
    CLK_SPI1,                           /**< \brief 时钟ID号 */
    INUM_SPI1,                          /**< \brief SPI1中断号 */
    PIOA_7_SPI1_MOSI | PIOA_7_AF_PP,    /**< \brief SPI1配置标识 */
    PIOA_7,                             /**< \brief MOSI引脚号 */
    __mm32_plfm_spi1_int_init,           /**< \brief SPI1平台初始化函数 */
    __mm32_plfm_spi1_int_deinit          /**< \brief SPI1平台解初始化函数 */
};

/** \brief SPI1 设备实例 */
static am_mm32_spi_int_dev_t __g_spi1_int_dev;

/** \brief SPI1 实例初始化，获得SPI标准服务句柄 */
am_spi_handle_t am_mm32l073_spi1_int_inst_init (void)
{
    return am_mm32_spi_int_init(&__g_spi1_int_dev, &__g_spi1_int_devinfo);
}

/** \brief SPI1 实例解初始化 */
void am_mm32l073_spi1_int_inst_deinit (am_spi_handle_t handle)
{
    am_mm32_spi_int_deinit(handle);
}

#if 0
/** \brief SPI2 平台初始化 */
static void __mm32_plfm_spi2_int_init (void)
{
    am_gpio_pin_cfg(PIOB_13, PIOB_13_SPI2_MISO | PIOB_13_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOB_14, PIOB_14_SPI2_MOSI | PIOB_14_AF_PP);
    am_gpio_pin_cfg(PIOB_15, PIOB_15_SPI2_SCK  | PIOB_15_AF_PP);

    am_clk_enable(CLK_SPI2);
}

/** \brief 解除 SPI2 平台初始化 */
static void __mm32_plfm_spi2_int_deinit (void)
{
    am_gpio_pin_cfg(PIOB_13, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOB_14, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOB_15, AM_GPIO_INPUT);

    am_clk_disable(CLK_SPI2);
}
/**
 * \brief SPI2 设备信息
 */
const  struct am_mm32_spi_int_devinfo  __g_spi2_int_devinfo = {
    MM32L073_SPI2_BASE,                      /**< \brief SPI2 寄存器指针 */
    CLK_SPI2,                              /**< \brief 时钟 ID 号 */
    INUM_SPI2,                             /**< \brief SPI2 中断号 */
    PIOB_14_SPI2_MOSI | PIOB_14_AF_PP,     /**< \brief SPI2 配置标识 */
    PIOB_14,                               /**< \brief MOSI 引脚号 */
    __mm32_plfm_spi2_int_init,              /**< \brief SPI2 平台初始化函数 */
    __mm32_plfm_spi2_int_deinit             /**< \brief SPI2 平台解初始化函数 */
};

/** \brief SPI2 设备实例 */
static am_mm32_spi_int_dev_t __g_spi2_int_dev;

/** \brief SPI2 实例初始化，获得 SPI 标准服务句柄 */
am_spi_handle_t am_mm32l073_spi2_int_inst_init (void)
{
    return am_mm32_spi_int_init(&__g_spi2_int_dev, &__g_spi2_int_devinfo);
}

/** \brief SPI2 实例解初始化 */
void am_mm32l073_spi2_int_inst_deinit (am_spi_handle_t handle)
{
    am_mm32_spi_int_deinit(handle);
}
#endif

/**
 * @}
 */

/* end of file */
