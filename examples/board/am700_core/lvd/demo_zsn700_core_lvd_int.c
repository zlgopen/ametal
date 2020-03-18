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
 * \brief LVD 中断例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   PIOB_7输入电压从高于基准电压变为低于基准电压，则产生LVD中断。
 *   串口打印 "LVD interrupt"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zsn700_core_lvd_int.c src_zsn700_core_lvd_int
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-09  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_core_lvd_int
 * \copydoc demo_zsn700_core_lvd_int.c
 */

/** [src_zsn700_hw_lvd_int] */
#include "ametal.h"
#include "am_zsn700.h"
#include "am_gpio.h"
#include "zsn700_pin.h"
#include "am_zsn700_lvd.h"
#include "demo_zlg_entries.h"

/**
 * \brief 用户回调函数
 */
static void __zsn700_lvd_callback (void *data)
{
    am_kprintf ("LVD interrupt!\r\n");
}

/**
 * \brief 例程入口
 */
void demo_zsn700_core_hw_lvd_int_entry (void)
{
    AM_DBG_INFO("Test start: demo-lvd-intterput !\r\n");

    /* 开启LVD时钟 */
    am_clk_enable (CLK_VC_LVD);

    /* PB07 LVD 监测输入端*/
    am_gpio_pin_cfg(PIOB_7, PIOB_7_AIN);

    /* PA04 LVD 输出端*/
    am_gpio_pin_cfg(PIOA_4, PIOA_4_LVD_OUT | PIOA_4_OUT_PP);

    demo_zsn700_hw_lvd_int_entry((void *)ZSN700_LVD, __zsn700_lvd_callback);
}

/* end of file */
