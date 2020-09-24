/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief USB keyboard 例程
 *
 * - 实验现象：
 * 1.由于板子资源有限，核心板上只有一个按键可用，必须将核心板上的res/key跳线帽与key短接，这样按键才有效。
 * 2.将USB一端接板子，另一端接PC机。
 * 3.录程序后，等串口打印"usb device init successful."后，按键按下，表示大小写锁定建按下，此时板子上LED0
 *   亮，同时可以敲击键盘验证。
 * 4.程序可更换不同的键值来测试，键盘的键值表在"am_usbd_keyboard.h"中获取。
 *
 * \par 源代码
 * \snippet demo_usbd_keyboard.c src_usbd_keyboard
 *
 * \internal
 * \par Modification History
 * - 1.00 19-2-18  adw, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_usbd_keyboard
 * \copydoc demo_usbd_keyboard.c
 */

/** [src_usbd_keyboard] */
#include "ametal.h"
#include "am_led.h"
#include "string.h"
#include "am_input.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_usbd_keyboard.h"
#include "demo_hc32f07x_entries.h"
#include "demo_std_entries.h"
#include "am_board.h"

#define __KEY0_CODE 82         /*按键键值，区别于键盘键值*/

// 按键事件处理函数
am_local void __input_key_proc (void *p_arg, int key_code, int key_state, int keep_time)
{
    am_usbd_keyboard_handle handle = (am_usbd_keyboard_handle)p_arg;
    uint8_t key_board_val[8] = {0,0,0,0,0,0,0,0};   // 键值数组，大小一般都为8

    if (key_code == __KEY0_CODE) {
        /* 判断按键按下状态是否触发*/
        if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
            key_board_val[2] = AM_USBD_KEY_CAPS_LOCK;

            /* 必须发两次，第一次为键值，第二次清空键值，否则屏幕上一直有字母打印*/
            am_usbd_keyboard_send(handle, key_board_val, 8);

        }else{
            key_board_val[2] = 0;
            am_usbd_keyboard_send(handle, key_board_val, 8);
        }
    }
}

/* 接收回调函数  */
void __g_keybroad_rec_cb(void *p_arg, uint8_t *p_buffer, uint8_t len)
{    
    if ((*p_buffer) & AM_USBD_CAP_LOCK_LIGHT) {   /* 大写锁定 */
        am_led_on(0);
    } else {
        am_led_off(0);
    }
}

/**
 * \brief 例程入口
 */
void demo_usbd_keyboard_entry (void* p_handle)
{
    am_usbd_keyboard_handle handle = p_handle;
    am_input_key_handler_t  key_handler;
    am_usbd_keyboard_recv_cb_set(handle, __g_keybroad_rec_cb, NULL);
    am_input_key_handler_register(&key_handler, __input_key_proc, handle);
    /* 等待键盘枚举成功  */
    while(handle->is_ready == AM_FALSE);
    am_mdelay(30);

    while(1){
        ;
    }
}
/** [src_usbd_keyboard] */

/* end of file */
