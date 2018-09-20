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
 * \brief MicroPort-DS1302 用户配置文件
 * \sa am_hwconf_microport_ds1302.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_time.h"
#include "am_ds1302.h"
#include "am_lpc82x.h"
#include "am_lpc82x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_microport_ds1302
 * \copydoc am_hwconf_microport_ds1302.c
 * @{
 */

/** \brief MicroPort-DS1302 设备信息 */
am_local am_const am_ds1302_gpio_devinfo_t __g_microport_ds1302_gpio_devinfo = {
    PIO0_15,    /* SCLK 引脚 */
    PIO0_14,    /* CE 引脚 */
    PIO0_13,    /* IO 引脚 */
};

/*******************************************************************************
  MicroPort-DS1302 实例初始化（使用芯片特殊功能）
*******************************************************************************/

/** \brief MicroPort-DS1302 (GPIO 模式) 设备实例 */
am_local am_ds1302_gpio_dev_t __g_microport_ds1302_gpio_dev;

/**
 * \brief MicroPort-DS1302 实例初始化（使用芯片特殊功能）
 */
am_ds1302_handle_t am_microport_ds1302_inst_init (void)
{
    return am_ds1302_gpio_init(&__g_microport_ds1302_gpio_dev,
                               &__g_microport_ds1302_gpio_devinfo);
}

/*******************************************************************************
  MicroPort-DS1302 实例初始化 （使用通用的 RTC 功能）
*******************************************************************************/

/** \brief MicroPort-DS1302 (RTC 标准服务) 设备实例 */
am_local am_rtc_serv_t __g_microport_ds1302_rtc;

/** \brief MicroPort-DS1302 实例初始化（使用通用的 RTC 功能） */
am_rtc_handle_t am_microport_ds1302_rtc_inst_init (void)
{
    am_ds1302_handle_t handle = am_microport_ds1302_inst_init();

    if (NULL != handle) {
        return am_ds1302_rtc_init(handle,
                                 &__g_microport_ds1302_rtc);
    } else {
        return NULL;
    }
}

/*******************************************************************************
  MicroPort-DS1302 实例初始化 （将 DS1302 用作系统时间）
*******************************************************************************/

/** \brief 每 1ms(1000000ns) 根据系统时钟更新一次系统时间值 */
#define __UPDATE_SYSCLK_NS    1000000

/** \brief 每 10s 根据 RTC 更新一次系统时间值 */
#define __UPDATE_RTC_S        10

/**
 * \brief MicroPort-DS1302 实例初始化（将 DS1302 用作系统时间）
 */
int am_microport_ds1302_time_inst_init (void)
{
    am_rtc_handle_t handle = am_microport_ds1302_rtc_inst_init();

    return am_time_init(handle,
                        __UPDATE_SYSCLK_NS,
                        __UPDATE_RTC_S);
}

/**
 * @}
 */

/* end of file */
