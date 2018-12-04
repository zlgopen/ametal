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
 * \brief
 *
 * - 操作步骤：
 *      - PTC1引脚外接32.786kHz的时钟输入
 *      - 使用调试串口打印
 *
 * - 实验现象：
 *      - 串口打印出设置的时间
 *      - 以后每秒打印一次时间
 *      - LED0的状态每秒钟翻转一次
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_rtc_sec_int.c src_amks16z_core_hw_rtc_sec_int
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_amks16z_core_if_hw_rtc_sec_int
 * \copydoc demo_amks16z_core_hw_rtc_sec_int.c
 */

/** [src_amks16z_core_hw_rtc_sec_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_int.h"
#include <time.h>
#include "am_kl26_inst_init.h"
#include "hw/amhw_kl26_sim.h"
#include "hw/amhw_fsl_rtc.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief AMetal 应用程序入口
 */
void demo_amks16z_core_hw_rtc_sec_int_entry (void)
{
	  
    /* RTC 时钟源选择为外部RTC时钟源 */
    amhw_kl26_sim_ercclk32k_src_set(KL26_SIM_OSC32KSEL_CLKIN);

    /* RTC 模块时钟使能 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_RTC);

    /* PORTC 时钟引脚使能 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_PORTC);

    /* PTC1 复用位 RTC_CLK_INPUT */
    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOC_1, 1);

    demo_fsl_hw_rtc_sec_int_entry(KL26_RTC);
}

/** [src_amks16z_core_hw_rtc_sec_int] */

/* end of file */
