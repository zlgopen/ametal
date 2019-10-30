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
 * \brief MiniPort-KEY 例程，通过标准接口实现
 *
 * - 操作步骤
 *   1. 将 MiniPort-KEY 板子直接与 AM118BLE 的 MiniPort接口相连接。
 *
 * - 实验现象：
 *   1. 若连接了调试串口，则按下按键时，会打印出当前的按键信息；
 *   2. 按键按下，同时用 AM118BLE 板上的LED灯反应出当前的按键编号，
 *      需要短接 J9 和 J10。
 *
 *           ------------------------------------
 *            按下按键        | LED1状态 |  LED0状态
 *           -------------|---------|------------
 *             KEY0       |    熄灭   |   熄灭
 *             KEY1       |    熄灭   |   点亮
 *             KEY2       |    点亮   |   熄灭
 *             KEY3       |    点亮   |   点亮
 *
 * \note
 *    测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE、
 *    AM_CFG_KEY_ENABLE 和 AM_CFG_SOFTIMER_ENABLE 定义为 1。但这些宏
 *    已经默认配置为 1， 用户不必再次配置。
 *
 * \par 源代码
 * \snippet demo_miniport_key.c src_miniport_key
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_miniport_key
 * \copydoc demo_miniport_key.c
 */

/** [src_miniport_key] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_event_category_input.h"
#include "am_event_input_key.h"
#include "am_zlg118_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am118_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg118_core_miniport_key_entry (void)
{
    AM_DBG_INFO("demo am118_core miniport key!\r\n");

    am_miniport_key_inst_init();

    demo_std_4key_entry();
}
/** [src_miniport_key] */

/* end of file */
