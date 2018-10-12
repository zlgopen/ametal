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
 * \brief MicroPort-FLASH(MX25xx) 用户配置文件
 * \sa am_hwconf_microport_flash.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_mtd.h"
#include "am_ftl.h"
#include "am_mx25xx.h"
#include "am_lpc82x.h"
#include "am_lpc82x_inst_init.h"
#include "raw_posix.h"

/**
 * \addtogroup am_if_src_hwconf_microport_flash
 * \copydoc am_hwconf_microport_flash.c
 * @{
 */

/** \brief MicroPort-FLASH 设备信息 */
am_local am_const am_mx25xx_devinfo_t __g_microport_flash_devinfo = {
    AM_SPI_MODE_0,         /* 使用模式 0 */
    PIO0_1,                /* 片选引脚 */
    1000000,               /* 总线速率 */
    AM_MX25XX_MX25L1606    /* 器件型号 */
};

/*******************************************************************************
  MicroPort-FLASH(MX25xx) 实例初始化
*******************************************************************************/

/** \brief MicroPort-FLASH 设备实例 */
am_local am_mx25xx_dev_t __g_microport_flash_dev;

/**
 * \brief MicroPort-FLASH 实例初始化
 */
am_mx25xx_handle_t am_microport_flash_inst_init (void)
{
    am_spi_handle_t spi_handle = am_lpc82x_spi0_dma_inst_init();

    return am_mx25xx_init(&__g_microport_flash_dev,
                          &__g_microport_flash_devinfo,
                           spi_handle);
}

/*******************************************************************************
  MTD 实例初始化
*******************************************************************************/

/** \brief MicroPort-FLASH (MTD) 设备实例 */
am_local am_mtd_serv_t __g_microport_flash_mtd_serv;

/**
 * \brief MicroPort-FLASH 实例初始化（使用 MTD 标准接口）
 */
am_mtd_handle_t am_microport_flash_mtd_inst_init (void)
{
    am_mx25xx_handle_t handle = am_microport_flash_inst_init();

    return am_mx25xx_mtd_init(handle,
                             &__g_microport_flash_mtd_serv,
                              0);
}

/*******************************************************************************
  FTL 实例初始化
*******************************************************************************/

#define __FTL_LOGIC_BLOCK_SZIE    256          /**< \brief 逻辑块大小 */
#define __FTL_LOG_BLOCK_NUM       2            /**< \brief 日志块个数（2~10） */

#define __CHIP_SZIE          (2 * 1024 * 1024) /**< \brief 总容量 */
#define __ERASE_UNIT_SZIE    4096              /**< \brief 最小擦除单元大小 */

/** \brief MicroPort-FLASH (FTL) 设备实例 */
am_local am_ftl_serv_t __g_microport_flash_ftl_serv;

/**
 * \brief MicroPort-FLASH RAM 缓冲区定义
 */
am_local uint8_t __g_ftl_buf[AM_FTL_RAM_SIZE_GET(__CHIP_SZIE,
                                                 __ERASE_UNIT_SZIE,
                                                 __FTL_LOGIC_BLOCK_SZIE,
                                                 __FTL_LOG_BLOCK_NUM)];

/** \brief MicroPort-FLASH FTL 信息 */
am_local am_const am_ftl_info_t __g_ftl_info = {
    __g_ftl_buf,               /* RAM 缓冲区 */
    sizeof(__g_ftl_buf),       /* RAM 缓冲区大小 */
    __FTL_LOGIC_BLOCK_SZIE,    /* 逻辑块的大小 */
    __FTL_LOG_BLOCK_NUM,       /* 日志块个数 */
    0                          /* 保留的物理块（擦除单元）个数 */
};

/**
 * \brief MicroPort-FLASH 实例初始化（使用 FTL 标准接口）
 */
am_ftl_handle_t am_microport_flash_ftl_inst_init (void)
{
    return am_ftl_init(&__g_microport_flash_ftl_serv,
                       &__g_ftl_info,
                       am_microport_flash_mtd_inst_init());
}


/*******************************************************************************
  文件系统 实例初始化
*******************************************************************************/
am_local file_t  __g_system_file_info_  = {
	"hash_students",	// 文件名
	0,								// start_block
	2 * 1024,					// block_nr
	0,								// 光标位置
	1024							// 文件大小 
};

block_dev_ftl_t __g_block_block;

void am_raw_file_inst_init(void)
{
	block_dev_t* p_dev = block_dev_ftl_init(&__g_block_block, am_microport_flash_ftl_inst_init(), 0, 2 * 1024);
	raw_posix_init(p_dev,&__g_system_file_info_, 1);
}


/**
 * @}
 */

/* end of file */
