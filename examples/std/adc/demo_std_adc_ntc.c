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
 * \brief ADC 利用板子的热敏电阻(NTC)，推算出温度例程，通过标准接口实现
 *
 * 该  demo 适用于典型的 NTC 电路：
 *
 *                -------------- Vref
 *                |
 *                |
 *               ---
 *               | |
 *          NTC  | |
 *               | |
 *               ---
 *                |
 *                |-------------- ADC 输入端（ADC将采集该点的电压）
 *                |
 *               ---
 *               | |
 *      分压电阻 | |
 *               | |
 *               ---
 *                |
 *                |
 *                ---------------- GND
 *
 * - 操作步骤：
 *   1. 将ADC输入端连接至相应的ADC输入引脚
 *
 * - 实验现象：
 *   1. 调试串口输出温度值；
 *
 * \par 源代码
 * \snippet demo_std_adc_ntc.c src_std_adc_ntc
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-10  hgo, modified
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_adc_ntc
 * \copydoc demo_std_adc_ntc.c
 */

/** [src_std_adc_ntc] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_adc.h"

/*******************************************************************************
 *  全局变量定义
*******************************************************************************/

/**
 * \brief 阻值温度表
 *
 * 列表中对应的温度值依次是-20~87℃，步进1℃。例如，-20度对应的数值是 70988， 表示
 * 70988欧姆对应的温度即为 -20度。
 *
 * 25度（室温）对应的阻值为 100K， 即对应的 NTC 为 100K NTC。NTC以常温阻值作为标称值。
 *
 * 不同 NTC 电阻，表格对应的数据可能不同，
 */
static const uint32_t g_temp_res_val[] = {

    /* -20℃ ~  -9℃ */
    70988,67491,64191,61077,58135,55356,52730,50246,47896,45672,43567,41572,

    /*  -8℃ ~   3℃ */
    39682,37891,36192,34580,33050,31597,30218,28907,27662,26477,25351,24279,

    /*   4℃ ~  15℃ */
    23259,22288,21363,20482,19642,18842,18078,17350,16656,15993,15360,14756,

    /*  16℃ ~  27℃ */
    14178,13627,13100,12596,12114,11653,11212,10790,10387,10000, 9630, 9275,

    /*  28℃ ~  39℃ */
    8936, 8610, 8298, 7999, 7712, 7437, 7173, 6920, 6677, 6444, 6220, 6005,

    /*  40℃ ~  51℃ */
    5799, 5600, 5410, 5227, 5050, 4881, 4718, 4562, 4411, 4267, 4127, 3995,

    /*  52℃ ~  63℃ */
    3867, 3744, 3626, 3512, 3402, 3296, 3194, 3096, 3002, 2910, 2822, 2737,

    /*  64℃ ~  75℃ */
    2655, 2576, 2500, 2426, 2355, 2286, 2220, 2156, 2094, 2034, 1976, 1920,

    /*  76℃ ~  87℃ */
    1866, 1813, 1763, 1714, 1666, 1621, 1576, 1533, 1492, 1451, 1412, 1375,
};

static const int res_val_num = sizeof(g_temp_res_val) / sizeof(g_temp_res_val[0]);
static const int temp_start  = -20;    /* 表格中起始温度为-20℃ */

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
 * \brief 温度误差在1℃之内通过算法取最合理值
 *
 * \param[in] t1 : 左边界温度
 * \param[in] t1 : 右边界温度
 *
 * \return temp : 温度值
 */
static int16_t __ntc_temp_get_from_range (int t1,int t2, uint32_t res)
{
    int r1 = g_temp_res_val[t1 - temp_start];  /* 得到温度1对应的阻值 */
    int r2 = g_temp_res_val[t2 - temp_start];  /* 得到温度2对应的阻值 */

    int r  = res;
    int temp;

    /* 为避免小数计算扩大了256倍（左移8位） */
    temp =  (((t2 - t1) * (r - r1)) << 8) / (r2 - r1) + (t1 << 8);

    return temp;
}

/**
 * \brief 二分法查表并返回相应的温度值
 *
 * \param[in]  res : 电阻阻值
 *
 * \return temp : 温度值
 */
static int16_t __ntc_res_to_temp (uint32_t res)
{
    int16_t   temp;
    int low, high, mid;     /* 二分法查表的三个下标 */
    int t1,t2;              /* 区间的两个温度值 t1 ~ t2 */

    low  = 0;               /* 初始表上边界为第一个 */
    high = res_val_num - 1; /* 初始表下边界为最后一个 */

    while(1) {

        /* 右移一位，等效于 (low + high)/2 */
        mid = (low + high) >> 1;

        /* 如果恰好相等 */
        if (res == g_temp_res_val[mid]) {
            /* 整数温度值，扩大256倍 */
            temp = (mid - 20) << 8;
            break;
        }

        /* 阻值大于中间值，则搜索范围为前半部分，更新high值 */
        if (res > g_temp_res_val[mid]) {
            high = mid;
        } else {

            /* 阻值小于中间值，则搜索范围为后半部分，更新low值 */
            low = mid;
        }

        /* 搜索范围确定至1℃内，找到温度所处范围 */
        if (high - low == 1) {

            /* 右边界温度值 */
            t2 = high + temp_start;

            /* 左边界温度值 */
            t1 = low - 1 + temp_start;

            /* 计算最佳温度值 */
            temp = __ntc_temp_get_from_range(t1, t2, res);

            break;
        }
    }

    return temp;
}

/**
 * \brief 例程入口
 */
void demo_std_adc_ntc_entry (am_adc_handle_t handle,
                             int             chan,
                             uint32_t        res_fixed)
{
    int16_t         temp;      /* 温度值 */
    uint32_t        res;       /* 热敏电阻阻值 */
    uint32_t        adc_bits;
    uint32_t        code;

    /* 设置采样率为 6000 */
    am_adc_rate_set(handle, chan, 6000);

    /* 获取ADC参考电压，单位：mV */
    adc_bits = am_adc_bits_get(handle,chan);

    while (1) {

        /**
         * 根据电阻分压电路可得:
         *    采样电压 =  vref * (res_fixed / (res_ntc + res_fixed))
         *
         * 假定 ADC位数为 N，采样得到的值为 CODE， 则采样电压为：
         *
         * (CODE / (1 << N)) * Vref
         *
         * 代入上面公式可得：
         *
         * (CODE / (1 << N)) * Vref =  vref * (res_fixed / (res_ntc + res_fixed))
         *
         * 同时消去 vref ：
         *
         * (CODE / (1 << N)) = (res_fixed / (res_ntc + res_fixed))
         *
         * 由此可得：
         *
         * res_ntc = (((1 << N) - CODE) * res_fixed) / CODE
         *
         */
        code = __adc_code_get(handle, chan);

        res  = (((1 << adc_bits) - code) * res_fixed) / code;

        temp = __ntc_res_to_temp(res);      /* 将电阻转换成温度参数 */

        AM_DBG_INFO("the temperature is %d.%2d ℃\r\n",
                    temp >> 8,                   /* 温度扩大了256倍  */
                    (temp & 0xFF) * 100 / 256);  /* 打印两位小数         */

        am_mdelay(500);
    }
}
/** [src_std_adc_ntc] */

/* end of file */
