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
 * - 1.00 17-08-21  zcb, first implementation
 * \endinternal
 */

#ifndef __AMHW_MM32L373_AFIO_H
#define __AMHW_MM32L373_AFIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"

/**
 * \addtogroup amhw_mm32l373_if_afio
 * \copydoc amhw_mm32l373_afio.h
 * @{
 */

/**
 * \brief GPIO 端口寄存器结构体
 */
typedef struct amhw_mm32l373_afio {
          uint32_t reserve;      /**< \brief 保留 */
    __IO  uint32_t mapr;         /**< \brief 复用重映射和调试I/O配置寄存器 */
    __IO  uint32_t exticr[4];    /**< \brief 外部中断配置寄存器 */
} amhw_mm32l373_afio_t;

/**
 * \brief 可重映像的外设编号
 */
typedef enum {
    AMHW_MM32L373_SPI1_REMAP = 0,     /**< \brief SPI1 */
    AMHW_MM32L373_I2C1_REMAP,         /**< \brief I2C1 */
    AMHW_MM32L373_UART1_REMAP,        /**< \brief UART1 */
    AMHW_MM32L373_UART3_REMAP = 4,    /**< \brief UART3 */
    AMHW_MM32L373_TIM1_REMAP = 6,     /**< \brief TIM1 */
    AMHW_MM32L373_TIM2_REMAP = 8,     /**< \brief TIM2 */
    AMHW_MM32L373_TIM3_REMAP = 10,    /**< \brief TIM3 */
    AMHW_MM32L373_CAN_REMAP  = 13,    /**< \brief CAN */
    AMHW_MM32L373_PD01_REMAP = 15,    /**< \brief PD01 */
    AMHW_MM32L373_SWJ_CFG,            /**< \brief SWJ_CFG */
    AMHW_MM32L373_NO_REMAP,           /**< \brief 没有重映像功能 */
}amhw_mm32l373_afio_remap_peripheral_t;

/**
 * \name  重映像位定义
 * @{
 */

/** \brief SPI1重映像 SCK/PB3，MISO/PB4，MOSI/PB5 */
#define AMHW_MM32L373_AFIO_MAPR_SPI1    (0u)

/** \brief I2C1重映像 SCL/PB8，SDA/PB9 */
#define AMHW_MM32L373_AFIO_MAPR_I2C1    (1u)

/** \brief UART1重映像 TX/PB6，RX/PB7 */
#define AMHW_MM32L373_AFIO_MAPR_UART1   (2u)

/** \brief UART3部分重映像 TX/PC10，RX/PC11 */
#define AMHW_MM32L373_AFIO_MAPR_UART3   (4u)

/** \brief
 * TIM1部分重映像
 * ETR/PA12，
 * CH1/PA8，CH2/PA9，CH3/PA10，CH4/PA11，
 * BAKIN/PA6，
 * CH1N/PA7，CH2N/PB0，CH3N/PB1
 */
#define AMHW_MM32L373_AFIO_MAPR_TIM1    (6u)

/** \brief TIM2重映像 */
#define AMHW_MM32L373_AFIO_MAPR_TIM2    (8u)

typedef enum {

    /** \brief CH1/ETR/PA0，CH2/PA1，CH3/PA2，CH4/PA3 */
    AMHW_MM32L373_AFIO_TIM2_REMAP_NO = 0,
    /** \brief CH1/ETR/PA15，CH2/PB3，CH3/PA2，CH4/PA3 */
    AMHW_MM32L373_AFIO_TIM2_REMAP_PART1,
    /** \brief CH1/ETR/PA0，CH2/PA1，CH3/PB10，CH4/PB11 */
    AMHW_MM32L373_AFIO_TIM2_REMAP_PART2,
    /** \brief CH1/ETR/PA15，CH2/PB3，CH3/PB10，CH4/PB11 */
    AMHW_MM32L373_AFIO_TIM2_REMAP_ALL
}amhw_mm32l373_afio_tim2_remap_mode_t;

/** \brief TIM3重映像 */
#define AMHW_MM32L373_AFIO_MAPR_TIM3    (10u)

typedef enum {

    /** \brief CH1/PA6，CH2/PA7，CH3/PB0，CH4/PB1 */
    AMHW_MM32L373_AFIO_TIM3_REMAP_NO = 0,
    /** \brief CH1 /PB4，CH2/PB5，CH3/PB0，CH4/PB1 */
    AMHW_MM32L373_AFIO_TIM3_REMAP_PART = 2,
    /** \brief CH1/PC6，CH2/PC7，CH3/PC8，CH4/PC9 */
    AMHW_MM32L373_AFIO_TIM3_REMAP_ALL
}amhw_mm32l373_afio_tim3_remap_mode_t;

