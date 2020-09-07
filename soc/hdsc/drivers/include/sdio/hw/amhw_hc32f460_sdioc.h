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
 * \brief SDIOC 硬件操作接口
 * \internal
 * \par Modification history
 * - 1.00 17-04-28  cds, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32F460_SDIO_H
#define __AMHW_HC32F460_SDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"


/**
 * \addtogroup amhw_hc32f460_sdio
 * \copydoc amhw_hc32f460_sdio.h
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
 * \brief sdio structure of register
 */

/**
 * \brief SDIOC 数据块长度寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t TBS                       :12;  /**< \brief 数据块长度 */
    uint16_t RESERVED12                     : 4;  /**< \brief 保留 */
} stc_sdioc_blksize_field_t;

/**
 * \brief SDIOC 传输模式寄存器位域结构体
 */
typedef struct
{
    uint16_t RESERVED0                      : 1;  /**< \brief 保留 */
    __IO uint16_t BCE                       : 1;  /**< \brief 块计数使能 */
    __IO uint16_t ATCEN                     : 2;  /**< \brief 自动命令使能 */
    __IO uint16_t DDIR                      : 1;  /**< \brief 数据传输方向 */
    __IO uint16_t MULB                      : 1;  /**< \brief 多数据块 */
    uint16_t RESERVED6                      :10;  /**< \brief 保留 */
} stc_sdioc_transmode_field_t;

/**
 * \brief SDIOC 命令寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t RESTYP                    : 2;  /**< \brief 应答类型 */
    uint16_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint16_t CCE                       : 1;  /**< \brief CRC检查 */
    __IO uint16_t ICE                       : 1;  /**< \brief ICE编号检查 */
    __IO uint16_t DAT                       : 1;  /**< \brief 带数据命令 */
    __IO uint16_t TYP                       : 2;  /**< \brief 命令类型 */
    __IO uint16_t IDX                       : 6;  /**< \brief 命令编号 */
    uint16_t RESERVED14                     : 2;  /**< \brief 保留 */
} stc_sdioc_cmd_field_t;

/**
 * \brief SDIOC 主机状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CIC                       : 1;  /**< \brief 命令抑制 */
    __IO uint32_t CID                       : 1;  /**< \brief 带数据命令抑制 */
    __IO uint32_t DA                        : 1;  /**< \brief 数据线传输状态 */
    uint32_t RESERVED3                      : 5;  /**< \brief 保留 */
    __IO uint32_t WTA                       : 1;  /**< \brief 写操作状态 */
    __IO uint32_t RTA                       : 1;  /**< \brief 读操作状态 */
    __IO uint32_t BWE                       : 1;  /**< \brief 数据缓冲空 */
    __IO uint32_t BRE                       : 1;  /**< \brief 数据缓冲满 */
    uint32_t RESERVED12                     : 4;  /**< \brief 保留 */
    __IO uint32_t CIN                       : 1;  /**< \brief 设备插入状态 */
    __IO uint32_t CSS                       : 1;  /**< \brief 设备稳定状态 */
    __IO uint32_t CDL                       : 1;  /**< \brief 卡识别线状态 */
    __IO uint32_t WPL                       : 1;  /**< \brief 写保护线状态 */
    __IO uint32_t DATL                      : 4;  /**< \brief 数据线状态 */
    __IO uint32_t CMDL                      : 1;  /**< \brief 命令线状态 */
    uint32_t RESERVED25                     : 7;  /**< \brief 保留 */
} stc_sdioc_pstat_field_t;

/**
 * \brief SDIOC 主机控制寄存器位域结构体
 */
typedef struct
{
    uint8_t RESERVED0                       : 1;  /**< \brief 保留 */
    __IO uint8_t DW                         : 1;  /**< \brief 数据位宽选择 */
    __IO uint8_t HSEN                       : 1;  /**< \brief 高速模式使能 */
    uint8_t RESERVED3                       : 2;  /**< \brief 保留 */
    __IO uint8_t EXDW                       : 1;  /**< \brief 扩展数据位宽 */
    __IO uint8_t CDTL                       : 1;  /**< \brief 卡识别测试信号状态 */
    __IO uint8_t CDSS                       : 1;  /**< \brief 卡识别线选择 */
} stc_sdioc_hostcon_field_t;

/**
 * \brief SDIOC 电源控制寄存器位域结构体
 */
typedef struct
{
    __IO uint8_t PWON                       : 1;  /**< \brief SDIOC使能 */
    uint8_t RESERVED1                       : 7;  /**< \brief 保留 */
} stc_sdioc_pwrcon_field_t;

/**
 * \brief SDIOC 数据块间隙控制寄存器位域结构体
 */
typedef struct
{
    __IO uint8_t SABGR                      : 1;  /**< \brief 数据块间隙停止传输 */
    __IO uint8_t CR                         : 1;  /**< \brief 继续传输 */
    __IO uint8_t RWC                        : 1;  /**< \brief RWC读等待控制 */
    __IO uint8_t IABG                       : 1;  /**< \brief 数据块间隙中断控制 */
    uint8_t RESERVED4                       : 4;  /**< \brief 保留 */
} stc_sdioc_blkgpcon_field_t;

/**
 * \brief SDIOC 时钟控制寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t ICE                       : 1;  /**< \brief 时钟使能 */
    uint16_t RESERVED1                      : 1;  /**< \brief 保留 */
    __IO uint16_t CE                        : 1;  /**< \brief SDIOx_CK输出控制 */
    uint16_t RESERVED3                      : 5;  /**< \brief 保留 */
    __IO uint16_t FS                        : 8;  /**< \brief 分频选择 */
} stc_sdioc_clkcon_field_t;

/**
 * \brief SDIOC 超时控制寄存器位域结构体
 */
typedef struct
{
    __IO uint8_t DTO                        : 4;  /**< \brief 数据超时时间 */
    uint8_t RESERVED4                       : 4;  /**< \brief 保留 */
} stc_sdioc_toutcon_field_t;

/**
 * \brief SDIOC 软件复位寄存器位域结构体
 */
typedef struct
{
    __IO uint8_t RSTA                       : 1;  /**< \brief 全部复位 */
    __IO uint8_t RSTC                       : 1;  /**< \brief 命令复位 */
    __IO uint8_t RSTD                       : 1;  /**< \brief 数据复位 */
    uint8_t RESERVED3                       : 5;  /**< \brief 保留 */
} stc_sdioc_sftrst_field_t;

