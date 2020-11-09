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
 * \brief 按键例程
 *
 * 本例程展示了按键处理的一般流程
 *
 * - 实验现象：
 *   1. 系统中任意按键按下，调试串口打印按键事件。
 *
 * \par 源代码
 * \snippet demo_std_key.c src_std_key
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-8  hbt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_key
 * \copydoc demo_std_key.c
 */

/** [src_std_key] */
#include "ametal.h"
#include "am_input.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**
 * \brief 按键事件回调函数
 */
am_local void __input_key_proc (void *p_arg, int key_code, int key_state, int keep_time)
{
    if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
        AM_DBG_INFO("key_code: %d Press! Keep_time is %d\r\n", key_code, keep_time);
    } else if (key_state == AM_INPUT_KEY_STATE_RELEASED){
        AM_DBG_INFO("key_code: %d Released! \r\n", key_code);
    }
}

/**
 * \brief 例程入口
 */
void demo_std_key_entry (void)
{
    am_local am_input_key_handler_t key_handler;

    am_input_key_handler_register(&key_handler, __input_key_proc, NULL);

    AM_FOREVER {
        am_mdelay(10);
        ; /* VOID */
    }
}
/** [src_std_key] */

/* end of file */
