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
 * \brief GPIO 例程，通过 HW 层接口实现
 *
 *
 * - 实验现象：
 *   1. 按一次按键 LED0 灯熄灭，再按一次按键 LED0 灯亮，如此反复。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32l13x_hw_gpio.c src_hc32l13x_hw_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-20 zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l13x_hw_gpio
 * \copydoc demo_hc32l13x_hw_gpio.c
 */

/** [src_hc32l13x_hw_gpio] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_hc32.h"
#include "am_vdebug.h"
#include "hw/amhw_hc32_gpio.h"
#include "demo_hc32_entries.h"
#include "demo_aml13x_core_entries.h"

#define INPUT_PIN  PIOA_15  /**< \brief 输入引脚 */
#define OUTPUT_PIN PIOD_5   /**< \brief 输出引脚 */

/**
 * \brief 例程入口
 */
void demo_hc32l13x_core_hw_gpio_entry (void)
{

    AM_DBG_INFO("demo aml13x_core hw gpio!\r\n");

    /* 开启 GPIOC 端口时钟 */
    am_clk_enable(CLK_GPIO);

    /* 开启系统配置的时钟 */
    am_clk_enable(CLK_GPIO);

    demo_hc32_hw_gpio_entry((void *)HC32_GPIO0, INPUT_PIN, OUTPUT_PIN);
}
/** [src_hc32l13x_hw_gpio] */

/* end of file */
