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
 * \brief adc24测量固定电压，通过标准接口实现
 *
 * - 实验现象：
 *   1. 连接好串口，并将测量电压输入对应的通道。
 *   2. 选择对应的增益倍数，被测电压*增益应小于基准电压
 *   3. 串口将会打印出电压值
 *   4. 可在程序运行中切换电压值  直接输入相应的数字即可。
 *
 * \par 源代码
 * \snippet dome_ADC24_adjust_vol_entry.c src_dome_ADC24_adjust_vol_entry
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-28  htf, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_ADC24_adjust_vol_entry
 * \copydoc dome_ADC24_adjust_vol_entry.c
 */
/*\brief  头文件*/
#include "ametal.h"
#include "string.h"
#include "am_vdebug.h"
#include "am_adc24.h"

/*\brief  宏定义*/


const static uint8_t __g_pag[8] = {1, 2, 4, 8, 16, 32, 64, 128};

/**
 * \brief 串口获取以'\n'结尾的字符串
 */
static void __uart_str_get(am_uart_handle_t handle, char *buffer)
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
 * \brief 打印校准系数公式
 */
static void __print_adjust_function(uint8_t pag_index,float para_1, float para_2)
{
    if(pag_index == 10){
        am_kprintf("The PT100 formula is : \r\n");
    }else if(pag_index == 11){
        am_kprintf("The Thermocouple formula is : \r\n ");
    }else {
        am_kprintf("The PGA %d formula is : \r\n ", __g_pag[pag_index]);
    }
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
/*
 * \brief  取出字符串中浮点数  数前标志位 '-'
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
 * \brief 字符串比较
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

/**
 * \brief AML166电压校准函数（7种增益下修调系数 不含128倍）
 */
void am_adc24_adjust_entry(am_adc24_handle_t   handle,
                           am_uart_handle_t    uart_handle,
                           float              *p_para)
{
    int     i;
    char    uart_data[20] ;
    int32_t val         = 0;
    int32_t adc_val[10] = {0};
    float   cail0_vol, cail1_vol, x1[8], x2[8];
    am_kprintf("Entry CS1239 adjust mode \r\n\r\n");

    while(AM_OK != __str_cmp(uart_data, "V0:", 3)){
        am_kprintf("Please input input cali0-cali1 voltage(mV). (eg:‘V0:3.145\\n’)\r\n");
        __uart_str_get(uart_handle, uart_data);
    }
    cail0_vol = __atof(&uart_data[3]);
    am_kprintf("You input valtage difference of cali0 and cali1 is %d.%03d mV\r\n\r\n",
               (uint32_t)(cail0_vol * 1000) / 1000,
               (uint32_t)(cail0_vol * 1000) % 1000);


    while(AM_OK != __str_cmp(uart_data, "V1:", 3)){
        am_kprintf("Please input cali2-cali1 voltage(mV). (eg:‘V1:30.755\\n’)\r\n");
        __uart_str_get(uart_handle, uart_data);
    }
    cail1_vol = __atof(&uart_data[3]);
    am_kprintf("You input valtage difference of cali2 and cali1` is %d.%03d mV\r\n\r\n",
               (uint32_t)(cail1_vol * 1000) / 1000,
               (uint32_t)(cail1_vol * 1000) % 1000);

    am_kprintf("Please config V0 measure(A12-cali0  A13-cali1)\n");
    while(AM_OK != __str_cmp(uart_data, "Y", 1)){
        am_kprintf("Are you do it? (Intput ‘Y\\n’ to ensure you have do it) \r\n");
        __uart_str_get(uart_handle, uart_data);
    }
    am_kprintf("Y\r\n\r\n");

    memset(uart_data, 0, 20);
    am_adc24_mux_set(handle, ADC24_INPS_AIN2 | ADC24_INNS_AIN3);
    for(i = 0 ; i < 7; i ++){
			  int  j = 0;
        val = 0;
        am_adc24_gain_set(handle, __g_pag[i]);
        am_adc_read(&handle->adc_serve, 0, (void *)adc_val, 10);
        for(j = 0 ;j < 10; j++){
            val += adc_val[j] / 10;
        }
        x1[i] = ((float)((float)(val  / 8388607.0) * handle->p_devinfo->vref)) / __g_pag[i];
    }

    am_kprintf("Please config V1 measure(A12-cali2  A13-cail1)\n");
    while(AM_OK != __str_cmp(uart_data, "Y", 1)){
        am_kprintf("Are you do it? (Intput ‘Y\\n’ to ensure you have do it) \r\n");
        __uart_str_get(uart_handle, uart_data);
    }
    am_kprintf("Y\r\n\r\n");

    memset(uart_data, 0, 20);
    am_adc24_mux_set(handle, ADC24_INPS_AIN3 | ADC24_INNS_AIN2);

    for(i = 0 ; i < 7; i ++){
        int j = 0;
			  val = 0;
        am_adc24_gain_set(handle, __g_pag[i]);
        am_adc_read(&handle->adc_serve, 0, (void *)adc_val, 10);
        for(j = 0 ;j < 10; j++){
            val += adc_val[j] / 10;
        }
            x2[i] = ((float)((float)(val  / 8388607.0) * handle->p_devinfo->vref)) / __g_pag[i];
            p_para[2 * i] = (cail1_vol - cail0_vol) / (x2[i] - x1[i]);
            p_para[2 * i + 1] = (cail1_vol) - p_para[2 * i] * x2[i];
            __print_adjust_function(i, p_para[2 * i], p_para[2 * i + 1]);
        }
    am_kprintf("Finish ADC1 calibration!\nExit ADC1 adjust mode \r\n\r\n");
}

/*
 * \brief ADC进行系数校准Demo
 */
void demo_adc24_vol_para_adjuet_entry(am_adc24_handle_t  handle,
                                      am_uart_handle_t   uart_handle,
                                      float             *p_para)
{
    char uart_data[20];

    while(uart_data[0] <'0' || uart_data[0] >'2'){
        am_kprintf("Please choose your option. eg: ‘1\\n’  \r\n");
        am_kprintf("1. Enter calibration mode.\r\n");
        am_kprintf("2. Check parameters.  \r\n");
        __uart_str_get(uart_handle, uart_data);
    }

    if(uart_data[0] == '1'){
        am_adc24_adjust_entry(handle, uart_handle, p_para);
    }else if(uart_data[0] == '2'){
			  int i  = 0;
        memcpy((void *)p_para, (uint32_t *)((FLASH_BLOCK_NUM * 1024)), 4 * 18);
        for(i = 0 ;i < 7; i++){
            __print_adjust_function(i, p_para[i * 2], p_para[i * 2 + 1]);
        }
        __print_adjust_function(10, p_para[16], p_para[17]);
        __print_adjust_function(11, p_para[8],  p_para[9]);
    }
}
