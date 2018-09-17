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
 * \brief BMA253 配置文件
 * \sa am_hwconf_bma253.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-10  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_bma253.h"
#include "am_zlg217_inst_init.h"
#include "zlg217_pin.h"

/**
 * \addtogroup am_if_src_hwconf_bma253
 * \copydoc am_hwconf_bma253.c
 * @{
 */

/** \brief 设备信息 */
am_local am_const am_bma253_devinfo_t __g_bma253_devinfo = {
    PMU_RANGE_2G,                         /**< \brief 加速度范围±2g          */
    PMU_BW_8Hz,                           /**< \brief 数据滤波带宽7.81Hz      */
    NORMAL_MODE,                          /**< \brief 正常模式                */
    SLEEP_DUR_HALF_MS                     /**< \brief 配置的睡眠周期为0.5ms   */
};

/*******************************************************************************
  BMA253 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_bma253_dev_t __g_bma253_dev;

/** \brief 实例初始化 */
am_bma253_handle_t am_bma253_inst_init (void)
{
    /* 获取 I2C 实例句柄 */
    am_i2c_handle_t i2c_handle =  am_zlg217_i2c1_inst_init();

    return am_bma253_init (&__g_bma253_dev, 
                           &__g_bma253_devinfo,
                           i2c_handle);   
}

/**
 * @}
 */

/* end of file */
