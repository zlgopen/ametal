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
 * \brief USART 发送数据例程（DMA 方式），通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口打印 "DMA tx transfer start:"；
 *   2. 串口输出 "aAbBcCdD...",共 256 个字节；
 *   3. DMA 传输完成后串口打印 "DMA transfer done!"。
 *
 * \note
 *    1. 例程使用 USART0，与 DEBUG 调试使用串口相同；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_usart_tx_dma.c src_lpc824_hw_usart_tx_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-15  bob, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_lpc824_hw_usart_tx_dma
 * \copydoc demo_lpc824_hw_usart_tx_dma.c
 */

/** [src_lpc824_hw_usart_tx_dma] */
#include "ametal.h"
#include "am_lpc84x_dma.h"
#include "hw/amhw_lpc_usart.h"
#include "hw/amhw_lpc84x_clk.h"
#include "lpc84x_dma_chan.h"

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/
am_local uint8_t __g_buf_src[256];                    /**< \brief 源端数据缓冲区 */
am_local am_lpc84x_dma_controller_t *__gp_ctr = NULL; /**< \brief DMA通道控制器 */

/**
 * \brief DMA 传输完成回调函数
 *
 * \param[in] p_arg 用户自定义参数
 * \param[in] stat  DMA 传输完成状态标志，该参数的可能取值：
 *                      AM_LPC84X_DMA_STAT_INTA
 *                      AM_LPC84X_DMA_STAT_INTB
 *                      AM_LPC84X_DMA_STAT_INTERR
 *
 * \return  无
 */
am_local void __tx_dma_callback (void *p_arg, int stat)
{
    amhw_lpc_usart_t *p_hw_usart = (amhw_lpc_usart_t *)p_arg;
    if (stat & AM_LPC84X_DMA_STAT_INTA) {

        /* DMA 传输完成 */
        am_lpc84x_dma_controller_release(__gp_ctr);

        amhw_lpc_usart_poll_send(p_hw_usart,
                                 (am_const uint8_t *)"\r\n",
                                 sizeof("\r\n") - 1);

        amhw_lpc_usart_poll_send(p_hw_usart,
                                 (am_const uint8_t *)"DMA transfer done!\r\n",
                                 sizeof("DMA transfer done!\r\n") - 1);
    }

    if (stat & AM_LPC84X_DMA_STAT_INTERR) {

        /* 传输错误执行代码 */
    }
}
/**
 * \brief DMA 传输
 *
 * \param[in] p_txbuf 要发送的数组地址
 * \param[in] nbytes  发送的字节数
 *
 * \return 无
 */
am_local void __usart_dma_send (amhw_lpc_usart_t *p_hw_usart,
                                am_const uint8_t *p_txbuf, 
                                uint32_t          nbytes,
                                int               chan)
{
    am_lpc84x_dma_transfer_t    transfer;

    /* 配置 DMA 控制器 */
    __gp_ctr = am_lpc84x_dma_controller_get(chan,
                                            DMA_CHAN_OPT_PRIO_3 |
                                            DMA_CHAN_OPT_PERIPH_REQ_EN);
    /* 建立 DMA 传输描述符 */
    am_lpc84x_dma_transfer_build(&transfer,
                                  (uint32_t)p_txbuf,
                                  (uint32_t)&(p_hw_usart->txdat),
                                  nbytes,     /* 发送字节数 */
                                  AM_LPC84X_DMA_TRANS_WIDTH_8BIT |
                                  AM_LPC84X_DMA_TRANS_SRCINC_1X |
                                  AM_LPC84X_DMA_TRANS_DSTINC_NOINC);
    /* 开始 DMA 传输 */
    am_lpc84x_dma_transfer(__gp_ctr,
                          &transfer,
                           __tx_dma_callback,
                           (void*)p_hw_usart);
}

/**
 * \brief USART 初始化
 */
void demo_lpc845_hw_usart_tx_dma_entry (amhw_lpc_usart_t *p_hw_usart,
                                        uint32_t          uclk,
                                        uint32_t          baudrate,
                                        int               chan)
{
    uint32_t i = 0;

    /* USART0 8 位数据长度 1 位停止位 无奇偶校验 */
    amhw_lpc_usart_config(p_hw_usart,
                          AMHW_LPC_USART_CFG_8BIT |
                          AMHW_LPC_USART_CFG_STOP_1 |
                          AMHW_LPC_USART_CFG_PARITY_NO);

   /* 波特率 115200 */
    amhw_lpc_usart_baudrate_set (p_hw_usart,
                                 uclk,
                                 baudrate);

    /* 使能串口 */
    amhw_lpc_usart_enable(p_hw_usart);
    amhw_lpc_usart_poll_send(p_hw_usart,
                             (am_const uint8_t *)"DMA tx transfer start:\r\n",
                             sizeof("DMA tx transfer start:\r\n") - 1);

    /* 对发送数组赋值 */
    for (i = 0; i < 256; i += 2) {
        __g_buf_src[i]     = i / 2 % 26 + 'a';
        __g_buf_src[i + 1] = i / 2 % 26 + 'A';
    }

    /* 启动DMA 发送 */
    __usart_dma_send(p_hw_usart,__g_buf_src, 256, chan);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_lpc824_hw_usart_tx_dma] */

/* end of file */
