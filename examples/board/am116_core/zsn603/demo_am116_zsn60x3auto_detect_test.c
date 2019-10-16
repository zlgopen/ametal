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
 * \brief zsn603 自动检测模式进行测试
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口；
 *   2. 正确连接好天线；
 *   3. 将卡片置于感应区
 *
 * - 实验现象：
 *   1. 将卡片放置天线感应区后串口打印响应信息；
 *
 * \par 源代码
 * \snippet demo_am116_zsn603_auto_detect_test.c src_am116_zsn603_auto_detect_test
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-18  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_am116_if_zsn603_auto_detect_test
 * \copydoc demo_am116_zsn603_auto_detect_test.c
 */

/** [src_am116_zsn603_auto_detect_test] */

#include "zsn603.h"
#include "demo_components_entries.h"
#include "demo_am116_core_entries.h"
#include "am_hwconf_zsn603_uart.h"

void demo_am116_zsn603_auto_detect_test_entry()
{
    zsn603_handle_t  handle = am_zsn603_uart_inst_init();
    demo_zsn603_auto_detect_test_entry(handle);
}
/** [src_am116_zsn603_auto_detect_test] */

/* end of file */
