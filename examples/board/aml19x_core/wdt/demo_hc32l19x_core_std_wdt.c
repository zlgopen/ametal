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
 * \brief WDT演示例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 修改宏定义 __WDT_FEED_TIME_MS 的值，超过 __WDT_TIMEOUT_MS，芯片复位；
 *   2. 修改宏定义 __WDT_FEED_TIME_MS 的值，小于 __WDT_TIMEOUT_MS，程序正常运行。
 *
 * \par 源代码
 * \snippet demo_hc32l19x_std_wdt.c src_hc32l19x_std_wdt
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-10
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l19x_std_wdt
 * \copydoc demo_hc32l19x_std_wdt.c
 */

/** [src_hc32l19x_std_wdt] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32_wdt.h"
#include "am_hc32l19x_inst_init.h"
#include "demo_std_entries.h"
#include "demo_aml19x_core_entries.h"

/**
 * \brief 看门狗超时时间
 */
#define __WDT_TIMEOUT_MS       HC32_WDT_51MS     /* 最大52.4s */

/**
 * \brief 看门狗喂狗时间，若喂狗时间超过WDT_TIMEOUT_MS的值,
 *        会产生看门狗事件。
 */
#define __WDT_FEED_TIME_MS     10

/**
 * \brief 例程入口
 */
void demo_hc32l19x_core_std_wdt_entry (void)
{
    AM_DBG_INFO("demo aml19x_core std wdt!\r\n");

    demo_std_wdt_entry(am_hc32_wdt_inst_init(),
                       __WDT_TIMEOUT_MS,
                       __WDT_FEED_TIME_MS);
}
/** [src_hc32l19x_std_wdt] */

/* end of file */
