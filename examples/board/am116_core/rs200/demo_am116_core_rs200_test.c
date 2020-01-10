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
/** [src_am116_core_rs200_test] */
#include "demo_std_entries.h"
#include "demo_am116_core_entries.h"
#include "am_hwconf_rs200.h"
#include "demo_components_entries.h"
#include "am_zlg116_inst_init.h"
/**
 * \brief Àý³ÌÈë¿Ú
 */
void demo_am116_core_rs200_test_entry (void)
{
    am_rs200_handle_t handle = am_rs200_inst_init();
    demo_rs200_entry(handle);
}
/** [src_am116_core_rs200_test] */

/* end of file */
