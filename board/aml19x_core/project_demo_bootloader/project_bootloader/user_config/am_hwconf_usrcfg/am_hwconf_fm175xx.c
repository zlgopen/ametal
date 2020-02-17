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
 * \brief FM175XX 配置文件
 * \sa am_hwconf_fm175xx.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-03-25  wk, first implementation.
 * \endinternal
 */
#include "hc32_pin.h"
#include "ametal.h"
#include "am_fm175xx.h"
#include "am_hc32l19x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_fm175xx
 * \copydoc am_hwconf_fm175xx.c
 * @{
 */

/* 定义 fm175xx 实例 */
static am_fm175xx_dev_t  __g_fm175xx_dev;

/* 配置FM175xx LPCD模式参数 */
am_local am_const am_fm175xx_lpcd_cfginfo_t  __g_lpcd_cfg_info = {
    AM_FM175XX_LCPD_SLEEP500MS,
    AM_FM175XX_LCPD_DETECT47US,
    AM_FM175XX_LCPD_TX2_RF_ENABLE,
    AM_FM175XX_LCPD_RF_NMOS_23,
    AM_FM175XX_LCPD_RF_PMOS_1,
    0.07,
    AM_FM175XX_LCPD_AUTO_DETECT_TIMES_1,
    AM_FM175XX_LCPD_INT_ENABLE,
    AM_FM175XX_LCPD_AUTO_WUP_ENABLE,
    AM_FM175XX_LCPD_AUTO_WUP_TIME_432MIN,
};

/* 定义 fm175xx 实例信息 */
am_local am_const am_fm175xx_devinfo_t __g_fm175xx_devinfo = {
    PIOB_6,
    PIOA_4,
    PIOB_5,
    AM_FM175XX_PROT_TYPE_ISO14443A_106,
    &__g_lpcd_cfg_info
};


am_fm175xx_handle_t am_fm175xx_inst_init (void)
{
      am_fm175xx_init(&__g_fm175xx_dev,
                       am_hc32_spi1_poll_inst_init(),
                      &__g_fm175xx_devinfo);

      return &__g_fm175xx_dev;
}

/**
 * @}
 */

/* end of file */
