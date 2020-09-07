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
 * \brief I2C操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-19
 * \endinternal
 */

#ifndef __AMHW_HC32_I2C_H
#define __AMHW_HC32_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/*
 * \brief 匿名结构体段的开始
 */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/**
 * \addtogroup amhw_hc32_if_i2c
 * \copydoc amhw_hc32_i2c.h
 * @{
 */

/**
 * \brief I2C寄存器块结构体
 */
typedef struct amhw_hc32_i2c {
    __IO uint32_t i2c_tmrun;    /* i2c波特率计数器使能寄存器 */
    __IO uint32_t i2c_tm;       /* i2c波特率计数器配置寄存器 */
    __IO uint32_t i2c_cr;       /* i2c配置寄存器 */
    __IO uint32_t i2c_data;     /* i2c数据寄存器 */
    __IO uint32_t i2c_addr;     /* i2c地址寄存器 */
    __I  uint32_t i2c_stat;     /* i2c状态寄存器 */
} amhw_hc32_i2c_t;

/**
 * \brief i2c配置标志
 */

/** \brief 快速模式(1Mbps)使能(i2c_tm < 9时必须使能) */
#define AMHW_HC32_I2C_HIGH_SPEED          (1ul)

/** \brief 应答标志使能 */
#define AMHW_HC32_I2C_REPLY_ENABLE        (1ul << 2)

/** \brief 中断标志位 */
#define AMHW_HC32_I2C_INT_FLAG            (1ul << 3)

/** \brief 停止标志使能 */
#define AMHW_HC32_I2C_STOP_ENABLE         (1ul << 4)

/** \brief 开始标志使能 */
#define AMHW_HC32_I2C_START_ENABLE        (1ul << 5)

/**
 * \brief I2C状态标志
 */
typedef enum amhw_hc32_status_flag{

    /* 无指定模式状态 */
    AMHW_HC32_STATUS_FLAG_0   = (0x00),   /**< \brief 出现总线错误或I2C进入未定义的状态 */
    AMHW_HC32_STATUS_FLAG_1   = (0x08),   /**< \brief 已发送起始条件 */
    AMHW_HC32_STATUS_FLAG_2   = (0x10),   /**< \brief 已发送重复起始条件 */

    /* 主发送器状态 */
    AMHW_HC32_STATUS_FLAG_3   = (0x18),   /**< \brief 已发送SLA+W，已接收ACK */
    AMHW_HC32_STATUS_FLAG_4   = (0x20),   /**< \brief 已发送SLA+W，已接收非ACK */
    AMHW_HC32_STATUS_FLAG_5   = (0x28),   /**< \brief 已发送数据，已接收ACK */
    AMHW_HC32_STATUS_FLAG_6   = (0x30),   /**< \brief 已发送数据 */
    AMHW_HC32_STATUS_FLAG_7   = (0x38),   /**< \brief 在SLA+ 读写或写数据字节时丢失仲裁 */

    /* 主接收器状态 */
    AMHW_HC32_STATUS_FLAG_8   = (0x40),   /**< \brief 已发送SLA +R,已接收ACK */
    AMHW_HC32_STATUS_FLAG_9   = (0x48),   /**< \brief 已发送SLA +R,已接收非ACK */
    AMHW_HC32_STATUS_FLAG_10  = (0x50),   /**< \brief 已接收数据字节，ACK已返回 */
    AMHW_HC32_STATUS_FLAG_11  = (0x58),   /**< \brief 已接收数据字节，非ACK已返回 */

    /* 从接收器状态 */
    AMHW_HC32_STATUS_FLAG_12  = (0x60),   /**< \brief 已接收自身的SLA+W；已接收ACK */
    AMHW_HC32_STATUS_FLAG_13  = (0x68),   /**< \brief 主控时在SLA+读写丢失仲裁；已接收自身的SLA+W；已返回ACK； */
    AMHW_HC32_STATUS_FLAG_14  = (0x70),   /**< \brief 已接收通用调用地址（0x00）；已返回ACK */
    AMHW_HC32_STATUS_FLAG_15  = (0x78),   /**< \brief 主控时在SLA+读写中丢失仲裁；已接收通用调用地址；已返回ACK； */
    AMHW_HC32_STATUS_FLAG_16  = (0x80),   /**< \brief 前一次寻址使用自身从地址；已接收数据字节；已返回ACK； */
    AMHW_HC32_STATUS_FLAG_17  = (0x88),   /**< \brief 前一次寻址使用自身从地址；已接收数据字节；已返回非ACK； */
    AMHW_HC32_STATUS_FLAG_18  = (0x90),   /**< \brief 前一次寻址使用通用调用地址；已接收数据；已返回ACK； */
    AMHW_HC32_STATUS_FLAG_19  = (0x98),   /**< \brief 前一次寻址使用通用调用地址；已接收数据；已返回非ACK； */
    AMHW_HC32_STATUS_FLAG_20  = (0xa0),   /**< \brief 当使用从接收/从发送模式中静态寻址时，接收到停止条件或重复起始条件 */

    /* 从发送器状态 */
    AMHW_HC32_STATUS_FLAG_22  = (0xa8),   /**< \brief 已接收自身的SLA+R；已返回ACK */
    AMHW_HC32_STATUS_FLAG_23  = (0xb0),   /**< \brief 当主控时在SLA+ 读写中丢失仲裁；已接收自身SLA+R；已返回ACK； */
    AMHW_HC32_STATUS_FLAG_24  = (0xb8),   /**< \brief 已发送数据；已接收ACK； */
    AMHW_HC32_STATUS_FLAG_25  = (0xc0),   /**< \brief 已发送数据字节；已接收非ACK; */
    AMHW_HC32_STATUS_FLAG_26  = (0xc8),   /**< \brief 装入的数据字节已被发送；已接收ACK； */

    /* 其它状态 */
    AMHW_HC32_STATUS_FLAG_27  = (0xf8)    /**< \brief 无可用的相关状态信息；si=0； */
}amhw_hc32_status_flag_t;

