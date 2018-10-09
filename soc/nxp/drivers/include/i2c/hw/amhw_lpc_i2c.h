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
 * \brief I2C interfaces for operating hardware.
 * 
 * - Independent Master, Slave, and Monitor functions.
 * - Bus speeds supported:
 *    1. Standard mode, up to 100k bits/s.
 *    2. Fast-mode, up to 400k bits/s.
 *    3. Fast-mode Plus, up to 1M bits/s
 *    4. High speed mode, 3.4M bits/s as a Slave only.
 * - Supports both Multi-master and Multi-master with Slave functions.
 * - Multiple I2C slave addresses supported in hardware.
 * - One slave address can be selectively qualified with a bit mask or an address 
 *   range in order to respond to multiple I2C bus addresses.
 * - 10-bit addressing supported with software assist.
 * - Supports System Management Bus (SMBus).
 * - Separate DMA requests for Master, Slave, and Monitor functions.
 * - No chip clocks are required in order to receive and compare an address as a 
 *   slave, so this event can wakeup the device from Power-down mode.
 * - Supports the I2C-bus specification up to Fast-mode Plus (FM+, up to 1 MHz) 
 *   in both master and slave modes. High-speed (HS, up to 3.4 MHz) I2C is support 
 *   in slave mode only.
 *
 * \internal
 * \par Modification History
 * - 1.00 14-11-01  jon, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_I2C_H
#define __AMHW_LPC_I2C_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"
#include "am_bitops.h"
/**
 * @addtogroup amhw_lpc_if_i2c
 * @copydoc amhw_lpc_i2c.h
 * @{
 */

/* 使用匿名联合体段结束 */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
    
  /* 默认使能匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif
    
/**
 * \brief I2C register block structure
 */
typedef struct amhw_lpc_i2c {
    __IO uint32_t  cfg;           /**< \brief Configuration for shared functions      */
    __IO uint32_t  stat;          /**< \brief Status register                         */
    __IO uint32_t  intenset;      /**< \brief Interrupt Enable Set and read register  */
    __O  uint32_t  intenclr;      /**< \brief Interrupt Enable Clear register         */
    __IO uint32_t  timeout;       /**< \brief Time-out value register                 */
    __IO uint32_t  clkdiv;        /**< \brief Clock pre-divider                       */
    __I  uint32_t  intstat;       /**< \brief Interrupt Status register               */
    __I  uint32_t  reserved;      /**< \brief Reserved                                */
    __IO uint32_t  mstctl;        /**< \brief Master control register                 */
    __IO uint32_t  msttime;       /**< \brief Master timing configuration             */
    __IO uint32_t  mstdat;        /**< \brief Master Tx and Rx data register.         */
    __I  uint32_t  reserved1[5];  /**< \brief Reserved                                */
    __IO uint32_t  slvctl;        /**< \brief Slave control register.                 */
    __IO uint32_t  slvdat;        /**< \brief Slave Tx and Rx data register.          */
    union {
        __IO uint32_t slvadr[4];
        struct {
            __IO uint32_t  slvadr0;  /**< \brief 从机地址0                */
            __IO uint32_t  slvadr1;  /**< \brief 从机地址1                */
            __IO uint32_t  slvadr2;  /**< \brief 从机地址2                */
            __IO uint32_t  slvadr3;  /**< \brief 从机地址3                */
        };
    };
    __IO uint32_t  slvqual0;      /**< \brief Slave Qualification                     */
    __I  uint32_t  reserved2[9];  /**< \brief Reserved                                */
    __I  uint32_t  monrxdat;      /**< \brief Monitor receiver data register          */
} amhw_lpc_i2c_t;


/**
 * \name I2C 从机地址编号
 * \anchor grp_amhw_lpc82x_i2c_addr_num
 * @{
 */

#define AMHW_LPC_I2C_SLVADR_0   (0)    /**< \brief I2C 从机地址编号0 */
#define AMHW_LPC_I2C_SLVADR_1   (1)    /**< \brief I2C 从机地址编号1 */
#define AMHW_LPC_I2C_SLVADR_2   (2)    /**< \brief I2C 从机地址编号2 */
#define AMHW_LPC_I2C_SLVADR_3   (3)    /**< \brief I2C 从机地址编号3 */

