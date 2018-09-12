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
 * \brief SCT 32 位捕获例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 使用杜邦线，将外部提供的 PWM 信号与 PIO0_25 连接。
 *
 * - 实验现象：
 *   1. PIO0_25(SCT_IN_2) 配置为捕获功能，捕获 PWM 周期和频率并通过串口输出。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    2. 若有示波器、逻辑分析仪等仪器，可以对比输出波形频率和周期，是否与捕获计算
 *       的一致；
 *    3. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_lpc_hw_sct_cap.c src_lpc_hw_sct_cap
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-19  hgo, modified
 * - 1.01 15-09-23  zxl, modified
 * - 1.00 15-07-14  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_hw_sct_cap
 * \copydoc demo_lpc_hw_sct_cap.c
 */

/** [src_lpc_hw_sct_cap] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc_sct.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/**
 * \brief 本例程使用了 1 个 SCT 事件
 *
 * 1. 事件 2：用于检测捕获条件，使用捕获通道 2，事件发生计数器计数重装到捕获寄存器中，
 *           并产生中断。
 * 所有事件在 SCT 状态 0 下有效，事件发生后 SCT 状态不发生改变；
 */

/** \brief 上升沿捕获 */
#define    __SCT_CAP_TRIGGER_RISE    1

/** \brief 下降沿捕获 */
#define    __SCT_CAP_TRIGGER_FALL    2

am_local volatile am_bool_t __g_flag =  AM_FALSE;   /**< \brief 捕获标志 */

/**
 * \brief 初始化 SCT
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 *
 * \return 无
 */
am_local void __sct_cap_init (amhw_lpc_sct_t *p_hw_sct)
{

    /* 置终止位，SCT 终止运行 */
    amhw_lpc_sct_ctrl_set(p_hw_sct,
                          AMHW_LPC_SCT_CTRL_STOP_L | AMHW_LPC_SCT_CTRL_HALT_L);

    /* 使用 32 位计数器，输入同步，系统时钟模式 */
    amhw_lpc_sct_config(p_hw_sct,
                        AMHW_LPC_SCT_CONFIG_32BIT_COUNTER |
                        AMHW_LPC_SCT_CONFIG_INSYNC_ALL |
                        AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK );

    /* 置 CLRCTR 位以清计数器为 0 */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_CLRCTR_L);

    /* 设置当前状态值为 0 */
    amhw_lpc_sct_state_set(p_hw_sct,
                           AMHW_LPC_SCT_MODE_UNIFY,
                           AMHW_LPC_SCT_STATE(0));

    /* 预分频为 0，使用系统时钟 */
    amhw_lpc_sct_prescale_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);
}

/**
 * \brief 配置 CAP 捕获
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] cap_num  捕获编号
 * \param[in] cap_edge 捕获边沿选择(#__SCT_CAP_TRIGGER_RISE)或
 *                     (#__SCT_CAP_TRIGGER_FALL)
 *
 * \return  无
 */
am_local void __sct_cap_chan_config (amhw_lpc_sct_t *p_hw_sct,
                                     uint32_t        cap_num,
                                     uint8_t         cap_edge)
{
    uint32_t iocond = 0;

    /* 设置捕获边沿 */
    if (cap_edge == __SCT_CAP_TRIGGER_RISE) {
        iocond = AMHW_LPC_SCT_EV_CTRL_IOCOND_RISE;
    } else {
        iocond = AMHW_LPC_SCT_EV_CTRL_IOCOND_FALL;
    }

    /* 将匹配捕捉寄存器配置为捕获功能 */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                                AMHW_LPC_SCT_MODE_UNIFY,
                                AMHW_LPC_SCT_MAT(cap_num),
                                AMHW_LPC_SCT_MATCAP_CAPTURE);

    /* 使能事件原因 CAPn_L (UNIFY = 0) 寄存器重加载 */
    amhw_lpc_sct_cap_ctrl(p_hw_sct,
                          AMHW_LPC_SCT_MODE_UNIFY,
                          AMHW_LPC_SCT_CAP(cap_num),
                          AMHW_LPC_SCT_EVT(cap_num),
                          AM_TRUE);

    /* 使用事件编号同于通道号。事件只能是 IO 条件，不能用匹配寄存器 */
    amhw_lpc_sct_event_ctrl(p_hw_sct,
                            AMHW_LPC_SCT_EVT(cap_num),
                            AMHW_LPC_SCT_EV_CTRL_OUTSEL_INPUT    |
                            AMHW_LPC_SCT_EV_CTRL_IOSEL(cap_num)  |
                            iocond                               |

                            /* 使用指定 IO */
                            AMHW_LPC_SCT_EV_CTRL_COMBMODE_IO     |

                            /*  STATEV 值加上 STATE */
                            AMHW_LPC_SCT_EV_CTRL_STATELD_ADD     |

                            /* 加 0，STATE 没有改变 */
                            AMHW_LPC_SCT_EV_CTRL_STATEV(0));
}

