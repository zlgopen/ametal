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
 * \brief LPUART DMA 接收例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 通过上位机串口一次性向 MCU 发送 5 个字符。
 *
 * - 实验现象：
 *   1. 指定串口打印出接收到的数据；
 *   2. 然后指定串口打印出 "DMA transfer done!"。
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_lpuart_rx_dma.c src_zlg118_hw_lpuart_rx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-26  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_lpuart_rx_dma
 * \copydoc demo_zlg118_hw_lpuart_rx_dma.c
 */

/** [src_zlg118_hw_lpuart_rx_dma] */
#include "ametal.h"
#include "am_int.h"
#include "am_zlg118_dma.h"
#include "hw/amhw_zlg118_lpuart.h"
#include "hw/amhw_zlg118_rcc.h"

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_zlg118_dma_xfer_desc_t g_desc;           /**< \brief DMA 重载描述符 */
static volatile am_bool_t       g_trans_done = AM_FALSE; /**< \brief 传输完成标志 */
static uint8_t                  g_buf_dst[5] = {0};  /**< \brief 目标端数据缓冲区 */
static int32_t                  g_dma_chan   = 0;    /**< \brief DMA 通道号 */
static amhw_zlg118_lpuart_t    *gp_hw_lpuart = NULL; /**< \brief LPUART 外设 */

static uint32_t lpuart_baudrate = 0;

/**
 * \brief DMA 中断服务程序
 *
 * \param[in] p_arg : 用户自定义参数，通过 am_zlg118_dma_isr_connect() 函数传递
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值
 *
 * \return 无
 */
static void lpuart_dma_isr (void *p_arg , uint32_t flag)
{
    int flag_chan  = (int)p_arg;

    if (flag == AM_ZLG118_DMA_INT_COMPLETE) {
        if (flag_chan == g_dma_chan) {

            /* 禁能LPUART发送时使用DMA传输 */
            amhw_zlg118_lpuart_disable(gp_hw_lpuart, AMHW_ZLG118_LPUART_DMA_RX);

            g_trans_done = AM_TRUE;
        }
    } else {
        /* 用户自定义执行代码 */
    }
}

/**
 * \brief LPUART接收DMA传输配置
 */
static int lpuart_rx_dma_tran_cfg (amhw_zlg118_lpuart_t *p_hw_lpuart,
                                 int32_t          dma_chan,
                                 uint32_t         dma_tran_len)
{
    uint32_t i;
    uint32_t flags;

    for (i = 0; i < sizeof(g_buf_dst); i++) {
        g_buf_dst[i] = 0;
    }

    /* 连接DMA中断服务函数 */
    am_zlg118_dma_isr_connect(dma_chan, lpuart_dma_isr, (void *)dma_chan);

    /* DMA 传输配置 */
    flags = AMHW_ZLG118_DMA_CHAN_TRANSFER_MODE_BLOCK    | /* SPI只有块传输模式 */
            AMHW_ZLG118_DMA_CHAN_SIZE_8BIT              | /* 数据位宽 8bit */

            AMHW_ZLG118_DMA_CHAN_SRC_ADD_INC_DISABLE    | /* 源地址不自增 */
            AMHW_ZLG118_DMA_CHAN_RELOAD_SRC_ADD_ENABLE  | /* 源地址重载*/

            AMHW_ZLG118_DMA_CHAN_DST_ADD_INC_ENABLE     | /* 目标地址自增 */
            AMHW_ZLG118_DMA_CHAN_RELOAD_DST_ADD_DISABLE | /* 目标地址不重载 */

            AMHW_ZLG118_DMA_CHAN_RELOAD_COUNTER_ENABLE  | /* 传输数据量重载 */
            AMHW_ZLG118_DMA_CHAN_INT_ERR_DISABLE        | /* 传输错误中断禁能 */
            AMHW_ZLG118_DMA_CHAN_INT_TX_CMP_ENABLE      | /* 传输完成中断使能 */
            AMHW_ZLG118_DMA_CHAN_CIRCULAR_MODE_DISABLE ;  /* 关闭循环模式 */

    /* 建立通道描述符 */
    am_zlg118_dma_xfer_desc_build(&g_desc,                      /* 通道描述符 */
                                   (uint32_t)(&(p_hw_lpuart->sbuf)), /* 源端数据缓冲 */
                                   (uint32_t)(g_buf_dst),       /* 目标数据缓冲 */
                                   (uint32_t)dma_tran_len,      /* 传输字节数 */
                                   flags);                      /* 传输配置 */

    /* 启动DMA传输，马上开始传输 */
    if (am_zlg118_dma_xfer_desc_chan_cfg(&g_desc,
                                          AMHW_ZLG118_DMA_PER_TO_MER, /* 外设到 内存 */
                                          (uint8_t)dma_chan) == AM_ERROR) {
        return AM_ERROR;
    } else {
        /* 开启传输 */
    }

    return AM_OK;
}

/**
 * \brief LPUART 初始化
 */
