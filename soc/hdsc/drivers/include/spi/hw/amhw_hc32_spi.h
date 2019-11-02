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
 * \brief SPI 硬件操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-0-17  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32_SPI_H
#define __AMHW_HC32_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_hc32_if_spi
 * \copydoc amhw_hc32_spi.h
 * @{
 */

/**
 * \brief SPI structure of register
 */
typedef struct amhw_hc32_spi {
    __IO uint32_t  cr;       /**< \brief SPI 配置寄存器 */
    __IO uint32_t  ssn;      /**< \brief SPI 片选配置寄存器 */
    __I  uint32_t  stat;     /**< \brief SPI 状态寄存器 */
    __IO uint32_t  data;     /**< \brief SPI 数据寄存器 */
    __IO uint32_t  cr2;      /**< \brief SPI 配置寄存器 2 */
    __O  uint32_t  iclr;     /**< \brief SPI 中断清除寄存器 */
} amhw_hc32_spi_t;

/**
 * \brief 波特率设置参数
 */
typedef enum {
    AMHW_HC32_SPI_BAUD_RATE_PCLK_DIV2 = 0,  /**< \brief 波特率为外设时钟的2分频 */
    AMHW_HC32_SPI_BAUD_RATE_PCLK_DIV4,      /**< \brief 波特率为外设时钟的4分频 */
    AMHW_HC32_SPI_BAUD_RATE_PCLK_DIV8,      /**< \brief 波特率为外设时钟的8分频 */
    AMHW_HC32_SPI_BAUD_RATE_PCLK_DIV16,     /**< \brief 波特率为外设时钟的16分频 */
    AMHW_HC32_SPI_BAUD_RATE_PCLK_DIV32,     /**< \brief 波特率为外设时钟的32分频 */
    AMHW_HC32_SPI_BAUD_RATE_PCLK_DIV64,     /**< \brief 波特率为外设时钟的64分频 */
    AMHW_HC32_SPI_BAUD_RATE_PCLK_DIV128,    /**< \brief 波特率为外设时钟的128分频 */
    AMHW_HC32_SPI_BAUD_RATE_PCLK_0,         /**< \brief 波特率为0 */
}amhw_hc32_spi_baud_rate_t;

/**
 * \brief SPI  波特率设置   获取
 * \param[in] p_hw_spi  : 指向SPI寄存器结构体的指针
 *
 * \return 波特率设置参数(amhw_hc32_spi_baud_rate_t枚举量)
 */
am_static_inline
amhw_hc32_spi_baud_rate_t
amhw_hc32_spi_baud_rate_get (amhw_hc32_spi_t *p_hw_spi)
{
   return (amhw_hc32_spi_baud_rate_t)((p_hw_spi->cr & 0x3ul) | 
                                        ((p_hw_spi->cr >> 5) & 0x4ul));
}

/**
 * \brief SPI  波特率设置   设置
 * \param[in] p_hw_spi  : 指向SPI寄存器结构体的指针
 * \param[in] baud_rate : 波特率设置参数(amhw_hc32_spi_baud_rate_t枚举量)
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_baud_rate_set (amhw_hc32_spi_t          *p_hw_spi,
                                    amhw_hc32_spi_baud_rate_t baud_rate)
{
    /* 对bit7 、 bit1 、bit0 清零 */
    p_hw_spi->cr &= ~(0x83ul);

    /* 设置bit7的值 */
    p_hw_spi->cr |= ((baud_rate & 0x4ul) << 5);

    /* 设置bit1、bit0的值 */
    p_hw_spi->cr |= ((baud_rate & 0x3ul) << 0);
}

/**
 * \brief SPI 模块使能设置
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : AM_TRUE or AM_FALSE
 * \return none
 */
am_static_inline
void amhw_hc32_spi_enable (amhw_hc32_spi_t *p_hw_spi, am_bool_t flag)
{
    p_hw_spi->cr = (p_hw_spi->cr & (~(1u << 6))) | (flag << 6);
}

/**
 * \brief 主机/从机模式选择
 */
typedef enum {
    AMHW_HC32_SPI_MODE_TYPE_MASTER = 1,  /**< \brief 主机模式 */
    AMHW_HC32_SPI_MODE_TYPE_SLAVER = 0,  /**< \brief 从机模式 */
}amhw_hc32_spi_mode_type_t;

/**
 * \brief SPI 主机/从机模式选择
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : amhw_hc32_spi_mode_type_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_mode_sel (amhw_hc32_spi_t          *p_hw_spi,
                               amhw_hc32_spi_mode_type_t mode_type)
{
    p_hw_spi->cr = (p_hw_spi->cr & (~(1u << 4))) | (mode_type << 4);
}

/**
 * \brief 时钟模式选择
 * \ @{
 */
#define AMHW_HC32_SPI_CLK_MODE_0    0 /**< \brief Clock mode 0 */
#define AMHW_HC32_SPI_CLK_MODE_1    1 /**< \brief Clock mode 1 */
#define AMHW_HC32_SPI_CLK_MODE_2    2 /**< \brief Clock mode 2 */
#define AMHW_HC32_SPI_CLK_MODE_3    3 /**< \brief Clock mode 3 */

