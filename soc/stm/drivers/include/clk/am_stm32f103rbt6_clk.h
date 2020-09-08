
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
 * \brief CLK驱动，时钟标准服务接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-27  yrh, first implementation
 * \endinternal
 */

#ifndef __AM_STM32F103RBT6_CLK_H
#define __AM_STM32F103RBT6_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_clk.h"
#include "amhw_stm32f103rbt6_rcc.h"

/**
 * \addtogroup am_stm32f103rbt6_if_clk
 * \copydoc am_stm32f103rbt6_clk.h
 * @{
 */
/**
 * \brief 系统时钟源
 */
typedef enum {
    AM_STM32F103RBT6_SYSCLK_HSI, /**< \brief HSI 作为系统时钟 */
    AM_STM32F103RBT6_SYSCLK_HSE, /**< \brief HSE 作为系统时钟 */
    AM_STM32F103RBT6_SYSCLK_PLL, /**< \brief PLL 输出作为系统时钟 */
    AM_STM32F103RBT6_SYSCLK_LSI, /**< \brief LSI 输出作为系统时钟 */
    AM_STM32F103RBT6_SYSCLK_LSE  /**< \brief LSE 输出作为系统时钟 */
}am_stm32f103rbt6_sys_clk_src;

/**
 * \brief CLK 设备信息参数结构体
 */
typedef struct am_stm32f103rbt6_clk_devinfo {
    /**
     * \brief 系统时钟源选择
     *
     * AM_STM32F103RBT6_SYSCLK_HSI : HSI 作为系统时钟
     * AM_STM32F103RBT6_SYSCLK_HSE : HSE 作为系统时钟
     * AM_STM32F103RBT6_SYSCLK_PLL : PLL 输出作为系统时钟
     * AM_STM32F103RBT6_SYSCLK_LSI : LSI 输出作为系统时钟
     * AM_STM32F103RBT6_SYSCLK_LSE : LSE 输出作为系统时钟
     */

    uint32_t sysclk_src;
    /**
     * \brief HSEOSC外部晶振频率
     *
     *  如果pllin_src选择 AMHW_STM32F103RBT6_PLLCLK_HSE 则 PLLIN = hse_osc）
     */
    uint32_t hse_osc;

    /**
     * \brief LSEOSC外部晶振频率
     *
     *  如果rtc_src选择 AMHW_STM32F103RBT6_RTCCLK_LSE 则 rtcclk = lse_osc）
     */
    uint32_t lse_osc;

    /** \brief
     *    PLL 时钟源选择
     *    -# AMHW_STM32F103RBT6_PLLCLK_HSI : HSI 作为PLL输入时钟
     *    -# AMHW_STM32F103RBT6_PLLCLK_HSE : HSE 作为PLL输入时钟
     */
    amhw_stm32f103rbt6_pll_clk_src pllin_src;

    /**
     * \brief PLL 倍频系数，可选2-17
     *        PLLOUT = PLLIN * pll_mul
     */
    uint8_t pll_mul;

    /** \brief USB分频系数，
     *
     *  AMHW_STM32F103RBT6_PLL_USB_DIV1P5 : PLL 1.5 分频作为usb输入时钟 ,USBCLK = PLLOUT / 1.5;
     *  AMHW_STM32F103RBT6_PLL_USB_DIV1   : PLL 1 分频作为usb输入时钟 ,  USBCLK = PLLOUT / 1。
     *
     *  建议配置成48Mhz
     */
    amhw_stm32f103rbt6_pll_usb_clk_div usb_div;

    /**
     * \brief AHB分频系数，AHBCLK = PLLOUT / DIV,AHB最大频率为80Mhz
     *
     *    ahb_div |  DIV
     *   ---------------------
     *        0-7 |   1
     *        8   |   2
     *        9   |   4
     *        10  |   8
     *        11  |   16
     *        12  |   64
     *        13  |   128
     *        14  |   256
     *        15  |   512
     */
    uint8_t ahb_div;

    /**
     * \brief APB1分频系数，APB1CLK = AHBCLK / (2 ^ apb1_div)
     *        APB1最大频率为80Mhz
     */
    uint8_t apb1_div;

    /**
     * \brief APB2分频系数，APB2CLK = AHBCLK / (2 ^ apb2_div)
     *        APB2最大频率为36Mhz
     */
    uint8_t apb2_div;

    /**
     * \brief ADC分频系数，ADCCLK = APB2 / DIV
     *    adc_div |  DIV
     *   -----------------
     *          0 | 2 分频
     *          1 | 4 分频
     *          2 | 8 分频
     *          3 | 16分频
     *
     *        ADC最大频率为14Mhz
     */
    uint8_t adc_div;

    /** \brief 平台初始化函数，配置引脚等工作 */
    void  (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void  (*pfn_plfm_deinit)(void);

} am_stm32f103rbt6_clk_devinfo_t;

/**
 * \brief CLK 设备结构体
 */
typedef struct am_stm32f103rbt6_clk_dev {

    /** \brief 指向CLK 设备信息的指针 */
    const am_stm32f103rbt6_clk_devinfo_t *p_devinfo;

    /** \brief PLL输入频率 */
    uint32_t sys_clk;

    /** \brief PLL输入频率 */
    uint32_t pllin_clk;

    /** \brief PLL输出频率 */
    uint32_t pllout_clk;

    /** \brief AHB时钟频率 */
    uint32_t ahb_clk;

    /** \brief APB1时钟频率 */
    uint32_t apb1_clk;

    /** \brief APB2时钟频率 */
    uint32_t apb2_clk;

    /** \brief USB时钟频率 */
    uint32_t usb_clk;

    /** \brief RTC时钟频率 */
    uint32_t rtc_clk;

} am_stm32f103rbt6_clk_dev_t;

/**
 * \brief 获取时钟频率
 */
int am_clk_rate_get (am_clk_id_t clk_id);

/**
 * \brief CLK 初始化
 *
 * \param[in] p_dev     : 指向CLK设备的指针
 * \param[in] p_devinfo : 指向CLK设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_stm32f103rbt6_clk_init (am_stm32f103rbt6_clk_dev_t           *p_dev,
                        const am_stm32f103rbt6_clk_devinfo_t *p_devinfo);

/**
 * \brief CLK 外设复位
 *
 * \param[in] clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \retval AM_OK : 操作成功
 */
int am_stm32f103rbt6_clk_reset (am_clk_id_t clk_id);

/**
 * \brief 更新总线频率值
 *
 *  当总线频率被重新设置时，需要调用此函数更新驱动内部存储的频率值
 *  仅对CLK_AHB，CLK_APB1，CLK_APB2有效
 *
 * \param[in] clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 * \param[in] clk     更新的频率值
 *
 * \retval AM_OK : 操作成功
 *
 * \note 注：此函数慎用
 */
int am_stm32f103rbt6_clk_update (am_clk_id_t clk_id, int clk);

/**
 * \brief 获取PLL的输入时钟源
 *  仅对CLK_AHB，CLK_APB1，CLK_APB2有效
 *
 * \return clk的时钟配置信息
 */
const am_stm32f103rbt6_clk_dev_t *am_stm32f103rbt6_clk_devinfo_get (void);

/**
 * \brief 返回总线或外设的CLK分频值
 *
 *
 * \param[in] clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \return: 返回对应总线或外设的分频值
 *
 */
int am_stm32f103rbt6_div_get (am_clk_id_t clk_id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_STM32F103RBT6_CLK_H */

/* end of file */
