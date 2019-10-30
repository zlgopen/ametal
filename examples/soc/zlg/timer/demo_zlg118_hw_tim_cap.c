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
 * \brief 定时器捕获例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 在定时器捕获引脚上输入 PWM。
 *
 * - 实验现象：
 *   1. 调试串口打印捕获到的 PWM 信号的周期和频率。
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_tim_cap.c src_zlg118_hw_tim_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_tim_cap
 * \copydoc demo_zlg118_hw_tim_cap.c
 */

/** [src_zlg118_hw_tim_cap] */
#include "ametal.h"
#include "am_cap.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_zlg118_tim.h"

static volatile am_bool_t g_flag       = AM_FALSE; /**< \brief 捕获标志 */
static volatile uint32_t  g_time_ns    = 0;        /**< \brief 捕获计数值 */
static uint32_t           __g_clk_rate = 0;        /**< \brief 时钟频率 */
static uint8_t            chans_num    = 0;        /**< \brief 某一定时器的通道数量 */
static uint16_t           __update_num = 0;        /**< \brief 更新溢出 */
static uint16_t           pre          = 1;        /**< \brief 分频系数 */

static void __zlg118_tim_hw_cap_irq_handler (void *p_arg)
{
    amhw_zlg118_tim_t *p_hw_tim  = (amhw_zlg118_tim_t *)p_arg;

    uint8_t            i         = 1;
    uint32_t           value     = 0;

    uint32_t           count_err = 0;
    uint64_t           time_ns   = 0;

    static uint32_t   count16   = 0;

    static am_bool_t  first     = AM_TRUE;

    if ((amhw_zlg118_tim_mode23_int_flag_check(p_hw_tim,
                AMHW_ZLG118_TIM_INT_FLAG_UPDATE)) == AM_TRUE) {

        __update_num++;

        amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim,
                                            AMHW_ZLG118_TIM_INT_FLAG_UPDATE);
    }

    for(i = 0; i < chans_num;i = i + 2) {
           if ((amhw_zlg118_tim_mode23_int_flag_check(p_hw_tim,
                                                      (i / 2) + 2)) ==
                                                      AM_TRUE) {

               /* 得到对应通道的值 */
               value = amhw_zlg118_tim_mode23_ccrxy_get(p_hw_tim, i) +
                       (__update_num << 16ul);

               if (g_flag == AM_FALSE) {

                   if (first == AM_TRUE) {

                       count16 = value;

                       first  = AM_FALSE;

                   } else {

                       /* 定时器TIM不是32位计数器时, 避免溢出时数据错误 */
                       if(count16 < value) {

                            count_err = value - count16;

                            /* 将两次读取值的差转换成时间 */
                            time_ns = (uint64_t)1000000000 *
                                                (uint64_t)count_err *
                                                pre /
                                                (uint64_t)__g_clk_rate;

                            g_time_ns = time_ns;
                       }

                       first = AM_TRUE;

                       /* 置标志为真，表明捕获完成 */
                       g_flag = AM_TRUE;
                   }
               }

               /* 清除通道i标志 */
               amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim, (i / 2) + 2);
           }
       }

       /* 判断标志  （CH0B、CH1B、CH2B */
       for(i = 1; i < chans_num;i = i + 2) {
           if ((amhw_zlg118_tim_mode23_int_flag_check(p_hw_tim,
                                                      (i / 2) + 5)) ==
                                                      AM_TRUE) {

               /* 得到对应通道的值 */
               value = amhw_zlg118_tim_mode23_ccrxy_get(p_hw_tim, i) +
                       (__update_num << 16ul);

               if (g_flag == AM_FALSE) {

                   if (first == AM_TRUE) {

                       count16 = value;

                       first  = AM_FALSE;

                   } else {

                       /* 定时器TIM不是32位计数器时, 避免溢出时数据错误 */
                       if(count16 < value) {

                            count_err = value - count16;

                            /* 将两次读取值的差转换成时间 */
                            time_ns = (uint64_t)1000000000 *
                                                (uint64_t)count_err *
                                                pre /
                                                (uint64_t)__g_clk_rate;

                            g_time_ns = time_ns;
                       }

                       first = AM_TRUE;

                       /* 置标志为真，表明捕获完成 */
                       g_flag = AM_TRUE;
                   }
               }

               /* 清除通道i标志 */
               amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim,  (i / 2) + 5);
           }
       }
}

/**
 * \brief 配置定时器为输入捕获
 */
