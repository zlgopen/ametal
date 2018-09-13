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
 * \brief HC595标准设备实现（SPI驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-23  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_HC595_SPI_H
#define __AM_HC595_SPI_H

#include "am_common.h"
#include "am_hc595.h"
#include "am_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup grp_am_if_hc595_spi
 * \copydoc am_hc595_spi.h
 * @{
 */

/**
 * \brief SPI驱动HC595的设备信息
 *
 * 未使用的引脚，如OE，固定为低电平时，可以将 oe引脚的值设置为 -1
 */
typedef struct am_hc595_spi_info {

    int         pin_lock;     /**< \brief 数据锁存引脚                        */
    int         pin_oe;       /**< \brief 输出使能引脚（未使用时可设置为 -1） */
    uint32_t    clk_speed;    /**< \brief SPI输出的时钟频率                   */

    /**
     * \brief HC595数据发送是否最低位先发送（默认高位先发送）
     *
     *  该值为 AM_TRUE ， 最低位先发送， 最低位将送出到 Q7位置
     *  该值为 AM_FALSE ， 最高位先发送， 最高位将送出到 Q7位置
     *
     *  例如：
     *  - 若Q0 与数码管 'a'相连接，则 数据 LSB 应该先发送
     *  - 若Q7 余 'a'相连接，则 数据 MSB 应该先发送
     */
    am_bool_t  lsb_first;

} am_hc595_spi_info_t;

/**
 * \brief SPI驱动HC595的设备
 */
typedef struct am_hc595_spi_dev {
    am_hc595_dev_t             isa;        /**< \brief 标准的HC595服务 */
    am_spi_device_t            spi_dev;    /**< \brief SPI 设备        */
    const am_hc595_spi_info_t *p_info;
} am_hc595_spi_dev_t;

/**
 * \brief SPI驱动HC595的设备初始化
 *
 * \param[in] p_dev     : GPIO驱动的 HC595设备
 * \param[in] handle    : SPI标准服务句柄
 * \param[in] pin_lock  : 锁存引脚将作为片选引脚
 * \param[in] clk_speed : 时钟线频率，如 3000000Hz
 * \param[in] lsb_first : 是否低位先发送
 *
 * \return hc595服务句柄，若为NULL，表明初始化失败
 */
am_hc595_handle_t am_hc595_spi_init (am_hc595_spi_dev_t        *p_dev,
                                     const am_hc595_spi_info_t *p_info,
                                     am_spi_handle_t            handle);

/**
 * \brief SPI驱动HC595的设备解初始化
 * \param[in] p_dev  : GPIO驱动的 HC595设备
 * \return AM_OK，解初始化成功；其它值，解初始化失败
 */
int am_hc595_spi_deinit (am_hc595_spi_dev_t *p_dev);

/** @} grp_am_if_hc595_spi */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC595_SPI_H */
