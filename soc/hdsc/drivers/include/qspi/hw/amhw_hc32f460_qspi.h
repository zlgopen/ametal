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
 * \brief QSPI操作接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-17  cds, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32F460_QSPI_H
#define __AMHW_HC32F460_QSPI_H

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
 * \brief QSPI 控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t MDSEL                     : 3;  /**< \brief QSPI读取模式选择 */
    __IO uint32_t PFE                       : 1;  /**< \brief 预读取许可 */
    __IO uint32_t PFSAE                     : 1;  /**< \brief 预读取即时停止许可 */
    __IO uint32_t DCOME                     : 1;  /**< \brief 直接通信许可 */
    __IO uint32_t XIPE                      : 1;  /**< \brief XIP模式许可 */
    __IO uint32_t SPIMD3                    : 1;  /**< \brief SPI模式选择 */
    __IO uint32_t IPRSL                     : 2;  /**< \brief 指令发送阶段SPI协议选择 */
    __IO uint32_t APRSL                     : 2;  /**< \brief 地址发送阶段SPI协议选择 */
    __IO uint32_t DPRSL                     : 2;  /**< \brief 数据接收阶段SPI协议选择 */
    uint32_t RESERVED14                     : 2;  /**< \brief 保留 */
    __IO uint32_t DIV                       : 6;  /**< \brief 基准时钟选择位 */
    uint32_t RESERVED22                     :10;  /**< \brief 保留 */
} stc_qspi_cr_field_t;

/**
 * \brief QSPI 片选控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SSHW                      : 4;  /**< \brief QSSN最小无效时间设置 */
    __IO uint32_t SSNW                      : 2;  /**< \brief QSSN有效时间延长设置 */
    uint32_t RESERVED6                      :26;  /**< \brief 保留 */
} stc_qspi_cscr_field_t;

/**
 * \brief QSPI 格式控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t AWSL                      : 2;  /**< \brief 地址宽度选择 */
    __IO uint32_t FOUR_BIC                  : 1;  /**< \brief 4字节地址读取指令代码选择 */
    uint32_t RESERVED3                      : 1;  /**< \brief 保留 */
    __IO uint32_t SSNHD                     : 1;  /**< \brief QSSN信号释放时间延迟设置 */
    __IO uint32_t SSNLD                     : 1;  /**< \brief QSSN信号输出时间延迟设置 */
    __IO uint32_t WPOL                      : 1;  /**< \brief WP管脚输出电平设置 */
    uint32_t RESERVED7                      : 1;  /**< \brief 保留 */
    __IO uint32_t DMCYCN                    : 4;  /**< \brief 虚拟周期设置 */
    uint32_t RESERVED12                     : 3;  /**< \brief 保留 */
    __IO uint32_t DUTY                      : 1;  /**< \brief 占空比补正 */
    uint32_t RESERVED16                     :16;  /**< \brief 保留 */
} stc_qspi_fcr_field_t;

/**
 * \brief QSPI 状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t BUSY                      : 1;  /**< \brief 总线忙标志 */
    uint32_t RESERVED1                      : 5;  /**< \brief 保留 */
    __IO uint32_t XIPF                      : 1;  /**< \brief XIP模式标志 */
    __IO uint32_t RAER                      : 1;  /**< \brief ROM访问错误标志 */
    __IO uint32_t PFNUM                     : 5;  /**< \brief 预读取缓冲区已存储数据字节数 */
    uint32_t RESERVED13                     : 1;  /**< \brief 保留 */
    __IO uint32_t PFFUL                     : 1;  /**< \brief 预读取缓冲区状态 */
    __IO uint32_t PFAN                      : 1;  /**< \brief 预读取动作状态 */
    uint32_t RESERVED16                     :16;  /**< \brief 保留 */
} stc_qspi_sr_field_t;

/**
 * \brief QSPI 直接通信指令寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DCOM                      : 8;  /**< \brief 直接通信模式指令 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_qspi_dcom_field_t;

/**
 * \brief QSPI 指令代码寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t RIC                       : 8;  /**< \brief 替换指令代码 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_qspi_ccmd_field_t;

/**
 * \brief QSPI XIP模式代码寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XIPMC                     : 8;  /**< \brief XIP模式代码 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_qspi_xcmd_field_t;

/**
 * \brief QSPI 系统配置寄存器位域结构体
 */