static void tim_cap_chan_config (amhw_zlg118_tim_t *p_hw_tim,
                                 uint32_t           chan,
                                 uint32_t           cap_edge)
{

     amhw_zlg118_tim_filter_type_t filter_type =
                                       AMHW_ZLG118_TIM_FLITER_TYPE_FLTA0;

    /* 分频系数设置 */
    amhw_zlg118_tim_mode_clkdiv_set(p_hw_tim, AMHW_ZLG118_TIM_CLK_DIV1);

    pre = 1;

    switch(chan) {
    case 0:
        break;

    case 1:
        filter_type = AMHW_ZLG118_TIM_FLITER_TYPE_FLTB0;
        break;

    case 2:
        filter_type = AMHW_ZLG118_TIM_FLITER_TYPE_FLTA1;
        break;

    case 3:
        filter_type = AMHW_ZLG118_TIM_FLITER_TYPE_FLTB1;
        break;

    case 4:
        filter_type = AMHW_ZLG118_TIM_FLITER_TYPE_FLTA2;
        break;

    case 5:
        filter_type = AMHW_ZLG118_TIM_FLITER_TYPE_FLTB2;
        break;

    default:
        break;
    }

    /* 设置输入滤波器的分频值为0(默认不使用滤波) */
    amhw_zlg118_tim_mode23_fliter_set(p_hw_tim,
                                      filter_type,
                                      AMHW_ZLG118_TIM_FILTER_NO);

    /*  选择上升沿触发 */
    if ((cap_edge & AM_CAP_TRIGGER_RISE) == AM_CAP_TRIGGER_RISE) {

        if((chan % 2) == 0) {
            amhw_zlg118_tim_mode23_cra_enable(p_hw_tim, chan);
            amhw_zlg118_tim_mode23_cfa_disable(p_hw_tim, chan);
        } else {
            amhw_zlg118_tim_mode23_crb_enable(p_hw_tim, chan);
            amhw_zlg118_tim_mode23_cfb_disable(p_hw_tim, chan);
        }
    }

    /*  选择下降沿触发 */
    if ((cap_edge & AM_CAP_TRIGGER_FALL) == AM_CAP_TRIGGER_FALL) {

        if((chan % 2) == 0) {
            amhw_zlg118_tim_mode23_cra_disable(p_hw_tim, chan);
            amhw_zlg118_tim_mode23_cfa_enable(p_hw_tim, chan);
        } else {
            amhw_zlg118_tim_mode23_crb_disable(p_hw_tim, chan);
            amhw_zlg118_tim_mode23_cfb_enable(p_hw_tim, chan);
        }
    }
}

static void tim_cap_enable (amhw_zlg118_tim_t *p_hw_tim,
                            uint32_t           chan,
                            uint8_t            int_num)
{

    /* 中断连接并使能 */
    am_int_connect(int_num, __zlg118_tim_hw_cap_irq_handler, (void *)p_hw_tim);
    am_int_enable(int_num);

    /* 设置自动重装寄存器的值 */
    amhw_zlg118_tim_arr_count_set(p_hw_tim, 0xffff);

    /* 允许更新中断 */
    amhw_zlg118_tim_mode23_int_enable(p_hw_tim, AMHW_ZLG118_TIM_INT_UIE);

    /* 捕获中断使能 */
    if((chan % 2) == 0) {
        amhw_zlg118_tim_mode23_ciea_int_enable(p_hw_tim, chan);
    } else {
        amhw_zlg118_tim_mode23_cieb_int_enable(p_hw_tim, chan);
    }

    /* 使能定时器 */
    amhw_zlg118_tim_enable(p_hw_tim);

    /* 清零计数器 */
    amhw_zlg118_tim_cnt16_count_set(p_hw_tim, 0);

    /* 清除中断标志 */
    amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim, AMHW_ZLG118_TIM_INT_FLAG_ALL);
}

/**
 * \brief 定时器输入捕获初始化函数
 */
static void tim_cap_init (amhw_zlg118_tim_t *p_hw_tim, uint32_t chan)
{
    /* 设置定时器模式2 */
    amhw_zlg118_tim_mode_set(p_hw_tim, AMHW_ZLG118_TIM_MODE_ZIGZAG);

    /* 向上计数 */
    amhw_zlg118_tim_mode23_counter_up(p_hw_tim);

    /* 设置通道为捕获模式 */
    if((chan % 2) == 0) {
        amhw_zlg118_tim_mode23_csa_cap(p_hw_tim, chan);
    } else {
        amhw_zlg118_tim_mode23_csb_cap(p_hw_tim, chan);
    }

    /* 定时器时钟为内部时钟 */
    amhw_zlg118_tim_mode_clk_src_set(p_hw_tim,AMHW_ZLG118_TIM_CLK_SRC_TCLK);

    /* 清零计数器 */
    amhw_zlg118_tim_cnt16_count_set(p_hw_tim, 0);
}

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_tim_cap_entry (void     *p_hw_tim,
                                   uint8_t   type,
                                   uint32_t  chan,
                                   uint32_t  clk_rate,
                                   int32_t   inum)
{
    amhw_zlg118_tim_t *p_tim = (amhw_zlg118_tim_t *)p_hw_tim;

    uint32_t freq = 0; /* 捕获到的频率 */

    __g_clk_rate = clk_rate;

    if((amhw_zlg118_tim_type_t)type == AMHW_ZLG118_TIM_TYPE_TIM3) {
        chans_num = 6;
    } else {
        chans_num = 2;
    }

    /* 初始化定时器为捕获功能 */
    tim_cap_init(p_tim, chan);

    /* 配置定时器捕获通道 (不支持双边沿触发捕获) */
    tim_cap_chan_config(p_tim,
                        chan,
                        AM_CAP_TRIGGER_RISE);

    tim_cap_enable(p_tim, chan, inum);

    while (1) {

        if (g_flag == AM_TRUE) {

            freq = 1000000000 / g_time_ns;
            AM_DBG_INFO("The period is %d ns, The freq is %d Hz \r\n",
                        g_time_ns,
                        freq);
            g_flag = AM_FALSE;
        }
    }
}
/** [src_zlg118_hw_tim_cap] */

/* end of file */
