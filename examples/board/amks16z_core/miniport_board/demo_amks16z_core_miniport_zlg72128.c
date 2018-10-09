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
 * \brief MiniPort zlg72128，仅供参考
 *
 * - 操作步骤
 *   1. 先将MiniPort zlg72128板子直接与 底板(AMKS16Z-Core)标号为J4的 MiniPort 相连接；
 *   2. 在 am_prj_config.h 里面将 AM_CFG_KEY_GPIO_ENABLE 和 AM_CFG_KEY_ENABLE
 *      定义为1。
 *
 * - 实验现象：
 *   1. 按下一个按键，两位数码管显示按键编号的二进制；
 *   2. 偶数编号时，数码管闪烁，奇数编号时，数码管不闪烁。
 *
 * \par 源代码
 * \snippet demo_miniport_zlg72128.c src_miniport_zlg72128
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-03  nwt, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_miniport_zlg72128
 * \copydoc demo_miniport_zlg72128.c
 */

/** [src_miniport_zlg72128] */
#include "am_vdebug.h"
#include "am_digitron_disp.h"
#include "demo_std_entries.h"
#include "am_hwconf_miniport_view.h"
#include "am_kl26_inst_init.h"

/* 例程入口 */
void demo_amks16z_core_miniport_zlg72128_entry (void)
{
    AM_DBG_INFO("demo am116_core miniport zlg72128!\r\n");

    am_miniport_zlg72128_inst_init();

    demo_std_key_digitron_rngbuf_entry(0);
}

/** [src_miniport_zlg72128] */

/* end of file */
