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
 * \brief SCT 32 位 PWM 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. PIO0_23(SCT_OUT0) 输出 4kHz 的 PWM，占空比为 50%；
 *   2. PIO0_27(SCT_OUT4) 输出 4kHz 的 PWM，占空比为 25%；
 *   3. LED0 以 0.2s 的时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. SCT 使用 32 位计数器，只能产生一组独立的 PWM，所有 PWM 输出使用同一频率，
 *       计数器和最大匹配值为 0xFFFFFFFF；
 *    3. SCT 使用 16 位计数器，可以产生 2 组独立的 PWM，每一组 PWM 的输出使用同一
 *       频率，计数器和最大匹配值为 0xFFFF；
 *    4. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_sct_1_32bit_pwm.c src_lpc_hw_sct_1_32bit_pwm
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-19  hgo, modified
 * - 1.00 15-07-13  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_sct_1_32bit_pwm
 * \copydoc demo_lpc_hw_sct_1_32bit_pwm.c
 */

/** [src_lpc_hw_sct_1_32bit_pwm] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc_sct.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/**
 * \brief 本例程使用了三个 SCT 事件
 *
 * 1. 事件 0：用于产生 2 路 PWM 的共同周期，使用匹配通道 0，周期性控制
 *           __PWM0_OUT_NUM、__PWM0_OUT_NUM 置位，事件发生计数器清零；
 * 2. 事件 1：用于控制第 1 路 PWM 的占空比，使用匹配通道 __PWM0_MAT_NUM，周期性控制
 *           __PWM0_OUT_NUM 清零；
 * 3. 事件 2：用于控制第 2 路 PWM 的占空比，使用匹配通道 __PWM1_MAT_NUM，周期性控制
 *           __PWM1_OUT_NUM 清零；
 * 所有事件在 SCT 状态 0 下有效，事件发生后 SCT 状态不发生改变；
 */
#define __PWM0_MAT_NUM   AMHW_LPC_SCT_MAT(1)   /**< \brief PWM0，使用匹配 1 */
#define __PWM0_OUT_NUM   (0)                   /**< \brief PWM0, SCT_OUT0 */

#define __PWM1_MAT_NUM   AMHW_LPC_SCT_MAT(2)   /**< \brief PWM1，使用匹配 2 */
#define __PWM1_OUT_NUM   (4)                   /**< \brief PWM1, SCT_OUT4 */

/**
 * \brief 初始化 SCT 为 32bit 定时器，使能 MATCH0 以输出 PWM
 *
 * \param[in] p_hw_sct 指向SCT寄存器块的指针
 *
 * \return 无
 */
am_local void __sct_pwm_init (amhw_lpc_sct_t *p_hw_sct)
{

    /* 初始化匹配 0 为自动限定，以确定 PWM 的周期 */
    amhw_lpc_sct_config(p_hw_sct,

                        /* 使用 32-bit 定时器 */
                        AMHW_LPC_SCT_CONFIG_32BIT_COUNTER  |

                        /* SCT 时钟使用系统时钟 */
                        AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK |

                        /* 匹配 0 为自动限定 */
                        AMHW_LPC_SCT_CONFIG_AUTOLIMIT_L );

    /* 将匹配捕捉寄存器配置为匹配功能 */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                                AMHW_LPC_SCT_MODE_UNIFY,
                                AMHW_LPC_SCT_MAT(0),
                                AMHW_LPC_SCT_MATCAP_MATCH);

    /* 设置匹配寄存器的匹配值 */
    amhw_lpc_sct_mat_val_set(p_hw_sct,
                             AMHW_LPC_SCT_MODE_UNIFY,
                             AMHW_LPC_SCT_MAT(0), 0);

    /* 设置事件 0 为 PWM0 */
    amhw_lpc_sct_event_ctrl(p_hw_sct,

                            /* 事件 0 */
                            AMHW_LPC_SCT_EVT(0),

                            /* 事件关联到匹配 0 */
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(0)    |

                            /* 只能使用指定的匹配寄存器 */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH |

                            /* STATEV 值加上 STATE */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD    |

                            /* 加 0，STATE 没有改变 */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));

    /* 使能状态 0 中事件 0 产生 */
    amhw_lpc_sct_event_state_enable(p_hw_sct,
                                    AMHW_LPC_SCT_EVT(0),
                                    AMHW_LPC_SCT_STATE(0));

    /* 置 CLRCTR 位，以清计数器为 0 */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);

    /* 置当前状态值为 0 */
    amhw_lpc_sct_state_set(p_hw_sct,
                           AMHW_LPC_SCT_MODE_UNIFY,
                           AMHW_LPC_SCT_STATE(0));

    /* 预分频为 0，使用系统时钟 */
    amhw_lpc_sct_prescale_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);
}

