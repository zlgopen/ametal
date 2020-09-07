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
 * \brief CLK 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口打印各总线频率；
 *   2. 断开J10跳线帽，PLL 时钟 2 分频后从 PIOA_8 输出。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hk32f103rbt6_hw_clk.c src_hk32f103rbt6_hw_clk
 *
 * \internal
 * \par Modification History
 * - 1.00 15-7-13  sss, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hk32f103rbt6_hw_clk
 * \copydoc demo_hk32f103rbt6_hw_clk.c
 */

/** [src_hk32f103rbt6_hw_clk] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_hk32f103rbt6.h"
#include "amhw_hk32f103rbt6_rcc.h"
#include "am_hk32f103rbt6_inst_init.h"
#include "demo_hk32f103rbt6_entries.h"
#include "demo_hk32f103rbt6_core_entries.h"

/** \brief 配置调试串口输出的波特率 */
#define __DEBUG_BAUDRATE        115200

/**
 * \brief 初始化串口 2 为调试串口
 */
am_local void __uart_init (void)
{
    am_uart_handle_t handle = NULL;

    handle = am_hk32f103rbt6_usart2_inst_init();

    /* 调试初始化 */
    am_debug_init(handle, __DEBUG_BAUDRATE);
}


/**
 * \brief 例程入口
 */
void demo_hk32f103rbt6_core_hw_clk_entry (void)
{
    am_clk_id_t clk_id[] = {CLK_PLLIN,
                            CLK_PLLOUT,
                            CLK_AHB,
                            CLK_APB1,
                            CLK_APB2,
                            CLK_HSEOSC,
                            CLK_LSI,
                            CLK_HSI};

    __uart_init();

    AM_DBG_INFO("demo hk32f103rbt6_core hw clk!\r\n");

    am_gpio_pin_cfg(PIOA_8, PIOA_8_MCO | PIOA_8_AF_PP | PIOA_8_SPEED_2MHz);

    amhw_hk32f103rbt6_rcc_mco_src_set(7);

    demo_hk32f103rbt6_hw_clk_entry(&clk_id[0], AM_NELEMENTS(clk_id));

}
/** [src_hk32f103rbt6_hw_clk] */

/* end of file */
