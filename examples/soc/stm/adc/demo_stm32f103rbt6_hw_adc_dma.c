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
 * \snippet demo_zlg_adc_dma.c src_zlg_adc_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 19-02-27  ipk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg_adc_dma
 * \copydoc demo_zlg_adc_dma.c
 */

/** [src_zlg_adc_dma] */
#include "am_zlg237.h"
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_zlg_dma.h"
#include "am_zlg237_clk.h"
#include "hw/amhw_zlg_dma.h"
#include "hw/amhw_zlg237_adc.h"

#define  BUF_SIZE    2*4                 /**< \brief 用于设定缓冲区大小
                                          *    建议修改成adc_chan_num的2倍关系
                                          */

static uint32_t __g_buf_dst[BUF_SIZE];   /**< \brief 目标端数据缓冲区 */

static uint16_t __g_adc_dat[BUF_SIZE];   /**< \brief ADC数据缓冲区 */

static am_bool_t g_trans_done;           /**< \brief 传输完成标志 */

static amhw_zlg_dma_xfer_desc_t g_desc;  /**< \brief DMA描述符 */


/**
 * \brief DMA 中断服务程序
 */
static void dma_isr (void *p_arg , uint32_t flag)
{
    int i;

    if (flag == AM_ZLG_DMA_INT_NORMAL) {

        for (i = 0; i < BUF_SIZE; i++) {

            /* 低12位为ADC采集数据 */
            __g_adc_dat[i] = __g_buf_dst[i] & 0xfff;
        }

        g_trans_done = AM_TRUE;
    }
}

static void __zlg_adc_init (amhw_zlg237_adc_t *p_hw_adc,
                            int               *p_adc_chan,
                            int                adc_chan_num)
{
    int i;

    /* 设置ADC工作频率，72MHz的8分频 */
    amhw_zlg237_rcc_adc_div_set (2);

    /* ADC禁能 */
    amhw_zlg237_adc_disable(p_hw_adc);

    /* 设置ADC工规则通道长度 */
    if(adc_chan_num < 16) {
        amhw_zlg237_adc_regular_channel_length_set(
            p_hw_adc,
            (amhw_zlg237_adc_regular_channel_length_t)(adc_chan_num - 1));
    }

    for(i=0;i<adc_chan_num;i++) {

        /* 链接规则通道序列和ADC采样通道 */
        amhw_zlg237_adc_regular_channel_order_set(
            p_hw_adc,
           (amhw_zlg237_adc_regular_channel_order_t)(AMHW_ZLG237_ADC_REGULAR_CHAN_ORDER_1st + i),
           (amhw_zlg237_adc_channel_t)p_adc_chan[i]);

        /* 设置采样通道的采样周期 */
        amhw_zlg237_adc_smpr_set(
            p_hw_adc,
            AMHW_ZLG237_ADC_CHAN_ST55_5,
            (amhw_zlg237_adc_channel_t)p_adc_chan[i]);
    }

    /* 开启扫描模式 */
    amhw_zlg237_adc_scan_mode_enable(p_hw_adc);

    /* 连续转换 */
    amhw_zlg237_adc_cont_set(p_hw_adc, AMHW_ZLG237_ADC_CONVERSION_CONTINUOUS);

    /* 设置成独立模式 */
    amhw_zlg237_adc_dul_mode_set(p_hw_adc, AMHW_ZLG237_ADC_DUL_MODE_0);

    /* 对齐方式--右对齐 */
    amhw_zlg237_adc_data_alignment_set(p_hw_adc,AMHW_ZLG237_ADC_DATA_RIGHT);

    /* 关闭ADC16通道内部温度传感器（仅ADC1有效）*/
    amhw_zlg237_adc_tsvrefe_disable(p_hw_adc);

    /* 开启ADC的DMA功能 */
    amhw_zlg237_adc_dma_enable(p_hw_adc);
}

/**
 * \brief 例程入口
 */
void demo_zlg237_hw_adc_dma_entry (amhw_zlg237_adc_t *p_hw_adc,
                                   int               *p_adc_chan,
                                   int                adc_chan_num,
                                   uint8_t            dma_chan)
{
    uint32_t adc_mv = 0;    /* 采样电压 */
    uint32_t flags;
    int      i;

    /* 使用非默认demo测试时，请先根据具体使用的ADC通道数量修改BUF_SIZE宏定义*/

    am_kprintf("The ADC HW dma Demo\r\n");

    /* adc相关初始化配置 */
    __zlg_adc_init(p_hw_adc, p_adc_chan, adc_chan_num);

    /* ADC使能*/
    /* 在CR2寄存器中与ADON一起还有其他位被改变，则转换不被触发。这是为了防止触发错误的转换。*/
    amhw_zlg237_adc_enable(p_hw_adc);

    /* DMA 传输配置 */
    flags = AMHW_ZLG_DMA_CHAN_PRIORITY_HIGH         |  /* 中断优先级 高 */
            AMHW_ZLG_DMA_CHAN_MEM_SIZE_32BIT        |  /* 内存数据宽度 2 字节 */
            AMHW_ZLG_DMA_CHAN_PER_SIZE_32BIT        |  /* 外设数据宽度 2 字节 */
            AMHW_ZLG_DMA_CHAN_MEM_ADD_INC_ENABLE    |  /* 内存地址自增 */
            AMHW_ZLG_DMA_CHAN_PER_ADD_INC_DISABLE   |  /* 外设地址不自增 */
            AMHW_ZLG_DMA_CHAN_CIRCULAR_MODE_ENABLE  |  /* 打开循环模式 */
            AMHW_ZLG_DMA_CHAN_INT_TX_CMP_ENABLE;       /* 使能DMA传输完成中断 */

    /* 连接 DMA 中断服务函数 */
    am_zlg_dma_isr_connect(dma_chan, dma_isr, (void *)p_hw_adc);

    /* 建立通道描述符 */
    am_zlg_dma_xfer_desc_build(&g_desc,                      /* 通道描述符 */
                               (uint32_t)(&p_hw_adc->dr),    /* 源端数据缓冲区 */
                               (uint32_t)(__g_buf_dst),      /* 目标端数据缓冲区 */
                               sizeof(__g_buf_dst),          /* 传输字节数 */
                               flags);                       /* 传输配置 */

    /* DMA寄存器配置 */
    am_zlg_dma_xfer_desc_chan_cfg(&g_desc,
                                   AMHW_ZLG_DMA_PER_TO_MER, /* 外设到内存 */
                                   dma_chan);

    /* 启动 DMA 传输，马上开始传输 */
    am_zlg_dma_chan_start(dma_chan);

    /* ADC转换启动未采用外部触发，需要注意的是，此处是第二次ADC使能，本次的实际作用是开启ADC转换*/
    amhw_zlg237_adc_enable(p_hw_adc);

    while(1) {

        /* 等待传输完成 */
        while(g_trans_done == AM_FALSE);

        for (i = 0; i < adc_chan_num; i++) {

            /* 转换为电压值对应的整数值 */
            adc_mv = __g_adc_dat[i] * 3300 / ((1UL << 12) -1);

            /* 串口打印数据*/
            am_kprintf("chan: %d, Sample : %d, Vol: %d mv\r\n", p_adc_chan[i],
                                                                __g_adc_dat[i],
                                                                adc_mv);
        }

        am_kprintf("\r\n");
        am_mdelay(500);

        /* DMA传输标志失效 */
        g_trans_done = AM_FALSE;

    }
}
/** [src_zlg_adc_dma] */

/* end of file */
