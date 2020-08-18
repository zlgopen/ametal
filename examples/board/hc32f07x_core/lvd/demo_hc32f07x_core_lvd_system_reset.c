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
 * \brief LVD 系统复位例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   PIOB_7输入电平低于设置的基准电压。则系统处于复位状态，当再次高于基准电压LED闪烁一次，系统正常工作。
 *   串口连续打印 "Mcu is running!"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f19x_hw_lvd_int.c src_hc32f19x_hw_lvd_int
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-09  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f19x_hw_lvd_int
 * \copydoc demo_hc32f19x_hw_lvd_int.c
 */

/** [src_hc32f19x_hw_lvd_int] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "hc32_pin.h"
#include "am_hc32_lvd.h"
#include "demo_hc32_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f19x_core_hw_lvd_system_reset_entry (void)
{
    /* 开启LVD时钟 */
    am_clk_enable (CLK_VC_LVD);

    /* PB07 LVD 监测输入端*/
    am_gpio_pin_cfg(PIOB_7, PIOB_7_AIN);

    /* PA04 LVD 输出端*/
    am_gpio_pin_cfg(PIOA_4, PIOA_4_LVD_OUT | PIOA_4_OUT_PP);

    demo_hc32_hw_lvd_system_reset_entry(
        (void *)HC32_LVD,
        AMHW_HC32_LVD_VIDS_1P9_V,
        AMHW_HC32_LVD_SRC_PB07);
}

/* end of file */
