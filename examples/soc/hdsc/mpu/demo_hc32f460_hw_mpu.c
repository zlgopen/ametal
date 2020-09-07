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
 * \brief MPU例程，通过硬件层接口实现
 *
 * - 实验现象：
 *   1. 使能前MPU前，DMA正常读写内存，串口打印"dma test success";
 *   2. 配置MPU，设置DMA源地址内存禁止读，DMA无法读取数据，传输结果全为0，串口打印"mpu test success";
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_mpu.c src_hc32f460_hw_mpu
 *
 * \internal
 * \par Modification History
 * - 1.00 20-05-14  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_mpu
 * \copydoc demo_hc32f460_hw_mpu.c
 */

/** [src_hc32f460_hw_mpu] */

#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_clk.h"
#include "hw/amhw_hc32f460_mpu.h"
#include "am_hc32f460_dma.h"
#include "hw/amhw_hc32f460_dma.h"
#include "hc32f460_intctrl.h"
#include "hc32f460_clk.h"
#include "string.h"


/* 区域大小 */
#define __REGION_SIZE    32

/* 区域编号 */
#define __REGION_NUM     AMHW_HC32F460_MPU_REGION_NUM0

/* 数据buffer，起始地址必须以size个字节对齐 */
static uint8_t __attribute__((aligned(__REGION_SIZE))) g_buf_src[__REGION_SIZE];    /**< \brief 源端数据缓冲区 */
static uint8_t __attribute__((aligned(__REGION_SIZE))) g_buf_dst[__REGION_SIZE];    /**< \brief 目标端数据缓冲区 */

extern am_hc32f460_dma_dev_t __g_dma1_dev;
#define  DMA_TEST_DEV     __g_dma1_dev
#define  DMA_M2M_CH       DMA_CHAN_1   /**< \brief 通道 */

static volatile am_bool_t g_trans_done; /**< \brief 传输完成标志 */

static amhw_hc32f460_dma_xfer_desc_t g_desc; /**< \brief 描述符 */
/**
 * \brief DMA 中断服务程序
 */
static void dma_isr (void *p_arg , uint32_t flag)
{
    if (flag == AM_HC32F460_DMA_INT_COMPLETE) {
        g_trans_done = AM_TRUE;
    } else if (flag == AM_HC32F460_DMA_INT_BLK_COMPLETE) {
        *(volatile uint32_t *)HC32F460_AOS_BASE = 0x1;
    }
}

/**
 * \brief DMA 配置
 */
am_err_t dma_cfg(am_hc32f460_dma_dev_t *p_dma_dev, int dma_chan)
{
    uint32_t flags;

    /* 连接 DMA 中断服务函数 */
    am_hc32f460_dma_isr_connect(p_dma_dev, dma_chan, dma_isr, (void *)0);

    /* DMA 传输配置 */
    flags = AMHW_HC32F460_DMA_CHAN_CFG_INT_ENABLE            |  /* 通道中断使能 */
            AMHW_HC32F460_DMA_CHAN_CFG_SIZE_8BIT             |  /* 数据宽度 1 字节 */
            AMHW_HC32F460_DMA_CHAN_CFG_LLP_DISABLE           |  /* 连锁传输禁能 */
            AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_DISABLE |  /* 目标地址不连续传输禁能 */
            AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_DISABLE |  /* 源地址不连续传输禁能 */
            AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_DISABLE      |  /* 目标重复传输禁能 */
            AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_DISABLE      |  /* 源重复传输禁能 */
            AMHW_HC32F460_DMA_CHAN_DST_ADD_INCREASING        |  /* 目标地址自增 */
            AMHW_HC32F460_DMA_CHAN_SRC_ADD_INCREASING;          /* 源地址自增 */



    /* 建立通道描述符 */
    am_hc32f460_dma_xfer_desc_build(&g_desc,                /* 通道描述符 */
                                     (uint32_t)(g_buf_src), /* 源端数据缓冲区 */
                                     (uint32_t)(g_buf_dst), /* 目标端数据缓冲区 */
                                      __REGION_SIZE,        /* 传输字节数 */
                                     flags);

    /* 启动 DMA 传输，马上开始传输 */
    if (am_hc32f460_dma_xfer_desc_chan_cfg(p_dma_dev,
                                          &g_desc,
                                           AMHW_HC32F460_DMA_MER_TO_MER, /* 内存到内存 */
                                           dma_chan) == AM_ERROR) {
        return AM_ERROR;
    } else {
        /*外设电路触发功能使能*/
        am_clk_enable(CLK_PTDIS);

        am_mdelay(500);

        /* 设置触发源为内部事件触发 */
        am_hc32f460_dma_chan_src_set(p_dma_dev, dma_chan, EVT_AOS_STRG);

        return AM_OK;
    }

}

