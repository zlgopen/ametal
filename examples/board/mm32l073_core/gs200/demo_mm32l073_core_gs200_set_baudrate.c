/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \brief GS200 模块切换波特率例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 单片机切换波特率连接 GS200 模块；
 *   2. 如果切换成功，通过串口打印当前的波特率，并检测手势动作（成功检测手势动作后将切换 LED1 状态）；
 *      否则 LED0 按 100 ms 的间隔快闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIOB_1 控制；LED1 需要短接 J10 跳线帽，才能被 PIOB_2 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    3. 测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE、
 *       AM_CFG_KEY_ENABLE 和 AM_CFG_SOFTIMER_ENABLE 定义为 1。但这些宏
 *       已经默认配置为 1， 用户不必再次配置；
 *    4. GS200 模块与 MCU 的串口 2 通信。
 *
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_gs200_set_baudrate.c src_mm32l073_core_gs200_set_baudrate
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-23  fzb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_gs200_set_baudrate
 * \copydoc demo_mm32l073_core_gs200_set_baudrate.c
 */

/** [src_mm32l073_core_mm32l073_set_baudrate] */
#include "ametal.h"
#include "am_gs200.h"
#include "am_hwconf_gs200.h"
#include "demo_components_entries.h"
#include "demo_mm32l073_core_entries.h"

void demo_mm32l073_core_gs200_set_baudrate_entry()
{
    am_gs200_handle_t  handle = am_gs200_inst_init();

    demo_gs200_baudrate_entry(handle, AM_GS200_BAUDRATE_115200);
}


/** [src_mm32l073_core_gs200_set_baudrate] */

/* end of file */

