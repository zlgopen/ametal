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
 * \brief SCT 作为 2 个 16 位定时器输出 PWM 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. PIO0_23(SCT_OUT0) 输出 4KHz 的 PWM，占空比为 50%；
 *   2. PIO0_27(SCT_OUT2) 输出 1KHz 的 PWM，占空比为 25%。
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
 * \snippet demo_lpc_hw_sct_2_16bit_pwm.c src_lpc_hw_sct_2_16bit_pwm
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-19  hgo, modified
 * - 1.00 15-07-13  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_sct_2_16bit_pwm
 * \copydoc demo_lpc_hw_sct_2_16bit_pwm.c
 */

/** [src_lpc_hw_sct_2_16bit_pwm] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc_sct.h"
#include "demo_nxp_entries.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define   __SCT_HANDLE_H        1              /**< \brief 用于操作高组 */
#define   __SCT_HANDLE_L        0              /**< \brief 用于操作低组 */

/**
 * \brief 本例程使用了 4 个 SCT 事件
 *
 * 1. 事件 0：用于产生 L 路 PWM 的周期，使用匹配通道 0 的低 16 位，周期性
 *           __PWM_L_0_OUT_NUM 置位，事件发生低组计数器清零；
 * 2. 事件 7：用于产生 H 路 PWM 的周期，使用匹配通道 0 的高 16 位，周期性
 *           __PWM_H_0_OUT_NUM 置位，事件发生高组计数器清零；
 * 2. 事件 1：用于控制 L 路 PWM 的占空比，使用匹配通道 __PWM_L_0_MAT_NUM 的低组，
 *           周期性控制 __PWM_L_0_OUT_NUM 清零；
 * 3. 事件 2：用于控制 H 路 PWM 的占空比，使用匹配通道 __PWM_H_0_MAT_NUM 的高组，
 *           周期性控制 __PWM_H_0_OUT_NUM 清零；
 * 所有事件在 SCT 状态 0 下有效，事件发生后 SCT 状态不发生改变；
 */

/**
 * \name 定义低组 PWM
 * @{
 */

/** \brief 低组 PWM0 使用匹配 1 低位 */
#define __PWM_L_0_MAT_NUM    AMHW_LPC_SCT_MAT(1)

/** \brief 低组 PWM0 使用事件 1 */
#define __PWM_L_0_EVT_NUM    AMHW_LPC_SCT_EVT(1)

/** \brief 低组 PWM0，SCT_OUT1 */
#define __PWM_L_0_OUT_NUM    AMHW_LPC_SCT_OUT(1)
/**
 * @}
 */

/**
 * \name 定义高组 PWM
 * @{
 */

/** \brief 高组 PWM0 使用匹配 1 高位 */
#define __PWM_H_0_MAT_NUM    AMHW_LPC_SCT_MAT(1)

/** \brief 高组 PWM0 使用事件 2 */
#define __PWM_H_0_EVT_NUM    AMHW_LPC_SCT_EVT(2)

/** \brief 高组 PWM0, SCT_OUT2 */
#define __PWM_H_0_OUT_NUM    AMHW_LPC_SCT_OUT(2)
/**
 * @}
 */

/**
 * \brief 计算 16 位定时器指定计数值，最佳的预分频值和匹配寄存器值
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
    *p_match      = 65536;

    /**
     * 计数值最小为 65536，
     * 预分频为 1，匹配值等于计数值
     */
    if (count <= 65536) {
        prescale      = 1;
        *p_match      = count;
        return prescale;
    }

    /* 检查匹配值，直到找到最好的 */
    for (match = 65536; match > 0; match--) {

        result_quot = count / match;
        result_rem  = count % match;

        /**
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

                /* 发现最合适的预分频和匹配值 */
                min_remainder = result_rem;
                prescale      = result_quot;
                *p_match      = match;
            }
        }
    }

    return prescale;
}

/**
 * \brief 初始化 SCT 产生 PWM
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] handle   高组：#__SCT_HANDLE_H，低组：#__SCT_HANDLE_L
 *
 * \return 无
 *
 * \note 高组使用事件 12，匹配 0 的高 16 位 bits 设置周期
 *       高组使用事件 0，匹配 0 的低 16 位 bits 设置周期
 */
