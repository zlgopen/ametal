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
 * \brief 模拟比较器例程（中断模式），通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIO0_1 连接模拟输入 2；
 *   2. PIO0_14 连接模拟输入 3。
 *
 * - 实验现象：
 *   1. 当输入 2 与输入 3 比较结果发生变化的时候会输出提示字符；
 *   2. 输入 2 大于输入 3 时，串口会输出比较结果 "result: PIO0_1_ACMP_I2 > PIO0_14_ACMP_I3"；
 *   3. 输入 2 小于输入 3 时，串口会输出比较结果 "result: PIO0_1_ACMP_I2 < PIO0_14_ACMP_I3"。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_acmp_int.c src_lpc824_hw_acmp_int
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-05  hgo, modified
 * - 1.00 15-07-21  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_acmp_int
 * \copydoc demo_lpc824_hw_acmp_int.c
 */

/** [src_lpc824_hw_acmp_int] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "hw/amhw_lpc82x_acmp.h"


/** \brief ACMP 中断标志 */
am_local volatile am_bool_t __g_complete_flag = AM_FALSE;


/**
 * \brief ACMP 中断服务函数,当比较结果发生变化时会产生中断
 */
am_local void __acmp_irq_handler (void *p_arg)
{
  amhw_lpc82x_acmp_t *p_hw_acmp = (amhw_lpc82x_acmp_t *)p_arg;
    __g_complete_flag = AM_TRUE;
    amhw_lpc82x_acmp_int_clr(p_hw_acmp);
}


void demo_lpc824_hw_acmp_int_entry(amhw_lpc82x_acmp_t *p_hw_acmp, 
                                   uint32_t            flags, 
                                   int                 inum, 
                                   int                 pin)
{
    amhw_lpc82x_acmp_config(p_hw_acmp, flags);

    /* 连接 ADC 中断服务函数，并使能中断 */
    am_int_connect(inum, __acmp_irq_handler, (void *)p_hw_acmp);
    am_int_enable (inum);

    AM_FOREVER {

        while ( AM_FALSE == __g_complete_flag);

        if (am_gpio_get(pin)) {
            AM_DBG_INFO("result: PIO0_1_ACMP_I2 > PIO0_14_ACMP_I3\r\n");
        } else {
            AM_DBG_INFO("result: PIO0_1_ACMP_I2 < PIO0_14_ACMP_I3\r\n");
        }

        __g_complete_flag =  AM_FALSE;
    }
}
/** [src_lpc824_hw_acmp_int] */

/* end of file */

