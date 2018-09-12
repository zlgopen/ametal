/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief SPI interfaces for operating hardware.
 * 
 * - Data transmits of 1~16 bits supported directly. Larger frames supported by software. 
 * - Master and slave operation.
 * - Data can be transmitted to a slave without the need to read incoming data. 
 *   This can be useful while setting up an SPI memory.
 * - Control information can optionally be written along with data. This allows 
 *   very versatile operation, including frames of arbitrary length.
 * - Up to four Slave Select input/outputs with selectable polarity and flexible usage.
 * - Supports DMA transfers: SPIn transmit and receive functions can operated 
 *   with the system DMA controller.
 * - FIFO support from the System FIFO
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  jon, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_SPI_H
#define __AMHW_LPC_SPI_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"

/**
 * \addtogroup amhw_lpc_if_spi
 * \copydoc amhw_lpc_spi.h
 * @{
 */
 
    
/**
 * \brief SPI register block structure
 */
typedef struct amhw_lpc_spi {
    __IO uint32_t  cfg;         /**< \brief SPI Configuration register        */
    __IO uint32_t  dly;         /**< \brief SPI Delay register                */
    __IO uint32_t  stat;        /**< \brief SPI Status                        */
    __IO uint32_t  intenset;    /**< \brief SPI Interrupt Enable read and Set */
    __O  uint32_t  intenclr;    /**< \brief SPI Interrupt Enable Clear        */
    __I  uint32_t  rxdat;       /**< \brief SPI Receive Data                  */
    __IO uint32_t  txdatctl;    /**< \brief SPI Transmit Data with Control    */
    __IO uint32_t  txdat;       /**< \brief SPI Transmit Data                 */
    __IO uint32_t  txctl;       /**< \brief SPI Transmit Control              */
    __IO uint32_t  div;         /**< \brief SPI clock Divider                 */
    __I  uint32_t  intstat;     /**< \brief SPI Interrupt Status              */
} amhw_lpc_spi_t;

/**
 * \name SPI DMA operate address
 * @{
 */

/** \brief SPI0 Receive Data physical address */
#define AMHW_LPC_SPI0_RXDAT_ADDR  (&(AMHW_LPC5410X_SPI0->rxdat))

/** \brief SPI0 Transmit Data physical address */
#define AMHW_LPC_SPI0_TXDAT_ADDR  (&(AMHW_LPC5410X_SPI0->txdat))

/** \brief SPI1 Receive Data physical address  */
#define AMHW_LPC_SPI1_RXDAT_ADDR  (&(AMHW_LPC5410X_SPI1->rxdat))

/** \brief SPI1 Transmit Data physical address */
#define AMHW_LPC_SPI1_TXDAT_ADDR  (&(AMHW_LPC5410X_SPI1->txdat))

/** @} SPI DMA address */


/**
 * \name SPI enable/set interrupt register definition
 * @{
 */
 
/** \brief Receiver Ready interrupt enable       */
#define AMHW_LPC_SPI_INTENSET_RXRDY           (1UL << 0)  

/** \brief Transmitter Ready interrupt enable     */ 
#define AMHW_LPC_SPI_INTENSET_TXRDY           (1UL << 1)   

/** \brief Receiver Overrun interrupt enable      */
#define AMHW_LPC_SPI_INTENSET_RXOVERRUN       (1UL << 2)   

/** \brief Transmitter Underrun interrupt enable  */
#define AMHW_LPC_SPI_INTENSET_TXUNDERRUN      (1UL << 3) 

/** \brief Slave Select Assert interrupt enable   */  
#define AMHW_LPC_SPI_INTENSET_SELNASSERT      (1UL << 4) 

/** \brief Slave Select Deassert interrupt enable */  
#define AMHW_LPC_SPI_INTENSET_SELNDEASSERT    (1UL << 5)  

/** \brief Master idle interrupt enable           */ 
#define AMHW_LPC_SPI_INTENSET_MSTIDLE         (1UL << 8)   

