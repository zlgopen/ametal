/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg118.cn/
*******************************************************************************/

/**
 * \file
 * \brief DMA hardware operation interface.
 *
 * 1. 2 independent programmable DMA channel.
 * 2. 8,16,32 bits data to transfer.
 * 3. support programmable source address,destination address and data size。
 *
 * 存在一个 硬件配置项： AMHW_HC32F460_DMA_CHAN_CNT。用以配置 DMA通道数目，使用本文件时，
 * 应在 soc_cfg.h 文件中定义该宏。
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-04  cds, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32F460_DMA_H
#define __AMHW_HC32F460_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_common.h"
#include "am_vdebug.h"
#include "hc32f460_dma_chan.h"
#include "hc32f460_regbase.h"
#include "hc32f460_inum.h"
/**
 * \addtogroup amhw_hc32f460_if_dma
 * \copydoc amhw_hc32f460_dma.h
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

/** \brief DMA 通道数目  */
#define AMHW_HC32F460_DMA_CHAN_CNT  4

#define AMHW_HC32F460_DMA_DTCTL_CNT_MAX         (0XFFFFUL)  /** <brief 数据控制寄存器最大传输次数 */
#define AMHW_HC32F460_DMA_DTCTL_BLKSIZE_MAX     (1024UL)    /** <brief 数据控制寄存器最大数据块大小 */
#define AMHW_HC32F460_DMA_RPT_DRPT_MAX          (0X3FFUL)   /** <brief 目标地址重复区域最大大小 */
#define AMHW_HC32F460_DMA_RPT_SRPT_MAX          (0X3FFUL)   /** <brief 源地址重复区域最大大小 */
#define AMHW_HC32F460_DMA_SNSEQCTL_SNSCNT_MAX   (0XFFFUL)   /** <brief 源地址跳转最大数据量 */
#define AMHW_HC32F460_DMA_SNSEQCTL_SOFFSET_MAX  (0XFFFFFUL) /** <brief 源地址跳转的最大地址偏移量 */
#define AMHW_HC32F460_DMA_DNSEQCTL_DNSCNT_MAX   (0XFFFUL)   /** <brief 目标地址跳转最大数据量 */
#define AMHW_HC32F460_DMA_DNSEQCTL_DSOFFSET_MAX (0XFFFFFUL) /** <brief 目标地址跳转的最大地址偏移量 */

#define AMHW_HC32F460_DMA_DTCTL_CNT_SHIFT        (16UL)     /** <brief DTCTL寄存器CNT位 */
#define AMHW_HC32F460_DMA_RPT_DRPT_SHIFT         (16UL)     /** <brief RPT寄存器DRPT位 */
#define AMHW_HC32F460_DMA_SNSEQCTL_SNSCNT_SHIFT  (20UL)     /** <brief SNSEQCTL寄存器SNSCNT位 */
#define AMHW_HC32F460_DMA_DNSEQCTL_DNSCNT_SHIFT  (20UL)     /** <brief DNSEQCTL寄存器DNSCNT位 */

#define AMHW_HC32F460_DMA_RCFGCTL_CNTMD_SHIFT    (20UL)     /** <brief RCFGCTL寄存器CNTMD位 */
#define AMHW_HC32F460_DMA_RCFGCTL_DARMD_SHIFT    (18UL)     /** <brief RCFGCTL寄存器DARMD位 */
#define AMHW_HC32F460_DMA_RCFGCTL_SARMD_SHIFT    (16UL)     /** <brief RCFGCTL寄存器SARMD位 */
#define AMHW_HC32F460_DMA_RCFGCTL_RCFGCHS_SHIFT  (8UL)      /** <brief RCFGCTL寄存器RCFGCHS位 */
#define AMHW_HC32F460_DMA_RCFGCTL_RCFGLLP_SHIFT  (1UL)      /** <brief RCFGCTL寄存器RCFGLLP位 */
#define AMHW_HC32F460_DMA_RCFGCTL_RCFGEN_SHIFT   (0UL)      /** <brief RCFGCTL寄存器RCFGEN位 */

/**
 * \brief DMA使能寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t EN                        : 1;  /** <brief DMA使能位 */
    uint32_t RESERVED1                      :31;  /** <brief 保留 */
} stc_dma_en_field_t;

/**
 * \brief DMA中断状态寄存器0位域结构体
 */
typedef struct
{
    __IO uint32_t TRNERR                    : 4;  /** <brief 传输错误中断位 */
    uint32_t RESERVED4                      :12;  /** <brief 保留 */
    __IO uint32_t REQERR                    : 4;  /** <brief 传输请求溢出错误中断位 */
    uint32_t RESERVED20                     :12;  /** <brief 保留 */
} stc_dma_intstat0_field_t;

/**
 * \brief DMA中断状态寄存器1位域结构体
 */
typedef struct
{
    __IO uint32_t TC                        : 4;  /** <brief 传输完成中断位 */
    uint32_t RESERVED4                      :12;  /** <brief 保留 */
    __IO uint32_t BTC                       : 4;  /** <brief 块传输完成中断位 */
    uint32_t RESERVED20                     :12;  /** <brief 保留 */
} stc_dma_intstat1_field_t;

/**
 * \brief DMA中断屏蔽寄存器0位域结构体
 */
typedef struct
{
    __IO uint32_t MSKTRNERR                 : 4;  /** <brief 传输错误中断屏蔽*/
    uint32_t RESERVED4                      :12;  /** <brief 保留 */
    __IO uint32_t MSKREQERR                 : 4;  /** <brief 传输请求溢出中断屏蔽*/
    uint32_t RESERVED20                     :12;  /** <brief 保留 */
} stc_dma_intmask0_field_t;

/**
 * \brief DMA中断屏蔽寄存器1位域结构体
 */
typedef struct
{
    __IO uint32_t MSKTC                     : 4;  /** <brief 传输完成中断屏蔽 */
    uint32_t RESERVED4                      :12;  /** <brief 保留 */
    __IO uint32_t MSKBTC                    : 4;  /** <brief 块传输完成中断屏蔽*/
    uint32_t RESERVED20                     :12;  /** <brief 保留 */
} stc_dma_intmask1_field_t;

/**
 * \brief DMA中断复位寄存器0位域结构体
 */
typedef struct
{
    __IO uint32_t CLRTRNERR                 : 4;  /** <brief 传输错误中断复位 */
    uint32_t RESERVED4                      :12;  /** <brief 保留 */
    __IO uint32_t CLRREQERR                 : 4;  /** <brief 传输请求溢出中断复位 */
    uint32_t RESERVED20                     :12;  /** <brief 保留 */
} stc_dma_intclr0_field_t;

/**
 * \brief DMA中断复位寄存器1位域结构体
 */
typedef struct
{
    __IO uint32_t CLRTC                     : 4;  /** <brief 传输完成中断复位 */
    uint32_t RESERVED4                      :12;  /** <brief 保留 */
    __IO uint32_t CLRBTC                    : 4;  /** <brief 块传输完成中断复位 */
    uint32_t RESERVED20                     :12;  /** <brief 保留 */
} stc_dma_intclr1_field_t;

