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
 * \brief WDT 喂狗和复位例子，通过驱动层实现
 *
 * - 实验现象：
 *   1. 看门狗设置超时时间为256ms，延时50ms后喂狗，程序正常执行，
 *      串口打印喂狗信息；
 *   2. 程序执行到延时1s时，系统复位，程序重新开始执行。
 *
 * \note 测试看门狗例程时请将am_prj_config.h文件中的宏AM_CFG_SIM_COP置为1
 *
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_wdt.c src_amks16z_core_std_wdt
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-30  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_std_wdt
 * \copydoc demo_amks16z_core_std_wdt.c
 */

/** [src_amks16z_core_std_wdt] */
#include "ametal.h"
#include "am_uart.h"
#include "am_wdt.h"
#include "am_delay.h"
#include "am_kl26_inst_init.h"
#include "demo_amks16z_core_all_entries.h"
#include "demo_std_entries.h"
#include "demo_amks16z_core_all_entries.h"

#define   TIME_OUT_MS     256       /**< \brief 超时时间 */
#define   FEED_TIME_MS    300       /**< \brief 喂狗时间 */

/**
 * \brief 看门狗入口函数，标准接口层实现
 *
 * \note 测试看门狗例程时请将am_prj_config.h文件中的宏AM_CFG_SIM_COP置为1
 *
 * \return 无
 */
void demo_amks16z_core_std_wdt_entry (void)
{
    am_wdt_handle_t  handle = NULL;     /**< \brief 看门狗标准服务句柄 */

    /* 初始化WDT，获取WDT句柄值 */
    handle = am_kl26_wdt_inst_init();

    demo_std_wdt_entry(handle, TIME_OUT_MS, FEED_TIME_MS);
}

/** [src_amks16z_core_std_wdt] */

/* end of file */
