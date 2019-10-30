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
 * \brief 定时器端口刹车中断输出例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIOB_5与低电平连接，TIM1_CHA（PIOA_0）输出占空比为25%的2Hz的PWM波。
 *   2. PIOB_5与高电平连接，发生刹车中断，串口打印"brake int!"，TIM1_CHA（PIOA_0）的PWM
 *      输出停止，再次将PIOB_5与低电平连接，恢复正常输出。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_tim_port_brake.c src_zlg118_hw_tim_port_brake
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-12  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_tim_port_brake
 * \copydoc demo_zlg118_hw_tim_port_brake.c
 */

/** [src_zlg118_hw_tim_port_brake] */
#include "ametal.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_zlg118.h"
#include "hw/amhw_zlg118_adtim.h"
#include "am_zlg118_inst_init.h"
#include "demo_zlg_entries.h"
#include "demo_am118_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg118_core_hw_tim_port_brake_entry (void)
{

    AM_DBG_INFO("demo am118_core hw adtim port brake!\r\n");

    /* 刹车检测引脚初始化 */
    am_gpio_pin_cfg(PIOB_5, PIOB_5_TIM1_BK | PIOB_5_INPUT_FLOAT);

    /* TIM1_CHA通道引脚配置 */
    am_gpio_pin_cfg(PIOA_0, PIOA_0_TIM1_CHA | PIOA_0_OUT_PP);

    /* 使能定时器时钟 */
    am_clk_enable(CLK_TIM012);

    demo_zlg118_hw_tim_port_brake_entry(ZLG118_TIM1,
                                        AMHW_ZLG118_TIM_TYPE_TIM1,
                                        ZLG118_TIM_CH0A,
                                        INUM_TIM1,
                                        500000 / 4,
                                        500000);
}
/** [src_zlg118_hw_tim_port_brake] */

/* end of file */