/**
 * \brief DMA通道使能寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CHEN                      : 4;  /** <brief 通道使能位 */
    uint32_t RESERVED4                      :28;  /** <brief 保留 */
} stc_dma_chen_field_t;

/**
 * \brief DMA通道状态观测寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DMAACT                    : 1;  /** <brief DMA动作中监视位 */
    __IO uint32_t RCFGACT                   : 1;  /** <brief DMA重置动作中监视位 */
    uint32_t RESERVED2                      :14;  /** <brief 保留 */
    __IO uint32_t CHACT                     : 4;  /** <brief 传输动作中通道监视位 */
    uint32_t RESERVED20                     :12;  /** <brief 保留 */
} stc_dma_chstat_field_t;

/**
 * \brief DMA通道重置控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t RCFGEN                    : 1;  /** <brief 通道重置许可 */
    __IO uint32_t RCFGLLP                   : 1;  /** <brief 链指针式通道重置 */
    uint32_t RESERVED2                      : 6;  /** <brief 保留 */
    __IO uint32_t RCFGCHS                   : 4;  /** <brief 重置通道选择 */
    uint32_t RESERVED12                     : 4;  /** <brief 保留 */
    __IO uint32_t SARMD                     : 2;  /** <brief 源地址重置方式 */
    __IO uint32_t DARMD                     : 2;  /** <brief 目标地址重置方式 */
    __IO uint32_t CNTMD                     : 2;  /** <brief 剩余传输次数计数器重置方式 */
    uint32_t RESERVED22                     :10;  /** <brief 保留 */
} stc_dma_rcfgctl_field_t;

/**
 * \brief DMA数据控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t BLKSIZE                   :10;  /** <brief 数据块的大小 */
    uint32_t RESERVED10                     : 6;  /** <brief 保留 */
    __IO uint32_t CNT                       :16;  /** <brief 传输次数 */
} stc_dma_dtctl_field_t;

/**
 * \brief DMA重复区域大小寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SRPT                      :10;  /** <brief 源地址重复区域大小 */
    uint32_t RESERVED10                     : 6;  /** <brief 保留 */
    __IO uint32_t DRPT                      :10;  /** <brief 目标地址重复区域大小 */
    uint32_t RESERVED26                     : 6;  /** <brief 保留 */
} stc_dma_rpt_field_t;

/**
 * \brief DMA重复区域大小寄存器B位域结构体
 */
typedef struct
{
    __IO uint32_t SRPTB                     :10;  /** <brief 源地址重复区域大小 */
    uint32_t RESERVED10                     : 6;  /** <brief 保留 */
    __IO uint32_t DRPTB                     :10;  /** <brief 目标地址重复区域大小 */
    uint32_t RESERVED26                     : 6;  /** <brief 保留 */
} stc_dma_rptb_field_t;

/**
 * \brief DMA源设备不连续地址传输控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SOFFSET                   :20;  /** <brief 源地址跳转的地址偏移量 */
    __IO uint32_t SNSCNT                    :12;  /** <brief 源地址跳转的数据量 */
} stc_dma_snseqctl_field_t;

/**
 * \brief DMA源设备不连续地址传输控制寄存器B位域结构体
 */
typedef struct
{
    __IO uint32_t SNSDIST                   :20;  /** <brief 源设备不连续区域地址间距 */
    __IO uint32_t SNSCNTB                   :12;  /** <brief 源地址跳转的数据量 */
} stc_dma_snseqctlb_field_t;

/**
 * \brief DMA目标设备不连续地址传输控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DOFFSET                   :20;  /** <brief 目标地址跳转的地址偏移量 */
    __IO uint32_t DNSCNT                    :12;  /** <brief 目标地址跳转的数据量 */
} stc_dma_dnseqctl_field_t;


/**
 * \brief DMA目标设备不连续地址传输控制寄存器B位域结构体
 */
typedef struct
{
    __IO uint32_t DNSDIST                   :20;  /** <brief 目标不连续区域地址间距 */
    __IO uint32_t DNSCNTB                   :12;  /** <brief 目标地址跳转的数据量 */
} stc_dma_dnseqctlb_field_t;

/**
 * \brief DMA链指针寄存器位域结构体
 */
typedef struct
{
    uint32_t RESERVED0                      : 2;  /** <brief 保留 */
    __IO uint32_t LLP                       :30;  /** <brief 链指针 */
} stc_dma_llp_field_t;

/**
 * \brief DMA通道控制寄存器0位域结构体
 */
typedef struct
{
    __IO uint32_t SINC                      : 2;  /** <brief 源地址的更新方式 */
    __IO uint32_t DINC                      : 2;  /** <brief 目标地址的更新方式 */
    __IO uint32_t SRTPEN                    : 1;  /** <brief 源重复传输功能使能位 */
    __IO uint32_t DRPTEN                    : 1;  /** <brief 目标重复传输功能使能位 */
    __IO uint32_t SNSEQEN                   : 1;  /** <brief 源地址不连续传输使能 */
    __IO uint32_t DNSEQEN                   : 1;  /** <brief 目标地址不连续传输使能 */
    __IO uint32_t HSIZE                     : 2;  /** <brief 传输数据的宽度 */
    __IO uint32_t LLPEN                     : 1;  /** <brief 连锁传输使能 */
    __IO uint32_t LLPRUN                    : 1;  /** <brief 连锁传输模式选择 */
    __IO uint32_t IE                        : 1;  /** <brief 中断使能位 */
    uint32_t RESERVED13                     :19;  /** <brief 保留 */
} stc_dma_ch0ctl_field_t;

typedef struct
{
    __IO uint32_t BLKSIZE                   :10;
    uint32_t RESERVED10                     : 6;
    __IO uint32_t CNT                       :16;
} stc_dma_mondtctl_field_t;

typedef struct
{
    __IO uint32_t SRPT                      :10;
    uint32_t RESERVED10                     : 6;
    __IO uint32_t DRPT                      :10;
    uint32_t RESERVED26                     : 6;
} stc_dma_monrpt_field_t;

typedef struct
{
    __IO uint32_t SOFFSET                   :20;
    __IO uint32_t SNSCNT                    :12;
} stc_dma_monsnseqctl_field_t;

typedef struct
{
    __IO uint32_t DOFFSET                   :20;
    __IO uint32_t DNSCNT                    :12;
} stc_dma_mondnseqctl_field_t;

/**
 * \brief DMA通道控制寄存器1位域结构体
 */
