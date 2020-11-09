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
 * \brief 高级定时器VC刹车例程，通过 HW层接口实现
 *
 * - 操作步骤：
 *   1.VC0_P("+"输入端PA1)连接一个模拟电压，建议不超过3.3V。
 *   2.VC0_N("-"输入端PA0)连接一个模拟电压，建议不超过3.3V。
 *   3.根据VC0_P和VC0_N之间的大小关系，将产生下述实验现象。
 *
 * - 实验现象：
 *   1.VC0_P("+"输入端PA1)的电压大于VC0_N("-"输入端PA0)的电压，VC0_OUT(PA6)输出高电平，
 *     触发VC0中断(高电平一直触发中断)，定时器4刹车配置生效，TIM4_CHA(PA8)的PWM输出停止。
 *   2.VC0_P("+"输入端PA1)的电压小于VC0_N("-"输入端PA0)的电压，VC0_OUT(PA6)输出低电平，
 *     VC0无中断产生，TIM4_CHA(PA8)的PWM正常输出。
 *
 * \note 本实例仅适用于高级定时器（TIM4、5、6）。
 *       VC默认使用VC0端口作为演示实例。
 *
 * \par 源代码
 * \snippet demo_zsl42x_hw_adtim_vc_brake.c src_zsl42x_hw_adtim_vc_brake
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-12  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_adtim_vc_brake
 * \copydoc demo_zsl42x_hw_adtim_vc_brake.c
 */

/** [src_zsl42x_hw_adtim_vc_brake] */
#include "ametal.h"
#include "am_clk.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "am_zsl42x.h"
#include "hw/amhw_zsl42x_adtim.h"
#include "hw/amhw_zsl42x_vc.h"

/*
 * \brief VC中断服务函数
 */
static void __vc0_irq_handler (void *drv)
{

    amhw_zsl42x_vc_t *p_hw_vc = (amhw_zsl42x_vc_t *)drv;

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
                                 AMHW_ZSL42x_VC_OUT_CFG_BREAK_EN,
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

static am_bool_t __zsl42x_hw_adtim_init(void          *p_hw_adtim,
                                        uint8_t        chan,
                                        unsigned long  duty_ns,
                                        unsigned long  period_ns) {
    amhw_zsl42x_adtim_basecnt_cfg_t        basecnt;
    amhw_zsl42x_adtim_timx_chx_port_cfg_t  portcfg;

    uint32_t clkfreq;
    uint16_t fre_div = 1;
    uint32_t period_c, duty_c;

    basecnt.cntmode   = AMHW_ZSL42x_ADTIM_CNT_MODE_SAWTOOTH;
    basecnt.cntdir    = AMHW_ZSL42x_ADTIM_CNT_DIR_UP;
    basecnt.cntclkdiv = AMHW_ZSL42x_ADTIM_PCLK_DIV16;

    /* 设置定时器模式、计数方向、计数时钟分频系数 */
    amhw_zsl42x_adtim_init(p_hw_adtim, &basecnt);

    fre_div = 16;

    /* 获取外设时钟频率 */
    clkfreq = am_clk_rate_get(CLK_TIM456);

    /* 计算出来得到的是计数值CNT, 公式ns * 10e-9= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_ns) * (clkfreq) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clkfreq) / (uint64_t)1000000000;

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / fre_div;
    duty_c   = duty_c   / fre_div;

    /* 参数不合法 */
    if ((period_c == 0) || (period_c > 65535UL)) {
        return AM_FALSE;
    }

    /* 设置计数周期 */
    amhw_zsl42x_adtim_setperiod(p_hw_adtim, period_c);

    /* 清零计数器 */
    amhw_zsl42x_adtim_clearcount(p_hw_adtim);

    duty_c = period_c - duty_c;

    /* 设置比较值 */
    amhw_zsl42x_adtim_setcomparevalue(p_hw_adtim, 
		                                  (amhw_zsl42x_adtim_compare_x_t)chan, 
		                                  duty_c);

    portcfg.portmode  = AMHW_ZSL42x_ADTIM_PORT_MODE_CMP_OUTPUT;
    portcfg.outenable = AM_TRUE;
    portcfg.perc      = AMHW_ZSL42x_ADTIM_PERIOD_INV;
    portcfg.cmpc      = AMHW_ZSL42x_ADTIM_COMPARE_INV;
    portcfg.stastps   = AMHW_ZSL42x_ADTIM_STATE_SEL_SS;
    portcfg.staout    = AMHW_ZSL42x_ADTIM_PORT_OUT_LOW;
    portcfg.stpout    = AMHW_ZSL42x_ADTIM_PORT_OUT_LOW;
    portcfg.dissel    = AMHW_ZSL42x_ADTIM_DISSEL0;    // 选择强制输出无效条件0(VC刹车)
    portcfg.disval    = AMHW_ZSL42x_ADTIM_DISVAL_LOW; // 强制输出无效条件0~3中条件成立时，CHx端口输出低电平
    portcfg.fltenable = AM_FALSE;
    portcfg.fltclk    = AMHW_ZSL42x_ADTIM_FLTCLK_PCLK_DIV1;
    amhw_zsl42x_adtim_timxchxportcfg(p_hw_adtim, 
		                                 (amhw_zsl42x_adtim_chx_t)chan, 
																		 &portcfg);

    return AM_TRUE;
}

/**
 * \brief 高级定时器VC刹车例程，通过 HW 层接口实现
 */
void demo_zsl42x_hw_adtim_vc_brake_entry (void         *p_hw_adtim,
                                        uint8_t       chan,
                                        unsigned long duty_ns,
                                        unsigned long period_ns,
                                        void         *p_hw_vc,
                                        uint8_t       vc0_n,
                                        uint8_t       vc0_p)
{
    amhw_zsl42x_adtim_t *p_adtim = (amhw_zsl42x_adtim_t *)p_hw_adtim;
    amhw_zsl42x_vc_t    *p_vc    = (amhw_zsl42x_vc_t *)p_hw_vc;
    /* 定时器初始化配置 */
    if(__zsl42x_hw_adtim_init(p_adtim, chan, duty_ns, period_ns) ==
       AM_FALSE) {
        return;
    }

    /* VC初始化配置 */
    if(__vc_init(p_vc, 
			           (amhw_zsl42x_vc0_n_sel_t)vc0_n, 
		             (amhw_zsl42x_vc0_p_sel_t)vc0_p) == 
		   AM_FALSE) {
        return;
    }

    /* 使能定时器 */
    amhw_zsl42x_adtim_startcount(p_adtim);

    /* 使能电压比较器 */
    amhw_zsl42x_vc_volt_cmp_enable(p_vc, AMHW_ZSL42x_VC0);

    while (1) {
        ;
    }
}
/** [src_zsl42x_hw_adtim_vc_brake] */

/* end of file */
