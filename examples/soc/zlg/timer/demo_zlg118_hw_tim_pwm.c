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
 * \brief 定时器 PWM 输出例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 指定引脚输出 4KHz 的 PWM，占空比为 50%。
 *
 * \par 源代码
 * \snippet demo_zlg118116_hw_tim_pwm.c src_zlg118116_hw_tim_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_zlg118116_hw_tim_pwm
 * \copydoc demo_zlg118116_hw_tim_pwm.c
 */

/** [src_zlg118116_hw_tim_pwm] */
#include "ametal.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_zlg118_tim.h"
#include "am_led.h"

#define LED0            0

static uint8_t chan_num = 0;

/**
  * \brief PWM 中断服务函数
  */
static void __zlg118_tim_hw_pwm_irq_handler (void *p_arg)
{
    amhw_zlg118_tim_t *p_hw_tim = (amhw_zlg118_tim_t *)p_arg;
    uint8_t         i        = 1;

    for(i = 0; i < chan_num;i = i + 2) {
        if ((amhw_zlg118_tim_mode23_int_flag_check(p_hw_tim,
                                                   (i / 2) + 2)) ==
                                                   AM_TRUE) {

            if(amhw_zlg118_tim_mode23_ciea_int_get(p_hw_tim,i) == AM_TRUE)
            {
                am_led_toggle(LED0);

                /* 清除通道i标志 */
                amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim, (i / 2) + 2);
            }
        }
    }

    /* 判断标志  （CH0B、CH1B、CH2B */
    for(i = 1; i < chan_num;i = i + 2) {
        if ((amhw_zlg118_tim_mode23_int_flag_check(p_hw_tim,
                                                   (i / 2) + 5)) ==
                                                   AM_TRUE) {

            if(amhw_zlg118_tim_mode23_cieb_int_get(p_hw_tim,i) == AM_TRUE)
            {
                am_led_toggle(LED0);

                /* 清除通道i标志 */
                amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim,  (i / 2) + 5);
            }
        }
    }
}

/**
 * \brief 配置定时器为 PWM 输出(PWM 模式 2)
 */
void tim_pwm_chan_config (amhw_zlg118_tim_t *p_hw_tim,
                          uint32_t           chan,
                          uint32_t           duty_ns,
                          uint32_t           period_ns,
                          uint32_t           clk_rate,
                          uint8_t            clk_div)
{
    uint32_t period_c, duty_c;
    uint16_t fre_div  = 1;

    /* 参数不合法 */
    if ((period_ns == 0) || (duty_ns > 4294967295UL) ||
        (period_ns > 4294967295UL) || (duty_ns > period_ns)) {
        return ;
    }

    /* 计算出来得到的是计数值CNT, 公式ns * 10e-9= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_ns) * (clk_rate) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clk_rate) / (uint64_t)1000000000;

    /* 分频系数设置 */
    amhw_zlg118_tim_mode_clkdiv_set(p_hw_tim, (amhw_zlg118_tim_clkdiv_t)clk_div);

    switch(clk_div) {
    case 0:
        fre_div = 1;
        break;

    case 1:
        fre_div = 2;
        break;

    case 2:
        fre_div = 4;
        break;

    case 3:
        fre_div = 8;
        break;

    case 4:
        fre_div = 16;
        break;

    case 5:
        fre_div = 32;
        break;

    case 6:
        fre_div = 64;
        break;

    case 7:
        fre_div = 256;
        break;

    default:
        break;
    }

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / fre_div;
    duty_c   = duty_c   / fre_div;

    /* 参数不合法 */
    if ((period_c == 0) || (period_c > 65535UL)) {
        return ;
    }

    /* 设置自动重装寄存器的值 */
    amhw_zlg118_tim_arr_count_set(p_hw_tim, period_c - 1);

    /* 清零计数器 */
    amhw_zlg118_tim_cnt16_count_set(p_hw_tim, 0);

    duty_c = period_c - duty_c;

    /* 设置某一通道的比较值 */
    amhw_zlg118_tim_mode23_ccrxy_set(p_hw_tim, chan, duty_c - 1);

    /* 设置PWM输出模式为PWM2 */
    amhw_zlg118_tim_mode23_compare_set(
        p_hw_tim,
        (amhw_zlg118_tim_compare_type_t)(chan * 4),
        AMHW_ZLG118_TIM_COMPARE_PWM2);

    /* 正常输出 */
    amhw_zlg118_tim_mode23_phase_same(
        p_hw_tim,
        (amhw_zlg118_tim_phase_type_t)(chan * 4 + 3));
}

