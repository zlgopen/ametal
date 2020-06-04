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
 * \brief DMA 内存到内存例程，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. 指定的 LED1 以 10Hz 的频率闪烁。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOB_3 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_aml165_core_std_delay.c src_aml165_core_std_delay
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-11 ari , first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_std_delay
 * \copydoc demo_aml165_core_std_delay.c
 */

/** [src_aml165_core_std_delay] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zml165.h"
#include "am_board.h"
#include "demo_std_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_std_delay_entry (void)
{
    AM_DBG_INFO("demo aml165_core std delay!\r\n");

    demo_std_delay_entry(LED1);
}
/** [src_aml165_core_std_delay] */

/* end of file */
