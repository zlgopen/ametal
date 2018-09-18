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
 * \brief 按键输入例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 调试串口打印按键事件。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 使用按键需要将 J14 跳线帽的 KEY 和 PIO0_1 短接，且必须在 am_prj_config.h
 *       内将 AM_CFG_KEY_GPIO_ENABLE 定义为 1，但该宏已经默认配置为 1， 用户不必再
 *       次配置；
 *    3. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_std_key_input_event.c src_am824_std_key_input_event
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-8  hbt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_key_input_event
 * \copydoc demo_am824_std_key_input_event.c
 */

/** [src_am824_std_key_input_event] */
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_core_std_key_input_event_entry (void)
{
    AM_DBG_INFO("demo am824 std input event!\r\n");

    demo_std_key_entry();
}
/** [src_am824_std_key_input_event] */

/* end of file */