void tim_pwm_enable (amhw_zlg118_tim_t      *p_hw_tim,
                     amhw_zlg118_tim_type_t  type,
                     uint32_t                chan,
                     int32_t                 int_num)
{
    /* 连接中断回调函数 */
    am_int_connect(int_num, __zlg118_tim_hw_pwm_irq_handler, (void *)p_hw_tim);
    am_int_enable(int_num);

    /* 捕获比较触发中断使能 */
    if((chan % 2) == 0) {
        amhw_zlg118_tim_mode23_ciea_int_enable(p_hw_tim, chan);
    } else {
        amhw_zlg118_tim_mode23_cieb_int_enable(p_hw_tim, chan);
    }

    /* 清零计数器 */
    amhw_zlg118_tim_cnt16_count_set(p_hw_tim, 0);

    /* 清除中断标志 */
    amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim, AMHW_ZLG118_TIM_INT_FLAG_ALL);

    /* PWM功能使能 */
    amhw_zlg118_tim_mode23_dtr_enable(p_hw_tim, AMHW_ZLG118_TIM_DTR_MOE);

    /* 使能定时器 */
    amhw_zlg118_tim_enable(p_hw_tim);
}

/**
 * \brief 定时器TIM PWM输出初始化函数
 *
 * \param[in] p_hw_tim : 指向定时器寄存器块的指针
 * \param[in] clk_id   : 时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \return 无
 */
void tim_pwm_init (amhw_zlg118_tim_t *p_hw_tim, uint8_t type, uint8_t chan)
{
    if((amhw_zlg118_tim_type_t)type == AMHW_ZLG118_TIM_TYPE_TIM3) {
        chan_num = 6;
    } else {
        chan_num = 2;
    }

    /* 设置定时器模式2 */
    amhw_zlg118_tim_mode_set(p_hw_tim, AMHW_ZLG118_TIM_MODE_ZIGZAG);

    /* 向上计数 */
    amhw_zlg118_tim_mode23_counter_up(p_hw_tim);

    /* 设置通道为比较模式 */
    if((chan % 2) == 0) {
        amhw_zlg118_tim_mode23_csa_compare(p_hw_tim, chan);
    } else {
        amhw_zlg118_tim_mode23_csb_compare(p_hw_tim, chan);
    }

    /* 定时器时钟为内部时钟 */
    amhw_zlg118_tim_mode_clk_src_set(p_hw_tim,AMHW_ZLG118_TIM_CLK_SRC_TCLK);
}

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_tim_pwm_entry (void    *p_hw_tim,
                                   uint8_t  type,
                                   uint32_t chan,
                                   uint32_t clk_rate,
                                   uint8_t  clk_div,
                                   int32_t  inum)
{

    amhw_zlg118_tim_t *p_tim = (amhw_zlg118_tim_t *)p_hw_tim;

    /* 初始化定时器TIM为PWM功能 */
    tim_pwm_init(p_tim, type, chan);

    /* 配置定时器TIM PWM输出通道 */
    tim_pwm_chan_config(p_tim,
                        chan,
                        50000000,
                        100000000,
                        clk_rate,
                        clk_div);

    tim_pwm_enable(p_tim, (amhw_zlg118_tim_type_t)type, chan, inum);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zlg118116_hw_tim_pwm] */

/* end of file */
