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
 * \brief ≥Ã–Ú«Âµ• 9.81
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#include "app_mvc_bool_main.h"
#include "am_lpc82x_inst_init.h"

int am_main (void)
{
    am_zm516x_handle_t zm516x_handle = am_zm516x_inst_init();
    return app_mvc_bool_main(0, KEY_F1, zm516x_handle);
}
/* end of file */
