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
 * \brief ADC 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. PIOA_1 (ADC 通道 1) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_std_adcc.c src_mm32l073_core_std_adc
 *
 * \internal
 * \par Modification History
 * - 1.00 14-04-18  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_std_adc
 * \copydoc demo_mm32l073_core_std_adc.c
 */

/** [src_std_adc] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_mm32l073_inst_init.h"
#include "demo_std_entries.h"
#include "demo_mm32l073_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_mm32l073_core_std_adc_entry (void)
{
    AM_DBG_INFO("demo mm32l073_core std adc int!\r\n");

    demo_std_adc_entry(am_mm32l073_adc_inst_init(), 1);  /* 使用通道1 （PIOA_1）*/
}
/** [src_mm32l073_core_std_adc] */
 
/* end of file */
