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
 * \brief SCT 捕获例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. PIO0_25 引脚捕获跳变沿来获取 PWM 周期；
 *   2. 串口打印捕获到的周期和频率。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    2. 由于 SCT 默认作为驱动蜂鸣器，使用测试本 Demo 前需要将 am_prj_config.h 中
 *       的 AM_CFG_BUZZER_ENABLE 定义为 0，不使用蜂鸣器。
 *
 * \par 源代码
 * \snippet demo_am824_std_sct_cap.c src_am824_std_sct_cap
 *
 * \internal
 * \par Modification history
 * - 1.02 15-12-07  hgo, modified
 * - 1.01 15-09-24  zxl, modified
 * - 1.00 15-07-15  oce, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_std_sct_cap
 * \copydoc demo_am824_std_sct_cap.c
 */

/** [src_am824_std_sct_cap] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_std_sct_cap_entry (void)
{

    AM_DBG_INFO("The SCT demo for standard CAP service\r\n");

    demo_std_timer_cap_entry(am_lpc82x_sct0_cap_inst_init(), 0);

}
/** [src_am824_std_sct_cap] */

/* end of file */
