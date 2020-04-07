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
 * \brief ZSN700_READER 配置文件
 * \sa am_hwconf_zsn700_reader.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-03-25  wk, first implementation.
 * \endinternal
 */
#include "zsn700_pin.h"
#include "ametal.h"
#include "am_zsn700_inst_init.h"
#include "am_zsn700_reader.h"
/**
 * \addtogroup am_if_src_hwconf_zsn700_reader
 * \copydoc am_hwconf_zsn700_reader.c
 * @{
 */

/* 定义 zsn700_reader 实例 */
static am_zsn700_reader_dev_t  __g_zsn700_reader_dev;

/* 配置FM175xx LPCD模式参数 */
am_local am_const am_zsn700_reader_lpcd_cfginfo_t  __g_lpcd_cfg_info = {
    AM_ZSN700_READER_LCPD_SLEEP500MS,
    AM_ZSN700_READER_LCPD_DETECT47US,
    AM_ZSN700_READER_LCPD_TX2_RF_ENABLE,
    AM_ZSN700_READER_LCPD_RF_NMOS_23,
    AM_ZSN700_READER_LCPD_RF_PMOS_1,
    0.07,
    AM_ZSN700_READER_LCPD_AUTO_DETECT_TIMES_1,
    AM_ZSN700_READER_LCPD_INT_ENABLE,
    AM_ZSN700_READER_LCPD_AUTO_WUP_ENABLE,
    AM_ZSN700_READER_LCPD_AUTO_WUP_TIME_432MIN,
};

/* 定义 zsn700_reader 实例信息 */
am_local am_const am_zsn700_reader_devinfo_t __g_zsn700_reader_devinfo = {
    AM_ZSN700_READER_PROT_TYPE_ISO14443A_106,
    NULL
};


am_zsn700_reader_handle_t am_zsn700_reader_inst_init (void)
{
      am_zsn700_reader_init(&__g_zsn700_reader_dev,
                            &__g_zsn700_reader_devinfo);

      return &__g_zsn700_reader_dev;
}

/**
 * @}
 */

/* end of file */