/**
 * \brief SDIOC 普通中断状态寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t CC                        : 1;  /**< \brief 命令完成 */
    __IO uint16_t TC                        : 1;  /**< \brief 传输完成 */
    __IO uint16_t BGE                       : 1;  /**< \brief 数据块间隙停止传输 */
    uint16_t RESERVED3                      : 1;  /**< \brief 保留  */
    __IO uint16_t BWR                       : 1;  /**< \brief 缓冲器可写 */
    __IO uint16_t BRR                       : 1;  /**< \brief 缓冲器可读 */
    __IO uint16_t CIST                      : 1;  /**< \brief 卡插入 */
    __IO uint16_t CRM                       : 1;  /**< \brief 卡移除 */
    __IO uint16_t CINT                      : 1;  /**< \brief 卡中断 */
    uint16_t RESERVED9                      : 6;  /**< \brief 保留  */
    __IO uint16_t EI                        : 1;  /**< \brief 错误中断 */
} stc_sdioc_norintst_field_t;

/**
 * \brief SDIOC 错误中断状态寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t CTOE                      : 1;  /**< \brief 命令超时错误  */
    __IO uint16_t CCE                       : 1;  /**< \brief 命令CRC校验错误  */
    __IO uint16_t CEBE                      : 1;  /**< \brief 命令停止位错误  */
    __IO uint16_t CIE                       : 1;  /**< \brief 命令编号错误  */
    __IO uint16_t DTOE                      : 1;  /**< \brief 数据超时错误  */
    __IO uint16_t DCE                       : 1;  /**< \brief 数据CRC校验错误  */
    __IO uint16_t DEBE                      : 1;  /**< \brief 数据停止位错误  */
    uint16_t RESERVED7                      : 1;  /**< \brief 保留  */
    __IO uint16_t ACE                       : 1;  /**< \brief 自动发送命令错误  */
    uint16_t RESERVED9                      : 7;  /**< \brief 保留  */
} stc_sdioc_errintst_field_t;

/**
 * \brief SDIOC 普通中断状态使能寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t CCEN                      : 1;  /**< \brief 命令完成状态使能 */
    __IO uint16_t TCEN                      : 1;  /**< \brief 传输完成状态使能 */
    __IO uint16_t BGEEN                     : 1;  /**< \brief 数据块间隙停止传输状态使能 */
    uint16_t RESERVED3                      : 1;  /**< \brief 保留  */
    __IO uint16_t BWREN                     : 1;  /**< \brief 缓冲器可写状态使能 */
    __IO uint16_t BRREN                     : 1;  /**< \brief 缓冲器可读状态使能 */
    __IO uint16_t CISTEN                    : 1;  /**< \brief 卡插入状态使能 */
    __IO uint16_t CRMEN                     : 1;  /**< \brief 卡移除状态使能 */
    __IO uint16_t CINTEN                    : 1;  /**< \brief 卡中断状态使能 */
    uint16_t RESERVED9                      : 7;  /**< \brief 保留  */
} stc_sdioc_norintsten_field_t;

/**
 * \brief SDIOC 错误中断状态使能寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t CTOEEN                    : 1;  /**< \brief 命令超时错误状态使能  */
    __IO uint16_t CCEEN                     : 1;  /**< \brief 命令CRC校验错误状态使能  */
    __IO uint16_t CEBEEN                    : 1;  /**< \brief 命令停止错误状态使能  */
    __IO uint16_t CIEEN                     : 1;  /**< \brief 命令编号错误状态使能  */
    __IO uint16_t DTOEEN                    : 1;  /**< \brief 数据超时错误状态使能  */
    __IO uint16_t DCEEN                     : 1;  /**< \brief 数据CRC校验错误状态使能 */
    __IO uint16_t DEBEEN                    : 1;  /**< \brief 数据停止位错误状态使能  */
    uint16_t RESERVED7                      : 1;  /**< \brief 保留  */
    __IO uint16_t ACEEN                     : 1;  /**< \brief 自动发送命令错误状态使能   */
    uint16_t RESERVED9                      : 7;  /**< \brief 保留  */
} stc_sdioc_errintsten_field_t;

/**
 * \brief SDIOC 普通中断信号使能寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t CCSEN                     : 1;  /**< \brief 命令完成信号使能   */
    __IO uint16_t TCSEN                     : 1;  /**< \brief 传输完成信号使能   */
    __IO uint16_t BGESEN                    : 1;  /**< \brief 数据块间隙停止传输信号使能   */
    uint16_t RESERVED3                      : 1;  /**< \brief 保留  */
    __IO uint16_t BWRSEN                    : 1;  /**< \brief 缓冲器可写信号使能   */
    __IO uint16_t BRRSEN                    : 1;  /**< \brief 缓冲器可读信号使能   */
    __IO uint16_t CISTSEN                   : 1;  /**< \brief 卡插入信号使能   */
    __IO uint16_t CRMSEN                    : 1;  /**< \brief 卡移除信号使能   */
    __IO uint16_t CINTSEN                   : 1;  /**< \brief 自动发送命令错误状态使能   */
    uint16_t RESERVED9                      : 7;  /**< \brief 保留  */
} stc_sdioc_norintsgen_field_t;

/**
 * \brief SDIOC 错误中断信号使能寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t CTOESEN                   : 1;  /**< \brief 命令超时错误信号使能  */
    __IO uint16_t CCESEN                    : 1;  /**< \brief 命令CRC校验错误信号使能  */
    __IO uint16_t CEBESEN                   : 1;  /**< \brief 命令停止位错误信号使能  */
    __IO uint16_t CIESEN                    : 1;  /**< \brief 命令编号错误信号使能  */
    __IO uint16_t DTOESEN                   : 1;  /**< \brief 数据超时错误信号使能  */
    __IO uint16_t DCESEN                    : 1;  /**< \brief 数据CRC校验错误信号使能  */
    __IO uint16_t DEBESEN                   : 1;  /**< \brief 数据停止位错误信号使能  */
    uint16_t RESERVED7                      : 1;  /**< \brief 保留  */
    __IO uint16_t ACESEN                    : 1;  /**< \brief 自动发送命令错误信号使能  */
    uint16_t RESERVED9                      : 7;  /**< \brief 保留  */
} stc_sdioc_errintsgen_field_t;

