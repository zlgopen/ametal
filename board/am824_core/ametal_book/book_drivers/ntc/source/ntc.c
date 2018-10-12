/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief book`s ntc driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-18  adw, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_lpc82x_inst_init.h"
#include "ntc.h"
static const uint32_t g_temp_res_val [] = {
    70988, 67491, 64191, 61077, 58135, 55356, 52730, 50246, 47896, 45672, 43567, 41572, // -20℃~-9℃
    39682, 37891, 36192, 34580, 33050, 31597, 30218, 28907, 27662, 26477, 25351, 24279, // -8℃~ 3℃
    23259, 22288, 21363, 20482, 19642, 18842, 18078, 17350, 16656, 15993, 15360, 14756, // 4℃~15℃
    14178, 13627, 13100, 12596, 12114, 11653, 11212, 10790, 10387, 10000, 9630, 9275,   // 16℃~ 27℃
    8936, 8610, 8298, 7999, 7712, 7437, 7173, 6920, 6677, 6444, 6220, 6005,             // 28℃~ 39℃
    5799, 5600, 5410, 5227, 5050, 4881, 4718, 4562, 4411, 4267, 4127, 3995,             // 40℃~51℃
    3867, 3744, 3626, 3512, 3402, 3296, 3194, 3096, 3002, 2910, 2822, 2737,             // 52℃~ 63℃
    2655, 2576, 2500, 2426, 2355, 2286, 2220, 2156, 2094, 2034, 1976, 1920,             // 64℃~75℃
    1866, 1813, 1763, 1714, 1666, 1621, 1576, 1533, 1492, 1451, 1412, 1375,             // 76℃~87℃
};                                                                                      
const int res_val_num = sizeof(g_temp_res_val) / sizeof(g_temp_res_val[0]);             // 阻值的个数
static const int temp_start  = -20;       // 表格中的起始温度为-20 ℃
static am_adc_handle_t  adc0_handle;
void ntc_init (void)
{
    adc0_handle = am_lpc82x_adc0_int_inst_init();
}

static uint32_t ntc_vol_get (void)
{
    uint16_t     vol_buf[12];                       // 保存ADC采集的模拟值
    uint16_t     max,min;                           // 12次采样中的最大值与最小值定义
    uint32_t     sum_vol = 0;                       // 采集的电压和值，便于取平均，初始值为0
    uint8_t      i;

    am_adc_read(adc0_handle, 7, vol_buf, 12);       // 采集ADC0通道7的12次adc值
    max = vol_buf[0];
    min = vol_buf[0];
    for (i = 0; i < 12; i++) {
        sum_vol += vol_buf[i];
        if (vol_buf[i] > max) {
            max = vol_buf[i];
        }
        if (vol_buf[i] < min) {
            min = vol_buf[i];
        }
    }
    return AM_ADC_VAL_TO_MV(adc0_handle, 7, (sum_vol - max - min) / 10);
}

static uint32_t ntc_res_get(void)
{
    uint32_t vol = ntc_vol_get();
    return (3300 - vol) * 2000 / vol;
}

static int16_t ntc_temp_get_from_range (int t1,int t2, uint32_t res)
{
    int r1 = g_temp_res_val[t1 - temp_start];          // 得到温度1对应的阻值
    int r2 = g_temp_res_val[t2 - temp_start];          // 得到温度2对应的阻值
    int r  = res;
    int temp;

    temp =  (((t2 - t1) * (r - r1)) << 8) / (r2 - r1) + (t1 << 8); // 为避免小数计算左移8位
    return temp;
}

int16_t ntc_temp_read (void)
{
    uint32_t    res;
    int16_t     temp;
    int         low, high, mid;
    int         t1,t2;

    res  = ntc_res_get();                        // 获取NTC对应的阻值
	  low  = 0;                                    // 初始时low = 0
    high = res_val_num - 1;                     // 初始时high为最后一个元素的索引
    while(1) {                                   // 二分法搜索
        mid = (low + high) >> 1;                 // 右移一位，等效于 (low + high)/2
        if (res == g_temp_res_val[mid]) {        // 恰好相等
            temp = (mid - 20) << 8;              // 整数温度值，扩大256倍
            break;                               // 获取到温度值，结束循环
        }
        if (res > g_temp_res_val[mid]) {
            // 阻值大于中间值，则搜索范围为前半部分，更新high值
            high = mid;
        } else {
            // 阻值小于中间值，则搜索范围为后半部分，更新low值
            low = mid;
        }
        if (high - low == 1) {
            // 搜索范围确定至1℃内，找到温度所处范围
            t2 = high + temp_start;                     // 右边界温度值
            t1 = low - 1 + temp_start;                  // 左边界温度值
            temp = ntc_temp_get_from_range(t1, t2, res);
            break;                             // 获取到温度值，结束循环
        }
    }
    return temp;
}



/* end of file */
