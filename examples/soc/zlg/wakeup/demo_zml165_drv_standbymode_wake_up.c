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
 * \brief 待机模式例程，通过驱动层接口实现
 *
 * - 操作步骤：
 *   1. 上电前将 WAKE_UP 连接到低电平；
 *   2. 等待 MCU 进入待机模式；
 *   3. 将 WAKE_UP 连接到高电平，使 WAKE_UP 上产生上升沿唤醒 MCU。
 *
 * - 实现现象
 *   1. 成功进入待机模式时，串口输出"enter standby!"；
 *      否则串口输出"WAKE_UP must be low!"；
 *   2. WAKE_UP 产生上升沿后，MCU 复位运行。
 *
 * \note
 *    只有在 WAKE_UP 为低电平的时候才能进入待机模式
 *
 * \par 源代码
 * \snippet demo_zlg116_drv_standbymode_wake_up.c src_zlg116_drv_standbymode_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_zlg116_drv_standbymode_wake_up
 * \copydoc demo_zlg116_drv_standbymode_wake_up.c
 */
 
/** [src_zlg116_drv_standbymode_wake_up] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zml165_pwr.h"

/**
 * \brief 例程入口
 */
void demo_zml165_drv_standbymode_wake_up_entry (void)
{

    /* 唤醒配置 */
    am_zml165_wake_up_cfg(AM_ZML165_PWR_MODE_STANBY, NULL, NULL);

    /* 进入待机模式 */
    if (am_zml165_pwr_mode_into(AM_ZML165_PWR_MODE_STANBY) != AM_OK) {

        /* 只有在 WAKE_UP 为低电平的时候才能进入待机模式 */
        AM_DBG_INFO("WAKE_UP must be low!\r\n");
    }

    /* 不应该执行到这里，因为待机模式唤醒之后芯片会直接复位 */
    AM_DBG_INFO("error!\r\n");

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zlg116_drv_standbymode_wake_up] */

/* end of file */
