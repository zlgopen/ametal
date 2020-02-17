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
 * \brief GPIO 硬件操作接口
 *
 * 1. GPIO 引脚能够通过软件配置为输入输出；
 * 2. 所有GPIO 引脚默认为输入，在复位时中断禁能；
 * 3. 引脚寄存器允许引脚单独配置或多个同时配置。
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-07  nwt, first implementation
 * \endinternal
 */

#ifndef __AMHW_MM32_GPIO_H
#define __AMHW_MM32_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_mm32_if_gpio
 * \copydoc amhw_mm32_gpio.h
 * @{
 */

/**
 * \name GPIO 端口编号
 * @{
 */

#define AMHW_MM32_GPIO_PORT_A    0        /**< \brief 端口 A */
#define AMHW_MM32_GPIO_PORT_B    16       /**< \brief 端口 B */
#define AMHW_MM32_GPIO_PORT_C    32       /**< \brief 端口 C */
#define AMHW_MM32_GPIO_PORT_D    48       /**< \brief 端口 D */

/** @} */

/**
 * \name AF selection
 * @{
 */
 
/**
 * \brief AF 0 selection
 * SPI1, MC0, TIM17_BKIN, SWDIO,SWCLK, UART1
 */
#define AMHW_MM32_GPIO_AF_0       ((uint8_t)0x00)

/**
 * \brief AF 1 selection
 * UART1, TIM3_CH1, TIM3_CH2, TIM3_CH3,TIM3_CH4, I2C1
 */
#define AMHW_MM32_GPIO_AF_1       ((uint8_t)0x01)

/**
 * \brief AF 2 selection
 * TIM2_CH1_ETR, TIM2_CH2, TIM2_CH3, TIM2_CH3, TIM2_CH4, TIM1_BKIN,
 * TIM1_CH1N, TIM1_CH1, TIM1_CH2,TIM1_CH3, TIM1_CH4, TIM1_ETR,
 * TIM1_CH2N, TIM1_CH3N, TIM2_CH2,TIM1 6_BKIN, TIM16_CH1N, TIM17_CH1N,
 * TIM16_CH1, TIM17_CH1
 */
#define AMHW_MM32_GPIO_AF_2       ((uint8_t)0x02)

/** \brief AF 3 selection */
#define AMHW_MM32_GPIO_AF_3       ((uint8_t)0x03)

/** \brief AF 4 selection, TIM14_CH1, I2C1  */
#define AMHW_MM32_GPIO_AF_4       ((uint8_t)0x04)

/** \brief AF 5 selection */
#define AMHW_MM32_GPIO_AF_5       ((uint8_t)0x05)

/** \brief AF 6 selection */
#define AMHW_MM32_GPIO_AF_6       ((uint8_t)0x06)

/** \brief AF 7 selection */
#define AMHW_MM32_GPIO_AF_7       ((uint8_t)0x07)

/** \brief AF default selection, 引脚没有复用(默认GPIO)  */
#define AMHW_MM32_GPIO_AF_DEFAULT ((uint8_t)0x0f)

/** @} */
 
/**
 * \brief 管脚复用范围检测
 */
#define AMHW_MM32_GPIO_IS_AF(af) (((af) == AMHW_MM32_GPIO_AF_0) || \
                                 ((af) == AMHW_MM32_GPIO_AF_1) || \
                                 ((af) == AMHW_MM32_GPIO_AF_2) || \
                                 ((af) == AMHW_MM32_GPIO_AF_3) || \
                                 ((af) == AMHW_MM32_GPIO_AF_4) || \
                                 ((af) == AMHW_MM32_GPIO_AF_5) || \
                                 ((af) == AMHW_MM32_GPIO_AF_6) || \
                                 ((af) == AMHW_MM32_GPIO_AF_7))

/**
 * \name GPIO 引脚电平
 * @{
 */
#define AMHW_MM32_GPIO_LEVEL_LOW     0        /**< \brief 低电平 */
#define AMHW_MM32_GPIO_LEVEL_HIGH    1        /**< \brief 高电平 */
/** @} */

/* 使用无名结构体和联合体区域的开始 */
#if defined(__CC_ARM)
    #pragma push
    #pragma anon_unions
