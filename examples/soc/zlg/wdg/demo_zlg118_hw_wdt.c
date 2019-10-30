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
 * \brief WDT 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 修改宏定义 __WDT_FEED_TIME_MS 的值，超过 __WDT_TIMEOUT_MS，芯片复位；
 *   2. 修改宏定义 __WDT_FEED_TIME_MS 的值，小于 __WDT_TIMEOUT_MS，程序正常运行。
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_wdt.c src_zlg118_hw_wdt
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-10
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_wdt
 * \copydoc demo_zlg118_hw_wdt.c
 */

/** [src_zlg118_hw_wdt] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_zlg118.h"
#include "am_zlg118_wdt.h"

#define ZLG118_WDT_RST_COUNT1   (0x1e)
#define ZLG118_WDT_RST_COUNT2   (0xe1)

/**
 * \brief 看门狗使能
 *
 * \param[in] timeout_ms : 超时时间值，单位：ms
 *
 * \return 无
 *
 */
static int __zlg118_wdt_enable (amhw_zlg118_wdt_t *p_hw_wdt,
                                uint32_t           timeout_ms)
{
    if (timeout_ms > ZLG118_WDT_52_4S) {
        return AM_ERROR;
    }

    /* 设置溢出时间 */
    amhw_zlg118_wdt_wov_set(p_hw_wdt, (uint8_t)timeout_ms);

    /* 设置溢出后复位 */
    amhw_zlg118_wdt_en_set(p_hw_wdt, 0);

    /* 启动看门狗 */
    amhw_zlg118_wdt_rst_set(p_hw_wdt, ZLG118_WDT_RST_COUNT1);
    amhw_zlg118_wdt_rst_set(p_hw_wdt, ZLG118_WDT_RST_COUNT2);

    return AM_OK;
}

/**
 * \brief 看门狗喂狗
 *
 * \return 无
 *
 */
static void __zlg_sdt_feed (amhw_zlg118_wdt_t *p_hw_wdt)
{
    amhw_zlg118_wdt_rst_set(p_hw_wdt, ZLG118_WDT_RST_COUNT1);
    amhw_zlg118_wdt_rst_set(p_hw_wdt, ZLG118_WDT_RST_COUNT2);
}

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_wdt_entry (void     *p_hw_wdt,
                               uint32_t  time_out_ms,
                               uint32_t  feed_time_ms)
{
    __zlg118_wdt_enable((amhw_zlg118_wdt_t *)p_hw_wdt, time_out_ms);

    while (1) {

        /* 喂狗操作 */
        __zlg_sdt_feed(p_hw_wdt);

        /* 延时，当延时大于喂狗时间时,会产生看门狗事件，MCU复位 */
        am_mdelay(feed_time_ms);
    }
}
/** [src_zlg118_hw_wdt] */

/* end of file */
