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
 * \brief HC32F460 中断处理函数
 *
 * \internal
 * \par Modification history
 * - 1.00 20-01-13  cds, first implementation
 * \endinternal
 */

#include "am_hc32f460.h"
#include "am_hc32f460_gpio.h"
#include "am_hc32f460_uart.h"
#include "am_hc32f460_tim_timing.h"
#include "am_hc32f460_timea_timing.h"
#include "am_hc32f460_tim4_timing.h"
#include "am_hc32f460_adtim_timing.h"
#include "am_hc32f460_timea_cap.h"
#include "am_hc32f460_adtim_cap.h"
#include "am_hc32f460_i2c.h"
#include "am_hc32f460_rtc.h"
#include "am_hc32f460_dma.h"
#include "am_hc32f460_spi_int.h"
#include "am_hc32f460_adc.h"
#include "am_hc32f460_sdioc.h"
#include "am_hc32f460_wdt.h"
#include "am_hc32f460_swdt.h"
#include "am_hc32f460_usbd.h"

#define AM_HC32F460_INT_EXTI0_MASK    (1 << 0)
#define AM_HC32F460_INT_EXTI1_MASK    (1 << 1)
#define AM_HC32F460_INT_EXTI2_MASK    (1 << 2)
#define AM_HC32F460_INT_EXTI3_MASK    (1 << 3)
#define AM_HC32F460_INT_EXTI4_MASK    (1 << 4)
#define AM_HC32F460_INT_EXTI5_MASK    (1 << 5)
#define AM_HC32F460_INT_EXTI6_MASK    (1 << 6)
#define AM_HC32F460_INT_EXTI7_MASK    (1 << 7)
#define AM_HC32F460_INT_EXTI8_MASK    (1 << 8)
#define AM_HC32F460_INT_EXTI9_MASK    (1 << 9)
#define AM_HC32F460_INT_EXTI10_MASK   (1 << 10)
#define AM_HC32F460_INT_EXTI11_MASK   (1 << 11)
#define AM_HC32F460_INT_EXTI12_MASK   (1 << 12)
#define AM_HC32F460_INT_EXTI13_MASK   (1 << 13)
#define AM_HC32F460_INT_EXTI14_MASK   (1 << 14)
#define AM_HC32F460_INT_EXTI15_MASK   (1 << 15)

#define AM_HC32F460_INT_EXTI0_ENABLE_MASK    (1 << 0)
#define AM_HC32F460_INT_EXTI1_ENABLE_MASK    (1 << 1)
#define AM_HC32F460_INT_EXTI2_ENABLE_MASK    (1 << 2)
#define AM_HC32F460_INT_EXTI3_ENABLE_MASK    (1 << 3)
#define AM_HC32F460_INT_EXTI4_ENABLE_MASK    (1 << 4)
#define AM_HC32F460_INT_EXTI5_ENABLE_MASK    (1 << 5)
#define AM_HC32F460_INT_EXTI6_ENABLE_MASK    (1 << 6)
#define AM_HC32F460_INT_EXTI7_ENABLE_MASK    (1 << 7)
#define AM_HC32F460_INT_EXTI8_ENABLE_MASK    (1 << 8)
#define AM_HC32F460_INT_EXTI9_ENABLE_MASK    (1 << 9)
#define AM_HC32F460_INT_EXTI10_ENABLE_MASK   (1 << 10)
#define AM_HC32F460_INT_EXTI11_ENABLE_MASK   (1 << 11)
#define AM_HC32F460_INT_EXTI12_ENABLE_MASK   (1 << 12)
#define AM_HC32F460_INT_EXTI13_ENABLE_MASK   (1 << 13)
#define AM_HC32F460_INT_EXTI14_ENABLE_MASK   (1 << 14)
#define AM_HC32F460_INT_EXTI15_ENABLE_MASK   (1 << 15)

/* DMA 共享中断使能宏定义 */
#define AM_HC32F460_INT_VSSEL_DMA1_TC0_MASK    (1 << 0)
#define AM_HC32F460_INT_VSSEL_DMA1_TC1_MASK    (1 << 1)
#define AM_HC32F460_INT_VSSEL_DMA1_TC2_MASK    (1 << 2)
#define AM_HC32F460_INT_VSSEL_DMA1_TC3_MASK    (1 << 3)
#define AM_HC32F460_INT_VSSEL_DMA2_TC0_MASK    (1 << 4)
#define AM_HC32F460_INT_VSSEL_DMA2_TC1_MASK    (1 << 5)
#define AM_HC32F460_INT_VSSEL_DMA2_TC2_MASK    (1 << 6)
#define AM_HC32F460_INT_VSSEL_DMA2_TC3_MASK    (1 << 7)
#define AM_HC32F460_INT_VSSEL_DMA1_BTC0_MASK   (1 << 8)
#define AM_HC32F460_INT_VSSEL_DMA1_BTC1_MASK   (1 << 9)
#define AM_HC32F460_INT_VSSEL_DMA1_BTC2_MASK   (1 << 10)
#define AM_HC32F460_INT_VSSEL_DMA1_BTC3_MASK   (1 << 11)
#define AM_HC32F460_INT_VSSEL_DMA2_BTC0_MASK   (1 << 12)
#define AM_HC32F460_INT_VSSEL_DMA2_BTC1_MASK   (1 << 13)
#define AM_HC32F460_INT_VSSEL_DMA2_BTC2_MASK   (1 << 14)
#define AM_HC32F460_INT_VSSEL_DMA2_BTC3_MASK   (1 << 15)
#define AM_HC32F460_INT_VSSEL_DMA1_ERR_MASK    (1 << 16)
#define AM_HC32F460_INT_VSSEL_DMA2_ERR_MASK    (1 << 17)

/* SPI */
#define __HC32F460_SPI_CR1_IDIE_MASK    (1 << 11)   /**< \brief SPI空闲中断使能 */
#define __HC32F460_SPI_CR1_RXIE_MASK    (1 << 10)   /**< \brief SPI接收中断使能 */
#define __HC32F460_SPI_CR1_TXIE_MASK    (1 << 9)    /**< \brief SPI发送中断使能 */
#define __HC32F460_SPI_CR1_EIE_MASK     (1 << 8)    /**< \brief SPI错误中断使能 */

#define __HC32F460_SPI_SR_RDFF_MASK     (1 << 7)    /**< \brief 接收缓冲器满标志 */
#define __HC32F460_SPI_SR_TDEF_MASK     (1 << 5)    /**< \brief 发送缓冲器空标志 */
#define __HC32F460_SPI_SR_UDRERF_MASK   (1 << 4)    /**< \brief 欠载错误标志 */
#define __HC32F460_SPI_SR_PERF_MASK     (1 << 3)    /**< \brief 奇偶校验错误标志 */
#define __HC32F460_SPI_SR_MODFERF_MASK  (1 << 2)    /**< \brief 模式故障错误标志 */
#define __HC32F460_SPI_SR_IDLNF_MASK    (1 << 1)    /**< \brief SPI空闲标志 */
#define __HC32F460_SPI_SR_OVRERF_MASK   (1 << 0)    /**< \brief 过载错误标志 */

#define __HC32F460_INTC_VSSEL_SPI1_SRRI_MASK   (1 << 11)
#define __HC32F460_INTC_VSSEL_SPI1_SRTI_MASK   (1 << 12)
#define __HC32F460_INTC_VSSEL_SPI1_SPII_MASK   (1 << 13)
#define __HC32F460_INTC_VSSEL_SPI1_SPEI_MASK   (1 << 14)
#define __HC32F460_INTC_VSSEL_SPI1_SPTEND_MASK (1 << 15)

#define __HC32F460_INTC_VSSEL_SPI2_SRRI_MASK   (1 << 16)
#define __HC32F460_INTC_VSSEL_SPI2_SRTI_MASK   (1 << 17)
#define __HC32F460_INTC_VSSEL_SPI2_SPII_MASK   (1 << 18)
#define __HC32F460_INTC_VSSEL_SPI2_SPEI_MASK   (1 << 19)
#define __HC32F460_INTC_VSSEL_SPI2_SPTEND_MASK (1 << 20)

#define __HC32F460_INTC_VSSEL_SPI3_SRRI_MASK   (1 << 21)
#define __HC32F460_INTC_VSSEL_SPI3_SRTI_MASK   (1 << 22)
#define __HC32F460_INTC_VSSEL_SPI3_SPII_MASK   (1 << 23)
#define __HC32F460_INTC_VSSEL_SPI3_SPEI_MASK   (1 << 24)
#define __HC32F460_INTC_VSSEL_SPI3_SPTEND_MASK (1 << 25)

#define __HC32F460_INTC_VSSEL_SPI4_SRRI_MASK   (1 << 26)
#define __HC32F460_INTC_VSSEL_SPI4_SRTI_MASK   (1 << 27)
#define __HC32F460_INTC_VSSEL_SPI4_SPII_MASK   (1 << 28)
#define __HC32F460_INTC_VSSEL_SPI4_SPEI_MASK   (1 << 29)
#define __HC32F460_INTC_VSSEL_SPI4_SPTEND_MASK (1 << 30)

/**
 *******************************************************************************
 ** \brief flag status
 ******************************************************************************/
typedef enum en_flag_status
{
    Reset = 0u,
    Set   = 1u,
} en_flag_status_t, en_int_status_t;

/**
 *******************************************************************************
 ** \brief Int No.128 share IRQ handler
 **
 ******************************************************************************/
void IRQ128_Handler(void)
{
    uint32_t int_vssel = HC32F460_INTC.VSSEL[128 - 128];
    uint32_t int_flag = HC32F460_INTC.EIFR;

    /** \brief external interrupt 00 */
    if ((int_flag == AM_HC32F460_INT_EXTI0_MASK) && (int_vssel & AM_HC32F460_INT_EXTI0_ENABLE_MASK))
    {
        Extint00_IrqHandler();
    }
    /** \brief external interrupt 01 */
    if ((int_flag == AM_HC32F460_INT_EXTI1_MASK) && (int_vssel & AM_HC32F460_INT_EXTI1_ENABLE_MASK))
    {
        Extint01_IrqHandler();
    }
    /** \brief external interrupt 02 */
    if ((int_flag == AM_HC32F460_INT_EXTI2_MASK) && (int_vssel & AM_HC32F460_INT_EXTI2_ENABLE_MASK))
    {
        Extint02_IrqHandler();
    }
    /** \brief external interrupt 03 */
    if ((int_flag == AM_HC32F460_INT_EXTI3_MASK) && (int_vssel & AM_HC32F460_INT_EXTI3_ENABLE_MASK))
    {
        Extint03_IrqHandler();
    }
    /** \brief external interrupt 04 */
    if ((int_flag == AM_HC32F460_INT_EXTI4_MASK) && (int_vssel & AM_HC32F460_INT_EXTI4_ENABLE_MASK))
    {
        Extint04_IrqHandler();
    }
    /** \brief external interrupt 05 */
    if ((int_flag == AM_HC32F460_INT_EXTI5_MASK) && (int_vssel & AM_HC32F460_INT_EXTI5_ENABLE_MASK))
    {
        Extint05_IrqHandler();
    }
    /** \brief external interrupt 06 */
    if ((int_flag == AM_HC32F460_INT_EXTI6_MASK) && (int_vssel & AM_HC32F460_INT_EXTI6_ENABLE_MASK))
    {
        Extint06_IrqHandler();
    }
    /** \brief external interrupt 07 */
    if ((int_flag == AM_HC32F460_INT_EXTI7_MASK) && (int_vssel & AM_HC32F460_INT_EXTI7_ENABLE_MASK))
    {
        Extint07_IrqHandler();
    }
    /** \brief external interrupt 08 */
    if ((int_flag == AM_HC32F460_INT_EXTI8_MASK) && (int_vssel & AM_HC32F460_INT_EXTI8_ENABLE_MASK))
    {
        Extint08_IrqHandler();
    }
    /** \brief external interrupt 09 */
    if ((int_flag == AM_HC32F460_INT_EXTI9_MASK) && (int_vssel & AM_HC32F460_INT_EXTI9_ENABLE_MASK))
    {
        Extint09_IrqHandler();
    }
    /** \brief external interrupt 10 */
    if ((int_flag == AM_HC32F460_INT_EXTI10_MASK) && (int_vssel & AM_HC32F460_INT_EXTI10_ENABLE_MASK))
    {
        Extint10_IrqHandler();
    }
    /** \brief external interrupt 11 */
    if ((int_flag == AM_HC32F460_INT_EXTI11_MASK) && (int_vssel & AM_HC32F460_INT_EXTI11_ENABLE_MASK))
    {
        Extint11_IrqHandler();
    }
    /** \brief external interrupt 12 */
    if ((int_flag == AM_HC32F460_INT_EXTI12_MASK) && (int_vssel & AM_HC32F460_INT_EXTI12_ENABLE_MASK))
    {
        Extint12_IrqHandler();
    }
    /** \brief external interrupt 13 */
    if ((int_flag == AM_HC32F460_INT_EXTI13_MASK) && (int_vssel & AM_HC32F460_INT_EXTI13_ENABLE_MASK))
    {
        Extint13_IrqHandler();
    }
    /** \brief external interrupt 14 */
    if ((int_flag == AM_HC32F460_INT_EXTI14_MASK) && (int_vssel & AM_HC32F460_INT_EXTI14_ENABLE_MASK))
    {
        Extint14_IrqHandler();
    }
    /** \brief external interrupt 15 */
    if ((int_flag == AM_HC32F460_INT_EXTI15_MASK) && (int_vssel & AM_HC32F460_INT_EXTI15_ENABLE_MASK))
    {
        Extint15_IrqHandler();
    }
}


extern void dma_int_handler (void *p_arg);
extern am_hc32f460_dma_dev_t __g_dma1_dev;
extern am_hc32f460_dma_dev_t __g_dma2_dev;
/**
 *******************************************************************************
 ** \brief Int No.129 share IRQ handler
 **
 ******************************************************************************/
