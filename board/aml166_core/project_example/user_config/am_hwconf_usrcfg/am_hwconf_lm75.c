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
 * \brief LM75 配置文件
 * \sa am_hwconf_lm75.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation.
 * \endinternal
 */
#include <am_aml166_inst_init.h>
#include "ametal.h"
#include "am_temp_lm75.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_lm75
 * \copydoc am_hwconf_lm75.c
 * @{
 */

/* 定义 LM75 实例 */
static am_temp_lm75_t  __g_temp_lm75;

/* 定义 LM75 实例信息 */
static const am_temp_lm75_info_t __g_temp_lm75_info = {
    0x48
};

am_temp_handle_t am_temp_lm75_inst_init (void)
{
    return  am_temp_lm75_init(&__g_temp_lm75,
                              &__g_temp_lm75_info,
                              am_zlg116_gpio_i2c_inst_init());
}

/**
 * @}
 */

/* end of file */
