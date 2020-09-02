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
 * \brief GPIO 引脚中断例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将 J14 的 KEY 和 PIOC_7 短接在一起。
 *
 * - 实验现象：
 *    每次按键时串口打印 "the gpio interrupt happen!"。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIOC_9 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zlg237_hw_gpio_trigger.c src_zlg237_hw_gpio_trigger
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-24  zcb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg237_hw_gpio_trigger
 * \copydoc demo_zlg237_hw_gpio_trigger.c
 */

/** [src_zlg237_hw_gpio_trigger] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_zlg237.h"
#include "amhw_zlg237_gpio.h"
#include "amhw_zlg237_exti.h"
#include "amhw_zlg237_afio.h"
#include "demo_zlg_entries.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg237_core_hw_gpio_trigger_entry (void)
{

    AM_DBG_INFO("demo am237_core hw gpio trigger!\r\n");

    /* 开启 GPIOC 端口时钟 */
    am_clk_enable(CLK_IOPC);

    am_clk_enable(CLK_AFIO);

    demo_zlg_hw_gpio_trigger_entry((amhw_zlg_gpio_t *)ZLG237_GPIO,
                                   (amhw_zlg_syscfg_t *)ZLG237_AFIO,
                                   (amhw_zlg_exti_t *)ZLG237_EXTI,
                                   PIOC_7,
                                   AMHW_ZLG_SYSCFG_EXTI_PORTSOURCE_GPIOC,
                                   AMHW_ZLG_SYSCFG_EXTI_PINSOURCE_7,
                                   (amhw_zlg_exti_line_num_t)AMHW_ZLG237_LINE_NUM7);
}
/** [src_zlg237_hw_gpio_trigger] */

/* end of file */
