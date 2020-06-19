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
 *   1. 串口打印计算结果 ：  The crc-16 init failed
 *
 *                     The CRC-CCITT result is   : 0xa12b
 *
 *                     CRC-32 result is          : 0x9ae0daaf
 *
 * \note
 *    1. 只支持 4 字节对齐的序列(CRC-32)，即：字节数 % 4 == 0；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f460_std_crc.c src_hc32f460_std_crc
 *
 * \internal
 * \par Modification History
 * - 1.00 20-05-18  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_std_crc
 * \copydoc demo_hc32f460_std_crc.c
 */

/** [src_hc32f460_std_crc] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f460_crc.h"
#include "am_hc32f460_inst_init.h"
#include "demo_std_entries.h"
#include "demo_hc32f460_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_std_crc_entry (void)
{
    AM_DBG_INFO("demo amhc32f460_core std crc!\r\n");

    demo_std_crc_entry(am_hc32f460_crc_inst_init());
}
/** [src_hc32f460_std_crc] */

/* end of file */
