/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zsn700.cn/
*******************************************************************************/

/**
 * \file
 * \brief SPI驱动，服务SPI标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-12  YRZ, first implementation
 * \endinternal
 */

#ifndef __AM_ZSN700_SPI_POLL_H
#define __AM_ZSN700_SPI_POLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_zsn700_spi_poll.h"

#include "am_types.h"
#include "am_spi.h"
#include "am_int.h"

#include "hw/amhw_zsn700_spi.h"

/**
 * \addtogroup am_zsn700_if_spi_poll
 * \copydoc am_zsn700_spi_poll.h
 * @{
 */

/**
 * \name SPI传输模式
 * @{
 */

/** @} */

/**
 * \brief SPI设备信息结构体
 */
typedef struct am_zsn700_spi_poll_devinfo {
    uint32_t  spi_reg_base;   /**< \brief SPI寄存器块的基地址 */
    int       clk_id;         /**< \brief 时钟ID */

    /** \brief SPI平台初始化函数，如打开时钟，配置引脚等工作 */
    void    (*pfn_plfm_init)(void);

    /** \brief SPI平台解初始化函数 */
    void    (*pfn_plfm_deinit)(void);

} am_zsn700_spi_poll_devinfo_t;

/**
 * \brief SPI设备
 */
typedef struct am_zsn700_spi_poll_dev {

    am_spi_serv_t                   spi_serve;         /**< \brief SPI标准服务句柄 */
    const am_zsn700_spi_poll_devinfo_t *p_devinfo; /**< \brief SPI设备信息的指针 */

    /** \brief 指向SPI消息结构体的指针,同一时间只能处理一个消息 */
    am_spi_message_t          *p_cur_msg;

    /** \brief 指向SPI传输结构体的指针,同一时间只能处理一个传输 */
    am_spi_transfer_t         *p_cur_trans;

    am_spi_device_t           *p_cur_spi_dev;     /**< \brief 当前传输的SPI设备 */
    am_spi_device_t           *p_tgl_dev;         /**< \brief 当前自锁的SPI设备 */

    uint32_t                   nbytes_to_recv;    /**< \brief 待接收的字节数 */
    uint32_t                   data_ptr;          /**< \brief 数据传输计数 */

    am_bool_t                  lsbfirst;          /**< \brief 低位先发送标志 */
    am_bool_t                  busy;              /**< \brief SPI忙标识 */
    uint32_t                   state;             /**< \brief SPI控制器状态机状态 */

    uint8_t                    bef_bits_per_word; /**< \brief SPI上次传输字的字节数*/
    uint32_t                   bef_speed_hz;      /**< \brief SPI上次传输的速度*/
} am_zsn700_spi_poll_dev_t;

/**
 * \brief SPI初始化
 *
 * \param[in] p_dev     : 指向SPI设备结构体的指针
 * \param[in] p_devinfo : 指向SPI设备信息结构体的指针
 *
 * \return SPI标准服务操作句柄
 */
am_spi_handle_t am_zsn700_spi_poll_init (am_zsn700_spi_poll_dev_t              *p_dev,
                                        const am_zsn700_spi_poll_devinfo_t *p_devinfo);

/**
 * \brief 解除SPI初始化
 *
 * \param[in] handle : SPI标准服务操作句柄
 *
 * \return 无
 */
void am_zsn700_spi_poll_deinit (am_spi_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSN700_SPI_POLL_H */

/* end of file */