void IRQ129_Handler(void)
{
    uint32_t VSSEL129 = HC32F460_INTC.VSSEL[129 - 128];
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;

    /****************** DMA1 中断处理  *******************/
    if (1ul == HC32F460_DMA1->CH0CTL_f.IE)
    {
    	/** \brief DMA1 ch.0 Tx completed */
        if (0ul == (HC32F460_DMA1->INTMASK1_f.MSKTC & (1 << 0)))
        {
            if (((1ul << 0) & HC32F460_DMA1->INTSTAT1_f.TC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_TC0_MASK))
            {
                dma_int_handler(&__g_dma1_dev);
            }
        }
        /** \brief DMA1 ch.0 Block Tx completed */
        if (0ul == (HC32F460_DMA1->INTMASK1_f.MSKBTC & (1 << 0)))
        {
            if (((1ul << 0) & HC32F460_DMA1->INTSTAT1_f.BTC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_BTC0_MASK))
            {
                dma_int_handler(&__g_dma1_dev);
            }
        }
        /** \brief DMA1 ch.0 Transfer/Request Error */
        u32Tmp1 = HC32F460_DMA1->INTSTAT0 & 0x00010001ul;
        u32Tmp2 = (uint32_t)(~(HC32F460_DMA1->INTMASK0) & 0x00010001ul);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_ERR_MASK))
        {
            dma_int_handler(&__g_dma1_dev);
        }
    }
    if (1ul == HC32F460_DMA1->CH1CTL_f.IE)
    {
    	/** \brief DMA1 ch.1 Tx completed */
        if (0ul == (HC32F460_DMA1->INTMASK1_f.MSKTC & (1 << 1)))
        {
            if (((1ul << 1) & HC32F460_DMA1->INTSTAT1_f.TC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_TC1_MASK))
            {
                dma_int_handler(&__g_dma1_dev);
            }
        }
        /** \brief DMA1 ch.0 Block Tx completed */
        if (0ul == (HC32F460_DMA1->INTMASK1_f.MSKBTC & (1 << 1)))
        {
            if (((1ul << 1) & HC32F460_DMA1->INTSTAT1_f.BTC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_BTC1_MASK))
            {
                dma_int_handler(&__g_dma1_dev);
            }
        }
        /** \brief DMA1 ch.1 Transfer/Request Error */
        u32Tmp1 = HC32F460_DMA1->INTSTAT0 & 0x00020002ul;
        u32Tmp2 = (uint32_t)(~(HC32F460_DMA1->INTMASK0) & 0x00020002ul);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_ERR_MASK))
        {
            dma_int_handler(&__g_dma1_dev);
        }
    }
    if (1ul == HC32F460_DMA1->CH2CTL_f.IE)
    {
    	/** \brief DMA1 ch.1 Tx completed */
        if (0ul == (HC32F460_DMA1->INTMASK1_f.MSKTC & (1 << 2)))
        {
            if (((1ul << 2) & HC32F460_DMA1->INTSTAT1_f.TC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_TC2_MASK))
            {
                dma_int_handler(&__g_dma1_dev);
            }
        }
        /** \brief DMA1 ch.0 Block Tx completed */
        if (0ul == (HC32F460_DMA1->INTMASK1_f.MSKBTC & (1 << 2)))
        {
            if (((1ul << 2) & HC32F460_DMA1->INTSTAT1_f.BTC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_BTC2_MASK))
            {
                dma_int_handler(&__g_dma1_dev);
            }
        }
        /** \brief DMA1 ch.1 Transfer/Request Error */
        u32Tmp1 = HC32F460_DMA1->INTSTAT0 & 0x00040004ul;
        u32Tmp2 = (uint32_t)(~(HC32F460_DMA1->INTMASK0) & 0x00040004ul);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_ERR_MASK))
        {
            dma_int_handler(&__g_dma1_dev);
        }
    }
    if (1ul == HC32F460_DMA1->CH3CTL_f.IE)
    {
        /** \brief DMA1 ch.3 Tx completed */
        if (0ul == (HC32F460_DMA1->INTMASK1_f.MSKTC & (1 << 3)))
        {
            if (((1ul << 3) & HC32F460_DMA1->INTSTAT1_f.TC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_TC3_MASK))
            {
                dma_int_handler(&__g_dma1_dev);
            }
        }
        /** \brief DMA1 ch.3 Block Tx completed */
        if (0ul == (HC32F460_DMA1->INTMASK1_f.MSKBTC & (1 << 3)))
        {
            if (((1ul << 3) & HC32F460_DMA1->INTSTAT1_f.BTC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_BTC3_MASK))
            {
                dma_int_handler(&__g_dma1_dev);
            }
        }
        /** \brief DMA1 ch.1 Transfer/Request Error */
        u32Tmp1 = HC32F460_DMA1->INTSTAT0 & 0x00080008ul;
        u32Tmp2 = (uint32_t)(~(HC32F460_DMA1->INTMASK0) & 0x00080008ul);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA1_ERR_MASK))
        {
            dma_int_handler(&__g_dma1_dev);
        }
    }
    /****************** DMA2 中断处理  *******************/
    if (1ul == HC32F460_DMA2->CH0CTL_f.IE)
    {
        /** \brief DMA1 ch.0 Tx completed */
        if (0ul == (HC32F460_DMA2->INTMASK1_f.MSKTC & (1 << 0)))
        {
            if (((1ul << 0) & HC32F460_DMA2->INTSTAT1_f.TC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_TC0_MASK))
            {
                dma_int_handler(&__g_dma2_dev);
            }
        }
        /** \brief DMA2 ch.0 Block Tx completed */
        if (0ul == (HC32F460_DMA2->INTMASK1_f.MSKBTC & (1 << 0)))
        {
            if (((1ul << 0) & HC32F460_DMA2->INTSTAT1_f.BTC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_BTC0_MASK))
            {
                dma_int_handler(&__g_dma2_dev);
            }
        }
        /** \brief DMA2 ch.0 Transfer/Request Error */
        u32Tmp1 = HC32F460_DMA2->INTSTAT0 & 0x00010001ul;
        u32Tmp2 = (uint32_t)(~(HC32F460_DMA2->INTMASK0) & 0x00010001ul);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_ERR_MASK))
        {
        	dma_int_handler(&__g_dma2_dev);
        }
    }
    if (1ul == HC32F460_DMA2->CH1CTL_f.IE)
    {
        /** \brief DMA2 ch.1 Tx completed */
        if (0ul == (HC32F460_DMA2->INTMASK1_f.MSKTC & (1 << 1)))
        {
            if (((1ul << 1) & HC32F460_DMA2->INTSTAT1_f.TC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_TC1_MASK))
            {
                dma_int_handler(&__g_dma2_dev);
            }
        }
        /** \brief DMA2 ch.0 Block Tx completed */
        if (0ul == (HC32F460_DMA2->INTMASK1_f.MSKBTC & (1 << 1)))
        {
            if (((1ul << 1) & HC32F460_DMA2->INTSTAT1_f.BTC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_BTC1_MASK))
            {
                dma_int_handler(&__g_dma2_dev);
            }
        }
        /** \brief DMA2 ch.1 Transfer/Request Error */
        u32Tmp1 = HC32F460_DMA2->INTSTAT0 & 0x00020002ul;
        u32Tmp2 = (uint32_t)(~(HC32F460_DMA2->INTMASK0) & 0x00020002ul);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_ERR_MASK))
        {
            dma_int_handler(&__g_dma2_dev);
        }
    }
    if (1ul == HC32F460_DMA2->CH2CTL_f.IE)
    {
        /** \brief DMA2 ch.1 Tx completed */
        if (0ul == (HC32F460_DMA2->INTMASK1_f.MSKTC & (1 << 2)))
        {
            if (((1ul << 2) & HC32F460_DMA2->INTSTAT1_f.TC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_TC2_MASK))
            {
                dma_int_handler(&__g_dma2_dev);
            }
        }
        /** \brief DMA2 ch.0 Block Tx completed */
        if (0ul == (HC32F460_DMA2->INTMASK1_f.MSKBTC & (1 << 2)))
        {
            if (((1ul << 2) & HC32F460_DMA2->INTSTAT1_f.BTC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_BTC2_MASK))
            {
                dma_int_handler(&__g_dma2_dev);
            }
        }
        /** \brief DMA2 ch.1 Transfer/Request Error */
        u32Tmp1 = HC32F460_DMA2->INTSTAT0 & 0x00040004ul;
        u32Tmp2 = (uint32_t)(~(HC32F460_DMA2->INTMASK0) & 0x00040004ul);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_ERR_MASK))
        {
            dma_int_handler(&__g_dma2_dev);
        }
    }
    if (1ul == HC32F460_DMA2->CH3CTL_f.IE)
    {
        /** \brief DMA2 ch.3 Tx completed */
        if (0ul == (HC32F460_DMA2->INTMASK1_f.MSKTC & (1 << 3)))
        {
            if (((1ul << 3) & HC32F460_DMA2->INTSTAT1_f.TC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_TC3_MASK))
            {
                dma_int_handler(&__g_dma2_dev);
            }
        }
        /** \brief DMA2 ch.3 Block Tx completed */
        if (0ul == (HC32F460_DMA2->INTMASK1_f.MSKBTC & (1 << 3)))
        {
            if (((1ul << 3) & HC32F460_DMA2->INTSTAT1_f.BTC) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_BTC3_MASK))
            {
                dma_int_handler(&__g_dma2_dev);
            }
        }
        /** \brief DMA2 ch.1 Transfer/Request Error */
        u32Tmp1 = HC32F460_DMA2->INTSTAT0 & 0x00080008ul;
        u32Tmp2 = (uint32_t)(~(HC32F460_DMA2->INTMASK0) & 0x00080008ul);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & AM_HC32F460_INT_VSSEL_DMA2_ERR_MASK))
        {
            dma_int_handler(&__g_dma2_dev);
        }
    }
#if 0
    /* EFM program/erase Error */
    if (1ul == bM4_EFM_FITE_PEERRITE)
    {
        if ((M4_EFM->FSR & 0x0Fu) && (VSSEL129 & BIT_MASK_18))
        {
            EfmPgmEraseErr_IrqHandler();
        }
    }
    /* EFM collision Error */
    if (1ul == bM4_EFM_FITE_RDCOLERRITE)
    {
        if ((1ul == bM4_EFM_FSR_RDCOLERR) && (VSSEL129 & BIT_MASK_19))
        {
            EfmColErr_IrqHandler();
        }
    }
    /* EFM operate end */
    if (1ul == bM4_EFM_FITE_OPTENDITE)
    {
        if ((1ul == bM4_EFM_FSR_OPTEND) && (VSSEL129 & BIT_MASK_20))
        {
            EfmOpEnd_IrqHandler();
        }
    }
    /* QSPI interrupt */
    if ((1ul == M4_QSPI->SR_f.RAER) && (VSSEL129 & BIT_MASK_22))
    {
        QspiInt_IrqHandler();
    }
    /* DCU ch.1 */
    u32Tmp1 = M4_DCU1->INTSEL;
    u32Tmp2 = M4_DCU1->FLAG;
    if ((u32Tmp1 & u32Tmp2 & 0x7Ful) && (VSSEL129 & BIT_MASK_23))
    {
        Dcu1_IrqHandler();
    }
    /* DCU ch.2 */
    u32Tmp1 = M4_DCU2->INTSEL;
    u32Tmp2 = M4_DCU2->FLAG;
    if ((u32Tmp1 & u32Tmp2 & 0x7Ful) && (VSSEL129 & BIT_MASK_24))
    {
        Dcu2_IrqHandler();
    }
    /* DCU ch.3 */
    u32Tmp1 = M4_DCU3->INTSEL;
    u32Tmp2 = M4_DCU3->FLAG;
    if ((u32Tmp1 & u32Tmp2 & 0x7Ful) && (VSSEL129 & BIT_MASK_25))
    {
        Dcu3_IrqHandler();
    }
    /* DCU ch.4 */
    u32Tmp1 = M4_DCU4->INTSEL;
    u32Tmp2 = M4_DCU4->FLAG;
    if ((u32Tmp1 & u32Tmp2 & 0x7Ful) && (VSSEL129 & BIT_MASK_26))
    {
        Dcu4_IrqHandler();
//    }
#endif
}

#define __HC32F460_UART_CR1_RIE_MASK    (1 << 5)    /**< \brief 接收中断使能 */
#define __HC32F460_UART_SR_PE_MASK      (1 << 0)    /**< \brief 接收数据错误标志 */
#define __HC32F460_UART_SR_FE_MASK      (1 << 1)    /**< \brief 接收帧错误标志 */
#define __HC32F460_UART_SR_ORE_MASK     (1 << 2)    /**< \brief 接收上溢错误标志 */
#define __HC32F460_UART_SR_RX_ERROR_MASK (__HC32F460_UART_SR_PE_MASK | \
                                          __HC32F460_UART_SR_FE_MASK | \
                                          __HC32F460_UART_SR_ORE_MASK)
#define __HC32F460_INTC_VSSEL_USART1_EI_MASK  (1 << 22)
#define __HC32F460_INTC_VSSEL_USART1_RI_MASK  (1 << 23)
#define __HC32F460_INTC_VSSEL_USART1_TI_MASK  (1 << 24)
#define __HC32F460_INTC_VSSEL_USART1_TCI_MASK (1 << 25)
#define __HC32F460_INTC_VSSEL_USART1_RTO_MASK (1 << 26)

#define __HC32F460_INTC_VSSEL_USART2_EI_MASK  (1 << 27)
#define __HC32F460_INTC_VSSEL_USART2_RI_MASK  (1 << 28)
#define __HC32F460_INTC_VSSEL_USART2_TI_MASK  (1 << 29)
#define __HC32F460_INTC_VSSEL_USART2_TCI_MASK (1 << 30)
#define __HC32F460_INTC_VSSEL_USART2_RTO_MASK (1 << 31)

#define __HC32F460_INTC_VSSEL_USART3_EI_MASK  (1 << 0)
#define __HC32F460_INTC_VSSEL_USART3_RI_MASK  (1 << 1)
#define __HC32F460_INTC_VSSEL_USART3_TI_MASK  (1 << 2)
#define __HC32F460_INTC_VSSEL_USART3_TCI_MASK (1 << 3)
#define __HC32F460_INTC_VSSEL_USART3_RTO_MASK (1 << 4)

#define __HC32F460_INTC_VSSEL_USART4_EI_MASK  (1 << 5)
#define __HC32F460_INTC_VSSEL_USART4_RI_MASK  (1 << 6)
#define __HC32F460_INTC_VSSEL_USART4_TI_MASK  (1 << 7)
#define __HC32F460_INTC_VSSEL_USART4_TCI_MASK (1 << 8)
#define __HC32F460_INTC_VSSEL_USART4_RTO_MASK (1 << 9)

#define __HC32F460_UART_CR1_TXEIE_MASK    (1 << 7)    /**< \brief 发送中断使能 */
#define __HC32F460_UART_CR1_TCIE_MASK     (1 << 6)    /**< \brief 发送完成中断使能 */
#define __HC32F460_UART_SR_TXE_MASK       (1 << 7)    /**< \brief 发送空中断标志 */
#define __HC32F460_UART_SR_TC_MASK        (1 << 6)    /**< \brief 发送完成中断标志 */

#define __HC32F460_UART_CR1_RTOIE_MASK    (1 << 1)    /**< \brief 串口超时中断使能 */
#define __HC32F460_UART_SR_RTOF_MASK      (1 << 8)    /**< \brief 超时中断标志 */


#include "amhw_arm_systick.h"
/**
 *******************************************************************************
 ** \brief Int No.136 share IRQ handler
 **
 ******************************************************************************/

#if 0

extern am_hc32f460_uart_dev_t __g_uart1_dev;
extern am_hc32f460_uart_dev_t __g_uart2_dev;
extern am_hc32f460_uart_dev_t __g_uart3_dev;
extern am_hc32f460_uart_dev_t __g_uart4_dev;

void IRQ136_Handler(void *parg)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;

    uint32_t int_vssel136 = HC32F460_INTC.VSSEL[136 - 128];

    u32Tmp1 = HC32F460_UART1->SR;
    u32Tmp2 = HC32F460_UART1->CR1;

    /* USART Ch.1 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_EI_MASK))
    {
    	UsartRxErr_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_RI_MASK))
    {
    	UsartRxEnd_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_TI_MASK))
    {
    	UsartTxEmpty_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_TCI_MASK))
    {
    	UsartTxEnd_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_RTO_MASK))
    {
    	UsartRxTO_IrqHandler(&__g_uart1_dev);
    }

    u32Tmp1 = HC32F460_UART2->SR;
    u32Tmp2 = HC32F460_UART2->CR1;

    /* USART Ch.2 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.2 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.2 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.2 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.2 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart2_dev);
    }
}

/**
 *******************************************************************************
 ** \brief Int No.137 share IRQ handler
 **
 ******************************************************************************/
void IRQ137_Handler(void *parg)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t int_vssel137 = HC32F460_INTC.VSSEL[137 - 128];


    u32Tmp1 = HC32F460_UART3->SR;
    u32Tmp2 = HC32F460_UART3->CR1;

    /* USART Ch.3 Receive error */
//    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_EI_MASK))
//    {
//        UsartRxErr_IrqHandler(&__g_uart3_dev);
//        return;
//    }
    /* USART Ch.3 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & (1 << 5)) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart3_dev);
    }
    /* USART Ch.3 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart3_dev);
        return;
    }
    /* USART Ch.3 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart3_dev);
        return;
    }
    /* USART Ch.3 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart3_dev);
        return;
    }

    u32Tmp1 = HC32F460_UART4->SR;
    u32Tmp2 = HC32F460_UART4->CR1;

    /* USART Ch.4 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart4_dev);
    }
}

#endif

extern am_hc32f460_tim_timing_dev_t __g_tim01_timing_dev;
extern am_hc32f460_tim_timing_dev_t __g_tim02_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea1_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea2_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea3_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea4_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea5_timing_dev;
extern am_hc32f460_timea_timing_dev_t __g_timea6_timing_dev;
extern am_hc32f460_tim4_timing_dev_t __g_tim41_timing_dev;
extern am_hc32f460_tim4_timing_dev_t __g_tim42_timing_dev;
extern am_hc32f460_tim4_timing_dev_t __g_tim43_timing_dev;

extern am_hc32f460_adtim_timing_dev_t __g_adtim61_timing_dev;
extern am_hc32f460_adtim_timing_dev_t __g_adtim62_timing_dev;
extern am_hc32f460_adtim_timing_dev_t __g_adtim63_timing_dev;


extern am_hc32f460_timea_cap_dev_t __g_timea1_cap_dev;
extern am_hc32f460_timea_cap_dev_t __g_timea2_cap_dev;
extern am_hc32f460_timea_cap_dev_t __g_timea3_cap_dev;
extern am_hc32f460_timea_cap_dev_t __g_timea4_cap_dev;
extern am_hc32f460_timea_cap_dev_t __g_timea5_cap_dev;
extern am_hc32f460_timea_cap_dev_t __g_timea6_cap_dev;

extern am_hc32f460_adtim_cap_dev_t  __g_adtim61_cap_dev;
extern am_hc32f460_adtim_cap_dev_t  __g_adtim62_cap_dev;
extern am_hc32f460_adtim_cap_dev_t  __g_adtim63_cap_dev;

extern am_hc32f460_i2c_dev_t __g_i2c1_dev;
extern am_hc32f460_i2c_dev_t __g_i2c2_dev;
extern am_hc32f460_i2c_dev_t __g_i2c3_dev;
extern am_hc32f460_rtc_dev_t __g_rtc_dev;
extern am_hc32f460_usbd_dev_t  __g_hc32f460_usbd_msc;
/** \brief Bit mask definition*/
#define     BIT_MASK_00                 (1ul << 0)
#define     BIT_MASK_01                 (1ul << 1)
#define     BIT_MASK_02                 (1ul << 2)
#define     BIT_MASK_03                 (1ul << 3)
#define     BIT_MASK_04                 (1ul << 4)
#define     BIT_MASK_05                 (1ul << 5)
#define     BIT_MASK_06                 (1ul << 6)
#define     BIT_MASK_07                 (1ul << 7)
#define     BIT_MASK_08                 (1ul << 8)
#define     BIT_MASK_09                 (1ul << 9)
#define     BIT_MASK_10                 (1ul << 10)
#define     BIT_MASK_11                 (1ul << 11)
#define     BIT_MASK_12                 (1ul << 12)
#define     BIT_MASK_13                 (1ul << 13)
#define     BIT_MASK_14                 (1ul << 14)
#define     BIT_MASK_15                 (1ul << 15)
#define     BIT_MASK_16                 (1ul << 16)
#define     BIT_MASK_17                 (1ul << 17)
#define     BIT_MASK_18                 (1ul << 18)
#define     BIT_MASK_19                 (1ul << 19)
#define     BIT_MASK_20                 (1ul << 20)
#define     BIT_MASK_21                 (1ul << 21)
#define     BIT_MASK_22                 (1ul << 22)
#define     BIT_MASK_23                 (1ul << 23)
#define     BIT_MASK_24                 (1ul << 24)
#define     BIT_MASK_25                 (1ul << 25)
#define     BIT_MASK_26                 (1ul << 26)
#define     BIT_MASK_27                 (1ul << 27)
#define     BIT_MASK_28                 (1ul << 28)
#define     BIT_MASK_29                 (1ul << 29)
#define     BIT_MASK_30                 (1ul << 30)
#define     BIT_MASK_31                 (1ul << 31)

#define bM4_TMR01_BCONR_INTENA                    (*((volatile unsigned int*)(0x42480208UL)))
#define bM4_TMR01_STFLR_CMAF                      (*((volatile unsigned int*)(0x42480280UL)))
#define bM4_TMR01_BCONR_INTENB                    (*((volatile unsigned int*)(0x42480248UL)))
#define bM4_TMR01_STFLR_CMBF                      (*((volatile unsigned int*)(0x424802C0UL)))
#define bM4_TMR02_BCONR_INTENA                    (*((volatile unsigned int*)(0x42488208UL)))
#define bM4_TMR02_STFLR_CMAF                      (*((volatile unsigned int*)(0x42488280UL)))
#define bM4_TMR02_BCONR_INTENB                    (*((volatile unsigned int*)(0x42488248UL)))
#define bM4_TMR02_STFLR_CMBF                      (*((volatile unsigned int*)(0x424882C0UL)))

extern void hc32f460_swdt_irq_handler (void *p_arg);
extern am_hc32f460_swdt_dev_t __g_swdt_dev;
#define __HC32F460_SWDT_STATUS_UNDERFLOW_MASK    (1 << 16)    /**< \brief 计数溢出标志 */
#define __HC32F460_SWDT_STATUS_REFRESH_MASK      (1 << 17)    /**< \brief 刷新错误标志 */

#define __HC32F460_INTC_VSSEL_SWDT_INT_MASK  (1 << 23)

/**
 *******************************************************************************
 ** \brief Int No.130 share IRQ handler
 **
 ******************************************************************************/
