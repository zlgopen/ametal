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
 * \brief WDT 中断演示例程，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. feed_time_ms 超过 time_out_ms，产生中断，打印" wdt counter underflow !"；
 *   2. feed_time_ms 小于 time_out_ms，程序不产生wdt中断，打印"normal feed dog !"。
 *
 * \par 源代码
 * \snippet demo_std_wdt.c src_std_wdt
 *
 * \internal
 * \par Modification history
 * - 1.00 20-06-03  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_drv_wdt
 * \copydoc demo_drv_wdt.c
 */

/** [src_drv_wdt] */
#include "ametal.h"
#include "am_wdt.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_hc32f460_wdt.h"

static uint8_t g_wdt_status_flag = 0xFF;

/**
 * \brief WDT 中断服务程序
 */
static void wdt_isr (void *p_arg , uint32_t flag)
{
    if (flag == AM_HC32F460_WDT_STATUS_REFRESH_ERROR) {
        g_wdt_status_flag = AM_HC32F460_WDT_STATUS_REFRESH_ERROR;
    } else if (flag == AM_HC32F460_WDT_STATUS_COUNT_UNDERFLOW) {
        g_wdt_status_flag = AM_HC32F460_WDT_STATUS_COUNT_UNDERFLOW;
    }
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_drv_wdt_int_entry (am_wdt_handle_t         handle,
                                      am_hc32f460_wdt_dev_t  *p_dev,
                                      uint32_t                time_out_ms,
                                      uint32_t                feed_time_ms)
{
    am_wdt_info_t info;

    AM_DBG_INFO("The chip Reset by External Reset Pin or WDT \r\n");

    am_wdt_info_get(handle, &info);

    AM_DBG_INFO("The WDT support min time is %d ms\r\n", info.min_timeout_ms);
    AM_DBG_INFO("The WDT support max time is %d ms\r\n", info.max_timeout_ms);

    am_hc32f460_wdt_isr_connect(p_dev, wdt_isr, (void *)0);

    am_wdt_enable(handle, time_out_ms);

    while (1) {

        /* 喂狗操作 */
        am_wdt_feed(handle);

        /* 延时，当延时大于喂狗时间时,会产生看门狗事件，MCU 复位 */
        am_mdelay(feed_time_ms);

        if (g_wdt_status_flag == AM_HC32F460_WDT_STATUS_COUNT_UNDERFLOW) {
            AM_DBG_INFO("wdt counter underflow !\r\n");
        } else if (g_wdt_status_flag == AM_HC32F460_WDT_STATUS_REFRESH_ERROR) {
            AM_DBG_INFO("wdt counter refresh error !\r\n");
        } else {
            AM_DBG_INFO("normal feed dog !\r\n");
        }

    }
}
/** [src_drv_wdt] */

/* end of file */
