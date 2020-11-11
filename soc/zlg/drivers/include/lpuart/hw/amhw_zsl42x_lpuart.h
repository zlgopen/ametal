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
 * - 1.00 19-09-26  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZSL42x_LPUART_H
#define __AMHW_ZSL42x_LPUART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup amhw_zsl42x_if_lpuart
 * \copydoc amhw_zsl42x_lpuart.h
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
typedef struct amhw_zsl42x_lpuart {
    __IO uint32_t sbuf;       /**< \brief 数据寄存器 */
    __IO uint32_t scon;       /**< \brief 控制寄存器 */
    __IO uint32_t saddr;      /**< \brief 地址寄存器 */
    __IO uint32_t saden;      /**< \brief 地址掩码寄存器 */
    __IO uint32_t isr;        /**< \brief 中断标志位寄存器 */
    __IO uint32_t icr;        /**< \brief 中断标志位清除寄存器 */
    __IO uint32_t scnt;       /**< \brief 波特率寄存器 */
} amhw_zsl42x_lpuart_t;

/**
 * \brief 设定DATA[8]
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] am_bool_t   : AM_TRUE（1）  或者      AM_FALSE（0）
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_lpuart_data_8th_set (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                    am_bool_t           stat)
{
    p_hw_lpuart->sbuf = (p_hw_lpuart->sbuf & (~(0x1ul << 8))) | (stat << 8);
}

/**
 * \brief 读取DATA[8]
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 *
 * \return  AM_TRUE（1）  或者      AM_FALSE（0）
 */
am_static_inline
am_bool_t amhw_zsl42x_lpuart_data_8th_get (amhw_zsl42x_lpuart_t *p_hw_lpuart)
{
    return ((p_hw_lpuart->sbuf >> 8) & 0x1ul);
}

/**
 * \brief 写数据
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] data_w      : 待写入数据
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_lpuart_data_write (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                  uint8_t             data_w)
{
    p_hw_lpuart->sbuf = (p_hw_lpuart->sbuf & (~0xfful)) | (0xff & data_w);
}

/**
 * \brief 读数据
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 *
 * \return 寄存器数据
 */
am_static_inline
uint8_t amhw_zsl42x_lpuart_data_read (amhw_zsl42x_lpuart_t *p_hw_lpuart)
{
    return (0xffu & p_hw_lpuart->sbuf);
}

/**
 * \brief HDSEL 单线半双工模式   禁能
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 *
 * \return 无
 */

am_static_inline
void amhw_zsl42x_lpuart_single_line_half_disable (amhw_zsl42x_lpuart_t *p_hw_lpuart)
{
    p_hw_lpuart->scon &= ~(0x1ul << 22);
}

/**
 * \brief HDSEL 单线半双工模式   使能
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 *
 * \return 无
 */

am_static_inline
void amhw_zsl42x_lpuart_single_line_half_enable (amhw_zsl42x_lpuart_t *p_hw_lpuart)
{
    p_hw_lpuart->scon |= (0x1ul << 22);
}


#define AMHW_ZSL42x_LPUART_INT_FRAME_ERR   (1ul << 21)   /* 帧错误中断使能位 */
#define AMHW_ZSL42x_LPUART_INT_CTS_TRIGGER (1ul << 20)   /* CTS翻转中断使能位 */
#define AMHW_ZSL42x_LPUART_INT_PARITY_ERR  (1ul << 13)   /* 奇偶校验错误中断使能位 */
#define AMHW_ZSL42x_LPUART_INT_TX_EMPTY    (1ul << 8)    /* TX空中断使能位  */
#define AMHW_ZSL42x_LPUART_INT_TX_COMPLETE (1ul << 1)    /* 发送中断使能位 */
#define AMHW_ZSL42x_LPUART_INT_RX_COMPLETE (1ul << 0)    /* 接收中断使能位 */
#define AMHW_ZSL42x_LPUART_INT_ALL         0xFF          /* 所有中断标志 */

/**
 * \brief UART 相关中断  禁能
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] type        : 中断宏定义  AMHW_ZSL42x_LPUART_INT_FRAME_ERR
 *                                 。
 *                                 。
 *                                 。
 *                                 AMHW_ZSL42x_LPUART_INT_ALL
 *
 * \return 无
 */

