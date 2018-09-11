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
 * - 实验现象：
 *   1. PIO0_1 的电平由高电平改变为低电平（按键 KEY/RES 按下）时，LED0 状态改变。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 按键需要将 J14 跳线帽的 KEY 和 PIO0_1 短接才能使用。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_gpio_int.c src_lpc824_hw_gpio_int
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_gpio_int
 * \copydoc demo_lpc824_hw_gpio_int.c
 */

/** [src_lpc824_hw_gpio_int] */
#include "ametal.h"
#include "am_int.h"
#include "hw/amhw_lpc82x_gpio.h"
#include "hw/amhw_lpc82x_pint.h"
#include "hw/amhw_lpc82x_syscon.h"


/**
 * \brief 引脚中断服务函数
 */
am_local void __hw_pint_isr (void *p_arg)
{

    int led_pin = (int)p_arg;

    /* 清除中断标志 */
    amhw_lpc82x_pint_edge_dec_clr(LPC82X_PINT, AMHW_LPC82X_PINT_CHAN_0);

    /* 引脚输出状态翻转 */
    amhw_lpc82x_gpio_pin_out_tog(LPC82X_GPIO, led_pin);
}

void demo_lpc824_hw_gpio_int_entry(int                 key_pin,
                                   int                 led_pin,
                                   int                 pint_sel,
                                   int                 inum)
{
    /* 引脚中断 0 选择 KEY 引脚 */
    amhw_lpc82x_syscon_pint_sel(pint_sel, key_pin);

    /* 连接中断服务函数 */
    am_int_connect(inum, __hw_pint_isr, (void *)led_pin);

    /* 使能引脚中断 */
    am_int_enable(inum);

    AM_FOREVER {
        ; /* VOID */
    }
}

/** [src_lpc824_hw_gpio_int] */

/* end of file */
