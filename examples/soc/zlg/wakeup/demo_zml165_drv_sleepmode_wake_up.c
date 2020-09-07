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
 * \brief 睡眠模式例程，通过驱动层接口实现
 *
 * - 实现现象
 *   1. MCU 进入停止模式时， 调试串口打印调试信息；
 *   2. 指定引脚上的中断唤醒 MCU，串口输出 wake_up，程序继续运行。
 *
 * \par 源代码
 * \snippet demo_zlg116_drv_sleepmode_wake_up.c src_zlg116_drv_sleepmode_wake_up
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg116_drv_sleepmode_wake_up
 * \copydoc demo_zlg116_drv_sleepmode_wake_up.c
 */

/** [src_zlg116_drv_sleepmode_wake_up] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zml165_pwr.h"

/**
 * \brief 引脚中断服务函数
 */
static void gpio_isr (void *p_arg)
{
    ;
}

/**
 * \brief 例程入口
 */
void demo_zml165_drv_sleepmode_wake_up_entry (void)
{

    /* 唤醒配置 */
    am_zml165_wake_up_cfg(AM_ZML165_PWR_MODE_SLEEP, gpio_isr, (void *)0);

    /* 进入睡眠模式 */
    am_zml165_pwr_mode_into(AM_ZML165_PWR_MODE_SLEEP);

    AM_DBG_INFO("wake_up!\r\n");

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zlg116_drv_sleepmode_wake_up] */

/* end of file */
