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
 * \brief ADC例程，软件触发转换，DMA传输差分转换结果，通过标准接口实现
 *
 * \note 使用此demo需要配置使能am_kl26_hwconfig_adc0.c文件中的AM_FSL_ADC_DIF_EN宏
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. J12跳线帽短接（此时参考电压为2.5v）；
 *   4. PIOE_20(ADC_DP0)和PIOE_21(ADC_DM0)连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_fsl_std_adc_dif_st.c src_fsl_std_adc_dif_st
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-19  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_std_adc_dif_st
 * \copydoc demo_fsl_std_adc_dif_st.c
 */

/** [src_fsl_std_adc_dif_st] */
#include "ametal.h"
#include "am_adc.h"
#include "am_vdebug.h"
#include "am_board.h"

/**
 * \brief ADC差分输入，DMA传输，软件触发
 * \return 无
 */
void demo_fsl_std_adc_dif_st_entry (am_adc_handle_t adc0_handle)
{
    int32_t  adc_bits     = 0;          /**< \brief ADC位数 */
    int32_t  adc_vref     = 0;          /**< \brief ADC参考电压*/
    uint16_t adv_val0[10] = {0};        /**< \brief ADC采样原始值 */
    int32_t  adc_sum      = 0;
    int16_t  adc_mv;                /**< \brief 采样电压 */
    int      chan = 0;              /**< \brief 通道0 */
    int      i;

    adc_bits = am_adc_bits_get(adc0_handle, chan); /* 获取ADC转换精度 */
    adc_vref = am_adc_vref_get(adc0_handle, chan); /* 获取ADC参考电压，单位：mV */

    am_kprintf("The KL26 Adc Dif Software Trigger Demo\r\n");

    while (1) {

        /* 获取ADC转换结果，采样完成才返回 */
        am_adc_read(adc0_handle, chan, adv_val0, 10);

        adc_sum = 0;
        for (i = 0; i < 10; i++) {

            if (adv_val0[i] & 0x8000) {
                /* 数据为负数的补码，进行转换 */
                adv_val0[i] = ((~adv_val0[i]) + 1) & 0xFFFF;
            }

            adc_sum += adv_val0[i];
        }
        /**
         * 差分模式下，位数改变，最高的若干位数变成符号位，但是尽管位数改变了，
         * 但是ADC模块的量程不应该改变的，所以应该其采样的数值基础上×2
         */
        adc_sum /= 5;
        adc_mv = adc_sum * adc_vref / ((1 << adc_bits) - 1);

        /* 串口输出采样电压值 */
        am_kprintf("Vol: %d mv\r\n", adc_mv);

        am_mdelay(500);
    }
}

/** [src_fsl_std_adc_dif_st] */

/* end of file */
