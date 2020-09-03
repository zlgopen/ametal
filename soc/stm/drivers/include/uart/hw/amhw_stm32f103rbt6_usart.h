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
 * \brief USART操作接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-07-05  ari, first implementation
 * \endinternal
 */

#ifndef __AMHW_STM32F103RBT6_USART_H
#define __AMHW_STM32F103RBT6_USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup amhw_stm32f103rbt6_if_usart
 * \copydoc amhw_stm32f103rbt6_usart.h
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
 * \brief usart 寄存器块结构体
 */
typedef struct amhw_stm32f103rbt6_usart {
    __IO uint32_t sr;        /**< \brief USART 状态寄存器 */
    __IO uint32_t dr;        /**< \brief USART 数据寄存器 */
    __IO uint32_t brr;       /**< \brief USART 波特比率寄存器 */
    __IO uint32_t cr1;       /**< \brief USART 控制寄存器1 */
    __IO uint32_t cr2;       /**< \brief USART 控制寄存器2 */
    __IO uint32_t cr3;       /**< \brief USART 控制寄存器3 */
    __IO uint32_t gtpr;      /**< \brief USART 保护时间和预分频寄存器 */
} amhw_stm32f103rbt6_usart_t;


/**
 * \brief 获取状态寄存器某一标志位是否有效
 *
 * \param[in] none
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_all_clr (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->sr   = 0x00c0;
    p_hw_usart->dr   = 0x0000;
    p_hw_usart->brr  = 0x0000;
    p_hw_usart->cr1  = 0x0000;
    p_hw_usart->cr2  = 0x0000;
    p_hw_usart->cr3  = 0x0000;
    p_hw_usart->gtpr = 0x0000;
}


/**
 * \name  状态标志
 * \anchor amhw_stm32f103rbt6_usart_
 * @{
 */
/** \brief CTS nCTS状态线变化标志 */
#define AMHW_STM32F103RBT6_USART_CTS_FLAG               AM_BIT(9)

/** \brief LIN 断开检测标志 */
#define AMHW_STM32F103RBT6_USART_LIN_BREAK_FLAG         AM_BIT(8)

/** \brief TXE 发送数据寄存器空标志 */
#define AMHW_STM32F103RBT6_USART_TX_EMPTY_FLAG          AM_BIT(7)

/** \brief TC 发送完成标志 */
#define AMHW_STM32F103RBT6_USART_TX_COMPLETE_FLAG       AM_BIT(6)

/** \brief RXNE 读数据寄存器非空标志 */
#define AMHW_STM32F103RBT6_USART_RX_NOT_EMPTY_FLAG      AM_BIT(5)

/** \brief IDLE 检测到总线空闲标志 */
#define AMHW_STM32F103RBT6_USART_IDLE_FREE_FLAG         AM_BIT(4)

/** \brief ORE 过载错误标志 */
#define AMHW_STM32F103RBT6_USART_OVERRUN_ERROR_FLAG     AM_BIT(3)

/** \brief NE 噪声错误标志 */
#define AMHW_STM32F103RBT6_USART_NOISE_ERROR_FLAG       AM_BIT(2)

/** \brief FE 帧错误标志 */
#define AMHW_STM32F103RBT6_USART_FRAMING_ERROR_FLAG     AM_BIT(1)

/** \brief PE 检验错误标志 */
#define AMHW_STM32F103RBT6_USART_PARITY_ERROR_FLAG      AM_BIT(0)
/** @} */

/**
 * \brief 获取状态寄存器某一标志位是否有效
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag      : 待查看标志位宏定义
 *
 * \retval TRUE or FALSE
 */
