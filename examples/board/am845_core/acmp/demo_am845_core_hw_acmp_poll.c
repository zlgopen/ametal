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
 * \brief 模拟比较器例程（查询模式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIO0_1 连接模拟输入 2；
 *   2. PIO0_14 连接模拟输入 3。
 *
 * - 实验现象：
 *   1. 输入 2 大于输入 3 时，串口会输出比较结果 "result: PIO0_1_ACMP_I2 > PIO0_14_ACMP_I3"；
 *   2. 输入 2 小于输入 3 时，串口会输出比较结果 "result: PIO0_1_ACMP_I2 < PIO0_14_ACMP_I3"。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_acmp_poll.c src_am845_core_hw_acmp_poll
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-05  hgo, modified
 * - 1.00 15-07-23  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_acmp_poll
 * \copydoc demo_am845_core_hw_acmp_poll.c
 */

/** [src_am845_core_hw_acmp_poll] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "lpc84x_pin.h"
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "hw/amhw_lpc82x_acmp.h"
#include "demo_nxp_entries.h"

/**
 * \brief ACMP 平台初始化函数
 */
am_local void __plfm_acmp_init()
{
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_ACMP);
    amhw_lpc84x_syscon_powerup(AMHW_LPC84X_SYSCON_PD_ACMP);

    /* 使能管脚  PIO0_1 为比较器输入 2 功能（比较器输入 2 管脚固定为 PIO0_1） */
    am_gpio_pin_cfg (PIO0_1, PIO0_1_ACMP_I2);

    /* 使能管脚  PIO0_14 为比较器输入 3 功能（比较器输入 3 管脚固定为 PIO0_14） */
    am_gpio_pin_cfg (PIO0_14, PIO0_14_ACMP_I3);

    /* 使能管脚 PIO0_18 为比较器输出功能 */
    am_gpio_pin_cfg (PIO0_18, PIO_FUNC_ACMP_O);
}


/**
 * \brief 例程入口
 */
void demo_am845_core_hw_acmp_poll_entry (void)
{

    am_kprintf("demo am845_core hw acmp poll!\r\n");

    /* 平台初始化 */
    __plfm_acmp_init();

    demo_lpc845_hw_acmp_poll_entry(LPC84X_ACMP, PIO0_18);
}
/** [src_am845_core_hw_acmp_poll] */

/* end of file */
