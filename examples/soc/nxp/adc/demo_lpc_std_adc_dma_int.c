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
 * \brief ADC DMA 例程（中断方式），通过标准接口实现
 *
 * - 操作步骤：
 *   1. 短接 J12 跳线帽，使得参考电压为 2.5V；
 *   2. PIO0_7(ADC 通道 0) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc_std_adc_dma_int.c src_lpc_std_adc_dma_int
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-07-23  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_std_adc_dma_int
 * \copydoc demo_lpc_std_adc_dma_int.c
 */

/** [src_lpc_std_adc_dma_int] */
#include "ametal.h"
#include "am_adc.h"
#include "am_vdebug.h"

/*******************************************************************************
  宏定义
*******************************************************************************/


/** \brief ADC BUF1 采集完成回调参数标志 */
#define __ADC_BUF0_FLAG    0

/** \brief ADC BUF2 采集完成回调参数标志 */
#define __ADC_BUF1_FLAG    1

/** \brief 一个序列使用 2 个缓冲区描述符 */
#define __ADC_DESC_NUN     2

/** \brief ADC BUF 有 100 个数据长度（32位） */
#define __ADC_BUF_LEN      100

/** \brief 序列启动连续转换 */
#define __ADC_TRANS_COUNT  0

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

/** \brief 当前完成 AD 采样的描述符号，有效值小于 __ADC_DESC_NUN */
am_local volatile uint8_t    __g_buf_flag = 255;

/** \brief ADC 缓冲区描述符 */
am_local am_adc_buf_desc_t   __g_desc[__ADC_DESC_NUN];

/** \brief ADC 数据缓冲区 */
am_local uint16_t            __g_adc_val[__ADC_DESC_NUN][__ADC_BUF_LEN];

/**
 * \brief ADC 转换回调函数
 */
am_local void __adc_convert_cb (void *p_arg, int state)
{
    if (__g_buf_flag > __ADC_DESC_NUN && state == AM_OK) {
        __g_buf_flag = (uint32_t)p_arg;  
    } else {

        /* 处理速度与采样速度不匹配（应降低采样率） */
        AM_DBG_INFO("the speed too high\r\n");
    }
}


void demo_lpc_std_adc_dma_int_entry (am_adc_handle_t adc_handle,
                                     int             chan,
                                     uint32_t        rate)
{
    uint32_t i        = 0;
    uint32_t adc_aver = 0;      /* 采样平均值 */
  
    /* 创建描述符 */
    am_adc_mkbufdesc(&__g_desc[0],
                     &__g_adc_val[0][0],
                      __ADC_BUF_LEN,
                      __adc_convert_cb,
                      (void *)__ADC_BUF0_FLAG);

    am_adc_mkbufdesc(&__g_desc[1],
                     &__g_adc_val[1][0],
                      __ADC_BUF_LEN,
                      __adc_convert_cb,
                     (void *)__ADC_BUF1_FLAG);

    /**
     * 设置 10KHz 采样率（支持的采样率与用户数据处理运算量有关）
     * 若使用中断工作模式，不能使用过高的采样率
     * 实际需设定的采样率跟用户的运算量和采集信号有关
     */
    am_adc_rate_set(adc_handle, chan, rate);

    /* 启动 ADC 转换 */
    am_adc_start(adc_handle,
                 chan,
                &__g_desc[0],
                 __ADC_DESC_NUN,
                 __ADC_TRANS_COUNT,
                 AM_ADC_DATA_ALIGN_RIGHT,
                 NULL,
                 NULL);

    AM_FOREVER {

        if (__g_buf_flag == __ADC_BUF0_FLAG) {

            /* ADC 数据处理 */
            adc_aver = 0;
            for (i = 0; i < __ADC_BUF_LEN; i++) {

                /* 因为数据是右对齐 */
                adc_aver += __g_adc_val[__g_buf_flag][i];
            }

            /* 算出完成 AD 的描述符 buf 数据平均值 （算作对 buf 中作处理） */
            adc_aver /= __ADC_BUF_LEN;

            adc_aver = adc_aver * 2500 / ((1 << 12) -1);

            __g_buf_flag = 255;        /* 处理完数据标志清除 */

            AM_DBG_INFO("the verf0 is : %d mV \r\n", adc_aver);

        } else if (__g_buf_flag == __ADC_BUF1_FLAG) {

            /* ADC 数据处理 */
            adc_aver = 0;
            for (i = 0; i < __ADC_BUF_LEN; i++) {

                /* 因为数据是右对齐 */
                adc_aver += __g_adc_val[__g_buf_flag][i];
            }

            /* 算出完成 AD 的描述符 buf 数据平均值 （算作对 buf 中作处理） */
            adc_aver /= __ADC_BUF_LEN;
            adc_aver = adc_aver * 2500 / ((1 << 12) -1);

            __g_buf_flag = 255;        /* 处理完数据标志清除 */

            AM_DBG_INFO("the verf1 is : %d mV \r\n", adc_aver);
        }
    }
}

/** [src_lpc_std_adc_dma_int] */

/* end of file */
