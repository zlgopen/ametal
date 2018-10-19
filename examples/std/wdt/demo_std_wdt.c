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
 * \brief WDT 演示例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. feed_time_ms 超过 time_out_ms，芯片复位；
 *   2. feed_time_ms 小于 time_out_ms，程序正常运行。
 *
 * \par 源代码
 * \snippet demo_std_wdt.c src_std_wdt
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-26  sdy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_wdt
 * \copydoc demo_std_wdt.c
 */

/** [src_std_wdt] */
#include "ametal.h"
#include "am_wdt.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**
 * \brief 例程入口
 */
void demo_std_wdt_entry (am_wdt_handle_t handle,
                         uint32_t        time_out_ms,
                         uint32_t        feed_time_ms)
{
    am_wdt_info_t info;

    AM_DBG_INFO("The chip Reset by External Reset Pin or WDT \r\n");

    am_wdt_info_get(handle, &info);

    AM_DBG_INFO("The WDT support min time is %d ms\r\n", info.min_timeout_ms);
    AM_DBG_INFO("The WDT support max time is %d ms\r\n", info.max_timeout_ms);

    am_wdt_enable(handle, time_out_ms);

    while (1) {

        /* 喂狗操作 */
        am_wdt_feed(handle);

        /* 延时，当延时大于喂狗时间时,会产生看门狗事件，MCU 复位 */
        am_mdelay(feed_time_ms);

    }
}
/** [src_std_wdt] */

/* end of file */