am_static_inline
am_bool_t amhw_stm32f103rbt6_usart_status_flag_check (amhw_stm32f103rbt6_usart_t *p_hw_usart,
                                               uint32_t             flag)
{
    return ((0x3ffu & p_hw_usart->sr) & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除状态寄存器标志
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag       : 待清除标志位宏定义
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_status_flag_clr (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                        uint32_t                flag)
{
    p_hw_usart->sr = p_hw_usart->sr & (~(flag));
}

/**
 * \brief 载入发送数据
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] data_w     : 待载入数据
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_data_write (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                   uint32_t                data_w)
{
    p_hw_usart->dr = (uint32_t)(0x1ffu & data_w);
}
/**
 * \brief 读取接收数据
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return the data of RDR register
 */
am_static_inline
uint8_t amhw_stm32f103rbt6_usart_data_read (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    return (uint8_t)(0x1ffu & p_hw_usart->dr);
}


/**
 * \brief 设置波特率分频系数整数部分
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] data_w     : 待载入数据
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_br_div_mantissa_set (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                            uint32_t                data_w)
{
    AM_BITS_SET(p_hw_usart->brr, 4, 12, data_w);
}

/**
 * \brief 设置波特率分频系数小数部分
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] data_w     : 待载入数据
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_br_div_fraction_set (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                            uint32_t                data_w)
{
    AM_BITS_SET(p_hw_usart->brr, 0, 4, data_w);
}

/**
 * \brief 发送一个断开帧
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_send_break_set (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr1 |= (1u << 0);
}

/**
 * \brief 读取是否正在发送断开帧
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
am_bool_t amhw_stm32f103rbt6_usart_send_break_check (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    return ((p_hw_usart->cr1) & (1u << 0)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief RWU 接收唤醒使能（静默模式）
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \note: 在把 USART置于静默模式(设置 RWU位)之前，USART要已经先接收
 *        了一个数据字节。否则在静默模式下，不能被空闲总线检测唤醒。
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_receiver_wakeup_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr1 |= (1u << 1);
}

/**
 * \brief RWU 接收唤醒禁能（正常工作模式）
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_receiver_wakeup_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr1 &= ~(1u << 1);
}

/**
 * \brief RE 接收使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_receiver_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr1 |= (1u << 2);
}

/**
 * \brief RE 接收禁能
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_receiver_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr1 &= ~(1u << 2);
}

/**
 * \brief TE 发送使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_transmitter_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr1 |= (1u << 3);
}

/**
 * \brief TE 发送禁能
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_transmitter_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr1 &= ~(1u << 3);
}

/**
 * \name  Interrupt enable
 * \anchor amhw_stm32f103rbt6_uart_
 * @{
 */
/** \brief CTSIE CTS中断使能          CR3 BIT 10*/
#define AMHW_STM32F103RBT6_USART_INT_CTS_ENABLE               (0x1u << 11)

/** \brief EIE 错误中断使能                 CR3 BIT 0*/
#define AMHW_STM32F103RBT6_USART_INT_ERROR_ENABLE             (0x1u << 10)

/** \brief LBDIE 断开符检测中断使能     CR2 BIT 6*/
#define AMHW_STM32F103RBT6_USART_LIN_ENABLE                   (0x1u << 9)

/** \brief PE interrupt enable */
#define AMHW_STM32F103RBT6_USART_INT_PARITY_ENABLE            (0x1u << 8)

/** \brief TXE interrupt enable */
#define AMHW_STM32F103RBT6_USART_INT_TX_EMPTY_ENABLE          (0x1u << 7)

/** \brief TCIE interrupt enable */
#define AMHW_STM32F103RBT6_USART_INT_COMPLETE_ENABLE          (0x1u << 6)

/** \brief RXNE interrupt enable */
#define AMHW_STM32F103RBT6_USART_INT_RX_NOT_EMPTY_ENABLE      (0x1u << 5)

/** \brief IDLE interrupt enable */
#define AMHW_STM32F103RBT6_USART_INT_IDLE_FREE_ENABLE         (0x1u << 4)

/** \brief 所有中断标志位 */
#define AMHW_STM32F103RBT6_USART_INT_ALL_ENABLE_MASK         (0xffu << 4)
/** @} */


/**
 * \brief 中断使能
 *
 * \param[in] p_hw_usart : pointer to UART register block
 * \param[in] flag       : interrupt enable   see the filed of interrupt enable
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_int_enable (amhw_stm32f103rbt6_usart_t *p_hw_uart, uint32_t flag)
{

    /* 开启所有中断的情况 */
    if(flag == (0xffu << 4)) {
        p_hw_uart->cr1 |= (0x1fu << 4);             /* 开启cr1中的IDLE、RXNE、TCIE、TXE、PE*/
        p_hw_uart->cr2 |= (0x01u << 6);             /* 开启cr2中的LBDIE*/
        p_hw_uart->cr3 |= (0x01u << 0);             /* 开启cr3中的EIE*/
        p_hw_uart->cr3 |= (0x01u << 10);            /* 开启cr3中的CTSIE*/
    }else {
        if (flag == AMHW_STM32F103RBT6_USART_LIN_ENABLE){
            p_hw_uart->cr2 |= (0x01u << 6);         /* 开启cr2中的LBDIE*/
        } else if (flag == AMHW_STM32F103RBT6_USART_INT_ERROR_ENABLE) {
            p_hw_uart->cr3 |= (0x01u << 0);         /* 开启cr3中的EIE*/
        } else if (flag == AMHW_STM32F103RBT6_USART_INT_CTS_ENABLE) {
            p_hw_uart->cr3 |= (0x01u << 10);        /* 开启cr3中的CTSIE*/
        } else {
            p_hw_uart->cr1 |= (flag);               /* 开启cr1中的对应中断*/
        }
    }
}

/**
 * \brief 中断禁能
 *
 * \param[in] p_hw_uart : pointer to UART register block
 * \param[in] flag      : interrupt enable   see the filed of interrupt enable
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_int_disable (amhw_stm32f103rbt6_usart_t *p_hw_uart, uint32_t flag)
{
    /* 关闭所有中断的情况 */
    if(flag == (0xffu << 4)) {
        p_hw_uart->cr1 &= ~(0x1fu << 4);             /* 关闭cr1中的IDLE、RXNE、TCIE、TXE、PE*/
        p_hw_uart->cr2 &= ~(0x01u << 6);             /* 关闭cr2中的LBDIE*/
        p_hw_uart->cr3 &= ~(0x01u << 0);             /* 关闭cr3中的EIE*/
        p_hw_uart->cr3 &= ~(0x01u << 10);            /* 关闭cr3中的CTSIE*/
    }else {
        if (flag == AMHW_STM32F103RBT6_USART_LIN_ENABLE){
            p_hw_uart->cr2 &= ~(0x01u << 6);         /* 关闭cr2中的LBDIE*/
        } else if (flag == AMHW_STM32F103RBT6_USART_INT_ERROR_ENABLE) {
            p_hw_uart->cr3 &= ~(0x01u << 0);         /* 关闭cr3中的EIE*/
        } else if (flag == AMHW_STM32F103RBT6_USART_INT_CTS_ENABLE) {
            p_hw_uart->cr3 &= ~(0x01u << 10);        /* 关闭cr3中的CTSIE*/
        } else {
            p_hw_uart->cr1 &= ~(flag);               /* 关闭cr1中的对应中断*/
        }
    }
}
/**
 * \brief USART parity bit select
 * @{
 */
