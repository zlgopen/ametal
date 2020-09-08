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
 * \brief WWDG 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 修改宏定义 __WWDG_FEED_TIME_MS 的值，超过 __WWDG_TIMEOUT_MS，芯片复位；
 *   2. 修改宏定义 __WWDG_FEED_TIME_MS 的值，小于 __WWDG_TIMEOUT_MS，程序正常运行。
 *
 * \par 源代码
 * \snippet demo_zlg116_hw_wwdg.c src_zlg116_hw_wwdg
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-26  sdy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg116_hw_wwdg
 * \copydoc demo_zlg116_hw_wwdg.c
 */

/** [src_zlg116_hw_wwdg] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
//#include "am_zlg116.h"
#include "hw/amhw_stm32f103rbt6_wwdg.h"

static uint8_t __g_count = 0x7f;

/**
 * \brief 看门狗使能
 *
 * \param[in] timeout_ms : 超时时间值，单位：ms
 *
 * \return 无
 *
 */
static int __stm32f103rbt6_wwdg_enable (amhw_stm32f103rbt6_wwdg_t *p_hw_wwdg,
                              uint32_t         timeout_ms,
                              uint32_t         clk_rate)
{
    uint32_t wdt_freq = clk_rate / 4096;
    uint32_t ticks;
    uint32_t div = 1;

    ticks = (uint64_t) (timeout_ms) * wdt_freq / 1000;

    div = ticks / 0x40 + 1;

    amhw_stm32f103rbt6_wwdg_winvalue_set (p_hw_wwdg, 0x7f);

    if (div <= 1) {
        amhw_stm32f103rbt6_wwdg_timerbase_set(p_hw_wwdg, 0);
        div = 1;
    } else if (div <= 2) {
        amhw_stm32f103rbt6_wwdg_timerbase_set (p_hw_wwdg, 1);
        div = 2;
    } else if (div <= 4) {
        amhw_stm32f103rbt6_wwdg_timerbase_set (p_hw_wwdg, 2);
        div = 4;
    } else if (div <= 8) {
        amhw_stm32f103rbt6_wwdg_timerbase_set (p_hw_wwdg, 3);
        div = 8;
    } else {
        return AM_ERROR;
    }

    wdt_freq /= div;
    ticks = (uint64_t) (timeout_ms) * wdt_freq / 1000;
    __g_count = (ticks + 0x3f) % 0x7f;

    amhw_stm32f103rbt6_wwdg_counter_set (p_hw_wwdg, __g_count);

    /* 启动看门狗 */
    amhw_stm32f103rbt6_wwdg_enable(p_hw_wwdg);

    return AM_OK;
}

/**
 * \brief 看门狗喂狗
 *
 * \return 无
 *
 */
static void __stm32f103rbt6_sdt_feed (amhw_stm32f103rbt6_wwdg_t *p_hw_wwdg)
{
    amhw_stm32f103rbt6_wwdg_counter_set(p_hw_wwdg, __g_count);
}

/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_hw_wwdg_entry (amhw_stm32f103rbt6_wwdg_t *p_hw_wwdg,
                             uint32_t         clk_rate,
                             uint32_t         time_out_ms,
                             uint32_t         feed_time_ms)
{
    __stm32f103rbt6_wwdg_enable(p_hw_wwdg, time_out_ms, clk_rate);

    while (1) {

        /* 喂狗操作 */
        __stm32f103rbt6_sdt_feed(p_hw_wwdg);

        /* 延时，当延时大于喂狗时间时,会产生看门狗事件，MCU复位 */
        am_mdelay(feed_time_ms);
    }
}
/** [src_zlg116_hw_wwdg] */

/* end of file */