/** @} SPI enable/set interrupt */

/**
 * \name SPI clear interrupt register definition
 * @{
 */
 
/** \brief Receiver Ready interrupt clear        */
#define AMHW_LPC_SPI_INTENCLR_RXRDY           (1UL << 0) 

/** \brief Transmitter Ready interrupt clear     */  
#define AMHW_LPC_SPI_INTENCLR_TXRDY           (1UL << 1) 

/** \brief Receiver Overrun interrupt clear      */  
#define AMHW_LPC_SPI_INTENCLR_RXOVERRUN       (1UL << 2)

/** \brief Transmitter Underrun interrupt clear  */   
#define AMHW_LPC_SPI_INTENCLR_TXUNDERRUN      (1UL << 3)   

/** \brief Slave Select Assert interrupt clear   */
#define AMHW_LPC_SPI_INTENCLR_SELNASSERT      (1UL << 4)  

/** \brief Slave Select Deassert interrupt clear */ 
#define AMHW_LPC_SPI_INTENCLR_SELNDEASSERT    (1UL << 5) 

/** \brief Master idle interrupt clear           */  
#define AMHW_LPC_SPI_INTENCLR_MSTIDLE         (1UL << 8)   

/** @} SPI clear interrupt */

/**
 * \name status register definition
 * @{
 */
 
/** \brief Receiver Ready status         */
#define AMHW_LPC_SPI_STAT_RXRDY               (1UL << 0) 
 
/** \brief Transmitter Readystatus       */ 
#define AMHW_LPC_SPI_STAT_TXRDY               (1UL << 1)

/** \brief Receiver Overrun interrupt    */   
#define AMHW_LPC_SPI_STAT_RXOVERRUN           (1UL << 2)   

/** \brief Transmitter Underrun interrup */
#define AMHW_LPC_SPI_STAT_TXUNDERRUN          (1UL << 3)  

/** \brief Slave Select Assert           */ 
#define AMHW_LPC_SPI_STAT_SELNASSERT          (1UL << 4) 

/** \brief Slave Select Deassert         */  
#define AMHW_LPC_SPI_STAT_SELNDEASSERT        (1UL << 5)   
                                          
#define AMHW_LPC_SPI_STAT_CLKSTALL            (1UL << 6)   /**< \brief Stalled status        */
#define AMHW_LPC_SPI_STAT_EOF                 (1UL << 7)   /**< \brief End Transfer          */
#define AMHW_LPC_SPI_STAT_IDLE                (1UL << 8)   /**< \brief Master idle status    */

/** @} SPI status register */


/**
 * \name SPI mode configure definition
 * @{
 */
 
#define AMHW_LPC_SPI_CFG_ENABLE               (1UL << 0)   /**< \brief SPI enable            */        
#define AMHW_LPC_SPI_CFG_MASTER               (1UL << 2)   /**< \brief SPI master enable     */        
#define AMHW_LPC_SPI_CFG_SLAVE                (0UL << 2)   /**< \brief SPI slave enable      */        
#define AMHW_LPC_SPI_CFG_LSBF                 (1UL << 3)   /**< \brief LSB First mode enable */
#define AMHW_LPC_SPI_CFG_LOOPBACK             (1UL << 7)   /**< \brief Loopback mode enable  */   
     
/** @} SPI mode */

/**
 * \name data control register definition
 * @{
 */

#define AMHW_LPC_SPI_TXDATCTL_MASK            (0xF7F0000)  /**< \brief data control mask bit       */        
#define AMHW_LPC_SPI_TXDATCTL_SSELN(s)        ((s) << 16)  /**< \brief slave select control bits   */        
#define AMHW_LPC_SPI_TXDATCTL_EOT             (1UL << 20)  /**< \brief end of transfer control bit */        
#define AMHW_LPC_SPI_TXDATCTL_EOF             (1UL << 21)  /**< \brief end of frame control bit    */      
#define AMHW_LPC_SPI_TXDATCTL_RX_IGNORE       (1UL << 22)  /**< \brief receive ignore bit          */       
#define AMHW_LPC_SPI_TXDATCTL_FSIZE(s)        ((s) << 24)  /**< \brief frame size control bits     */ 