#define AMHW_STM32F103RBT6_USART_PARITY_NO   (0U)  /** <brief 无校验  00 */
#define AMHW_STM32F103RBT6_USART_PARITY_ODD  (3U)  /** <brief 奇校验  11 */
#define AMHW_STM32F103RBT6_USART_PARITY_EVEN (2U)  /** <brief 偶校验  10 */
/** @} */

/**
 * \brief 校验方式设置
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag       : 校验方式宏定义
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_parity_bit_sel (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                       uint32_t                parity)
{
    p_hw_usart->cr1 = (p_hw_usart->cr1 & (~(0x3u << 9))) | (parity << 9);
}

/**
 * \brief 唤醒方法设置
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag       : 0：被空闲总线唤醒
 *                            1：被地址标记唤醒
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_wakeup_method_sel (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                          uint32_t                flag)
{
    p_hw_usart->cr1 = (p_hw_usart->cr1 & (~(0x1u << 11))) | (flag << 11);
}

/**
 * \brief USART data length
 * @{
 */
#define AMHW_STM32F103RBT6_USART_DATA_8BIT   (0u << 4) /** <brief 8-bit data */
#define AMHW_STM32F103RBT6_USART_DATA_9BIT   (1u << 4) /** <brief 9-bit data */
/** @} */
/**
 * \brief 字长设置
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag       : 0：1个起始位，8个数据位，n个停止位
 *                         1：1个起始位，9个数据位，n个停止位
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_word_length_sel (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                        uint32_t                flag)
{
    p_hw_usart->cr1 = (p_hw_usart->cr1 & (~(0x1u << 12))) | ((flag >> 4) << 12);
}

/**
 * \brief USART使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr1 |= (1 << 13);
}

/**
 * \brief USART禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr1 &= ~(1u << 13);
}

/**
 * \brief 本设备节点地址设置
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag       : 节点地址设定值（0~15）
 *
 *  note: 在多处理器通信下的静默模式中使用，使用地址标记来唤醒某个 USART设备
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_addr_set (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                 uint32_t                flag)
{
    p_hw_usart->cr2 = (p_hw_usart->cr2 & (~(0xfu << 0))) | (flag << 0);
}

/**
 * \brief LIN断开符检测长度设置
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag       : 0：10 位的断开符检测
 *                         1：11 位的断开符检测
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_lbd_length_sel (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                       uint32_t                flag)
{
    p_hw_usart->cr2 = (p_hw_usart->cr2 & (~(0x1u << 5))) | (flag << 5);
}

/**
 * \brief LBCL 最后一位时钟脉冲
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag      : 0：最后一位数据的时钟脉冲不从 CK输出
 *                        1：最后一位数据的时钟脉冲会从 CK输出
 *
 * \noet: 在同步模式下，使用该位来控制是否在 CK引脚上输出最后 发送的
 *        那个数据字节(MSB)对应的时钟脉冲.
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_last_bit_clock_pulse (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                             uint32_t                flag)
{
    p_hw_usart->cr2 = (p_hw_usart->cr2 & (~(0x1u << 8))) | (flag << 8);
}

/**
 * \brief CPHA 时钟相位
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag      : 0：在时钟的第一个边沿进行数据捕获
 *                        1：在时钟的第一个边沿进行数据捕获
 *
 * \noet: 在同步模式下，可以用该位选择 SLCK引脚上时钟输出的相位。和
 *        CPOL位一起配合来产生需要的时钟/数据的采样关系。
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_clock_phase (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                    uint32_t                flag)
{
    p_hw_usart->cr2 = (p_hw_usart->cr2 & (~(0x1u << 9))) | (flag << 9);
}

/**
 * \brief CPOL 时钟极性
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag      : 0：总线空闲时 CK引脚上保持低电平
 *                        1：总线空闲时 CK引脚上保持高电平
 *
 * \noet: 在同步模式下，可以用该位选择 SLCK引脚上时钟输出的极性。
 *        和 CPHA位一起配合来产生需要的时钟/数据的采样关系
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_clock_polarity (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                       uint32_t                flag)
{
    p_hw_usart->cr2 = (p_hw_usart->cr2 & (~(0x1u << 10))) | (flag << 10);
}

/**
 * \brief CLKEN 时钟使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_clock_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr2 |= (0x1u << 11);
}

/**
 * \brief CLKEN 时钟禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_clock_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr2 &= ~(0x1u << 11);
}


/**
 * \brief USART stop bit select
 * @{
 */
