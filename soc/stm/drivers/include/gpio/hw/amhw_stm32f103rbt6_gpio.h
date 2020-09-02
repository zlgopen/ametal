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
 * \brief GPIO hardware operation interface
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-4  zc, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG237_GPIO_H
#define __AMHW_ZLG237_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"

/**
 * \addtogroup amhw_zlg237_if_gpio
 * \copydoc amhw_zlg237_gpio.h
 * @{
 */

#define AMHW_ZLG237_GPIO_PORT_MAX 5  /**< \brief GPIO 定义GPIO端口数量 */

/**
 * \brief GPIO 端口寄存器结构体
 */
typedef struct gpio_reg {
    __IO  uint32_t crl;               /**< \brief 端口配置低寄存器 */
    __IO  uint32_t crh;               /**< \brief 端口配置高寄存器 */
    __I   uint32_t idr;               /**< \brief 端口输入数据寄存器 */
    __IO  uint32_t odr;               /**< \brief 端口输出数据寄存器 */
    __O   uint32_t bsrr;              /**< \brief 端口设置/清除寄存器 */
    __O   uint32_t brr;               /**< \brief 端口位清除寄存器 */
    __IO  uint32_t lckr;              /**< \brief 端口配置锁定寄存器 */
          uint32_t reserve[249];      /**< \brief 保留 */
} amhw_zlg237_gpio_reg_t;

/**
 * \brief GPIO 寄存器块结构体
 */
typedef struct amhw_zlg237_gpio {
    amhw_zlg237_gpio_reg_t    gpio[AMHW_ZLG237_GPIO_PORT_MAX];    /**< \brief GPIO 端口 */
} amhw_zlg237_gpio_t;

/**
 * \brief GPIO最大输出速度
 * \note 选择该枚举设置GPIO引脚驱动电路响应速度时，GPIO引脚也确定为输出方向
 */
typedef enum amhw_zlg237_gpio_speed_mode {
    AMHW_ZLG237_GPIO_SPEED_10MHz = 1UL,
    AMHW_ZLG237_GPIO_SPEED_2MHz = 2UL,
    AMHW_ZLG237_GPIO_SPEED_50MHz = 3UL
}amhw_zlg237_gpio_speed_mode_t;

/**
 * \brief GPIO引脚模式
 *
 * \note 引脚作为输入与输出时，具有不同的模式
 */
typedef enum amhw_zlg237_gpiomode
{
    /** \brief 当GPIO引脚的方向为输入方向，引脚具有以下几种模式 */
    AMHW_ZLG237_GPIO_MODE_AIN = 0x00,         /**< \brief 模拟模式 */
    AMHW_ZLG237_GPIO_MODE_IN_FLOATING = 0x01, /**< \brief 浮空模式(该模式仅限定于输入) */
    AMHW_ZLG237_GPIO_MODE_IPD = 0x02,         /**< \brief 下拉模式(该模式仅限定于输入) */
    AMHW_ZLG237_GPIO_MODE_IPU = 0x02,         /**< \brief 上拉模式 (该模式仅限定于输入) */

    /** \brief 当GPIO引脚的方向为输出方向，引脚具有以下几种模式 */
    AMHW_ZLG237_GPIO_MODE_OUT_PP = 0x00,      /**< \brief 通用推挽模式 (该模式仅限定于输出) */
    AMHW_ZLG237_GPIO_MODE_OUT_OD = 0x01,      /**< \brief 通用开漏模式 (该模式仅限定于输出) */
    AMHW_ZLG237_GPIO_MODE_AF_PP = 0x02,       /**< \brief 复用推免模式(该模式仅限定于输出) */
    AMHW_ZLG237_GPIO_MODE_AF_OD = 0x03        /**< \brief 复用开漏模式(该模式仅限定于输出) */

}amhw_zlg237_gpiomode_t;

/**
 * \brief 设置GPIO引脚方向为输出及引脚驱动电路响应速度
 *
 * \param[in] p_hw_gpio  : 指向GPIO寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIOA_0)
 * \param[in] speed_mode : 引脚为输现时，其驱动电路响应速度模式，
 *                         值为 amhw_zlg237_gpio_speed_mode_t 这个枚举类型
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_gpio_pin_dir_output (amhw_zlg237_gpio_t            *p_hw_gpio,
                                      amhw_zlg237_gpio_speed_mode_t  speed_mode,
                                      int                            pin)
{
    if (pin & 0x8) {
        p_hw_gpio->gpio[pin >> 4].crh &= ~(0x3 << ((pin & 0x7) << 2));
        p_hw_gpio->gpio[pin >> 4].crh |= (speed_mode & 0x3) << ((pin & 0x7) << 2);
    } else {
        p_hw_gpio->gpio[pin >> 4].crl &= ~(0x3 << ((pin & 0x7) << 2));
        p_hw_gpio->gpio[pin >> 4].crl |= (speed_mode & 0x3) << ((pin & 0x7) << 2);
    }
}

/**
 * \brief 设置GPIO引脚方向为输入
 *
 * \param[in] p_hw_gpio  : 指向GPIO寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_gpio_pin_dir_input (amhw_zlg237_gpio_t *p_hw_gpio, int pin)
{
    if (pin & 0x8) {
        p_hw_gpio->gpio[pin >> 4].crh &= ~((0x3) << ((pin & 0x7) << 2));
    } else {
        p_hw_gpio->gpio[pin >> 4].crl &= ~((0x3) << ((pin & 0x7) << 2));
    }
}

/**
 * \brief 获取GPIO引脚输入输出方向
 *
 * \param[in] p_hw_gpio  : 指向GPIO寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 0 ： 输入
 *          1 ： 输出
 */
