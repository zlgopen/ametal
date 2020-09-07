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
 * \brief 定时器定时例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 在中断时调试串口打印信息。
 *   2. LED0 翻转
 *
 * \par 源代码
 * \snippet demo_hc32_hw_tim_timing.c src_hc32_hw_tim_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-23  zp, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_hc32_hw_tim_timing
 * \copydoc demo_hc32_hw_tim_timing.c
 */

/** [src_hc32_hw_tim_timing] */
#include "ametal.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_hc32_tim.h"
#include "am_led.h"
#include "am_board.h"

/**
 * \brief PWM 中断服务函数
 */
static void __hc32_tim_hw_timing_irq_handler (void *p_arg)
{
    amhw_hc32_tim_t *p_hw_tim = (amhw_hc32_tim_t *)p_arg;

    if (amhw_hc32_tim_mode0_int_update_flag_check(p_hw_tim) == AM_TRUE ) {

        AM_DBG_INFO("timing irq!\r\n");

        am_led_toggle(LED0);

        /* 清除溢出标志 */
        amhw_hc32_tim_mode0_int_update_flag_clr(p_hw_tim);
    }
}

/**
 * \brief 配置定时器TIM为定时功能timing
 *
 * \param[in] p_hw_tim : 指向定时器寄存器块的指针
 * \param[in] cnt      : 计数值，相对于定时器输入时钟频率来计算
 *
 * \return 无
 */
void tim_timing_chan_config (amhw_hc32_tim_t *p_hw_tim, uint32_t cnt)
{
    uint32_t match;
    uint16_t pre_real = 1;

    /* 设置分频值 */
    amhw_hc32_tim_mode_clkdiv_set(p_hw_tim, AMHW_HC32_TIM_CLK_DIV256);

    pre_real = 256;

    /* 计算自动重载值 */
    match = cnt / pre_real ;

    if(match > 65535) {
        return;
    }

    /* 设置自动重装寄存器的值 */
    amhw_hc32_tim_arr_count_set(p_hw_tim, 65535 - (match - 1));

    /* 重置计数器 */
    amhw_hc32_tim_cnt16_count_set(p_hw_tim, 65535 - (match - 1));
}

/**
 * \brief 使能定时器为定时功能
 */
void tim_timing_enable (amhw_hc32_tim_t *p_hw_tim, uint8_t int_num)
{

    /* 连接中断回调函数 */
    am_int_connect(int_num, __hc32_tim_hw_timing_irq_handler, (void *)p_hw_tim);

    am_int_enable(int_num);

    /* 更新定时器时会产生更新事件,清除标志位 */
    amhw_hc32_tim_mode0_int_update_flag_clr(p_hw_tim);

    /* 允许更新中断 */
    amhw_hc32_tim_mode0_int_update_enable(p_hw_tim);

    /* 使能定时器TIM允许计数 */
    amhw_hc32_tim_enable(p_hw_tim);
}

/**
 * \brief 定时器TIM timing初始化函数
 */
void tim_timing_init (amhw_hc32_tim_t *p_hw_tim, amhw_hc32_tim_type_t type)
{
    /* 16位重载计数器 */
    amhw_hc32_tim_mode0_counter_set(p_hw_tim,
                                      AMHW_HC32_TIM_MODE0_COUNTER_16);

    /* 设置定时器模式0 */
    amhw_hc32_tim_mode_set(p_hw_tim, AMHW_HC32_TIM_MODE_TIMING);

    /* 计数时钟设置 */
    amhw_hc32_tim_mode_clk_src_set(p_hw_tim,AMHW_HC32_TIM_CLK_SRC_TCLK);
}

/**
 * \brief 定时器定时例程，通过 HW 层接口实现
 */
void demo_hc32_hw_tim_timing_entry (void    *p_hw_tim,
                                    uint8_t  type,
                                    uint32_t clk_rate,
                                    int32_t  int_num)
{
    amhw_hc32_tim_t *p_tim = (amhw_hc32_tim_t *)p_hw_tim;

    /* 初始化定时器为定时功能 */
    tim_timing_init(p_tim, (amhw_hc32_tim_type_t)type);

    tim_timing_chan_config(p_tim, clk_rate / 20);

    tim_timing_enable(p_tim, int_num);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32_hw_tim_timing] */

/* end of file */
