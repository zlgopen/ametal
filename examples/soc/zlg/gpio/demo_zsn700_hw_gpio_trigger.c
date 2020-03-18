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
 *   1. 使对应的中断引脚上产生下降沿。
 *
 * - 实验现象：
 *   1. 中断产生时调试串口输出 "the gpio interrupt happen!"。
 *
 * \par 源代码
 * \snippet demo_zsn700_hw_gpio_trigger.c src_zsn700_hw_gpio_trigger
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-10  YRZ, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_hw_gpio_trigger
 * \copydoc demo_zsn700_hw_gpio_trigger.c
 */

/** [src_zsn700_hw_gpio_trigger] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "hw/amhw_zsn700_gpio.h"
#include "hw/amhw_zsn700_rcc.h"

/**
 * \brief 引脚中断服务函数
 */
static void __gpio_isr (void *p_arg)
{
    AM_DBG_INFO("the gpio interrupt happen!\r\n");
}

/**
 * \brief 例程入口
 */
void demo_zsn700_hw_gpio_trigger_entry (void *p_hw_gpio, int32_t pin)
{
	amhw_zsn700_gpio_t *p_gpio = (amhw_zsn700_gpio_t *)p_hw_gpio;

    /* 取消引脚的复用功能 */
    amhw_zsn700_gpio_afio_set(p_gpio, AMHW_ZSN700_AFIO_NO, pin);

    /* 数字端口设置*/
    amhw_zsn700_gpio_pin_digital_set(p_gpio, pin);

    /* 引脚输入 */
    amhw_zsn700_gpio_pin_dir_input(p_gpio, pin);

    /* 使能引脚的上拉电阻 */
    amhw_zsn700_gpio_pin_pu_enable(p_gpio, pin);

    /* 下降沿触发 */
    amhw_zsn700_gpio_pin_falling_int_enable(p_gpio, pin);

    /* 连接用户注册的中断回调函数 */
    am_gpio_trigger_connect(pin, __gpio_isr, NULL);

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zsn700_hw_gpio_trigger] */

/* end of file */
