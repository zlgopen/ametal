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
 * \brief CLKTRIM驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-18
 * \endinternal
 */

#include "ametal.h"
#include "am_zsn700_clktrim.h"
#include "hw/amhw_zsn700_clktrim.h"
#include "zsn700_periph_map.h"
#include "zsn700_inum.h"
#include "am_int.h"
#include "am_zsn700_clk.h"
#include "zsn700_clk.h"

/* 中断服务函数 */
am_local void __clktrim_irq_handler(void *parg)
{
    /* 禁能中断 */
    am_int_disable(INUM_CLKTRIM);
    amhw_zsn700_clktrim_int_enable(ZSN700_CLKTRIM, 0);

    /* 结束校准 */
    amhw_zsn700_clktrim_trim_start(ZSN700_CLKTRIM, 0);
    *(int *)parg = 1;
    if(0 == amhw_zsn700_clktrim_ifr_get(ZSN700_CLKTRIM,
                                        AMHW_CLKTRIM_CALCNT_OF)) {
        amhw_zsn700_clktrim_ifr_clear(ZSN700_CLKTRIM, AMHW_CLKTRIM_XTH_FAULT);
        amhw_zsn700_clktrim_ifr_clear(ZSN700_CLKTRIM, AMHW_CLKTRIM_XTL_FAULT);
    }
    amhw_zsn700_clktrim_ifr_clear(ZSN700_CLKTRIM, AMHW_CLKTRIM_CALCNT_OF);
    amhw_zsn700_clktrim_ifr_clear(ZSN700_CLKTRIM, AMHW_CLKTRIM_STOP);
}

/**
 * \brief CLKTRIM监测模式配置
 */
uint8_t am_zsn700_clktrim_monitor (uint32_t                     rcntval,
                                   uint32_t                     ccntval,
                                   amhw_clktrim_refclk_sel_t    refclk_sel,
                                   amhw_clktrim_calclk_sel_t    calclk_sel)
{
    int       break_flag = 0;
    uint8_t   ret        = AM_OK;

    am_clk_enable(CLK_CLOCKTRIM);

    /* 选择参考时钟 */
    amhw_zsn700_clktrim_refclk_sel(ZSN700_CLKTRIM, refclk_sel);

    /* 选择被被监控时钟 */
    amhw_zsn700_clktrim_calclk_sel(ZSN700_CLKTRIM, calclk_sel);

    /* 设置监控时间间隔 */
    amhw_zsn700_clktrim_refcon_set(ZSN700_CLKTRIM, rcntval);

    /* 设置被监控时钟溢出时间 */
    amhw_zsn700_clktrim_calcon_set(ZSN700_CLKTRIM, ccntval);

    /* 使能监控功能 */
    amhw_zsn700_clktrim_mon_enable(ZSN700_CLKTRIM, 1);

    /* 连接中断 */
    am_int_connect(INUM_CLKTRIM,
                   __clktrim_irq_handler,
                   (void *)&break_flag);

    /* 使能中断 */
    amhw_zsn700_clktrim_int_enable(ZSN700_CLKTRIM, 1);
    am_int_enable(INUM_CLKTRIM);

    /* 开始监测 */
    amhw_zsn700_clktrim_trim_start(ZSN700_CLKTRIM, 1);

    while(0 == break_flag) {
        ;
    }
    if(2 == break_flag) {
        ret = -AM_ERROR;
    }

    am_clk_disable(CLK_CLOCKTRIM);

    return ret;
}

/**
 * \brief CLKTRIM校准模式配置
 */
uint32_t am_zsn700_clktrim_calibrate (uint32_t                     rcntval,
                                     amhw_clktrim_refclk_sel_t    refclk_sel,
                                     amhw_clktrim_calclk_sel_t    calclk_sel)
{
    int       break_flag = 0;
    uint32_t  calcnt     = 0;
    am_bool_t ret        = AM_OK;

    am_clk_enable(CLK_CLOCKTRIM);

    /* 选择参考时钟 */
    amhw_zsn700_clktrim_refclk_sel(ZSN700_CLKTRIM, refclk_sel);

    /* 选择被被校准时钟 */
    amhw_zsn700_clktrim_calclk_sel(ZSN700_CLKTRIM, calclk_sel);

    /* 设置校准时间 */
    amhw_zsn700_clktrim_refcon_set(ZSN700_CLKTRIM, rcntval);

    /* 连接中断 */
    am_int_connect(CLKTRIM_IRQn,
                   __clktrim_irq_handler,
                   (void *)&break_flag);

    /* 使能中断 */
    amhw_zsn700_clktrim_int_enable(ZSN700_CLKTRIM, 1);
    am_int_enable(INUM_CLKTRIM);

    /* 开始校准 */
    amhw_zsn700_clktrim_trim_start(ZSN700_CLKTRIM, 1);

    while(0 == break_flag) {
        ;
    }
    if(1 == amhw_zsn700_clktrim_ifr_get(ZSN700_CLKTRIM,
                                        AMHW_CLKTRIM_CALCNT_OF)) {
        ret = -AM_ERROR;
    }

    calcnt = amhw_zsn700_clktrim_calcnt_get(ZSN700_CLKTRIM);

    am_clk_disable(CLK_CLOCKTRIM);

    if(ret != AM_OK) {
        return ret;
    }

    return calcnt;
}

/* end of file */
