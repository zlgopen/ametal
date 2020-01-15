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
 * \brief MM32L373 kboot KinetisFlashTool 用户配置文件
 *
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-13  yrh, first implementation
 * \endinternal
 */

/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 *
 * \param 无
 *
 * \return bootloader flash标准服务句柄，若为 NULL，表明初始化失败
 */

#include "am_boot_firmware.h"
#include "am_boot_flash.h"
#include "am_boot_autobaud.h"
/**
 * \brief bootloader kboot KinetisFlashTool 实例初始化
 *
 * \return 若为 AM_OK，表明初始化成功
 */
int am_mm32l373_boot_kft_inst_init( am_boot_flash_handle_t flash_handle);

/** \brief bootloader标准接口实例初始化，获得自动波特率服务句柄 */
int am_mm32l373_std_boot_inst_init(am_boot_flash_handle_t flash_handle);

/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 *
 * \param 无
 *
 * \return bootloader flash标准服务句柄，若为 NULL，表明初始化失败
 */
am_boot_flash_handle_t am_mm32l373_boot_flash_inst_init(void);

/** \brief 实例初始化，获得自动波特率服务句柄 */
am_boot_autobaud_handle_t am_mm32l373_boot_autobaud_inst_init (void);

/** \brief 自动波特率实例解初始化 */
void am_mm32l373_boot_autobaud_inst_deinit (am_boot_autobaud_handle_t handle);