void IRQ130_Handler(void)
{
    uint32_t VSSEL130 = HC32F460_INTC.VSSEL[130 - 128];
    uint32_t u32Tmp1 = 0ul;
    /** \brief Timer0 Ch. 1 A compare match */
    if (Set == bM4_TMR01_BCONR_INTENA)
    {
        if ((Set == bM4_TMR01_STFLR_CMAF) && (VSSEL130 & BIT_MASK_00))
        {
            Timer01GCMA_IrqHandler(&__g_tim01_timing_dev);
        }
    }
    /** \brief Timer0 Ch. 1 B compare match */
    if (Set == bM4_TMR01_BCONR_INTENB)
    {
        if ((Set == bM4_TMR01_STFLR_CMBF) && (VSSEL130 & BIT_MASK_01))
        {
            Timer01GCMB_IrqHandler(&__g_tim01_timing_dev);
        }
    }
    /** \brief Timer0 Ch. 2 A compare match */
    if (Set == bM4_TMR02_BCONR_INTENA)
    {
        if ((Set == bM4_TMR02_STFLR_CMAF) && (VSSEL130 & BIT_MASK_02))
        {
            Timer02GCMA_IrqHandler(&__g_tim02_timing_dev);
        }
    }
    /** \brief Timer0 Ch. 2 B compare match */
    if (Set == bM4_TMR02_BCONR_INTENB)
    {
        if ((Set == bM4_TMR02_STFLR_CMBF) && (VSSEL130 & BIT_MASK_03))
        {
            Timer02GCMB_IrqHandler(&__g_tim02_timing_dev);
        }
    }
    
    u32Tmp1 = HC32F460_RTC->CR2;

    if ((u32Tmp1 & BIT_MASK_06) && (u32Tmp1 & BIT_MASK_03) && (VSSEL130 & BIT_MASK_17))
    {
        RTC_ALM_IrqHandler(&__g_rtc_dev);
    }
    if ((u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_02) && (VSSEL130 & BIT_MASK_18))
    {
        RTC_PRD_IrqHandler(&__g_rtc_dev);
    }
#if 0
    /* Main-OSC stop */
    if (Set == bM4_SYSREG_CMU_XTALSTDCR_XTALSTDIE)
    {
        if ((Set == bM4_SYSREG_CMU_XTALSTDSR_XTALSTDF) && (VSSEL130 & BIT_MASK_21))
        {
            MainOscStop_IrqHandler();
        }
    }
    /* Wakeup timer */
    if ((Set == bM4_WKTM_CR_WKOVF) && (VSSEL130 & BIT_MASK_22))
    {
        WakeupTimer_IrqHandler();
    }
#endif
    /** \brief SWDT */
    if ((HC32F460_SWDT->SR & (__HC32F460_SWDT_STATUS_UNDERFLOW_MASK | __HC32F460_SWDT_STATUS_REFRESH_MASK)) &&
        (VSSEL130 & __HC32F460_INTC_VSSEL_SWDT_INT_MASK))
    {
        hc32f460_swdt_irq_handler(&__g_swdt_dev);
    }
}
#define bM4_TMR61_ICONR_INTENA                    (*((volatile unsigned int*)(0x42300A80UL)))
#define bM4_TMR61_ICONR_INTENB                    (*((volatile unsigned int*)(0x42300A84UL)))
#define bM4_TMR61_ICONR_INTENC                    (*((volatile unsigned int*)(0x42300A88UL)))
#define bM4_TMR61_ICONR_INTEND                    (*((volatile unsigned int*)(0x42300A8CUL)))
#define bM4_TMR61_ICONR_INTENE                    (*((volatile unsigned int*)(0x42300A90UL)))
#define bM4_TMR61_ICONR_INTENF                    (*((volatile unsigned int*)(0x42300A94UL)))
#define bM4_TMR61_ICONR_INTENOVF                  (*((volatile unsigned int*)(0x42300A98UL)))
#define bM4_TMR61_ICONR_INTENUDF                  (*((volatile unsigned int*)(0x42300A9CUL)))
#define bM4_TMR61_ICONR_INTENDTE                  (*((volatile unsigned int*)(0x42300AA0UL)))

#define bM4_TMR61_STFLR_CMAF                      (*((volatile unsigned int*)(0x42300E00UL)))
#define bM4_TMR61_STFLR_CMBF                      (*((volatile unsigned int*)(0x42300E04UL)))
#define bM4_TMR61_STFLR_CMCF                      (*((volatile unsigned int*)(0x42300E08UL)))
#define bM4_TMR61_STFLR_CMDF                      (*((volatile unsigned int*)(0x42300E0CUL)))
#define bM4_TMR61_STFLR_CMEF                      (*((volatile unsigned int*)(0x42300E10UL)))
#define bM4_TMR61_STFLR_CMFF                      (*((volatile unsigned int*)(0x42300E14UL)))
#define bM4_TMR61_STFLR_OVFF                      (*((volatile unsigned int*)(0x42300E18UL)))
#define bM4_TMR61_STFLR_UDFF                      (*((volatile unsigned int*)(0x42300E1CUL)))
#define bM4_TMR61_STFLR_DTEF                      (*((volatile unsigned int*)(0x42300E20UL)))

#define bM4_TMR62_ICONR_INTENA                    (*((volatile unsigned int*)(0x42308A80UL)))
#define bM4_TMR62_ICONR_INTENB                    (*((volatile unsigned int*)(0x42308A84UL)))
#define bM4_TMR62_ICONR_INTENC                    (*((volatile unsigned int*)(0x42308A88UL)))
#define bM4_TMR62_ICONR_INTEND                    (*((volatile unsigned int*)(0x42308A8CUL)))
#define bM4_TMR62_ICONR_INTENE                    (*((volatile unsigned int*)(0x42308A90UL)))
#define bM4_TMR62_ICONR_INTENF                    (*((volatile unsigned int*)(0x42308A94UL)))
#define bM4_TMR62_ICONR_INTENOVF                  (*((volatile unsigned int*)(0x42308A98UL)))
#define bM4_TMR62_ICONR_INTENUDF                  (*((volatile unsigned int*)(0x42308A9CUL)))
#define bM4_TMR62_ICONR_INTENDTE                  (*((volatile unsigned int*)(0x42308AA0UL)))


#define bM4_TMR62_STFLR_CMAF                      (*((volatile unsigned int*)(0x42308E00UL)))
#define bM4_TMR62_STFLR_CMBF                      (*((volatile unsigned int*)(0x42308E04UL)))
#define bM4_TMR62_STFLR_CMCF                      (*((volatile unsigned int*)(0x42308E08UL)))
#define bM4_TMR62_STFLR_CMDF                      (*((volatile unsigned int*)(0x42308E0CUL)))
#define bM4_TMR62_STFLR_CMEF                      (*((volatile unsigned int*)(0x42308E10UL)))
#define bM4_TMR62_STFLR_CMFF                      (*((volatile unsigned int*)(0x42308E14UL)))
#define bM4_TMR62_STFLR_OVFF                      (*((volatile unsigned int*)(0x42308E18UL)))
#define bM4_TMR62_STFLR_UDFF                      (*((volatile unsigned int*)(0x42308E1CUL)))
#define bM4_TMR62_STFLR_DTEF                      (*((volatile unsigned int*)(0x42308E20UL)))

#define bM4_TMR63_ICONR_INTENA                    (*((volatile unsigned int*)(0x42310A80UL)))
#define bM4_TMR63_ICONR_INTENB                    (*((volatile unsigned int*)(0x42310A84UL)))
#define bM4_TMR63_ICONR_INTENC                    (*((volatile unsigned int*)(0x42310A88UL)))
#define bM4_TMR63_ICONR_INTEND                    (*((volatile unsigned int*)(0x42310A8CUL)))
#define bM4_TMR63_ICONR_INTENE                    (*((volatile unsigned int*)(0x42310A90UL)))
#define bM4_TMR63_ICONR_INTENF                    (*((volatile unsigned int*)(0x42310A94UL)))
#define bM4_TMR63_ICONR_INTENOVF                  (*((volatile unsigned int*)(0x42310A98UL)))
#define bM4_TMR63_ICONR_INTENUDF                  (*((volatile unsigned int*)(0x42310A9CUL)))
#define bM4_TMR63_ICONR_INTENDTE                  (*((volatile unsigned int*)(0x42310AA0UL)))

#define bM4_TMR63_STFLR_CMAF                      (*((volatile unsigned int*)(0x42310E00UL)))
#define bM4_TMR63_STFLR_CMBF                      (*((volatile unsigned int*)(0x42310E04UL)))
#define bM4_TMR63_STFLR_CMCF                      (*((volatile unsigned int*)(0x42310E08UL)))
#define bM4_TMR63_STFLR_CMDF                      (*((volatile unsigned int*)(0x42310E0CUL)))
#define bM4_TMR63_STFLR_CMEF                      (*((volatile unsigned int*)(0x42310E10UL)))
#define bM4_TMR63_STFLR_CMFF                      (*((volatile unsigned int*)(0x42310E14UL)))
#define bM4_TMR63_STFLR_OVFF                      (*((volatile unsigned int*)(0x42310E18UL)))
#define bM4_TMR63_STFLR_UDFF                      (*((volatile unsigned int*)(0x42310E1CUL)))
#define bM4_TMR63_STFLR_DTEF                      (*((volatile unsigned int*)(0x42310E20UL)))

#define bM4_USBFS_GAHBCFG_GINTMSK                 (*((volatile unsigned int*)(0x43800100UL)))
/**
 *******************************************************************************
 ** \brief Int No.131 share IRQ handler
 **
 ******************************************************************************/
