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
 * \brief fm175xx读块数据，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将A类卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 先用密钥A对卡片的块2进行验证，如果验证不通过则打印key A authent failed。
 *   2. 如果验证成功，则读取块0（包含卡片序列号）、块3（包含密钥A，存取控制部分和密钥B）、块4（和块2
 *      不在同一个扇区），并打印出来。
 *
 * - 注意：
 *   1. 每个扇区包含4个块，其中第0到第2个块为数据块，第3个块为控制块。验证其实是对扇区而言的，但是给
 *      am_fm175xx_picca_m1_authent()函数传入的最后一个参数为块地址，因此该函数的意义
 *      其实是验证传入的块所在的扇区，验证成功后便可以根据控制块中的控制权限对本扇区所有的块进行读写。
 *   2. 块0中的数据是卡片序列号，该块只读。
 *   3. 块3位控制块，虽然本例传入的A密钥为全F，但是读取出来的A密钥却为全0，因为A密钥在任何情况下都没有
 *      读的权限。
 *   4. 块4与验证的块2不在同一个扇区，因此读出来的数据是不正确的，除非对块4所在的扇区进行验证。
 *
 * \par 源代码
 * \snippet demo_am116_dr_fm175xx_picca_read_block.c src_am116_dr_fm175xx_picca_read_block
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-21  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_fm175xx_picca_read_block
 * \copydoc demo_am116_dr_fm175xx_picca_read_block.c
 */

/** [src_am116_dr_fm175xx_picca_read_block] */

#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_mm32l073_core_entries.h"

/**
 * \brief A类卡读块例程
 */
void demo_mm32l073_core_dr_fm175xx_picca_read_block (void)
{
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    demo_fm175xx_picca_read_block(handle);
}


/** [src_am116_dr_fm175xx_picca_read_block] */

/* end of file */
