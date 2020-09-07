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
 * \brief UART DMA 接收例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 通过上位机串口一次性向 MCU 发送 5 个字符。
 *
 * - 实验现象：
 *   1. 指定串口打印出接收到的数据；
 *   2. 然后指定串口打印出 "DMA transfer done!"。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_uart_rx_dma.c src_hc32f460_hw_uart_rx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-11  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_uart_rx_dma
 * \copydoc demo_hc32f460_hw_uart_rx_dma.c
 */

/** [src_hc32f460_hw_uart_rx_dma] */
#include "ametal.h"
#include "am_int.h"
#include "am_hc32f460_dma.h"
#include "hw/amhw_hc32f460_uart.h"
#include "hc32f460_inum.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define UART_BAUDRATE      (115200)                    /**< \brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_hc32f460_dma_xfer_desc_t g_desc;              /**< \brief DMA 重载描述符 */
static volatile am_bool_t       g_trans_done = AM_FALSE;  /**< \brief 传输完成标志 */
static uint8_t                  g_buf_dst[5] = {0};       /**< \brief 目标端数据缓冲区 */
static int32_t                  g_dma_chan = 0;           /**< \brief DMA 通道号 */
static amhw_hc32f460_uart_t    *gp_hw_uart = NULL;          /**< \brief UART 外设 */

/**
 * \brief DMA 中断服务程序
 *
 * \param[in] p_arg : 用户自定义参数，通过 am_hc32f460_dma_isr_connect() 函数传递
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值：
 *                    (#AM_ZLG116_DMA_INT_ERROR) 或 (#AM_ZLG116_DMA_INT_NORMAL)
 *
 * \return 无
 */
static void uart_dma_isr (void *p_arg , uint32_t flag)
{
    int flag_chan  = (int)p_arg;

    if (flag == AM_HC32F460_DMA_INT_COMPLETE) {
        if (flag_chan == g_dma_chan) {
            g_trans_done = AM_TRUE;
        }
    } else {
        /* 用户自定义执行代码 */
    }
}

/**
 * \brief UART接收DMA传输配置
 */
static int uart_rx_dma_tran_cfg (amhw_hc32f460_uart_t  *p_hw_uart,
                                 int32_t                dma_chan,
                                 am_hc32f460_dma_dev_t *p_dma_dev,
                                 uint32_t               dma_tran_len)
{
    uint32_t i;
    uint32_t flags;

    for (i = 0; i < sizeof(g_buf_dst); i++) {
        g_buf_dst[i] = 0;
    }

    /* 连接DMA中断服务函数 */
    am_hc32f460_dma_isr_connect(p_dma_dev, dma_chan, uart_dma_isr, (void *)dma_chan);

    /* DMA 传输配置 */
    flags = AMHW_HC32F460_DMA_CHAN_CFG_INT_ENABLE            |  /* 通道中断使能 */
            AMHW_HC32F460_DMA_CHAN_CFG_SIZE_8BIT             |  /* 数据宽度 1 字节 */
            AMHW_HC32F460_DMA_CHAN_CFG_LLP_DISABLE           |  /* 连锁传输禁能 */
            AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_DISABLE |  /* 目标地址不连续传输禁能 */
            AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_DISABLE |  /* 源地址不连续传输禁能 */
            AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_DISABLE      |  /* 目标重复传输禁能 */
            AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_DISABLE      |  /* 源重复传输禁能 */
            AMHW_HC32F460_DMA_CHAN_DST_ADD_INCREASING        |  /* 目标地址自增 */
            AMHW_HC32F460_DMA_CHAN_SRC_ADD_FIXED;               /* 源地址固定 */

    /* 建立通道描述符 */
    am_hc32f460_dma_xfer_desc_build(&g_desc,                         /* 通道描述符 */
                                   ((uint32_t)&(p_hw_uart->DR) + 2), /* 源端数据缓冲 */
                                   (uint32_t)(g_buf_dst),            /* 目标数据缓冲 */
                                   (uint32_t)dma_tran_len,           /* 传输字节数 */
                                   flags);                           /* 传输配置 */

    /* 启动DMA传输，马上开始传输 */
    if (am_hc32f460_dma_xfer_desc_chan_cfg(p_dma_dev,
                                           &g_desc,
                                           AMHW_HC32F460_DMA_PER_TO_MER, /* 外设到 内存 */
                                           (uint8_t)dma_chan) == AM_ERROR) {
        return AM_ERROR;
    } else {
        /* 开启传输 */
    }

    return AM_OK;
}

