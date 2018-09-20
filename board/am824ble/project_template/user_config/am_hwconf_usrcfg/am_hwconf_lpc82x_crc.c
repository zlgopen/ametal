/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X CRC 用户配置文件
 * \sa am_hwconf_lpc82x_crc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-08  sss, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc_crc.h"
#include "hw/amhw_lpc82x_clk.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_crc
 * \copydoc am_hwconf_lpc82x_crc.c
 * @{
 */

/**
 * \brief CRC 平台初始化
 */
am_local void __lpc82x_crc_plfm_init (void)
{
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_CRC);
}

/**
 * \brief CRC 平台解初始化
 */
am_local void __lpc82x_crc_plfm_deinit (void)
{
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_CRC);
}

/** \brief CRC 设备信息 */
am_local am_const am_lpc_crc_devinfo_t __g_lpc82x_crc_devinfo = {
    LPC82X_CRC_BASE,                /* CRC 寄存器块基址 */
    __lpc82x_crc_plfm_init,         /* 平台初始化函数 */
    __lpc82x_crc_plfm_deinit        /* 平台解初始化函数 */
};

/** \brief CRC 设备实例 */
am_local am_lpc_crc_dev_t __g_lpc82x_crc_dev;

/**
 * \brief CRC 实例初始化
 */
am_crc_handle_t am_lpc82x_crc_inst_init (void)
{
    return am_lpc_crc_init(&__g_lpc82x_crc_dev, &__g_lpc82x_crc_devinfo);
}

/**
 * \brief CRC 实例解初始化
 */
void am_lpc82x_crc_inst_deinit (void)
{
    am_lpc_crc_deinit();
}

/**
 * @}
 */

/* end of file */
