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
 * \snippet demo_lpc845_hw_acmp_poll.c src_lpc845_hw_acmp_poll
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-05  hgo, modified
 * - 1.00 15-07-23  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc845_hw_acmp_poll
 * \copydoc demo_lpc845_hw_acmp_poll.c
 */

/** [src_lpc845_hw_acmp_poll] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "hw/amhw_lpc82x_acmp.h"

void demo_lpc845_hw_acmp_poll_entry(amhw_lpc82x_acmp_t *p_hw_acmp,
                                    int                 pin)
{
    amhw_lpc82x_acmp_config(p_hw_acmp,
                            AMHW_LPC82X_ACMP_CTRL_VP_CMP2 | /* 输入 2 为同相端输入 */
                            AMHW_LPC82X_ACMP_CTRL_VM_CMP3); /* 输入 3 为反相端输入 */

    amhw_lpc82x_acmp_ladder_disable(p_hw_acmp);
    AM_FOREVER {

        if (am_gpio_get(pin)) {
            AM_DBG_INFO("result: PIO0_1_ACMP_I2 > PIO0_14_ACMP_I3\r\n");
        } else {
            AM_DBG_INFO("result: PIO0_1_ACMP_I2 < PIO0_14_ACMP_I3\r\n");
        }
        am_mdelay(200);
    }
}

/** [src_lpc845_hw_acmp_poll] */

/* end of file */