#elif defined(__ICCARM__)
    #pragma language=extended
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
    #pragma warning 586
#else
    #warning Not supported compiler t
#endif

/**
 * \brief gpio引脚中断触发方式定义
 */
typedef enum amhw_mm32_gpio_trig_mode {
    AMHW_MM32_PINT_TRIGGER_HIGH       = 0x0,   /**< \brief 高电平触发 */
    AMHW_MM32_PINT_TRIGGER_LOW        = 0x1,   /**< \brief 低电平触发 */
    AMHW_MM32_PINT_TRIGGER_RISE       = 0x2,   /**< \brief 上升沿触发 */
    AMHW_MM32_PINT_TRIGGER_FALL       = 0x3,   /**< \brief 下降沿触发 */
    AMHW_MM32_PINT_TRIGGER_BOTH_EDGES = 0x4    /**< \brief 双边沿触发 */
} amhw_mm32_pin_trig_mode_t;

/**
 * \brief GPIO引脚驱动电路响应速度
 * \note 选择该枚举设置GPIO引脚驱动电路响应速度时，GPIO引脚也确定为输出方向
 */
typedef enum amhw_mm32_gpio_speed_mode {
    AMHW_MM32_GPIO_SPEED_10MHz = 3UL,
    AMHW_MM32_GPIO_SPEED_20MHz = 2UL,
    AMHW_MM32_GPIO_SPEED_50MHz = 1UL
}amhw_mm32_gpio_speed_mode_t;

/**
 * \brief GPIO引脚模式
 *
 * \note 引脚作为输入与输出时，具有不同的模式
 */
typedef enum amhw_mm32_gpio_mode {
    
    /** \brief 当GPIO引脚的方向为输入方向，引脚具有以下几种模式 */
    AMHW_MM32_GPIO_MODE_AIN = 0x00,         /**< \brief 模拟输入模式     */
    AMHW_MM32_GPIO_MODE_IN_FLOATING = 0x01, /**< \brief 浮空输入模式     */
    AMHW_MM32_GPIO_MODE_IPD = 0x02,         /**< \brief 下拉输入模式     */
    AMHW_MM32_GPIO_MODE_IPU = 0x02,         /**< \brief 上拉输入模式     */

    /** \brief 当GPIO引脚的方向为输出方向，引脚具有以下几种模式 */
    AMHW_MM32_GPIO_MODE_OUT_PP = 0x00,      /**< \brief 通用推挽输出模式 */
    AMHW_MM32_GPIO_MODE_OUT_OD = 0x01,      /**< \brief 通用开漏输出模式 */
    AMHW_MM32_GPIO_MODE_AF_PP  = 0x02,      /**< \brief 复用推挽输出模式 */
    AMHW_MM32_GPIO_MODE_AF_OD  = 0x03       /**< \brief 复用开漏输出模式 */

} amhw_mm32_gpio_mode_t;

/**
 * \brief GPIO 寄存器块结构体
 */
typedef struct amhw_mm32_gpio {
    __IO uint32_t cr[2];       /**< \brief 端口配置(高低)寄存器 */
    __I  uint32_t idr;         /**< \brief 端口输入数据寄存器 */
    __IO uint32_t odr;         /**< \brief 端口输出寄存器 */
    __O  uint32_t bsrr;        /**< \brief 端口设置/清除寄存器 */
    __O  uint32_t brr;         /**< \brief 端口位清除寄存器 */
    __IO uint32_t lckr;        /**< \brief 端口配置锁定寄存器 */
    __I  uint32_t reserve;     /**< \brief 保留 */
    __IO uint32_t afr[2];      /**< \brief 端口复用功能(高低)寄存器 */
} amhw_mm32_gpio_t;