/**
 * \brief SDIOC 自动命令错误状态寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t NE                        : 1;  /**< \brief 未执行错误  */
    __IO uint16_t TOE                       : 1;  /**< \brief 命令超时错误  */
    __IO uint16_t CE                        : 1;  /**< \brief 数据超时错误  */
    __IO uint16_t EBE                       : 1;  /**< \brief 停止位错误  */
    __IO uint16_t IE                        : 1;  /**< \brief 命令编号错误  */
    uint16_t RESERVED5                      : 2;  /**< \brief 保留  */
    __IO uint16_t CMDE                      : 1;  /**< \brief 未发送错误  */
    uint16_t RESERVED8                      : 8;  /**< \brief 保留  */
} stc_sdioc_atcerrst_field_t;

/**
 * \brief SDIOC 强制自动命令错误状态控制寄存器位域结构体
 */
typedef struct
{
    __IO uint16_t FNE                       : 1;  /**< \brief 强制未执行错误  */
    __IO uint16_t FTOE                      : 1;  /**< \brief 强制命令超时错误  */
    __IO uint16_t FCE                       : 1;  /**< \brief 强制数据超时错误  */
    __IO uint16_t FEBE                      : 1;  /**< \brief 强制停止位错误  */
    __IO uint16_t FIE                       : 1;  /**< \brief 强制命令编号错误  */
    uint16_t RESERVED5                      : 2;  /**< \brief 保留  */
    __IO uint16_t FCMDE                     : 1;  /**< \brief 强制未发送错误  */
    uint16_t RESERVED8                      : 8;  /**< \brief 保留  */
} stc_sdioc_fea_field_t;

typedef struct
{
    __IO uint16_t FCTOE                     : 1;  /**< \brief 强制命令超时错误  */
    __IO uint16_t FCCE                      : 1;  /**< \brief 强制命令CEC校验错误  */
    __IO uint16_t FCEBE                     : 1;  /**< \brief 强制命令停止位错误  */
    __IO uint16_t FCIE                      : 1;  /**< \brief 强制命令编号错误  */
    __IO uint16_t FDTOE                     : 1;  /**< \brief 强制数据超时错误  */
    __IO uint16_t FDCE                      : 1;  /**< \brief 强制数据CRC校验错误  */
    __IO uint16_t FDEBE                     : 1;  /**< \brief 强制数据停止位错误  */
    uint16_t RESERVED7                      : 1;  /**< \brief 保留  */
    __IO uint16_t FACE                      : 1;  /**< \brief 强制自动发送命令错误  */
    uint16_t RESERVED9                      : 7;  /**< \brief 保留  */
} stc_sdioc_fee_field_t;

typedef struct amhw_hc32f460_sdioc {
    uint8_t RESERVED0[4];                       /**< \brief 保留  */
    union
    {
        __IO uint16_t BLKSIZE;                  /**< \brief 数据块长度寄存器 */
        stc_sdioc_blksize_field_t BLKSIZE_f;
    };
    __IO uint16_t BLKCNT;                       /**< \brief 数据块计数寄存器 */
    __IO uint16_t ARG0;                         /**< \brief 参数0 */
    __IO uint16_t ARG1;                         /**< \brief 参数1 */
    union
    {
        __IO uint16_t TRANSMODE;                /**< \brief 传输模式 */
        stc_sdioc_transmode_field_t TRANSMODE_f;
    };
    union
    {
        __IO uint16_t CMD;                      /**< \brief 命令 */
        stc_sdioc_cmd_field_t CMD_f;
    };
    __IO uint16_t RESP0;                        /**< \brief 应答0 */
    __IO uint16_t RESP1;                        /**< \brief 应答1 */
    __IO uint16_t RESP2;                        /**< \brief 应答2 */
    __IO uint16_t RESP3;                        /**< \brief 应答3 */
    __IO uint16_t RESP4;                        /**< \brief 应答4 */
    __IO uint16_t RESP5;                        /**< \brief 应答5 */
    __IO uint16_t RESP6;                        /**< \brief 应答6 */
    __IO uint16_t RESP7;                        /**< \brief 应答7 */
    __IO uint16_t BUF0;                         /**< \brief 数据缓冲0 */
    __IO uint16_t BUF1;                         /**< \brief 数据缓冲1 */
    union
    {
        __IO uint32_t PSTAT;                    /**< \brief 主机状态 */
        stc_sdioc_pstat_field_t PSTAT_f;
    };
    union
    {
        __IO uint8_t HOSTCON;                    /**< \brief 主机控制 */
        stc_sdioc_hostcon_field_t HOSTCON_f;
    };
    union
    {
        __IO uint8_t PWRCON;                     /**< \brief 电源控制 */
        stc_sdioc_pwrcon_field_t PWRCON_f;
    };
    union
    {
        __IO uint8_t BLKGPCON;                   /**< \brief 数据块间隔控制 */
        stc_sdioc_blkgpcon_field_t BLKGPCON_f;
    };
    uint8_t RESERVED20[1];                       /**< \brief 保留 */
    union
    {
        __IO uint16_t CLKCON;                    /**< \brief 时钟控制 */
        stc_sdioc_clkcon_field_t CLKCON_f;
    };
    union
    {
        __IO uint8_t TOUTCON;                    /**< \brief 超时控制 */
        stc_sdioc_toutcon_field_t TOUTCON_f;
    };
    union
    {
        __IO uint8_t SFTRST;                     /**< \brief 软件复位 */
        stc_sdioc_sftrst_field_t SFTRST_f;
    };
    union
    {
        __IO uint16_t NORINTST;                  /**< \brief 普通中断状态 */
        stc_sdioc_norintst_field_t NORINTST_f;
    };
    union
    {
        __IO uint16_t ERRINTST;                  /**< \brief 错误中断状态 */
        stc_sdioc_errintst_field_t ERRINTST_f;
    };
    union
    {
        __IO uint16_t NORINTSTEN;                /**< \brief 普通中断状态使能 */
        stc_sdioc_norintsten_field_t NORINTSTEN_f;
    };
    union
    {
        __IO uint16_t ERRINTSTEN;                /**< \brief 错误中断状态使能 */
        stc_sdioc_errintsten_field_t ERRINTSTEN_f;
    };
    union
    {
        __IO uint16_t NORINTSGEN;                /**< \brief 普通中断信号使能 */
        stc_sdioc_norintsgen_field_t NORINTSGEN_f;
    };
    union
    {
        __IO uint16_t ERRINTSGEN;                /**< \brief 错误中断信号使能 */
        stc_sdioc_errintsgen_field_t ERRINTSGEN_f;
    };
    union
    {
        __IO uint16_t ATCERRST;                  /**< \brief 自动命令错误状态 */
        stc_sdioc_atcerrst_field_t ATCERRST_f;
    };
    uint8_t RESERVED30[18];                      /**< \brief 保留 */
    union
    {
        __IO uint16_t FEA;                       /**< \brief 强制自动命令错误状态控制*/
        stc_sdioc_fea_field_t FEA_f;
    };
    union
    {
        __IO uint16_t FEE;                       /**< \brief 强制错误状态控制*/
        stc_sdioc_fee_field_t FEE_f;
    };
} amhw_hc32f460_sdioc_t;

