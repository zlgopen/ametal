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
 *   1. 将 SPI 的 MOSI(PIOA_7) 引脚和 MISO(PIOA_6) 引脚用线相连，模拟从机设备，回环测试。
 *
 * - 实验现象：
 *   1. 主机通过 MISO 发送数据，发出的数据从 MOSI 读回；
 *   2. 调试串口打印测试结果。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_aml165_core_hw_spi_master.c src_aml165_core_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.00 17-04-27  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_hw_spi_master
 * \copydoc demo_aml165_core_hw_spi_master.c
 */

/** [src_aml165_core_hw_spi_master] */
#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_clk.h"
#include "am_vdebug.h"
#include "am_zml165.h"
#include "demo_zlg_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_hw_spi_master_entry (void)
{
    am_kprintf("demo aml165_core hw spi master!\r\n");

    /* 开启时钟 */
    am_clk_enable(CLK_SPI1);

    /* 配置引脚 */
    am_gpio_pin_cfg(PIOA_4, PIOA_4_SPI1_NSS | PIOA_4_AF_PP);
    am_gpio_pin_cfg(PIOA_5, PIOA_5_SPI1_SCK | PIOA_5_AF_PP);
    am_gpio_pin_cfg(PIOA_6, PIOA_6_SPI1_MISO | PIOA_6_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOA_7, PIOA_7_SPI1_MOSI | PIOA_7_AF_PP);

    demo_zlg_hw_spi_master_entry(ZML165_SPI1,
                                 PIOA_4,
                                 am_clk_rate_get(CLK_SPI1));
}
/** [src_aml165_core_hw_spi_master] */

/* end of file */
