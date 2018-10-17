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
 * \brief BME280 配置文件
 * \sa am_hwconf_bme280.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_bme280.h"
#include "am_zlg116_inst_init.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_bme280
 * \copydoc am_hwconf_bme280.c
 * @{
 */

/** \brief 设备信息 */
am_local am_const am_bme280_devinfo_t __g_bme280_devinfo = {
    HUMIDITY_OVERESAMPLE_X16,
    PRESSURE_OVERESAMPLE_X16,
    TEMP_OVERESAMPLE_X16,
    NORMAL_MODE,
    STANDBY_TIME_HALF_MS
};

/*******************************************************************************
  BME280 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_bme280_dev_t __g_bme280_dev;

/** \brief 实例初始化 */
am_bme280_handle_t am_bme280_inst_init (void)
{
    /* 获取 I2C 实例句柄 */
    am_i2c_handle_t i2c_handle =  am_zlg116_i2c1_inst_init();

    return am_bme280_init (&__g_bme280_dev, 
                           &__g_bme280_devinfo,
                           i2c_handle);   
}

/**
 * @}
 */

/* end of file */
