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
 * \brief RX8025T 配置文件
 * \sa am_hwconf_rx8025t.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_lpc82x_inst_init.h"
#include "lpc82x_pin.h"
#include "am_rx8025t.h"
#include "am_time.h"
#include "am_alarm_clk.h"

/**
 * \addtogroup am_if_src_hwconf_rx8025t
 * \copydoc am_hwconf_rx8025t.c
 * @{
 */

/*******************************************************************************
    RX8025T 实例初始化（使用芯片特殊功能）
*******************************************************************************/

static am_rx8025t_dev_t    __g_rx8025t_dev; /* 定义一个rx8025t实例 */

static const am_rx8025t_devinfo_t  __g_rx8025t_devinfo = {  /* RX8025T实例信息 */
    PIO0_1,                                                 /* INT对应的IO引脚号 */
    PIO0_10,                                                /* CLK_EN对应的IO引脚号 */
};

am_rx8025t_handle_t am_rx8025t_inst_init (void)
{
    am_i2c_handle_t i2c_handle = am_lpc82x_i2c1_inst_init(); /* 获取I2C1实例句柄 */
    static am_rx8025t_handle_t rx8025t_handle = NULL;

    if (rx8025t_handle == NULL) {
        rx8025t_handle = am_rx8025t_init(&__g_rx8025t_dev,
                                         &__g_rx8025t_devinfo,
                                          i2c_handle);
    }

    return rx8025t_handle;
}

/*******************************************************************************
    RX8025T 实例初始化 （使用通用的RTC功能）
*******************************************************************************/

static am_rtc_serv_t      __g_rx8025t_rtc; /* 定义一个 RTC 实例 */

am_rtc_handle_t am_rx8025t_rtc_inst_init(void)
{
    am_rx8025t_handle_t rx8025t_handle = am_rx8025t_inst_init();

    return am_rx8025t_rtc_init(rx8025t_handle, &__g_rx8025t_rtc);
}

/*******************************************************************************
    RX8025T 实例初始化 （使用通用的闹钟功能）
*******************************************************************************/

static am_alarm_clk_serv_t  __g_rx8025t_alarm_clk; /* 定义一个闹钟实例 */

am_alarm_clk_handle_t  am_rx8025t_alarm_clk_inst_init(void)
{
    am_rx8025t_handle_t  rx8025t_handle = am_rx8025t_inst_init();
    return am_rx8025t_alarm_clk_init (rx8025t_handle, &__g_rx8025t_alarm_clk);
}

/*******************************************************************************
    RX8025T 实例初始化 （将RX8025T用作系统时间）
*******************************************************************************/

#define __UPDATE_SYSCLK_NS   1000000 /**< \brief 每1ms(1000000ns)根据系统时钟更新一次系统时间值 */
#define __UPDATE_RTC_S       10      /**< \brief 每10s根据RTC更新一次系统时间值 */

int am_rx8025t_time_inst_init(void)
{
    am_rtc_handle_t rtc_handle = am_rx8025t_rtc_inst_init();

    return am_time_init(rtc_handle,
                        __UPDATE_SYSCLK_NS,
                        __UPDATE_RTC_S);
}

/**
 * @}
 */

/* end of file */
