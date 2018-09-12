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
 * \brief SPI 从机接收数据例程（DMA 方式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 接口和数据源对应的 SPI 接口连接起来。
 *
 * - 实验现象：
 *   1. 接收到数据，通过串口打印出来。
 *
 * \note
 *    1. 数据源可选用 demo_am824_std_spi_master_dma.c 的程序；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. 由于 PIO0_12 拉低会导致单片机复位时进入 ISP 模式，所以 PIO0_12 应
 *       单片机复位后再连接。
 *
 * \par 源代码
 * \snippet demo_am824_hw_spi_slave_dma.c src_am824_hw_spi_slave_dma
 *
 * \internal
 * \par History
 * - 1.01 15-11-27  sky, modified
 * - 1.00 15-07-16  aii, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_spi_slave_dma
 * \copydoc demo_am824_hw_spi_slave_dma.c
 */

/** [src_am824_hw_spi_slave_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_hw_spi_slave_dma_entry (void)
{

    /* SPI0 引脚配置 */
    am_gpio_pin_cfg(PIO0_14, PIO_FUNC_SPI0_SSEL0);
    am_gpio_pin_cfg(PIO0_15, PIO_FUNC_SPI0_SCK);
    am_gpio_pin_cfg(PIO0_12, PIO_FUNC_SPI0_MOSI);
    am_gpio_pin_cfg(PIO0_13, PIO_FUNC_SPI0_MISO);

    /* 使能 SPI0 时钟 */
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SPI0);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SPI0);

        /* DMA 平台初始化 */
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_DMA);
  
    demo_lpc824_hw_spi_slave_dma_entry(LPC82X_SPI0,
                                       DMA_CHAN_SPI0_RX_REQ);
}
/** [src_am824_hw_spi_slave_dma] */

/* end of file */
