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
 * \brief AFIO hardware operation interface
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-3  zc, first implementation
 * \endinternal
 */

#ifndef __AMHW_STM32F103RBT6_AFIO_H
#define __AMHW_STM32F103RBT6_AFIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"

/**
 * \addtogroup amhw_stm32f103rbt6_if_afio
 * \copydoc amhw_stm32f103rbt6_afio.h
 * @{
 */

/**
 * \brief GPIO 端口寄存器结构体
 */
typedef struct amhw_stm32f103rbt6_afio {
    __IO  uint32_t evcr;         /**< \brief 事件控制寄存器 */
    __IO  uint32_t mapr;         /**< \brief 复用重映射和调试I/O配置寄存器 */
    __IO  uint32_t exticr[4];    /**< \brief 外部中断配置寄存器 */
} amhw_stm32f103rbt6_afio_t;

/**
 * \brief 可重映像的外设编号
 */
typedef enum {
    AMHW_STM32F103RBT6_SPI1_REMAP     = 0,     /**< \brief SPI1 */
    AMHW_STM32F103RBT6_I2C1_REMAP,             /**< \brief I2C1 */
    AMHW_STM32F103RBT6_UART1_REMAP,            /**< \brief UART1 */
    AMHW_STM32F103RBT6_UART2_REMAP,            /**< \brief UART2 */
    AMHW_STM32F103RBT6_UART3_REMAP,            /**< \brief UART3 */
    AMHW_STM32F103RBT6_TIM1_REMAP     = 6,     /**< \brief TIM1 */
    AMHW_STM32F103RBT6_TIM2_REMAP     = 8,     /**< \brief TIM2 */
    AMHW_STM32F103RBT6_TIM3_REMAP     = 10,    /**< \brief TIM3 */
    AMHW_STM32F103RBT6_CAN_REMAP      = 13,    /**< \brief CAN */
    AMHW_STM32F103RBT6_TIM4_REMAP     = 12,    /**< \brief TIM4 */
    AMHW_STM32F103RBT6_PD01_REMAP     = 15,    /**< \brief PD01 */
    AMHW_STM32F103RBT6_TIM5_CH4_REMAP,    /**< \brief TIM5_CH4 */
    AMHW_STM32F103RBT6_ADC1_ETRGINJ_REMAP,    /**< \brief ADC1_ETRGINJ */
    AMHW_STM32F103RBT6_ADC1_ETRGREG_REMAP,    /**< \brief ADC1_ETRGREG */
    AMHW_STM32F103RBT6_ADC2_ETRGINJ_REMAP,    /**< \brief ADC2_ETRGINJ */
    AMHW_STM32F103RBT6_ADC2_ETRGREG_REMAP,    /**< \brief ADC2_ETRGREG */
    AMHW_STM32F103RBT6_SWJ_CFG        = 24,                /**< \brief SWJ_CFG */
    AMHW_STM32F103RBT6_NO_REMAP,               /**< \brief 没有重映像功能 */
}amhw_stm32f103rbt6_afio_remap_peripheral_t;

/**
 * \name  重映像位定义
 * @{
 */
/** \brief 引脚选择  选择用于输出Cortex的EVENTOUT信号的引脚 */
#define AMHW_STM32F103RBT6_AFIO_EVCR_PIN_EVENTOUT     (0u)

/** \brief 端口选择  选择用于输出Cortex的EVENTOUT信号的端口 */
#define AMHW_STM32F103RBT6_AFIO_EVCR_PORT_EVENTOUT     (4u)

/** \brief EVOE 允许事件输出位 Cortex的EVENTOUT将连接PORT[2:0] 和 PIN[3:0]选定的I/O */
#define AMHW_STM32F103RBT6_AFIO_EVCR_EVOE     (7u)

