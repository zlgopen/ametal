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
 * \brief ADC INT 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将指定ADC通道对应的引脚连接模拟输入。
 *
 * - 实验现象：
 *   1. 调试串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_zsl42x_hw_multichan_adc.c src_zsl42x_hw_multichan_adc
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-25  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_multichan_adc
 * \copydoc demo_zsl42x_hw_multichan_adc.c
 */

/** [src_zsl42x_hw_multichan_adc] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_int.h"
#include "am_zsl42x_clk.h"
#include "hw/amhw_zsl42x_adc.h"
#include "zsl42x_periph_map.h"

static volatile uint16_t  __g_adc_dat[20];             /**< \brief 采样值缓存 */
static volatile am_bool_t __g_adc_complete = AM_FALSE;

static volatile uint16_t  __adc_sqr_num = 0;
static volatile am_bool_t __adc_jqr_num = 0;
/**
 * \brief ADC 中断服务函数
 */
static void __adc_isr (void *p_arg)
{
    static uint8_t i = 0;

    amhw_zsl42x_adc_t *p_hw_adc = (amhw_zsl42x_adc_t *)p_arg;

    if(amhw_zsl42x_adc_flag_check(p_hw_adc,
                                  AMHW_ZSL42x_ADC_FLAG_CONVERT_SQR) ==
                                  AM_TRUE) {

        amhw_zsl42x_adc_flag_clr(p_hw_adc, AMHW_ZSL42x_ADC_FLAG_CONVERT_SQR);

        for(i = 0; i < __adc_sqr_num; i++) {

            __g_adc_dat[i] = amhw_zsl42x_adc_sqr_chan_data_get(p_hw_adc, i);
        }
    }

    if(amhw_zsl42x_adc_flag_check(p_hw_adc,
                                  AMHW_ZSL42x_ADC_FLAG_CONVERT_JQR) ==
                                  AM_TRUE) {

        amhw_zsl42x_adc_flag_clr(p_hw_adc, AMHW_ZSL42x_ADC_FLAG_CONVERT_JQR);

        for(i = __adc_sqr_num; i < __adc_sqr_num + __adc_jqr_num; i++) {

           __g_adc_dat[i] = amhw_zsl42x_adc_jqr_chan_data_get(p_hw_adc,
                                                              i - __adc_sqr_num);
        }
    }

    if(i == __adc_sqr_num + __adc_jqr_num) {

        i = 0;

        __g_adc_complete = AM_TRUE;

        amhw_zsl42x_adc_sqr_convert_stop(p_hw_adc);
        amhw_zsl42x_adc_jqr_convert_stop(p_hw_adc);
    }
}

/**
 * \brief ADC 硬件层（使用了中断）例程入口
 */
void demo_zsl42x_hw_multichan_adc_entry (void    *p_hw_adc,
                                       int      int_num,
                                       uint32_t vref_mv,
                                       int     *p_adc_chan,
                                       uint8_t  sqr_num,
                                       uint8_t  jqr_num)
{
    uint8_t            i      = 0;
    uint32_t           adc_mv = 0;    /* 采样电压 */
    amhw_zsl42x_adc_t *p_adc  = (amhw_zsl42x_adc_t *)p_hw_adc;

    __adc_sqr_num = sqr_num;
    __adc_jqr_num = jqr_num;

    am_kprintf("The ADC HW Multichan Demo\r\n");

    /* 连接转换完成中断 */
    am_int_connect(int_num, __adc_isr, p_adc);
    am_int_enable(int_num);

    /* 内部参考电压使能 */
    amhw_zsl42x_adc_inrefvol_enable(p_adc);

    /* 设置参考电压源 */
    amhw_zsl42x_adc_refvol_sel(p_adc, AMHW_ZSL42x_ADC_REFVOL_INSIDE_2500MV);

    /* BGR模块使能 */
    amhw_zsl42x_bgr_enable(AM_TRUE);

    /* ADC使能*/
    amhw_zsl42x_adc_enable(p_adc);

    /* 延时 20us，等待 ADC及 BGR模块启动完成 */
    am_udelay(20);

    /* ADC禁能 */
    amhw_zsl42x_adc_disable(p_adc);

    /* 使能扫描转换模式 */
    amhw_zsl42x_adc_mode_sel(p_adc, AMHW_ZSL42x_ADC_MODE_SCAN);

    amhw_zsl42x_adc_align_way_sel(p_adc, AMHW_ZSL42x_ADC_DATA_ALIGN_RIGHT);

    /* 设置转换周期 */
    amhw_zsl42x_adc_sample_period_sel(p_adc,
                                      AMHW_ZSL42x_ADC_SAMPLE_PERIOD_12);

    /* 设置ADC时钟分频 */
    amhw_zsl42x_adc_pclk_div_sel(p_adc, AMHW_ZSL42x_ADC_PCLK_DIV8);

    /* 链接ADC顺序扫描转换通道和引脚 */
    for(i = 0; i < __adc_sqr_num; i++) {
        amhw_zsl42x_adc_sqr_chan_sel(p_adc,
                                     i,
                                     (amhw_zsl42x_adc_chan_sel_t)p_adc_chan[i]);
    }

    amhw_zsl42x_adc_sqr_num_set(p_adc, __adc_sqr_num - 1);

    /* 链接ADC插入扫描转换通道和引脚 */
    for(i = __adc_sqr_num; i < __adc_sqr_num + __adc_jqr_num; i++) {

        amhw_zsl42x_adc_jqr_chan_sel(p_adc,
                                     i - __adc_sqr_num,
                                     (amhw_zsl42x_adc_chan_sel_t)p_adc_chan[i]);
    }

    amhw_zsl42x_adc_jqr_num_set(p_adc, __adc_jqr_num - 1);

    /* 中断使能*/
    amhw_zsl42x_adc_int_enable(p_adc);

    /* 清标志位 */
    amhw_zsl42x_adc_flag_clr(p_adc, AMHW_ZSL42x_ADC_FLAG_ALL);

    /* ADC使能*/
    amhw_zsl42x_adc_enable(p_adc);

    while(1) {

        /* 开始转换 */
        amhw_zsl42x_adc_sqr_convert_start(p_adc);
        amhw_zsl42x_adc_jqr_convert_start(p_adc);

        /* 等待转换完成 */
        while (__g_adc_complete == AM_FALSE);

        for(i = 0; i < __adc_sqr_num; i++) {

            /* 转换为电压值对应的整数值 */
            adc_mv = __g_adc_dat[i] * vref_mv / ((1UL << 12) -1);

            am_kprintf("sqr chan : %d, Sample : %d, Vol: %d mv\r\n", i, __g_adc_dat[i], adc_mv);
        }

        am_kprintf("\r\n");

        for(i = __adc_sqr_num; i < __adc_sqr_num + __adc_jqr_num; i++) {

            /* 转换为电压值对应的整数值 */
            adc_mv = __g_adc_dat[i] * vref_mv / ((1UL << 12) -1);

            am_kprintf("jqr chan : %d, Sample : %d, Vol: %d mv\r\n", i, __g_adc_dat[i], adc_mv);
        }

        am_kprintf("\r\n");

        am_mdelay(500);
    }

}
/** [src_zsl42x_hw_multichan_adc] */

/* end of file */
