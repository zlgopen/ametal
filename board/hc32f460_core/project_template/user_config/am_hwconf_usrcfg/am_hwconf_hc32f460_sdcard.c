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
 * \brief 用户配置文件
 * \sa am_hwconf_sdcard.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-30  cds, first implementation
 * \endinternal
 */
#include "am_hc32f460_sdioc.h"
#include "hw/amhw_hc32f460_sdioc.h"
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_gpio.h"
#include "am_sdcard.h"
#include "am_clk.h"
#include "am_hc32f460_clk.h"
#include "am_hc32f460_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_sdcard
 * \copydoc am_hwconf_hc32f460_sdcard.c
 * @{
 */

/**
 * \brief SDCARD 设备信息实例
 */
static const am_sdcard_devinfo_t __g_sdcard1_info = {

        /** \brief SDIO SD 4线模式 */
        AM_SDIO_SD_4B_M,

        /** \brief SDIO速率*/
        1000000,

        /** \brief SDIO CRC使能标志*/
        AM_TRUE,

        /** \brief SD Card支持的ORC（电压范围） */
        (AM_SD_OCR_VDD_27_28 | AM_SD_OCR_VDD_28_29 | AM_SD_OCR_VDD_29_30 |
         AM_SD_OCR_VDD_30_31 | AM_SD_OCR_VDD_31_32 | AM_SD_OCR_VDD_32_33 |
         AM_SD_OCR_VDD_33_34 | AM_SD_OCR_VDD_34_35 | AM_SD_OCR_VDD_35_36),
};

/**
 * \brief SDCARD 设备实例
 */
static am_sdcard_dev_t    __g_sdcard1_dev;

/* 实例初始化 */
am_sdcard_handle_t am_sdcard1_inst_init (void)
{

    return  am_sdcard_init(&__g_sdcard1_dev,
                           &__g_sdcard1_info,
                           (am_sdio_handle_t)am_hc32f460_sdioc1_inst_init());

}

/**
 * @}
 */

/* end of file */