/**
 * \brief 初始化一路 PWM
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] mat_num  匹配编号
 * \param[in] out_num  输出通道 0~5
 *
 * \note 匹配 0 用于确定 PWM 周期，故 mat_num 不能为 AMHW_LPC_SCT_MAT(0)。
 */
am_local void __sct_pwm_out_init (amhw_lpc_sct_t *p_hw_sct,
                                  uint32_t        mat_num,
                                  uint8_t         out_num)
{
    uint8_t evt_num = (uint8_t)mat_num;

    if (mat_num == AMHW_LPC_SCT_MAT(0)) {
        return;
    }

    /* 将匹配捕捉寄存器配置为匹配功能 */
    amhw_lpc_sct_regmode_config(p_hw_sct,

                                /* SCT 模式为 UNIFY(1 32-bit 计数器) */
                                AMHW_LPC_SCT_MODE_UNIFY,

                                /* 匹配通道 */
                                mat_num,

                                /* 操作匹配寄存器 */
                                AMHW_LPC_SCT_MATCAP_MATCH);

    /* 使用事件号同于匹配号 */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            evt_num,
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(mat_num) |

                            /* 只能使用指定的匹配寄存器 */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH    |

                            /*  STATEV 值加上 STATE */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD       |

                            /* 加 0，STATE 没有改变 */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));

    /* 使能状态 0 中事件 0 产生 */
    amhw_lpc_sct_event_state_enable(p_hw_sct, evt_num, AMHW_LPC_SCT_STATE(0));

    /* 事件 0 输出 PWM */
    amhw_lpc_sct_out_set_enable(p_hw_sct, out_num, AMHW_LPC_SCT_EVT(0));

    /* 清 PWM 输出 */
    amhw_lpc_sct_out_clr_enable(p_hw_sct, out_num, evt_num);

    /**
     * 当有效时间等于周期将会产生冲突。在该种情况下，占空比为 100%，
     * 故此时要置位输出。
     */
    amhw_lpc_sct_conflict_res_config(p_hw_sct, out_num,
                                     AMHW_LPC_SCT_RES_SET_OUTPUT);

    /* 置位和清零不依赖于方向 */
    amhw_lpc_sct_output_dir_ctrl(p_hw_sct, out_num,
                                 AMHW_LPC_SCT_OUTPUTDIRCTRL_INDEPENDENT);
}

/**
 * \brief 配置 PWM 占空比时间
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] num      匹配号
 * \param[in] duty_ns  有效时间，单位为 ns
 *
 * \return 无
 *
 * \note 如果想让该配置马上生效，应在调用该函数前禁止 SCT 的
 *       PWM(__sct_pwm_disable())，调用该函数后，再使能 (__sct_pwm_enable())
 *       如果 SCT 没有终止，该值将会写入重载匹配值，并在下一周期生效
 */
am_local void __sct_pwm_duty_config (amhw_lpc_sct_t *p_hw_sct,
                                     uint32_t        num,
                                     uint32_t        duty_ns,
                                     uint32_t        frq)
{

    /* 将时间转换为占空有效时间 */
    uint32_t duty_c = (uint64_t)(duty_ns) *
                      frq /
                      (uint64_t)1000000000;

    /* 占空有效时间至少为 1 */
    if (duty_c == 0) {
        duty_c = 1;
    }

    /* 只有处于 HALT 状态下时，才能设置匹配寄存器时 */
    if (amhw_lpc_sct_halt_check(p_hw_sct,
                                AMHW_LPC_SCT_MODE_UNIFY) == AM_TRUE ) {

        amhw_lpc_sct_mat_val_set(p_hw_sct,
                                 AMHW_LPC_SCT_MODE_UNIFY,
                                 num,
                                 duty_c - 1);
    }

    /* 设置重载匹配值 */
    amhw_lpc_sct_mat_reload_val_set(p_hw_sct,
                                    AMHW_LPC_SCT_MODE_UNIFY,
                                    num,
                                    duty_c - 1);

}

