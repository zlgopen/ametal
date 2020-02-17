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
 * \brief ADC例程，利用查询方式获取ADC转换结果，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将指定ADC通道对应的引脚连接模拟输入。
 *  am_hwconf_hc32l19x_adc.c文件修改设备信息中：
 *   2. 参考电压选择： AMHW_HC32_ADC_REFVOL_AVCC；
 *   3. 参考电压(mv)： 3300(AVCC)；
 *   4. 禁能ADC通道28内部温度传感器.(1：使能，0：禁能)
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_std_adc.c src_std_adc
 *
 * \internal
 * \par Modification History
 * - 1.00 17-05-05  sdy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_adc
 * \copydoc demo_std_adc.c
 */

/** [src_std_adc] */
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
void demo_std_adc_entry (am_adc_handle_t handle, int chan)
{

    int adc_bits = am_adc_bits_get(handle , chan);        /* 获取ADC转换精度 */
    int adc_vref = am_adc_vref_get(handle , chan);

    uint32_t adc_code;                                    /* 采样 Code 值   */
    uint32_t adc_mv;                                      /* 采样电压 */

    am_kprintf("The ADC value channel is %d: \r\n",chan);

    if (adc_bits < 0 || adc_bits >= 32) {
        am_kprintf("The ADC  channel is error, Please check! \r\n");
        return;
    }

    while (1) {

        adc_code = __adc_code_get(handle, chan);
        adc_mv   = adc_code * adc_vref / ((1UL << adc_bits) - 1);

        /* 串口输出采样电压值 */
        am_kprintf("Sample : %d, Vol: %d mv\r\n", adc_code, adc_mv);

        am_mdelay(500);
    }
}
/** [src_std_adc] */

/* end of file */