#define AMHW_HC32F460_SDIOC1_INT_VSSEL_BITS_MASK    (0x7)
#define AMHW_HC32F460_SDIOC2_INT_VSSEL_BITS_MASK    (0x7 << 3)

/**
 * \brief 数据块长度设置函数
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] size      : 数据块长度（1~512字节）
 */

am_static_inline
void amhw_hc32f460_sdioc_blksize_set (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                      uint16_t               size)
{
    p_hw_sdioc->BLKSIZE_f.TBS = size;
}

/**
 * \brief 设定传输数据块的数量
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] count     : 要传输的数据块数量
 */

am_static_inline
void amhw_hc32f460_sdioc_blkcount_set (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                       uint16_t               count)
{
    p_hw_sdioc->BLKCNT = count;
}

/**
 * \brief 写参数寄存器0
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] arg       : 命令参数
 */

am_static_inline
void amhw_hc32f460_sdioc_arg0_set (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                   uint16_t               arg)
{
    p_hw_sdioc->ARG0 = arg;
}

/**
 * \brief 写参数寄存器1
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] arg       : 命令参数
 */

am_static_inline
void amhw_hc32f460_sdioc_arg1_set (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                   uint16_t               arg)
{
    p_hw_sdioc->ARG1 = arg;
}

/**
 * \brief 设置为单数据块传输
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_transmode_single_blk_set (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->TRANSMODE_f.MULB = 0;
}

/**
 * \brief 设置为多数据块传输
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_transmode_multi_blk_set (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->TRANSMODE_f.MULB = 1;
}

typedef enum {
    AMHW_HC32F460_SDIOC_TRANSMODE_DIR_WRITE = 0,
    AMHW_HC32F460_SDIOC_TRANSMODE_DIR_READ  = 1,
} amhw_hc32f460_sdioc_transmode_dir_t;

/**
 * \brief 数据传输方向设置
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] dir       : 传输方向
 */

am_static_inline
void amhw_hc32f460_sdioc_transmode_dir_set (amhw_hc32f460_sdioc_t              *p_hw_sdioc,
                                            amhw_hc32f460_sdioc_transmode_dir_t dir)
{
    p_hw_sdioc->TRANSMODE_f.DDIR = dir;
}

/**
 * \brief 自动命令使能
 *        多块数据传输结束后自动发送CMD12
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_transmode_autocmd_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->TRANSMODE_f.ATCEN = 1;
}

/**
 * \brief 自动命令禁能
 *        不发送自动命令
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_transmode_autocmd_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->TRANSMODE_f.ATCEN = 0;
}

/**
 * \brief 块计数使能
 *        允许数据块计数
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_transmode_blkcnt_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->TRANSMODE_f.BCE = 1;
}

/**
 * \brief 块计数禁能
 *        禁用数据块计数
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_transmode_blkcnt_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->TRANSMODE_f.BCE = 0;
}




/**
 * \brief 命令编号设置
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] id        : 要发送的命令编号
 */

am_static_inline
void amhw_hc32f460_sdioc_cmd_id_set (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                     uint8_t               id)
{
    p_hw_sdioc->CMD_f.IDX = id;
}



typedef enum {
    AMHW_HC32F460_SDIOC_CMD_TYPE_NORMAL  = 0,  /**< \brief 普通命令 */
    AMHW_HC32F460_SDIOC_CMD_TYPE_SUSPEND = 1,  /**< \brief 挂起命令 */
    AMHW_HC32F460_SDIOC_CMD_TYPE_RESUME  = 2,  /**< \brief 恢复命令 */
    AMHW_HC32F460_SDIOC_CMD_TYPE_ABORT   = 3,  /**< \brief 终止命令 */
} amhw_hc32f460_sdioc_cmd_type_t;

/**
 * \brief 命令类型设置
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] type      : 要发送的命令类型
 */

am_static_inline
void amhw_hc32f460_sdioc_cmd_type_set (amhw_hc32f460_sdioc_t          *p_hw_sdioc,
                                       amhw_hc32f460_sdioc_cmd_type_t  type)
{
    p_hw_sdioc->CMD_f.TYP = type;
}

/**
 * \brief 使能带数据命令
 *        即当前命令需要使用数据线
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_cmd_dat_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->CMD_f.DAT = 1;
}

/**
 * \brief 禁能带数据命令
 *        即当前命令只使用命令线
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_cmd_dat_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->CMD_f.DAT = 0;
}

/**
 * \brief 使能编号检查
 *        即检查应答中的命令编号
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_cmd_ice_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->CMD_f.ICE = 1;
}

/**
 * \brief 禁能编号检查
 *        即不检查应答中的命令编号
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_cmd_ice_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->CMD_f.ICE = 0;
}

/**
 * \brief 使能CRC检查
 *        即检查应答中的CRC校验码
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] flag      : AM_TRUE/AM_FALSE
 */

am_static_inline
void amhw_hc32f460_sdioc_cmd_crc_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                         am_bool_t              flag)
{
    p_hw_sdioc->CMD_f.CCE = flag;
}


typedef enum {
    AMHW_HC32F460_SDIOC_CMD_NO_RESPONSE = 0,
    AMHW_HC32F460_SDIOC_CMD_136BIT_RESPONSE,
    AMHW_HC32F460_SDIOC_CMD_48BIT_RESPONSE,
    AMHW_HC32F460_SDIOC_CMD_48BIT_BUSY_RESPONS,
}amhw_hc32f460_sdioc_cmd_res_type_t;


/**
 * \brief 设置应答类型
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 */

am_static_inline
void amhw_hc32f460_sdioc_cmd_restype_set (amhw_hc32f460_sdioc_t              *p_hw_sdioc,
                                          amhw_hc32f460_sdioc_cmd_res_type_t  type)
{
    p_hw_sdioc->CMD_f.RESTYP = type;
}



