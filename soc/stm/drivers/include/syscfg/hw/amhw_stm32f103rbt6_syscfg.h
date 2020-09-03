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
 * \brief 系统控制接口
 *
 * 1. 重映射部分从 TIM16 和 TIM17，UART1 和 ADC 的 DMA 触发源到其它不同的 DMA通道上；
 * 2. 管理连接到 GPIO口的外部中断；
 * 3. 重映射存储器到代码起始区域；
 * 4. 外部中断引脚配置；
 *
 * \internal
 * \par History
 * - 1.00 17-04-07  nwt, first implementation
 * \endinternal
 */

#ifndef __AMHW_STM32F103RBT6_SYSCFG_H
#define __AMHW_STM32F103RBT6_SYSCFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_stm32f103rbt6_if_syscfg
 * \copydoc amhw_stm32f103rbt6_syscfg.h
 * @{
 */

/**
 * \name 外部中断端口源
 * @{
 */

#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOA       ((uint8_t)0x00)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOB       ((uint8_t)0x01)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOC       ((uint8_t)0x02)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOD       ((uint8_t)0x03)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOE       ((uint8_t)0x04)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOF       ((uint8_t)0x05)

#define AMHW_STM32F103RBT6_SYSCFG_IS_EXTI_PORTSOURCE(portsource) \
           (((portsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOA) || \
            ((portsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOB) || \
            ((portsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOC) || \
            ((portsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOD) || \
            ((portsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOE) || \
            ((portsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOF))

/** @} */

/**
 * \name 外部中断的引脚源
 * @{
 */
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_0            ((uint8_t)0x00)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_1            ((uint8_t)0x01)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_2            ((uint8_t)0x02)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_3            ((uint8_t)0x03)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_4            ((uint8_t)0x04)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_5            ((uint8_t)0x05)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_6            ((uint8_t)0x06)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_7            ((uint8_t)0x07)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_8            ((uint8_t)0x08)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_9            ((uint8_t)0x09)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_10           ((uint8_t)0x0A)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_11           ((uint8_t)0x0B)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_12           ((uint8_t)0x0C)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_13           ((uint8_t)0x0D)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_14           ((uint8_t)0x0E)
#define AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_15           ((uint8_t)0x0F)

#define AMHW_STM32F103RBT6_SYSCFG_IS_EXTI_PIN_SOURCE(pinsource) \
           (((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_0)  || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_1)  || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_2)  || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_3)  || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_4)  || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_5)  || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_6)  || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_7)  || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_8)  || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_9)  || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_10) || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_11) || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_12) || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_13) || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_14) || \
            ((pinsource) == AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_15))

/** @} */

/**
  * \brief 系统控制寄存器块结构体
  */
typedef struct amhw_stm32f103rbt6_syscfg {
    __IO uint32_t cfgr;        /**< \brief 系统配置寄存器 */
    __I  uint32_t reserve;     /**< \brief 保留 */
    __IO uint32_t exticr[4];   /**< \brief 外部中断配置寄存器 */
} amhw_stm32f103rbt6_syscfg_t;

/**
 * \brief 系统内存重映射，用于重映射存储器到代码起始区域
 */
typedef enum amhw_stm32f103rbt6_syscfg_bootmode_remap {
    AMHW_STM32F103RBT6_SYSCFG_REMAP_BOOT_MAIN_FLASH = 0,  /**< \brief 主闪存存储器映射到 0x00000000 */
    AMHW_STM32F103RBT6_SYSCFG_REMAP_BOOT_SYS_FLASH  = 1,  /**< \brief 系统闪存映射到 0x00000000 */
    AMHW_STM32F103RBT6_SYSCFG_REMAP_BOOT_SRAM       = 3,  /**< \brief 嵌入式 RAM 映射到 0x00000000 */
} amhw_stm32f103rbt6_syscfg_bootmode_remap_t;

/**
 * \brief 某些外设(UART1, TIM16、TIM17)DMA通道重映射
 */
