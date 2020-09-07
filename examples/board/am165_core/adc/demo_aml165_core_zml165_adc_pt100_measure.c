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
 *   1. 连接好串口，并将测量电压输入对应的通道。
 *   2. 串口将会打印出电压值
 *
 * \par 源代码
 * \snippet demo_aml165_core_zml165_adc_vol_measure.c aml165_core_zml165_adc_vol_measure
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-15 htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_aml165_core_zml165_adc_vol_measure
 * \copydoc demo_aml165_core_zml165_adc_vol_measure.c
 */

/** [src_aml165_core_zml165_adc_vol_measure] */
#include <am_hwconf_zml165_24adc.h>
#include "string.h"
#include "am_zml165_adc.h"
#include "am_zlg_flash.h"
#include "zml165_periph_map.h"
#include "demo_zlg_entries.h"
#include "am_aml165_inst_init.h"
#include "am_pt100_to_temperature.h"
#include "demo_aml165_core_entries.h"

#define FLASH_BLOCK_NUM 31

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
 * \brief 字符串比较函数
 */
static int __str_cmp(char *p_str1, char *p_str2, int count)
{
    int i = 0;
    while(*p_str1 != '\0' && *p_str2 != '\0'){
        if(*p_str1 == *p_str2){
            p_str1 ++;
            p_str2 ++;
            i ++;
            if(i == count){
                return AM_OK;
            }
            continue;
        }else{
            return 1;
        }
    }
    return 1;
}

/*
 * \brief  取出字符串中浮点数
 */
static float __atof(char *str_a)
{
    int     sum_int  = 0.0;
    float   temp     = 1.0;
    float   sum_f    = 0.0;
    uint8_t flag     = 0;
    uint8_t flag_neg = 0;

    if (*str_a == '-') {
        flag_neg = 1;
        str_a++;
    }
    for (; *str_a != '\n'; ) {

        if ('.' == *str_a) {
            flag = 1;
            str_a++;
            continue;
        }
        if(0 == flag && *str_a >= '0' && *str_a <= '9'){
            sum_int = sum_int * 10 + (*str_a - '0');
        } else if( *str_a >= '0' && *str_a <= '9'){
            temp /= 10.0;
            sum_f = sum_f  + (*str_a - '0') * temp;
        }
        str_a++;
    }
    sum_f += sum_int;
    sum_f += 0.0005;
    if (flag_neg == 1) {
        return -sum_f;
    } else {
        return sum_f;
    }
}

/**
 * \brief 公式打印函数
 */
static void __print_adjust_function(float para_1, float para_2)
{
    am_kprintf("The PT100 formula is : \r\n");

    if(para_2 > 0){
        am_kprintf("Y = %1d.%07d * X + %1d.%04d\r\n\r\n",
                   ((int32_t)(para_1 * 10000000) / 10000000),
                   ((int32_t)(para_1 * 10000000)) % 10000000,
                   ((int32_t)(para_2 * (10000)) / 10000),
                   ((int32_t)(para_2 * 10000)) % 10000);
    }else{
        am_kprintf("Y = %1d.%07d * X - %1d.%04d\r\n\r\n",
                   ((int32_t)((para_1 * 10000000) / 10000000)),
                   ((int32_t)(para_1 * 10000000)) % 10000000,
                   ((int32_t)(para_2 * (-10000)) / 10000),
                   ((int32_t)(para_2 * (-10000))) % 10000);
    }
}


