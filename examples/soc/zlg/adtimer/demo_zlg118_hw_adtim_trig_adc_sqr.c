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
 * \brief 高级定时器触发控制ADC顺序扫描转换例程，通过 HW层接口实现
 *
 * - 实验现象：
 *   1.ADC以10Hz的频率进行顺序扫描转换（定时器定时100ms），每转换一次LED翻转一次（闪烁频率10/2=5Hz）
 *   2.串口数据打印的频率与ADC转换的时间间隔不同，串口打印间隔为500ms。
 *
 * \note
 *   1.adc通道数量（sqr_num）的设定范围为1~16
 *   2.定时器周期时间（period_us）的设定范围为0~262140us（0~262ms之间），如果想设定其他时间值，
 *     则需要更改demo_zlg118_hw_adtim_trig_adc_sqr.c中定时器分频系数的设定。
 *   3.不同情况下，ADC的转换速度有限，需要考虑定时器的周期时间是否大于ADC转换时间（多个通道则需要累加转换时间）
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_adtim_trig_adc_sqr.c src_zlg118_hw_adtim_trig_adc_sqr
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-11  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_adtim_trig_adc_sqr
 * \copydoc demo_zlg118_hw_adtim_trig_adc_sqr.c
 */

/** [src_zlg118_hw_adtim_trig_adc_sqr] */
#include "ametal.h"
#include "am_delay.h"
#include "am_int.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_clk.h"
#include "am_zlg118.h"
#include "hw/amhw_zlg118_adtim.h"
#include "hw/amhw_zlg118_adc.h"

static volatile uint16_t  __g_adc_dat[20];             /**< \brief 采样值缓存 */
static volatile am_bool_t __g_adc_complete = AM_FALSE;

static volatile uint16_t  __vref_mv     = 0;
static volatile uint16_t  __adc_sqr_num = 0;
/**
 * \brief ADC 中断服务函数
 */
static void __adc_isr (void *p_arg)
{
    uint8_t i = 0;

    amhw_zlg118_adc_t *p_hw_adc = (amhw_zlg118_adc_t *)p_arg;

    if(amhw_zlg118_adc_flag_check(p_hw_adc,
                                  AMHW_ZLG118_ADC_FLAG_CONVERT_SQR) ==
                                  AM_TRUE) {

        amhw_zlg118_adc_flag_clr(p_hw_adc, AMHW_ZLG118_ADC_FLAG_CONVERT_SQR);

        for(i = 0; i < __adc_sqr_num; i++) {

            __g_adc_dat[i] = amhw_zlg118_adc_sqr_chan_data_get(p_hw_adc, i);
        }

        __g_adc_complete = AM_TRUE;

        am_led_toggle(LED0);
    }
}

static am_bool_t __zlg118_hw_adtim_init (amhw_zlg118_adtim_t *p_hw_adtim,
                                         uint32_t             period_us)
{
    amhw_zlg118_adtim_basecnt_cfg_t   basecnt;
    amhw_zlg118_adtim_irq_trig_cfg_t irq_trig;

    uint32_t clkfreq;
    uint16_t fre_div = 1;
    uint32_t period_c;

    /* 获取外设时钟频率 */
    clkfreq = am_clk_rate_get(CLK_TIM456);

    /* 计算出来得到的是计数值CNT, 公式us * 10e-6= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_us) * (clkfreq) / (uint64_t)1000000;

    /* 锯齿波、向上计数、64分频 */
    basecnt.cntmode   = AMHW_ZLG118_ADTIM_CNT_MODE_SAWTOOTH;
    basecnt.cntdir    = AMHW_ZLG118_ADTIM_CNT_DIR_UP;
    basecnt.cntclkdiv = AMHW_ZLG118_ADTIM_PCLK_DIV64;

    fre_div = 64;

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / fre_div;

    /* 参数不合法 */
    if ((period_c == 0) || (period_c > 65535UL)) {
        return AM_FALSE;
    }

    /* 设置定时器模式、计数方向、计数时钟分频系数 */
    amhw_zlg118_adtim_init(p_hw_adtim, &basecnt);

    /* 设置 ADC触发条件（上溢匹配使能触发ADC）*/
    irq_trig.specilmatch_B_trigdma = AM_FALSE;
    irq_trig.specilmatch_A_trigdma = AM_FALSE;
    irq_trig.underflow_trigdma     = AM_FALSE;
    irq_trig.overflow_trigdma      = AM_FALSE;
    irq_trig.cntmatch_D_trigdma    = AM_FALSE;
    irq_trig.cntmatch_C_trigdma    = AM_FALSE;
    irq_trig.cntmatch_B_trigdma    = AM_FALSE;
    irq_trig.cntmatch_A_trigdma    = AM_FALSE;
    irq_trig.specilmatch_B_trig    = AM_FALSE;
    irq_trig.specilmatch_A_trig    = AM_FALSE;
    irq_trig.underflow_trig        = AM_FALSE;
    irq_trig.overflow_trig         = AM_TRUE;
    irq_trig.cntmatch_D_trig       = AM_FALSE;
    irq_trig.cntmatch_C_trig       = AM_FALSE;
    irq_trig.cntmatch_B_trig       = AM_FALSE;
    irq_trig.cntmatch_A_trig       = AM_FALSE;
    amhw_zlg118_adtim_irqtrigcfg(p_hw_adtim, &irq_trig);

    /* 设置计数周期 */
    amhw_zlg118_adtim_setperiod(p_hw_adtim, period_c);

    /* 清零计数器 */
    amhw_zlg118_adtim_clearcount(p_hw_adtim);

    return AM_TRUE;
}