/** @} SPI data control */

/**
 * \brief slave select definition
 */
typedef enum amhw_lpc_slave {
    AMHW_LPC_SPI_SLAVE0 = ((~(1 << 0)) & 0xf),             /**< \brief The active state of the SSEL0 pin */
    AMHW_LPC_SPI_SLAVE1 = ((~(1 << 1)) & 0xf),             /**< \brief The active state of the SSEL1 pin */
    AMHW_LPC_SPI_SLAVE2 = ((~(1 << 2)) & 0xf),             /**< \brief The active state of the SSEL2 pin */
    AMHW_LPC_SPI_SLAVE3 = ((~(1 << 3)) & 0xf)              /**< \brief The active state of the SSEL3 pin */
} amhw_lpc_slave_t;

/**
 * \name SPI operating modes: clock and phase selection
 * @{
 */ 

/** \brief SCK rest state(LOW)  SCK data change edge(falling)  SCK data sample edge(rising) */
#define AMHW_LPC_SPI_MODE_0     ((0 << 4) |(0 << 5))  
 
/** \brief SCK rest state(LOW)  SCK data change edge(rising)  SCK data sample edge(falling) */
#define AMHW_LPC_SPI_MODE_1     ((1 << 4) |(0 << 5))

/** \brief SCK rest state(HIGH) SCK data change edge(rising)  SCK data sample edge(falling) */
#define AMHW_LPC_SPI_MODE_2     ((0 << 4) |(1 << 5)) 
     
/** \brief SCK rest state(HIGH) SCK data change edge(falling) SCK data sample edge(rising) */
#define AMHW_LPC_SPI_MODE_3     ((1 << 4) |(1 << 5))

/**
 * @} SPI operate mode 
 */
 
/**
 * \name SPI operating modes: clock and phase selection
 * @{
 */


#define AMHW_LPC_SPI_CFG_LSB         AM_BIT(3)  /**< \brief SPI lsb first                    */
#define AMHW_LPC_SPI_CFG_CHANGE      AM_BIT(4)  /**< \brief SCK data change when first edge   */
#define AMHW_LPC_SPI_CFG_STAT_HIGH   AM_BIT(5)  /**< \brief SCK data sample when first edge   */
#define AMHW_LPC_SPI_CFG_LOOP        AM_BIT(7)  /**< \brief SPI loop mode               */
#define AMHW_LPC_SPI_CFG_SPOL0_HIGH  AM_BIT(8)  /**< \brief SSEL0 high electrical level */
#define AMHW_LPC_SPI_CFG_SPOL1_HIGH  AM_BIT(9)  /**< \brief SSEL1 high electrical level */
#define AMHW_LPC_SPI_CFG_SPOL2_HIGH  AM_BIT(10) /**< \brief SSEL2 high electrical level */
#define AMHW_LPC_SPI_CFG_SPOL3_HIGH  AM_BIT(11) /**< \brief SSEL3 high electrical level */

/** @} */

 
/**
 * \name delay register definition 
 * @{
 */

#define AMHW_LPC_SPI_PREDELAY(d)     ((d) << 0)   /**< \brief pre-delay value in SPI clocks  */
#define AMHW_LPC_SPI_POSTDELAY(d)    ((d) << 4)   /**< \brief post-delay value in SPI clocks */

/** \brief delay value between frames in SPI clocks    */
#define AMHW_LPC_SPI_FRAMEDELAY(d)   ((d) << 8) 

/** \brief delay value between transfers in SPI clocks */  
#define AMHW_LPC_SPI_INTERDELAY(d)   ((d) << 12) 

/** @} SPI delay register */


/**
 * \brief SPI Delay Configure Structure
 */
