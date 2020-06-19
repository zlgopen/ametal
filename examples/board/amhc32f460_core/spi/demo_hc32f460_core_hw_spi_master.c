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
 * \snippet demo_hc32f460_hw_spi_master.c src_hc32f460_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.00 20-03-25  cds, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_spi_master
 * \copydoc demo_hc32f460_hw_spi_master.c
 */

/** [src_hc32f460_hw_spi_master] */
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_hc32f460_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_hw_spi_master_entry (void)
{
    am_kprintf("demo amhc32f460_core hw spi master!\r\n");

    /* 开启时钟 */
    am_clk_enable(CLK_SPI3);

    /* 配置引脚 */
    am_gpio_pin_cfg(PIOE_1, AM_GPIO_OUTPUT_INIT_HIGH | AM_GPIO_PULLUP);
    am_gpio_set(PIOE_1, 1);

    am_gpio_pin_cfg(PIOE_0,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_SCK));
    am_gpio_pin_cfg(PIOE_2,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_MOSI));
    am_gpio_pin_cfg(PIOE_3,  GPIO_AFIO(AMHW_HC32F460_AFIO_SPI3_MISO));

    demo_hc32f460_hw_spi_master_entry(HC32F460_SPI3, PIOE_1);
}
/** [src_hc32f460_hw_spi_master] */

/* end of file */
