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
 * \brief SCT 作为 2 个 16 位定时器定时例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. LED0 以 10Hz 的频率闪烁;
 *   2. 蜂鸣器引脚(PIO0_24)状态以 8KHz 的频率翻转，输出波形为 4KHz，
 *      每次鸣叫 50 毫秒，鸣叫频率为 10Hz。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 蜂鸣器需要短接 J7 跳线帽，才能被 PIO0_24 控制；
 *    3. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不默认初始化蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_sct_2_16bit_timing.c src_lpc_hw_sct_2_16bit_timing
 *
 * \internal
 * \par Modification history
 * - 1.02 15-12-14  hgo, change PIO0_15 to PIO0_24 .
 * - 1.01 15-11-19  hgo, modified
 * - 1.00 15-07-13  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_sct_2_16bit_timing
 * \copydoc demo_lpc_hw_sct_2_16bit_timing.c
 */

/** [src_lpc_hw_sct_2_16bit_timing] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_gpio.h"
#include "hw/amhw_lpc_sct.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define   __SCT_HANDLE_H        1            /**< \brief 用于操作高组定时器 */
#define   __SCT_HANDLE_L        0            /**< \brief 用于操作低组定时器 */

volatile am_bool_t  __g_beep_on =  AM_FALSE; /**< \brief 蜂鸣器工作标志位 */

static int __buzzer_pin = 24;
/**
 * \brief 本例程使用了 2 个 SCT 事件
 *
 * 1. 事件 0：用于周期性中断，使用匹配通道 0 的低 16 位，事件发生计数器低组清零；
 * 2. 事件 1：用于周期性中断，使用匹配通道 0 的高 16 位，事件发生计数器高组清零；
 *
 * 所有事件在 SCT 状态 0 下有效，事件发生后 SCT 状态不发生改变；
 */

/**
 * \brief 计算 16 位定时器指定计数值的时候 ，最佳的预分频值和匹配寄存器值
 *
 * \param[in]  count   计数值
 * \param[out] p_match 指向定时器匹配值的指针
 *
 * \return 计算出的 8bits 预分频值
 */
am_local uint32_t __prescale_and_match_calculate (uint32_t  count,
                                                  uint32_t *p_match)
{
    uint32_t match, min_remainder, prescale;
    uint32_t result_quot, result_rem;

    min_remainder = count;
    prescale      = 1;
   *p_match       = 65536;

    /**
     * 计数值最小为 65536，
     * 预分频为 1，匹配值等于计数值
     */
    if (count <= 65536) {
        prescale = 1;
       *p_match = count;

        return prescale;
    }

    /* 检查匹配值，直到找到最好的 */
    for (match = 65536; match > 0; match--) {

        result_quot = count / match;
        result_rem  = count % match;

        /*
         * 由于预分频宽度为 8bits，result_quot 的最大值为 256，如果超过 256，计数值
         * 将无法达到
         */
        if (result_quot <= 256) {
            if (result_rem == 0) {

                /* 发现最合适的预分频和匹配值 */
                prescale = result_quot;
                *p_match = match;
                break;

            } else if (result_rem < min_remainder) {

                /* 发现更合适的预分频和匹配值 */
                min_remainder = result_rem;
                prescale      = result_quot;
               *p_match      = match;
            }
        }
    }

    return prescale;
}

/**
 * \brief SCT0 中断服务函数
 *
 * \param[in] p_arg 用户自定义参数，由 am_int_connect() 函数传递
 *
 * \return 无
 */
am_local void __sct0_irq_handler (void *p_arg)
{

    amhw_lpc_sct_t *p_hw_sct = (amhw_lpc_sct_t *)p_arg;
    /* 事件 0 中断 */
    if (amhw_lpc_sct_event_flag_chk(p_hw_sct,
                                    AMHW_LPC_SCT_EVT(0)) == AM_TRUE) {

        am_led_toggle(LED0);

        /* 清标志 */
        amhw_lpc_sct_event_flag_clr(p_hw_sct, AMHW_LPC_SCT_EVT(0));

        /* 蜂鸣器进行鸣叫 */
        __g_beep_on = AM_TRUE;
    }

    /* 事件 1 中断 */
    if (amhw_lpc_sct_event_flag_chk(p_hw_sct,
                                    AMHW_LPC_SCT_EVT(1)) == AM_TRUE) {

        /* 蜂鸣器工作 */
        am_gpio_toggle(__buzzer_pin);

        amhw_lpc_sct_event_flag_clr(p_hw_sct,
                                    AMHW_LPC_SCT_EVT(1));
    }
}

