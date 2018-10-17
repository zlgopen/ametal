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
 * \brief 按键例程（4个按键）
 *
 * 本例程展示了 4个按键的按下与释放处理（仅打印信息作为处理示例），4个按键对应的
 * 按键编码固定为：KEY_0、KEY_1、KEY_2、KEY_3。运行该例程时，应确保系统有4个按键，
 * 且它们对应的按键编码为：KEY_0 ~ KEY_4
 *
 * - 操作步骤
 *   1. 调用入口函数
 *
 * - 实验现象：
 *   1. 按下按键时，调试串口打印当前按键信息。
 *
 * \par 源代码
 * \snippet demo_std_4key.c src_std_4key
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_4key
 * \copydoc demo_std_4key.c
 */

/** [src_std_4key] */
#include "ametal.h"
#include "am_delay.h"
#include "am_input.h"
#include "am_vdebug.h"
#include "am_event_input_key.h"
#include "am_event_category_input.h"

static void __input_key_proc (void *p_arg, int key_code, int key_state, int keep_time)
{
    switch (key_code) {

    case KEY_0:
        if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
            AM_DBG_INFO("key0 Press! Keep_time is %d\r\n", keep_time);
        } else if (key_state == AM_INPUT_KEY_STATE_RELEASED){
            AM_DBG_INFO("key0 Released! \r\n");
        }

        break;

    case KEY_1:
        if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
            AM_DBG_INFO("key1 Press! Keep_time is %d\r\n", keep_time);
        } else if (key_state == AM_INPUT_KEY_STATE_RELEASED){
            AM_DBG_INFO("key1 Released! \r\n");
        }

        break;

    case KEY_2:
        if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
            AM_DBG_INFO("key2 Press! Keep_time is %d\r\n", keep_time);
        } else if (key_state == AM_INPUT_KEY_STATE_RELEASED){
            AM_DBG_INFO("key2 Released! \r\n");
        }

        break;

    case KEY_3:
        if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
            AM_DBG_INFO("key3 Press! Keep_time is %d\r\n", keep_time);
        } else if (key_state == AM_INPUT_KEY_STATE_RELEASED){
            AM_DBG_INFO("key3 Released! \r\n");
        }

        break;

    default :
        break;
    }
 }

/**
 * \brief 例程入口
 */
void demo_std_4key_entry (void)
{
    static am_input_key_handler_t key_handler;

    am_input_key_handler_register(&key_handler, __input_key_proc, NULL);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_4key] */

/* end of file */
