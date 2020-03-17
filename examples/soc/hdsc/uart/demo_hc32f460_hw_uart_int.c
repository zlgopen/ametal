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
 * \snippet demo_hc32f460_hw_uart_int.c src_hc32f460_hw_uart_int
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-06  cds, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_hc32f460_hw_uart_int
 * \copydoc demo_hc32f460_hw_uart_int.c
 */

/** [src_hc32f460_hw_uart_int] */
#include "ametal.h"
#include "am_int.h"
#include "hw/amhw_hc32f460_uart.h"
#include "hc32f460_irq_handle.h"

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
static uint8_t uart_int_send (amhw_hc32f460_uart_t *p_hw_uart,
                              const uint8_t        *p_buf,
                              uint32_t             len)
{
    if (g_tx_index >= g_tx_nbytes)  {   /* 上次发送已经发送完成 */
        gp_tx_buf   = p_buf;            /* 重新定义发送缓冲器区 */
        g_tx_index  = 0;                /* 从缓存区起始字节开始发送 */
        g_tx_nbytes = len;              /* 发送g_tx_nbytes个字节 */

        while(amhw_hc32f460_uart_status_flag_check(p_hw_uart,
                AMHW_HC32F460_UART_TX_EMPTY_FLAG) ==
                                          AM_FALSE);

        /* 必须先写一个一个字节的数据才可以产生发送空中断 */
        amhw_hc32f460_uart_data_write(p_hw_uart, p_buf[g_tx_index++]);

        amhw_hc32f460_uart_int_enable(p_hw_uart,
                AMHW_HC32F460_UART_TX_EMPTY_FLAG);

        return 1;
    }

    return 0;
}

/**
 * \brief 串口接收中断服务函数
 *
 * \param[in] p_arg 指向需控制的串口设备结构体 ，在 am_int_connect() 注册
 */
static void uart_hw_irq_rx_handler (void *p_arg)
{
    amhw_hc32f460_uart_t *p_hw_uart = (amhw_hc32f460_uart_t *)p_arg;
    uint8_t               data;

    /* 获取新接收数据，并发送出去 */
    data = amhw_hc32f460_uart_data_read(p_hw_uart);

    amhw_hc32f460_uart_data_write(p_hw_uart, data);
}

/**
 * \brief 串口发送中断服务函数
 *
 * \param[in] p_arg 指向需控制的串口设备结构体 ，在 am_int_connect() 注册
 */
static void uart_hw_irq_tx_handler (void *p_arg)
{
    amhw_hc32f460_uart_t *p_hw_uart = (amhw_hc32f460_uart_t *)p_arg;

    /* 发送中断 */
    if(g_tx_index < g_tx_nbytes) {

     /* 缓冲区还有待发送数据 */
      amhw_hc32f460_uart_data_write(p_hw_uart, gp_tx_buf[g_tx_index++]);
      } else {
      /* 缓冲区没有发送数据，关闭发送中断 */
      amhw_hc32f460_uart_int_disable(p_hw_uart,
              AMHW_HC32F460_UART_INT_TX_EMPTY_ENABLE);
    }
}




/**
 * \brief UART hw 中断收发初始化
 */
void uart_int_init (amhw_hc32f460_uart_t *p_hw_uart,
                    void               (* pfn_init)(void),
                    int                   clk_rate,
                    unsigned long         uart_base)
{
    /* 停止位设置 */
    amhw_hc32f460_uart_stop_bit_sel(p_hw_uart  , AMHW_HC32F460_UART_STOP_1BIT);

    /* 校验设置 */
    amhw_hc32f460_uart_parity_bit_sel(p_hw_uart, AMHW_HC32F460_UART_PARITY_NO);

    /* 波特率设置 */
    amhw_hc32f460_uart_baudrate_set(p_hw_uart, clk_rate, UART_BAUDRATE);

    uint32_t enIRQ_RX = 16;
    uint32_t enIRQ_TX = 17;
    uint32_t INT_USART4_RI = 294u;
    uint32_t INT_USART4_TI = 295u;
    stc_intc_sel_field_t *stcIntSel;

    stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * enIRQ_RX));
    stcIntSel->INTSEL = INT_USART4_RI;
    am_int_connect(enIRQ_RX, uart_hw_irq_rx_handler, (void *)p_hw_uart);
    am_int_enable(enIRQ_RX);

    stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * enIRQ_TX));
    stcIntSel->INTSEL = INT_USART4_TI;
    am_int_connect(enIRQ_TX, uart_hw_irq_tx_handler, (void *)p_hw_uart);
    am_int_enable(enIRQ_TX);

    /* 接收完成中断使能 */
    amhw_hc32f460_uart_int_enable(p_hw_uart, AMHW_HC32F460_UART_RX_VAL_FLAG);

    /* 接收、发送使能 */
    amhw_hc32f460_uart_rx_enable (p_hw_uart, AM_TRUE);
    amhw_hc32f460_uart_tx_enable (p_hw_uart, AM_TRUE);

    if(pfn_init != NULL){
        pfn_init();
    }
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_hw_uart_int_entry (void             *p_hw_uart,
                                      void (* pfn_init)(void),
                                      int              clk_rate,
                                      unsigned long    uart_base)
{
    amhw_hc32f460_uart_t *p_uart = (amhw_hc32f460_uart_t *)p_hw_uart;

    if (pfn_init == NULL) {
        return;
    }
    uart_int_init(p_uart, pfn_init, clk_rate , uart_base);

    uart_int_send(p_uart, hw_polling_str, sizeof(hw_polling_str));

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32f460_hw_uart_int] */

/* end of file */
