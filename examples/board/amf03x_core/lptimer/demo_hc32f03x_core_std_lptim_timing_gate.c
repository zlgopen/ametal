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
 * \brief LPTIM0定时器TIMING功能，LPTIM0定时器对外部信号下降沿进行计数，通过标准接口实现
 *
 * \步骤
 *   1.在am_hwconf_hc32f03x_lptim.c文件中，__g_lptim0_timing_devinfo设备信息结构体选择
 *             AMHW_HC32_LPTIM_FUNCTION_TIMER  ----------- 定时器模式
 *             AMHW_HC32_LPTIM_MODE_RELOAD     ----------- 自动重载模式
 *             AM_FALSE                          ----------- 门控关闭
 *             AM_TRUE                           ----------- TOG、TOGN输出使能打开
 *   2.在am_hwconf_hc32f03x_lptim.c文件中，__g_lptim1_timing_devinfo设备信息结构体选择
 *             AMHW_HC32_LPTIM_FUNCTION_COUNTER----------- 计数器模式
 *             AMHW_HC32_LPTIM_MODE_RELOAD     ----------- 自动重载模式
 *             AM_FALSE                          ----------- 门控关闭
 *             AM_TRUE                           ----------- TOG、TOGN输出使能打开
 *   3.将PIOD_6（LPTIM1_ETR）和PIOC_1（LPTIM0_TOG）连接。
 *
 *
 * - 实验现象：
 *   1.PIOC_1（LPTIM0_TOG）和PIOC_2（LPTIM0_TOGN）输出反相、频率为500kHz的方波。
 *   2.LED0以5Hz的频率翻转。
 *
 * \note由于定时器定时1us不精确的缘故，可能存在较大误差，但是PIOC_1和LED0闪烁的频率关系固定为100000:1
 *
 * \par 源代码
 * \snippet demo_hc32f03x_std_lptim_timing.c src_hc32f03x_std_lptim_timing
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-29  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f03x_std_lptim_timing
 * \copydoc demo_hc32f03x_std_lptim_timing.c
 */

/** [src_hc32f03x_std_lptim_timing] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_timer.h"
#include "demo_std_entries.h"
#include "am_hc32f03x_inst_init.h"
#include "demo_amf03x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f03x_core_std_lptim_timing_gate_entry (void)
{
    am_timer_handle_t  handle = am_hc32_lptim0_timing_inst_init();
    
    AM_DBG_INFO("demo amf03x_core std lptim timing gate!\r\n");

    /* 设置回调函数 */
    am_timer_callback_set(handle, 0, NULL, NULL);

    /* 设置定时时间为1us,TOG、TOGN输出频率周期时间为1*2=2us，即500KHz */
    am_timer_enable_us(handle, 0, 1);

    demo_std_timer_timing_entry(am_hc32_lptim0_timing_inst_init(), 0);
}
/** [src_hc32f03x_std_lptim_timing] */

/* end of file */
