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
 * \brief GPIO引脚中断例程，通过标准接口实现
 *
 * - 操作：
 * - 1.将J14的key和PIOC_0短接在一起;
 *   2.将J9的LED0和PIOC_4短接在一起;
 *   3.将J10的LED1和PIOC_8短接在一起;
 *   4.PIOA_1 引脚连接PC串口的TXD;
 *   5.PIOA_2 引脚连接PC串口的RXD;
 *   6.配置上位机串口波特率为115200，8位数据长度 1位停止位 无奇偶校验;
 *
 *- 实现现象：程序下载到demo板后，按下按键，demo板上的LED0会闪,
 *          同时串口输出 "the gpio interrupt happen!"
 *
 * \par 源代码
 * \snippet demo_kl26_hw_gpio_int.c src_kl26_hw_gpio_int
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-09-20  nwt, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_hw_gpio_int
 * \copydoc demo_kl26_hw_gpio_int.c
 */

/** [src_kl26_hw_gpio_int] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_led.h"
#include "am_gpio.h"
#include "hw/amhw_kl26_gpio.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"


/** 
 * \brief 引脚中断服务函数
 */
static void hw_gpio_isr (void *p_arg)
{
    amhw_kl26_port_t *p_hw_port = (amhw_kl26_port_t *)p_arg;

    /* 清除中断标志 */
    amhw_kl26_port_pin_isf_clr(p_hw_port, PIOC_0);
    /* 引脚输出状态翻转 */
    am_gpio_toggle(PIOC_4);

    AM_DBG_INFO("the gpio interrupt happen!\r\n");
}

/**
 * \brief 例程入口
 */
void demo_kl26_hw_gpio_int_entry (void)
{
    /* 设置 PORTC 0,4号管脚为GPIO功能 */
    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOC_4, 0x1);
    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOC_0, 0x1);

    /* 设置GPIOC 4号管脚为输出方向 */
    amhw_kl26_gpio_pin_dir_output(KL26_GPIO, PIOC_4);
    /* 配置GPIOC 0引脚方向为输入 */
    amhw_kl26_gpio_pin_dir_input(KL26_GPIO, PIOC_0);

    /* 设置GPIOC 4号管脚初始电平 */
    amhw_kl26_gpio_pin_init_out_low(KL26_GPIO, PIOC_4);

    /* 配置GPIOC 0引脚为上拉 */
    amhw_kl26_port_pin_pull_enable(KL26_PORT, PIOC_0);
    amhw_kl26_port_pin_pull_up(KL26_PORT, PIOC_0);

    /* 配置GPIOC 0为下降沿触发 */
    amhw_kl26_port_pin_irq_cfg(KL26_PORT, PIOC_0, AMHW_KL26_INT_FALLING);

    /* 连接中断服务函数 */
    am_gpio_trigger_connect(PIOC_0, hw_gpio_isr, (void *)KL26_PORT);

    while (1) {
        ;
    }
}

/** [src_kl26_hw_gpio_int] */

/* end of file */
