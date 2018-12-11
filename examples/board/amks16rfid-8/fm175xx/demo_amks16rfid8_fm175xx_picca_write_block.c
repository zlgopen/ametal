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
 * \brief fm175xx块数据读写操作，通过驱动接口实现
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
 * \snippet demo_amks16rfid8_fm175xx_picca_block.c src_amks16rfid8_fm175xx_picca_block
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-21  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_fm175xx_picca_block
 * \copydoc demo_amks16rfid8_fm175xx_picca_block.c
 */

/** [src_amks16rfid8_fm175xx_picca_block] */
#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "kl26_pin.h"
#include "am_kl26_inst_init.h"
#include "am_fm175xx_ant.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_amks16rfid8_entries.h"

/**
 * \name 定义CD4051的引脚
 * @{
 */
#define __CD4051_PIN_EN PIOB_19
#define __CD4051_PIN_S2 PIOB_18
#define __CD4051_PIN_S1 PIOB_17
#define __CD4051_PIN_S0 PIOB_16

/* 定义 fm175xx 天线切换信息 */
static am_antenna_info_t  __g_antenna_info = {
    {
        __CD4051_PIN_EN,
        __CD4051_PIN_S0,
        __CD4051_PIN_S1,
        __CD4051_PIN_S2
    },
    AM_FM175XX_ANTENNA_EIGHT
};

/**
 * \brief A类卡块读写例程
 */
void demo_amks16rfid8_fm175xx_picca_write_block (void)
{
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    am_cd4051_pin_init(&__g_antenna_info);
    /* 选择天线1 */
    am_cd4051_channel_selected(&__g_antenna_info, 0);

    demo_fm175xx_picca_write_block(handle);
}

/** [src_amks16rfid8_fm175xx_picca_block] */

/* end of file */
