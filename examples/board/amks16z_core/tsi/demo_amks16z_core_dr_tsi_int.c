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
 * \brief   TSI触摸中断检测，驱动层实现
 *
 *  实验现象：
 *  1.实验配置PIOA_1引脚为TSI通道2功能，当手不接触PIOA_1引脚时不会触发中断
 *  2.当触摸PIOA_1引脚后，电容检测大于阀值上限1值时，进入中断，LED1状态翻转
 *
 *  注意：
 *  1.当TSI设置为软件触发模式时，需要执行启动软件扫描函数才能执行一次扫描，所以
 *  在使用时，当配置为软件触发模式时，需要一直或者定时执行启动扫描函数；
 *  2.运行该例程时PIOA_1不能配置成串口引脚。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_dr_tsi_int.c src_amks16z_core_dr_tsi_int
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-26  nwt, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_amks16z_core_if_dr_tsi_int
 * \copydoc demo_amks16z_core_dr_tsi_int.c
 */
 
/** [src_amks16z_core_dr_tsi_int] */
#include "ametal.h"
#include "hw/amhw_fsl_tsi.h"
#include "am_fsl_tsi.h"
#include "am_KL26_inst_init.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "demo_fsl_entrys.h"

/**
 * \brief 主函数入口
 */
void demo_amks16z_core_dr_tsi_int_entry (void)
{
    AM_DBG_INFO("demo amks16z_core dr tsi int!\r\n");

    /* TSI设备初始化 */
    am_kl26_tsi_inst_init();

    demo_fsl_dr_tsi_int_entry();
}

/** [src_amks16z_core_dr_tsi_int] */
 
/* end of file */