typedef struct
{
    uint32_t RESERVED0                      : 7;  /**< \brief 保留 */
    __IO uint32_t RAERCLR                   : 1;  /**< \brief RAER清除 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_qspi_sr2_field_t;

/**
 * \brief QSPI 外部扩展地址寄存器位域结构体
 */
typedef struct
{
    uint32_t RESERVED0                      :26;
    __IO uint32_t EXADR                     : 6;
} stc_qspi_exar_field_t;

typedef struct amhw_hc32f460_qspi {
    union
    {
        __IO uint32_t CR;         /**< \brief QSPI控制寄存器 */
        stc_qspi_cr_field_t CR_f;
    };
    union
    {
        __IO uint32_t CSCR;       /**< \brief QSPI片选控制寄存器 */
        stc_qspi_cscr_field_t CSCR_f;
    };
    union
    {
        __IO uint32_t FCR;        /**< \brief QSPI格式控制寄存器 */
        stc_qspi_fcr_field_t FCR_f;
    };
    union
    {
        __IO uint32_t SR;         /**< \brief QSPI状态寄存器 */
        stc_qspi_sr_field_t SR_f;
    };
    union
    {
        __IO uint32_t DCOM;        /**< \brief QSPI指令代码寄存器 */
        stc_qspi_dcom_field_t DCOM_f;
    };
    union
    {
        __IO uint32_t CCMD;        /**< \brief QSPI直接通信指令寄存器 */
        stc_qspi_ccmd_field_t CCMD_f;
    };
    union
    {
        __IO uint32_t XCMD;         /**< \brief XIP模式代码寄存器 */
        stc_qspi_xcmd_field_t XCMD_f;
    };
    uint8_t RESERVED7[8];
    union
    {
        __IO uint32_t SR2;          /**< \brief XIP系统配置寄存器 */
        stc_qspi_sr2_field_t SR2_f;
    };
    uint8_t RESERVED8[2012];
    union
    {
        __IO uint32_t EXAR;          /**< \brief 外部扩展地址寄存器 */
        stc_qspi_exar_field_t EXAR_f;
    };
} amhw_hc32f460_qspi_t;

/*******************************************************************************
  defines
*******************************************************************************/
/**
 * \brief QSPI protocol
 * @{
 */
#define AMHW_HC32F460_QSPI_PROTOCOL_EXTEND      0  /**< \brief extend spi */
#define AMHW_HC32F460_QSPI_PROTOCOL_TWOWIRES    1  /**< \brief two wires spi */
#define AMHW_HC32F460_QSPI_PROTOCOL_FOURWIRES   2  /**< \brief four wires spi */
/** @} */

/**
 * \brief QSPI  spi mode
 * @{
 */
#define AMHW_HC32F460_QSPI_SPI_MODE0      0  /**< \brief spi mode 0 */
#define AMHW_HC32F460_QSPI_SPI_MODE3      1  /**< \brief spi mode 3 */
/** @} */

/**
 * \brief QSPI bus mode
 * @{
 */
#define AMHW_HC32F460_QSPI_ROM_ACCESS      0  /**< \brief rom access mode*/
#define AMHW_HC32F460_QSPI_DIRECT_ACCESS   1  /**< \brief direct access mode */
/** @} */

/**
 * \brief QSPI prefetch config
 * @{
 */
#define AMHW_HC32F460_QSPI_PREFETCH_STOP_COMPLETE      0  /**< \brief stop after prefetch data complete*/
#define AMHW_HC32F460_QSPI_PREFETCH_STOP_IMMEDIATELY   1  /**< \brief immediately stop prefetch */
/** @} */

/**
 * \brief QSPI read mode
 * @{
 */
#define AMHW_HC32F460_QSPI_READ_MODE_STANDARD         0  /**< \brief standard read */
#define AMHW_HC32F460_QSPI_READ_MODE_FAST             1  /**< \brief fast read */
#define AMHW_HC32F460_QSPI_READ_MODE_TWOWIRES_OUTPUT  2  /**< \brief two wires output fast read */
#define AMHW_HC32F460_QSPI_READ_MODE_TWOWIRES_IO      3  /**< \brief two wires io fast read */
#define AMHW_HC32F460_QSPI_READ_MODE_FOURWIRES_OUTPUT 4  /**< \brief four wires output fast read */
#define AMHW_HC32F460_QSPI_READ_MODE_FOURWIRES_IO     5  /**< \brief four wires io fast read */
#define AMHW_HC32F460_QSPI_READ_MODE_CUSTOM_STANDARD  6  /**< \brief custom standard read */
#define AMHW_HC32F460_QSPI_READ_MODE_CUSTOM_FAST      7  /**< \brief custom fast read */

/** @} */



typedef enum amhw_hc32f460_qspi_clk_div {
    AMHW_HC32F460_QSPI_HCLK_DIV2  = 0u,                     /**< \brief Clock source: HCLK/2 */
    AMHW_HC32F460_QSPI_HCLK_DIV3  = 2u,                     /**< \brief Clock source: HCLK/3 */
    AMHW_HC32F460_QSPI_HCLK_DIV4  = 3u,                     /**< \brief Clock source: HCLK/4 */
    AMHW_HC32F460_QSPI_HCLK_DIV5  = 4u,                     /**< \brief Clock source: HCLK/5 */
    AMHW_HC32F460_QSPI_HCLK_DIV6  = 5u,                     /**< \brief Clock source: HCLK/6 */
    AMHW_HC32F460_QSPI_HCLK_DIV7  = 6u,                     /**< \brief Clock source: HCLK/7 */
    AMHW_HC32F460_QSPI_HCLK_DIV8  = 7u,                     /**< \brief Clock source: HCLK/8 */
    AMHW_HC32F460_QSPI_HCLK_DIV9  = 8u,                     /**< \brief Clock source: HCLK/9 */
    AMHW_HC32F460_QSPI_HCLK_DIV10 = 9u,                     /**< \brief Clock source: HCLK/10 */
    AMHW_HC32F460_QSPI_HCLK_DIV11 = 10u,                    /**< \brief Clock source: HCLK/11 */
    AMHW_HC32F460_QSPI_HCLK_DIV12 = 11u,                    /**< \brief Clock source: HCLK/12 */
    AMHW_HC32F460_QSPI_HCLK_DIV13 = 12u,                    /**< \brief Clock source: HCLK/13 */
    AMHW_HC32F460_QSPI_HCLK_DIV14 = 13u,                    /**< \brief Clock source: HCLK/14 */
    AMHW_HC32F460_QSPI_HCLK_DIV15 = 14u,                    /**< \brief Clock source: HCLK/15 */
    AMHW_HC32F460_QSPI_HCLK_DIV16 = 15u,                    /**< \brief Clock source: HCLK/16 */
    AMHW_HC32F460_QSPI_HCLK_DIV17 = 16u,                    /**< \brief Clock source: HCLK/17 */
    AMHW_HC32F460_QSPI_HCLK_DIV18 = 17u,                    /**< \brief Clock source: HCLK/18 */
    AMHW_HC32F460_QSPI_HCLK_DIV19 = 18u,                    /**< \brief Clock source: HCLK/19 */
    AMHW_HC32F460_QSPI_HCLK_DIV20 = 19u,                    /**< \brief Clock source: HCLK/20 */
    AMHW_HC32F460_QSPI_HCLK_DIV21 = 20u,                    /**< \brief Clock source: HCLK/21 */
    AMHW_HC32F460_QSPI_HCLK_DIV22 = 21u,                    /**< \brief Clock source: HCLK/22 */
    AMHW_HC32F460_QSPI_HCLK_DIV23 = 22u,                    /**< \brief Clock source: HCLK/23 */
    AMHW_HC32F460_QSPI_HCLK_DIV24 = 23u,                    /**< \brief Clock source: HCLK/24 */
    AMHW_HC32F460_QSPI_HCLK_DIV25 = 24u,                    /**< \brief Clock source: HCLK/25 */
    AMHW_HC32F460_QSPI_HCLK_DIV26 = 25u,                    /**< \brief Clock source: HCLK/26 */
    AMHW_HC32F460_QSPI_HCLK_DIV27 = 26u,                    /**< \brief Clock source: HCLK/27 */
    AMHW_HC32F460_QSPI_HCLK_DIV28 = 27u,                    /**< \brief Clock source: HCLK/28 */
    AMHW_HC32F460_QSPI_HCLK_DIV29 = 28u,                    /**< \brief Clock source: HCLK/29 */
    AMHW_HC32F460_QSPI_HCLK_DIV30 = 29u,                    /**< \brief Clock source: HCLK/30 */
    AMHW_HC32F460_QSPI_HCLK_DIV31 = 30u,                    /**< \brief Clock source: HCLK/31 */
    AMHW_HC32F460_QSPI_HCLK_DIV32 = 31u,                    /**< \brief Clock source: HCLK/32 */
    AMHW_HC32F460_QSPI_HCLK_DIV33 = 32u,                    /**< \brief Clock source: HCLK/33 */
    AMHW_HC32F460_QSPI_HCLK_DIV34 = 33u,                    /**< \brief Clock source: HCLK/34 */
    AMHW_HC32F460_QSPI_HCLK_DIV35 = 34u,                    /**< \brief Clock source: HCLK/35 */
    AMHW_HC32F460_QSPI_HCLK_DIV36 = 35u,                    /**< \brief Clock source: HCLK/36 */
    AMHW_HC32F460_QSPI_HCLK_DIV37 = 36u,                    /**< \brief Clock source: HCLK/37 */
    AMHW_HC32F460_QSPI_HCLK_DIV38 = 37u,                    /**< \brief Clock source: HCLK/38 */
    AMHW_HC32F460_QSPI_HCLK_DIV39 = 38u,                    /**< \brief Clock source: HCLK/39 */
    AMHW_HC32F460_QSPI_HCLK_DIV40 = 39u,                    /**< \brief Clock source: HCLK/40 */
    AMHW_HC32F460_QSPI_HCLK_DIV41 = 40u,                    /**< \brief Clock source: HCLK/41 */
    AMHW_HC32F460_QSPI_HCLK_DIV42 = 41u,                    /**< \brief Clock source: HCLK/42 */
    AMHW_HC32F460_QSPI_HCLK_DIV43 = 42u,                    /**< \brief Clock source: HCLK/43 */
    AMHW_HC32F460_QSPI_HCLK_DIV44 = 43u,                    /**< \brief Clock source: HCLK/44 */
    AMHW_HC32F460_QSPI_HCLK_DIV45 = 44u,                    /**< \brief Clock source: HCLK/45 */
    AMHW_HC32F460_QSPI_HCLK_DIV46 = 45u,                    /**< \brief Clock source: HCLK/46 */
    AMHW_HC32F460_QSPI_HCLK_DIV47 = 46u,                    /**< \brief Clock source: HCLK/47 */
    AMHW_HC32F460_QSPI_HCLK_DIV48 = 47u,                    /**< \brief Clock source: HCLK/48 */
    AMHW_HC32F460_QSPI_HCLK_DIV49 = 48u,                    /**< \brief Clock source: HCLK/49 */
    AMHW_HC32F460_QSPI_HCLK_DIV50 = 49u,                    /**< \brief Clock source: HCLK/50 */
    AMHW_HC32F460_QSPI_HCLK_DIV51 = 50u,                    /**< \brief Clock source: HCLK/51 */
    AMHW_HC32F460_QSPI_HCLK_DIV52 = 51u,                    /**< \brief Clock source: HCLK/52 */
    AMHW_HC32F460_QSPI_HCLK_DIV53 = 52u,                    /**< \brief Clock source: HCLK/53 */
    AMHW_HC32F460_QSPI_HCLK_DIV54 = 53u,                    /**< \brief Clock source: HCLK/54 */
    AMHW_HC32F460_QSPI_HCLK_DIV55 = 54u,                    /**< \brief Clock source: HCLK/55 */
    AMHW_HC32F460_QSPI_HCLK_DIV56 = 55u,                    /**< \brief Clock source: HCLK/56 */
    AMHW_HC32F460_QSPI_HCLK_DIV57 = 56u,                    /**< \brief Clock source: HCLK/57 */
    AMHW_HC32F460_QSPI_HCLK_DIV58 = 57u,                    /**< \brief Clock source: HCLK/58 */
    AMHW_HC32F460_QSPI_HCLK_DIV59 = 58u,                    /**< \brief Clock source: HCLK/59 */
    AMHW_HC32F460_QSPI_HCLK_DIV60 = 59u,                    /**< \brief Clock source: HCLK/60 */
    AMHW_HC32F460_QSPI_HCLK_DIV61 = 60u,                    /**< \brief Clock source: HCLK/61 */
    AMHW_HC32F460_QSPI_HCLK_DIV62 = 61u,                    /**< \brief Clock source: HCLK/62 */
    AMHW_HC32F460_QSPI_HCLK_DIV63 = 62u,                    /**< \brief Clock source: HCLK/63 */
    AMHW_HC32F460_QSPI_HCLK_DIV64 = 63u,                    /**< \brief Clock source: HCLK/64 */
} amhw_hc32f460_qspi_clk_div_t;


/**
 * \brief 设置基准时钟
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] div       : 串行接口基准时钟
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_clk_div_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                    uint32_t              div)
{
    p_hw_qspi->CR_f.DIV = div;
}

/**
 * \brief 数据接收阶段SPI协议选择
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : 协议标志： AMHW_HC32F460_QSPI_PROTOCOL_EXTEND
 *                                AMHW_HC32F460_QSPI_PROTOCOL_TWOWIRES
 *                                AMHW_HC32F460_QSPI_PROTOCOL_FOURWIRES
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_dprsl_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                  uint8_t               flag)
{
    p_hw_qspi->CR_f.DPRSL = flag;
}

/**
 * \brief 地址发送阶段SPI协议选择
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : 协议标志： AMHW_HC32F460_QSPI_PROTOCOL_EXTEND
 *                                AMHW_HC32F460_QSPI_PROTOCOL_TWOWIRES
 *                                AMHW_HC32F460_QSPI_PROTOCOL_FOURWIRES
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_aprsl_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                  uint8_t               flag)
{
    p_hw_qspi->CR_f.APRSL = flag;
}

/**
 * \brief 指令发送阶段SPI协议选择
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : 协议标志： AMHW_HC32F460_QSPI_PROTOCOL_EXTEND
 *                                AMHW_HC32F460_QSPI_PROTOCOL_TWOWIRES
 *                                AMHW_HC32F460_QSPI_PROTOCOL_FOURWIRES
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_iprsl_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                  uint8_t               flag)
{
    p_hw_qspi->CR_f.IPRSL = flag;
}

/**
 * \brief SPI模式选择
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] mode      : 模式标志： AMHW_HC32F460_QSPI_SPI_MODE0
 *                                AMHW_HC32F460_QSPI_SPI_MODE3
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_mode_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                  uint8_t              mode)
{
    p_hw_qspi->CR_f.SPIMD3 = mode;
}

/**
 * \brief XIP模式设置
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] mode      : AM_TRUE(XIP模式许可)/AM_FALSE(XIP模式禁止)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_xip_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                am_bool_t             flag)
{
    p_hw_qspi->CR_f.XIPE = flag;
}

/**
 * \brief 直接通信设置
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] mode      : AM_TRUE(直接通信模式)/AM_FALSE(ROM访问模式)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_dcome_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                  am_bool_t             flag)
{
    p_hw_qspi->CR_f.DCOME = flag;
}

/**
 * \brief 预读取即时停止许可
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : AM_TRUE(当前预读取动作即时中止)/AM_FALSE(当前预读取动作在字节边界中止)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_pfsae_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                  am_bool_t             flag)
{
    p_hw_qspi->CR_f.PFSAE = flag;
}

/**
 * \brief 预读取许可设置
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : AM_TRUE(预读取功能有效)/AM_FALSE(预读取功能无效)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_pfe_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                am_bool_t             flag)
{
    p_hw_qspi->CR_f.PFE = flag;
}


/**
 * \brief 读取模式设置
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : AMHW_HC32F460_QSPI_READ_MODE_STANDARD
 *                        AMHW_HC32F460_QSPI_READ_MODE_FAST
 *                        AMHW_HC32F460_QSPI_READ_MODE_TWOWIRES_OUTPUT
 *                        AMHW_HC32F460_QSPI_READ_MODE_TWOWIRES_IO
 *                        AMHW_HC32F460_QSPI_READ_MODE_FOURWIRES_OUTPUT
 *                        AMHW_HC32F460_QSPI_READ_MODE_FOURWIRES_IO
 *                        AMHW_HC32F460_QSPI_READ_MODE_CUSTOM_STANDARD
 *                        AMHW_HC32F460_QSPI_READ_MODE_CUSTOM_FAST
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_readmode_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                     uint8_t               flag)
{
    p_hw_qspi->CR_f.MDSEL = flag;
}


/**
 * \brief QSPI valid extend time
 * @{
 */
#define AMHW_HC32F460_QSPI_QSSN_VALID_EXTEND_NOT     0  /** <brief Don't extend QSSN valid time */
#define AMHW_HC32F460_QSPI_QSSN_VALID_EXTEND_32SCK   1  /** <brief QSSN valid time extend 32 QSCK cycles */
#define AMHW_HC32F460_QSPI_QSSN_VALID_EXTEND_128SCK  2  /** <brief QSSN valid time extend 138 QSCK cycles */
#define AMHW_HC32F460_QSPI_QSSN_VALID_EXTEND_FOREVER 3  /** <brief QSSN valid time extend to ever */
/** @} */
/**
 * \brief QSPI QSSN minimum interval time enumeration
 * @{
 */
typedef enum amhw_hc32f460_qspi_qssn_interval_time {
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK1  = 0u,    /** <brief QSSN signal min interval time 1 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK2  = 1u,    /** <brief QSSN signal min interval time 2 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK3  = 2u,    /** <brief QSSN signal min interval time 3 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK4  = 3u,    /** <brief QSSN signal min interval time 4 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK5  = 4u,    /** <brief QSSN signal min interval time 5 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK6  = 5u,    /** <brief QSSN signal min interval time 6 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK7  = 6u,    /** <brief QSSN signal min interval time 7 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK8  = 7u,    /** <brief QSSN signal min interval time 8 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK9  = 8u,    /** <brief QSSN signal min interval time 9 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK10 = 9u,    /** <brief QSSN signal min interval time 10 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK11 = 10u,   /** <brief QSSN signal min interval time 11 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK12 = 11u,   /** <brief QSSN signal min interval time 12 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK13 = 12u,   /** <brief QSSN signal min interval time 13 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK14 = 13u,   /** <brief QSSN signal min interval time 14 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK15 = 14u,   /** <brief QSSN signal min interval time 15 QSCK */
    AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK16 = 15u,   /** <brief QSSN signal min interval time 16 QSCK */
} amhw_hc32f460_qspi_qssn_interval_time_t;
/** @} */

/**
 * \brief QSSN有效时间延长设置
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : AMHW_HC32F460_QSPI_QSSN_VALID_EXTEND_NOT
 *                        AMHW_HC32F460_QSPI_QSSN_VALID_EXTEND_32SCK   1
 *                        AMHW_HC32F460_QSPI_QSSN_VALID_EXTEND_128SCK  2
 *                        AMHW_HC32F460_QSPI_QSSN_VALID_EXTEND_FOREVER 3
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_cscr_ssnw_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                      uint8_t               flag)
{
    p_hw_qspi->CSCR_f.SSNW = flag;
}

/**
 * \brief QSSN最小无效时间设置
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : QSSN信号最小无效时间(amhw_hc32f460_qspi_qssn_interval_time_t)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_cscr_sshw_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                      uint8_t               num)
{
    p_hw_qspi->CSCR_f.SSHW = num;
}
/**
 * \brief QSPI duty cycle correction
 * @{
 */
#define AMHW_HC32F460_QSPI_QSCK_DUTY_CORR_NOT        0  /** <brief Don't correction duty cycle */
#define AMHW_HC32F460_QSPI_QSSN_DUTY_CORR_HALFHCLK   1  /** <brief qsck's rising edge delay 0.5 HCLK cycle when qsck select hclk is odd */
/** @} */

/**
 * \brief 占空比补正使能
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : AM_TRUE(将QSCK的上升沿滞后0.5个HCLK周期)/AM_FALSE(不进行占空比补正)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_qsfcr_duty_enable(amhw_hc32f460_qspi_t *p_hw_qspi,
                                          am_bool_t             flag)
{
    p_hw_qspi->FCR_f.DUTY = flag;
}

/**
 * \brief QSPI virtual period enumeration
 * @{
 */
typedef enum amhw_hc32f460_qspi_virtual_period
{
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK3  = 0u,    /** <brief Virtual period select 3 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK4  = 1u,    /** <brief Virtual period select 4 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK5  = 2u,    /** <brief Virtual period select 5 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK6  = 3u,    /** <brief Virtual period select 6 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK7  = 4u,    /** <brief Virtual period select 7 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK8  = 5u,    /** <brief Virtual period select 8 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK9  = 6u,    /** <brief Virtual period select 9 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK10 = 7u,    /** <brief Virtual period select 10 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK11 = 8u,    /** <brief Virtual period select 11 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK12 = 9u,    /** <brief Virtual period select 12 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK13 = 10u,   /** <brief Virtual period select 13 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK14 = 11u,   /** <brief Virtual period select 14 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK15 = 12u,   /** <brief Virtual period select 15 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK16 = 13u,   /** <brief Virtual period select 16 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK17 = 14u,   /** <brief Virtual period select 17 QSCK */
    AMHW_QSPI_VIRTUAL_PRTIOD_QSCK18 = 15u,   /** <brief Virtual period select 18 QSCK */
} amhw_hc32f460_qspi_virtual_period_t;
/** @} */

/**
 * \brief 虚拟周期设置
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : 使用快速读指令时虚拟周期数量(amhw_hc32f460_qspi_virtual_period_t)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_qsfcr_virtual_cycle_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                                uint8_t               flag)
{
    p_hw_qspi->FCR_f.DMCYCN = flag;
}

/**
 * \brief QSPI wp pin output level
 * @{
 */
#define AMHW_HC32F460_QSPI_WP_PINOUTPUT_LOW    (0)
#define AMHW_HC32F460_QSPI_WP_PINOUTPUT_HIGH   (1)
/** @} */

/**
 * \brief wp管脚输出电平设置
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] level     : AMHW_HC32F460_QSPI_WP_PINOUTPUT_LOW
 *                        AMHW_HC32F460_QSPI_WP_PINOUTPUT_HIGH
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_qsfcr_wp_pinlevel_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                              uint8_t               level)
{
    p_hw_qspi->FCR_f.WPOL = level;
}

/**
 * \brief QSPI qssn setup delay time
 * @{
 */
#define AMHW_HC32F460_QSPI_QSSN_SETUP_DELAY_HALFQSCK       (0)
#define AMHW_HC32F460_QSPI_QSSN_SETUP_DELAY_1DOTHALFQSCK   (1)
/** @} */

/**
 * \brief QSSN信号输出时间延迟设置
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : AMHW_HC32F460_QSPI_QSSN_SETUP_DELAY_HALFQSCK
 *                        AMHW_HC32F460_QSPI_QSSN_SETUP_DELAY_1DOTHALFQSCK
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_qsfcr_ssnld_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                        uint8_t               flag)
{
    p_hw_qspi->FCR_f.SSNLD = flag;
}


/**
 * \brief QSPI qssn hold delay time
 * @{
 */
#define AMHW_HC32F460_QSPI_QSSN_HOLD_DELAY_HALFQSCK       (0)
#define AMHW_HC32F460_QSPI_QSSN_HOLD_DELAY_1DOTHALFQSCK   (1)
/** @} */

/**
 * \brief QSSN信号释放时间延迟设置
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : AMHW_HC32F460_QSPI_QSSN_HOLD_DELAY_HALFQSCK
 *                        AMHW_HC32F460_QSPI_QSSN_HOLD_DELAY_1DOTHALFQSCK
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_qsfcr_ssnhd_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                        uint8_t               flag)
{
    p_hw_qspi->FCR_f.SSNHD = flag;
}

/**
 * \brief 4字节地址读指令选择
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : AM_TRUE(使用4字节地址读指令代码)/AM_FALSE(不使用4字节地址读指令代码)
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_qsfcr_4byte_addr_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                             am_bool_t             flag)
{
    p_hw_qspi->FCR_f.FOUR_BIC = flag;
}

/**
 * \brief QSPI address width
 * @{
 */
#define AMHW_HC32F460_QSPI_ADDRESS_BYTE_ONE     (0)
#define AMHW_HC32F460_QSPI_ADDRESS_BYTE_TWO     (1)
#define AMHW_HC32F460_QSPI_ADDRESS_BYTE_THREE   (2)
#define AMHW_HC32F460_QSPI_ADDRESS_BYTE_FOUR    (3)
/** @} */

/**
 * \brief 地址宽度选择
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] flag      : AMHW_HC32F460_QSPI_ADDRESS_BYTE_ONE
 *                        AMHW_HC32F460_QSPI_ADDRESS_BYTE_TWO
 *                        AMHW_HC32F460_QSPI_ADDRESS_BYTE_THREE
 *                        AMHW_HC32F460_QSPI_ADDRESS_BYTE_FOUR
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_qsfcr_addr_width_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                             uint8_t               flag)
{
    p_hw_qspi->FCR_f.AWSL = flag;
}


/**
 * \brief 读取QSPI状态
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 *
 * \return ROM访问错误标志、XIP模式标志、总线忙标志
 */
am_static_inline
uint32_t amhw_hc32f460_qspi_qssr_status_get(amhw_hc32f460_qspi_t *p_hw_qspi)
{
    return p_hw_qspi->SR & 0xFF;
}

/**
 * \brief QSPI 4-byte instruction mode using instruction set
 * @{
 */
#define AMHW_HC32F460_QSPI_4BINSTR_STANDARD_READ                      (0x13u)
#define AMHW_HC32F460_QSPI_4BINSTR_FAST_READ                          (0x0Cu)
#define AMHW_HC32F460_QSPI_4BINSTR_TWO_WIRES_OUTPUT_READ              (0x3Cu)
#define AMHW_HC32F460_QSPI_4BINSTR_TWO_WIRES_IO_READ                  (0xBCu)
#define AMHW_HC32F460_QSPI_4BINSTR_FOUR_WIRES_OUTPUT_READ             (0x6Cu)
#define AMHW_HC32F460_QSPI_4BINSTR_FOUR_WIRES_IO_READ                 (0xECu)
#define AMHW_HC32F460_QSPI_4BINSTR_EXIT_4BINSTR_MODE                  (0xB7u)
/** @} */

/**
 * \brief QSPI 3-byte instruction mode using instruction set
 * @{
 */
#define AMHW_HC32F460_QSPI_3BINSTR_STANDARD_READ                      (0x03u)
#define AMHW_HC32F460_QSPI_3BINSTR_FAST_READ                          (0x0Bu)
#define AMHW_HC32F460_QSPI_3BINSTR_TWO_WIRES_OUTPUT_READ              (0x3Bu)
#define AMHW_HC32F460_QSPI_3BINSTR_TWO_WIRES_IO_READ                  (0xBBu)
#define AMHW_HC32F460_QSPI_3BINSTR_FOUR_WIRES_OUTPUT_READ             (0x6Bu)
#define AMHW_HC32F460_QSPI_3BINSTR_FOUR_WIRES_IO_READ                 (0xEBu)
#define AMHW_HC32F460_QSPI_3BINSTR_ENTER_4BINSTR_MODE                 (0xE9u)
/** @} */

/**
 * \brief QSPI General instruction set
 * @{
 */
#define AMHW_HC32F460_QSPI_WRITE_MODE_ENABLE                          0x06u
/** @} */


/**
 * \brief 设置指令代码
 *
 * \param[in] p_hw_qspi : 指向QSPI寄存器块的指针
 * \param[in] cmd       : 指令代码
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_romaccess_cmd_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                          uint8_t               cmd)
{
    p_hw_qspi->CCMD_f.RIC = cmd;
}

/**
 * \brief 直接通信模式写数据
 *
 * \param[in] p_hw_qspi  : 指向QSPI寄存器块的指针
 * \param[in] data       : 要写入的数据
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_directcom_write(amhw_hc32f460_qspi_t *p_hw_qspi,
                                        uint8_t               data)
{
    p_hw_qspi->DCOM = data;
}

/**
 * \brief 直接通信模式读数据
 *
 * \param[in] p_hw_qspi  : 指向QSPI寄存器块的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_hc32f460_qspi_directcom_read(amhw_hc32f460_qspi_t *p_hw_qspi)
{
    return (uint8_t)p_hw_qspi->DCOM;
}

/**
 * \brief 设置XIP模式代码
 *
 * \param[in] p_hw_qspi  : 指向QSPI寄存器块的指针
 * \param[in] xip_mode   : xip模式码
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_xipmode_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                    uint8_t               xip_mode)
{
    p_hw_qspi->XCMD_f.XIPMC = xip_mode;
}

/**
 * \brief 清除raer标志
 *
 * \param[in] p_hw_qspi  : 指向QSPI寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_raer_clr(amhw_hc32f460_qspi_t *p_hw_qspi)
{
    p_hw_qspi->SR2_f.RAERCLR = 1;
}

/**
 * \brief 设置外部扩展地址寄存器
 *
 * \param[in] p_hw_qspi  : 指向QSPI寄存器块的指针
 * \param[in] addr       : 外部扩展地址代码
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_qspi_extended_addr_set(amhw_hc32f460_qspi_t *p_hw_qspi,
                                          uint8_t               addr)
{
    p_hw_qspi->EXAR_f.EXADR = addr;
}


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
 * @} amhw_hc32f460_if_qspi
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_QSPI_H */

/* end of file */
