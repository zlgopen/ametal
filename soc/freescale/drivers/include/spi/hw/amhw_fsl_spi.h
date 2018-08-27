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
 * 3. 支持64-bit FIFO模式传输(仅SPI1)
 * 4. 支持DMA传输(发送和接收)；
 * 5. 波特率可配置；
 * 6. 时钟相位和极性可配置；
 * 7. 支持8bits或者16bits传输,可配置LSB或者MSB传输；
 * 8. 可以设置匹配接收数据，匹配成功时产生中断；
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-26  sdy, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_SPI_H
#define __AMHW_FSL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"
#include "am_bitops.h"
/**
 * \addtogroup amhw_fsl_if_spi
 * \copydoc amhw_fsl_spi.h
 * @{
 */
 
/**
 * \brief SPI寄存器块结构体
 */
typedef struct amhw_fsl_spi {
    __IO uint8_t  s;             /**< \brief SPI 状态寄存器       */
    __IO uint8_t  br;            /**< \brief SPI 波特率配置寄存器 */
    __IO uint8_t  c2;            /**< \brief SPI 控制寄存器2      */
    __IO uint8_t  c1;            /**< \brief SPI 控制寄存器1      */
    __IO uint8_t  ml;            /**< \brief SPI 低字节匹配寄存器 */
    __IO uint8_t  mh;            /**< \brief SPI 高字节匹配寄存器 */
    __IO uint8_t  dl;            /**< \brief SPI 低字节数据寄存器 */
    __IO uint8_t  dh;            /**< \brief SPI 高字节数据寄存器 */
    __I  uint8_t  reserved[2];   /**< \brief SPI 保留寄存器       */
    __IO uint8_t  ci;            /**< \brief SPI 中断清除寄存器   */
    __IO uint8_t  c3;            /**< \brief SPI 控制寄存器3      */
} amhw_fsl_spi_t;


/**
 * \name SPI 状态标志
 * @{
 */

#define AMHW_FSL_SPI_STAT_R_FULL        AM_BIT(7)  /**< \brief 接收完成       */
#define AMHW_FSL_SPI_STAT_MATCH         AM_BIT(6)  /**< \brief 数据发生匹配   */
#define AMHW_FSL_SPI_STAT_T_EMPTY       AM_BIT(5)  /**< \brief 发送完成       */
#define AMHW_FSL_SPI_STAT_MODF          AM_BIT(4)  /**< \brief 多主机         */
#define AMHW_FSL_SPI_STAT_R_NEAR_FULL   AM_BIT(3)  /**< \brief 接收FIFO接近满 */
#define AMHW_FSL_SPI_STAT_T_NEAR_EMPTY  AM_BIT(2)  /**< \brief 发送FIFO接近空 */
#define AMHW_FSL_SPI_STAT_T_FIFO_FULL   AM_BIT(1)  /**< \brief 发送FIFO满     */
#define AMHW_FSL_SPI_STAT_R_FIFO_EMPTY  AM_BIT(0)  /**< \brief 接收FIFO空     */

/** @} */

/**
 * \name SPI FIFO错误标志
 * @{
 */

#define AMHW_FSL_SPI_ERROR_T_FIFO   AM_BIT(7)     /**< \brief 发送FIFO错误    */
#define AMHW_FSL_SPI_ERROR_R_FIFO   AM_BIT(6)     /**< \brief 接收FIFO错误    */
#define AMHW_FSL_SPI_OV_T_FIFO      AM_BIT(5)     /**< \brief 发送FIFO溢出    */
#define AMHW_FSL_SPI_OV_R_FIFO      AM_BIT(4)     /**< \brief 接收FIFO溢出    */

/** @} */

/**
 * \name SPI 普通中断
 * @{
 */

#define AMHW_FSL_SPI_IRQ_MATCH       AM_BIT(7)  /**< \brief 数据匹配中断      */
#define AMHW_FSL_SPI_IRQ_R_MODDEF    AM_BIT(6)  /**< \brief 接收或多主机中断  */
#define AMHW_FSL_SPI_IRQ_T           AM_BIT(5)  /**< \brief 发送完成中断      */

/**
 * \name SPI FIFO中断
 * @{
 */

#define AMHW_FSL_SPI_FIFO_IRQ_T_NERAEMPTY AM_BIT(2) /**< \brief 发送FIFO近空中断 */
#define AMHW_FSL_SPI_FIFO_IRQ_R_NERAFULL  AM_BIT(1) /**< \brief 接收FIFO近满中断 */