/**
 * \brief 使能定时器
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] handle   高组定时器：#__SCT_HANDLE_H，低组定时器：#__SCT_HANDLE_L
 *
 * \return 无
 */
am_local void __sct_2_16bit_timer_enable (amhw_lpc_sct_t *p_hw_sct,
                                          uint8_t         handle)
{
    if (handle == __SCT_HANDLE_H) {

        /* 清终止位，SCT 高组计数器开始运行 */
        amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_H |
                                        AMHW_LPC_SCT_CTRL_HALT_H);

    } else {

        /* 清终止位，SCT 低组计数器开始运行 */
        amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L |
                                        AMHW_LPC_SCT_CTRL_HALT_L);
    }
}

/**
 * \brief 禁能定时器
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] handle   高组定时器：#__SCT_HANDLE_H，低组定时器：#__SCT_HANDLE_L
 *
 * \return 无
 */
am_local void __sct_2_16bit_timer_disable (amhw_lpc_sct_t *p_hw_sct,
                                           uint8_t         handle)
{
    if (handle == __SCT_HANDLE_H) {

        /* 置终止位，SCT 高组计数器终止运行 */
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_H |
                                        AMHW_LPC_SCT_CTRL_HALT_H);

    } else {

        /* 置终止位，SCT 低组计数器终止运行 */
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L |
                                        AMHW_LPC_SCT_CTRL_HALT_L);
    }
}

/**
 * \brief 初始化定时器
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] handle   高组定时器：#__SCT_HANDLE_H，低组定时器：#__SCT_HANDLE_L
 * \param[in] freq     定时器中断频率
 *
 * \return 无
 *
 * \note 低组定时器使用事件 0，高组定时器使用事件 1
 */
am_local void __sct_2_16bit_timer_init (amhw_lpc_sct_t *p_hw_sct,
                                        uint8_t         handle,
                                        uint32_t        freq,
                                        uint32_t        frq)
{
    uint32_t counters;
    uint32_t prescale, match;

    amhw_lpc_sct_mode_t  sct_mode;
    uint32_t             event_num;
    uint32_t             event_ctl;


    if (handle == __SCT_HANDLE_H) {

        sct_mode  = AMHW_LPC_SCT_MODE_SEPARATE_H;
        event_num = AMHW_LPC_SCT_EVT(1);           /* 高组定时器使用事件 1 */

        /* 事件关联到匹配高16bits */
        event_ctl = AMHW_LPC_SCT_EV_CTRL_HEVENT_H;

    } else {
        sct_mode  = AMHW_LPC_SCT_MODE_SEPARATE_L;
        event_num = AMHW_LPC_SCT_EVT(0);           /* 低组定时器使用事件 0 */

        /* 事件关联到匹配低 16bits */
        event_ctl = AMHW_LPC_SCT_EV_CTRL_HEVENT_L;
    }

    /* 配置匹配/捕获寄存器 0 为 16bits 匹配寄存器 */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                                sct_mode,
                                AMHW_LPC_SCT_MAT(0),
                                AMHW_LPC_SCT_MATCAP_MATCH);

    /**
     * 计算 16 位定时器预分频和匹配值。例如，要产生 10Hz 的中断，系统时钟频率为
     * 100MHz，定时器计数值为：100 000 000 / 10 = 10 000 000，
     * 计数器为 16bits，最大值为 65536，故使用预分频：10 000 000 = 200 * 50000；
     * 匹配值为：50000；
     * 预分频值为：200
     */
    counters = frq / freq;

    prescale = __prescale_and_match_calculate(counters, &match);

    prescale -= 1;
    match    -= 1;

    /* 如要改变预分频，应先终止 SCT */
    if (prescale != amhw_lpc_sct_prescale_get(p_hw_sct, sct_mode)) {

        if (amhw_lpc_sct_halt_check(p_hw_sct, sct_mode) == AM_TRUE ) {
            amhw_lpc_sct_prescale_set(p_hw_sct, sct_mode, prescale);
        } else {
            __sct_2_16bit_timer_disable(p_hw_sct, handle);
            amhw_lpc_sct_prescale_set(p_hw_sct, sct_mode, prescale);
            __sct_2_16bit_timer_enable(p_hw_sct, handle);
        }
    }

    /* 只有处于 HALT 状态下时，才能设置匹配寄存器时 */
    if (amhw_lpc_sct_halt_check(p_hw_sct, sct_mode) == AM_TRUE ) {
        amhw_lpc_sct_mat_val_set(p_hw_sct,
                                 sct_mode,
                                 AMHW_LPC_SCT_MAT(0),
                                 match);
    }

    /* 设置重载匹配值 */
    amhw_lpc_sct_mat_reload_val_set(p_hw_sct,
                                    sct_mode,
                                    AMHW_LPC_SCT_MAT(0),
                                    match);

    /* 高组定时器使用事件 1，配置事件 1 */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            event_num,
                            event_ctl                           |

                            /* 事件关联到匹配 0 */
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(0)    |

                            /* 只能使用指定匹配 */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH |

                            /* STATEV 值加上 STATE */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD    |

                            /* 加 0，STATE 没有改变 */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));

    /* 设置当前状态值为 0 */
    amhw_lpc_sct_state_set(p_hw_sct, sct_mode, AMHW_LPC_SCT_STATE(0));

    /* 使能状态 0 中产生事件 */
    amhw_lpc_sct_event_state_enable(p_hw_sct,
                                    event_num,
                                    AMHW_LPC_SCT_STATE(0));

    /* 使能事件做计数器限定 */
    amhw_lpc_sct_event_limit_enable(p_hw_sct, sct_mode, event_num);

    /* 使能事件产生中断 */
    amhw_lpc_sct_event_int_enable(p_hw_sct, event_num);

    if (handle == __SCT_HANDLE_H) {

        /* 设置 CLRCTR 位以清高组计数器为 0 */
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_H);
    } else {
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);
    }
}

