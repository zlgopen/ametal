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
 * \brief WDT 中断演示例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 修改宏定义 __WDT_FEED_TIME_MS 的值，超过 __WDT_TIMEOUT_MS，产生中断，打印" wdt counter underflow !"；
 *   2. 修改宏定义 __WDT_FEED_TIME_MS 的值，小于 __WDT_TIMEOUT_MS，程序不产生wdt中断，打印"normal feed dog !"。
 *
 * \par 源代码
 * \snippet demo_hc32f460_drv_wdt.c src_hc32f460_drv_wdt
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-12
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_drv_wdt
 * \copydoc demo_hc32f460_drv_wdt.c
 */

/** [src_hc32f460_drv_wdt] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f460_wdt.h"
#include "am_hc32f460_inst_init.h"
#include "demo_std_entries.h"
#include "demo_hc32f460_entries.h"

/**
 * \brief 看门狗超时时间
 */
#define __WDT_TIMEOUT_MS       1000

/**
 * \brief 看门狗喂狗时间，若喂狗时间超过WDT_TIMEOUT_MS的值,
 *        会产生看门狗事件。
 */
#define __WDT_FEED_TIME_MS     1200
extern am_hc32f460_wdt_dev_t __g_wdt_dev;
/**
 * \brief 例程入口
 */
void demo_hc32f460_core_drv_wdt_int_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core drv wdt int !\r\n");

    demo_hc32f460_drv_wdt_int_entry(am_hc32f460_wdt_inst_init(),
                                    &__g_wdt_dev,
                                    __WDT_TIMEOUT_MS,
                                    __WDT_FEED_TIME_MS);
}
/** [src_hc32f460_drv_wdt] */

/* end of file */
