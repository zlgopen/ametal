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
 * \brief MiniPort-LED 示例程序，仅供参考
 *
 * - 操作步骤
 *   1. 将 MiniPort-LED 板子直接与 底板(AMKS16Z-Core)标号为J4的 MiniPort接口相连接。
 *
 * - 实验现象：
 *   1. 流水灯现象。
 *
 * - 注意：
 *   1. 要使用该例程，需要在am_prj_config.h里面将AM_CFG_LED_ENABLE定义为0。
 *
 * \par 源代码
 * \snippet demo_miniport_led.c src_miniport_led
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_miniport_led
 * \copydoc demo_miniport_led.c
 */

/** [src_miniport_led] */
#include "am_vdebug.h"
#include "am_digitron_disp.h"
#include "demo_std_entries.h"
#include "am_hwconf_miniport_view.h"
#include "am_kl26_inst_init.h"

/*
 * 例程入口
 */
void demo_amks16z_core_miniport_led_entry (void)
{
    AM_DBG_INFO("demo amks16z_core miniport led!\r\n");

    am_miniport_led_inst_init();

    demo_std_led_water_light_entry(2, 9);
}

/** [src_miniport_led] */

/* end of file */
