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
 * \brief PCNT 双通道正交脉冲计数例程，通过标准接口和中间驱动层函数实现
 *
 * - 实验现象：
 *   1. PCNT通道正交脉冲计数，在PB5、PB7引脚输入正交脉冲信号，串口打印计数值。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zsl42x_drv_pcnt_double.c src_zsl42x_drv_pcnt_double
 *
 * \internal
 * \par Modification history
 * - 1.00 19-11-22  RZ, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_drv_pcnt_double
 * \copydoc demo_zsl42x_drv_pcnt_double.c
 */

/** [src_zsl42x_drv_pcnt_double] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_zsl42x_pcnt.h"
#include "am_zsl42x_inst_init.h"
#include "demo_am42x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zsl42x_core_drv_pcnt_double_entry (void)
{
    am_zsl42x_pcnt_handle_t handle;
    uint16_t cnt = 0;

    AM_DBG_INFO("demo am42x_core pcnt double entry!\r\n");

    handle = am_zsl42x_pcnt_inst_init();

    am_zsl42x_pcnt_clk_sel(handle, ZSL42x_PCNT_PCLK);

    am_zsl42x_pcnt_polar_set(handle, ZSL42x_PCNT_S0PNOINVERT);

    am_zsl42x_pcnt_flt_enable(handle, 5, 5);

    am_zsl42x_pcnt_timeover_disable(handle);

    am_zsl42x_pcnt_start(handle,
                       ZSL42x_PCNT_DOUBLE,
                       ZSL42x_PCNT_UP,
                       100,
                       ZSL42x_PCNT_DGB_0);
    while(1) {
        cnt = am_zsl42x_pcnt_cnt_get(handle);
        AM_DBG_INFO("pcnt double cnt is %d !\r\n", cnt);
        am_mdelay(500); 
    }
}
/** [src_zsl42x_drv_pcnt_double] */

/* end of file */
