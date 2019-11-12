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
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. DMA 传输成功，打印 "transfer success"；
 *   2. DMA 传输出错，打印 "transfer failure"。
 *
 * \par 源代码
 * \snippet demo_hc32_drv_dma_m2m.c src_hc32_drv_dma_m2m
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32_drv_dma_m2m
 * \copydoc demo_hc32_drv_dma_m2m.c
 */

/** [src_hc32_drv_dma_m2m] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_hc32_dma.h"
#include "hc32_dma_chan.h"

#include "hw/amhw_hc32_dma.h"

static uint8_t g_buf_src[50];           /**< \brief 源端数据缓冲区 */
static uint8_t g_buf_dst[50];           /**< \brief 目标端数据缓冲区 */

static volatile am_bool_t g_trans_done; /**< \brief 传输完成标志 */

static amhw_hc32_dma_xfer_desc_t g_desc; /**< \brief 描述符 */

/**
 * \brief DMA 中断服务程序
 */
static void dma_isr (void *p_arg , uint32_t flag)
{
    if (flag == AM_HC32_DMA_INT_COMPLETE) {
        g_trans_done = AM_TRUE;
    }
}

/**
 * \brief DMA M2M 模式测试程序
 *
 * \retval  AM_OK    传输成功
 * \retval  AM_ERROR 传输失败
 */
static int dma_m2m_test (uint8_t dma_chan)
{
    uint32_t flags;
    uint32_t i;

    for (i = 0; i < 50; i++) {
        g_buf_src[i] = i;
        g_buf_dst[i] = 0;
    }

    /* 连接 DMA 中断服务函数 */
    am_hc32_dma_isr_connect(dma_chan, dma_isr, (void *)0);

    /* DMA 传输配置 */
    flags = AMHW_HC32_DMA_CHAN_TRANSFER_MODE_BURST   |    /* 突发传输模式 */
            AMHW_HC32_DMA_CHAN_SIZE_8BIT             |    /* 数据位宽 8bit */
            AMHW_HC32_DMA_CHAN_SRC_ADD_INC_ENABLE    |    /* 源地址自增 */
            AMHW_HC32_DMA_CHAN_DST_ADD_INC_ENABLE    |    /* 目标地址自增 */
            AMHW_HC32_DMA_CHAN_RELOAD_COUNTER_ENABLE |    /* 重载传输数据量 */
            AMHW_HC32_DMA_CHAN_RELOAD_SRC_ADD_ENABLE |    /* 重载源地址 */
            AMHW_HC32_DMA_CHAN_RELOAD_DST_ADD_ENABLE |    /* 重载目标地址 */
            AMHW_HC32_DMA_CHAN_INT_ERR_DISABLE       |    /* 错误中断禁能 */
            AMHW_HC32_DMA_CHAN_INT_TX_CMP_ENABLE     |    /* 传输完成中断使能 */
            AMHW_HC32_DMA_CHAN_CIRCULAR_MODE_ENABLE ;     /* 循环工作模式 */

    /* 建立通道描述符 */
    am_hc32_dma_xfer_desc_build(&g_desc,             /* 通道描述符 */
                                (uint32_t)(g_buf_src), /* 源端数据缓冲区 */
                                (uint32_t)(g_buf_dst), /* 目标端数据缓冲区 */
                                50,                    /* 传输字节数 */
                                flags);                /* 传输配置 */

    /* 启动 DMA 传输，马上开始传输 */
    if (am_hc32_dma_xfer_desc_chan_cfg(&g_desc,
                                       AMHW_HC32_DMA_MER_TO_MER, /* 内存到内存 */
                                       dma_chan) == AM_ERROR) {
        return AM_ERROR;
    } else {

        /* 默认通道0和通道1优先级一样（轮询）*/
        am_hc32_dma_priority_same();

        /* 设置传输请求信号来源为软件请求 */
        am_hc32_dma_chan_src_set (dma_chan, HC32_DMA_SRC_TYPE_SOFT);

        /* 设置每个DMA传输块大小为已开启的通道个数（1） */
        am_hc32_dma_block_data_size(dma_chan, 1);

        /* 开始通道传输 */
        am_hc32_dma_chan_start(dma_chan);
    }

    /* 软件请求触发DMA传输 */
    am_hc32_dma_chan_soft_ask_start(dma_chan);

    while(g_trans_done == AM_FALSE); /* 等待传输完成 */

    for (i = 0; i < 50; i++) {
        if (g_buf_src[i] != g_buf_dst[i]) {
            return AM_ERROR;
        }
    }
    return AM_OK;
}

/**
 * \brief 例程入口
 */
void demo_hc32_drv_dma_m2m_entry (uint32_t dma_chan)
{
    if (dma_m2m_test(dma_chan) == AM_OK){
        am_kprintf("transfer success\n");
    } else {
        am_kprintf("transfer failure\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32_drv_dma_m2m] */

/* end of file */
