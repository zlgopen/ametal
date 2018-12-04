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
 *
 * - 实验现象：
 *      - 串口打印出设置的时间
 *      - 以后每秒打印一次时间
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_rtc.c src_amks16z_core_std_rtc
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_amks16z_core_if_std_rtc
 * \copydoc demo_amks16z_core_std_rtc.c
 */

/** [src_amks16z_core_std_rtc] */
#include "am_kl26_inst_init.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_fsl_rtc.h"
#include "am_board.h"
#include "demo_std_entries.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief AMetal 应用程序入口
 */
void demo_amks16z_core_std_rtc_entry (void)
{
    /** RTC 服务句柄 */
    am_rtc_handle_t rtc_handler = am_kl26_rtc_inst_init();

    demo_std_rtc_entry(rtc_handler);
}

/** [src_amks16z_core_std_rtc] */

/* end of file */
