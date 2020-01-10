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
 * \brief PCNT 双通道非交脉冲计数例程，通过标准接口和中间驱动层函数实现
 *
 * - 实验现象：
 *   1. PCNT双通道非交脉冲计数，在PB5、PB7引脚输入非交脉冲信号，串口打印计数值。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32l19x_drv_pcnt_special.c src_hc32l19x_drv_pcnt_special
 *
 * \internal
 * \par Modification history
 * - 1.00 19-11-22  RZ, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32l19x_drv_pcnt_special
 * \copydoc demo_hc32l19x_drv_pcnt_special.c
 */

/** [src_hc32l19x_drv_pcnt_special] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32_pcnt.h"
#include "am_hc32l19x_inst_init.h"
#include "demo_aml19x_core_entries.h"
#include "hc32_pin.h"
/**
 * \brief 例程入口
 */
void demo_hc32l19x_core_drv_pcnt_special_entry (void)
{
    am_hc32_pcnt_handle_t handle;
    uint16_t cnt = 0;

    AM_DBG_INFO("demo aml19x_core pcnt special entry!\r\n");

    handle = am_hc32_pcnt_inst_init();

    am_hc32_pcnt_clk_sel(handle, HC32_PCNT_PCLK);

    am_hc32_pcnt_polar_set(handle, HC32_PCNT_S0PNOINVERT);
    am_hc32_pcnt_polar_set(handle, HC32_PCNT_S1PNOINVERT);

    am_hc32_pcnt_flt_enable(handle, 5, 5);

    am_hc32_pcnt_timeover_disable(handle);

    am_hc32_pcnt_start(handle,
                       HC32_PCNT_SPECIAL,
                       HC32_PCNT_UP,
                       100,
                       HC32_PCNT_DGB_0);
    while(1) {
        cnt = am_hc32_pcnt_cnt_get(handle);
        AM_DBG_INFO("pcnt special cnt is %d !\r\n", cnt);
        am_mdelay(500);
    }
}
/** [src_hc32l19x_drv_pcnt_special] */

/* end of file */