static void lpuart_hw_init (amhw_zlg118_lpuart_t *p_hw_lpuart,
                            uint8_t               clk_src,
                            uint32_t              clk_rate)
{
    /* 工作模式设置  */
    amhw_zlg118_lpuart_mode_sel(p_hw_lpuart, AMHW_ZLG118_LPUART_WORK_MODE_1);

    /* 设置通信时钟源 */
    amhw_zlg118_lpuart_clk_src_sel(p_hw_lpuart,
                                   (amhw_zlg118_lpuart_sclk_src_t)clk_src);

    if(clk_src == AMHW_ZLG118_LPUART_SCLK_SRC_XTL) {

        /* 外部晶振驱动能力选择 */
        amhw_zlg118_rcc_xtl_xtal_driver_set(AMHW_ZLG118_XTL_XTAL_DRIVER_DEFAULT);

        /* XTL晶振振荡幅度的调整 */
        amhw_zlg118_rcc_xtl_xtal_amplitude_set(
            AMHW_ZLG118_XTL_XTAL_AMPLITUDE_BIG_DEFAULT);

        /* 外部高速时钟XTL稳定时间选择 */
        amhw_zlg118_rcc_xtl_waittime_set(AMHW_ZLG118_XTL_WAITTIME_16384);

        amhw_zlg118_rcc_set_start(0x5A5A);
        amhw_zlg118_rcc_set_start(0xA5A5);

        amhw_zlg118_rcc_xtl_enable();

        /* 等待稳定*/
        while(amhw_zlg118_rcc_xtl_state_get() == AM_FALSE);

    } else if(clk_src == AMHW_ZLG118_LPUART_SCLK_SRC_RCL) {

        /* 设置RCL时钟频率校准值 （38.4KHz）*/
        amhw_zlg118_rcc_rcl_trim_set(*((uint16_t*)0x00100C20));

        amhw_zlg118_rcc_rcl_waittime_set(AMHW_ZLG118_RCL_WAITTIME_256);

        amhw_zlg118_rcc_set_start(0x5A5A);
        amhw_zlg118_rcc_set_start(0xA5A5);

        amhw_zlg118_rcc_rcl_enable();

        /* 等待稳定*/
        while(amhw_zlg118_rcc_rcl_state_get() == AM_FALSE);
    }

    /* 分频系数设置  */
    amhw_zlg118_lpuart_clk_div_sel(p_hw_lpuart,
                                   AMHW_ZLG118_LPUART_SCLK_DIV_MODE13_8);

    /* 等待发送空完成 */
    while (amhw_zlg118_lpuart_flag_check(p_hw_lpuart,
                                       AMHW_ZLG118_LPUART_FLAG_TX_EMPTY) ==
                                       AM_FALSE);

    /* 停止位设置 */
    amhw_zlg118_lpuart_stop_bit_sel(p_hw_lpuart,
                                    AMHW_ZLG118_LPUART_STOP_1_0_BIT);

    /* 校验设置 */
    amhw_zlg118_lpuart_parity_bit_sel(p_hw_lpuart,
                                      AMHW_ZLG118_LPUART_PARITY_NO);

    /* 波特率设置 */
    amhw_zlg118_lpuart_baudrate_set( p_hw_lpuart, clk_rate, lpuart_baudrate);

    /* 关闭所有串口中断 */
    amhw_zlg118_lpuart_int_disable(p_hw_lpuart, AMHW_ZLG118_LPUART_INT_ALL);
}

/**
 * \brief LPUART接收传输时DMA初始化
 */
static void lpuart_hw_dma_init (amhw_zlg118_lpuart_t *p_hw_lpuart)
{
    /* 串口发送DMA传输使能 */
    amhw_zlg118_lpuart_enable(p_hw_lpuart, AMHW_ZLG118_LPUART_DMA_RX);

    /* 接收使能
     * Mode0:     0：发送; 1：接收
     * Mode1/2/3: 0：发送; 1： 接收/发送
     */
    amhw_zlg118_lpuart_enable(p_hw_lpuart,AMHW_ZLG118_LPUART_RX);
}

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_lpuart_rx_dma_entry (void    *p_hw_lpuart,
                                         uint8_t  clk_src,
                                         uint32_t clk_rate,
                                         uint32_t baudrate,
                                         int32_t  dma_chan,
                                         int32_t  dma_src)
{
    gp_hw_lpuart = (amhw_zlg118_lpuart_t *)p_hw_lpuart;
    g_dma_chan = dma_chan;

    lpuart_baudrate = baudrate;

    /* LPUART初始化 */
    lpuart_hw_init(gp_hw_lpuart, clk_src, clk_rate);

    /* LPUART接收DMA传输的初始化 */
    lpuart_hw_dma_init(gp_hw_lpuart);

    amhw_zlg118_lpuart_poll_send(gp_hw_lpuart,
                                 (uint8_t *)"LPUART DMA RX start:\r\n",
                                 sizeof("LPUART DMA RX start:\r\n") - 1);
    /* 开始DMA传输 */
    lpuart_rx_dma_tran_cfg(gp_hw_lpuart, dma_chan, sizeof(g_buf_dst));

    /* 默认通道0和通道1优先级一样（轮询）*/
    am_zlg118_dma_priority_same();

    /* 设置传输请求信号来源 */
    am_zlg118_dma_chan_src_set (dma_chan, dma_src);

    /* 设置每个DMA传输块大小为已开启的通道个数（1） */
    am_zlg118_dma_block_data_size(dma_chan, 1);

    /* 开始通道传输 */
    am_zlg118_dma_chan_start(dma_chan);

    /* 等待传输完成 */
    while (g_trans_done == AM_FALSE);

    g_trans_done = AM_FALSE;

    amhw_zlg118_lpuart_poll_send(gp_hw_lpuart, g_buf_dst, sizeof(g_buf_dst));
    amhw_zlg118_lpuart_poll_send(gp_hw_lpuart,
                                 (uint8_t *)"\r\nDMA transfer done!\r\n",
                                  sizeof("\r\nDMA transfer done!\r\n") - 1);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zlg118_hw_lpuart_rx_dma] */

/* end of file */
