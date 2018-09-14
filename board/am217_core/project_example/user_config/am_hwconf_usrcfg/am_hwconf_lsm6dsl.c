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
 * \brief LSM6DSL 配置文件
 * \sa am_hwconf_lsm6dsl.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  wk, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_lsm6dsl.h"
#include "am_zlg217_inst_init.h"
#include "zlg217_pin.h"

/**
 * \addtogroup am_if_src_hwconf_lsm6dsl
 * \copydoc am_hwconf_lsm6dsl.c
 * @{
 */
 
 
/** \brief 设备信息 */
am_local am_const am_lsm6dsl_devinfo_t __g_lsm6dsl_devinfo = {
  
	 CTRL1_XL,
	 STATUS_REG,
   FIFO_ODR_12point5       /**< \brief FIFO输出速率为12.5Hz   */

};
 
/*******************************************************************************
  LSM6DSL 实例初始化
*******************************************************************************/

/** \brief 设备实例定义 */
am_local am_lsm6dsl_dev_t __g_lsm6dsl_dev;

/** \brief 实例初始化 */
am_lsm6dsl_handle_t am_lsm6dsl_inst_init (void)
{
    /* 获取 I2C 实例句柄 */
    am_i2c_handle_t i2c_handle =  am_zlg217_i2c1_inst_init();

    return am_lsm6dsl_init (&__g_lsm6dsl_dev, 
                          &__g_lsm6dsl_devinfo,
                                     i2c_handle);   
}

/**
 * @}
 */

/* end of file */