/** @} */


/**
 * \name I2C  STAT register description definition
 * @{
 */

#define AMHW_LPC_I2C_STAT_MSTPEND          (0x1 << 0)  /**< \brief I2C master pending         */
#define AMHW_LPC_I2C_MASTER_STATE_MASK     (0x7 << 1)  /**< \brief I2C master status mask code*/
#define AMHW_LPC_I2C_STAT_MSTIDLE          (0x0 << 1)  /**< \brief I2C master idle            */
#define AMHW_LPC_I2C_STAT_MSTRX            (0x1 << 1)  /**< \brief I2C master receive readly  */
#define AMHW_LPC_I2C_STAT_MSTTX            (0x2 << 1)  /**< \brief I2C master transmit readly */
#define AMHW_LPC_I2C_STAT_MSTNACKADDR      (0x3 << 1)  /**< \brief I2C master address non-ack */
#define AMHW_LPC_I2C_STAT_MSTNACKTX        (0x4 << 1)  /**< \brief I2C master transmit non-ack*/
#define AMHW_LPC_I2C_STAT_MSTARBLOSS       (0x1 << 4)  /**< \brief I2C master arbitrate loss  */
#define AMHW_LPC_I2C_STAT_MSTSSERR         (0x1 << 6)  /**< \brief I2C master start/stop error*/
                                       
#define AMHW_LPC_I2C_STAT_SLVPEND          (0x1 << 8)   /**< \brief I2C slave pending         */
#define AMHW_LPC_I2C_SLAVE_STATE_MASK      (0x3 << 9)   /**< \brief I2C slave status mask code*/
#define AMHW_LPC_I2C_STAT_SLVADDR          (0x0 << 9)   /**< \brief I2C slave address         */
#define AMHW_LPC_I2C_STAT_SLVRX            (0x1 << 9)   /**< \brief I2C slave receive readly  */
#define AMHW_LPC_I2C_STAT_SLVTX            (0x2 << 9)   /**< \brief I2C slave transmit readly */
#define AMHW_LPC_I2C_STAT_SLVNOTSTR        (0x1 << 11)  /**< \brief I2C slave not Stretching  */
#define AMHW_LPC_I2C_STAT_SLVSEL           (0x1 << 14)  /**< \brief I2C slave select    */
#define AMHW_LPC_I2C_STAT_SLVDESEL         (0x1 << 15)  /**< \brief I2C slave deselect  */
#define AMHW_LPC_I2C_STAT_SLVIDX_MASK      AM_SBF(0x3, 12) /** \brief I2C 从机地址匹配状态      */
                                       
#define AMHW_LPC_I2C_STAT_EVTIMEOUT        (0x1 << 24)  /**< \brief I2C event time out(start. stop. clock edges)    */
#define AMHW_LPC_I2C_STAT_SCLTIMEOUT       (0x1 << 25)  /**< \brief I2C SCL timeout     */

/** @} */


/**
 * \name I2C enable/set interrupt register description definition
 * @{
 */

#define AMHW_LPC_I2C_INTENSET_MSTPEND      (0x1 << 0)  /**< \brief I2C master pending interrupt enable        */
#define AMHW_LPC_I2C_INTENSET_MSTARBLOSS   (0x1 << 4)  /**< \brief I2C master arbitrate loss interrupt enable */
#define AMHW_LPC_I2C_INTENSET_MSTSSERR     (0x1 << 6)  /**< \brief I2C master start/stop error interrupt enable */
                                       
#define AMHW_LPC_I2C_INTENSET_SLVPEND      (0x1 << 8)  /**< \brief I2C slave pending interrupt enable         */
#define AMHW_LPC_I2C_INTENSET_SLVNOTSTR    (0x1 << 11) /**< \brief I2C slave not Stretching interrupt enable  */
#define AMHW_LPC_I2C_INTENSET_SLVDESEL     (0x1 << 15) /**< \brief I2C slave deselect interrupt enable        */
                                       