/**
 * \brief SDIOC 命令配置结构体
 */
typedef struct amhw_hc32f460_sdioc_cmd_cfg {
    uint8_t                             cmd_id;     /**< \brief 命令编号 */

    uint32_t                            cmd_arg;    /**< \brief 命令参数 */

    amhw_hc32f460_sdioc_cmd_type_t      cmd_type;   /**< \brief 命令类型 */

    amhw_hc32f460_sdioc_cmd_res_type_t  rsp_type;   /**< \brief 响应类型 */

    am_bool_t                           use_data;   /**< \brief 是否使用数据线 */
} amhw_hc32f460_sdioc_cmd_cfg_t;

/**
 * \brief 设置命令寄存器，发送命令
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] flag      : 要设置的值
 */

am_static_inline
void amhw_hc32f460_sdioc_cmd_set (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                  uint16_t               flag)
{
    p_hw_sdioc->CMD = flag;
}


/**
 * \brief 读取应答信息的0~15位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return 应答信息的0到15位
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_resp0_15b_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->RESP0;
}

/**
 * \brief 读取应答信息的16~31位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return 应答信息的16到31位
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_resp16_31b_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->RESP1;
}
/**
 * \brief 读取应答信息的32~47位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return 应答信息的32到47位
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_resp32_47b_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->RESP2;
}
/**
 * \brief 读取应答信息的48~63位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return 应答信息的48到63位
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_resp48_63b_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->RESP3;
}
/**
 * \brief 读取应答信息的64~79位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return 应答信息的64到79位
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_resp64_79b_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->RESP4;
}
/**
 * \brief 读取应答信息的80~95位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return 应答信息的80到95位
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_resp80_95b_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->RESP5;
}
/**
 * \brief 读取应答信息的96~111位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return 应答信息的96到111位
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_resp96_111b_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->RESP6;
}
/**
 * \brief 读取应答信息的112~127位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return 应答信息的112到127位
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_resp112_127b_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->RESP7;
}

/**
 * \brief 写数据的低16位到缓冲寄存器
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] data      : 要写入的数据
 */
am_static_inline
void amhw_hc32f460_sdioc_data_low_write (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                         uint16_t               data)
{
    p_hw_sdioc->BUF0 = data;
}

/**
 * \brief 写数据的高16位到缓冲寄存器
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] data      : 要写入的数据
 */
am_static_inline
void amhw_hc32f460_sdioc_data_high_write (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                          uint16_t               data)
{
    p_hw_sdioc->BUF1 = data;
}

/**
 * \brief 写32位数据到buffer
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] data      : 要写入的数据
 */
am_static_inline
void amhw_hc32f460_sdioc_data_write (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                     uint32_t               data)
{
    uint32_t *addr_buffer = (uint32_t *)&(p_hw_sdioc->BUF0);
    *addr_buffer = data;
}



/**
 * \brief 从缓冲寄存器读取数据的低16位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return    接收的数据的低16位
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_data_low_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->BUF0;
}

/**
 * \brief 从缓冲寄存器读取数据的高16位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return    接收的数据的高16位
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_data_high_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->BUF1;
}

/**
 * \brief 从缓冲寄存器读取32位数据
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return    接收32位的数据
 */
am_static_inline
uint32_t amhw_hc32f460_sdioc_data_read (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    uint32_t *addr_buffer = (uint32_t *)(&(p_hw_sdioc->BUF0));
    return (*addr_buffer);
}



/** \brief 主机状态标志 */
#define  AMHW_HC32F460_SDIOC_PSTAT_CMDL (1 << 24)   /**< \brief 命令线状态 */
#define  AMHW_HC32F460_SDIOC_PSTAT_DATL (0xF << 20) /**< \brief 数据线状态（D0-D3） */
#define  AMHW_HC32F460_SDIOC_PSTAT_WPL  (1 << 19)   /**< \brief 写保护线状态 */
#define  AMHW_HC32F460_SDIOC_PSTAT_CDL  (1 << 18)   /**< \brief 卡识别线状态 */
#define  AMHW_HC32F460_SDIOC_PSTAT_CSS  (1 << 17)   /**< \brief 设备稳定状态 */
#define  AMHW_HC32F460_SDIOC_PSTAT_CIN  (1 << 16)   /**< \brief 设备插入状态 */
#define  AMHW_HC32F460_SDIOC_PSTAT_BRE  (1 << 11)   /**< \brief 数据缓冲满 */
#define  AMHW_HC32F460_SDIOC_PSTAT_BWE  (1 << 10)   /**< \brief 数据缓冲空 */
#define  AMHW_HC32F460_SDIOC_PSTAT_RTA  (1 << 9)    /**< \brief 读操作状态 */
#define  AMHW_HC32F460_SDIOC_PSTAT_WTA  (1 << 8)    /**< \brief 写操作状态 */
#define  AMHW_HC32F460_SDIOC_PSTAT_DA   (1 << 2)    /**< \brief 数据线传输状态 */
#define  AMHW_HC32F460_SDIOC_PSTAT_CID  (1 << 1)    /**< \brief 带数据命令抑制 */
#define  AMHW_HC32F460_SDIOC_PSTAT_CIC  (1 << 0)    /**< \brief 命令抑制 */

/**
 * \brief 读取主机状态
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return    主机状态
 */
am_static_inline
uint32_t amhw_hc32f460_sdioc_pstat_all_get (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->PSTAT;
}

/**
 * \brief 读取主机对应状态位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * \return    主机状态
 */
am_static_inline
uint32_t amhw_hc32f460_sdioc_pstat_get (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                        uint32_t               flag)
{
    return (p_hw_sdioc->PSTAT & flag) ? AM_TRUE : AM_FALSE;
}


typedef enum{
    AMHW_HC32F460_SDIOC_HOSTCON_CDSS_TRUE = 0, /**< \brief 采用真实SDIOx_CD线反映卡识别状态 */
    AMHW_HC32F460_SDIOC_HOSTCON_CDSS_TEST = 1, /**< \brief 采用卡识别测试信号反映卡识别状态 */
}amhw_hc32f460_sdioc_hostcon_cdss_t;

/**
 * \brief 卡识别线选择
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] cdss      : 卡识别线选择标志
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_hostcon_cdss_set (amhw_hc32f460_sdioc_t             *p_hw_sdioc,
                                           amhw_hc32f460_sdioc_hostcon_cdss_t cdss)
{
    p_hw_sdioc->HOSTCON_f.CDSS = cdss;
}

/**
 * \brief 卡识别测试信号检测是否有设备插入
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 * return AM_TRUE : 检测到设备插入
 *        AM_FALSE: 无设备插入
 */
