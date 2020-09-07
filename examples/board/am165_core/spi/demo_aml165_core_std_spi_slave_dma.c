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
 * \file SPI 从机 DMA 方式例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 主机与从机连接。
 *
 * - 实验现象：
 *   1. 传输完成之后从机打印接收缓冲区的数据。
 *
 * \par 源代码
 * \snippet demo_aml165_core_std_spi_slave_dma.c src_aml165_core_std_spi_slave_dma
 *
 * \internal
 * \par History
 * - 1.00 17-09-19 fra, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_std_spi_slave_dma
 * \copydoc demo_aml165_core_std_spi_slave_dma.c
 */

/** [src_aml165_core_std_spi_slave_dma] */
#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_std_spi_slave_dma_entry (void)
{
    AM_DBG_INFO("demo aml165_core std spi slave dma!\r\n");

    demo_std_spi_slave_entry(am_zml165_spi1_slv_dma_inst_init());
}
/** [src_aml165_core_std_spi_slave_dma] */

/* end of file */
