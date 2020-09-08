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
 * \brief IWDG 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 修改宏定义 __IWDG_FEED_TIME_MS 的值，超过 1500ms(存在 5ms 误差)，芯片复位；
 *   2. 修改宏定义 __IWDG_FEED_TIME_MS 的值，小于 1500ms(存在 5ms 误差)，程序正常运行。
 *
 * \par 源代码
 * \snippet demo_stm32f103rbt6_hw_iwdg.c src_stm32f103rbt6_hw_iwdg
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-26  sdy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_stm32f103rbt6_hw_iwdg
 * \copydoc demo_stm32f103rbt6_hw_iwdg.c
 */

/** [src_stm32f103rbt6_hw_iwdg] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "hw/amhw_stm32f103rbt6_iwdg.h"

/**
 * \brief 看门狗使能
 *
 * \param[in] timeout_ms : 超时时间值，单位：ms
 *
 * \return 无
 *
 */
static void __stm32f103rbt6_iwdg_enable (amhw_stm32f103rbt6_iwdg_t *p_hw_iwdg, uint32_t timeout_ms)
{
    uint32_t wdt_freq = 40000;
    uint32_t ticks;
    uint32_t div = 1;

    ticks = (uint64_t) (timeout_ms) * wdt_freq / 1000;

    div = ticks / 0xFFF + 1;

    amhw_stm32f103rbt6_iwdg_keyvalue_set(p_hw_iwdg, 0x5555);

    while(amhw_stm32f103rbt6_iwdg_status_get(p_hw_iwdg) & 0x1ul);
    if (div <= 4) {
        amhw_stm32f103rbt6_iwdg_div_set (p_hw_iwdg, 0);
        div = 4;
    } else if (div <= 8) {
        amhw_stm32f103rbt6_iwdg_div_set (p_hw_iwdg, 1);
        div = 8;
    } else if (div <= 16) {
        amhw_stm32f103rbt6_iwdg_div_set (p_hw_iwdg, 2);
        div = 16;
    } else if (div <= 32) {
        amhw_stm32f103rbt6_iwdg_div_set (p_hw_iwdg, 3);
        div = 32;
    } else if (div <= 64) {
        amhw_stm32f103rbt6_iwdg_div_set (p_hw_iwdg, 4);
        div = 64;
    } else if (div <= 128) {
        amhw_stm32f103rbt6_iwdg_div_set (p_hw_iwdg, 5);
        div = 128;
    } else {
        amhw_stm32f103rbt6_iwdg_div_set (p_hw_iwdg, 6);
        div = 256;
    }

    wdt_freq /= div;

    ticks = (uint64_t) (timeout_ms) * wdt_freq / 1000;

    amhw_stm32f103rbt6_iwdg_keyvalue_set(p_hw_iwdg, 0x5555);
    while(amhw_stm32f103rbt6_iwdg_status_get(p_hw_iwdg) & 0x2ul);
    amhw_stm32f103rbt6_iwdg_reload_set (p_hw_iwdg, ticks);

    /* 启动看门狗 */
    amhw_stm32f103rbt6_iwdg_keyvalue_set(p_hw_iwdg, 0xAAAA);
    amhw_stm32f103rbt6_iwdg_keyvalue_set(p_hw_iwdg, 0xCCCC);
}

/**
 * \brief 看门狗喂狗
 *
 * \return 无
 *
 */
static void __stm32f103rbt6_sdt_feed(amhw_stm32f103rbt6_iwdg_t *p_hw_iwdg)
{
    amhw_stm32f103rbt6_iwdg_keyvalue_set(p_hw_iwdg, 0xAAAA);
}

/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_hw_iwdg_entry (amhw_stm32f103rbt6_iwdg_t *p_hw_iwdg,
                             uint32_t         time_out_ms,
                             uint32_t         feed_time_ms)
{
    __stm32f103rbt6_iwdg_enable(p_hw_iwdg, time_out_ms);

    while (1) {

        /* 喂狗操作 */
        __stm32f103rbt6_sdt_feed(p_hw_iwdg);

        /* 延时，当延时大于喂狗时间时（大于5ms以上）,会产生看门狗事件，MCU复位 */
        am_mdelay(feed_time_ms);
    }
}
/** [src_stm32f103rbt6_hw_iwdg] */

/* end of file */