float am_zml165_thermistor_res_data_get(am_zml165_adc_handle_t handle)
{
    uint8_t i   = 0;
    float   r_data = 0;
    int32_t adc_val[1] = {0};
    am_zml165_adc_config_t config;
    int32_t vol_bc = 0, vol_ac = 0, vol_res = 0;
    config.pga = AM_ZML165_ADC_PGA_1;
    config.speed = AM_ZML165_ADC_SPEED_10HZ;
    config.channel = AM_ZML165_ADC_CHANNEL_B;
    config.refo_off = AM_ZML165_ADC_VOUT_DISABLE;
    am_zml165_adc_config_load(handle, &config);

    /* 丢弃前四个数据以保证设置生效后数据建立时间 */
    for(i = 0 ; i < AM_NELEMENTS(adc_val); i++){
        if(adc_val[i] >= 0x800000)  {
            adc_val[i] &= 0x7fffff;
            adc_val[i] |= 0xff800000;
        }
        vol_bc += (adc_val[i] / ((double)AM_NELEMENTS(adc_val)));
    }

    vol_bc *= -1;
    config.pga = AM_ZML165_ADC_PGA_1;
    config.speed = AM_ZML165_ADC_SPEED_10HZ;
    config.channel = AM_ZML165_ADC_CHANNEL_A;
    config.refo_off = AM_ZML165_ADC_VOUT_DISABLE;
    am_zml165_adc_config_load(handle, &config);
    am_adc_read(&handle->adc_serve, 0, (void *)adc_val, AM_NELEMENTS(adc_val));
    /* 丢弃前四个数据以保证设置生效后数据建立时间 */
    for(i = 0 ; i < AM_NELEMENTS(adc_val); i++){
        if(adc_val[i] >= 0x800000)  {
            adc_val[i] &= 0x7fffff;
            adc_val[i] |= 0xff800000;
        }
        vol_ac += (adc_val[i] / ((double)AM_NELEMENTS(adc_val)));
    }
    vol_res = vol_bc * 2 - vol_ac ;
    if(vol_res < 0){
        vol_res *= -1;
    }

    r_data = (float)(((float)(vol_res) / ((1 << 24) -1)) * 1999.36);
    return r_data;
}

/**
 * \brief 热电阻校准系数获取函数
 */
void demo_aml165_thermistor_para_adjust_entry(am_uart_handle_t       uart_handle,
                                              am_zml165_adc_handle_t adc_handle)
{
    char  uart_data[20];
    float pt100_para[2] = {1, 0};
    float r1_std, r2_std, r1_mea, r2_mea;

    am_kprintf("Enter PT100 ratio adjust mode...\n");
   /* 获取R1标准值 */
    while(AM_OK != __str_cmp(uart_data, "r1:", 3)){
        am_kprintf("Please input R1 resistance(unit:Ω). eg:  r1:30.112\\n\n ");
        uart_str_get(uart_handle, uart_data);
    }
    r1_std = __atof(&uart_data[3]);
    am_kprintf("You intput R1 is %d.%03d Ω\r\n\r\n",
               (uint32_t)(r1_std*1000)/1000,
               (uint32_t)(r1_std*1000)%1000);
    /* 获取R2标准值 */
    while(AM_OK != __str_cmp(uart_data, "r2:", 3)){
        am_kprintf("Please input R2 resistance(unit:Ω). eg:  r2:30.112\\n\n ");
        uart_str_get(uart_handle, uart_data);
    }
    r2_std = __atof(&uart_data[3]);
    am_kprintf("You intput R2 is %d.%03d Ω\r\n\r\n",
               (uint32_t)(r2_std * 1000) / 1000,
               (uint32_t)(r2_std * 1000) % 1000);

    am_kprintf("Please config R1 measure.\n");
    while(AM_OK != __str_cmp(uart_data, "Y", 1)){
        am_kprintf("Are you do it? (Intput ‘Y\\n’ to ensure you have do it) \r\n");
        uart_str_get(uart_handle, uart_data);
    }
    am_kprintf("Y\r\n\r\n");

    r1_mea = am_zml165_thermistor_res_data_get(adc_handle);

    memset(uart_data, 0 , 20);
    am_kprintf("Please config R2 measure.\n");
    while(AM_OK != __str_cmp(uart_data, "Y", 1)){
        am_kprintf("Are you do it? (Intput ‘Y\\n’ to ensure you have do it) \r\n");
        uart_str_get(uart_handle, uart_data);
    }
    am_kprintf("Y\r\n\r\n");

    r2_mea = am_zml165_thermistor_res_data_get(adc_handle);

    if((r2_std > r1_std && r2_mea > r1_mea) || (r2_std < r1_std && r2_mea < r1_mea)){
        pt100_para[0] = (r2_std - r1_std) / (r2_mea - r1_mea);
        pt100_para[1] = r2_std - pt100_para[0] * r2_mea;
    }else{
        r2_std = r2_std + r1_std;
        r1_std = r2_std - r1_std;
        r2_std = r2_std - r1_std;
        pt100_para[0] = (r2_std - r1_std) / (r2_mea - r1_mea);
        pt100_para[1] = r2_std - pt100_para[0] * r2_mea;
    }

    __print_adjust_function(pt100_para[0], pt100_para[1]);

    //判断温度系数修调是否成功
    if((pt100_para[0] < 1.02 && pt100_para[0] > 0.98) &&
            (pt100_para[1] < 0.3 && pt100_para[1] > -0.3)){
        am_kprintf("Demo-2 Success!\r\n\r\n");
    }else{
        am_kprintf("Demo-2 Fail!\r\n\r\n");
        pt100_para[0] = 1;
        pt100_para[1] = 0;
    }

    am_zlg_flash_init(ZML165_FLASH);

    am_zlg_flash_sector_erase (ZML165_FLASH, (1024 * FLASH_BLOCK_NUM));

    am_zlg_flash_sector_program(ZML165_FLASH,
                               (1024 * FLASH_BLOCK_NUM),
                               (uint32_t *)pt100_para,
                                2);
}




