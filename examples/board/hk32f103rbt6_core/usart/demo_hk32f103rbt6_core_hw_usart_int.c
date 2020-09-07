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
 * \brief USART 中断发送例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口输出 "HW example---USART test in interrupt mode:"；
 *   2. 串口输出接收到的字符。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_3 引脚连接 PC 串口的 TXD，
 *       PIOA_2 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hk32f103rbt6_hw_usart_int.c src_hk32f103rbt6_hw_usart_int
 *
 * \internal
 * \par Modification History
 * - 1.00 17-01-20  ari, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_hk32f103rbt6_hw_usart_int
 * \copydoc demo_hk32f103rbt6_hw_usart_int.c
 */

/** [src_hk32f103rbt6_hw_usart_int] */
#include "ametal.h"
#include "am_hk32f103rbt6.h"
#include "am_board.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "demo_hk32f103rbt6_entries.h"
#include "demo_hk32f103rbt6_core_entries.h"


static void __hk32f103rbt6_usart_pins_intit (void)
{
    /* 初始化引脚 */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_NO_REMAP | PIOA_2_AF_PP );
    am_gpio_pin_cfg(PIOA_3, PIOA_3_NO_REMAP | PIOA_3_INPUT_FLOAT);
}

/**
 * \brief 例程入口
 */
void demo_hk32f103rbt6_core_hw_usart_int_entry (void)
{
    AM_DBG_INFO("demo hk32f103rbt6_core hw usart int!\r\n");

    /* 等待调试串口发送数据完成 */
    am_mdelay(100);

    /* 使能串口时钟 */
    am_clk_enable(CLK_USART2);

    demo_hk32f103rbt6_hw_usart_int_entry((void *)HK32F103RBT6_USART2,
                                   __hk32f103rbt6_usart_pins_intit,
                                   am_clk_rate_get(CLK_USART2),
                                   HK32F103RBT6_USART2_BASE,
                                   INUM_USART2);
}
/** [src_hk32f103rbt6_hw_usart_int] */

/* end of file */
