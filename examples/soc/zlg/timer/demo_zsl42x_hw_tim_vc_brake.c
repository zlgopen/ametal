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
 * \brief 通用定时器VC刹车例程，通过 HW层接口实现
 *
 * - 操作步骤：
 *   1.VC0_P("+"输入端PA1)连接一个模拟电压，建议不超过3.3V。
 *   2.VC0_N("-"输入端PA0)连接一个模拟电压，建议不超过3.3V。
 *   3.根据VC0_P和VC0_N之间的大小关系，将产生下述实验现象。
 *
 * - 实验现象：
 *   1.VC0_P("+"输入端PA1)的电压大于VC0_N("-"输入端PA0)的电压，VC0_OUT(PA6)输出高电平，
 *     触发VC0中断(高电平一直触发中断)，定时器4刹车配置生效，TIM1_CHB(PIOB_6)的PWM输出停止。
 *   2.VC0_P("+"输入端PA1)的电压小于VC0_N("-"输入端PA0)的电压，VC0_OUT(PA6)输出低电平，
 *     VC0无中断产生，TIM1_CHB(PIOB_6)的PWM正常输出。
 *
 * \note 本实例仅适用于通用定时器（TIM0、1、2）。
 *       VC默认使用VC0端口作为演示实例。
 *
 * \par 源代码
 * \snippet demo_zsl42x_hw_tim_vc_brake.c src_zsl42x_hw_tim_vc_brake
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-12  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_tim_vc_brake
 * \copydoc demo_zsl42x_hw_tim_vc_brake.c
 */

/** [src_zsl42x_hw_tim_vc_brake] */
#include "ametal.h"
#include "am_clk.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "am_zsl42x.h"
#include "hw/amhw_zsl42x_tim.h"
#include "hw/amhw_zsl42x_vc.h"

/*
 * \brief VC中断服务函数
 */
static void __vc0_irq_handler (void *p_arg)
{

    amhw_zsl42x_vc_t *p_hw_vc = (amhw_zsl42x_vc_t *)p_arg;

    //当VC高电平中断有效时，触发Timer3PWM刹车动作，MOE会自动清零，（AOE不要使能）
    //当MOE自动清零，PWM输出就会关断，若需要恢复PWM输出，则需要再次使能MOE
    /* VC0 通道有效 */
    if(amhw_zsl42x_vc_int_status_check(p_hw_vc, AMHW_ZSL42x_VC0_FLT_INT_FLAG)) {
        amhw_zsl42x_vc_int_status_clr(p_hw_vc,AMHW_ZSL42x_VC0_FLT_INT_FLAG_CLR);

        AM_DBG_INFO("vc int!\r\n");
    }
}

/*
 * \brief VC初始化
 */
static am_bool_t __vc_init (amhw_zsl42x_vc_t       *p_hw_vc,
                            amhw_zsl42x_vc0_n_sel_t vc0_n,
                            amhw_zsl42x_vc0_p_sel_t vc0_p)
{
    /* 使能BGR */
    amhw_zsl42x_bgr_enable(AM_TRUE);

    /* 配置参考电压为VCC */
    amhw_zsl42x_vc_ref2p5_sel(p_hw_vc, AMHW_ZSL42x_VC_REF2P5_VCC);

    /* 设置参考电压不分压 */
    amhw_zsl42x_vc_div_enable(p_hw_vc);
    amhw_zsl42x_vc_div_sel(p_hw_vc, AMHW_ZSL42x_VC_DIV_1);

    /* 选择 迟滞电压、功耗*/
    amhw_zsl42x_vc0_hys_sel(p_hw_vc, AMHW_ZSL42x_VC0_HYS_10_MV);
    amhw_zsl42x_vc0_bias_sel(p_hw_vc, AMHW_ZSL42x_VC0_BIAS_10_UA);

    /*  通道N端输入、通道P端输入配置 */
    amhw_zsl42x_vc0_n_sel(p_hw_vc, vc0_n);
    amhw_zsl42x_vc0_p_sel(p_hw_vc, vc0_p);

    /* 高电平触发中断 */
    amhw_zsl42x_vc0_out_tri_int_sel(p_hw_vc,
                                    AMHW_ZSL42x_VC_OUT_TRI_INT_HIGH_LEVEL);

    /* VC0结果使能输出到Advanced Timer刹车 */
    amhw_zsl42x_vc_outcfg_enable(p_hw_vc,
                                 AMHW_ZSL42x_VC_OUT_CFG_FIT_DATA_TIM0_1_2_3_EN,
                                 AMHW_ZSL42x_VC0);

    /* 配置输出 */
    amhw_zsl42x_vc_outcfg_enable(p_hw_vc,
                                 AMHW_ZSL42x_VC_OUT_CFG_DISABLE,
                                 AMHW_ZSL42x_VC0);

    /* 滤波使能 */
    amhw_zsl42x_vc_flt_disable (p_hw_vc, AMHW_ZSL42x_VC0);

    /* 输出触发中断使能 */
    amhw_zsl42x_vc_int_enable (p_hw_vc, AMHW_ZSL42x_VC0);

    am_int_connect(INUM_VC0, __vc0_irq_handler, (void *)p_hw_vc);
    am_int_enable (INUM_VC0);

    return AM_TRUE;
}

