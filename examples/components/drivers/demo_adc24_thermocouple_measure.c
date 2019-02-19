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
 * \brief ADC24测量固定电压，通过标准接口实现
 *
 * - 实验现象：
 *   1. 连接好串口，将PT100电阻接入TCB与TCA之间。
 *   2. 串口将会打印出热电偶测量温度。
 *
 * \par 源代码
 * \snippet dome_adc24_pt100_measure.c src_demo_adc24_thermistor_measure
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-28  htf, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_demo_adc24_thermistor_measure
 * \copydoc demo_adc24_thermistor_measure.c
 */

#include "am_adc24.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_thermocouple.h"
#include "math.h"
/**
 * \brief 测试AML166外接热电偶测量温度例程
 */
void dome_adc24_thermocouple_measure_entry(am_adc24_handle_t  handle,
                                           float             *p_para,
                                           uint8_t            type)
{

    int32_t  adc_val[1];
    double   temp;

    am_adc_handle_t     adc_handle  = &handle->adc_serve;

    while(1){
        double    temperature, v1;
        /* 测量冷端温度 */
        am_adc24_gain_set(handle, 1);
        am_adc24_reg_set(handle, ADC24_ADC3_ADDR, ADC24_LVSHIFT_ENABLE|ADC24_LVSCP_ENABLE);
        am_adc24_mux_set(handle, ADC24_INPS_AIN2 | ADC24_INNS_GND);
        am_adc_read(adc_handle, 0, (uint32_t *)adc_val, AM_NELEMENTS(adc_val));

        temp  = (double)((double)adc_val[0] / 8388607 * 2500 );
        temp  = temp * p_para[0] + p_para[1];
        temp = (double)((temp * 8388607 / 2500 ) / (double)((1<<23)-adc_val[0]));
        temperature = 1 / (log(temp)/3435 + 1 / 298.15)-273.15;

        am_thermocouplie_t2v(type, temperature, &v1);

        am_adc24_reg_set(handle, ADC24_ADC3_ADDR, ADC24_LVSHIFT_DISABLE|ADC24_LVSCP_ENABLE);
        am_adc24_mux_set(handle, ADC24_INPS_AIN3 | ADC24_INNS_AIN4);

        am_adc24_gain_set(handle, 16);
        am_adc_read(adc_handle, 0, (uint32_t *)adc_val, AM_NELEMENTS(adc_val));
        temp  = (double)((double)adc_val[0] / 16.0 * 2500 / 8388607  );
        temp  = temp * p_para[8] + p_para[9];

        am_thermocouplie_v2t(type, temp + v1 * 1000, &temperature);

        temperature *= 1000;

        if(temperature < 0){
            temperature *= -1;
            am_kprintf("Tem = -%d.%03d°\r\n", (uint32_t)temperature/1000 , (uint32_t)temperature%1000);
        }else{
            am_kprintf("Tem = %d.%03d°\r\n\r\n", (uint32_t)temperature/1000 , (uint32_t)temperature%1000);
        }
        am_mdelay(300);
    }
}
