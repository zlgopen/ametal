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
 * \brief SHT20 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口打印SHT20采集的温湿度。
 *
 * \note
 *    串口打印温湿度信息，需要将 PIOB_11 引脚连接 PC 串口的 TXD，
 *    PIOB_12 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zsl42x_std_sht20.c src_std_sht20
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-20  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_std_sht20
 * \copydoc demo_zsl42x_std_sht20.c
 */

/** [src_std_sht20] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "demo_am42x_core_entries.h"
#include "am_hwconf_sht20.h"

/**
 * \brief 例程入口
 */
void demo_zsl42x_core_std_sht20_entry (void)
{
    AM_DBG_INFO("demo am42x_core std sht20!\r\n");

    am_sht20_handle_t handle = am_sht20_inst_init();

    demo_std_sht20_entry(handle);
}
/** [src_std_led] */

/* end of file */
