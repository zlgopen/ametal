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
 * \snippet demo_hc32f460_hw_adc_int.c src_hc32f460_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 20-04-17  cds, first implementation
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
#define ADC_CHAN    0


/**
 * \brief ADC 中断服务函数
 */
static void __adc_isr (void *p_arg)
{
    static uint8_t i = 0;

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

    __g_adc_dat[i] = amhw_hc32f460_adc_ch_data_get (p_hw_adc,
                                                    ADC_CHAN,
                                                    AMHW_HC32F460_ADC_RESOLUTION_12BIT,
                                                    AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    i++;
    if (i == 10) {

        i = 0;

        __g_adc_complete = AM_TRUE;

        /* 单次转换停止 */
        amhw_hc32f460_adc_convert_stop(p_hw_adc);
    } else {

        /* 单次转换启动 */
    	amhw_hc32f460_adc_convert_start(p_hw_adc);
    }
}

/** \brief 例程入口  */
void demo_hc32f460_hw_adc_int_entry (void    *p_hw_adc,
                                     int      int_num,
                                     uint8_t  chan,
                                     uint32_t vref_mv)
{
    uint8_t                    i        = 0;
    uint32_t                   adc_mv   = 0;    /* 采样电压 */
    uint32_t                   sum      = 0;
    amhw_hc32f460_adc_t         *p_adc    = (amhw_hc32f460_adc_t *)p_hw_adc;

    am_kprintf("The ADC HW Int Demo\r\n");


    /* 映射中断源到具体中断向量 */
    amhw_hc32f460_int_sel0_31(int_num, EVT_ADC1_EOCA);
    /* 连接转换完成中断 */
    am_int_connect(int_num, __adc_isr, p_adc);
    am_int_enable(int_num);

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
