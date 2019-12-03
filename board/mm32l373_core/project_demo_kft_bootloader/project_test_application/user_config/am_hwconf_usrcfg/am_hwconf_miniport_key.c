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
 * \brief MiniPort-Key 用户配置文件
 * \sa am_hwconf_miniport_key.c
 *
 * 单独使用 Miniport-KEY 时使用该文件提供的实例初始化函数
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  tee, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_key_matrix_gpio.h"
#include "am_input.h"
#include "mm32l373_pin.h"

/**
 * \addtogroup am_if_src_hwconf_miniport_key
 * \copydoc am_hwconf_miniport_key.c
 * @{
 */

/*******************************************************************************
   按编码信息
*******************************************************************************/

static const int __g_key_codes[] = {
    KEY_0, KEY_1,
    KEY_2, KEY_3
};

/*******************************************************************************
   按键GPIO行线引脚
*******************************************************************************/
static const int __g_key_pins_row[] = {PIOB_3, PIOD_2};

/*******************************************************************************
   按键GPIO列线引脚
*******************************************************************************/
static const int __g_key_pins_col[] = {PIOA_15, PIOC_15};

/*******************************************************************************
   Public functions
*******************************************************************************/

/* MiniPort-Key 单独使用实例初始化 */
int am_miniport_key_inst_init (void)
{
    static am_key_matrix_gpio_softimer_t             miniport_key;
    static const am_key_matrix_gpio_softimer_info_t  miniport_key_info = {
        {
            {
                2,                           /* 2行按键 */
                2,                           /* 2列按键 */
                __g_key_codes,               /* 各按键对应的编码 */
                AM_TRUE,                     /* 按键低电平视为按下 */
                AM_KEY_MATRIX_SCAN_MODE_COL, /* 扫描方式，按列扫描（便于列线引脚复用） */
            },
            __g_key_pins_row,
            __g_key_pins_col,
        },
        5,                                   /* 扫描时间间隔，5ms */
    };

    return am_key_matrix_gpio_softimer_init(&miniport_key, &miniport_key_info);
}

/**
 * @}
 */

/* end of file */
