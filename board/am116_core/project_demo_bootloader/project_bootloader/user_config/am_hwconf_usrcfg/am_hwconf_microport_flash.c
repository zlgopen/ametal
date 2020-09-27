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
 * \brief MicroPort FLASH(MX25xx) 配置文件
 * \sa am_hwconf_microport_flash.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_mtd.h"
#include "am_mx25xx.h"
#include "am_zlg116_inst_init.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_microport_flash
 * \copydoc am_hwconf_microport_flash.c
 * @{
 */

/** \brief 设备信息 */
am_local am_const am_mx25xx_devinfo_t __g_microport_flash_devinfo = {
    AM_SPI_MODE_0,          /**< \brief 使用模式 0 */
    PIOA_4,                 /**< \brief 片选引脚 */
    30000000,               /**< \brief 总线速率 */
    AM_MX25XX_MX25L1606     /**< \brief 器件型号 */
};

/*******************************************************************************
  MicroPort FLASH(MX25xx) 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_mx25xx_dev_t __g_microport_flash_dev;

/** \brief 实例初始化 */
am_mx25xx_handle_t am_microport_flash_inst_init (void)
{

    /* 获取 SPI0 实例句柄 */
    am_spi_handle_t spi_handle = am_zlg116_spi1_dma_inst_init();

    return am_mx25xx_init(&__g_microport_flash_dev,
                          &__g_microport_flash_devinfo,
                           spi_handle);
}

/*******************************************************************************
  MTD 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_mtd_serv_t __g_microport_flash_mtd;

/** \brief 实例初始化 */
am_mtd_handle_t am_microport_flash_mtd_inst_init (void)
{
    am_mx25xx_handle_t microport_flash_handle = am_microport_flash_inst_init();

    return am_mx25xx_mtd_init(microport_flash_handle,
                             &__g_microport_flash_mtd,
                              0);
}

/*******************************************************************************
  FTL 实例初始化
*******************************************************************************/

#include "am_ftl.h"

#define  MX25XX_FTL_LOGIC_BLOCK_SZIE  256 /**< \brief 逻辑块大小（自定义）  */
#define  MX25XX_FTL_LOG_BLOCK_NUM     2   /**< \brief 日志块个数（2 ~ 10,自定义）*/

/*
 * 器件 AM_MX25XX_MX25L1606 对应的相关信息
 */
#define  MX25XX_CHIP_SZIE        (2 * 1024 * 1024) /**< \brief 总容量 */
#define  MX25XX_ERASE_UNIT_SZIE   4096             /**< \brief 最小擦除单元大小 */

/** \brief 设备定义 */
am_local am_ftl_serv_t __g_ftl;

/** \brief RAM 缓冲区定义 */
am_local uint8_t __g_ftl_buf[AM_FTL_RAM_SIZE_GET(MX25XX_CHIP_SZIE,
                                                 MX25XX_ERASE_UNIT_SZIE,
                                                 MX25XX_FTL_LOGIC_BLOCK_SZIE,
                                                 MX25XX_FTL_LOG_BLOCK_NUM)];

/** \brief FTL 信息 */
am_local am_const am_ftl_info_t __g_ftl_info = {
    __g_ftl_buf,                 /**< \brief RAM 缓冲区  */
    sizeof(__g_ftl_buf),         /**< \brief RAM 缓冲区大小  */
    MX25XX_FTL_LOGIC_BLOCK_SZIE, /**< \brief 逻辑块的大小  */
    MX25XX_FTL_LOG_BLOCK_NUM,    /**< \brief 日志块个数  */
    0                            /**< \brief 保留的物理块（擦除单元）个数 */
};

/** \brief 实例初始化 */
am_ftl_handle_t am_microport_flash_ftl_inst_init (void)
{
    return am_ftl_init(&__g_ftl,
                       &__g_ftl_info,
                       am_microport_flash_mtd_inst_init());
}

/**
 * @}
 */

/* end of file */