typedef struct
{
    __IO uint32_t SINC                      : 2;  /** <brief 源地址的更新方式 */
    __IO uint32_t DINC                      : 2;  /** <brief 目标地址的更新方式 */
    __IO uint32_t SRTPEN                    : 1;  /** <brief 源重复传输功能使能位 */
    __IO uint32_t DRPTEN                    : 1;  /** <brief 目标重复传输功能使能位 */
    __IO uint32_t SNSEQEN                   : 1;  /** <brief 源地址不连续传输使能 */
    __IO uint32_t DNSEQEN                   : 1;  /** <brief 目标地址不连续传输使能 */
    __IO uint32_t HSIZE                     : 2;  /** <brief 传输数据的宽度 */
    __IO uint32_t LLPEN                     : 1;  /** <brief 连锁传输使能 */
    __IO uint32_t LLPRUN                    : 1;  /** <brief 连锁传输模式选择 */
    __IO uint32_t IE                        : 1;  /** <brief 中断使能位 */
    uint32_t RESERVED13                     :19;  /** <brief 保留 */
} stc_dma_ch1ctl_field_t;

/**
 * \brief DMA通道控制寄存器2位域结构体
 */
typedef struct
{
    __IO uint32_t SINC                      : 2;  /** <brief 源地址的更新方式 */
    __IO uint32_t DINC                      : 2;  /** <brief 目标地址的更新方式 */
    __IO uint32_t SRTPEN                    : 1;  /** <brief 源重复传输功能使能位 */
    __IO uint32_t DRPTEN                    : 1;  /** <brief 目标重复传输功能使能位 */
    __IO uint32_t SNSEQEN                   : 1;  /** <brief 源地址不连续传输使能 */
    __IO uint32_t DNSEQEN                   : 1;  /** <brief 目标地址不连续传输使能 */
    __IO uint32_t HSIZE                     : 2;  /** <brief 传输数据的宽度 */
    __IO uint32_t LLPEN                     : 1;  /** <brief 连锁传输使能 */
    __IO uint32_t LLPRUN                    : 1;  /** <brief 连锁传输模式选择 */
    __IO uint32_t IE                        : 1;  /** <brief 中断使能位 */
    uint32_t RESERVED13                     :19;  /** <brief 保留 */
} stc_dma_ch2ctl_field_t;

/**
 * \brief DMA通道控制寄存器3位域结构体
 */
typedef struct
{
    __IO uint32_t SINC                      : 2;  /** <brief 源地址的更新方式 */
    __IO uint32_t DINC                      : 2;  /** <brief 目标地址的更新方式 */
    __IO uint32_t SRTPEN                    : 1;  /** <brief 源重复传输功能使能位 */
    __IO uint32_t DRPTEN                    : 1;  /** <brief 目标重复传输功能使能位 */
    __IO uint32_t SNSEQEN                   : 1;  /** <brief 源地址不连续传输使能 */
    __IO uint32_t DNSEQEN                   : 1;  /** <brief 目标地址不连续传输使能 */
    __IO uint32_t HSIZE                     : 2;  /** <brief 传输数据的宽度 */
    __IO uint32_t LLPEN                     : 1;  /** <brief 连锁传输使能 */
    __IO uint32_t LLPRUN                    : 1;  /** <brief 连锁传输模式选择 */
    __IO uint32_t IE                        : 1;  /** <brief 中断使能位 */
    uint32_t RESERVED13                     :19;  /** <brief 保留 */
} stc_dma_ch3ctl_field_t;

/**
 * \brief structure of DMA controller register
 */