am_static_inline
am_bool_t amhw_hc32f460_sdioc_hostcon_cdtl_check (amhw_hc32f460_sdioc_t  *p_hw_sdioc)
{
    return (p_hw_sdioc->HOSTCON_f.CDTL) ? AM_FALSE : AM_TRUE;
}



/**
 * \brief 扩展数据位宽，数据线位宽为8位
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_hostcon_exdw_enable (amhw_hc32f460_sdioc_t  *p_hw_sdioc)
{
    p_hw_sdioc->HOSTCON_f.EXDW = 1;
}

/**
 * \brief 禁止扩展数据位宽，数据线位宽根据DW位设定
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_hostcon_exdw_disable (amhw_hc32f460_sdioc_t  *p_hw_sdioc)
{
    p_hw_sdioc->HOSTCON_f.EXDW = 0;
}

typedef enum {
    AMHW_HC32F460_HOSTCON_DATA_WIDE_1BIT = 0,
    AMHW_HC32F460_HOSTCON_DATA_WIDE_4BIT = 1,
}amhw_hc32f460_sdioc_hostcon_datawide_t;

/**
 * \brief 数据位宽选择（非扩展）
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_hostcon_datawide_set (amhw_hc32f460_sdioc_t                  *p_hw_sdioc,
                                               amhw_hc32f460_sdioc_hostcon_datawide_t  flag)
{
    p_hw_sdioc->HOSTCON_f.DW = flag;
}

/**
 * \brief sdio速度模式配置
 */
typedef enum amhw_hc32f460_sdioc_speed_mode {
    AMHW_HC32F460_SDIO_SPEED_MODE_LOW      = 0x0,   /**< \brief 低速模式 */
    AMHW_HC32F460_SDIO_SPEED_MODE_HIGH     = 0x1,   /**< \brief 高速模式 */
} amhw_hc32f460_sdioc_speed_mode_t;

/**
 * \brief 启用高速模式
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_hostcon_speedmode_set (amhw_hc32f460_sdioc_t           *p_hw_sdioc,
                                                amhw_hc32f460_sdioc_speed_mode_t speed_mode)
{
    p_hw_sdioc->HOSTCON_f.HSEN = speed_mode;
}


/**
 * \brief SDIOC控制器使能
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->PWRCON_f.PWON = 1;
}

/**
 * \brief SDIOC控制器禁能
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->PWRCON_f.PWON = 0;
}

/**
 * \brief 开启数据块间隙期间接收SDIO设备中断
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_blkgpcon_iabg_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->BLKGPCON_f.IABG = 1;
}

/**
 * \brief 禁止数据块间隙期间接收SDIO设备中断
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_blkgpcon_iabg_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->BLKGPCON_f.IABG = 0;
}

/**
 * \brief 启用读等待功能
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_blkgpcon_read_wait_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->BLKGPCON_f.RWC = 1;
}

/**
 * \brief 禁用读等待功能
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_blkgpcon_read_wait_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->BLKGPCON_f.RWC = 0;
}

/**
 * \brief 解除因数据块间隙停止的传输，继续传输
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_blkgpcon_continue_trans (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->BLKGPCON_f.CR = 1;
}

/**
 * \brief 在数据块间隙时停止传输
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_blkgpcon_stop_trans_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->BLKGPCON_f.SABGR = 1;
}

/**
 * \brief 不在数据块间隙时停止传输
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_blkgpcon_stop_trans_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->BLKGPCON_f.SABGR = 0;
}

/* SDIOx_CK 时钟分频选择，基准时钟为EXCLK */
typedef enum {
    AMHW_HC32F460_SDIOC_CK_DIV_1   = 0x00, /**< \brief EXCLK的1分频 */
    AMHW_HC32F460_SDIOC_CK_DIV_2   = 0x01, /**< \brief EXCLK的2分频 */
    AMHW_HC32F460_SDIOC_CK_DIV_4   = 0x02, /**< \brief EXCLK的4分频 */
    AMHW_HC32F460_SDIOC_CK_DIV_8   = 0x04, /**< \brief EXCLK的8分频 */
    AMHW_HC32F460_SDIOC_CK_DIV_16  = 0x08, /**< \brief EXCLK的16分频 */
    AMHW_HC32F460_SDIOC_CK_DIV_32  = 0x10, /**< \brief EXCLK的32分频 */
    AMHW_HC32F460_SDIOC_CK_DIV_64  = 0x20, /**< \brief EXCLK的64分频 */
    AMHW_HC32F460_SDIOC_CK_DIV_128 = 0x40, /**< \brief EXCLK的128分频 */
    AMHW_HC32F460_SDIOC_CK_DIV_256 = 0x80, /**< \brief EXCLK的256分频 */
}amhw_hc32f460_sdioc_clkdiv_t;


/**
 * \brief 设置SDIO_CK时钟分频,基准时钟为EXCLK
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_clkcon_clkdiv_set (amhw_hc32f460_sdioc_t       *p_hw_sdioc,
                                            amhw_hc32f460_sdioc_clkdiv_t clk_div)
{
    p_hw_sdioc->CLKCON_f.FS = clk_div;
}

/**
 * \brief SDIOx_CK输出
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_clkcon_ck_output (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->CLKCON_f.CE = 1;
}

/**
 * \brief SDIOx_CK停止输出
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_clkcon_ck_stop_output (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->CLKCON_f.CE = 0;
}

/**
 * \brief SDIOx_CK时钟开启
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_clkcon_ck_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->CLKCON_f.ICE = 1;
}

/**
 * \brief SDIOx_CK时钟关闭
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_clkcon_ck_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    p_hw_sdioc->CLKCON_f.ICE = 0;
}

typedef enum {
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_13 = 0,  /**< \brief 超时时间为EXCLK × 2^13 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_14 = 1,  /**< \brief 超时时间为EXCLK × 2^14 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_15 = 2,  /**< \brief 超时时间为EXCLK × 2^15 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_16 = 3,  /**< \brief 超时时间为EXCLK × 2^16 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_17 = 4,  /**< \brief 超时时间为EXCLK × 2^17 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_18 = 5,  /**< \brief 超时时间为EXCLK × 2^18 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_19 = 6,  /**< \brief 超时时间为EXCLK × 2^19 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_20 = 7,  /**< \brief 超时时间为EXCLK × 2^20 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_21 = 8,  /**< \brief 超时时间为EXCLK × 2^21 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_22 = 9,  /**< \brief 超时时间为EXCLK × 2^22 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_23 = 10, /**< \brief 超时时间为EXCLK × 2^23 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_24 = 11, /**< \brief 超时时间为EXCLK × 2^24 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_25 = 12, /**< \brief 超时时间为EXCLK × 2^25 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_26 = 13, /**< \brief 超时时间为EXCLK × 2^26 */
    AMHW_HC32F460_SDIOC_TIMEOUT_EXCLK_2_27 = 14, /**< \brief 超时时间为EXCLK × 2^27 */
}amhw_hc32f460_sdioc_timeout_time_t;


