/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief PCF85063 配置文件
 * \sa am_hwconf_pcf85063.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x_inst_init.h"
#include "lpc82x_pin.h"
#include "am_pcf85063.h"
#include "am_time.h"
#include "am_alarm_clk.h"

/**
 * \addtogroup am_if_src_hwconf_pcf85063
 * \copydoc am_hwconf_pcf85063.c
 * @{
 */

/*******************************************************************************
    PCF85063 实例初始化（使用芯片特殊功能）
*******************************************************************************/

static am_pcf85063_dev_t    __g_pcf85063_dev; /* 定义一个pcf85063实例 */

static const am_pcf85063_devinfo_t __g_pcf85063_devinfo = { /* 定义实例信息 */
    PIO0_1,                                                 /* INT对应的IO引脚号 */
    -1,                                                     /* CLK_EN未使用 */
};

am_pcf85063_handle_t am_pcf85063_inst_init (void)
{
    am_i2c_handle_t             i2c_handle      = am_lpc82x_i2c1_inst_init();
    static am_pcf85063_handle_t pcf85063_handle = NULL;

    if (pcf85063_handle == NULL) {
        pcf85063_handle = am_pcf85063_init(&__g_pcf85063_dev,
                                           &__g_pcf85063_devinfo,
                                            i2c_handle);
    }

    return pcf85063_handle;
}

/*******************************************************************************
    PCF85063 实例初始化 （使用通用的RTC功能）
*******************************************************************************/

static am_rtc_serv_t      __g_pcf85063_rtc;                 /* 定义一个 RTC 实例 */

am_rtc_handle_t am_pcf85063_rtc_inst_init(void)
{
    am_pcf85063_handle_t pcf85063_handle = am_pcf85063_inst_init();

    return am_pcf85063_rtc_init(pcf85063_handle, &__g_pcf85063_rtc);
}

/*******************************************************************************
    PCF85063 实例初始化 （使用通用的闹钟功能）
*******************************************************************************/

static am_alarm_clk_serv_t  __g_pcf85063_alarm_clk;         /* 定义一个闹钟实例 */

am_alarm_clk_handle_t  am_pcf85063_alarm_clk_inst_init(void)
{
    am_pcf85063_handle_t  pcf85063_handle = am_pcf85063_inst_init();
    return am_pcf85063_alarm_clk_init (pcf85063_handle, &__g_pcf85063_alarm_clk);
}

/*******************************************************************************
    PCF85063 实例初始化 （将PCF85063用作系统时间）
*******************************************************************************/

#define __UPDATE_SYSCLK_NS   1000000 /**< \brief 每1ms(1000000ns)根据系统时钟更新一次系统时间值 */
#define __UPDATE_RTC_S       10      /**< \brief 每10s根据RTC更新一次系统时间值 */

int am_pcf85063_time_inst_init(void)
{
    am_rtc_handle_t rtc_handle = am_pcf85063_rtc_inst_init();

    return am_time_init(rtc_handle,
                        __UPDATE_SYSCLK_NS,
                        __UPDATE_RTC_S);
}

/**
 * @}
 */

/* end of file */
