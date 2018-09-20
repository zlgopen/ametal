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
 * \brief BH1730FVC 配置文件
 * \sa am_hwconf_bh1730fvc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_bh1730fvc.h"
#include "am_zlg217_inst_init.h"
#include "zlg217_pin.h"

/**
 * \addtogroup am_if_src_hwconf_bh1730fvc
 * \copydoc am_hwconf_bh1730fvc.c
 * @{
 */

/** \brief 设备信息 */
am_local am_const am_bh1730fvc_devinfo_t __g_bh1730fvc_devinfo = {
    MEAS_CONTINUE,
    TYPE0_ONLY,
    ADC_EN,
    POWER_ON,
    0xDA,
    ADC_MODE_X64
};

/*******************************************************************************
  HTS221 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_bh1730fvc_dev_t __g_bh1730fvc_dev;

/** \brief 实例初始化 */
am_bh1730fvc_handle_t am_bh1730fvc_inst_init (void)
{
    /* 获取 I2C 实例句柄 */
    am_i2c_handle_t i2c_handle = am_zlg217_i2c1_inst_init();

    return am_bh1730fvc_init (&__g_bh1730fvc_dev, 
                              &__g_bh1730fvc_devinfo,
                              i2c_handle);   
}

/**
 * @}
 */

/* end of file */
