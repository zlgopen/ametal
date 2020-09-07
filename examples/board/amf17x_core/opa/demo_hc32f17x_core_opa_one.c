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
 * \brief OPA 电压跟随例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   PB00输入模拟电压。对应OPA输出引脚PA04也输出与PB00大小相等电压。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f17x_core_opa_one.c src_hc32f17x_core_opa_one
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-10  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f17x_core_opa_one
 * \copydoc demo_hc32f17x_core_opa_one.c
 */

/** [src_hc32f17x_core_opa_one] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "hc32_pin.h"
#include "am_hc32_opa.h"
#include "demo_hc32_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f17x_core_hw_opa_one_entry (void)
{

    AM_DBG_INFO("demo amf17x_core hw opa one test!\r\n");

    /* 开启OPA时钟 */
    am_clk_enable (CLK_OPA);

    /* 开启BGR时钟 */
    am_clk_enable (CLK_ADC_BGR);

    /* PB00 OPA输入 */
    am_gpio_pin_cfg (PIOB_0, PIOB_0_AIN);

    demo_hc32_hw_opa_entry(HC32_OPA);
}

/* end of file */
