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
 * \snippet demo_stm32f103rbt6_hw_tim_timing.c src_stm32f103rbt6_hw_tim_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-22  nwt, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_stm32f103rbt6_hw_tim_timing
 * \copydoc demo_stm32f103rbt6_hw_tim_timing.c
 */

/** [src_stm32f103rbt6_hw_tim_timing] */
#include "ametal.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_stm32f103rbt6_tim.h"
#include "am_led.h"

#define LED0   0
/**
 * \brief PWM 中断服务函数
 */
static void __stm32f103rbt6_tim_hw_timing_irq_handler (void *p_arg)
{
    amhw_stm32f103rbt6_tim_t *p_hw_tim = (amhw_stm32f103rbt6_tim_t *)p_arg;

    AM_DBG_INFO("timing irq!\r\n");

    am_led_toggle(LED0);

    /* 清除更新标志 */
    amhw_stm32f103rbt6_tim_status_flg_clr(p_hw_tim, AMHW_STM32F103RBT6_TIM_UIF);
}

/**
 * \brief 配置定时器TIM为定时功能timing
 *
 * \param[in] p_hw_tim : 指向定时器寄存器块的指针
 * \param[in] cnt      : 计数值，相对于定时器输入时钟频率来计算
 *
 * \return 无
 */
void tim_timing_chan_config (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint32_t cnt)
{
    uint32_t match, temp;
    uint16_t pre_real = 1, pre_reg = 0;

    {
        temp = cnt;

        /* 16位计数器最大值65536, 分频器最大值65536 */
        if (temp  > 4294967295UL) {
            return ;
        }

        /* 当计数小于65536时，不分频(值为1,1代表为1分频) */
        temp = cnt  / 65536 + 1;

        /* 16位定时器需要运算取得合适的分频值 */
        for (pre_real = 1; pre_real < temp; ) {
            pre_reg++;           /* 计算写入寄存器的分频值0,1,2,... */
            pre_real++;          /* 分频数 */
        }
    }

    /* 计算自动重载值 */
    match = cnt / pre_real ;

    /* 设置分频值 */
    amhw_stm32f103rbt6_tim_prescale_set(p_hw_tim, pre_reg);

    /* 设置自动重装寄存器的值 */
    amhw_stm32f103rbt6_tim_arr_set(p_hw_tim, match - 1);

    /* 计数器清 0 */
    amhw_stm32f103rbt6_tim_count_set(p_hw_tim, 0);
}

/**
 * \brief 使能定时器为定时功能
 */
void tim_timing_enable (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t int_num)
{

    /* 产生更新事件，重新初始化Prescaler计数器 及Repetition计数器 */
    amhw_stm32f103rbt6_tim_egr_set(p_hw_tim, AMHW_STM32F103RBT6_TIM_UG);

    if (amhw_stm32f103rbt6_tim_status_flg_get(p_hw_tim, AMHW_STM32F103RBT6_TIM_UG) != 0) {

        /* 更新定时器时会产生更新事件,清除标志位 */
        amhw_stm32f103rbt6_tim_status_flg_clr(p_hw_tim, AMHW_STM32F103RBT6_TIM_UG);
    }

    /* 连接中断回调函数 */
    am_int_connect(int_num, __stm32f103rbt6_tim_hw_timing_irq_handler, (void *)p_hw_tim);

    /* 使能更新中断 */
    amhw_stm32f103rbt6_tim_int_enable(p_hw_tim, AMHW_STM32F103RBT6_TIM_UIE);

    am_int_enable(int_num);

    /*　使能定时器TIM允许计数 */
    amhw_stm32f103rbt6_tim_enable(p_hw_tim);
}

/**
 * \brief 定时器TIM timing初始化函数
 */
void tim_timing_init (amhw_stm32f103rbt6_tim_t *p_hw_tim, amhw_stm32f103rbt6_tim_type_t type)
{
    if ((AMHW_STM32F103RBT6_TIM_TYPE0 == type) || (AMHW_STM32F103RBT6_TIM_TYPE1 == type)) {

        /* 边沿对齐模式 */
        amhw_stm32f103rbt6_tim_cms_set(p_hw_tim, 0);

        /* 向上计数 */
        amhw_stm32f103rbt6_tim_dir_set(p_hw_tim, 0);
    }

    /* 设置时钟分割:TDTS = Tck_tin */
    amhw_stm32f103rbt6_tim_ckd_set(p_hw_tim, 0);

    /* 允许更新事件 */
    amhw_stm32f103rbt6_tim_udis_enable(p_hw_tim);

}

/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_hw_tim_timing_entry (amhw_stm32f103rbt6_tim_t     *p_hw_tim,
                                   amhw_stm32f103rbt6_tim_type_t type,
                                   uint32_t            clk_rate,
                                   int32_t             int_num)
{

    /* 初始化定时器为定时功能 */
    tim_timing_init(p_hw_tim, type);

    tim_timing_chan_config(p_hw_tim, clk_rate / 2);

    tim_timing_enable(p_hw_tim, int_num);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_stm32f103rbt6_hw_tim_timing] */

/* end of file */