/**
 * \brief 使能 CAP 捕获
 *
 * \param[in] p_hw_sct 指向 SCT 寄存器块的指针
 * \param[in] num      配置 CAP 的捕获通道编号
 *
 * \return 无
 */
am_local int __sct_cap_chan_enable (amhw_lpc_sct_t *p_hw_sct, uint32_t cap_num)
{

    /* 使能状态 0 中事件 0 产生 */
    amhw_lpc_sct_event_state_enable(p_hw_sct,
                                    AMHW_LPC_SCT_EVT(cap_num),
                                    AMHW_LPC_SCT_STATE(0));

    /* 清除事件标志 */
    amhw_lpc_sct_event_flag_clr(p_hw_sct, AMHW_LPC_SCT_EVT(cap_num));

    /* 使能事件产生中断 */
    amhw_lpc_sct_event_int_enable(p_hw_sct, AMHW_LPC_SCT_EVT(cap_num));

    /* 清零控制寄存器的相应位 */
    amhw_lpc_sct_ctrl_clr(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L |
                                    AMHW_LPC_SCT_CTRL_HALT_L);

    return AM_OK;
}

/**
 * \brief SCT 捕获中断服务函数
 *
 * \param[in] p_arg 用户自定义参数，由 am_int_connect() 函数传递
 *
 * \return 无
 */
am_local void __sct_cap_isr (void *p_arg)
{
    amhw_lpc_sct_t *p_hw_sct = (amhw_lpc_sct_t *)p_arg;

    /* 捕获通道 2 使用事件 2 */
    if (amhw_lpc_sct_event_flag_chk(p_hw_sct,
                                    AMHW_LPC_SCT_EVT(2)) == AM_TRUE ) {
        __g_flag = AM_TRUE;

        /* 清事件标志 */
        amhw_lpc_sct_event_flag_clr(p_hw_sct, AMHW_LPC_SCT_EVT(2));
    }
}

void demo_lpc_hw_sct_cap_entry (amhw_lpc_sct_t *p_hw_sct, 
                                int             inum, 
                                uint32_t        frq)
{
    am_bool_t first   = AM_TRUE; /* 首次捕获 */
    uint32_t  count1  = 0;       /* 首次捕获计数 */
    uint32_t  cap_val = 0;       /* 捕获瞬间计数值 */
    uint32_t  time_ns = 0;       /* 捕获计数 */
    uint32_t  freq    = 0;       /* 捕获频率 */

    /* 连接 SCT 中断服务函数 */
    am_int_connect(inum, __sct_cap_isr, (void *)p_hw_sct);

    /* 使能 SCT 中断 */
    am_int_enable(inum);

    __sct_cap_init(p_hw_sct);
    __sct_cap_chan_config(p_hw_sct, 2, __SCT_CAP_TRIGGER_RISE);
    __sct_cap_chan_enable(p_hw_sct, 2);

    AM_FOREVER {
        if (__g_flag == AM_TRUE) {

            /* 获取捕获计数 */
            cap_val = amhw_lpc_sct_cap_val_get(p_hw_sct,
                                               AMHW_LPC_SCT_MODE_UNIFY,
                                               2);
            /* 是否为第一次计数 */
            if (first == AM_TRUE) {
                count1 = cap_val;
                first  =  AM_FALSE;
            } else {

                /* 转换计数值为时间 */
                time_ns = 1000000000 / frq * (cap_val - count1);

                first = AM_TRUE;

                /* 捕获计数完成 */
                freq = 1000000000 / time_ns;
                AM_DBG_INFO("The period is %d ns, The freq is %d Hz \r\n",
                             time_ns,
                             freq);
                am_mdelay(200);
            }
            __g_flag =  AM_FALSE;
        }
    }
}
/** [src_lpc_hw_sct_cap] */

/* end of file */