typedef struct amhw_hc32f460_dma_chan {
    union
    {
        __IO uint32_t EN;                /**< \brief DMA使能寄存器 */
        stc_dma_en_field_t EN_f;
    };
    union
    {
        __IO uint32_t INTSTAT0;          /**< \brief 中断状态寄存器0 */
        stc_dma_intstat0_field_t INTSTAT0_f;
    };
    union
    {
        __IO uint32_t INTSTAT1;          /**< \brief 中断状态寄存器1 */
        stc_dma_intstat1_field_t INTSTAT1_f;
    };
    union
    {
        __IO uint32_t INTMASK0;          /**< \brief 中断屏蔽寄存器0 */
        stc_dma_intmask0_field_t INTMASK0_f;
    };
    union
    {
        __IO uint32_t INTMASK1;          /**< \brief 中断屏蔽寄存器1 */
        stc_dma_intmask1_field_t INTMASK1_f;
    };
    union
    {
        __IO uint32_t INTCLR0;           /**< \brief 中断复位寄存器0 */
        stc_dma_intclr0_field_t INTCLR0_f;
    };
    union
    {
        __IO uint32_t INTCLR1;           /**< \brief 中断复位寄存器1 */
        stc_dma_intclr1_field_t INTCLR1_f;
    };
    union
    {
        __IO uint32_t CHEN;              /**< \brief 通道使能寄存器 */
        stc_dma_chen_field_t CHEN_f;
    };
    uint8_t RESERVED8[4];
    union
    {
        __IO uint32_t CHSTAT;            /**< \brief 传输中通道监视寄存器 */
        stc_dma_chstat_field_t CHSTAT_f;
    };
    uint8_t RESERVED9[4];
    union
    {
        __IO uint32_t RCFGCTL;           /**< \brief 通道重置控制寄存器 */
        stc_dma_rcfgctl_field_t RCFGCTL_f;
    };
    uint8_t RESERVED10[16];

    __IO uint32_t SAR0;                  /**< \brief 传输源地址寄存器 */

    __IO uint32_t DAR0;                  /**< \brief 传输目标地址寄存器 */
    union
    {
        __IO uint32_t DTCTL0;            /**< \brief 数据控制寄存器 */
        stc_dma_dtctl_field_t DTCTL0_f;
    };
    union
    {
        __IO uint32_t RPT0;              /**< \brief 重复区域大小寄存器 */
        stc_dma_rpt_field_t RPT0_f;

        __IO uint32_t RPTB0;             /**< \brief 重复区域大小寄存器B */
        stc_dma_rptb_field_t RPTB0_f;
    };
    union
    {

        __IO uint32_t SNSEQCTL0;         /**< \brief 源设备不连续地址传输控制寄存器 */
        stc_dma_snseqctl_field_t SNSEQCTL0_f;

        __IO uint32_t SNSEQCTLB0;        /**< \brief 源设备不连续地址传输控制寄存器B */
        stc_dma_snseqctlb_field_t SNSEQCTLB0_f;
    };
    union
    {

        __IO uint32_t DNSEQCTL0;         /**< \brief 目标设备不连续地址传输控制寄存器 */
        stc_dma_dnseqctl_field_t DNSEQCTL0_f;

        __IO uint32_t DNSEQCTLB0;        /**< \brief 目标设备不连续地址传输控制寄存器B */
        stc_dma_dnseqctlb_field_t DNSEQCTLB0_f;
    };
    union
    {   /* 链指针寄存器 */
        __IO uint32_t LLP0;
        stc_dma_llp_field_t LLP0_f;
    };
    union
    {
        __IO uint32_t CH0CTL;            /**< \brief 通道控制寄存器 */
        stc_dma_ch0ctl_field_t CH0CTL_f;
    };
    __IO uint32_t MONSAR0;
    __IO uint32_t MONDAR0;
    union
    {
        __IO uint32_t MONDTCTL0;
        stc_dma_mondtctl_field_t MONDTCTL0_f;
    };
    union
    {
        __IO uint32_t MONRPT0;
        stc_dma_monrpt_field_t MONRPT0_f;
    };
    union
    {
        __IO uint32_t MONSNSEQCTL0;
        stc_dma_monsnseqctl_field_t MONSNSEQCTL0_f;
    };
    union
    {
        __IO uint32_t MONDNSEQCTL0;
        stc_dma_mondnseqctl_field_t MONDNSEQCTL0_f;
    };
    uint8_t RESERVED27[8];            /**< \brief 保留 */
    __IO uint32_t SAR1;               /**< \brief 传输源地址寄存器 */
    __IO uint32_t DAR1;               /**< \brief 传输目标地址寄存器 */
    union
    {
        __IO uint32_t DTCTL1;         /**< \brief 数据控制寄存器 */
        stc_dma_dtctl_field_t DTCTL1_f;
    };
    union
    {
        __IO uint32_t RPT1;           /**< \brief 重复区域大小寄存器 */
        stc_dma_rpt_field_t RPT1_f;
        __IO uint32_t RPTB1;          /**< \brief 重复区域大小寄存器B */
        stc_dma_rptb_field_t RPTB1_f;
    };
    union
    {
        __IO uint32_t SNSEQCTL1;      /**< \brief 源设备不连续地址传输控制寄存器 */
        stc_dma_snseqctl_field_t SNSEQCTL1_f;
        __IO uint32_t SNSEQCTLB1;     /**< \brief 源设备不连续地址传输控制寄存器B */
        stc_dma_snseqctlb_field_t SNSEQCTLB1_f;
    };
    union
    {
        __IO uint32_t DNSEQCTL1;      /**< \brief 目标设备不连续地址传输控制寄存器 */
        stc_dma_dnseqctl_field_t DNSEQCTL1_f;
        __IO uint32_t DNSEQCTLB1;     /**< \brief 目标设备不连续地址传输控制寄存器B */
        stc_dma_dnseqctlb_field_t DNSEQCTLB1_f;
    };
    union
    {
        __IO uint32_t LLP1;           /**< \brief 链指针寄存器 */
        stc_dma_llp_field_t LLP1_f;
    };
    union
    {
        __IO uint32_t CH1CTL;         /**< \brief 通道控制寄存器 */
        stc_dma_ch1ctl_field_t CH1CTL_f;
    };
    __IO uint32_t MONSAR1;
    __IO uint32_t MONDAR1;
    union
    {
        __IO uint32_t MONDTCTL1;
        stc_dma_mondtctl_field_t MONDTCTL1_f;
    };
    union
    {
        __IO uint32_t MONRPT1;
        stc_dma_monrpt_field_t MONRPT1_f;
    };
    union
    {
        __IO uint32_t MONSNSEQCTL1;
        stc_dma_monsnseqctl_field_t MONSNSEQCTL1_f;
    };
    union
    {
        __IO uint32_t MONDNSEQCTL1;
        stc_dma_mondnseqctl_field_t MONDNSEQCTL1_f;
    };
    uint8_t RESERVED44[8];            /**< \brief 保留 */
    __IO uint32_t SAR2;               /**< \brief 传输源地址寄存器 */
    __IO uint32_t DAR2;               /**< \brief 传输目标地址寄存器 */
    union
    {
        __IO uint32_t DTCTL2;         /**< \brief 数据控制寄存器 */
        stc_dma_dtctl_field_t DTCTL2_f;
    };
    union
    {
        __IO uint32_t RPT2;           /**< \brief 重复区域大小寄存器 */
        stc_dma_rpt_field_t RPT2_f;
        __IO uint32_t RPTB2;
        stc_dma_rptb_field_t RPTB2_f; /**< \brief 重复区域大小寄存器B */
    };
    union
    {
        __IO uint32_t SNSEQCTL2;     /**< \brief 源设备不连续地址传输控制寄存器 */
        stc_dma_snseqctl_field_t SNSEQCTL2_f;
        __IO uint32_t SNSEQCTLB2;    /**< \brief 源设备不连续地址传输控制寄存器B */
        stc_dma_snseqctlb_field_t SNSEQCTLB2_f;
    };
    union
    {
        __IO uint32_t DNSEQCTL2;     /**< \brief 目标设备不连续地址传输控制寄存器 */
        stc_dma_dnseqctl_field_t DNSEQCTL2_f;
        __IO uint32_t DNSEQCTLB2;    /**< \brief 目标设备不连续地址传输控制寄存器B */
        stc_dma_dnseqctlb_field_t DNSEQCTLB2_f;
    };
    union
    {
        __IO uint32_t LLP2;          /**< \brief 链指针寄存器 */
        stc_dma_llp_field_t LLP2_f;
    };
    union
    {
        __IO uint32_t CH2CTL;        /**< \brief 通道控制寄存器 */
        stc_dma_ch2ctl_field_t CH2CTL_f;
    };
    __IO uint32_t MONSAR2;
    __IO uint32_t MONDAR2;
    union
    {
        __IO uint32_t MONDTCTL2;
        stc_dma_mondtctl_field_t MONDTCTL2_f;
    };
    union
    {
        __IO uint32_t MONRPT2;
        stc_dma_monrpt_field_t MONRPT2_f;
    };
    union
    {
        __IO uint32_t MONSNSEQCTL2;
        stc_dma_monsnseqctl_field_t MONSNSEQCTL2_f;
    };
    union
    {
        __IO uint32_t MONDNSEQCTL2;
        stc_dma_mondnseqctl_field_t MONDNSEQCTL2_f;
    };
    uint8_t RESERVED61[8];            /**< \brief 保留 */
    __IO uint32_t SAR3;               /**< \brief 传输源地址寄存器 */
    __IO uint32_t DAR3;               /**< \brief 传输目标地址寄存器 */
    union
    {
        __IO uint32_t DTCTL3;         /**< \brief 数据控制寄存器 */
        stc_dma_dtctl_field_t DTCTL3_f;
    };
    union
    {
        __IO uint32_t RPT3;           /**< \brief 重复区域大小寄存器 */
        stc_dma_rpt_field_t RPT3_f;
        __IO uint32_t RPTB3;
        stc_dma_rptb_field_t RPTB3_f; /**< \brief 重复区域大小寄存器B */
    };
    union
    {
        __IO uint32_t SNSEQCTL3;      /**< \brief 源设备不连续地址传输控制寄存器 */
        stc_dma_snseqctl_field_t SNSEQCTL3_f;
        __IO uint32_t SNSEQCTLB3;     /**< \brief 源设备不连续地址传输控制寄存器B */
        stc_dma_snseqctlb_field_t SNSEQCTLB3_f;
    };
    union
    {
        __IO uint32_t DNSEQCTL3;      /**< \brief 目标设备不连续地址传输控制寄存器 */
        stc_dma_dnseqctl_field_t DNSEQCTL3_f;
        __IO uint32_t DNSEQCTLB3;     /**< \brief 目标设备不连续地址传输控制寄存器B */
        stc_dma_dnseqctlb_field_t DNSEQCTLB3_f;
    };
    union
    {
        __IO uint32_t LLP3;           /**< \brief 链指针寄存器 */
        stc_dma_llp_field_t LLP3_f;
    };
    union
    {
        __IO uint32_t CH3CTL;         /**< \brief 通道控制寄存器 */
        stc_dma_ch3ctl_field_t CH3CTL_f;
    };
    __IO uint32_t MONSAR3;
    __IO uint32_t MONDAR3;
    union
    {
        __IO uint32_t MONDTCTL3;
        stc_dma_mondtctl_field_t MONDTCTL3_f;
    };
    union
    {
        __IO uint32_t MONRPT3;
        stc_dma_monrpt_field_t MONRPT3_f;
    };
    union
    {
        __IO uint32_t MONSNSEQCTL3;
        stc_dma_monsnseqctl_field_t MONSNSEQCTL3_f;
    };
    union
    {
        __IO uint32_t MONDNSEQCTL3;
        stc_dma_mondnseqctl_field_t MONDNSEQCTL3_f;
    };
} amhw_hc32f460_dma_t;