typedef struct amhw_lpc_spi_mst_delay {
    
    /** 
     * \brief Controls the amount of time between SSEL assertion and the 
     *        beginning of a data transfer 
     */
    uint8_t predelay;   

    /**
     * \brief Controls the amount of time between the end of a data transfer 
     *        and SSEL de-assertion 
     */
    uint8_t postdelay;

    /**
     * \brief If the EOF flag is set, controls the minimum amount of time between
     *        the current frame and the next frame (or SSEL de-assertion if EOT).
     */
    uint8_t framedelay;

    /**
     * \brief Controls the minimum amount of time that the SSEL is de-asserted 
     *        between transfers.
     */
    uint8_t transferdelay;
} amhw_lpc_spi_mst_delay_t;

/**
 * \brief SPI configure disable
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \return    None
 */
am_static_inline 
void amhw_lpc_spi_disable (amhw_lpc_spi_t *p_hw_spi)
{
    p_hw_spi->cfg &= ~AMHW_LPC_SPI_CFG_ENABLE;
} 

/**
 * \brief SPI configure enable
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \return    None
 */
am_static_inline 
void amhw_lpc_spi_enable (amhw_lpc_spi_t *p_hw_spi)
{
    p_hw_spi->cfg |= AMHW_LPC_SPI_CFG_ENABLE;
} 

/**
 * \brief SPI set parameter of cfg
 *
 * \param[in] p_hw_spi :  Pointer to SPI register block
 * \param[in] flags    :  cfg parameter
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_spi_cfg_set (amhw_lpc_spi_t *p_hw_spi, uint32_t flags)
{
    p_hw_spi->cfg = flags;
}


/**
 * \brief SPI master delay configure
 *
 * \param[in] p_hw_spi    : Pointer to SPI register block
 * \param[in] p_spi_delay : SPI transfer delay configure 
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_spi_master_dly (amhw_lpc_spi_t           *p_hw_spi,
                          amhw_lpc_spi_mst_delay_t *p_spi_delay)
{
    p_hw_spi->dly = (AMHW_LPC_SPI_PREDELAY  (p_spi_delay->predelay)   |
                     AMHW_LPC_SPI_POSTDELAY (p_spi_delay->postdelay)  |
                     AMHW_LPC_SPI_FRAMEDELAY(p_spi_delay->framedelay) |
                     AMHW_LPC_SPI_INTERDELAY(p_spi_delay->transferdelay));
} 
/**
 * \brief PRE_DELAY设置函数
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] delay    : 控制SSEL置未和数据帧起点之间的时间
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_spi_pre_delay (amhw_lpc_spi_t *p_hw_spi, uint8_t delay)
{
    p_hw_spi->dly = (p_hw_spi->dly & ~(0xf)) | (delay & 0xf);
}

/**
 * \brief POST_DELAY设置函数
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] delay    : 控制数据帧末尾和SSEL解除位之间的时间
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_spi_post_delay (amhw_lpc_spi_t *p_hw_spi, uint8_t delay)
{
    p_hw_spi->dly = (p_hw_spi->dly & ~AM_SBF(0xf, 4)) | AM_SBF(delay & 0xf, 4);
}

/**
 * \brief FRAME_DELAY设置函数
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] delay    : 控制相邻数据帧之间的最短时间
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_spi_frame_delay (amhw_lpc_spi_t *p_hw_spi, uint8_t delay)
{
    p_hw_spi->dly = (p_hw_spi->dly & ~AM_SBF(0xf, 8)) | AM_SBF(delay & 0xf, 8);
}

/**
 * \brief TRANSFER_DELAY设置函数
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] delay    : 控制两次传输之间SSEL解除置位的最短时间
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_spi_trans_delay (amhw_lpc_spi_t *p_hw_spi, uint8_t delay)
{
    p_hw_spi->dly = (p_hw_spi->dly & ~AM_SBF(0xf, 12)) | AM_SBF(delay & 0xf, 12);
}



/**
 * \brief SPI enable interrupt
 *
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \param[in] flags    : Or'ed value of AM_SPI_INTENSET_*(#AMHW_LPC_SPI_INTENSET_RXRDY)
 *
 * \return  None
 */

