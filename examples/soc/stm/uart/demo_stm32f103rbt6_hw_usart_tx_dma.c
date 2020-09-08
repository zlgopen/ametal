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
 * \brief USART DMA 发送例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 指定串口打印提示字符 "DMA tx transfer start:"；
 *   2. 然后指定串口打印一次内存定义的字符，分别为：
 *      usart tx dma test running
 *      usart tx dma test done
 *
 * \note
 *    1. 在 DMA 操作期间不要对串口数据寄存器进行操作。
 *
 * \par 源代码
 * \snippet demo_usart_tx_dma.c src_usart_tx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-10  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_usart_tx_dma
 * \copydoc demo_usart_tx_dma.c
 */

/** [src_usart_tx_dma] */
#include "am_stm32f103rbt6.h"
#include "amhw_stm32f103rbt6_usart.h"
#include "ametal.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_stm32f103rbt6_dma.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define USART_BAUDRATE      (115200)             /**< \brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/

/** \brief DMA 重载描述符 */
static amhw_stm32f103rbt6_dma_xfer_desc_t g_desc;

/** \brief 传输完成标志 */
static volatile am_bool_t g_trans_done = AM_FALSE;

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst[]  = "usart tx dma test running\r\n";

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst1[] = "usart tx dma test done\r\n";

/** \brief 目标端数据缓冲区 */
static uint8_t   g_buf_dst2[] = "DMA tx transfer start:\r\n";

static int32_t              g_dma_chan = 0;    /**< \brief DMA 通道号 */
static amhw_stm32f103rbt6_usart_t *gp_hw_usart = NULL; /**< \brief USART 外设 */

/**
 * \brief DMA 平台初始化。
 */
static void __stm32f103rbt6_plfm_dma_init (void)
{
    am_clk_enable(CLK_DMA);
}

/**
 * \brief DMA 平台去初始化。
 */
static void __stm32f103rbt6_plfm_dma_deinit (void)
{
    am_clk_disable(CLK_DMA);
}

/** \brief DMA 设备信息 */
static const am_stm32f103rbt6_dma_devinfo_t __g_dma_devinfo = {
    STM32F103RBT6_DMA_BASE,           /**< \brief 指向DMA寄存器块的指针 */
    INUM_DMA1_1,               /**< \brief DMA中断向量号开始 */
    INUM_DMA1_7,               /**< \brief DMA中断向量号结束 */
    __stm32f103rbt6_plfm_dma_init,    /**< \brief DMA平台初始化 */
    __stm32f103rbt6_plfm_dma_deinit   /**< \brief DMA平台解初始化 */
};

/** \brief DMA设备实例 */
static am_stm32f103rbt6_dma_dev_t __g_dma_dev;

/**
 * \brief DMA中断服务程序。
 *
 * \param[in] p_arg : 用户自定义参数，通过 am_stm32f103rbt6_dma_isr_connect() 函数传递。
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值：
 *                    (# AM_STM32F103RBT6_DMA_INT_NORMAL) 或 (# AM_STM32F103RBT6_DMA_INT_ERROR)
 *
 * \return 无。
 */
static void usart_dma_isr (void *p_arg , uint32_t flag)
{
    int flag_chan  = (int)p_arg;

    if (flag == AM_STM32F103RBT6_DMA_INT_NORMAL) {
        if (flag_chan == g_dma_chan) {

            /* 禁能USART发送时使用DMA传输 */
        	amhw_stm32f103rbt6_usart_dmat_disable(gp_hw_usart);
            g_trans_done = AM_TRUE;
        }
    } else {
        /* 用户自定义执行代码 */
    }

}

/**
 * \brief DMA传输配置。
 */
