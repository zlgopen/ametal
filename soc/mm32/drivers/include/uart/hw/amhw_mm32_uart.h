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
 * \brief UART操作接口
 *
 * \internal
 * \par Modification History
 * - 1.00 17-01-03  ari, first implementation
 * \endinternal
 */

#ifndef __AMHW_MM32_UART_H
#define __AMHW_MM32_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup amhw_mm32_if_uart
 * \copydoc amhw_mm32_uart.h
 * @{
 */

/**
 * \brief 使用匿名联合体段开始
 * @{
 */

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

/** @} */

/**
 * \brief uart 寄存器块结构体
 */
typedef struct amhw_mm32_uart {
    __IO uint32_t tdr;       /**< \brief LPUART version ID register */
    __I  uint32_t rdr;       /**< \brief LPUART parameter register */
    __I  uint32_t csr;       /**< \brief LPUART global register */
    __I  uint32_t isr;       /**< \brief LPUART pin Configuration register */
    __IO uint32_t ier;       /**< \brief LPUART baud rate register */
    __O  uint32_t icr;       /**< \brief LPUART status register */
    __IO uint32_t gcr;       /**< \brief LPUART control register */
    __IO uint32_t ccr;       /**< \brief LPUART data register */
    __IO uint32_t brr;       /**< \brief LPUART match address register */
    __IO uint32_t fra;       /**< \brief LPUART modem irDA register */
} amhw_mm32_uart_t;

/*******************************************************************************
  defines
*******************************************************************************/

/**
 * \brief UART data length
 * @{
 */
#define AMHW_MM32_UART_DATA_5BIT   (0u << 4) /** <brief 5-bit data */
#define AMHW_MM32_UART_DATA_6BIT   (1u << 4) /** <brief 6-bit data */
#define AMHW_MM32_UART_DATA_7BIT   (2u << 4) /** <brief 7-bit data */
#define AMHW_MM32_UART_DATA_8BIT   (3u << 4) /** <brief 8-bit data */
/** @} */
/**
 * \brief UART stop bit select
 * @{
 */

#define AMHW_MM32_UART_STOP_1BIT   (0u << 2)  /** <brief One stop bit */
#define AMHW_MM32_UART_STOP_2BIT   (1U << 2)  /** <brief two stop bit */
/** @} */

/**
 * \brief UART parity bit select
 * @{
 */
#define AMHW_MM32_UART_PARITY_NO   (0U)  /** <brief No parity */
#define AMHW_MM32_UART_PARITY_ODD  (1U)  /** <brief Odd parity */
#define AMHW_MM32_UART_PARITY_EVEN (3U)  /** <brief Even parity */
/** @} */
/**
 * \brief Write data to Transmit data register
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] data_w    : data to write
 *
 * \return none
 */

am_static_inline
void amhw_mm32_uart_data_write (amhw_mm32_uart_t    *p_hw_uart,
                               uint32_t            data_w)
{
    p_hw_uart->tdr = (uint32_t)(0xff & data_w);
}
/**
 * \brief read data form receive data register
 *
 * \param[in] p_hw_uart : pointer to UART register block
 *
 * \return the data of RDR register
 */
am_static_inline
uint8_t amhw_mm32_uart_data_read (amhw_mm32_uart_t *p_hw_uart)
{
    return (uint8_t)(0xffu & p_hw_uart->rdr);
}

/**
 * \name  Status flag
 * \anchor amhw_mm32_uart_
 * @{
 */

/** \brief Transmit buffer empty flag */
#define AMHW_MM32_UART_TX_EMPTY_FLAG      AM_BIT(3)

/** \brief Transmit buffer full flag */
#define AMHW_MM32_UART_TX_FULL_FLAG       AM_BIT(2)

/** \brief Receive valid data flag */
#define AMHW_MM32_UART_RX_VAL_FLAG        AM_BIT(1)

/** \brief Transmit complete flag */
#define AMHW_MM32_UART_TX_COMPLETE_FALG   AM_BIT(0)
/** @} */

/**
 * \brief get the value of status register
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : status flag  see the filed of  Status flag
 *
 * \retval TRUE or FALSE
 */
am_static_inline
am_bool_t amhw_mm32_uart_status_flag_check (amhw_mm32_uart_t    *p_hw_uart,
                                           uint32_t            flag)
{
    return ((0xfu & p_hw_uart->csr) & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \name  Interrupt flag
 * \anchor amhw_mm32_uart_
 * @{
 */

/** \brief Receive  break frame interrupt flag */
#define AMHW_MM32_UART_INT_RXBRK_FLAG      AM_BIT(6)

/** \brief Frame error interrupt flag */
#define AMHW_MM32_UART_INT_RXFERR_FLAG     AM_BIT(5)

/** \brief Parity error interrupt flag */
#define AMHW_MM32_UART_INT_RXPERR_FLAG     AM_BIT(4)

/** \brief Receive overflow error interrupt flag */
#define AMHW_MM32_UART_INT_RXOERR_FLAG     AM_BIT(3)

/** \brief Receive valid interrupt flag */
#define AMHW_MM32_UART_INT_RX_VAL_FLAG     AM_BIT(1)

/** \brief Transmit buffer empty interrupt flag */
#define AMHW_MM32_UART_INT_TX_EMPTY_FLAG   AM_BIT(0)
/** @} */

/**
 * \brief get the value of interrupt status register
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : interrupt flag   see the filed of interrupt flag
 * \return TRUE or FALSE
 */
am_static_inline
am_bool_t amhw_mm32_uart_int_flag_check (amhw_mm32_uart_t    *p_hw_uart,
                                        uint32_t            flag)
{
    return ((0x7bu & p_hw_uart->isr) & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief get the value of interrupt status register
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \return The value of ISR register
 */
am_static_inline
uint32_t amhw_mm32_uart_int_flag_get (amhw_mm32_uart_t *p_hw_uart)
{
    return p_hw_uart->isr;
}

/**
 * \name  Interrupt flag clear
 * \anchor amhw_mm32_uart_
 * @{
 */

/** \brief Receive  break frame interrupt flag */
#define AMHW_MM32_UART_INT_RXBRK_FLAG_CLR       AM_BIT(6)

/** \brief Frame error interrupt flag */
#define AMHW_MM32_UART_INT_RXFERR_FLAG_CLR      AM_BIT(5)

/** \brief Parity error interrupt flag */
#define AMHW_MM32_UART_INT_RXPERR_FLAG_CLR      AM_BIT(4)

/** \brief Receive overflow error interrupt flag */
#define AMHW_MM32_UART_INT_RXOERR_FLAG_CLR      AM_BIT(3)

/** \brief Receive timeout interrupt flag */
#define AMHW_MM32_UART_INT_TIME_OUT_FLAG_CLR AM_BIT(2)

/** \brief Receive valid interrupt flag */
#define AMHW_MM32_UART_INT_RX_VAL_FLAG_CLR      AM_BIT(1)

/** \brief Transmit buffer empty interrupt flag */
#define AMHW_MM32_UART_INT_TX_EMPTY_FLAG_CLR    AM_BIT(0)
/** @} */

/**
 * \brief Clear interrupt flag
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : interrupt flag   see the filed of interrupt flag clear
 * \return none
 */
am_static_inline
void amhw_mm32_uart_int_flag_clr (amhw_mm32_uart_t    *p_hw_uart,
                                 uint32_t            flag)
{
    p_hw_uart->icr = 0x7fu & flag;
}
/**
 * \name  Interrupt enable
 * \anchor amhw_mm32_uart_
 * @{
 */

/** \brief Receive  break frame interrupt enable */
#define AMHW_MM32_UART_INT_RXBRK_ENABLE       AM_BIT(6)

/** \brief Frame error interrupt enable */
#define AMHW_MM32_UART_INT_RXFERR_ENABLE      AM_BIT(5)

/** \brief Parity error interrupt enable */
#define AMHW_MM32_UART_INT_RXPERR_ENABLE      AM_BIT(4)

/** \brief Receive overflow error interrupt enable */
#define AMHW_MM32_UART_INT_RXOERR_ENABLE      AM_BIT(3)

/** \brief Receive timeout interrupt enable */
#define AMHW_MM32_UART_INT_TIME_OUT_ENABLE AM_BIT(2)

/** \brief Receive valid interrupt enable */
#define AMHW_MM32_UART_INT_RX_VAL_ENABLE      AM_BIT(1)

/** \brief Transmit buffer empty interrupt enable */
#define AMHW_MM32_UART_INT_TX_EMPTY_ENABLE    AM_BIT(0)

/** \brief All interrupt mask */
#define AMHW_MM32_UART_INT_ALL_ENABLE_MASK    0x7F
/** @} */

/**
 * \brief Enable interrupt
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : interrupt enable   see the filed of interrupt enable
 * \return none
 */
am_static_inline
void amhw_mm32_uart_int_enable (amhw_mm32_uart_t *p_hw_uart, uint32_t flag)
{
    p_hw_uart->ier = (p_hw_uart->ier & (~(flag))) | flag;
}

/**
 * \brief Disable interrupt
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : interrupt enable   see the filed of interrupt enable
 * \return none
 */
am_static_inline
void amhw_mm32_uart_int_disable (amhw_mm32_uart_t *p_hw_uart, uint32_t flag)
{
    p_hw_uart->ier = p_hw_uart->ier & (~(flag));
}

/**
 * \brief Transmit enable
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : TURE or FALSE
 * \return none
 */
am_static_inline
void amhw_mm32_uart_tx_enable (amhw_mm32_uart_t *p_hw_uart, am_bool_t flag)
{
    p_hw_uart->gcr = (p_hw_uart->gcr & (~(1u << 4))) | (flag << 4);
}

/**
 * \brief Receive enable
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : TURE or FALSE
 * \return none
 */
am_static_inline
void amhw_mm32_uart_rx_enable (amhw_mm32_uart_t *p_hw_uart, am_bool_t flag)
{
    p_hw_uart->gcr = (p_hw_uart->gcr & (~(1u << 3))) | (flag << 3);
}

/**
 * \brief Automatic flow control enable
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : TURE or FALSE
 * \return none
 */
am_static_inline
void amhw_mm32_uart_auto_flow_enable (amhw_mm32_uart_t *p_hw_uart,
                                        am_bool_t           flag)
{
    p_hw_uart->gcr = (p_hw_uart->gcr & (~(1u << 2))) | (flag << 2);
}

/**
 * \brief DMA mode enable
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : TURE or FALSE
 * \return none
 */
am_static_inline
void amhw_mm32_uart_dma_mode_enable (amhw_mm32_uart_t    *p_hw_uart,
                                    am_bool_t           flag)
{
    p_hw_uart->gcr = (p_hw_uart->gcr & (~(1u << 1))) | (flag << 1);
}

/**
 * \brief UART modules enable
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \return none
 */
am_static_inline
void amhw_mm32_uart_enable (amhw_mm32_uart_t *p_hw_uart)
{
    p_hw_uart->gcr |= (1u << 0);
}

/**
 * \brief UART modules disable
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \return none
 */
am_static_inline
void amhw_mm32_uart_disable (amhw_mm32_uart_t *p_hw_uart)
{
    p_hw_uart->gcr &= ~(1u << 0);
}

/**
 * \brief UART UARTEN get
 *
 * \param[in] p_hw_uart : pointer to UART register block
 *
 * \retval AM_TRUE  UARTEN is set
 * \retval AM_FALSE UAETEN is clean
 */
am_static_inline
am_bool_t amhw_mm32_uart_uarten_get (amhw_mm32_uart_t *p_hw_uart)
{
    return (p_hw_uart->gcr & (1u << 0)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief UART data data length select
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] data_len  : see the filed of  data length
 *                        such as AMHW_MM32_UART_DATA_*
 * \return none
 */
am_static_inline
void amhw_mm32_uart_data_length (amhw_mm32_uart_t    *p_hw_uart,
                                uint32_t            data_len)
{
    p_hw_uart->ccr = (p_hw_uart->ccr & (~(3u << 4))) | data_len ;
}

/**
 * \brief Transmit break frame
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : TURE or FALSE
 * \return none
 */
am_static_inline
void amhw_mm32_uart_send_break (amhw_mm32_uart_t *p_hw_uart, am_bool_t flag)
{
    p_hw_uart->ccr = (p_hw_uart->gcr & (~(1u << 3))) | (flag << 3);
}

/**
 * \brief UART stop bit select
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] stop_bit  : see the filed of  stop bit select
 *                        such as AMHW_MM32_UART_SIOP_*
 * \return none
 */
am_static_inline
void amhw_mm32_uart_stop_bit_sel (amhw_mm32_uart_t    *p_hw_uart,
                                 uint32_t            stop_bit)
{
    p_hw_uart->ccr = (p_hw_uart->ccr & (~(1u << 2))) | stop_bit;
}

/**
 * \brief UART parity bit select
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] parity    : see the filed of  parity bit select
 *                        such as AMHW_MM32_UART_PARITY_*
 * \return none
 */
am_static_inline
void amhw_mm32_uart_parity_bit_sel (amhw_mm32_uart_t    *p_hw_uart,
                                   uint32_t            parity)
{
    p_hw_uart->ccr = (p_hw_uart->ccr & (~(3u << 0))) | parity;
}

/**
 * \brief baud rate divide factor mantissa set
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] value     : divide factor mantissa
 * \return none
 */
am_static_inline
void amhw_mm32_uart_br_div_mantissa_set (amhw_mm32_uart_t    *p_hw_uart,
                                        uint32_t            value)
{
    p_hw_uart->brr = (0xffff & value);
}

/**
 * \brief baud rate divide factor fraction set
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] value     : divide factor fraction
 * \return none
 */
am_static_inline
void amhw_mm32_uart_br_div_fraction_set (amhw_mm32_uart_t    *p_hw_uart,
                                        uint32_t            value)
{
    p_hw_uart->fra = (0xf & value);
}

/**
 * \brief baud rate  set
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] uart_clk  : the clock of UART module
 * \param[in] baud      :  baud rate
 * \return baud rate
 */
int amhw_mm32_uart_baudrate_set (amhw_mm32_uart_t    *p_hw_uart,
                                uint32_t            uart_clk,
                                uint32_t            baud);

/**
 * \brief UART receive data(polling mode)
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] p_rxbuf   : the pointer buffer
 * \param[in] nbytes    : bytes
 *
 * \return bytes
 */
uint32_t amhw_mm32_uart_poll_receive (amhw_mm32_uart_t    *p_hw_uart,
                                     uint8_t            *p_rxbuf,
                                     uint32_t            nbytes);

/**
 * \brief UART transfer data (polling mode)
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] p_txbuf   : the pointer to buffer
 * \param[in] nbytes    :  bytes
 *
 * \return bytes
 */
uint32_t amhw_mm32_uart_poll_send (amhw_mm32_uart_t     *p_hw_uart,
                                  const uint8_t       *p_txbuf,
                                  uint32_t             nbytes);
/**
 * \brief 使用匿名联合体段结束
 * @{
 */

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

/** @} */

/**
 * @} amhw_mm32_if_uart
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_MM32_UART_H */

/* end of file */
