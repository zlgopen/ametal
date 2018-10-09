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
 * \brief MiniPort-LED 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 MiniPort-LED 扩展板直接与 AM824-Core 的 MiniPort 接口相连接。
 *
 * - 实验现象：
 *   1. 流水灯现象。
 *
 * \par 源代码
 * \snippet demo_am824_core_miniport_led.c src_am824_core_miniport_led
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_core_miniport_led
 * \copydoc demo_am824_core_miniport_led.c
 */

/** [src_miniport_led] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_core_miniport_led_entry (void)
{
    AM_DBG_INFO("demo am824_core miniport led!\r\n");

    am_miniport_led_inst_init();

    demo_std_led_water_light_entry (2, 8);
}
/** [src_am824_core_miniport_led] */

/* end of file */