am_static_inline
int amhw_zlg237_gpio_pin_dir_get (amhw_zlg237_gpio_t *p_hw_gpio, int pin)
{
    if (pin & 0x8) {
        return (p_hw_gpio->gpio[pin >> 4].crh & (0x3 << ((pin & 0x7) << 2))) ? 1 : 0;
    } else {
        return (p_hw_gpio->gpio[pin >> 4].crl & (0x3 << ((pin & 0x7) << 2))) ? 1 : 0;
    }
}

/**
 * \brief 设置GPIO引脚的模式
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] gpio_mode : 引脚的模式，值为 amhw_zlg237_gpiomode_t 这个枚举类型
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_gpio_pin_mode_set (amhw_zlg237_gpio_t     *p_hw_gpio,
                                    amhw_zlg237_gpiomode_t  gpio_mode,
                                    int                     pin)
{
    if (pin & 0x8) {
        p_hw_gpio->gpio[pin >> 4].crh &= ~(0x3 << (((pin & 0x7) << 2) + 2));
        p_hw_gpio->gpio[pin >> 4].crh |= ((gpio_mode & 0x3) << (((pin & 0x7) << 2) + 2));
    } else {
        p_hw_gpio->gpio[pin >> 4].crl &= ~(0x3 << (((pin & 0x7) << 2) + 2));
        p_hw_gpio->gpio[pin >> 4].crl |= ((gpio_mode & 0x3) << (((pin & 0x7) << 2) + 2));
    }
}

/**
 * \brief 读GPIO引脚输入电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 低电平
 * \retval 1 : 高电平
 */
am_static_inline
int amhw_zlg237_gpio_pin_input_get(amhw_zlg237_gpio_t *p_hw_gpio, int pin)
{
    return ((p_hw_gpio->gpio[pin >> 4].idr & (0x1 << (pin & 0xf))) ? 1 : 0);
}

/**
 * \brief 读GPIO引脚输出电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \retval 0 : 低电平
 * \retval 1 : 高电平
 */
am_static_inline
int amhw_zlg237_gpio_pin_output_get (amhw_zlg237_gpio_t *p_hw_gpio, int pin)
{
    return ((p_hw_gpio->gpio[pin >> 4].odr & (0x1 << (pin & 0xf))) ? 1 : 0);
}

/**
 * \brief 设置GPIO引脚电平为高
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 *
 * \note 该功能配置输入引脚的时候为使能上拉电阻并失能下拉电阻
 */
am_static_inline
void amhw_zlg237_gpio_pin_out_high (amhw_zlg237_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin >> 4].bsrr |= (0x1 << (pin & 0xf));
}

/**
 * \brief 设置GPIO引脚电平为低
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 *
 * \note 该功能配置输入引脚的时候为失能上拉电阻并使能下拉电阻
 */
am_static_inline
void amhw_zlg237_gpio_pin_out_low (amhw_zlg237_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin >> 4].bsrr |= ((0x1 << (pin & 0xf)) << 16);
}

/**
 * \brief 翻转GPIO引脚电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号,值为 PIO* (#PIOA_0)
 *
 * \return 无
 *
 * \note 该功能配置输入引脚的时候为配置上拉/下拉电阻
 */
am_static_inline
void amhw_zlg237_gpio_pin_out_toggle (amhw_zlg237_gpio_t *p_hw_gpio, int pin)
{
    if (amhw_zlg237_gpio_pin_output_get(p_hw_gpio, pin)) {
        amhw_zlg237_gpio_pin_out_low(p_hw_gpio, pin);
    } else {
        amhw_zlg237_gpio_pin_out_high(p_hw_gpio, pin);
    }
}

/**
 * \brief 设置GPIO引脚锁定
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \note 该函数调用多次锁定同一GPIO端口不同引脚的配置
 *
 * \return 无
 */
am_static_inline
void amhw_zlg237_gpio_pinlock_set(amhw_zlg237_gpio_t *p_hw_gpio, int pin)
{
    p_hw_gpio->gpio[pin >> 4].lckr |= (0x1 << (pin & 0xf));
}

/**
 * \brief GPIO端口锁键设置
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] port_num  : 端口编号，值为 PIO* (# PIOA)
 *
 * \note 该函数一般 在amhw_zlg237_gpio_pinlock_set调用后紧接着调用
 *
 * \return AM_OK    : 激活
 *         AM_ERROR : 失败
 */
am_static_inline
int amhw_zlg237_gpio_lckk_set(amhw_zlg237_gpio_t *p_hw_gpio, int port_num)
{
    uint32_t temp = (1UL << 16);

    /* Set LCKK bit */
    p_hw_gpio->gpio[port_num >> 4].lckr |= temp;
    /* Reset LCKK bit */
    p_hw_gpio->gpio[port_num >> 4].lckr &= ~temp;
    /* Set LCKK bit */
    p_hw_gpio->gpio[port_num >> 4].lckr |= temp;
    /* Read LCKK bit */
    temp = p_hw_gpio->gpio[port_num >> 4].lckr;
    /* Read LCKK bit */
    if (p_hw_gpio->gpio[port_num >> 4].lckr & (0x1 << 16)) {
        return AM_OK;
    } else {
        return AM_ERROR;
    }
}

/**
 * @} amhw_zlg237_if_gpio
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZLG237_GPIO_H */

/* end of file */