am_static_inline
void amhw_zsl42x_lpuart_int_disable (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                   uint32_t            type)
{
    if(type == 0xFF) {
        p_hw_lpuart->scon &= ~(1ul << 21);
        p_hw_lpuart->scon &= ~(1ul << 20);
        p_hw_lpuart->scon &= ~(1ul << 13);
        p_hw_lpuart->scon &= ~(1ul << 8);
        p_hw_lpuart->scon &= ~(1ul << 1);
        p_hw_lpuart->scon &= ~(1ul << 0);
    } else {
        p_hw_lpuart->scon &= ~type;
    }
}

/**
 * \brief UART 相关中断  使能
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] type        : 中断宏定义  AMHW_ZSL42x_LPUART_INT_FRAME_ERR
 *                                 。
 *                                 。
 *                                 。
 *                                 AMHW_ZSL42x_LPUART_INT_ALL
 *
 * \return 无
 */

am_static_inline
void amhw_zsl42x_lpuart_int_enable (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                  uint32_t            type)
{
    if(type == 0xFF) {
        p_hw_lpuart->scon |= (1ul << 21);
        p_hw_lpuart->scon |= (1ul << 20);
        p_hw_lpuart->scon |= (1ul << 13);
        p_hw_lpuart->scon |= (1ul << 8);
        p_hw_lpuart->scon |= (1ul << 1);
        p_hw_lpuart->scon |= (1ul << 0);
    } else {
        p_hw_lpuart->scon |= type;
    }
}

#define AMHW_ZSL42x_LPUART_CTS          (1ul << 19)/* 硬件流CTS使能位 */
#define AMHW_ZSL42x_LPUART_RTS          (1ul << 18)/* 硬件流RTS使能位 */
#define AMHW_ZSL42x_LPUART_DMA_TX       (1ul << 17)/* 发送DMA使能位 */
#define AMHW_ZSL42x_LPUART_DMA_RX       (1ul << 16)/* 接收DMA使能位 */
#define AMHW_ZSL42x_LPUART_MUT_ADR_AUTO (1ul << 5) /* 多机地址自动识别使能位 */
#define AMHW_ZSL42x_LPUART_RX           (1ul << 4) /* 接收使能
                                                    * Mode0:     0：发送; 1：接收
                                                    * Mode1/2/3: 0：发送; 1： 接收/发送
                                                    */
/**
 * \brief UART 相关控制  禁能
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] type      : 中断宏定义  AMHW_ZSL42x_LPUART_CTS
 *                               。
 *                               。
 *                               。
 *                               AMHW_ZSL42x_LPUART_RX
 *
 * \return 无
 */

am_static_inline
void amhw_zsl42x_lpuart_disable (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                               uint32_t            type)
{
    p_hw_lpuart->scon &= ~type;
}

/**
 * \brief UART 相关控制  使能
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] type        : 中断宏定义  AMHW_ZSL42x_LPUART_CTS
 *                                 。
 *                                 。
 *                                 。
 *                                 AMHW_ZSL42x_LPUART_RX
 *
 * \return 无
 */

am_static_inline
void amhw_zsl42x_lpuart_enable (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                              uint32_t            type)
{
    p_hw_lpuart->scon |= type;
}

#define AMHW_ZSL42x_LPUART_STOP_1_0_BIT   (0ul << 14)  /* 1个停止位 */
#define AMHW_ZSL42x_LPUART_STOP_1_5_BIT   (1ul << 14)  /* 1.5个停止位 */
#define AMHW_ZSL42x_LPUART_STOP_2_0_BIT   (2ul << 14)  /* 2个停止位 */

/**
 * \brief 停止位设置
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] stop_bit    : 停止位宏定义   AMHW_ZSL42x_LPUART_STOP_1_0_BIT 或
 *                                   AMHW_ZSL42x_LPUART_STOP_1_5_BIT 或
 *                                   AMHW_ZSL42x_LPUART_STOP_2_0_BIT
 *
 * \return none
 */
am_static_inline
void amhw_zsl42x_lpuart_stop_bit_sel (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                    uint32_t            stop_bit)
{
    p_hw_lpuart->scon = (p_hw_lpuart->scon & (~(0x3ul << 14))) | stop_bit;
}


