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
 * \brief UART中断方式下接收发送数据例程，通过HW层的接口实现
 *
 * - 操作步骤：
 *   1. PIOC_3 引脚连接PC串口的TXD;
 *   2. PIOC_4 引脚连接PC串口的RXD。
 *   3. 配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *    
 * - 实验现象：
 *   1. 串口输出"HW example---UART test in interrupt mode:"；
 *   2. 串口输出接收到的字符。
 *
 * \note 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）
 *
 * \par 源代码
 * \snippet demo_fsl_hw_uart_int.c src_fsl_hw_uart_int
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-19  nwt, make some changes.
 * - 1.00 15-10-20  xym, first implementation.
 * \endinternal
 */
 
 /**
 * \addtogroup demo_fsl_if_hw_uart_int
 * \copydoc demo_fsl_hw_uart_int.c
 */ 

/** [src_fsl_hw_uart_int] */
#include "ametal.h"
#include "hw/amhw_fsl_uart.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define UART_BAUDRATE     (115200)              /** <\brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/

static const uint8_t __g_hw_polling_str[]={"HW example---UART test in interrupt mode:\r\n"};

static const uint8_t   *__gp_tx_buf   = NULL; /**< \brief 发送缓冲区.        */
static volatile uint8_t __g_tx_index  = 0;    /**< \brief 当前发送数据索引.   */
static volatile uint8_t __g_tx_nbytes = 0;    /**< \brief 需要发送的总字节数. */
static am_bool_t       __g_uart0_flog = 0;    /**< \brief 串口0标志.       */
/** 
 * \brief 串口发送数据（中断模式下，该函数不会阻塞）
 *
 * \param[in] p_hw_uart : 指向需控制的串口设备结构体
 * \param[in] p_buf     : 指向发送数据的缓冲区
 * \param[in] len       : 发送的字节数
 * 
 * \retval    1         : 发送就绪
 * \retval    0         : 发送模块忙碌，无法发送
 * 
 * \note 发送缓冲区最好是全局的，保证在所有数据发送完成前，缓冲区没有被释放掉 
 */
static uint8_t __uart_int_send(amhw_fsl_uart_t *p_hw_uart, const uint8_t *p_buf, uint32_t len)
{
    if (__g_tx_index >= __g_tx_nbytes)  {   /* 上次发送已经发送完成     */
        __gp_tx_buf   = p_buf;            /* 重新定义发送缓冲器区     */
        __g_tx_index  = 0;                /* 从缓存区起始字节开始发送 */
        __g_tx_nbytes = len;              /* 发送g_tx_nbytes个字节    */

        amhw_fsl_uart_int_enable(p_hw_uart, AMHW_FSL_UART_INT_C2_TIE);

        return 1;
    } 

    return 0;
}

/** 
 * \brief 串口中断服务函数
 * \param[in] p_arg : 指向需控制的串口设备结构体 ，在am_int_connect()注册
 */
static void __uart_hw_irq_handler (void *p_arg)
{
    amhw_fsl_uart_t *p_hw_uart = (amhw_fsl_uart_t *)p_arg;

    uint32_t uart_int_stat;

    uint8_t data;
    
    if (__g_uart0_flog) {
        uart_int_stat = amhw_fsl_uart_ver0_intstat_get(p_hw_uart);
    } else {
        uart_int_stat = amhw_fsl_uart_ver1_intstat_get(p_hw_uart);
    }

    if (uart_int_stat & AMHW_FSL_UART_INTSTAT_S1_RDRF) {
        /* 获取新接收数据，并发送出去 */
        data = amhw_fsl_uart_rxdata_read(p_hw_uart);
        amhw_fsl_uart_txdata_write(p_hw_uart, data);

    } else if (uart_int_stat & AMHW_FSL_UART_INTSTAT_S1_TDRE) {
        /* 发送中断 */
        if(__g_tx_index < __g_tx_nbytes) {
            /* 缓冲区还有待发送数据 */
            amhw_fsl_uart_txdata_write(p_hw_uart, __gp_tx_buf[__g_tx_index++]);

        } else {
            /* 缓冲区没有发送数据，关闭发送中断 */
            amhw_fsl_uart_int_disable(p_hw_uart, AMHW_FSL_UART_INT_C2_TIE);
        } 
    } 
}

/**
 * \brief UART hw 中断收发初始化
 */
static void __uart_int_init (amhw_fsl_uart_t *p_hw_uart,
                             uint32_t         uart_clk,
                             int              inum)
{

    amhw_fsl_uart_stop_bit_set (p_hw_uart, AMHW_FSL_UART_BDH_SBNS_STOP_1);
    amhw_fsl_uart_data_mode_set(p_hw_uart, AMHW_FSL_UART_C1_M_8BIT);
    amhw_fsl_uart_parity_set(p_hw_uart,    AMHW_FSL_UART_C1_PARITY_NO);

    /* 设置串口波特率 */
    if (__g_uart0_flog) {
            amhw_fsl_uart_ver0_baudrate_set(p_hw_uart,
                                            uart_clk,
                                            UART_BAUDRATE);
    } else {
        amhw_fsl_uart_ver1_baudrate_set(p_hw_uart,
                                        uart_clk,
                                        UART_BAUDRATE);
    }

    /* 关闭所有串口中断 */
    amhw_fsl_uart_int_disable(p_hw_uart, AMHW_FSL_UART_INT_ALL);

    /* 关联中断向量号，开启中断 */
    am_int_connect(inum, __uart_hw_irq_handler, (void *)p_hw_uart);
    am_int_enable(inum);

    /* 使能串口.    */
    amhw_fsl_uart_enable(p_hw_uart);
    
    /* 使能RDRF接收准中断 */
    amhw_fsl_uart_int_enable(p_hw_uart, AMHW_FSL_UART_INT_C2_IRIE);
}

/**
 * \brief 例程入口
 */
void demo_fsl_hw_uart_int_entry (amhw_fsl_uart_t *p_hw_uart,
                                 int              inum,
                                 uint32_t         uart_clk,
                                 am_bool_t        uart0_flog)
{
    __g_uart0_flog = uart0_flog;

    /* UART中断初始化 */
    __uart_int_init(p_hw_uart, uart_clk, inum);
    
    __uart_int_send(p_hw_uart, __g_hw_polling_str, sizeof(__g_hw_polling_str));
    
    while (1) {

    }
}
/** [src_fsl_hw_uart_int] */

/* end of file */