#define AMHW_LPC_I2C_INTENSET_MONRX        (0x1 << 8)  /**< \brief I2C monitor receive ready  interrupt enable */
#define AMHW_LPC_I2C_INTENSET_MONOVEN      (0x1 << 11) /**< \brief I2C monitor Overrun interrupt enable */
#define AMHW_LPC_I2C_INTENSET_MONIDLE      (0x1 << 15) /**< \brief I2C monitor idle interrupt enable            */
                                       
#define AMHW_LPC_I2C_INTENSET_EVTIMEOUT    (0x1 << 24) /**< \brief I2C event time out interrupt enable          */
#define AMHW_LPC_I2C_INTENSET_SCLTIMEOUT   (0x1 << 25) /**< \brief I2C SCL timeout interrupt enable             */

/** @} */


/**
 * \name I2C enable/set interrupt register description definition
 * @{
 */

#define AMHW_LPC_I2C_INTENCLR_MSTPEND      (0x1 << 0)  /**< \brief I2C master pending interrupt disable        */
#define AMHW_LPC_I2C_INTENCLR_MSTARBLOSS   (0x1 << 4)  /**< \brief I2C master arbitrate loss interrupt disable */
#define AMHW_LPC_I2C_INTENCLR_MSTSSERR     (0x1 << 6)  /**< \brief I2C master start/stop error interrupt disable */

#define AMHW_LPC_I2C_INTENCLR_SLVPEND      (0x1 << 8)  /**< \brief I2C slave pending interrupt disable         */
#define AMHW_LPC_I2C_INTENCLR_SLVNOTSTR    (0x1 << 11) /**< \brief I2C slave not Stretching interrupt disable  */
#define AMHW_LPC_I2C_INTENCLR_SLVDESEL     (0x1 << 15) /**< \brief I2C slave deselect interrupt disable        */

#define AMHW_LPC_I2C_INTENCLR_MONRX        (0x1 << 8)  /**< \brief I2C monitor receive ready  interrupt disable */
#define AMHW_LPC_I2C_INTENCLR_MONOVEN      (0x1 << 11) /**< \brief I2C monitor Overrun interrupt disable */
#define AMHW_LPC_I2C_INTENCLR_MONIDLE      (0x1 << 15) /**< \brief I2C monitor idle interrupt disable            */

#define AMHW_LPC_I2C_INTENCLR_EVTIMEOUT    (0x1 << 24) /**< \brief I2C event time out interrupt disable          */
#define AMHW_LPC_I2C_INTENCLR_SCLTIMEOUT   (0x1 << 25) /**< \brief I2C SCL timeout interrupt disable             */

/** @} */


/**
 * \name I2C master CTRL register description definition 
 * @{
 */
 
#define AMHW_LPC_I2C_CTL_MSTCONTINUE       (1 << 0)  /**< \brief I2C master continue(ACK)     */ 
#define AMHW_LPC_I2C_CTL_MSTSTART          (1 << 1)  /**< \brief I2C master start control bit */
#define AMHW_LPC_I2C_CTL_MSTSTOP           (1 << 2)  /**< \brief I2C master stop control bit  */
#define AMHW_LPC_I2C_CTL_MSTDMA            (1 << 3)  /**< \brief I2C master DMA control bit   */

/** @} */


/**
 * \name I2C slave CTRL register description definition 
 * @{
 */ 

#define AMHW_LPC_I2C_CTL_SLVCONTINUE       (1 << 0)  /**< \brief I2C slave continue control bit */
#define AMHW_LPC_I2C_CTL_SLVNACK           (1 << 1)  /**< \brief I2C slave non ack control bit  */
#define AMHW_LPC_I2C_CTL_SLVDMA            (1 << 3)  /**< \brief I2C slave DMA control bit      */

/** @} */

/**
 * \anchor I2C_mode
 * \brief I2C modes definition 
 */
