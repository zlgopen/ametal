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
 * \brief fm175xx使用不同的方法读取卡片信息，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将A类卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 如果卡片支持halt指令，则每次重新将卡放置天线感应区后，串口只能打印出两次卡片信息。
 *   2. 如果卡片不支持halt指令，则将卡放置天线感应区后，串口能连续不断的打印出卡片信息。
 *
 * \par 源代码
 * \snippet demo_am116_dr_fm175xx_picca_halt.c src_am116_dr_fm175xx_picca_halt
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-17  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_fm175xx_picca_halt
 * \copydoc demo_am116_dr_fm175xx_picca_halt.c
 */

/** [src_am116_dr_fm175xx_picca_halt] */

#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_mm32l373_core_entries.h"

/**
 * \brief A类卡读休眠卡例程
 */
void demo_mm32l373_core_dr_fm175xx_picca_halt (void)
{
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    demo_fm175xx_picca_halt(handle);
}

/** [src_am116_dr_fm175xx_picca_halt] */

/* end of file */
