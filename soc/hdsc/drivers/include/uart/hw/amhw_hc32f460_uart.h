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

#ifndef __AMHW_HC32F460_UART_H
#define __AMHW_HC32F460_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"


/**
 * \addtogroup amhw_hc32f460_if_uart
 * \copydoc amhw_hc32f460_uart.h
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

/**
 * \brief UART 状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t PE                        : 1;  /**< \brief 接收数据校验错误标志*/
    __IO uint32_t FE                        : 1;  /**< \brief 接收帧错误标志*/
    uint32_t RESERVED2                      : 1;  /**< \brief 保留*/
    __IO uint32_t ORE                       : 1;  /**< \brief 接收上溢错误标志*/
    uint32_t RESERVED4                      : 1;  /**< \brief 保留*/
    __IO uint32_t RXNE                      : 1;  /**< \brief 接收数据寄存器不为空*/
    __IO uint32_t TC                        : 1;  /**< \brief 发送完成标志*/
    __IO uint32_t TXE                       : 1;  /**< \brief 发送数据寄存器为空*/
    __IO uint32_t RTOF                      : 1;  /**< \brief UART接收TIMEOUT标志位*/
    uint32_t RESERVED9                      : 7;  /**< \brief 保留*/
    __IO uint32_t MPB                       : 1;  /**< \brief 接收数据的多处理器位*/
    uint32_t RESERVED17                     :15;  /**< \brief 保留*/
} stc_usart_sr_field_t;

/**
 * \brief UART 数据寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t TDR                       : 9;  /**< \brief 发送数据值*/
    __IO uint32_t MPID                      : 1;  /**< \brief 多处理器模式ID位*/
    uint32_t RESERVED10                     : 6;  /**< \brief 保留*/
    __IO uint32_t RDR                       : 9;  /**< \brief 接收数据值*/
    uint32_t RESERVED25                     : 7;  /**< \brief 保留*/
} stc_usart_dr_field_t;

/**
 * \brief UART 波特率寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DIV_FRACTION              : 7;  /**< \brief 小数分频寄存器*/
    uint32_t RESERVED7                      : 1;  /**< \brief 保留*/
    __IO uint32_t DIV_INTEGER               : 8;  /**< \brief 整数分频寄存器*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_usart_brr_field_t;

/**
 * \brief UART 控制寄存器1位域结构体
 */
typedef struct
{
    __IO uint32_t RTOE                      : 1;  /**< \brief UART TIMEOUT 功能使能位*/
    __IO uint32_t RTOIE                     : 1;  /**< \brief UART TIMEOUT 中断使能位*/
    __IO uint32_t RE                        : 1;  /**< \brief 接收使能位*/
    __IO uint32_t TE                        : 1;  /**< \brief 发送使能位*/
    __IO uint32_t SLME                      : 1;  /**< \brief 静默模式使能位*/
    __IO uint32_t RIE                       : 1;  /**< \brief 接收中断请求有效位*/
    __IO uint32_t TCIE                      : 1;  /**< \brief 发送完成中断请求使能位*/
    __IO uint32_t TXEIE                     : 1;  /**< \brief 发送中断请求使能位*/
    uint32_t RESERVED8                      : 1;  /**< \brief 保留*/
    __IO uint32_t PS                        : 1;  /**< \brief 校验位*/
    __IO uint32_t PCE                       : 1;  /**< \brief 校验有效位*/
    uint32_t RESERVED11                     : 1;  /**< \brief 保留*/
    __IO uint32_t M                         : 1;  /**< \brief 字长设定位*/
    uint32_t RESERVED13                     : 2;  /**< \brief 保留*/
    __IO uint32_t OVER8                     : 1;  /**< \brief UART过采样模式*/
    __IO uint32_t CPE                       : 1;  /**< \brief PEF清零位*/
    __IO uint32_t CFE                       : 1;  /**< \brief FE清零位*/
    uint32_t RESERVED18                     : 1;  /**< \brief 保留*/
    __IO uint32_t CORE                      : 1;  /**< \brief ORE清零位*/
    __IO uint32_t CRTOF                     : 1;  /**< \brief RTOF清零位*/
    uint32_t RESERVED21                     : 3;  /**< \brief 保留*/
    __IO uint32_t MS                        : 1;  /**< \brief 模式选择位*/
    uint32_t RESERVED25                     : 3;  /**< \brief 保留*/
    __IO uint32_t ML                        : 1;  /**< \brief MSB/LSB选择位*/
    __IO uint32_t FBME                      : 1;  /**< \brief 小数波特率生成使能信号*/
    __IO uint32_t NFE                       : 1;  /**< \brief Noise Filter有效设定位*/
    __IO uint32_t SBS                       : 1;  /**< \brief UART模式接收数据起始位检测方式设定位*/
} stc_usart_cr1_field_t;