/**
 *  模式0--同步传输模式，半双工。其波特率固定为 SCLK时钟频率的1/12。 收发数据都由 RXD输入输出，RXD此时为
 *        双向端口。同步移位时钟由 TXD 输出，TXD此时为输出端口。
 *
 *        传输数据位宽为 8-Bit，没有起始位和结束位。

 *  模式1--异步传输模式，全双工。波特率由内部 16-Bit 可编程波特率发生器产生。发送数据通过 TXD发送，接收数据
 *        通过 RXD接收。1-Bit 起始位“0”开始，紧接着 8-Bit数据位（低位在先，高位在后），最后是 1/1.5/
 *        2-Bit结束位“1”。

 *  模式2--异步传输模式，全双工。其波特率固定为 SCLK 时钟频率的1/8。该数据由以下比特位组成：1-Bit起始位“0”
 *        开始，接着是 8-Bit数据位，1-Bit TB8 位和 1/1.5/2-Bit结束位。额外的 TB8位用于在多机通讯环境下使用，
 *        当 TB8=1，表明所接收的是地址帧；当 TB8=0，表明所接收的是数据帧。当不需要多机通讯时，此位也可以作为奇
 *        偶校验位或者数据位来使用。
 *
 *  模式3--异步传输模式，全双工。 波特率由内部 16-Bit可编程波特率发生器产生。发送数据通过 TXD 发送，接收数据
 *        通过 RXD 接收。该数据由以下比特位组成：1-Bit 起始位“0”开始，接着是 8-Bit数据位，1-Bit TB8位
 *        和 1/1.5/2-Bit结束位。额外的 TB8位用于在多机通讯环境下使用，
 *        当 TB8=1，表明所接收的是地址帧；当 TB8=0，表明所接收的是数据帧。当不需要多机通讯时，此位也可以作为奇
 *        偶校验位或者数据位来使用。结束位。
 */
typedef enum {
    AMHW_ZSL42x_LPUART_WORK_MODE_0 = 0,
    AMHW_ZSL42x_LPUART_WORK_MODE_1,
    AMHW_ZSL42x_LPUART_WORK_MODE_2,
    AMHW_ZSL42x_LPUART_WORK_MODE_3
}amhw_zsl42x_lpuart_work_mode_t;
/**
 * \brief 工作模式设定
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] stop_bit    : AMHW_ZSL42x_LPUART_WORK_MODE_0 或
 *                          AMHW_ZSL42x_LPUART_WORK_MODE_1 或
 *                          AMHW_ZSL42x_LPUART_WORK_MODE_2 或
 *                          AMHW_ZSL42x_LPUART_WORK_MODE_3
 *
 * \return none
 */
am_static_inline
void amhw_zsl42x_lpuart_mode_sel (amhw_zsl42x_lpuart_t           *p_hw_lpuart,
                                amhw_zsl42x_lpuart_work_mode_t  mode_type)
{
    p_hw_lpuart->scon = (p_hw_lpuart->scon & (~(0x3ul << 6))) |
                        ((mode_type & 0x3ul) << 6);
}

/**
 * \brief 工作模式获取
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 *
 * \return 当前工作模式0/1/2/3
 */
am_static_inline
amhw_zsl42x_lpuart_work_mode_t amhw_zsl42x_lpuart_mode_get (
        amhw_zsl42x_lpuart_t  *p_hw_lpuart)
{
    return (amhw_zsl42x_lpuart_work_mode_t)((p_hw_lpuart->scon >> 6) & 0x3ul);
}

#define AMHW_ZSL42x_LPUART_SOFT           (0ul << 2)  /* 软件读写SBUF[8] */
#define AMHW_ZSL42x_LPUART_HW_PARITY_EVEN (1ul << 2)  /* 硬件偶校验 */
#define AMHW_ZSL42x_LPUART_HW_PARITY_ODD  (2ul << 2)  /* 硬件奇校验 */
#define AMHW_ZSL42x_LPUART_PARITY_NO      (3ul << 2)  /* 保留 */
/**
 * \brief UART parity bit select
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] parity      : 奇偶校验宏定义       AMHW_ZSL42x_LPUART_SOFT            或
 *                                      AMHW_ZSL42x_LPUART_HW_PARITY_EVEN  或
 *                                      AMHW_ZSL42x_LPUART_HW_PARITY_ODD   或
 *                                      AMHW_ZSL42x_LPUART_PARITY_NO
 * \return none
 */
am_static_inline
void amhw_zsl42x_lpuart_parity_bit_sel (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                      uint32_t            parity)
{
    p_hw_lpuart->scon = (p_hw_lpuart->scon & (~(0x3ul << 2))) | parity;
}

