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
 * \brief 高级定时器端口、软件刹车（依次进行高电平刹车、软件刹车功能测试）例程，通过 HW层接口实现
 *
 * - 实验现象：
 *   1.串口打印提示信息；每5s间隔PWM的输出因为刹车的改变而改变
 *   2.demo测试开始，倒计时5秒，计时结束，高电平刹车触发，PWM输出因为刹车功能变为低电平。
 *                倒计时5秒，计时结束，清除标志，刹车配置禁能，PWM正常输出。
 *                倒计时5秒，计时结束，软件刹车使能，PWM输出因为刹车功能变为低电平。
 *                倒计时5秒，计时结束，软件刹车禁能，PWM持续正常输出。
 *
 * \par 源代码
 * \snippet demo_zsn700_hw_adtim_port_sw_brake.c src_zsn700_hw_adtim_port_sw_brake
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_hw_adtim_port_sw_brake
 * \copydoc demo_zsn700_hw_adtim_port_sw_brake.c
 */

/** [src_zsn700_hw_adtim_port_sw_brake] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_zsn700.h"
#include "hw/amhw_zsn700_adtim.h"

static am_bool_t __zsn700_hw_adtim_init(void          *p_hw_adtim,
                                        uint8_t        chan,
                                        unsigned long  duty_ns,
                                        unsigned long  period_ns) {
    amhw_zsn700_adtim_basecnt_cfg_t        basecnt;
    amhw_zsn700_adtim_timx_chx_port_cfg_t  portcfg;

    uint32_t clkfreq;
    uint16_t fre_div = 1;
    uint32_t period_c, duty_c;

    basecnt.cntmode   = AMHW_ZSN700_ADTIM_CNT_MODE_SAWTOOTH;
    basecnt.cntdir    = AMHW_ZSN700_ADTIM_CNT_DIR_UP;
    basecnt.cntclkdiv = AMHW_ZSN700_ADTIM_PCLK_DIV16;

    /* 设置定时器模式、计数方向、计数时钟分频系数 */
    amhw_zsn700_adtim_init(p_hw_adtim, &basecnt);

    fre_div = 16;

    /* 获取外设时钟频率 */
    clkfreq = am_clk_rate_get(CLK_TIM456);

    /* 计算出来得到的是计数值CNT, 公式ns * 10e-9= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_ns) * (clkfreq) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clkfreq) / (uint64_t)1000000000;

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / fre_div;
    duty_c   = duty_c   / fre_div;

    /* 参数不合法 */
    if ((period_c == 0) || (period_c > 65535UL)) {
        return AM_FALSE;
    }

    /* 设置计数周期 */
    amhw_zsn700_adtim_setperiod(p_hw_adtim, period_c);

    /* 清零计数器 */
    amhw_zsn700_adtim_clearcount(p_hw_adtim);

    duty_c = period_c - duty_c;

    /* 设置比较值 */
    amhw_zsn700_adtim_setcomparevalue(p_hw_adtim, 
		                                  (amhw_zsn700_adtim_compare_x_t)chan, 
		                                  duty_c);

    portcfg.portmode  = AMHW_ZSN700_ADTIM_PORT_MODE_CMP_OUTPUT;
    portcfg.outenable = AM_TRUE;
    portcfg.perc      = AMHW_ZSN700_ADTIM_PERIOD_INV;
    portcfg.cmpc      = AMHW_ZSN700_ADTIM_COMPARE_INV;
    portcfg.stastps   = AMHW_ZSN700_ADTIM_STATE_SEL_SS;
    portcfg.staout    = AMHW_ZSN700_ADTIM_PORT_OUT_LOW;
    portcfg.stpout    = AMHW_ZSN700_ADTIM_PORT_OUT_LOW;
    portcfg.dissel    = AMHW_ZSN700_ADTIM_DISSEL3;       // 选择强制输出无效条件3
    portcfg.disval    = AMHW_ZSN700_ADTIM_DISVAL_LOW;    // 强制输出无效条件0~3中条件成立时，CHx端口输出低电平
    portcfg.fltenable = AM_FALSE;
    portcfg.fltclk    = AMHW_ZSN700_ADTIM_FLTCLK_PCLK_DIV1;
    amhw_zsn700_adtim_timxchxportcfg(p_hw_adtim, 
		                                 (amhw_zsn700_adtim_chx_t)chan, 
		                                 &portcfg);

    return AM_TRUE;
}

