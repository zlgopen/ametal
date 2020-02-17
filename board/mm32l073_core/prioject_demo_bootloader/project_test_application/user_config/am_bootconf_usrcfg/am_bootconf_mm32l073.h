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
 * \brief MM32L073 kboot KinetisFlashTool 用户配置文件
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

am_boot_flash_handle_t am_mm32l073_boot_flash_inst_init(void);

/**
 * \brief bootloader 标准实例初始化
 *
 *
 * \return 若为 AM_OK，表明初始化成功
 */
int am_mm32l073_std_boot_inst_init(am_boot_flash_handle_t flash_handle);


am_boot_firmware_handle_t am_mm32l073_boot_firmware_flash(am_boot_flash_handle_t flash_handle);



/*end of file*/
