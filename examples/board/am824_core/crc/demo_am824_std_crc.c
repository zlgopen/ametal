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
 * \brief CRC 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口打印 "CRC-16 result is     : 0xbb3d\r\n"；
 *   2. 串口打印 "CRC-CCITT result is  : 0x29b1\r\n"；
 *   3. 串口打印 "CRC-32 result is     : 0xcbf43926\r\n"；
 *   4. LED 以 0.5s 的时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_std_crc.c src_am824_std_crc
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-09  sss, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_crc
 * \copydoc demo_am824_std_crc.c
 */

/** [src_am824_std_crc] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_lpc_crc.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_std_crc_entry (void)
{
    am_crc_handle_t crc_handle = NULL;

    am_kprintf("demo am824_core std crc!\r\n");
    crc_handle = am_lpc82x_crc_inst_init();

    demo_std_crc_entry(crc_handle);
}
/** [src_am824_std_crc] */

/* end of file */
