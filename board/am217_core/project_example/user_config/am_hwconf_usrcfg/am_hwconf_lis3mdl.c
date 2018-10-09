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
 * \brief LIS3MDL 配置文件
 * \sa am_hwconf_lis3mdl.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_lis3mdl.h"
#include "am_zlg217_inst_init.h"
#include "zlg217_pin.h"

/**
 * \addtogroup am_if_src_hwconf_lis3mdl
 * \copydoc am_hwconf_lis3mdl.c
 * @{
 */

/** \brief 设备信息 */
am_local am_const am_lis3mdl_devinfo_t __g_lis3mdl_devinfo = {
    TEMP_ENABLE,
    HIGH_MODE_XY,
    ODR_10Hz,
    FAST_ODR_DISABLE,
    SELF_TEST_ENABLE,
    FULL_4GAUSS,
    CONTINUE_MODE,
    HIGH_MODE_Z
};

/*******************************************************************************
  HTS221 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_lis3mdl_dev_t __g_lis3mdl_dev;

/** \brief 实例初始化 */
am_lis3mdl_handle_t am_lis3mdl_inst_init (void)
{
    /* 获取 I2C 实例句柄 */
    am_i2c_handle_t i2c_handle = am_zlg217_i2c1_inst_init();

    return am_lis3mdl_init (&__g_lis3mdl_dev, 
                            &__g_lis3mdl_devinfo,
                            i2c_handle);   
}

/**
 * @}
 */

/* end of file */