/**
 * \brief GPIO引脚相关设置
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] flags     : 配置参数
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_mm32_gpio_pin_set (amhw_mm32_gpio_t *p_hw_gpio, int flags, int pin)
{
    /* 取得当前引脚所属GPIO端口的基地址 */
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

    /*
     * pin & 0x0f 这样运算取得了相应端口引脚的余数，因为一个GPIO端口只有16个引脚
     * eg : 以芯片17号引脚为例：0x11 & 0x0f = 0x01, 代表是GPIO端口B第1个引脚
     *
     * ((pin & 0x0f) >> 3) 这样写的目的在于当一个GPIO端口的低8个引脚0 ~ 7，它的
     * 控制寄存器为cr[0], 高8个引脚8 ~ 15， 它的控制寄存器为cr[1]
     * eg：以芯片27号 引脚为例：((0x1B & 0x0f) >> 3) = 0x01, 代表是GPIO端口B第
     *     11个引脚的控制寄存器为cr[1]
     *
     * 4 * ((pin & 0x0f) & 0x07) 这样运算取得了该引脚是属于哪个位段，
     * 7 ~ 0 引脚分别属于cr[0]的位 [31：28]、[27：24]、[23：20]、[19：16]、
     *                             [15：12]、[11：8]、 [7：4]、  [3：0]
     *
     * 15 ~ 8引脚分别属于cr[1]的位 [31：28]、[27：24]、[23：20]、[19：16]、
     *                             [15：12]、[11：8]、 [7：4]、  [3：0]
     */

    /* 设置每个引脚模式相关位段 */
    AM_BITS_SET(p_gpio_addr->cr[((pin & 0x0f) >> 3)],
                (4 * ((pin & 0x0f) & 0x07)), 
                4, 
                flags);
}

/**
 * \brief 设置GPIO引脚方向为输出及引脚驱动电路响应速度
 *
 * \param[in] p_hw_gpio  : 指向GPIO寄存器块的指针
 * \param[in] pin        : 引脚编号，值为 PIO* (#PIOA_0)
 * \param[in] speed_mode : 引脚为输现时，其驱动电路响应速度模式，
 *                         值为 amhw_mm32_gpio_speed_mode_t 这个枚举类型
 *
 * \return 无
 */
am_static_inline
void amhw_mm32_gpio_pin_dir_output (amhw_mm32_gpio_t           *p_hw_gpio,
                                   amhw_mm32_gpio_speed_mode_t speed_mode,
                                   int                        pin)
{
    /* 取得当前引脚所属GPIO端口的基地址 */
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

    /*
     * pin & 0x0f 这样运算取得了相应端口引脚的余数，因为一个GPIO端口只有16个引脚
     * eg : 以芯片17号引脚为例：0x11 & 0x0f = 0x01, 代表是GPIO端口B第1个引脚
     *
     * ((pin & 0x0f) >> 3) 这样写的目 的在于当一个GPIO端口的低8个引脚0 ~ 7，它的控制寄
     * 存器为cr[0], 高8个引脚8 ~ 15， 它的控制寄存器为cr[1]
     * eg：以芯片27号 引脚为例：((0x1B & 0x0f) >> 3) = 0x01, 代表是GPIO端口B第11个引脚的
     *     控制寄存器为cr[1]
     *
     * 4 * ((pin & 0x0f) & 0x07) 这样运算取得了该引脚是属于哪个位段，
     * 7 ~ 0 引脚属于cr[0]的位 29：28 位 25：24 位 21：20 位 17：16 位 13：12位 9：8位 5：4 位 1：0
     * 15 ~ 8引脚属于cr[1]的位 29：28 位 25：24 位 21：20 位 17：16 位 13：12位 9：8位 5：4 位 1：0
     *
     */

    /* 设置引脚方向为输出相关位段, 调用改接口不会影响其它位段 */
    AM_BITS_SET(p_gpio_addr->cr[((pin & 0x0f) >> 3)], (4 * ((pin & 0x0f) & 0x07)), 2, speed_mode);
}

/**
 * \brief 设置GPIO引脚方向为输入
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_mm32_gpio_pin_dir_input (amhw_mm32_gpio_t *p_hw_gpio, int pin)
{
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

    /* 设置引脚方向为输出相关位段, 调用改接口不会影响其它位段 */
    AM_BITS_SET(p_gpio_addr->cr[((pin & 0x0f) >> 3)], (4 * ((pin & 0x0f) & 0x07)), 2, 0x00);
}

