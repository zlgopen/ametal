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
 * \brief bootloader 固件存储flash驱动，服务固件存储标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-21  yrh, first implementation
 * \endinternal
 */

#ifndef __AM_MM32L073_BOOT_FIRMWARE_H
#define __AM_MM32L073_BOOT_FIRMWARE_H

#include "am_boot_flash.h"
#include "am_boot_firmware.h"
typedef struct am_mm32l073_boot_firmware_flash_devinfo {
    uint32_t firmware_dst_addr;    /**< \brief 固件在flash存放的目的地址 */
    uint32_t buf_size;

    void (*pfn_plfm_init)(void);   /**< \brief 平台初始化函数 */

    void (*pfn_plfm_deinit)(void); /**< \brief 平台去初始化函数 */
}am_mm32l073_boot_firmware_flash_devinfo_t;

typedef struct am_mm32l073_boot_firmware_flash_dev {
    /**< \brief 固件存放的标准服务 */
    am_boot_firmware_serv_t           firmware_flash_serv;
    /**< \brief flash的操作句柄*/
    am_boot_flash_handle_t            flash_handle;
    /**< \brief 设备信息 */
    am_mm32l073_boot_firmware_flash_devinfo_t *p_devinfo;
    /**< \brief 数据缓冲区 */
    uint8_t                           buf_data[1024];
    /**< \brief 缓冲区的大小 */
    uint32_t                          buf_data_size;
    /**< \brief 当前缓冲区中的数据大小 */
    uint32_t                          curr_buf_data_size;
    /**< \brief 最近被擦除扇区的起始地址 */
    uint32_t                          erase_sector_start_addr;
    /**< \brief 当前写flash的地址 */
    uint32_t                          curr_program_flash_addr;
    /**< \brief 固件存放的目地地址 */
    uint32_t                          firmware_dst_addr;
    /**< \brief 固件总的大小 */
    uint32_t                          firmware_total_size;
    am_bool_t                         firmware_size_is_unknow;
}am_mm32l073_boot_firmware_flash_dev_t;

/**
 * \brief 初始化固件存放函数，返回标准服务操作句柄
 *
 * \param[in] p_dev     : 指向固件存放设备的指针
 * \param[in] p_devinfo : 指向固件存放设备信息常量的指针
 *
 * \return 固件存放标准服务操作句柄，值为NULL时表明初始化失败
 */
am_boot_firmware_handle_t am_mm32l073_boot_firmware_flash_init (
    am_mm32l073_boot_firmware_flash_dev_t     *p_dev,
    am_mm32l073_boot_firmware_flash_devinfo_t *p_devinfo,
    am_boot_flash_handle_t                   flash_handle);


/**
 * \brief 固件flash存储解初始化
 */
void am_mm32l073_boot_firmware_flash_deint(void);

#endif /* __AM_MM32L073_BOOT_FIRMWARE_H */

/* end of file */