/** \brief CAN重映像 */
#define AMHW_MM32L373_AFIO_MAPR_CAN     (13u)

typedef enum {
    AMHW_MM32L373_AFIO_CAN_RX_PA11_TX_PA12 = 0,
    AMHW_MM32L373_AFIO_CAN_RX_PB8_TX_PB9  = 2,
    AMHW_MM32L373_AFIO_CAN_RX_PD0_PD1  = 3,
}amhw_mm32l373_afio_can_remap_mode_t;

/** \brief PD01重映像 PD0映像到OSC_IN，PD1映像到OSC_OUT */
#define AMHW_MM32L373_AFIO_MAPR_PD01    (15u)

/** \brief 串行线JTAG配置 */
#define AMHW_MM32L373_AFIO_SWJ_CFG      (24u)

typedef enum {
    /**
     * \brief 完全SWJ（JTAG-DP + SW-DP）：复位状态
     *
     * PIOA13:I/0不可用   PIOA14:I/0不可用   PIOA15:I/0不可用
     * PIOB3 :I/0不可用   PIOB4 :I/0不可用
     */
    AMHW_MM32L373_AFIO_SWJ_CFG_0 = 0,

    /**
     * \brief 完全SWJ（JTAG-DP + SW-DP）但没有JNTRST
     *
     * PIOA13:I/0不可用   PIOA14:I/0不可用   PIOA15:I/0不可用
     * PIOB3 :I/0不可用   PIOB4 :I/0可用
     */
    AMHW_MM32L373_AFIO_SWJ_CFG_1,

    /**
     * \brief 关闭JTAG-DP，启动SW-DP
     *
     * PIOA13:I/0不可用   PIOA14:I/0不可用   PIOA15:I/0可用
     * PIOB3 :I/0可用   PIOB4 :I/0可用
     */
    AMHW_MM32L373_AFIO_SWJ_CFG_2,

    /**
     * \brief 关闭JTAG-DP，关闭SW-DP
     *
     * PIOA13:I/0可用   PIOA14:I/0可用   PIOA15:I/0可用
     * PIOB3 :I/0可用   PIOB4 :I/0可用
     */
    AMHW_MM32L373_AFIO_SWJ_CFG_7 = 7,
}amhw_mm32l373_afio_swj_cfg_mode_t;

/** @} */

