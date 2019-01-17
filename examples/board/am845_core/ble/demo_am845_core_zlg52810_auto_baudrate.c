/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 蓝牙模块自动波特率例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 单片机分别尝试通过 4800、9600、19200、38400、57600、115200、230400、
 *      460800、1000000连接ZLG52810；
 *
 *   2. 如果连接成功，通过串口打印当前的波特率并按 500 ms 的间隔慢闪烁 LED0，否则
 *      LED0 按 100 ms 的间隔快闪烁。

 * \par 源代码
 * \snippet demo_am845_zlg52810_baudrate.c src_am845_zlg52810_baudrate
 *
 * \internal
 * \par Modification history
 * - 1.00 19-01-24  htf, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_zlg52810_baudrate
 * \copydoc demo_am845_zlg52810_baudrate.c
 */

/** [src_am845_zlg52810_baudrate] */
#include "ametal.h"
#include "am_lpc84x.h"
#include "am_zlg52810.h"
#include "am_hwconf_zlg52810.h"
#include "demo_components_entries.h"
#include "demo_am845_core_entries.h"

void demo_am845_core_zlg52810_auto_baudrate_entry()
{
    am_zlg52810_handle_t  handle = am_zlg52810_inst_init();

    demo_zlg52810_baudrate_entry(handle);
}


/** [src_am845_zlg52810_baudrate] */

/* end of file */

