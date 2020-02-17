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
 * \snippet demo_mm32_adc_dma.c src_mm32_adc_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 19-02-27  ipk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32_adc_dma
 * \copydoc demo_mm32_adc_dma.c
 */

/** [src_mm32_adc_dma] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_mm32_dma.h"
#include "hw/amhw_mm32_dma.h"
#include "hw/amhw_mm32_adc.h"

#define  BUF_SIZE    10

static uint32_t __g_buf_dst[BUF_SIZE];   /**< \brief 目标端数据缓冲区 */

static uint16_t __g_adc_dat[8];          /**< \brief ADC数据缓冲区 */

static am_bool_t g_trans_done;           /**< \brief 传输完成标志 */

static amhw_mm32_dma_xfer_desc_t g_desc;  /**< \brief DMA描述符 */

static int *__gp_adc_chan = NULL;        /* 指向ADC通道列表 */

static int __g_adc_chan_num;             /* ADC通道数 */

/**
 * \brief DMA 中断服务程序
 */
static void dma_isr (void *p_arg , uint32_t flag)
{
    int i;
    int j;
    int cur_chan;

    if (flag == AM_MM32_DMA_INT_NORMAL) {

        for (i = 0; i < BUF_SIZE; i++) {

            /* 获取当前请求DMA的ADC通道 */
            cur_chan = (__g_buf_dst[i] >> 16) & 0x0f;

            /* 检查ADC通道是否合法 */
            for (j = 0; j < __g_adc_chan_num; j++) {
                if (cur_chan == __gp_adc_chan[j]) {
                    break;
                }
                if (j == __g_adc_chan_num - 1) {
                    return;
                }
            }

            /* 低12位为ADC采集数据 */
            __g_adc_dat[cur_chan] = __g_buf_dst[i] & 0xfff;
        }

        g_trans_done = AM_TRUE;
    }
}

static void __mm32_adc_init (amhw_mm32_adc_t *p_hw_adc,
                            int            *p_adc_chan,
                            int             adc_chan_num)
{
    int i;

    __gp_adc_chan    = p_adc_chan;
    __g_adc_chan_num = adc_chan_num;

    amhw_mm32_adc_data_resolution(p_hw_adc, AMHW_MM32_ADC_DATA_VALID_12BIT);
    amhw_mm32_adc_prescaler_val(p_hw_adc, 4);
    amhw_mm32_adc_chan_sample_time(p_hw_adc, AMHW_MM32_ADC_CHAN_ST55_5);

    for (i = 0; i < adc_chan_num; i++) {
        amhw_mm32_adc_input_chan_enable(p_hw_adc, p_adc_chan[i]);
    }

    amhw_mm32_adc_cgf_reg_set(p_hw_adc, AMHW_MM32_ADC_MODULE_EN_MASK);

    /* 使能ADC DMA请求 */
    amhw_mm32_adc_ctrl_reg_set(p_hw_adc,
                              AMHW_MM32_ADC_DMA_EN_MASK);

    amhw_mm32_adc_data_scan_set(p_hw_adc, AMHW_MM32_ADC_CONTINUE_SCAN);

    amhw_mm32_adc_data_transfer_set(p_hw_adc, AMHW_MM32_ADC_DATA_TRANSFER_START);
}

/**
 * \brief 例程入口
 */
void demo_mm32_hw_adc_dma_entry (amhw_mm32_adc_t *p_hw_adc,
                                int            *p_adc_chan,
                                int             adc_chan_num,
                                uint8_t         dma_chan)
{
    uint32_t adc_mv = 0;    /* 采样电压 */
    uint32_t flags;
    int      i;

    am_kprintf("The ADC HW dma Demo\r\n");

    __mm32_adc_init(p_hw_adc, p_adc_chan, adc_chan_num);

    /* 连接 DMA 中断服务函数 */
    am_mm32_dma_isr_connect(dma_chan, dma_isr, (void *)p_hw_adc);

    /* DMA 传输配置 */
    flags = AMHW_MM32_DMA_CHAN_PRIORITY_HIGH         |  /* 中断优先级 高 */
            AMHW_MM32_DMA_CHAN_MEM_SIZE_32BIT        |  /* 内存数据宽度 2 字节 */
            AMHW_MM32_DMA_CHAN_PER_SIZE_32BIT        |  /* 外设数据宽度 2 字节 */
            AMHW_MM32_DMA_CHAN_MEM_ADD_INC_ENABLE    |  /* 内存地址自增 */
            AMHW_MM32_DMA_CHAN_PER_ADD_INC_DISABLE   |  /* 外设地址不自增 */
            AMHW_MM32_DMA_CHAN_CIRCULAR_MODE_ENABLE  |  /* 打开循环模式 */
            AMHW_MM32_DMA_CHAN_INT_TX_CMP_ENABLE;       /* 使能DMA传输完成中断 */

        /* 建立通道描述符 */
    am_mm32_dma_xfer_desc_build(&g_desc,                      /* 通道描述符 */
                               (uint32_t)(&p_hw_adc->addata), /* 源端数据缓冲区 */
                               (uint32_t)(__g_buf_dst),    /* 目标端数据缓冲区 */
                               sizeof(__g_buf_dst),         /* 传输字节数 */
                               flags);                                     /* 传输配置 */

    am_mm32_dma_xfer_desc_chan_cfg(&g_desc,
                                   AMHW_MM32_DMA_PER_TO_MER, /* 外设到内存 */
                                   dma_chan);

    /* 启动 DMA 传输，马上开始传输 */
    am_mm32_dma_chan_start(dma_chan);

    while(1) {

        while(g_trans_done == AM_FALSE); /* 等待传输完成 */

        for (i = 0; i < adc_chan_num; i++) {

            /* 转换为电压值对应的整数值 */
            adc_mv = __g_adc_dat[p_adc_chan[i]] * 3300 / ((1UL << 12) -1);

            am_kprintf("chan: %d, Sample : %d, Vol: %d mv\r\n", p_adc_chan[i],
                                                                __g_adc_dat[p_adc_chan[i]],
                                                                adc_mv);
        }

        am_kprintf("\r\n");
        am_mdelay(500);

        g_trans_done = AM_FALSE;
    }
}
/** [src_mm32_adc_dma] */

/* end of file */