am_local void __sct_2_16bit_pwm_init (amhw_lpc_sct_t *p_hw_sct, uint8_t handle)
{
    uint32_t            event_num = 0;
    uint32_t            event_ctl = 0;
    amhw_lpc_sct_mode_t sct_mode  = AMHW_LPC_SCT_MODE_UNIFY;

    if (handle == __SCT_HANDLE_H) {

        sct_mode  = AMHW_LPC_SCT_MODE_SEPARATE_H;
        event_num = AMHW_LPC_SCT_EVT(7);        /* 高组使用事件 7 */

        /* 事件关联匹配高 16 位 */
        event_ctl = AMHW_LPC_SCT_EV_CTRL_HEVENT_H;

    } else {
        sct_mode  = AMHW_LPC_SCT_MODE_SEPARATE_L;
        event_num = AMHW_LPC_SCT_EVT(0);         /* 低组使用事件 0 */

        /* 事件关联匹配低 16 位 */
        event_ctl = AMHW_LPC_SCT_EV_CTRL_HEVENT_L;
    }

    /* 配置匹配/捕获寄存器为匹配寄存器 */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                                sct_mode,
                                AMHW_LPC_SCT_MAT(0),
                                AMHW_LPC_SCT_MATCAP_MATCH);

    amhw_lpc_sct_mat_val_set(p_hw_sct, sct_mode, AMHW_LPC_SCT_MAT(0), 0);


    /* 配置事件 0 */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            event_num,

                            /* 选择低状态和匹配寄存器 */
                            event_ctl                           |

                            /* 事件关联匹配 0 */
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(0)    |

                            /* 只能使用指定匹配 */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH |

                            /*  STATEV 值加上 STATE */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD    |

                            /* 加 0，STATE 没有改变 */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));

    /* 使能状态 0 产生事件 */
    amhw_lpc_sct_event_state_enable(p_hw_sct,
                                    event_num,
                                    AMHW_LPC_SCT_STATE(0));

    /* 设置当前状态值为 0 */
    amhw_lpc_sct_state_set(p_hw_sct, sct_mode, AMHW_LPC_SCT_STATE(0));

    if (handle == __SCT_HANDLE_H) {

    /* 置 CLRCTR 位以清高组计数器为 0 */
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_H);
    } else {
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);
    }
}

/**
 * \brief 初始化 SCT 为 2 组 PWM 输出
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 *
 * \return 无
 */
am_local void __sct_pwm_init (amhw_lpc_sct_t *p_hw_sct)
{
    amhw_lpc_sct_config(p_hw_sct,

                        /* 定时器分为 2 个 16 位定时器 */
                        AMHW_LPC_SCT_CONFIG_16BIT_COUNTER  |

                        /* 低组匹配 0 自动限制 */
                        AMHW_LPC_SCT_CONFIG_AUTOLIMIT_L    |

                        /* 高组匹配 0 自动限制 */
                        AMHW_LPC_SCT_CONFIG_AUTOLIMIT_H    |

                        /* SCT 时钟使用系统时钟 */
                        AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK);

    __sct_2_16bit_pwm_init(p_hw_sct, __SCT_HANDLE_L);
    __sct_2_16bit_pwm_init(p_hw_sct, __SCT_HANDLE_H);
}

/**
 * \brief 使能 SCT 输出 PWM
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] handle   高组：#__SCT_HANDLE_H，低组：#__SCT_HANDLE_L
 *
 * \return  无
 */
am_local void __sct_2_16bit_pwm_enable (amhw_lpc_sct_t *p_hw_sct,
                                        uint8_t         handle)
{
    if (handle == __SCT_HANDLE_H) {

        amhw_lpc_sct_ctrl_halt_to_stop(p_hw_sct,
                                       AMHW_LPC_SCT_MODE_SEPARATE_H);

        /* 清除停止位，SCT 高组计数器开始运行 */
        amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_H);

        if (amhw_lpc_sct_halt_check(p_hw_sct,
                                    AMHW_LPC_SCT_MODE_SEPARATE_L) == AM_TRUE) {

            /* 低组不可终止，建议改为停止，否则高组不能输出 PWM */
            amhw_lpc_sct_ctrl_halt_to_stop(p_hw_sct,
                                           AMHW_LPC_SCT_MODE_SEPARATE_L);
        }

    } else {

        /* 清除停止位，SCT 低组计数器开始运行 */
        amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L |
                                        AMHW_LPC_SCT_CTRL_HALT_L);
    }
}

