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
 * \brief BH1730FVC例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_bh1730fvc.c src_bh1730fvc
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-12  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_bh1730fvc
 * \copydoc demo_bh1730fvc.c
 */

/** [src_bh1730fvc] */
#include "ametal.h"
#include "am_delay.h"
#include "am_bh1730fvc.h"
#include "am_hwconf_bh1730fvc.h"
#include "am_vdebug.h"


/**
 * \brief BH1730FVC例程入口
 */
void demo_bh1730fvc_entry (void)
{  
    am_bh1730fvc_handle_t handle;
    uint16_t als_adc = 0;
    
    handle = am_bh1730fvc_inst_init();    

    while (1) {

        am_bh1730fvc_read_als_adc (handle, &als_adc);

        AM_DBG_INFO("BH1730FVC_ALS_ADC   =  %d  \r\n", als_adc);
        am_mdelay(500);
    }
}
/** [src_bh1730fvc] */

/* end of file */
