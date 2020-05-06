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
 * \brief 定时器通道比较匹配引脚翻转例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 指定引脚以 10Hz 的频率进行翻转。
 *   2. LED0 翻转。
 *
 * \par 源代码
 * \snippet demo_hc32_hw_tim_cmp_toggle.c src_hc32_hw_tim_cmp_toggle
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_hc32_hw_tim_cmp_toggle
 * \copydoc demo_hc32_hw_tim_cmp_toggle.c
 */

/** [src_hc32_hw_tim_cmp_toggle] */
#include "ametal.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_hc32_tim.h"
#include "am_led.h"
#include "am_board.h"

static uint8_t chan_num = 0;

/**
 * \brief CMP 中断服务函数
 */
static void __hc32_tim_cmp_irq_handler (void *p_arg)
{
    amhw_hc32_tim_t *p_hw_tim = (amhw_hc32_tim_t *)p_arg;
    uint8_t         i        = 1;

    /* 判断标志  （CH0A、CH1A、CH2A */
    for(i = 0; i < chan_num;i = i + 2) {
        if ((amhw_hc32_tim_mode23_int_flag_check(p_hw_tim,
                                                   (i / 2) + 2)) ==
                                                   AM_TRUE) {

            if(amhw_hc32_tim_mode23_ciea_int_get(p_hw_tim,i) == AM_TRUE)
            {
                am_led_toggle(LED0);

                /* 清除通道i标志 */
                amhw_hc32_tim_mode23_int_flag_clr(p_hw_tim, (i / 2) + 2);
            }
        }
    }

    /* 判断标志  （CH0B、CH1B、CH2B */
    for(i = 1; i < chan_num;i = i + 2) {
        if ((amhw_hc32_tim_mode23_int_flag_check(p_hw_tim,
                                                   (i / 2) + 5)) ==
                                                   AM_TRUE) {

            if(amhw_hc32_tim_mode23_cieb_int_get(p_hw_tim,i) == AM_TRUE)
            {
                am_led_toggle(LED0);

                /* 清除通道i标志 */
                amhw_hc32_tim_mode23_int_flag_clr(p_hw_tim,  (i / 2) + 5);
            }
        }
    }
}

/**
 * \brief 配置定时器输出比较模式，匹配后电平发生翻转
 */
static void tim_cmp_toggle_chan_config (amhw_hc32_tim_t *p_hw_tim,
                                        uint32_t        chan,
                                        uint32_t        cnt,
                                        uint8_t         clk_div)
{

    uint32_t match    = 0;
    uint16_t fre_div  = 1;

    /* 分频系数设置 */
    amhw_hc32_tim_mode_clkdiv_set(p_hw_tim,
                                    (amhw_hc32_tim_clkdiv_t)clk_div);

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
    match = cnt / fre_div ;

    if(match > 65535ul) {
        return ;
    }

    /* 设置自动重装寄存器的值 */
    amhw_hc32_tim_arr_count_set(p_hw_tim, match - 1);

    /* 清零计数器 */
    amhw_hc32_tim_cnt16_count_set(p_hw_tim, 0);

    /* 设置某一通道的比较值 */
    amhw_hc32_tim_mode23_ccr_set(p_hw_tim, chan, match / 2 - 1);

    /* 比较匹配时翻转 */
    amhw_hc32_tim_mode23_compare_set(
        p_hw_tim,
        (amhw_hc32_tim_compare_type_t)(chan * 4),
        AMHW_HC32_TIM_COMPARE_FIT_TOGGLE);

    /* 正常输出 */
    amhw_hc32_tim_mode23_phase_same(
        p_hw_tim,
        (amhw_hc32_tim_phase_type_t)(chan * 4 + 3));
}

void tim_cmp_toggle_enable (amhw_hc32_tim_t     *p_hw_tim,
                            amhw_hc32_tim_type_t type,
                            uint32_t               chan,
                            uint8_t                int_num)
{

    /* 连接中断回调函数 */
    am_int_connect(int_num, __hc32_tim_cmp_irq_handler, (void *)p_hw_tim);
    am_int_enable(int_num);

    /* 捕获比较触发中断使能 */
    if((chan % 2) == 0) {

        amhw_hc32_tim_mode23_ciea_int_enable(p_hw_tim, chan);
    } else {

        amhw_hc32_tim_mode23_cieb_int_enable(p_hw_tim, chan);
    }

    /* 清零计数器 */
    amhw_hc32_tim_cnt16_count_set(p_hw_tim, 0);

    /* 清除中断标志 */
    amhw_hc32_tim_mode23_int_flag_clr(p_hw_tim, AMHW_HC32_TIM_INT_FLAG_ALL);

    /* PWM功能使能 */
    amhw_hc32_tim_mode23_dtr_enable(p_hw_tim, AMHW_HC32_TIM_DTR_MOE);

    /* 使能定时器 */
    amhw_hc32_tim_enable(p_hw_tim);
}

/**
 * \brief 定时器 TIM 输出比较通道翻转初始化函数
 */
void tim_cmp_toggle_init (amhw_hc32_tim_t     *p_hw_tim,
                          amhw_hc32_tim_type_t type,
                          uint8_t                chan)
{
    if((amhw_hc32_tim_type_t)type == AMHW_HC32_TIM_TYPE_TIM3) {
        chan_num = 6;
    } else {
        chan_num = 2;
    }

    /* 设置定时器模式2 */
    amhw_hc32_tim_mode_set(p_hw_tim, AMHW_HC32_TIM_MODE_ZIGZAG);

    /* 向上计数 */
    amhw_hc32_tim_mode23_counter_up(p_hw_tim);

    /* 设置通道为比较模式 */
    if((chan % 2) == 0) {
        amhw_hc32_tim_mode23_csa_compare(p_hw_tim, chan);
    } else {
        amhw_hc32_tim_mode23_csb_compare(p_hw_tim, chan);
    }

    /* 定时器时钟为内部时钟 */
    amhw_hc32_tim_mode_clk_src_set(p_hw_tim,AMHW_HC32_TIM_CLK_SRC_TCLK);;
}

/**
 * \brief 定时器通道比较匹配引脚翻转例程，通过 HW 层接口实现
 */
void demo_hc32_hw_tim_cmp_toggle_entry (void    *p_hw_tim,
                                        uint8_t  type,
                                        uint32_t chan,
                                        uint32_t clk_rate,
                                        uint8_t  clk_div,
                                        uint8_t  inum)
{
    amhw_hc32_tim_t *p_tim = (amhw_hc32_tim_t *)p_hw_tim;

    /* 初始化定时器为输出比较通道翻转功能 */
    tim_cmp_toggle_init(p_tim, (amhw_hc32_tim_type_t)type, chan);
    tim_cmp_toggle_chan_config(p_tim, chan, clk_rate / 20 , clk_div);
    tim_cmp_toggle_enable(p_tim, (amhw_hc32_tim_type_t)type, chan,inum);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32_hw_tim_cmp_toggle] */

/* end of file */