am_static_inline 
void amhw_lpc_spi_int_enable (amhw_lpc_spi_t *p_hw_spi, uint32_t flags)
{
    p_hw_spi->intenset = flags;
}

/**
 * \brief SPI disable interrupt
 *
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \param[in] flags    : Or'ed value of AM_SPI_INTENCLR_*(#AMHW_LPC_SPI_INTENCLR_RXRDY)
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_spi_int_disable (amhw_lpc_spi_t *p_hw_spi, uint32_t flags)
{
    p_hw_spi->intenclr = flags;
}

/**
 * \brief Multiple SPI data control bits set
 *
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \param[in] flags    : Or'ed value of AMHW_LPC_SPI_TXDATCTL_*(#AMHW_LPC_SPI_TXDATCTL_EOT)
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_spi_txctl_set (amhw_lpc_spi_t *p_hw_spi, uint32_t flags)
{
    volatile uint32_t reg;

    reg             = (p_hw_spi->txctl) & AMHW_LPC_SPI_TXDATCTL_MASK;
    p_hw_spi->txctl = reg | flags;
}


/**
 * \brief spi fram bit bpw
 *
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \param[in] flen     : fram bit bpw
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_spi_data_flen_set (amhw_lpc_spi_t *p_hw_spi, uint8_t flen)
{
    p_hw_spi->txctl = (p_hw_spi->txctl & ~AM_SBF(0xf, 24)) |
                       AM_SBF(((flen - 1) & 0xf), 24);
}

/**
 * \brief Multiple SPI data control bits clear
 *
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \param[in] bits     : Or'ed value of AMHW_LPC_SPI_TXDATCTL_*(# AMHW_LPC_SPI_TXDATCTL_EOT)
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_spi_txctl_clear (amhw_lpc_spi_t *p_hw_spi, uint32_t bits)
{
  volatile uint32_t reg;

    reg = (p_hw_spi->txctl) & AMHW_LPC_SPI_TXDATCTL_MASK;
    p_hw_spi->txctl = reg & ~bits;
}

/**
 * \brief Clear status register
 *
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \param[in] flags    : Or'ed value of AMHW_LPC_SPI_STAT_*(#AMHW_LPC_SPI_STAT_RXRDY)
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_spi_stat_clear (amhw_lpc_spi_t *p_hw_spi, uint32_t flags)
{
    p_hw_spi->stat = flags;
}

/**
 * \brief get status register val
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \return SPI  status bit informatio
 */
am_static_inline
uint16_t amhw_lpc_spi_stat_get (amhw_lpc_spi_t *p_hw_spi)
{
    return (p_hw_spi->stat & 0xfff);
}

/**
 * \brief Get SPI interrupt status 
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \return    The value of INTSTAT
 */
am_static_inline 
uint32_t amhw_lpc_spi_intstat_get (amhw_lpc_spi_t *p_hw_spi)
{
    return p_hw_spi->intstat;
}

/**
 * \brief Get SPI receive data (8bits)
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \return    Receive data register value
 */
am_static_inline 
uint8_t amhw_lpc_spi_rxdata_8bit_read (amhw_lpc_spi_t *p_hw_spi)
{
    return (p_hw_spi->rxdat & 0xFF);
}

/**
 * \brief Get SPI receive data (16bits)
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \return    Receive data register value
 */
am_static_inline 
uint16_t amhw_lpc_spi_rxdata_16bit_read (amhw_lpc_spi_t *p_hw_spi)
{
    return (p_hw_spi->rxdat & 0xFFFF);
}

/**
 * \brief 读取SPI接收到的数据（16bit，带RXSSELN和SOT标志位）
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \return 返回接收到的数据（16bit，且带RXSSELN和SOT标志位）
 *
 * \note 0~15 位：为接收到的数据位
 *       16~19位：为接收数据的从机选择(RXSSELN)
 *       第20 位：为传送开始标志(SOT)
 *       其他位 ：为保留位恒为零
 */