/**
 * \brief UART 初始化
 */
static void uart_hw_init (amhw_hc32f460_uart_t *p_hw_uart, uint32_t clk_rate)
{
    /* 等待发送空完成 */
    while (amhw_hc32f460_uart_status_flag_check(p_hw_uart,
                                                AMHW_HC32F460_UART_TX_COMPLETE_FALG) ==
                                                AM_FALSE);

    /* 停止位设置 */
    amhw_hc32f460_uart_stop_bit_sel(p_hw_uart, AMHW_HC32F460_UART_STOP_1BIT);

    /* 校验设置 */
    amhw_hc32f460_uart_parity_bit_sel(p_hw_uart, AMHW_HC32F460_UART_PARITY_NO);

    /* 波特率设置 */
    amhw_hc32f460_uart_baudrate_set(p_hw_uart, clk_rate, UART_BAUDRATE);

    /* 禁能串口所有中断 */
    amhw_hc32f460_uart_int_disable(p_hw_uart, AMHW_HC32F460_UART_INT_ALL_ENABLE_MASK);

    /* 使能串口发送 */
    amhw_hc32f460_uart_tx_enable (p_hw_uart, AM_TRUE);
    /* 使能串口接收 */
    amhw_hc32f460_uart_rx_enable (p_hw_uart, AM_TRUE);
}


/**
 * \brief 例程入口
 */
void demo_hc32f460_hw_uart_rx_dma_entry (void    *p_hw_uart,
                                         uint32_t clk_rate,
                                         am_hc32f460_dma_dev_t *p_dma_dev,
                                         int32_t  dma_chan)
{
    gp_hw_uart = (amhw_hc32f460_uart_t *)p_hw_uart;
    g_dma_chan = dma_chan;

    /* UART初始化 */
    uart_hw_init(gp_hw_uart, clk_rate);

    amhw_hc32f460_uart_poll_send(gp_hw_uart,
                                 (uint8_t *)"UART DMA RX start:\r\n",
                                 (uint32_t)sizeof("UART DMA RX start:\r\n") - 1);
    /* 开始DMA传输 */
    uart_rx_dma_tran_cfg(gp_hw_uart, dma_chan, p_dma_dev, sizeof(g_buf_dst));

    /* 设置传输请求信号来源为串口4的接收满事件 */
    am_hc32f460_dma_chan_src_set(p_dma_dev, dma_chan, EVT_USART4_RI);

    /* 设置每个DMA传输块大小为已开启的通道个数（1） */
    am_hc32f460_dma_block_data_size(p_dma_dev, dma_chan, 1);

    /* 开始通道传输 */
    am_hc32f460_dma_chan_start(p_dma_dev, dma_chan);

    /* 等待传输完成 */
    while (g_trans_done == AM_FALSE);

    g_trans_done = AM_FALSE;

    amhw_hc32f460_uart_poll_send(gp_hw_uart, g_buf_dst, sizeof(g_buf_dst));
    amhw_hc32f460_uart_poll_send(gp_hw_uart,
                                (uint8_t *)"\r\nDMA transfer done!\r\n",
                                 sizeof("\r\nDMA transfer done!\r\n") - 1);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32f460_hw_uart_rx_dma] */

/* end of file */
