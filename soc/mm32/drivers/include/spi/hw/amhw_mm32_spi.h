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
 * - 1.00 17-04-13  ari, first implementation
 * \endinternal
 */

#ifndef __AMHW_MM32_SPI_H
#define __AMHW_MM32_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_mm32_if_spi
 * \copydoc amhw_mm32_spi.h
 * @{
 */

/**
 * \brief SPI structure of register
 */
typedef struct amhw_mm32_spi {
    __IO uint32_t  txreg;        /**< \brief SPI TX data register */
    __I  uint32_t  rxreg;        /**< \brief SPI RX data register */
    __I  uint32_t  cstat;        /**< \brief SPI current status register */
    __I  uint32_t  intstat;      /**< \brief SPI interrupt status register */
    __IO uint32_t  inten;        /**< \brief SPI interrupt enable register */
    __O  uint32_t  intclr;       /**< \brief SPI interrupt clear register */
    __IO uint32_t  gctl;         /**< \brief SPI global control */
    __IO uint32_t  cctl;         /**< \brief SPI common control */
    __IO uint32_t  spbrg;        /**< \brief SPI baud rate register */
    __IO uint32_t  rxdnr;        /**< \brief SPI RX data number */
    __IO uint32_t  nssr;         /**< \brief SPI slave selection register */
    __IO uint32_t  extctl;       /**< \brief SPI data control */
} amhw_mm32_spi_t;

/**
 * \brief 8 bit data write
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] data     : data to write
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_tx_data8_put (amhw_mm32_spi_t *p_hw_spi, uint8_t data)
{
    p_hw_spi->txreg = data;
}

/**
 * \brief 8 bit data read
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 *
 * \return data
 */
am_static_inline
uint8_t amhw_mm32_spi_rx_data8_get (amhw_mm32_spi_t *p_hw_spi)
{
    return (p_hw_spi->rxreg & 0xff);
}

/**
 * \brief 32 bit data write
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] data     : data to write
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_tx_data32_put (amhw_mm32_spi_t *p_hw_spi, uint32_t data)
{
    p_hw_spi->txreg = data;
}

/**
 * \brief 32 bit data read
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \return none
 */
am_static_inline
uint32_t amhw_mm32_spi_rx_data32_get (amhw_mm32_spi_t *p_hw_spi)
{
    return p_hw_spi->rxreg;
}

/**
 * \brief current status
 *  @{
 */
#define AMHW_MM32_SPI_CSTAT_RXVAL_4BYTE  (1U << 3) /**< \brief Receive available 4 byte data */
#define AMHW_MM32_SPI_CSTAT_TX_FULL      (1U << 2) /**< \brief Transmitter FIFO full status bit */
#define AMHW_MM32_SPI_CSTAT_RXVAL        (1U << 1) /**< \brief Receive available byte data message */
#define AMHW_MM32_SPI_CSTAT_TX_EMPTY     (1U << 0) /**< \brief Transmitter empty bit */
/** @}*/

/**
 * \brief current status get
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 *
 * \return the value of CSTAT
 */
am_static_inline
uint32_t amhw_mm32_spi_reg_cstat_get (amhw_mm32_spi_t *p_hw_spi)
{
    return p_hw_spi->cstat;
}

/**
 * \brief interrupt flag
 *  @{
 */
#define AMHW_MM32_SPI_INTSTAT_TXEPT_FALG    (1U << 6) /**< \brief Transmitter empty interrupt flag */
#define AMHW_MM32_SPI_INTSTAT_RX_FULL_FALG  (1U << 5) /**< \brief RX FIFO full interrupt flag */
#define AMHW_MM32_SPI_INTSTAT_RX_MATCH_FALG (1U << 4) /**< \brief Receive data match */
#define AMHW_MM32_SPI_INTSTAT_RX_OERR_FLAG  (1U << 3) /**< \brief Receive overrun error interrupt flag */
#define AMHW_MM32_SPI_INTSTAT_UNDERR_FALG   (1U << 2) /**< \brief underrun interrupt flag */
#define AMHW_MM32_SPI_INTSTAT_RX_FALG       (1U << 1) /**< \brief Receive data available interrupt flag */
#define AMHW_MM32_SPI_INTSTAT_TX_FALG       (1U << 0) /**< \brief Transfer FIFO available interrupt flag */
#define AMHW_MM32_SPI_INTSTAT_INT_ALL       (0X7F)    /**< \brief All */

/** @}*/

/**
 * \brief interrupt  status get
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 *
 * \return the value of INTSTAT
 */
am_static_inline
uint32_t amhw_mm32_spi_reg_intstat_get (amhw_mm32_spi_t *p_hw_spi)
{
    return p_hw_spi->intstat;
}

/**
 * \brief interrupt  enable
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of interrupt flag
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_int_enable (amhw_mm32_spi_t *p_hw_spi, uint32_t flag)
{
    p_hw_spi->inten |= flag;
}

/**
 * \brief interrupt  disable
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of interrupt flag
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_int_disable (amhw_mm32_spi_t *p_hw_spi, uint32_t flag)
{
    p_hw_spi->inten &= ~flag;
}

/**
 * \brief interrupt  flag clear
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of interrupt flag
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_int_flag_clr (amhw_mm32_spi_t *p_hw_spi, uint32_t flag)
{
    p_hw_spi->intclr |= flag;
}

/**
 * \brief valid data selection
 *  @{
 */
#define AMHW_MM32_SPI_VALID_DATA_8BIT    (0U << 11) /**< \brief valid data is low 8 bit */
#define AMHW_MM32_SPI_VALID_DATA_32BIT   (1U << 11) /**< \brief valid data is 32 bit */

/** @}*/
/**
 * \brief valid data selection
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of valid data selection
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_valid_data_sel (amhw_mm32_spi_t *p_hw_spi, uint32_t flag)
{
    p_hw_spi->gctl = (p_hw_spi->gctl & (~(1u << 11))) | flag;
}

/**
 * \brief NSS control selection
 *  @{
 */
#define AMHW_MM32_SPI_NSS_CTRL_BY_REG   (0U << 10) /**< \brief NSS control by register */
#define AMHW_MM32_SPI_NSS_CTRL_BT_HD    (1U << 10) /**< \brief NSS control by hardware */

/** @}*/

/**
 * \brief NSS control selection
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of NSS control selection
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_nss_ctrl_sel (amhw_mm32_spi_t *p_hw_spi, uint32_t flag)
{
    p_hw_spi->gctl = (p_hw_spi->gctl & (~(1u << 10))) | flag;
}

/**
 * \brief DMA enable
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : TRUE or FALSE
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_dma_enable (amhw_mm32_spi_t *p_hw_spi, am_bool_t flag)
{
    p_hw_spi->gctl = (p_hw_spi->gctl & (~(1u << 9))) | (flag << 9);
}

/**
 * \brief DMA TX trigger level
 *  @{
 */
#define AMHW_MM32_SPI_DMA_TRIG_ONE_DATA   (0U << 7) /**< \brief DMA trigger when FIFO has one data idle space */
#define AMHW_MM32_SPI_DMA_TRIG_HALF_DATA  (1U << 7) /**< \brief DMA trigger when FIFO has half data idle space */

/** @}*/

/**
 * \brief DMA TX trigger level selection
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of DMA trigger level
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_dma_txtri_level_sel (amhw_mm32_spi_t *p_hw_spi,
                                          uint32_t           flag)
{
    p_hw_spi->gctl = (p_hw_spi->gctl & (~(3u << 7))) | flag;
}

/** @}*/

/**
 * \brief DMA RX trigger level selection
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of DMA trigger level
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_dma_rxtri_level_sel (amhw_mm32_spi_t *p_hw_spi,
                                          uint32_t           flag)
{
    p_hw_spi->gctl = (p_hw_spi->gctl & (~(3u << 5))) | flag;
}

/**
 * \brief RX enable
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : TRUE or FALSE
 * \return none
 */
am_static_inline
void amhw_mm32_spi_rx_enable (amhw_mm32_spi_t *p_hw_spi, am_bool_t flag)
{
    p_hw_spi->gctl = (p_hw_spi->gctl & (~(1u << 4))) | (flag << 4);
}

/**
 * \brief TX enable
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : TRUE or FALSE
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_tx_enable (amhw_mm32_spi_t *p_hw_spi, am_bool_t flag)
{
    p_hw_spi->gctl = (p_hw_spi->gctl & (~(1u << 3))) | (flag << 3);
}

/**
 * \brief SPI mode selection
 * \ @{
 */
#define AMHW_MM32_SPI_MODE_MASTER   (1U << 2) /**< \brief Master mode */
#define AMHW_MM32_SPI_MODE_SLAVE    (0U << 2) /**< \brief Slave mode */

/** @}*/

/**
 * \brief mode selection
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of SPI mode selection
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_mode_sel (amhw_mm32_spi_t *p_hw_spi, uint32_t flag)
{
    p_hw_spi->gctl = (p_hw_spi->gctl & (~(1u << 2))) | flag;
}

/**
 * \brief Interrupt  enable
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : TRUE or FALSE
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_int_open (amhw_mm32_spi_t *p_hw_spi, am_bool_t flag)
{
    p_hw_spi->gctl = (p_hw_spi->gctl & (~(1u << 1))) | (flag << 1);
}

/**
 * \brief SPI module  enable
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : TRUE or FALSE
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_module_enable (amhw_mm32_spi_t *p_hw_spi, am_bool_t flag)
{
    p_hw_spi->gctl = (p_hw_spi->gctl & (~(1u << 0))) | flag;
}

/**
 * \brief receive  data edge selection
 * \ @{
 */
#define AMHW_MM32_SPI_RX_DATA_LAST_CLOCK   (1U << 4) /**< \brief Receive data at last clock */
#define AMHW_MM32_SPI_RX_DATA_MID_CLOCK    (0U << 4) /**< \brief Receive data at middle clock */

/** @}*/

/**
 * \brief receive data edge selection
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of receive data edge selection
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_rx_data_edge_sel (amhw_mm32_spi_t *p_hw_spi,
                                       uint32_t           flag)
{
    p_hw_spi->cctl = (p_hw_spi->cctl & (~(1u << 4))) | flag;
}

/**
 * \brief transmit data edge selection
 * \ @{
 */
#define AMHW_MM32_SPI_TX_DATA_IMMEDIATELY   (1U << 5) /**< \brief Transfer data immediately */
#define AMHW_MM32_SPI_TX_DATA_ONE_CLOCK     (0U << 5) /**< \brief Transfer data after a valid clock */

/** @}*/

/**
 * \brief transmit data edge selection
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of transmit data edge selection
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_tx_data_edge_sel (amhw_mm32_spi_t *p_hw_spi,
                                       uint32_t           flag)
{
    p_hw_spi->cctl = (p_hw_spi->cctl & (~(1u << 5))) | flag;
}

/**
 * \brief SPI character length bit
 * \ @{
 */
#define AMHW_MM32_SPI_DATA_LEN_7BIT    (0U << 3) /**< \brief character 7 bits */
#define AMHW_MM32_SPI_DATA_LEN_8BIT    (1U << 3) /**< \brief character 8 bits */

/** @}*/

/**
 * \brief receive data edge selection
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of SPI character length bit
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_data_len_sel (amhw_mm32_spi_t *p_hw_spi, uint32_t flag)
{
    p_hw_spi->cctl = (p_hw_spi->cctl & (~(1u << 3))) | flag;
}

/**
 * \brief SPI first enable bit
 * \ @{
 */
#define AMHW_MM32_SPI_DATA_MSB    (0U << 2) /**< \brief character 7 bits */
#define AMHW_MM32_SPI_DATA_LSB    (1U << 2) /**< \brief character 8 bits */

/** @}*/

/**
 * \brief SPI first enable bit selection
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of SPI first enable bit
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_first_bit_sel (amhw_mm32_spi_t *p_hw_spi, uint32_t flag)
{
    p_hw_spi->cctl = (p_hw_spi->cctl & (~(1u << 2))) | flag;
}

/**
 * \brief SPI Clock mode set
 * \ @{
 */
#define AMHW_MM32_SPI_CLK_MODE_0    (1U << 0) /**< \brief Clock mode 0 */
#define AMHW_MM32_SPI_CLK_MODE_1    (0U << 0) /**< \brief Clock mode 1 */
#define AMHW_MM32_SPI_CLK_MODE_2    (3U << 0) /**< \brief Clock mode 0 */
#define AMHW_MM32_SPI_CLK_MODE_3    (2U << 0) /**< \brief Clock mode 1 */

/** @}*/

/**
 * \brief SPI Clock polarity select
 * \param[in] p_hw_spi : The pointer to the block of SPI register
 * \param[in] flag     : see the field of SPI Clock polarity select
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_clk_mode_set (amhw_mm32_spi_t *p_hw_spi, uint32_t flag)
{
    p_hw_spi->cctl = (p_hw_spi->cctl & (~(3u << 0))) | flag;
}

/**
 * \brief SPI  baud rate get
 * \param[in] p_hw_spi  : The pointer to the block of SPI register
 *
 * \return baud rate
 */
am_static_inline
uint16_t amhw_mm32_spi_baud_rate_get (amhw_mm32_spi_t *p_hw_spi)
{
   return (p_hw_spi->spbrg & 0xffffu);
}

/**
 * \brief SPI  baud rate set
 * \param[in] p_hw_spi  : The pointer to the block of SPI register
 * \param[in] baud_rate :  0~65535
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_baud_rate_set (amhw_mm32_spi_t *p_hw_spi,
                                    uint32_t           baud_rate)
{
    p_hw_spi->spbrg = baud_rate & 0xffffu;
}

/**
 * \brief The register is used to hold a count of to be received bytes in next receive process
 * \param[in] p_hw_spi  : The pointer to the block of SPI register
 * \param[in] recv_byte :  0~65535
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_recv_byte_set (amhw_mm32_spi_t *p_hw_spi,
                                    uint32_t           recv_byte)
{
    p_hw_spi->rxdnr = recv_byte & 0xffffu;
}

/**
 * \brief Chip select output signal in Master mode
 * \param[in] p_hw_spi  : The pointer to the block of SPI register
 * \param[in] flag      :  TRUE or FALSE
 *
 * \return none
 */
am_static_inline
void amhw_mm32_spi_cs_sel (amhw_mm32_spi_t *p_hw_spi, am_bool_t flag)
{
    p_hw_spi->nssr = (~flag) & 0x1;
}

/**
 * \brief control data length
 * \param[in] p_hw_spi  : The pointer to the block of SPI register
 * \param[in] len       :  0~31
 *
 * \note the value 0 indicate  32bit
 * \return none
 */
am_static_inline
void amhw_mm32_spi_extlen_set (amhw_mm32_spi_t *p_hw_spi, uint32_t len)
{
    p_hw_spi->extctl = len & 0x1fu;
}
/**
 * @} amhw_mm32_if_spi
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_MM32_SPI_H */

/* end of file */
