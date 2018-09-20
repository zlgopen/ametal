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
 * \brief SHTC1 配置文件
 * \sa am_hwconf_shtc1.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_shtc1.h"
#include "am_zlg217_inst_init.h"
#include "zlg217_pin.h"

/**
 * \addtogroup am_if_src_hwconf_shtc1
 * \copydoc am_hwconf_shtc1.c
 * @{
 */

/*******************************************************************************
  SHTC1 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_shtc1_dev_t __g_shtc1_dev;

/** \brief 实例初始化 */
am_shtc1_handle_t am_shtc1_inst_init (void)
{
    /* 获取 I2C 实例句柄 */
    am_i2c_handle_t i2c_handle = am_zlg217_i2c1_inst_init();

    return am_shtc1_init (&__g_shtc1_dev, i2c_handle);   
}

/**
 * @}
 */

/* end of file */
