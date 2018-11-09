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
 * \brief 程序清单7.55
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-21  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_hwconf_miniport.h"
#include "app_digitron_count_down.h"

int am_main (void)
{
    am_miniport_view_595_inst_init();      // MiniPort-View数码管（HC595输出段码）实例初始化
    app_digitron_count_down(0);            // 使用显示器编号为0的数码管
    while (1) {
    }
}


/* end of file */