/** \brief SPI1重映像 SCK/PB3，MISO/PB4，MOSI/PB5 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_SPI1    (0u)

/** \brief I2C1重映像 SCL/PB8，SDA/PB9 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_I2C1    (1u)

/** \brief UART1重映像 TX/PB6，RX/PB7 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_UART1   (2u)

/** \brief UART2重映像
 *  CTS/PD3，RTS/PD4，
 *  TX/PD5，RX/PD6，
 *  CK/PD7
 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_UART2   (3u)

/** \brief UART3重映像 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_UART3   (4u)

typedef enum {

    /** \brief TX/PB10,RX/PB11,CK/PB12,CST/PB13,RTS/PB14 */
    AMHW_STM32F103RBT6_AFIO_UART3_REMAP_NO  = 0,
    /** \brief TX/PC10,RX/PC11,CK/PC12,CST/PB13,RTS/PB14 */
    AMHW_STM32F103RBT6_AFIO_UART3_REMAP_PART1,
    /** \brief TX/PD8,RX/PD9,CK/PD10,CST/PD11,RTS/PD12 */
    AMHW_STM32F103RBT6_AFIO_UART3_REMAP_ALL = 3,
}amhw_stm32f103rbt6_afio_uart3_remap_mode_t;

/** \brief
 * TIM1部分重映像
 * ETR/PA12，
 * CH1/PA8，CH2/PA9，CH3/PA10，CH4/PA11，
 * BAKIN/PA6，
 * CH1N/PA7，CH2N/PB0，CH3N/PB1
 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_TIM1    (6u)

typedef enum {

    /** \brief ETR/PA12，CH1/PA8，CH2/PA9，CH3/PA10，
     * CH4/PA11，BKIN/PB12，CH1N/PB13，CH2N/PB14，CH3N/PB15 */
    AMHW_STM32F103RBT6_AFIO_TIM1_REMAP_NO  = 0,
    /** \brief ETR/PA12，CH1/PA8，CH2/PA9，CH3/PA10，
     * CH4/PA11，BKIN/PA6，CH1N/PA7，CH2N/PB0，CH3N/PB1 */
    AMHW_STM32F103RBT6_AFIO_TIM1_REMAP_PART1,
    /** \brief ETR/PE7，CH1/PE9，CH2/PE11，CH3/PE13，
     * CH4/PE14，BKIN/PE15，CH1N/PE8，CH2N/PE10，CH3N/PE12 */
    AMHW_STM32F103RBT6_AFIO_TIM1_REMAP_ALL = 3,
}amhw_stm32f103rbt6_afio_tim1_remap_mode_t;




/** \brief TIM2重映像 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_TIM2    (8u)

typedef enum {

    /** \brief CH1/ETR/PA0，CH2/PA1，CH3/PA2，CH4/PA3 */
    AMHW_STM32F103RBT6_AFIO_TIM2_REMAP_NO = 0,
    /** \brief CH1/ETR/PA15，CH2/PB3，CH3/PA2，CH4/PA3 */
    AMHW_STM32F103RBT6_AFIO_TIM2_REMAP_PART1,
    /** \brief CH1/ETR/PA0，CH2/PA1，CH3/PB10，CH4/PB11 */
    AMHW_STM32F103RBT6_AFIO_TIM2_REMAP_PART2,
    /** \brief CH1/ETR/PA15，CH2/PB3，CH3/PB10，CH4/PB11 */
    AMHW_STM32F103RBT6_AFIO_TIM2_REMAP_ALL
}amhw_stm32f103rbt6_afio_tim2_remap_mode_t;

/** \brief TIM3重映像 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_TIM3    (10u)

typedef enum {

    /** \brief CH1/PA6，CH2/PA7，CH3/PB0，CH4/PB1 */
    AMHW_STM32F103RBT6_AFIO_TIM3_REMAP_NO = 0,
    /** \brief CH1/PB4，CH2/PB5，CH3/PB0，CH4/PB1 */
    AMHW_STM32F103RBT6_AFIO_TIM3_REMAP_PART = 2,
    /** \brief CH1/PC6，CH2/PC7，CH3/PC8，CH4/PC9 */
    AMHW_STM32F103RBT6_AFIO_TIM3_REMAP_ALL
}amhw_stm32f103rbt6_afio_tim3_remap_mode_t;

