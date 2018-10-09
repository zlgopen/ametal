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
 * \brief 在SPI中断模式下操作SPI_FLASH演示例程，通过标准接口实现
 *
 * - 操作步骤:
 *   1. 将SPI接口和SPI_FLASH对应的接口连接起来；
 *   2. 连接串口，波特率为115200、8个数据位、1停止位、无校验；
 *   3. 将程序下载到demo板，重新上电或者复位。
 *
 * - 实验现象:
 *   1. 从SPI_FLASH中读取到的数据，通过串口打印出来；
 *   2. 写入数据和读取的数据相同，SPI_FLASH读写测试成功，LED0以200ms间隔闪烁。
 *
 * \note
 *   1. 可见，大多数情况下，直接使用 am_spi_write_then_read() 和 
 *      am_spi_write_then_write() 函数即可。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_spi_flash_dma.c src_amks16z_core_std_spi_flash_dma
 *
 * \internal
 * \par History
 * - 1.01 15-11-27  sky, modified.
 * - 1.00 15-07-16  aii, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_amks16z_core_if_std_spi_flash_dma
 * \copydoc demo_amks16z_core_std_spi_flash_dma.c
 */
 
/** [src_amks16z_core_std_spi_flash_dma] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_spi.h"
#include "am_kl26_inst_init.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "demo_std_entries.h"
#include "../../../../soc/freescale/kl26/kl26_pin.h"

/**
 * \brief STD层SPI读写Flash测试函数入口
 *
 * \param[in] spi_handle : SPI标准服务操作句柄
 * \param[in] cs_pin     : 用户自定义片选脚
 *
 * \return 无
 */
void demo_amks16z_core_std_spi_flash_dma_entry (void)
{
    int             cs_pin;
    am_spi_handle_t spi_handle;

    cs_pin     = PIOD_0;
    spi_handle = am_kl26_spi0_dma_inst_init();

    demo_std_spi_flash_entry(spi_handle, cs_pin, 0, 24);
}

/** [src_amks16z_core_std_spi_flash_dma] */

/* end of file */
