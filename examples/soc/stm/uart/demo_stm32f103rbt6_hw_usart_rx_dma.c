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
 * \brief USART DMA 接收例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 通过上位机串口一次性向 MCU 发送 5 个字符。
 *
 * - 实验现象：
 *   1. 指定串口打印出接收到的数据；
 *   2. 然后指定串口打印出 "DMA transfer done!"。
 *
 * \par 源代码
 * \snippet demo_zlg237_hw_usart_rx_dma.c src_zlg237_hw_usart_rx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 17-03-10  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg237_hw_usart_rx_dma
 * \copydoc demo_zlg237_hw_usart_rx_dma.c
 */

/** [src_zlg237_hw_usart_rx_dma] */
#include "am_zlg237.h"
#include "amhw_zlg237_usart.h"
#include "ametal.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_zlg_dma.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define USART_BAUDRATE      (115200)             /**< \brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_zlg_dma_xfer_desc_t g_desc;                  /**< \brief DMA 重载描述符 */
static volatile am_bool_t       g_trans_done = AM_FALSE; /**< \brief 传输完成标志 */
static uint8_t                  g_buf_dst[5] = {0};      /**< \brief 目标端数据缓冲区 */
static int32_t                  g_dma_chan = 0;          /**< \brief DMA 通道号 */
static amhw_zlg237_usart_t     *gp_hw_usart = NULL;      /**< \brief USART 外设 */

/**
 * \brief DMA 平台初始化。
 */
static void __zlg237_plfm_dma_init (void)
{
    am_clk_enable(CLK_DMA);
}

/**
 * \brief DMA 平台去初始化。
 */
static void __zlg237_plfm_dma_deinit (void)
{
    am_clk_disable(CLK_DMA);
}

/** \brief DMA 设备信息 */
static const am_zlg_dma_devinfo_t __g_dma_devinfo = {
    ZLG237_DMA_BASE,           /**< \brief 指向DMA寄存器块的指针 */
    INUM_DMA1_1,               /**< \brief DMA中断向量号开始 */
    INUM_DMA1_7,               /**< \brief DMA中断向量号结束 */
    __zlg237_plfm_dma_init,    /**< \brief DMA平台初始化 */
    __zlg237_plfm_dma_deinit   /**< \brief DMA平台解初始化 */
};

/** \brief DMA设备实例 */
static am_zlg_dma_dev_t __g_dma_dev;

/**
 * \brief DMA 中断服务程序
 *
 * \param[in] p_arg : 用户自定义参数，通过 am_zlg_dma_isr_connect() 函数传递
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值：
 *                    (#AM_ZLG116_DMA_INT_ERROR) 或 (#AM_ZLG116_DMA_INT_NORMAL)
 *
 * \return 无
 */
static void usart_dma_isr (void *p_arg , uint32_t flag)
{
    int flag_chan  = (int)p_arg;

    if (flag == AM_ZLG_DMA_INT_NORMAL) {
        if (flag_chan == g_dma_chan) {

            /* 禁能USART接收 */
        	amhw_zlg237_usart_receiver_disable(gp_hw_usart);

            /* 禁能USART发送时使用DMA传输 */
        	amhw_zlg237_usart_dmat_enable(gp_hw_usart);

            g_trans_done = AM_TRUE;
        }
    } else {
        /* 用户自定义执行代码 */
    }
}

/**
 * \brief USART接收DMA传输配置
 */
