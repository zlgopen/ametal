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
 * \brief SWDT 中断演示例程，通过驱动层接口实现
 *
 *  SWDT只支持硬件启动，MCU启动时从主闪存区域读取看门狗计数器的设定信息（ICG0寄存器），
 *  计数器自动开始计数。
 *
 * - 操作步骤：
 *   1. 配置hc32f460_icg.h中 的宏HC32F460_ICG0_SWDT_HARDWARE_START的值为
 *      ICG_FUNCTION_ON，使能SWDT的硬件启动。
 *   2. 配置hc32f460_icg.h中 的宏 HC32F460_ICG0_SWDT_REG_CONFIG，设置SWDT计数器的控制参数，
 *      将宏定义HC32F460_ICG0_SWDT_ITS配置为HC32F460_ICG_SWDT_INTERRUPT_REQUEST以设置
 *      SWDT触发事件类型为中断。
 *
 * - 实验现象：
 *   1. 根据看门狗的喂狗时间是否超过设置的看门狗超时时间，打印"wdt counter underflow !"或
 *   "normal feed dog !"等提示信息。

 *
 * \par 源代码
 * \snippet demo_hc32f460_drv_wdt.c src_hc32f460_drv_wdt
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-12
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_drv_swdt
 * \copydoc demo_hc32f460_drv_swdt.c
 */

/** [src_hc32f460_drv_swdt] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f460_swdt.h"
#include "am_hc32f460_inst_init.h"
#include "demo_std_entries.h"
#include "demo_hc32f460_entries.h"

/**
 * \brief 看门狗喂狗时间，若喂狗时间超过看门狗超时时间,
 *        会产生看门狗事件。
 */
#define __WDT_FEED_TIME_MS     1200
extern am_hc32f460_swdt_dev_t __g_swdt_dev;


/**
 * \brief 例程入口
 */
void demo_hc32f460_core_drv_swdt_int_entry (void)
{
    AM_DBG_INFO("demo hc32f460_core drv swdt int !\r\n");

    demo_hc32f460_drv_swdt_int_entry(am_hc32f460_swdt_inst_init(), &__g_swdt_dev, __WDT_FEED_TIME_MS);
}
/** [src_hc32f460_drv_swdt] */

/* end of file */
