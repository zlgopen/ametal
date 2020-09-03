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
 * \snippet demo_stm32f103rbt6_drv_dma_m2m.c src_stm32f103rbt6_drv_dma_m2m
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-11  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_stm32f103rbt6_drv_dma_m2m
 * \copydoc demo_stm32f103rbt6_drv_dma_m2m.c
 */

/** [src_stm32f103rbt6_drv_dma_m2m] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
//#include "am_zlg116.h"
#include "am_stm32f103rbt6_dma.h"
#include "hw/amhw_stm32f103rbt6_dma.h"

static uint8_t g_buf_src[50];           /**< \brief 源端数据缓冲区 */
static uint8_t g_buf_dst[50];           /**< \brief 目标端数据缓冲区 */

static volatile am_bool_t g_trans_done; /**< \brief 传输完成标志 */

static amhw_stm32f103rbt6_dma_xfer_desc_t g_desc; /**< \brief 描述符 */

/**
 * \brief DMA 中断服务程序
 */
static void dma_isr (void *p_arg , uint32_t flag)
{
    if (flag == AM_STM32F103RBT6_DMA_INT_NORMAL) {
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
    am_stm32f103rbt6_dma_isr_connect(dma_chan, dma_isr, (void *)0);

    /* DMA 传输配置 */
    flags = AMHW_STM32F103RBT6_DMA_CHAN_PRIORITY_HIGH         |  /* 中断优先级 高 */
            AMHW_STM32F103RBT6_DMA_CHAN_MEM_SIZE_8BIT         |  /* 内存数据宽度 1 字节 */
            AMHW_STM32F103RBT6_DMA_CHAN_PER_SIZE_8BIT         |  /* 外设数据宽度 1 字节 */
            AMHW_STM32F103RBT6_DMA_CHAN_MEM_ADD_INC_ENABLE    |  /* 内存地址自增 */
            AMHW_STM32F103RBT6_DMA_CHAN_PER_ADD_INC_ENABLE    |  /* 外设地址自增 */
            AMHW_STM32F103RBT6_DMA_CHAN_CIRCULAR_MODE_DISABLE ;  /* 关闭循环模式 */


    /* 建立通道描述符 */
    am_stm32f103rbt6_dma_xfer_desc_build(&g_desc,                /* 通道描述符 */
                                (uint32_t)(g_buf_src), /* 源端数据缓冲区 */
                                (uint32_t)(g_buf_dst), /* 目标端数据缓冲区 */
                                50,                    /* 传输字节数 */
                                flags);                /* 传输配置 */

    /* 启动 DMA 传输，马上开始传输 */
    if (am_stm32f103rbt6_dma_xfer_desc_chan_cfg(&g_desc,
                                       AMHW_STM32F103RBT6_DMA_MER_TO_MER, /* 内存到内存 */
                                       dma_chan) == AM_ERROR) {
        return AM_ERROR;
    } else {
        am_stm32f103rbt6_dma_chan_start(dma_chan);
    }

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
void demo_stm32f103rbt6_drv_dma_m2m_entry (uint32_t dma_chan)
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
/** [src_stm32f103rbt6_drv_dma_m2m] */

/* end of file */
