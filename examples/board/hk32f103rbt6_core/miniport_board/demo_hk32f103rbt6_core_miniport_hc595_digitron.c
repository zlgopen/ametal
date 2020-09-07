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
 * \brief MiniPort-View + MiniPort-595 例程，通过标准接口实现
 *
 * - 操作步骤
 *   1. 请确认am_hwconf_hk32f103rbt6_spi_int。c中spi2设备信息结构体中cs_delay变量值为0。
 *   2. 请确认am_hwconf_hk32f103rbt6_spi_int。c中spi2设备信息结构体中cs_delay变量值为0。
 *   3. 先将 MiniPort-595 板子直接与 HK32F103RBT6 的 MiniPort 相连接；
 *   4. 将 MiniPort-View 板子与 MiniPort-595 接口相连接。
 *
 * - 实验现象：
 *   1. 可以看到数值 0 ~ 59 的秒计数器；
 *   2. 数组小于 30 时，个位闪烁，大于 30 时十位闪烁。
 *
 * \par 源代码
 * \snippet demo_miniport_hc595_digitron.c src_miniport_hc595_digitron
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_miniport_hc595_digitron
 * \copydoc demo_miniport_hc595_digitron.c
 */

/** [src_miniport_hc595_digitron] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_arm_nvic.h"
#include "am_hk32f103rbt6_inst_init.h"
#include "am_digitron_disp.h"
#include "demo_hk32f103rbt6_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hk32f103rbt6_core_miniport_hc595_digitron_entry (void)
{
    AM_DBG_INFO("demo hk32f103rbt6_core miniport hc595 digitron!\r\n");

    am_miniport_view_595_inst_init();

    demo_std_digitron_60s_counting_entry(0);
}
/** [src_miniport_hc595_digitron] */

/* end of file */
