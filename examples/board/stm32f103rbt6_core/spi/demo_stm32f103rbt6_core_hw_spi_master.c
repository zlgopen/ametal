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
 * \brief; SPI 主机例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 当从机使用STM32F103RBT6软件包对应的spi_slaver时，demo_stm32f103rbt6_hw_spi_master_entry函数传入的
 *      CS延时至少为100.
 *   2. 将本主机的 SPI接口与从机SPI接口对应连接 。
 *   3. 先复位主机，再复位从机，再运行主机程序。
 *   4. 特别注意，主机的整个复位动作会产生一个等效的片选信号，导致从机错误识别上传一次历史数据。
 *      请注意步骤2操作顺序，以避免从机错误数据上传。
 *
 * - 实验现象：
 *   1. 主机发送的数据不断累加1，从机发回的数据保持不变。（从机为STM32F103RBT6软件包对应的spi_slaver）
 *   2. 可直接短接自身的MOSI和MISO。
 *   3. 调试串口打印通过SPI接收和发送的具体数据。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_stm32f103rbt6_hw_spi_master.c src_stm32f103rbt6_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.00 19-07-23  ari, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_stm32f103rbt6_hw_spi_master
 * \copydoc demo_stm32f103rbt6_hw_spi_master.c
 */

/** [src_stm32f103rbt6_hw_spi_master] */
#include "ametal.h"
#include "am_stm32f103rbt6.h"
#include "am_stm32f103rbt6_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "demo_stm32f103rbt6_entries.h"
#include "am_stm32f103rbt6_inst_init.h"
#include "demo_stm32f103rbt6_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_core_hw_spi_master_entry (void)
{
    am_kprintf("demo stm32f103rbt6_core hw spi master!\r\n");

    /* 开启时钟 */
    am_clk_enable(CLK_SPI2);

    /* 配置片选引脚 */
    am_gpio_pin_cfg(PIOB_12, PIOB_12_OUT_PP);
    am_gpio_set(PIOB_12, 1);

    /* 配置SPI引脚 */
    am_gpio_pin_cfg(PIOB_13, PIOB_13_SPI2_SCK  | PIOB_13_AF_PP);
    am_gpio_pin_cfg(PIOB_14, PIOB_14_SPI2_MISO | PIOB_14_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOB_15, PIOB_15_SPI2_MOSI | PIOB_15_AF_PP);

    demo_stm32f103rbt6_hw_spi_master_entry((void *)STM32F103RBT6_SPI2,
                                    PIOB_12,
                                    am_clk_rate_get(CLK_SPI2),
                                    100);
}
/** [src_stm32f103rbt6_hw_spi_master] */

/* end of file */
