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
 * \brief GPIO 引脚中断例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 使对应的中断引脚上产生下降沿。
 *
 * - 实验现象：
 *   1. 中断产生时调试串口输出 "the gpio interrupt happen!"。
 *
 * \par 源代码
 * \snippet demo_std_gpio_trigger.c src_std_gpio_trigger
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-17  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_gpio_trigger
 * \copydoc demo_std_gpio_trigger.c
 */

/** [src_std_gpio_trigger] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"
extern void __chars_printf(char* str);

/**
 * \brief 引脚中断服务函数
 */
static void __gpio_isr (void *p_arg)
{
    int arg = (int)p_arg;

    if (arg == 0) {
        __chars_printf("the gpio interrupt happen!\r\n");
    }
}

/**
 * \brief 例程入口
 */
void demo_std_gpio_trigger_entry (int pin)
{

    /* 连接引脚中断服务函数 */
    am_gpio_trigger_connect(pin, __gpio_isr, (void *)0);

    /* 配置引脚中断触发方式 */
    am_gpio_trigger_cfg(pin, AM_GPIO_TRIGGER_FALL);

    /* 使能引脚触发中断 */
    am_gpio_trigger_on(pin);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_gpio_trigger] */

/* end of file */
