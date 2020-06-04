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
 *   1. 将 J14 的 KEY 和 PIOA_13 短接在一起。
 *
 * - 实验现象：
 *   1. 按一次按键 LED0 灯熄灭，再按一次按键 LED0 灯亮，如此反复；
 *   2. 每次按键时串口打印 "the gpio interrupt happen!"。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIOB_3 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_13 引脚连接 PC 串口的 RXD。
 *    3. 该引脚与SWD引脚复用，下载需拔掉SWD连接线方能正常工作
 * \par 源代码
 * \snippet demo_aml165_core_hw_gpio_trigger.c src_aml165_core_hw_gpio_trigger
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_hw_gpio_trigger
 * \copydoc demo_aml165_core_hw_gpio_trigger.c
 */

/** [src_aml165_core_hw_gpio_trigger] */
#include "ametal.h"
#include "am_clk.h"
#include "am_vdebug.h"
#include "am_zml165.h"
#include "hw/amhw_zlg_exti.h"
#include "hw/amhw_zlg_syscfg.h"
#include "demo_zlg_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_hw_gpio_trigger_entry (void)
{
    AM_DBG_INFO("demo aml165_core hw gpio trigger!\r\n");

    /* 使能时钟 */
    am_clk_enable(CLK_GPIOA);
    am_clk_enable(CLK_SYSCFG);

    demo_zlg_hw_gpio_trigger_entry(ZML165_GPIO,
                                   ZML165_SYSCFG,
                                   ZML165_EXTI,
                                   PIOA_13,
                                   AMHW_ZLG_SYSCFG_EXTI_PORTSOURCE_GPIOA,
                                   AMHW_ZLG_SYSCFG_EXTI_PINSOURCE_8,
                                   AMHW_ZLG_EXTI_LINE_NUM8);
}
/** [src_aml165_core_hw_gpio_trigger] */

/* end of file */