am_static_inline
uint32_t amhw_lpc_spi_rxdat_with_flags (amhw_lpc_spi_t *p_hw_spi)
{
    return (p_hw_spi->rxdat & 0x1fffff);
}

/**
 * \brief Write data to transmit data register
 *
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \param[in] data     : The data to transmit
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_spi_txdat_write (amhw_lpc_spi_t *p_hw_spi, uint16_t data)
{
    p_hw_spi->txdat = (uint32_t)data;
}

/**
 * \brief SPI transfer ignore receive
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \return  None
 */
am_static_inline 
void amhw_lpc_spi_ignore_recv (amhw_lpc_spi_t *p_hw_spi)
{
  volatile uint32_t reg;

    reg             = (p_hw_spi->txctl) & AMHW_LPC_SPI_TXDATCTL_MASK;
    p_hw_spi->txctl = (reg | AMHW_LPC_SPI_TXDATCTL_RX_IGNORE);
}

/**
 * \brief SPI end of transfer
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \return  None
 */
am_static_inline 
void amhw_lpc_spi_end_trans (amhw_lpc_spi_t *p_hw_spi)
{
  volatile uint32_t reg;

    reg             = (p_hw_spi->txctl) & AMHW_LPC_SPI_TXDATCTL_MASK;
    p_hw_spi->txctl = reg | AMHW_LPC_SPI_TXDATCTL_EOT;
}

/**
 * \brief SPI end of transfer(ingroe receive)
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \return  None
 */
am_static_inline 
void amhw_lpc_spi_ignore_recv_end_trans (amhw_lpc_spi_t *p_hw_spi)
{
  volatile uint32_t reg;

    reg             = (p_hw_spi->txctl) & AMHW_LPC_SPI_TXDATCTL_MASK;
    p_hw_spi->txctl = (reg | AMHW_LPC_SPI_TXDATCTL_EOT |
                             AMHW_LPC_SPI_TXDATCTL_RX_IGNORE);
}

/**
 * \brief SPI发送数据（带控制指令）
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] data     : 欲发送的数据
 * \param[in] flags    : 发送时的参数（多个参数可“或”运算） AMHW_LPC_SPI_TXDATCTL_*(# AMHW_LPC_SPI_TXDATCTL_EOT)
 * \param[in] flen     : 数据帧长度位数（1~16位）
 *
 * \return 无
 *
 * \note 参数flags 可以通过“或”运算一同传递（共4个参数），如果4个参数中存在某1个
 *       参数没有“或”上，则默认该参数为零。如果所有参数都要设置为0，可直接赋值0
 *       到该参数上。该函数可用于起始帧的发送和结束帧的发送，单纯的数据发送建议使用
 *       amhw_lpc_spi_txdat_write()，该函数不会影响发送的其他参数，仅会改变发送器中
 *       的数据。单纯的数据发送参数的改变，建议使用amhw_lpc_spi_txctl_set()和
 *       amhw_lpc_spi_txctl_clear()使能或禁能，这两个函数只会改变发送过程的参数。
 */
am_static_inline
void amhw_lpc_spi_txdatctl (amhw_lpc_spi_t *p_hw_spi,
                               uint16_t           data,
                               uint32_t           flags,
                               uint8_t            flen)
{
    p_hw_spi->txdatctl = data | flags | AM_SBF(((flen - 1) & 0xf), 24);
}

/**
 * \brief spi clock div set
 *
 * \param[in] p_hw_spi : Pointer to SPI register block
 * \param[in] div      : div val
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_spi_div_set (amhw_lpc_spi_t *p_hw_spi, uint16_t div)
{
    p_hw_spi->div = div - 1;
}


/** 
 * @} amhw_lpc_if_spi
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC_SPI_H */

/*end of file */
