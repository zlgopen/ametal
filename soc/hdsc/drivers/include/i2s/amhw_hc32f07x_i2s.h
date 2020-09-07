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
 * \brief I2S 硬件操作接口
 */
 
#ifndef __AMHW_HC32F07X_I2S_H
#define __AMHW_HC32F07X_I2S_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_hc32f07x_if_i2s
 * \copydoc amhw_hc32f07x_i2s.h
 * @{
 */

/**
 * \brief SPI structure of register
 */
typedef struct amhw_hc32f07x_i2s {
    __IO uint32_t  reserve0;     /**< \brief 保留*/
    __IO uint32_t  cr;           /**< \brief I2S 控制寄存器 */
    __IO uint32_t  sr;           /**< \brief I2S 中断和状态寄存器 */
    __IO uint32_t  reserve1[4];  /**< \brief 保留*/
    __IO uint32_t  cfgr;         /**< \brief I2S 配置寄存器 */
    __IO uint32_t  pr;           /**< \brief I2S 主模式分频寄存器 */
    __IO uint32_t  icr;          /**< \brief I2S 中断标志清除寄存器  */
    __IO uint32_t  drl;          /**< \brief I2S 左声道发送数据与接收数据寄存器 */
    __IO uint32_t  drr;          /**< \brief I2S 右声道发送数据与接收数据寄存器 */
} amhw_hc32f07x_i2s_t;	

/**
 * \brief I2S 发送缓冲区空中断使能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_tx_empty_enable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cr |= (0x1ul << 7);
}

/**
 * \brief I2S 发送缓冲区空中断禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_tx_empty_disable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cr &= ~(0x1ul << 7);
}

/**
 * \brief I2S 接收缓冲区非空中断使能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_rx_not_empty_enable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cr |= (0x1ul << 6);
}

/**
 * \brief I2S 接收缓冲区空中断禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_rx_not_empty_disable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cr &= ~(0x1ul << 6);
}

/**
 * \brief I2S 错误中断使能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_err_enable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cr |= (0x1ul << 5);
}

/**
 * \brief I2S 错误中断禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_err_disable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cr &= ~(0x1ul << 5);
}

/**
 * \brief I2S 右声道缓冲区DMA数据发送使能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_right_dma_enable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cr |= (0x1ul << 1);
}

/**
 * \brief I2S 右声道缓冲区DMA数据发送禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_right_dma_disable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cr &= ~(0x1ul << 1);
}

/**
 * \brief I2S 左声道缓冲区DMA数据发送使能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_left_dma_enable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cr |= (0x1ul << 0);
}

/**
 * \brief I2S 左声道缓冲区DMA数据发送禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_left_dma_disable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cr &= ~(0x1ul << 0);
}

/**
 * \brief I2S状态标志
 *  @{
 */
#define AMHW_HC32F07X_I2S_FLAG_RIGHT_TX_EMPTY         (1U << 15) /* 右通道发送缓冲器空标志 */
#define AMHW_HC32F07X_I2S_FLAG_RIGHT_RX_NOT_EMPTY     (1U << 14) /* 右通道接收缓冲器非空标志 */
#define AMHW_HC32F07X_I2S_FLAG_LEFT_TX_EMPTY          (1U << 1 ) /* 左通道发送缓冲器空标志 */
#define AMHW_HC32F07X_I2S_FLAG_LEFT_RX_NOT_EMPTY      (1U << 0 ) /* 左通道接收缓冲器非空标志 */
#define AMHW_HC32F07X_I2S_FLAG_FRE_ERR                (1U << 8 ) /* 帧错误标志 */
#define AMHW_HC32F07X_I2S_FLAG_BUSY                   (1U << 7 ) /* 忙标志 */
#define AMHW_HC32F07X_I2S_FLAG_RIGHT_OVER             (1U << 13) /* 右通道上溢标志 */
#define AMHW_HC32F07X_I2S_FLAG_RIGHT_UNDER            (1U << 3 ) /* 右通道下溢空标志 */
#define AMHW_HC32F07X_I2S_FLAG_LEFT_OVER              (1U << 6 ) /* 左通道上溢标志 */
#define AMHW_HC32F07X_I2S_FLAG_LEFT_UNDER             (1U << 2 ) /* 左通道下溢标志 */
/**
 * \brief 标志有效性判断
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] flag     : 标志宏定义
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32f07x_i2s_flag_check (amhw_hc32f07x_i2s_t *p_hw_i2s,
                                        uint16_t            flag)
{
    return ((p_hw_i2s->sr & flag)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 标志清零
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] flag     : 标志宏定义
 *
 * \return 
 */
am_static_inline
void amhw_hc32f07x_i2s_flag_clr (amhw_hc32f07x_i2s_t *p_hw_i2s,
                                        uint8_t            flag)
{
   p_hw_i2s->sr &= ~flag;
}

/**
 * \brief I2S 使能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_enable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cfgr |= (0x1ul << 10);
}

/**
 * \brief I2S 禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_disable (amhw_hc32f07x_i2s_t *p_hw_i2s)
{
    p_hw_i2s->cfgr &= ~(0x1ul << 10);
}

/**
 * \brief I2S配置模式选择
 */
typedef enum {
    AMHW_HC32F07X_I2S_MODE_SLAVER_TX = 0,  /**< \brief 从机模式发送 */
    AMHW_HC32F07X_I2S_MODE_SLAVER_RX = 1,  /**< \brief 从机模式接收 */
    AMHW_HC32F07X_I2S_MODE_MASTER_TX = 2,  /**< \brief 主机模式发送 */
    AMHW_HC32F07X_I2S_MODE_MASTER_RX = 3,  /**< \brief 主机模式接收 */
}amhw_hc32f07x_i2s_mode_t;

/**
 * \brief I2S 主机/从机模式选择
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] flag     : amhw_hc32f07x_i2s_mode_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_mode_sel (amhw_hc32f07x_i2s_t          *p_hw_i2s,
                                   amhw_hc32f07x_i2s_mode_t mode_type)
{
    p_hw_i2s->cfgr = (p_hw_i2s->cfgr & (~(3u << 8))) | (mode_type << 8);
}

/**
 * \brief I2S PCM帧同步选择
 */
typedef enum {
    AMHW_HC32F07X_I2S_PCM_SHORT = 0,  /**< \brief PCM 短帧同步 */
    AMHW_HC32F07X_I2S_PCM_LONG  = 1,  /**< \brief PCM 长帧同步 */
}amhw_hc32f07x_i2s_pcm_t;

/**
 * \brief I2S PCM帧同步
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] flag     : amhw_hc32f07x_i2s_mode_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_pcm (amhw_hc32f07x_i2s_t          *p_hw_i2s,
                              amhw_hc32f07x_i2s_pcm_t mode_type)
{
    p_hw_i2s->cfgr = (p_hw_i2s->cfgr & (~(1u << 7))) | (mode_type << 7 );
}

/**
 * \brief I2S 时钟选择
 */
typedef enum {
    AMHW_HC32F07X_I2S_PCLK = 0,  /**< \brief PCLK时钟 */
    AMHW_HC32F07X_I2S_HCLK = 1,  /**< \brief HCLK时钟 */
}amhw_hc32f07x_i2s_clk_t;
/**
 * \brief I2S 时钟选择
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] flag     : amhw_hc32f07x_i2s_mode_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_clk (amhw_hc32f07x_i2s_t          *p_hw_i2s,
                              amhw_hc32f07x_i2s_clk_t mode_clk)
{
    p_hw_i2s->cfgr = (p_hw_i2s->cfgr & (~(1u << 6))) | (mode_clk << 6 );
}

/**
 * \brief I2S 标准选择
 */
typedef enum {
    AMHW_HC32F07X_I2S_PHI = 0,  /**< \brief I2S philips 标准*/
    AMHW_HC32F07X_I2S_MSB = 1,  /**< \brief MSB左对齐标准 */
    AMHW_HC32F07X_I2S_LSB = 2,  /**< \brief LSB右对齐标准 */
    AMHW_HC32F07X_I2S_PCM = 3,  /**< \brief PCM 标准 */
}amhw_hc32f07x_i2s_std_t;

/**
 * \brief I2S 标准选择
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] flag     : amhw_hc32f07x_i2s_std_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_std (amhw_hc32f07x_i2s_t          *p_hw_i2s,
                              amhw_hc32f07x_i2s_std_t std)
{
    p_hw_i2s->cfgr = (p_hw_i2s->cfgr & (~(3u << 4))) | (std << 4 );
}

/**
 * \brief I2S 数据长度
 */
typedef enum {
    AMHW_HC32F07X_I2S_DATA_LEN16  = 0,  /**< \brief 16位数据长度 */
    AMHW_HC32F07X_I2S_DATA_LEN24  = 1,  /**< \brief 24位数据长度 */
    AMHW_HC32F07X_I2S_DATA_LEN32  = 2,  /**< \brief 32位数据长度 */
    AMHW_HC32F07X_I2S_DATA_FORBID = 3,  /**< \brief 不允许 */
}amhw_hc32f07x_i2s_data_t;

/**
 * \brief I2S 要传输的数据长度
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] flag     : amhw_hc32f07x_i2s_data_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_data_length (amhw_hc32f07x_i2s_t          *p_hw_i2s,
                                      amhw_hc32f07x_i2s_data_t length)
{
    p_hw_i2s->cfgr = (p_hw_i2s->cfgr & (~(3u << 1))) | (length << 1 );
}

/**
 * \brief I2S 通道数据长度
 */
typedef enum {
    AMHW_HC32F07X_I2S_CH_LEN16  = 0,  /**< \brief 16位通道长度 */

    AMHW_HC32F07X_I2S_CH_LEN32  = 1,  /**< \brief 32位通道长度 */

}amhw_hc32f07x_i2s_ch_t;

/**
 * \brief I2S 要传输的数据长度
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] flag     : amhw_hc32f07x_i2s_ch_t枚举量
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_ch_length (amhw_hc32f07x_i2s_t   *p_hw_i2s,
                                  amhw_hc32f07x_i2s_ch_t ch_length)
{
    p_hw_i2s->cfgr = (p_hw_i2s->cfgr & (~(1u << 1))) | (ch_length << 0 );
}

/**
 * \brief I2S 主时钟输出使能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_clk_enable (amhw_hc32f07x_i2s_t   *p_hw_i2s)
{
    p_hw_i2s->pr |= (1u<<9);
}

/**
 * \brief I2S 主时钟输出禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_clk_disable (amhw_hc32f07x_i2s_t   *p_hw_i2s)
{
    p_hw_i2s->pr &= ~(1u<<9);
}

/**
 * \brief I2S 预分频器小数设置
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] value    : 设置值
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_fract_set (amhw_hc32f07x_i2s_t   *p_hw_i2s,
                                  uint8_t               value)
{
    p_hw_i2s->pr = (p_hw_i2s->pr & (~(0x3fu << 10))) | ((value & 0x3f) << 10);
}

/**
 * \brief I2S 预分频器奇数因子使能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_odd_enable (amhw_hc32f07x_i2s_t   *p_hw_i2s)
{
    p_hw_i2s->pr |= (1 << 8);
}

/**
 * \brief I2S 预分频器奇数因子禁能
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_odd_disable (amhw_hc32f07x_i2s_t   *p_hw_i2s)
{
    p_hw_i2s->pr &= ~(1 << 8);
}

/**
 * \brief I2S 线性预分频器设置
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] value    : 预分频器设置值
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_div_set (amhw_hc32f07x_i2s_t   *p_hw_i2s,
                                  uint8_t               value)
{
    p_hw_i2s->pr = (p_hw_i2s->pr & (~0xffu )) | (value & 0xff);
}

/**
 * \brief I2S中断状态标志
 *  @{
 */
#define AMHW_HC32F07X_I2S_INT_FRE                  (1U << 8 ) /* 帧错误标志 */
#define AMHW_HC32F07X_I2S_INT_OVER                 (1U << 6 ) /* 上溢标志 */
#define AMHW_HC32F07X_I2S_INT_UNDER                (1U << 3 ) /* 下溢标志 */

/**
 * \brief I2S 线性预分频器设置
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] flag     : 中断状态标志，参考宏定义AMHW_HC32_I2S_INT_
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_intstat_clr (amhw_hc32f07x_i2s_t   *p_hw_i2s,
                                    uint32_t               flag)
{
    p_hw_i2s->icr &= flag;
}

/**
 * \brief I2S 写数据寄存器（左）
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] value    : 数据值
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_data_left_write (amhw_hc32f07x_i2s_t   *p_hw_i2s,
                                        uint16_t               value)
{
    p_hw_i2s->drl = value;
}

/**
 * \brief I2S 读数据寄存器（左）
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return  接收缓冲区值
 */
am_static_inline
uint16_t amhw_hc32f07x_i2s_data_left_read (amhw_hc32f07x_i2s_t   *p_hw_i2s)
{
    return (uint16_t)(p_hw_i2s->drl & 0xffff);
}

/**
 * \brief I2S 写数据寄存器（右）
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] value    : 数据值
 *
 * \return none
 */
am_static_inline
void amhw_hc32f07x_i2s_data_right_write (amhw_hc32f07x_i2s_t   *p_hw_i2s,
                                         uint16_t               value)
{
    p_hw_i2s->drr = value;
}

/**
 * \brief I2S 读数据寄存器（右）
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 *
 * \return  接收缓冲区值
 */
am_static_inline
uint16_t amhw_hc32f07x_i2s_data_right_read (amhw_hc32f07x_i2s_t   *p_hw_i2s)
{
    return (uint16_t)(p_hw_i2s->drr & 0xffff);
}

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32_SPI_H */

/* end of file */	
