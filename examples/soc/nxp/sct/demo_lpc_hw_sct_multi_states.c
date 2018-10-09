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
 * \brief SCT 操作多状态例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. PIO0_16(SCT_OUT5)引脚输出波形。
 *
 * \code
 *  |---         状态0        ---|--- 状态1 ---|--- 状态0      ......
 *             _____     _____     ___         __            ___
 *            |     |   |     |   |   |       |  |          |     ......
 *  __________|     |___|     |___|   |_______|  |__________|
 *
 *  0         10    15  18    23  26  3      10  12         10     (ms)
 *                              (状态1, 0)     (状态0, 0)
 *          事件 0  1   2      3   4   5      6  7
 * \endcode
 *
 * \note
 *    1. 连接 PIO0_16(SCT_OUT5) 引脚到逻辑分析仪，将会看到该波形，前 10ms 为低电平；
 *    2. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_sct_multi_states.c src_lpc_hw_sct_multi_states
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-08  hgo, modified
 * - 1.00 14-12-09  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_sct_multi_states
 * \copydoc demo_lpc_hw_sct_multi_states.c
 */

/** [src_lpc_hw_sct_multi_states] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc_sct.h"

#define   __OUT_NUM       AMHW_LPC_SCT_OUT(5)       /**< \brief 使用 SCT_OUT5 */

/**
 * \brief 初始化匹配寄存器
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] num      匹配
 * \param[in] ms       匹配时间，单位为 ms
 *
 * \return 无
 */
am_local void __sct_match_init (amhw_lpc_sct_t *p_hw_sct,
                                uint32_t        num,
                                uint32_t        ms,
                                uint32_t        frq)
{
    uint32_t ms_ticks = frq / 1000;

    /* 将匹配捕捉寄存器配置为匹配功能 */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                                AMHW_LPC_SCT_MODE_UNIFY,
                                AMHW_LPC_SCT_MAT(num),
                                AMHW_LPC_SCT_MATCAP_MATCH);

    /* 设置匹配 0 和重载匹配值 */
    amhw_lpc_sct_mat_val_set(p_hw_sct,
                             AMHW_LPC_SCT_MODE_UNIFY,
                             num,
                             ms_ticks * ms - 1);

    amhw_lpc_sct_mat_reload_val_set(p_hw_sct,
                                    AMHW_LPC_SCT_MODE_UNIFY,
                                    num,
                                    ms_ticks * ms - 1);
}

/**
 * \brief 初始化一个事件
 *
 * \param[in] p_hw_sct  指向 SCT 寄存器块的指针
 * \param[in] evt_num   事件
 * \param[in] match_num 匹配
 * \param[in] state_num 状态
 * \param[in] state_ld  0: state_ev 加上状态  1: the state_ev 加载为状态
 * \param[in] state_ev  该值改变状态
 *
 * \return 无
 */
am_local void __sct_evt_init (amhw_lpc_sct_t *p_hw_sct,
                              uint32_t        evt_num,
                              uint32_t        match_num,
                              uint32_t        state_num,
                              uint32_t        state_ld,
                              uint32_t        state_ev)
{
    if(state_ld == 0) {
        state_ld = AMHW_LPC_SCT_EV_CTRL_STATELD_ADD;
    } else {
        state_ld = AMHW_LPC_SCT_EV_CTRL_STATELD_LOAD;
    }

    /* 设置事件 0 控制 */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            evt_num,

                            /* 事件关联匹配 */
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(match_num) |

                            /* 使用指定匹配 */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH       |
                            state_ld                                  |
                            AMHW_LPC_SCT_EV_CTRL_STATEV(state_ev));

    /* 使能事件产生 */
    amhw_lpc_sct_event_state_enable(p_hw_sct, evt_num, state_num);
}

/**
 * \brief 使用 SCT 输出复杂波形
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 *
 * \return 无
 */
