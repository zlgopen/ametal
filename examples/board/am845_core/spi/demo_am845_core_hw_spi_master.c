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
 * \brief SPI 主机发送数据例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 主机与 SPI 从机进行物理连接。
 *
 * - 实验现象：
 *   1. 本例程每 500 毫秒通过 SPI，向从机发送 "nihao" 字符串；
 *   2. 从机接收到字符串，通过串口将字符串打印出来，且 LED0 闪烁。
 *
 * \note
 *    1. 本例程需要与 demo_am845_hw_spi_slave.c 一同测试；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *       PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_spi_master.c src_am845_core_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.01 15-11-26  sky, modified
 * - 1.00 15-07-15  aii, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_spi_master
 * \copydoc demo_am845_core_hw_spi_master.c
 */

/** [src_am845_core_hw_spi_master] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc84x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_spi_master_entry (void)
{
    uint32_t clk = 0;
  
    am_kprintf("demo845 hw spi master!\r\n");
  
    /* SPI0 引脚配置 */
    am_gpio_pin_cfg(PIO0_10, PIO_FUNC_SPI0_SSEL0);
    am_gpio_pin_cfg(PIO0_17, PIO_FUNC_SPI0_SCK);
    am_gpio_pin_cfg(PIO0_18, PIO_FUNC_SPI0_MOSI);
    am_gpio_pin_cfg(PIO0_19, PIO_FUNC_SPI0_MISO);

    /* 使能 SPI0 时钟 */
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SPI0);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_SPI0);
    amhw_lpc84x_clk_spi0_clk_sel_set (AMHW_LPC84X_CLK_DEVICE_FRG0CLK);
  
    clk = amhw_lpc84x_clk_periph_freq_get(LPC84X_SPI0);
  
    demo_lpc_hw_spi_master_entry(LPC84X_SPI0, clk);
}
/** [src_am845_core_hw_spi_master] */

/* end of file */
