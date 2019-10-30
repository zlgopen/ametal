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
 * \brief ZLG 三线接口 ILI9341 驱动
 *
 * \note 该驱动是通过寄存器直接操作硬件SPI(片选也采用硬件片选)，速度快
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-27  vir, first implementation
 * \endinternal
 */

#ifndef __AM_ZLG_ILI9341_INTFC_3WIRE_H
#define __AM_ZLG_ILI9341_INTFC_3WIRE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_ili9341.h"
#include "hw/amhw_zlg_spi.h"

/**
 * @addtogroup am_zlg_if_ili9341_3wire_spi
 * @copydoc am_zlg_ili9341_3wire_spi.h
 * @{
 */

/**
 * \brief ili9341三线接口信息
 */
typedef struct am_zlg_ili9341_intfc_3wire_devinfo {

    /** \brief ILI9341的SPI的片选引脚 CS所连接到的GPIO引脚编号 */
    int                cs_pin;

    /** \brief ILI9341的SPI的sck 所连接到的GPIO引脚编号 */
    int                sck_pin;

    /** \brief lcd_sck 的引脚功能 */
    uint32_t           sck_func;

    /** \brief ILI9341的SPI的MOSI所连接到的GPIO引脚编号 */
    int                mosi_pin;

    /** \brief lcd_mosi 的引脚功能 */
    uint32_t           mosi_func;

    /**  \brief ILI9341的SPI的MISO所连接到的GPIO引脚编号 */
    int                miso_pin;

    /** \brief lcd_mosi 的引脚功能 */
    uint32_t           miso_func;

    /** \brief lcd_mosi 的引脚功能 */
    uint32_t           cs_func;

    /** \brief SPI寄存器块的基地址 */
    amhw_zlg_spi_t    *spi_reg_base;

    /** \brief 时钟ID */
    uint32_t           clk_spi;

    /** \brief SPI 速度，最大 24Mhz */
    uint32_t           speed_hz;

}am_zlg_ili9341_intfc_3wire_devinfo_t;

/**
 * \brief ili9341三线接口设备
 */
typedef struct am_zlg_ili9341_intfc_3wire_dev {

    /** \brief 三线接口驱动服务 */
    am_ili9341_serv_t                 serv;

    /** \brief 三线接口的接口信息 */
    const am_zlg_ili9341_intfc_3wire_devinfo_t *p_devinfo;

}am_zlg_ili9341_intfc_3wire_dev_t;

/**
 * \brief ili9341 三线接口初始化函数
 *
 * \param[in] p_dev     指向ILI9341三线接口设备结构体的指针
 * \param[in] p_devinfo 指向ILI9341三线接口信息的指针
 *
 * \return 三线接口服务指针
 */
am_ili9341_serv_t *am_zlg_ili9341_intfc_3wire_init(
                         am_zlg_ili9341_intfc_3wire_dev_t           *p_dev,
                         const am_zlg_ili9341_intfc_3wire_devinfo_t *p_devinfo);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _AM_ZLG_ILI9341_INTFC_3WIRE_H */

/* end of file */