static am_bool_t __zsl42x_hw_tim_init (amhw_zsl42x_tim_t *p_hw_tim,
                                       uint8_t            chan,
                                       uint32_t           duty_ns,
                                       uint32_t           period_ns)
{
    uint32_t clkfreq;
    uint16_t fre_div = 1;
    uint32_t period_c, duty_c;

    /* 设置定时器模式2(锯齿波模式) */
    amhw_zsl42x_tim_mode_set(p_hw_tim, AMHW_ZSL42x_TIM_MODE_ZIGZAG);

    /* 向上计数 */
    amhw_zsl42x_tim_mode23_counter_up(p_hw_tim);

    /* 设置通道为比较模式 */
    if((chan % 2) == 0) {
        amhw_zsl42x_tim_mode23_csa_compare(p_hw_tim, chan);
    } else {
        amhw_zsl42x_tim_mode23_csb_compare(p_hw_tim, chan);
    }

    /* 定时器时钟为内部时钟 */
    amhw_zsl42x_tim_mode_clk_src_set(p_hw_tim,AMHW_ZSL42x_TIM_CLK_SRC_TCLK);

    /* 获取外设时钟频率 */
    clkfreq = am_clk_rate_get(CLK_TIM012);

    /* 计算出来得到的是计数值CNT, 公式ns * 10e-9= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_ns) * (clkfreq) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clkfreq) / (uint64_t)1000000000;

    /* 设置分频值 */
    amhw_zsl42x_tim_mode_clkdiv_set(p_hw_tim, AMHW_ZSL42x_TIM_CLK_DIV64);

    fre_div = 64;

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / fre_div;
    duty_c   = duty_c   / fre_div;

    /* 参数不合法 */
    if ((period_c == 0) || (period_c > 65535UL)) {
        return AM_FALSE;
    }

    /* 设置自动重装寄存器的值 */
    amhw_zsl42x_tim_arr_count_set(p_hw_tim, period_c - 1);

    /* 清零计数器 */
    amhw_zsl42x_tim_cnt16_count_set(p_hw_tim, 0);

    duty_c = period_c - duty_c;

    /* 设置某一通道的比较值 */
    amhw_zsl42x_tim_mode23_ccr_set(p_hw_tim, chan, duty_c - 1);

    /* 设置PWM输出模式为PWM2 */
    amhw_zsl42x_tim_mode23_compare_set(
       p_hw_tim,
       (amhw_zsl42x_tim_compare_type_t)(chan * 4),
       AMHW_ZSL42x_TIM_COMPARE_PWM2);

    /* 正常输出 */
    amhw_zsl42x_tim_mode23_phase_same(
       p_hw_tim,
       (amhw_zsl42x_tim_phase_type_t)(chan * 4 + 3));

    /* PWM功能使能 */
    amhw_zsl42x_tim_mode23_dtr_enable(p_hw_tim, AMHW_ZSL42x_TIM_DTR_MOE);
    amhw_zsl42x_tim_mode23_dtr_enable(p_hw_tim, AMHW_ZSL42x_TIM_DTR_AOE);

    /* TIM1/2使用本身的的刹车控制 */
    amhw_zsl42x_tim_mode23_bksel_mine(p_hw_tim);

    /* 刹车动作产生后输出电平选择 */
    if((chan % 2) == 0) {
        amhw_zsl42x_tim_mode23_bksa_set(p_hw_tim,
                                        chan,
                                        AMHW_ZSL42x_TIM_CMP_BRAKEOUT_LOW);
    } else {
        amhw_zsl42x_tim_mode23_bksb_set(p_hw_tim,
                                        chan,
                                        AMHW_ZSL42x_TIM_CMP_BRAKEOUT_LOW);
    }

    /* 刹车BK输入相位同相（高电平刹车） */
    amhw_zsl42x_tim_mode23_phase_same(p_hw_tim, AMHW_ZSL42x_TIM_PHASE_BKP);

    /* 刹车输入滤波（pclk 3个连续有效） */
    amhw_zsl42x_tim_mode23_fliter_set(p_hw_tim,
                                      AMHW_ZSL42x_TIM_FLITER_TYPE_FLTBK,
                                      AMHW_ZSL42x_TIM_FILTER_PCLKDIV1_3);

    /* VC刹车使能 */
    amhw_zsl42x_tim_mode23_dtr_enable(p_hw_tim, AMHW_ZSL42x_TIM_DTR_VCE);

    return AM_TRUE;
}

/**
 * \brief 通用定时器VC刹车例程，通过 HW 层接口实现
 */
void demo_zsl42x_hw_tim_vc_brake_entry (void         *p_hw_tim,
                                      uint8_t       chan,
                                      unsigned long duty_ns,
                                      unsigned long period_ns,
                                      void         *p_hw_vc,
                                      uint8_t       vc0_n,
                                      uint8_t       vc0_p)
{
    amhw_zsl42x_tim_t *p_tim = (amhw_zsl42x_tim_t *)p_hw_tim;
    amhw_zsl42x_vc_t  *p_vc  = (amhw_zsl42x_vc_t *)p_hw_vc;
    /* 定时器初始化配置 */
    if(__zsl42x_hw_tim_init(p_tim, chan, duty_ns, period_ns) ==
       AM_FALSE) {
        return;
    }

    /* VC初始化配置 */
    if(__vc_init(p_vc, 
			           (amhw_zsl42x_vc0_n_sel_t)vc0_n, 
		             (amhw_zsl42x_vc0_p_sel_t)vc0_p) == AM_FALSE) {
        return;
    }

    /* 使能定时器TIM允许计数 */
    amhw_zsl42x_tim_enable(p_tim);

    /* 使能电压比较器 */
    amhw_zsl42x_vc_volt_cmp_enable(p_vc, AMHW_ZSL42x_VC0);

    while (1) {
        ;
    }
}
/** [src_zsl42x_hw_tim_vc_brake] */

/* end of file */
