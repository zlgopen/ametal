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
 * \brief AM_ZML166_ADC测量固定电压，通过标准接口实现
 *
 * - 实验现象：
 *   1. 连接好串口，将PT100电阻接入TCB与TCA之间。
 *   2. 串口将会打印出热电偶测量温度。
 *
 * \par 源代码
 * \snippet dome_zml166_adc_pt100_measure.c src_demo_zml166_adc_thermistor_measure
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-28  htf, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_demo_zml166_adc_thermistor_measure
 * \copydoc demo_zml166_adc_thermistor_measure.c
 */

#include "am_zml166_adc.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_thermocouple.h"
#include "math.h"
#include "demo_zlg_entries.h"
/**
 * \brief 测试AML166外接热电偶测量温度例程
 */
void dome_zml166_adc_thermocouple_measure_entry(void     *p_handle,
                                                float    *p_para,
                                                void     *p_ther_handle)
{
    double   temp;
    int32_t  adc_val[1];
    am_zml166_adc_handle_t  handle      = (am_zml166_adc_handle_t)p_handle;
    am_ther_formula_t       ther_handle = (am_ther_formula_t)p_ther_handle;
    am_adc_handle_t         adc_handle  = &handle->adc_serve;

    while(1){
        double temperature, v1;
        /* 测量冷端温度 */
        am_zml166_adc_gain_set(handle, 1);
        am_zml166_adc_reg_set(handle,
                              AM_ZML166_ADC_ADC3_ADDR,
                              AM_ZML166_ADC_LVSHIFT_ENABLE|AM_ZML166_ADC_LVSCP_ENABLE);
        am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN2 | AM_ZML166_ADC_INNS_GND);
        am_adc_read(adc_handle, 0, (uint32_t *)adc_val, AM_NELEMENTS(adc_val));

        temp  = (double)((double)adc_val[0] / 8388607 * 2500 );
        temp  = temp * p_para[0] + p_para[1];
        temp = (double)((temp * 8388607 / 2500 ) / (double)((1<<23)-adc_val[0]));
        temperature = 1 / (log(temp)/3435 + 1 / 298.15)-273.15;

        am_ther_tem_to_vol(ther_handle, temperature, &v1);

        am_zml166_adc_reg_set(handle,
                              AM_ZML166_ADC_ADC3_ADDR,
                              AM_ZML166_ADC_LVSHIFT_DISABLE|AM_ZML166_ADC_LVSCP_ENABLE);
        am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN3 | AM_ZML166_ADC_INNS_AIN4);

        am_zml166_adc_gain_set(handle, 16);
        am_adc_read(adc_handle, 0, (uint32_t *)adc_val, AM_NELEMENTS(adc_val));
        temp  = (double)((double)adc_val[0] / 16.0 * 2500 / 8388607  );
        temp  = temp * p_para[8] + p_para[9];

        am_ther_vol_to_tem(ther_handle, temp + v1, &temperature);

        temperature *= 1000;

        if(temperature < 0){
            temperature *= -1;
            am_kprintf("Tem = -%d.%03d°\r\n",
             (uint32_t)temperature/1000 ,
             (uint32_t)temperature%1000);
        }else{
            am_kprintf("Tem = %d.%03d°\r\n\r\n",
             (uint32_t)temperature/1000 ,
             (uint32_t)temperature%1000);
        }
        am_mdelay(300);
    }
}
