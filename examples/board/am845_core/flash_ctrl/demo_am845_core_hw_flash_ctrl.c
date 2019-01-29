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
 * \brief 闪存控制器例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 本程序会将签名信息通过串口打印到终端上；
 *   2. 显示分为两部分，分别为软件计算的结果和硬件计算的结果；
 *   3. 软件计算结果和硬件结算结果应该相同；
 *   4. LED 以 0.5s 的时间间隔闪烁。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *       PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \note
 *   可访问的地址范围为 FLASH 空间(0x0000~0x8000)。
 *
 * \par 源代码
 * \snippet demo_am845_core_hw_flash_ctrl.c src_am845_core_hw_flash_ctrl
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-04  sky, modified
 * - 1.00 15-07-16  aii, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_hw_flash_ctrl
 * \copydoc demo_am845_core_hw_flash_ctrl.c
 */

/** [src_am845_core_hw_flash_ctrl] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc_fmc.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am845_core_hw_flash_ctrl_entry (void)
{
    AM_DBG_INFO("demo_am845_hw_flash_ctrl!\r\n");

    /* 设置闪存访问时间为两个系统时钟 */
    demo_lpc845_hw_flash_ctrl_entry(AMHW_LPC_FMC_FLASHTIM_2);
}
/** [src_am845_core_hw_flash_ctrl] */

/* end of file */
