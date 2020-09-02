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
 * \brief USART 轮询方式例程，通过 HW 层接口实现
 *
 *
 * - 实验现象：
 *   1. 串口输出 "HW example---USART test in polling mode:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_3 引脚连接 PC 串口的 TXD，
 *       PIOA_2 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zlg237_hw_usart_polling.c src_zlg237_hw_usart_polling
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-17  ari, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_zlg237_hw_usart_polling
 * \copydoc demo_zlg237_hw_usart_polling.c
 */

/** [src_zlg237_hw_usart_polling] */
#include "ametal.h"
#include "am_board.h"
#include "am_zlg237.h"
#include "am_gpio.h"
#include "demo_zlg_entries.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg237_core_hw_usart_polling_entry (void)
{
    AM_DBG_INFO("demo am237_core hw uart polling!\r\n");

    /* 等待调试串口发送数据完成 */
    am_mdelay(100);

    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_NO_REMAP | PIOA_2_AF_PP );
    am_gpio_pin_cfg(PIOA_3, PIOA_3_NO_REMAP | PIOA_3_INPUT_FLOAT);

    /* 使能时钟 */
    am_clk_enable(CLK_USART2);

    demo_zlg237_hw_usart_polling_entry((void *)ZLG237_USART2, am_clk_rate_get(CLK_USART2));
}
/** [src_zlg237_hw_usart_polling] */

/* end of file */
