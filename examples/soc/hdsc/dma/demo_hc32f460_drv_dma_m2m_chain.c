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
 * \brief DMA 内存到内存例程,采用连锁传输方式，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. DMA 传输成功，打印 "transfer success"；
 *   2. DMA 传输出错，打印 "transfer failure"。
 *
 * \par 源代码
 * \snippet demo_hc32f460_drv_dma_m2m.c src_hc32f460_drv_dma_m2m
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-04  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_drv_dma_m2m
 * \copydoc demo_hc32f460_drv_dma_m2m.c
 */

/** [src_hc32f460_drv_dma_m2m] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_clk.h"
#include "am_hc32f460_dma.h"
#include "hw/amhw_hc32f460_dma.h"
#include "hc32f460_intctrl.h"
#include "hc32f460_clk.h"
#include "hc32f460_inum.h"

static uint8_t g_buf_src1[32];           /**< \brief 源端数据缓冲区1 */
static uint8_t g_buf_dst1[32];           /**< \brief 目标端数据缓冲区1 */
static uint8_t g_buf_src2[32];           /**< \brief 源端数据缓冲区2 */
static uint8_t g_buf_dst2[32];           /**< \brief 目标端数据缓冲区2 */
static uint8_t g_buf_src3[32];           /**< \brief 源端数据缓冲区3 */
static uint8_t g_buf_dst3[32];           /**< \brief 目标端数据缓冲区4 */

static volatile am_bool_t g_trans_done; /**< \brief 传输完成标志 */

static amhw_hc32f460_dma_chain_xfer_desc_t g_desc1; /**< \brief 描述符1 */
static amhw_hc32f460_dma_chain_xfer_desc_t g_desc2; /**< \brief 描述符2 */
static amhw_hc32f460_dma_chain_xfer_desc_t g_desc3; /**< \brief 描述符3 */
/**
 * \brief DMA 中断服务程序
 */
static void dma_isr (void *p_arg , uint32_t flag)
{
    if (flag == AM_HC32F460_DMA_INT_COMPLETE) {
        am_hc32f460_dma_soft_trig();
        g_trans_done = AM_TRUE;
    } else if (flag == AM_HC32F460_DMA_INT_BLK_COMPLETE) {
        am_hc32f460_dma_soft_trig();
    }
}

/**
 * \brief DMA M2M 模式测试程序
 *
 * \retval  AM_OK    传输成功
 * \retval  AM_ERROR 传输失败
 */
