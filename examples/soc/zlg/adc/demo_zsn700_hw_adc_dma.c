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
 * \snippet demo_zsn700_adc_dma.c src_zsn700_adc_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-09  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_adc_dma
 * \copydoc demo_zsn700_adc_dma.c
 */

/** [src_zsn700_adc_dma] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_zsn700_dma.h"
#include "am_zsn700_clk.h"
#include "hw/amhw_zsn700_adc.h"
#include "zsn700_periph_map.h"

#define  BUF_SIZE    16

static uint32_t __g_buf_dst[BUF_SIZE];   /**< \brief 目标端数据缓冲区 */

static uint16_t __g_adc_dat[16];         /**< \brief ADC数据缓冲区 */

static am_bool_t g_trans_done;           /**< \brief 传输完成标志 */

static int __g_adc_chan_num;             /* ADC通道数 */

/**
 * \brief DMA 中断服务程序
 */
static void dma_isr (void *p_arg , uint32_t flag)
{
    int i;

    if (flag == AM_ZSN700_DMA_INT_COMPLETE) {

        for (i = 0; i < __g_adc_chan_num; i++) {

            /* 低12位为ADC采集数据 */
            __g_adc_dat[i] = __g_buf_dst[i] & 0xfff;
        }

        /* 顺序扫描转换停止 */
        amhw_zsn700_adc_sqr_convert_stop((amhw_zsn700_adc_t *)p_arg);

        g_trans_done = AM_TRUE;
    }
}

static void __zsn700_adc_init (amhw_zsn700_adc_t *p_hw_adc,
                               int               *p_adc_chan,
                               int                chan_num)
{
    int i;

    /* 内部参考电压使能 */
    amhw_zsn700_adc_inrefvol_enable(p_hw_adc);

    /* 设置参考电压源 */
    amhw_zsn700_adc_refvol_sel(p_hw_adc, AMHW_ZSN700_ADC_REFVOL_AVCC);

    /* BGR模块使能 */
    amhw_zsn700_bgr_enable(AM_TRUE);

    /* ADC使能*/
    amhw_zsn700_adc_enable(p_hw_adc);

    /* 延时 20us，等待 ADC及 BGR模块启动完成 */
    am_udelay(20);

    /* ADC禁能 */
    amhw_zsn700_adc_disable(p_hw_adc);

    /* 使能扫描转换模式 */
    amhw_zsn700_adc_mode_sel(p_hw_adc, AMHW_ZSN700_ADC_MODE_SCAN);

    amhw_zsn700_adc_align_way_sel(p_hw_adc, AMHW_ZSN700_ADC_DATA_ALIGN_RIGHT);

    /* 设置转换周期 */
    amhw_zsn700_adc_sample_period_sel(p_hw_adc,
                                  AMHW_ZSN700_ADC_SAMPLE_PERIOD_12);

    /* 设置ADC时钟分频 */
    amhw_zsn700_adc_pclk_div_sel(p_hw_adc, AMHW_ZSN700_ADC_PCLK_DIV8);

    /* 对齐方式设置 */
    amhw_zsn700_adc_align_way_sel(p_hw_adc,
                              AMHW_ZSN700_ADC_DATA_ALIGN_RIGHT);

    /* 链接ADC顺序扫描转换通道和引脚 */
    for(i = 0; i < chan_num; i++) {
        amhw_zsn700_adc_sqr_chan_sel(p_hw_adc,
                                     i,
                                     (amhw_zsn700_adc_chan_sel_t)p_adc_chan[i]);
    }

    amhw_zsn700_adc_sqr_num_set(p_hw_adc, chan_num - 1);

    /* 顺序扫描转换DMA使能*/
    amhw_zsn700_adc_dma_sqr_enable(p_hw_adc);

    /* 清标志位 */
    amhw_zsn700_adc_flag_clr(p_hw_adc, AMHW_ZSN700_ADC_FLAG_ALL);

    /* ADC使能*/
    amhw_zsn700_adc_enable(p_hw_adc);
}

/**
 * \brief 例程入口
 */
