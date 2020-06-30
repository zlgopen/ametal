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
 * \brief ADC多通道通过DMA传输，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. 对应ADC通道打印电压值。
 *
 * \par 源代码
 * \snippet demo_hc32f460_adc_dma.c src_hc32f460_adc_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 20-04-22  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_adc_dma
 * \copydoc demo_hc32f460_adc_dma.c
 */

/** [src_hc32f460_adc_dma] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "am_hc32f460_dma.h"
#include "am_hc32f460_clk.h"
#include "hw/amhw_hc32f460_adc.h"

#define  BUF_SIZE    16

static uint16_t __g_buf_dst[BUF_SIZE];   /**< \brief 目标端数据缓冲区 */

static uint16_t __g_adc_dat[16];         /**< \brief ADC数据缓冲区 */

static volatile am_bool_t g_trans_done;           /**< \brief 传输完成标志 */

#define __ADC_CHAN_NUM_MAX    17         /* ADC最大通道数 */
static int __g_adc_chan_num;             /* ADC通道数 */
static amhw_adc_chmux_t   __adc_chmux[17];

/**
 * \brief DMA 中断服务程序
 */
static void dma_isr (void *p_arg , uint32_t flag)
{
    int i;

    amhw_hc32f460_adc_t *p_hw_adc = (amhw_hc32f460_adc_t *)p_arg;

    amhw_hc32f460_adc_int_flag_clr (p_hw_adc, AMHW_HC32F460_ADC_INT_EOCAF);

    if (flag == AM_HC32F460_DMA_INT_COMPLETE) {

        for (i = 0; i < __g_adc_chan_num; i++) {

            /* 低12位为ADC采集数据 */
            __g_adc_dat[i] = __g_buf_dst[__adc_chmux[i].chan] & 0xfff;
        }


        /* 顺序扫描转换停止 */
        amhw_hc32f460_adc_convert_stop(p_hw_adc);

        g_trans_done = AM_TRUE;
    }
}