/**
 * \brief DMA transfer mode select
 */
typedef enum {
    AMHW_HC32F460_DMA_PER_TO_MER,     /**< \brief DMA peripheral to memory mode */
    AMHW_HC32F460_DMA_MER_TO_PER,     /**< \brief DMA memory to peripheral mode */
    AMHW_HC32F460_DMA_MER_TO_MER,     /**< \brief DMA memory to memory mode */
    AMHW_HC32F460_DMA_PER_TO_PER      /**< \brief DMA peripheral to peripheral mode */
} amhw_hc32f460_dma_transfer_type_t;


/**
 * \brief DMA descriptor
 */
typedef struct amhw_hc32f460_dma_xfer_desc {
    volatile uint32_t          xfercfg;        /**< \brief transfer configuration */
    volatile uint32_t          src_addr;       /**< \brief source address */
    volatile uint32_t          dst_addr;       /**< \brief destination address */
    volatile uint32_t          nbytes;         /**< \brief the bytes of data to transfer */
} amhw_hc32f460_dma_xfer_desc_t;

/**
 * \brief DMA chain transmission descriptor
 */
typedef struct amhw_hc32f460_dma_chain_xfer_desc {
    volatile uint32_t src_addr;       /**< \brief 源地址  */
    volatile uint32_t dst_addr;       /**< \brief 目标地址  */
    volatile uint32_t data_ctrl;      /**< \brief 数据控制寄存器  */
    volatile uint32_t rpt;            /**< \brief 重复区域大小寄存器  */
    volatile uint32_t snseqctl;       /**< \brief 源设备不连续地址传输控制寄存器  */
    volatile uint32_t dnseqctl;       /**< \brief 目标设备不连续地址传输控制寄存器  */
    volatile uint32_t llp;            /**< \brief 链指针寄存器  */
    volatile uint32_t ch_ctl;         /**< \brief 通道控制寄存器  */
} amhw_hc32f460_dma_chain_xfer_desc_t;

/**
 * \brief DMA 通道总使能控制位     使能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_enable(amhw_hc32f460_dma_t *p_hw_dma)
{
    p_hw_dma->EN_f.EN = 0x01;
}

/**
 * \brief DMA 通道总使能控制位     禁能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_disable(amhw_hc32f460_dma_t *p_hw_dma)
{
    p_hw_dma->EN_f.EN = 0x0;
}


/**
 * \brief DMA 通道状态标志位
 *@{
 */
typedef enum {
    AMHW_HC32F460_DMA_CHSTAT_DMAACT     = 0,               /* DMA动作监视位 */
    AMHW_HC32F460_DMA_CHSTAT_RCFGACT    = 1 ,              /* DMA通道重置动作中监视位 */
    AMHW_HC32F460_DMA_CHSTAT_CHACT      = 16,              /* 传输动作中通道监视位 */
}amhw_hc32f460_dma_trans_status_t;

/**
 * \brief DMA 所有通道的传输状态   获取
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 *
 * \note
 *
 * \return none
 */
am_static_inline
int amhw_hc32f460_dma_transfer_check (amhw_hc32f460_dma_t             *p_hw_dma,
                                      uint8_t                          channel,
                                      amhw_hc32f460_dma_trans_status_t flag)
{
    if (flag == AMHW_HC32F460_DMA_CHSTAT_DMAACT) {
        /* DMA是否处于传输动作中 */
        return ((p_hw_dma->CHSTAT) & 0x1ul) ? AM_TRUE : AM_FALSE;
    } else if (flag == AMHW_HC32F460_DMA_CHSTAT_RCFGACT) {
        /* DMA是否处通道重置动作中 */
        return ((p_hw_dma->CHSTAT) & 0x2ul) ? AM_TRUE : AM_FALSE;
    } else if (flag == AMHW_HC32F460_DMA_CHSTAT_CHACT) {
        /* DMA对应通道是否处于动作中 */
        return (((p_hw_dma->CHSTAT) >> 16) & (1 << channel)) ? AM_TRUE : AM_FALSE;
    } else {
            return -AM_EINVAL;
    }
}