#define AMHW_STM32F103RBT6_USART_STOP_10_BIT   (0u << 2)  /** <brief 1   stop bit */
#define AMHW_STM32F103RBT6_USART_STOP_05_BIT   (1u << 2)  /** <brief 0.5 stop bit */
#define AMHW_STM32F103RBT6_USART_STOP_20_BIT   (2u << 2)  /** <brief 2   stop bit */
#define AMHW_STM32F103RBT6_USART_STOP_15_BIT   (3u << 2)  /** <brief 1.5 stop bit */
/** @} */

/**
 * \brief STOP 停止位设置
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag       : 停止位宏定义
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_stop_bit_sel (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                     uint32_t                flag)
{
    p_hw_usart->cr2 = (p_hw_usart->cr2 & (~(0x3u << 12))) | ((flag >> 2) << 12);
}

/**
 * \brief LINEN LIN模式使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_lin_mode_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr2 |= (1u << 14);
}

/**
 * \brief LINEN LIN模式禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_lin_mode_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr2 &= ~(1u << 14);
}

/**
 * \brief IREN 红外模式使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \note: 在多缓冲区通信模式下，当有帧错误、过载或者噪声错误时(USART_SR
 *        中的 FE=1，或者 ORE=1，或者 NE=1)产生中断。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_irda_mode_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 |= (1u << 1);
}

/**
 * \brief IREN 红外模式禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_irda_mode_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 &= ~(1u << 1);
}

/**
 * \brief IRLP 红外低功耗使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_irda_lowpower_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 |= (1u << 2);
}

/**
 * \brief IRLP 红外低功耗禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_irda_lowpower_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 &= ~(1u << 2);
}

/**
 * \brief HDSEL 半双工选择
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag       : 0：不选择半双工模式
 *                         1：    选择半双工模式
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_half_duplex_sel (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                        uint32_t                flag)
{
    p_hw_usart->cr3 = (p_hw_usart->cr3 & (~(0x1u << 3))) | (flag << 3);
}

/**
 * \brief NACK 智能卡 NACK使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_smartcard_nack_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 |= (1u << 4);
}

/**
 * \brief NACK 智能卡 NACK禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_smartcard_nack_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 &= ~(1u << 4);
}

/**
 * \brief SCEN 智能卡模式使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_smartcard_mode_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 |= (1u << 5);
}

/**
 * \brief SCEN 智能卡模式禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_smartcard_mode_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 &= ~(1u << 5);
}

/**
 * \brief DMAR DMA使能接收使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_dmar_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 |= (1u << 6);
}

/**
 * \brief DMAR DMA使能接收禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_dmar_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 &= ~(1u << 6);
}

/**
 * \brief DMAT DMA使能发送使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_dmat_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 |= (1u << 7);
}

/**
 * \brief DMAT DMA使能发送禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_dmat_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 &= ~(1u << 7);
}

/**
 * \brief RTSE RTS使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \note:RTS 中断使能，只有接收缓冲区内有空余的空间时才请求下一个数据。
 *           当前数据发送完成后，发送操作就需要暂停下来。如果可以接收
 *           数据了，将 nRTS输出置为有效(拉至低电平)。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_rts_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 |= (1u << 8);
}

/**
 * \brief RTSE RTS禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_rts_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 &= ~(1u << 8);
}

/**
 * \brief CTSE CTS使能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \note: CTS 模式使能，只有 nCTS 输入信号有效(拉成低电平)时才能发送数据。
 *            如果在数据传输的过程中，nCTS 信号变成无效，那么发完这个数据
 *            后，传输就停止下来。如果当nCTS 为无效时，往数据寄存器里写数
 *            据，则要等到 nCTS有效时才会发送这个数据。
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_cts_enable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 |= (1u << 9);
}

/**
 * \brief CTSE CTS禁能
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_cts_disable (amhw_stm32f103rbt6_usart_t *p_hw_usart)
{
    p_hw_usart->cr3 &= ~(1u << 9);
}

/**
 * \brief PSC[7:0] 预分频器值设置
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag       : 预分频器值数据
 *
 * \note: 1.在红外(IrDA)低功耗模式下：
 *          PSC[7:0]=红外低功耗波特率对系统时钟分频以获得低功耗模式下的频率:
 *              00000000：保留 – 不要写入该值；
 *                00000001：对源时钟 1 分频；
 *                00000010：对源时钟 2 分频；
 *                ......
 *          2.在红外(IrDA)的正常模式下：
 *            PSC只能设置为 00000001
 *          3.在智能卡模式下,位[7:5]在智能卡模式下没有意义. 对系统时钟进行分频，给智能卡提供时钟。
 *                    寄存器中给出的值(低 5 位有效)乘以 2 后，作为对源时钟的分频因子
 *              00000：保留 – 不要写入该值；
 *              00001：对源时钟进行 2 分频；
 *              00010：对源时钟进行 4 分频；
 *              00011：对源时钟进行 6 分频；
 *              ......
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_prescaler_value_sel (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                            uint32_t                data)
{
    p_hw_usart->gtpr = (p_hw_usart->gtpr & (~(0xffu << 0))) | (data << 0);
}

/**
 * \brief GT[7:0] 保护时间值设置
 *
 * \param[in] p_hw_usart : 串口寄存器结构体的指针
 * \param[in] flag      : 设定时间值数据
 *
 * \note: 该位域规定了以波特时钟为单位的保护时间。在智能卡模式下，
 *        需要这个功能。当保护时间过去后，才会设置发送完成标志。
 *
 * \retval none
 */
