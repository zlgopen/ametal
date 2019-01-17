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
 * \brief 模拟比较器例程（电压阶梯），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIO0_14 连接模拟输入 3；
 *   2. PIO0_6 电压作为阶梯电压电源，此实验接 3.3V 电压，即 VDDCMP = 3.3V。
 *
 * - 实验现象：
 *   1. 输入 3 大于 1703mV 时，串口会输出比较结果 "result: PIO0_14_ACMP_I3 > PIO0_6_ACMP_LAD(1703mV)"；
 *   1. 输入 3 小于 1703mV 时，串口会输出比较结果 "result: PIO0_14_ACMP_I3 < PIO0_6_ACMP_LAD(1703mV)"。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *    PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_acmp_lad.c src_am845_core_hw_acmp_lad
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-05  hgo, modified
 * - 1.00 15-07-21  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_acmp_lad
 * \copydoc demo_am845_core_hw_acmp_lad.c
 */

/** [src_am845_core_hw_acmp_lad] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc82x_acmp.h"
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "demo_nxp_entries.h"

/**
 * \brief ACMP 平台初始化函数
 */
am_local void __plfm_acmp_init()
{
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_ACMP);
    amhw_lpc84x_syscon_powerup(AMHW_LPC84X_SYSCON_PD_ACMP);

    /* 使能管脚  PIO0_14 为比较器输入 3 功能（比较器输入 3 管脚固定为 PIO0_14） */
    am_gpio_pin_cfg (PIO0_14, PIO0_14_ACMP_I3);

    /* 使能管脚  PIO0_6 为比较器阶梯电压功能（比较器阶梯电压管脚固定为 PIO0_6） */
    am_gpio_pin_cfg (PIO0_6, PIO0_6_VDDCMP);

    /* 使能管脚 PIO0_18 为比较器输出功能 */
    am_gpio_pin_cfg (PIO0_18, PIO_FUNC_ACMP_O);
}

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_acmp_lad_entry (void)
{

    am_kprintf("demo am845_core hw acmp lad!\r\n");

    /* 平台初始化 */
    __plfm_acmp_init();
    
    demo_lpc824_hw_acmp_lad_entry(LPC84X_ACMP, PIO0_18);
}
/** [src_am845_core_hw_acmp_lad] */

/* end of file */