/** @} */

/**
 * \name SPI 工作模式
 * @{
 */

#define AMHW_FSL_SPI_WMODE_MASTER    (1 << 4)  /**< \brief 主机模式         */
#define AMHW_FSL_SPI_WMODE_SLAVE     (0 << 4)  /**< \brief 从机模式         */
#define AMHW_FSL_SPI_WMODE_M_SINGLE  ( 0x11 )  /**< \brief 单数据线主机模式 */
#define AMHW_FSL_SPI_WMODE_S_SINGLE  ( 0x01 )  /**< \brief 单数据线从机模式 */

/** @} */

/**
 * \name SPI 特性
 * @{
 */

/** \brief 传输数据宽度为8位 */
#define AMHW_FSL_SPI_CFG_8BIT_WIDTH     (AM_BIT(31) | AM_SBF(0,0))

/** \brief 传输数据宽度为16位 */
#define AMHW_FSL_SPI_CFG_16BIT_WIDTH    (AM_BIT(31) | AM_SBF(1,0))

/** \brief 禁能DMA发送 */
#define AMHW_FSL_SPI_CFG_TDMA_DIS       (AM_BIT(30) | AM_SBF(0,1))

/** \brief 使能DMA发送 */
#define AMHW_FSL_SPI_CFG_TDMA_EN        (AM_BIT(30) | AM_SBF(1,1))

/** \brief 禁能DMA接收 */
#define AMHW_FSL_SPI_CFG_RDMA_DIS       (AM_BIT(29) | AM_SBF(0,2))

/** \brief 使能DMA接收 */
#define AMHW_FSL_SPI_CFG_RDMA_EN        (AM_BIT(29) | AM_SBF(1,2))

/** \brief 停止模式SPI继续工作 */
#define AMHW_FSL_SPI_CFG_WIAT_EN        (AM_BIT(28) | AM_SBF(0,3))

/** \brief 停止模式SPI不工作 */
#define AMHW_FSL_SPI_CFG_WIAT_DIS       (AM_BIT(28) | AM_SBF(1,3))

/** \brief 高位优先发送 */
#define AMHW_FSL_SPI_CFG_MSB            (AM_BIT(27) | AM_SBF(0,4))

/** \brief 低位优先发送 */
#define AMHW_FSL_SPI_CFG_LSB            (AM_BIT(27) | AM_SBF(1,4))

/** @} */

/**
 * \name SPI 片选引脚功能
 * @{
 */

/** \brief SPI使用CS引脚作为片选输出 */
#define AMHW_FSL_SPI_CS_SPI_OUT   ((1 << 4) | (1 << 1))

/** \brief SPI使用CS引脚作为输入，检测多主机 */
#define AMHW_FSL_SPI_CS_SPI_MODF  ((1 << 4) | (0 << 1))

/** \brief SPI不使用CS引脚，CS引脚为GPIO模式 */
#define AMHW_FSL_SPI_CS_GPIO      ((0 << 4) | (0 << 1))

/** @} */

/**
 * \name SPI 传输模式
 * @{
 */ 

/** \brief SCK默认为低，数据下降沿改变，上升沿保持 */
#define AMHW_FSL_SPI_MODE_0     ((0 << 3) |(0 << 2))
 
/** \brief SCK默认为低，数据上升沿改变，下降沿保持 */
#define AMHW_FSL_SPI_MODE_1     ((0 << 3) |(1 << 2))

/** \brief SCK默认为高，数据上升沿改变，下降沿保持 */
#define AMHW_FSL_SPI_MODE_2     ((1 << 3) |(0 << 2))
     
/** \brief SCK默认为高，数据下降沿改变，上升保持   */
#define AMHW_FSL_SPI_MODE_3     ((1 << 3) |(1 << 2))

/** @} */

/**
 * \name SPI FIFO中断清除选项
 * @{
 */

/** \brief 清除发送FIFO接近空中断 */
#define AMHW_FSL_SPI_FIFO_IRQ_CLR_TNE   (1 << 3)

/** \brief 清除接收FIFO接近满中断 */
#define AMHW_FSL_SPI_FIFO_IRQ_CLR_RNF   (1 << 2)

/** \brief 清除发送完成中断(发送FIFO空) */
#define AMHW_FSL_SPI_FIFO_IRQ_CLR_TE    (1 << 1)

/** \brief 清除接收完成(接收FIFO满) */
#define AMHW_FSL_SPI_FIFO_IRQ_CLR_RF    (1 << 0)

