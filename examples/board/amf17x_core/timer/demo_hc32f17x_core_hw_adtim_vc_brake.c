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
 * \brief 高级定时器VC刹车例程，通过 HW层接口实现
 *
 * - 操作步骤：
 *   1.VC0_P("+"输入端PA1)连接一个模拟电压，建议不超过3.3V。
 *   2.VC0_N("-"输入端PA0)连接一个模拟电压，建议不超过3.3V。
 *   3.根据VC0_P和VC0_N之间的电压大小关系，将产生下述实验现象。
 *
 * - 实验现象：
 *   1.VC0_P("+"输入端PA1)的电压大于VC0_N("-"输入端PA0)的电压，VC0_OUT(PA6)输出高电平，
 *     触发VC0中断(高电平一直触发中断)，定时器4刹车配置生效，TIM4_CHA(PA8)的PWM输出停止。
 *   2.VC0_P("+"输入端PA1)的电压小于VC0_N("-"输入端PA0)的电压，VC0_OUT(PA6)输出低电平，
 *     VC0无中断产生，TIM4_CHA(PA8)的PWM正常输出（2KHz，占空比25%）。
 *
 * \note
 *    1. 本实例仅适用于高级定时器（TIM4、5、6）。
 *    2. VC默认使用VC0端口作为演示实例。
 *    3. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f17x_hw_adtim_vc_brake.c src_hc32f17x_hw_adtim_vc_brake
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-12  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f17x_hw_adtim_vc_brake
 * \copydoc demo_hc32f17x_hw_adtim_vc_brake.c
 */

/** [src_hc32f17x_hw_adtim_vc_brake] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_hc32.h"
#include "hw/amhw_hc32_adtim.h"
#include "hw/amhw_hc32_vc.h"
#include "am_hc32f17x_inst_init.h"
#include "demo_hc32_entries.h"
#include "demo_amf17x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f17x_core_hw_adtim_vc_brake_entry (void)
{

    AM_DBG_INFO("demo amf17x_core hw adtim vc_brake!\r\n");

    /* 配置PIOA_6为VC_OUT功能 */
    am_gpio_pin_cfg(PIOA_6, PIOA_6_VC0_OUT | PIOA_6_OUT_PP);

    /* VC0_N、VC0_P输入端引脚初始化 */
    am_gpio_pin_cfg(PIOA_0, PIOA_0_AIN);
    am_gpio_pin_cfg(PIOA_1, PIOA_1_AIN);

    /* TIM4_CHA定时器PWM输出引脚初始化 */
    am_gpio_pin_cfg(PIOA_8,  PIOA_8_TIM4_CHA | PIOA_8_OUT_PP);

    /* 开启TIM4、VC、BGR时钟 */
    am_clk_enable(CLK_TIM456);
    am_clk_enable(CLK_VC_LVD);
    am_clk_enable(CLK_ADC_BGR);

    demo_hc32_hw_adtim_vc_brake_entry(HC32_TIM4,
                                        AMHW_HC32_ADTIM_CHX_A,
                                        500000 / 4,
                                        500000,
                                        HC32_VC,
                                        AMHW_HC32_VC0_N_INPUT_PA0,
                                        AMHW_HC32_VC0_P_INPUT_PA1);
}
/** [src_hc32f17x_hw_adtim_vc_brake] */

/* end of file */
