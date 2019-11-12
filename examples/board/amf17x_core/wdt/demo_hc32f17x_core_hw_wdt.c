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
 * \snippet demo_hc32f17x_hw_wdt.c src_hc32f17x_hw_wdt
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-10
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f17x_hw_wdt
 * \copydoc demo_hc32f17x_hw_wdt.c
 */

/** [src_hc32f17x_hw_wdt] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32.h"
#include "am_hc32_clk.h"
#include "am_hc32_wdt.h"
#include "demo_hc32_entries.h"
#include "demo_amf17x_core_entries.h"

/**
 * \brief 看门狗超时时间
 */
#define __WDT_TIMEOUT_MS       HC32_WDT_500MS

/**
 * \brief 看门狗喂狗时间，若喂狗时间超过WDT_TIMEOUT_MS的值芯片会产生复位
 */
#define __WDT_FEED_TIME_MS     600


/**
 * \brief 例程入口
 */
void demo_hc32f17x_core_hw_wdt_entry (void)
{
    AM_DBG_INFO("demo amf17x_core hw wdt!\r\n");

    am_clk_enable(CLK_WDT);

    demo_hc32_hw_wdt_entry(HC32_WDT,
                             __WDT_TIMEOUT_MS,
                             __WDT_FEED_TIME_MS);
}
/** [src_hc32f17x_hw_wdt] */

/* end of file */
