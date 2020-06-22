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
 * \brief QSPI驱动，服务QSPI标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-05-28
 * \endinternal
 */

#ifndef __AM_HC32F460_QSPI_H
#define __AM_HC32F460_QSPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hw/amhw_hc32f460_qspi.h"

/**
 * \brief hc32f460 qspi 设备信息
 */
typedef struct am_hc32f460_qspi_devinfo {

    /** \brief 寄存器基地址 */
    uint32_t          regbase;

    /** \brief 输入时钟ID */
    uint32_t          clk_id;

    /** \brief 中断号 */
    int               inum;

    uint8_t           clk_div;       /**< \brief 基准时钟设置 */
    uint8_t           protocol_data; /**< \brief 数据接收阶段SPI协议 */
    uint8_t           protocol_addr; /**< \brief 地址发送阶段SPI协议 */
    uint8_t           protocol_cmd;  /**< \brief 指令发送阶段SPI协议 */
    uint8_t           spi_mode;      /**< \brief spi模式 */
    uint8_t           read_mode;     /**< \brief 读取模式 */

    /** \brief 平台初始化函数 */
    void (*pfn_plfm_init) (void);
    /** \brief 平台解初始化函数 */
    void (*pfn_plfm_deinit) (void);
} am_hc32f460_qspi_devinfo_t;

/**
 *
 * \brief hc32f460 spifi 设备实例
 */
typedef struct am_hc32f460_qspi_dev {

    /**< \brief 指向设备信息常量的指针 */
    const am_hc32f460_qspi_devinfo_t *p_devinfo;
}am_hc32f460_qspi_dev_t;

/** \brief qspi标准服务操作句柄类型定义 */
typedef am_hc32f460_qspi_dev_t *am_qspi_handle_t;


/**
 * \brief QSPI 读数据
 *
 * \param[in] : p_dev  指向QSPI设备的指针
 * \param[in] : addr   读取的地址
 * \param[in] : p_buf  用于存放读取出的数据的buffer
 * \param[in] : nbytes 读取的长度
 *
 * \retval : AM_OK     读取成功
 *           AM_EINVAL 参数无效
 */
int am_hc32f460_qspi_read (am_hc32f460_qspi_dev_t *p_dev,
                           uint32_t                addr,
                           uint8_t                *p_buf,
                           uint32_t                nbytes);


/**
 * \brief QSPI 进入直接通信模式
 *
 * \param[in] : p_dev  指向QSPI设备的指针
 *
 * \retval : AM_OK     操作成功
 *           AM_EINVAL 参数无效
 */
int am_hc32f460_qspi_enter_directcom (am_hc32f460_qspi_dev_t *p_dev);


/**
 * \brief QSPI 退出直接通信模式
 *
 * \param[in] : p_dev  指向QSPI设备的指针
 *
 * \retval : AM_OK     操作成功
 *           AM_EINVAL 参数无效
 */
int am_hc32f460_qspi_exit_directcom (am_hc32f460_qspi_dev_t *p_dev);


/**
 * \brief QSPI 直接通信模式读取数据
 *
 * \param[in] : p_dev  指向QSPI设备的指针
 *
 * \retval :    读取出的数据
 */
uint8_t am_hc32f460_qspi_directcom_read (am_hc32f460_qspi_dev_t *p_dev);


/**
 * \brief QSPI 直接通信模式写数据
 *
 * \param[in] : p_dev  指向QSPI设备的指针
 * \param[in] : data   要写入的数据
 *
 * \retval : AM_OK     操作成功
 *           AM_EINVAL 参数无效
 */
int am_hc32f460_qspi_directcom_write (am_hc32f460_qspi_dev_t *p_dev,
                                      uint8_t                 data);


/**
 * \brief QSPI 设备初始化
 *
 * \param[in] : p_dev     指向QSPI设备的指针
 * \param[in] : p_devinfo 设备信息初始化实例
 *
 * \retval : qspi标准服务操作句柄
 */
am_qspi_handle_t am_hc32f460_qspi_init (am_hc32f460_qspi_dev_t            *p_dev,
                                        const am_hc32f460_qspi_devinfo_t  *p_devinfo);

/**
 * \brief QSPI 设备解初始化
 *
 * \param[in] : p_dev     qspi标准服务操作句柄
 *
 * \retval : 无
 */
void am_hc32f460_qspi_deinit (am_qspi_handle_t p_dev);



/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_QSPI_H */

/* end of file */