/**
 * \brief DMA 某一通道的使能控制位    使能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \note CHEN[3:0]每个位对应一个通道，使用该通道需将该位置1.传输过程中使能位保持为1，
 *       当传输完成次数寄存器DMA_DTCTLx.CNT设置的传输次数后将会自动清零。如果DMA_CTLx.CNT
 *       设置为0，则传输完成后不会自动清零，即无限次传输。
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_enable(amhw_hc32f460_dma_t *p_hw_dma,
                                   uint8_t              channel)
{
    p_hw_dma->CHEN_f.CHEN |= (1 << channel);
}

/**
 * \brief DMA 某一通道的使能控制位    禁能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \note CHEN[3:0]每个位对应一个通道，使用该通道需将该位置1.传输过程中使能位保持为1，
 *       当传输完成次数寄存器DMA_DTCTLx.CNT设置的传输次数后将会自动清零。如果DMA_CTLx.CNT
 *       设置为0，则传输完成后不会自动清零，即无限次传输。
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_disable(amhw_hc32f460_dma_t *p_hw_dma,
                                    uint8_t              channel)
{
    p_hw_dma->CHEN_f.CHEN &= (~(1 << channel));
}


#define HC32F460_AOS_DMA1_TRIGSEL0_BASE    (HC32F460_AOS_BASE + 20UL)
#define HC32F460_AOS_DMA2_TRIGSEL0_BASE    (HC32F460_AOS_BASE + 36UL)

/**
 * \brief DMA TRGSEL[8:0]位选择传输请求的触发源
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] src        : 传输请求设定（hc32f460_dma_chan.h宏定义）
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_trigger_src_set(amhw_hc32f460_dma_t *p_hw_dma,
                                            uint8_t             id,
                                            uint8_t             channel,
                                            en_event_src_t      src)
{
    if (id == 1) {
        *(volatile uint32_t *)((uint32_t)(HC32F460_AOS_DMA1_TRIGSEL0_BASE) + (channel) * 4ul) = src;
    } else if (id == 2){
        *(volatile uint32_t *)((uint32_t)(HC32F460_AOS_DMA2_TRIGSEL0_BASE) + (channel) * 4ul) = src;
    }
}

/**
 * \brief DMA TRGSELRC[8:0]位选择重置请求的触发源
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] src        : 传输请求设定（hc32f460_dma_chan.h宏定义）
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_rcfg_trigger_src_set(en_event_src_t      src)
{
    *(uint32_t *)(0x40010834) = src;
}

/**
 * \brief DMA BLKSIZE[9:0]位指定块的大小
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] num        : 待设值
 *
 * \note 设置数据块的大小，最大可以配置1024个数据。每个数据的宽度由DMA_CHxCTL寄存器的HSIZE位决定。
 *       寄存器值设定位1则每次传输1个数据，设为0则每次传输1024个数据。
 *
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_blksize_set(amhw_hc32f460_dma_t *p_hw_dma,
                                        uint8_t              channel,
                                        uint16_t              size)
{
      uint32_t *addr = (uint32_t *)(((uint32_t)&(p_hw_dma->DTCTL0)) + (channel * 0x40));
    volatile uint32_t value_reg = *addr;
      value_reg &= (~0x3FF);
      value_reg |= size;
    *addr = value_reg;
}

/**
 * \brief DMA BLKSIZE[9:0] 获取块的大小
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \note 设置数据块的大小，最大可以配置1024个数据。每个数据的宽度由DMA_CHxCTL寄存器的HSIZE位决定。
 *       寄存器值设定位1则每次传输1个数据，设为0则每次传输1024个数据。
 *
 *
 * \return 数据块的大小。
 */
am_static_inline
uint16_t amhw_hc32f460_dma_chan_blksize_get(amhw_hc32f460_dma_t *p_hw_dma,
                                            uint8_t              channel)
{
    volatile uint32_t value = *(volatile uint32_t *)((uint32_t)(&(p_hw_dma->DTCTL0)) + (channel * 0x40));
    value &= 0x3ff;
    return (uint16_t)value;
}

/**
 * \brief DMA CNT[15:0] 位用于指定总的传输次数。
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] num        : 待设值(最大65535)
 *
 * \note 每次请求启动一个数据块的传输，完成时传输次数减1，当减到0时发生传输完成中断。如果设置为0，
 *       表示无线次传输，每次启动请求传输一个数据块，完成时传输次数保持0不变，不会产出传输完成中断。
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_tran_data_num_set (amhw_hc32f460_dma_t *p_hw_dma,
                                               uint8_t              channel,
                                               uint16_t             num)
{
    volatile uint32_t reg_val = *(volatile uint32_t *)((uint32_t)(&(p_hw_dma->DTCTL0)) + (channel * 0x40));
    reg_val &= (~0xFFFF0000);
    reg_val |= (num << 16);
    *(uint32_t *)((uint32_t)(&(p_hw_dma->DTCTL0)) + (channel * 0x40)) = reg_val;
}

/**
 * \brief DMA CNT[15:0] 位用于指定块的传输次数。     获取
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \note
 *
 * \return 当前传输剩余的次数。
 */
am_static_inline
uint16_t amhw_hc32f460_dma_chan_tran_data_num_get (amhw_hc32f460_dma_t *p_hw_dma,
                                                   uint8_t              channel)
{
    volatile uint32_t reg_val = *(volatile uint32_t *)((uint32_t)(&(p_hw_dma->DTCTL0)) + (channel * 0x40));
    return (uint16_t)(reg_val >> 16);
}


/**
 * \brief DMA CHxCTL[9:8] 位用于设置传输数据的宽度
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] hsize      : 数据宽度(枚举amhw_hc32f460_dma_chan_data_hsize)
 *                         00: 8bit
 *                         01: 16bit
 *                         10,11: 32bit
 *
 * \note
 */
am_static_inline
void amhw_hc32f460_dma_chan_data_hsize_set (amhw_hc32f460_dma_t *p_hw_dma,
                                            uint8_t              channel,
                                            uint8_t              hsize)
{
    volatile uint32_t *addr = (volatile uint32_t *)((uint32_t)&(p_hw_dma->CH0CTL) + (channel * 0x40));
    volatile uint32_t value_reg = *addr;
    value_reg &= (~(0x3 << 8));
    value_reg |= (hsize << 8);
    *addr = value_reg;
}

/**
 * \brief DMA CHxCTL[9:8] 获取设置传输数据的宽度
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] hsize      : 数据宽度(枚举amhw_hc32f460_dma_chan_data_hsize)
 *                         00: 8bit
 *                         01: 16bit
 *                         10,11: 32bit
 *
 * \note
 */
am_static_inline
uint8_t amhw_hc32f460_dma_chan_data_hsize_get (amhw_hc32f460_dma_t *p_hw_dma,
                                               uint8_t              channel)
{
    volatile uint32_t reg_val = *(uint32_t *)((uint32_t)&(p_hw_dma->CH0CTL) + (channel * 0x40));
    return (uint8_t)((reg_val & (0x3 << 8)) >> 8);
}

/**
 * \brief DMA CHxCTL[3:2] 位用于设置对应通道目标地址的更新方式
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] method     : 更新方式
 *
 * \note
 */
am_static_inline
void amhw_hc32f460_dma_chan_srcaddr_update_set (amhw_hc32f460_dma_t *p_hw_dma,
                                                uint8_t              channel,
                                                uint8_t              method)
{
    volatile uint32_t *addr = (volatile uint32_t *)((uint32_t)&(p_hw_dma->CH0CTL) + (channel * 0x40));
    volatile uint32_t value_reg = *addr;
    value_reg &= (~(0x3 << 2));
    value_reg |= (method << 2);
    *addr = value_reg;
}

/**
 * \brief DMA CHxCTL[3:2] 位用于设置对应通道源地址的更新方式
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] method     : 更新方式
 *
 * \note
 */
am_static_inline
void amhw_hc32f460_dma_chan_dstaddr_update_set (amhw_hc32f460_dma_t *p_hw_dma,
                                                uint8_t              channel,
                                                uint8_t              method)
{
    volatile uint32_t *addr = (volatile uint32_t *)((uint32_t)&(p_hw_dma->CH0CTL) + (channel * 0x40));
    volatile uint32_t value_reg = *addr;
    value_reg &= (~(0x3 << 0));
    value_reg |= (method << 0);
    *addr = value_reg;
}


#define  AMHW_HC32F460_DMA_CHAN_CFG_INT_ENABLE      (1UL << 12) /**< \brief 使能该通道中断 */
#define  AMHW_HC32F460_DMA_CHAN_CFG_INT_DISABLE     (1UL << 12) /**< \brief 禁能该通道中断 */

