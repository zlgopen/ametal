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
 * \brief MMC5883MA 配置文件
 * \sa am_hwconf_mmc5883ma.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  wk, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_mmc5883ma.h"
#include "am_zlg217_inst_init.h"
#include "zlg217_pin.h"

/**
 * \addtogroup am_if_src_hwconf_mmc5883ma
 * \copydoc am_hwconf_mmc5883ma.c
 * @{
 */
 
/** \brief 设备信息 */
am_local am_const am_mmc5883ma_devinfo_t __g_mmc5883ma_devinfo = {

          FREQUENCY_14,         /**< \brief 测量频率为14Hz   */
	        Start_Measurement
	
};
 

/*******************************************************************************
  MMC5883MA 实例初始化
*******************************************************************************/

/** \brief 设备实例定义 */
am_local am_mmc5883ma_dev_t __g_mmc5883ma_dev;

/** \brief 实例初始化 */
am_mmc5883ma_handle_t am_mmc5883ma_inst_init (void)
{
    /* 获取 I2C 实例句柄 */
    am_i2c_handle_t i2c_handle =  am_zlg217_i2c1_inst_init();

    return am_mmc5883ma_init (&__g_mmc5883ma_dev, 
                              &__g_mmc5883ma_devinfo,
                              i2c_handle);   
}

/**
 * @}
 */

/* end of file */
