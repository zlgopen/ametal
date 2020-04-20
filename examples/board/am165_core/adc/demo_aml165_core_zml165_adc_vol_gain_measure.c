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
 * \brief
 * - 实验现象：
 *   1. 连接好串口,连接好PT100。
 *   2. 串口将会打印对应的温度值
 *
 * \par 源代码
 * \snippet demo_aml165_core_adc24_vol_measure.c src_aml165_core_adc24_vol_measure
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-15  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_aml165_core_adc24_vol_measure
 * \copydoc demo_aml165_core_adc24_vol_measure.c
 */

/** [src_aml165_core_adc24_vol_measure] */

#include <am_hwconf_zml165_24adc.h>
#include "am_zml165_adc.h"
#include "demo_zlg_entries.h"
#include "am_aml165_inst_init.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 串口接收以'\n'为帧尾的数据
 */
static void uart_str_get(am_uart_handle_t handle, char *buffer)
{
    int   uart_data_count = 0;
    while(1){
        if(am_uart_poll_getchar(handle, &buffer[uart_data_count]) == AM_OK){
            uart_data_count ++;
            if(buffer[uart_data_count - 1] == '\n'){
                break;
            }
        }
    }
}

/**
 * \brief ZML165  24位ADC 固定电压测量例程
 */
void demo_aml165_core_zml165_adc_vol_gain_measure (void)
{
    char    uart_data[20] ;
    const uint8_t pga[4] = {1, 2, 64, 128};
    int32_t adc_val[10];
    uint8_t gpa_index = 0;
    uint8_t is_vol = 0;
    am_zml165_adc_config_t config;
    am_uart_handle_t       uart_handle  = am_zml165_uart1_inst_init();
    am_zml165_adc_handle_t handle = am_zml165_24adc_inst_init();

    while(1){
        am_kprintf("Please chose your option. eg: ‘1\\n’ \n");
        am_kprintf("1. Channel A \r\n");
        am_kprintf("2. Channel B  \r\n");
        uart_str_get(uart_handle, uart_data);
        if((uart_data[0] <= '2') && (uart_data[0] >= '1')){
            config.channel = uart_data[0] - '1';
            break;
        }else{
            am_kprintf("Input error \r\n");
        }
    }

    while(1){
        am_kprintf("Please chose your option. eg: ‘1\\n’ \n");
        am_kprintf("1. Speed 10 \r\n");
        am_kprintf("2. Speed 40  \r\n");
        am_kprintf("3. Speed 640 \r\n");
        am_kprintf("4. Speed 1280  \r\n");
        uart_str_get(uart_handle, uart_data);
        if((uart_data[0] <= '4') && (uart_data[0] >= '1')){
            config.speed = uart_data[0] - '1';
            break;
        }else{
            am_kprintf("Input error \r\n");
        }
    }

    while(1){
        am_kprintf("Please chose your option. eg: ‘1\\n’ \n");
        am_kprintf("1. GAIN = 1  \r\n");
        am_kprintf("2. GAIN = 2  \r\n");
        am_kprintf("3. GAIN = 64  \r\n");
        am_kprintf("4. GAIN = 128  \r\n");
        uart_str_get(uart_handle, uart_data);
        if((uart_data[0] <= '4') && (uart_data[0] >= '1')){
            gpa_index = uart_data[0] - '1';
            break;
        }else{
            am_kprintf("Input error \r\n");
        }
    }
    reset:
    while(1){
        am_kprintf("Please chose your option. eg: ‘1\\n’ \n");
        am_kprintf("1. Print Data \r\n");
        am_kprintf("2. Print Vol  \r\n");
        am_kprintf("3. 1000 Times Code Change  \r\n");
        uart_str_get(uart_handle, uart_data);
        if((uart_data[0] <= '3') && (uart_data[0] >= '1')){
            is_vol = uart_data[0] - '1';
            break;
        }else{
            am_kprintf("Input error \r\n");
        }
    }


    config.pga = gpa_index;
    config.refo_off = AM_ZML165_ADC_VOUT_DISABLE;
    am_zml165_adc_config_load(handle, &config);
    am_adc_read(&handle->adc_serve, 0, (void *)adc_val, 4);

    int32_t max = 0, min = 0xffffff;
    if(is_vol == 2){
        am_kprintf("Working...\r\n");
        for(uint8_t j = 0 ; j < 10 ; j ++){
            int32_t adc_val1[100];
            am_adc_read(&handle->adc_serve, 0, (void *)adc_val1, AM_NELEMENTS(adc_val1));
            for(uint8_t i = 0; i < 100; i ++){
                if(adc_val1[i] > max){
                    max = adc_val1[i];
                }
                if(adc_val1[i] <  min){
                    min = adc_val1[i];
                }
            }
        }
        am_kprintf("Max - Min = %d", max - min);
        goto reset;
    }



    while(1){
        char    str;
        int32_t adc_val[1];
        float   vol = 0;

        am_adc_read(&handle->adc_serve, 0, (void *)adc_val, AM_NELEMENTS(adc_val));
        /* 丢弃前四个数据以保证设置生效后数据建立时间 */
        for(uint8_t i = 0 ; i < AM_NELEMENTS(adc_val); i++){
            if(adc_val[i] >= 0x800000)  {
                adc_val[i] &= 0x7fffff;
                adc_val[i] |= 0xff800000;
            }
            if (!is_vol){
                am_kprintf("%d\n", adc_val[i]);
            }
            vol += (adc_val[i] / ((double)AM_NELEMENTS(adc_val)));
        }

        vol = (double)((double)(vol  / ((1 << 23) -1)) * handle->p_devinfo->vref * 0.5);
        vol *= 10000;

        vol /= pga[config.pga];

        if(is_vol){
            if(vol > 0){
                am_kprintf("%d.%04d mV\r\n", (int32_t)vol/10000, (int32_t)vol%10000);
            }else {
                vol *= -1;
                am_kprintf("-%d.%04d mV\r\n", (int32_t)vol/10000, (int32_t)vol%10000);
            }
        }

        if(am_uart_poll_getchar(uart_handle, &str) == AM_OK){
            if(str == 'e'){
                break;
            }
        }
    }
}

/** [src_aml165_core_adc24_vol_measure] */

/* end of file */
