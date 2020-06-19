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
 * \brief USB interfaces for operating hardware.
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-18  sdq, first implementation
 * \endinternal
 */
#ifndef __AMHW_HC32F460_USBD_H
#define __AMHW_HC32F460_USBD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
#include "am_types.h"
#include "hc32f460_periph_map.h"
/**
 * \addtogroup amhw_hc32f460_if_usbd
 * \copydoc HC32F460_USBd.h
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

typedef struct
{
    __IO uint32_t GINTMSK                   : 1;
    __IO uint32_t HBSTLEN                   : 4;
    __IO uint32_t DMAEN                     : 1;
    uint32_t RESERVED6                      : 1;
    __IO uint32_t TXFELVL                   : 1;
    __IO uint32_t PTXFELVL                  : 1;
    uint32_t RESERVED9                      :23;
} stc_usbfs_gahbcfg_field_t;

typedef struct
{
    __IO uint32_t TOCAL                     : 3;
    uint32_t RESERVED3                      : 3;
    __IO uint32_t PHYSEL                    : 1;
    uint32_t RESERVED7                      : 3;
    __IO uint32_t TRDT                      : 4;
    uint32_t RESERVED14                     :15;
    __IO uint32_t FHMOD                     : 1;
    __IO uint32_t FDMOD                     : 1;
    uint32_t RESERVED31                     : 1;
} stc_usbfs_gusbcfg_field_t;

typedef struct
{
    __IO uint32_t CSRST                     : 1;
    __IO uint32_t HSRST                     : 1;
    __IO uint32_t FCRST                     : 1;
    uint32_t RESERVED3                      : 1;
    __IO uint32_t RXFFLSH                   : 1;
    __IO uint32_t TXFFLSH                   : 1;
    __IO uint32_t TXFNUM                    : 5;
    uint32_t RESERVED11                     :19;
    __IO uint32_t DMAREQ                    : 1;
    __IO uint32_t AHBIDL                    : 1;
} stc_usbfs_grstctl_field_t;

typedef struct
{
    __IO uint32_t CMOD                      : 1;
    __IO uint32_t MMIS                      : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t SOF                       : 1;
    __IO uint32_t RXFLNE                    : 1;
    __IO uint32_t NPTXFE                    : 1;
    __IO uint32_t GINAKEFF                  : 1;
    __IO uint32_t GOUTNAKEFF                : 1;
    uint32_t RESERVED8                      : 2;
    __IO uint32_t ESUSP                     : 1;
    __IO uint32_t USBSUSP                   : 1;
    __IO uint32_t USBRST                    : 1;
    __IO uint32_t ENUMDNE                   : 1;
    __IO uint32_t ISOODRP                   : 1;
    __IO uint32_t EOPF                      : 1;
    uint32_t RESERVED16                     : 2;
    __IO uint32_t IEPINT                    : 1;
    __IO uint32_t OEPINT                    : 1;
    __IO uint32_t IISOIXFR                  : 1;
    __IO uint32_t IPXFR_INCOMPISO           : 1;
    __IO uint32_t DATAFSUSP                 : 1;
    uint32_t RESERVED23                     : 1;
    __IO uint32_t HPRTINT                   : 1;
    __IO uint32_t HCINT                     : 1;
    __IO uint32_t PTXFE                     : 1;
    uint32_t RESERVED27                     : 1;
    __IO uint32_t CIDSCHG                   : 1;
    __IO uint32_t DISCINT                   : 1;
    __IO uint32_t VBUSVINT                  : 1;
    __IO uint32_t WKUINT                    : 1;
} stc_usbfs_gintsts_field_t;

typedef struct
{
    uint32_t RESERVED0                      : 1;
    __IO uint32_t MMISM                     : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t SOFM                      : 1;
    __IO uint32_t RXFNEM                    : 1;
    __IO uint32_t NPTXFEM                   : 1;
    __IO uint32_t GINAKEFFM                 : 1;
    __IO uint32_t GOUTNAKEFFM               : 1;
    uint32_t RESERVED8                      : 2;
    __IO uint32_t ESUSPM                    : 1;
    __IO uint32_t USBSUSPM                  : 1;
    __IO uint32_t USBRSTM                   : 1;
    __IO uint32_t ENUMDNEM                  : 1;
    __IO uint32_t ISOODRPM                  : 1;
    __IO uint32_t EOPFM                     : 1;
    uint32_t RESERVED16                     : 2;
    __IO uint32_t IEPIM                     : 1;
    __IO uint32_t OEPIM                     : 1;
    __IO uint32_t IISOIXFRM                 : 1;
    __IO uint32_t IPXFRM_INCOMPISOOUTM      : 1;
    __IO uint32_t DATAFSUSPM                : 1;
    uint32_t RESERVED23                     : 1;
    __IO uint32_t HPRTIM                    : 1;
    __IO uint32_t HCIM                      : 1;
    __IO uint32_t PTXFEM                    : 1;
    uint32_t RESERVED27                     : 1;
    __IO uint32_t CIDSCHGM                  : 1;
    __IO uint32_t DISCIM                    : 1;
    __IO uint32_t VBUSVIM                   : 1;
    __IO uint32_t WKUIM                     : 1;
} stc_usbfs_gintmsk_field_t;

typedef struct
{
    __IO uint32_t CHNUM_EPNUM               : 4;
    __IO uint32_t BCNT                      :11;
    __IO uint32_t DPID                      : 2;
    __IO uint32_t PKTSTS                    : 4;
    uint32_t RESERVED21                     :11;
} stc_usbfs_grxstsr_field_t;

typedef struct
{
    __IO uint32_t CHNUM_EPNUM               : 4;
    __IO uint32_t BCNT                      :11;
    __IO uint32_t DPID                      : 2;
    __IO uint32_t PKTSTS                    : 4;
    uint32_t RESERVED21                     :11;
} stc_usbfs_grxstsp_field_t;

typedef struct
{
    __IO uint32_t RXFD                      :11;
    uint32_t RESERVED11                     :21;
} stc_usbfs_grxfsiz_field_t;

typedef struct
{
    __IO uint32_t NPTXFSA                   :16;
    __IO uint32_t NPTXFD                    :16;
} stc_usbfs_hnptxfsiz_field_t;

typedef struct
{
    __IO uint32_t NPTXFSAV                  :16;
    __IO uint32_t NPTQXSAV                  : 8;
    __IO uint32_t NPTXQTOP                  : 7;
    uint32_t RESERVED31                     : 1;
} stc_usbfs_hnptxsts_field_t;

typedef struct
{
    __IO uint32_t PTXSA                     :12;
    uint32_t RESERVED12                     : 4;
    __IO uint32_t PTXFD                     :11;
    uint32_t RESERVED27                     : 5;
} stc_usbfs_hptxfsiz_field_t;

typedef struct
{
    __IO uint32_t INEPTXSA                  :12;
    uint32_t RESERVED12                     : 4;
    __IO uint32_t INEPTXFD                  :10;
    uint32_t RESERVED26                     : 6;
} stc_usbfs_dieptxf_field_t;

typedef struct
{
    __IO uint32_t FSLSPCS                   : 2;
    __IO uint32_t FSLSS                     : 1;
    uint32_t RESERVED3                      :29;
} stc_usbfs_hcfg_field_t;

typedef struct
{
    __IO uint32_t FRIVL                     :16;
    uint32_t RESERVED16                     :16;
} stc_usbfs_hfir_field_t;

typedef struct
{
    __IO uint32_t FRNUM                     :16;
    __IO uint32_t FTREM                     :16;
} stc_usbfs_hfnum_field_t;

typedef struct
{
    __IO uint32_t PTXFSAVL                  :16;
    __IO uint32_t PTXQSAV                   : 8;
    __IO uint32_t PTXQTOP                   : 8;
} stc_usbfs_hptxsts_field_t;

typedef struct
{
    __IO uint32_t HAINT                     :12;
    uint32_t RESERVED12                     :20;
} stc_usbfs_haint_field_t;

typedef struct
{
    __IO uint32_t HAINTM                    :12;
    uint32_t RESERVED12                     :20;
} stc_usbfs_haintmsk_field_t;

typedef struct
{
    __IO uint32_t PCSTS                     : 1;
    __IO uint32_t PCDET                     : 1;
    __IO uint32_t PENA                      : 1;
    __IO uint32_t PENCHNG                   : 1;
    uint32_t RESERVED4                      : 2;
    __IO uint32_t PRES                      : 1;
    __IO uint32_t PSUSP                     : 1;
    __IO uint32_t PRST                      : 1;
    uint32_t RESERVED9                      : 1;
    __IO uint32_t PLSTS                     : 2;
    __IO uint32_t PWPR                      : 1;
    uint32_t RESERVED13                     : 4;
    __IO uint32_t PSPD                      : 2;
    uint32_t RESERVED19                     :13;
} stc_usbfs_hprt_field_t;

typedef struct
{
    __IO uint32_t MPSIZ                     :11;
    __IO uint32_t EPNUM                     : 4;
    __IO uint32_t EPDIR                     : 1;
    uint32_t RESERVED16                     : 1;
    __IO uint32_t LSDEV                     : 1;
    __IO uint32_t EPTYP                     : 2;
    uint32_t RESERVED20                     : 2;
    __IO uint32_t DAD                       : 7;
    __IO uint32_t ODDFRM                    : 1;
    __IO uint32_t CHDIS                     : 1;
    __IO uint32_t CHENA                     : 1;
} stc_usbfs_hcchar_field_t;

typedef struct
{
    __IO uint32_t XFRC                      : 1;
    __IO uint32_t CHH                       : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t STALL                     : 1;
    __IO uint32_t NAK                       : 1;
    __IO uint32_t ACK                       : 1;
    uint32_t RESERVED6                      : 1;
    __IO uint32_t TXERR                     : 1;
    __IO uint32_t BBERR                     : 1;
    __IO uint32_t FRMOR                     : 1;
    __IO uint32_t DTERR                     : 1;
    uint32_t RESERVED11                     :21;
} stc_usbfs_hcint_field_t;

typedef struct
{
    __IO uint32_t XFRCM                     : 1;
    __IO uint32_t CHHM                      : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t STALLM                    : 1;
    __IO uint32_t NAKM                      : 1;
    __IO uint32_t ACKM                      : 1;
    uint32_t RESERVED6                      : 1;
    __IO uint32_t TXERRM                    : 1;
    __IO uint32_t BBERRM                    : 1;
    __IO uint32_t FRMORM                    : 1;
    __IO uint32_t DTERRM                    : 1;
    uint32_t RESERVED11                     :21;
} stc_usbfs_hcintmsk_field_t;

typedef struct
{
    __IO uint32_t XFRSIZ                    :19;
    __IO uint32_t PKTCNT                    :10;
    __IO uint32_t DPID                      : 2;
    uint32_t RESERVED31                     : 1;
} stc_usbfs_hctsiz_field_t;

typedef struct
{
    __IO uint32_t DSPD                      : 2;
    __IO uint32_t NZLSOHSK                  : 1;
    uint32_t RESERVED3                      : 1;
    __IO uint32_t DAD                       : 7;
    __IO uint32_t PFIVL                     : 2;
    uint32_t RESERVED13                     :19;
} stc_usbfs_dcfg_field_t;

typedef struct
{
    __IO uint32_t RWUSIG                    : 1;
    __IO uint32_t SDIS                      : 1;
    __IO uint32_t GINSTS                    : 1;
    __IO uint32_t GONSTS                    : 1;
    __IO uint32_t TCTL                      : 3;
    __IO uint32_t SGINAK                    : 1;
    __IO uint32_t CGINAK                    : 1;
    __IO uint32_t SGONAK                    : 1;
    __IO uint32_t CGONAK                    : 1;
    __IO uint32_t POPRGDNE                  : 1;
    uint32_t RESERVED12                     :20;
} stc_usbfs_dctl_field_t;

typedef struct
{
    __IO uint32_t SUSPSTS                   : 1;
    __IO uint32_t ENUMSPD                   : 2;
    __IO uint32_t EERR                      : 1;
    uint32_t RESERVED4                      : 4;
    __IO uint32_t FNSOF                     :14;
    uint32_t RESERVED22                     :10;
} stc_usbfs_dsts_field_t;

typedef struct
{
    __IO uint32_t XFRCM                     : 1;
    __IO uint32_t EPDM                      : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t TOM                       : 1;
    __IO uint32_t ITTXFEMSK                 : 1;
    __IO uint32_t INEPNMM                   : 1;
    __IO uint32_t IINEPNEM                  : 1;
    uint32_t RESERVED7                      :25;
} stc_usbfs_diepmsk_field_t;

typedef struct
{
    __IO uint32_t XFRCM                     : 1;
    __IO uint32_t EPDM                      : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t STUPM                     : 1;
    __IO uint32_t OTEPDM                    : 1;
    uint32_t RESERVED5                      :27;
} stc_usbfs_doepmsk_field_t;

typedef struct
{
    __IO uint32_t IEPINT                    : 6;
    uint32_t RESERVED6                      :10;
    __IO uint32_t OEPINT                    : 6;
    uint32_t RESERVED22                     :10;
} stc_usbfs_daint_field_t;

typedef struct
{
    __IO uint32_t IEPINTM                   : 6;
    uint32_t RESERVED6                      :10;
    __IO uint32_t OEPINTM                   : 6;
    uint32_t RESERVED22                     :10;
} stc_usbfs_daintmsk_field_t;

typedef struct
{
    __IO uint32_t INEPTXFEM                 : 6;
    uint32_t RESERVED6                      :26;
} stc_usbfs_diepempmsk_field_t;

typedef struct
{
    __IO uint32_t MPSIZ                     : 2;
    uint32_t RESERVED2                      :13;
    __IO uint32_t USBAEP                    : 1;
    uint32_t RESERVED16                     : 1;
    __IO uint32_t NAKSTS                    : 1;
    __IO uint32_t EPTYP                     : 2;
    uint32_t RESERVED20                     : 1;
    __IO uint32_t STALL                     : 1;
    __IO uint32_t TXFNUM                    : 4;
    __IO uint32_t CNAK                      : 1;
    __IO uint32_t SNAK                      : 1;
    uint32_t RESERVED28                     : 2;
    __IO uint32_t EPDIS                     : 1;
    __IO uint32_t EPENA                     : 1;
} stc_usbfs_diepctl0_field_t;

typedef struct
{
    __IO uint32_t XFRC                      : 1;
    __IO uint32_t EPDISD                    : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t TOC                       : 1;
    __IO uint32_t TTXFE                     : 1;
    uint32_t RESERVED5                      : 1;
    __IO uint32_t NEPNE                     : 1;
    __IO uint32_t TXFE                      : 1;
    uint32_t RESERVED8                      :24;
} stc_usbfs_diepint0_field_t;

typedef struct
{
    __IO uint32_t XFRSIZ                    : 7;
    uint32_t RESERVED7                      :12;
    __IO uint32_t PKTCNT                    : 2;
    uint32_t RESERVED21                     :11;
} stc_usbfs_dieptsiz0_field_t;

typedef struct
{
    __IO uint32_t INEPTFSAV                 :16;
    uint32_t RESERVED16                     :16;
} stc_usbfs_dtxfsts0_field_t;

typedef struct
{
    __IO uint32_t MPSIZ                     :11;
    uint32_t RESERVED11                     : 4;
    __IO uint32_t USBAEP                    : 1;
    __IO uint32_t EONUM_DPID                : 1;
    __IO uint32_t NAKSTS                    : 1;
    __IO uint32_t EPTYP                     : 2;
    uint32_t RESERVED20                     : 1;
    __IO uint32_t STALL                     : 1;
    __IO uint32_t TXFNUM                    : 4;
    __IO uint32_t CNAK                      : 1;
    __IO uint32_t SNAK                      : 1;
    __IO uint32_t SD0PID_SEVNFRM            : 1;
    __IO uint32_t SODDFRM                   : 1;
    __IO uint32_t EPDIS                     : 1;
    __IO uint32_t EPENA                     : 1;
} stc_usbfs_diepctl_field_t;

typedef struct
{
    __IO uint32_t XFRC                      : 1;
    __IO uint32_t EPDISD                    : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t TOC                       : 1;
    __IO uint32_t TTXFE                     : 1;
    uint32_t RESERVED5                      : 1;
    __IO uint32_t NEPNE                     : 1;
    __IO uint32_t TXFE                      : 1;
    uint32_t RESERVED8                      :24;
} stc_usbfs_diepint_field_t;

typedef struct
{
    __IO uint32_t XFRSIZ                    :19;
    __IO uint32_t PKTCNT                    :10;
    __IO uint32_t MC                        : 2;
    uint32_t RESERVED31                     : 1;
} stc_usbfs_dieptsiz_field_t;

typedef struct
{
    __IO uint32_t INEPTFSAV                 :16;
    uint32_t RESERVED16                     :16;
} stc_usbfs_dtxfsts_field_t;

typedef struct
{
    __IO uint32_t MPSIZ                     : 2;
    uint32_t RESERVED2                      :13;
    __IO uint32_t USBAEP                    : 1;
    uint32_t RESERVED16                     : 1;
    __IO uint32_t NAKSTS                    : 1;
    __IO uint32_t EPTYP                     : 2;
    __IO uint32_t SNPM                      : 1;
    __IO uint32_t STALL                     : 1;
    uint32_t RESERVED22                     : 4;
    __IO uint32_t CNAK                      : 1;
    __IO uint32_t SNAK                      : 1;
    uint32_t RESERVED28                     : 2;
    __IO uint32_t EPDIS                     : 1;
    __IO uint32_t EPENA                     : 1;
} stc_usbfs_doepctl0_field_t;

typedef struct
{
    __IO uint32_t XFRC                      : 1;
    __IO uint32_t EPDISD                    : 1;
    uint32_t RESERVED2                      : 1;
    __IO uint32_t STUP                      : 1;
    __IO uint32_t OTEPDIS                   : 1;
    uint32_t RESERVED5                      : 1;
    __IO uint32_t B2BSTUP                   : 1;
    uint32_t RESERVED7                      :25;
} stc_usbfs_doepint_field_t;

typedef struct
{
    __IO uint32_t XFRSIZ                    : 7;
    uint32_t RESERVED7                      :12;
    __IO uint32_t PKTCNT                    : 1;
    uint32_t RESERVED20                     : 9;
    __IO uint32_t STUPCNT                   : 2;
    uint32_t RESERVED31                     : 1;
} stc_usbfs_doeptsiz0_field_t;

typedef struct
{
    __IO uint32_t MPSIZ                     :11;
    uint32_t RESERVED11                     : 4;
    __IO uint32_t USBAEP                    : 1;
    __IO uint32_t DPID                      : 1;
    __IO uint32_t NAKSTS                    : 1;
    __IO uint32_t EPTYP                     : 2;
    __IO uint32_t SNPM                      : 1;
    __IO uint32_t STALL                     : 1;
    uint32_t RESERVED22                     : 4;
    __IO uint32_t CNAK                      : 1;
    __IO uint32_t SNAK                      : 1;
    __IO uint32_t SD0PID                    : 1;
    __IO uint32_t SD1PID                    : 1;
    __IO uint32_t EPDIS                     : 1;
    __IO uint32_t EPENA                     : 1;
} stc_usbfs_doepctl_field_t;

typedef struct
{
    __IO uint32_t XFRSIZ                    :19;
    __IO uint32_t PKTCNT                    :10;
    uint32_t RESERVED29                     : 3;
} stc_usbfs_doeptsiz_field_t;

typedef struct
{
    __IO uint32_t STPPCLK                   : 1;
    __IO uint32_t GATEHCLK                  : 1;
    uint32_t RESERVED2                      :30;
} stc_usbfs_pcgcctl_field_t;

/**
 *  \brief USB 寄存器结构体定义
 */
