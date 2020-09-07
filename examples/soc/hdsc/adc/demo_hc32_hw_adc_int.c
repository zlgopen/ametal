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
 * \snippet demo_hc32_hw_adc_int.c src_hc32_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-18  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32_hw_adc_int
 * \copydoc demo_hc32_hw_adc_int.c
 */

/** [src_hc32_hw_adc_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_int.h"
#include "hw/amhw_hc32_adc.h"
#include "am_hc32.h"

static volatile uint16_t  __g_adc_dat[10];              /**< \brief 采样值缓存 */
static volatile am_bool_t __g_adc_complete = AM_FALSE;



/**
 * \brief ADC 中断服务函数
 */
static void __adc_isr (void *p_arg)
{
    static uint8_t i = 0;

    amhw_hc32_adc_t *p_hw_adc = (amhw_hc32_adc_t *)p_arg;

    amhw_hc32_adc_flag_clr(p_hw_adc,
                             AMHW_HC32_ADC_FLAG_CONVERT_SINGLE);

    __g_adc_dat[i] = amhw_hc32_adc_data_get (p_hw_adc) & 0x0fff;

    i++;
    if (i == 10) {

        i = 0;

        __g_adc_complete = AM_TRUE;

        /* 单次转换停止 */
        amhw_hc32_adc_single_convert_stop(p_hw_adc);
    } else {

        /* 单次转换启动 */
        amhw_hc32_adc_single_convert_start(p_hw_adc);
    }
}

/**
 * \brief ADC 硬件层（使用了中断）例程入口
 */
void demo_hc32_hw_adc_int_entry (void    *p_hw_adc,
                                 int      int_num,
                                 uint8_t  chan,
                                 uint32_t vref_mv)
{
    uint8_t                    i        = 0;
    uint32_t                   adc_mv   = 0;    /* 采样电压 */
    uint32_t                   sum      = 0;
    amhw_hc32_adc_chan_sel_t chan_sel = (amhw_hc32_adc_chan_sel_t)chan;
    amhw_hc32_adc_t         *p_adc    = (amhw_hc32_adc_t *)p_hw_adc;

    am_kprintf("The ADC HW Int Demo\r\n");

    /* 连接转换完成中断 */
    am_int_connect(int_num, __adc_isr, p_adc);
    am_int_enable(int_num);

    /* 内部参考电压使能 */
    amhw_hc32_adc_inrefvol_enable(p_adc);

    /* 设置参考电压源 */
    amhw_hc32_adc_refvol_sel(p_adc, AMHW_HC32_ADC_REFVOL_AVCC);

    /* BGR模块使能 */
    amhw_hc32_bgr_enable(AM_TRUE);

    /* ADC使能*/
    amhw_hc32_adc_enable(p_adc);

    /* 延时 20us，等待 ADC及 BGR模块启动完成 */
    am_udelay(20);

    /* ADC禁能 */
    amhw_hc32_adc_disable(p_adc);

    /* 使能单次转换模式 */
    amhw_hc32_adc_mode_sel(p_adc, AMHW_HC32_ADC_MODE_SINGLE);

    amhw_hc32_adc_align_way_sel(p_adc, AMHW_HC32_ADC_DATA_ALIGN_RIGHT);

    /* 设置转换周期 */
    amhw_hc32_adc_sample_period_sel(p_adc,
                                      AMHW_HC32_ADC_SAMPLE_PERIOD_12);

    /* 设置ADC时钟分频 */
    amhw_hc32_adc_pclk_div_sel(p_adc, AMHW_HC32_ADC_PCLK_DIV8);

    /* 链接ADC采样通道和引脚 */
    amhw_hc32_adc_sgl_chan_sel(p_adc, chan_sel);

    /* 对齐方式设置 */
    amhw_hc32_adc_align_way_sel(p_adc,
                                  AMHW_HC32_ADC_DATA_ALIGN_RIGHT);

    /* 中断使能*/
    amhw_hc32_adc_int_enable(p_adc);

    /* 清标志位 */
    amhw_hc32_adc_flag_clr(p_adc, AMHW_HC32_ADC_FLAG_ALL);

    /* ADC使能*/
    amhw_hc32_adc_enable(p_adc);


    while(1) {

        /* 单次转换启动 */
        amhw_hc32_adc_single_convert_start(p_adc);

        /* 等待转换完成 */
        while (__g_adc_complete == AM_FALSE);

        for (sum = 0,i = 0; i < 10 ; i++) {
            sum += __g_adc_dat[i];
        }

        sum = sum / 10;

        /* 转换为电压值对应的整数值 */
        adc_mv = sum * 3300 / ((1UL << 12) -1);

        AM_DBG_INFO("Sample : %d, Vol: %d mv\r\n", sum, adc_mv);

        am_mdelay(500);
    }
}
/** [src_hc32_hw_adc_int] */

/* end of file */
