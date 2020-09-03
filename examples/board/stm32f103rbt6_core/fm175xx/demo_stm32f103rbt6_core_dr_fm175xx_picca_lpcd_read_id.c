/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief fm175xx读卡类型和ID，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将A类卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 串口打印出卡片类型号和卡号及卡片信息
 *
 * \par 源代码
 * \snippet demo_am116_dr_fm175xx_picca_read_id.c src_am116_dr_fm175xx_picca_read_id
 *
 * \internal
 * \par Modification history
 * - 1.00 18-08-07  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_am116_core_dr_fm175xx_picca_read_id
 * \copydoc demo_am116_core_dr_fm175xx_picca_read_id.c
 */

/** [src_am116_dr_fm175xx_picca_read_id] */

#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_stm32f103rbt6_core_entries.h"

/**
 * \brief A类LPCD模式 卡读卡类型和卡号例程
 */
void demo_stm32f103rbt6_core_dr_fm175xx_picca_lpcd_read_id (void)
{
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    demo_fm175xx_picca_lpcd_mode(handle);
}

/** [demo_am116_core_dr_fm175xx_picca_read_id] */

/* end of file */



