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
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 的 MOSI 引脚和 MISO 引脚用线相连，模拟从机设备，回环测试。
 *
 * - 实验现象：
 *   1. 主机通过 MISO 发送数据，发出的数据从 MOSI 读回；
 *   2. 比较发出和读回的数据，如果相同 LED0 长亮,否则以 200ms 时间间隔闪烁。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32l17x_hw_spi_master.c src_hc32l17x_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.00 19-09-20  zp, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l17x_hw_spi_master
 * \copydoc demo_hc32l17x_hw_spi_master.c
 */

/** [src_hc32l17x_hw_spi_master] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_hc32_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "demo_hc32_entries.h"
#include "am_hc32l17x_inst_init.h"
#include "demo_aml17x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32l17x_core_hw_spi_master_entry (void)
{
    am_kprintf("demo am217_core hw spi master!\r\n");

    /* 开启时钟 */
    am_clk_enable(CLK_SPI1);

    /* 配置引脚 */
    am_gpio_pin_cfg(PIOB_12, PIOB_12_SPI1_CS   | PIOB_10_OUT_PP);
    am_gpio_pin_cfg(PIOB_10, PIOB_10_SPI1_SCK  | PIOB_10_OUT_PP);
    am_gpio_pin_cfg(PIOB_15, PIOB_15_SPI1_MOSI | PIOB_15_OUT_PP);
    am_gpio_pin_cfg(PIOC_2,  PIOC_2_SPI1_MISO  | PIOC_2_INPUT_PU);

    demo_hc32_hw_spi_master_entry((void *)HC32_SPI1, PIOB_12);
}
/** [src_hc32l17x_hw_spi_master] */

/* end of file */
