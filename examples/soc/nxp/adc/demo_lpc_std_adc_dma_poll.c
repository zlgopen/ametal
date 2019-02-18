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
 * \brief ADC DMA 例程（轮询方式），通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc_std_adc_dma_poll.c src_lpc_std_adc_dma_poll
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-09-10  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc_std_adc_dma_poll
 * \copydoc demo_lpc_std_adc_dma_poll.c
 */

/** [src_lpc_std_adc_dma_poll] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_adc.h"
#include "demo_nxp_entries.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/** \brief ADC 数据缓冲区长度 */
#define __ADC_BUF_LEN      100

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

/** \brief ADC 数据缓冲区 */
am_local uint16_t __g_adc_val_buf[__ADC_BUF_LEN];



void demo_lpc_std_adc_dma_poll_entry(am_adc_handle_t adc_handle,
                                     int             chan,
                                     uint32_t        rate)
{
    uint8_t i = 0;
/* 设置 10KHz 采样率（支持的采样率与用户数据处理运算量有关） */
    am_adc_rate_set(adc_handle, chan, rate);

    AM_FOREVER {

        /* 读取 ADC 转换的电压值 */
        am_adc_read(adc_handle,
                    chan,
                    __g_adc_val_buf,
                    __ADC_BUF_LEN);

        for (i = 0; i < __ADC_BUF_LEN; i++) {
            AM_DBG_INFO("%dmV    \n", AM_ADC_VAL_TO_MV(adc_handle,
                                                     chan,
                                                     __g_adc_val_buf[i]));
        }

        AM_DBG_INFO("\r\n");
    }
}


/** [src_lpc_std_adc_dma_poll] */

/* end of file */

