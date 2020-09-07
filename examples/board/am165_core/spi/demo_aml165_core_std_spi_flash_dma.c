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
 * \brief SPI 主机 DMA 方式与 FLASH 通信例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 接口和 SPI FLASH 对应的接口连接起来。
 *
 * - 实验现象：
 *   1. 写数据到 FLASH；
 *   2. 读出刚才写入的数据；
 *   3. 调试串口打印测试结果。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOB_3 引脚连接 PC 串口的 RXD；
 *    2. 大多数情况下，直接使用 am_spi_write_then_read() 和
 *       am_spi_write_then_write() 函数即可。
 *
 * \par 源代码
 * \snippet demo_aml165_core_std_spi_flash_dma.c src_aml165_core_std_spi_flash_dma
 *
 * \internal
 * \par History
 * - 1.00 17-04-27  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_std_spi_flash_dma
 * \copydoc demo_aml165_core_std_spi_flash_dma.c
 */

/** [src_aml165_core_std_spi_flash_dma] */
#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zml165.h"
#include "demo_std_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_std_spi_flash_dma_entry (void)
{
    AM_DBG_INFO("demo aml165_core std spi flash dma!\r\n");

    demo_std_spi_flash_entry(am_zml165_spi1_dma_inst_init(), PIOA_4, 0, 256);
}
/** [src_aml165_core_std_spi_flash_dma] */

/* end of file */
