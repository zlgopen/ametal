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
 * \brief MiniPort-View + MiniPort-595 温度显示（使用底板LM75BD读取温度）程序
 *
 * - 操作步骤
 *   1. 要使用底板(AMKS16Z-Core)上的 LM75 温度传感器，需要短接 J11和J13；
 *   2. 先将 MiniPort-595 板子直接与 AMKS16Z-Core 标号为J4的 MiniPort 相连接；
 *   3. 将 MiniPort-View 板子与 MiniPort-595 接口相连接。
 *
 * - 实验现象：
 *   1. 可以看到数码管显示当前温度，若J14跳冒选择短接 KEY 和 RES，则按下底板上的
 *      KEY/RES按键功率电阻会发热，导致温度传感器采集到的温度升高。
 *
 * - 其它说明
 *   1. 具体如何使用I2C读取LM75的温度，可以使用查看I2C下的相关例程
 *
 * \par 源代码
 * \snippet demo_miniport_hc595_digitron_temp.c src_miniport_hc595_digitron_temp
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_miniport_hc595_digitron_temp
 * \copydoc demo_miniport_hc595_digitron_temp.c
 */

/** [src_miniport_hc595_digitron_temp] */
#include "am_vdebug.h"
#include "am_digitron_disp.h"
#include "demo_std_entries.h"
#include "am_hwconf_miniport_view.h"
#include "am_kl26_inst_init.h"

/* 例程入口 */
void demo_amks16z_core_miniport_hc595_digitron_temp_entry (void)
{
    AM_DBG_INFO("demo amks16z_core miniport hc595 digitron temp!\r\n");

    am_miniport_view_595_inst_init();

    demo_std_digitron_temp_entry(0, am_temp_lm75_inst_init());
}

/** [src_miniport_hc595_digitron_temp] */

/* end of file */