static int usart_tx_dma_tran_cfg (amhw_stm32f103rbt6_usart_t *p_hw_usart,
                                 int32_t               dma_chan,
                                 const uint8_t        *p_txbuf,
                                 uint32_t              dma_tran_len)
{
    uint32_t flags;

    /* 连接DMA中断服务函数 */
    am_stm32f103rbt6_dma_isr_connect(dma_chan, usart_dma_isr, (void *)dma_chan);

    /* DMA传输配置 */
    flags = AMHW_STM32F103RBT6_DMA_CHAN_PRIORITY_HIGH         |  /* 外设请求源禁能 */
            AMHW_STM32F103RBT6_DMA_CHAN_MEM_SIZE_8BIT         |  /* 源地址1字节 */
            AMHW_STM32F103RBT6_DMA_CHAN_PER_SIZE_8BIT         |  /* 目的地址1字节写入 */
            AMHW_STM32F103RBT6_DMA_CHAN_MEM_ADD_INC_ENABLE    |  /* 请求有影响 */
            AMHW_STM32F103RBT6_DMA_CHAN_PER_ADD_INC_DISABLE   |  /* 无通道连接 */
            AMHW_STM32F103RBT6_DMA_CHAN_CIRCULAR_MODE_DISABLE ,  /* DMA中断使能 */

    /* 建立通道描述符 */
    am_stm32f103rbt6_dma_xfer_desc_build(&g_desc,                        /* 通道描述符 */
                               (uint32_t)(p_txbuf),            /* 源端数据缓冲区 */
                               (uint32_t)(&(p_hw_usart->dr)),  /* 目标端数据缓冲区 */
                                dma_tran_len,                  /* 传输字节数 */
                                flags);                        /* 传输配置 */

    /* 启动DMA传输，马上开始传输 */
    if (am_stm32f103rbt6_dma_xfer_desc_chan_cfg(&g_desc,
                                       AMHW_STM32F103RBT6_DMA_MER_TO_PER,  /* 内存到外设 */
                                       dma_chan) == AM_ERROR) {
        return AM_ERROR;
    }

    return AM_OK;
}

/**
 * \brief USART初始化
 */
static void usart_hw_init (amhw_stm32f103rbt6_usart_t *p_hw_usart, uint32_t clk_rate)
{
	/* 串口的波特率、停止位、数据位、校验位设置 */
	amhw_stm32f103rbt6_usart_baudrate_set(p_hw_usart,clk_rate, USART_BAUDRATE);
    amhw_stm32f103rbt6_usart_stop_bit_sel(p_hw_usart, AMHW_STM32F103RBT6_USART_STOP_10_BIT);
    amhw_stm32f103rbt6_usart_word_length_sel(p_hw_usart, AMHW_STM32F103RBT6_USART_DATA_8BIT);
    amhw_stm32f103rbt6_usart_parity_bit_sel(p_hw_usart,  AMHW_STM32F103RBT6_USART_PARITY_NO);

    /* 串口发送DMA传输使能 */
	amhw_stm32f103rbt6_usart_dmat_enable(p_hw_usart);

    /* 接收、发送使能，串口使能 */
    amhw_stm32f103rbt6_usart_receiver_enable(p_hw_usart);
    amhw_stm32f103rbt6_usart_transmitter_enable(p_hw_usart);
    amhw_stm32f103rbt6_usart_enable(p_hw_usart);
}


/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_hw_usart_tx_dma_entry (amhw_stm32f103rbt6_usart_t *p_hw_usart,
                                        uint32_t             clk_rate,
                                        int32_t              dma_chan)
{
    uint8_t i = 0;
    uint8_t j = 0;

    gp_hw_usart = p_hw_usart;
    g_dma_chan = dma_chan;

    /* USART初始化 */
    usart_hw_init(p_hw_usart, clk_rate);

    am_stm32f103rbt6_dma_init(&__g_dma_dev, &__g_dma_devinfo);
    usart_tx_dma_tran_cfg(p_hw_usart, dma_chan, g_buf_dst2, sizeof(g_buf_dst2) - 1);

    am_stm32f103rbt6_dma_chan_start(dma_chan);

    while (1) {

        if (g_trans_done == AM_TRUE) {

            j++;
            g_trans_done = AM_FALSE;

            /* 串口发送DMA传输使能 */
            amhw_stm32f103rbt6_usart_dmat_enable(p_hw_usart);

            /* 串口发送 */
            amhw_stm32f103rbt6_usart_transmitter_enable(p_hw_usart);

            if (j < 3) {
                am_stm32f103rbt6_dma_chan_stop(dma_chan);
                if (0 == i) {
                    usart_tx_dma_tran_cfg(p_hw_usart,
                                          dma_chan,
                                          g_buf_dst,
                                          sizeof(g_buf_dst) - 1);
                } else {
                    usart_tx_dma_tran_cfg(p_hw_usart,
                                          dma_chan,
                                          g_buf_dst1,
                                          sizeof(g_buf_dst1) - 1);
                }
                i++;
                am_stm32f103rbt6_dma_chan_start(dma_chan);
            }

        }
    }
}
/** [src_stm32f103rbt6_hw_usart_tx_dma] */

/* end of file */