/** @} */

/**
 * \name FIFO接近程度选项
 * @{
 */

/** \brief 发送缓冲区小于16bit,接收缓冲区多于48bit */
#define AMHW_FSL_SPI_FIFO_T_16BITS_R_48BITS ( (0 << 5) | (0 << 4))

/** \brief 发送缓冲区小于16bit,接收缓冲区多于32bit */
#define AMHW_FSL_SPI_FIFO_T_16BITS_R_32BITS ( (0 << 5) | (1 << 4) )

/** \brief 发送缓冲区小于32bit,接收缓冲区多于48bit */
#define AMHW_FSL_SPI_FIFO_T_32BITS_R_48BITS ( (1 << 5) | (0 << 4) )

/** \brief 发送缓冲区小于32bit,接收缓冲区多于32bit */
#define AMHW_FSL_SPI_FIFO_T_32BITS_R_32BITS ( (1 << 5) | (1 << 4) )

/** @} */

/**
 * \brief 使能SPI模块
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \return 无
 */
am_static_inline 
void amhw_fsl_spi_enable (amhw_fsl_spi_t *p_hw_spi)
{
    p_hw_spi->c1 |= (1 << 6);
}

/**
 * \brief 禁能SPI模块
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \return  无
 */
am_static_inline
void amhw_fsl_spi_disable (amhw_fsl_spi_t *p_hw_spi)
{
    p_hw_spi->c1 &= (~(1 << 6));
}

/**
 * \brief 获取状态标志
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \return  状态标志(#AMHW_FSL_SPI_STAT_R_FULL)
 */
am_static_inline
uint8_t amhw_fsl_spi_stat_get (amhw_fsl_spi_t *p_hw_spi)
{
    return p_hw_spi->s;
}

/**
 * \brief 配置分频系数
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] pdiv     : 预分频系数(0-8)
 * \param[in] div      : 分配系数(0-9)
 *
 * \return  无
 *
 * \note 总分频系数Div=(pdiv+1)*2^(div+1),SPI波特率：Baud=输入时钟/总分频系数
 */
am_static_inline
void amhw_fsl_spi_div_cfg (amhw_fsl_spi_t *p_hw_spi,
                            uint8_t          pdiv,
                            uint8_t          div)
{
    p_hw_spi->br = ((pdiv & 0x70) | (div & 0x0F));
}

/**
 * \brief 使能普通中断
 *
 * \param[in] p_hw_spi  : 指向SPI寄存器块的指针
 * \param[in] irq_flags : 中断标识，AMHW_FSL_SPI_IRQ_*宏值或多个
 *                        (#AMHW_FSL_SPI_IRQ_MATCH)
 * \return  无
 */
void amhw_fsl_spi_int_enable (amhw_fsl_spi_t *p_hw_spi, uint8_t irq_flags);

/**
 * \brief 禁能普通中断
 *
 * \param[in] p_hw_spi  : 指向SPI寄存器块的指针
 * \param[in] irq_flags : 中断标识，AMHW_FSL_SPI_IRQ_*宏值或多个
 *                       (#AMHW_FSL_SPI_IRQ_MATCH)
 * \return  无
 */
void amhw_fsl_spi_int_disable (amhw_fsl_spi_t *p_hw_spi, uint8_t irq_flags);

/**
 * \brief 配置SPI工作模式(默认为从机模式)
 *
 * \param[in] p_hw_spi  : 指向SPI寄存器块的指针
 * \param[in] mode_flag : 工作模式，AMHW_FSL_SPI_WMODE_*宏值
 *                        (#AMHW_FSL_SPI_WMODE_MASTER)
 * \return  无
 */
void amhw_fsl_spi_workmode_cfg (amhw_fsl_spi_t *p_hw_spi, uint8_t mode_flag);

/**
 * \brief 配置SPI特性
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] flags    : 特性标识，AMHW_FSL_SPI_CFG_*宏值或多个
 *                      (#AMHW_FSL_SPI_CFG_8BIT_WIDTH)
 *
 * \return  无
 *
 * \note 先前配置的特性被保留，只更改flags参数包含的特性
 */
void amhw_fsl_spi_feature_cfg (amhw_fsl_spi_t *p_hw_spi, uint32_t flags);

/**
 * \brief 配置CS片选引脚(默认为GPIO功能)
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] cs_flag  : 功能标识，AMHW_FSL_SPI_CS_*宏值(#AMHW_FSL_SPI_CS_GPIO)
 *
 * \return  无
 *
 * \note 只适用于主机工作模式(从机模式，CS只能为片选输入)
 */