static int dma_m2m_chain_test (am_hc32f460_dma_dev_t *p_dev, uint8_t dma_chan)
{
    uint32_t flags1, flags2, flags3;
    uint32_t i;

    for (i = 0; i < 32; i++) {
        g_buf_src1[i] = i;
        g_buf_src2[i] = i + 32;
        g_buf_src3[i] = i + 64;
        g_buf_dst1[i] = 0;
        g_buf_dst2[i] = 0;
        g_buf_dst3[i] = 0;
    }

    /* 连接 DMA 中断服务函数 */
    am_hc32f460_dma_isr_connect(p_dev, dma_chan, dma_isr, (void *)0);

    /* DMA 传输配置 */
    flags1 = AMHW_HC32F460_DMA_CHAN_CFG_INT_DISABLE           |  /* 通道中断禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SIZE_8BIT             |  /* 数据宽度 1 字节 */
             AMHW_HC32F460_DMA_CHAN_CFG_LLPRUN_ENABLE         |  /* 连锁传输模式下，新描述符载入后，立即开始传输 */
             AMHW_HC32F460_DMA_CHAN_CFG_LLP_ENABLE            |  /* 连锁传输使能 */
             AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_DISABLE |  /* 目标地址不连续传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_DISABLE |  /* 源地址不连续传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_DISABLE      |  /* 目标重复传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_DISABLE      |  /* 源重复传输禁能 */
             AMHW_HC32F460_DMA_CHAN_DST_ADD_INCREASING        |  /* 目标地址自增 */
             AMHW_HC32F460_DMA_CHAN_SRC_ADD_INCREASING;          /* 源地址自增 */



    /* 建立第一个描述符 */
    am_hc32f460_dma_chain_xfer_desc_build(&g_desc1,
                                          (uint32_t)&g_buf_src1,
                                          (uint32_t)&g_buf_dst1,
                                          1,
                                          32,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          (uint32_t)&g_desc2,
                                          flags1);

    /* DMA 传输配置 */
    flags2 = AMHW_HC32F460_DMA_CHAN_CFG_INT_DISABLE           |  /* 通道中断禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SIZE_8BIT             |  /* 数据宽度 1 字节 */
             AMHW_HC32F460_DMA_CHAN_CFG_LLPRUN_ENABLE         |  /* 连锁传输模式下，新描述符载入后，立即开始传输 */
             AMHW_HC32F460_DMA_CHAN_CFG_LLP_ENABLE            |  /* 连锁传输使能 */
             AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_DISABLE |  /* 目标地址不连续传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_DISABLE |  /* 源地址不连续传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_DISABLE      |  /* 目标重复传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_DISABLE      |  /* 源重复传输禁能 */
             AMHW_HC32F460_DMA_CHAN_DST_ADD_INCREASING        |  /* 目标地址自增 */
             AMHW_HC32F460_DMA_CHAN_SRC_ADD_INCREASING;          /* 源地址自增 */



    /* 建立第二个描述符 */
    am_hc32f460_dma_chain_xfer_desc_build(&g_desc2,
                                          (uint32_t)&g_buf_src2,
                                          (uint32_t)&g_buf_dst2,
                                          1,
                                          32,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          (uint32_t)&g_desc3,
                                          flags2);

    /* DMA 传输配置 */
    flags3 = AMHW_HC32F460_DMA_CHAN_CFG_INT_DISABLE           |  /* 通道中断使能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SIZE_8BIT             |  /* 数据宽度 1 字节 */
             AMHW_HC32F460_DMA_CHAN_CFG_LLPRUN_ENABLE         |  /* 连锁传输模式下，新描述符载入后，立即开始传输 */
             AMHW_HC32F460_DMA_CHAN_CFG_LLP_DISABLE           |  /* 连锁传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_DISABLE |  /* 目标地址不连续传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_DISABLE |  /* 源地址不连续传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_DISABLE      |  /* 目标重复传输禁能 */
             AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_DISABLE      |  /* 源重复传输禁能 */
             AMHW_HC32F460_DMA_CHAN_DST_ADD_INCREASING        |  /* 目标地址自增 */
             AMHW_HC32F460_DMA_CHAN_SRC_ADD_INCREASING;          /* 源地址自增 */



    /* 建立第三个描述符 */
    am_hc32f460_dma_chain_xfer_desc_build(&g_desc3,
                                          (uint32_t)&g_buf_src3,
                                          (uint32_t)&g_buf_dst3,
                                          1,
                                          32,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          (uint32_t)flags3);



    /* 启动 DMA 传输，马上开始传输 */
    if (am_hc32f460_dma_chain_xfer_desc_chan_cfg(p_dev,
                                                &g_desc1,
                                                 AMHW_HC32F460_DMA_MER_TO_MER, /* 内存到内存 */
                                                 dma_chan) == AM_ERROR) {
        return AM_ERROR;
    } else {
        /*外设电路触发功能使能*/
        am_clk_enable(CLK_PTDIS);

        amhw_hc32f460_intc_int_vssel_bits_set(137, (0x1ul << 31ul));

        am_mdelay(500);

        /* 屏蔽块中断 */
        am_hc32f460_dma_chan_int_mask_set(p_dev, dma_chan, AM_HC32F460_DMA_INT_BLK_COMPLETE);

        /* 设置触发源为内部事件触发 */
        am_hc32f460_dma_chan_src_set(p_dev, dma_chan, EVT_AOS_STRG);

        am_hc32f460_dma_chan_start(p_dev, dma_chan);
    }

    /* 软件使能内部触发事件触发DMA */
    am_hc32f460_dma_soft_trig();

    while(g_trans_done == AM_FALSE); /* 等待传输完成 */

    for (i = 0; i < 32; i++) {
        if (g_buf_src1[i] != g_buf_dst1[i]) {
            return AM_ERROR;
        }
    }

    for (i = 0; i < 32; i++) {
        if (g_buf_src2[i] != g_buf_dst2[i]) {
            return AM_ERROR;
        }
    }

    for (i = 0; i < 32; i++) {
        if (g_buf_src3[i] != g_buf_dst3[i]) {
            return AM_ERROR;
        }
    }
    return AM_OK;
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_drv_dma_m2m_chain_entry (am_hc32f460_dma_dev_t *p_dev, uint32_t dma_chan)
{
    if (dma_m2m_chain_test(p_dev, dma_chan) == AM_OK){
        am_kprintf("DMA chain transfer success !\n");
    } else {
        am_kprintf("DMA chain transfer failure !\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32f460_drv_dma_m2m] */

/* end of file */
