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
 * \brief CLKTRIM 例程，通过HW接口实现
 *
 * - 实验现象：
 *   1. 时钟校准：串口打印待校准时钟与参考时钟频率比值；
 *   2. 时钟监测：串口打印被监控时钟工作状态信息；
 *
 * \par 源代码
 * \snippet demo_hc32f07x_core_clktrim.c src_hc32f07x_core_clktrim
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f07x_core_clktrim
 * \copydoc demo_hc32f07x_core_clktrim.c
 */

/** [src_hc32f07x_core_fclktrim] */
#include "hw/amhw_hc32_rcc.h"
#include "ametal.h"
#include "am_delay.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_hc32f07x_clktrim.h"
#include "am_hc32.h"
#include "am_hc32_clk.h"
#include "hc32_clk.h"

/**
 * \brief 例程入口
 */
void demo_hc32f07x_core_clktrim_entry (void)
{
    uint16_t ret = 0;

    AM_DBG_INFO("demo amf07x_core clktrim entry!\r\n");

    /* 校准时钟参数设置 */
    amhw_hc32_rcc_rch_trim_set(*((volatile uint16_t*) AMHW_HC32_RCH_FRE_8MHz));

    /* 时钟校准 */
    ret = am_hc32f07x_clktrim_calibrate (1000,
                                         AMHW_CLKTRIM_REFCLK_XTH,
                                         AMHW_CLKTRIM_CALCLK_RCH);

    AM_DBG_INFO("the Fcal cnt is: %d\r\n",ret);

    /* 时钟监测 */
    ret = am_hc32f07x_clktrim_monitor (10000,
                                       100,
                                       AMHW_CLKTRIM_REFCLK_IRC10K,
                                       AMHW_CLKTRIM_CALCLK_XTL);
    if (ret == AM_OK){
        AM_DBG_INFO("the calclk is Ok!");
    } else {
        AM_DBG_INFO("the calclk is Fail!");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32f07x_core_fclktrim] */

/* end of file */