void amhw_fsl_spi_cs_cfg (amhw_fsl_spi_t *p_hw_spi, uint8_t cs_flag);

/**
 * \brief 模式配置(时钟相位和极性)
 *
 * \param[in] p_hw_spi  : 指向SPI寄存器块的指针
 * \param[in] mode_flag : 功能标识，AMHW_FSL_SPI_MODE*宏值(#AMHW_FSL_SPI_MODE_0)
 *
 * \return  无
 */
am_static_inline
void amhw_fsl_spi_mode_cfg (amhw_fsl_spi_t *p_hw_spi, uint8_t mode_flag)
{
    uint8_t tmp  = mode_flag & (0x0C);

    p_hw_spi->c1 = (tmp | (p_hw_spi->c1 & (~0x0C)));
}

/**
 * \brief 读取接收到的数据(读16bits)
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \return  读取到的数据
 */
am_static_inline
uint16_t amhw_fsl_spi_data16_read (amhw_fsl_spi_t *p_hw_spi)
{
    uint16_t tmp = p_hw_spi->dh;

    tmp <<= 8;
    tmp |= p_hw_spi->dl;

    return tmp;
}

/**
 * \brief 读取接收到的数据(读8bits)
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \return  读取到的数据
 */
am_static_inline
uint8_t amhw_fsl_spi_data8_read (amhw_fsl_spi_t *p_hw_spi)
{
    return p_hw_spi->dl;
}

/**
 * \brief 发送数据(16bits)
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] data     ：发送的数据
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_spi_data16_wirte (amhw_fsl_spi_t *p_hw_spi, uint16_t data)
{
    p_hw_spi->dh = (data >> 8);
    p_hw_spi->dl = data;
}

/**
 * \brief 发送数据(8bits)
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] data     ：发送的数据
 *
 * \return  读取到的数据
 */
am_static_inline
void amhw_fsl_spi_data8_wirte (amhw_fsl_spi_t *p_hw_spi, uint8_t data)
{
    p_hw_spi->dl = data;
}

/**
 * \brief 获取当前设置的匹配数据(16bit)
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \return  读取到的数据
 */
am_static_inline
uint16_t amhw_fsl_spi_matchdata16_read (amhw_fsl_spi_t *p_hw_spi)
{
    uint16_t tmp = p_hw_spi->mh;

    tmp <<= 8;
    tmp |= p_hw_spi->ml;

    return tmp;
}

/**
 * \brief 配置匹配数据(16bit)
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] data     ：需要匹配的数据
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_spi_matchdata16_wirte (amhw_fsl_spi_t *p_hw_spi, uint16_t data)
{
    p_hw_spi->mh = (data >> 8);
    p_hw_spi->ml = data;
}

/**
 * \brief MOMI或者SISO数据线的方向配置
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] dir      ：方向
 *      \arg 0   :  输入
 *      \arg 非0 ： 输出
 *
 * \return 无
 *
 * \note 主机模式使用MIMO,从机模式使用SISO，单数据线模式有效
 */
am_static_inline
void amhw_fsl_spi_dataline_dir_cfg (amhw_fsl_spi_t *p_hw_spi, uint8_t dir)
{
    if (dir) {
        p_hw_spi->c2 &= (~0x08);
    } else {
        p_hw_spi->c2 |= 0x08;
    }
}

/******************************************************************************
 * 以下为FIFO特性配置,只有SPI1支持下面特性                                    *
 * ***************************************************************************/

/**
 * \brief FIFO模式使用配置
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] use_fifo : 是否使用FIFO
 *      \arg AM_TRUE  : 启用FIFO模式
 *      \arg AM_FALSE : 不使用FIFO模式
 *
 * \return  无
 *
 * \note 只有SPI1可以使用此函数
 */
am_static_inline
void amhw_fsl_spi_fifo_cfg (amhw_fsl_spi_t *p_hw_spi, am_bool_t use_fifo)
{
    /* 使用FIFO */
    if (use_fifo) {
        p_hw_spi->c3 |= 0x01;
    } else {
        p_hw_spi->c3 &= (~0x01);
    }
}

/**
 * \brief 获取FIFO错误标志
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \return  错误标志
 * \note 读取标志后硬件清除，此函数只能被SPI1调用
 */
am_static_inline
uint8_t amhw_fsl_spi_error_get (amhw_fsl_spi_t *p_hw_spi)
{
    return (p_hw_spi->ci & 0xF0);
}

