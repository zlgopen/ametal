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
 * \snippet demo_zlg116_hw_tim_pwm.c src_zlg116_hw_tim_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-22  nwt, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_zlg116_hw_tim_pwm
 * \copydoc demo_zlg116_hw_tim_pwm.c
 */

/** [src_zlg116_hw_tim_pwm] */
#include "ametal.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"
//#include "am_zlg116.h"
#include "hw/amhw_stm32f103rbt6_tim.h"
#include "am_led.h"

#define LED0          0
/**
  * \brief PWM 中断服务函数
  */
static void __stm32f103rbt6_tim_hw_pwm_irq_handler (void *p_arg)
{
    amhw_stm32f103rbt6_tim_t *p_hw_tim = (amhw_stm32f103rbt6_tim_t *)p_arg;
    uint8_t         i        = 1;

    for (i = 1; i <= 4 ; i++) {
        if ((amhw_stm32f103rbt6_tim_status_flg_get(p_hw_tim, 1UL << i)) != 0) {

					  am_led_toggle(LED0);

            /* 清除通道 i 标志 */
            amhw_stm32f103rbt6_tim_status_flg_clr(p_hw_tim, (1UL << i));
        }
    }
}

/**
 * \brief 配置定时器为 PWM 输出(PWM 模式 2)
 */
void tim_pwm_chan_config (amhw_stm32f103rbt6_tim_t *p_hw_tim,
                          uint32_t        chan,
                          uint32_t        duty_ns,
                          uint32_t        period_ns,
                          uint32_t        clk_rate)
{
    uint32_t period_c, duty_c, temp;
    uint16_t pre_real = 1, pre_reg = 0;

    /* 参数不合法 */
    if ((period_ns == 0) || (duty_ns > 4294967295UL) ||
        (period_ns > 4294967295UL) || (duty_ns > period_ns)) {
        return ;
    }

    /* 计算出来得到的是计数值CNT, 公式ns * 10e-9= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_ns) * (clk_rate) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clk_rate) / (uint64_t)1000000000;

    {
         /* 当计数小于65536时，不分频(值为1,1代表为1分频) */
         temp = period_c / 65536 + 1;

         /* 16位定时器需要运算取得合适的分频值 */
         for (pre_real = 1; pre_real < temp; ) {
              pre_reg++;           /* 计算写入寄存器的分频值0,1,2,... */
              pre_real++;          /* 分频数 */
         }
     }

    /* 设置分频值 */
    amhw_stm32f103rbt6_tim_prescale_set(p_hw_tim, pre_reg);

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / pre_real;
    duty_c = duty_c / pre_real;

    /* 设置自动重装寄存器的值 */
    amhw_stm32f103rbt6_tim_arr_set(p_hw_tim, period_c - 1);

    /* 设置比较输出通道的匹配值 */
    amhw_stm32f103rbt6_tim_ccr_ouput_reload_val_set(p_hw_tim,  duty_c - 1, chan);

    /* 计数器清0 */
    amhw_stm32f103rbt6_tim_count_set(p_hw_tim, 0);

    /*　使能定ARR预装载 */
    amhw_stm32f103rbt6_tim_arpe_enable(p_hw_tim);

    /* 选择该通道为输出 */
    amhw_stm32f103rbt6_tim_ccs_set(p_hw_tim, 0, chan);

    /* 选择该通道的模式为PWM模式2 */
    amhw_stm32f103rbt6_tim_ocm_set(p_hw_tim, AMHW_STM32F103RBT6_TIM_PWM_MODE2, chan);

    /* PWM输出通道预装载使能 */
    amhw_stm32f103rbt6_tim_ccs_ocpe_enable(p_hw_tim, chan);

    /* 设置比较输出通道ccp高电平极性有效 */
    amhw_stm32f103rbt6_tim_ccp_output_set(p_hw_tim, 0, chan);
}

void tim_pwm_enable (amhw_stm32f103rbt6_tim_t      *p_hw_tim,
                     amhw_stm32f103rbt6_tim_type_t  type,
                     uint32_t             chan,
                     int32_t              int_num)
{

    /* 使能通道PWM输出 */
    amhw_stm32f103rbt6_tim_cce_output_enable(p_hw_tim, chan);

    /* 高级定时器使能主输出MOE */
    if ((AMHW_STM32F103RBT6_TIM_TYPE0 == type) ||
        (AMHW_STM32F103RBT6_TIM_TYPE2 == type) ||
        (AMHW_STM32F103RBT6_TIM_TYPE3 == type)) {

        amhw_stm32f103rbt6_tim_bdtr_enable(p_hw_tim, AMHW_STM32F103RBT6_TIM_MOE);
    }

    /* 产生更新事件，重新初始化Prescaler计数器 及Repetition计数器 */
    amhw_stm32f103rbt6_tim_egr_set(p_hw_tim, AMHW_STM32F103RBT6_TIM_UG);

    if (amhw_stm32f103rbt6_tim_status_flg_get(p_hw_tim, AMHW_STM32F103RBT6_TIM_UG) != 0) {
        /* 更新定时器时会产生更新事件,清除标志位 */
        amhw_stm32f103rbt6_tim_status_flg_clr(p_hw_tim, AMHW_STM32F103RBT6_TIM_UG);
    }

    /* 连接中断回调函数 */
    am_int_connect(int_num, __stm32f103rbt6_tim_hw_pwm_irq_handler, (void *)p_hw_tim);

    /* 使能中断 */
    amhw_stm32f103rbt6_tim_int_enable(p_hw_tim, (1ul << (chan + 1)));

    am_int_enable(int_num);

    /*　使能定时器TIM允许计数 */
    amhw_stm32f103rbt6_tim_enable(p_hw_tim);
}

/**
 * \brief 定时器TIM PWM输出初始化函数
 *
 * \param[in] p_hw_tim : 指向定时器寄存器块的指针
 * \param[in] clk_id   : 时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \return 无
 */
void tim_pwm_init (amhw_stm32f103rbt6_tim_t     *p_hw_tim, amhw_stm32f103rbt6_tim_type_t type)
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
void demo_stm32f103rbt6_hw_tim_pwm_entry (amhw_stm32f103rbt6_tim_t     *p_hw_tim,
                                amhw_stm32f103rbt6_tim_type_t type,
                                uint32_t            chan,
                                uint32_t            clk_rate,
                                int32_t             inum)
{

    /* 初始化定时器TIM为PWM功能 */
    tim_pwm_init(p_hw_tim, type);

    /* 配置定时器TIM PWM输出通道 */
    tim_pwm_chan_config(p_hw_tim,
                        chan,
                        250000 / 2,
                        250000,
                        clk_rate);

    tim_pwm_enable(p_hw_tim, type, chan, inum);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zlg116_hw_tim_pwm] */

/* end of file */
