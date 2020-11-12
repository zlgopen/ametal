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
 * \brief CTS时钟校准例程
 *
 * - 实验现象：
 *   1. 串口打印校准成功信息
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f07x_core_cts_entry.c src_hc32f07x_core_cts_entry
 *
 * \internal
 * \par Modification history
 * - 1.00 19-11-22  RZ, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f07x_core_cts_entry
 * \copydoc demo_hc32f07x_core_cts_entry.c
 */

/** [src_hc32f07x_core_cts_entry] */
#include "hw/amhw_hc32f07x_rcc.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32f07x_cts.h"
#include "demo_amf07x_core_entries.h"
#include "am_hc32f07x_clk.h"
#include "hc32f07x_clk.h"

/**
 * \brief 例程入口
 */
void demo_hc32f07x_core_cts_entry (void)
{
    int ret = 0;	
    
    AM_DBG_INFO("demo amf07x_core cts entry!\r\n");

    amhw_hc32f07x_rcc_set_start(0x5A5A);
    amhw_hc32f07x_rcc_set_start(0xA5A5);
    amhw_hc32f07x_rcc_xtl_enable();

    am_clk_enable(CLK_CTS);

    ret = am_hc32f07x_cts_calibrate( 12000000,
                                     32768,
                                     AMHW_HC32F07X_CTS_CLKSRC_XTH ,
                                     AMHW_HC32F07X_CTS_REFSRC_XTL);
    if(ret == AM_OK){
        AM_DBG_INFO("the calibrate is success!\r\n");
    } else {
        AM_DBG_INFO("the calibrate is fail!\r\n");
    }

    AM_FOREVER{
       ;	
    }

}
/** [src_hc32f07x_core_cts_entry] */

/* end of file */