/**
 * \brief 配置FIFO接近程度(默认T-16bit,R-48bit)
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] flags    : 接近程度标识(#AMHW_FSL_SPI_FIFO_T_16BITS_R_48BITS)
 *
 * \return  无
 *
 * \note 只有SPI1可以调用此函数
 */
am_static_inline
void amhw_fsl_spi_fifo_near_cfg (amhw_fsl_spi_t *p_hw_spi, uint8_t flags)
{
    uint8_t tmp = flags & 0x30;

    p_hw_spi->c3 = (tmp | (p_hw_spi->c3 & (~0x30)));
}

/**
 * \brief 配置FIFO中断标志清除模式(默认自动清除标志)
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] is_auto  : 是否根据FIFO状态自动清除
 *      \arg AM_TRUE  : 根据FIFO状态自动清除
 *      \arg AM_FALSE ：使用amhw_fsl_spi_fifo_flags_clr清除状态标志
 *
 * \return  无
 *
 * \note 只有SPI1可以调用此函数
 */
am_static_inline
void amhw_fsl_spi_fifo_irqs_clrmode_cfg (amhw_fsl_spi_t *p_hw_spi,
                                          am_bool_t           is_auto)
{
    if (is_auto) {
        p_hw_spi->c3 &= (~0x08);
    } else {
        p_hw_spi->c3 |= 0x08;
    }
}

/**
 * \brief 清除FIFO中断标志
 *
 * \param[in] p_hw_spi : 指向SPI寄存器块的指针
 * \param[in] flags    : FIFO标志，AMHW_FSL_SPI_FIFO_IRQ_CLR_*宏值
 *                      (#AMHW_FSL_SPI_FIFO_IRQ_CLR_TNE)
 *
 * \return  无
 *
 * \note 只有SPI1可以调用此函数
 */
am_static_inline
void amhw_fsl_spi_fifo_irqs_clr (amhw_fsl_spi_t *p_hw_spi, uint8_t flags)
{
    p_hw_spi->ci |= (flags & 0x0F);
}

/**
 * \brief 使能FIFO中断
 *
 * \param[in] p_hw_spi       : 指向SPI寄存器块的指针
 * \param[in] fifo_irq_flags : 中断标识，AMHW_FSL_SPI_FIFO_IRQ_*宏值或多个
 *                             (#AMHW_FSL_SPI_FIFO_IRQ_T_NERAEMPTY)
 *
 * \return  无
 *
 * \note 此函数只能被SPI1调用
 */
am_static_inline
void amhw_fsl_spi_fifo_int_enable (amhw_fsl_spi_t *p_hw_spi,
                                    uint8_t          fifo_irq_flags)
{
    if (fifo_irq_flags & AMHW_FSL_SPI_FIFO_IRQ_T_NERAEMPTY) {
        p_hw_spi->c3 |= AMHW_FSL_SPI_FIFO_IRQ_T_NERAEMPTY;
    }
    if (fifo_irq_flags & AMHW_FSL_SPI_FIFO_IRQ_R_NERAFULL) {
        p_hw_spi->c3 |= AMHW_FSL_SPI_FIFO_IRQ_R_NERAFULL;
    }
}

/**
 * \brief 禁能FIFO中断
 *
 * \param[in] p_hw_spi       : 指向SPI寄存器块的指针
 * \param[in] fifo_irq_flags : 中断标识，AMHW_FSL_SPI_FIFO_IRQ_*宏值或多个
 *                           (#AMHW_FSL_SPI_FIFO_IRQ_T_NERAEMPTY)
 * \return  无
 *
 * \note 此函数只能被SPI1调用
 */
am_static_inline
void amhw_fsl_spi_fifo_int_disable (amhw_fsl_spi_t *p_hw_spi,
                                     uint8_t          fifo_irq_flags)
{
    if (fifo_irq_flags & AMHW_FSL_SPI_FIFO_IRQ_T_NERAEMPTY) {
        p_hw_spi->c3 &= ~AMHW_FSL_SPI_FIFO_IRQ_T_NERAEMPTY;
    }
    if (fifo_irq_flags & AMHW_FSL_SPI_FIFO_IRQ_R_NERAFULL) {
        p_hw_spi->c3 &= ~AMHW_FSL_SPI_FIFO_IRQ_R_NERAFULL;
    }
}

/** 
 * @} amhw_fsl_if_spi
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_SPI_H */

/*end of file */
