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
 * \brief ZLG LCD 用户配置文件
 * \sa am_hwconf_zsn700_lcd.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-27
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_zsn700.h"
#include "am_zsn700_lcd.h"
#include "hw/amhw_zsn700_lcd.h"
#include "zsn700_pin.h"

/**
 * \addtogroup am_if_src_hwconf_zsn700_lcd
 * \copydoc am_hwconf_zsn700_lcd.c
 * @{
 */

/**
 * \brief LCD 平台初始化
 */
void __zsn700_plfm_lcd_init (void)
{
    /* 初始化LCD外部GPIO引脚 */
    am_gpio_pin_cfg(PIOA_9,  PIOA_9_AIN);  /**< \brief COM0 */
    am_gpio_pin_cfg(PIOA_10, PIOA_10_AIN); /**< \brief COM1 */
    am_gpio_pin_cfg(PIOA_11, PIOA_11_AIN); /**< \brief COM2 */
    am_gpio_pin_cfg(PIOA_12, PIOA_12_AIN); /**< \brief COM3 */

    am_gpio_pin_cfg(PIOA_8,  PIOA_8_AIN);  /**< \brief SEG0 */
    am_gpio_pin_cfg(PIOC_9,  PIOC_9_AIN);  /**< \brief SEG1 */
    am_gpio_pin_cfg(PIOC_8,  PIOC_8_AIN);  /**< \brief SEG2 */
    am_gpio_pin_cfg(PIOC_7,  PIOC_7_AIN);  /**< \brief SEG3 */
    am_gpio_pin_cfg(PIOC_6,  PIOC_6_AIN);  /**< \brief SEG4 */
    am_gpio_pin_cfg(PIOB_15, PIOB_15_AIN); /**< \brief SEG5 */
    am_gpio_pin_cfg(PIOB_14, PIOB_14_AIN); /**< \brief SEG6 */
    am_gpio_pin_cfg(PIOB_13, PIOB_13_AIN); /**< \brief SEG7 */
    am_gpio_pin_cfg(PIOB_3,  PIOB_3_AIN);  /**< \brief VLCDH */
    am_gpio_pin_cfg(PIOB_4,  PIOB_4_AIN);  /**< \brief VLCD3 */
    am_gpio_pin_cfg(PIOB_5,  PIOB_5_AIN);  /**< \brief VLCD2 */
    am_gpio_pin_cfg(PIOB_6,  PIOB_6_AIN);  /**< \brief VLCD1 */

}

/**
 * \brief LCD 平台去初始化
 */
void __zsn700_plfm_lcd_deinit (void)
{
    /* 关闭LCD时钟 */
    am_clk_disable(CLK_LCD);
}

/** \brief LCD 设备信息 */
static const am_zsn700_lcd_devinfo_t __g_lcd_devinfo =
{
    /**< \brief 指向LCD寄存器块的指针 */
    ZSN700_LCD_BASE,

    /**< \brief LCD中断编号 */
    INUM_LCD,

    /**
     * \brief LCD偏置源
     *
     * \note : 其值参考amhw_zsn700_lcd.h中宏定义:Bias电压来源选择掩码
     */
    AMHW_ZSN700_LCD_BIAS_CAP_DIV,

    /**
     * \brief LCD占空比
     *
     * \note : 其值参考amhw_zsn700_lcd.h中枚举定义：LCD duty配置
     *
     */
    AMHW_ZSN700_DUTY_1_4,

    /**
     * \brief LCD偏置电压
     *
     * \note : 其值参考amhw_zsn700_lcd.h中宏定义：偏置电压配置掩码
     */
    AMHW_ZSN700_BIAS_1_3,

    /**
     * \brief LCD电压泵时钟频率
     *
     * \note : 其值参考amhw_zsn700_lcd.h中宏定义：电压泵时钟频率选择掩码
     */
    AMHW_ZSN700_CPCLK_2K_HZ,

    /**
     * \brief LCD扫描频率
     *
     * \note : 其值参考amhw_zsn700_lcd.h中宏定义：LCD扫描频率选择掩码
     */
    AMHW_ZSN700_LCDCLK_128_HZ,

    /**
     * \brief 时钟源    （XTL RCL）
     *
     */
    AMHW_ZSN700_LCD_CLK_SRC_RCL,

    /**< \brief LCD平台初始化函数 */
    __zsn700_plfm_lcd_init,

    /**< \brief LCD平台解初始化函数  */
    __zsn700_plfm_lcd_deinit,
};

/** \brief LCD设备实例 */
static am_zsn700_lcd_dev_t __g_lcd_dev;

/** \brief lcd 实例初始化，获得LCD标准服务句柄 */
am_lcd_handle_t am_zsn700_lcd_inst_init (void)
{
    return am_zsn700_lcd_init(&__g_lcd_dev, &__g_lcd_devinfo);
}

/** \brief LCD 实例解初始化 */
void am_zsn700_lcd_inst_deinit (am_lcd_handle_t handle)
{
    am_zsn700_lcd_deinit(handle);
}

/**
 * @}
 */

/* end of file */
