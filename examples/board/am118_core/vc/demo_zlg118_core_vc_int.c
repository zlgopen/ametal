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
 * \brief VC中断触发例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   PA07接VCC或GND对应PA06输出高电平或低电平。当PA07 电压低于 PC00 (接DAC0_out 输出2400mv)
 *   产生中断，串口打印出 "vc trigger interrupt !"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_vc_int.c src_zlg118_hw_vc_int
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-30  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_vc_int
 * \copydoc demo_zlg118_hw_vc_int.c
 */

/** [src_zlg118_hw_vc_int] */
#include "ametal.h"
#include "am_zlg118.h"
#include "am_gpio.h"
#include "zlg118_pin.h"
#include "am_zlg118_vc.h"
#include "demo_zlg_entries.h"

/**
 *  \brief 用户设置中断触发回调函数
 */
static void vc_tri_callback (void *p_cookie)
{
    am_kprintf ("TSET:vc trigger interrupt !\r\n");
}

/* DAC电压值设置 */
static uint16_t mv_val = 2400;

/**
 * \brief 例程入口
 */
void demo_zlg118_core_hw_vc_int_entry (void)
{

    /* 配置PIOA_4为DAC0_OUT功能 */
    am_gpio_pin_cfg(PIOA_4, PIOA_4_AOUT);

    /* 开启DAC时钟 */
    am_clk_enable(CLK_DAC);

    /* 配置PIOA_6为VC_OUT功能 */
    am_gpio_pin_cfg(PIOA_6, PIOA_6_VC0_OUT | PIOA_6_OUT_PP);

    /* 配置PIOC_0为VC0 P端输入 */
    am_gpio_pin_cfg(PIOC_0, PIOC_0_AIN);

    /* 配置PIOC_0为VC0 P端输入 */
    am_gpio_pin_cfg(PIOA_7, PIOA_7_AIN);

    /* 开启VC时钟 */
    am_clk_enable(CLK_VC_LVD);

    /* 开启BGR时钟 */
    am_clk_enable(CLK_ADC_BGR);

    demo_zlg118_hw_vc_int_entry((void *)ZLG118_VC_BASE,
                                vc_tri_callback,
                                ZLG118_DAC,
                                mv_val);
}

/* end of file */
