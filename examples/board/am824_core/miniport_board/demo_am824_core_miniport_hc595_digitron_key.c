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
 * \brief MiniPort-595 + MiniPort-View + MiniPort-KEY 例程，通过标准接口实现
 *
 * - 操作步骤
 *   1. 先将 MiniPort-595 扩展板直接与 AM824-Core 的 MiniPort 相连接；
 *   2. 将 MiniPort-View 扩展板与 MiniPort-595 接口相连接；
 *   3. 将 MiniPort-KEY 与 MiniPort-View 连接。
 *
 * - 实验现象：
 *   1. 按下一个按键，两位数码管显示按键编号的二进制；
 *   2. 偶数编号时，数码管闪烁，奇数编号时，数码管不闪烁。
 *
 * \note
 *    测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE 定义为 1，
 *    但该宏已经默认配置为 1， 用户不必再次配置。
 *
 * \par 源代码
 * \snippet demo_am824_core_miniport_hc595_digitron_key.c src_am824_core_miniport_hc595_digitron_key
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_core_miniport_hc595_digitron_key
 * \copydoc demo_am824_core_miniport_hc595_digitron_key.c
 */

/** [src_am824_core_miniport_hc595_digitron_key] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_core_miniport_hc595_digitron_key_entry (void)
{
    AM_DBG_INFO("demo am824_core miniport hc595 digitron key!\r\n");

    am_miniport_view_key_595_inst_init();

    demo_std_key_digitron_entry(0);
}
/** [src_am824_core_miniport_hc595_digitron_key] */

/* end of file */
