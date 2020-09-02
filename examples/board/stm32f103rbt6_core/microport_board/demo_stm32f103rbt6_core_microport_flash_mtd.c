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
 * \brief MicroPort FLASH 例程，通过 MTD 接口实现。
 *
 * 由于 MX25L1606 是典型的 FLASH 存储期间，因此可以使用 MTD 接口读写，
 * 使之与具体器件无关，实现跨平台调用。
 *
 * - 操作步骤:
 *   1. 将 MicroPort FLASH 配板连接到 AM237 的 MicroPort 接口。
 *
 * - 实验现象:
 *   1. 主机写数据到 FLASH；
 *   2. 主机从 FLASH 读取数据，并通过串口打印处理；
 *   3. 串口打印出测试结果。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_microport_flash_mtd.c src_microport_flash_mtd
 *
 * \internal
 * \par Modification history
 * - 1.00  17-11-13  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_microport_flash_mtd
 * \copydoc demo_microport_flash_mtd.c
 */

/** [src_microport_flash_mtd] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg237_inst_init.h"
#include "demo_am237_core_entries.h"

#define __TEST_LENTH      16    /**< \brief 读写字节数 */

/**
 * \brief MicroPort FLASH 例程，通过 MTD 接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_zlg237_core_microport_flash_mtd_entry (void)
{
    AM_DBG_INFO("demo am237_core microport flash mtd!\r\n");

    demo_mtd_entry(am_microport_flash_mtd_inst_init(), 8);
}
/** [src_microport_flash_mtd] */

/* end of file */
