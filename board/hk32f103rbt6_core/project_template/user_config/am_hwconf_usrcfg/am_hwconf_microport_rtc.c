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
 * \brief MicroPort RTC(PCF85063) 用户配置文件
 * \sa am_hwconf_microport_rtc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation
 * \endinternal
 */

#include "hk32f103rbt6_pin.h"
#include "ametal.h"
#include "am_time.h"
#include "am_pcf85063.h"
#include "am_alarm_clk.h"
#include "am_hk32f103rbt6_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_microport_rtc
 * \copydoc am_hwconf_microport_rtc.c
 * @{
 */

/** \brief 设备信息 */
am_local am_const am_pcf85063_devinfo_t __g_microport_rtc_devinfo = {
    PIOC_2,    /**< \brief INT 对应的 IO 引脚号 */
    -1,        /**< \brief CLK_EN 未使用 */
};

/*******************************************************************************
  MicroPort PCF85063 实例初始化（使用芯片特殊功能）
*******************************************************************************/

/** \brief 设备定义 */
am_local am_pcf85063_dev_t __g_microport_rtc_dev;

/** \brief 实例初始化 */
am_pcf85063_handle_t am_microport_rtc_inst_init (void)
{
    am_i2c_handle_t               i2c_handle           = am_hk32f103rbt6_i2c1_inst_init();
    am_local am_pcf85063_handle_t microport_rtc_handle = NULL;

    if (microport_rtc_handle == NULL) {
        microport_rtc_handle = am_pcf85063_init(&__g_microport_rtc_dev,
                                                &__g_microport_rtc_devinfo,
                                                 i2c_handle);
    }

    return microport_rtc_handle;
}

/*******************************************************************************
  MicroPort PCF85063 实例初始化 （使用通用的 RTC 功能）
*******************************************************************************/

/** \brief 设备定义 */
am_local am_rtc_serv_t __g_microport_rtc_rtc;

/** \brief 实例初始化 */
am_rtc_handle_t am_microport_std_rtc_inst_init (void)
{
    am_pcf85063_handle_t microport_rtc_handle = am_microport_rtc_inst_init();

    return am_pcf85063_rtc_init(microport_rtc_handle, &__g_microport_rtc_rtc);
}

/*******************************************************************************
  MicroPort PCF85063 实例初始化 （使用通用的闹钟功能）
*******************************************************************************/

/** \brief 设备定义 */
am_local am_alarm_clk_serv_t __g_microport_rtc_alarm_clk;

/** \brief 实例初始化 */
am_alarm_clk_handle_t am_microport_rtc_alarm_clk_inst_init (void)
{
    am_pcf85063_handle_t microport_rtc_handle = am_microport_rtc_inst_init();
    return am_pcf85063_alarm_clk_init(microport_rtc_handle,
                                     &__g_microport_rtc_alarm_clk);
}

/*******************************************************************************
  MicroPort PCF85063 实例初始化 （将 PCF85063 用作系统时间）
*******************************************************************************/

/** \brief 每 1ms(1000000ns) 根据系统时钟更新一次系统时间值 */
#define __UPDATE_SYSCLK_NS   1000000

/** \brief 每 10s 根据 RTC 更新一次系统时间值 */
#define __UPDATE_RTC_S       10

/** \brief 实例初始化 */
int am_microport_rtc_time_inst_init (void)
{
    am_rtc_handle_t rtc_handle = am_microport_std_rtc_inst_init();

    return am_time_init(rtc_handle,
                        __UPDATE_SYSCLK_NS,
                        __UPDATE_RTC_S);
}

/**
 * @}
 */

/* end of file */