/**
 * \brief 设置GPIO引脚的模式
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] gpio_mode : 引脚的模式，值为 amhw_mm32_gpiomode_t 这个枚举类型
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_mm32_gpio_pin_mode_set (amhw_mm32_gpio_t       *p_hw_gpio,
                                 amhw_mm32_gpio_mode_t   gpio_mode,
                                 int                    pin)
{
    /* 取得当前引脚所属GPIO端口的基地址 */
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

    /*
     * pin & 0x0f 这样运算取得了相应端口引脚的余数，因为一个GPIO端口只有16个引脚
     * eg : 以芯片17号引脚为例：0x11 & 0x0f = 0x01, 代表是GPIO端口B第1个引脚
     *
     * ((pin & 0x0f) >> 3) 这样写的目 的在于当一个GPIO端口的低8个引脚0 ~ 7，它的控制寄
     * 存器为cr[0], 高8个引脚8 ~ 15， 它的控制寄存器为cr[1]
     * eg：以芯片27号 引脚为例：((0x1B & 0x0f) >> 3) = 0x01, 代表是GPIO端口B第11个引脚的
     *     控制寄存器为cr[1]
     *
     * 4 * ((pin & 0x0f) & 0x07) 这样运算取得了该引脚是属于哪个位段，
     * 7 ~ 0 引脚属于cr[0]的位 29：28 位 25：24 位 21：20 位 17：16 位 13：12位 9：8位 5：4 位 1：0
     * 15 ~ 8引脚属于cr[1]的位 29：28 位 25：24 位 21：20 位 17：16 位 13：12位 9：8位 5：4 位 1：0
     *
     */

    /* 设置每个引脚模式相关位段 */
    AM_BITS_SET(p_gpio_addr->cr[((pin & 0x0f) >> 3)], (4 * ((pin & 0x0f) & 0x07) + 2), 2, gpio_mode);
}

/**
 * \brief 获取指定引脚的方向
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \retval  0  : 输入
 * \retval  1  : 输出
 */
am_static_inline
int amhw_mm32_gpio_pin_dir_get (amhw_mm32_gpio_t *p_hw_gpio, int pin)
{
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

    return (((AM_BITS_GET(p_gpio_addr->cr[((pin & 0x0f) >> 3)], \
            (4 * ((pin & 0x0f) & 0x07)), 2)) != 0) ? 1 : 0);
}

/**
 * \brief 设置GPIO引脚方向为输入
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 返回GPIO的引脚模式
 */
am_static_inline
int amhw_mm32_gpio_pin_mode_get (amhw_mm32_gpio_t *p_hw_gpio, int pin)
{
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

    /* 设置引脚方向为输出相关位段, 调用改接口不会影响其它位段 */
    return AM_BITS_GET(p_gpio_addr->cr[((pin & 0x0f) >> 3)], (4 * ((pin & 0x0f) & 0x07) + 2), 2);
}

/**
 * \brief 获取引脚的电平状态
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 *\note 在引脚方向为输出，且引脚为开漏模式时，对输入数据寄存器的访问可得到 I/O 状态
 *
 * \retval  0  : 低电平
 * \retval  1  : 高电平
 */
am_static_inline
int amhw_mm32_gpio_pin_get (amhw_mm32_gpio_t *p_hw_gpio, int pin)
{
    int gpio_mode = 0;

    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

    /* 当为推挽输出时，获取引脚的状态可以通过读取ODR寄存器的值来实现 */
    if (amhw_mm32_gpio_pin_dir_get(p_hw_gpio, pin) != 0) {

        gpio_mode = amhw_mm32_gpio_pin_mode_get(p_hw_gpio, pin);

        if (gpio_mode == AMHW_MM32_GPIO_MODE_OUT_PP ||
            gpio_mode == AMHW_MM32_GPIO_MODE_OUT_PP ) {
            return (((p_gpio_addr->odr & (1UL << (pin & 0x0f))) != 0) ? 1 : 0);
        } else {
            return (((p_gpio_addr->idr & (1UL << (pin & 0x0f))) != 0) ? 1 : 0);
        }
    } else {
        return (((p_gpio_addr->idr & (1UL << (pin & 0x0f))) != 0) ? 1 : 0);
    }
}

/**
 * \brief 设置GPIO引脚输出高电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无.
 *
 * \note 该功能配置输入引脚的时候为使能上拉电阻并失能下拉电阻
 */
