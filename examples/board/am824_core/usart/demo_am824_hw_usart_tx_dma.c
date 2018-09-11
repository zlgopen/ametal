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
 * \snippet demo_am824_hw_usart_tx_dma.c src_am824_hw_usart_tx_dma
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-15  bob, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_am824_hw_usart_tx_dma
 * \copydoc demo_am824_hw_usart_tx_dma.c
 */

/** [src_am824_hw_usart_tx_dma] */
#include "ametal.h"
#include "hw/amhw_lpc_usart.h"
#include "hw/amhw_lpc82x_clk.h"
#include "am_vdebug.h"
#include "demo_nxp_entries.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/**
 * \brief 基本输入频率设置（基本输入频率必须小于系统时钟频率且应为波特率的整数倍）
 *
 * 为了设置波特率为 115200，故设置串口基本输入频率为：
 * 11059200Hz(11059200 = 115200 * 96)。
 * 串口基本输入频率设置为 11.059200MHz，可满足大多数波特率的设置 (9600,4800,115200)
 *
 * \note  USART0\1\2 共用一个基本输入频率,不要轻易改动
 */
#define  __LPC82X_UASART_BASE_RATE  11059200

/** \brief 波特率 */
#define __USART_BAUDRATE      115200

/**
 * \brief 例程入口
 */
void demo_am824_hw_usart_tx_dma_entry (void)
{
    am_kprintf("demo am824 hw usart tx dma!\r\n");
  
    /* 设置串口基础时钟 */
    amhw_lpc82x_clk_usart_baseclkrate_set(__LPC82X_UASART_BASE_RATE);

    /* 使能串口 0 时钟并复位 */
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_UART0);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_UART0);

    /* 配置引脚复用 */
    am_gpio_pin_cfg(PIO0_4, PIO_FUNC_U0_TXD);
    am_gpio_pin_cfg(PIO0_0, PIO_FUNC_U0_RXD);
    /* USART 初始化 */
    demo_lpc824_hw_usart_tx_dma_entry(LPC82X_USART0, 
                                      amhw_lpc82x_clk_usart_baseclkrate_get(),
                                      __USART_BAUDRATE,
                                      DMA_CHAN_USART0_TX_REQ);


}
/** [src_am824_hw_usart_tx_dma] */

/* end of file */
