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
 * \brief ADC 多通道转换 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将指定ADC通道对应的引脚连接模拟输入。
 *
 * - 实验现象：
 *   1. 调试串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_multichan_adc.c src_hc32_hw_multichan_adc
 *
 * \internal
 * \par Modification History
 * - 1.00 20-04-22  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_multichan_adc
 * \copydoc demo_hc32f460_hw_multichan_adc.c
 */

/** [src_hc32f460_hw_multichan_adc] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_int.h"
#include "am_hc32f460_clk.h"
#include "hw/amhw_hc32f460_adc.h"
#include "hc32f460_irq_handle.h"
#include "hc32f460_inum.h"

static volatile uint16_t  __g_adc_dat[20];             /**< \brief 采样值缓存 */
static volatile am_bool_t __g_adc_complete = AM_FALSE;

static volatile uint16_t  __adc_chan_num = 0;
static amhw_adc_chmux_t   __adc_chmux[17];

/**
 * \brief ADC 中断服务函数
 */
static void __adc_isr (void *p_arg)
{
    static uint8_t i = 0;

    amhw_hc32f460_adc_t *p_hw_adc = (amhw_hc32f460_adc_t *)p_arg;

    if (amhw_hc32f460_adc_int_flag_check(p_hw_adc,
                                        AMHW_HC32F460_ADC_INT_EOCAF) ==
                                        AM_TRUE) {

        amhw_hc32f460_adc_int_flag_clr(p_hw_adc, AMHW_HC32F460_ADC_INT_EOCAF);

        for(i = 0; i < __adc_chan_num; i++) {

            __g_adc_dat[i] = amhw_hc32f460_adc_ch_data_get(p_hw_adc, __adc_chmux[i].chan, AMHW_HC32F460_ADC_RESOLUTION_12BIT, AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);
        }
     }


    if(i == __adc_chan_num) {

        i = 0;

        __g_adc_complete = AM_TRUE;

        amhw_hc32f460_adc_convert_stop(p_hw_adc);
    }
}

/** \brief 例程入口  */
void demo_hc32f460_hw_multichan_adc_entry (void    *p_hw_adc,
                                           int      int_num,
                                           uint32_t vref_mv,
                                           amhw_adc_chmux_t     *p_adc_chan,
                                           uint8_t  chan_num)
{
    uint8_t            i      = 0;
    uint8_t            j      = 0;
    uint32_t           adc_mv = 0;    /* 采样电压 */
    amhw_hc32f460_adc_t *p_adc  = (amhw_hc32f460_adc_t *)p_hw_adc;

    __adc_chan_num = chan_num;
    am_kprintf("The ADC HW Multichan Demo\r\n");

    /* 映射中断源到具体中断向量 */
    amhw_hc32f460_int_sel0_31(int_num, EVT_ADC1_EOCA);
    /* 连接转换完成中断 */
    am_int_connect(int_num, __adc_isr, p_adc);
    am_int_enable(int_num);


    /* 停止ADC转换 */
    amhw_hc32f460_adc_convert_stop(p_adc);

    /* 转换模式为序列A单次转换 */
    amhw_hc32f460_adc_mode_set(p_adc, AMHW_HC32F460_ADC_SEQ_A_ONCE);

    /* 设置数据对齐方式为右对齐 */
    amhw_hc32f460_adc_data_align_set(p_adc,
                                     AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    /* 设置转换周期 */
    for(j = 0; j < chan_num; j++ ){
    amhw_hc32f460_adc_sampling_period_set(p_adc, p_adc_chan[j].chan, 10);
    }
    /* 链接ADC顺序扫描转换通道和引脚 */
    for(i = 0; i < chan_num; i++) {
        __adc_chmux[i].chan = p_adc_chan[i].chan;
        __adc_chmux[i].type =  p_adc_chan[i].type;
        amhw_hc32f460_adc_channel_mux_set(p_adc, p_adc_chan[i].chan,p_adc_chan[i].type);
        amhw_hc32f460_adc_seq_a_channel_sel0(p_adc, p_adc_chan[i].chan);
    }

    amhw_hc32f460_adc_int_flag_clr(p_adc, AMHW_HC32F460_ADC_INT_EOCAF);

    /* 禁止转换完成中断 */
    amhw_hc32f460_adc_int_disable(p_hw_adc, AMHW_HC32F460_ADC_INT_EOCAF);

    /* 使能转换完成中断 */
    amhw_hc32f460_adc_int_enable(p_adc, AMHW_HC32F460_ADC_INT_EOCAF);

    while(1) {

        /* 开始转换 */
        amhw_hc32f460_adc_convert_start(p_adc);

        /* 等待转换完成 */
        while (__g_adc_complete == AM_FALSE);

        for(i = 0; i < chan_num; i++) {

            /* 转换为电压值对应的整数值 */
            adc_mv = __g_adc_dat[i] * vref_mv / ((1UL << 12) -1);

            am_kprintf("sqr chan : %d, Sample : %d, Vol: %d mv\r\n", i, __g_adc_dat[i], adc_mv);
        }

        am_mdelay(500);
    }

}
/** [src_hc32f460_hw_multichan_adc] */

/* end of file */
