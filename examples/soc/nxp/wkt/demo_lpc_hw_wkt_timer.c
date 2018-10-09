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
 * \brief WKT 定时器例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. LED0 以 1s 间隔闪烁。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_wkt_timer.c src_lpc_hw_wkt_timer
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-02  sky, modified
 * - 1.00 15-07-13  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_wkt_timer
 * \copydoc demo_lpc_hw_wkt_timer.c
 */

/** [src_lpc_hw_wkt_timer] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc_wkt.h"


/* 选择 IRC 时钟 频率为 750kHz */
#define __IRC_CLOCK_FREQ          750

/* 选择低功耗时钟 频率为 10kHz */
#define __LOW_POWER_CLOCK_FREQ    10

/* 选择外部时钟 频率为 10kHz */
#define __EXT_CLOCK_FREQ          10

/**
 * \brief 设置当前定时器的计数值
 *
 * \param[in] delay_inms 设置定时时间 delay_inms：延时时间(单位：ms)
 *
 * \return 无
 */
void wkt_delayms (amhw_lpc_wkt_t *p_hw_wkt, uint32_t delay_inms)
{

    /* 设置 WKT 计数器计数值 */
    if(amhw_lpc_wkt_clksel_get(p_hw_wkt) == 1) {
        amhw_lpc_wkt_count_set(p_hw_wkt,
                               __LOW_POWER_CLOCK_FREQ * delay_inms);
    } else if(amhw_lpc_wkt_clksel_get(p_hw_wkt) == 0) {
        amhw_lpc_wkt_count_set(p_hw_wkt, __IRC_CLOCK_FREQ * delay_inms);
    } else {

    /* 此种情况是使用外部时钟,具体设置数目需由用户设定的外部时钟频率决定 */
        amhw_lpc_wkt_count_set(p_hw_wkt, __EXT_CLOCK_FREQ * delay_inms);
    }

    /* 等待超时 */
    while(!(amhw_lpc_wkt_alarmflag_get(p_hw_wkt))) {
        ; /* VOID */
    }

    /* 清除超时标志 */
    amhw_lpc_wkt_alarmflag_clear(p_hw_wkt);
}

void demo_lpc_hw_wkt_timer(amhw_lpc_wkt_t *p_hw_wkt,
                           uint32_t        wkt_mdelay)
{
    AM_FOREVER {

        /* LED0 以 wkt_mdelays 的间隔一直闪烁 */
        am_led_toggle(LED0);

        wkt_delayms(p_hw_wkt, wkt_mdelay);
    }
}
/** [src_lpc_hw_wkt_timer] */

/* end of file */
