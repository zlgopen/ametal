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
 * \snippet demo_mm32_hw_uart_tx_dma.c src_mm32_hw_uart_tx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-10  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32_hw_uart_tx_dma
 * \copydoc demo_mm32_hw_uart_tx_dma.c
 */

/** [src_mm32_hw_uart_tx_dma] */
#include "ametal.h"
#include "am_int.h"
//#include "am_mm32l073.h"
#include "am_mm32_dma.h"
#include "hw/amhw_mm32_uart.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define UART_BAUDRATE      (115200)             /**< \brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/

/** \brief DMA 重载描述符 */
static amhw_mm32_dma_xfer_desc_t g_desc;

/** \brief 传输完成标志 */
static volatile am_bool_t g_trans_done = AM_FALSE;

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst[]  = "uart tx dma test running\r\n";

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst1[] = "uart tx dma test done\r\n";

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst2[] = "DMA tx transfer start:\r\n";

static int32_t          g_dma_chan = 0;    /**< \brief DMA 通道号 */
static amhw_mm32_uart_t *gp_hw_uart = NULL; /**< \brief UART 外设 */

/**
 * \brief DMA中断服务程序。
 *
 * \param[in] p_arg : 用户自定义参数，通过 am_mm32_dma_isr_connect() 函数传递。
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值：
 *                    (# AM_MM32_DMA_INT_NORMAL) 或 (# AM_MM32_DMA_INT_ERROR)
 *
 * \return 无。
 */
static void uart_dma_isr (void *p_arg , uint32_t flag)
{
    int flag_chan  = (int)p_arg;

    if (flag == AM_MM32_DMA_INT_NORMAL) {
        if (flag_chan == g_dma_chan) {

            /* 禁能UART发送时使用DMA传输 */
            amhw_mm32_uart_dma_mode_enable(gp_hw_uart, AM_FALSE);
            g_trans_done = AM_TRUE;
        }
    } else {
        /* 用户自定义执行代码 */
    }

}

/**
 * \brief DMA传输配置。
 */
static int uart_tx_dma_tran_cfg (amhw_mm32_uart_t *p_hw_uart,
                                 int32_t          dma_chan,
                                 const uint8_t   *p_txbuf,
                                 uint32_t         dma_tran_len)
{
    uint32_t flags;

    /* 连接DMA中断服务函数 */
    am_mm32_dma_isr_connect(dma_chan, uart_dma_isr, (void *)dma_chan);

    /* DMA传输配置 */
    flags = AMHW_MM32_DMA_CHAN_PRIORITY_HIGH         |  /* 外设请求源禁能 */
            AMHW_MM32_DMA_CHAN_MEM_SIZE_8BIT         |  /* 源地址1字节 */
            AMHW_MM32_DMA_CHAN_PER_SIZE_8BIT         |  /* 目的地址1字节写入 */
            AMHW_MM32_DMA_CHAN_MEM_ADD_INC_ENABLE    |  /* 请求有影响 */
            AMHW_MM32_DMA_CHAN_PER_ADD_INC_DISABLE   |  /* 无通道连接 */
            AMHW_MM32_DMA_CHAN_CIRCULAR_MODE_DISABLE ,  /* DMA中断使能 */

    /* 建立通道描述符 */
    am_mm32_dma_xfer_desc_build(&g_desc,                        /* 通道描述符 */
                                (uint32_t)(p_txbuf),           /* 源端数据缓冲区 */
                                (uint32_t)(&(p_hw_uart->tdr)), /* 目标端数据缓冲区 */
                                dma_tran_len,                  /* 传输字节数 */
                                flags);                        /* 传输配置 */

    /* 启动DMA传输，马上开始传输 */
    if (am_mm32_dma_xfer_desc_chan_cfg(&g_desc,
                                       AMHW_MM32_DMA_MER_TO_PER,  /* 内存到外设 */
                                       dma_chan) == AM_ERROR) {
        return AM_ERROR;
    }

    return AM_OK;
}

/**
 * \brief UART初始化
 */
static void uart_hw_init (amhw_mm32_uart_t *p_hw_uart, uint32_t clk_rate)
{
    amhw_mm32_uart_baudrate_set(p_hw_uart,clk_rate, UART_BAUDRATE);

    amhw_mm32_uart_stop_bit_sel(p_hw_uart, AMHW_MM32_UART_STOP_1BIT);
    amhw_mm32_uart_data_length(p_hw_uart, AMHW_MM32_UART_DATA_8BIT);
    amhw_mm32_uart_parity_bit_sel(p_hw_uart,  AMHW_MM32_UART_PARITY_NO);

    /* 使能串口 */
    amhw_mm32_uart_tx_enable(p_hw_uart,AM_TRUE);
    amhw_mm32_uart_enable(p_hw_uart);
}


/**
 * \brief UART发送DMA传输初始化
 */
static void uart_hw_dma_init (amhw_mm32_uart_t *p_hw_uart)
{

    /* 串口发送DMA传输使能 */
    amhw_mm32_uart_dma_mode_enable(p_hw_uart, AM_TRUE);

    /* 使能串口 */
    amhw_mm32_uart_enable(p_hw_uart);
}

/**
 * \brief 例程入口
 */
void demo_mm32_hw_uart_tx_dma_entry (amhw_mm32_uart_t *p_hw_uart,
                                    uint32_t         clk_rate,
                                    int32_t          dma_chan)
{
    uint8_t i = 0;
    uint8_t j = 0;

    gp_hw_uart = p_hw_uart;
    g_dma_chan = dma_chan;

    /* UART初始化 */
    uart_hw_init(p_hw_uart, clk_rate);

    /* UART用DMA传输的初始化 */
    uart_hw_dma_init(p_hw_uart);

    uart_tx_dma_tran_cfg(p_hw_uart, dma_chan, g_buf_dst2, sizeof(g_buf_dst2) - 1);

    am_mm32_dma_chan_start(dma_chan);

    while (1) {

        if (g_trans_done == AM_TRUE) {

            j++;
            g_trans_done = AM_FALSE;

            /* 串口发送DMA传输使能 */
            amhw_mm32_uart_dma_mode_enable(p_hw_uart,AM_TRUE);

            /* 使能串口 */
            amhw_mm32_uart_tx_enable(p_hw_uart, AM_TRUE);

            if (j < 3) {
                am_mm32_dma_chan_stop(dma_chan);
                if (0 == i) {
                    uart_tx_dma_tran_cfg(p_hw_uart,
                                         dma_chan,
                                         g_buf_dst,
                                         sizeof(g_buf_dst) - 1);
                } else {
                    uart_tx_dma_tran_cfg(p_hw_uart,
                                         dma_chan,
                                         g_buf_dst1,
                                         sizeof(g_buf_dst1) - 1);
                }
                i++;
                am_mm32_dma_chan_start(dma_chan);
            }

        }
    }
}
/** [src_mm32l073_hw_uart_tx_dma] */

/* end of file */