/**
 * \brief 设定超时时间
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] time      : 要设定的超时时间
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_timeout_time_set (amhw_hc32f460_sdioc_t               *p_hw_sdioc,
                                           amhw_hc32f460_sdioc_timeout_time_t   time)
{
    p_hw_sdioc->TOUTCON_f.DTO = time;
}

/**
 * \brief 数据复位
 *        复位所有数据相关的寄存器，包含以下寄存器位：
 *        BUF0,BUF1
 *        PSTAT.BRE, PSTAT.BWE, PSTAT.RTA, PSTAT.WTA, PSTAT.DLA, PSTAT.CID
 *        BLKGPCOM.CR, BLKGPCON.SABGR
 *        NORINTST.BRR, NORINTST.BWR, NORINTST.BGE, NORINTST.TC
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] flag      : 是否进行数据复位（AM_TRUE:复位，AM_FALSE:不复位）
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_data_rst (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                   am_bool_t              flag)
{
    p_hw_sdioc->SFTRST_f.RSTD = flag;
}

/**
 * \brief 命令复位
 *        复位所有命令相关的寄存器，包含以下寄存器位：
 *        PSTAT.CIC
 *        NORINTST.CC
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] flag      : 是否进行数据复位（AM_TRUE:复位，AM_FALSE:不复位）
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_cmd_rst (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                  am_bool_t              flag)
{
    p_hw_sdioc->SFTRST_f.RSTC = flag;
}

/**
 * \brief 全部复位
 *        复位除卡识别功能外的全部SDIOC寄存器
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针
 * \param[in] flag      : 是否进行数据复位（AM_TRUE:复位，AM_FALSE:不复位）
 *
 */
am_static_inline
void amhw_hc32f460_sdioc_all_rst (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                  am_bool_t              flag)
{
    p_hw_sdioc->SFTRST_f.RSTA = flag;
}

#define AMHW_HC32F460_SDIOC_NORMAL_INT_EI   (1 << 15) /**< \brief 错误中断 */
#define AMHW_HC32F460_SDIOC_NORMAL_INT_CINT (1 << 8)  /**< \brief 卡中断 */
#define AMHW_HC32F460_SDIOC_NORMAL_INT_CRM  (1 << 7)  /**< \brief 卡移除 */
#define AMHW_HC32F460_SDIOC_NORMAL_INT_CIST (1 << 6)  /**< \brief 卡插入 */
#define AMHW_HC32F460_SDIOC_NORMAL_INT_BRR  (1 << 5)  /**< \brief 缓冲器可读 */
#define AMHW_HC32F460_SDIOC_NORMAL_INT_BWR  (1 << 4)  /**< \brief 缓冲器可写 */
#define AMHW_HC32F460_SDIOC_NORMAL_INT_BGE  (1 << 2)  /**< \brief 数据块间隙停止传输 */
#define AMHW_HC32F460_SDIOC_NORMAL_INT_TC   (1 << 1)  /**< \brief 传输完成 */
#define AMHW_HC32F460_SDIOC_NORMAL_INT_CC   (1 << 0)  /**< \brief 命令完成 */
#define AMHW_HC32F460_SDIOC_NORMAL_INT_ALL  (AMHW_HC32F460_SDIOC_NORMAL_INT_CRM  | \
                                             AMHW_HC32F460_SDIOC_NORMAL_INT_CIST | \
                                             AMHW_HC32F460_SDIOC_NORMAL_INT_BRR  | \
                                             AMHW_HC32F460_SDIOC_NORMAL_INT_BWR  | \
                                             AMHW_HC32F460_SDIOC_NORMAL_INT_BGE  | \
                                             AMHW_HC32F460_SDIOC_NORMAL_INT_TC   | \
                                             AMHW_HC32F460_SDIOC_NORMAL_INT_CC)/**< \brief 所有普通中断，错误中断和卡中断除外 */




/**
 * \brief 读取普通中断状态标志
 *
 * \param[in] p_hw_sdioc: 指向SDIOC寄存器结构体的指针

 * \return  中断状态
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_normal_intstat_all_get (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->NORINTST;
}

/**
 * \brief 读取普通中断某一中断的状态标志
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] intstat_mask: 中断标志掩码

 * \return  中断状态
 */
am_static_inline
am_bool_t amhw_hc32f460_sdioc_normal_intstat_get (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                                  uint16_t               intstat_mask)
{
    return (p_hw_sdioc->NORINTST & intstat_mask) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除普通中断某一中断的状态标志
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] intstat_mask: 中断标志掩码(错误中断EI、卡中断CINT除外，卡中断CINT在SDIO设备撤销申请后复位)
 *
 * \return  中断状态
 */
am_static_inline
void amhw_hc32f460_sdioc_normal_intstat_clr (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                             uint32_t               intstat_mask)
{
    p_hw_sdioc->NORINTST = intstat_mask;
}

/**
 * \brief 普通中断状态使能(不包含错误中断)
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] intstat_mask: 中断标志掩码
 * \return  中断状态
 */
am_static_inline
void amhw_hc32f460_sdioc_normal_intstat_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                                uint16_t               intstat_mask)
{
    p_hw_sdioc->NORINTSTEN |= intstat_mask;
}

/**
 * \brief 普通中断状态禁止(不包含错误中断)
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] intstat_mask: 中断标志掩码
 */
am_static_inline
void amhw_hc32f460_sdioc_normal_intstat_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                                 uint16_t               intstat_mask)
{
    p_hw_sdioc->NORINTSTEN &= (~intstat_mask);
}

/**
 * \brief 普通中断信号使能(不包含错误中断)
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] intstat_mask: 中断标志掩码
 */