typedef enum amhw_lpc_i2c_mode {
    AM_I2C_MASTER_MODE    = (1UL << 0),  /**< \brief master mode enable  */
    AM_I2C_SLAVE_MODE     = (1UL << 1),  /**< \brief slave mode enable   */
    AM_I2C_MONITOR_MODE   = (1UL << 2),  /**< \brief monitor mode enable */
    AM_I2C_TIMEOUT_MODE   = (1UL << 3),  /**< \brief time out enable     */
    AM_I2C_MONCLKSTR_MODE = (1UL << 4),  /**< \brief monitor function clock stretching enable */
} amhw_lpc_i2c_mode_t;

/**
 * \brief I2C write to MSTDAT register
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] value    : write to MSTDAT 
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_i2c_mstdat_write (amhw_lpc_i2c_t *p_hw_i2c, uint32_t value)
{
    p_hw_i2c->mstdat = value;  
}

/**
 * \brief I2C read from MSTDAT register
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \return  I2C MSTDAT value
 */
am_static_inline 
uint32_t amhw_lpc_i2c_mstdat_read (amhw_lpc_i2c_t *p_hw_i2c)
{
    return (p_hw_i2c->mstdat);  
}

/**
 * \brief I2C write to SLVDAT register
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] value    : write to SLVDAT 
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_i2c_slvdat_write (amhw_lpc_i2c_t *p_hw_i2c, uint32_t value)
{
    p_hw_i2c->slvdat =  value;  
}

/**
 * \brief I2C read from SLVDAT register
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \return  I2C SLVDAT value
 */
am_static_inline 
uint32_t amhw_lpc_i2c_slvdat_read (amhw_lpc_i2c_t *p_hw_i2c)
{
    return (p_hw_i2c->slvdat);  
}

/**
 * \brief I2C 超时值设置
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] value    : I2C时钟经过((value + 1) * 16)次计数后超时一次
 *
 * \return 无
 * \note 超出设定值通过状态标志和/或中断进行通知。若要在I2C工作时改变该
 *       数值，则需禁用所有超时并向TIMEOUT寄存器写入新值，重新使能超时。
 */
am_static_inline
void amhw_lpc_i2c_timeout_set (amhw_lpc_i2c_t *p_hw_i2c, uint32_t value)
{
    uint32_t time_out;

    if (value <= 0xFFF) {
        time_out = value;
    } else {
        time_out = 0;
    }

    p_hw_i2c->timeout = (AM_SBF(time_out, 4) | 0xF);
}

/**
 * \brief I2C 超时值获取
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \return I2C 超时值
 */
am_static_inline
uint32_t amhw_lpc_i2c_timeout_get (amhw_lpc_i2c_t *p_hw_i2c)
{
    return (uint32_t)AM_BITS_GET(p_hw_i2c->timeout, 4, 12);
}

/**
 * \brief get CLKDIV register value
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \return  I2C CLKDIV value
 */
am_static_inline 
uint32_t amhw_lpc_i2c_clkdiv_get (amhw_lpc_i2c_t *p_hw_i2c)
{
    return (p_hw_i2c->clkdiv);
}

/**
 * \brief write value to CLKDIV register
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] div      : setting CLKDIV register value 
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_i2c_clkdiv_set (amhw_lpc_i2c_t *p_hw_i2c, uint32_t div)
{
    uint32_t clkdiv;
    
    if((div >= 1) && (div <= 65535)) {
        clkdiv = div - 1;
    } else {
        clkdiv = 0;
    }
    
    p_hw_i2c->clkdiv = clkdiv;
}


/**
 * \brief 设定MSTTIME（SCL高电平和SCL低电平时间）
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] scl_h    : SCL高电平时间为 scl_h 个 I2C 预分频时钟周期
 * \param[in] scl_l    : SCL低电平时间为 scl_l 个 I2C 预分频时钟周期
 *
 * \return  无
 * \note MSTTIME 时间设置范围为2 ~ 9 个 I2C 预分频时钟周期
 */
