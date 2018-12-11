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
 *   1. 先用密钥A对卡片指定的块进行验证，如果验证不通过则打印key A authent failed。
 *   2. 如果验证成功，则将缓冲区的数据写入指定的块后清空缓冲区，再将块中的数据读取出来，如果与写入的数据
 *      一致，表明写数据成功。
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
void demo_amks16rfid8_fm175xx_picca_read_block (void)
{
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    am_cd4051_pin_init(&__g_antenna_info);
    /* 选择天线1 */
    am_cd4051_channel_selected(&__g_antenna_info, 0);

    demo_fm175xx_picca_read_block(handle);
}

/** [src_amks16rfid8_fm175xx_picca_block] */

/* end of file */
