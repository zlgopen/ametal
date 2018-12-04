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
 * \brief 模拟比较器比较电压例程，通过HW层的接口中断方式实现

 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. PIOC_6外接可调的电压输入（可用滑动变阻器）。
 * - 实验现象：
 *   当输入电压经过1.65v附近时，LED翻转（类似于过零检测），串口打印信息：“CMP INT！”
 *
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_cmp_int.c src_amks16z_core_hw_cmp_int
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-22  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_cmp_int
 * \copydoc demo_amks16z_core_hw_cmp_int.c
 */

/** [src_amks16z_core_hw_cmp_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_kl26_sim.h"
#include "am_gpio.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief CMP比较器HW层中断例程
 * \return 无
 */
void demo_amks16z_core_hw_cmp_int_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw cmp int!\r\n");

    /* 使能CMP时钟 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_CMP);

    /* PIOC_6为CMP0_IN0   */
    am_gpio_pin_cfg (PIOC_6, PIOC_6_CMP0_IN0);

    /* PIOE_0配置为CMP0_OUT */
    am_gpio_pin_cfg (PIOE_0, PIOE_0_CMP0_OUT);

    demo_fsl_hw_cmp_int_entry(KL26_CMP0,
                              INUM_CMP0);
}

/** [src_amks16z_core_hw_cmp_int] */

/* end of file */
