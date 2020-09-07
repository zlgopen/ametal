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
 * \brief UART DMA 发送例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 指定串口打印提示字符 "DMA tx transfer start:"；
 *   2. 然后指定串口打印一次内存定义的字符，分别为：
 *      uart tx dma test running
 *      uart tx dma test done
 *
 * \note
 *    1. 在 DMA 操作期间不要对串口数据寄存器进行操作。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_uart_tx_dma.c src_hc32f460_hw_uart_tx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-10  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_uart_tx_dma
 * \copydoc demo_hc32f460_hw_uart_tx_dma.c
 */

/** [src_hc32f460_hw_uart_tx_dma] */
#include "ametal.h"
#include "am_int.h"
#include "am_hc32f460_dma.h"
#include "hw/amhw_hc32f460_uart.h"
#include "hc32f460_intctrl.h"
#include "hc32f460_inum.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define UART_BAUDRATE      (115200)             /**< \brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/

/** \brief DMA 重载描述符 */
static amhw_hc32f460_dma_xfer_desc_t g_desc;

/** \brief 传输完成标志 */
static volatile am_bool_t g_trans_done = AM_FALSE;

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst[]  = "uart tx dma test running\r\n";

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst1[] = "uart tx dma test done\r\n";

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst2[] = "DMA tx transfer start:\r\n";

static int32_t             g_dma_chan = 0;    /**< \brief DMA 通道号 */
static amhw_hc32f460_uart_t *gp_hw_uart = NULL; /**< \brief UART 外设 */


/**
 * \brief DMA中断服务程序。
 *
 * \param[in] p_arg : 用户自定义参数，通过 am_hc32f460_dma_isr_connect() 函数传递。
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值：
 *                    (# AM_HC32F460_DMA_INT_COMPLETE) 或 (# AM_HC32F460_DMA_INT_BLK_COMPLETE)
 *                    或(# AM_HC32F460_DMA_INT_ERR_TRNERR)或(# AM_HC32F460_DMA_INT_ERR_REQERR)
 *
 * \return 无。
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
 * \brief DMA传输配置。
 */
static int uart_tx_dma_tran_cfg (amhw_hc32f460_uart_t  *p_hw_uart,
                                 am_hc32f460_dma_dev_t *p_dma_dev,
                                 int32_t                dma_chan,
                                 const uint8_t         *p_txbuf,
                                 uint32_t               dma_tran_len)
{
    uint32_t flags;

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
             AMHW_HC32F460_DMA_CHAN_DST_ADD_FIXED             |  /* 目标地址固定 */
             AMHW_HC32F460_DMA_CHAN_SRC_ADD_INCREASING;          /* 源地址自增 */

    /* 建立通道描述符 */
    am_hc32f460_dma_xfer_desc_build(&g_desc,                         /* 通道描述符 */
                                    (uint32_t)(p_txbuf),             /* 源端数据缓冲区 */
                                    (uint32_t)(&(p_hw_uart->DR)),    /* 目标端数据缓冲区 */
                                    dma_tran_len,                    /* 传输字节数 */
                                    flags);                          /* 传输配置 */

    /* 启动DMA传输，马上开始传输 */
    if (am_hc32f460_dma_xfer_desc_chan_cfg(p_dma_dev,
    		                               &g_desc,
                                           AMHW_HC32F460_DMA_PER_TO_MER,/* 内存到外设 */
                                           dma_chan) == AM_ERROR) {
        return AM_ERROR;
    }

    return AM_OK;
}

/**
 * \brief UART初始化
 */
static void uart_hw_init (amhw_hc32f460_uart_t *p_hw_uart, uint32_t clk_rate)
{
    /* 停止位设置 */
    amhw_hc32f460_uart_stop_bit_sel(p_hw_uart  , AMHW_HC32F460_UART_STOP_1BIT);

    /* 校验设置 */
    amhw_hc32f460_uart_parity_bit_sel(p_hw_uart, AMHW_HC32F460_UART_PARITY_NO);

    /* 波特率设置 */
    amhw_hc32f460_uart_baudrate_set(p_hw_uart, clk_rate, UART_BAUDRATE);

    /* 禁能串口所有中断 */
    amhw_hc32f460_uart_int_disable(p_hw_uart, AMHW_HC32F460_UART_INT_ALL_ENABLE_MASK);

    /* 使能串口发送 */
    amhw_hc32f460_uart_tx_enable (p_hw_uart, AM_TRUE);
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_hw_uart_tx_dma_entry (void                   *p_hw_uart,
                                         uint32_t                clk_rate,
                                         am_hc32f460_dma_dev_t  *p_dma_dev,
                                         int32_t                 dma_chan)
{
    uint8_t i = 0;
    uint8_t j = 0;

    gp_hw_uart = (amhw_hc32f460_uart_t *)p_hw_uart;
    g_dma_chan = dma_chan;

    /* UART初始化 */
    uart_hw_init(gp_hw_uart, clk_rate);

    /* 设置每个DMA传输块大小为已开启的通道个数（1） */
    am_hc32f460_dma_block_data_size(p_dma_dev, dma_chan, 1);

    uart_tx_dma_tran_cfg(gp_hw_uart,
                         p_dma_dev,
                         dma_chan,
                         &g_buf_dst2[1],
                         sizeof(g_buf_dst2) - 1 -1);

    /* 设置传输请求信号来源为串口4的发送空事件 */
    am_hc32f460_dma_chan_src_set(p_dma_dev, dma_chan, EVT_USART4_TI);

    /* 开始通道传输 */
    am_hc32f460_dma_chan_start(p_dma_dev, dma_chan);

    /*
     * 由于是边沿触发，因此，事件产生必须在通道开启之后，为保证触发，先
     * 向串口数据寄存器写一个数据，数据移入移位寄存器后，发送空标志置1，
     * 产生发送空事件，触发DMA
     *  */
    amhw_hc32f460_uart_data_write(gp_hw_uart, g_buf_dst2[0]);

    while (1) {

        if (g_trans_done == AM_TRUE) {

            j++;
            g_trans_done = AM_FALSE;

            if (j < 3) {
                am_hc32f460_dma_chan_stop(p_dma_dev, dma_chan);

               /* 等待上一次传输完成 */
                while (amhw_hc32f460_uart_status_flag_check(p_hw_uart,
                	   AMHW_HC32F460_UART_TX_COMPLETE_FALG) == AM_FALSE);
                if (0 == i) {

                    uart_tx_dma_tran_cfg(gp_hw_uart,
                                         p_dma_dev,
                                         dma_chan,
                                         &g_buf_dst[1],
                                         sizeof(g_buf_dst) - 1 -1);

                	amhw_hc32f460_uart_data_write(gp_hw_uart, g_buf_dst[0]);


                } else {
                    uart_tx_dma_tran_cfg(gp_hw_uart,
                                         p_dma_dev,
                                         dma_chan,
                                         &g_buf_dst1[1],
                                         sizeof(g_buf_dst1) - 1 - 1);

                	amhw_hc32f460_uart_data_write(gp_hw_uart, g_buf_dst1[0]);

                }

                i++;

                am_hc32f460_dma_chan_start(p_dma_dev, dma_chan);

            }

        }
    }
}
/** [src_hc32f460_hw_uart_tx_dma] */

/* end of file */
