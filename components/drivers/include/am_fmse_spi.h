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
 * \brief 安全芯片FMSE-A03 SPI驱动头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-07-02  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_FMSE_SPI_H
#define __AM_FMSE_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_spi.h"
#include "am_fmse_a03.h"

/**
 * \brief FMSE-A03 设备信息结构体
 */
typedef struct am_fmse_spi_devinfo {

    int       cs_pin;
    
    uint32_t  speed;

} am_fmse_spi_devinfo_t;

/**
 * \breif FMSE-A03 设备结构体定义
 */
typedef struct am_fmse_spi_dev {

    am_fmse_serv_t                    fmse_dev;  /**< \breif FMSE标准服务 */

    am_spi_device_t                   spi_dev;   /**< \brief spi设备实例 */

    const am_fmse_spi_devinfo_t      *dev_info;  /** \brief 设备信息 */

} am_fmse_spi_dev_t;

/**
 * \brief SPI驱动 FMSE的设备初始化
 *
 * \param[in] p_dev     : SPI驱动的 FMSE设备
 * \param[in] p_devinfo : FMSE设备信息
 * \param[in] handle    : SPI标准服务句柄
 *
 * \return FMSE标准服务句柄，若为NULL，表明初始化失败
 */
am_fmse_handle_t am_fmse_spi_init (am_fmse_spi_dev_t           *p_dev,
                                   const am_fmse_spi_devinfo_t *p_devinfo,
                                   am_spi_handle_t              handle);

/**
 * \brief FMSE的设备去初始化
 *
 * \param[in] handle : FMSE初始化获取的标准服务句柄
 *
 * \return 
 */
am_err_t am_fmse_spi_deinit (am_fmse_handle_t handle);
                                   
#ifdef __cplusplus
}
#endif

#endif /* __AM_FMSE_SPI_H */

/* end of file */
