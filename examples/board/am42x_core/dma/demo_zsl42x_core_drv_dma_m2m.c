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
 *   1. DMA 传输成功，打印 "transfer success";
 *   2. DMA 传输出错，打印 "transfer failure";
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOB_11 引脚连接 PC 串口的 TXD，
 *    PIOB_12 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zsl42x_drv_dma_m2m.c src_zsl42x_drv_dma_m2m
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-20 zp , first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_drv_dma_m2m
 * \copydoc demo_zsl42x_drv_dma_m2m.c
 */

/** [src_zsl42x_drv_dma_m2m] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_zsl42x.h"
#include "demo_zsl42x_entries.h"
#include "demo_am42x_core_entries.h"


#define  DMA_M2M_CH       DMA_CHAN_1   /**< \brief 通道 */

/**
 * \brief 例程入口
 */
void demo_zsl42x_core_drv_dma_m2m_entry (void)
{
    AM_DBG_INFO("demo am42x_core drv dma m2m!\r\n");

    demo_zsl42x_drv_dma_m2m_entry(DMA_M2M_CH);
}
/** [src_zsl42x_drv_dma_m2m] */

/* end of file */
