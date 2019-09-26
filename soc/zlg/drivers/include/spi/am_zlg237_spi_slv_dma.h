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
 * \brief SPI 从机驱动，服务 SPI 从机标准接口
 *
 * \internal
 * \par Modification History
 * -1.00 19-07-18 fra, first implementation
 * \endinternal
 */

#ifndef __AM_ZLG237_SPI_SLV_DMA_H
#define __AM_ZLG237_SPI_SLV_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_spi_slv.h"
#include "am_softimer.h"
#include "am_zlg_dma.h"
#include "hw/amhw_zlg237_spi.h"

/**
 * \addtogroup am_zlg237_if_spi_slv_dma
 * \copydoc am_zlg_spi_slv_dma.h
 * @{
 */

/**
 * \brief SPI 从机设备信息结构体
 */
typedef struct am_zlg237_spi_slv_dma_devinfo {
    uint32_t spi_reg_base;           /**< \brief 从机 SPI 寄存器块的基地址 */
    int      clk_id;                 /**< \brief 时钟 ID */
    uint32_t dma_chan_tx;            /**< \brief DMA 发送通道号 */
    uint32_t dma_chan_rx;            /**< \brief DMA 接收通道号 */
    int      cs_pin;                 /**< \breif 片选引脚 */
    void   (*pfn_plfm_init)(void);   /**< \brief SPI 平台初始化函数 */
    void   (*pfn_plfm_deinit)(void); /**< \brief SPI 平台解初始化函数 */
} am_zlg237_spi_slv_dma_devinfo_t;

/**
 * \brief SPI 设备结构体
 */
typedef struct am_zlg237_spi_slv_dma_dev {

    /** \brief SPI 从机标准服务句柄 */
    am_spi_slv_serv_t                      spi_slv_serve;

    /** \brief SPI 从机设备信息的指针 */
    const am_zlg237_spi_slv_dma_devinfo_t *p_devinfo;

    /** \brief 从机设备  */
    am_spi_slv_device_t                   *p_spi_slv_dev;

    /** \brief 传输体 */
    am_spi_slv_transfer_t                  tansfer;

    /** \brief 从片选有效到片选无效传输的总字节数 */
    size_t                                 sum_nbytes;

    /** \brief DMA 通道描述符 */
    amhw_zlg_dma_xfer_desc_t               g_desc[2];

    uint32_t                               dma_flags;

    /** \breif 用于舍弃接受的数据或发送空数据 */
    uint32_t                               dummy_dma_flags;

    uint32_t                               dummy_txbuf;
    uint32_t                               dummy_rxbuf;

} am_zlg237_spi_slv_dma_dev_t;

/**
 * \brief SPI 从机初始化
 *
 * \param[in] p_dev     指向 SPI 从设备结构体的指针
 * \param[in] p_devinfo 指向 SPI 从设备信息结构体的指针
 *
 * \return SPI 标准服务操作句柄
 */
am_spi_slv_handle_t
am_zlg237_spi_slv_dma_init (am_zlg237_spi_slv_dma_dev_t           *p_dev,
                            const am_zlg237_spi_slv_dma_devinfo_t *p_devinfo);

/**
 * \brief 解除 SPI 从机初始化
 *
 * \param[in] handle SPI 从设备标准服务操作句柄
 *
 * \return 无
 */
void am_zlg237_spi_slv_dma_deinit (am_spi_slv_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG_SPI_SLV_DMA_H */

/* end of file */
