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
 * - 1.00 17-07-13  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_fm175xx.h"
#include "zlg116_pin.h"
#include "am_zlg116_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_fm175xx
 * \copydoc am_hwconf_fm175xx.c
 * @{
 */

/* 定义 fm175xx 实例 */
static am_fm175xx_dev_t  __g_fm175xx_dev;

/* 定义 fm175xx 实例信息 */
static const am_fm175xx_devinfo_t __g_fm175xx_devinfo = {
    PIOA_1,
    PIOA_4,
    AM_FM175XX_PROT_TYPE_ISO14443A_106
};

am_fm175xx_handle_t am_fm175xx_inst_init (void)
{
      am_fm175xx_init(&__g_fm175xx_dev,
                       am_zlg116_spi1_poll_inst_init(),
                      &__g_fm175xx_devinfo);

      return &__g_fm175xx_dev;
}

/**
 * @}
 */

/* end of file */
