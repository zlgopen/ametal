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
 * \brief HC32F460 SPI DMA 用户配置文件
 * \sa am_hwconf_hc32f460_spi_slv_dma.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-01  cds, first implementation
 * \endinternal
 */

#include "am_hc32f460.h"
#include "ametal.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "hw/amhw_hc32f460_spi.h"
#include "am_hc32f460_spi_slv_dma.h"
/**
 * \addtogroup am_if_src_hwconf_hc32f460_spi_slv_dma
 * \copydoc am_hwconf_hc32f460_spi_slv_dma.c
 * @{
 */

/** \brief SPI1 平台初始化 */
static void __hc32f460_plfm_spi3_slv_dma_init (void)
{
    am_gpio_pin_cfg(PIOE_0,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_SCK));
    am_gpio_pin_cfg(PIOE_2,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_MOSI));
    am_gpio_pin_cfg(PIOE_3,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_MISO));

    am_clk_enable(CLK_SPI3);
    am_clk_enable(CLK_PTDIS);
}

/** \brief 解除 SPI1 平台初始化 */
static void __hc32f460_plfm_spi3_slv_dma_deinit (void)
{
    am_gpio_pin_cfg(PIOE_0, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOE_2, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOE_3, AM_GPIO_INPUT);

    am_clk_disable(CLK_SPI3);
}


/**
 * \brief SPI3 设备信息
 */
static const struct am_hc32f460_spi_slv_dma_devinfo  __g_spi3_slv_dma_devinfo = {
    HC32F460_SPI3_BASE,                 /**< \brief SPI3寄存器指针 */
    CLK_SPI3,                           /**< \brief 时钟ID号 */
    3,
	1,
    DMA_CHAN_1,
    DMA_CHAN_2,
    PIOE_1,                             /**< \brief SPI3的NSS引脚，只能使用默认的NSS引脚*/
    __hc32f460_plfm_spi3_slv_dma_init,  /**< \brief SPI3平台初始化函数 */
    __hc32f460_plfm_spi3_slv_dma_deinit /**< \brief SPI3平台解初始化函数 */
};

/** \brief SPI3 设备实例 */
static am_hc32f460_spi_slv_dma_dev_t __g_spi3_slv_dma_dev;


/** \brief SPI3 实例初始化，获得SPI标准服务句柄 */
am_spi_slv_handle_t am_hc32f460_spi3_slv_dma_inst_init (void)
{
    return am_hc32f460_spi_slv_dma_init(&__g_spi3_slv_dma_dev,
                                        &__g_spi3_slv_dma_devinfo);
}

/** \brief SPI3 实例解初始化 */
void am_hc32f460_spi3_slv_dma_inst_deinit (am_spi_slv_handle_t handle)
{
    am_hc32f460_spi_slv_dma_deinit(handle);
}


/**
 * @}
 */

/* end of file */
