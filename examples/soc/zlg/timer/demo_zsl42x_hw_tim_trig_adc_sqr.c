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
 * \brief 定时器触发控制ADC顺序扫描转换例程，通过 HW层接口实现
 *
 * - 实验现象：
 *   1.定时器通道一组频率为10Hz，占空比25%的PWM。
 *   2.ADC以10Hz的频率进行顺序扫描转换（定时器定时100ms），每转换一次LED翻转一次（闪烁频率10/2=5Hz）
 *   2.串口数据打印的频率与ADC转换的时间间隔不同，串口打印间隔为500ms。
 *
 * \note
 *   1.adc通道数量（sqr_num）的设定范围为1~16
 *   2.定时器周期时间（period_us）的设定范围为0~262140us（0~262ms之间），如果想设定其他时间值，
 *     则需要更改demo_zsl42x_hw_tim_trig_adc_sqr.c中定时器分频系数的设定。
 *   3.不同情况下，ADC的转换速度有限，需要考虑定时器的周期时间是否大于ADC转换时间（多个通道则需要累加转换时间）
 *
 * \par 源代码
 * \snippet demo_zsl42x_hw_tim_trig_adc_sqr.c src_zsl42x_hw_tim_trig_adc_sqr
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-11  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_tim_trig_adc_sqr
 * \copydoc demo_zsl42x_hw_tim_trig_adc_sqr.c
 */

/** [src_zsl42x_hw_tim_trig_adc_sqr] */
#include "ametal.h"
#include "am_delay.h"
#include "am_int.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_clk.h"
#include "am_zsl42x.h"
#include "hw/amhw_zsl42x_tim.h"
#include "hw/amhw_zsl42x_adc.h"

static volatile uint16_t  __g_adc_dat[20];             /**< \brief 采样值缓存 */
static volatile am_bool_t __g_adc_complete = AM_FALSE;

static volatile uint16_t  __vref_mv     = 0;
static volatile uint16_t  __adc_sqr_num = 0;

static volatile uint8_t   chan_num = 0;
/**
 * \brief ADC 中断服务函数
 */
static void __adc_isr (void *p_arg)
{
    uint8_t i = 0;

    amhw_zsl42x_adc_t *p_hw_adc = (amhw_zsl42x_adc_t *)p_arg;

    if(amhw_zsl42x_adc_flag_check(p_hw_adc,
                                  AMHW_ZSL42x_ADC_FLAG_CONVERT_SQR) ==
                                  AM_TRUE) {

        amhw_zsl42x_adc_flag_clr(p_hw_adc, AMHW_ZSL42x_ADC_FLAG_CONVERT_SQR);

        for(i = 0; i < __adc_sqr_num; i++) {

            __g_adc_dat[i] = amhw_zsl42x_adc_sqr_chan_data_get(p_hw_adc, i);
        }

        __g_adc_complete = AM_TRUE;

        am_led_toggle(LED0);
    }
}

static am_bool_t __zsl42x_hw_tim_init (amhw_zsl42x_tim_t *p_hw_tim,
                                       uint8_t            type,
                                       uint8_t            chan,
                                       uint32_t           duty_us,
                                       uint32_t           period_us)
{
    uint32_t clkfreq;
    uint16_t fre_div = 1;
    uint32_t period_c, duty_c;

    if((amhw_zsl42x_tim_type_t)type == AMHW_ZSL42x_TIM_TYPE_TIM3) {
        chan_num = 6;
    } else {
        chan_num = 2;
    }

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

    /* 计算出来得到的是计数值CNT, 公式us * 10e-6= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_us) * (clkfreq) / (uint64_t)1000000;
    duty_c   = (uint64_t)(duty_us)   * (clkfreq) / (uint64_t)1000000;

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

    /* 使能ADC触发全局控制（总开关） */
    amhw_zsl42x_tim_mode23_adc_trigger_enable(
        p_hw_tim,
        AMHW_ZSL42x_TIM_ADC_TRIGGER_ADTE);

    /* 使能事件更新触发ADC（实际事件选择） */
    amhw_zsl42x_tim_mode23_adc_trigger_enable(
        p_hw_tim,
        AMHW_ZSL42x_TIM_ADC_TRIGGER_UEVE);

    return AM_TRUE;
}