typedef enum amhw_stm32f103rbt6_syscfg_dma_chan_remap {
    AMHW_STM32F103RBT6_SYSCFG_REMAP_TIM17_DMA    = (1 << 12), /**< \brief 定时器17DMA通道重映射 */
    AMHW_STM32F103RBT6_SYSCFG_REMAP_TIM16_DMA    = (1 << 11), /**< \brief 定时器16DMA通道重映射 */
    AMHW_STM32F103RBT6_SYSCFG_REMAP_UART1_RX_DMA = (1 << 10), /**< \brief UART1_RX_DMA通道重映射 */
    AMHW_STM32F103RBT6_SYSCFG_REMAP_UART1_TX_DMA = (1 << 9),  /**< \brief UART1_TX_DMA通道重映射 */
    AMHW_STM32F103RBT6_SYSCFG_REMAP_ADC_DMA      = (1 << 8),  /**< \brief ADC_DM通道重映射 */
} amhw_stm32f103rbt6_syscfg_dma_chan_remap_t;

/**
 * \brief 重映射存储器到代码起始区域
 *
 * \param[in] remap       ：映射代码起始区的存储位置
 * \param[in] p_hw_syscfg ：指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_syscfg_mem_mode_remap_set (
                                amhw_stm32f103rbt6_syscfg_t               *p_hw_syscfg,
                                amhw_stm32f103rbt6_syscfg_bootmode_remap_t remap)
{
    AM_BITS_SET(p_hw_syscfg->cfgr, 0, 2, remap);
}

/**
 * \brief 获取存储器到代码起始区域的重映射值
 *
 * \param[in] p_hw_syscfg ：指向系统配置寄存器块的指针
 *
 * \return 储器到代码起始区域的重映射值
 */
am_static_inline
int amhw_stm32f103rbt6_syscfg_mem_mode_remap_get (amhw_stm32f103rbt6_syscfg_t *p_hw_syscfg)
{
    return (amhw_stm32f103rbt6_syscfg_bootmode_remap_t)p_hw_syscfg->cfgr;
}

/**
 * \brief 重映射某些外设(UART1, TIM16、TIM17)的DMA通道
 *
 * \param[in] remap       ：重映射的DMA通道，值为 amhw_stm32f103rbt6_syscfg_dma_chan_remap_t
 *                         该枚举类型，可以使用或组合
 * \param[in] p_hw_syscfg ：指向系统配置寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_syscfg_dma_chan_remap_set (
                                   amhw_stm32f103rbt6_syscfg_t               *p_hw_syscfg,
                                   amhw_stm32f103rbt6_syscfg_dma_chan_remap_t remap)
{
    AM_BITS_SET(p_hw_syscfg->cfgr, 8, 5, remap);
}

/**
 * \brief 选择 EXTIx(x=A...D) 外部中断的输入源
 *
 * \param[in] p_hw_syscfg ：指向系统配置寄存器块的指针
 * \param[in] portsource  ：端口源,值为 AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE* 这一类宏
 *                          (# AMHW_STM32F103RBT6_SYSCFG_EXTI_PORTSOURCE_GPIOA)
 * \param[in] pinsource   ：引脚源，值为  AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE* 这一类宏
 *                          (# AMHW_STM32F103RBT6_SYSCFG_EXTI_PINSOURCE_0)
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_syscfg_exti_line_config (amhw_stm32f103rbt6_syscfg_t *p_hw_syscfg,
                                       uint8_t            portsource,
                                       uint8_t            pinsource)
{
    uint32_t tmp = 0x00;

    /*
     * 原理：在选择第0个引脚，则应该设置最低4位，如果最低4位为0，则代表是PA0引脚
     */

    tmp = ((uint32_t)0x0F) << (0x04 * (pinsource & (uint8_t)0x03));

    p_hw_syscfg->exticr[pinsource >> 0x02] &= ~tmp;
    p_hw_syscfg->exticr[pinsource >> 0x02] |= \
               (((uint32_t)portsource) << (0x04 * (pinsource & (uint8_t)0x03)));
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_STM32F103RBT6_SYSCFG_H */

/* end of file */
