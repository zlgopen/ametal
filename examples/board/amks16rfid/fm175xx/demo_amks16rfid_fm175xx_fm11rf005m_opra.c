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
 * \brief fm175xx对FM11R005M类型卡进行操作
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将FM11R005M类型卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 串口将会打印FM11R005M卡的UID
 *   2. 串口将会打印写入的数据
 *
 * \par 源代码
 * \snippet demo_amks16rfid_dr_fm11rf005m_opra.c src_amks16rfid_dr_fm11rf005m_opra
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-17  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16rfid_dr_fm11rf005m_opra
 * \copydoc demo_amks16rfid_dr_fm11rf005m_opra.c
 */

/** [src_amks16rfid_dr_fm11rf005m_opra] */

#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"

/**
 * \brief FM11RF005M卡操作例程
 */
void demo_amks16rfid_dr_fm11rf005m_operate (void)
{
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    demo_fm175xx_fm11rf005m_operate(handle);
}

/** [src_amks16rfid_dr_fm11rf005m_opra] */

/* end of file */