am_static_inline 
void amhw_lpc_i2c_msttime_set (amhw_lpc_i2c_t *p_hw_i2c,
                                  uint32_t           scl_h,
                                  uint32_t           scl_l)
{
    p_hw_i2c->msttime = AM_SBF(scl_h - 2, 4) | (scl_l - 2);
}

/**
 * \brief get I2C Interrupt Status register value
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \return  Interrupt Status register value 
 */
am_static_inline 
uint32_t amhw_lpc_i2c_intstat_get (amhw_lpc_i2c_t *p_hw_i2c)
{
    return (p_hw_i2c->intstat);
}

/**
 * \brief get I2C status register value
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \return  I2C Status register value 
 */
am_static_inline 
uint32_t amhw_lpc_i2c_stat_get (amhw_lpc_i2c_t *p_hw_i2c)
{
    return (p_hw_i2c->stat);
}
/**
 * \brief set I2C status register value
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] value    : setting I2C status register value 
 *                       Or'ed value of AMHW_LPC_I2C_STAT_*(#AMHW_LPC_I2C_STAT_MSTPEND) 
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_i2c_stat_clr (amhw_lpc_i2c_t *p_hw_i2c, uint32_t value)
{
    p_hw_i2c->stat = value;
}
/**
 * \brief I2C interrupt enable
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] flags    : Or'ed value of AMHW_LPC_I2C_INTENSET_*(#AMHW_LPC_I2C_INTENSET_MSTPEND) 
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_i2c_int_enable (amhw_lpc_i2c_t *p_hw_i2c, uint32_t flags)
{
    p_hw_i2c->intenset = flags;
}

/**
 * \brief I2C interrupt disable
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] flags    : Or'ed value of AMHW_LPC_I2C_INTENCLR_*(#AMHW_LPC_I2C_INTENCLR_MSTPEND) 
 *
 * \return  None
 */
am_static_inline 
void amhw_lpc_i2c_int_disable (amhw_lpc_i2c_t *p_hw_i2c, uint32_t flags)
{
    p_hw_i2c->intenclr = flags;
}
/**
 * \brief I2C master start
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \return  None
 */
am_static_inline 
void amhw_lpc_i2c_mst_start (amhw_lpc_i2c_t *p_hw_i2c)
{
    p_hw_i2c->mstctl = AMHW_LPC_I2C_CTL_MSTSTART;  
}

/**
 * \brief I2C master continue
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \return    None
 */
am_static_inline 
void amhw_lpc_i2c_mst_continue (amhw_lpc_i2c_t *p_hw_i2c)
{
    p_hw_i2c->mstctl = AMHW_LPC_I2C_CTL_MSTCONTINUE;  
}
/**
 * \brief  I2C master stop 
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \return  None
 */
am_static_inline 
void amhw_lpc_i2c_mst_stop (amhw_lpc_i2c_t *p_hw_i2c)
{
    p_hw_i2c->mstctl = AMHW_LPC_I2C_CTL_MSTSTOP;
}

/**
 * \brief I2C 主机 DMA 使能
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \return  无
 */
am_static_inline
void amhw_lpc_i2c_mst_dma_enable (amhw_lpc_i2c_t *p_hw_i2c)
{
    p_hw_i2c->mstctl = AMHW_LPC_I2C_CTL_MSTDMA;
}

/**
 * \brief I2C 主机 DMA 禁能
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \return  无
 */
am_static_inline
void amhw_lpc_i2c_mst_dma_disable (amhw_lpc_i2c_t *p_hw_i2c)
{
    p_hw_i2c->mstctl = ~AMHW_LPC_I2C_CTL_MSTDMA;
}

/**
 * \brief I2C slave continue
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \return  None
 */
am_static_inline 
void amhw_lpc_i2c_slv_continue (amhw_lpc_i2c_t *p_hw_i2c)
{
    p_hw_i2c->slvctl = AMHW_LPC_I2C_CTL_SLVCONTINUE;
}

/**
 * \brief I2C 从机DAM使能
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \return  无
 */