/**
 * \brief 使能波特率计数器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_tm_enable (amhw_hc32_i2c_t *p_hw_i2c)
{
    p_hw_i2c->i2c_tmrun |= 0x1ul;
}

/**
 * \brief 禁能波特率计数器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_tm_disable (amhw_hc32_i2c_t *p_hw_i2c)
{
    p_hw_i2c->i2c_tmrun = 0x0ul;
}

/**
 * \brief 设置波特率寄存器值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  tm        : 计数器配置值
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_tm_set (amhw_hc32_i2c_t  *p_hw_i2c,
                             uint8_t             tm)
{
    p_hw_i2c->i2c_tm = tm;
}

/**
 * \brief 设置控制寄存器值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  flag      : i2c配置标志， see:AMHW_HC32_I2C_HIGH_SPEED
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_cr_set (amhw_hc32_i2c_t  *p_hw_i2c,
                             uint16_t            flag)
{
    p_hw_i2c->i2c_cr |= flag;
}

/**
 * \brief 清除控制寄存器值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  flag      : i2c配置标志，see:AMHW_HC32_I2C_HIGH_SPEED
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_cr_clear (amhw_hc32_i2c_t    *p_hw_i2c,
                               uint16_t              flag)
{
    p_hw_i2c->i2c_cr &= ~((uint32_t)flag);
}

/**
 * \brief 获取中断标志
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_hc32_i2c_irq_get (amhw_hc32_i2c_t    *p_hw_i2c)
{
    return ((p_hw_i2c->i2c_cr >> 3) & 0x1);
}

/**
 * \brief 使能 I2C 模块
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_enable (amhw_hc32_i2c_t *p_hw_i2c)
{
    p_hw_i2c->i2c_cr |= (0x1ul << 6);
}

/**
 * \brief 禁能 I2C 模块（发送和接收缓冲保持擦除状态）
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_disable (amhw_hc32_i2c_t *p_hw_i2c)
{
    p_hw_i2c->i2c_cr &= ~(0x1ul << 6);
}
/**
 * \brief 配置为高级滤波，抗扰性强
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_hlm_disable (amhw_hc32_i2c_t *p_hw_i2c)
{
    p_hw_i2c->i2c_cr &= ~(0x1ul << 0);
}

/**
 * \brief 简单滤波，通信速率更高
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_hlm_enable (amhw_hc32_i2c_t *p_hw_i2c)
{
    p_hw_i2c->i2c_cr |= (0x1ul << 0);
}
/**
 * \brief 写数据.
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  dat       : 数据
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_dat_write (amhw_hc32_i2c_t   *p_hw_i2c,
                                uint8_t              dat)
{
    p_hw_i2c->i2c_data = dat;
}

/**
 * \brief 读数据.
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 数据
 */
am_static_inline
uint8_t amhw_hc32_i2c_dat_read (amhw_hc32_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->i2c_data);
}

/**
 * \brief 设置从机地址.
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  addr      : 从机地址
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_i2c_slave_addr_set (amhw_hc32_i2c_t  *p_hw_i2c,
                                     uint16_t            addr)
{
    p_hw_i2c->i2c_addr = (p_hw_i2c->i2c_addr & ~0xfe) | ((addr << 1) & 0xfe);
}

/**
 * \brief I2C接受广播呼叫 响应 ACK
 *
 */
am_static_inline
void amhw_hc32_i2c_gen_call_ack (amhw_hc32_i2c_t *p_hw_i2c)
{
    p_hw_i2c->i2c_addr |= 0x1ul;
}

/**
 * \brief I2C接受广播呼叫 不响应
 *
 */
am_static_inline
void amhw_hc32_i2c_gen_call_nack (amhw_hc32_i2c_t *p_hw_i2c)
{
    p_hw_i2c->i2c_addr &= ~0x1ul;
}

/**
 * \brief 获取I2C状态
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 状态（或关系）
 */
am_static_inline
amhw_hc32_status_flag_t amhw_hc32_i2c_status_get (amhw_hc32_i2c_t *p_hw_i2c)
{
    return (amhw_hc32_status_flag_t)(p_hw_i2c->i2c_stat);
}

/**
 * @}
 */
/*
 * \brief 匿名结构体段的结束
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

#ifdef __cplusplus
}
#endif /* __AMHW_HC32_I2C_H */

#endif

/* end of file */
