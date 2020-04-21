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
 * \sa am_hwconf_hc32_lcd.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-27
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_hc32.h"
#include "am_hc32_lcd.h"
#include "hw/amhw_hc32_lcd.h"
#include "hc32x3x_pin.h"

/**
 * \addtogroup am_if_src_hwconf_hc32_lcd
 * \copydoc am_hwconf_hc32_lcd.c
 * @{
 */

/**
 * \brief LCD 平台初始化
 */
void __hc32_plfm_lcd_init (void)
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
    am_gpio_pin_cfg(PIOB_12, PIOB_12_AIN); /**< \brief SEG8 */
    am_gpio_pin_cfg(PIOB_11, PIOB_11_AIN); /**< \brief SEG9 */
    am_gpio_pin_cfg(PIOB_10, PIOB_10_AIN); /**< \brief SEG10 */
    am_gpio_pin_cfg(PIOB_2,  PIOB_2_AIN);  /**< \brief SEG11 */
    am_gpio_pin_cfg(PIOC_5,  PIOC_5_AIN);  /**< \brief SEG12 */
    am_gpio_pin_cfg(PIOB_0,  PIOB_0_AIN);  /**< \brief SEG13 */

    am_gpio_pin_cfg(PIOB_3,  PIOB_3_AIN);  /**< \brief VLCDH */
    am_gpio_pin_cfg(PIOB_4,  PIOB_4_AIN);  /**< \brief VLCD3 */
    am_gpio_pin_cfg(PIOB_5,  PIOB_5_AIN);  /**< \brief VLCD2 */
    am_gpio_pin_cfg(PIOB_6,  PIOB_6_AIN);  /**< \brief VLCD1 */

}

/**
 * \brief LCD 平台去初始化
 */
void __hc32_plfm_lcd_deinit (void)
{
    /* 关闭LCD时钟 */
    am_clk_disable(CLK_LCD);

    am_gpio_pin_cfg(PIOA_9,  PIOA_9_GPIO  | PIOA_9_INPUT_PU);
    am_gpio_pin_cfg(PIOA_10, PIOA_10_GPIO | PIOA_10_INPUT_PU);
    am_gpio_pin_cfg(PIOA_11, PIOA_11_GPIO | PIOA_11_INPUT_PU);
    am_gpio_pin_cfg(PIOA_12, PIOA_12_GPIO | PIOA_12_INPUT_PU);

    am_gpio_pin_cfg(PIOA_8,  PIOA_8_GPIO  | PIOA_8_INPUT_PU);
    am_gpio_pin_cfg(PIOC_9,  PIOC_9_GPIO  | PIOC_9_INPUT_PU);
    am_gpio_pin_cfg(PIOC_8,  PIOC_8_GPIO  | PIOC_8_INPUT_PU);
    am_gpio_pin_cfg(PIOC_7,  PIOC_7_GPIO  | PIOC_7_INPUT_PU);
    am_gpio_pin_cfg(PIOC_6,  PIOC_6_GPIO  | PIOC_6_INPUT_PU);
    am_gpio_pin_cfg(PIOB_15, PIOB_15_GPIO | PIOB_15_INPUT_PU);
    am_gpio_pin_cfg(PIOB_14, PIOB_14_GPIO | PIOB_14_INPUT_PU);
    am_gpio_pin_cfg(PIOB_13, PIOB_13_GPIO | PIOB_13_INPUT_PU);
    am_gpio_pin_cfg(PIOB_12, PIOB_12_GPIO | PIOB_12_INPUT_PU);
    am_gpio_pin_cfg(PIOB_11, PIOB_11_GPIO | PIOB_11_INPUT_PU);
    am_gpio_pin_cfg(PIOB_10, PIOB_10_GPIO | PIOB_10_INPUT_PU);
    am_gpio_pin_cfg(PIOB_2,  PIOB_2_GPIO  | PIOB_2_INPUT_PU);
    am_gpio_pin_cfg(PIOC_5,  PIOC_5_GPIO  | PIOC_5_INPUT_PU);
    am_gpio_pin_cfg(PIOB_0,  PIOB_0_GPIO  | PIOB_0_INPUT_PU);

    am_gpio_pin_cfg(PIOB_3,  PIOB_3_GPIO  | PIOB_3_INPUT_PU);
    am_gpio_pin_cfg(PIOB_4,  PIOB_4_GPIO  | PIOB_4_INPUT_PU);
    am_gpio_pin_cfg(PIOB_5,  PIOB_5_GPIO  | PIOB_5_INPUT_PU);
    am_gpio_pin_cfg(PIOB_6,  PIOB_6_GPIO  | PIOB_6_INPUT_PU);
}

/** \brief LCD 设备信息 */
static const am_hc32_lcd_devinfo_t __g_lcd_devinfo =
{
    /**< \brief 指向LCD寄存器块的指针 */
    HC32_LCD_BASE,

    /**< \brief LCD中断编号 */
    INUM_LCD,

    /**
     * \brief LCD偏置源
     *
     * \note : 其值参考amhw_hc32_lcd.h中宏定义:Bias电压来源选择掩码
     */
    AMHW_HC32_LCD_BIAS_EXT_RES,//AMHW_HC32_LCD_BIAS_CAP_DIV,

    /**
     * \brief LCD占空比
     *
     * \note : 其值参考amhw_hc32_lcd.h中枚举定义：LCD duty配置
     *
     */
    AMHW_HC32_DUTY_1_4,//AMHW_HC32_DUTY_1_6,

    /**
     * \brief LCD偏置电压
     *
     * \note : 其值参考amhw_hc32_lcd.h中宏定义：偏置电压配置掩码
     */
    AMHW_HC32_BIAS_1_3,

    /**
     * \brief LCD电压泵时钟频率
     *
     * \note : 其值参考amhw_hc32_lcd.h中宏定义：电压泵时钟频率选择掩码
     */
    AMHW_HC32_CPCLK_2K_HZ,//AMHW_HC32_CPCLK_16K_HZ,

    /**
     * \brief LCD扫描频率
     *
     * \note : 其值参考amhw_hc32_lcd.h中宏定义：LCD扫描频率选择掩码
     */
    AMHW_HC32_LCDCLK_128_HZ,//AMHW_HC32_LCDCLK_512_HZ,

    /**
     * \brief 时钟源    （XTL RCL）
     *
     */
    AMHW_HC32_LCD_CLK_SRC_RCL,

    /**< \brief LCD平台初始化函数 */
    __hc32_plfm_lcd_init,

    /**< \brief LCD平台解初始化函数  */
    __hc32_plfm_lcd_deinit,
};


/** \brief LCD设备实例 */
static am_hc32_lcd_dev_t __g_lcd_dev;

/** \brief lcd 实例初始化，获得LCD标准服务句柄 */
am_lcd_handle_t am_hc32_lcd_inst_init (void)
{
    return am_hc32_lcd_init(&__g_lcd_dev, &__g_lcd_devinfo);
}

/** \brief LCD 实例解初始化 */
void am_hc32_lcd_inst_deinit (am_lcd_handle_t handle)
{
    am_hc32_lcd_deinit(handle);
}

/**
 * @}
 */

/* end of file */
