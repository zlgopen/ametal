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
 * \brief LTR_553ALS 例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_ltr_553als.c src_ltr_553als
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-07  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_ltr_553als
 * \copydoc demo_ltr_553als.c
 */

/** [src_ltr_553als] */
#include "ametal.h"
#include "am_delay.h"
#include "am_ltr_553als_01.h"
#include "am_hwconf_ltr_553als.h"
#include "am_vdebug.h"


/**
 * \brief 例程入口
 */
void demo_ltr_553als_entry (void)
{  
    uint16_t als = 0;
    uint16_t ps = 0;
    am_ltr_553als_handle_t handle;

    handle = am_ltr_553als_inst_init();
    
    while (1) {
        am_ltr_553als_read_als (handle, &als);
        am_ltr_553als_read_ps (handle, &ps);
        AM_DBG_INFO("LTR_553ALS = %d     LTR_553PS = %d\r\n", als, ps);
        am_mdelay(500);
    }
}
/** [src_ltr_553als] */

/* end of file */
