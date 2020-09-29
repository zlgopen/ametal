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
 * \brief 定时器端口刹车中断输出例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIOB_5与低电平连接，TIM1_CHA（PIOA_0）输出占空比为25%的2Hz的PWM波。
 *   2. PIOB_5与高电平连接，发生刹车中断，串口打印"brake int!"，TIM1_CHA（PIOA_0）的PWM
 *      输出停止，再次将PIOB_5与低电平连接，恢复正常输出。
 *
 * \par 源代码
 * \snippet demo_zsl42x116_hw_tim_port_brake.c src_zsl42x116_hw_tim_port_brake
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-12  zp, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_zsl42x116_hw_tim_port_brake
 * \copydoc demo_zsl42x116_hw_tim_port_brake.c
 */

/** [src_zsl42x116_hw_tim_port_brake] */
#include "ametal.h"
#include "am_clk.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_zsl42x.h"
#include "hw/amhw_zsl42x_tim.h"

/**
  * \brief 中断服务函数
  */
static void __irq_handler (void *p_arg)
{
    amhw_zsl42x_tim_t *p_hw_tim = (amhw_zsl42x_tim_t *)p_arg;

    if(amhw_zsl42x_tim_mode23_int_flag_check(
           p_hw_tim,
           AMHW_ZSL42x_TIM_INT_FLAG_BRAKE) ==
           AM_TRUE) {

        amhw_zsl42x_tim_mode23_int_flag_clr(p_hw_tim,
                                            AMHW_ZSL42x_TIM_INT_FLAG_BRAKE);

        AM_DBG_INFO("brake int!\r\n");
    }

}

static am_bool_t __zsl42x_hw_tim_init (amhw_zsl42x_tim_t *p_hw_tim,
                                       uint8_t            type,
                                       uint8_t            chan,
                                       uint32_t           duty_ns,
                                       uint32_t           period_ns)
{
    uint32_t clkfreq;
    uint16_t fre_div = 1;
    uint32_t period_c, duty_c;

    /* 设置定时器模式2(锯齿波模式) */
    amhw_zsl42x_tim_mode_set(p_hw_tim, AMHW_ZSL42x_TIM_MODE_ZIGZAG);

    /* 向上计数 */
    amhw_zsl42x_tim_mode23_counter_up(p_hw_tim);

    /* 设置通道为比较模式 */
    if((chan % 2) == 0) {
        amhw_zsl42x_tim_mode23_csa_compare(p_hw_tim, chan);
    } else {
        amhw_zsl42x_tim_mode23_csb_compare(p_hw_tim, chan);
    }

    /* 定时器时钟为内部时钟 */
    amhw_zsl42x_tim_mode_clk_src_set(p_hw_tim,AMHW_ZSL42x_TIM_CLK_SRC_TCLK);

    /* 获取外设时钟频率 */
    clkfreq = am_clk_rate_get(CLK_TIM012);

    /* 计算出来得到的是计数值CNT, 公式ns * 10e-9= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_ns) * (clkfreq) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clkfreq) / (uint64_t)1000000000;

    /* 设置分频值 */
    amhw_zsl42x_tim_mode_clkdiv_set(p_hw_tim, AMHW_ZSL42x_TIM_CLK_DIV64);

    fre_div = 64;

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / fre_div;
    duty_c   = duty_c   / fre_div;

    /* 参数不合法 */
    if ((period_c == 0) || (period_c > 65535UL)) {
        return AM_FALSE;
    }

    /* 设置自动重装寄存器的值 */
    amhw_zsl42x_tim_arr_count_set(p_hw_tim, period_c - 1);

    /* 清零计数器 */
    amhw_zsl42x_tim_cnt16_count_set(p_hw_tim, 0);

    duty_c = period_c - duty_c;

    /* 设置某一通道的比较值 */
    amhw_zsl42x_tim_mode23_ccr_set(p_hw_tim, chan, duty_c - 1);

    /* 设置PWM输出模式为PWM2 */
    amhw_zsl42x_tim_mode23_compare_set(
       p_hw_tim,
       (amhw_zsl42x_tim_compare_type_t)(chan * 4),
       AMHW_ZSL42x_TIM_COMPARE_PWM2);

    /* 正常输出 */
    amhw_zsl42x_tim_mode23_phase_same(
       p_hw_tim,
       (amhw_zsl42x_tim_phase_type_t)(chan * 4 + 3));

    /* PWM功能使能 */
    amhw_zsl42x_tim_mode23_dtr_enable(p_hw_tim, AMHW_ZSL42x_TIM_DTR_MOE);
    amhw_zsl42x_tim_mode23_dtr_enable(p_hw_tim, AMHW_ZSL42x_TIM_DTR_AOE);

    /* TIM1/2使用本身的的刹车控制 */
    amhw_zsl42x_tim_mode23_bksel_mine(p_hw_tim);

    /* 刹车动作产生后输出电平选择 */
    if((chan % 2) == 0) {
        amhw_zsl42x_tim_mode23_bksa_set(p_hw_tim,
                                        chan,
                                        AMHW_ZSL42x_TIM_CMP_BRAKEOUT_LOW);
    } else {
        amhw_zsl42x_tim_mode23_bksb_set(p_hw_tim,
                                        chan,
                                        AMHW_ZSL42x_TIM_CMP_BRAKEOUT_LOW);
    }

    /* 刹车BK输入相位同相（高电平刹车） */
    amhw_zsl42x_tim_mode23_phase_same(p_hw_tim, AMHW_ZSL42x_TIM_PHASE_BKP);

    /* 刹车输入滤波（pclk 3个连续有效） */
    amhw_zsl42x_tim_mode23_fliter_set(p_hw_tim,
                                      AMHW_ZSL42x_TIM_FLITER_TYPE_FLTBK,
                                      AMHW_ZSL42x_TIM_FILTER_PCLKDIV1_3);

    /* 刹车使能 */
    amhw_zsl42x_tim_mode23_dtr_enable(p_hw_tim, AMHW_ZSL42x_TIM_DTR_BKE);

    /* 刹车中断使能 */
    amhw_zsl42x_tim_mode23_int_enable(p_hw_tim, AMHW_ZSL42x_TIM_INT_BIE);

    return AM_TRUE;
}

/**
 * \brief 通用定时器端口刹车例程，通过 HW 层接口实现
 */
void demo_zsl42x_hw_tim_port_brake_entry (void    *p_hw_tim,
                                        uint8_t  type,
                                        uint8_t  chan,
                                        int      inum,
                                        uint32_t duty_ns,
                                        uint32_t period_ns)
{

    amhw_zsl42x_tim_t *p_tim = (amhw_zsl42x_tim_t *)p_hw_tim;

    /* 定时器配置 */
    if((__zsl42x_hw_tim_init(p_tim, type, chan, duty_ns , period_ns)) ==
       AM_FALSE) {
        return;
    }

    am_int_connect(inum, __irq_handler, (void *)p_hw_tim);
    am_int_enable(inum);

    /* 清除中断标志 */
    amhw_zsl42x_tim_mode23_int_flag_clr(p_hw_tim, AMHW_ZSL42x_TIM_INT_FLAG_ALL);


    /* 使能定时器TIM允许计数 */
    amhw_zsl42x_tim_enable(p_tim);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zsl42x116_hw_tim_port_brake] */

/* end of file */
