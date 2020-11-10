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
 * \brief MiniPort-View 配置文件
 * \sa am_hwconf_miniport_view.c
 *
 * 它可以单独使用，也可以和 MiniPort-595 联合使用，以节省控制引脚。
 *
 * 当前支持的用法有：
 * 1. MiniPort-View                : am_miniport_view_inst_init();
 * 2. MiniPort-View + MiniPort-595 : am_miniport_view_595_inst_init();
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-25  tee, first implementation
 * \endinternal
 */

#include "zsl42x_pin.h"
#include "ametal.h"
#include "am_zsl42x_inst_init.h"
#include "am_key_matrix_gpio.h"
#include "am_key_matrix_softimer.h"
#include "am_digitron_scan_gpio.h"
#include "am_digitron_scan_hc595_gpio.h"
#include "am_miniport_view_key.h"
#include "am_input.h"
#include "am_miniport_view_key_595.h"

/**
 * \addtogroup am_if_src_hwconf_miniport_view
 * \copydoc am_hwconf_miniport_view.c
 * @{
 */

/*******************************************************************************
   数码管扫描相关信息 （am_digitron_scan_devinfo_t类型）
*******************************************************************************/

/* 显示缓存，8段数码管，类型为uint8_t，缓存大小为数码管个数，即2 */
static uint8_t __g_disp_buf[2];

/* 扫描缓存，8段数码管，类型为uint8_t，每次仅可扫描一个数码管，因此大小为1 */
static uint8_t __g_scan_buf[1];

/*******************************************************************************
   数码管GPIO段码相关信息
*******************************************************************************/
static const int __g_digitron_seg_pins[] = {
    PIOB_7, PIOB_6, PIOB_15, PIOB_13, PIOA_10, PIOB_14, PIOB_12, PIOA_14
};

/*******************************************************************************
   数码管GPIO位码相关信息
*******************************************************************************/
static const int __g_digitron_com_pins[] = {
    PIOB_15, PIOC_15
};

/*******************************************************************************
   Public functions
*******************************************************************************/

/* MiniPort-View 单独使用实例初始化 */
int am_miniport_view_inst_init (void)
{
    static am_digitron_scan_gpio_dev_t         miniport_view;
    static const am_digitron_scan_gpio_info_t  miniport_view_info = {
        {
            {
                0,                       /* 数码管对应的数码管显示器ID为0 */
            },
            50,                          /* 扫描频率， 50Hz */
            500,                         /* 闪烁时亮的时长：500ms */
            500,                         /* 闪烁时灭的时长：500ms */
            __g_disp_buf,                /* 显示缓存 */
            __g_scan_buf,                /* 扫描缓存 */
        },
        {
            8,                           /* 8段数码管 */
            1,                           /* 仅单行数码管 */
            2,                           /* 两列数码管 */
            AM_DIGITRON_SCAN_MODE_COL,   /* 扫描方式，按列扫描 */
            AM_TRUE,                     /* 段码低电平有效 */
            AM_TRUE,                     /* 位选低电平有效 */
        },
        __g_digitron_seg_pins,
        __g_digitron_com_pins,
    };

    return am_digitron_scan_gpio_init(&miniport_view, &miniport_view_info);
}

/******************************************************************************/

/* MiniPort-View 和  MiniPort-595 联合使用实例初始化 */
int am_miniport_view_595_inst_init (void)
{
    static am_digitron_scan_hc595_gpio_dev_t         miniport_view_595;
    static const am_digitron_scan_hc595_gpio_info_t  miniport_view_595_info = {
        {
            {
                0,                       /* 数码管对应的数码管显示器ID为0 */
            },
            100,                          /* 扫描频率， 50Hz */
            500,                         /* 闪烁时亮的时长：500ms */
            500,                         /* 闪烁时灭的时长：500ms */
            __g_disp_buf,                /* 显示缓存 */
            __g_scan_buf,                /* 扫描缓存 */
        },
        {
            8,                           /* 8段数码管 */
            1,                           /* 仅单行数码管 */
            2,                           /* 两列数码管 */
            AM_DIGITRON_SCAN_MODE_COL,   /* 扫描方式，按列扫描 */
            AM_TRUE,                     /* 段码低电平有效 */
            AM_TRUE,                     /* 位选低电平有效 */
        },
        __g_digitron_com_pins,
    };

    return am_digitron_scan_hc595_gpio_init(&miniport_view_595,
                                            &miniport_view_595_info,
                                            am_miniport_595_inst_init());
}

/**
 * @}
 */

/* end of file */