/**
 * \brief 禁止 SCT 输出 PWM
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] handle   高组：#__SCT_HANDLE_H，低组：#__SCT_HANDLE_L
 *
 * \return 无
 */
am_local void __sct_2_16bit_pwm_disable (amhw_lpc_sct_t *p_hw_sct,
                                         uint8_t         handle)
{
    if (handle == __SCT_HANDLE_H) {

        /* 置终止位，SCT 高组计数器终止运行 */
        amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_H |
                                        AMHW_LPC_SCT_CTRL_HALT_H);

    } else {

        /* 高组仍在运行 */
        if (amhw_lpc_sct_halt_check(p_hw_sct,
                                    AMHW_LPC_SCT_MODE_SEPARATE_H) == AM_FALSE &&
            amhw_lpc_sct_stop_check(p_hw_sct,
                                    AMHW_LPC_SCT_MODE_SEPARATE_H) == AM_FALSE){

            /* 低组不可终止，建议改为停止，否则高组不能输出 PWM */
            amhw_lpc_sct_ctrl_halt_to_stop(p_hw_sct,
                                           AMHW_LPC_SCT_MODE_SEPARATE_L);

        } else {

            /* 置终止位，SCT 低组计数器终止运行 */
            amhw_lpc_sct_ctrl_set(p_hw_sct,
                                  AMHW_LPC_SCT_CTRL_STOP_L |
                                  AMHW_LPC_SCT_CTRL_HALT_L);
        }
    }
}

/**
 * \brief 初始化输出一路 PWM
 *
 * \param[in] p_hw_sct  指向 SCT 寄存器块的指针
 * \param[in] handle    高组：#__SCT_HANDLE_H，低组：#__SCT_HANDLE_L
 * \param[in] match_num 匹配编号
 * \param[in] event_num 事件编号
 * \param[in] out_num   输出通道，为 0~5 之间
 *
 * \note 匹配 0 用于确定周期，match_num 不可使用 AMHW_LPC_SCT_MAT(0)
 *
 */
am_local void __sct_pwm_out_init (amhw_lpc_sct_t *p_hw_sct,
                                  uint8_t         handle,
                                  uint32_t        match_num,
                                  uint32_t        event_num,
                                  uint8_t         out_num)
{
    uint32_t            hevent_mode;
    amhw_lpc_sct_mode_t sct_mode;

    /* 匹配 0 使用自动限制 */
    if (match_num == AMHW_LPC_SCT_MAT(0)) {
        return;
    }

    /* 事件 0、事件 7 用于设置输出 */
    if (event_num == AMHW_LPC_SCT_EVT(0) ||
        event_num == AMHW_LPC_SCT_EVT(7)) {
        return;
    }

    if (handle == __SCT_HANDLE_H) {
        sct_mode    = AMHW_LPC_SCT_MODE_SEPARATE_H;
        hevent_mode = AMHW_LPC_SCT_EV_CTRL_HEVENT_H;
    } else {
        sct_mode    = AMHW_LPC_SCT_MODE_SEPARATE_L;
        hevent_mode = AMHW_LPC_SCT_EV_CTRL_HEVENT_L;
    }

    /* 将匹配捕捉寄存器配置为匹配功能 */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                                sct_mode,
                                match_num,
                                AMHW_LPC_SCT_MATCAP_MATCH);

    /* 配置事件 */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            event_num,
                            hevent_mode |
                            AMHW_LPC_SCT_EV_CTRL_MATCHSEL(match_num) |
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_MATCH |
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD |
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));

    /* 使能状态 0 中产生事件 */
    amhw_lpc_sct_event_state_enable(p_hw_sct,
                                    event_num,
                                    AMHW_LPC_SCT_STATE(0));

    if (sct_mode == AMHW_LPC_SCT_MODE_SEPARATE_H) {

        /* 设置事件 7 为 PWM 输出 */
        amhw_lpc_sct_out_set_enable(p_hw_sct,
                                    out_num,
                                    AMHW_LPC_SCT_EVT(7));
    } else {

        /* 设置事件 0 为 PWM 输出 */
        amhw_lpc_sct_out_set_enable(p_hw_sct,
                                    out_num,
                                    AMHW_LPC_SCT_EVT(0));
    }

    /* 事件清除 PWM 输出 */
    amhw_lpc_sct_out_clr_enable(p_hw_sct, out_num, event_num);

    /**
     * 当有效时间等于周期将会产生冲突，在该种情况下，占空比为 100%，
     * 故此时要置位输出
     */
    amhw_lpc_sct_conflict_res_config(p_hw_sct,
                                     out_num,
                                     AMHW_LPC_SCT_RES_SET_OUTPUT);

    /* 置位和清零不依赖于方向 */
    amhw_lpc_sct_output_dir_ctrl(p_hw_sct,
                                 out_num,
                                 AMHW_LPC_SCT_OUTPUTDIRCTRL_INDEPENDENT);
}

