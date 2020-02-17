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
 * \brief 软件定时器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 软件定时器按照 100ms 的周期中断，并在中断时打印调试信息。
 *
 * \note
 *    1. 测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_SOFTIMER_ENABLE 定义为 1。
 *       但该宏已经默认配置为 1， 用户不必再次配置；
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_std_softimer.c src_mm32l073_core_std_softimer
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_std_softimer
 * \copydoc demo_mm32l073_core_std_softimer.c
 */

/** [src_mm32l073_core_std_softimer] */
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "demo_mm32l073_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_mm32l073_core_std_softimer_entry (void)
{
    AM_DBG_INFO("demo mm32l073_core std softimer!\r\n");

    demo_std_softimer_entry();
}
/** [src_mm32l073_core_std_softimer] */

/* end of file */