/**
 * \brief UART 控制寄存器2位域结构体
 */
typedef struct
{
    __IO uint32_t MPE                       : 1;  /**< \brief 多处理器功能有效位*/
    uint32_t RESERVED1                      :10;  /**< \brief 保留*/
    __IO uint32_t CLKC                      : 2;  /**< \brief 时钟控制位*/
    __IO uint32_t STOP                      : 1;  /**< \brief 停止位设定位*/
    uint32_t RESERVED14                     :18;  /**< \brief 保留*/
} stc_usart_cr2_field_t;

/**
 * \brief UART 控制寄存器3位域结构体
 */
typedef struct
{
    uint32_t RESERVED0                      : 5;  /**< \brief 保留*/
    __IO uint32_t SCEN                      : 1;  /**< \brief 智能卡模式使能位*/
    uint32_t RESERVED6                      : 3;  /**< \brief 保留*/
    __IO uint32_t CTSE                      : 1;  /**< \brief CTS功能使能位*/
    uint32_t RESERVED10                     :11;  /**< \brief 保留*/
    __IO uint32_t BCN                       : 3;  /**< \brief 基本时钟数*/
    uint32_t RESERVED24                     : 8;  /**< \brief 保留*/
} stc_usart_cr3_field_t;

/**
 * \brief UART 预分频寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t PSC                       : 2;  /**< \brief 预分频器值*/
    uint32_t RESERVED2                      :30;  /**< \brief 保留*/
} stc_usart_pr_field_t;

/** @} */

/**
 * \brief uart 寄存器块结构体
 */
typedef struct amhw_hc32f460_uart {
    union
    {
        __IO uint32_t SR;            /**< \brief 状态寄存器*/
        stc_usart_sr_field_t SR_f;
    };
    union
    {
        __IO uint32_t DR;            /**< \brief 数据寄存器*/
        stc_usart_dr_field_t DR_f;
    };
    union
    {
        __IO uint32_t BRR;           /**< \brief 波特率寄存器*/
        stc_usart_brr_field_t BRR_f;
    };
    union
    {
        __IO uint32_t CR1;           /**< \brief 控制寄存器1*/
        stc_usart_cr1_field_t CR1_f;
    };
    union
    {
        __IO uint32_t CR2;           /**< \brief 控制寄存器2*/
        stc_usart_cr2_field_t CR2_f;
    };
    union
    {
        __IO uint32_t CR3;           /**< \brief 控制寄存器2*/
        stc_usart_cr3_field_t CR3_f;
    };
    union
    {
        __IO uint32_t PR;            /**< \brief 预分频寄存器*/
        stc_usart_pr_field_t PR_f;
    };
} amhw_hc32f460_uart_t;

/*******************************************************************************
  defines
*******************************************************************************/

/**
 * \brief UART data length
 * @{
 */
#define AMHW_HC32F460_UART_DATA_5BIT   (0u << 4) /** <brief 5-bit data */
#define AMHW_HC32F460_UART_DATA_6BIT   (1u << 4) /** <brief 6-bit data */
#define AMHW_HC32F460_UART_DATA_7BIT   (2u << 4) /** <brief 7-bit data */
#define AMHW_HC32F460_UART_DATA_8BIT   (3u << 4) /** <brief 8-bit data */
#define AMHW_HC32F460_UART_DATA_9BIT   (4u << 4) /** <brief 8-bit data */
/** @} */
/**
 * \brief UART stop bit select
 * @{
 */

#define AMHW_HC32F460_UART_STOP_1BIT   (0u)  /** <brief One stop bit */
#define AMHW_HC32F460_UART_STOP_2BIT   (1U)  /** <brief two stop bit */
/** @} */

/**
 * \brief UART parity bit select
 * @{
 */
