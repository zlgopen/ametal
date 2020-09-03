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
 * \snippet demo_stm32f103rbt6_core_usbd_keyboard.c src_stm32f103rbt6_core_usbd_keyboard
 *
 * \internal
 * \par Modification History
 * - 1.00 19-05-28  htf, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_demo_stm32f103rbt6_core_usbd_keyboard
 * \copydoc demo_stm32f103rbt6_core_usbd_keyboard.c
 */

/** [src_demo_stm32f103rbt6_core_usbd_keyboard] */

#include "demo_stm32f103rbt6_entries.h"
#include "am_stm32f103rbt6_inst_init.h"
#include "am_usbd_keyboard.h"
#include "demo_stm32f103rbt6_entries.h"
#include "demo_stm32f103rbt6_core_entries.h"

void demo_stm32f103rbt6_usbd_keyboard_entry (void)
{

    AM_DBG_INFO("demo stm32f103rbt6_core usbd keyboard!\r\n");
    /* usb打印机实例初始化*/
    am_usbd_keyboard_handle handle = am_stm32f103rbt6_usbd_keyboard_inst_init();

    demo_usbd_keyboard_entry(handle);
}

/** [src_demo_stm32f103rbt6_core_usbd_keyboard] */

/* end of file */