typedef enum {
    AMHW_ZSL42x_LPUART_SCLK_DIV_MODE0_NO  = 0,    /* 模式0，采样分频无效 */
    AMHW_ZSL42x_LPUART_SCLK_DIV_MODE13_16 = 0,    /* 模式1/3，16采样分频 */
    AMHW_ZSL42x_LPUART_SCLK_DIV_MODE13_8  = 1,    /* 模式1/3，8采样分频 */
    AMHW_ZSL42x_LPUART_SCLK_DIV_MODE13_4  = 2,    /* 模式1/3，4采样分频 */
    AMHW_ZSL42x_LPUART_SCLK_DIV_MODE2_32  = 0,    /* 模式2，32采样分频 */
    AMHW_ZSL42x_LPUART_SCLK_DIV_MODE2_16  = 1,    /* 模式2，16采样分频 */
    AMHW_ZSL42x_LPUART_SCLK_DIV_MODE2_8   = 2,    /* 模式2，8采样分频 */
}amhw_zsl42x_lpuart_sclk_div_t;
/**
 * \brief UART 采样分频设置
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] parity      : 分频系数amhw_zsl42x_lpuart_sclk_div_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_zsl42x_lpuart_clk_div_sel (amhw_zsl42x_lpuart_t          *p_hw_lpuart,
                                   amhw_zsl42x_lpuart_sclk_div_t  clk_div)
{
    p_hw_lpuart->scon = (p_hw_lpuart->scon & (~(0x3ul << 9))) | (clk_div << 9);
}

/**
 * \brief UART 采样分频获取
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 *
 * \return 分频系数设定值（具体的分频值需要结合工作模式一起判定，
 *         如amhw_zsl42x_lpuart_sclk_div_t枚举量）
 */
am_static_inline
uint8_t amhw_zsl42x_lpuart_clk_div_get (amhw_zsl42x_lpuart_t *p_hw_lpuart)
{
    return ((p_hw_lpuart->scon >> 9) & 0x3ul);
}

typedef enum {
    AMHW_ZSL42x_LPUART_SCLK_SRC_PCLK = 0,    /* LPUART通信传输时钟为  PCLK */
    AMHW_ZSL42x_LPUART_SCLK_SRC_XTL  = 2,    /* LPUART通信传输时钟为  外部低速时钟 */
    AMHW_ZSL42x_LPUART_SCLK_SRC_RCL  = 3,    /* LPUART通信传输时钟为  内部低速时钟 */
}amhw_zsl42x_lpuart_sclk_src_t;

/**
 * \brief UART 工作时钟选择
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] parity      : amhw_zsl42x_lpuart_sclk_src_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_zsl42x_lpuart_clk_src_sel (amhw_zsl42x_lpuart_t          *p_hw_lpuart,
                                   amhw_zsl42x_lpuart_sclk_src_t  clk_src)
{
    p_hw_lpuart->scon = (p_hw_lpuart->scon & (~(0x3ul << 11))) | (clk_src << 11);
}

/**
 * \brief UART 工作时钟获取
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 *
 * \return amhw_zsl42x_lpuart_sclk_src_t枚举量
 */
am_static_inline
amhw_zsl42x_lpuart_sclk_src_t
amhw_zsl42x_lpuart_clk_src_get (amhw_zsl42x_lpuart_t *p_hw_lpuart)
{
    return (amhw_zsl42x_lpuart_sclk_src_t)((p_hw_lpuart->scon >> 11) & 0x3ul);
}

/**
 * \brief UART 从设备地址设置
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] parity      : 从设备地址（8位）
 *
 * \return none
 */
am_static_inline
void amhw_zsl42x_lpuart_slaver_addr_set (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                       uint8_t             slaver_addr)
{
    p_hw_lpuart->saddr = slaver_addr & 0xFF;
}

/**
 * \brief UART 从设备地址获取
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 *
 * \return 从设备地址（8位）
 */
am_static_inline
uint8_t amhw_zsl42x_lpuart_slaver_addr_get (amhw_zsl42x_lpuart_t *p_hw_lpuart)
{
    return (p_hw_lpuart->saddr & 0xFF);
}

/**
 * \brief UART 从设备地址掩码设置
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] parity      : 从设备地址掩码（0：不关心           1：关心）
 *
 * \return none
 */
am_static_inline
void amhw_zsl42x_lpuart_slaver_addr_mask_set (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                            uint8_t             addr_mask)
{
    p_hw_lpuart->saden = addr_mask & 0xFF;
}

/**
 * \brief UART 从设备地址掩码获取
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 *
 * \return 从设备地址掩码（0：不关心           1：关心）
 */
am_static_inline
uint8_t amhw_zsl42x_lpuart_slaver_addr_mask_get (amhw_zsl42x_lpuart_t *p_hw_lpuart)
{
    return (p_hw_lpuart->saden & 0xFF);
}

