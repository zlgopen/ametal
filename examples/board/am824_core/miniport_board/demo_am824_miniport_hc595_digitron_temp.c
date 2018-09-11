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
 * \brief MiniPort-595 + MiniPort-View + 板载 LM75 例程，通过标准接口实现
 *
 * - 操作步骤
 *   1. 先将 MiniPort-595 扩展板直接与 AM824-Core 的 MiniPort 相连接；
 *   2. 将 MiniPort-View 扩展板与 MiniPort-595 接口相连接。
 *
 * - 实验现象：
 *   1. 数码管显示当前温度；
 *   2. 按下 RES 按键，过一会儿，数码管显示温度值升高。
 *
 * \note
 *    1. 需要短接 J11 和 J13 跳线帽，I2C1 才会与 LM75 连接；
 *    2. 按键需要将 J14 跳线帽的 KEY 和 RES 短接才能使用；
 *
 *
 * \par 源代码
 * \snippet demo_am824_core_miniport_hc595_digitron_temp.c src_am824_core_miniport_hc595_digitron_temp
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_core_miniport_hc595_digitron_temp
 * \copydoc demo_am824_core_miniport_hc595_digitron_temp.c
 */

/** [src_am824_core_miniport_hc595_digitron_temp] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_miniport_hc595_digitron_temp_entry (void)
{
    AM_DBG_INFO("demo am824_core miniport hc595 digitron temp!\r\n");

    am_miniport_view_595_inst_init();

    demo_std_digitron_temp_entry(0, am_temp_lm75_inst_init());
}
/** [src_am824_core_miniport_hc595_digitron_temp] */

/* end of file */
