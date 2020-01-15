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
 * \brief MiniPort-View 例程，通过标准接口实现
 *
 * - 操作步骤
 *   1. 将 MiniPort-View 板子直接与 AM116-Core 的 MiniPort 接口相连接。
 *
 * - 实验现象：
 *   1. 可以看到数值 0 ~ 59 的秒计数器；
 *   2. 数组小于30时，个位闪烁，大于30时十位闪烁。
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_miniport_digitron.c src_mm32l073_core_miniport_digitron
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_miniport_digitron
 * \copydoc demo_mm32l073_core_miniport_digitron.c
 */

/** [src_mm32l073_core_miniport_digitron] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_mm32l073_inst_init.h"
#include "demo_std_entries.h"
#include "demo_mm32l073_core_entries.h"

/**
 * \brief 例程入口 
 */
void demo_mm32l073_core_miniport_digitron_entry (void)
{
    AM_DBG_INFO("demo mm32l073_core miniport digitron!\r\n");

    am_miniport_view_inst_init();

    demo_std_digitron_60s_counting_entry(0);
}
/** [src_mm32l073_core_miniport_digitron] */

/* end of file */
