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
 *   1. 连接好串口，将PT100电阻接入RTDC与RTDB之间，并短接RTDA。
 *   2. 串口将会打印出PT100阻值以及温度值
 *
 * \par 源代码
 * \snippet dome_adc24_pt100_measure.c src_dome_adc24_pt100_measure
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-28  htf, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_dome_adc24_pt100_measure
 * \copydoc dome_adc24_pt100_measure.c
 */
#include "ametal.h"
#include "am_adc24.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_pt100_to_temperature.h"

/**
 * \brief 获取PT100热电阻阻值
 */
float am_adc24_thermistor_res_data_get(am_adc24_handle_t  handle)
{
    uint8_t  i;
    float  r_data = 0;
    int32_t   vol_rtdb_c = 0, vol_rtda_c = 0, vol_res = 0;
    int32_t  adc_val[4];
    am_adc_handle_t adc_handle = &handle->adc_serve;
    //设置通道为ADC_2 ADC_4    RTDB---RTDC
    am_adc24_mux_set(handle, ADC24_INPS_AIN(2) | ADC24_INNS_AIN(3));
    am_adc_read(adc_handle, 0, (uint32_t *)adc_val, AM_NELEMENTS(adc_val));
    for(i = 0; i < AM_NELEMENTS(adc_val); i++){
        vol_rtdb_c += adc_val[i];
    }
    vol_rtdb_c /= AM_NELEMENTS(adc_val);

    /*  设置通道为ADC_3 ADC_4     RTDA---RTDC */
    am_adc24_mux_set(handle, ADC24_INPS_AIN(3) | ADC24_INNS_AIN(3));
    am_adc_read(adc_handle, 0, (uint32_t *)adc_val, AM_NELEMENTS(adc_val));

    for(i = 0; i < AM_NELEMENTS(adc_val); i++){
        vol_rtda_c += adc_val[i];
    }
    vol_rtda_c /= AM_NELEMENTS(adc_val);

    vol_res = vol_rtdb_c * 2 - vol_rtda_c ;

    /* 调用电压校准系数  */
    r_data = (float)((float)(vol_res / 8388607.0) * 1999.36);

    return r_data;
}
/**
 * \brief 测试AML166板级外接PT100热电阻的阻值以及对应的转换温度
 */
void demo_adc24_pt100_measure_entry(am_adc24_handle_t  handle,
                                    float             *p_para)
{
    float  r_data = 0, temperature = 0;

    while(1){
        /* 设置PT100增益倍数 */
        am_adc24_gain_set(handle, 1);

        r_data = am_adc24_thermistor_res_data_get(handle);
        /* 电阻校准系数  */
        r_data = p_para[0] * r_data + p_para[1];

        /* PT100电阻转温度  */
        temperature = pt100_to_temperature(r_data);

        if(temperature < 0){
            temperature *= -1;
            am_kprintf("Tem = -%d.%03d°\r\n",
               ((int32_t)(temperature * 1000) / 1000) ,
               ((int32_t)(temperature * 1000) % 1000));
        }else{
            am_kprintf("Tem = %d.%03d°\r\n\r\n",
               ((int32_t)(temperature * 1000) / 1000) ,
               ((int32_t)(temperature * 1000) % 1000));
        }
        am_mdelay(200);
    }
}
