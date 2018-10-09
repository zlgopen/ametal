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
 * \snippet demo_amks16z_core_hw_uart_int.c src_amks16z_core_hw_uart_int
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-19  nwt, make some changes.
 * - 1.00 15-10-20  xym, first implementation.
 * \endinternal
 */
 
 /**
 * \addtogroup demo_amks16z_core_if_hw_uart_int
 * \copydoc demo_amks16z_core_hw_uart_int.c
 */ 

/** [src_amks16z_core_hw_uart_int] */
#include "ametal.h"
#include "hw/amhw_fsl_uart.h"
#include "hw/amhw_kl26_sim.h"
#include "am_kl26_clk.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"
#include "../../../../soc/freescale/kl26/kl26_pin.h"

/**
 * \brief UART时钟使能初始化
 * \param[in] p_hw_uart : 指向串口设备寄存器结构体, 如KL26_UART1.
 */
static void __uart_int_clock_init (void *p_hw_uart)
{
    uint32_t base_addr = (uint32_t)(p_hw_uart);

    switch (base_addr) {

    /* 串口0时钟初始化 */
    case KL26_UART0_BASE:
        amhw_kl26_sim_uart0_src_set(KL26_SIM_UART0SRC_PLLFLLCLK);
        /* 开启UART0时钟                  */
        amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_UART0);
        break;

    /* 串口1时钟初始化 */
    case KL26_UART1_BASE:
        amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_UART1);
        break;

    /* 串口2时钟初始化 */
    case KL26_UART2_BASE:
        amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_UART2);
        break;
    }
}

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_uart_int_entry (void)
{
    uint32_t uart_clk;

    __uart_int_clock_init(KL26_UART1);

    /* 引脚初始化      PIOC_3_UART1_RX  PIOC_4_UART1_TX     */
    am_gpio_pin_cfg (PIOC_3,PIOC_3_UART1_RX);
    am_gpio_pin_cfg (PIOC_4,PIOC_4_UART1_TX);

    uart_clk = am_kl26_clk_periph_rate_get((void *)(KL26_UART1));

    demo_fsl_hw_uart_int_entry(KL26_UART1, INUM_UART1, uart_clk, 0);
}
/** [src_amks16z_core_hw_uart_int] */

/* end of file */
