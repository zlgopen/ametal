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
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_acmp_lad.c src_lpc824_hw_acmp_lad
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-05  hgo, modified
 * - 1.00 15-07-21  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_acmp_lad
 * \copydoc demo_lpc82x_h4_acmp_lad.c
 */

/** [src_lpc824_hw_acmp_lad] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "hw/amhw_lpc82x_acmp.h"


void demo_lpc824_hw_acmp_lad_entry (amhw_lpc82x_acmp_t *p_hw_acmp, int pin)
{
    
    amhw_lpc82x_acmp_config(p_hw_acmp,

                            /* 选择输入 2 通道为同相端输入 */
                            AMHW_LPC82X_ACMP_CTRL_VP_CMP3 |

                            /* 选择阶梯电压为反相端输入 */
                            AMHW_LPC82X_ACMP_CTRL_VM_LADOUT);

        /* 使能阶梯电压功能 */
    amhw_lpc82x_acmp_ladder_enable(p_hw_acmp);

    /* 选择比较器电源 VDDCMP */
    amhw_lpc82x_acmp_ladref_set(p_hw_acmp,
                                AMHW_LPC82X_ACMP_LADDER_VDDCMP);

    /* 阶梯电压输入 Vol = VDDCMP * value / 31 = 3300mV * 16 / 31 = 1703mV */
    amhw_lpc82x_acmp_ladsel_set(p_hw_acmp, 16);
  
    AM_FOREVER {
        if (am_gpio_get(pin)) {
            AM_DBG_INFO("result: PIO0_14_ACMP_I3 > PIO0_6_ACMP_LAD(1703mV)\r\n");
        } else {
            AM_DBG_INFO("result: PIO0_14_ACMP_I3 < PIO0_6_ACMP_LAD(1703mV)\r\n");
        }

        am_mdelay(200);
    }
}


/** [src_lpc824_hw_acmp_lad] */

/* end of file */

