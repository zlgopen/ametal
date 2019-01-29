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
 * \brief 时钟频率输出例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口打印 "The CLKOUT  rate is  xxx Hz\r\n"，xxx 为当前频率；
 *   2. PIO0_27 引脚（默认配置 CLKOUT 引脚输出）输出当前时钟频率 2.5MHz；
 *   3. LED0 以 0.2s 的时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_clkout.c src_lpc824_hw_clkout
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_clkout
 * \copydoc demo_lpc824_hw_clkout.c
 */

/** [src_lpc824_hw_clkout] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc84x_clk.h"


/** \brief 时钟源定义 */
#define __CLKOUT_SRC  AMHW_LPC84X_CLKOUT_SRC_MAIN

/** \brief 时钟分频参数定义 */
#define __CLKOUT_DIV  10

/** \brief 时钟分频参数定义 */
#define __RETE        2

#define __DIV         2

/**
 * \brief 例程入口
 */
void demo_lpc845_hw_clkout_entry (void)
{
    /* 开启内部 IRC */
    am_kprintf("demo am845 hw clkout!\r\n");

    /* Configure the SWM for clock out */
    am_gpio_pin_cfg(16, PIO_FUNC_CLKOUT);

    amhw_lpc84x_outclk_div_set(__CLKOUT_DIV);

    amhw_lpc84x_outclk_src_set(__CLKOUT_SRC);

    if(__CLKOUT_SRC == AMHW_LPC84X_CLKOUT_SRC_FRO_WDT_OSC_CLK) {
        amhw_lpc84x_clk_wdtoscc_cfg(__RETE, __DIV);
		amhw_lpc84x_syscon_powerup(AMHW_LPC84X_SYSCON_PD_WDT_OSC);
    }

    AM_DBG_INFO("The CLKOUT  rate is  %8d Hz\r\n",
    		amhw_lpc84x_outclk_clkrate_get() / __CLKOUT_DIV);

    AM_FOREVER {
        am_led_on(LED0);
        am_mdelay(200);
        am_led_off(LED0);
        am_mdelay(200);
    }
}
/** [src_lpc824_hw_clkout] */

/* end of file */
