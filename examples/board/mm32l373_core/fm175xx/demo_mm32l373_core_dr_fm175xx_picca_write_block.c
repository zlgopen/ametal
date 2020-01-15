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
 *   2. 如果验证成功，则将缓冲区的数据写入指定的块后清空缓冲区，再将块中的数据读取出来，如果与写入的数据
 *      一致，表明写数据成功。
 *
 * \par 源代码
 * \snippet demo_am116_dr_fm175xx_picca_write_block.c src_am116_dr_fm175xx_picca_write_block
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-21  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_fm175xx_picca_write_block
 * \copydoc demo_am116_dr_fm175xx_picca_write_block.c
 */

/** [src_am116_dr_fm175xx_picca_write_block] */

#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_mm32l373_core_entries.h"

/**
 * \brief A类卡写块例程
 */
void demo_mm32l373_core_dr_fm175xx_picca_write_block (void)
{
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    demo_fm175xx_picca_write_block(handle);
}

/** [src_am116_dr_fm175xx_picca_write_block] */

/* end of file */
