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
 * \brief SD卡多块读写例程
 *
 * - 操作步骤：
 *   1. 将SD卡插入卡槽
 *
 * - 实验现象：
 *   1. 擦除SD卡指定块；
 *   2. 写入块数据；
 *   2. 读出刚才写入的数据；
 *   3. 调试串口打印测试结果。
 *
 * \par 源代码
 * \snippet demo_hc32f460_core_std_sdcard.c src_hc32f460_core_std_sdcard
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-14  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_core_std_sdcard
 * \copydoc demo_hc32f460_core_std_sdcard.c
 */

/** [src_hc32f460_core_std_sdcard] */
#include "ametal.h"
#include "am_sdio.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_sdcard.h"
#include "demo_std_entries.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_std_sdcard_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core std sdcard!\r\n");

    demo_std_sdcard_entry(am_sdcard1_inst_init());
}
/** [src_hc32f460_core_std_sdcard] */

/* end of file */
