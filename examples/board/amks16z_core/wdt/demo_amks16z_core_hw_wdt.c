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
 * \brief WDT 喂狗和复位例子，通过硬件层实现
 *
 * - 实验现象：
 *   1. 看门狗设置超时时间为256ms，延时50ms后喂狗，程序正常执行，
 *      串口打印喂狗信息；
 *   2. 程序执行到延时1s时，喂狗超时，系统复位，程序重新开始执行。
 *
 * \note 测试看门狗例程时请将am_prj_config.h文件中的宏AM_CFG_SIM_COP置为1
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_wdt.c src_amks16z_core_hw_wdt
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-30  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_wdt
 * \copydoc demo_amks16z_core_hw_wdt.c
 */

/** [src_kl26_hw_wdt] */
#include "ametal.h"
#include "am_delay.h"
#include "am_uart.h"
#include "am_vdebug.h"
#include "am_kl26_inst_init.h"
#include "demo_fsl_entrys.h"

/**
 * \brief WDT入口函数，HW层实现
 *
 * \note 测试看门狗例程时请将am_prj_config.h文件中的宏AM_CFG_SIM_COP置为1
 *
 * \return 无
 */
void demo_amks16z_core_hw_wdt_entry (void)
{
    AM_DBG_INFO("demo amks16z_core hw wdt!\r\n");

    demo_kl26_hw_wdt_entry();
}

/** [src_amks16z_core_hw_wdt] */

/* end of file */