/* 测试DMA在没有使用MPU时是否运行正常 */
am_err_t dma_test(am_hc32f460_dma_dev_t *p_dma_dev, int dma_chan) {

    uint32_t i;

    for (i = 0; i < 32; i++) {
        g_buf_src[i] = i;
        g_buf_dst[i] = 0;
    }

    am_hc32f460_dma_chan_start(p_dma_dev, dma_chan);

    /* 软件使能内部触发事件触发DMA */
    *(volatile uint32_t *)HC32F460_AOS_BASE = 0x1;

    while(g_trans_done == AM_FALSE); /* 等待传输完成 */

    g_trans_done = AM_FALSE;

    am_hc32f460_dma_chan_stop(p_dma_dev, dma_chan);

    for (i = 0; i < __REGION_SIZE; i++) {
        if (g_buf_dst[i] != g_buf_src[i]) {
            return AM_ERROR;
        }
    }

    return AM_OK;
}

/* 测试开启MPU后，DMA是否能访问到数据 */
am_err_t dma_mpu_test(am_hc32f460_dma_dev_t *p_dma_dev, int dma_chan) {

    uint32_t i = 0;

    for (i = 0; i < __REGION_SIZE; i++) {
        g_buf_src[i] = i;
        g_buf_dst[i] = 0xFF;
    }

    if (am_hc32f460_dma_xfer_desc_chan_cfg(p_dma_dev,
                                          &g_desc,
                                           AMHW_HC32F460_DMA_MER_TO_MER,
                                           dma_chan) == AM_ERROR) {
        return AM_ERROR;
    }

    am_hc32f460_dma_chan_start(p_dma_dev, dma_chan);

    /* 软件使能内部触发事件触发DMA */
    *(volatile uint32_t *)HC32F460_AOS_BASE = 0x1;

    while(g_trans_done == AM_FALSE); /* 等待传输完成 */

    if (!memcmp(g_buf_src, g_buf_dst, sizeof(g_buf_dst)))
    {
        return AM_ERROR;
    }

    return AM_OK;
}

static am_err_t __smpu1_cfg(void *p_hw_mpu)
{
    /* 区域0基地址设置 */
    amhw_hc32f460_mpu_region_base_set(p_hw_mpu, __REGION_NUM, (uint32_t)g_buf_src);

    /* 区域0大小设置 */
    amhw_hc32f460_mpu_region_size_set(p_hw_mpu, __REGION_NUM, AMHW_HC32F460_MPU_RGSIZE_32BYTE);

    /* 区域0写使能 */
    amhw_hc32f460_mpu_rgcrn_smpu1_wr_enable(p_hw_mpu, __REGION_NUM);

    /* 区域0读禁能 */
    amhw_hc32f460_mpu_rgcrn_smpu1_rd_disable(p_hw_mpu, __REGION_NUM);
	
    /* smpu1使能 */
    amhw_hc32f460_mpu_rgcrn_smpu1_enable(p_hw_mpu, __REGION_NUM);	
  
    /* 设置发生非法访问时的操作为无视（即读访问读到0，写访问忽略） */
    amhw_hc32f460_mpu_smpu1act_set(p_hw_mpu, AMHW_HC32F460_MPU_ACT_NONEACTION);

    return AM_OK;
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_hw_mpu_entry (void *p_hw_mpu)
{
    /* DMA配置 */
    dma_cfg(&DMA_TEST_DEV, DMA_M2M_CH);

    /* 测试DMA是否正常 */
    if (dma_test(&DMA_TEST_DEV, DMA_M2M_CH) == AM_OK){
        am_kprintf("dma test success\n");
    } else {
        am_kprintf("dma test failure\n");
    }

    /* 配置MPU */
    __smpu1_cfg(p_hw_mpu);

    /* 使能MPU */
    amhw_hc32f460_mpu_smpu1_enable(p_hw_mpu);

    /* 测试MPU是否生效，若生效，则DMA无法读取相应内存 */
    if (dma_mpu_test(&DMA_TEST_DEV, DMA_M2M_CH) == AM_OK){
        am_kprintf("mpu test success\n");
    } else {
        am_kprintf("mpu test failure\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32f460_hw_mpu] */
/* end of file */
