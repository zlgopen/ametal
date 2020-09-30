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
 * \brief zsl42x lora SPI读写适配
 *
 * \internal
 * \par modification history:
 * - 1.00 19-04-16 zcb, first implementation.
 * - 1.01 20-09-23  yg, adaptation zsl42x.
 * \endinternal
 */

#ifndef __AM_ZSL42X_LORA_IF_SPI_H
#define __AM_ZSL42X_LORA_IF_SPI_H

/**
 * \addtogroup am_if_zsl42x_lora_if_spi
 * \copydoc am_zsl42x_lora_if_spi.h
 * @{
 */

#include "am_zsl42x_lora.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ZSL42X_LORA_SPI_CS_ENABLE      0   /**< \brief SPI片选有效电平 */
#define ZSL42X_LORA_SPI_CS_DISABLE     1   /**< \brief SPI片选无效电平 */

/**
 * \brief ZSL42X LORA SPI驱动服务信息
 */
typedef struct am_zsl42x_lora_if_spi_devinfo {

    /** \brief zsl42x lora 设备实例  */
    am_zsl42x_lora_devinfo_t   zsl42x_lora_devinfo;

    /** \brief SPI 外设基地址 */
    uint32_t                   spi_rebase;

    /** \brief SPI 平台初始化函数 */
    void                     (*pfn_plfm_init) (void);

    /** \brief SPI 平台解初始化函数 */
    void                     (*pfn_plfm_deinit)(void);
} am_zsl42x_lora_if_spi_devinfo_t;

/**
 * \brief ZSL42X LORA SPI 设备结构体
 */
typedef struct am_zsl42x_lora_if_spi_dev {

    /** \brief zsl42x lora 设备实例  */
    am_zsl42x_lora_dev_t                   zsl42x_lora_dev;

    /** \brief ZSL42X LORA SPI驱动服务信息 */
    const am_zsl42x_lora_if_spi_devinfo_t *p_spi_devinfo;

} am_zsl42x_lora_if_spi_dev_t;


/**
 * \brief ZSL42X LORA SPI适配初始化
 *
 * \param[in] p_dev     : zsl42x lora spi设备
 * \param[in] p_devinfo : 设备信息
 *
 * return : 初始化成功返回lora句柄，返回其他表示初始化错误
 */
am_zsl42x_lora_handle_t
am_zsl42x_lora_if_spi_init (am_zsl42x_lora_if_spi_dev_t       *p_dev,
                      const am_zsl42x_lora_if_spi_devinfo_t   *p_devinfo);

/**
 * \brief ZSL42X LORA SPI适配去初始化
 *
 * \param[in] handle : lora handle
 *
 * \retval AM_OK    : 去初始化成功
 * \retval AM_ERROR : 去初始化失败
 */
int am_zsl42x_lora_if_spi_deinit (am_zsl42x_lora_handle_t handle);

#ifdef __cplusplus
}
#endif

/** @} grp_am_if_zsl42x_lora_if_spi */

#endif /* } __AM_ZSL42X_LORA_IF_SPI_H */

/* end of file */
