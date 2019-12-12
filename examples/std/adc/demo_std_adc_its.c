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
 * \brief ADC内部温度传感器例程，利用查询方式获取ADC转换结果，通过标准接口实现
 *
 * - 操作步骤：
 *  am_hwconf_hc32l19x_adc.c文件修改设备信息中：
 *   1. 参考电压选择： AMHW_HC32_ADC_REFVOL_INSIDE_1500MV 或 AMHW_HC32_ADC_REFVOL_INSIDE_2500MV；
 *   2. 参考电压(mv)： 1500 或 2500；
 *   3. 使能ADC通道28内部温度传感器.(1：使能，0：禁能)
 *
 *
 * - 实验现象：
 *   1. 串口输出温度采样值。
 *
 * \par 源代码
 * \snippet demo_std_adc_its.c src_std_adc_its
 *
 * \internal
 * \par Modification History
 * - 1.00 19-12-09  fzb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_adc_its
 * \copydoc demo_std_adc_its.c
 */

/** [src_std_adc_its] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_adc.h"

/**
 * \brief 获取 ADC 转换值
 */
static uint32_t __adc_code_get (am_adc_handle_t handle, int chan)
{
    int      adc_bits = am_adc_bits_get(handle, chan);
    int      i;
    uint32_t sum;

    /*
     * 启动ADC转换器，采集12次 CODE 值
     *
     * 实际应用中，所使用的 ADC 位数是确定的，无需这样分多种情况考虑
     */
    if (adc_bits <= 8) {
        uint8_t val_buf[12];
        am_adc_read(handle, chan, val_buf, 12);
        for (sum = 0, i = 0; i < 12; i++) {       /* 均值处理 */
            sum += val_buf[i];
        }
    } else if (adc_bits <= 16) {
        uint16_t val_buf[12];
        am_adc_read(handle, chan, val_buf, 12);
        for (sum = 0, i = 0; i < 12; i++) {       /* 均值处理 */
            sum += val_buf[i];
        }
    } else {
        uint32_t val_buf[12];
        am_adc_read(handle, chan, val_buf, 12);
        for (sum = 0, i = 0; i < 12; i++) {       /* 均值处理 */
            sum += val_buf[i];
        }
    }
    return (sum / 12);
}

/**
 * \brief 例程入口
 */
void demo_std_adc_its_entry (am_adc_handle_t handle, int chan)
{

    int adc_bits = am_adc_bits_get(handle , chan);          /* 获取ADC转换精度  */
    int adc_vref = am_adc_vref_get(handle , chan);

    uint32_t adc_code;                                      /* 采样 Code 值     */
    uint16_t adc_trim;                                      /* 16Bits校准值     */
    int32_t adc_temp;                                       /* 环境温度         */

    am_kprintf("The ADC value channel is %d: \r\n",chan);

    if (adc_bits < 0 || adc_bits >= 32) {
        am_kprintf("The ADC  channel is error, Please check! \r\n");
        return;
    }

    if (adc_vref == 1500) {
        adc_trim = (*((volatile uint16_t*)(0x00100C34)));
    } else if (adc_vref == 2500) {
        adc_trim = (*((volatile uint16_t*)(0x00100C36)));
    } else {
        return;
    }

    /* 串口输出参考电压与16Bits的校准值 */
    am_kprintf("Reference Voltage : %d mV , Trim : %d \r\n", adc_vref, adc_trim);

    while (1) {

        adc_code = __adc_code_get(handle, chan);

        if (adc_vref == 1500) {
            adc_temp = (int32_t)(25 + 0.0839 * 1.5 * (int32_t)(adc_code - adc_trim));
        } else if (adc_vref == 2500) {
            adc_temp = (int32_t)(25 + 0.0839 * 2.5 * (int32_t)(adc_code - adc_trim));
        } else {
            return;
        }

        /* 串口输出采样温度值 */
        am_kprintf("Sample : %d, Temperature: %d °C\r\n", adc_code, adc_temp);

        am_mdelay(500);
    }
}
/** [src_std_adc_its] */

/* end of file */
