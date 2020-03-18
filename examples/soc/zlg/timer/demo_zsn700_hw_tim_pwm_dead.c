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
 * \brief 通用定时器带死区的互补PWM例程，通过 HW层接口实现
 *
 * - 实验现象：
 *   1.串口打印提示信息.
 *   2.PIOA_2(TIM0_CHA)和PIOA_1(TIM0_CHB)输出占空比25%、20Hz的PWM波，死区时间为100us。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *    2. 使用示波器或逻辑分析仪测量互补PWM的死区时间。
 *    3. 死区时间应小于PWM的周期时间。死区时间的设定范围（6——2686us），如果需要设定其他死区时间，则需
 *       要更改demo_zsn700_hw_tim_pwm_dead.c中的分频系数，外设时钟两个参数。
 *
 * \par 源代码
 * \snippet demo_zsn700116_hw_tim_pwm_dead.c src_zsn700116_hw_tim_pwm_dead
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-12  zp, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_zsn700116_hw_tim_pwm_dead
 * \copydoc demo_zsn700116_hw_tim_pwm_dead.c
 */

/** [src_zsn700116_hw_tim_pwm_dead] */
#include "ametal.h"
#include "am_clk.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_zsn700.h"
#include "hw/amhw_zsn700_tim.h"

static am_bool_t __zsn700_hw_tim_init (amhw_zsn700_tim_t *p_hw_tim,
                                       uint8_t            type,
                                       uint8_t            chan_a,
                                       uint8_t            chan_b,
                                       uint32_t           duty_us,
                                       uint32_t           period_us,
                                       uint16_t           dead_us)
{
	float    deadtime = 0.0;
    uint32_t clkfreq  = 0;
    uint16_t fre_div  = 1;
    uint32_t period_c, duty_c;

    /* 设置定时器模式2(锯齿波模式) */
    amhw_zsn700_tim_mode_set(p_hw_tim, AMHW_ZSN700_TIM_MODE_ZIGZAG);

    /* 向上计数 */
    amhw_zsn700_tim_mode23_counter_up(p_hw_tim);

    /* 使能互补输出 */
    amhw_zsn700_tim_mode23_enable(p_hw_tim, AMHW_ZSN700_TIM_COMP);

    /* 设置通道A为比较模式 */
    amhw_zsn700_tim_mode23_csa_compare(p_hw_tim, chan_a);

    /* 设置通道 B为比较模式 */
    amhw_zsn700_tim_mode23_csb_compare(p_hw_tim, chan_b);

    /* 定时器时钟为内部时钟 */
    amhw_zsn700_tim_mode_clk_src_set(p_hw_tim,AMHW_ZSN700_TIM_CLK_SRC_TCLK);

    /* 获取外设时钟频率 */
    clkfreq = am_clk_rate_get(CLK_TIM012);

    /* 计算出来得到的是计数值CNT, 公式us * 10e-6= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_us) * (clkfreq) / (uint64_t)1000000;
    duty_c   = (uint64_t)(duty_us)   * (clkfreq) / (uint64_t)1000000;

    /* 设置分频值 */
    amhw_zsn700_tim_mode_clkdiv_set(p_hw_tim, AMHW_ZSN700_TIM_CLK_DIV64);

    fre_div = 64;

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / fre_div;
    duty_c   = duty_c   / fre_div;

    /* 参数不合法 */
    if ((period_c == 0) || (period_c > 65535UL)) {
        return AM_FALSE;
    }

    /* 设置自动重装寄存器的值 */
    amhw_zsn700_tim_arr_count_set(p_hw_tim, period_c - 1);

    /* 清零计数器 */
    amhw_zsn700_tim_cnt16_count_set(p_hw_tim, 0);

    /* 设置某一通道A的比较值 */
    amhw_zsn700_tim_mode23_ccrxy_set(p_hw_tim, chan_a, duty_c - 1);

    /* 设置某一通道B的比较值 */
    amhw_zsn700_tim_mode23_ccrxy_set(p_hw_tim, chan_b, duty_c - 1);

    /* 设置通道A的PWM输出模式为PWM2 */
    amhw_zsn700_tim_mode23_compare_set(
       p_hw_tim,
       (amhw_zsn700_tim_compare_type_t)(chan_a * 4),
       AMHW_ZSN700_TIM_COMPARE_PWM2);

    /* 设置通道B的PWM输出模式为PWM2 */
    amhw_zsn700_tim_mode23_compare_set(
       p_hw_tim,
       (amhw_zsn700_tim_compare_type_t)(chan_b * 4),
       AMHW_ZSN700_TIM_COMPARE_PWM2);

    /* 通道A正常输出 */
    amhw_zsn700_tim_mode23_phase_same(
       p_hw_tim,
       (amhw_zsn700_tim_phase_type_t)(chan_a * 4 + 3));

    /* 通道A正常输出 */
    amhw_zsn700_tim_mode23_phase_same(
       p_hw_tim,
       (amhw_zsn700_tim_phase_type_t)(chan_b * 4 + 3));

    /* 清除标志位 */
    amhw_zsn700_tim_mode23_int_flag_clr(p_hw_tim, AMHW_ZSN700_TIM_INT_FLAG_ALL);

    /* 死区控制使能  */
    amhw_zsn700_tim_mode23_dtr_enable(p_hw_tim, AMHW_ZSN700_TIM_DTR_DTEN);

    deadtime = (float)dead_us * (float)clkfreq / (float)(fre_div * 1000000) + 0.5;

    /* 死区时间设置  */
    amhw_zsn700_tim_mode23_dtr_set(p_hw_tim, (uint8_t)deadtime);

    /* PWM功能使能 */
    amhw_zsn700_tim_mode23_dtr_enable(p_hw_tim, AMHW_ZSN700_TIM_DTR_MOE);

    return AM_TRUE;
}

/**
 * \brief 例程入口
 */
void demo_zsn700_hw_tim_pwm_dead_entry (void    *p_hw_tim,
                                        uint8_t  type,
                                        uint8_t  chan_a,
                                        uint8_t  chan_b,
                                        uint32_t duty_us,
                                        uint32_t period_us,
                                        uint16_t dead_us)
{

    amhw_zsn700_tim_t *p_tim = (amhw_zsn700_tim_t *)p_hw_tim;

    /* 定时器配置 */
    if((__zsn700_hw_tim_init(p_tim,
                             type,
                             chan_a,
                             chan_b,
                             duty_us ,
                             period_us,
                             dead_us)) ==
       AM_FALSE) {
        return;
    }

    /* 使能定时器TIM允许计数 */
    amhw_zsn700_tim_enable(p_tim);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zsn700116_hw_tim_pwm_dead] */

/* end of file */