static void __zsl42x_hw_adc_init (amhw_zsl42x_tim_t *p_hw_tim,
                                  amhw_zsl42x_adc_t *p_hw_adc,
                                  int                int_num,
                                  int               *p_adc_chan,
                                  uint8_t            sqr_num)
{
    uint8_t i = 0;

    __adc_sqr_num = sqr_num;

    /* 连接转换完成中断 */
    am_int_connect(int_num, __adc_isr, p_hw_adc);
    am_int_enable(int_num);

    /* 内部参考电压使能 */
    amhw_zsl42x_adc_inrefvol_enable(p_hw_adc);

    /* 设置参考电压源 */
    amhw_zsl42x_adc_refvol_sel(p_hw_adc, AMHW_ZSL42x_ADC_REFVOL_AVCC);

    __vref_mv = 3300;

    /* BGR模块使能 */
    amhw_zsl42x_bgr_enable(AM_TRUE);

    /* ADC使能*/
    amhw_zsl42x_adc_enable(p_hw_adc);

    /* 延时 20us，等待 ADC及 BGR模块启动完成 */
    am_udelay(20);

    /* ADC禁能 */
    amhw_zsl42x_adc_disable(p_hw_adc);

    /* 使能扫描转换模式 */
    amhw_zsl42x_adc_mode_sel(p_hw_adc, AMHW_ZSL42x_ADC_MODE_SCAN);

    /* 设置转换周期 */
    amhw_zsl42x_adc_sample_period_sel(p_hw_adc,
                                      AMHW_ZSL42x_ADC_SAMPLE_PERIOD_4);

    /* 设置ADC时钟分频 */
    amhw_zsl42x_adc_pclk_div_sel(p_hw_adc, AMHW_ZSL42x_ADC_PCLK_DIV2);

    /* 对齐方式设置 */
    amhw_zsl42x_adc_align_way_sel(p_hw_adc,
                                  AMHW_ZSL42x_ADC_DATA_ALIGN_RIGHT);

    /* 链接ADC顺序扫描转换通道和引脚 */
    for(i = 0; i < __adc_sqr_num; i++) {
        amhw_zsl42x_adc_sqr_chan_sel(p_hw_adc,
                                     i,
                                     (amhw_zsl42x_adc_chan_sel_t)p_adc_chan[i]);
    }

    amhw_zsl42x_adc_sqr_num_set(p_hw_adc, __adc_sqr_num - 1);

    /* 清标志位 */
    amhw_zsl42x_adc_flag_clr(p_hw_adc, AMHW_ZSL42x_ADC_FLAG_ALL);

    /* 外部中断触发源设置 */
    if((uint32_t)p_hw_tim == ZSL42x_TIM0_BASE) {
        amhw_zsl42x_adc_sgl_sqr_trigger_src_set(
            p_hw_adc,
            AMHW_ZSL42x_ADC_INT_TRIGGER_TIM0);
    } else if((uint32_t)p_hw_tim == ZSL42x_TIM1_BASE) {
        amhw_zsl42x_adc_sgl_sqr_trigger_src_set(
            p_hw_adc,
            AMHW_ZSL42x_ADC_INT_TRIGGER_TIM1);
    } else if((uint32_t)p_hw_tim == ZSL42x_TIM2_BASE) {
        amhw_zsl42x_adc_sgl_sqr_trigger_src_set(
            p_hw_adc,
            AMHW_ZSL42x_ADC_INT_TRIGGER_TIM2);
    } else if((uint32_t)p_hw_tim == ZSL42x_TIM3_BASE) {
        amhw_zsl42x_adc_sgl_sqr_trigger_src_set(
            p_hw_adc,
            AMHW_ZSL42x_ADC_INT_TRIGGER_TIM3);
    }

    /* 中断使能*/
    amhw_zsl42x_adc_int_enable(p_hw_adc);

    /* ADC使能*/
    amhw_zsl42x_adc_enable(p_hw_adc);
}

/**
 * \brief 通用定时器定时触发ADC转换例程，通过 HW 层接口实现
 */
void demo_zsl42x_hw_tim_trig_adc_sqr_entry (void    *p_hw_tim,
                                          uint8_t  type,
                                          uint8_t  chan,
                                          uint32_t duty_us,
                                          uint32_t period_us,
                                          void    *p_hw_adc,
                                          int      adc_int_num,
                                          int     *p_adc_chan,
                                          uint8_t  sqr_num)
{

    amhw_zsl42x_tim_t *p_tim = (amhw_zsl42x_tim_t *)p_hw_tim;
    amhw_zsl42x_adc_t *p_adc = (amhw_zsl42x_adc_t *)p_hw_adc;

    uint16_t adc_mv = 0;
    uint8_t  i      = 0;

    /* 定时器配置 */
    if((__zsl42x_hw_tim_init(p_tim, type, chan, duty_us , period_us)) ==
       AM_FALSE) {
        return;
    }

    /* ADC配置 */
    __zsl42x_hw_adc_init(p_tim, p_adc, adc_int_num, p_adc_chan, sqr_num);

    /* 使能定时器TIM允许计数 */
    amhw_zsl42x_tim_enable(p_tim);

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
/** [src_zsl42x_hw_tim_trig_adc_sqr] */

/* end of file */