typedef struct
{
    uint8_t RESERVED0[8];
    union
    {
        __IO uint32_t GAHBCFG;
        stc_usbfs_gahbcfg_field_t GAHBCFG_f;
    };
    union
    {
        __IO uint32_t GUSBCFG;
        stc_usbfs_gusbcfg_field_t GUSBCFG_f;
    };
    union
    {
        __IO uint32_t GRSTCTL;
        stc_usbfs_grstctl_field_t GRSTCTL_f;
    };
    union
    {
        __IO uint32_t GINTSTS;
        stc_usbfs_gintsts_field_t GINTSTS_f;
    };
    union
    {
        __IO uint32_t GINTMSK;
        stc_usbfs_gintmsk_field_t GINTMSK_f;
    };
    union
    {
        __IO uint32_t GRXSTSR;
        stc_usbfs_grxstsr_field_t GRXSTSR_f;
    };
    union
    {
        __IO uint32_t GRXSTSP;
        stc_usbfs_grxstsp_field_t GRXSTSP_f;
    };
    union
    {
        __IO uint32_t GRXFSIZ;
        stc_usbfs_grxfsiz_field_t GRXFSIZ_f;
    };
    union
    {
        __IO uint32_t HNPTXFSIZ;
        stc_usbfs_hnptxfsiz_field_t HNPTXFSIZ_f;
    };
    union
    {
        __IO uint32_t HNPTXSTS;
        stc_usbfs_hnptxsts_field_t HNPTXSTS_f;
    };
    uint8_t RESERVED10[12];
    __IO uint32_t CID;
    uint8_t RESERVED11[192];
    union
    {
        __IO uint32_t HPTXFSIZ;
        stc_usbfs_hptxfsiz_field_t HPTXFSIZ_f;
    };
    union
    {
        __IO uint32_t DIEPTXF1;
        stc_usbfs_dieptxf_field_t DIEPTXF1_f;
    };
    union
    {
        __IO uint32_t DIEPTXF2;
        stc_usbfs_dieptxf_field_t DIEPTXF2_f;
    };
    union
    {
        __IO uint32_t DIEPTXF3;
        stc_usbfs_dieptxf_field_t DIEPTXF3_f;
    };
    union
    {
        __IO uint32_t DIEPTXF4;
        stc_usbfs_dieptxf_field_t DIEPTXF4_f;
    };
    union
    {
        __IO uint32_t DIEPTXF5;
        stc_usbfs_dieptxf_field_t DIEPTXF5_f;
    };
    uint8_t RESERVED17[744];
    union
    {
        __IO uint32_t HCFG;
        stc_usbfs_hcfg_field_t HCFG_f;
    };
    union
    {
        __IO uint32_t HFIR;
        stc_usbfs_hfir_field_t HFIR_f;
    };
    union
    {
        __IO uint32_t HFNUM;
        stc_usbfs_hfnum_field_t HFNUM_f;
    };
    uint8_t RESERVED20[4];
    union
    {
        __IO uint32_t HPTXSTS;
        stc_usbfs_hptxsts_field_t HPTXSTS_f;
    };
    union
    {
        __IO uint32_t HAINT;
        stc_usbfs_haint_field_t HAINT_f;
    };
    union
    {
        __IO uint32_t HAINTMSK;
        stc_usbfs_haintmsk_field_t HAINTMSK_f;
    };
    uint8_t RESERVED23[36];
    union
    {
        __IO uint32_t HPRT;
        stc_usbfs_hprt_field_t HPRT_f;
    };
    uint8_t RESERVED24[188];
    union
    {
        __IO uint32_t HCCHAR0;
        stc_usbfs_hcchar_field_t HCCHAR0_f;
    };
    uint8_t RESERVED25[4];
    union
    {
        __IO uint32_t HCINT0;
        stc_usbfs_hcint_field_t HCINT0_f;
    };
    union
    {
        __IO uint32_t HCINTMSK0;
        stc_usbfs_hcintmsk_field_t HCINTMSK0_f;
    };
    union
    {
        __IO uint32_t HCTSIZ0;
        stc_usbfs_hctsiz_field_t HCTSIZ0_f;
    };
    __IO uint32_t HCDMA0;
    uint8_t RESERVED29[8];
    union
    {
        __IO uint32_t HCCHAR1;
        stc_usbfs_hcchar_field_t HCCHAR1_f;
    };
    uint8_t RESERVED30[4];
    union
    {
        __IO uint32_t HCINT1;
        stc_usbfs_hcint_field_t HCINT1_f;
    };
    union
    {
        __IO uint32_t HCINTMSK1;
        stc_usbfs_hcintmsk_field_t HCINTMSK1_f;
    };
    union
    {
        __IO uint32_t HCTSIZ1;
        stc_usbfs_hctsiz_field_t HCTSIZ1_f;
    };
    __IO uint32_t HCDMA1;
    uint8_t RESERVED34[8];
    union
    {
        __IO uint32_t HCCHAR2;
        stc_usbfs_hcchar_field_t HCCHAR2_f;
    };
    uint8_t RESERVED35[4];
    union
    {
        __IO uint32_t HCINT2;
        stc_usbfs_hcint_field_t HCINT2_f;
    };
    union
    {
        __IO uint32_t HCINTMSK2;
        stc_usbfs_hcintmsk_field_t HCINTMSK2_f;
    };
    union
    {
        __IO uint32_t HCTSIZ2;
        stc_usbfs_hctsiz_field_t HCTSIZ2_f;
    };
    __IO uint32_t HCDMA2;
    uint8_t RESERVED39[8];
    union
    {
        __IO uint32_t HCCHAR3;
        stc_usbfs_hcchar_field_t HCCHAR3_f;
    };
    uint8_t RESERVED40[4];
    union
    {
        __IO uint32_t HCINT3;
        stc_usbfs_hcint_field_t HCINT3_f;
    };
    union
    {
        __IO uint32_t HCINTMSK3;
        stc_usbfs_hcintmsk_field_t HCINTMSK3_f;
    };
    union
    {
        __IO uint32_t HCTSIZ3;
        stc_usbfs_hctsiz_field_t HCTSIZ3_f;
    };
    __IO uint32_t HCDMA3;
    uint8_t RESERVED44[8];
    union
    {
        __IO uint32_t HCCHAR4;
        stc_usbfs_hcchar_field_t HCCHAR4_f;
    };
    uint8_t RESERVED45[4];
    union
    {
        __IO uint32_t HCINT4;
        stc_usbfs_hcint_field_t HCINT4_f;
    };
    union
    {
        __IO uint32_t HCINTMSK4;
        stc_usbfs_hcintmsk_field_t HCINTMSK4_f;
    };
    union
    {
        __IO uint32_t HCTSIZ4;
        stc_usbfs_hctsiz_field_t HCTSIZ4_f;
    };
    __IO uint32_t HCDMA4;
    uint8_t RESERVED49[8];
    union
    {
        __IO uint32_t HCCHAR5;
        stc_usbfs_hcchar_field_t HCCHAR5_f;
    };
    uint8_t RESERVED50[4];
    union
    {
        __IO uint32_t HCINT5;
        stc_usbfs_hcint_field_t HCINT5_f;
    };
    union
    {
        __IO uint32_t HCINTMSK5;
        stc_usbfs_hcintmsk_field_t HCINTMSK5_f;
    };
    union
    {
        __IO uint32_t HCTSIZ5;
        stc_usbfs_hctsiz_field_t HCTSIZ5_f;
    };
    __IO uint32_t HCDMA5;
    uint8_t RESERVED54[8];
    union
    {
        __IO uint32_t HCCHAR6;
        stc_usbfs_hcchar_field_t HCCHAR6_f;
    };
    uint8_t RESERVED55[4];
    union
    {
        __IO uint32_t HCINT6;
        stc_usbfs_hcint_field_t HCINT6_f;
    };
    union
    {
        __IO uint32_t HCINTMSK6;
        stc_usbfs_hcintmsk_field_t HCINTMSK6_f;
    };
    union
    {
        __IO uint32_t HCTSIZ6;
        stc_usbfs_hctsiz_field_t HCTSIZ6_f;
    };
    __IO uint32_t HCDMA6;
    uint8_t RESERVED59[8];
    union
    {
        __IO uint32_t HCCHAR7;
        stc_usbfs_hcchar_field_t HCCHAR7_f;
    };
    uint8_t RESERVED60[4];
    union
    {
        __IO uint32_t HCINT7;
        stc_usbfs_hcint_field_t HCINT7_f;
    };
    union
    {
        __IO uint32_t HCINTMSK7;
        stc_usbfs_hcintmsk_field_t HCINTMSK7_f;
    };
    union
    {
        __IO uint32_t HCTSIZ7;
        stc_usbfs_hctsiz_field_t HCTSIZ7_f;
    };
    __IO uint32_t HCDMA7;
    uint8_t RESERVED64[8];
    union
    {
        __IO uint32_t HCCHAR8;
        stc_usbfs_hcchar_field_t HCCHAR8_f;
    };
    uint8_t RESERVED65[4];
    union
    {
        __IO uint32_t HCINT8;
        stc_usbfs_hcint_field_t HCINT8_f;
    };
    union
    {
        __IO uint32_t HCINTMSK8;
        stc_usbfs_hcintmsk_field_t HCINTMSK8_f;
    };
    union
    {
        __IO uint32_t HCTSIZ8;
        stc_usbfs_hctsiz_field_t HCTSIZ8_f;
    };
    __IO uint32_t HCDMA8;
    uint8_t RESERVED69[8];
    union
    {
        __IO uint32_t HCCHAR9;
        stc_usbfs_hcchar_field_t HCCHAR9_f;
    };
    uint8_t RESERVED70[4];
    union
    {
        __IO uint32_t HCINT9;
        stc_usbfs_hcint_field_t HCINT9_f;
    };
    union
    {
        __IO uint32_t HCINTMSK9;
        stc_usbfs_hcintmsk_field_t HCINTMSK9_f;
    };
    union
    {
        __IO uint32_t HCTSIZ9;
        stc_usbfs_hctsiz_field_t HCTSIZ9_f;
    };
    __IO uint32_t HCDMA9;
    uint8_t RESERVED74[8];
    union
    {
        __IO uint32_t HCCHAR10;
        stc_usbfs_hcchar_field_t HCCHAR10_f;
    };
    uint8_t RESERVED75[4];
    union
    {
        __IO uint32_t HCINT10;
        stc_usbfs_hcint_field_t HCINT10_f;
    };
    union
    {
        __IO uint32_t HCINTMSK10;
        stc_usbfs_hcintmsk_field_t HCINTMSK10_f;
    };
    union
    {
        __IO uint32_t HCTSIZ10;
        stc_usbfs_hctsiz_field_t HCTSIZ10_f;
    };
    __IO uint32_t HCDMA10;
    uint8_t RESERVED79[8];
    union
    {
        __IO uint32_t HCCHAR11;
        stc_usbfs_hcchar_field_t HCCHAR11_f;
    };
    uint8_t RESERVED80[4];
    union
    {
        __IO uint32_t HCINT11;
        stc_usbfs_hcint_field_t HCINT11_f;
    };
    union
    {
        __IO uint32_t HCINTMSK11;
        stc_usbfs_hcintmsk_field_t HCINTMSK11_f;
    };
    union
    {
        __IO uint32_t HCTSIZ11;
        stc_usbfs_hctsiz_field_t HCTSIZ11_f;
    };
    __IO uint32_t HCDMA11;
    uint8_t RESERVED84[392];
    union
    {
        __IO uint32_t DCFG;
        stc_usbfs_dcfg_field_t DCFG_f;
    };
    union
    {
        __IO uint32_t DCTL;
        stc_usbfs_dctl_field_t DCTL_f;
    };
    union
    {
        __IO uint32_t DSTS;
        stc_usbfs_dsts_field_t DSTS_f;
    };
    uint8_t RESERVED87[4];
    union
    {
        __IO uint32_t DIEPMSK;
        stc_usbfs_diepmsk_field_t DIEPMSK_f;
    };
    union
    {
        __IO uint32_t DOEPMSK;
        stc_usbfs_doepmsk_field_t DOEPMSK_f;
    };
    union
    {
        __IO uint32_t DAINT;
        stc_usbfs_daint_field_t DAINT_f;
    };
    union
    {
        __IO uint32_t DAINTMSK;
        stc_usbfs_daintmsk_field_t DAINTMSK_f;
    };
    uint8_t RESERVED91[20];
    union
    {
        __IO uint32_t DIEPEMPMSK;
        stc_usbfs_diepempmsk_field_t DIEPEMPMSK_f;
    };
    uint8_t RESERVED92[200];
    union
    {
        __IO uint32_t DIEPCTL0;
        stc_usbfs_diepctl0_field_t DIEPCTL0_f;
    };
    uint8_t RESERVED93[4];
    union
    {
        __IO uint32_t DIEPINT0;
        stc_usbfs_diepint0_field_t DIEPINT0_f;
    };
    uint8_t RESERVED94[4];
    union
    {
        __IO uint32_t DIEPTSIZ0;
        stc_usbfs_dieptsiz0_field_t DIEPTSIZ0_f;
    };
    __IO uint32_t DIEPDMA0;
    union
    {
        __IO uint32_t DTXFSTS0;
        stc_usbfs_dtxfsts0_field_t DTXFSTS0_f;
    };
    uint8_t RESERVED97[4];
    union
    {
        __IO uint32_t DIEPCTL1;
        stc_usbfs_diepctl_field_t DIEPCTL1_f;
    };
    uint8_t RESERVED98[4];
    union
    {
        __IO uint32_t DIEPINT1;
        stc_usbfs_diepint_field_t DIEPINT1_f;
    };
    uint8_t RESERVED99[4];
    union
    {
        __IO uint32_t DIEPTSIZ1;
        stc_usbfs_dieptsiz_field_t DIEPTSIZ1_f;
    };
    __IO uint32_t DIEPDMA1;
    union
    {
        __IO uint32_t DTXFSTS1;
        stc_usbfs_dtxfsts_field_t DTXFSTS1_f;
    };
    uint8_t RESERVED102[4];
    union
    {
        __IO uint32_t DIEPCTL2;
        stc_usbfs_diepctl_field_t DIEPCTL2_f;
    };
    uint8_t RESERVED103[4];
    union
    {
        __IO uint32_t DIEPINT2;
        stc_usbfs_diepint_field_t DIEPINT2_f;
    };
    uint8_t RESERVED104[4];
    union
    {
        __IO uint32_t DIEPTSIZ2;
        stc_usbfs_dieptsiz_field_t DIEPTSIZ2_f;
    };
    __IO uint32_t DIEPDMA2;
    union
    {
        __IO uint32_t DTXFSTS2;
        stc_usbfs_dtxfsts_field_t DTXFSTS2_f;
    };
    uint8_t RESERVED107[4];
    union
    {
        __IO uint32_t DIEPCTL3;
        stc_usbfs_diepctl_field_t DIEPCTL3_f;
    };
    uint8_t RESERVED108[4];
    union
    {
        __IO uint32_t DIEPINT3;
        stc_usbfs_diepint_field_t DIEPINT3_f;
    };
    uint8_t RESERVED109[4];
    union
    {
        __IO uint32_t DIEPTSIZ3;
        stc_usbfs_dieptsiz_field_t DIEPTSIZ3_f;
    };
    __IO uint32_t DIEPDMA3;
    union
    {
        __IO uint32_t DTXFSTS3;
        stc_usbfs_dtxfsts_field_t DTXFSTS3_f;
    };
    uint8_t RESERVED112[4];
    union
    {
        __IO uint32_t DIEPCTL4;
        stc_usbfs_diepctl_field_t DIEPCTL4_f;
    };
    uint8_t RESERVED113[4];
    union
    {
        __IO uint32_t DIEPINT4;
        stc_usbfs_diepint_field_t DIEPINT4_f;
    };
    uint8_t RESERVED114[4];
    union
    {
        __IO uint32_t DIEPTSIZ4;
        stc_usbfs_dieptsiz_field_t DIEPTSIZ4_f;
    };
    __IO uint32_t DIEPDMA4;
    union
    {
        __IO uint32_t DTXFSTS4;
        stc_usbfs_dtxfsts_field_t DTXFSTS4_f;
    };
    uint8_t RESERVED117[4];
    union
    {
        __IO uint32_t DIEPCTL5;
        stc_usbfs_diepctl_field_t DIEPCTL5_f;
    };
    uint8_t RESERVED118[4];
    union
    {
        __IO uint32_t DIEPINT5;
        stc_usbfs_diepint_field_t DIEPINT5_f;
    };
    uint8_t RESERVED119[4];
    union
    {
        __IO uint32_t DIEPTSIZ5;
        stc_usbfs_dieptsiz_field_t DIEPTSIZ5_f;
    };
    __IO uint32_t DIEPDMA5;
    union
    {
        __IO uint32_t DTXFSTS5;
        stc_usbfs_dtxfsts_field_t DTXFSTS5_f;
    };
    uint8_t RESERVED122[324];
    union
    {
        __IO uint32_t DOEPCTL0;
        stc_usbfs_doepctl0_field_t DOEPCTL0_f;
    };
    uint8_t RESERVED123[4];
    union
    {
        __IO uint32_t DOEPINT0;
        stc_usbfs_doepint_field_t DOEPINT0_f;
    };
    uint8_t RESERVED124[4];
    union
    {
        __IO uint32_t DOEPTSIZ0;
        stc_usbfs_doeptsiz0_field_t DOEPTSIZ0_f;
    };
    __IO uint32_t DOEPDMA0;
    uint8_t RESERVED126[8];
    union
    {
        __IO uint32_t DOEPCTL1;
        stc_usbfs_doepctl_field_t DOEPCTL1_f;
    };
    uint8_t RESERVED127[4];
    union
    {
        __IO uint32_t DOEPINT1;
        stc_usbfs_doepint_field_t DOEPINT1_f;
    };
    uint8_t RESERVED128[4];
    union
    {
        __IO uint32_t DOEPTSIZ1;
        stc_usbfs_doeptsiz_field_t DOEPTSIZ1_f;
    };
    __IO uint32_t DOEPDMA1;
    uint8_t RESERVED130[8];
    union
    {
        __IO uint32_t DOEPCTL2;
        stc_usbfs_doepctl_field_t DOEPCTL2_f;
    };
    uint8_t RESERVED131[4];
    union
    {
        __IO uint32_t DOEPINT2;
        stc_usbfs_doepint_field_t DOEPINT2_f;
    };
    uint8_t RESERVED132[4];
    union
    {
        __IO uint32_t DOEPTSIZ2;
        stc_usbfs_doeptsiz_field_t DOEPTSIZ2_f;
    };
    __IO uint32_t DOEPDMA2;
    uint8_t RESERVED134[8];
    union
    {
        __IO uint32_t DOEPCTL3;
        stc_usbfs_doepctl_field_t DOEPCTL3_f;
    };
    uint8_t RESERVED135[4];
    union
    {
        __IO uint32_t DOEPINT3;
        stc_usbfs_doepint_field_t DOEPINT3_f;
    };
    uint8_t RESERVED136[4];
    union
    {
        __IO uint32_t DOEPTSIZ3;
        stc_usbfs_doeptsiz_field_t DOEPTSIZ3_f;
    };
    __IO uint32_t DOEPDMA3;
    uint8_t RESERVED138[8];
    union
    {
        __IO uint32_t DOEPCTL4;
        stc_usbfs_doepctl_field_t DOEPCTL4_f;
    };
    uint8_t RESERVED139[4];
    union
    {
        __IO uint32_t DOEPINT4;
        stc_usbfs_doepint_field_t DOEPINT4_f;
    };
    uint8_t RESERVED140[4];
    union
    {
        __IO uint32_t DOEPTSIZ4;
        stc_usbfs_doeptsiz_field_t DOEPTSIZ4_f;
    };
    __IO uint32_t DOEPDMA4;
    uint8_t RESERVED142[8];
    union
    {
        __IO uint32_t DOEPCTL5;
        stc_usbfs_doepctl_field_t DOEPCTL5_f;
    };
    uint8_t RESERVED143[4];
    union
    {
        __IO uint32_t DOEPINT5;
        stc_usbfs_doepint_field_t DOEPINT5_f;
    };
    uint8_t RESERVED144[4];
    union
    {
        __IO uint32_t DOEPTSIZ5;
        stc_usbfs_doeptsiz_field_t DOEPTSIZ5_f;
    };
    __IO uint32_t DOEPDMA5;
    uint8_t RESERVED146[584];
    union
    {
        __IO uint32_t PCGCCTL;
        stc_usbfs_pcgcctl_field_t PCGCCTL_f;
    };
}amhw_hc32f460_usbd_t;


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
 * @}if_HC32F460_USBd
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_USBD_H */

/* end of file */
