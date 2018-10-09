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
 * \brief MiniPort-View + MiniPort-KEY 示例程序，仅供参考
 *
 * - 操作步骤
 *   1. 先将 MiniPort-KEY 板子直接与底板(AMKS16Z-Core)标号为J4的 MiniPort 相连接；
 *   2. 将 MiniPort-View 板子与 MiniPort-KEY 接口相连接；
 *   3. 在 am_prj_config.h 里面将 AM_CFG_KEY_GPIO_ENABLE 和 AM_CFG_KEY_ENABLE
 *      定义为1。
 *
 * - 实验现象：
 *   1. 按下一个按键，两位数码管显示按键编号的二进制；
 *   2. 偶数编号时，数码管闪烁，奇数编号时，数码管不闪烁。
 *
 * \par 源代码
 * \snippet demo_miniport_key_digitron.c src_miniport_key_digitron
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-03  nwt, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_miniport_key_digitron
 * \copydoc demo_miniport_key_digitron.c
 */

/** [src_miniport_key_digitron] */
#include "am_vdebug.h"
#include "am_digitron_disp.h"
#include "demo_std_entries.h"
#include "am_hwconf_miniport_view.h"
#include "am_kl26_inst_init.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_miniport_key_digitron_entry (void)
{

    AM_DBG_INFO("demo amks16z_core miniport key digitron!\r\n");

    am_miniport_view_key_inst_init();

    demo_std_key_digitron_entry(0);
}

/** [src_miniport_key_digitron] */

/* end of file */