am_local void __sct_init (amhw_lpc_sct_t *p_hw_sct,
                          uint32_t        frq)
{
    amhw_lpc_sct_config(p_hw_sct,

                        /* 作 32 位定时器 */
                        AMHW_LPC_SCT_CONFIG_32BIT_COUNTER  |

                        /* 系统时钟做 SCT 时钟 */
                        AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK);

    __sct_match_init(p_hw_sct, AMHW_LPC_SCT_MAT(0), 10, frq); /* 设置匹配 0 值为 10ms */
    __sct_match_init(p_hw_sct, AMHW_LPC_SCT_MAT(1), 15, frq); /* 设置匹配 1 值为 15ms */
    __sct_match_init(p_hw_sct, AMHW_LPC_SCT_MAT(2), 18, frq); /* 设置匹配 2 值为 18ms */
    __sct_match_init(p_hw_sct, AMHW_LPC_SCT_MAT(3), 23, frq); /* 设置匹配 3 值为 23ms */
    __sct_match_init(p_hw_sct, AMHW_LPC_SCT_MAT(4), 26, frq); /* 设置匹配 4 值为 26ms */
    __sct_match_init(p_hw_sct, AMHW_LPC_SCT_MAT(5), 3 , frq); /* 设置匹配 5 值为 3ms */
    __sct_match_init(p_hw_sct, AMHW_LPC_SCT_MAT(6), 12, frq); /* 设置匹配 6 值为 12ms */

    amhw_lpc_sct_output_state_clr(p_hw_sct, __OUT_NUM);  /* 清除输出 */

    /* 状态 0 事件控制 */

    /* 事件 0，关联匹配 0，不改变状态值，输出 */
    __sct_evt_init(p_hw_sct,
                   AMHW_LPC_SCT_EVT(0),
                   AMHW_LPC_SCT_MAT(0),
                   AMHW_LPC_SCT_STATE(0),
                   0,
                   0);
    amhw_lpc_sct_out_set_enable(p_hw_sct, __OUT_NUM, AMHW_LPC_SCT_EVT(0));

    /* 事件 1，关联匹配 1，不改变状态值，清除输出 */
    __sct_evt_init(p_hw_sct,
                   AMHW_LPC_SCT_EVT(1),
                   AMHW_LPC_SCT_MAT(1),
                   AMHW_LPC_SCT_STATE(0),
                   0,
                   0);
    amhw_lpc_sct_out_clr_enable(p_hw_sct, __OUT_NUM, AMHW_LPC_SCT_EVT(1));

    /* 事件 2，关联匹配 2，不改变状态值，输出 */
    __sct_evt_init(p_hw_sct,
                   AMHW_LPC_SCT_EVT(2),
                   AMHW_LPC_SCT_MAT(2),
                   AMHW_LPC_SCT_STATE(0),
                   0,
                   0);
    amhw_lpc_sct_out_set_enable(p_hw_sct, __OUT_NUM, AMHW_LPC_SCT_EVT(2));

    /* 事件 3，关联匹配 3，不改变状态值，清除输出 */
    __sct_evt_init(p_hw_sct,
                   AMHW_LPC_SCT_EVT(3),
                   AMHW_LPC_SCT_MAT(3),
                   AMHW_LPC_SCT_STATE(0),
                   0,
                   0);
    amhw_lpc_sct_out_clr_enable(p_hw_sct, __OUT_NUM, AMHW_LPC_SCT_EVT(3));

    /* 事件 4，关联匹配 4，改变状态值为 1，清除输出 */
    __sct_evt_init(p_hw_sct,
                   AMHW_LPC_SCT_EVT(4),
                   AMHW_LPC_SCT_MAT(4),
                   AMHW_LPC_SCT_STATE(0),
                   0,
                   1);
    amhw_lpc_sct_out_set_enable(p_hw_sct, __OUT_NUM, AMHW_LPC_SCT_EVT(4));

    /* 在状态 0 中，事件 4 作为限制事件 */
    amhw_lpc_sct_event_limit_enable(p_hw_sct,
                                    AMHW_LPC_SCT_MODE_UNIFY,
                                    AMHW_LPC_SCT_EVT(4));

    /* 状态 1 事件控制 */

    /* 事件 5，关联匹配 5(3ms)，不改变状态值，清除输出 */
    __sct_evt_init(p_hw_sct,
                   AMHW_LPC_SCT_EVT(5),
                   AMHW_LPC_SCT_MAT(5),
                   AMHW_LPC_SCT_STATE(1),
                   0,
                   0);
    amhw_lpc_sct_out_clr_enable(p_hw_sct, __OUT_NUM, AMHW_LPC_SCT_EVT(5));

    /* 事件 6，关联匹配 0(10ms)，不改变状态值，输出 */
    __sct_evt_init(p_hw_sct,
                   AMHW_LPC_SCT_EVT(6),
                   AMHW_LPC_SCT_MAT(0),
                   AMHW_LPC_SCT_STATE(1),
                   0,
                   0);
    amhw_lpc_sct_out_set_enable(p_hw_sct, __OUT_NUM, AMHW_LPC_SCT_EVT(6));

    /* 事件 7，关联匹配 6(12ms)，改变状态值为 0，清除输出 */
    __sct_evt_init(p_hw_sct,
                   AMHW_LPC_SCT_EVT(7),
                   AMHW_LPC_SCT_MAT(6),
                   AMHW_LPC_SCT_STATE(1),
                   AMHW_LPC_SCT_EV_CTRL_STATELD_LOAD,
                   0);
    amhw_lpc_sct_out_clr_enable(p_hw_sct, __OUT_NUM, AMHW_LPC_SCT_EVT(7));

    /* 使能事件 7 限制计数器值 */
    amhw_lpc_sct_event_limit_enable(p_hw_sct,
                                    AMHW_LPC_SCT_MODE_UNIFY,
                                    AMHW_LPC_SCT_EVT(7));

    /* 设置当前状态值为 0 */
    amhw_lpc_sct_state_set(p_hw_sct,
                           AMHW_LPC_SCT_MODE_UNIFY,
                           AMHW_LPC_SCT_STATE(0));

    /* 置 CLRCTR 位以清计数器为 0 */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);

    /* 预分频为 0，使用系统时钟 */
    amhw_lpc_sct_prescale_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);
}

/**
 * \brief 使能 SCT 输出复杂波形
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 *
 * \return 无
 */
am_local void __sct_enable (amhw_lpc_sct_t *p_hw_sct)
{

    /* 清终止位，SCT 开始运行 */
    amhw_lpc_sct_ctrl_clr(p_hw_sct,
                          AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
}

/**
 * \brief 禁止 SCT 输出复杂波形
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 *
 * \return 无
 */
am_local void __sct_disable (amhw_lpc_sct_t *p_hw_sct)
{

    /* 置终止位，SCT 终止运行 */
    amhw_lpc_sct_ctrl_set(p_hw_sct,
                          AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
} 


void demo_lpc_hw_sct_multi_states_entry(amhw_lpc_sct_t *p_hw_sct,
                                        uint32_t        frq)
{
    /* SCT 禁能 */
    __sct_disable(p_hw_sct);

    /* SCT 实现定时器功能 */
    __sct_init(p_hw_sct, frq);

    /* SCT 使能 */
    __sct_enable(p_hw_sct);

    AM_FOREVER {
        ;
    }
}
/** [src_lpc_hw_sct_multi_states] */

/* end of file */
