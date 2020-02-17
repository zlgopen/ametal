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
 * \snippet demo_mm32l373_hw_spi_master.c src_mm32l373_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.00 17-04-27  ari, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l373_hw_spi_master
 * \copydoc demo_mm32l373_hw_spi_master.c
 */

/** [src_mm32l373_hw_spi_master] */
#include "ametal.h"
#include "am_mm32l373.h"
#include "am_mm32l373_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "demo_mm32_entries.h"
#include "am_mm32l373_inst_init.h"
#include "demo_mm32l373_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_mm32l373_core_hw_spi_master_entry (void)
{
    am_kprintf("demo mm32l373_core hw spi master!\r\n");

    /* 开启时钟 */
    am_clk_enable(CLK_SPI1);

    /* 配置引脚 */
    am_gpio_pin_cfg(PIOA_4, PIOA_4_SPI1_NSS | PIOA_4_AF_PP);

    am_gpio_pin_cfg(PIOA_5, PIOA_5_SPI1_SCK_REMAP0 | PIOA_5_AF_PP);
    am_gpio_pin_cfg(PIOA_6, PIOA_6_SPI1_MISO_REMAP0 | PIOA_6_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOA_7, PIOA_7_SPI1_MOSI_REMAP0 | PIOA_7_AF_PP);

    demo_mm32_hw_spi_master_entry(MM32L373_SPI1,
                                 PIOA_4,
                                 am_clk_rate_get(CLK_SPI1));
}
/** [src_mm32l373_hw_spi_master] */

/* end of file */