am_static_inline
void amhw_mm32_gpio_pin_out_high (amhw_mm32_gpio_t *p_hw_gpio, int pin)
{
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

     /* 相应引脚输出高电平 */
    p_gpio_addr->bsrr|= (1UL << (pin & 0x0f));
}

/**
 * \brief 设置GPIO引脚输出低电平
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 *
 * \note 该功能配置输入引脚的时候为失能上拉电阻并使能下拉电阻
 */
am_static_inline
void amhw_mm32_gpio_pin_out_low (amhw_mm32_gpio_t *p_hw_gpio, int pin)
{
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

    p_gpio_addr->brr |= (1UL << (pin & 0x0f));
}

/**
 * \brief 翻转GPIO输出引脚的电平状态
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 *
 * \note 该功能配置输入引脚的时候为配置上拉/下拉电阻
 */
am_static_inline
void amhw_mm32_gpio_pin_toggle (amhw_mm32_gpio_t *p_hw_gpio, int pin)
{
    if (amhw_mm32_gpio_pin_get(p_hw_gpio,pin)) {
        amhw_mm32_gpio_pin_out_low(p_hw_gpio,pin);
    } else {
        amhw_mm32_gpio_pin_out_high(p_hw_gpio,pin);
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
void amhw_mm32_gpio_pinlock_set (amhw_mm32_gpio_t *p_hw_gpio, int pin)
{
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);
    p_gpio_addr->lckr |= (1UL << (pin & 0x0f));
}

/**
 * \brief GPIO端口锁键设置
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] port_num  : 端口编号，值为 PIO* (# AMHW_GPIO_PORT_A)
 *
 * \note 该函数一般 在amhw_mm32_gpio_pinlock_set调用后紧接着调用
 *
 * \return 无
 */
am_static_inline
void amhw_mm32_gpio_lckk_set (amhw_mm32_gpio_t *p_hw_gpio, int port_num)
{
    uint32_t temp = (1UL << 16);

    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (port_num >> 4) * 0x400);

    /* Set LCKK bit */
    p_gpio_addr->lckr |= temp;
    /* Reset LCKK bit */
    p_gpio_addr->lckr &= ~temp;
    /* Set LCKK bit */
    p_gpio_addr->lckr |= temp;
    /* Read LCKK bit */
    temp =  p_gpio_addr->lckr;
    /* Read LCKK bit */
    temp =  p_gpio_addr->lckr;
}

/**
 * \brief 引脚复用功能重映射设置
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] gpio_af   : 引脚复用功能编号，值为 AMHW_GPIO_AF_* 这一类宏 (# AMHW_GPIO_AF_0)
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
am_static_inline
void amhw_mm32_gpio_pin_afr_set (amhw_mm32_gpio_t *p_hw_gpio, int gpio_af, int pin)
{
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

    /* 设置引脚复用功能的相关位段, 调用改接口不会影响其它位段 */
    AM_BITS_SET(p_gpio_addr->afr[((pin & 0x0f) >> 3)], (4 * ((pin & 0x0f) & 0x07)), 4, gpio_af);
}

/**
 * \brief 获取当前引脚复用功能码AF
 *
 * \param[in] p_hw_gpio : 指向GPIO寄存器块的指针
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 返回 AMHW_GPIO_AF_* 这一类宏值 (# AMHW_GPIO_AF_0)
 */
am_static_inline
uint32_t amhw_mm32_gpio_pin_afr_get (amhw_mm32_gpio_t *p_hw_gpio, int pin)
{
    amhw_mm32_gpio_t *p_gpio_addr = (amhw_mm32_gpio_t *)((int)p_hw_gpio \
                                      + (pin >> 4) * 0x400);

    /* 设置引脚方向为输出相关位段, 调用改接口不会影响其它位段 */
    return AM_BITS_GET(p_gpio_addr->afr[((pin & 0x0f) >> 3)], (4 * ((pin & 0x0f) & 0x07)), 4);
}

/**
 * @}
 */

/* 使用无名结构体和联合体区域的结束 */
#if defined(__CC_ARM)
    #pragma pop
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
    #pragma warning restore
#else
    #warning Not supported compiler t
#endif

/**
 * @} amhw_mm32_if_mm32_gpio
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_MM32_GPIO_H */

/* end of file */
