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
 * \brief 富通公司的MB85系列MB85RSXX芯片驱动 
 * 
 * 已知兼容芯片： 
 *   - MB85RS64
 * 
 * \internal
 * \par Modification history
 * - 1.00 19-12-09  RZ, first implementation.
 * \endinternal
 */
 
#ifndef __AM_MB85RSXX_H
#define __AM_MB85RSXX_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "ametal.h"
#include "am_types.h"
#include "am_gpio.h"
#include "am_spi.h"
#include "am_nvram.h"

/**
 * \addtogroup am_if_mb85rsxx
 * \copydoc am_mb85rsxx.h
 * @{
 */


/**
 * \brief MB85RSXX 实例信息
 */
typedef struct am_mb85rsxx_devinfo {
 
    uint16_t          spi_mode;      /**< \brief 器件使用的SPI模式 */
    int               spi_cs_pin;    /**< \brief SPI片选引脚       */
    uint32_t          spi_speed;     /**< \brief 使用的SPI速率     */
} am_mb85rsxx_devinfo_t;
     
/**
 * \brief MB85RSXX 实例
 */
typedef struct am_mb85rsxx_dev {

    /**< \brief SPI设备              */
    am_spi_device_t              spi_dev;

    /**< \brief mb85rsxx设备提供的 NVRAM 标准服务 */
    am_nvram_dev_t               *p_serv;

    /**< \brief 实例信息             */
    const am_mb85rsxx_devinfo_t  *p_devinfo;

} am_mb85rsxx_dev_t;

/** \brief 定义 MB85RSXX 的实例句柄类型 */
typedef struct am_mb85rsxx_dev *am_mb85rsxx_handle_t;


/** 
 * \brief 初始化 MB85RSXX，获取操作 MB85RSXX 的 handle
 * 
 * \param[in] p_dev      : 指向 MB85RSXX 实例的指针
 * \param[in] p_devinfo  : 指向 MB85RSXX 实例信息的指针
 * \param[in] handle     : SPI标准服务handle,使用该 handle 与 MB85RSXX 进行通信
 *
 * \return MB85RSXX 操作句柄，如果初始化失败，则返回值为NULL
 */
am_mb85rsxx_handle_t am_mb85rsxx_init(am_mb85rsxx_dev_t            *p_dev,
                                      const am_mb85rsxx_devinfo_t  *p_devinfo,
                                      am_spi_handle_t               spi_handle);

/**
 * \brief 读取数据
 *
 * \param[in] handle : MB85RSXX 操作句柄
 * \param[in] addr   : 读取数据的首地址
 * \param[in] p_buf  : 读取数据存放的缓冲区
 * \param[in] len    : 数据读取的长度
 *
 * \retval  AM_OK     : 读取数据成功
 * \retval -AM_EINVAL : 读取数据失败, 参数错误
 * \retval -AM_EIO    : 读取数据失败, SPI通信出错
 */
int am_mb85rsxx_read(am_mb85rsxx_handle_t  handle,
                     uint32_t              addr,
                     uint8_t              *p_buf,
                     uint32_t              len);


/** 
 * \brief 写入数据
 *
 * \param[in] handle : MB85RSXX 操作句柄
 * \param[in] addr   : 写入数据的首地址
 * \param[in] p_buf  : 写入数据存放的缓冲区
 * \param[in] len    : 数据读取的长度
 *
 * \retval  AM_OK     : 读取数据成功
 * \retval -AM_EINVAL : 读取数据失败, 参数错误
 * \retval -AM_EIO    : 读取数据失败, SPI通信出错
 */
int am_mb85rsxx_write(am_mb85rsxx_handle_t  handle,
                      uint32_t              addr,
                      uint8_t              *p_buf,
                      uint32_t              len);

                            
/** 
 * \brief 读取MB85RSXX状态寄存器的值
 * 
 * \param[in]  handle : MB85RSXX 操作句柄
 * \param[out] p_stat : 获取到的状态寄存器的值
 *
 * \retval  AM_OK     : 读取状态寄存器成功
 * \retval -AM_EINVAL : 读取失败, 参数错误
 * \retval -AM_EIO    : 读取失败, SPI通信出错
 */
int am_mb85rsxx_status_read(am_mb85rsxx_handle_t  handle,
                            uint8_t              *p_stat);

/** 
 * \brief 设置MB85RSXX状态寄存器的值
 * 
 * \param[in] handle : MB85RSXX 操作句柄
 * \param[in] val    : 写入状态寄存器的值
 *
 * \retval  AM_OK     : 写入状态寄存器成功
 * \retval -AM_EINVAL : 写入失败, 参数错误
 * \retval -AM_EIO    : 写入失败, SPI通信出错
 *
 */
int am_mb85rsxx_status_write(am_mb85rsxx_handle_t  handle,
                             uint8_t               val);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
 
#endif /* __AM_MB85RSXX_H */

/* end of file */
