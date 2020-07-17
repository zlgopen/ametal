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
 * \brief 安全芯片FMSE-A03 I2C驱动头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-07-02  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_FMSE_I2C_H
#define __AM_FMSE_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_fmse_a03.h"

typedef struct{
    uint8_t lenlo;
    uint8_t lenhi;
    uint8_t nad;
    union{
        uint8_t cmd;
        uint8_t sta;
    } flag;
} am_fmse_i2c_head_t;
    
/**
 * \brief FMSE-A03 设备信息结构体
 */
typedef struct am_fmse_i2c_devinfo {

    /**
     * \brief I2C 7位 设备地址
     */
    uint8_t i2c_addr;

} am_fmse_i2c_devinfo_t;

/**
 * \breif FMSE-A03 设备结构体定义
 */
typedef struct am_fmse_i2c_dev {

    am_fmse_serv_t                    fmse_dev;  /**< \breif FMSE标准服务 */

    am_i2c_device_t                   i2c_dev;   /**< \brief i2c设备实例 */

    const am_fmse_i2c_devinfo_t      *dev_info;  /** \brief 设备信息 */

} am_fmse_i2c_dev_t;

/**
 * \brief I2C驱动FMSE的设备初始化
 *
 * \param[in] p_dev     : I2C驱动的 FMSE设备
 * \param[in] p_devinfo : FMSE设备信息
 * \param[in] handle    : I2C标准服务句柄
 *
 * \return FMSE标准服务句柄，若为NULL，表明初始化失败
 */
am_fmse_handle_t am_fmse_i2c_init (am_fmse_i2c_dev_t           *p_dev,
                                   const am_fmse_i2c_devinfo_t *p_devinfo,
                                   am_i2c_handle_t              handle);

/**
 * \brief FMSE的设备去初始化
 *
 * \param[in] handle : FMSE初始化获取的标准服务句柄
 *
 * \return 
 */
am_err_t am_fmse_i2c_deinit (am_fmse_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_FMSE_I2C_H */

/* end of file */
