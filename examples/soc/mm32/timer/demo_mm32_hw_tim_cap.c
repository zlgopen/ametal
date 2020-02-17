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
 * \snippet demo_mm32_hw_tim_cap.c src_mm32_hw_tim_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 16-04-22  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32_hw_tim_cap
 * \copydoc demo_mm32_hw_tim_cap.c
 */

/** [src_mm32_hw_tim_cap] */
#include "ametal.h"
#include "am_cap.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_mm32_tim.h"

static volatile am_bool_t g_flag       = AM_FALSE; /**< \brief 捕获标志 */
static volatile uint32_t  g_time_ns    = 0;        /**< \brief 捕获计数值 */
static uint32_t           __g_clk_rate = 0;        /**< \brief 时钟频率 */

static void __mm32_tim_hw_cap_irq_handler (void *p_arg)
{
    amhw_mm32_tim_t   *p_hw_tim  = (amhw_mm32_tim_t *)p_arg;
    uint8_t           chans_num = 4;

    uint8_t           i         = 1;
    uint32_t          value     = 0;

    uint32_t          count_err = 0;
    uint64_t          time_ns   = 0;
    uint16_t          reg_pre   = 0,  pre = 1;

    static uint32_t   count16   = 0;

    static am_bool_t  first     = AM_TRUE;


    for (i = 1; i <= chans_num; i++) {
        if ((amhw_mm32_tim_status_flg_get(p_hw_tim, 1UL << i)) != 0) {

            /* 得到对应通道的值 */
            value = amhw_mm32_tim_ccr_cap_val_get(p_hw_tim, i - 1);
					  if(0 == i - 1) {
                if (g_flag == AM_FALSE) {

                    if (first == AM_TRUE) {

                        count16 = value;

                        first  = AM_FALSE;

                    } else {

                        /* 定时器TIM不是32位计数器时, 避免溢出时数据错误 */
                        if(count16 < value) {

														reg_pre = (uint16_t)amhw_mm32_tim_prescale_get(p_hw_tim);

														pre = reg_pre + 1;

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
					  }
            /* 清除通道i标志 */
            amhw_mm32_tim_status_flg_clr(p_hw_tim, (1UL << i));
        }
    }
}

/**
 * \brief 配置定时器为输入捕获
 */
static void tim_cap_chan_config (amhw_mm32_tim_t     *p_hw_tim,
                                 amhw_mm32_tim_type_t type,
                                 uint32_t            chan,
                                 uint32_t            cap_edge)
{

    /* 配置输入不分频, 1 次边沿就触发 1 次捕获 */
    amhw_mm32_tim_icpsc_set(p_hw_tim, 0, chan);

    /* 选择输入端通道映射不反相 */
    amhw_mm32_tim_ccs_set(p_hw_tim, 1, chan);

    /* 设置输入滤波器的分频值为 0(默认不使用滤波) */
    amhw_mm32_tim_icf_set(p_hw_tim, AMHW_MM32_TIM_ICF_FSAMPLING0, chan);

    /*  选择上升沿触发 */
    if ((cap_edge & AM_CAP_TRIGGER_RISE) == AM_CAP_TRIGGER_RISE) {
        amhw_mm32_tim_ccp_captrigger_set(p_hw_tim,
                                           type,
                                           AM_CAP_TRIGGER_RISE,
                                           chan);
    }

    /*  选择下降沿触发 */
    if ((cap_edge & AM_CAP_TRIGGER_FALL) == AM_CAP_TRIGGER_FALL) {
        amhw_mm32_tim_ccp_captrigger_set(p_hw_tim,
                                           type,
                                           AM_CAP_TRIGGER_FALL,
                                           chan);
    }
}

static void tim_cap_enable (amhw_mm32_tim_t *p_hw_tim,
                            uint32_t        chan,
                            uint8_t         int_num)
{

    /* 中断连接并使能 */
    am_int_connect(int_num, __mm32_tim_hw_cap_irq_handler, (void *)p_hw_tim);
    am_int_enable(int_num);

    /* 设置自动重装寄存器的值 */
    amhw_mm32_tim_arr_set(p_hw_tim, 0xffffffff);

    /* 清零计数器 */
    amhw_mm32_tim_count_set(p_hw_tim, 0);

    /* 产生更新事件，重新初始化Prescaler计数器 及Repetition计数器 */
    amhw_mm32_tim_egr_set(p_hw_tim, AMHW_MM32_TIM_UG);

    if (amhw_mm32_tim_status_flg_get(p_hw_tim, AMHW_MM32_TIM_UG) != 0) {

        /* 更新定时器时会产生更新事件,清除标志位 */
        amhw_mm32_tim_status_flg_clr(p_hw_tim, AMHW_MM32_TIM_UG);
    }

    /* 允许捕获计数器的值到捕获寄存器中 */
    amhw_mm32_tim_cce_cap_enable(p_hw_tim, chan);

    /* 允许捕获中断 */
    amhw_mm32_tim_int_enable(p_hw_tim, (1UL << (chan + 1)));

    /*　使能定时器TIM允许计数 */
    amhw_mm32_tim_enable(p_hw_tim);
}

/**
 * \brief 定时器输入捕获初始化函数
 */
static void tim_cap_init (amhw_mm32_tim_t *p_hw_tim, amhw_mm32_tim_type_t type)
{
    if ((AMHW_MM32_TIM_TYPE0 == type) || (AMHW_MM32_TIM_TYPE1 == type)) {

        /* 边沿对齐模式 */
        amhw_mm32_tim_cms_set(p_hw_tim, 0);

        /* 向上计数 */
        amhw_mm32_tim_dir_set(p_hw_tim, 0);
    }

    /* 设置时钟分割(用于确定输入滤波器的输入时钟频率,默认 Fdts = Fck_in */
    amhw_mm32_tim_ckd_set(p_hw_tim, 0);

    /* 清零计数器 */
    amhw_mm32_tim_count_set(p_hw_tim, 0);

    /* 设置分频器 */
    amhw_mm32_tim_prescale_set(p_hw_tim, 0x00);

    /* 允许更新事件 */
    amhw_mm32_tim_udis_enable(p_hw_tim);
}

/**
 * \brief 例程入口
 */
void demo_mm32_hw_tim_cap_entry (amhw_mm32_tim_t      *p_hw_tim,
                                amhw_mm32_tim_type_t  type,
                                uint32_t             chan,
                                uint32_t             clk_rate,
                                int32_t              inum)
{
    uint32_t freq = 0; /* 捕获到的频率 */

    __g_clk_rate = clk_rate;

    /* 初始化定时器为捕获功能 */
    tim_cap_init(p_hw_tim, type);

    /* 配置定时器捕获通道 (不支持双边沿触发捕获) */
    tim_cap_chan_config(p_hw_tim,
                        type,
                        chan,
                        AM_CAP_TRIGGER_RISE);

    tim_cap_enable(p_hw_tim, chan, inum);

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
/** [src_mm32_hw_tim_cap] */

/* end of file */
