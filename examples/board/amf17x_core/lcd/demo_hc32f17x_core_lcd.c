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
 * \brief LCD 控制器显示例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   在LCD屏上循环显示数字"0000~9999"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f17x_core_lcd.c src_hc32f17x_core_lcd
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-16  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f17x_core_lcd
 * \copydoc demo_hc32f17x_core_lcd.c
 */

/** [src_hc32f17x_hw_lcd] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "hc32_pin.h"
#include "am_hc32_lcd.h"
#include "demo_hc32_entries.h"
/**
 * \brief 显示数据
 *
 * \note : LCD屏一共四个数字显示单元，LCD每个数字显示由两个字节的各低四位控制,
 *         例如显示一个数字3则 lcd_data = {0x00000000,0x00000f02};
 *         数字0-9 数据码：0-------0x0f05
 *                     1-------0x0600
 *                     2-------0x0b06
 *                     3-------0x0f02
 *                     4-------0x0603
 *                     5-------0x0d03
 *                     6-------0x0d07
 *                     7-------0x0700
 *                     8-------0x0f07
 *                     9-------0x0f03
 *                     .-------0x0008
 *                     :-------0x0008
 *
 */
static uint32_t lcd_data[12] = {
    0x0f050f05, 0x06000600, 0x0b060b06, 0x0f020f02, 0x06030603,
    0x0d030d03, 0x0d070d07, 0x07000700, 0x0f070f07, 0x0f030f03,
    0x00080008, 0x00080008
};

/**
 * \brief 例程入口
 */
void demo_hc32f17x_core_hw_lcd_entry (void)
{
    AM_DBG_INFO("Test start: demo-lcd-mode !\r\n");

    /* 初始化LCD外部GPIO引脚 */
    am_gpio_pin_cfg(PIOA_9,  PIOA_9_AIN);  /**< \brief COM0 */
    am_gpio_pin_cfg(PIOA_10, PIOA_10_AIN); /**< \brief COM1 */
    am_gpio_pin_cfg(PIOA_11, PIOA_11_AIN); /**< \brief COM2 */
    am_gpio_pin_cfg(PIOA_12, PIOA_12_AIN); /**< \brief COM3 */

    am_gpio_pin_cfg(PIOA_8,  PIOA_8_AIN);  /**< \brief SEG0 */
    am_gpio_pin_cfg(PIOC_9,  PIOC_9_AIN);  /**< \brief SEG1 */
    am_gpio_pin_cfg(PIOC_8,  PIOC_8_AIN);  /**< \brief SEG2 */
    am_gpio_pin_cfg(PIOC_7,  PIOC_7_AIN);  /**< \brief SEG3 */
    am_gpio_pin_cfg(PIOC_6,  PIOC_6_AIN);  /**< \brief SEG4 */
    am_gpio_pin_cfg(PIOB_15, PIOB_15_AIN); /**< \brief SEG5 */
    am_gpio_pin_cfg(PIOB_14, PIOB_14_AIN); /**< \brief SEG6 */
    am_gpio_pin_cfg(PIOB_13, PIOB_13_AIN); /**< \brief SEG7 */
    am_gpio_pin_cfg(PIOB_3,  PIOB_3_AIN);  /**< \brief VLCDH */
    am_gpio_pin_cfg(PIOB_4,  PIOB_4_AIN);  /**< \brief VLCD3 */
    am_gpio_pin_cfg(PIOB_5,  PIOB_5_AIN);  /**< \brief VLCD2 */
    am_gpio_pin_cfg(PIOB_6,  PIOB_6_AIN);  /**< \brief VLCD1 */

    demo_hc32_hw_lcd_entry(HC32_LCD, lcd_data);
}

/* end of file */
