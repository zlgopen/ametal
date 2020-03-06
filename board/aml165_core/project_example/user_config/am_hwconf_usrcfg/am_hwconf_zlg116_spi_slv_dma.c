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
 * \brief ZLG116 SPI 从设备用户配置文件
 * \sa am_hwconf_zlg116_spi_slv_dma.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-12-07  vir, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_zlg116.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_zlg_spi_slv_dma.h"
#include "hw/amhw_zlg_spi.h"

/**
 * \addtogroup am_if_src_hwconf_zlg116_spi_slv_dma
 * \copydoc am_hwconf_zlg116_spi_slv_dma.c
 * @{
 */

/** \brief SPI1 从设备平台初始化 */
static void __zlg116_spi1_slv_dma_plfm_init (void)
{
    am_gpio_pin_cfg(PIOB_3, PIOB_3_SPI1_SCK  | PIOB_3_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOB_4, PIOB_4_SPI1_MISO | PIOB_4_AF_PP);
    am_gpio_pin_cfg(PIOB_5, PIOB_5_SPI1_MOSI | PIOB_5_INPUT_FLOAT);

    am_clk_enable(CLK_SPI1);
}

/** \brief 解除 SPI1 从设备平台初始化 */
static void __zlg116_spi1_slv_dma_plfm_deinit (void)
{
    am_clk_disable(CLK_SPI1);
}

/**
 * \brief SPI1 从设备设备信息
 */
const  struct am_zlg116_spi_slv_dma_devinfo __g_spi1_slv_devinfo = {
    ZLG116_SPI1_BASE,                      /* SPI1 寄存器指针 */
    CLK_SPI1,                              /* 时钟 ID 号 */
    DMA_CHAN_SPI1_TX,
    DMA_CHAN_SPI1_RX,
    PIOA_4,
    __zlg116_spi1_slv_dma_plfm_init,       /* SPI1 平台初始化函数 */
    __zlg116_spi1_slv_dma_plfm_deinit      /* SPI1 平台解初始化函数 */
};

/** \brief SPI1 从机设备实例 */
static am_zlg_spi_slv_dma_dev_t __g_spi1_slv_dev;

/** \brief SPI1 从机实例初始化，获得 SPI 从机标准服务句柄 */
am_spi_slv_handle_t am_zlg116_spi1_slv_dma_inst_init (void)
{
    return am_zlg_spi_slv_dma_init(&__g_spi1_slv_dev, &__g_spi1_slv_devinfo);
}

/** \brief SPI1 从设备实例解初始化 */
void am_zlg116_spi1_slv_dma_inst_deinit (am_spi_slv_handle_t handle)
{
    am_zlg_spi_slv_dma_deinit(handle);
}
/**
 * @}
 */

/* end of file */