static int usart_rx_dma_tran_cfg (amhw_zlg237_usart_t *p_hw_usart,
                                 int32_t               dma_chan,
                                 uint32_t              dma_tran_len)
{
    uint32_t i;
    uint32_t flags;

    for (i = 0; i < sizeof(g_buf_dst); i++) {
        g_buf_dst[i] = 0;
    }

    /* DMA传输配置 */
    flags = AMHW_ZLG_DMA_CHAN_PRIORITY_HIGH         |  /* 外设请求源禁能 */
            AMHW_ZLG_DMA_CHAN_MEM_SIZE_8BIT         |  /* 源地址1字节 */
            AMHW_ZLG_DMA_CHAN_PER_SIZE_8BIT         |  /* 目的地址1字节写入 */
            AMHW_ZLG_DMA_CHAN_MEM_ADD_INC_ENABLE    |  /* 请求有影响 */
            AMHW_ZLG_DMA_CHAN_PER_ADD_INC_DISABLE   |  /* 无通道连接 */
            AMHW_ZLG_DMA_CHAN_CIRCULAR_MODE_DISABLE ,  /* DMA中断使能 */

    /* 连接DMA中断服务函数 */
    am_zlg_dma_isr_connect(dma_chan, usart_dma_isr, (void *)dma_chan);


    /* 建立通道描述符 */
    am_zlg_dma_xfer_desc_build(&g_desc,                      /* 通道描述符 */
                               (uint32_t)(&(p_hw_usart->dr)), /* 源端数据缓冲 */
                               (uint32_t)(g_buf_dst),       /* 目标数据缓冲 */
                               (uint32_t)dma_tran_len,      /* 传输字节数 */
                               flags);                      /* 传输配置 */

    /* 启动DMA传输，马上开始传输 */
    if (am_zlg_dma_xfer_desc_chan_cfg(&g_desc,
                                       AMHW_ZLG_DMA_PER_TO_MER, /* 外设到 内存 */
                                      (uint8_t)dma_chan) == AM_ERROR) {
        return AM_ERROR;
    } else {
        /* 开启传输 */
    }

    return AM_OK;
}

/**
 * \brief USART 初始化
 */
static void usart_hw_init (amhw_zlg237_usart_t *p_hw_usart, uint32_t clk_rate)
{
    amhw_zlg237_usart_baudrate_set(p_hw_usart, clk_rate, USART_BAUDRATE);

    amhw_zlg237_usart_stop_bit_sel(p_hw_usart, AMHW_ZLG237_USART_STOP_10_BIT);
    amhw_zlg237_usart_word_length_sel(p_hw_usart, AMHW_ZLG237_USART_DATA_8BIT);
    amhw_zlg237_usart_parity_bit_sel(p_hw_usart,  AMHW_ZLG237_USART_PARITY_NO);
}

/**
 * \brief USART接收传输时DMA初始化
 */
static void usart_hw_dma_init (amhw_zlg237_usart_t *p_hw_usart)
{
    /* 串口接收DMA传输使能 */
	amhw_zlg237_usart_dmar_enable(p_hw_usart);

    /* 使能串口 */
    amhw_zlg237_usart_receiver_enable(p_hw_usart);
    amhw_zlg237_usart_transmitter_enable(p_hw_usart);
    amhw_zlg237_usart_enable(p_hw_usart);
}

/**
 * \brief 例程入口
 */
void demo_zlg237_hw_usart_rx_dma_entry (amhw_zlg237_usart_t *p_hw_usart,
                                        uint32_t             clk_rate,
                                        int32_t              dma_chan)
{
    gp_hw_usart = p_hw_usart;
    g_dma_chan = dma_chan;

    /* USART初始化 */
    usart_hw_init(p_hw_usart, clk_rate);

    /* USART接收DMA传输的初始化 */
    usart_hw_dma_init(p_hw_usart);

    amhw_zlg237_usart_poll_send(p_hw_usart,
                               (uint8_t *)"USART DMA RX start:\r\n",
                               (uint32_t)sizeof("USART DMA RX start:\r\n") - 1);
    /* 开始DMA传输 */
    am_zlg_dma_init(&__g_dma_dev, &__g_dma_devinfo);
    usart_rx_dma_tran_cfg(p_hw_usart, dma_chan, sizeof(g_buf_dst));
    am_zlg_dma_chan_start(dma_chan);

    /* 等待传输完成 */
    while (g_trans_done == AM_FALSE);

    g_trans_done = AM_FALSE;

    amhw_zlg237_usart_poll_send(p_hw_usart, g_buf_dst, sizeof(g_buf_dst));
    amhw_zlg237_usart_poll_send(p_hw_usart,
                               (uint8_t *)"\r\nDMA transfer done!\r\n",
                                sizeof("\r\nDMA transfer done!\r\n") - 1);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zlg237_hw_usart_rx_dma] */

/* end of file */