am_static_inline
void amhw_stm32f103rbt6_usart_guard_time_value_sel (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                             uint32_t                data)
{
    p_hw_usart->gtpr = (p_hw_usart->gtpr & (~(0xffu << 8))) | (data << 8);
}


/**
 * \brief baud rate  set
 *
 * \param[in] p_hw_usart : pointer to USART register block
 * \param[in] usart_clk  : the clock of USART module
 * \param[in] baud      :  baud rate
 * \return baud rate
 */
int amhw_stm32f103rbt6_usart_baudrate_set (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                    uint32_t                usart_clk,
                                    uint32_t                baud);

/**
 * \brief USART receive data(polling mode)
 * \param[in] p_hw_usart : pointer to USART register block
 * \param[in] p_rxbuf    : the pointer buffer
 * \param[in] nbytes     : bytes
 *
 * \return bytes
 */
uint32_t amhw_stm32f103rbt6_usart_poll_receive (amhw_stm32f103rbt6_usart_t    *p_hw_usart,
                                         uint8_t                *p_rxbuf,
                                         uint32_t                nbytes);

/**
 * \brief USART transfer data (polling mode)
 * \param[in] p_hw_usart : pointer to USART register block
 * \param[in] p_txbuf    : the pointer to buffer
 * \param[in] nbytes     :  bytes
 *
 * \return bytes
 */
uint32_t amhw_stm32f103rbt6_usart_poll_send (amhw_stm32f103rbt6_usart_t     *p_hw_usart,
                                      const uint8_t           *p_txbuf,
                                      uint32_t                 nbytes);
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
 * @} amhw_stm32f103rbt6_if_usart
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_STM32F103RBT6_USART_H */

/* end of file */
