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
 * \brief 多ADC 协同工作 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将指定ADC通道对应的引脚(PA0,PA5)连接模拟输入。
 *   2. 将PB7引脚接地，产生下降沿，触发ADC1开始转换
 * - 实验现象：
 *   1. PB7拉低后同时触发ADC1和ADC2进行转换，串口打印两个ADC对应通道的转换结果
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_adc_int.c src_hc32f460_hw_adc_int
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

static volatile uint16_t  __g_adc1_data;              /**< \brief 采样值缓存 */
static volatile uint16_t  __g_adc2_data;              /**< \brief 采样值缓存 */
static volatile am_bool_t __g_adc1_complete = AM_FALSE;
static volatile am_bool_t __g_adc2_complete = AM_FALSE;
static uint8_t __g_adc1_chan;
static uint8_t __g_adc2_chan;

/**
 * \brief ADC1 中断服务函数
 */
static void __adc1_isr (void *p_arg)
{

    amhw_hc32f460_adc_t *p_hw_adc = (amhw_hc32f460_adc_t *)p_arg;

    if (amhw_hc32f460_adc_int_flag_check(p_hw_adc,
                                         AMHW_HC32F460_ADC_INT_EOCAF)){
        amhw_hc32f460_adc_int_flag_clr(p_hw_adc,
                                       AMHW_HC32F460_ADC_INT_EOCAF);
    }

    __g_adc1_data = amhw_hc32f460_adc_ch_data_get (p_hw_adc,
                                                   __g_adc1_chan,
                                                   AMHW_HC32F460_ADC_RESOLUTION_12BIT,
                                                   AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    __g_adc1_complete = AM_TRUE;
}

/**
 * \brief ADC2 中断服务函数
 */
static void __adc2_isr (void *p_arg)
{

    amhw_hc32f460_adc_t *p_hw_adc = (amhw_hc32f460_adc_t *)p_arg;

    if (amhw_hc32f460_adc_int_flag_check(p_hw_adc,
                                         AMHW_HC32F460_ADC_INT_EOCAF)){
        amhw_hc32f460_adc_int_flag_clr(p_hw_adc,
                                       AMHW_HC32F460_ADC_INT_EOCAF);
    }

    __g_adc2_data = amhw_hc32f460_adc_ch_data_get (p_hw_adc,
                                                   __g_adc2_chan,
                                                   AMHW_HC32F460_ADC_RESOLUTION_12BIT,
                                                   AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    __g_adc2_complete = AM_TRUE;
}

/** \brief 例程入口  */
void demo_hc32f460_hw_adc_sync_entry (void    *p_hw_adc1,
                                      void    *p_hw_adc2,
                                      int      int_num_adc1,
                                      int      int_num_adc2,
                                      uint8_t  chan_adc1,
                                      uint8_t  chan_adc2,
                                      uint32_t vref_mv)
{
    uint32_t                   adc1_mv   = 0;    /* 采样电压 */
    uint32_t                   sum_adc1  = 0;
    uint32_t                   adc2_mv   = 0;    /* 采样电压 */
    uint32_t                   sum_adc2  = 0;
    amhw_hc32f460_adc_t       *p_adc1   = (amhw_hc32f460_adc_t *)p_hw_adc1;
    amhw_hc32f460_adc_t       *p_adc2   = (amhw_hc32f460_adc_t *)p_hw_adc2;

    __g_adc1_chan = chan_adc1;
    __g_adc2_chan = chan_adc2;

    am_kprintf("The ADC HW SYNC Demo\r\n");

    /* 映射中断源到具体中断向量 */
    amhw_hc32f460_int_sel0_31(int_num_adc1, EVT_ADC1_EOCA);
    /* 连接转换完成中断 */
    am_int_connect(int_num_adc1, __adc1_isr, p_adc1);
    am_int_enable(int_num_adc1);

    /* 映射中断源到具体中断向量 */
    amhw_hc32f460_int_sel0_31(int_num_adc2, EVT_ADC2_EOCA);
    /* 连接转换完成中断 */
    am_int_connect(int_num_adc2, __adc2_isr, p_adc2);
    am_int_enable(int_num_adc2);

    /* 停止ADC转换 */
    amhw_hc32f460_adc_convert_stop(p_hw_adc1);
    amhw_hc32f460_adc_convert_stop(p_hw_adc2);

    /* 设置数据对齐方式为右对齐 */
    amhw_hc32f460_adc_data_align_set(p_hw_adc1,
                                     AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);
    amhw_hc32f460_adc_data_align_set(p_hw_adc2,
                                     AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    /* 平均转换通道设置 */
    amhw_hc32f460_adc_average_channel_sel0(p_hw_adc1, chan_adc1);
    amhw_hc32f460_adc_average_channel_sel0(p_hw_adc2, chan_adc2);

    /* 平均转换次数设置 */
    amhw_hc32f460_adc_average_count_set(p_hw_adc1, AMHW_HC32F460_ADC_AVERAGE_COUNT_32);
    amhw_hc32f460_adc_average_count_set(p_hw_adc2, AMHW_HC32F460_ADC_AVERAGE_COUNT_32);

    /* 禁止数据寄存器自动清除 */
    amhw_hc32f460_adc_data_autoclr_disable(p_hw_adc1);
    amhw_hc32f460_adc_data_autoclr_disable(p_hw_adc2);

    /* 设置ADC精度为12bit */
    amhw_hc32f460_adc_resolution_set(p_hw_adc1,AMHW_HC32F460_ADC_RESOLUTION_12BIT);
    amhw_hc32f460_adc_resolution_set(p_hw_adc2,AMHW_HC32F460_ADC_RESOLUTION_12BIT);

    /* 转换模式为序列A单次转换 */
    amhw_hc32f460_adc_mode_set(p_hw_adc1, AMHW_HC32F460_ADC_SEQ_A_ONCE);
    amhw_hc32f460_adc_mode_set(p_hw_adc2, AMHW_HC32F460_ADC_SEQ_A_ONCE);

    /* ADC序列A通道选择 */
    amhw_hc32f460_adc_seq_a_channel_sel0(p_hw_adc1, chan_adc1);
    amhw_hc32f460_adc_seq_a_channel_sel0(p_hw_adc2, chan_adc2);

    amhw_hc32f460_adc_sync_mode_set(p_hw_adc1,
                                    AMHW_HC32F460_ADC_SYNC_MODE_SINGLE |
                                    AMHW_HC32F460_ADC_SYNC_MODE_PARALLEL  |
                                    AMHW_HC32F460_ADC_SYNC_MODE_ADC0_1_2);

    amhw_hc32f460_adc_sync_mode_enable(p_hw_adc1);

    amhw_hc32f460_adc_seq_a_trigger_source_sel(p_hw_adc1, AMHW_HC32F460_ADC_SEQ_TRIGGER_ADTRx);

    amhw_hc32f460_adc_seq_a_trigger_enable(p_hw_adc1);

    /* 禁止转换完成中断 */
    amhw_hc32f460_adc_int_disable(p_hw_adc1, AMHW_HC32F460_ADC_INT_EOCAF);
    amhw_hc32f460_adc_int_disable(p_hw_adc1, AMHW_HC32F460_ADC_INT_EOCBF);
    amhw_hc32f460_adc_int_disable(p_hw_adc2, AMHW_HC32F460_ADC_INT_EOCAF);
    amhw_hc32f460_adc_int_disable(p_hw_adc2, AMHW_HC32F460_ADC_INT_EOCBF);

    /* 使能转换完成中断 */
    amhw_hc32f460_adc_int_enable(p_hw_adc1, AMHW_HC32F460_ADC_INT_EOCAF);
    amhw_hc32f460_adc_int_enable(p_hw_adc2, AMHW_HC32F460_ADC_INT_EOCAF);

    amhw_hc32f460_adc_sampling_period_set(p_hw_adc1, chan_adc1, 50);
    amhw_hc32f460_adc_sampling_period_set(p_hw_adc2, chan_adc2, 50);

    while(1) {

        /* 等待转换完成 */
        while ((__g_adc1_complete == AM_FALSE) ||
               (__g_adc2_complete == AM_FALSE));

        __g_adc1_complete = AM_FALSE;
        __g_adc2_complete = AM_FALSE;

        sum_adc1 = __g_adc1_data;

        /* 转换为电压值对应的整数值 */
        adc1_mv = sum_adc1 * 3300 / ((1UL << 12) -1);

        AM_DBG_INFO("ADC1 Sample : %d, Vol: %d mv\r\n", sum_adc1, adc1_mv);

        sum_adc2 = __g_adc2_data;

        /* 转换为电压值对应的整数值 */
        adc2_mv = sum_adc2 * 3300 / ((1UL << 12) -1);

        AM_DBG_INFO("ADC2 Sample : %d, Vol: %d mv\r\n", sum_adc2, adc2_mv);

        am_mdelay(500);
    }
}
/** [src_hc32f460_hw_adc_int] */

/* end of file */