void IRQ131_Handler(void)
{
    uint32_t VSSEL131 = HC32F460_INTC.VSSEL[131 - 128];
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    /** \brief Timer6 Ch.1 A compare match */
    if (Set == bM4_TMR61_ICONR_INTENA)
    {
        if ((Set == bM4_TMR61_STFLR_CMAF) && (VSSEL131 & BIT_MASK_00))
        {
            Timer61GCMA_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /** \brief Timer6 Ch.1 B compare match */
    if (Set == bM4_TMR61_ICONR_INTENB)
    {
        if ((Set == bM4_TMR61_STFLR_CMBF) && (VSSEL131 & BIT_MASK_01))
        {
            Timer61GCMB_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /** \brief Timer6 Ch.1 C compare match */
    if (Set == bM4_TMR61_ICONR_INTENC)
    {
        if ((Set == bM4_TMR61_STFLR_CMCF) && (VSSEL131 & BIT_MASK_02))
        {
            Timer61GCMC_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /** \brief Timer6 Ch.1 D compare match */
    if (Set == bM4_TMR61_ICONR_INTEND)
    {
        if ((Set == bM4_TMR61_STFLR_CMDF) && (VSSEL131 & BIT_MASK_03))
        {
            Timer61GCMD_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /** \brief Timer6 Ch.1 E compare match */
    if (Set == bM4_TMR61_ICONR_INTENE)
    {
        if ((Set == bM4_TMR61_STFLR_CMEF) && (VSSEL131 & BIT_MASK_04))
        {
            Timer61GCME_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /** \brief Timer6 Ch.1 F compare match */
    if (Set == bM4_TMR61_ICONR_INTENF)
    {
        if ((Set == bM4_TMR61_STFLR_CMFF) && (VSSEL131 & BIT_MASK_05))
        {
            Timer61GCMF_IrqHandler(&__g_adtim61_cap_dev);
        }
    }
    /** \brief Timer6 Ch.1 overflow */
    if (Set == bM4_TMR61_ICONR_INTENOVF)
    {
        if ((Set == bM4_TMR61_STFLR_OVFF) && (VSSEL131 & BIT_MASK_06))
        {
            Timer61GOV_IrqHandler(&__g_adtim61_timing_dev);
        }
    }
    /** \brief Timer6 Ch.1 underflow */
    if (Set == bM4_TMR61_ICONR_INTENUDF)
    {
        if ((Set == bM4_TMR61_STFLR_UDFF) && (VSSEL131 & BIT_MASK_07))
        {
            Timer61GUD_IrqHandler(&__g_adtim61_timing_dev);
        }
    }
    /** \brief Timer6 Ch.1 dead time */
    if (Set == bM4_TMR61_ICONR_INTENDTE)
    {
        if (((Set == bM4_TMR61_STFLR_DTEF)) && (VSSEL131 & BIT_MASK_08))
        {
            Timer61GDT_IrqHandler(&__g_adtim61_timing_dev);
        }
    }
    /** \brief Timer6 Ch.1 A up-down compare match */
    u32Tmp1 = (HC32F460_TMR61->ICONR & (BIT_MASK_16 | BIT_MASK_17)) >> 7u;
    u32Tmp2 = HC32F460_TMR61->STFLR & (BIT_MASK_09 | BIT_MASK_10);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_11))
    {
        Timer61SCMA_IrqHandler(&__g_adtim61_timing_dev);
    }
    /** \brief Timer6 Ch.1 B up-down compare match */
    u32Tmp1 = (HC32F460_TMR61->ICONR & (BIT_MASK_18 | BIT_MASK_19)) >> 7u;
    u32Tmp2 = HC32F460_TMR61->STFLR & (BIT_MASK_11 | BIT_MASK_12);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_12))
    {
        Timer61SCMB_IrqHandler(&__g_adtim61_timing_dev);
    }
    /** \brief Timer6 Ch.2 A compare match */
    if (Set == bM4_TMR62_ICONR_INTENA)
    {
        if ((Set == bM4_TMR62_STFLR_CMAF) && (VSSEL131 & BIT_MASK_16))
        {
            Timer62GCMA_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /** \brief Timer6 Ch.2 B compare match */
    if (Set == bM4_TMR62_ICONR_INTENB)
    {
        if ((Set == bM4_TMR62_STFLR_CMBF) && (VSSEL131 & BIT_MASK_17))
        {
            Timer62GCMB_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /** \brief Timer6 Ch.2 C compare match */
    if (Set == bM4_TMR62_ICONR_INTENC)
    {
        if ((Set == bM4_TMR62_STFLR_CMCF) && (VSSEL131 & BIT_MASK_18))
        {
            Timer62GCMC_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /** \brief Timer6 Ch.2 D compare match */
    if (Set == bM4_TMR62_ICONR_INTEND)
    {
        if ((Set == bM4_TMR62_STFLR_CMDF) && (VSSEL131 & BIT_MASK_19))
        {
            Timer62GCMD_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /** \brief Timer6 Ch.2 E compare match */
    if (Set == bM4_TMR62_ICONR_INTENE)
    {
        if ((Set == bM4_TMR62_STFLR_CMEF) && (VSSEL131 & BIT_MASK_20))
        {
            Timer62GCME_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /** \brief Timer6 Ch.2 F compare match */
    if (Set == bM4_TMR62_ICONR_INTENF)
    {
        if ((Set == bM4_TMR62_STFLR_CMFF) && (VSSEL131 & BIT_MASK_21))
        {
            Timer62GCMF_IrqHandler(&__g_adtim62_cap_dev);
        }
    }
    /** \brief Timer6 Ch.2 overflow */
    if (Set == bM4_TMR62_ICONR_INTENOVF)
    {
        if ((Set == bM4_TMR62_STFLR_OVFF) && (VSSEL131 & BIT_MASK_22))
        {
            Timer62GOV_IrqHandler(&__g_adtim62_timing_dev);
        }
    }
    /** \brief Timer6 Ch.2 underflow */
    if (Set == bM4_TMR62_ICONR_INTENUDF)
    {
        if ((Set == bM4_TMR62_STFLR_UDFF) && (VSSEL131 & BIT_MASK_23))
        {
            Timer62GUD_IrqHandler(&__g_adtim62_timing_dev);
        }
    }
    /** \brief Timer6 Ch.2 dead time */
    if (Set == bM4_TMR62_ICONR_INTENDTE)
    {
        if (((Set == bM4_TMR62_STFLR_DTEF)) && (VSSEL131 & BIT_MASK_24))
        {
            Timer62GDT_IrqHandler(&__g_adtim62_timing_dev);
        }
    }
    /** \brief Timer6 Ch.2 A up-down compare match */
    u32Tmp1 = (HC32F460_TMR62->ICONR & (BIT_MASK_16 | BIT_MASK_17)) >> 7u;
    u32Tmp2 = HC32F460_TMR62->STFLR & (BIT_MASK_09 | BIT_MASK_10);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_27))
    {
        Timer62SCMA_IrqHandler(&__g_adtim62_timing_dev);
    }
    /** \brief Timer6 Ch.2 B up-down compare match */
    u32Tmp1 = (HC32F460_TMR62->ICONR & (BIT_MASK_18 | BIT_MASK_19)) >> 7u;
    u32Tmp2 = HC32F460_TMR62->STFLR & (BIT_MASK_11 | BIT_MASK_12);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_28))
    {
        Timer62SCMB_IrqHandler(&__g_adtim62_timing_dev);
    }
}

/**
 *******************************************************************************
 ** \brief Int No.132 share IRQ handler
 **
 ******************************************************************************/
void IRQ132_Handler(void)
{
    uint32_t VSSEL132 = HC32F460_INTC.VSSEL[132 - 128];
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    /** \brief Timer6 Ch.3 A compare match */
    if (Set == bM4_TMR63_ICONR_INTENA)
    {
        if ((Set == bM4_TMR63_STFLR_CMAF) && (VSSEL132 & BIT_MASK_00))
        {
            Timer63GCMA_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /** \brief Timer6 Ch.3 B compare match */
    if (Set == bM4_TMR63_ICONR_INTENB)
    {
        if ((Set == bM4_TMR63_STFLR_CMBF) && (VSSEL132 & BIT_MASK_01))
        {
            Timer63GCMB_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /** \brief Timer6 Ch.3 C compare match */
    if (Set == bM4_TMR63_ICONR_INTENC)
    {
        if ((Set == bM4_TMR63_STFLR_CMCF) && (VSSEL132 & BIT_MASK_02))
        {
            Timer63GCMC_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /** \brief Timer6 Ch.3 D compare match */
    if (Set == bM4_TMR63_ICONR_INTEND)
    {
        if ((Set == bM4_TMR63_STFLR_CMDF) && (VSSEL132 & BIT_MASK_03))
        {
            Timer63GCMD_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /** \brief Timer6 Ch.3 E compare match */
    if (Set == bM4_TMR63_ICONR_INTENE)
    {
        if ((Set == bM4_TMR63_STFLR_CMEF) && (VSSEL132 & BIT_MASK_04))
        {
            Timer63GCME_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /** \brief Timer6 Ch.3 F compare match */
    if (Set == bM4_TMR63_ICONR_INTENF)
    {
        if ((Set == bM4_TMR63_STFLR_CMFF) && (VSSEL132 & BIT_MASK_05))
        {
            Timer63GCMF_IrqHandler(&__g_adtim63_cap_dev);
        }
    }
    /** \brief Timer6 Ch.3 overflow */
    if (Set == bM4_TMR63_ICONR_INTENOVF)
    {
        if ((Set == bM4_TMR63_STFLR_OVFF) && (VSSEL132 & BIT_MASK_06))
        {
            Timer63GOV_IrqHandler(&__g_adtim63_timing_dev);
        }
    }
    /** \brief Timer6 Ch.3 underflow */
    if (Set == bM4_TMR63_ICONR_INTENUDF)
    {
        if ((Set == bM4_TMR63_STFLR_UDFF) && (VSSEL132 & BIT_MASK_07))
        {
            Timer63GUD_IrqHandler(&__g_adtim63_timing_dev);
        }
    }
    /** \brief Timer6 Ch.3 dead time */
    if (Set == bM4_TMR63_ICONR_INTENDTE)
    {
        if (((Set == bM4_TMR63_STFLR_DTEF)) && (VSSEL132 & BIT_MASK_08))
        {
            Timer63GDT_IrqHandler(&__g_adtim63_timing_dev);
        }
    }
    /** \brief Timer6 Ch.3 A up-down compare match */
    u32Tmp1 = (HC32F460_TMR63->ICONR & (BIT_MASK_16 | BIT_MASK_17)) >> 7u;
    u32Tmp2 = HC32F460_TMR63->STFLR & (BIT_MASK_09 | BIT_MASK_10);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL132 & BIT_MASK_11))
    {
        Timer63SCMA_IrqHandler(&__g_adtim63_timing_dev);
    }
    /** \brief Timer6 Ch.3 B up-down compare match */
    u32Tmp1 = (HC32F460_TMR63->ICONR & (BIT_MASK_18 | BIT_MASK_19)) >> 7u;
    u32Tmp2 = HC32F460_TMR63->STFLR & (BIT_MASK_11 | BIT_MASK_12);
    if ((u32Tmp1 & u32Tmp2) && (VSSEL132 & BIT_MASK_12))
    {
        Timer63SCMB_IrqHandler(&__g_adtim63_timing_dev);
    }
}

/**
 *******************************************************************************
 ** \brief Int No.136 share IRQ handler
 **
 ******************************************************************************/
void IRQ136_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;

    uint32_t int_vssel136 = HC32F460_INTC.VSSEL[136 - 128];
    
#if 0
    u32Tmp1 = HC32F460_UART1->SR;
    u32Tmp2 = HC32F460_UART1->CR1;
 
    /* USART Ch.1 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart1_dev);
    }
    /* USART Ch.1 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART1_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart1_dev);
    }

    u32Tmp1 = HC32F460_UART2->SR;
    u32Tmp2 = HC32F460_UART2->CR1;

    /* USART Ch.1 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.1 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.1 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.1 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart2_dev);
    }
    /* USART Ch.1 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel136 & __HC32F460_INTC_VSSEL_USART2_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart2_dev);
    }
#endif

    u32Tmp1 = HC32F460_TMRA1->BCSTR;
    /** \brief TimerA Ch.1 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_00))
    {
        TimerA1OV_IrqHandler(&__g_timea1_timing_dev);
    }
    /** \brief TimerA Ch.1 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_01))
    {
        TimerA1UD_IrqHandler(&__g_timea1_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA1->ICONR;
    u32Tmp2 = HC32F460_TMRA1->STFLR;
    /** \brief TimerA Ch.1 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_02))
    {
        TimerA1CMP_IrqHandler(&__g_timea1_cap_dev);
    }

    u32Tmp1 = HC32F460_TMRA2->BCSTR;
    /** \brief TimerA Ch.2 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_03))
    {
        TimerA2OV_IrqHandler(&__g_timea2_timing_dev);
    }
    /** \brief TimerA Ch.2 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_04))
    {
        TimerA2UD_IrqHandler(&__g_timea2_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA2->ICONR;
    u32Tmp2 = HC32F460_TMRA2->STFLR;
    /** \brief TimerA Ch.2 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_05))
    {
        TimerA2CMP_IrqHandler(&__g_timea2_cap_dev);
    }

    u32Tmp1 = HC32F460_TMRA3->BCSTR;
    /** \brief TimerA Ch.3 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_06))
    {
        TimerA3OV_IrqHandler(&__g_timea3_timing_dev);
    }
    /** \brief TimerA Ch.3 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_07))
    {
        TimerA3UD_IrqHandler(&__g_timea3_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA3->ICONR;
    u32Tmp2 = HC32F460_TMRA3->STFLR;
    /** \brief TimerA Ch.3 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_08))
    {
        TimerA3CMP_IrqHandler(&__g_timea3_cap_dev);
    }

    u32Tmp1 = HC32F460_TMRA4->BCSTR;
    /** \brief TimerA Ch.4 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_09))
    {
        TimerA4OV_IrqHandler(&__g_timea4_timing_dev);
    }
    /** \brief TimerA Ch.4 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_10))
    {
        TimerA4UD_IrqHandler(&__g_timea4_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA4->ICONR;
    u32Tmp2 = HC32F460_TMRA4->STFLR;
    /** \brief TimerA Ch.4 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_11))
    {
        TimerA4CMP_IrqHandler(&__g_timea4_cap_dev);
    }

    u32Tmp1 = HC32F460_TMRA5->BCSTR;
    /** \brief TimerA Ch.5 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_12))
    {
        TimerA5OV_IrqHandler(&__g_timea5_timing_dev);
    }
    /** \brief TimerA Ch.5 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_13))
    {
        TimerA5UD_IrqHandler(&__g_timea5_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA5->ICONR;
    u32Tmp2 = HC32F460_TMRA5->STFLR;
    /** \brief TimerA Ch.5 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_14))
    {
        TimerA5CMP_IrqHandler(&__g_timea5_cap_dev);
    }

    u32Tmp1 = HC32F460_TMRA6->BCSTR;
    /** \brief TimerA Ch.6 overflow */
    if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (int_vssel136 & BIT_MASK_16))
    {
        TimerA6OV_IrqHandler(&__g_timea6_timing_dev);
    }
    /** \brief TimerA Ch.6 underflow */
    if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (int_vssel136 & BIT_MASK_17))
    {
        TimerA6UD_IrqHandler(&__g_timea6_timing_dev);
    }
    u32Tmp1 = HC32F460_TMRA6->ICONR;
    u32Tmp2 = HC32F460_TMRA6->STFLR;
    /** \brief TimerA Ch.6 compare match */
    if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (int_vssel136 & BIT_MASK_18))
    {
        TimerA6CMP_IrqHandler(&__g_timea6_cap_dev);
    }

    /** \brief USBFS global interrupt */
    if(Set == bM4_USBFS_GAHBCFG_GINTMSK)
    {
        u32Tmp1 = HC32F460_USB->GINTMSK & 0xF77CFCFBul;
        u32Tmp2 = HC32F460_USB->GINTSTS & 0xF77CFCFBul;
        if ((u32Tmp1 & u32Tmp2) && (int_vssel136 & BIT_MASK_19))
        {
            UsbGlobal_IrqHandler(&__g_hc32f460_usbd_msc);
        }
    }
}
extern void __spi_irq_handler (void *p_arg);
extern am_hc32f460_spi_int_dev_t __g_spi1_int_dev;
extern am_hc32f460_spi_int_dev_t __g_spi2_int_dev;
extern am_hc32f460_spi_int_dev_t __g_spi3_int_dev;
extern am_hc32f460_spi_int_dev_t __g_spi4_int_dev;
/**
 *******************************************************************************
 ** \brief Int No.137 share IRQ handler
 **
 ******************************************************************************/
void IRQ137_Handler(void *parg)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t int_vssel137 = HC32F460_INTC.VSSEL[137 - 128];

#if 0
    u32Tmp1 = HC32F460_UART3->SR;
    u32Tmp2 = HC32F460_UART3->CR1;

    /* USART Ch.3 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart3_dev);
        return;
    }
    /* USART Ch.3 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & (1 << 5)) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart3_dev);
    }
    /* USART Ch.3 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart3_dev);
        return;
    }
    /* USART Ch.3 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart3_dev);
        return;
    }
    /* USART Ch.3 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART3_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart3_dev);
        return;
    }

    u32Tmp1 = HC32F460_UART4->SR;
    u32Tmp2 = HC32F460_UART4->CR1;

    /* USART Ch.4 Receive error */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RX_ERROR_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_EI_MASK))
    {
        UsartRxErr_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Receive completed */
    if ((u32Tmp2 & u32Tmp1 & __HC32F460_UART_CR1_RIE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_RI_MASK))
    {
        UsartRxEnd_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Transmit data empty */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TXEIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TXE_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_TI_MASK))
    {
        UsartTxEmpty_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Transmit completed */
    if ((u32Tmp2 & __HC32F460_UART_CR1_TCIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_TC_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_TCI_MASK))
    {
        UsartTxEnd_IrqHandler(&__g_uart4_dev);
    }
    /* USART Ch.4 Receive timeout */
    if ((u32Tmp2 & __HC32F460_UART_CR1_RTOIE_MASK) && (u32Tmp1 & __HC32F460_UART_SR_RTOF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_USART4_RTO_MASK))
    {
        UsartRxTO_IrqHandler(&__g_uart4_dev);
    }
#endif
    u32Tmp1 = HC32F460_SPI1->CR1;
    u32Tmp2 = HC32F460_SPI1->SR;
    /** \brief SPI Ch.1 Receive completed */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_RXIE_MASK) && (u32Tmp2 & __HC32F460_SPI_SR_RDFF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI1_SRRI_MASK))
     {
        __spi_irq_handler(&__g_spi1_int_dev);
     }
     /** \brief SPI Ch.1 Transmit buf empty */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_TXIE_MASK) && (u32Tmp2 & __HC32F460_SPI_SR_TDEF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI1_SRTI_MASK))
     {
        __spi_irq_handler(&__g_spi1_int_dev);
     }
     /** \brief SPI Ch.1 bus idle */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_IDIE_MASK) && (!(u32Tmp2 & __HC32F460_SPI_SR_IDLNF_MASK)) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI1_SPII_MASK))
     {
        __spi_irq_handler(&__g_spi1_int_dev);
     }
     /** \brief SPI Ch.1 parity/overflow/underflow/mode error */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_EIE_MASK)         &&  \
         ((u32Tmp2 & (__HC32F460_SPI_SR_OVRERF_MASK  |
                      __HC32F460_SPI_SR_MODFERF_MASK |
                      __HC32F460_SPI_SR_PERF_MASK    |
                      __HC32F460_SPI_SR_UDRERF_MASK)))   &&  \
         (int_vssel137 & __HC32F460_INTC_VSSEL_SPI1_SPEI_MASK))
     {
        __spi_irq_handler(&__g_spi1_int_dev);
     }

     u32Tmp1 = HC32F460_SPI2->CR1;
     u32Tmp2 = HC32F460_SPI2->SR;
     /** \brief SPI Ch.2 Receive completed */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_RXIE_MASK) && (u32Tmp2 & __HC32F460_SPI_SR_RDFF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI2_SRRI_MASK))
     {
        __spi_irq_handler(&__g_spi2_int_dev);
     }
     /** \brief SPI Ch.2 Transmit buf empty */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_TXIE_MASK) && (u32Tmp2 & __HC32F460_SPI_SR_TDEF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI2_SRTI_MASK))
     {
        __spi_irq_handler(&__g_spi2_int_dev);
     }
     /** \brief SPI Ch.2 bus idle */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_IDIE_MASK) && (!(u32Tmp2 & __HC32F460_SPI_SR_IDLNF_MASK)) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI2_SPII_MASK))
     {
        __spi_irq_handler(&__g_spi2_int_dev);
     }
     /** \brief SPI Ch.2 parity/overflow/underflow/mode error */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_EIE_MASK)         &&  \
         ((u32Tmp2 & (__HC32F460_SPI_SR_OVRERF_MASK  |
                      __HC32F460_SPI_SR_MODFERF_MASK |
                      __HC32F460_SPI_SR_PERF_MASK    |
                      __HC32F460_SPI_SR_UDRERF_MASK)))   &&  \
         (int_vssel137 & __HC32F460_INTC_VSSEL_SPI2_SPEI_MASK))
     {
        __spi_irq_handler(&__g_spi2_int_dev);
     }

     u32Tmp1 = HC32F460_SPI3->CR1;
     u32Tmp2 = HC32F460_SPI3->SR;
     /** \brief SPI Ch.3 Receive completed */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_RXIE_MASK) && (u32Tmp2 & __HC32F460_SPI_SR_RDFF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI3_SRRI_MASK))
     {
        __spi_irq_handler(&__g_spi3_int_dev);
     }
     /** \brief SPI Ch.3 Transmit buf empty */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_TXIE_MASK) && (u32Tmp2 & __HC32F460_SPI_SR_TDEF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI3_SRTI_MASK))
     {
        __spi_irq_handler(&__g_spi3_int_dev);
     }
     /** \brief SPI Ch.3 bus idle */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_IDIE_MASK) && (!(u32Tmp2 & __HC32F460_SPI_SR_IDLNF_MASK)) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI3_SPII_MASK))
     {
        __spi_irq_handler(&__g_spi3_int_dev);
     }
     /** \brief SPI Ch.3 parity/overflow/underflow/mode error */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_EIE_MASK)         &&  \
         ((u32Tmp2 & (__HC32F460_SPI_SR_OVRERF_MASK  |
                      __HC32F460_SPI_SR_MODFERF_MASK |
                      __HC32F460_SPI_SR_PERF_MASK    |
                      __HC32F460_SPI_SR_UDRERF_MASK)))   &&  \
         (int_vssel137 & __HC32F460_INTC_VSSEL_SPI3_SPEI_MASK))
     {
       __spi_irq_handler(&__g_spi3_int_dev);
     }

     u32Tmp1 = HC32F460_SPI4->CR1;
     u32Tmp2 = HC32F460_SPI4->SR;
     /** \brief SPI Ch.4 Receive completed */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_RXIE_MASK) && (u32Tmp2 & __HC32F460_SPI_SR_RDFF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI4_SRRI_MASK))
     {
        __spi_irq_handler(&__g_spi4_int_dev);
     }
     /** \brief SPI Ch.4 Transmit buf empty */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_TXIE_MASK) && (u32Tmp2 & __HC32F460_SPI_SR_TDEF_MASK) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI4_SRTI_MASK))
     {
        __spi_irq_handler(&__g_spi4_int_dev);
     }
     /** \brief SPI Ch.4 bus idle */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_IDIE_MASK) && (!(u32Tmp2 & __HC32F460_SPI_SR_IDLNF_MASK)) && (int_vssel137 & __HC32F460_INTC_VSSEL_SPI3_SPII_MASK))
     {
        __spi_irq_handler(&__g_spi4_int_dev);
     }
     /** \brief SPI Ch.4 parity/overflow/underflow/mode error */
     if ((u32Tmp1 & __HC32F460_SPI_CR1_EIE_MASK)         &&  \
         ((u32Tmp2 & (__HC32F460_SPI_SR_OVRERF_MASK  |
                      __HC32F460_SPI_SR_MODFERF_MASK |
                      __HC32F460_SPI_SR_PERF_MASK    |
                      __HC32F460_SPI_SR_UDRERF_MASK)))   &&  \
         (int_vssel137 & __HC32F460_INTC_VSSEL_SPI4_SPEI_MASK))
     {
        __spi_irq_handler(&__g_spi4_int_dev);
     }
}


/**
 *******************************************************************************
 ** \brief Int No.138 share IRQ handler
 **
 ******************************************************************************/