/**
 * \brief 配置 PWM 周期
 *
 * \param[in] p_hw_sct  指向 SCT 寄存器块的指针
 * \param[in] handle    高组：#__SCT_HANDLE_H，低组：#__SCT_HANDLE_L
 * \param[in] period_ns 周期，单位为 ns
 *
 * \return 无
 *
 * \note 如果想让该配置马上生效，应在调用该函数前禁止 SCT 的 PWM，
 *       调用该函数后，再使能
 */
am_local void __sct_pwm_period_config(amhw_lpc_sct_t *p_hw_sct,
                                      uint8_t         handle,
                                      uint32_t        period_ns,
                                      uint32_t        frq)
{

    /* 转换时间为周期 */
    uint32_t            period_c = (uint64_t)(period_ns) * 
                                    frq /
                                   (uint64_t)1000000000;
    uint32_t            prescale = 0;
    uint32_t            match    = 0;
    amhw_lpc_sct_mode_t sct_mode;

    if (handle == __SCT_HANDLE_H) {
        sct_mode = AMHW_LPC_SCT_MODE_SEPARATE_H;
    } else {
        sct_mode = AMHW_LPC_SCT_MODE_SEPARATE_L;
    }

    /* 周期至少为 1 */
    if (period_c == 0) {
        period_c = 1;
    }

    /**
     * 计数器最大计数为：65536 * 256(预分频), 最大时间为：
     * 65536 / (g_system_clkfreq / 256) = 65536 * 256 / g_system_clkfreq.
     *
     * 在 100MHz 时，最大时间为：256 * 65536 / 100 us =  167772 us
     * 在 12MHz 时，最大时间为：256 * 65536 /  12 us  =  1398101 us
     * 在 24MHz 时，最大时间为：256 * 65536 /  24 us  =  699051 us
     */
    if (period_c >= 256 * 65536) {
        period_c = 256 * 65536;     /* 最大周期为 256 * 65536 */
    }

    /* 计算预分频和匹配值 */
    prescale = __prescale_and_match_calculate(period_c, &match);

    prescale -= 1;

    if (prescale != amhw_lpc_sct_prescale_get(p_hw_sct, sct_mode)) {

        if (amhw_lpc_sct_halt_check(p_hw_sct, sct_mode) == AM_TRUE ) {
            amhw_lpc_sct_prescale_set(p_hw_sct, sct_mode, prescale);

        } else {
            __sct_2_16bit_pwm_disable(p_hw_sct, handle);
            amhw_lpc_sct_prescale_set(p_hw_sct, sct_mode, prescale);
            __sct_2_16bit_pwm_enable(p_hw_sct, handle);
        }
    }

    /* 只有处于 HALT 状态下时，才能设置匹配寄存器时 */
    if (amhw_lpc_sct_halt_check(p_hw_sct, sct_mode) == AM_TRUE ) {
        amhw_lpc_sct_mat_val_set(p_hw_sct,
                                 sct_mode,
                                 AMHW_LPC_SCT_MAT(0),
                                 match - 1);
    }

    /* 设置重载匹配值 */
    amhw_lpc_sct_mat_reload_val_set(p_hw_sct,
                                    sct_mode,
                                    AMHW_LPC_SCT_MAT(0),
                                    match - 1);
}

