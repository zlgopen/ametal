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
 * \brief CS1237 配置文件
 * \sa am_hwconf_cs1237.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-11-11  htf, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "zml165_pin.h"
#include "am_cs1237.h"

/**
 * \addtogroup am_if_src_hwconf_cs1237
 * \copydoc am_hwconf_cs1237.c
 * @{
 */

/* 定义 CS1237 实例 */
static am_cs1237_adc_dev_t __g_cs1237_dev;

/* 定义 CS1237 实例信息 */
const am_cs1237_adc_devinfo_t __g_cs1237_info = {
    PIOB_8,
    PIOB_0,
    PIOB_0,
    0,
    3000,
    0,
    0,
    100
};

am_cs1237_adc_handle_t am_cs1237_inst_init(void)
{
    return am_cs1237_init(&__g_cs1237_dev, &__g_cs1237_info);
}
/**
 * @}
 */

/* end of file */

