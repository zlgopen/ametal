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
 * 1. 支持主机和从机传输；
 * 2. 支持单数据线模式；
 * 3. 全双工同步传输
 * 4. 支持DMA传输(发送和接收)；
 * 5. 波特率可配置；
 * 6. 时钟相位和极性可配置；
 * 7. 可配置LSB或者MSB传输；
 * 8. 可以设置匹配接收数据，匹配成功时产生中断；
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-18  ari, first implementation
 * \endinternal
 */

#ifndef _AMHW_HK32F103RBT6_SPI_H_
#define _AMHW_HK32F103RBT6_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_hk32f103rbt6_if_spi
 * \copydoc amhw_hk32f103rbt6_spi.h
 * @{
 */

/**
 * \brief SPI structure of register
 */
typedef struct amhw_hk32f103rbt6_spi {
    __IO uint16_t  cr1;           /**< \brief SPI 控制寄存器1 */
    __I  uint16_t  reserve1;      /**< \brief 保留位 */
    __IO uint16_t  cr2;           /**< \brief SPI 控制寄存器2 */
    __I  uint16_t  reserve2;      /**< \brief 保留位 */
    __IO uint16_t  sr;            /**< \brief SPI 状态寄存器 */
    __I  uint16_t  reserve3;      /**< \brief 保留位 */
    __IO uint16_t  dr;            /**< \brief SPI 数据寄存器 */
    __I  uint16_t  reserve4;      /**< \brief 保留位 */
    __IO uint16_t  crcpr;         /**< \brief SPI CRC多项式寄存器 */
    __I  uint16_t  reserve5;      /**< \brief 保留位 */
    __I  uint16_t  rxcrcr;        /**< \brief SPI RX_CRC寄存器 */
    __I  uint16_t  reserve6;      /**< \brief 保留位 */
    __I  uint16_t  txcrcr;        /**< \brief SPI TX_CRC寄存器 */
    __I  uint16_t  reserve7;      /**< \brief 保留位 */
    __IO uint16_t  cr3;           /**< \brief SPI 控制寄存器3 */
    __I  uint16_t  reserve8;      /**< \brief 保留位 */
} amhw_hk32f103rbt6_spi_t;

/**
 * \brief BIDIMODE 模式选择——双线双向模式
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_bidimode_double (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 &= ~(1u << 15);
}

/**
 * \brief BIDIMODE 模式选择——单线双向模式
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_bidimode_single (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 |= (1u << 15);
}

/**
 * \brief BIDIOE 双向模式下输出控制——输入（只收）
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note 和 BIDIMODE位一起决定在“单线双向”模式下数据的输出方向
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_bidioe_input (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 &= ~(1u << 14);
}

/**
 * \brief BIDIOE 双向模式下输出控制——输出（只发）
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note 和 BIDIMODE位一起决定在“单线双向”模式下数据的输出方向
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_bidioe_output (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 |= (1u << 14);
}

/**
 * \brief CRCEN　硬件 CRC校验禁能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note 只有在禁止 SPI 时(SPE=0)，才能写该位，否则出错。该位只能在全双工模式下使用。
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_crc_disable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 &= ~(1u << 13);
}

/**
 * \brief CRCEN　硬件 CRC校验使能
 * \\param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note 只有在禁止 SPI 时(SPE=0)，才能写该位，否则出错。该位只能在全双工模式下使用
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_crc_enable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 |= (1u << 13);
}

/**
 * \brief current status
 *  @{
 */
#define AMHW_HK32F103RBT6_SPI_CRC_NEXT_BUFF          0     /**< 下一个发送的值来自发送缓冲区 */
#define AMHW_HK32F103RBT6_SPI_CRC_NEXT_CRCREG        1     /**< 下一个发送的值来自CRC寄存器 */

#define AMHW_HK32F103RBT6_SPI_DATA_8BIT              0     /**<  8位数据帧格式 */
#define AMHW_HK32F103RBT6_SPI_DATA_16BIT             1     /**< 16位数据帧格式 */
/** @}*/

/**
 * \brief CRCNEXT 下一个发送的值来源设置
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_HK32F103RBT6_SPI_CRC_NEXT_BUFF  或
 *                            AMHW_HK32F103RBT6_SPI_CRC_NEXT_CRCREG
 *
 * \note ：在 SPI_DR寄存器写入最后一个数据后应马上设置该位
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_crcnext_set (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t flag)
{
    p_hw_spi->cr1 = (p_hw_spi->cr1 & (~(1u << 12))) | ((uint16_t)flag << 12);
}

/**
 * \brief DFF 数据帧格式长度设置
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_HK32F103RBT6_SPI_DATA_8BIT  或
 *                            AMHW_HK32F103RBT6_SPI_DATA_16BIT
 *
 * \note ：只有当 SPI禁止(SPE=0)时，才能写该位，否则出错
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_data_length_set (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t flag)
{
    p_hw_spi->cr1 = (p_hw_spi->cr1 & (~(1u << 11))) | ((uint16_t)flag << 11);
}

/**
 * \brief RXONLY 只接收禁能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note 该位和 BIDIMODE 位一起决定在“双线双向”模式下的传输方向。在多个从设备的
 *       配置中，在未被访问的从设备上该位被置 1，使得只有被访问的从设备有输出，从而不
 *       会造成数据线上数据冲突。
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_rxonly_disable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 &= ~(1u << 10);
}

/**
 * \brief RXONLY 只接收使能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note 该位和 BIDIMODE 位一起决定在“双线双向”模式下的传输方向。在多个从设备的
 *       配置中，在未被访问的从设备上该位被置 1，使得只有被访问的从设备有输出，从而不
 *       会造成数据线上数据冲突。
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_rxonly_enable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 |= (1u << 10);
}

/**
 * \brief SSM 软件从设备管理禁能（NSS软件管理禁能）
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note 当 SSM 被置位时，NSS 引脚上的电平由 SSI 位的值决定
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_ssm_disable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 &= ~(1u << 9);
}

/**
 * \brief SSM 软件从设备管理使能 （NSS软件管理使能）
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note 当 SSM 被置位时，NSS 引脚上的电平由 SSI 位的值决定
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_ssm_enable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 |= (1u << 9);
}

/**
 * \brief current status
 *  @{
 */
#define AMHW_HK32F103RBT6_SPI_SSI_TO_NSS_IGNORED      0     /**< NSS引脚忽略  */
#define AMHW_HK32F103RBT6_SPI_SSI_TO_NSS_ENABLE       1     /**< NSS引脚使能  */

#define AMHW_HK32F103RBT6_SPI_LSB_FIRST_SEND_MSB      0     /**< 帧格式设置   先发送 MSB */
#define AMHW_HK32F103RBT6_SPI_LSB_FIRST_SEND_LSB      1     /**< 帧格式设置   先发送LSB */
/** @}*/

/**
 * \brief SSI 内部从设备选择
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_HK32F103RBT6_SPI_SSI_TO_NSS_IGNORED  或
 *                            AMHW_HK32F103RBT6_SPI_SSI_TO_NSS_ENABLE
 *
 * \note ：该位只在 SSM位为‘1’时有意义。它决定了 NSS上的电平，在 NSS引脚上的 I/O操作无效，即与引脚无关
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_ssi_set (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t flag)
{
    p_hw_spi->cr1 = (p_hw_spi->cr1 & (~(1u << 8))) | ((uint16_t)flag << 8);
}

/**
 * \brief LSBFIRST 帧格式设置
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_HK32F103RBT6_SPI_LSB_FIRST_MSB  或
 *                            AMHW_HK32F103RBT6_SPI_LSB_FIRST_LSB
 *
 * \note ：当通信在进行时不能改变该位的值。
 *
 * \return none
 */
am_static_inline

void amhw_hk32f103rbt6_spi_lsbfirst_set (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t flag)
{
    p_hw_spi->cr1 = (p_hw_spi->cr1 & (~(1u << 7))) | ((uint16_t)flag << 7);
}

/**
 * \brief SPE SPI禁能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_disable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 &= ~(1u << 6);
}

/**
 * \brief SPE SPI使能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_enable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr1 |= (1u << 6);
}

/**
 * \brief current status
 *  @{
 */
#define AMHW_HK32F103RBT6_SPI_BAUDRATE_PRESCALER_2       0     /**< fPCLK/2 */
#define AMHW_HK32F103RBT6_SPI_BAUDRATE_PRESCALER_4       1     /**< fPCLK/4 */
#define AMHW_HK32F103RBT6_SPI_BAUDRATE_PRESCALER_8       2     /**< fPCLK/8 */
#define AMHW_HK32F103RBT6_SPI_BAUDRATE_PRESCALER_16      3     /**< fPCLK/16 */
#define AMHW_HK32F103RBT6_SPI_BAUDRATE_PRESCALER_32      4     /**< fPCLK/32 */
#define AMHW_HK32F103RBT6_SPI_BAUDRATE_PRESCALER_64      5     /**< fPCLK/64 */
#define AMHW_HK32F103RBT6_SPI_BAUDRATE_PRESCALER_128     6     /**< fPCLK/128 */
#define AMHW_HK32F103RBT6_SPI_BAUDRATE_PRESCALER_256     7     /**< fPCLK/256 */
/** @}*/

/**
 * \brief BR[2:0] 波特率分频控制
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_HK32F103RBT6_SPI_BAUDRATE_PRESCALER_2
 *                            .
 *                            .
 *                            .
 *                            AMHW_HK32F103RBT6_SPI_BAUDRATE_PRESCALER_256
 *
 * \note ：当通信在进行时不能改变该位的值。
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_baudratefre_set (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t flag)
{
    p_hw_spi->cr1 = (p_hw_spi->cr1 & (~(0x7u << 3))) | ((uint16_t)( 0x7u & flag ) << 3);
}

/**
 * \brief current status
 *  @{
 */
#define AMHW_HK32F103RBT6_SPI_MASTER                   1     /**< 设置为主设备 */
#define AMHW_HK32F103RBT6_SPI_SLAVER                   0     /**< 设置为从设备 */
/** @}*/

/**
 * \brief MSTR 主设备选择
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_HK32F103RBT6_SPI_MASTER  或
 *                            AMHW_HK32F103RBT6_SPI_MASTER
 *
 * \note ：当通信在进行时不能改变该位的值。
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_master_salver_set (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint16_t flag)
{
    p_hw_spi->cr1 = (p_hw_spi->cr1 & (~(1u << 2))) | ((uint16_t)flag << 2);
}

/**
 * \brief SPI Clock mode set
 * \ @{
 */
#define AMHW_HK32F103RBT6_SPI_CLK_MODE_0    (0U << 0) /* 模式0（空闲低电平，第一个时钟沿）*/
#define AMHW_HK32F103RBT6_SPI_CLK_MODE_1    (1U << 0) /* 模式1（空闲低电平，第二个时钟沿）*/
#define AMHW_HK32F103RBT6_SPI_CLK_MODE_2    (2U << 0) /* 模式2（空闲高电平，第一个时钟沿）*/
#define AMHW_HK32F103RBT6_SPI_CLK_MODE_3    (3U << 0) /* 模式3（空闲高电平，第二个时钟沿）*/
/** @}*/

/**
 * \brief SPI 时钟模式选择
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_HK32F103RBT6_SPI_CLK_MODE_0  或
 *                            AMHW_HK32F103RBT6_SPI_CLK_MODE_1  或
 *                            AMHW_HK32F103RBT6_SPI_CLK_MODE_2  或
 *                            AMHW_HK32F103RBT6_SPI_CLK_MODE_3  或
 *
 * \note ：当通信在进行时不能改变该位的值。
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_clk_mode_set (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t flag)
{
    p_hw_spi->cr1 = (p_hw_spi->cr1 & (~(3u << 0))) | ((uint16_t)flag << 0);
}

/**
 * \brief current status
 *  @{
 */
#define AMHW_HK32F103RBT6_SPI_INT_TX_EMPTY          (1u << 7)     /**< 发送缓冲区空中断使能控制位 */
#define AMHW_HK32F103RBT6_SPI_INT_RX_NOT_EMPTY      (1u << 6)     /**< 接收缓冲区非空中断使能控制位 */
#define AMHW_HK32F103RBT6_SPI_INT_ERROR             (1u << 5)     /**< 错误中断使能控制位 */
#define AMHW_HK32F103RBT6_SPI_INT_ALL             (0x7u << 5)     /**< 所有中断使能控制位 */

/** @}*/

/**
 * \brief cr2中断控制禁能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_HK32F103RBT6_SPI_INT_TX_EMPTY      或
 *                            AMHW_HK32F103RBT6_SPI_INT_RX_NOT_EMPTY  或
 *                            AMHW_HK32F103RBT6_SPI_INT_ERROR
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_int_disable (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t flag)
{
    p_hw_spi->cr2 &= ~flag;
}

/**
 * \brief cr2中断控制使能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_HK32F103RBT6_SPI_INT_TX_EMPTY      或
 *                            AMHW_HK32F103RBT6_SPI_INT_RX_NOT_EMPTY  或
 *                            AMHW_HK32F103RBT6_SPI_INT_ERROR
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_int_enable (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t flag)
{
    p_hw_spi->cr2 |= flag;
}

/**
 * \brief SSOE 硬件SPI的NSS引脚输出禁能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_ssout_disable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 &= ~(1u << 2);
}

/**
 * \brief SSOE 硬件SPI的NSS引脚输出使能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_ssout_enable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 |= (1u << 2);
}

/**
 * \brief TXDMAEN 发送缓冲区 DMA禁能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_dma_tx_disable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 &= ~(1u << 1);
}

/**
 * \brief TXDMAEN 发送缓冲区 DMA使能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_dma_tx_enable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 |= (1u << 1);
}

/**
 * \brief RXDMAEN 接收缓冲区DMA禁能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_dma_rx_disable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 &= ~(1u << 0);
}

/**
 * \brief RXDMAEN 接收缓冲区DMA使能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_dma_rx_enable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr2 |= (1u << 0);
}

/**
 * \brief current status
 *  @{
 */
/** 忙标志
 *
 *  note: 该位由硬件置位或者复位
 */
#define AMHW_HK32F103RBT6_SPI_BUSY_FLAG            (1u << 7)

/** 溢出标志
 *
 *  note: 该位由硬件置位，由软件序列复位
 */
#define AMHW_HK32F103RBT6_SPI_OVERRUN_FLAG         (1u << 6)

/** 模式错误标志
 *
 *  note: 该位由硬件置位，由软件序列复位。
 */
#define AMHW_HK32F103RBT6_SPI_MDOE_ERROR_FLAG      (1u << 5)

/** CRC错误标志
 *
 *  note ：该位由硬件置位，由软件写‘0’而复位。
 */
#define AMHW_HK32F103RBT6_SPI_CRC_ERROR_FLAG       (1u << 4)

/** 下溢标志
 *
 *  note: 该标志位由硬件置‘1’，由一个软件序列清‘0’
 *       在 SPI 模式下不使用。
 */
#define AMHW_HK32F103RBT6_SPI_UNDERRUN_FLAG        (1u << 3)

/** 声道标志
 *  对于声道标志而言，调用函数后的返回值分别代表
 *              AM_TRUE  : 需要传输或者接收左声道
 *              AM_FALSE : 需要传输或者接收右声道
 *
 *  note ：在 SPI 模式下不使用。在 PCM 模式下无意义。
 */
#define AMHW_HK32F103RBT6_SPI_CHANNEL_SIDE_FLAG    (1u << 2)

/** 发送缓冲为空标志
 *
 *  note ：只读
 */
#define AMHW_HK32F103RBT6_SPI_TX_EMPTY_FLAG        (1u << 1)

/** 接收缓冲非空标志
 *
 *  note ：只读
 */
#define AMHW_HK32F103RBT6_SPI_RX_NOT_EMPTY_FLAG    (1u << 0)

/** @}*/

/**
 * \brief 状态寄存器标志位获取
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag : 传入参数应为   AMHW_HK32F103RBT6_SPI_BUSY_FLAG          或
 *                            AMHW_HK32F103RBT6_SPI_OVERRUN_FLAG       或
 *                            AMHW_HK32F103RBT6_SPI_MDOE_ERROR_FLAG    或
 *                            AMHW_HK32F103RBT6_SPI_CRC_ERROR_FLAG     或
 *                            AMHW_HK32F103RBT6_SPI_UNDERRUN_FLAG      或
 *                            AMHW_HK32F103RBT6_SPI_CHANNEL_SIDE_FLAG  或
 *                            AMHW_HK32F103RBT6_SPI_TX_EMPTY_FLAG      或
 *                            AMHW_HK32F103RBT6_SPI_RX_NOT_EMPTY_FLAG
 *
 * \return none
 */
am_static_inline
am_bool_t amhw_hk32f103rbt6_spi_status_flag_check (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t flag)
{
    return (p_hw_spi->sr & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 状态寄存器标志位清除
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag     : 传入参数应只能为   AMHW_HK32F103RBT6_SPI_CRC_ERROR_FLAG
 *                       其他宏定义只读或者由软件序列复位
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_status_flag_clr (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t flag)
{
    p_hw_spi->sr &= ~flag;
}

/**
 * \brief 写数据函数
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] data     : 待写入数据
 *
 * \note : 根据 SPI_CR1的 DFF位对数据帧格式的选择，数据的发送和接收可以是 8位或者 16位的。
 *         为保证正确的操作，需要在启用 SPI 之前就确定好数据帧格式.
 *
 *         对于 8 位的数据，缓冲器是 8位的，发送和接收时只会用到SPI_DR[7:0]。在接收时，
 *         SPI_DR[15:8]被强制为 0。
 *
 *         对于 16 位的数据，缓冲器是 16 位的，发送和接收时会用到整个数据寄存器，即 SPI_DR[15:0]。
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_tx_put (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint16_t data)
{
    p_hw_spi->dr = (uint16_t)data;
}

/**
 * \brief 读数据函数
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note : 根据 SPI_CR1的 DFF位对数据帧格式的选择，数据的发送和接收可以是 8位或者 16位的。
 *         为保证正确的操作，需要在启用 SPI 之前就确定好数据帧格式。
 *
 *         对于 8 位的数据，缓冲器是 8位的，发送和接收时只会用到SPI_DR[7:0]。在接收时，
 *         SPI_DR[15:8]被强制为 0。
 *
 *         对于 16 位的数据，缓冲器是 16 位的，发送和接收时会用到整个数据寄存器，即 SPI_DR[15:0]。
 *
 * \return 返回数据
 */
am_static_inline
uint16_t amhw_hk32f103rbt6_spi_rx_get (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    return p_hw_spi->dr;
}

/**
 * \brief CRC多项式寄存器
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag     : 待写入CRC计算时用到的多项式数据
 *
 * \note ：  该寄存器包含了 CRC计算时用到的多项式。
 *         其复位值为 0x0007，根据应用可以设置其他数值。
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_crcpr_write (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint16_t crc_data)
{
    p_hw_spi->crcpr = crc_data;
}

/**
 * \brief CRC多项式寄存器  读出数据
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note ：  该寄存器包含了 CRC计算时用到的多项式。
 *         其复位值为 0x0007，根据应用可以设置其他数值。
 *
 * \return 当前寄存器存储的CRC计算时用到的多项式数据
 */
am_static_inline
uint16_t amhw_hk32f103rbt6_spi_crcpr_read (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    return p_hw_spi->crcpr;
}

/**
 * \brief RxCRC寄存器（只读）
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note ：  当在SPI_CR1 的CRCEN 位写入‘1’时，该寄存器被复位。
 *         CRC计算使用SPI_CRCPR 中的多项式。
 *
 *         当数据帧格式被设置为8位时，仅低8位参与计算，并且按照CRC8的方法进行；当数据帧
 *         格式为 16位时，寄存器中的所有16位都参与计算，并且按照CRC16的标准
 *
 *         当BSY 标志为‘1’时读该寄存器，将可能读到不正确的数值。
 *
 * \return 依据收到的字节计算的CRC数值
 */
am_static_inline
uint16_t amhw_hk32f103rbt6_spi_rxcrc_read (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    return p_hw_spi->rxcrcr;
}

/**
 * \brief TxCRC寄存器（只读）
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note ：  当在SPI_CR1 的CRCEN 位写入‘1’时，该寄存器被复位。
 *         CRC计算使用SPI_CRCPR 中的多项式。
 *
 *         当数据帧格式被设置为8位时，仅低8位参与计算，并且按照CRC8的方法进行；当数据帧
 *         格式为 16位时，寄存器中的所有16位都参与计算，并且按照CRC16的标准
 *
 *         当BSY 标志为‘1’时读该寄存器，将可能读到不正确的数值。
 *
 * \return 将要发送的字节计算的CRC数值
 */
am_static_inline
uint16_t amhw_hk32f103rbt6_spi_txcrc_read (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    return p_hw_spi->txcrcr;
}

/**
 * \brief FlexLenEn 可变包长禁能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note 不使用2~32比特数据包长度可配功能。
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_flexlength_disable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr3 &= ~(1u << 7);
}

/**
 * \brief FlexLenEn 可变包长使能
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note 使用2~32比特数据包长度可配功能，数据位宽由Len[4:0]定义
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_flexlength_enable (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    p_hw_spi->cr3 |= (1u << 7);
}

/**
 * \brief Len[4:0] 可变包长设置（位宽）
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 * \param[in] flag     : 数据包长设定值（期望包长度-1）
 *
 * \note ：  数据包长为Len[4:0]+1个比特；例如Len[4:0] =5’b00101, 数据包长为6比特。
 *         设定值的[4:0]位有效
 *
 * \return none
 */
am_static_inline
void amhw_hk32f103rbt6_spi_flex_length_set (amhw_hk32f103rbt6_spi_t *p_hw_spi, uint8_t length)
{
    p_hw_spi->crcpr = ( p_hw_spi->crcpr & (~(0x1fu << 0))) | ((0x1fu & length) << 0);
}

/**
 * \brief Len[4:0] 可变包长设置数据读出（位宽）
 * \param[in] p_hw_spi : 指向SPI结构体的指针
 *
 * \note ：  数据包长为Len[4:0]+1个比特；例如Len[4:0] =5’b00101, 数据包长为6比特。
 *
 * \return 当前数据包长设定值（实际包长度-1）
 */
am_static_inline
uint8_t amhw_hk32f103rbt6_flex_length_get (amhw_hk32f103rbt6_spi_t *p_hw_spi)
{
    return (p_hw_spi->crcpr) & 0x1fu;
}

/**
 * @} amhw_hk32f103rbt6_if_spi
 */

#ifdef __cplusplus
}
#endif

#endif /* SOC_HK32F103RBT6_DRIVERS_INCLUDE_SPI_HW_AMHW_HK32F103RBT6_SPI_H_ */
