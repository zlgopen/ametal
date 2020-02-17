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
 * \brief WWDG演示例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 修改宏定义 __WWDG_FEED_TIME_MS 的值，超过 __WWDG_TIMEOUT_MS，芯片复位；
 *   2. 修改宏定义 __WWDG_FEED_TIME_MS 的值，小于 __WWDG_TIMEOUT_MS，程序正常运行。
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_std_wwdg.c src_mm32l073_core_std_wwdg
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-26  sdy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_std_wwdg
 * \copydoc demo_mm32l073_core_std_wwdg.c
 */

/** [src_mm32l073_core_std_wwdg] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_mm32l073_inst_init.h"
#include "demo_std_entries.h"
#include "demo_mm32l073_core_entries.h"

/**
 * \brief 看门狗超时时间
 */
#define __WWDG_TIMEOUT_MS       50

/**
 * \brief 看门狗喂狗时间，若喂狗时间超过WWDG_TIMEOUT_MS的值,
 *        会产生看门狗事件。
 */
#define __WWDG_FEED_TIME_MS     75

/**
 * \brief 例程入口
 */
void demo_mm32l073_core_std_wwdg_entry (void)
{
    AM_DBG_INFO("demo mm32l073_core std wwdg!\r\n");

    demo_std_wdt_entry(am_mm32l073_wwdg_inst_init(),
                       __WWDG_TIMEOUT_MS,
                       __WWDG_FEED_TIME_MS);
}
/** [src_mm32l073_core_std_wwdg] */

/* end of file */
