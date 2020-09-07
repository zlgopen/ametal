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
 *   1. 连接好串口，将PT100电阻接入RTDC与RTDB之间，并短接RTDA。
 *   2. 串口将会打印出PT100阻值以及温度值
 *
 * \par 源代码
 * \snippet dome_zml166_adc_pt100_measure.c src_dome_zml166_adc_pt100_measure
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-28  htf, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_dome_zml166_adc_pt100_measure
 * \copydoc dome_zml166_adc_pt100_measure.c
 */
#include "ametal.h"
#include "am_zml166_adc.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_pt100_to_temperature.h"
#include "am_common.h"

/**
 * \brief 获取PT100热电阻阻值
 */
float am_zml166_adc_thermistor_res_data_get(void *p_handle)
{
    am_zml166_adc_handle_t  handle = (am_zml166_adc_handle_t)p_handle;
    uint8_t  i;
    float  r_data = 0;
    int32_t   vol_rtdb_c = 0, vol_rtda_c = 0, vol_res = 0;
    int32_t  adc_val[4];
    am_adc_handle_t adc_handle = &handle->adc_serve;
    //设置通道为ADC_2 ADC_4    RTDB---RTDC
    am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN(2) | AM_ZML166_ADC_INNS_AIN(3));
    am_adc_read(adc_handle, 0, (uint32_t *)adc_val, AM_NELEMENTS(adc_val));
    for(i = 0; i < AM_NELEMENTS(adc_val); i++){
        vol_rtdb_c += adc_val[i];
    }
    vol_rtdb_c /= 4;

    /*  设置通道为ADC_3 ADC_4     RTDA---RTDC */
    am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN(3) | AM_ZML166_ADC_INNS_AIN(3));
    am_adc_read(adc_handle, 0, (uint32_t *)adc_val, AM_NELEMENTS(adc_val));

    for(i = 0; i < AM_NELEMENTS(adc_val); i++){
        vol_rtda_c += adc_val[i];
    }
    vol_rtda_c /= AM_NELEMENTS(adc_val);

    vol_res = vol_rtdb_c * 2 - vol_rtda_c ;

    if(vol_res < 0){
        vol_res *= -1;
    }

    /* 调用电压校准系数  */
    r_data = (float)((float)(vol_res / 8388607.0) * 1999.36);

    return r_data;
}
/**
 * \brief 测试AML166板级外接PT100热电阻的阻值以及对应的转换温度
 */
void demo_zml166_adc_pt100_measure_entry(am_zml166_adc_handle_t  handle,
                                         float                  *p_para)
{
    float  r_data = 0, temperature = 0;

    while(1){
        /* 设置PT100增益倍数 */
        am_zml166_adc_gain_set(handle, 1);

        r_data = am_zml166_adc_thermistor_res_data_get(handle);
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