#define AMHW_HC32F460_UART_PARITY_NO   (0U)  /** <brief No parity */
#define AMHW_HC32F460_UART_PARITY_ODD  (1U)  /** <brief Odd parity */
#define AMHW_HC32F460_UART_PARITY_EVEN (2U)  /** <brief Even parity */
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
void amhw_hc32f460_uart_data_write (amhw_hc32f460_uart_t    *p_hw_uart,
                                    uint32_t                 data_w)
{
    p_hw_uart->DR_f.TDR = (uint32_t)(0xffu & data_w);
}
/**
 * \brief read data form receive data register
 *
 * \param[in] p_hw_uart : pointer to UART register block
 *
 * \return the data of RDR register
 */
am_static_inline
uint8_t amhw_hc32f460_uart_data_read (amhw_hc32f460_uart_t *p_hw_uart)
{
    return (uint8_t)(0xffu & p_hw_uart->DR_f.RDR);
}

/**
 * \name  Status flag
 * \anchor amhw_hc32f460_uart_
 * @{
 */

/** \brief Transmit buffer empty flag */
#define AMHW_HC32F460_UART_TX_EMPTY_FLAG      AM_BIT(7)

/** \brief Receive valid data flag */
#define AMHW_HC32F460_UART_RX_VAL_FLAG        AM_BIT(5)

