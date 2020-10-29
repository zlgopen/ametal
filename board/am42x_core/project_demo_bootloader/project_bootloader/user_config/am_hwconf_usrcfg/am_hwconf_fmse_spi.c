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
 * \brief 安全芯片FMSE A03 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-03  yrz, first implementation.
 * \endinternal
 */

#include "zsl42x_pin.h"
#include "am_fmse_a03.h"
#include "am_fmse_spi.h"
#include "am_common.h"
#include "am_zsl42x_inst_init.h"

/** \brief 安全芯片FMSE A03 设备信息实例 */
am_const am_local am_fmse_spi_devinfo_t __g_fmse_spi_devinfo = {
    PIOB_12,
    2000000
};

/** \breif 安全芯片FMSE A03 设备结构体定义 */
am_local struct am_fmse_spi_dev __g_fmse_spi_dev;

/** \brief 安全芯片FMSE A03 设备实例化 */
am_fmse_handle_t am_fmse_spi_inst_init (void)
{
    return am_fmse_spi_init(&__g_fmse_spi_dev,
                            &__g_fmse_spi_devinfo,
                             am_zsl42x_spi1_poll_inst_init());
}

/** \brief 安全芯片FMSE A03 实例解初始化 */
am_err_t am_fmse_spi_inst_deinit (am_fmse_handle_t handle)
{
    return am_fmse_spi_deinit(handle);
}

/* end of file */
