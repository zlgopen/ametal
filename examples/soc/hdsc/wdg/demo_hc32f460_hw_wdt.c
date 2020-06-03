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
 * \brief WDT 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 修改宏定义 __WDT_FEED_TIME_MS 的值，超过 __WDT_TIMEOUT_MS，芯片复位；
 *   2. 修改宏定义 __WDT_FEED_TIME_MS 的值，小于 __WDT_TIMEOUT_MS，程序正常运行。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_wdt.c src_hc32f460_hw_wdt
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-12
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_wdt
 * \copydoc demo_hc32f460_hw_wdt.c
 */

/** [src_hc32f460_hw_wdt] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_hc32f460.h"
#include "am_hc32f460_wdt.h"

#define __AMHW_HC32F460_WDT_COUNTER_MAX_CLK_DIV  (8192ULL)  /* 看门狗计数时钟最大分频 */
#define __AMHW_HC32F460_WDT_COUNTER_MIN_CLK_DIV  (4UL)     /* 看门狗计数时钟最小分频 */
#define __AMHW_HC32F460_WDT_COUNTER_MAX_PERIOD   (65536ULL) /* 看门狗最大计数周期 */
#define __AMHW_HC32F460_WDT_COUNTER_MIN_PERIOD   (256UL)   /* 看门狗最小计数周期 */


/**
 * \brief 看门狗使能
 *
 * \param[in] timeout_ms : 超时时间值，单位：ms
 *
 * \return 无
 *
 */
static int __hc32f460_wdt_enable (amhw_hc32f460_wdt_t *p_hw_wdt,
                                  uint32_t           timeout_ms,
                                  uint32_t           clk_rate)
{
    if ((timeout_ms > 1000UL * __AMHW_HC32F460_WDT_COUNTER_MAX_PERIOD *
                      __AMHW_HC32F460_WDT_COUNTER_MAX_CLK_DIV / clk_rate) ||
        (timeout_ms < 1000 * __AMHW_HC32F460_WDT_COUNTER_MIN_PERIOD *
                      __AMHW_HC32F460_WDT_COUNTER_MIN_CLK_DIV / clk_rate)) {
        return AM_ERROR;
    }

    uint8_t i ,j = 0;
    float temp_time = 0;
    float err_currunt = 0; /* 当前误差 */
    float err_min = 0;     /* 最小误差 */
    uint32_t temp_clk_div = 0;
    float temp_preiod_count = 0;
    uint8_t temp_clk_div_regval = 0;
    uint8_t temp_period_regval = 0;
    uint8_t clk_div_regval = 0;
    uint8_t period_regval = 0;
    float time_true = 0;
    uint32_t reg_cr_val = 0;

    for (i = 0; i < 8; i++) {

        switch (i) {

        case 0: temp_clk_div = 4;    temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_4;    break;
        case 1: temp_clk_div = 64;   temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_64;   break;
        case 2: temp_clk_div = 128;  temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_128;  break;
        case 3: temp_clk_div = 256;  temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_256;  break;
        case 4: temp_clk_div = 512;  temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_512;  break;
        case 5: temp_clk_div = 1024; temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_1024; break;
        case 6: temp_clk_div = 2048; temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_2048; break;
        case 7: temp_clk_div = 8192; temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_8192; break;
        }

        for (j = 0; j < 4; j++) {
            switch (j) {

            case 0: temp_preiod_count = 256;   temp_period_regval = AMHW_HC32F460_WDT_COUNT_PERIOD_256;   break;
            case 1: temp_preiod_count = 4096;  temp_period_regval = AMHW_HC32F460_WDT_COUNT_PERIOD_4096;  break;
            case 2: temp_preiod_count = 16384; temp_period_regval = AMHW_HC32F460_WDT_COUNT_PERIOD_16384; break;
            case 3: temp_preiod_count = 65536; temp_period_regval = AMHW_HC32F460_WDT_COUNT_PERIOD_65536; break;
            }

            temp_time = temp_preiod_count * temp_clk_div / clk_rate  * 1000;
            err_currunt = temp_time > timeout_ms ? (temp_time - timeout_ms) : (timeout_ms - temp_time);

            if (i == 0 && j == 0) {
                err_min = err_currunt;
                clk_div_regval = temp_clk_div_regval;
                period_regval = temp_period_regval;
                time_true = temp_time;
            } else {
               if (err_currunt < err_min) {
                   err_min = err_currunt;
                   clk_div_regval = temp_clk_div_regval;
                   period_regval = temp_period_regval;
                   time_true = temp_time;
               }
            }
        }
    }

    AM_DBG_INFO("the true wdt time : %d ms!\r\n", (uint32_t)time_true);

    reg_cr_val |= AMHW_HC32F460_WDT_TRIGGER_RESET_REQUEST;

    reg_cr_val |= AMHW_HC32F460_WDT_SLEEPOFF;

    reg_cr_val |= AMHW_HC32F460_WDT_REFRESH_RANGE_0_100;

    reg_cr_val |= (clk_div_regval << 4);

    reg_cr_val |= period_regval;


    amhw_hc32f460_wdt_cr_set(p_hw_wdt, reg_cr_val);


    /* 启动看门狗 */
    amhw_hc32f460_wdt_refresh_set(p_hw_wdt, AMHW_HC32F460_WDT_REFRESH_START);
    amhw_hc32f460_wdt_refresh_set(p_hw_wdt, AMHW_HC32F460_WDT_REFRESH_END);

    return AM_OK;
}

/**
 * \brief 看门狗喂狗
 *
 * \return 无
 *
 */
static void __hc32f460_sdt_feed (amhw_hc32f460_wdt_t *p_hw_wdt)
{
    amhw_hc32f460_wdt_refresh_set(p_hw_wdt, AMHW_HC32F460_WDT_REFRESH_START);
    amhw_hc32f460_wdt_refresh_set(p_hw_wdt, AMHW_HC32F460_WDT_REFRESH_END);
    AM_DBG_INFO("feed dog !\r\n");
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_hw_wdt_entry (void     *p_hw_wdt,
                                 uint32_t  time_out_ms,
                                 uint32_t  feed_time_ms,
                                 uint32_t  clk_rate)
{


	__hc32f460_wdt_enable((amhw_hc32f460_wdt_t *)p_hw_wdt, time_out_ms, clk_rate);

    while (1) {

        /* 喂狗操作 */
        __hc32f460_sdt_feed(p_hw_wdt);

        /* 延时，当延时大于喂狗时间时,会产生看门狗事件，MCU复位 */
        am_mdelay(feed_time_ms);
    }
}
/** [src_hc32f460_hw_wdt] */

/* end of file */
