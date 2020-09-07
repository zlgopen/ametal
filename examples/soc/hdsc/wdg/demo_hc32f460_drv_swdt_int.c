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
 * \snippet demo_std_wdt.c src_std_wdt
 *
 * \internal
 * \par Modification history
 * - 1.00 20-06-03  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_drv_wdt
 * \copydoc demo_drv_wdt.c
 */

/** [src_drv_wdt] */
#include "ametal.h"
#include "am_wdt.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_hc32f460_swdt.h"

static uint8_t g_swdt_status_flag = 0xFF;

/**
 * \brief SWDT 中断服务程序
 */
static void swdt_isr (void *p_arg , uint32_t flag)
{
    if (flag == AM_HC32F460_SWDT_STATUS_REFRESH_ERROR) {
        g_swdt_status_flag = AM_HC32F460_SWDT_STATUS_REFRESH_ERROR;
    } else if (flag == AM_HC32F460_SWDT_STATUS_COUNT_UNDERFLOW) {
        g_swdt_status_flag = AM_HC32F460_SWDT_STATUS_COUNT_UNDERFLOW;
    }
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_drv_swdt_int_entry (am_wdt_handle_t         handle,
                                       am_hc32f460_swdt_dev_t *p_dev,
                                       uint32_t                feed_time_ms)
{
    am_wdt_info_t info;

    AM_DBG_INFO("The chip Reset by External Reset Pin or SWDT \r\n");

    am_wdt_info_get(handle, &info);

    AM_DBG_INFO("The SWDT support min time is %d ms\r\n", info.min_timeout_ms);
    AM_DBG_INFO("The SWDT support max time is %d ms\r\n", info.max_timeout_ms);

    am_hc32f460_swdt_isr_connect(p_dev, swdt_isr, (void *)0);

    while (1) {

        /* 喂狗操作 */
        am_wdt_feed(handle);

        /* 延时，当延时大于喂狗时间时,会产生看门狗事件，MCU 复位 */
        am_mdelay(feed_time_ms);

        if (g_swdt_status_flag == AM_HC32F460_SWDT_STATUS_COUNT_UNDERFLOW) {
            AM_DBG_INFO("wdt counter underflow !\r\n");
        } else if (g_swdt_status_flag == AM_HC32F460_SWDT_STATUS_REFRESH_ERROR) {
            AM_DBG_INFO("wdt counter refresh error !\r\n");
        } else {
            AM_DBG_INFO("normal feed dog !\r\n");
        }

    }
}
/** [src_drv_swdt] */

/* end of file */
