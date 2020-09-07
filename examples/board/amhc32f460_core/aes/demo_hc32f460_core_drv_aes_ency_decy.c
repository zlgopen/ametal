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
 * \brief trng 例程，通过驱动层接口实现
 *
 * - 实验现象：
 *
 * \note
 *
 * \par 源代码
 * \snippet demo_hc32f460_drv_aes.c src_drv_trng
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-25  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_drv_aes
 * \copydoc demo_hc32f460_drv_aes.c
 */

/** [src_hc32f460_drv_aes] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "am_hc32f460_aes.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_entries.h"

/*******************************************************************************
 * 全局变量
 ******************************************************************************/
/**< \brief 存放待加密的数据*/
static uint32_t aes_data[4] = {0x33221100, 0x77665544, 0xBBAA9988, 0xFFEEDDCC};

/**< \brief AES密匙 */
static uint32_t aes_key[8]  = {0x03020100, 0x07060504, 0x0B0A0908, 0x0F0E0D0C,\
                               0x13121110, 0x17161514, 0x1B1A1918, 0x1F1E1D1C};

/**
 * \brief 例程入口
 */
void demo_hc32f460_core_drv_aes_entry (void)
{
    AM_DBG_INFO("demo amhc32f460_core drv aes-ency-decy !\r\n");
    demo_hc32f460_drv_aes_ency_decy_entry (am_hc32f460_aes_inst_init(), aes_data, aes_key);
}
/** [src_drv_aes] */

/* end of file */
