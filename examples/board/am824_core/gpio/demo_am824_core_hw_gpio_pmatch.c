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
 * \brief GPIO 模式匹配引擎演示例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 逻辑表达式为:(/IN0) * IN1 + (/IN3) + (/IN4）*(/IN5)
 *      下列条件任何一个满足触发中断：
 *          - IN0 为低电平且 IN1 都为高电平；
 *          - IN3 为低电平
 *          - IN4 为低电平且 IN5 发生过下降沿事件
 *   2. 中断通道与引脚对应关系：
 *          - IN0 - PIO0_14
 *          - IN1 - PIO0_15
 *          - IN3 - PIO0_16
 *          - IN4 - PIO0_17
 *          - IN5 - PIO0_18
 *
 * - 实验现象：
 *   1. 中断被触发时，LED0 状态翻转，串口打印 "gpio pmatch count %d\r\n"。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am824_hw_gpio_pmatch.c src_am824_hw_gpio_pmatch
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-24  mem, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_gpio_pmatch
 * \copydoc demo_am824_hw_gpio_pmatch.c
 */

/** [src_am824_hw_gpio_pmatch] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc82x_gpio.h"
#include "lpc82x_periph_map.h"
#include "demo_nxp_entries.h"


/** \brief LED0 引脚 */
#define __LED0_PIN  PIO0_20

/**
 * \brief 例程入口
 */
void demo_am824_hw_gpio_pmatch_entry (void)
{

    am_kprintf("demo am824 hw gpio pmatch!\r\n");

    /* 配置 LED0 引脚方向为输出 */
    amhw_lpc82x_gpio_pin_dir_output(LPC82X_GPIO, __LED0_PIN);

    demo_lpc824_hw_gpio_pmatch_entry(__LED0_PIN);

}
/** [src_am824_hw_gpio_pmatch] */

/* end of file */
