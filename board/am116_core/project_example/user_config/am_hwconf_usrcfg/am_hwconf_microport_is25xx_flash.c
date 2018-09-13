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
 * \brief MicroPort FLASH(ISSI25xx) 配置文件
 * \sa am_hwconf_microport_is25xx_flash.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-03  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_mtd.h"
#include "am_is25xx.h"
#include "am_zlg116_inst_init.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_microport_is25xx_flash
 * \copydoc am_hwconf_microport_is25xx_flash.c
 * @{
 */

/** \brief 设备信息 */
am_local am_const am_is25xx_devinfo_t __g_is25xx_flash_devinfo = {
    AM_SPI_MODE_0,          /**< \brief 使用模式 0 */
    PIOA_4,                 /**< \brief 片选引脚 */
    50000000,               /**< \brief 总线速率 */
    AM_IS25XX_IS25WP064     /**< \brief 器件型号 */
};

/*******************************************************************************
  MicroPort FLASH(IS25xx) 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_is25xx_dev_t __g_is25xx_flash_dev;

/** \brief 实例初始化 */
am_is25xx_handle_t am_microport_flash_is25xx_inst_init (void)
{
    /* 获取 SPI0 实例句柄 */
    am_spi_handle_t spi_handle = am_zlg116_spi1_dma_inst_init();

    return am_is25xx_init(&__g_is25xx_flash_dev,
                          &__g_is25xx_flash_devinfo,
                           spi_handle);   
}

/**
 * @}
 */

/* end of file */
