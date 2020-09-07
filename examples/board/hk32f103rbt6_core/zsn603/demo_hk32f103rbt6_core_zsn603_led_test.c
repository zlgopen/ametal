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
 * \brief zsn603 LED测试程序
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口；
 *
 * - 实验现象：
 *   1. ZSN600的LED灯将会闪烁，若执行错误则会打印响应的错误标识号 ；
 *
 * \par 源代码
 * \snippet demo_hk32f103rbt6_core_zsn603_led_test.c src_hk32f103rbt6_core_zsn603_led_test
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-18  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_hk32f103rbt6_if_zsn603_led_test
 * \copydoc demo_hk32f103rbt6_core_zsn603_led_test.c
 */

/** [src_hk32f103rbt6_core_zsn603_led_test] */

#include "zsn603.h"
#include "demo_components_entries.h"
#include "demo_hk32f103rbt6_core_entries.h"
#include "am_hwconf_zsn603_uart.h"

void demo_hk32f103rbt6_core_zsn603_led_test_entry()
{
    zsn603_handle_t  handle = am_zsn603_uart_inst_init();
    demo_zsn603_led_test_entry(handle);
}
/** [src_hk32f103rbt6_core_zsn603_led_test] */

/* end of file */
