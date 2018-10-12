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
 * \brief MiniPort-View 用户配置文件
 * \sa am_hwconf_miniport_view.c
 *
 * MiniPort-View 可以单独使用，也可以和 MiniPort-595 联合使用，以节省控制引脚
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

#include "ametal.h"
#include "am_input.h"
#include "am_key_matrix_gpio.h"
#include "am_key_matrix_softimer.h"
#include "am_miniport_view_key.h"
#include "am_miniport_view_key_595.h"
#include "am_digitron_scan_gpio.h"
#include "am_digitron_scan_hc595_gpio.h"
#include "am_lpc82x.h"
#include "am_lpc82x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_miniport_view
 * \copydoc am_hwconf_miniport_view.c
 * @{
 */

/** \brief 显示缓存，大小为数码管个数，即 2 */
am_local uint8_t __g_miniport_view_disp_buf[2];

/** \brief 扫描缓存，每次仅可扫描一个数码管，因此大小为 1 */
am_local uint8_t __g_miniport_view_scan_buf[1];

/** \brief MiniPort-View 数码管段选引脚 */
am_local am_const int __g_miniport_view_seg_pins[] = {
    PIO0_8,     /* SEG A 引脚 */
    PIO0_9,     /* SEG B 引脚 */
    PIO0_10,    /* SEG C 引脚 */
    PIO0_11,    /* SEG D 引脚 */
    PIO0_12,    /* SEG E 引脚 */
    PIO0_13,    /* SEG F 引脚 */
    PIO0_14,    /* SEG G 引脚 */
    PIO0_15     /* SEG DP 引脚 */
};

/** \brief MiniPort-View 数码管位选引脚 */
am_local am_const int __g_miniport_view_com_pins[] = {
    PIO0_17,    /* COM 0 引脚 */
    PIO0_23     /* COM 1 引脚 */
};

/*******************************************************************************
  MiniPort-View 单独使用
*******************************************************************************/

/** \brief MiniPort-View 设备信息 */
am_local am_const am_digitron_scan_gpio_info_t __g_miniport_view_devinfo = {
    {
        {
            0,                         /* 数码管对应的数码管显示器 ID 为 0 */
        },
        50,                            /* 扫描频率， 50Hz */
        500,                           /* 闪烁时亮的时长：500ms */
        500,                           /* 闪烁时灭的时长：500ms */
        __g_miniport_view_disp_buf,    /* 显示缓存 */
        __g_miniport_view_scan_buf,    /* 扫描缓存 */
    },
    {
        8,                             /* 8 段数码管 */
        1,                             /* 仅单行数码管 */
        2,                             /* 两列数码管 */
        AM_DIGITRON_SCAN_MODE_COL,     /* 扫描方式，按列扫描 */
        AM_TRUE,                       /* 段码低电平有效 */
        AM_TRUE,                       /* 位选低电平有效 */
    },
    __g_miniport_view_seg_pins,        /* 数码管段选引脚 */
    __g_miniport_view_com_pins,        /* 数码管位选引脚 */
};

/** \brief MiniPort-View 设备实例 */
am_local am_digitron_scan_gpio_dev_t __g_miniport_view_dev;

/**
 * \brief MiniPort-View 实例初始化（单独使用）
 */
int am_miniport_view_inst_init (void)
{
    return am_digitron_scan_gpio_init(&__g_miniport_view_dev,
                                      &__g_miniport_view_devinfo);
}

/*******************************************************************************
  MiniPort-View 和 MiniPort-595 联合使用
*******************************************************************************/

/** \brief MiniPort-595 + MiniPort-View 设备信息 */
am_local am_const
am_digitron_scan_hc595_gpio_info_t __g_miniport_view_595_devinfo = {
    {
        {
            0,                         /* 数码管对应的数码管显示器 ID 为 0 */
        },
        50,                            /* 扫描频率， 50Hz */
        500,                           /* 闪烁时亮的时长：500ms */
        500,                           /* 闪烁时灭的时长：500ms */
        __g_miniport_view_disp_buf,    /* 显示缓存 */
        __g_miniport_view_scan_buf,    /* 扫描缓存 */
    },
    {
        8,                             /* 8 段数码管 */
        1,                             /* 仅单行数码管 */
        2,                             /* 两列数码管 */
        AM_DIGITRON_SCAN_MODE_COL,     /* 扫描方式，按列扫描 */
        AM_TRUE,                       /* 段码低电平有效 */
        AM_TRUE,                       /* 位选低电平有效 */
    },
    __g_miniport_view_com_pins,        /* 数码管位选引脚 */
};

/** \brief MiniPort-595 + MiniPort-View 设备实例 */
am_local am_digitron_scan_hc595_gpio_dev_t __g_miniport_view_595_dev;

/**
 * \brief MiniPort-595 + MiniPort-View 实例初始化
 */
int am_miniport_view_595_inst_init (void)
{
    return am_digitron_scan_hc595_gpio_init(&__g_miniport_view_595_dev,
                                            &__g_miniport_view_595_devinfo,
                                             am_miniport_595_inst_init());
}

/**
 * @}
 */

/* end of file */
