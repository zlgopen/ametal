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
 * \brief ADC例程，软件触发转换，DMA传输转换结果，使用多个传输描述符，通过标准接口实现
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. J12跳线帽短接（此时参考电压为2.5v）；
 *   4. PIOE_20(ADC通道0) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_fsl_std_adc_descs_st.c src_fsl_std_adc_descs_st
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-18  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_std_adc_descs_st
 * \copydoc demo_fsl_std_adc_descs_st.c
 */

/** [src_fsl_std_adc_descs_st] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_adc.h"
#include "am_board.h"
#include "hw/amhw_fsl_adc.h"
#include "am_int.h"

volatile static uint8_t __g_flag_complete  = 0;   /**< \brief ADC转换完成标志 */
volatile static uint8_t __g_flag_complete0 = 0;   /**< \brief 数据缓冲区0完成标志 */
volatile static uint8_t __g_flag_complete1 = 0;   /**< \brief 数据缓冲区1完成标志 */

/** \brief 缓冲区描述符填充满回调函数 */
static void __fsl_adc_callback (void *p_arg, int state)
{
    if (state != AM_OK) {
        return;
    }

    if ((int)p_arg == 0) {
        __g_flag_complete0 = 1;
    }

    if ((int)p_arg == 1) {
        __g_flag_complete1 = 1;
    }
}

/** \brief ADC转换完成回调函数 */
void __fsl_adc_seq_complete (void *p_arg, int state)
{
    __g_flag_complete = 1;
}

/**
 * \brief adc软件触发，DMA传输结果，一次采样两个ADC描述符
 * \return 无
 */
void demo_fsl_std_adc_descs_st_entry (am_adc_handle_t adc0_handle)
{
    uint32_t adc_bits = 0;          /**< \brief ADC位数 */
    uint32_t adc_vref = 0;          /**< \brief ADC参考电压*/
    uint32_t adc_mv;                /**< \brief 采样电压 */
    int      chan = 0;              /**< \brief 通道0 */
    int      i;
    am_adc_buf_desc_t   desc[2];                /**< \brief 使用两个缓冲区描述 */
    uint16_t            adc_val0[10];           /**< \brief 数据缓冲区1     */
    uint16_t            adc_val1[10];           /**< \brief 数据缓冲区2     */
    uint32_t key;
    uint32_t adc_sum;

    adc_bits = am_adc_bits_get(adc0_handle, chan); /* 获取ADC转换精度 */
    adc_vref = am_adc_vref_get(adc0_handle, chan); /* 获取ADC参考电压，单位：mV */

    /* 配置缓冲区描述符 */
    am_adc_mkbufdesc(&desc[0], adc_val0, 10, __fsl_adc_callback, (void *)0);
    am_adc_mkbufdesc(&desc[1], adc_val1, 10, __fsl_adc_callback, (void *)1);

    /* 启动ADC转换 */
    am_adc_start(adc0_handle,
                 chan,
                 desc,
                 2,
                 0,
                 AM_ADC_DATA_ALIGN_RIGHT,
                 __fsl_adc_seq_complete,
                 NULL);

    am_kprintf("The KL26 Adc Std descs Software Trigger Demo\r\n");

    while (1) {

        if (1 == __g_flag_complete0) {
            key = am_int_cpu_lock();
            __g_flag_complete0 = 0;
            am_int_cpu_unlock(key);
            am_kprintf("Finished the 1th desc\r\n");
        }

        if (1 == __g_flag_complete1) {
            key = am_int_cpu_lock();
            __g_flag_complete1 = 0;
            am_int_cpu_unlock(key);
            am_kprintf("Finished the 2th desc\r\n");
        }

        if (1 == __g_flag_complete) {
            key = am_int_cpu_lock();
            __g_flag_complete = 0;
            am_int_cpu_unlock(key);

            adc_sum = 0;
            for (i = 0; i < 10; i++) {
                adc_sum += adc_val0[i];
            }

            adc_sum /= 10;
            adc_mv = adc_sum * adc_vref / ((1UL << adc_bits) - 1);

            /* 串口输出采样电压值 */
            am_kprintf("Vol0: %d mv\r\n", adc_mv);

            adc_mv  = 0;
            adc_sum = 0;
            for (i = 0; i < 10; i++) {
                adc_sum += adc_val1[i];
            }

            adc_sum /= 10;
            adc_mv = adc_sum * adc_vref / ((1UL << adc_bits) - 1);

            /* 串口输出采样电压值 */
            am_kprintf("Vol1: %d mv\r\n", adc_mv);

            for (i = 0; i < 10; i++) {
                adc_val0[i] = 0;
                adc_val1[i] = 0;
            }
        }
    }
}

/** [src_fsl_std_adc_descs_st] */

/* end of file */
