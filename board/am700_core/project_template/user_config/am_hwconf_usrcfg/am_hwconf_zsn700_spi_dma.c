/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zsn700.cn/
*******************************************************************************/

/**
 * \file
 * \brief ZSN700 SPI DMA 用户配置文件
 * \sa am_hwconf_zsn700_spi.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-18  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_zsn700_spi_dma.h"
#include "am_gpio.h"
#include "hw/amhw_zsn700_spi.h"
#include "am_clk.h"
#include "am_zsn700.h"
#include "zsn700_dma_chan.h"
#include "am_zsn700_spi_dma.h"
/**
 * \addtogroup am_if_src_hwconf_zsn700_spi_dma
 * \copydoc am_hwconf_zsn700_spi_dma.c
 * @{
 */
/** \brief SPI1 平台初始化 */
static void __zsn700_plfm_spi1_dma_init (void)
{
    am_gpio_pin_cfg(PIOB_10, PIOB_10_SPI1_SCK  | PIOB_10_OUT_PP);
    am_gpio_pin_cfg(PIOB_15, PIOB_15_SPI1_MOSI | PIOB_15_OUT_PP);
    am_gpio_pin_cfg(PIOC_2,  PIOC_2_SPI1_MISO  | PIOC_2_INPUT_PU);

    am_clk_enable(CLK_SPI1);
}

/** \brief 解除 SPI1 平台初始化 */
static void __zsn700_plfm_spi1_dma_deinit (void)
{
    am_gpio_pin_cfg(PIOB_10, PIOB_10_INPUT_PU);
    am_gpio_pin_cfg(PIOB_15, PIOB_15_INPUT_PU);
    am_gpio_pin_cfg(PIOC_2,  PIOC_2_INPUT_PU);

    am_clk_disable(CLK_SPI1);
}

/** \brief SPI1 设备信息 */
static const  struct am_zsn700_spi_dma_devinfo  __g_spi1_dma_devinfo = {
    ZSN700_SPI1_BASE,                        /**< \brief SPI1寄存器指针 */
    CLK_SPI1,                                /**< \brief 时钟ID号 */
    INUM_SPI1,                               /**< \brief SPI1中断号 */
    DMA_CHAN_1,
    DMA_CHAN_2,
    __zsn700_plfm_spi1_dma_init,             /**< \brief SPI1平台初始化函数 */
    __zsn700_plfm_spi1_dma_deinit            /**< \brief SPI1平台解初始化函数 */
};
/** \brief SPI1 设备实例 */
static am_zsn700_spi_dma_dev_t __g_spi1_dma_dev;

/** \brief SPI1 实例初始化，获得SPI标准服务句柄 */
am_spi_handle_t am_zsn700_spi1_dma_inst_init (void)
{
    return am_zsn700_spi_dma_init(&__g_spi1_dma_dev, &__g_spi1_dma_devinfo);
}

/** \brief SPI1 实例解初始化 */
void am_zsn700_spi1_dma_inst_deinit (am_spi_handle_t handle)
{
    am_zsn700_spi_dma_deinit(handle);
}

/**
 * @}
 */

/* end of file */