#define  AMHW_HC32F460_DMA_CHAN_CFG_LLPRUN_ENABLE   (1 << 11)   /**< \brief 连锁传输模式下，新描述符载入后，立即开始传输 */
#define  AMHW_HC32F460_DMA_CHAN_CFG_LLPRUN_DISABLE  (0 << 11)   /**< \brief 连锁传输模式下，新描述符载入后，等下一个传输请求产生后开始传输 */

#define  AMHW_HC32F460_DMA_CHAN_CFG_LLP_ENABLE      (1 << 10)   /**< \brief 连锁传输使能 */
#define  AMHW_HC32F460_DMA_CHAN_CFG_LLP_DISABLE     (0 << 10)   /**< \brief 连锁传输禁能 */

#define  AMHW_HC32F460_DMA_CHAN_CFG_SIZE_8BIT       (0UL << 8)  /**< \brief 数据宽度 8bit */
#define  AMHW_HC32F460_DMA_CHAN_CFG_SIZE_16BIT      (1UL << 8)  /**< \brief 数据宽度 16bit */
#define  AMHW_HC32F460_DMA_CHAN_CFG_SIZE_32BIT      (2UL << 8)  /**< \brief 数据宽度 32bit */

#define  AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_ENABLE  (1UL << 7) /**< \brief 目标地址不连续传输使能 */
#define  AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_DISABLE (0UL << 7) /**< \brief 目标地址不连续传输禁能 */

#define  AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_ENABLE  (1UL << 6) /**< \brief 源地址不连续传输使能 */
#define  AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_DISABLE (0UL << 6) /**< \brief 源地址不连续传输禁能 */

#define  AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_ENABLE  (1UL << 5) /**< \brief 目标重复传输功能使能 */
#define  AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_DISABLE (0UL << 5) /**< \brief 目标重复传输功能禁能 */

#define  AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_ENABLE  (1UL << 4) /**< \brief 源重复传输功能使能 */
#define  AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_DISABLE (0UL << 4) /**< \brief 源重复传输功能禁能 */

#define  AMHW_HC32F460_DMA_CHAN_DST_ADD_FIXED      (0UL << 2) /**< \brief 目标地址固定 */
#define  AMHW_HC32F460_DMA_CHAN_DST_ADD_INCREASING (1UL << 2) /**< \brief 目标地址递增 */
#define  AMHW_HC32F460_DMA_CHAN_DST_ADD_DECREASING (2UL << 2) /**< \brief 目标地址递减 */

#define  AMHW_HC32F460_DMA_CHAN_SRC_ADD_FIXED      (0UL << 0) /**< \brief 源地址固定 */
#define  AMHW_HC32F460_DMA_CHAN_SRC_ADD_INCREASING (1UL << 0) /**< \brief 源地址递增 */
#define  AMHW_HC32F460_DMA_CHAN_SRC_ADD_DECREASING (2UL << 0) /**< \brief 源地址递减 */
/**
 * \brief DMA 通道控制寄存器设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] flag       : 设置标志
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_config_set(amhw_hc32f460_dma_t *p_hw_dma,
                                       uint8_t              channel,
                                       uint32_t             flag)

{
    *(uint32_t *)((uint32_t)&(p_hw_dma->CH0CTL) + (channel * 0x40)) = flag;
}


#define  AMHW_HC32F460_DMA_CHAN_INT_ERR      (1UL << 20) /**< \brief 未完成中断  */
#define  AMHW_HC32F460_DMA_CHAN_INT_TX_CMP   (1UL << 19) /**< \brief 完成中断 */

/**
 * \brief DMA 中断状态
 *@{
 */
typedef enum {
    AMHW_HC32F460_DMA_INT_FLAG_DEFAULT         = 0,         /**< \brief 初始默认值 */
    AMHW_HC32F460_DMA_INT_FLAG_ERR_REQ_ERR     = (1 << 0),  /**< \brief 传输请求溢出错误中断 */
    AMHW_HC32F460_DMA_INT_FLAG_ERR_TRNERR      = (1 << 1),  /**< \brief 传输错误中断 */
    AMHW_HC32F460_DMA_INT_FLAG_BLK_TX_COMPLETE = (1 << 2),  /**< \brief 块传输完成中断 */
    AMHW_HC32F460_DMA_INT_FLAG_TX_COMPLETE     = (1 << 3),  /**< \brief 传输完成中断*/
}amhw_hc32f460_dma_int_flag_t;
/**
 * \brief DMA 某一通道中断使能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_int_enable(amhw_hc32f460_dma_t *p_hw_dma,
                                       uint8_t              channel)
{
    volatile uint32_t *addr = (volatile uint32_t *)((uint32_t)&(p_hw_dma->CH0CTL) + (channel * 0x40));
    volatile uint32_t value_reg = *addr;
    value_reg |= (1 << 12);
    *addr = value_reg;
}

/**
 * \brief DMA 某一通道中断禁能
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_int_disable(amhw_hc32f460_dma_t *p_hw_dma,
                                        uint8_t              channel)
{
    volatile uint32_t *addr = (volatile uint32_t *)((uint32_t)&(p_hw_dma->CH0CTL) + (channel * 0x40));
    volatile uint32_t value_reg = *addr;
    value_reg &= (~(1 << 12));
    *addr = value_reg;
}

/**
 * \brief 设置中断屏蔽寄存器
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] chan       : 通道号
 * \param[in] flag       : 中断编号
 *
 * \note

 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_int_mask_set(amhw_hc32f460_dma_t *p_hw_dma,
                                    uint8_t              chan,
                                    uint32_t             flag)
{
    if (flag == AMHW_HC32F460_DMA_INT_FLAG_ERR_REQ_ERR) {
        p_hw_dma->INTMASK0_f.MSKREQERR |= (1 << chan);
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_ERR_TRNERR) {
        p_hw_dma->INTMASK0_f.MSKTRNERR |= (1 << chan);
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_BLK_TX_COMPLETE) {
        p_hw_dma->INTMASK1_f.MSKBTC    |= (1 << chan);
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_TX_COMPLETE) {
        p_hw_dma->INTMASK1_f.MSKTC     |= (1 << chan);
    }
}

/**
 * \brief 清除中断屏蔽寄存器
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] chan       : 通道号
 * \param[in] flag       : 中断编号
 *
 * \note

 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_int_mask_clr(amhw_hc32f460_dma_t *p_hw_dma,
                                    uint8_t              chan,
                                    uint32_t             flag)
{
    if (flag == AMHW_HC32F460_DMA_INT_FLAG_ERR_REQ_ERR) {
        p_hw_dma->INTMASK0_f.MSKREQERR &= (~(1 << chan));
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_ERR_TRNERR) {
        p_hw_dma->INTMASK0_f.MSKTRNERR &= (~(1 << chan));
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_BLK_TX_COMPLETE) {
        p_hw_dma->INTMASK1_f.MSKBTC    &= (~(1 << chan));
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_TX_COMPLETE) {
        p_hw_dma->INTMASK1_f.MSKTC     &= (~(1 << chan));
    }
}


/**
 * \brief 中断状态标志获取
 *
 * \param[in] p_hw_dma : 指向DMA寄存器结构体的指针
 * \param[in] channel  : 通道号
 * \param[in] flag     : 状态标志
 *
 * \return AM_TRUE or AM_FALSE
 */