void IRQ138_Handler(void)
{
    uint32_t u32Tmp1 = 0u;
    uint32_t VSSEL138 = HC32F460_INTC.VSSEL[138 - 128];

    u32Tmp1 = HC32F460_TMR41->OCSRU;
    /** \brief Timer4 Ch.1 U phase higher compare match */
    if ((VSSEL138 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer41GCMUH_IrqHandler(&__g_tim41_timing_dev);
    }
    /** \brief Timer4 Ch.1 U phase lower compare match */
    if ((VSSEL138 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer41GCMUL_IrqHandler(&__g_tim41_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR41->OCSRV;
    /** \brief Timer4 Ch.1 V phase higher compare match */
    if ((VSSEL138 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer41GCMVH_IrqHandler(&__g_tim41_timing_dev);
    }
    /** \brief Timer4 Ch.1 V phase lower compare match */
    if ((VSSEL138 & BIT_MASK_03) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer41GCMVL_IrqHandler(&__g_tim41_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR41->OCSRW;
    /** \brief Timer4 Ch.1 W phase higher compare match */
    if ((VSSEL138 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer41GCMWH_IrqHandler(&__g_tim41_timing_dev);
    }
    /** \brief Timer4 Ch.1 W phase lower compare match */
    if ((VSSEL138 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer41GCMWL_IrqHandler(&__g_tim41_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR41->CCSR;
    /** \brief Timer4 Ch.1 overflow */
    if ((VSSEL138 & BIT_MASK_06) && (u32Tmp1 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_09))
    {
        Timer41GOV_IrqHandler(&__g_tim41_timing_dev);
    }
    /** \brief Timer4 Ch.1 underflow */
    if ((VSSEL138 & BIT_MASK_07) && (u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_14))
    {
        Timer41GUD_IrqHandler(&__g_tim41_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR41->RCSR;
    /** \brief Timer4 Ch.1 U phase reload */
    if ((VSSEL138 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04))
    {
        Timer41ReloadU_IrqHandler(&__g_tim41_timing_dev);
    }
    /** \brief Timer4 Ch.1 V phase reload */
    if ((VSSEL138 & BIT_MASK_09) && (u32Tmp1 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_08))
    {
        Timer41ReloadV_IrqHandler(&__g_tim41_timing_dev);
    }
    /** \brief Timer4 Ch.1 W phase reload */
    if ((VSSEL138 & BIT_MASK_10) && (u32Tmp1 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_12))
    {
        Timer41ReloadW_IrqHandler(&__g_tim41_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR42->OCSRU;
    /** \brief Timer4 Ch.2 U phase higher compare match */
    if ((VSSEL138 & BIT_MASK_16) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer42GCMUH_IrqHandler(&__g_tim42_timing_dev);
    }
    /** \brief Timer4 Ch.2 U phase lower compare match */
    if ((VSSEL138 & BIT_MASK_17) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer42GCMUL_IrqHandler(&__g_tim42_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR42->OCSRV;
    /** \brief Timer4 Ch.2 V phase higher compare match */
    if ((VSSEL138 & BIT_MASK_18) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer42GCMVH_IrqHandler(&__g_tim42_timing_dev);
    }
    /** \brief Timer4 Ch.2 V phase lower compare match */
    if ((VSSEL138 & BIT_MASK_19) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer42GCMVL_IrqHandler(&__g_tim42_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR42->OCSRW;
    /** \brief Timer4 Ch.2 W phase higher compare match */
    if ((VSSEL138 & BIT_MASK_20) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer42GCMWH_IrqHandler(&__g_tim42_timing_dev);
    }
    /** \brief Timer4 Ch.2 W phase lower compare match */
    if ((VSSEL138 & BIT_MASK_21) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer42GCMWL_IrqHandler(&__g_tim42_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR42->CCSR;
    /** \brief Timer4 Ch.2 overflow */
    if ((VSSEL138 & BIT_MASK_22) && (u32Tmp1 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_09))
    {
        Timer42GOV_IrqHandler(&__g_tim42_timing_dev);
    }
    /** \brief Timer4 Ch.2 underflow */
    if ((VSSEL138 & BIT_MASK_23) && (u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_14))
    {
        Timer42GUD_IrqHandler(&__g_tim42_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR42->RCSR;
    /** \brief Timer4 Ch.2 U phase reload */
    if ((VSSEL138 & BIT_MASK_24) && (u32Tmp1 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04))
    {
        Timer42ReloadU_IrqHandler(&__g_tim42_timing_dev);
    }
    /** \brief Timer4 Ch.2 V phase reload */
    if ((VSSEL138 & BIT_MASK_25) && (u32Tmp1 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_08))
    {
        Timer42ReloadV_IrqHandler(&__g_tim42_timing_dev);
    }
    /** \brief Timer4 Ch.2 W phase reload */
    if ((VSSEL138 & BIT_MASK_26) && (u32Tmp1 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_12))
    {
        Timer42ReloadW_IrqHandler(&__g_tim42_timing_dev);
    }
}

/**
 *******************************************************************************
 ** \brief Int No.139 share IRQ handler
 **
 ******************************************************************************/
void IRQ139_Handler(void)
{
    uint32_t u32Tmp1 = 0u;
    uint32_t VSSEL139 = HC32F460_INTC.VSSEL[139 - 128];

    u32Tmp1 = HC32F460_TMR43->OCSRU;
    /** \brief Timer4 Ch.3 U phase higher compare match */
    if ((VSSEL139 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer43GCMUH_IrqHandler(&__g_tim43_timing_dev);
    }
    /** \brief Timer4 Ch.3 U phase lower compare match */
    if ((VSSEL139 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer43GCMUL_IrqHandler(&__g_tim43_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR43->OCSRV;
    /** \brief Timer4 Ch.3 V phase higher compare match */
    if ((VSSEL139 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer43GCMVH_IrqHandler(&__g_tim43_timing_dev);
    }
    /** \brief Timer4 Ch.3 V phase lower compare match */
    if ((VSSEL139 & BIT_MASK_03) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer43GCMVL_IrqHandler(&__g_tim43_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR43->OCSRW;
    /** \brief Timer4 Ch.3 W phase higher compare match */
    if ((VSSEL139 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
    {
        Timer43GCMWH_IrqHandler(&__g_tim43_timing_dev);
    }
    /** \brief Timer4 Ch.3 W phase lower compare match */
    if ((VSSEL139 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
    {
        Timer43GCMWL_IrqHandler(&__g_tim43_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR43->CCSR;
    /** \brief Timer4 Ch.3 overflow */
    if ((VSSEL139 & BIT_MASK_06) && (u32Tmp1 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_09))
    {
        Timer43GOV_IrqHandler(&__g_tim43_timing_dev);
    }
    /** \brief Timer4 Ch.3 underflow */
    if ((VSSEL139 & BIT_MASK_07) && (u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_14))
    {
        Timer43GUD_IrqHandler(&__g_tim43_timing_dev);
    }

    u32Tmp1 = HC32F460_TMR43->RCSR;
    /** \brief Timer4 Ch.3 U phase reload */
    if ((VSSEL139 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04))
    {
        Timer41ReloadU_IrqHandler(&__g_tim43_timing_dev);
    }
    /** \brief Timer4 Ch.3 V phase reload */
    if ((VSSEL139 & BIT_MASK_09) && (u32Tmp1 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_08))
    {
        Timer43ReloadV_IrqHandler(&__g_tim43_timing_dev);
    }
    /** \brief Timer4 Ch.3 W phase reload */
    if ((VSSEL139 & BIT_MASK_10) && (u32Tmp1 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_12))
    {
        Timer43ReloadW_IrqHandler(&__g_tim43_timing_dev);
    }
}

#define bM4_I2C1_CR1_PE                           (*((volatile unsigned int*)(0x429C0000UL)))
#define bM4_I2C1_CR1_SMBUS                        (*((volatile unsigned int*)(0x429C0004UL)))
#define bM4_I2C1_CR1_SMBALRTEN                    (*((volatile unsigned int*)(0x429C0008UL)))
#define bM4_I2C1_CR1_SMBDEFAULTEN                 (*((volatile unsigned int*)(0x429C000CUL)))
#define bM4_I2C1_CR1_SMBHOSTEN                    (*((volatile unsigned int*)(0x429C0010UL)))
#define bM4_I2C1_CR1_ENGC                         (*((volatile unsigned int*)(0x429C0018UL)))
#define bM4_I2C1_CR1_RESTART                      (*((volatile unsigned int*)(0x429C001CUL)))
#define bM4_I2C1_CR1_START                        (*((volatile unsigned int*)(0x429C0020UL)))
#define bM4_I2C1_CR1_STOP                         (*((volatile unsigned int*)(0x429C0024UL)))
#define bM4_I2C1_CR1_ACK                          (*((volatile unsigned int*)(0x429C0028UL)))
#define bM4_I2C1_CR1_SWRST                        (*((volatile unsigned int*)(0x429C003CUL)))
#define bM4_I2C1_CR2_STARTIE                      (*((volatile unsigned int*)(0x429C0080UL)))
#define bM4_I2C1_CR2_SLADDR0IE                    (*((volatile unsigned int*)(0x429C0084UL)))
#define bM4_I2C1_CR2_SLADDR1IE                    (*((volatile unsigned int*)(0x429C0088UL)))
#define bM4_I2C1_CR2_TENDIE                       (*((volatile unsigned int*)(0x429C008CUL)))
#define bM4_I2C1_CR2_STOPIE                       (*((volatile unsigned int*)(0x429C0090UL)))
#define bM4_I2C1_CR2_RFULLIE                      (*((volatile unsigned int*)(0x429C0098UL)))
#define bM4_I2C1_CR2_TEMPTYIE                     (*((volatile unsigned int*)(0x429C009CUL)))
#define bM4_I2C1_CR2_ARLOIE                       (*((volatile unsigned int*)(0x429C00A4UL)))
#define bM4_I2C1_CR2_NACKIE                       (*((volatile unsigned int*)(0x429C00B0UL)))
#define bM4_I2C1_CR2_TMOUTIE                      (*((volatile unsigned int*)(0x429C00B8UL)))
#define bM4_I2C1_CR2_GENCALLIE                    (*((volatile unsigned int*)(0x429C00D0UL)))
#define bM4_I2C1_CR2_SMBDEFAULTIE                 (*((volatile unsigned int*)(0x429C00D4UL)))
#define bM4_I2C1_CR2_SMHOSTIE                     (*((volatile unsigned int*)(0x429C00D8UL)))
#define bM4_I2C1_CR2_SMBALRTIE                    (*((volatile unsigned int*)(0x429C00DCUL)))
#define bM4_I2C1_CR3_TMOUTEN                      (*((volatile unsigned int*)(0x429C0100UL)))
#define bM4_I2C1_CR3_LTMOUT                       (*((volatile unsigned int*)(0x429C0104UL)))
#define bM4_I2C1_CR3_HTMOUT                       (*((volatile unsigned int*)(0x429C0108UL)))
#define bM4_I2C1_CR3_FACKEN                       (*((volatile unsigned int*)(0x429C011CUL)))
#define bM4_I2C1_SLR0_SLADDR00                    (*((volatile unsigned int*)(0x429C0200UL)))
#define bM4_I2C1_SLR0_SLADDR01                    (*((volatile unsigned int*)(0x429C0204UL)))
#define bM4_I2C1_SLR0_SLADDR02                    (*((volatile unsigned int*)(0x429C0208UL)))
#define bM4_I2C1_SLR0_SLADDR03                    (*((volatile unsigned int*)(0x429C020CUL)))
#define bM4_I2C1_SLR0_SLADDR04                    (*((volatile unsigned int*)(0x429C0210UL)))
#define bM4_I2C1_SLR0_SLADDR05                    (*((volatile unsigned int*)(0x429C0214UL)))
#define bM4_I2C1_SLR0_SLADDR06                    (*((volatile unsigned int*)(0x429C0218UL)))
#define bM4_I2C1_SLR0_SLADDR07                    (*((volatile unsigned int*)(0x429C021CUL)))
#define bM4_I2C1_SLR0_SLADDR08                    (*((volatile unsigned int*)(0x429C0220UL)))
#define bM4_I2C1_SLR0_SLADDR09                    (*((volatile unsigned int*)(0x429C0224UL)))
#define bM4_I2C1_SLR0_SLADDR0EN                   (*((volatile unsigned int*)(0x429C0230UL)))
#define bM4_I2C1_SLR0_ADDRMOD0                    (*((volatile unsigned int*)(0x429C023CUL)))
#define bM4_I2C1_SLR1_SLADDR10                    (*((volatile unsigned int*)(0x429C0280UL)))
#define bM4_I2C1_SLR1_SLADDR11                    (*((volatile unsigned int*)(0x429C0284UL)))
#define bM4_I2C1_SLR1_SLADDR12                    (*((volatile unsigned int*)(0x429C0288UL)))
#define bM4_I2C1_SLR1_SLADDR13                    (*((volatile unsigned int*)(0x429C028CUL)))
#define bM4_I2C1_SLR1_SLADDR14                    (*((volatile unsigned int*)(0x429C0290UL)))
#define bM4_I2C1_SLR1_SLADDR15                    (*((volatile unsigned int*)(0x429C0294UL)))
#define bM4_I2C1_SLR1_SLADDR16                    (*((volatile unsigned int*)(0x429C0298UL)))
#define bM4_I2C1_SLR1_SLADDR17                    (*((volatile unsigned int*)(0x429C029CUL)))
#define bM4_I2C1_SLR1_SLADDR18                    (*((volatile unsigned int*)(0x429C02A0UL)))
#define bM4_I2C1_SLR1_SLADDR19                    (*((volatile unsigned int*)(0x429C02A4UL)))
#define bM4_I2C1_SLR1_SLADDR1EN                   (*((volatile unsigned int*)(0x429C02B0UL)))
#define bM4_I2C1_SLR1_ADDRMOD1                    (*((volatile unsigned int*)(0x429C02BCUL)))
#define bM4_I2C1_SLTR_TOUTLOW0                    (*((volatile unsigned int*)(0x429C0300UL)))
#define bM4_I2C1_SLTR_TOUTLOW1                    (*((volatile unsigned int*)(0x429C0304UL)))
#define bM4_I2C1_SLTR_TOUTLOW2                    (*((volatile unsigned int*)(0x429C0308UL)))
#define bM4_I2C1_SLTR_TOUTLOW3                    (*((volatile unsigned int*)(0x429C030CUL)))
#define bM4_I2C1_SLTR_TOUTLOW4                    (*((volatile unsigned int*)(0x429C0310UL)))
#define bM4_I2C1_SLTR_TOUTLOW5                    (*((volatile unsigned int*)(0x429C0314UL)))
#define bM4_I2C1_SLTR_TOUTLOW6                    (*((volatile unsigned int*)(0x429C0318UL)))
#define bM4_I2C1_SLTR_TOUTLOW7                    (*((volatile unsigned int*)(0x429C031CUL)))
#define bM4_I2C1_SLTR_TOUTLOW8                    (*((volatile unsigned int*)(0x429C0320UL)))
#define bM4_I2C1_SLTR_TOUTLOW9                    (*((volatile unsigned int*)(0x429C0324UL)))
#define bM4_I2C1_SLTR_TOUTLOW10                   (*((volatile unsigned int*)(0x429C0328UL)))
#define bM4_I2C1_SLTR_TOUTLOW11                   (*((volatile unsigned int*)(0x429C032CUL)))
#define bM4_I2C1_SLTR_TOUTLOW12                   (*((volatile unsigned int*)(0x429C0330UL)))
#define bM4_I2C1_SLTR_TOUTLOW13                   (*((volatile unsigned int*)(0x429C0334UL)))
#define bM4_I2C1_SLTR_TOUTLOW14                   (*((volatile unsigned int*)(0x429C0338UL)))
#define bM4_I2C1_SLTR_TOUTLOW15                   (*((volatile unsigned int*)(0x429C033CUL)))
#define bM4_I2C1_SLTR_TOUTHIGH0                   (*((volatile unsigned int*)(0x429C0340UL)))
#define bM4_I2C1_SLTR_TOUTHIGH1                   (*((volatile unsigned int*)(0x429C0344UL)))
#define bM4_I2C1_SLTR_TOUTHIGH2                   (*((volatile unsigned int*)(0x429C0348UL)))
#define bM4_I2C1_SLTR_TOUTHIGH3                   (*((volatile unsigned int*)(0x429C034CUL)))
#define bM4_I2C1_SLTR_TOUTHIGH4                   (*((volatile unsigned int*)(0x429C0350UL)))
#define bM4_I2C1_SLTR_TOUTHIGH5                   (*((volatile unsigned int*)(0x429C0354UL)))
#define bM4_I2C1_SLTR_TOUTHIGH6                   (*((volatile unsigned int*)(0x429C0358UL)))
#define bM4_I2C1_SLTR_TOUTHIGH7                   (*((volatile unsigned int*)(0x429C035CUL)))
#define bM4_I2C1_SLTR_TOUTHIGH8                   (*((volatile unsigned int*)(0x429C0360UL)))
#define bM4_I2C1_SLTR_TOUTHIGH9                   (*((volatile unsigned int*)(0x429C0364UL)))
#define bM4_I2C1_SLTR_TOUTHIGH10                  (*((volatile unsigned int*)(0x429C0368UL)))
#define bM4_I2C1_SLTR_TOUTHIGH11                  (*((volatile unsigned int*)(0x429C036CUL)))
#define bM4_I2C1_SLTR_TOUTHIGH12                  (*((volatile unsigned int*)(0x429C0370UL)))
#define bM4_I2C1_SLTR_TOUTHIGH13                  (*((volatile unsigned int*)(0x429C0374UL)))
#define bM4_I2C1_SLTR_TOUTHIGH14                  (*((volatile unsigned int*)(0x429C0378UL)))
#define bM4_I2C1_SLTR_TOUTHIGH15                  (*((volatile unsigned int*)(0x429C037CUL)))
#define bM4_I2C1_SR_STARTF                        (*((volatile unsigned int*)(0x429C0380UL)))
#define bM4_I2C1_SR_SLADDR0F                      (*((volatile unsigned int*)(0x429C0384UL)))
#define bM4_I2C1_SR_SLADDR1F                      (*((volatile unsigned int*)(0x429C0388UL)))
#define bM4_I2C1_SR_TENDF                         (*((volatile unsigned int*)(0x429C038CUL)))
#define bM4_I2C1_SR_STOPF                         (*((volatile unsigned int*)(0x429C0390UL)))
#define bM4_I2C1_SR_RFULLF                        (*((volatile unsigned int*)(0x429C0398UL)))
#define bM4_I2C1_SR_TEMPTYF                       (*((volatile unsigned int*)(0x429C039CUL)))
#define bM4_I2C1_SR_ARLOF                         (*((volatile unsigned int*)(0x429C03A4UL)))
#define bM4_I2C1_SR_ACKRF                         (*((volatile unsigned int*)(0x429C03A8UL)))
#define bM4_I2C1_SR_NACKF                         (*((volatile unsigned int*)(0x429C03B0UL)))
#define bM4_I2C1_SR_TMOUTF                        (*((volatile unsigned int*)(0x429C03B8UL)))
#define bM4_I2C1_SR_MSL                           (*((volatile unsigned int*)(0x429C03C0UL)))
#define bM4_I2C1_SR_BUSY                          (*((volatile unsigned int*)(0x429C03C4UL)))
#define bM4_I2C1_SR_TRA                           (*((volatile unsigned int*)(0x429C03C8UL)))
#define bM4_I2C1_SR_GENCALLF                      (*((volatile unsigned int*)(0x429C03D0UL)))
#define bM4_I2C1_SR_SMBDEFAULTF                   (*((volatile unsigned int*)(0x429C03D4UL)))
#define bM4_I2C1_SR_SMBHOSTF                      (*((volatile unsigned int*)(0x429C03D8UL)))
#define bM4_I2C1_SR_SMBALRTF                      (*((volatile unsigned int*)(0x429C03DCUL)))
#define bM4_I2C1_CLR_STARTFCLR                    (*((volatile unsigned int*)(0x429C0400UL)))
#define bM4_I2C1_CLR_SLADDR0FCLR                  (*((volatile unsigned int*)(0x429C0404UL)))
#define bM4_I2C1_CLR_SLADDR1FCLR                  (*((volatile unsigned int*)(0x429C0408UL)))
#define bM4_I2C1_CLR_TENDFCLR                     (*((volatile unsigned int*)(0x429C040CUL)))
#define bM4_I2C1_CLR_STOPFCLR                     (*((volatile unsigned int*)(0x429C0410UL)))
#define bM4_I2C1_CLR_RFULLFCLR                    (*((volatile unsigned int*)(0x429C0418UL)))
#define bM4_I2C1_CLR_TEMPTYFCLR                   (*((volatile unsigned int*)(0x429C041CUL)))
#define bM4_I2C1_CLR_ARLOFCLR                     (*((volatile unsigned int*)(0x429C0424UL)))
#define bM4_I2C1_CLR_NACKFCLR                     (*((volatile unsigned int*)(0x429C0430UL)))
#define bM4_I2C1_CLR_TMOUTFCLR                    (*((volatile unsigned int*)(0x429C0438UL)))
#define bM4_I2C1_CLR_GENCALLFCLR                  (*((volatile unsigned int*)(0x429C0450UL)))
#define bM4_I2C1_CLR_SMBDEFAULTFCLR               (*((volatile unsigned int*)(0x429C0454UL)))
#define bM4_I2C1_CLR_SMBHOSTFCLR                  (*((volatile unsigned int*)(0x429C0458UL)))
#define bM4_I2C1_CLR_SMBALRTFCLR                  (*((volatile unsigned int*)(0x429C045CUL)))
#define bM4_I2C1_DTR_DT0                          (*((volatile unsigned int*)(0x429C0480UL)))
#define bM4_I2C1_DTR_DT1                          (*((volatile unsigned int*)(0x429C0484UL)))
#define bM4_I2C1_DTR_DT2                          (*((volatile unsigned int*)(0x429C0488UL)))
#define bM4_I2C1_DTR_DT3                          (*((volatile unsigned int*)(0x429C048CUL)))
#define bM4_I2C1_DTR_DT4                          (*((volatile unsigned int*)(0x429C0490UL)))
#define bM4_I2C1_DTR_DT5                          (*((volatile unsigned int*)(0x429C0494UL)))
#define bM4_I2C1_DTR_DT6                          (*((volatile unsigned int*)(0x429C0498UL)))
#define bM4_I2C1_DTR_DT7                          (*((volatile unsigned int*)(0x429C049CUL)))
#define bM4_I2C1_DRR_DR0                          (*((volatile unsigned int*)(0x429C0500UL)))
#define bM4_I2C1_DRR_DR1                          (*((volatile unsigned int*)(0x429C0504UL)))
#define bM4_I2C1_DRR_DR2                          (*((volatile unsigned int*)(0x429C0508UL)))
#define bM4_I2C1_DRR_DR3                          (*((volatile unsigned int*)(0x429C050CUL)))
#define bM4_I2C1_DRR_DR4                          (*((volatile unsigned int*)(0x429C0510UL)))
#define bM4_I2C1_DRR_DR5                          (*((volatile unsigned int*)(0x429C0514UL)))
#define bM4_I2C1_DRR_DR6                          (*((volatile unsigned int*)(0x429C0518UL)))
#define bM4_I2C1_DRR_DR7                          (*((volatile unsigned int*)(0x429C051CUL)))
#define bM4_I2C1_CCR_SLOWW0                       (*((volatile unsigned int*)(0x429C0580UL)))
#define bM4_I2C1_CCR_SLOWW1                       (*((volatile unsigned int*)(0x429C0584UL)))
#define bM4_I2C1_CCR_SLOWW2                       (*((volatile unsigned int*)(0x429C0588UL)))
#define bM4_I2C1_CCR_SLOWW3                       (*((volatile unsigned int*)(0x429C058CUL)))
#define bM4_I2C1_CCR_SLOWW4                       (*((volatile unsigned int*)(0x429C0590UL)))
#define bM4_I2C1_CCR_SHIGHW0                      (*((volatile unsigned int*)(0x429C05A0UL)))
#define bM4_I2C1_CCR_SHIGHW1                      (*((volatile unsigned int*)(0x429C05A4UL)))
#define bM4_I2C1_CCR_SHIGHW2                      (*((volatile unsigned int*)(0x429C05A8UL)))
#define bM4_I2C1_CCR_SHIGHW3                      (*((volatile unsigned int*)(0x429C05ACUL)))
#define bM4_I2C1_CCR_SHIGHW4                      (*((volatile unsigned int*)(0x429C05B0UL)))
#define bM4_I2C1_CCR_FREQ0                        (*((volatile unsigned int*)(0x429C05C0UL)))
#define bM4_I2C1_CCR_FREQ1                        (*((volatile unsigned int*)(0x429C05C4UL)))
#define bM4_I2C1_CCR_FREQ2                        (*((volatile unsigned int*)(0x429C05C8UL)))
#define bM4_I2C1_FLTR_DNF0                        (*((volatile unsigned int*)(0x429C0600UL)))
#define bM4_I2C1_FLTR_DNF1                        (*((volatile unsigned int*)(0x429C0604UL)))
#define bM4_I2C1_FLTR_DNFEN                       (*((volatile unsigned int*)(0x429C0610UL)))
#define bM4_I2C1_FLTR_ANFEN                       (*((volatile unsigned int*)(0x429C0614UL)))
#define bM4_I2C2_CR1_PE                           (*((volatile unsigned int*)(0x429C8000UL)))
#define bM4_I2C2_CR1_SMBUS                        (*((volatile unsigned int*)(0x429C8004UL)))
#define bM4_I2C2_CR1_SMBALRTEN                    (*((volatile unsigned int*)(0x429C8008UL)))
#define bM4_I2C2_CR1_SMBDEFAULTEN                 (*((volatile unsigned int*)(0x429C800CUL)))
#define bM4_I2C2_CR1_SMBHOSTEN                    (*((volatile unsigned int*)(0x429C8010UL)))
#define bM4_I2C2_CR1_ENGC                         (*((volatile unsigned int*)(0x429C8018UL)))
#define bM4_I2C2_CR1_RESTART                      (*((volatile unsigned int*)(0x429C801CUL)))
#define bM4_I2C2_CR1_START                        (*((volatile unsigned int*)(0x429C8020UL)))
#define bM4_I2C2_CR1_STOP                         (*((volatile unsigned int*)(0x429C8024UL)))
#define bM4_I2C2_CR1_ACK                          (*((volatile unsigned int*)(0x429C8028UL)))
#define bM4_I2C2_CR1_SWRST                        (*((volatile unsigned int*)(0x429C803CUL)))
#define bM4_I2C2_CR2_STARTIE                      (*((volatile unsigned int*)(0x429C8080UL)))
#define bM4_I2C2_CR2_SLADDR0IE                    (*((volatile unsigned int*)(0x429C8084UL)))
#define bM4_I2C2_CR2_SLADDR1IE                    (*((volatile unsigned int*)(0x429C8088UL)))
#define bM4_I2C2_CR2_TENDIE                       (*((volatile unsigned int*)(0x429C808CUL)))
#define bM4_I2C2_CR2_STOPIE                       (*((volatile unsigned int*)(0x429C8090UL)))
#define bM4_I2C2_CR2_RFULLIE                      (*((volatile unsigned int*)(0x429C8098UL)))
#define bM4_I2C2_CR2_TEMPTYIE                     (*((volatile unsigned int*)(0x429C809CUL)))
#define bM4_I2C2_CR2_ARLOIE                       (*((volatile unsigned int*)(0x429C80A4UL)))
#define bM4_I2C2_CR2_NACKIE                       (*((volatile unsigned int*)(0x429C80B0UL)))
#define bM4_I2C2_CR2_TMOUTIE                      (*((volatile unsigned int*)(0x429C80B8UL)))
#define bM4_I2C2_CR2_GENCALLIE                    (*((volatile unsigned int*)(0x429C80D0UL)))
#define bM4_I2C2_CR2_SMBDEFAULTIE                 (*((volatile unsigned int*)(0x429C80D4UL)))
#define bM4_I2C2_CR2_SMHOSTIE                     (*((volatile unsigned int*)(0x429C80D8UL)))
#define bM4_I2C2_CR2_SMBALRTIE                    (*((volatile unsigned int*)(0x429C80DCUL)))
#define bM4_I2C2_CR3_TMOUTEN                      (*((volatile unsigned int*)(0x429C8100UL)))
#define bM4_I2C2_CR3_LTMOUT                       (*((volatile unsigned int*)(0x429C8104UL)))
#define bM4_I2C2_CR3_HTMOUT                       (*((volatile unsigned int*)(0x429C8108UL)))
#define bM4_I2C2_CR3_FACKEN                       (*((volatile unsigned int*)(0x429C811CUL)))
#define bM4_I2C2_SLR0_SLADDR00                    (*((volatile unsigned int*)(0x429C8200UL)))
#define bM4_I2C2_SLR0_SLADDR01                    (*((volatile unsigned int*)(0x429C8204UL)))
#define bM4_I2C2_SLR0_SLADDR02                    (*((volatile unsigned int*)(0x429C8208UL)))
#define bM4_I2C2_SLR0_SLADDR03                    (*((volatile unsigned int*)(0x429C820CUL)))
#define bM4_I2C2_SLR0_SLADDR04                    (*((volatile unsigned int*)(0x429C8210UL)))
#define bM4_I2C2_SLR0_SLADDR05                    (*((volatile unsigned int*)(0x429C8214UL)))
#define bM4_I2C2_SLR0_SLADDR06                    (*((volatile unsigned int*)(0x429C8218UL)))
#define bM4_I2C2_SLR0_SLADDR07                    (*((volatile unsigned int*)(0x429C821CUL)))
#define bM4_I2C2_SLR0_SLADDR08                    (*((volatile unsigned int*)(0x429C8220UL)))
#define bM4_I2C2_SLR0_SLADDR09                    (*((volatile unsigned int*)(0x429C8224UL)))
#define bM4_I2C2_SLR0_SLADDR0EN                   (*((volatile unsigned int*)(0x429C8230UL)))
#define bM4_I2C2_SLR0_ADDRMOD0                    (*((volatile unsigned int*)(0x429C823CUL)))
#define bM4_I2C2_SLR1_SLADDR10                    (*((volatile unsigned int*)(0x429C8280UL)))
#define bM4_I2C2_SLR1_SLADDR11                    (*((volatile unsigned int*)(0x429C8284UL)))
#define bM4_I2C2_SLR1_SLADDR12                    (*((volatile unsigned int*)(0x429C8288UL)))
#define bM4_I2C2_SLR1_SLADDR13                    (*((volatile unsigned int*)(0x429C828CUL)))
#define bM4_I2C2_SLR1_SLADDR14                    (*((volatile unsigned int*)(0x429C8290UL)))
#define bM4_I2C2_SLR1_SLADDR15                    (*((volatile unsigned int*)(0x429C8294UL)))
#define bM4_I2C2_SLR1_SLADDR16                    (*((volatile unsigned int*)(0x429C8298UL)))
#define bM4_I2C2_SLR1_SLADDR17                    (*((volatile unsigned int*)(0x429C829CUL)))
#define bM4_I2C2_SLR1_SLADDR18                    (*((volatile unsigned int*)(0x429C82A0UL)))
#define bM4_I2C2_SLR1_SLADDR19                    (*((volatile unsigned int*)(0x429C82A4UL)))
#define bM4_I2C2_SLR1_SLADDR1EN                   (*((volatile unsigned int*)(0x429C82B0UL)))
#define bM4_I2C2_SLR1_ADDRMOD1                    (*((volatile unsigned int*)(0x429C82BCUL)))
#define bM4_I2C2_SLTR_TOUTLOW0                    (*((volatile unsigned int*)(0x429C8300UL)))
#define bM4_I2C2_SLTR_TOUTLOW1                    (*((volatile unsigned int*)(0x429C8304UL)))
#define bM4_I2C2_SLTR_TOUTLOW2                    (*((volatile unsigned int*)(0x429C8308UL)))
#define bM4_I2C2_SLTR_TOUTLOW3                    (*((volatile unsigned int*)(0x429C830CUL)))
#define bM4_I2C2_SLTR_TOUTLOW4                    (*((volatile unsigned int*)(0x429C8310UL)))
#define bM4_I2C2_SLTR_TOUTLOW5                    (*((volatile unsigned int*)(0x429C8314UL)))
#define bM4_I2C2_SLTR_TOUTLOW6                    (*((volatile unsigned int*)(0x429C8318UL)))
#define bM4_I2C2_SLTR_TOUTLOW7                    (*((volatile unsigned int*)(0x429C831CUL)))
#define bM4_I2C2_SLTR_TOUTLOW8                    (*((volatile unsigned int*)(0x429C8320UL)))
#define bM4_I2C2_SLTR_TOUTLOW9                    (*((volatile unsigned int*)(0x429C8324UL)))
#define bM4_I2C2_SLTR_TOUTLOW10                   (*((volatile unsigned int*)(0x429C8328UL)))
#define bM4_I2C2_SLTR_TOUTLOW11                   (*((volatile unsigned int*)(0x429C832CUL)))
#define bM4_I2C2_SLTR_TOUTLOW12                   (*((volatile unsigned int*)(0x429C8330UL)))
#define bM4_I2C2_SLTR_TOUTLOW13                   (*((volatile unsigned int*)(0x429C8334UL)))
#define bM4_I2C2_SLTR_TOUTLOW14                   (*((volatile unsigned int*)(0x429C8338UL)))
#define bM4_I2C2_SLTR_TOUTLOW15                   (*((volatile unsigned int*)(0x429C833CUL)))
#define bM4_I2C2_SLTR_TOUTHIGH0                   (*((volatile unsigned int*)(0x429C8340UL)))
#define bM4_I2C2_SLTR_TOUTHIGH1                   (*((volatile unsigned int*)(0x429C8344UL)))
#define bM4_I2C2_SLTR_TOUTHIGH2                   (*((volatile unsigned int*)(0x429C8348UL)))
#define bM4_I2C2_SLTR_TOUTHIGH3                   (*((volatile unsigned int*)(0x429C834CUL)))
#define bM4_I2C2_SLTR_TOUTHIGH4                   (*((volatile unsigned int*)(0x429C8350UL)))
#define bM4_I2C2_SLTR_TOUTHIGH5                   (*((volatile unsigned int*)(0x429C8354UL)))
#define bM4_I2C2_SLTR_TOUTHIGH6                   (*((volatile unsigned int*)(0x429C8358UL)))
#define bM4_I2C2_SLTR_TOUTHIGH7                   (*((volatile unsigned int*)(0x429C835CUL)))
#define bM4_I2C2_SLTR_TOUTHIGH8                   (*((volatile unsigned int*)(0x429C8360UL)))
#define bM4_I2C2_SLTR_TOUTHIGH9                   (*((volatile unsigned int*)(0x429C8364UL)))
#define bM4_I2C2_SLTR_TOUTHIGH10                  (*((volatile unsigned int*)(0x429C8368UL)))
#define bM4_I2C2_SLTR_TOUTHIGH11                  (*((volatile unsigned int*)(0x429C836CUL)))
#define bM4_I2C2_SLTR_TOUTHIGH12                  (*((volatile unsigned int*)(0x429C8370UL)))
#define bM4_I2C2_SLTR_TOUTHIGH13                  (*((volatile unsigned int*)(0x429C8374UL)))
#define bM4_I2C2_SLTR_TOUTHIGH14                  (*((volatile unsigned int*)(0x429C8378UL)))
#define bM4_I2C2_SLTR_TOUTHIGH15                  (*((volatile unsigned int*)(0x429C837CUL)))
#define bM4_I2C2_SR_STARTF                        (*((volatile unsigned int*)(0x429C8380UL)))
#define bM4_I2C2_SR_SLADDR0F                      (*((volatile unsigned int*)(0x429C8384UL)))
#define bM4_I2C2_SR_SLADDR1F                      (*((volatile unsigned int*)(0x429C8388UL)))
#define bM4_I2C2_SR_TENDF                         (*((volatile unsigned int*)(0x429C838CUL)))
#define bM4_I2C2_SR_STOPF                         (*((volatile unsigned int*)(0x429C8390UL)))
#define bM4_I2C2_SR_RFULLF                        (*((volatile unsigned int*)(0x429C8398UL)))
#define bM4_I2C2_SR_TEMPTYF                       (*((volatile unsigned int*)(0x429C839CUL)))
#define bM4_I2C2_SR_ARLOF                         (*((volatile unsigned int*)(0x429C83A4UL)))
#define bM4_I2C2_SR_ACKRF                         (*((volatile unsigned int*)(0x429C83A8UL)))
#define bM4_I2C2_SR_NACKF                         (*((volatile unsigned int*)(0x429C83B0UL)))
#define bM4_I2C2_SR_TMOUTF                        (*((volatile unsigned int*)(0x429C83B8UL)))
#define bM4_I2C2_SR_MSL                           (*((volatile unsigned int*)(0x429C83C0UL)))
#define bM4_I2C2_SR_BUSY                          (*((volatile unsigned int*)(0x429C83C4UL)))
#define bM4_I2C2_SR_TRA                           (*((volatile unsigned int*)(0x429C83C8UL)))
#define bM4_I2C2_SR_GENCALLF                      (*((volatile unsigned int*)(0x429C83D0UL)))
#define bM4_I2C2_SR_SMBDEFAULTF                   (*((volatile unsigned int*)(0x429C83D4UL)))
#define bM4_I2C2_SR_SMBHOSTF                      (*((volatile unsigned int*)(0x429C83D8UL)))
#define bM4_I2C2_SR_SMBALRTF                      (*((volatile unsigned int*)(0x429C83DCUL)))
#define bM4_I2C2_CLR_STARTFCLR                    (*((volatile unsigned int*)(0x429C8400UL)))
#define bM4_I2C2_CLR_SLADDR0FCLR                  (*((volatile unsigned int*)(0x429C8404UL)))
#define bM4_I2C2_CLR_SLADDR1FCLR                  (*((volatile unsigned int*)(0x429C8408UL)))
#define bM4_I2C2_CLR_TENDFCLR                     (*((volatile unsigned int*)(0x429C840CUL)))
#define bM4_I2C2_CLR_STOPFCLR                     (*((volatile unsigned int*)(0x429C8410UL)))
#define bM4_I2C2_CLR_RFULLFCLR                    (*((volatile unsigned int*)(0x429C8418UL)))
#define bM4_I2C2_CLR_TEMPTYFCLR                   (*((volatile unsigned int*)(0x429C841CUL)))
#define bM4_I2C2_CLR_ARLOFCLR                     (*((volatile unsigned int*)(0x429C8424UL)))
#define bM4_I2C2_CLR_NACKFCLR                     (*((volatile unsigned int*)(0x429C8430UL)))
#define bM4_I2C2_CLR_TMOUTFCLR                    (*((volatile unsigned int*)(0x429C8438UL)))
#define bM4_I2C2_CLR_GENCALLFCLR                  (*((volatile unsigned int*)(0x429C8450UL)))
#define bM4_I2C2_CLR_SMBDEFAULTFCLR               (*((volatile unsigned int*)(0x429C8454UL)))
#define bM4_I2C2_CLR_SMBHOSTFCLR                  (*((volatile unsigned int*)(0x429C8458UL)))
#define bM4_I2C2_CLR_SMBALRTFCLR                  (*((volatile unsigned int*)(0x429C845CUL)))
#define bM4_I2C2_DTR_DT0                          (*((volatile unsigned int*)(0x429C8480UL)))
#define bM4_I2C2_DTR_DT1                          (*((volatile unsigned int*)(0x429C8484UL)))
#define bM4_I2C2_DTR_DT2                          (*((volatile unsigned int*)(0x429C8488UL)))
#define bM4_I2C2_DTR_DT3                          (*((volatile unsigned int*)(0x429C848CUL)))
#define bM4_I2C2_DTR_DT4                          (*((volatile unsigned int*)(0x429C8490UL)))
#define bM4_I2C2_DTR_DT5                          (*((volatile unsigned int*)(0x429C8494UL)))
#define bM4_I2C2_DTR_DT6                          (*((volatile unsigned int*)(0x429C8498UL)))
#define bM4_I2C2_DTR_DT7                          (*((volatile unsigned int*)(0x429C849CUL)))
#define bM4_I2C2_DRR_DR0                          (*((volatile unsigned int*)(0x429C8500UL)))
#define bM4_I2C2_DRR_DR1                          (*((volatile unsigned int*)(0x429C8504UL)))
#define bM4_I2C2_DRR_DR2                          (*((volatile unsigned int*)(0x429C8508UL)))
#define bM4_I2C2_DRR_DR3                          (*((volatile unsigned int*)(0x429C850CUL)))
#define bM4_I2C2_DRR_DR4                          (*((volatile unsigned int*)(0x429C8510UL)))
#define bM4_I2C2_DRR_DR5                          (*((volatile unsigned int*)(0x429C8514UL)))
#define bM4_I2C2_DRR_DR6                          (*((volatile unsigned int*)(0x429C8518UL)))
#define bM4_I2C2_DRR_DR7                          (*((volatile unsigned int*)(0x429C851CUL)))
#define bM4_I2C2_CCR_SLOWW0                       (*((volatile unsigned int*)(0x429C8580UL)))
#define bM4_I2C2_CCR_SLOWW1                       (*((volatile unsigned int*)(0x429C8584UL)))
#define bM4_I2C2_CCR_SLOWW2                       (*((volatile unsigned int*)(0x429C8588UL)))
#define bM4_I2C2_CCR_SLOWW3                       (*((volatile unsigned int*)(0x429C858CUL)))
#define bM4_I2C2_CCR_SLOWW4                       (*((volatile unsigned int*)(0x429C8590UL)))
#define bM4_I2C2_CCR_SHIGHW0                      (*((volatile unsigned int*)(0x429C85A0UL)))
#define bM4_I2C2_CCR_SHIGHW1                      (*((volatile unsigned int*)(0x429C85A4UL)))
#define bM4_I2C2_CCR_SHIGHW2                      (*((volatile unsigned int*)(0x429C85A8UL)))
#define bM4_I2C2_CCR_SHIGHW3                      (*((volatile unsigned int*)(0x429C85ACUL)))
#define bM4_I2C2_CCR_SHIGHW4                      (*((volatile unsigned int*)(0x429C85B0UL)))
#define bM4_I2C2_CCR_FREQ0                        (*((volatile unsigned int*)(0x429C85C0UL)))
#define bM4_I2C2_CCR_FREQ1                        (*((volatile unsigned int*)(0x429C85C4UL)))
#define bM4_I2C2_CCR_FREQ2                        (*((volatile unsigned int*)(0x429C85C8UL)))
#define bM4_I2C2_FLTR_DNF0                        (*((volatile unsigned int*)(0x429C8600UL)))
#define bM4_I2C2_FLTR_DNF1                        (*((volatile unsigned int*)(0x429C8604UL)))
#define bM4_I2C2_FLTR_DNFEN                       (*((volatile unsigned int*)(0x429C8610UL)))
#define bM4_I2C2_FLTR_ANFEN                       (*((volatile unsigned int*)(0x429C8614UL)))
#define bM4_I2C3_CR1_PE                           (*((volatile unsigned int*)(0x429D0000UL)))
#define bM4_I2C3_CR1_SMBUS                        (*((volatile unsigned int*)(0x429D0004UL)))
#define bM4_I2C3_CR1_SMBALRTEN                    (*((volatile unsigned int*)(0x429D0008UL)))
#define bM4_I2C3_CR1_SMBDEFAULTEN                 (*((volatile unsigned int*)(0x429D000CUL)))
#define bM4_I2C3_CR1_SMBHOSTEN                    (*((volatile unsigned int*)(0x429D0010UL)))
#define bM4_I2C3_CR1_ENGC                         (*((volatile unsigned int*)(0x429D0018UL)))
#define bM4_I2C3_CR1_RESTART                      (*((volatile unsigned int*)(0x429D001CUL)))
#define bM4_I2C3_CR1_START                        (*((volatile unsigned int*)(0x429D0020UL)))
#define bM4_I2C3_CR1_STOP                         (*((volatile unsigned int*)(0x429D0024UL)))
#define bM4_I2C3_CR1_ACK                          (*((volatile unsigned int*)(0x429D0028UL)))
#define bM4_I2C3_CR1_SWRST                        (*((volatile unsigned int*)(0x429D003CUL)))
#define bM4_I2C3_CR2_STARTIE                      (*((volatile unsigned int*)(0x429D0080UL)))
#define bM4_I2C3_CR2_SLADDR0IE                    (*((volatile unsigned int*)(0x429D0084UL)))
#define bM4_I2C3_CR2_SLADDR1IE                    (*((volatile unsigned int*)(0x429D0088UL)))
#define bM4_I2C3_CR2_TENDIE                       (*((volatile unsigned int*)(0x429D008CUL)))
#define bM4_I2C3_CR2_STOPIE                       (*((volatile unsigned int*)(0x429D0090UL)))
#define bM4_I2C3_CR2_RFULLIE                      (*((volatile unsigned int*)(0x429D0098UL)))
#define bM4_I2C3_CR2_TEMPTYIE                     (*((volatile unsigned int*)(0x429D009CUL)))
#define bM4_I2C3_CR2_ARLOIE                       (*((volatile unsigned int*)(0x429D00A4UL)))
#define bM4_I2C3_CR2_NACKIE                       (*((volatile unsigned int*)(0x429D00B0UL)))
#define bM4_I2C3_CR2_TMOUTIE                      (*((volatile unsigned int*)(0x429D00B8UL)))
#define bM4_I2C3_CR2_GENCALLIE                    (*((volatile unsigned int*)(0x429D00D0UL)))
#define bM4_I2C3_CR2_SMBDEFAULTIE                 (*((volatile unsigned int*)(0x429D00D4UL)))
#define bM4_I2C3_CR2_SMHOSTIE                     (*((volatile unsigned int*)(0x429D00D8UL)))
#define bM4_I2C3_CR2_SMBALRTIE                    (*((volatile unsigned int*)(0x429D00DCUL)))
#define bM4_I2C3_CR3_TMOUTEN                      (*((volatile unsigned int*)(0x429D0100UL)))
#define bM4_I2C3_CR3_LTMOUT                       (*((volatile unsigned int*)(0x429D0104UL)))
#define bM4_I2C3_CR3_HTMOUT                       (*((volatile unsigned int*)(0x429D0108UL)))
#define bM4_I2C3_CR3_FACKEN                       (*((volatile unsigned int*)(0x429D011CUL)))
#define bM4_I2C3_SLR0_SLADDR00                    (*((volatile unsigned int*)(0x429D0200UL)))
#define bM4_I2C3_SLR0_SLADDR01                    (*((volatile unsigned int*)(0x429D0204UL)))
#define bM4_I2C3_SLR0_SLADDR02                    (*((volatile unsigned int*)(0x429D0208UL)))
#define bM4_I2C3_SLR0_SLADDR03                    (*((volatile unsigned int*)(0x429D020CUL)))
#define bM4_I2C3_SLR0_SLADDR04                    (*((volatile unsigned int*)(0x429D0210UL)))
#define bM4_I2C3_SLR0_SLADDR05                    (*((volatile unsigned int*)(0x429D0214UL)))
#define bM4_I2C3_SLR0_SLADDR06                    (*((volatile unsigned int*)(0x429D0218UL)))
#define bM4_I2C3_SLR0_SLADDR07                    (*((volatile unsigned int*)(0x429D021CUL)))
#define bM4_I2C3_SLR0_SLADDR08                    (*((volatile unsigned int*)(0x429D0220UL)))
#define bM4_I2C3_SLR0_SLADDR09                    (*((volatile unsigned int*)(0x429D0224UL)))
#define bM4_I2C3_SLR0_SLADDR0EN                   (*((volatile unsigned int*)(0x429D0230UL)))
#define bM4_I2C3_SLR0_ADDRMOD0                    (*((volatile unsigned int*)(0x429D023CUL)))
#define bM4_I2C3_SLR1_SLADDR10                    (*((volatile unsigned int*)(0x429D0280UL)))
#define bM4_I2C3_SLR1_SLADDR11                    (*((volatile unsigned int*)(0x429D0284UL)))
#define bM4_I2C3_SLR1_SLADDR12                    (*((volatile unsigned int*)(0x429D0288UL)))
#define bM4_I2C3_SLR1_SLADDR13                    (*((volatile unsigned int*)(0x429D028CUL)))
#define bM4_I2C3_SLR1_SLADDR14                    (*((volatile unsigned int*)(0x429D0290UL)))
#define bM4_I2C3_SLR1_SLADDR15                    (*((volatile unsigned int*)(0x429D0294UL)))
#define bM4_I2C3_SLR1_SLADDR16                    (*((volatile unsigned int*)(0x429D0298UL)))
#define bM4_I2C3_SLR1_SLADDR17                    (*((volatile unsigned int*)(0x429D029CUL)))
#define bM4_I2C3_SLR1_SLADDR18                    (*((volatile unsigned int*)(0x429D02A0UL)))
#define bM4_I2C3_SLR1_SLADDR19                    (*((volatile unsigned int*)(0x429D02A4UL)))
#define bM4_I2C3_SLR1_SLADDR1EN                   (*((volatile unsigned int*)(0x429D02B0UL)))
#define bM4_I2C3_SLR1_ADDRMOD1                    (*((volatile unsigned int*)(0x429D02BCUL)))
#define bM4_I2C3_SLTR_TOUTLOW0                    (*((volatile unsigned int*)(0x429D0300UL)))
#define bM4_I2C3_SLTR_TOUTLOW1                    (*((volatile unsigned int*)(0x429D0304UL)))
#define bM4_I2C3_SLTR_TOUTLOW2                    (*((volatile unsigned int*)(0x429D0308UL)))
#define bM4_I2C3_SLTR_TOUTLOW3                    (*((volatile unsigned int*)(0x429D030CUL)))
#define bM4_I2C3_SLTR_TOUTLOW4                    (*((volatile unsigned int*)(0x429D0310UL)))
#define bM4_I2C3_SLTR_TOUTLOW5                    (*((volatile unsigned int*)(0x429D0314UL)))
#define bM4_I2C3_SLTR_TOUTLOW6                    (*((volatile unsigned int*)(0x429D0318UL)))
#define bM4_I2C3_SLTR_TOUTLOW7                    (*((volatile unsigned int*)(0x429D031CUL)))
#define bM4_I2C3_SLTR_TOUTLOW8                    (*((volatile unsigned int*)(0x429D0320UL)))
#define bM4_I2C3_SLTR_TOUTLOW9                    (*((volatile unsigned int*)(0x429D0324UL)))
#define bM4_I2C3_SLTR_TOUTLOW10                   (*((volatile unsigned int*)(0x429D0328UL)))
#define bM4_I2C3_SLTR_TOUTLOW11                   (*((volatile unsigned int*)(0x429D032CUL)))
#define bM4_I2C3_SLTR_TOUTLOW12                   (*((volatile unsigned int*)(0x429D0330UL)))
#define bM4_I2C3_SLTR_TOUTLOW13                   (*((volatile unsigned int*)(0x429D0334UL)))
#define bM4_I2C3_SLTR_TOUTLOW14                   (*((volatile unsigned int*)(0x429D0338UL)))
#define bM4_I2C3_SLTR_TOUTLOW15                   (*((volatile unsigned int*)(0x429D033CUL)))
#define bM4_I2C3_SLTR_TOUTHIGH0                   (*((volatile unsigned int*)(0x429D0340UL)))
#define bM4_I2C3_SLTR_TOUTHIGH1                   (*((volatile unsigned int*)(0x429D0344UL)))
#define bM4_I2C3_SLTR_TOUTHIGH2                   (*((volatile unsigned int*)(0x429D0348UL)))
#define bM4_I2C3_SLTR_TOUTHIGH3                   (*((volatile unsigned int*)(0x429D034CUL)))
#define bM4_I2C3_SLTR_TOUTHIGH4                   (*((volatile unsigned int*)(0x429D0350UL)))
#define bM4_I2C3_SLTR_TOUTHIGH5                   (*((volatile unsigned int*)(0x429D0354UL)))
#define bM4_I2C3_SLTR_TOUTHIGH6                   (*((volatile unsigned int*)(0x429D0358UL)))
#define bM4_I2C3_SLTR_TOUTHIGH7                   (*((volatile unsigned int*)(0x429D035CUL)))
#define bM4_I2C3_SLTR_TOUTHIGH8                   (*((volatile unsigned int*)(0x429D0360UL)))
#define bM4_I2C3_SLTR_TOUTHIGH9                   (*((volatile unsigned int*)(0x429D0364UL)))
#define bM4_I2C3_SLTR_TOUTHIGH10                  (*((volatile unsigned int*)(0x429D0368UL)))
#define bM4_I2C3_SLTR_TOUTHIGH11                  (*((volatile unsigned int*)(0x429D036CUL)))
#define bM4_I2C3_SLTR_TOUTHIGH12                  (*((volatile unsigned int*)(0x429D0370UL)))
#define bM4_I2C3_SLTR_TOUTHIGH13                  (*((volatile unsigned int*)(0x429D0374UL)))
#define bM4_I2C3_SLTR_TOUTHIGH14                  (*((volatile unsigned int*)(0x429D0378UL)))
#define bM4_I2C3_SLTR_TOUTHIGH15                  (*((volatile unsigned int*)(0x429D037CUL)))
#define bM4_I2C3_SR_STARTF                        (*((volatile unsigned int*)(0x429D0380UL)))
#define bM4_I2C3_SR_SLADDR0F                      (*((volatile unsigned int*)(0x429D0384UL)))
#define bM4_I2C3_SR_SLADDR1F                      (*((volatile unsigned int*)(0x429D0388UL)))
#define bM4_I2C3_SR_TENDF                         (*((volatile unsigned int*)(0x429D038CUL)))
#define bM4_I2C3_SR_STOPF                         (*((volatile unsigned int*)(0x429D0390UL)))
#define bM4_I2C3_SR_RFULLF                        (*((volatile unsigned int*)(0x429D0398UL)))
#define bM4_I2C3_SR_TEMPTYF                       (*((volatile unsigned int*)(0x429D039CUL)))
#define bM4_I2C3_SR_ARLOF                         (*((volatile unsigned int*)(0x429D03A4UL)))
#define bM4_I2C3_SR_ACKRF                         (*((volatile unsigned int*)(0x429D03A8UL)))
#define bM4_I2C3_SR_NACKF                         (*((volatile unsigned int*)(0x429D03B0UL)))
#define bM4_I2C3_SR_TMOUTF                        (*((volatile unsigned int*)(0x429D03B8UL)))
#define bM4_I2C3_SR_MSL                           (*((volatile unsigned int*)(0x429D03C0UL)))
#define bM4_I2C3_SR_BUSY                          (*((volatile unsigned int*)(0x429D03C4UL)))
#define bM4_I2C3_SR_TRA                           (*((volatile unsigned int*)(0x429D03C8UL)))
#define bM4_I2C3_SR_GENCALLF                      (*((volatile unsigned int*)(0x429D03D0UL)))
#define bM4_I2C3_SR_SMBDEFAULTF                   (*((volatile unsigned int*)(0x429D03D4UL)))
#define bM4_I2C3_SR_SMBHOSTF                      (*((volatile unsigned int*)(0x429D03D8UL)))
#define bM4_I2C3_SR_SMBALRTF                      (*((volatile unsigned int*)(0x429D03DCUL)))
#define bM4_I2C3_CLR_STARTFCLR                    (*((volatile unsigned int*)(0x429D0400UL)))
#define bM4_I2C3_CLR_SLADDR0FCLR                  (*((volatile unsigned int*)(0x429D0404UL)))
#define bM4_I2C3_CLR_SLADDR1FCLR                  (*((volatile unsigned int*)(0x429D0408UL)))
#define bM4_I2C3_CLR_TENDFCLR                     (*((volatile unsigned int*)(0x429D040CUL)))
#define bM4_I2C3_CLR_STOPFCLR                     (*((volatile unsigned int*)(0x429D0410UL)))
#define bM4_I2C3_CLR_RFULLFCLR                    (*((volatile unsigned int*)(0x429D0418UL)))
#define bM4_I2C3_CLR_TEMPTYFCLR                   (*((volatile unsigned int*)(0x429D041CUL)))
#define bM4_I2C3_CLR_ARLOFCLR                     (*((volatile unsigned int*)(0x429D0424UL)))
#define bM4_I2C3_CLR_NACKFCLR                     (*((volatile unsigned int*)(0x429D0430UL)))
#define bM4_I2C3_CLR_TMOUTFCLR                    (*((volatile unsigned int*)(0x429D0438UL)))
#define bM4_I2C3_CLR_GENCALLFCLR                  (*((volatile unsigned int*)(0x429D0450UL)))
#define bM4_I2C3_CLR_SMBDEFAULTFCLR               (*((volatile unsigned int*)(0x429D0454UL)))
#define bM4_I2C3_CLR_SMBHOSTFCLR                  (*((volatile unsigned int*)(0x429D0458UL)))
#define bM4_I2C3_CLR_SMBALRTFCLR                  (*((volatile unsigned int*)(0x429D045CUL)))
#define bM4_I2C3_DTR_DT0                          (*((volatile unsigned int*)(0x429D0480UL)))
#define bM4_I2C3_DTR_DT1                          (*((volatile unsigned int*)(0x429D0484UL)))
#define bM4_I2C3_DTR_DT2                          (*((volatile unsigned int*)(0x429D0488UL)))
#define bM4_I2C3_DTR_DT3                          (*((volatile unsigned int*)(0x429D048CUL)))
#define bM4_I2C3_DTR_DT4                          (*((volatile unsigned int*)(0x429D0490UL)))
#define bM4_I2C3_DTR_DT5                          (*((volatile unsigned int*)(0x429D0494UL)))
#define bM4_I2C3_DTR_DT6                          (*((volatile unsigned int*)(0x429D0498UL)))
#define bM4_I2C3_DTR_DT7                          (*((volatile unsigned int*)(0x429D049CUL)))
#define bM4_I2C3_DRR_DR0                          (*((volatile unsigned int*)(0x429D0500UL)))
#define bM4_I2C3_DRR_DR1                          (*((volatile unsigned int*)(0x429D0504UL)))
#define bM4_I2C3_DRR_DR2                          (*((volatile unsigned int*)(0x429D0508UL)))
#define bM4_I2C3_DRR_DR3                          (*((volatile unsigned int*)(0x429D050CUL)))
#define bM4_I2C3_DRR_DR4                          (*((volatile unsigned int*)(0x429D0510UL)))
#define bM4_I2C3_DRR_DR5                          (*((volatile unsigned int*)(0x429D0514UL)))
#define bM4_I2C3_DRR_DR6                          (*((volatile unsigned int*)(0x429D0518UL)))
#define bM4_I2C3_DRR_DR7                          (*((volatile unsigned int*)(0x429D051CUL)))
#define bM4_I2C3_CCR_SLOWW0                       (*((volatile unsigned int*)(0x429D0580UL)))
#define bM4_I2C3_CCR_SLOWW1                       (*((volatile unsigned int*)(0x429D0584UL)))
#define bM4_I2C3_CCR_SLOWW2                       (*((volatile unsigned int*)(0x429D0588UL)))
#define bM4_I2C3_CCR_SLOWW3                       (*((volatile unsigned int*)(0x429D058CUL)))
#define bM4_I2C3_CCR_SLOWW4                       (*((volatile unsigned int*)(0x429D0590UL)))
#define bM4_I2C3_CCR_SHIGHW0                      (*((volatile unsigned int*)(0x429D05A0UL)))
#define bM4_I2C3_CCR_SHIGHW1                      (*((volatile unsigned int*)(0x429D05A4UL)))
#define bM4_I2C3_CCR_SHIGHW2                      (*((volatile unsigned int*)(0x429D05A8UL)))
#define bM4_I2C3_CCR_SHIGHW3                      (*((volatile unsigned int*)(0x429D05ACUL)))
#define bM4_I2C3_CCR_SHIGHW4                      (*((volatile unsigned int*)(0x429D05B0UL)))
#define bM4_I2C3_CCR_FREQ0                        (*((volatile unsigned int*)(0x429D05C0UL)))
#define bM4_I2C3_CCR_FREQ1                        (*((volatile unsigned int*)(0x429D05C4UL)))
#define bM4_I2C3_CCR_FREQ2                        (*((volatile unsigned int*)(0x429D05C8UL)))
#define bM4_I2C3_FLTR_DNF0                        (*((volatile unsigned int*)(0x429D0600UL)))
#define bM4_I2C3_FLTR_DNF1                        (*((volatile unsigned int*)(0x429D0604UL)))
#define bM4_I2C3_FLTR_DNFEN                       (*((volatile unsigned int*)(0x429D0610UL)))
#define bM4_I2C3_FLTR_ANFEN                       (*((volatile unsigned int*)(0x429D0614UL)))

extern void hc32f460_wdt_irq_handler (void *p_arg);
extern am_hc32f460_wdt_dev_t __g_wdt_dev;
#define __HC32F460_WDT_STATUS_UNDERFLOW_MASK    (1 << 16)    /**< \brief 计数溢出标志 */
#define __HC32F460_WDT_STATUS_REFRESH_MASK      (1 << 17)    /**< \brief 刷新错误标志 */

#define __HC32F460_INTC_VSSEL_WDT_INT_MASK  (1 << 23)

/**
 *******************************************************************************
 ** \brief Int No.137 share IRQ handler
 **
 ******************************************************************************/
void IRQ141_Handler(void *parg)
{
    uint32_t VSSEL141 = HC32F460_INTC.VSSEL[141 - 128];
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    /** \brief I2C Ch.1 Receive completed */
    if(Set == bM4_I2C1_CR2_RFULLIE)
    {
        if ((Set == bM4_I2C1_SR_RFULLF) && (VSSEL141 & BIT_MASK_04))
        {
            I2c1RxEnd_IrqHandler(&__g_i2c1_dev);
        }
    }
    /** \brief I2C Ch.1 Transmit completed */
    if(Set == bM4_I2C1_CR2_TENDIE)
    {
        if ((Set == bM4_I2C1_SR_TENDF) && (VSSEL141 & BIT_MASK_05))
        {
            I2c1TxEnd_IrqHandler(&__g_i2c1_dev);
        }
    }
    /** \brief I2C Ch.1 Transmit data empty */
    if(Set == bM4_I2C1_CR2_TEMPTYIE)
    {
        if ((Set == bM4_I2C1_SR_TEMPTYF) && (VSSEL141 & BIT_MASK_06))
        {
            I2c1TxEmpty_IrqHandler(&__g_i2c1_dev);
        }
    }
    /** \brief I2C Ch.1 Error */
    u32Tmp1 = HC32F460_I2C1->CR2 & 0x00F05217ul;
    u32Tmp2 = HC32F460_I2C1->SR & 0x00F05217ul;
    if ((u32Tmp1 & u32Tmp2) && (VSSEL141 & BIT_MASK_07))
    {
        I2c1Err_IrqHandler(&__g_i2c1_dev);
    }
    /** \brief I2C Ch.2 Receive completed */
    if(Set == bM4_I2C2_CR2_RFULLIE)
    {
        if ((Set == bM4_I2C2_SR_RFULLF) && (VSSEL141 & BIT_MASK_08))
        {
            I2c2RxEnd_IrqHandler(&__g_i2c2_dev);
        }
    }
    /** \brief I2C Ch.2 Transmit completed */
    if(Set == bM4_I2C2_CR2_TENDIE)
    {
        if ((Set == bM4_I2C2_SR_TENDF)  && (VSSEL141 & BIT_MASK_09))
        {
            I2c2TxEnd_IrqHandler(&__g_i2c2_dev);
        }
    }
    /** \brief I2C Ch.2 Transmit data empty */
    if(Set == bM4_I2C2_CR2_TEMPTYIE)
    {
        if ((Set == bM4_I2C2_SR_TEMPTYF) && (VSSEL141 & BIT_MASK_10))
        {
            I2c2TxEmpty_IrqHandler(&__g_i2c2_dev);
        }
    }
    /** \brief I2C Ch.2 Error */
    u32Tmp1 = HC32F460_I2C2->CR2 & 0x00F05217ul;
    u32Tmp2 = HC32F460_I2C2->SR & 0x00F05217ul;
    if ((u32Tmp1 & u32Tmp2) && (VSSEL141 & BIT_MASK_11))
    {
        I2c2Err_IrqHandler(&__g_i2c2_dev);
    }
    /** \brief I2C Ch.3 Receive completed */
    if(Set == bM4_I2C3_CR2_RFULLIE)
    {
        if ((Set == bM4_I2C3_SR_RFULLF) && (VSSEL141 & BIT_MASK_12))
        {
            I2c3RxEnd_IrqHandler(&__g_i2c3_dev);
        }
    }
    /** \brief I2C Ch.3 Transmit completed */
    if(Set == bM4_I2C3_CR2_TENDIE)
    {
        if ((Set == bM4_I2C3_SR_TENDF)  && (VSSEL141 & BIT_MASK_13))
        {
            I2c3TxEnd_IrqHandler(&__g_i2c3_dev);
        }
    }
    /** \brief I2C Ch.3 Transmit data empty */
    if(Set == bM4_I2C3_CR2_TEMPTYIE)
    {
        if ((Set == bM4_I2C3_SR_TEMPTYF) && (VSSEL141 & BIT_MASK_14))
        {
            I2c3TxEmpty_IrqHandler(&__g_i2c3_dev);
        }
    }
    /** \brief I2C Ch.3 Error */
    u32Tmp1 = HC32F460_I2C3->CR2 & 0x00F05217ul;
    u32Tmp2 = HC32F460_I2C3->SR & 0x00F05217ul;
    if ((u32Tmp1 & u32Tmp2) && (VSSEL141 & BIT_MASK_15))
    {
        I2c3Err_IrqHandler(&__g_i2c3_dev);
    }
#if 0    
    /* LVD Ch.1 detected */
    if (Set == bM4_SYSREG_PWR_PVDCR1_PVD1IRE)
    {
        if((Set == bM4_SYSREG_PWR_PVDDSR_PVD1DETFLG) && (VSSEL141 & BIT_MASK_17))
        {
            Lvd1_IrqHandler();
        }
    }
    if (Set == bM4_SYSREG_PWR_PVDCR1_PVD2IRE)
    {
        /* LVD Ch.2 detected */
        if((Set == bM4_SYSREG_PWR_PVDDSR_PVD2DETFLG) && (VSSEL141 & BIT_MASK_18))
        {
            Lvd2_IrqHandler();
        }
    }
    /* Freq. calculate error detected */
    if(Set == bM4_FCM_RIER_ERRIE)
    {
        if((Set == bM4_FCM_SR_ERRF) && (VSSEL141 & BIT_MASK_20))
        {
            FcmErr_IrqHandler();
        }
    }
    /* Freq. calculate completed */
    if(Set == bM4_FCM_RIER_MENDIE)
    {
        if((Set == bM4_FCM_SR_MENDF) && (VSSEL141 & BIT_MASK_21))
        {
            FcmEnd_IrqHandler();
        }
    }
    /* Freq. calculate overflow */
    if(Set == bM4_FCM_RIER_OVFIE)
    {
        if((Set == bM4_FCM_SR_OVF) && (VSSEL141 & BIT_MASK_22))
        {
            FcmOV_IrqHandler();
        }
    }

#endif
    /** \brief WDT */
    if ((HC32F460_WDT->SR & (__HC32F460_WDT_STATUS_UNDERFLOW_MASK | __HC32F460_WDT_STATUS_REFRESH_MASK)) &&
        (VSSEL141 & __HC32F460_INTC_VSSEL_WDT_INT_MASK))
    {
        hc32f460_wdt_irq_handler(&__g_wdt_dev);
    }

}

#define __HC32F460_ADC_ICR_EOCBIEN_MASK    (1 << 1)   /**< \brief 序列B转换完成中断使能 */
#define __HC32F460_ADC_ICR_EOCAIEN_MASK    (1 << 0)   /**< \brief 序列A转换完成中断使能 */

#define __HC32F460_ADC_ISR_EOCBIEN_MASK    (1 << 1)   /**< \brief 序列B转换完成中断标志 */
#define __HC32F460_ADC_ISR_EOCAIEN_MASK    (1 << 0)   /**< \brief 序列A转换完成中断标志 */

#define __HC32F460_INTC_VSSEL_ADC1_EOCA_MASK   (1 << 0)
#define __HC32F460_INTC_VSSEL_ADC1_EOCB_MASK   (1 << 1)
#define __HC32F460_INTC_VSSEL_ADC1_CHCMP_MASK  (1 << 2)
#define __HC32F460_INTC_VSSEL_ADC1_SEQCMP_MASK (1 << 3)
#define __HC32F460_INTC_VSSEL_ADC2_EOCA_MASK   (1 << 4)
#define __HC32F460_INTC_VSSEL_ADC2_EOCB_MASK   (1 << 5)
#define __HC32F460_INTC_VSSEL_ADC2_CHCMP_MASK  (1 << 6)
#define __HC32F460_INTC_VSSEL_ADC2_SEQCMP_MASK (1 << 7)
extern void hc32f460_adc_irq_handle (void *p_arg);
extern am_hc32f460_adc_dev_t __g_adc1_dev;
extern am_hc32f460_adc_dev_t __g_adc2_dev;

/**
 *******************************************************************************
 ** \brief Int No.142 share IRQ handler
 **
 ******************************************************************************/
void IRQ142_Handler(void)
{
    uint32_t u32Tmp1 = 0ul;
    uint32_t u32Tmp2 = 0ul;
    uint32_t int_vssel142 = HC32F460_INTC.VSSEL[142 - 128];

    /** \brief ADC unit.1 seq. A */
    u32Tmp1 = HC32F460_ADC1->ICR;
    u32Tmp2 = HC32F460_ADC1->ISR;


    if (u32Tmp1 & __HC32F460_ADC_ICR_EOCAIEN_MASK)
    {
        if ((u32Tmp2 & __HC32F460_ADC_ISR_EOCAIEN_MASK) && (int_vssel142 & __HC32F460_INTC_VSSEL_ADC1_EOCA_MASK))
        {
            hc32f460_adc_irq_handle(&__g_adc1_dev);
        }
    }
    /** \brief ADC unit.1 seq. B */
    if (u32Tmp1 == __HC32F460_ADC_ICR_EOCAIEN_MASK)
    {
        if ((u32Tmp2 == __HC32F460_ADC_ISR_EOCAIEN_MASK) && (int_vssel142 & __HC32F460_INTC_VSSEL_ADC1_EOCB_MASK))
        {
            hc32f460_adc_irq_handle(&__g_adc1_dev);
        }
    }
#if 0
    /* ADC unit.1 seq. A */
    u16Tmp = M4_ADC1->AWDSR0;
    if (Set == bM4_ADC1_AWDCR_AWDIEN)
    {
        if (((Set == bM4_ADC1_AWDSR1_AWDF16) || (u16Tmp)) && (int_vssel142 & __HC32F460_INTC_VSSEL_ADC1_CHCMP_MASK))
        {
            ADC1ChCmp_IrqHandler();
        }
    }
    /* ADC unit.1 seq. cmp */
    if (Set == bM4_ADC1_AWDCR_AWDIEN)
    {
        if (((Set == bM4_ADC1_AWDSR1_AWDF16) || (u16Tmp)) && (int_vssel142 & __HC32F460_INTC_VSSEL_ADC1_SEQCMP_MASK))
        {
            ADC1SeqCmp_IrqHandler();
        }
    }
#endif

    u32Tmp1 = HC32F460_ADC2->ICR;
    u32Tmp2 = HC32F460_ADC2->ISR;
    /* ADC unit.2 seq. A */
    if (u32Tmp1 & __HC32F460_ADC_ICR_EOCAIEN_MASK)
    {
        if ((u32Tmp2 & __HC32F460_ADC_ISR_EOCAIEN_MASK) && (int_vssel142 & __HC32F460_INTC_VSSEL_ADC2_EOCA_MASK))
        {
            hc32f460_adc_irq_handle(&__g_adc2_dev);
        }
    }
    /* ADC unit.1 seq. B */
    if (u32Tmp1 == __HC32F460_ADC_ICR_EOCAIEN_MASK)
    {
        if ((u32Tmp2 == __HC32F460_ADC_ISR_EOCAIEN_MASK) && (int_vssel142 & __HC32F460_INTC_VSSEL_ADC2_EOCB_MASK))
        {
            hc32f460_adc_irq_handle(&__g_adc2_dev);
        }
    }
#if 0
    /* ADC unit.2 seq. A */
    if (Set == bM4_ADC2_AWDCR_AWDIEN)
    {
        if ((M4_ADC2->AWDSR0 & 0x1FFu) && (u32VSSEL142 & BIT_MASK_06))
        {
            ADC2ChCmp_IrqHandler();
        }
    }
    /* ADC unit.2 seq. cmp */
    if (Set == bM4_ADC2_AWDCR_AWDIEN)
    {
        if ((M4_ADC2->AWDSR0 & 0x1FFu) && (u32VSSEL142 & BIT_MASK_07))
        {
            ADC2SeqCmp_IrqHandler();
        }
    }
#endif
}

#define __HC32F460_SDIOC_NORINT_MASK         (0x1F7)
#define __HC32F460_SDIOC_ERRINT_MASK         (0x17F)
extern void hc32f460_sdioc_irq_handle (void *p_arg);
extern am_hc32f460_sdioc_dev_t __g_sdioc1_dev;
extern am_hc32f460_sdioc_dev_t __g_sdioc2_dev;
/**
 *******************************************************************************
 ** \brief Int No.143 share IRQ handler
 **
 ******************************************************************************/
void IRQ143_Handler(void)
{
    uint16_t NORINTST = 0u;
    uint16_t NORINTSGEN = 0u;
    uint16_t ERRINTST = 0u;
    uint16_t ERRINTSGEN = 0u;

    uint32_t int_vssel143 = HC32F460_INTC.VSSEL[143 - 128];

    /** \brief SDIO Ch.1 */
    if (int_vssel143 & AMHW_HC32F460_SDIOC1_INT_VSSEL_BITS_MASK)
    {
        NORINTST = HC32F460_SDIOC1->NORINTST;
        NORINTSGEN = HC32F460_SDIOC1->NORINTSGEN;
        ERRINTST = HC32F460_SDIOC1->ERRINTST;
        ERRINTSGEN = HC32F460_SDIOC1->ERRINTSGEN;

        if ((NORINTST & NORINTSGEN & __HC32F460_SDIOC_NORINT_MASK) || (ERRINTST & ERRINTSGEN & __HC32F460_SDIOC_ERRINT_MASK))
        {
            hc32f460_sdioc_irq_handle(&__g_sdioc1_dev);
        }
    }

    /** \brief SDIO Ch.2 */
    if (int_vssel143 == AMHW_HC32F460_SDIOC2_INT_VSSEL_BITS_MASK)
    {
        NORINTST = HC32F460_SDIOC2->NORINTST;
        NORINTSGEN = HC32F460_SDIOC2->NORINTSGEN;
        ERRINTST = HC32F460_SDIOC2->ERRINTST;
        ERRINTSGEN = HC32F460_SDIOC2->ERRINTSGEN;

        if ((NORINTST & NORINTSGEN & __HC32F460_SDIOC_NORINT_MASK) || (ERRINTST & ERRINTSGEN & __HC32F460_SDIOC_ERRINT_MASK))
        {
        	hc32f460_sdioc_irq_handle(&__g_sdioc2_dev);
        }
    }
#if 0
    /** \brief CAN */
    if (Set == bM4_INTC_VSSEL143_VSEL6)
    {
        RTIF = M4_CAN->RTIF;
        RTIE = M4_CAN->RTIE;
        ERRINT = M4_CAN->ERRINT;
        TTCFG = M4_CAN->TTCFG;
        if ( (TTCFG & BIT_MASK_05)                                  ||         \
             (RTIF & BIT_MASK_00)                                   ||         \
             (RTIF & RTIE & 0xFEu)                                  ||         \
             ((ERRINT & BIT_MASK_00) && (ERRINT & BIT_MASK_01))     ||         \
             ((ERRINT & BIT_MASK_02) && (ERRINT & BIT_MASK_03))     ||         \
             ((ERRINT & BIT_MASK_04) && (ERRINT & BIT_MASK_05))     ||         \
             ((TTCFG & BIT_MASK_03) && (TTCFG & BIT_MASK_04))       ||         \
             ((TTCFG & BIT_MASK_06) && (TTCFG & BIT_MASK_07)))
        {
            Can_IrqHandler();
        }
    }
#endif
}

/* end of file */
