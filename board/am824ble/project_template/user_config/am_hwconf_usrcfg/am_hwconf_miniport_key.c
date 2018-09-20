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
 * \brief MiniPort-KEY 用户配置文件
 * \sa am_hwconf_miniport_key.c
 *
 * 单独使用 MiniPort-KEY 时使用该文件提供的实例初始化函数
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  tee, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_input.h"
#include "am_key_matrix_gpio.h"
#include "am_lpc82x.h"

/**
 * \addtogroup am_if_src_hwconf_miniport_key
 * \copydoc am_hwconf_miniport_key.c
 * @{
 */

/** \brief MiniPort-KEY 按键编码信息 */
am_local am_const int __g_miniport_key_codes[] = {
    KEY_0,    /* 按键 KEY0 的键值 */
    KEY_1,    /* 按键 KEY1 的键值 */
    KEY_2,    /* 按键 KEY2 的键值 */
    KEY_3     /* 按键 KEY3 的键值 */
};

/** \brief MiniPort-KEY 按键行线引脚 */
am_local am_const int __g_miniport_key_pins_row[] = {
    PIO0_6,    /* 第 0 行引脚 */
    PIO0_7     /* 第 1 行引脚 */
};

/** \brief MiniPort-KEY 按键列线引脚 */
am_local am_const int __g_miniport_key_pins_col[] = {
    PIO0_17,    /* 第 0 列引脚 */
    PIO0_23     /* 第 1 列引脚 */
};

/** \brief MiniPort-KEY 设备信息 */
am_local am_const
am_key_matrix_gpio_softimer_info_t __g_miniport_key_devinfo = {
    {
        {
            2,                           /* 2 行按键 */
            2,                           /* 2 列按键 */
            __g_miniport_key_codes,      /* 各按键对应的编码 */
            AM_TRUE,                     /* 按键低电平视为按下 */
            AM_KEY_MATRIX_SCAN_MODE_COL, /* 扫描方式，按列扫描（便于列线引脚复用） */
        },
        __g_miniport_key_pins_row,       /* 按键行线引脚 */
        __g_miniport_key_pins_col,       /* 按键列线引脚 */
    },
    5,                                   /* 扫描时间间隔，5ms */
};

/** \brief MiniPort-KEY 设备实例 */
am_local am_key_matrix_gpio_softimer_t __g_miniport_key_dev;

/**
 * \brief MiniPort-KEY 实例初始化（单独使用）
 */
int am_miniport_key_inst_init (void)
{
    return am_key_matrix_gpio_softimer_init(&__g_miniport_key_dev,
                                            &__g_miniport_key_devinfo);
}

/**
 * @}
 */

/* end of file */
