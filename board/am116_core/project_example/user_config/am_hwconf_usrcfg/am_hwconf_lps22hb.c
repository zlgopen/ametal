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
 * \brief LPS22HB 配置文件
 * \sa am_hwconf_lps22hb.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_lps22hb.h"
#include "am_zlg116_inst_init.h"
#include "zlg116_pin.h"

/**
 * \addtogroup am_if_src_hwconf_lps22hb
 * \copydoc am_hwconf_lps22hb.c
 * @{
 */

/** \brief 设备信息 */
am_local am_const am_lps22hb_devinfo_t __g_lps22hb_devinfo = {
    DATA_UPDATE_50Hz,
    ENABLE_LPFP,
    LPFP_BW_ODR20
};

/*******************************************************************************
  LPS22HB 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_lps22hb_dev_t __g_lps22hb_dev;

/** \brief 实例初始化 */
am_lps22hb_handle_t am_lps22hb_inst_init (void)
{
    /* 获取 I2C 实例句柄 */
    am_i2c_handle_t i2c_handle = am_zlg116_i2c1_inst_init();

    return am_lsp22hb_init (&__g_lps22hb_dev, 
                            &__g_lps22hb_devinfo,
                            i2c_handle);   
}

/**
 * @}
 */

/* end of file */
