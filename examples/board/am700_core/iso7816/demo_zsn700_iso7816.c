/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2020 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief ISO7816 接触式卡例程
 *
 * - 实验现象：
 *   1. 串口输出获取的随机数。
 *
 *
 * \par 源代码
 * \snippet demo_zsn700_iso7816.c src_iso7816
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-27  licl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_iso7816
 * \copydoc demo_zsn700_iso7816.c
 */

/** [src_iso7816] */
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_components_entries.h"
#include "demo_am700_core_entries.h"
#include "am_zsn700_inst_init.h"

/**
 * \brief 例程入口
 */
void demo_zsn700_iso7816_entry (void)
{
    AM_DBG_INFO("demo am700 iso7816!\r\n");

    am_iso7816_handle_t iso7816_handle = am_iso7816_inst_init();

    demo_iso7816_entry(iso7816_handle);
}
/** [src_iso7816] */

/* end of file */