/**
 * \brief SCT 实现定时功能
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 *
 * \return 无
 */
am_local void __sct_2_16bit_init (amhw_lpc_sct_t *p_hw_sct, 
                                  uint32_t        frq)
{
    amhw_lpc_sct_config(p_hw_sct,

                        /* 定时器实现 2 个 16bits 定时器 */
                        AMHW_LPC_SCT_CONFIG_16BIT_COUNTER  |

                        /* SCT 时钟使用系统时钟 */
                        AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK);

    /* 使能低组定时器中断，中断频率为 10Hz */
    __sct_2_16bit_timer_init(p_hw_sct, __SCT_HANDLE_L, 10, frq);

    /* 使能高组定时器中断，中断频率为 8KHz */
    __sct_2_16bit_timer_init(p_hw_sct, __SCT_HANDLE_H, 8000, frq);
}

void demo_lpc_hw_sct_2_16bit_timing_entry (amhw_lpc_sct_t *p_hw_sct, 
                                           int             inum, 
                                           uint32_t        frq,
                                           int             buzzer_pin)
{
   /* 蜂鸣器引脚 */
    __buzzer_pin = buzzer_pin;
  
    /* 使用 SCT 实现定时功能 */
    __sct_2_16bit_init(p_hw_sct, frq);

    am_int_connect(inum, __sct0_irq_handler, (void *)p_hw_sct);

    /* 使能 SCT0 中断 */
    am_int_enable(inum);

    /* 使能低组定时器 */
    __sct_2_16bit_timer_enable(p_hw_sct, __SCT_HANDLE_L);

    AM_FOREVER {

        if (__g_beep_on == AM_TRUE) {

            /* 使能高组定时器，运行蜂鸣器输出 */
            __sct_2_16bit_timer_enable(p_hw_sct, __SCT_HANDLE_H);

            am_mdelay(50);

            /* 禁能高组定时器，停止蜂鸣器输出 */
            __sct_2_16bit_timer_disable(p_hw_sct, __SCT_HANDLE_H);

            __g_beep_on =  AM_FALSE;
        }
    }
}
/** [src_lpc_hw_sct_2_16bit_timing] */

/* end of file */
