/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief SPI驱动，DMA传输模式
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-07  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC82X_SPI_DMA_H
#define __AM_LPC82X_SPI_DMA_H

#include "am_types.h"
#include "am_spi.h"
#include "am_int.h"
#include "am_lpc82x_dma.h"
#include "hw/amhw_lpc82x_dma.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_lpc82x_if_spi_dma
 * \copydoc am_lpc82x_spi_dma.h
 * @{
 */

/**
 * \brief SPI 设备信息结构体
 */
typedef struct am_lpc82x_spi_dma_devinfo {
    uint32_t          spi_regbase;       /**< \brief SPI寄存器块的基地址 */
    uint32_t          clk_id;            /**< \brief SPI clk id */
    uint32_t          dma_chan_tx;       /**< \brief DMA发送通道号 */
    uint32_t          dma_chan_rx;       /**< \brief DMA接收通道号 */
    uint32_t          dma_chan_prio_tx;  /**< \brief DMA发送通道优先级 */
    uint32_t          dma_chan_prio_rx;  /**< \brief DMA接收通道优先级 */

    /** \brief SPI平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief SPI平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_lpc82x_spi_dma_devinfo_t;

/**
 * \brief SPI 设备，必须16字节对齐
 */
typedef struct am_lpc82x_spi_dma_dev {
    am_spi_serv_t                      spi_serve;   /**< \brief SPI标准服务句柄 */
    const am_lpc82x_spi_dma_devinfo_t *p_devinfo;   /**< \brief SPI设备信息的指针 */

    struct am_list_head        msg_list;        /**< \brief SPI控制器消息队列 */

    /** \brief 指向SPI消息结构体的指针,同一时间只能处理一个消息 */
    am_spi_message_t          *p_cur_msg;

    /** \brief 指向SPI传输结构体的指针,同一时间只能处理一个传输 */
    am_spi_transfer_t         *p_cur_trans;

    am_spi_device_t           *p_cur_spi_dev;   /**< \brief 当前传输的SPI设备 */
    am_spi_device_t           *p_tgl_dev;       /**< \brief 当前自锁的SPI设备 */

    am_bool_t                  busy;            /**< \brief SPI忙标识 */
    am_bool_t                  eot;             /**< \brief 结束字节 */
    uint32_t                   state;           /**< \brief SPI控制器状态机状态 */

    uint16_t                   rx_trash;        /**< \brief 丢弃数据的临时接收变量 */
    uint32_t                   p_tx_buf_eot;    /**< \brief 一帧中的最后一个数据 */

    /** \brief SPI选用DMA传输模式下,实际传输的数据个数 */
    uint32_t                   dma_trans_num;

    /** \brief SPI选用DMA传输计数，用于地址偏移 */
    uint32_t                   dma_trans_count;

    /** \brief SPI选用DMA传输模式标志(用于标志单次传输是否大于1024个长度) */
    uint32_t                   dma_trans_flg;

    __attribute__((aligned(16)))
    am_lpc82x_dma_xfer_desc_t   g_desc[3];  /**< \brief DMA通道描述符 */

    am_lpc82x_dma_controller_t *p_ctr_tx;   /**< \brief DMA发送通道控制器 */
    am_lpc82x_dma_controller_t *p_ctr_rx;   /**< \brief DMA接收通道控制器 */

} am_lpc82x_spi_dma_dev_t;

/**
 * \brief SPI 初始化
 *
 * 使用前，务必确保DMA实例已经初始化
 *
 * \param[in] p_dev     : 指向SPI设备结构体的指针
 * \param[in] p_devinfo : 指向SPI设备信息结构体的指针
 *
 * \return SPI标准服务操作句柄
 */
am_spi_handle_t am_lpc82x_spi_dma_init (am_lpc82x_spi_dma_dev_t           *p_dev,
                                        const am_lpc82x_spi_dma_devinfo_t *p_devinfo);

/**
 * \brief 解除SPI初始化
 * \param[in] handle : SPI标准服务操作句柄
 * \return 无
 */
void am_lpc82x_spi_dma_deinit (am_spi_handle_t handle);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC82X_SPI_DMA_H */

/*end of file */