/** @}*/

/**
 * \brief 时钟模式选择
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : 模式设置宏定义
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_clk_mode_set (amhw_hc32_spi_t *p_hw_spi, uint32_t flag)
{
    p_hw_spi->cr = (p_hw_spi->cr & (~(3u << 2))) | (flag << 2);
}

/**
 * \brief 主机模式下,软件配置 ssn值控制 SPI_CS端口电平高低
 * \param[in] p_hw_spi  : 指向SPI寄存器结构体的指针
 * \param[in] flag      : AM_TRUE : 片选有效
 *                        AM_FALSE: 片选失效
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_cs_sel (amhw_hc32_spi_t *p_hw_spi, am_bool_t flag)
{
    p_hw_spi->ssn = (~flag) & 0x1;
}

/**
 * \brief SPI状态标志
 *  @{
 */
#define AMHW_HC32_SPI_FLAG_TX_FINISH        (1U << 7) /* 传输结束标志 */
#define AMHW_HC32_SPI_FLAG_SLAVER_SSN_ERR   (1U << 5) /* 从机模式SSN错误标志*/
#define AMHW_HC32_SPI_FLAG_MASTER_ERR       (1U << 4) /* 主机模式错误标志 */
#define AMHW_HC32_SPI_FLAG_BUSY             (1U << 3) /* SPI总线忙标志 */
#define AMHW_HC32_SPI_FLAG_TX_EMPTY         (1U << 2) /* 发送缓冲器空标志 */
#define AMHW_HC32_SPI_FLAG_RX_NOT_EMPTY     (1U << 1) /* 接收缓冲器非空标志 */
/**
 * \brief 标志有效性判断
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] flag     : 标志宏定义
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32_spi_flag_check (amhw_hc32_spi_t *p_hw_spi,
                                      uint8_t            flag)
{
    return ((p_hw_spi->stat & flag)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief  SPI中断清除
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_flag_clr (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->iclr &= ~(0x1ul << 0);
}

/**
 * \brief  SPI中断保持
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_flag_hold (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->iclr |= (0x1ul << 0);
}

/**
 * \brief  8bit数据写
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] data     : 待发送数据
 * \param[in] lsbfirst : 低位先发送标志，硬件只有高位先发送功能。为适应标准层，软件实现低位先发送功能
 *
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_tx_data8_put (amhw_hc32_spi_t *p_hw_spi,
                                   uint8_t            data,
                                   am_bool_t          lsbfirst)
{
    uint8_t temp = 0,i = 0;

    /* 移位操作 */
    if(lsbfirst) {
        for(i = 0; i < 8; i++) {
            if(((data >> i) & 0x1ul) == 1) {
                temp |= (1 << (7 - i));
            }
        }
        data = temp;
    }

    p_hw_spi->data = data;
}

/**
 * \brief 8bit数据读
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 * \param[in] lsbfirst : 低位先发送标志，硬件只有高位先发送功能。为适应标准层，软件实现低位先发送功能
 *
 * \return 接收数据
 */
am_static_inline
uint8_t amhw_hc32_spi_rx_data8_get (amhw_hc32_spi_t *p_hw_spi,
                                      am_bool_t          lsbfirst)
{
    uint8_t temp = 0,i = 0;

    uint8_t data = p_hw_spi->data;


    /* 移位操作 */
    if(lsbfirst) {
        for(i = 0; i < 8; i++) {
            if(((data >> i) & 0x1ul) == 1) {
                temp |= (1 << (7 - i));
            }
        }
        data = temp;
    }

    return data;
}

/**
 * \brief 接收缓冲器非空中断使能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_int_rx_not_empty_enable (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 |= (0x1ul << 6);
}

/**
 * \brief 接收缓冲器非空中断禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_int_rx_not_empty_disable (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 &= ~(0x1ul << 6);
}

/**
 * \brief 发送缓冲器空中断使能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_tx_empty_enable (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 |= (0x1ul << 5);
}

/**
 * \brief 发送缓冲器空中断禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_tx_empty_disable (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 &= ~(0x1ul << 5);
}

/**
 * \brief DMA硬件访问发送使能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_dma_tx_enable (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 |= (0x1ul << 4);
}

/**
 * \brief DMA硬件访问发送禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_dma_tx_disable (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 &= ~(0x1ul << 4);
}

/**
 * \brief DMA硬件访问接收使能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_dma_rx_enable (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 |= (0x1ul << 3);
}

/**
 * \brief DMA硬件访问接收禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_dma_rx_disable (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 &= ~(0x1ul << 3);
}

/**
 * \brief SPI中断使能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_int_enable (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 |= (0x1ul << 2);
}

/**
 * \brief SPI中断禁能
 * \param[in] p_hw_spi : 指向SPI寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32_spi_int_disable (amhw_hc32_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 &= ~(0x1ul << 2);
}

/**
 * @} amhw_hc32_if_spi
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32_SPI_H */

/* end of file */