/**
 * \brief 配置 PWM 有效时间（占空比）
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] handle   高组：#__SCT_HANDLE_H，低组：#__SCT_HANDLE_L
 * \param[in] mat_num  匹配编号
 * \param[in] duty_ns  有效时间，单位为 ns
 *
 * \return 无
 *
 * \note 如果想让该配置马上生效，应在调用该函数前禁止 SCT 的 PWM(sct_pwm_disable())，
 *       调用该函数后，再使能(sct_pwm_enable())。如果 SCT 没有终止，该值将会写入
 *       重载匹配值，并在下一周期生效
 */
am_local void __sct_pwm_duty_config (amhw_lpc_sct_t *p_hw_sct,
                                     uint8_t         handle,
                                     uint32_t        mat_num,
                                     uint32_t        duty_ns,
                                     uint32_t        frq)
{
    uint32_t            clkfreq  = 0;
    uint32_t            duty_c   = 0;
    amhw_lpc_sct_mode_t sct_mode = AMHW_LPC_SCT_MODE_UNIFY;

    if (handle == __SCT_HANDLE_H) {
        sct_mode = AMHW_LPC_SCT_MODE_SEPARATE_H;

    } else {
        sct_mode = AMHW_LPC_SCT_MODE_SEPARATE_L;
    }

    /* 将时间转换为周期 */
    clkfreq = frq / (amhw_lpc_sct_prescale_get(p_hw_sct, sct_mode) + 1);

    duty_c = (uint64_t)(duty_ns) * (clkfreq) / (uint64_t)1000000000;

    /* 占空有效时间最小为 1 */
    if (duty_c == 0) {
        duty_c = 1;
    }

    if (duty_c > 65536) {
        duty_c = 65536;
    }

    if (amhw_lpc_sct_halt_check(p_hw_sct,
                                AMHW_LPC_SCT_MODE_UNIFY) == AM_TRUE ) {

        amhw_lpc_sct_mat_val_set(p_hw_sct, sct_mode, mat_num, duty_c - 1);
    }

    /* 设置重载匹配值 */
    amhw_lpc_sct_mat_reload_val_set(p_hw_sct, sct_mode, mat_num, duty_c - 1);
}


void demo_lpc_hw_sct_2_16bit_pwm_entry(amhw_lpc_sct_t *p_hw_sct,
                                       int             frq)
{
/* 初始化 SCT 为 2 组 PWM 输出 */
    __sct_pwm_init(p_hw_sct);

    /* SCT_OUT1，低组 */
    __sct_pwm_out_init(p_hw_sct,
                       __SCT_HANDLE_L,       /* 低组 */
                       __PWM_L_0_MAT_NUM,    /* 匹配号 */
                       __PWM_L_0_EVT_NUM,    /* 事件号 */
                       __PWM_L_0_OUT_NUM);   /* 输出号 */

    /* SCT_OUT2，高组 */
    __sct_pwm_out_init(p_hw_sct,
                       __SCT_HANDLE_H,
                       __PWM_H_0_MAT_NUM,
                       __PWM_H_0_EVT_NUM,
                       __PWM_H_0_OUT_NUM);

    /**
     * 低组周期为 250 000ns(4KHz)，
     * 在 100MHz 时，最大周期为：256 * 65536 / 100 us  =  167772 us
     * 在 24MHz  时，最大周期为：256 * 65536 /  24 us  =  699051 us
     * 该例程中系统时钟为 24MHz
     */
    __sct_pwm_period_config(p_hw_sct, __SCT_HANDLE_L, 250000, frq);

    /* 高组周期为 1000000ns(1KHz) */
    __sct_pwm_period_config(p_hw_sct, __SCT_HANDLE_H, 1000000, frq);

    /* 低组 PWM0 有效时间 */
    __sct_pwm_duty_config(p_hw_sct,
                        __SCT_HANDLE_L,
                        __PWM_L_0_MAT_NUM,
                        250000 / 2,
                        frq);

    /* 高组 PWM0 有效时间 */
    __sct_pwm_duty_config(p_hw_sct,
                          __SCT_HANDLE_H,
                          __PWM_H_0_MAT_NUM,
                          1000000 / 4,
                          frq);

    __sct_2_16bit_pwm_enable(p_hw_sct, __SCT_HANDLE_L); /* 使能低组 PWM */
    __sct_2_16bit_pwm_enable(p_hw_sct, __SCT_HANDLE_H); /* 使能高组 PWM */

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_lpc_hw_sct_2_16bit_pwm] */

/* end of file */
