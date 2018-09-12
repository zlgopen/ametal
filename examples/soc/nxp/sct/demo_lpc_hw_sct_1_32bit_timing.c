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
 * \brief SCT 32 位定时例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. LED0 以 10Hz 的频率闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器；
 *    3. 本例程使用了 SCT 事件 0，用于产生周期性中断，使用匹配通道 0，事件发生计数器
 *       清零。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_sct_1_32bit_timing.c src_lpc_hw_sct_1_32bit_timing
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-08  hgo, modified
 * - 1.00 15-07-13  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_sct_1_32bit_timing
 * \copydoc demo_lpc_hw_sct_1_32bit_timing.c
 */

/** [src_lpc_hw_sct_1_32bit_timing] */
#include "ametal.h"
#include "am_int.h"
#include "am_board.h"
#include "hw/amhw_lpc_sct.h"

/**
 * \brief SCT0 中断服务函数
 *
 * \param[in] p_arg 用户自定义参数，由 am_int_connect() 函数传递
 *
 * \return  无
 */
am_local void __sct0_irq_handler (void *p_arg)
{
    amhw_lpc_sct_t *p_hw_sct = (amhw_lpc_sct_t *)p_arg;
    /* 事件 0 产生中断 */
    if (amhw_lpc_sct_event_flag_chk(p_hw_sct,
                                    AMHW_LPC_SCT_EVT(0)) == AM_TRUE ) {
        am_led_toggle(LED0);

        /* 清除标志 */
        amhw_lpc_sct_event_flag_clr(p_hw_sct, AMHW_LPC_SCT_EVT(0));
    }
}

/**
 * \brief 初始化 SCT 为定时器功能
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] freq     中断频率。
 *
 * \return 无
 */
am_local void __sct_timer_init (amhw_lpc_sct_t *p_hw_sct, 
                                uint32_t        freq, 
                                int             inum,
                                uint32_t        frq)
{
    amhw_lpc_sct_config(p_hw_sct,
                        AMHW_LPC_SCT_CONFIG_32BIT_COUNTER  | /* 使用 32 位定时器 */
                        AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK); /* 使用系统时钟 */

    /* 将匹配捕捉寄存器配置为匹配功能 */
    amhw_lpc_sct_regmode_config(p_hw_sct,

                                /* SCT 模式为 UNIFY(1 32-bit 计数器) */
                                AMHW_LPC_SCT_MODE_UNIFY,

                                /* 匹配/捕获 0 */
                                0,

                                /* 操作匹配寄存器 */
                                AMHW_LPC_SCT_MATCAP_MATCH);

    /* 设置匹配 0 和重载匹配值 */
    amhw_lpc_sct_mat_val_set(p_hw_sct,
                             AMHW_LPC_SCT_MODE_UNIFY,
                             AMHW_LPC_SCT_MAT(0),
                             frq / freq - 1);

    amhw_lpc_sct_mat_reload_val_set(
                               p_hw_sct,
                               AMHW_LPC_SCT_MODE_UNIFY,
                               AMHW_LPC_SCT_MAT(0),
                               frq / freq - 1);

    /* 设置事件 0 控制寄存器 */
    amhw_lpc_sct_event_ctrl(p_hw_sct,

                            /* 事件 0 */
                            AMHW_LPC_SCT_EVT(0),

                            /* 事件关联到匹配 0 */
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(0)    |

                            /* 只能使用指定的匹配寄存器 */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH |

                            /*  STATEV 值加上 STATE */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD    |

                            /* 加 0，STATE 没有改变 */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));

    /* 设置当前状态值为 0 */
    amhw_lpc_sct_state_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);

    /* 使能状态 0 中事件 0 产生 */
    amhw_lpc_sct_event_state_enable(p_hw_sct,
                                    AMHW_LPC_SCT_EVT(0),
                                    AMHW_LPC_SCT_STATE(0));

    /* 使能事件 0 作限定计数器 */
    amhw_lpc_sct_event_limit_enable(p_hw_sct,
                                    AMHW_LPC_SCT_MODE_UNIFY,
                                    AMHW_LPC_SCT_EVT(0));

    /* 使能事件 0 产生中断 */
    amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(0));

    /* 连接 SCT0 的中断服务函数 */
    am_int_connect(inum, __sct0_irq_handler, (void *)p_hw_sct);

    /* 使能 SCT0 中断 */
    am_int_enable(inum);

    /* 置 CLRCTR 位以清计数器为 0 */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);

    /* 预分频为 0，使用系统时钟 */
    amhw_lpc_sct_prescale_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);
}

/**
 * \brief 使能 SCT 为定时器(开始计数)
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 *
 * \return 无
 */
am_local void __sct_timer_enable (amhw_lpc_sct_t *p_hw_sct)
{

    /* 清终止位，SCT 开始运行 */
    amhw_lpc_sct_ctrl_clr(p_hw_sct,
                          AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
}

void demo_lpc_hw_sct_1_32bit_timing_entry (amhw_lpc_sct_t *p_hw_sct, 
                                              int            inum, 
                                              uint32_t       frq)
{
    /* 使用 SCT 实现定时功能，中断频率为 10Hz */
    __sct_timer_init(p_hw_sct, 10, inum, frq);

    /* 使能 SCT 为定时器 */
    __sct_timer_enable(p_hw_sct);

    AM_FOREVER {
        ; /* VOID */
    }
}

/** [src_lpc_hw_sct_1_32bit_timing] */

/* end of file */
