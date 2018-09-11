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
 * \snippet demo_lpc824_hw_gpio_pmatch.c src_lpc824_hw_gpio_pmatch
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-24  mem, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_gpio_pmatch
 * \copydoc demo_lpc824_hw_gpio_pmatch.c
 */

/** [src_lpc824_hw_gpio_pmatch] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc82x_swm.h"
#include "hw/amhw_lpc82x_gpio.h"
#include "hw/amhw_lpc82x_pint.h"
#include "hw/amhw_lpc82x_iocon.h"
#include "hw/amhw_lpc82x_syscon.h"


/** \brief 匹配输入引脚 */
#define __IN_PIN0 PIO0_14
#define __IN_PIN1 PIO0_15
#define __IN_PIN2 PIO0_16
#define __IN_PIN3 PIO0_17
#define __IN_PIN4 PIO0_18

/**
 * \brief 引脚中断服务函数
 */
am_local void __pint_isr (void *p_arg)
{
    am_local int i = 0;

    int pin = (int)p_arg;

    AM_DBG_INFO("gpio pmatch count %d\r\n", i++);
    
    /* 引脚输出状态翻转 */
    amhw_lpc82x_gpio_pin_out_tog(LPC82X_GPIO, pin);
}

/**
 * \brief 引脚初始化
 */
void demo_lpc824_hw_gpio_pmatch_entry (int pin)
{

    /* 禁用特殊功能 */
    amhw_lpc82x_swm_pin_fixed_func_disable(LPC82X_SWM, __IN_PIN0);
    amhw_lpc82x_swm_pin_fixed_func_disable(LPC82X_SWM, __IN_PIN1);
    amhw_lpc82x_swm_pin_fixed_func_disable(LPC82X_SWM, __IN_PIN2);
    amhw_lpc82x_swm_pin_fixed_func_disable(LPC82X_SWM, __IN_PIN3);
    amhw_lpc82x_swm_pin_fixed_func_disable(LPC82X_SWM, __IN_PIN4);

    /* 配置为上拉输入 */
    amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                               __IN_PIN0,
                               AMHW_LPC82X_IOCON_MODE_PULLUP);
    amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                               __IN_PIN1,
                               AMHW_LPC82X_IOCON_MODE_PULLUP);
    amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                               __IN_PIN2,
                               AMHW_LPC82X_IOCON_MODE_PULLUP);
    amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                               __IN_PIN3,
                               AMHW_LPC82X_IOCON_MODE_PULLUP);
    amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                               __IN_PIN4,
                               AMHW_LPC82X_IOCON_MODE_PULLUP);

    amhw_lpc82x_gpio_port_dir_input(LPC82X_GPIO, AM_SBF(0x1F,4));

    /* 为每个通道选择引脚 */
    amhw_lpc82x_syscon_pint_sel(AMHW_LPC82X_PINT_CHAN_0, __IN_PIN0);
    amhw_lpc82x_syscon_pint_sel(AMHW_LPC82X_PINT_CHAN_1, __IN_PIN1);
    amhw_lpc82x_syscon_pint_sel(AMHW_LPC82X_PINT_CHAN_3, __IN_PIN2);
    amhw_lpc82x_syscon_pint_sel(AMHW_LPC82X_PINT_CHAN_4, __IN_PIN3);
    amhw_lpc82x_syscon_pint_sel(AMHW_LPC82X_PINT_CHAN_5, __IN_PIN4);

    /* 配置每个位片 */
    amhw_lpc82x_pint_pmatch_slice_cfg(
        LPC82X_PINT,
        AMHW_LPC82X_PINT_BITSLICE_0,            /* 配置位片 0 */
        AMHW_LPC82X_PINT_CHAN_0,                /* 选择通道 0 */
        AMHW_LPC82X_PINT_SLICE_ARG_LOW_LEVEL,   /* 低电平有效 */
        AM_FALSE);                              /* 不是端点 */

    amhw_lpc82x_pint_pmatch_slice_cfg(
        LPC82X_PINT,
        AMHW_LPC82X_PINT_BITSLICE_1,            /* 配置位片 1 */
        AMHW_LPC82X_PINT_CHAN_1,                /* 选择通道 1 */
        AMHW_LPC82X_PINT_SLICE_ARG_HIGH_LEVEL,  /* 高电平有效 */
        AM_TRUE);                               /* 是端点 */

    amhw_lpc82x_pint_pmatch_slice_cfg(
        LPC82X_PINT,
        AMHW_LPC82X_PINT_BITSLICE_3,            /* 配置位片 3 */
        AMHW_LPC82X_PINT_CHAN_3,                /* 选择通道 3 */
        AMHW_LPC82X_PINT_SLICE_ARG_LOW_LEVEL,   /* 低电平有效 */
        AM_TRUE);                               /* 是端点 */

    amhw_lpc82x_pint_pmatch_slice_cfg(
        LPC82X_PINT,
        AMHW_LPC82X_PINT_BITSLICE_4,            /* 配置位片 4 */
        AMHW_LPC82X_PINT_CHAN_4,                /* 选择通道 4 */
        AMHW_LPC82X_PINT_SLICE_ARG_LOW_LEVEL,   /* 低电平有效 */
        AM_FALSE);                              /* 不是端点 */

    amhw_lpc82x_pint_pmatch_slice_cfg(
        LPC82X_PINT,
        AMHW_LPC82X_PINT_BITSLICE_5,            /* 配置位片 5 */
        AMHW_LPC82X_PINT_CHAN_5,                /* 选择通道 5 */
        AMHW_LPC82X_PINT_SLICE_ARG_STICKY_FALL, /* 发生过下降沿 */
        AM_TRUE);                               /* 是端点 */

    /* 配置中断模式为模式匹配 */
    amhw_lpc82x_pint_pmatch_mode_set(LPC82X_PINT);

    /* 开启对应端点的中断 */
    am_int_connect(INUM_PIN_INT1, __pint_isr, (void *)pin);
    am_int_connect(INUM_PIN_INT3, __pint_isr, (void *)pin);
    am_int_connect(INUM_PIN_INT5, __pint_isr, (void *)pin);

    am_int_enable(INUM_PIN_INT1);
    am_int_enable(INUM_PIN_INT3);
    am_int_enable(INUM_PIN_INT5);
}


/** [src_lpc824_hw_gpio_pmatch] */

/* end of file */