am_static_inline
void amhw_lpc_i2c_slv_dma_enable (amhw_lpc_i2c_t *p_hw_i2c)
{
    p_hw_i2c->slvctl = AMHW_LPC_I2C_CTL_SLVDMA;
}

/**
 * \brief I2C 从机DAM禁能
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \return  无
 */
am_static_inline
void amhw_lpc_i2c_slv_dma_disable (amhw_lpc_i2c_t *p_hw_i2c)
{
    p_hw_i2c->slvctl = ~AMHW_LPC_I2C_CTL_SLVDMA;
}

/**
 * \brief  I2C controller enable
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] mode     : I2C mode,refer to \ref I2C_mode "I2C modes definition"
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_i2c_mode_enable (amhw_lpc_i2c_t *p_hw_i2c, uint32_t mode)
{
    p_hw_i2c->cfg |= mode;
}


/**
 * \brief  I2C controller disable
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] mode     : I2C mode,refer to \ref I2C_mode "I2C modes definition"
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_i2c_mode_disable (amhw_lpc_i2c_t *p_hw_i2c, uint32_t mode)
{
    p_hw_i2c->cfg &= ~mode;
}


/**
 * \brief Set the slave 1 address
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] sla_addr : slave address
 *
 * \return    None
 */
am_static_inline
void amhw_lpc_i2c_slvadr1_set (amhw_lpc_i2c_t *p_hw_i2c, uint32_t sla_addr)
{
    p_hw_i2c->slvadr1 = sla_addr;
}

/**
 * \brief Set the slave 2 address
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] sla_addr : slave address
 *
 * \return    None
 */
am_static_inline
void amhw_lpc_i2c_slvadr2_set (amhw_lpc_i2c_t *p_hw_i2c, uint32_t sla_addr)
{
    p_hw_i2c->slvadr2 = sla_addr;
}

/**
 * \brief Set the slave 3 address
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] sla_addr : slave address
 *
 * \return    None
 */
am_static_inline
void amhw_lpc_i2c_slvadr3_set (amhw_lpc_i2c_t *p_hw_i2c, uint32_t sla_addr)
{
    p_hw_i2c->slvadr3 = sla_addr;
}

/**
 * \brief Set the slave 0 address
 *
 * \param[in] p_hw_i2c : Pointer to I2C register block
 * \param[in] sla_addr : slave address
 *
 * \return    None
 */
am_static_inline 
void amhw_lpc_i2c_slvadr0_set (amhw_lpc_i2c_t *p_hw_i2c, uint32_t sla_addr)
{
    p_hw_i2c->slvadr0 = sla_addr;
}


/**
 * \brief I2C 从机地址设置
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] addr_num : 从机地址编号，AMHW_LPC_I2C_SLVADR_*宏
 * \param[in] sla_addr : 7位从机地址值
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_i2c_slv_addr_set (amhw_lpc_i2c_t *p_hw_i2c,
                                uint32_t           addr_num,
                                uint32_t           sla_addr)
{
    p_hw_i2c->slvadr[addr_num & 0x3] = AM_SBF(sla_addr, 1);
}

/**
 * \brief 设置 I2C 从机地址验证寄存器0的值
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] flags    : 需要设置的值
 *
 * \return 无
 */
am_static_inline
void amhw_lpc_i2c_slvqual_set (amhw_lpc_i2c_t *p_hw_i2c, uint32_t flags)
{
    p_hw_i2c->slvqual0 = flags;
}

/**
 * \brief 获取 I2C 从机地址验证寄存器0的值
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \return 从机地址验证寄存器0的值
 */
am_static_inline
uint32_t amhw_lpc_i2c_slvqual_get (amhw_lpc_i2c_t *p_hw_i2c)
{
    return (p_hw_i2c->slvqual0);
}

/* 使用匿名联合体段结束 */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
 
  /* 允许匿名联合体使能 */
#elif defined(__GNUC__)

  /* 默认使用匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

/** 
 * @} amhw_lpc_if_i2c 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC_I2C_H */


/* end of file */