/**
 * \brief ZML165  24位ADC 固定电压测量例程
 */
void demo_aml165_core_zml165_adc_thermistor_measure (void)
{

    am_uart_handle_t uart_handle  = am_zml165_uart1_inst_init();
    float para[2] = {0};
    /* 从flash中获取PT100电阻的修正系数 */
    am_zlg_flash_init(ZML165_FLASH);
    memcpy((void *)para, (uint32_t *)(0x08000000 + 1024 * FLASH_BLOCK_NUM), 2 * 4);
    __print_adjust_function(para[0], para[1]);
    /* 若flash中未保存系数 */
    if(!(para[0] < 1.02 && para[0] > 0.98)){
        para[0] = 1;
    }
    if(!(para[1] < 0.5 && para[1] > -0.5)){
        para[1] = 0;
    }
    while(1){
        char  buffer[20] = {0};
        volatile char  buffer1[20] = {0};
        while(AM_OK != __str_cmp(buffer, "demo-", 5)){
            am_kprintf("please select which demo you want to do:(eg: ‘demo-1\\n’) \r\n\r\n");
            am_kprintf("demo-1. PT100 temperature measure  Demo\r\n");
            am_kprintf("demo-2. PT100 temperature para adjust Demo\r\n");
            am_kprintf("demo-3. 24bit ADC Test Voltage Demo\r\n");
            uart_str_get(uart_handle, buffer);
        }
        if(buffer[5] == '1'){
            am_zml165_adc_handle_t handle = am_zml165_24adc_inst_init();
            am_kprintf("Start demo1.\n");
            while(1){
                float   r_data = 0, temperature = 0;

                r_data = am_zml165_thermistor_res_data_get(handle);
                r_data = para[0] * r_data + para[1];
                temperature   = pt100_to_temperature(r_data);

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
            }
        }else if(buffer[5] == '2'){

            am_kprintf("Start demo2.\n");
            while(AM_OK != __str_cmp((char *)buffer1, (char *)"item-", 5)){
                am_kprintf("please select which demo you want to do:(eg: ‘item-1\\n’) \r\n\r\n");
                am_kprintf("1. Show Temperature Para\r\n");
                am_kprintf("2. Temperature Para Adjust\r\n");
                uart_str_get(uart_handle, (char *)buffer1);
            }

            if(buffer1[5] == '1'){
                __print_adjust_function(para[0], para[1]);
            }else{

                am_zml165_adc_handle_t handle = am_zml165_24adc_inst_init();
                demo_aml165_thermistor_para_adjust_entry(uart_handle, handle);
            }
        }else if(buffer[5] == '3'){
            am_zml165_adc_handle_t handle = am_zml165_24adc_inst_init();
            demo_zml165_adc_vol_measure_entry((void *)handle);
        }
    }
}

/** [src_aml165_core_adc24_vol_measure] */

/* end of file */