am_static_inline
int amhw_hc32f460_dma_chan_stat_check (amhw_hc32f460_dma_t         *p_hw_dma,
                                             uint8_t                      channel,
                                             amhw_hc32f460_dma_int_flag_t flag)
{
    if (flag == AMHW_HC32F460_DMA_INT_FLAG_ERR_REQ_ERR) {
        return (((p_hw_dma->INTSTAT0 >> 16) & (1 << channel)) ? AM_TRUE: AM_FALSE);
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_ERR_TRNERR) {
        return (((p_hw_dma->INTSTAT0 >> 0) & (1 << channel)) ? AM_TRUE: AM_FALSE);
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_BLK_TX_COMPLETE) {
        return (((p_hw_dma->INTSTAT1 >> 16) & (1 << channel)) ? AM_TRUE: AM_FALSE);
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_TX_COMPLETE) {
        return (((p_hw_dma->INTSTAT1 >> 0) & (1 << channel)) ? AM_TRUE: AM_FALSE);
    } else{
        return -AM_EINVAL;
    }
}

/**
 * \brief 中断状态标志清除
 *
 * \param[in] p_hw_dma    : 指向DMA寄存器结构体的指针
 * \param[in] channel     : 通道号
 * \param[in] flag        : 状态标志
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_flag_clear (amhw_hc32f460_dma_t         *p_hw_dma,
                                        uint8_t                      channel,
                                        amhw_hc32f460_dma_int_flag_t flag)
{
    if (flag == AMHW_HC32F460_DMA_INT_FLAG_ERR_REQ_ERR) {
        p_hw_dma->INTCLR0 |= (1 << (16 + channel));
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_ERR_TRNERR) {
        p_hw_dma->INTCLR0 |= (1 << (0 + channel));
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_BLK_TX_COMPLETE) {
        p_hw_dma->INTCLR1 |= (1 << (16 + channel));
    } else if (flag == AMHW_HC32F460_DMA_INT_FLAG_TX_COMPLETE) {
        p_hw_dma->INTCLR1 |= (1 << (0 + channel));
    }
}

/**
 * \brief 源地址设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] address    : 地址
 *
 * \note
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_src_address_set(amhw_hc32f460_dma_t *p_hw_dma,
                                            uint8_t            channel,
                                            uint32_t           address)
{
    *(uint32_t *)((uint32_t)&(p_hw_dma->SAR0) + (channel * 0x40)) =  address;
}

/**
 * \brief 目标地址设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] address    : 地址
 *
 * \note

 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_dst_address_set(amhw_hc32f460_dma_t *p_hw_dma,
                                            uint8_t              channel,
                                            uint32_t             address)
{
    *(uint32_t *)((uint32_t)&(p_hw_dma->DAR0) + (channel * 0x40)) =  address;
}

/**
 * \brief DMA数据控制寄存器设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] cfg        : 设置值
 *
 * \note

 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_dtctl_set(amhw_hc32f460_dma_t *p_hw_dma,
                                      uint8_t              channel,
                                      uint32_t             cfg)
{
    *(uint32_t *)((uint32_t)&(p_hw_dma->DTCTL0) + (channel * 0x40)) =  cfg;
}

/**
 * \brief DMA重复区域寄存器设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] cfg        : 设置值
 *
 * \note

 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_rpt_set(amhw_hc32f460_dma_t *p_hw_dma,
                                    uint8_t              channel,
                                    uint32_t             cfg)
{
    *(uint32_t *)((uint32_t)&(p_hw_dma->RPT0) + (channel * 0x40)) =  cfg;
}

/**
 * \brief DMA目标地址重复区域大小设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] size       : 目标地址重复区域大小（以数据个数为单位）
 *
 * \note

 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_drpt_set(amhw_hc32f460_dma_t *p_hw_dma,
                                     uint8_t              channel,
                                     uint32_t             size)
{
    volatile uint32_t *addr = (volatile uint32_t *)((uint32_t)&(p_hw_dma->RPT0) + (channel * 0x40));
    volatile uint32_t value_reg = *addr;
    value_reg &= (~(0x3FF << 16));
    value_reg |= (size << 16);
    *addr = value_reg;
}

/**
 * \brief DMA源地址重复区域大小设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] size       : 源地址重复区域大小（以数据个数为单位）
 *
 * \note

 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_srpt_set(amhw_hc32f460_dma_t *p_hw_dma,
                                     uint8_t              channel,
                                     uint32_t             size)
{
    volatile uint32_t *addr = (volatile uint32_t *)((uint32_t)&(p_hw_dma->RPT0) + (channel * 0x40));
    volatile uint32_t value_reg = *addr;
    value_reg &= (~0x3FF);
    value_reg |= size;
    *addr = value_reg;
}

/**
 * \brief DMA源设备不连续地址传输控制寄存器设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] cfg        : 设置值
 *
 * \note
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_snseqctl_set(amhw_hc32f460_dma_t *p_hw_dma,
                                         uint8_t              channel,
                                         uint32_t             cfg)
{
    *(uint32_t *)((uint32_t)&(p_hw_dma->SNSEQCTL0) + (channel * 0x40)) =  cfg;
}

/**
 * \brief DMA目标设备不连续地址传输控制寄存器设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] cfg        : 设置值
 *
 * \note

 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_dnseqctl_set(amhw_hc32f460_dma_t *p_hw_dma,
                                         uint8_t              channel,
                                         uint32_t             cfg)
{
    *(uint32_t *)((uint32_t)&(p_hw_dma->DNSEQCTL0) + (channel * 0x40)) =  cfg;
}

/**
 * \brief DMA链指针寄存器设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] cfg        : 设置值
 *
 * \note

 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_llp_set(amhw_hc32f460_dma_t *p_hw_dma,
                                    uint8_t              channel,
                                    uint32_t             cfg)
{
    *(uint32_t *)((uint32_t)&(p_hw_dma->LLP0) + (channel * 0x40)) =  cfg;
}

/**
 * \brief DMA通道重置控制寄存器设置
 *
 * \param[in] p_hw_dma   : 指向DMA寄存器结构体的指针
 * \param[in] channel    : 通道号
 * \param[in] cfg        : 设置值
 *
 * \note

 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_chan_rcfg_set(amhw_hc32f460_dma_t *p_hw_dma, uint32_t flag)
{
    p_hw_dma->RCFGCTL = flag;
}

/**
 * \brief 软件触发DMA
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_dma_soft_trig(void)
{
    *(volatile uint32_t *)HC32F460_AOS_BASE = 0x1;
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
 * @} amhw_if_hc32f460_dma
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AMHW_HC32F460_DMA_H */

/* end of file */