/**
 * \brief 例程入口
 */
void demo_zsn700_hw_adtim_port_sw_brake_entry (void          *p_hw_adtim,
                                               uint8_t        chan,
                                               unsigned long  duty_ns,
                                               unsigned long  period_ns,
                                               int            output_pin,
                                               uint8_t        src)
{
    char                              i;
    uint8_t                           t;
    amhw_zsn700_adtim_disable_3_cfg_t disable_cfg;
    amhw_zsn700_adtim_t              *p_adtim = (amhw_zsn700_adtim_t *)
                                                p_hw_adtim;

    /* 定时器初始化配置 */
    if(__zsn700_hw_adtim_init(p_adtim, chan, duty_ns, period_ns) ==
       AM_FALSE) {
        return;
    }

    /* 无效条件3的配置结构体（仅针对portenable成员即可）初始化*/
    for(t = 0; t < 15; t++) {
        disable_cfg.stcbrkptcfg[t].portenable  = AM_FALSE;
    }

    /* 引脚输出低电平 */
    am_gpio_set(output_pin, 0);

    src = (amhw_zsn700_adtim_trigsel_t)src;

    disable_cfg.stcbrkptcfg[src].portenable  = AM_TRUE;
    disable_cfg.stcbrkptcfg[src].polaritysel = AMHW_ZSN700_ADTIM_PtBrkHigh;
    disable_cfg.fltenable = AM_TRUE;
    disable_cfg.fltclk    = AMHW_ZSN700_ADTIM_FLTCLK_PCLK_DIV1;

    /* 无效条件3配置(端口刹车) */
    amhw_zsn700_adtim_disable3cfg(&disable_cfg);

    /* 启动定时器 */
    amhw_zsn700_adtim_startcount(p_adtim);

    /* 5s后输出引脚将输出高电平，触发定时器刹车 */
    for(i = 5; i > 0; i--) {
        am_mdelay(1000);
        AM_DBG_INFO("%d\r\n",i);
    }

    /* 引脚输出高电平（触发刹车）*/
    am_gpio_set(output_pin, 1);

    AM_DBG_INFO("high port_sw_brake was happened!\r\n");

    /* 5s后输出引脚将输出低电平，同时解除刹车设置，PWM输出恢复正常 */
    for(i = 5; i > 0; i--) {
        am_mdelay(1000);
        AM_DBG_INFO("%d\r\n",i);
    }

    /* 引脚输出低电平（解除触发条件） */
    am_gpio_set(output_pin, 0);

    /* 引脚输出低电平的C语句执行完，输出改变可能存在硬件延迟，所以加入延时确保引脚已经变成低电平*/
    am_mdelay(1);

    /* 确保刹车触发条件无效，否不能清除刹车标志（刹车设置之前先清除刹车标志） */
    amhw_zsn700_adtim_clearportbrakeflag();

    AM_DBG_INFO("PWM function is normal!\r\n");

    /* 重新配置无效条件3配置(端口刹车)--解除刹车功能 */
    disable_cfg.stcbrkptcfg[src].portenable  = AM_FALSE;
    disable_cfg.fltenable = AM_FALSE;
    amhw_zsn700_adtim_disable3cfg(&disable_cfg);

    /* 5s后输出引脚将输出高电平，触发定时器刹车 */
    for(i = 5; i > 0; i--) {
        am_mdelay(1000);
        AM_DBG_INFO("%d\r\n",i);
    }

    /* 软件刹车开启 */
    amhw_zsn700_adtim_swbrake(AM_TRUE);

    AM_DBG_INFO("software port_sw_brake was happened!\r\n");

    /* 5s后输出引脚将输出低电平，同时解除刹车设置，PWM输出恢复正常 */
    for(i = 5; i > 0; i--) {
        am_mdelay(1000);
        AM_DBG_INFO("%d\r\n",i);
    }

    /* 软件刹车关闭 */
    amhw_zsn700_adtim_swbrake(AM_FALSE);

    AM_DBG_INFO("PWM function is normal!\r\n");

    while (1) {
        ;
    }
}
/** [src_zsn700_hw_adtim_port_sw_brake] */

/* end of file */
