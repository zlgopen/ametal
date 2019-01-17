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
 * \brief ADC 例程（中断方式），通过标准接口实现
 *
 * - 操作步骤：
 *   2. PIO0_7(ADC 通道 0) 连接模拟输入电压。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *    PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am845_core_std_adc_int.c src_am845_core_std_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-07-23  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_std_adc_int
 * \copydoc demo_am845_core_std_adc_int.c
 */

/** [src_am845_core_std_adc_int] */
#include "ametal.h"
#include "am_adc.h"
#include "am_vdebug.h"
#include "am_lpc84x_inst_init.h"
#include "demo_nxp_entries.h"


/** \brief 使用的 ADC 通道 */
#define __ADC_CHAN         0

/**
 * \brief 例程入口
 */
void demo_am845_core_std_adc_int_entry (void)
{
    am_adc_handle_t adc_handle = NULL;

    am_kprintf("demo am845_core std adc int!\r\n");

    adc_handle = am_lpc84x_adc0_int_inst_init();

    demo_lpc_std_adc_int_entry(adc_handle, __ADC_CHAN, 10000);
}
/** [src_am845_core_std_adc_int] */

/* end of file */