#define AMHW_ZSL42x_LPUART_FLAG_CTS          (1ul << 6)   /* CTS高低电平标志位,硬件清零 */
#define AMHW_ZSL42x_LPUART_FLAG_CTS_TRIGGER  (1ul << 5)   /* CTS翻转标志位,软件清0 */
#define AMHW_ZSL42x_LPUART_FLAG_PARITY_ERR   (1ul << 4)   /* 奇偶校验错误标志位,软件清0 */
#define AMHW_ZSL42x_LPUART_FLAG_TX_EMPTY     (1ul << 3)   /* 发送空标志位,硬件清零 */
#define AMHW_ZSL42x_LPUART_FLAG_FRAME_ERR    (1ul << 2)   /* 帧错误标志位,软件清0 */
#define AMHW_ZSL42x_LPUART_FLAG_TX_COMPLETE  (1ul << 1)   /* 发送完成标志位,软件清0 */
#define AMHW_ZSL42x_LPUART_FLAG_RX_COMPLETE  (1ul << 0)   /* 接收完成标志位,软件清0 */

/**
 * \brief UART 标志获取
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] flag        : 标志宏定义   AMHW_ZSL42x_LPUART_FLAG_CTS          或
 *                                  AMHW_ZSL42x_LPUART_FLAG_CTS_TRIGGER  或
 *                                  AMHW_ZSL42x_LPUART_FLAG_PARITY_ERR   或
 *                                  AMHW_ZSL42x_LPUART_FLAG_TX_EMPTY     或
 *                                  AMHW_ZSL42x_LPUART_FLAG_FRAME_ERR    或
 *                                  AMHW_ZSL42x_LPUART_FLAG_TX_COMPLETE  或
 *                                  AMHW_ZSL42x_LPUART_FLAG_RX_COMPLETE  或
 *
 * \return AM_TRUE  : 标志有效
 *         AM_FALSE : 标志无效
 */
am_static_inline
am_bool_t amhw_zsl42x_lpuart_flag_check (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                       uint8_t             flag)
{
    return (p_hw_lpuart->isr & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief UART 标志清除
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 * \param[in] flag        : 标志宏定义   AMHW_ZSL42x_LPUART_FLAG_CTS_TRIGGER  或
 *                                  AMHW_ZSL42x_LPUART_FLAG_PARITY_ERR   或
 *                                  AMHW_ZSL42x_LPUART_FLAG_FRAME_ERR    或
 *                                  AMHW_ZSL42x_LPUART_FLAG_TX_COMPLETE  或
 *                                  AMHW_ZSL42x_LPUART_FLAG_RX_COMPLETE  或
 *
 * \return 无
 */
am_static_inline
void amhw_zsl42x_lpuart_flag_clr (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                uint8_t             flag)
{
    p_hw_lpuart->icr &= ~ flag;
}

/**
 * \brief UART 状态寄存器数值获取
 *
 * \param[in] p_hw_lpuart : 指向UART寄存器结构体的指针
 *
 * \return 状态寄存器数值
 */
am_static_inline
uint32_t amhw_zsl42x_lpuart_flag_get(amhw_zsl42x_lpuart_t *p_hw_lpuart)
{

    return p_hw_lpuart->isr;
}


/**
 * \brief baud rate  set
 *
 * \param[in] p_hw_lpuart : pointer to UART register block
 * \param[in] uart_clk    : the clock of UART module
 * \param[in] baud        :  baud rate
 * \return baud rate
 */
int amhw_zsl42x_lpuart_baudrate_set (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                   uint32_t            uart_clk,
                                   uint32_t            baud);

/**
 * \brief UART receive data(polling mode)
 * \param[in] p_hw_lpuart : pointer to UART register block
 * \param[in] p_rxbuf     : the pointer buffer
 * \param[in] nbytes      : bytes
 *
 * \return bytes
 */
uint32_t amhw_zsl42x_lpuart_poll_receive (amhw_zsl42x_lpuart_t *p_hw_lpuart,
                                        uint8_t            *p_rxbuf,
                                        uint32_t            nbytes);

/**
 * \brief UART transfer data (polling mode)
 * \param[in] p_hw_lpuart : pointer to UART register block
 * \param[in] p_txbuf     : the pointer to buffer
 * \param[in] nbytes      : bytes
 *
 * \return bytes
 */
uint32_t amhw_zsl42x_lpuart_poll_send (amhw_zsl42x_lpuart_t  *p_hw_lpuart,
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
 * @} amhw_zsl42x_if_lpuart
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZSL42x_LPUART_H */

/* end of file */
