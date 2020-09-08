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
 * \brief fm175xx读B类卡的ID，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将B类卡（如二代身份证）置于天线感应区。
 *
 * - 实验现象：
 *   1. 将卡片放置天线感应区后串口打印出卡片的ID。
 *
 * - 注意：
 *   1. 将卡片放置天线感应区后串口只能打印一次ID，若要再次打印ID需要将卡片移除感应区再移入;
 *   2. fm17510不支持B类卡的读写。
 *
 * \par 源代码
 * \snippet demo_stm32f103rbt6_dr_fm175xx_piccb_read_id.c src_stm32f103rbt6_dr_fm175xx_piccb_read_id
 *
 * \internal
 * \par Modification history
 * - 1.00 19-03-25  wk, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_fm175xx_piccb_read_id
 * \copydoc demo_stm32f103rbt6_dr_fm175xx_piccb_read_id.c
 */

/** [src_stm32f103rbt6_dr_fm175xx_piccb_read_id] */

#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_stm32f103rbt6_core_entries.h"

/**
 * \brief B类卡读卡号例程
 */
void demo_stm32f103rbt6_core_dr_fm175xx_piccb_read_id (void)
{
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    demo_fm175xx_piccb_read_id(handle);
}

/** [src_am116_dr_fm175xx_piccb_read_id] */

/* end of file */
