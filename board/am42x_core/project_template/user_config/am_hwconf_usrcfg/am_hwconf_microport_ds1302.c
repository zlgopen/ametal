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
 * \brief MicroPort DS1302 用户配置文件
 * \sa am_hwconf_microport_ds1302.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-23  ly, first implementation
 * \endinternal
 */

#include "zsl42x_pin.h"
#include "ametal.h"
#include "am_time.h"
#include "am_ds1302.h"
#include "am_zsl42x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_microport_ds1302
 * \copydoc am_hwconf_microport_ds1302.c
 * @{
 */

/** \brief 设备信息 */
am_local am_const am_ds1302_gpio_devinfo_t __g_microport_ds1302_gpio_devinfo = {
    PIOA_3,      /**< \brief SCLK 对应的 IO 引脚号 */
    PIOA_4,      /**< \brief CE 对应的 IO 引脚号 */
    PIOA_8,      /**< \brief IO 对应的 IO 引脚号 */
};

/*******************************************************************************
  MicroPort DS1302 实例初始化（使用芯片特殊功能）
*******************************************************************************/

/** \brief 设备定义 */
am_local am_ds1302_gpio_dev_t __g_microport_ds1302_gpio_dev;

/** \brief 实例初始化 */
am_ds1302_handle_t am_microport_ds1302_inst_init (void)
{
    am_local am_ds1302_handle_t microport_ds1302_handle = NULL;

    if (microport_ds1302_handle == NULL) {
        microport_ds1302_handle = am_ds1302_gpio_init(
                                            &__g_microport_ds1302_gpio_dev,
                                            &__g_microport_ds1302_gpio_devinfo);
    }

    return microport_ds1302_handle;
}

/*******************************************************************************
  MicroPort DS1302 实例初始化 （使用通用的 RTC 功能）
*******************************************************************************/

/** \brief 设备定义 */
am_local am_rtc_serv_t __g_microport_ds1302_rtc;

/** \brief 实例初始化 */
am_rtc_handle_t am_microport_ds1302_rtc_inst_init (void)
{
    am_ds1302_handle_t microport_ds1302_handle = am_microport_ds1302_inst_init();

    return am_ds1302_rtc_init(microport_ds1302_handle,
                             &__g_microport_ds1302_rtc);
}

/*******************************************************************************
  MicroPort DS1302 实例初始化 （将 DS1302 用作系统时间）
*******************************************************************************/

/** \brief 每 1ms(1000000ns) 根据系统时钟更新一次系统时间值 */
#define __UPDATE_SYSCLK_NS   1000000

/** \brief 每 10s 根据 RTC 更新一次系统时间值 */
#define __UPDATE_RTC_S       10

/** \brief 实例初始化 */
int am_microport_ds1302_time_inst_init (void)
{
    am_rtc_handle_t rtc_handle = am_microport_ds1302_rtc_inst_init();

    return am_time_init(rtc_handle,
                        __UPDATE_SYSCLK_NS,
                        __UPDATE_RTC_S);
}

/**
 * @}
 */

/* end of file */
