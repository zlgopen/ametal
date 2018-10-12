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
 * \brief EEPROM 配置文件
 * \sa am_hwconf_ep24cxx.c
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-28  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_lpc82x_inst_init.h"
#include "am_ep24cxx.h"
#include "am_hwconf_ep24cxx.h"
#include "am_i2c.h"

/**
 * \addtogroup am_if_src_hwconf_ep24cxx
 * \copydoc am_hwconf_ep24cxx.c
 * @{
 */

/*******************************************************************************
    FM24C02 实例初始化（使用芯片功能）
*******************************************************************************/

/* EEPROM 器件信息定义 */
static const am_ep24cxx_devinfo_t __g_24c02_devinfo = {
    0x50,                                               /* 器件的I2C地址 */
    AM_EP24CXX_FM24C02,                                 /* 器件型号 */
};

/* FM24C02器件实例定义 */
static  am_ep24cxx_dev_t         __g_24c02_dev;


am_ep24cxx_handle_t am_fm24c02_inst_init (void)
{
    am_i2c_handle_t i2c_handle = am_lpc82x_i2c1_inst_init();
    return am_ep24cxx_init(&__g_24c02_dev,
                           &__g_24c02_devinfo,
                            i2c_handle);
}

/*******************************************************************************
    FM24C02 实例初始化（将FM24C02用作标准的NVRAM设备）
*******************************************************************************/

static am_nvram_dev_t  __g_24c02_nvram_dev;   /* NVRAM设备实例定义 */

int am_fm24c02_nvram_inst_init (void)         /* NVRAM初始化 */
{
    am_ep24cxx_handle_t fm24c02_handle = am_fm24c02_inst_init();
    am_ep24cxx_nvram_init(fm24c02_handle, &__g_24c02_nvram_dev, "fm24c02");
    return AM_OK;
}

/**
 * @}
 */

/* end of file */