static void __zlg118_hw_adc_init (amhw_zlg118_adtim_t *p_hw_adtim,
                                  amhw_zlg118_adc_t   *p_hw_adc,
                                  int                  int_num,
                                  int                 *p_adc_chan,
                                  uint8_t              sqr_num)
{
    uint8_t i = 0;

    __adc_sqr_num = sqr_num;

    /* 连接转换完成中断 */
    am_int_connect(int_num, __adc_isr, p_hw_adc);
    am_int_enable(int_num);

    /* 内部参考电压使能 */
    amhw_zlg118_adc_inrefvol_enable(p_hw_adc);

    /* 设置参考电压源 */
    amhw_zlg118_adc_refvol_sel(p_hw_adc, AMHW_ZLG118_ADC_REFVOL_AVCC);

    __vref_mv = 3300;

    /* BGR模块使能 */
    amhw_zlg118_bgr_enable(AM_TRUE);

    /* ADC使能*/
    amhw_zlg118_adc_enable(p_hw_adc);

    /* 延时 20us，等待 ADC及 BGR模块启动完成 */
    am_udelay(20);

    /* ADC禁能 */
    amhw_zlg118_adc_disable(p_hw_adc);

    /* 使能扫描转换模式 */
    amhw_zlg118_adc_mode_sel(p_hw_adc, AMHW_ZLG118_ADC_MODE_SCAN);

    /* 设置转换周期 */
    amhw_zlg118_adc_sample_period_sel(p_hw_adc,
                                      AMHW_ZLG118_ADC_SAMPLE_PERIOD_4);

    /* 设置ADC时钟分频 */
    amhw_zlg118_adc_pclk_div_sel(p_hw_adc, AMHW_ZLG118_ADC_PCLK_DIV2);

    /* 对齐方式设置 */
    amhw_zlg118_adc_align_way_sel(p_hw_adc,
                                  AMHW_ZLG118_ADC_DATA_ALIGN_RIGHT);

    /* 链接ADC顺序扫描转换通道和引脚 */
    for(i = 0; i < __adc_sqr_num; i++) {
        amhw_zlg118_adc_sqr_chan_sel(p_hw_adc,
                                     i,
                                     (amhw_zlg118_adc_chan_sel_t)p_adc_chan[i]);
    }

    amhw_zlg118_adc_sqr_num_set(p_hw_adc, __adc_sqr_num - 1);

    /* 清标志位 */
    amhw_zlg118_adc_flag_clr(p_hw_adc, AMHW_ZLG118_ADC_FLAG_ALL);

    /* 外部中断触发源设置 */
    if((uint32_t)p_hw_adtim == ZLG118_TIM4_BASE) {
        amhw_zlg118_adc_sgl_sqr_trigger_src_set(
            p_hw_adc,
            AMHW_ZLG118_ADC_INT_TRIGGER_TIM4);
    } else if((uint32_t)p_hw_adtim == ZLG118_TIM5_BASE) {
        amhw_zlg118_adc_sgl_sqr_trigger_src_set(
            p_hw_adc,
            AMHW_ZLG118_ADC_INT_TRIGGER_TIM5);
    } else if((uint32_t)p_hw_adtim == ZLG118_TIM6_BASE) {
        amhw_zlg118_adc_sgl_sqr_trigger_src_set(
            p_hw_adc,
            AMHW_ZLG118_ADC_INT_TRIGGER_TIM6);
    }

    /* 中断使能*/
    amhw_zlg118_adc_int_enable(p_hw_adc);

    /* ADC使能*/
    amhw_zlg118_adc_enable(p_hw_adc);
}

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_adtim_trig_adc_sqr_entry (void    *p_hw_adtim,
                                              uint32_t period_us,
                                              void    *p_hw_adc,
                                              int      adc_int_num,
                                              int     *p_adc_chan,
                                              uint8_t  sqr_num)
{

    amhw_zlg118_adtim_t *p_adtim = (amhw_zlg118_adtim_t *)p_hw_adtim;
    amhw_zlg118_adc_t   *p_adc   = (amhw_zlg118_adc_t *)p_hw_adc;

    uint16_t adc_mv = 0;
    uint8_t  i      = 0;

    /* 定时器配置 */
    if((__zlg118_hw_adtim_init(p_adtim, period_us)) == AM_FALSE) {
        return;
    }

    /* ADC配置 */
    __zlg118_hw_adc_init(p_adtim, p_adc, adc_int_num, p_adc_chan, sqr_num);

    /* 启动定时器 */
    amhw_zlg118_adtim_startcount(p_adtim);

    while (1) {

        /* 等待转换完成 */
          while (__g_adc_complete == AM_FALSE);

          for(i = 0; i < __adc_sqr_num; i++) {

              /* 转换为电压值对应的整数值 */
              adc_mv = __g_adc_dat[i] * __vref_mv / ((1UL << 12) -1);

              am_kprintf("sqr chan : %2d, Sample : %4d, Vol: %4d mv\r\n",
                         i,
                         __g_adc_dat[i],
                         adc_mv);
          }

          am_kprintf("\r\n");

          am_mdelay(500);
    }
}
/** [src_zlg118_hw_adtim_trig_adc_sqr] */

/* end of file */
