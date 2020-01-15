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
 * \brief UART 中断发送例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 指定串口输出 "HW example---UART test in interrupt mode:"；
 *   2. 指定串口输出接收到的字符。
 *
 * \note
 *   1.若该串口与调试串口一致，需关闭调试串口以免影响效果
 *
 * \par 源代码
 * \snippet demo_mm32_hw_uart_int.c src_mm32_hw_uart_int
 *
 * \internal
 * \par Modification History
 * - 1.00 17-01-20  ari, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_mm32_hw_uart_int
 * \copydoc demo_mm32_hw_uart_int.c
 */

/** [src_mm32_hw_uart_int] */
#include "ametal.h"
#include "am_int.h"
#include "hw/amhw_mm32_uart.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define UART_BAUDRATE      (115200)           /**< \brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/

static const uint8_t hw_polling_str[] =
                              {"HW example---UART test in interrupt mode:\r\n"};

static const uint8_t   *gp_tx_buf   = NULL; /**< \brief 发送缓冲区 */
static volatile uint8_t g_tx_index  = 0;    /**< \brief 当前发送数据索引 */
static volatile uint8_t g_tx_nbytes = 0;    /**< \brief 需要发送的总字节数 */

/**
 * \brief 串口发送数据（中断模式下，该函数不会阻塞）
 *
 * \param[in] p_hw_uart指向需控制的串口设备结构体
 * \param[in] p_buf    指向发送数据的缓冲区
 * \param[in] len      发送的字节数
 *
 * \retval 1 发送就绪
 * \retval 0 发送模块忙碌，无法发送
 *
 * \note 发送缓冲区最好是全局的，保证在所有数据发送完成前，缓冲区没有被释放掉
 */
static uint8_t uart_int_send (amhw_mm32_uart_t *p_hw_uart,
                              const uint8_t   *p_buf,
                              uint32_t         len)
{
    if (g_tx_index >= g_tx_nbytes)  {   /* 上次发送已经发送完成 */
        gp_tx_buf   = p_buf;            /* 重新定义发送缓冲器区 */
        g_tx_index  = 0;                /* 从缓存区起始字节开始发送 */
        g_tx_nbytes = len;              /* 发送g_tx_nbytes个字节 */

        /* 必须先写一个一个字节的数据才以产生发送中断 */
        while(amhw_mm32_uart_status_flag_check(p_hw_uart,
                                AMHW_MM32_UART_TX_COMPLETE_FALG) == AM_FALSE);

        amhw_mm32_uart_data_write(p_hw_uart, p_buf[g_tx_index++]);

        amhw_mm32_uart_int_enable(p_hw_uart,
                                    AMHW_MM32_UART_INT_TX_EMPTY_ENABLE);

        return 1;
    }

    return 0;
}

/**
 * \brief 串口中断服务函数
 *
 * \param[in] p_arg 指向需控制的串口设备结构体 ，在 am_int_connect() 注册
 */
static void uart_hw_irq_handler (void *p_arg)
{
    amhw_mm32_uart_t *p_hw_uart = (amhw_mm32_uart_t *)p_arg;
    uint8_t          data;

    if (amhw_mm32_uart_int_flag_check(p_hw_uart,
                                     AMHW_MM32_UART_INT_RX_VAL_FLAG) == AM_TRUE) {
        amhw_mm32_uart_int_flag_clr(p_hw_uart, AMHW_MM32_UART_INT_RX_VAL_FLAG);

        /* 获取新接收数据，并发送出去 */
        data = amhw_mm32_uart_data_read(p_hw_uart);
        amhw_mm32_uart_data_write(p_hw_uart, data);

    } else if (amhw_mm32_uart_int_flag_check(p_hw_uart,
                               AMHW_MM32_UART_INT_TX_EMPTY_FLAG) == AM_TRUE) {
        amhw_mm32_uart_int_flag_clr(p_hw_uart,
                                   AMHW_MM32_UART_INT_TX_EMPTY_FLAG);

        /* 发送中断 */
        if(g_tx_index < g_tx_nbytes) {

            /* 缓冲区还有待发送数据 */
            amhw_mm32_uart_data_write(p_hw_uart, gp_tx_buf[g_tx_index++]);

        } else {

            /* 缓冲区没有发送数据，关闭发送中断 */
            amhw_mm32_uart_int_disable(p_hw_uart,
                                      AMHW_MM32_UART_INT_TX_EMPTY_ENABLE);
        }
    }
}

/**
 * \brief UART hw 中断收发初始化
 */
void uart_int_init (amhw_mm32_uart_t *p_hw_uart,
                    void (* pfn_init)(void),
                    uint32_t         clk_rate,
                    unsigned long    uart_base,
                    unsigned char    inum_uart)
{
    uint8_t inum = 0;

    amhw_mm32_uart_stop_bit_sel(p_hw_uart, AMHW_MM32_UART_STOP_1BIT);
    amhw_mm32_uart_data_length(p_hw_uart, AMHW_MM32_UART_DATA_8BIT);
    amhw_mm32_uart_parity_bit_sel(p_hw_uart,  AMHW_MM32_UART_PARITY_NO);

    /* 设置串口波特率 */
    amhw_mm32_uart_baudrate_set(p_hw_uart, clk_rate, UART_BAUDRATE);

    /* 关闭所有串口中断 */
    amhw_mm32_uart_int_disable(p_hw_uart, AMHW_MM32_UART_INT_ALL_ENABLE_MASK);

    /* 计算出不同串口对应的中断向量号 */
    if ((uint32_t)p_hw_uart - uart_base) {
        inum = inum_uart + 1;
    } else {
        inum = inum_uart;
    }

    /* 使能串口 */
    amhw_mm32_uart_rx_enable(p_hw_uart,AM_TRUE);
    amhw_mm32_uart_tx_enable(p_hw_uart,AM_TRUE);
    amhw_mm32_uart_enable(p_hw_uart);

    /* 使能RDRF接收准中断 */
    amhw_mm32_uart_int_enable(p_hw_uart, AMHW_MM32_UART_INT_RX_VAL_ENABLE);

    /* 关联中断向量号，开启中断 */
    am_int_connect(inum, uart_hw_irq_handler, (void *)p_hw_uart);
    am_int_enable(inum);
    if(pfn_init != NULL){
        pfn_init();
    }
}

/**
 * \brief 例程入口
 */
void demo_mm32_hw_uart_int_entry (amhw_mm32_uart_t *p_hw_uart, 
                                 void (* pfn_init)(void),
                                 uint32_t         clk_rate,
                                 unsigned long    uart_base,
                                 unsigned char    inum_uart)
{
    if (pfn_init == NULL) {
        return;
    }
    uart_int_init(p_hw_uart, pfn_init, clk_rate , uart_base, inum_uart);

    uart_int_send(p_hw_uart, hw_polling_str, sizeof(hw_polling_str));

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_mm32_hw_uart_int] */

/* end of file */