/**
 * \brief SPI1引脚重映像使能
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_spi1_pin_remap_enable (amhw_mm32l373_afio_t *p_hw_afio)
{
    p_hw_afio->mapr |= 1 << AMHW_MM32L373_AFIO_MAPR_SPI1;
}

/**
 * \brief SPI1引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_spi1_pin_remap_disable (amhw_mm32l373_afio_t *p_hw_afio)
{
    p_hw_afio->mapr &= ~(1 << AMHW_MM32L373_AFIO_MAPR_SPI1);
}

/**
 * \brief I2C1引脚重映像使能
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_i2c1_pin_remap_enable (amhw_mm32l373_afio_t *p_hw_afio)
{
    p_hw_afio->mapr |= 1 << AMHW_MM32L373_AFIO_MAPR_I2C1;
}

/**
 * \brief I2C1引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_i2c1_pin_remap_disable (amhw_mm32l373_afio_t *p_hw_afio)
{
    p_hw_afio->mapr &= ~(1 << AMHW_MM32L373_AFIO_MAPR_I2C1);
}

/**
 * \brief UART1引脚重映像使能
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 * \param[in] num       串口号：0；2
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_uart_pin_remap_enable (amhw_mm32l373_afio_t *p_hw_afio,
                                             int                 num)
{
    if (num == 1) {
        p_hw_afio->mapr |= 1 << AMHW_MM32L373_AFIO_MAPR_UART1;
    } else if (num == 3) {
        p_hw_afio->mapr |= 1 << AMHW_MM32L373_AFIO_MAPR_UART3;
    }
}

/**
 * \brief UART1引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 * \param[in] num       串口号：0；2
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_uart_pin_remap_disable (amhw_mm32l373_afio_t *p_hw_afio,
                                              int num)
{
    if (num == 1) {
        p_hw_afio->mapr &= ~(1 << AMHW_MM32L373_AFIO_MAPR_UART1);
    } else if (num == 3) {
        p_hw_afio->mapr &= ~(1 << AMHW_MM32L373_AFIO_MAPR_UART3);
    }
}

/**
 * \brief TIM1引脚重映像使能
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_tim1_pin_remap_enable (amhw_mm32l373_afio_t *p_hw_afio)
{
    p_hw_afio->mapr |= 1 << AMHW_MM32L373_AFIO_MAPR_TIM1;
}

/**
 * \brief TIM1引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_tim1_pin_remap_disable (amhw_mm32l373_afio_t *p_hw_afio)
{
    p_hw_afio->mapr &= ~(1 << AMHW_MM32L373_AFIO_MAPR_TIM1);
}

/**
 * \brief TIM2引脚重映像模式设置
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 * \param[in] mode      重映设模式：amhw_mm32l373_afio_tim2_remap_mode枚举的宏
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_tim2_remap_mode_set (amhw_mm32l373_afio_t *p_hw_afio,
                                           amhw_mm32l373_afio_tim2_remap_mode_t mode)
{
    p_hw_afio->mapr &= ~(0x3 << AMHW_MM32L373_AFIO_MAPR_TIM2);
    p_hw_afio->mapr |= (mode & 0x3) << AMHW_MM32L373_AFIO_MAPR_TIM2;
}

/**
 * \brief TIM3引脚重映像模式设置
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 * \param[in] mode      重映设模式：amhw_mm32l373_afio_tim3_remap_mode枚举的宏
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_tim3_remap_mode_set (amhw_mm32l373_afio_t *p_hw_afio,
                                           amhw_mm32l373_afio_tim3_remap_mode_t mode)
{
    p_hw_afio->mapr &= ~(0x3 << AMHW_MM32L373_AFIO_MAPR_TIM3);
    p_hw_afio->mapr |= (mode & 0x3) << AMHW_MM32L373_AFIO_MAPR_TIM3;
}

/**
 * \brief CAN引脚重映像模式设置
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 * \param[in] mode      重映设模式：amhw_mm32l373_afio_can_remap_mode枚举的宏
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_can_remap_mode_set (amhw_mm32l373_afio_t *p_hw_afio,
                                          amhw_mm32l373_afio_can_remap_mode_t mode)
{
    p_hw_afio->mapr &= ~(0x3 << AMHW_MM32L373_AFIO_MAPR_CAN);
    p_hw_afio->mapr |= (mode & 0x3) << AMHW_MM32L373_AFIO_MAPR_CAN;
}

/**
 * \brief PD01引脚重映像使能
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_pd01_pin_remap_enable (amhw_mm32l373_afio_t *p_hw_afio)
{
    p_hw_afio->mapr |= 1 << AMHW_MM32L373_AFIO_MAPR_PD01;
}

/**
 * \brief PD01引脚重映像禁能
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_pd01_pin_remap_disable (amhw_mm32l373_afio_t *p_hw_afio)
{
    p_hw_afio->mapr &= ~(1 << AMHW_MM32L373_AFIO_MAPR_PD01);
}

/**
 * \brief SWJ_CFG重映像模式设置
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 * \param[in] mode      重映设模式：amhw_mm32l373_afio_swj_cfg_mode_t枚举的宏
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_swj_cfg_remap_mode_set (amhw_mm32l373_afio_t *p_hw_afio,
                                              amhw_mm32l373_afio_swj_cfg_mode_t mode)
{
    uint32_t flags = p_hw_afio->mapr;

    flags &= ~(0x7 << AMHW_MM32L373_AFIO_SWJ_CFG);
    flags |= (mode & 0x7) << AMHW_MM32L373_AFIO_SWJ_CFG;

    p_hw_afio->mapr |= flags;
}

/**
 * \brief 外部中断号设置
 *
 * \param[in] p_hw_afio 指向 amhw_mm32l373_afio_t 结构的指针
 * \param[in] pin       引脚号
 *
 * \return 无
 */
am_static_inline
void amhw_mm32l373_afio_exti_pin_set (amhw_mm32l373_afio_t *p_hw_afio, int pin)
{
    p_hw_afio->exticr[(pin >> 2) & 0x3] |=
                                     ((pin >> 4) & 0x3) << ((pin & 0x3) << 2);
}

/**
 * @} amhw_mm32l373_if_afio
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_MM32L373_AFIO_H */

/* end of file */
