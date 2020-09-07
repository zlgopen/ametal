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
 * \brief LPUART 轮询方式例程，通过 HW 层接口实现
 *
 *
 * - 实验现象：
 *   1. 串口输出 "HW example---LPUART test in polling mode:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOB_11 引脚连接 PC 串口的 TXD，
 *       PIOB_12 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f19x_hw_lpuart_polling.c src_hc32f19x_hw_lpuart_polling
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-26  zp, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_hc32f19x_hw_lpuart_polling
 * \copydoc demo_hc32f19x_hw_lpuart_polling.c
 */

/** [src_hc32f19x_hw_lpuart_polling] */
#include "ametal.h"
#include "am_board.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "demo_hc32_entries.h"
#include "demo_amf19x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f19x_core_hw_lpuart_polling_entry (void)
{
    AM_DBG_INFO("demo amf19x_core hw lpuart polling!\r\n");

     /* 等待发送数据完成 */
    am_mdelay(100);

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOB_11, PIOB_11_LPUART0_RXD | PIOB_11_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOB_12, PIOB_12_LPUART0_TXD | PIOB_12_OUT_PP );

    /* 使能时钟 */
    am_clk_enable(CLK_LPUART0);

    demo_hc32_hw_lpuart_polling_entry(HC32_LPUART0,
                                      AMHW_HC32_LPUART_SCLK_SRC_PCLK,
                                      am_clk_rate_get(CLK_LPUART0),
                                      115200);
}
/** [src_hc32f19x_hw_lpuart_polling] */

/* end of file */
