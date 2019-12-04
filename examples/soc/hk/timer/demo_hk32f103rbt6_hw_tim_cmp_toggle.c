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
 * \snippet demo_hk32f103rbt6_hw_tim_cmp_toggle.c src_hk32f103rbt6_hw_tim_cmp_toggle
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-22  nwt, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_hk32f103rbt6_hw_tim_cmp_toggle
 * \copydoc demo_hk32f103rbt6_hw_tim_cmp_toggle.c
 */

/** [src_hk32f103rbt6_hw_tim_cmp_toggle] */
#include "ametal.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_hk32f103rbt6_tim.h"
#include "am_led.h"

#define LED0          0

/**
 * \brief CMP 中断服务函数
 */
static void __hk32f103rbt6_tim_cmp_irq_handler (void *p_arg)
{
    uint8_t         i        = 1;
    amhw_hk32f103rbt6_tim_t *p_hw_tim = (amhw_hk32f103rbt6_tim_t *)p_arg;

    for (i = 1; i <= 4 ; i++) {
        if ((amhw_hk32f103rbt6_tim_status_flg_get(p_hw_tim, 1UL << i) & AMHW_HK32F103RBT6_TIM_CC1IF) != 0) {

            am_led_toggle(LED0);

            /* 清除通道i标志 */
            amhw_hk32f103rbt6_tim_status_flg_clr(p_hw_tim, (1UL << i));
        }
    }
}

/**
 * \brief 配置定时器输出比较模式，匹配后电平发生翻转
 */
static void tim_cmp_toggle_chan_config (amhw_hk32f103rbt6_tim_t *p_hw_tim,
                                        uint32_t        chan,
                                        uint32_t        cnt)
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
    amhw_hk32f103rbt6_tim_prescale_set(p_hw_tim, pre_reg);

    /* 设置自动重装寄存器的值 */
    amhw_hk32f103rbt6_tim_arr_set(p_hw_tim, match - 1);

    /* 设置比较输出通道的匹配值 */
    amhw_hk32f103rbt6_tim_ccr_ouput_reload_val_set(p_hw_tim,  match / 2 - 1, chan);

    /* 计数器清0 */
    amhw_hk32f103rbt6_tim_count_set(p_hw_tim, 0);

    /* 选择该通道为输出 */
    amhw_hk32f103rbt6_tim_ccs_set(p_hw_tim, 0, chan);

    /* 选择该通道的模式为CMP_TOGGLE */
    amhw_hk32f103rbt6_tim_ocm_set(p_hw_tim, AMHW_HK32F103RBT6_TIM_TOGGLE, chan);

    /* 禁止预装载 */
    amhw_hk32f103rbt6_tim_ccs_ocpe_disable(p_hw_tim, chan);

    /* 设置比较输出通道ccp高电平极性有效 */
    amhw_hk32f103rbt6_tim_ccp_output_set(p_hw_tim, 0, chan);
}

void tim_cmp_toggle_enable (amhw_hk32f103rbt6_tim_t      *p_hw_tim,
                            amhw_hk32f103rbt6_tim_type_t  type,
                            uint32_t             chan,
                            uint8_t              int_num)
{

    /* 使能通道比较输出 */
    amhw_hk32f103rbt6_tim_cce_output_enable(p_hw_tim, chan);

    /* 高级定时器使能主输出 MOE */
    if ((AMHW_HK32F103RBT6_TIM_TYPE0 == type) ||
        (AMHW_HK32F103RBT6_TIM_TYPE2 == type) ||
        (AMHW_HK32F103RBT6_TIM_TYPE3 == type)) {

        amhw_hk32f103rbt6_tim_bdtr_enable(p_hw_tim, AMHW_HK32F103RBT6_TIM_MOE);
    }

    /* 产生更新事件，重新初始化Prescaler计数器 及Repetition计数器 */
    amhw_hk32f103rbt6_tim_egr_set(p_hw_tim, AMHW_HK32F103RBT6_TIM_UG);

    if (amhw_hk32f103rbt6_tim_status_flg_get(p_hw_tim, AMHW_HK32F103RBT6_TIM_UG) != 0) {

        /* 更新定时器时会产生更新事件,清除标志位 */
        amhw_hk32f103rbt6_tim_status_flg_clr(p_hw_tim, AMHW_HK32F103RBT6_TIM_UG);
    }

    /* 连接中断回调函数 */
    am_int_connect(int_num, __hk32f103rbt6_tim_cmp_irq_handler, (void *)p_hw_tim);

    /* 使能中断 */
    amhw_hk32f103rbt6_tim_int_enable(p_hw_tim, (1ul << (chan + 1)));

    am_int_enable(int_num);

    /*　使能定时器TIM允许计数 */
    amhw_hk32f103rbt6_tim_enable(p_hw_tim);
}

/**
 * \brief 定时器 TIM 输出比较通道翻转初始化函数
 */
void tim_cmp_toggle_init (amhw_hk32f103rbt6_tim_t *p_hw_tim, amhw_hk32f103rbt6_tim_type_t type)
{
    if ((AMHW_HK32F103RBT6_TIM_TYPE0 == type) || (AMHW_HK32F103RBT6_TIM_TYPE1 == type)) {

        /* 边沿对齐模式 */
        amhw_hk32f103rbt6_tim_cms_set(p_hw_tim, 0);

        /* 向上计数 */
        amhw_hk32f103rbt6_tim_dir_set(p_hw_tim, 0);
    }

    /* 设置时钟分割:TDTS = Tck_tin */
    amhw_hk32f103rbt6_tim_ckd_set(p_hw_tim, 0);

    /* 允许更新事件 */
    amhw_hk32f103rbt6_tim_udis_enable(p_hw_tim);
}

/**
 * \brief 例程入口
 */
void demo_hk32f103rbt6_hw_tim_cmp_toggle_entry (amhw_hk32f103rbt6_tim_t      *p_hw_tim,
                                       amhw_hk32f103rbt6_tim_type_t  type,
                                       uint32_t             chan,
                                       uint32_t             clk_rate,
                                       uint8_t              inum)
{

    /* 初始化定时器为输出比较通道翻转功能 */
    tim_cmp_toggle_init(p_hw_tim, type);
    tim_cmp_toggle_chan_config(p_hw_tim, chan, clk_rate / 10);
    tim_cmp_toggle_enable(p_hw_tim, type, chan, inum);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hk32f103rbt6_hw_tim_cmp_toggle] */

/* end of file */
