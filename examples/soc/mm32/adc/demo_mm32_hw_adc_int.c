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
 * \snippet demo_mm32_hw_adc_int.c src_mm32_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-18  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32_hw_adc_int
 * \copydoc demo_mm32_hw_adc_int.c
 */

/** [src_mm32_hw_adc_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_int.h"
#include "hw/amhw_mm32_adc.h"

static volatile uint16_t  __g_adc_dat[10];              /**< \brief 采样值缓存 */
static volatile am_bool_t __g_adc_complete = AM_FALSE;

/**
 * \brief ADC 中断服务函数
 */
static void __adc_isr (void *p_arg)
{
    static uint8_t i = 0;

    amhw_mm32_adc_t *p_hw_adc = (amhw_mm32_adc_t *)p_arg;

    amhw_mm32_adc_data_get(p_hw_adc, (uint16_t *)&__g_adc_dat[i]);
    __g_adc_dat[i] &= 0x0fff;

    i++;
    if (i == 10) {
        i = 0;
        __g_adc_complete = AM_TRUE;

        amhw_mm32_adc_ctrl_reg_clr(p_hw_adc, AMHW_MM32_ADC_INT_EN_MASK);

        amhw_mm32_adc_data_transfer_set(p_hw_adc,
                                       AMHW_MM32_ADC_DATA_TRANSFER_END);
    }
}

/** \brief 例程入口  */
void demo_mm32_hw_adc_int_entry (amhw_mm32_adc_t *p_hw_adc,
                                int             int_num,
                                int             chan,
                                uint32_t        vref_mv)
{
    uint8_t  i      = 0;
    uint32_t adc_mv = 0;    /* 采样电压 */
    uint32_t sum    = 0;

    am_kprintf("The ADC HW Int Demo\r\n");

    amhw_mm32_adc_data_resolution(p_hw_adc, AMHW_MM32_ADC_DATA_VALID_12BIT);
    amhw_mm32_adc_prescaler_val(p_hw_adc, 4);

    /* 连接转换完成中断 */
    am_int_connect(int_num, __adc_isr, p_hw_adc);
    am_int_enable(int_num);

    amhw_mm32_adc_input_chan_enable(p_hw_adc, chan);
    amhw_mm32_adc_cgf_reg_set(p_hw_adc, AMHW_MM32_ADC_MODULE_EN_MASK);

    while(1) {
        amhw_mm32_adc_ctrl_reg_set(p_hw_adc, AMHW_MM32_ADC_INT_EN_MASK);

        amhw_mm32_adc_data_transfer_set(p_hw_adc, AMHW_MM32_ADC_DATA_TRANSFER_START);

        /* 等待转换完成 */
        while (__g_adc_complete == AM_FALSE);

        for (sum = 0,i = 0; i < 10 ; i++) {
            sum += __g_adc_dat[i];
        }

        sum = sum / 10;

        /* 转换为电压值对应的整数值 */
        adc_mv = sum * 3300 / ((1UL << 12) -1);

        am_kprintf("Sample : %d, Vol: %d mv\r\n", sum, adc_mv);

        am_mdelay(500);
    }
}
/** [src_mm32_hw_adc_int] */

/* end of file */