am_static_inline
void amhw_hc32f460_sdioc_normal_int_signal_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                                   uint16_t               int_mask)
{
    p_hw_sdioc->NORINTSGEN |= int_mask;
}



/**
 * \brief 普通中断信号禁止(不包含错误中断)
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] intstat_mask: 中断标志掩码
 */
am_static_inline
void amhw_hc32f460_sdioc_normal_int_signal_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                                    uint16_t               int_mask)
{
	p_hw_sdioc->NORINTSGEN &= (~int_mask);
}


#define AMHW_HC32F460_SDIOC_ERR_INT_ACE  (1 << 8) /**< \brief 自动发送命令错误 */
#define AMHW_HC32F460_SDIOC_ERR_INT_DEBE (1 << 6) /**< \brief 数据停止位错误 */
#define AMHW_HC32F460_SDIOC_ERR_INT_DCE  (1 << 5) /**< \brief 数据CRC校验错误 */
#define AMHW_HC32F460_SDIOC_ERR_INT_DTOE (1 << 4) /**< \brief 数据超时错误 */
#define AMHW_HC32F460_SDIOC_ERR_INT_CIE  (1 << 3) /**< \brief 命令编号错误 */
#define AMHW_HC32F460_SDIOC_ERR_INT_CEBE (1 << 2) /**< \brief 命令停止位错误 */
#define AMHW_HC32F460_SDIOC_ERR_INT_CCE  (1 << 1) /**< \brief 命令CRC校验错误 */
#define AMHW_HC32F460_SDIOC_ERR_INT_CTOE (1 << 0) /**< \brief 命令超时错误 */
#define AMHW_HC32F460_SDIOC_ERR_INT_ALL  (AMHW_HC32F460_SDIOC_ERR_INT_ACE  | \
                                          AMHW_HC32F460_SDIOC_ERR_INT_DEBE | \
                                          AMHW_HC32F460_SDIOC_ERR_INT_DCE  | \
                                          AMHW_HC32F460_SDIOC_ERR_INT_DTOE | \
                                          AMHW_HC32F460_SDIOC_ERR_INT_CIE  | \
                                          AMHW_HC32F460_SDIOC_ERR_INT_CEBE | \
                                          AMHW_HC32F460_SDIOC_ERR_INT_CCE  | \
                                          AMHW_HC32F460_SDIOC_ERR_INT_CTOE)


/**
 * \brief 获取错误中断状态标志
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 *
 * \return  错误中断状态
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_err_intstat_all_get (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->ERRINTST;
}

/**
 * \brief 清除错误中断的状态标志
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] intstat_mask: 中断标志掩码(错误中断EI、卡中断CINT除外，卡中断CINT在SDIO设备撤销申请后复位)
 *
 * \return  中断状态
 */
am_static_inline
void amhw_hc32f460_sdioc_err_intstat_clr (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                          uint32_t               int_mask)
{
    p_hw_sdioc->ERRINTST = int_mask;
}


/**
 * \brief 错误中断状态使能
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] int_mask    : 错误中断掩码
 */
am_static_inline
void amhw_hc32f460_sdioc_err_intstat_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                             uint16_t               int_mask)
{
    p_hw_sdioc->ERRINTSTEN |= int_mask;
}

/**
 * \brief 错误中断状态禁能
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] int_mask    : 错误中断掩码
 */
am_static_inline
void amhw_hc32f460_sdioc_err_intstat_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                              uint16_t               int_mask)
{
    p_hw_sdioc->ERRINTSTEN &= (~int_mask);
}


/**
 * \brief 错误中断信号使能
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] int_mask    : 错误中断掩码
 */
am_static_inline
void amhw_hc32f460_sdioc_err_int_signal_enable (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                                uint16_t               int_mask)
{
    p_hw_sdioc->ERRINTSGEN |= int_mask;
}

/**
 * \brief 错误中断信号禁能
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] int_mask    : 错误中断掩码
 */
am_static_inline
void amhw_hc32f460_sdioc_err_int_signal_disable (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                                 uint16_t               int_mask)
{
    p_hw_sdioc->ERRINTSGEN &= (~int_mask);
}

#define AMHW_HC32F460_SDIOC_AUTOCMD_ERRSTAT_CMDE (1 << 7) /**< \brief 未发送错误 */
#define AMHW_HC32F460_SDIOC_AUTOCMD_ERRSTAT_IE   (1 << 4) /**< \brief 命令编号错误 */
#define AMHW_HC32F460_SDIOC_AUTOCMD_ERRSTAT_EBE  (1 << 3) /**< \brief 停止位错误 */
#define AMHW_HC32F460_SDIOC_AUTOCMD_ERRSTAT_CE   (1 << 2) /**< \brief 数据超时错误 */
#define AMHW_HC32F460_SDIOC_AUTOCMD_ERRSTAT_TOE  (1 << 1) /**< \brief 命令超时错误 */
#define AMHW_HC32F460_SDIOC_AUTOCMD_ERRSTAT_NE   (1 << 0) /**< \brief 未执行错误 */

/**
 * \brief 获取自动命令错误状态
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] int_mask    : 错误中断掩码
 */
am_static_inline
uint16_t amhw_hc32f460_sdioc_err_int_signal_all_get (amhw_hc32f460_sdioc_t *p_hw_sdioc)
{
    return p_hw_sdioc->ATCERRST;
}

/**
 * \brief 获取自动命令错误状态
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] int_mask    : 自动错误中断掩码
 */
am_static_inline
am_bool_t amhw_hc32f460_sdioc_autocmd_err_stat_get (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                                    uint16_t               int_mask)
{
    return (p_hw_sdioc->ATCERRST & int_mask) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 强制发生某自动命令错误状态
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] int_mask    : 自动错误中断掩码
 */
am_static_inline
void amhw_hc32f460_sdioc_autocmd_err_stat_set (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                               uint16_t               int_mask)
{
    p_hw_sdioc->FEA |= int_mask;
}

/**
 * \brief 强制发生某错误状态
 *
 * \param[in] p_hw_sdioc  : 指向SDIOC寄存器结构体的指针
 * \param[in] int_mask    : 错误中断掩码
 */
am_static_inline
void amhw_hc32f460_sdioc_err_stat_set (amhw_hc32f460_sdioc_t *p_hw_sdioc,
                                       uint16_t               int_mask)
{
    p_hw_sdioc->FEE |= int_mask;
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
 * @} amhw_hc32f460_if_sdioc
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_SDIOC_H */

/* end of file */