/** \brief TIM4完全重映像
 * CH1/PD12，CH2/PD13，CH3/PD14，CH4/PD15
 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_TIM4    (12u)

/** \brief CAN重映像 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_CAN     (13u)

typedef enum {
    AMHW_STM32F103RBT6_AFIO_CAN_RX_PA11_TX_PA12 = 0,
    AMHW_STM32F103RBT6_AFIO_CAN_RX_PB8_TX_PB9   = 2,
    AMHW_STM32F103RBT6_AFIO_CAN_RX_PD0_TX_PD1   = 3,
}amhw_stm32f103rbt6_afio_can_remap_mode_t;

/** \brief PD01重映像 PD0映像到OSC_IN，PD1映像到OSC_OUT */
#define AMHW_STM32F103RBT6_AFIO_MAPR_PD01    (15u)

/** \brief TIM5通道4 内部重映射
 * 0 TIM5_CH4/PA3
 * 1 TIM5_CH4/LSI内部振荡器
 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_TIM5_CH4    (16u)

/** \brief ADC1 注入转换外部触发重映射
 * 0 ADC1注入转换触发与EXTI15相连
 * 1 ADC1注入转换外部触发与TIM8_CH4相连
 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGINJ    (17u)

/** \brief ADC1 规则转换外部触发重映射
 * 0 ADC1规则转换触发与EXTI11相连
 * 1 ADC1规则转换外部触发与TIM8_TRGO相连
 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGREG    (18u)

/** \brief ADC2 注入转换外部触发重映射
 * 0 ADC2注入转换触发与EXTI15相连
 * 1 ADC2注入转换外部触发与TIM8_CH4相连
 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGINJ    (19u)

/** \brief ADC1 规则转换外部触发重映射
 * 0 ADC1规则转换触发与EXTI11相连
 * 1 ADC1规则转换外部触发与TIM8_TRGO相连
 */
#define AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGREG    (20u)


/** \brief 串行线JTAG配置 */
#define AMHW_STM32F103RBT6_AFIO_SWJ_CFG      (24u)

typedef enum {
    /**
     * \brief 完全SWJ（JTAG-DP + SW-DP）：复位状态
     *
     * PIOA13:I/0不可用   PIOA14:I/0不可用   PIOA15:I/0不可用
     * PIOB3 :I/0不可用   PIOB4 :I/0不可用
     */
    AMHW_STM32F103RBT6_AFIO_SWJ_CFG_0 = 0,

    /**
     * \brief 完全SWJ（JTAG-DP + SW-DP）但没有JNTRST
     *
     * PIOA13:I/0不可用   PIOA14:I/0不可用   PIOA15:I/0不可用
     * PIOB3 :I/0不可用   PIOB4 :I/0可用
     */
    AMHW_STM32F103RBT6_AFIO_SWJ_CFG_1,

    /**
     * \brief 关闭JTAG-DP，启动SW-DP
     *
     * PIOA13:I/0不可用   PIOA14:I/0不可用   PIOA15:I/0可用
     * PIOB3 :I/0可用   PIOB4 :I/0可用
     */
    AMHW_STM32F103RBT6_AFIO_SWJ_CFG_2,

    /**
     * \brief 关闭JTAG-DP，关闭SW-DP
     *
     * PIOA13:I/0可用   PIOA14:I/0可用   PIOA15:I/0可用
     * PIOB3 :I/0可用   PIOB4 :I/0可用
     */
    AMHW_STM32F103RBT6_AFIO_SWJ_CFG_4 = 4,
}amhw_stm32f103rbt6_afio_swj_cfg_mode_t;

/** @} */


