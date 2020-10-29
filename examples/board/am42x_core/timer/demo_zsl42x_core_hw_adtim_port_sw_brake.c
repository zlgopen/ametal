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
 * \brief 高级定时器端口、软件刹车（依次进行高电平刹车、软件刹车功能测试）例程，通过 HW层接口实现
 *
 * - 实验现象：
 *   1.串口打印提示信息；每5s间隔PWM的输出因为刹车的改变而改变
 *   2.demo测试开始，倒计时5秒，计时结束，高电平刹车触发，PWM输出因为刹车功能变为低电平。
 *                倒计时5秒，计时结束，清除标志，刹车配置禁能，PWM正常输出。
 *                倒计时5秒，计时结束，软件刹车使能，PWM输出因为刹车功能变为低电平。
 *                倒计时5秒，计时结束，软件刹车禁能，PWM持续正常输出。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *    2. 使用导线连接PIOB_1（电平条件模拟引脚）和PIOB_11（高电平刹车检测引脚）。
 *    3. PIOA_8为（TIM4_CHA）PWM输出引脚。
 *
 * \par 源代码
 * \snippet demo_zsl42x_hw_adtim_port_sw_brake.c src_zsl42x_hw_adtim_port_sw_brake
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-10  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_adtim_port_sw_brake
 * \copydoc demo_zsl42x_hw_adtim_port_sw_brake.c
 */

/** [src_zsl42x_hw_adtim_port_sw_brake] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_zsl42x.h"
#include "hw/amhw_zsl42x_adtim.h"
#include "am_zsl42x_inst_init.h"
#include "demo_zsl42x_entries.h"
#include "demo_am42x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zsl42x_core_hw_adtim_port_sw_brake_entry (void)
{

    AM_DBG_INFO("demo am42x_core hw adtim port_sw_brake!\r\n");

    am_gpio_pin_cfg(PIOB_1, PIOB_1_GPIO | PIOB_1_OUT_PP);

    am_gpio_pin_cfg(PIOB_11, PIOB_11_GPIO | PIOB_11_INPUT_FLOAT);
    am_gpio_pin_cfg(PIOA_8,  PIOA_8_TIM4_CHA | PIOA_8_OUT_PP);

    /* 定时器时钟使能 */
    am_clk_enable(CLK_TIM456);

    demo_zsl42x_hw_adtim_port_sw_brake_entry(ZSL42x_TIM4,
                                             AMHW_ZSL42x_ADTIM_CHX_A,
                                             500000 / 4,
                                             500000,
                                             PIOB_1,
                                             AMHW_ZSL42x_ADTIM_TRIG_PB11);
}
/** [src_zsl42x_hw_adtim_port_sw_brake] */

/* end of file */
