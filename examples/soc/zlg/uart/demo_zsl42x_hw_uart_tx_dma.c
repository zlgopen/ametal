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
 * \snippet demo_zsl42x_hw_uart_tx_dma.c src_zsl42x_hw_uart_tx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-23  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_uart_tx_dma
 * \copydoc demo_zsl42x_hw_uart_tx_dma.c
 */

/** [src_zsl42x_hw_uart_tx_dma] */
#include "ametal.h"
#include "am_int.h"
#include "am_zsl42x_dma.h"
#include "hw/amhw_zsl42x_uart.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define UART_BAUDRATE      (115200)             /**< \brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/

/** \brief DMA 重载描述符 */
static amhw_zsl42x_dma_xfer_desc_t g_desc;

/** \brief 传输完成标志 */
static volatile am_bool_t g_trans_done = AM_FALSE;

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst[]  = "uart tx dma test running\r\n";

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst1[] = "uart tx dma test done\r\n";

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst2[] = "DMA tx transfer start:\r\n";

static int32_t             g_dma_chan = 0;    /**< \brief DMA 通道号 */
static amhw_zsl42x_uart_t *gp_hw_uart = NULL; /**< \brief UART 外设 */

/**
 * \brief DMA中断服务程序。
 *
 * \param[in] p_arg : 用户自定义参数，通过 am_zsl42x_dma_isr_connect() 函数传递。
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值：
 *                    (# AM_zsl42x_DMA_INT_NORMAL) 或 (# AM_zsl42x_DMA_INT_ERROR)
 *
 * \return 无。
 */
static void uart_dma_isr (void *p_arg , uint32_t flag)
{
    int flag_chan  = (int)p_arg;

    if (flag == AM_ZSL42x_DMA_INT_COMPLETE) {
        if (flag_chan == g_dma_chan) {

            /* 禁能UART发送时使用DMA传输 */
            amhw_zsl42x_uart_disable(gp_hw_uart, AMHW_ZSL42x_UART_DMA_TX);
            g_trans_done = AM_TRUE;
        }
    } else {
        /* 用户自定义执行代码 */
    }

}

/**
 * \brief DMA传输配置。
 */
static int uart_tx_dma_tran_cfg (amhw_zsl42x_uart_t *p_hw_uart,
                                 int32_t             dma_chan,
                                 const uint8_t      *p_txbuf,
                                 uint32_t            dma_tran_len)
{
    uint32_t flags;

    /* 连接DMA中断服务函数 */
    am_zsl42x_dma_isr_connect(dma_chan, uart_dma_isr, (void *)dma_chan);

    /* DMA 传输配置 */
       flags = AMHW_ZSL42x_DMA_CHAN_TRANSFER_MODE_BLOCK    | /* SPI只有块传输模式 */
               AMHW_ZSL42x_DMA_CHAN_SIZE_8BIT              | /* 数据位宽 8bit */

               AMHW_ZSL42x_DMA_CHAN_SRC_ADD_INC_ENABLE     | /* 源地址自增 */
               AMHW_ZSL42x_DMA_CHAN_RELOAD_SRC_ADD_DISABLE | /* 源地址不重载*/

               AMHW_ZSL42x_DMA_CHAN_DST_ADD_INC_DISABLE    | /* 目标地址不自增 */
               AMHW_ZSL42x_DMA_CHAN_RELOAD_DST_ADD_ENABLE  | /* 目标地址重载 */

               AMHW_ZSL42x_DMA_CHAN_RELOAD_COUNTER_ENABLE  | /* 传输数据量重载 */
               AMHW_ZSL42x_DMA_CHAN_INT_ERR_DISABLE        | /* 传输错误中断禁能 */
               AMHW_ZSL42x_DMA_CHAN_INT_TX_CMP_ENABLE      | /* 传输完成中断使能 */
               AMHW_ZSL42x_DMA_CHAN_CIRCULAR_MODE_DISABLE ;  /* 关闭循环模式 */

    /* 建立通道描述符 */
    am_zsl42x_dma_xfer_desc_build(&g_desc,                       /* 通道描述符 */
                                  (uint32_t)(p_txbuf),           /* 源端数据缓冲区 */
                                  (uint32_t)(&(p_hw_uart->sbuf)),/* 目标端数据缓冲区 */
                                  dma_tran_len,                  /* 传输字节数 */
                                  flags);                        /* 传输配置 */

    /* 启动DMA传输，马上开始传输 */
    if (am_zsl42x_dma_xfer_desc_chan_cfg(&g_desc,
                                         AMHW_ZSL42x_DMA_PER_TO_MER,/* 内存到外设 */
                                         dma_chan) == AM_ERROR) {
        return AM_ERROR;
    }

    return AM_OK;
}