/**
 * \brief eventout evcr事件输出引脚设置
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[in] pin
 * \param[in] stats  0 1 使能状态
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_evcr_mode_set (amhw_stm32f103rbt6_afio_t              *p_hw_afio,
                                     int                              pin,
                                     int                              stats)
{
    p_hw_afio->evcr |= (pin >> 4) << AMHW_STM32F103RBT6_AFIO_EVCR_PORT_EVENTOUT;
    p_hw_afio->evcr |= (pin & 0xf) << AMHW_STM32F103RBT6_AFIO_EVCR_PIN_EVENTOUT;
    p_hw_afio->evcr |= stats << AMHW_STM32F103RBT6_AFIO_EVCR_EVOE;
}


/**
 * \brief SPI1引脚重映像使能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_spi1_pin_remap_enable (amhw_stm32f103rbt6_afio_t *p_hw_afio)
{
    p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_SPI1;
}

/**
 * \brief SPI1引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_spi1_pin_remap_disable (amhw_stm32f103rbt6_afio_t *p_hw_afio)
{
    p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_SPI1);
}

/**
 * \brief I2C1引脚重映像使能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_i2c1_pin_remap_enable (amhw_stm32f103rbt6_afio_t *p_hw_afio)
{
    p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_I2C1;
}

/**
 * \brief I2C1引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_i2c1_pin_remap_disable (amhw_stm32f103rbt6_afio_t *p_hw_afio)
{
    p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_I2C1);
}

/**
 * \brief UART引脚重映像使能
 *
 * \notes : 不实用串口3时 mode 为 0
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[in] num       串口号：1,2,3
 * \param[in] mode      amhw_stm32f103rbt6_afio_uart3_remap_mode_t
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_uart_pin_remap_enable (amhw_stm32f103rbt6_afio_t *p_hw_afio,
                                             int                 num)
{

    if (num == 1) {
        p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_UART1;
    } else if (num == 2) {
        p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_UART2;
    }
}

/**
 * \brief UART引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[in] num       串口号：1,2,3
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_uart_pin_remap_disable (amhw_stm32f103rbt6_afio_t *p_hw_afio,
                                              int num)
{
    if (num == 1) {
        p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_UART1);
    } else if (num == 2) {
        p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_UART2);
    }
}

/**
 * \brief uart3引脚重映像模式设置
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[in] mode      重映设模式：amhw_stm32f103rbt6_afio_uart3_remap_mode_t枚举的宏
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_uart3_remap_mode_set (amhw_stm32f103rbt6_afio_t *p_hw_afio,
                                           amhw_stm32f103rbt6_afio_uart3_remap_mode_t mode)
{
     p_hw_afio->mapr &= ~(0x3<< AMHW_STM32F103RBT6_AFIO_MAPR_UART3);
     p_hw_afio->mapr |= (mode & 0x3) << AMHW_STM32F103RBT6_AFIO_MAPR_UART3;

}

/**
 * \brief TIM1引脚重映像模式设置
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[in] mode      重映设模式：amhw_stm32f103rbt6_afio_tim1_remap_mode枚举的宏
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_tim1_remap_mode_set (amhw_stm32f103rbt6_afio_t *p_hw_afio,
                                           amhw_stm32f103rbt6_afio_tim1_remap_mode_t mode)
{
    p_hw_afio->mapr &= ~(0x3<< AMHW_STM32F103RBT6_AFIO_MAPR_TIM1);
    p_hw_afio->mapr |= (mode & 0x3) << AMHW_STM32F103RBT6_AFIO_MAPR_TIM1;

}

/**
 * \brief TIM2引脚重映像模式设置
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[in] mode      重映设模式：amhw_stm32f103rbt6_afio_tim2_remap_mode枚举的宏
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_tim2_remap_mode_set (amhw_stm32f103rbt6_afio_t *p_hw_afio,
                                           amhw_stm32f103rbt6_afio_tim2_remap_mode_t mode)
{
    p_hw_afio->mapr &= ~(0x3 << AMHW_STM32F103RBT6_AFIO_MAPR_TIM2);
    p_hw_afio->mapr |= (mode & 0x3) << AMHW_STM32F103RBT6_AFIO_MAPR_TIM2;
}

/**
 * \brief TIM3引脚重映像模式设置
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[in] mode      重映设模式：amhw_stm32f103rbt6_afio_tim3_remap_mode枚举的宏
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_tim3_remap_mode_set (amhw_stm32f103rbt6_afio_t *p_hw_afio,
                                           amhw_stm32f103rbt6_afio_tim3_remap_mode_t mode)
{
    p_hw_afio->mapr &= ~(0x3 << AMHW_STM32F103RBT6_AFIO_MAPR_TIM3);
    p_hw_afio->mapr |= (mode & 0x3) << AMHW_STM32F103RBT6_AFIO_MAPR_TIM3;
}

/**
 * \brief TIM4引脚重映像使能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_tim4_pin_remap_enable (amhw_stm32f103rbt6_afio_t *p_hw_afio)
{
      p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_TIM4;
}

/**
 * \brief TIM4引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_tim4_pin_remap_disable (amhw_stm32f103rbt6_afio_t *p_hw_afio)
{
      p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_TIM4);
}

/**
 * \brief CAN引脚重映像模式设置
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[in] mode      重映设模式：amhw_stm32f103rbt6_afio_can_remap_mode枚举的宏
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_can_remap_mode_set (amhw_stm32f103rbt6_afio_t *p_hw_afio,
                                          amhw_stm32f103rbt6_afio_can_remap_mode_t mode)
{
    p_hw_afio->mapr &= ~(0x3 << AMHW_STM32F103RBT6_AFIO_MAPR_CAN);
    p_hw_afio->mapr |= (mode & 0x3) << AMHW_STM32F103RBT6_AFIO_MAPR_CAN;
}

/**
 * \brief PD01引脚重映像使能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_pd01_pin_remap_enable (amhw_stm32f103rbt6_afio_t *p_hw_afio)
{
    p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_PD01;
}

/**
 * \brief PD01引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_pd01_pin_remap_disable (amhw_stm32f103rbt6_afio_t *p_hw_afio)
{
    p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_PD01);
}

/**
 * \brief TIM5CH4引脚重映像使能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_tim5_ch4_pin_remap_enable (amhw_stm32f103rbt6_afio_t *p_hw_afio)
{
      p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_TIM5_CH4;
}

/**
 * \brief TIM5CH4引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_tim5_ch4_pin_remap_disable (amhw_stm32f103rbt6_afio_t *p_hw_afio)
{
      p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_TIM5_CH4);
}

/**
 * \brief ADC重映像模式设置使能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[mode] AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGINJ
 *              AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGREG
 *              AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGINJ
 *              AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGREG
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_adc_pin_remap_enable (amhw_stm32f103rbt6_afio_t *p_hw_afio,
                                            int                 mode)
{
      switch (mode) {

      case AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGINJ:
          p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGINJ;
          break;

      case AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGREG:
          p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGREG;
          break;

      case AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGINJ:
          p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGINJ;
          break;

      case AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGREG:
          p_hw_afio->mapr |= 1 << AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGREG;
          break;

      default:
          break;
      }
}

/**
 * \brief ADC重映像模式设置禁能
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[mode] AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGINJ
 *              AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGREG
 *              AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGINJ
 *              AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGREG
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_adc_pin_remap_disable (amhw_stm32f103rbt6_afio_t *p_hw_afio,
                                             int                 mode)
{
      switch (mode) {

      case AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGINJ:
          p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGINJ);
          break;

      case AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGREG:
          p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_ADC1_ETRGREG);
          break;

      case AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGINJ:
          p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGINJ);
          break;

      case AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGREG:
          p_hw_afio->mapr &= ~(1 << AMHW_STM32F103RBT6_AFIO_MAPR_ADC2_ETRGREG);
          break;

      default:
          break;
      }
}


/**
 * \brief SWJ_CFG重映像模式设置
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[in] mode      重映设模式：amhw_stm32f103rbt6_afio_swj_cfg_mode_t枚举的宏
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_swj_cfg_remap_mode_set (amhw_stm32f103rbt6_afio_t *p_hw_afio,
                                              amhw_stm32f103rbt6_afio_swj_cfg_mode_t mode)
{
    uint32_t flags = p_hw_afio->mapr;

    flags &= ~(0x7 << AMHW_STM32F103RBT6_AFIO_SWJ_CFG);
    flags |= (mode & 0x7) << AMHW_STM32F103RBT6_AFIO_SWJ_CFG;

    p_hw_afio->mapr |= flags;
}

/**
 * \brief 外部中断号设置
 *
 * \param[in] p_hw_afio 指向 amhw_stm32f103rbt6_afio_t 结构的指针
 * \param[in] pin       引脚号
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_afio_exti_pin_set (amhw_stm32f103rbt6_afio_t *p_hw_afio, int pin)
{
    p_hw_afio->exticr[(pin >> 2) & 0x3] |=
                                     ((pin >> 4) & 0x3) << ((pin & 0x3) << 2);
}

/**
 * @} amhw_stm32f103rbt6_if_afio
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_STM32F103RBT6_AFIO_H */

/* end of file */