void demo_zsn700_hw_adc_dma_entry (void    *p_hw_adc,
                                   int     *p_adc_chan,
                                   int      chan_num,
                                   uint32_t vref_mv,
                                   int      dma_chan,
                                   uint8_t  dma_src)
{
    uint32_t adc_mv = 0;    /* 采样电压 */
    uint32_t flags;
    int      i;
    static   amhw_zsn700_dma_xfer_desc_t g_desc;  /**< \brief DMA描述符 */

    amhw_zsn700_adc_t *p_adc = (amhw_zsn700_adc_t *)p_hw_adc;

    __g_adc_chan_num = chan_num;

    am_kprintf("The ADC HW dma Demo\r\n");

    __zsn700_adc_init(p_adc, p_adc_chan, chan_num);

    /* 连接DMA中断服务函数 */
    am_zsn700_dma_isr_connect(dma_chan, dma_isr, (void *)p_adc);

    /* DMA 传输配置 */
    flags = AMHW_ZSN700_DMA_CHAN_TRANSFER_MODE_BLOCK    | /* SPI只有块传输模式 */
            AMHW_ZSN700_DMA_CHAN_SIZE_32BIT             | /* 数据位宽 32bit */

            AMHW_ZSN700_DMA_CHAN_SRC_ADD_INC_ENABLE     | /* 源地址自增 */
            AMHW_ZSN700_DMA_CHAN_RELOAD_SRC_ADD_ENABLE  | /* 源地址重载*/

            AMHW_ZSN700_DMA_CHAN_DST_ADD_INC_ENABLE     | /* 目标地址自增 */
            AMHW_ZSN700_DMA_CHAN_RELOAD_DST_ADD_ENABLE  | /* 目标地址重载 */

            AMHW_ZSN700_DMA_CHAN_RELOAD_COUNTER_ENABLE  | /* 传输数据量重载 */
            AMHW_ZSN700_DMA_CHAN_INT_ERR_DISABLE        | /* 传输错误中断禁能 */
            AMHW_ZSN700_DMA_CHAN_INT_TX_CMP_ENABLE      | /* 传输完成中断使能 */
            AMHW_ZSN700_DMA_CHAN_CIRCULAR_MODE_ENABLE ;   /* 开启循环模式 */

    /* 建立通道描述符 */
    am_zsn700_dma_xfer_desc_build(&g_desc,                        /* 通道描述符 */
                                  (uint32_t)(&(p_adc->sqrresult[0])),/* 源端数据缓冲 */
                                  (uint32_t)(__g_buf_dst),        /* 目标数据缓冲 */
                                  (uint32_t)1,                    /* 传输字节数 */
                                  flags);                         /* 传输配置 */

    am_zsn700_dma_xfer_desc_chan_cfg(&g_desc,
                                     AMHW_ZSN700_DMA_PER_TO_MER, /* 外设到内存 */
                                     dma_chan);

    /* 默认通道0和通道1优先级一样（轮询）*/
    am_zsn700_dma_priority_same();

    /* 设置传输请求信号来源 */
    am_zsn700_dma_chan_src_set (dma_chan, dma_src);

    /* 设置每个DMA传输块大小为已开启的通道个数（chan_num） */
    am_zsn700_dma_block_data_size(dma_chan, chan_num);

    /* 开始通道传输 */
    am_zsn700_dma_chan_start(dma_chan);

    while(1) {

        /* 顺序扫描转换启动 */
        amhw_zsn700_adc_sqr_convert_start(p_adc);

        while(g_trans_done == AM_FALSE); /* 等待传输完成 */

        for (i = 0; i < chan_num; i++) {

            /* 转换为电压值对应的整数值 */
            adc_mv = __g_adc_dat[p_adc_chan[i]] * vref_mv / ((1UL << 12) -1);

            am_kprintf("chan: %d, Sample : %d, Vol: %d mv\r\n", i,
                                                                __g_adc_dat[i],
                                                                adc_mv);
        }

        am_kprintf("\r\n");
        am_mdelay(500);

        g_trans_done = AM_FALSE;
    }
}
/** [src_zsn700_adc_dma] */

/* end of file */
