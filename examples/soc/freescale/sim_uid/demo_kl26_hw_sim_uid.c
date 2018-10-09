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
 * \brief sim例程，通过HW层接口实现
 *
 * - 实验现象：
 *   1. 串口打印通用唯一标识符UID。
 *
 *
 * \par 源代码
 * \snippet demo_kl26_hw_sim_uid.c src_kl26_hw_sim_uid
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_hw_sim_uid
 * \copydoc demo_kl26_hw_sim_uid.c
 */

/** [src_kl26_hw_sim_uid] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_kl26_clk.h"
#include "hw/amhw_kl26_sim.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

/**
 * \brief 例程入口
 */
void demo_kl26_hw_sim_uid_entry (void)
{
    uint16_t uid_h = 0;

    uint32_t uid_m = 0;

    uint32_t uid_l = 0;

    uid_h = amhw_kl26_sim_uid_high_get();
    uid_m = amhw_kl26_sim_uid_mid_get();
    uid_l = amhw_kl26_sim_uid_low_get();

    am_kprintf("%d",uid_h);
    am_kprintf("%d",uid_m);
    am_kprintf("%d\r\n",uid_l);
}


/** [src_kl26_hw_sim_uid] */

/* end of file */
