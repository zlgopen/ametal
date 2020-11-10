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
 * - 1.00 19-09-30
 * \endinternal
 */

#include "ametal.h"
#include "am_zsl42x_clktrim.h"
#include "hw/amhw_zsl42x_clktrim.h"
#include "zsl42x_periph_map.h"
#include "zsl42x_inum.h"
#include "am_int.h"
#include "am_zsl42x_clk.h"
#include "zsl42x_clk.h"

/* 中断服务函数 */
am_local void __clktrim_irq_handler(void *parg)
{
    /* 禁能中断 */
    am_int_disable(INUM_CLKTRIM);
    amhw_zsl42x_clktrim_int_enable(ZSL42x_CLKTRIM, 0);

    /* 结束校准 */
    amhw_zsl42x_clktrim_trim_start(ZSL42x_CLKTRIM, 0);
    *(int *)parg = 1;
    if(0 == amhw_zsl42x_clktrim_ifr_get(ZSL42x_CLKTRIM,
                                        AMHW_CLKTRIM_CALCNT_OF)) {
        amhw_zsl42x_clktrim_ifr_clear(ZSL42x_CLKTRIM, AMHW_CLKTRIM_XTH_FAULT);
        amhw_zsl42x_clktrim_ifr_clear(ZSL42x_CLKTRIM, AMHW_CLKTRIM_XTL_FAULT);
    }
    amhw_zsl42x_clktrim_ifr_clear(ZSL42x_CLKTRIM, AMHW_CLKTRIM_CALCNT_OF);
    amhw_zsl42x_clktrim_ifr_clear(ZSL42x_CLKTRIM, AMHW_CLKTRIM_STOP);
}

/**
 * \brief CLKTRIM监测模式配置
 */
uint8_t am_zsl42x_clktrim_monitor (uint32_t                     rcntval,
                                   uint32_t                     ccntval,
                                   amhw_clktrim_refclk_sel_t    refclk_sel,
                                   amhw_clktrim_calclk_sel_t    calclk_sel)
{
    int       break_flag = 0;
    uint8_t   ret        = AM_OK;

    am_clk_enable(CLK_CLOCKTRIM);

    /* 选择参考时钟 */
    amhw_zsl42x_clktrim_refclk_sel(ZSL42x_CLKTRIM, refclk_sel);

    /* 选择被被监控时钟 */
    amhw_zsl42x_clktrim_calclk_sel(ZSL42x_CLKTRIM, calclk_sel);

    /* 设置监控时间间隔 */
    amhw_zsl42x_clktrim_refcon_set(ZSL42x_CLKTRIM, rcntval);

    /* 设置被监控时钟溢出时间 */
    amhw_zsl42x_clktrim_calcon_set(ZSL42x_CLKTRIM, ccntval);

    /* 使能监控功能 */
    amhw_zsl42x_clktrim_mon_enable(ZSL42x_CLKTRIM, 1);

    /* 连接中断 */
    am_int_connect(INUM_CLKTRIM,
                   __clktrim_irq_handler,
                   (void *)&break_flag);

    /* 使能中断 */
    amhw_zsl42x_clktrim_int_enable(ZSL42x_CLKTRIM, 1);
    am_int_enable(INUM_CLKTRIM);

    /* 开始监测 */
    amhw_zsl42x_clktrim_trim_start(ZSL42x_CLKTRIM, 1);

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
uint32_t am_zsl42x_clktrim_calibrate (uint32_t                     rcntval,
                                      amhw_clktrim_refclk_sel_t    refclk_sel,
                                      amhw_clktrim_calclk_sel_t    calclk_sel)
{
    int       break_flag = 0;
    uint32_t  calcnt     = 0;
    am_bool_t ret        = AM_OK;

    am_clk_enable(CLK_CLOCKTRIM);

    /* 选择参考时钟 */
    amhw_zsl42x_clktrim_refclk_sel(ZSL42x_CLKTRIM, refclk_sel);

    /* 选择被被校准时钟 */
    amhw_zsl42x_clktrim_calclk_sel(ZSL42x_CLKTRIM, calclk_sel);

    /* 设置校准时间 */
    amhw_zsl42x_clktrim_refcon_set(ZSL42x_CLKTRIM, rcntval);

    /* 连接中断 */
    am_int_connect(CLKTRIM_IRQn,
                   __clktrim_irq_handler,
                   (void *)&break_flag);

    /* 使能中断 */
    amhw_zsl42x_clktrim_int_enable(ZSL42x_CLKTRIM, 1);
    am_int_enable(INUM_CLKTRIM);

    /* 开始校准 */
    amhw_zsl42x_clktrim_trim_start(ZSL42x_CLKTRIM, 1);

    while(0 == break_flag) {
        ;
    }
    if(1 == amhw_zsl42x_clktrim_ifr_get(ZSL42x_CLKTRIM,
                                        AMHW_CLKTRIM_CALCNT_OF)) {
        ret = -AM_ERROR;
    }

    calcnt = amhw_zsl42x_clktrim_calcnt_get(ZSL42x_CLKTRIM);

    am_clk_disable(CLK_CLOCKTRIM);

    if(ret != AM_OK) {
        return ret;
    }

    return calcnt;
}

/* end of file */
