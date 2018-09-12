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
 *   1. 将 MiniPort-KEY 板子直接与 AM116-Core 的 MiniPort接口相连接。
 *
 * - 实验现象：
 *   1. 按下按键时，调试串口打印当前按键信息。
 *
 * \note
 *    1. 测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE 定义为 1，
 *       但该宏已经默认配置为 1， 用户不必再次配置；
 *    2. LED0/LED1 需要短接 J9/J10 跳线帽，才能分别被 PIO0_20/PIO0_21 控制；
 *    3. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_core_miniport_key.c src_am824_core_miniport_key
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

/** [src_am824_core_miniport_key] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_miniport_key_entry (void)
{
    AM_DBG_INFO("demo am824_core miniport key!\r\n");

    am_miniport_key_inst_init();

    demo_std_4key_entry();
}
/** [src_am824_core_miniport_key] */

/* end of file */
