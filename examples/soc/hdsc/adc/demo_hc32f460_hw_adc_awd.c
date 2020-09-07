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
 * \brief ADC AWD 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将指定ADC通道对应的引脚连接电位器输出端 VRO。
 *
 * - 实验现象：
 *   1. 调试串口输出电位器输出电压,当超过AWD设定阈值后产生中断，打印AD转换结果和AWD阈值。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_adc_awd.c src_hc32f460_hw_awd_int
 *
 * \internal
 * \par Modification History
 * - 1.00 20-04-26  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_adc_int
 * \copydoc demo_hc32f460_hw_adc_int.c
 */

/** [src_hc32f460_hw_adc_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_int.h"
#include "hw/amhw_hc32f460_adc.h"
#include "am_hc32f460.h"
#include "hc32f460_irq_handle.h"

static volatile uint16_t  __g_adc_dat[10];              /**< \brief 采样值缓存 */
static volatile am_bool_t __g_adc_complete = AM_FALSE;
static volatile am_bool_t __g_adc_seqcmp   = AM_FALSE;
#define ADC_CHAN    0

#define THRESHOLD_VALUE_LOW  500
#define THRESHOLD_VALUE_HIGH 1000

/**
 * \brief ADC 中断服务函数
 */
static void __adc_isr (void *p_arg)
{

    amhw_hc32f460_adc_t *p_hw_adc = (amhw_hc32f460_adc_t *)p_arg;

    if (amhw_hc32f460_adc_int_flag_check(p_hw_adc,
                                         AMHW_HC32F460_ADC_INT_EOCAF)){
        amhw_hc32f460_adc_int_flag_clr(p_hw_adc,
                                       AMHW_HC32F460_ADC_INT_EOCAF);
    } else if(amhw_hc32f460_adc_int_flag_check(p_hw_adc,
                                               AMHW_HC32F460_ADC_INT_EOCBF)){
        amhw_hc32f460_adc_int_flag_clr(p_hw_adc,
                                       AMHW_HC32F460_ADC_INT_EOCBF);
    }

    __g_adc_dat[0] = amhw_hc32f460_adc_ch_data_get (p_hw_adc,
                                                    ADC_CHAN,
                                                    AMHW_HC32F460_ADC_RESOLUTION_12BIT,
                                                    AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    __g_adc_complete = AM_TRUE;

    /* 单次转换停止 */
    amhw_hc32f460_adc_convert_stop(p_hw_adc);

}

static void __adc_seqcmp_isr(void *p_arg)
{

    amhw_hc32f460_adc_t *p_hw_adc = (amhw_hc32f460_adc_t *)p_arg;

    if (amhw_hc32f460_adc_awd_chan0_15_cmpstat_get(p_hw_adc) & AMHW_HC32F460_ADC_CHAN0_MASK) {
        amhw_hc32f460_adc_awd_chan0_15_cmpstat_clr(p_hw_adc, AMHW_HC32F460_ADC_CHAN0_MASK);
    }

    __g_adc_seqcmp = AM_TRUE;

}

/** \brief 例程入口  */
void demo_hc32f460_hw_adc_awd_entry (void    *p_hw_adc,
                                     int      int_num_eoc,
                                     int      int_num_seqcmp,
                                     uint8_t  chan,
                                     uint32_t vref_mv)
{
    uint32_t                   adc_mv   = 0;    /* 采样电压 */
    uint32_t                   sum      = 0;
    amhw_hc32f460_adc_t       *p_adc    = (amhw_hc32f460_adc_t *)p_hw_adc;
	  uint16_t                   threshold_value_low  = THRESHOLD_VALUE_LOW;
	  uint16_t                   threshold_value_high = THRESHOLD_VALUE_HIGH;	

    am_kprintf("The ADC HW Int Demo\r\n");

    /* 连接转换完成中断 */
    amhw_hc32f460_int_sel0_31(int_num_eoc, EVT_ADC1_EOCA);
    am_int_connect(int_num_eoc, __adc_isr, p_adc);
    am_int_enable(int_num_eoc);

    /* 连接转换完成中断 */
    amhw_hc32f460_int_sel0_31(int_num_seqcmp, EVT_ADC1_SEQCMP);
    am_int_connect(int_num_seqcmp, __adc_seqcmp_isr, p_adc);
    am_int_enable(int_num_seqcmp);

    /* 停止ADC转换 */
    amhw_hc32f460_adc_convert_stop(p_hw_adc);

    /* 设置数据对齐方式为右对齐 */
    amhw_hc32f460_adc_data_align_set(p_hw_adc,
                                     AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    /* 禁止数据寄存器自动清除 */
    amhw_hc32f460_adc_data_autoclr_disable(p_hw_adc);

    /* 设置ADC精度为12bit */
    amhw_hc32f460_adc_resolution_set(p_hw_adc,AMHW_HC32F460_ADC_RESOLUTION_12BIT);

    /* 转换模式为序列A单次转换 */
    amhw_hc32f460_adc_mode_set(p_hw_adc, AMHW_HC32F460_ADC_SEQ_A_ONCE);

    /* ADC序列A通道选择 */
    amhw_hc32f460_adc_seq_a_channel_sel0(p_hw_adc, chan);

    /* 设定低阀值 */
    amhw_hc32f460_adc_awd_low_threshold_set(p_hw_adc,
                                            threshold_value_low,
                                            AMHW_HC32F460_ADC_RESOLUTION_12BIT,
                                            AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    /* 设定高阀值 */
    amhw_hc32f460_adc_awd_high_threshold_set(p_hw_adc,
                                            threshold_value_high,
                                            AMHW_HC32F460_ADC_RESOLUTION_12BIT,
                                            AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    amhw_hc32f460_adc_awd_chan0_15_set (p_hw_adc, AMHW_HC32F460_ADC_CHAN0_MASK);
    amhw_hc32f460_adc_awd_seq_set(p_hw_adc, AMHW_HC32F460_ADC_AWD_SEQ_A_ONLY);
    amhw_hc32f460_adc_awd_cmp_mode_set(p_hw_adc, AMHW_HC32F460_ADC_AWD_CMPMODE0);
    amhw_hc32f460_adc_awd_int_enable(p_hw_adc);
    amhw_hc32f460_adc_awd_enable(p_hw_adc);


    /* 禁止转换完成中断 */
    amhw_hc32f460_adc_int_disable(p_hw_adc, AMHW_HC32F460_ADC_INT_EOCAF);
    amhw_hc32f460_adc_int_disable(p_hw_adc, AMHW_HC32F460_ADC_INT_EOCBF);

    /* 使能转换完成中断 */
    amhw_hc32f460_adc_int_enable(p_hw_adc, AMHW_HC32F460_ADC_INT_EOCAF);

    amhw_hc32f460_adc_sampling_period_set(p_hw_adc, chan, 50);

    while(1) {

        /* 单次转换启动 */
        amhw_hc32f460_adc_convert_start(p_hw_adc);

        /* 等待转换完成 */
        while (__g_adc_complete == AM_FALSE);

        if (__g_adc_seqcmp == AM_TRUE) {
            AM_DBG_INFO("ADC_SEQCMP happen !\r\n");
            __g_adc_seqcmp = AM_FALSE;
        }

        sum = __g_adc_dat[0];

        /* 转换为电压值对应的整数值 */
        adc_mv = sum * 3300 / ((1UL << 12) -1);

        AM_DBG_INFO("threshold value low  : %d\r\n", threshold_value_low);
        AM_DBG_INFO("threshold value high : %d\r\n", threshold_value_high);
        AM_DBG_INFO("Sample : %d, Vol: %d mv\r\n", sum, adc_mv);

        __g_adc_complete = AM_FALSE;

        am_mdelay(500);

    }
}
/** [src_hc32f460_hw_adc_int] */

/* end of file */
