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
 * \brief fm175xx写块数据，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将A类卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 先用密钥A对卡片指定的块进行验证，如果验证不通过则打印key A authent failed。
 *   2. 如果验证成功，并且指定的块中的格式为数值块的格式那么将块中的数值减5再打印出来。
 *   3. 如果验证成功，但是指定块中的格式不是数值块的格式，那么不会打印数值，此时可以将130行的条件编译
 *      改为1，编译后执行，刷卡时会不断的打印同一个数字，之后再将130行的条件编译改为0，编译后执行，再
 *      刷卡，则串口将打印出从15开始，不断-5的数值。
 *
 * \par 源代码
 * \snippet demo_am116_dr_fm175xx_picca_val_operate.c src_am116_dr_fm175xx_picca_val_operate
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-22  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_fm175xx_picca_val_operate
 * \copydoc demo_am116_dr_fm175xx_picca_val_operate.c
 */

/** [src_am116_dr_fm175xx_picca_val_operate] */

#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_hk32f103rbt6_core_entries.h"

/**
 * \brief A类卡值操作例程
 */
void demo_hk32f103rbt6_core_dr_fm175xx_picca_val_operate (void)
{
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    demo_fm175xx_picca_val_operate(handle);
}

/** [src_am116_dr_fm175xx_picca_val_operate] */

/* end of file */
