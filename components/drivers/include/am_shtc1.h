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
 * \brief SHTC1温湿度传感器驱动
 *
 * \internal
 * \par modification history:
 * - 1.00 18-09-12  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_SHTC1_H
#define __AM_SHTC1_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_i2c.h"
 
/**
 * \addtogroup am_if_shtc1
 * \copydoc am_shtc1.h
 * @{
 */
    
/**
 * \brief SHTC1 设备结构体
 */
typedef struct am_shtc1_dev {
    
    /**< \brief SHTC1 I2C设备 */
    am_i2c_device_t i2c_dev; 
  
} am_shtc1_dev_t;

/** \brief SHTC1操作句柄定义 */
typedef am_shtc1_dev_t *am_shtc1_handle_t;

/**
 * \brief SHTC1传感器初始化
 *
 * \param[in] p_dev      : 指向SHTC1设备结构体的指针
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return SHTC1服务操作句柄,如果为 NULL，表明初始化失败
 */
am_shtc1_handle_t am_shtc1_init (am_shtc1_dev_t *p_dev, 
                                 am_i2c_handle_t i2c_handle);

/**
 * \brief SHTC1传感器读取温湿度
 *
 * \param[in]  handle : SHTC1服务操作句柄
 * \param[out] p_temp : 指向温度数据的指针
 * \param[out] p_hum  : 指向湿度数据的指针
 *
 * \retval AM_OK : 读取成功
 * \retval OTHER : 读取失败
 */                                 
int am_shtc1_read_temp_hum (am_shtc1_handle_t handle, 
                            int8_t *p_temp, 
                            uint8_t *p_hum); 

/**
 * @}
 */     

#ifdef __cplusplus
}
#endif

#endif /* __SHTC_1_H */

/* end of file */
