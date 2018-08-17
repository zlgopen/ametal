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
 * \brief  三线接口 ILI9341 驱动 (采用的是标准层的SPI来驱动 ，速度慢)
 *
 * \ note  三线： MOSI 、 CS、 SCK
 *             MOSI 一次传输 9位的，最先发送的一位是 数据/命令的  选择 位 ，然后发送数据或命令，高位先传输
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-27  vir, first implementation.
 * \endinternal
 */


#ifndef __AM_ILI9341_INTFC_3WIRE_H
#define __AM_ILI9341_INTFC_3WIRE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_spi.h"
#include "am_ili9341.h"

/**
 * @addtogroup am_if_ili9341_intfc_3wire
 * @copydoc am_ili9341_intfc_3wire.h
 * @{
 */


/**
 * \brief ili9341三线接口信息
 */
typedef struct am_ili9341_intfc_3wire_devinfo {

    /** \brief ILI9341的SPI的片选引脚 CS所连接到的GPIO引脚编号 */
    int cs_pin;

    /** \brief ILI9341的SPI的clk 所连接到的GPIO引脚编号 */
    int sck_pin;

    /* lcd_sck 的引脚功能 */
    uint32_t sck_func;

    /** \brief ILI9341的SPI的MOSI所连接到的GPIO引脚编号 */
    int mosi_pin;

    /* lcd_mosi 的引脚功能 */
    uint32_t mosi_func;

    /** \brief ILI9341的SPI的MISO所连接到的GPIO引脚编号 */
    int miso_pin;

    /* lcd_miso 的引脚功能 */
    uint32_t miso_func;

    /**
     *  \brief SPI 速度
     *  \note 这是采用标准SPI接口，可达到50MHZ。
     */
    uint32_t max_speed_hz;

}am_ili9341_intfc_3wire_devinfo_t;


/**
 * \brief ili9341三线接口设备
 */
typedef struct am_ili9341_intfc_3wire_dev {

    /**< \brief 三线接口驱动服务 */
    am_ili9341_serv_t                 serv;

    /**< \brief SPI设备  */
    am_spi_device_t                   spi_dev;

    /**< \brief 三线接口的接口信息 */
    const am_ili9341_intfc_3wire_devinfo_t *p_devinfo;

}am_ili9341_intfc_3wire_dev_t;


/**
 * \brief ili9341 三线接口初始化函数
 * \param[in] p_dev          :指向ILI9341三线接口设备结构体的指针
 * \param[in] p_devinfo      :指向ILI9341三线接口信息的指针
 * \param[in] spi_handle     : SPI 句柄
 *
 * \return  三线接口服务指针
 */
am_ili9341_serv_t * am_ili9341_intfc_3wire_init(am_ili9341_intfc_3wire_dev_t *p_dev,
                             const am_ili9341_intfc_3wire_devinfo_t *p_devinfo,
                             am_spi_handle_t                         spi_handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _AM_ILI9341_INTFC_3WIRE_H */

/* end of file */

