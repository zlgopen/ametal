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
 * \brief  SPI硬件层实现文件
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-26  sdy, first implementation.
 * \endinternal
 */

#include "hw/amhw_fsl_spi.h"

/**
 * \brief 使能SPI普通中断
 */
void amhw_fsl_spi_int_enable (amhw_fsl_spi_t *p_hw_spi, uint8_t irq_flags)
{
    if ( irq_flags & AMHW_FSL_SPI_IRQ_MATCH) {       /* 数据匹配中断       */
        p_hw_spi->c2 |= 0x80;
    }
    if ( irq_flags & AMHW_FSL_SPI_IRQ_R_MODDEF) {    /* 接收或多主机中断   */
        p_hw_spi->c1 |= 0x80;
    }
    if ( irq_flags & AMHW_FSL_SPI_IRQ_T) {           /* 发送完成中断       */
        p_hw_spi->c1 |= 0x20;
    }
}

/**
 * \brief 禁能SPI普通中断
 */
void amhw_fsl_spi_int_disable (amhw_fsl_spi_t *p_hw_spi, uint8_t irq_flags)
{
    if ( irq_flags & AMHW_FSL_SPI_IRQ_MATCH) {       /* 数据匹配中断       */
        p_hw_spi->c2 &= (~0x80);
    }
    if ( irq_flags & AMHW_FSL_SPI_IRQ_R_MODDEF) {    /* 接收或多主机中断   */
        p_hw_spi->c1 &= (~0x80);
    }
    if ( irq_flags & AMHW_FSL_SPI_IRQ_T) {           /* 发送完成中断       */
        p_hw_spi->c1 &= (~0x20);
    }
}

/**
 * \brief 配置SPI工作模式
 */
void amhw_fsl_spi_workmode_cfg (amhw_fsl_spi_t *p_hw_spi, uint8_t mode_flag)
{
    uint8_t tmp  = mode_flag & 0x10;

    p_hw_spi->c1 = (tmp | ( p_hw_spi->c1 & (~0x10)));   /* 主从机选择     */

    tmp          =  mode_flag & 0x01;
    p_hw_spi->c2 = (tmp | ( p_hw_spi->c2 & (~0x01)));   /* 单双数据线选择 */
}

/**
 * \brief 配置SPI工作特性
 */
void amhw_fsl_spi_feature_cfg (amhw_fsl_spi_t *p_hw_spi, uint32_t flags)
{
    if (flags & AM_BIT(31)) { /* 传输数据宽度（8bit/16bit） */
        p_hw_spi->c2 = (((flags & 0x01) << 6) | (p_hw_spi->c2 & (~AM_BIT(6))));
    }
    if (flags & AM_BIT(30)) {   /* 是否使能发送DMA          */
        p_hw_spi->c2 = (((flags & 0x02) << 4) | (p_hw_spi->c2 & (~AM_BIT(5))));
    }
    if (flags & AM_BIT(29)) {   /* 是否使能接收DMA          */
        p_hw_spi->c2 = (((flags & 0x04) << 0) | (p_hw_spi->c2 & (~AM_BIT(2))));
    }
    if (flags & AM_BIT(28)) {   /* 是否在wait模式下工作     */
        p_hw_spi->c2 = (((flags & 0x08) >> 2) | (p_hw_spi->c2 & (~AM_BIT(1))));
    }
    if (flags & AM_BIT(27)) {   /* 高位先发送还是低位先发送 */
        p_hw_spi->c1 = (((flags & 0x10) >> 4) | (p_hw_spi->c1 & (~AM_BIT(0))));
    }
}

/**
 * \brief 配置SPI片选引脚CS功能
 */
void amhw_fsl_spi_cs_cfg (amhw_fsl_spi_t *p_hw_spi, uint8_t cs_flag)
{
    uint8_t tmp  = cs_flag & 0x10;

    p_hw_spi->c2 = (tmp | (p_hw_spi->c2 & (~0x10))); /*  配置是否为GPIO     */

    tmp = cs_flag & 0x02;
    p_hw_spi->c1 = (tmp | (p_hw_spi->c1 & (~0x02))); /*  配置为输入还是输出 */
}

/* end of file */
