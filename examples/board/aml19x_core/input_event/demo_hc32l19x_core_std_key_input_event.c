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
 * \brief 按键输入演示例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 J14 的 KEY 和 PIOA_7 短接在一起。
 *
 * - 实验现象:
 *   1. 调试串口打印按键事件。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *    2. 测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE、
 *       AM_CFG_KEY_ENABLE 和 AM_CFG_SOFTIMER_ENABLE 定义为 1。但这些宏
 *       已经默认配置为 1， 用户不必再次配置；
 *
 * \par 源代码
 * \snippet demo_hc32l19x_std_key_input_event.c src_hc32l19x_std_key_input_event
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-27  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l19x_std_key_input_event
 * \copydoc demo_hc32l19x_std_key_input_event.c
 */

/** [src_hc32l19x_std_key_input_event] */
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "demo_aml19x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32l19x_core_std_key_input_event_entry (void)
{
    AM_DBG_INFO("demo aml19x_core std input event!\r\n");

    demo_std_key_entry();
}
/** [src_hc32l19x_std_key_input_event] */

/* end of file */