/**
 * \brief 使能 SCT 的 PWM 输出
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 *
 * \return 无
 */
am_local void __sct_pwm_enable (amhw_lpc_sct_t *p_hw_sct)
{

    /* 清除终止位，SCT 开始运行 */
    amhw_lpc_sct_ctrl_clr(p_hw_sct,
                          AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
}

/**
 * \brief 禁能 SCT 的 PWM 输出
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 *
 * \return 无
 */
am_local void __sct_pwm_disable (amhw_lpc_sct_t *p_hw_sct)
{

    /* 置终止位，SCT 终止运行 */
    amhw_lpc_sct_ctrl_set(p_hw_sct,
                          AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);
}

/**
 * \brief 配置 PWM 周期
 *
 * \param[in] p_hw_sct  指向 SCT 寄存器块的指针
 * \param[in] period_ns 周期，单位为 ns
 *
 * \return 无
 *
 * \note 如果想让该配置马上生效，应在调用该函数前禁止 SCT 的
 *       PWM(__sct_pwm_disable())，调用该函数后，再使能 (__sct_pwm_enable())
 */
am_local void __sct_pwm_period_config (amhw_lpc_sct_t *p_hw_sct,
                                       uint32_t        period_ns,
                                       uint32_t        frq)
{

    /* 将时间转变为周期 */
    uint32_t period_c = (uint64_t)(period_ns) *
                        frq /
                        (uint64_t)1000000000;

    /* 周期至少为 1 */
    if (period_c == 0) {
        period_c = 1;
    }

    /* 只有处于 HALT 状态下时，才能设置匹配寄存器时 */
    if (amhw_lpc_sct_halt_check(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY) == AM_TRUE ) {

        amhw_lpc_sct_mat_val_set(p_hw_sct,
                                 AMHW_LPC_SCT_MODE_UNIFY,
                                 AMHW_LPC_SCT_MAT(0),
                                 period_c - 1);
    }

    /* 设置重载匹配值 */
    amhw_lpc_sct_mat_reload_val_set(p_hw_sct,
                                    AMHW_LPC_SCT_MODE_UNIFY,
                                    AMHW_LPC_SCT_MAT(0),
                                    period_c - 1);
}

void demo_lpc_hw_sct_1_32bit_pwm_entry (amhw_lpc_sct_t *p_hw_sct, uint32_t frq)
{
    /* 先关闭 PWM 输出 */
    __sct_pwm_disable(p_hw_sct);

    /* 初始化 SCT 以输出 PWM */
    __sct_pwm_init(p_hw_sct);

    /* PWM0 初始化 */
    __sct_pwm_out_init(p_hw_sct, __PWM0_MAT_NUM, __PWM0_OUT_NUM);

    /* PWM1 初始化 */
    __sct_pwm_out_init(p_hw_sct, __PWM1_MAT_NUM, __PWM1_OUT_NUM);

    /* 设置周期为 250000ns(4KHz) */
    __sct_pwm_period_config(p_hw_sct, 250000, frq);

    /* 设置 PWM0 占空比 */
    __sct_pwm_duty_config(p_hw_sct, __PWM0_MAT_NUM, 125000, frq);

    /* 设置 PWM1 占空比 */
    __sct_pwm_duty_config(p_hw_sct, __PWM1_MAT_NUM, 62500, frq);

    /* 使能 SCT0，输出 PWM */
    __sct_pwm_enable(p_hw_sct);

    AM_FOREVER {
        am_led_toggle(LED0);
        am_mdelay(200);
    }
}
/** [src_lpc_hw_sct_1_32bit_pwm] */

/* end of file */
