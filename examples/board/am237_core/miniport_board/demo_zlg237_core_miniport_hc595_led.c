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
 * \brief MiniPort-LED + MiniPort-595 例程，通过标准接口实现
 *
 * - 操作步骤
 *   1. 请确认am_hwconf_zlg237_spi_int。c中spi2设备信息结构体中cs_delay变量值为0。
 *   2. 先将 MiniPort-595 板子直接与 AM237 的 MiniPort 相连接；
 *   3. 将 MiniPort-LED 板子与 MiniPort-595 接口相连接。
 *
 * - 实验现象：
 *   1. 流水灯现象。
 *
 * \par 源代码
 * \snippet demo_miniport_hc595_led.c src_miniport_hc595_led
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_miniport_hc595_led
 * \copydoc demo_miniport_hc595_led.c
 */

/** [src_miniport_hc595_led] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg237_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg237_core_miniport_hc595_led_entry (void)
{
    AM_DBG_INFO("demo am237_core miniport hc595 led!\r\n");

    demo_std_hc595_led_entry(am_miniport_595_inst_init());
}
/** [src_miniport_hc595_led] */

/* end of file */