static void __hc32f460_adc_init (amhw_hc32f460_adc_t *p_hw_adc,
                                 amhw_adc_chmux_t    *p_adc_chan,
                                 int                  chan_num)
{
    uint8_t i = 0;
    uint8_t j = 0;

    /* 停止ADC转换 */
    amhw_hc32f460_adc_convert_stop(p_hw_adc);

    /* 转换模式为序列A单次转换 */
    amhw_hc32f460_adc_mode_set(p_hw_adc, AMHW_HC32F460_ADC_SEQ_A_ONCE);

    /* 设置数据对齐方式为右对齐 */
    amhw_hc32f460_adc_data_align_set(p_hw_adc,
                                     AMHW_HC32F460_ADC_DATA_ALIGN_FORMAT_RIGHT);

    /* 设置转换周期 */
    for(j = 0; j < chan_num; j++ ){
        amhw_hc32f460_adc_sampling_period_set(p_hw_adc, p_adc_chan[j].chan, 10);
    }
    /* 链接ADC顺序扫描转换通道和引脚 */
    for(i = 0; i < chan_num; i++) {
        __adc_chmux[i].chan = p_adc_chan[i].chan;
        __adc_chmux[i].type =  p_adc_chan[i].type;
        amhw_hc32f460_adc_channel_mux_set(p_hw_adc, p_adc_chan[i].chan,p_adc_chan[i].type);
        amhw_hc32f460_adc_seq_a_channel_sel0(p_hw_adc, p_adc_chan[i].chan);
    }

    amhw_hc32f460_adc_int_flag_clr(p_hw_adc, AMHW_HC32F460_ADC_INT_EOCAF);

    /* 禁止转换完成中断 */
    amhw_hc32f460_adc_int_disable(p_hw_adc, AMHW_HC32F460_ADC_INT_EOCAF);
    amhw_hc32f460_adc_int_disable(p_hw_adc, AMHW_HC32F460_ADC_INT_EOCBF);
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_hw_adc_dma_entry (void                  *p_hw_adc,
                                     amhw_adc_chmux_t      *p_adc_chan,
                                     am_hc32f460_dma_dev_t *p_dma_dev,
                                     int                    chan_num,
                                     uint32_t               vref_mv,
                                     int                    dma_chan)
{
    uint32_t adc_mv = 0;    /* 采样电压 */
    uint32_t flags;
    int      i;
    static   amhw_hc32f460_dma_xfer_desc_t g_desc;  /**< \brief DMA描述符 */

    amhw_hc32f460_adc_t *p_adc = (amhw_hc32f460_adc_t *)p_hw_adc;

    __g_adc_chan_num = chan_num;

    am_kprintf("The ADC HW dma Demo\r\n");

    __hc32f460_adc_init(p_adc, p_adc_chan, chan_num);

    /* 连接DMA中断服务函数 */
    am_hc32f460_dma_isr_connect(p_dma_dev, dma_chan, dma_isr, (void *)p_adc);

    /* DMA 传输配置 */
     flags = AMHW_HC32F460_DMA_CHAN_CFG_INT_ENABLE            |  /* 通道中断使能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SIZE_16BIT            |  /* 数据宽度 2 字节 */
             AMHW_HC32F460_DMA_CHAN_CFG_LLP_DISABLE           |  /* 连锁传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_DISABLE |  /* 目标地址不连续传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_DISABLE |  /* 源地址不连续传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_DISABLE      |  /* 目标重复传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_DISABLE      |  /* 源重复传输禁能 */
             AMHW_HC32F460_DMA_CHAN_DST_ADD_INCREASING        |  /* 目标地址固定 */
             AMHW_HC32F460_DMA_CHAN_SRC_ADD_INCREASING;          /* 源地址自增 */

    /* 建立通道描述符 */
    am_hc32f460_dma_xfer_desc_build(&g_desc,                    /* 通道描述符 */
                                    (uint32_t)(&(p_adc->DR0)),  /* 源端数据缓冲 */
                                    (uint32_t)(__g_buf_dst),    /* 目标数据缓冲 */
                                    (uint32_t)34,               /* 传输字节数 */
                                    flags);                     /* 传输配置 */

    /* 设置每个DMA传输块大小为ADC最大通道个数（chan_num） */
    am_hc32f460_dma_block_data_size(p_dma_dev, dma_chan, 17);

    am_hc32f460_dma_xfer_desc_chan_cfg(p_dma_dev,
                                       &g_desc,
                                       AMHW_HC32F460_DMA_PER_TO_MER, /* 外设到内存 */
                                       dma_chan);


    /* 设置传输请求信号来源为ADC1序列A转换完成事件 */
    am_hc32f460_dma_chan_src_set(p_dma_dev, dma_chan, EVT_ADC1_EOCA);


    while(1) {

        am_hc32f460_dma_xfer_desc_chan_cfg(p_dma_dev,
                                           &g_desc,
                                           AMHW_HC32F460_DMA_PER_TO_MER, /* 外设到内存 */
                                           dma_chan);

        /* 开始通道传输 */
        am_hc32f460_dma_chan_start(p_dma_dev, dma_chan);

        /* 扫描转换启动 */
        amhw_hc32f460_adc_convert_start(p_adc);

        while(g_trans_done == AM_FALSE); /* 等待传输完成 */

        am_hc32f460_dma_chan_stop(p_dma_dev, dma_chan);

        for (i = 0; i < chan_num; i++) {

            /* 转换为电压值对应的整数值 */
            adc_mv = __g_adc_dat[p_adc_chan[i].chan] * vref_mv / ((1UL << 12) -1);

            am_kprintf("chan: %d, Sample : %d, Vol: %d mv\r\n", i,
                                                                __g_adc_dat[i],
                                                                adc_mv);
        }

        am_kprintf("\r\n");
        am_mdelay(500);

        g_trans_done = AM_FALSE;
    }
}
/** [src_hc32f460_adc_dma] */

/* end of file */