/** \brief Transmit complete flag */
#define AMHW_HC32F460_UART_TX_COMPLETE_FALG   AM_BIT(6)

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
am_bool_t amhw_hc32f460_uart_status_flag_check (amhw_hc32f460_uart_t *p_hw_uart,
                                                uint32_t              flag)
{
    return ((0xffffu & p_hw_uart->SR) & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \name  Interrupt flag clear
 * \anchor amhw_hc32f460_uart_
 * @{
 */

/** \brief Receive  break frame interrupt flag */
#define AMHW_HC32F460_UART_INT_RXBRK_FLAG_CLR       AM_BIT(6)

/** \brief Frame error interrupt flag */
#define AMHW_HC32F460_UART_INT_RXFERR_FLAG_CLR      AM_BIT(5)

/** \brief Parity error interrupt flag */
#define AMHW_HC32F460_UART_INT_RXPERR_FLAG_CLR      AM_BIT(4)

/** \brief Receive overflow error interrupt flag */
#define AMHW_HC32F460_UART_INT_RXOERR_FLAG_CLR      AM_BIT(3)

/** \brief Receive timeout interrupt flag */
#define AMHW_HC32F460_UART_INT_TIME_OUT_FLAG_CLR    AM_BIT(2)

/** \brief Receive valid interrupt flag */
#define AMHW_HC32F460_UART_INT_RX_VAL_FLAG_CLR      AM_BIT(1)

/** \brief Transmit buffer empty interrupt flag */
#define AMHW_HC32F460_UART_INT_TX_EMPTY_FLAG_CLR    AM_BIT(0)
/** @} */

/**
 * \name  Interrupt enable
 * \anchor amhw_hc32f460_uart_
 * @{
 */
/** \brief Receive timeout interrupt enable */
#define AMHW_HC32F460_UART_INT_TIME_OUT_ENABLE    AM_BIT(1)

/** \brief Receive valid interrupt enable */
#define AMHW_HC32F460_UART_INT_RX_VAL_ENABLE      AM_BIT(5)

/** \brief Transmit complete interrupt enable */
#define AMHW_HC32F460_UART_INT_TX_COMPLETE_ENABLE AM_BIT(6)

/** \brief Transmit enable */
#define AMHW_HC32F460_UART_TX_ENABLE              AM_BIT(3)

/** \brief Transmit buffer empty interrupt enable */
#define AMHW_HC32F460_UART_INT_TX_EMPTY_ENABLE    AM_BIT(7)


/** \brief All interrupt mask */
#define AMHW_HC32F460_UART_INT_ALL_ENABLE_MASK    0x72
/** @} */

/**
 * \brief Enable interrupt
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : interrupt enable   see the filed of interrupt enable
 * \return none
 */
am_static_inline
void amhw_hc32f460_uart_int_enable (amhw_hc32f460_uart_t *p_hw_uart,
                                    uint32_t              flag)
{
    /** \brief 使能发送数据寄存器空中断（必须同时使能发送功能） */
    if (flag & AMHW_HC32F460_UART_INT_TX_EMPTY_ENABLE) {
        p_hw_uart->CR1 = (p_hw_uart->CR1 & (~(flag))) |
                         (flag | AMHW_HC32F460_UART_TX_ENABLE);
    } else {
        p_hw_uart->CR1 = (p_hw_uart->CR1 & (~(flag))) | flag;
    }
}

/**
 * \brief Disable interrupt
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : interrupt enable   see the filed of interrupt enable
 * \return none
 */
am_static_inline
void amhw_hc32f460_uart_int_disable (amhw_hc32f460_uart_t *p_hw_uart,
                                     uint32_t              flag)
{
    p_hw_uart->CR1 = p_hw_uart->CR1 & (~(flag));
}

/**
 * \brief Transmit enable
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : TURE or FALSE
 * \return none
 */
am_static_inline
void amhw_hc32f460_uart_tx_enable (amhw_hc32f460_uart_t *p_hw_uart,
                                   am_bool_t             flag)
{
    p_hw_uart->CR1_f.TE = flag;
}

/**
 * \brief Receive enable
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : TURE or FALSE
 * \return none
 */
am_static_inline
void amhw_hc32f460_uart_rx_enable (amhw_hc32f460_uart_t *p_hw_uart,
                                   am_bool_t             flag)
{
    p_hw_uart->CR1_f.RE = flag;
}

/**
 * \brief Timeout enable
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : TURE or FALSE
 * \return none
 */
am_static_inline
void amhw_hc32f460_uart_timeout_enable (amhw_hc32f460_uart_t *p_hw_uart,
                                        am_bool_t             flag)
{
    p_hw_uart->CR1_f.RTOE = flag;
}

/**
 * \brief UART data data length select
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] data_len  : see the filed of  data length
 *                        such as AMHW_HC32F460_UART_DATA_*
 * \return none
 */
am_static_inline
void amhw_hc32f460_uart_data_length (amhw_hc32f460_uart_t    *p_hw_uart,
                                     uint32_t                 data_len)
{
    p_hw_uart->CR1_f.M = data_len;
}

/**
 * \brief UART stop bit select
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] stop_bit  : see the filed of  stop bit select
 *                        such as AMHW_HC32F460_UART_SIOP_*
 * \return none
 */
am_static_inline
void amhw_hc32f460_uart_stop_bit_sel (amhw_hc32f460_uart_t    *p_hw_uart,
                                     uint32_t                  stop_bit)
{
    p_hw_uart->CR2_f.STOP = stop_bit;
}

/**
 * \brief UART parity bit select
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] parity    : see the filed of  parity bit select
 *                        such as AMHW_HC32F460_UART_PARITY_*
 * \return none
 */
am_static_inline
void amhw_hc32f460_uart_parity_bit_sel (amhw_hc32f460_uart_t    *p_hw_uart,
                                        uint32_t                 parity)
{
    switch(parity)
    {
        case AMHW_HC32F460_UART_PARITY_NO:
            p_hw_uart->CR1_f.PCE = (uint32_t)0ul;
            break;
        case AMHW_HC32F460_UART_PARITY_EVEN:
            p_hw_uart->CR1_f.PS = (uint32_t)0ul;
            p_hw_uart->CR1_f.PCE = (uint32_t)1ul;
            break;
        case AMHW_HC32F460_UART_PARITY_ODD:
            p_hw_uart->CR1_f.PS = (uint32_t)1ul;
            p_hw_uart->CR1_f.PCE = (uint32_t)1ul;
            break;
        default:
            break;
    }
}

/**
 * \brief baud rate  set
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] uart_clk  : the clock of UART module
 * \param[in] baud      :  baud rate
 * \return baud rate / erro
 */
int amhw_hc32f460_uart_baudrate_set (amhw_hc32f460_uart_t    *p_hw_uart,
                                     int                      uart_clk,
                                     uint32_t                 baud);

/**
 * \brief UART receive data(polling mode)
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] p_rxbuf   : the pointer buffer
 * \param[in] nbytes    : bytes
 *
 * \return bytes
 */
uint32_t amhw_hc32f460_uart_poll_receive (amhw_hc32f460_uart_t    *p_hw_uart,
                                          uint8_t                 *p_rxbuf,
                                          uint32_t                 nbytes);

/**
 * \brief UART transfer data (polling mode)
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] p_txbuf   : the pointer to buffer
 * \param[in] nbytes    :  bytes
 *
 * \return bytes
 */
uint32_t amhw_hc32f460_uart_poll_send (amhw_hc32f460_uart_t     *p_hw_uart,
                                       const uint8_t            *p_txbuf,
                                       uint32_t                  nbytes);
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
 * @} amhw_hc32f460_if_uart
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_UART_H */

/* end of file */