/**
 * \brief UART初始化
 */
static void uart_hw_init (amhw_zsl42x_uart_t *p_hw_uart, uint32_t clk_rate)
{
    /* 工作模式设置  */
    amhw_zsl42x_uart_mode_sel(p_hw_uart, AMHW_ZSL42x_UART_WORK_MODE_1);

    /* 分频系数设置  */
    amhw_zsl42x_uart_clk_div_sel(p_hw_uart, AMHW_ZSL42x_UART_CLK_DIV_MODE13_8);

    /* 等待发送空完成 */
    while (amhw_zsl42x_uart_flag_check(p_hw_uart,
                                       AMHW_ZSL42x_UART_FLAG_TX_EMPTY) ==
                                       AM_FALSE);

    /* 停止位设置 */
    amhw_zsl42x_uart_stop_bit_sel(p_hw_uart  , AMHW_ZSL42x_UART_STOP_1_0_BIT);

    /* 校验设置 */
    amhw_zsl42x_uart_parity_bit_sel(p_hw_uart, AMHW_ZSL42x_UART_PARITY_NO);

    /* 波特率设置 */
    amhw_zsl42x_uart_baudrate_set( p_hw_uart, clk_rate, UART_BAUDRATE);

    /* 关闭所有串口中断 */
    amhw_zsl42x_uart_int_disable(p_hw_uart, AMHW_ZSL42x_UART_INT_ALL);
}


/**
 * \brief UART发送DMA传输初始化
 */
static void uart_hw_dma_init (amhw_zsl42x_uart_t *p_hw_uart)
{
    /* 串口发送DMA传输使能 */
    amhw_zsl42x_uart_enable(p_hw_uart, AMHW_ZSL42x_UART_DMA_TX);

    /* 接收使能
     * Mode0:     0：发送; 1：接收
     * Mode1/2/3: 0：发送; 1： 接收/发送
     */
    amhw_zsl42x_uart_enable(p_hw_uart,AMHW_ZSL42x_UART_RX);
}

/**
 * \brief UART DMA 发送例程，通过 HW 层接口实现
 */
void demo_zsl42x_hw_uart_tx_dma_entry (void    *p_hw_uart,
                                     uint32_t clk_rate,
                                     int32_t  dma_chan,
                                     int32_t  dma_src)
{
    uint8_t i = 0;
    uint8_t j = 0;

    gp_hw_uart = (amhw_zsl42x_uart_t *)p_hw_uart;
    g_dma_chan = dma_chan;

    /* UART初始化 */
    uart_hw_init(gp_hw_uart, clk_rate);

    /* UART用DMA传输的初始化 */
    uart_hw_dma_init(gp_hw_uart);

    uart_tx_dma_tran_cfg(gp_hw_uart,
                         dma_chan,
                         g_buf_dst2,
                         sizeof(g_buf_dst2) - 1);

    /* 默认通道0和通道1优先级一样（轮询）*/
    am_zsl42x_dma_priority_same();

    /* 设置传输请求信号来源 */
    am_zsl42x_dma_chan_src_set (dma_chan, dma_src);

    /* 设置每个DMA传输块大小为已开启的通道个数（1） */
    am_zsl42x_dma_block_data_size(dma_chan, 1);

    /* 开始通道传输 */
    am_zsl42x_dma_chan_start(dma_chan);

    while (1) {

        if (g_trans_done == AM_TRUE) {

            j++;
            g_trans_done = AM_FALSE;

            /* 串口发送DMA传输使能 */
            amhw_zsl42x_uart_enable(p_hw_uart, AMHW_ZSL42x_UART_DMA_TX);

            if (j < 3) {
                am_zsl42x_dma_chan_stop(dma_chan);
                if (0 == i) {
                    uart_tx_dma_tran_cfg(gp_hw_uart,
                                         dma_chan,
                                         g_buf_dst,
                                         sizeof(g_buf_dst) - 1);
                } else {
                    uart_tx_dma_tran_cfg(gp_hw_uart,
                                         dma_chan,
                                         g_buf_dst1,
                                         sizeof(g_buf_dst1) - 1);
                }
                i++;
                am_zsl42x_dma_chan_start(dma_chan);
            }

        }
    }
}
/** [src_zsl42x116_hw_uart_tx_dma] */

/* end of file */
