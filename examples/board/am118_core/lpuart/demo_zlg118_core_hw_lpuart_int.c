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
 * \brief LPUART 中断发送例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "HW example---LPUART test in interrupt mode:"；
 *   2. 串口输出接收到的字符。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOB_11 引脚连接 PC 串口的 TXD，
 *       PIOB_12 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_lpuart_int.c src_zlg118_hw_lpuart_int
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-26  zp, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_zlg118_hw_lpuart_int
 * \copydoc demo_zlg118_hw_lpuart_int.c
 */

/** [src_zlg118_hw_lpuart_int] */
#include "ametal.h"
#include "am_zlg118.h"
#include "am_board.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "demo_zlg_entries.h"
#include "demo_am118_core_entries.h"

static void __zlg118_lpuart_pins_intit (void)
{
    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOB_11, PIOB_11_LPUART0_RXD | PIOB_11_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOB_12, PIOB_12_LPUART0_TXD | PIOB_12_OUT_PP );
}

/**
 * \brief 例程入口
 */
void demo_zlg118_core_hw_lpuart_int_entry (void)
{
    AM_DBG_INFO("demo am118_core hw lpuart int!\r\n");

     /* 等待发送数据完成 */
    am_mdelay(100);

    /* 使能时钟 */
    am_clk_enable(CLK_LPUART0);

    demo_zlg118_hw_lpuart_int_entry(ZLG118_LPUART0,
                                    __zlg118_lpuart_pins_intit,
                                    AMHW_ZLG118_LPUART_SCLK_SRC_PCLK,
                                    16000000,
                                    115200,
                                    ZLG118_LPUART0_BASE,
                                    INUM_LPUART0);
}
/** [src_zlg118_hw_lpuart_int] */

/* end of file */
