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

/**
 * \brief USBFS AHB 控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t GINTMSK                   : 1;  /**< \brief 全局中断屏蔽*/
    __IO uint32_t HBSTLEN                   : 4;  /**< \brief 批量长度/类型*/
    __IO uint32_t DMAEN                     : 1;  /**< \brief DMA使能*/
    uint32_t RESERVED6                      : 1;  /**< \brief 保留*/
    __IO uint32_t TXFELVL                   : 1;  /**< \brief 设备TxFIFO空门限*/
    __IO uint32_t PTXFELVL                  : 1;  /**< \brief 周期性TxFIFO空门限*/
    uint32_t RESERVED9                      :23;  /**< \brief 保留*/
} stc_usbfs_gahbcfg_field_t;

/**
 * \brief USBFS USB 配置寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t TOCAL                     : 3;  /**< \brief */
    uint32_t RESERVED3                      : 3;  /**< \brief 保留 */
    __IO uint32_t PHYSEL                    : 1;  /**< \brief 全速系列收发器选择 */
    uint32_t RESERVED7                      : 3;  /**< \brief 保留 */
    __IO uint32_t TRDT                      : 4;  /**< \brief USB周转时间 */
    uint32_t RESERVED14                     :15;  /**< \brief 保留 */
    __IO uint32_t FHMOD                     : 1;  /**< \brief 强制主机模式 */
    __IO uint32_t FDMOD                     : 1;  /**< \brief 强制设备模式 */
    uint32_t RESERVED31                     : 1;  /**< \brief 保留 */
} stc_usbfs_gusbcfg_field_t;

/**
 * \brief USBFS 复位寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CSRST                     : 1;  /**< \brief 模块软复位 */
    __IO uint32_t HSRST                     : 1;  /**< \brief HCLK域逻辑软复位 */
    __IO uint32_t FCRST                     : 1;  /**< \brief 主机帧计数器复位 */
    uint32_t RESERVED3                      : 1;  /**< \brief 保留 */
    __IO uint32_t RXFFLSH                   : 1;  /**< \brief RxFIFO刷新 */
    __IO uint32_t TXFFLSH                   : 1;  /**< \brief TxFIFO刷新 */
    __IO uint32_t TXFNUM                    : 5;  /**< \brief TxFIFO编号 */
    uint32_t RESERVED11                     :19;  /**< \brief 保留 */
    __IO uint32_t DMAREQ                    : 1;  /**< \brief DMA请求信号 */
    __IO uint32_t AHBIDL                    : 1;  /**< \brief AHB主器件空闲 */
} stc_usbfs_grstctl_field_t;

/**
 * \brief USBFS 全局中断状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CMOD                      : 1;  /**< \brief 当前工作模式 */
    __IO uint32_t MMIS                      : 1;  /**< \brief 模式不匹配中断 */
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t SOF                       : 1;  /**< \brief 帧起始中断 */
    __IO uint32_t RXFLNE                    : 1;  /**< \brief RxFIFO非空中断 */
    __IO uint32_t NPTXFE                    : 1;  /**< \brief 非周期性TxFIFO空中断 */
    __IO uint32_t GINAKEFF                  : 1;  /**< \brief 全局非周期性INNAK有效中断 */
    __IO uint32_t GOUTNAKEFF                : 1;  /**< \brief 全局OUT NAK有效中断 */
    uint32_t RESERVED8                      : 2;  /**< \brief 保留 */
    __IO uint32_t ESUSP                     : 1;  /**< \brief 早期挂起中断 */
    __IO uint32_t USBSUSP                   : 1;  /**< \brief USB挂起中断 */
    __IO uint32_t USBRST                    : 1;  /**< \brief USB复位中断 */
    __IO uint32_t ENUMDNE                   : 1;  /**< \brief 枚举完成中断 */
    __IO uint32_t ISOODRP                   : 1;  /**< \brief 丢弃同步OUT数据包中断 */
    __IO uint32_t EOPF                      : 1;  /**< \brief 周期性帧结束中断 */
    uint32_t RESERVED16                     : 2;  /**< \brief 保留 */
    __IO uint32_t IEPINT                    : 1;  /**< \brief IN端点中断 */
    __IO uint32_t OEPINT                    : 1;  /**< \brief OUT端点中断 */
    __IO uint32_t IISOIXFR                  : 1;  /**< \brief 未完成IN同步传输 */
    __IO uint32_t IPXFR_INCOMPISO           : 1;  /**< \brief 未完成周期性传输/未完成OUT同步传输 */
    __IO uint32_t DATAFSUSP                 : 1;  /**< \brief 数据获取挂起 */
    uint32_t RESERVED23                     : 1;  /**< \brief 保留 */
    __IO uint32_t HPRTINT                   : 1;  /**< \brief 主机端口中断 */
    __IO uint32_t HCINT                     : 1;  /**< \brief 主机通道中断*/
    __IO uint32_t PTXFE                     : 1;  /**< \brief 周期性TxFIFO空中断 */
    uint32_t RESERVED27                     : 1;  /**< \brief 保留 */
    __IO uint32_t CIDSCHG                   : 1;  /**< \brief 连接器ID线状态变化中断 */
    __IO uint32_t DISCINT                   : 1;  /**< \brief 检测到断开连接中断 */
    __IO uint32_t VBUSVINT                  : 1;  /**< \brief VBUS有效中断 */
    __IO uint32_t WKUINT                    : 1;  /**< \brief 检测到恢复/远程唤醒中断 */
} stc_usbfs_gintsts_field_t;

/**
 * \brief USBFS 全局中断屏蔽寄存器位域结构体
 */
typedef struct
{
    uint32_t RESERVED0                      : 1;  /**< \brief 保留 */
    __IO uint32_t MMISM                     : 1;  /**< \brief 模式不匹配中断屏蔽 */
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t SOFM                      : 1;  /**< \brief 保留 */
    __IO uint32_t RXFNEM                    : 1;  /**< \brief RxFIFO非空中断屏蔽 */
    __IO uint32_t NPTXFEM                   : 1;  /**< \brief 非周期性TxFIFO空中断屏蔽 */
    __IO uint32_t GINAKEFFM                 : 1;  /**< \brief 全局非周期性IN NAK有效中断屏蔽 */
    __IO uint32_t GOUTNAKEFFM               : 1;  /**< \brief 全局OUT NAK有效中断屏蔽 */
    uint32_t RESERVED8                      : 2;  /**< \brief 保留 */
    __IO uint32_t ESUSPM                    : 1;  /**< \brief 早期挂起中断屏蔽 */
    __IO uint32_t USBSUSPM                  : 1;  /**< \brief USB挂起中断屏蔽 */
    __IO uint32_t USBRSTM                   : 1;  /**< \brief USB复位中断屏蔽 */
    __IO uint32_t ENUMDNEM                  : 1;  /**< \brief 枚举完成中断屏蔽 */
    __IO uint32_t ISOODRPM                  : 1;  /**< \brief 丢弃同步OUT数据包中断屏蔽 */
    __IO uint32_t EOPFM                     : 1;  /**< \brief 周期性帧结束中断屏蔽 */
    uint32_t RESERVED16                     : 2;  /**< \brief 保留 */
    __IO uint32_t IEPIM                     : 1;  /**< \brief IN端点中断屏蔽 */
    __IO uint32_t OEPIM                     : 1;  /**< \brief OUT端点中断屏蔽 */
    __IO uint32_t IISOIXFRM                 : 1;  /**< \brief 未完成IN同步传输中断屏蔽 */
    __IO uint32_t IPXFRM_INCOMPISOOUTM      : 1;  /**< \brief 未完成周期性传输中断屏蔽/未完成OUT同步传输中断屏蔽 */
    __IO uint32_t DATAFSUSPM                : 1;  /**< \brief 数据获取挂起中断屏蔽 */
    uint32_t RESERVED23                     : 1;  /**< \brief 保留 */
    __IO uint32_t HPRTIM                    : 1;  /**< \brief 主机端口中断屏蔽 */
    __IO uint32_t HCIM                      : 1;  /**< \brief 主机通道中断屏蔽 */
    __IO uint32_t PTXFEM                    : 1;  /**< \brief 周期性TxFIFO空中断屏蔽 */
    uint32_t RESERVED27                     : 1;  /**< \brief 保留 */
    __IO uint32_t CIDSCHGM                  : 1;  /**< \brief 中断连接器ID线状态变化中断屏蔽 */
    __IO uint32_t DISCIM                    : 1;  /**< \brief 检测到断开连接中断屏蔽 */
    __IO uint32_t VBUSVIM                   : 1;  /**< \brief VBUS有效中断屏蔽 */
    __IO uint32_t WKUIM                     : 1;  /**< \brief 检测到恢复/远程唤醒中断屏蔽 */
} stc_usbfs_gintmsk_field_t;

/**
 * \brief USBFS 接收状态调试读取寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CHNUM_EPNUM               : 4;  /**< \brief 通道编号 */
    __IO uint32_t BCNT                      :11;  /**< \brief 字节计数 */
    __IO uint32_t DPID                      : 2;  /**< \brief 数据PID */
    __IO uint32_t PKTSTS                    : 4;  /**< \brief 数据包状态 */
    uint32_t RESERVED21                     :11;  /**< \brief 保留 */
} stc_usbfs_grxstsr_field_t;

/**
 * \brief USBFS 状态读取和出栈寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CHNUM_EPNUM               : 4;  /**< \brief 通道编号 */
    __IO uint32_t BCNT                      :11;  /**< \brief 字节计数 */
    __IO uint32_t DPID                      : 2;  /**< \brief 数据PID */
    __IO uint32_t PKTSTS                    : 4;  /**< \brief 数据包状态 */
    uint32_t RESERVED21                     :11;  /**< \brief 保留 */
} stc_usbfs_grxstsp_field_t;

/**
 * \brief USBFS 接收FIFO大小寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t RXFD                      :11;  /**< \brief RxFIFO深度 */
    uint32_t RESERVED11                     :21;  /**< \brief 保留 */
} stc_usbfs_grxfsiz_field_t;

/**
 * \brief USBFS 主机非周期性发送FIFO大小寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t NPTXFSA                   :16;  /**< \brief 非周期性发送RAM起始地址/端点0发送RAM起始地址 */
    __IO uint32_t NPTXFD                    :16;  /**< \brief 非周期性TxFIFO深度/端点0 TxFIFO深度 */
} stc_usbfs_hnptxfsiz_field_t;

/**
 * \brief USBFS 主机非周期性发送FIFO/队列状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t NPTXFSAV                  :16;  /**< \brief 非周期性发送请求队列顶部 */
    __IO uint32_t NPTQXSAV                  : 8;  /**< \brief 非周期性发送请求队列可用空间 */
    __IO uint32_t NPTXQTOP                  : 7;  /**< \brief 非周期性发送请求队列顶部 */
    uint32_t RESERVED31                     : 1;  /**< \brief 保留 */
} stc_usbfs_hnptxsts_field_t;

/**
 * \brief USBFS 主机周期性发送FIFO大小寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t PTXSA                     :12;  /**< \brief 主机周期性TxFIFO起始地址 */
    uint32_t RESERVED12                     : 4;  /**< \brief 保留 */
    __IO uint32_t PTXFD                     :11;  /**< \brief 主机周期性TxFIFO深度 */
    uint32_t RESERVED27                     : 5;  /**< \brief 保留 */
} stc_usbfs_hptxfsiz_field_t;

/**
 * \brief USBFS 设备IN端点发送FIFO大小寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t INEPTXSA                  :12;  /**< \brief IN端点TxFIFOx RAM起始地址 */
    uint32_t RESERVED12                     : 4;  /**< \brief 保留 */
    __IO uint32_t INEPTXFD                  :10;  /**< \brief IN端点TxFIFO深度 */
    uint32_t RESERVED26                     : 6;  /**< \brief 保留 */
} stc_usbfs_dieptxf_field_t;

/**
 * \brief USBFS 主机配置寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t FSLSPCS                   : 2;  /**< \brief FS/LS PHY时钟选择 */
    __IO uint32_t FSLSS                     : 1;  /**< \brief 仅支持FS和LS */
    uint32_t RESERVED3                      :29;  /**< \brief 保留 */
} stc_usbfs_hcfg_field_t;

/**
 * \brief USBFS 主机帧时间间隔寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t FRIVL                     :16;  /**< \brief 帧间隔 */
    uint32_t RESERVED16                     :16;  /**< \brief 保留 */
} stc_usbfs_hfir_field_t;

/**
 * \brief USBFS 主机帧编号/帧剩余时间寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t FRNUM                     :16;  /**< \brief 帧编号 */
    __IO uint32_t FTREM                     :16;  /**< \brief 帧剩余时间 */
} stc_usbfs_hfnum_field_t;

/**
 * \brief USBFS 主机周期性发送FIFO/队列状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t PTXFSAVL                  :16;  /**< \brief 周期性发送数据FIFO可用空间 */
    __IO uint32_t PTXQSAV                   : 8;  /**< \brief 周期性发送请求队列可用空间 */
    __IO uint32_t PTXQTOP                   : 8;  /**< \brief 周期性发送请求队列顶部 */
} stc_usbfs_hptxsts_field_t;

/**
 * \brief USBFS 主机全体通道中断寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t HAINT                     :12;  /**< \brief 通道中断 */
    uint32_t RESERVED12                     :20;  /**< \brief 保留 */
} stc_usbfs_haint_field_t;

/**
 * \brief USBFS 主机全体通道中断屏蔽寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t HAINTM                    :12;  /**< \brief 通道中断屏蔽 */
    uint32_t RESERVED12                     :20;  /**< \brief 保留 */
} stc_usbfs_haintmsk_field_t;

/**
 * \brief USBFS 主机端口控制和状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t PCSTS                     : 1;  /**< \brief 端口连接状态 */
    __IO uint32_t PCDET                     : 1;  /**< \brief 检测到端口连接 */
    __IO uint32_t PENA                      : 1;  /**< \brief 端口使能 */
    __IO uint32_t PENCHNG                   : 1;  /**< \brief 端口使能/禁止变化 */
    uint32_t RESERVED4                      : 2;  /**< \brief 保留 */
    __IO uint32_t PRES                      : 1;  /**< \brief 端口恢复 */
    __IO uint32_t PSUSP                     : 1;  /**< \brief 端口挂起 */
    __IO uint32_t PRST                      : 1;  /**< \brief 端口复位 */
    uint32_t RESERVED9                      : 1;  /**< \brief 保留 */
    __IO uint32_t PLSTS                     : 2;  /**< \brief 端口线状态 */
    __IO uint32_t PWPR                      : 1;  /**< \brief 端口电源 */
    uint32_t RESERVED13                     : 4;  /**< \brief 保留 */
    __IO uint32_t PSPD                      : 2;  /**< \brief 端口速度 */
    uint32_t RESERVED19                     :13;  /**< \brief 保留 */
} stc_usbfs_hprt_field_t;

/**
 * \brief USBFS 主机通道x特性寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t MPSIZ                     :11;  /**< \brief 最大数据包大小*/
    __IO uint32_t EPNUM                     : 4;  /**< \brief 端点编号 */
    __IO uint32_t EPDIR                     : 1;  /**< \brief 端点方向 */
    uint32_t RESERVED16                     : 1;  /**< \brief 保留 */
    __IO uint32_t LSDEV                     : 1;  /**< \brief 低速设备 */
    __IO uint32_t EPTYP                     : 2;  /**< \brief 端点类型 */
    uint32_t RESERVED20                     : 2;  /**< \brief 保留 */
    __IO uint32_t DAD                       : 7;  /**< \brief 设备地址 */
    __IO uint32_t ODDFRM                    : 1;  /**< \brief 奇数帧 */
    __IO uint32_t CHDIS                     : 1;  /**< \brief 通道禁止 */
    __IO uint32_t CHENA                     : 1;  /**< \brief 通道使能 */
} stc_usbfs_hcchar_field_t;

/**
 * \brief USBFS 主机通道x中断寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRC                      : 1;  /**< \brief 传输完成 */
    __IO uint32_t CHH                       : 1;  /**< \brief 通道停止 */
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t STALL                     : 1;  /**< \brief 收到STALL响应 */
    __IO uint32_t NAK                       : 1;  /**< \brief 收到NAK响应 */
    __IO uint32_t ACK                       : 1;  /**< \brief 收到/发出ACK响应 */
    uint32_t RESERVED6                      : 1;  /**< \brief 保留 */
    __IO uint32_t TXERR                     : 1;  /**< \brief 通信事务错误 */
    __IO uint32_t BBERR                     : 1;  /**< \brief 串扰错误 */
    __IO uint32_t FRMOR                     : 1;  /**< \brief 帧溢出错误 */
    __IO uint32_t DTERR                     : 1;  /**< \brief 数据切换错误 */
    uint32_t RESERVED11                     :21;  /**< \brief 保留 */
} stc_usbfs_hcint_field_t;

/**
 * \brief USBFS 主机通道x中断屏蔽寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRCM                     : 1;  /**< \brief 传输完成中断屏蔽 */
    __IO uint32_t CHHM                      : 1;  /**< \brief 通道停止中断屏蔽 */
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t STALLM                    : 1;  /**< \brief 收到STALL响应中断屏蔽 */
    __IO uint32_t NAKM                      : 1;  /**< \brief 收到NAK响应中断屏蔽 */
    __IO uint32_t ACKM                      : 1;  /**< \brief 收到/发出ACK响应中断屏蔽 */
    uint32_t RESERVED6                      : 1;  /**< \brief 保留 */
    __IO uint32_t TXERRM                    : 1;  /**< \brief 通信事务错误中断屏蔽 */
    __IO uint32_t BBERRM                    : 1;  /**< \brief 串扰错误中断屏蔽 */
    __IO uint32_t FRMORM                    : 1;  /**< \brief 帧溢出错误中断屏蔽 */
    __IO uint32_t DTERRM                    : 1;  /**< \brief 数据切换错误中断屏蔽 */
    uint32_t RESERVED11                     :21;  /**< \brief 保留 */
} stc_usbfs_hcintmsk_field_t;

/**
 * \brief USBFS 主机通道x传输大小寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRSIZ                    :19;  /**< \brief 传输大小 */
    __IO uint32_t PKTCNT                    :10;  /**< \brief 数据包计数 */
    __IO uint32_t DPID                      : 2;  /**< \brief 数据PID */
    uint32_t RESERVED31                     : 1;  /**< \brief 保留 */
} stc_usbfs_hctsiz_field_t;

/**
 * \brief USBFS 设备配置寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DSPD                      : 2;  /**< \brief 设备速度 */
    __IO uint32_t NZLSOHSK                  : 1;  /**< \brief 非零长度状态OUT握手信号 */
    uint32_t RESERVED3                      : 1;  /**< \brief 保留 */
    __IO uint32_t DAD                       : 7;  /**< \brief 设备地址 */
    __IO uint32_t PFIVL                     : 2;  /**< \brief 周期性帧间隔 */
    uint32_t RESERVED13                     :19;  /**< \brief 保留 */
} stc_usbfs_dcfg_field_t;

/**
 * \brief USBFS 设备控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t RWUSIG                    : 1;  /**< \brief 发送远程唤醒信号 */
    __IO uint32_t SDIS                      : 1;  /**< \brief 软断连 */
    __IO uint32_t GINSTS                    : 1;  /**< \brief 全局IN NAK状态 */
    __IO uint32_t GONSTS                    : 1;  /**< \brief 全局OUT NAK状态 */
    __IO uint32_t TCTL                      : 3;
    __IO uint32_t SGINAK                    : 1;  /**< \brief 置位全局IN NAK */
    __IO uint32_t CGINAK                    : 1;  /**< \brief 清零全局IN NAK */
    __IO uint32_t SGONAK                    : 1;  /**< \brief 置位全局OUT NAK */
    __IO uint32_t CGONAK                    : 1;  /**< \brief 清零全局OUT NAK */
    __IO uint32_t POPRGDNE                  : 1;  /**< \brief 上电编程完成 */
    uint32_t RESERVED12                     :20;  /**< \brief 保留 */
} stc_usbfs_dctl_field_t;

/**
 * \brief USBFS 设备状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SUSPSTS                   : 1;  /**< \brief 挂起状态 */
    __IO uint32_t ENUMSPD                   : 2;  /**< \brief 枚举速度 */
    __IO uint32_t EERR                      : 1;  /**< \brief 不定错误 */
    uint32_t RESERVED4                      : 4;  /**< \brief 保留 */
    __IO uint32_t FNSOF                     :14;  /**< \brief 接收SOF的帧编号 */
    uint32_t RESERVED22                     :10;  /**< \brief 保留 */
} stc_usbfs_dsts_field_t;

/**
 * \brief USBFS 设备IN端点通用中断屏蔽寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRCM                     : 1;  /**< \brief 传输完成中断屏蔽 */
    __IO uint32_t EPDM                      : 1;  /**< \brief 端点禁止中断屏蔽 */
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t TOM                       : 1;  /**< \brief 超时中断屏蔽 */
    __IO uint32_t ITTXFEMSK                 : 1;  /**< \brief TxFIFO为空时接收到IN令牌中断屏蔽 */
    __IO uint32_t INEPNMM                   : 1;  /**< \brief EP不匹配时接收到IN令牌中断屏蔽 */
    __IO uint32_t IINEPNEM                  : 1;  /**< \brief IN端点NAK有效中断屏蔽 */
    uint32_t RESERVED7                      :25;  /**< \brief 保留 */
} stc_usbfs_diepmsk_field_t;

/**
 * \brief USBFS 设备OUT端点通用中断屏蔽寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRCM                     : 1;  /**< \brief 传输完成中断屏蔽 */
    __IO uint32_t EPDM                      : 1;  /**< \brief 端点禁止中断屏蔽*/
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t STUPM                     : 1;  /**< \brief SETUP阶段完成中断屏蔽 */
    __IO uint32_t OTEPDM                    : 1;  /**< \brief 端点禁止时接收到OUT令牌中断屏蔽 */
    uint32_t RESERVED5                      :27;  /**< \brief 保留 */
} stc_usbfs_doepmsk_field_t;

/**
 * \brief USBFS 设备全体端点中断寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t IEPINT                    : 6;  /**< \brief IN端点中断位 */
    uint32_t RESERVED6                      :10;  /**< \brief 保留 */
    __IO uint32_t OEPINT                    : 6;  /**< \brief OUT端点中断位 */
    uint32_t RESERVED22                     :10;  /**< \brief 保留 */
} stc_usbfs_daint_field_t;

/**
 * \brief USBFS 设备全体端点中断屏蔽寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t IEPINTM                   : 6;  /**< \brief IN端点中断屏蔽位 */
    uint32_t RESERVED6                      :10;  /**< \brief 保留 */
    __IO uint32_t OEPINTM                   : 6;  /**< \brief OUT端点中断屏蔽位 */
    uint32_t RESERVED22                     :10;  /**< \brief 保留 */
} stc_usbfs_daintmsk_field_t;

/**
 * \brief USBFS 设备IN端点FIFO空中断屏蔽寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t INEPTXFEM                 : 6;  /**< \brief IN EP TxFIFO空中断屏蔽位 */
    uint32_t RESERVED6                      :26;  /**< \brief 保留 */
} stc_usbfs_diepempmsk_field_t;

/**
 * \brief USBFS 设备IN端点0控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t MPSIZ                     : 2;  /**< \brief 最大数据包大小 */
    uint32_t RESERVED2                      :13;  /**< \brief 保留 */
    __IO uint32_t USBAEP                    : 1;  /**< \brief USB活动端点 */
    uint32_t RESERVED16                     : 1;  /**< \brief 保留 */
    __IO uint32_t NAKSTS                    : 1;  /**< \brief NAK状态 */
    __IO uint32_t EPTYP                     : 2;  /**< \brief 端点类型 */
    uint32_t RESERVED20                     : 1;  /**< \brief 保留 */
    __IO uint32_t STALL                     : 1;  /**< \brief STALL握手 */
    __IO uint32_t TXFNUM                    : 4;  /**< \brief TxFIFO编号 */
    __IO uint32_t CNAK                      : 1;  /**< \brief 清零NAK位 */
    __IO uint32_t SNAK                      : 1;  /**< \brief 置为NAK位 */
    uint32_t RESERVED28                     : 2;  /**< \brief 保留 */
    __IO uint32_t EPDIS                     : 1;  /**< \brief 端点禁止 */
    __IO uint32_t EPENA                     : 1;  /**< \brief 端点使能 */
} stc_usbfs_diepctl0_field_t;

/**
 * \brief USBFS 设备IN端点x中断寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRC                      : 1;  /**< \brief 传输完成中断 */
    __IO uint32_t EPDISD                    : 1;  /**< \brief 端点禁止中断 */
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t TOC                       : 1;  /**< \brief 超时 */
    __IO uint32_t TTXFE                     : 1;  /**< \brief TxFIFO为空时接收到IN令牌 */
    uint32_t RESERVED5                      : 1;  /**< \brief 保留 */
    __IO uint32_t NEPNE                     : 1;  /**< \brief IN端点NAK有效 */
    __IO uint32_t TXFE                      : 1;  /**< \brief 发送FIFO为空 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_usbfs_diepint0_field_t;

/**
 * \brief USBFS 设备IN端点0传输大小寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRSIZ                    : 7;  /**< \brief 传输大小 */
    uint32_t RESERVED7                      :12;  /**< \brief 保留 */
    __IO uint32_t PKTCNT                    : 2;  /**< \brief 数据包计数 */
    uint32_t RESERVED21                     :11;  /**< \brief 保留 */
} stc_usbfs_dieptsiz0_field_t;

/**
 * \brief USBFS 设备IN端点发送FIFO状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t INEPTFSAV                 :16;  /**< \brief IN端点TxFIFO可用空间 */
    uint32_t RESERVED16                     :16;  /**< \brief 保留 */
} stc_usbfs_dtxfsts0_field_t;

/**
 * \brief USBFS 设备IN端点x控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t MPSIZ                     :11;  /**< \brief 最大数据包大小 */
    uint32_t RESERVED11                     : 4;  /**< \brief 保留 */
    __IO uint32_t USBAEP                    : 1;  /**< \brief USB活动端点 */
    __IO uint32_t EONUM_DPID                : 1;  /**< \brief 偶数/奇数帧端点数据PID */
    __IO uint32_t NAKSTS                    : 1;  /**< \brief NAK状态 */
    __IO uint32_t EPTYP                     : 2;  /**< \brief 端点类型 */
    uint32_t RESERVED20                     : 1;  /**< \brief 保留 */
    __IO uint32_t STALL                     : 1;  /**< \brief STALL握手 */
    __IO uint32_t TXFNUM                    : 4;  /**< \brief TxFIFO编号 */
    __IO uint32_t CNAK                      : 1;  /**< \brief 清零NAK位 */
    __IO uint32_t SNAK                      : 1;  /**< \brief 置为NAK位 */
    __IO uint32_t SD0PID_SEVNFRM            : 1;  /**< \brief 设置DATA0 PID/SEVNFRM */
    __IO uint32_t SODDFRM                   : 1;  /**< \brief 设置奇数帧 */
    __IO uint32_t EPDIS                     : 1;  /**< \brief 端点禁止 */
    __IO uint32_t EPENA                     : 1;  /**< \brief 端点使能 */
} stc_usbfs_diepctl_field_t;

/**
 * \brief USBFS 设备IN端点x中断寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRC                      : 1;  /**< \brief 传输完成中断 */
    __IO uint32_t EPDISD                    : 1;  /**< \brief 端点禁止中断 */
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t TOC                       : 1;  /**< \brief 超时 */
    __IO uint32_t TTXFE                     : 1;  /**< \brief TxFIFO为空时接收到的IN令牌 */
    uint32_t RESERVED5                      : 1;  /**< \brief 保留 */
    __IO uint32_t NEPNE                     : 1;  /**< \brief IN端点NAK有效 */
    __IO uint32_t TXFE                      : 1;  /**< \brief 发送FIFO为空 */
    uint32_t RESERVED8                      :24;  /**< \brief 保留 */
} stc_usbfs_diepint_field_t;

/**
 * \brief USBFS 设备IN端点x传输大小寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRSIZ                    :19;  /**< \brief 传输大小 */
    __IO uint32_t PKTCNT                    :10;  /**< \brief 数据包计数 */
    __IO uint32_t MC                        : 2;
    uint32_t RESERVED31                     : 1;  /**< \brief 保留 */
} stc_usbfs_dieptsiz_field_t;

/**
 * \brief USBFS 设备IN端点发送FIFO状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t INEPTFSAV                 :16;  /**< \brief IN端点TxFIFO可用空间 */
    uint32_t RESERVED16                     :16;  /**< \brief 保留 */
} stc_usbfs_dtxfsts_field_t;

/**
 * \brief USBFS 设备控制OUT端点0控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t MPSIZ                     : 2;  /**< \brief 最大数据包大小 */
    uint32_t RESERVED2                      :13;  /**< \brief 保留 */
    __IO uint32_t USBAEP                    : 1;  /**< \brief USB活动端点 */
    uint32_t RESERVED16                     : 1;  /**< \brief 保留 */
    __IO uint32_t NAKSTS                    : 1;  /**< \brief NAK状态 */
    __IO uint32_t EPTYP                     : 2;  /**< \brief 端点类型 */
    __IO uint32_t SNPM                      : 1;  /**< \brief 监听模式 */
    __IO uint32_t STALL                     : 1;  /**< \brief STALL握手 */
    uint32_t RESERVED22                     : 4;  /**< \brief 保留 */
    __IO uint32_t CNAK                      : 1;  /**< \brief 清零NAK位 */
    __IO uint32_t SNAK                      : 1;  /**< \brief 置为NAK位 */
    uint32_t RESERVED28                     : 2;  /**< \brief 保留 */
    __IO uint32_t EPDIS                     : 1;  /**< \brief 端点禁止 */
    __IO uint32_t EPENA                     : 1;  /**< \brief 端点使能 */
} stc_usbfs_doepctl0_field_t;

/**
 * \brief USBFS 设备OUT端点x中断寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRC                      : 1;  /**< \brief 传输完成中断 */
    __IO uint32_t EPDISD                    : 1;  /**< \brief 端点禁止中断 */
    uint32_t RESERVED2                      : 1;  /**< \brief 保留 */
    __IO uint32_t STUP                      : 1;  /**< \brief SETUP阶段完成 */
    __IO uint32_t OTEPDIS                   : 1;  /**< \brief 端点禁止时接收到OUT令牌 */
    uint32_t RESERVED5                      : 1;  /**< \brief 保留 */
    __IO uint32_t B2BSTUP                   : 1;  /**< \brief 连接到连续的SETUP数据包 */
    uint32_t RESERVED7                      :25;  /**< \brief 保留 */
} stc_usbfs_doepint_field_t;

/**
 * \brief USBFS 设备OUT端点0传输大小寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRSIZ                    : 7;  /**< \brief 传输大小 */
    uint32_t RESERVED7                      :12;  /**< \brief 保留 */
    __IO uint32_t PKTCNT                    : 1;  /**< \brief 数据包计数 */
    uint32_t RESERVED20                     : 9;  /**< \brief 保留 */
    __IO uint32_t STUPCNT                   : 2;  /**< \brief SETUP数据包计数 */
    uint32_t RESERVED31                     : 1;  /**< \brief 保留 */
} stc_usbfs_doeptsiz0_field_t;

/**
 * \brief USBFS 设备OUT端点x控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t MPSIZ                     :11;  /**< \brief 最大数据包大小 */
    uint32_t RESERVED11                     : 4;  /**< \brief 保留 */
    __IO uint32_t USBAEP                    : 1;  /**< \brief USB活动端点 */
    __IO uint32_t DPID                      : 1;  /**< \brief 偶数/奇数帧/端点数据PID */
    __IO uint32_t NAKSTS                    : 1;  /**< \brief NAK状态 */
    __IO uint32_t EPTYP                     : 2;  /**< \brief 端点类型 */
    __IO uint32_t SNPM                      : 1;  /**< \brief 监听模式 */
    __IO uint32_t STALL                     : 1;  /**< \brief STALL握手 */
    uint32_t RESERVED22                     : 4;  /**< \brief 保留 */
    __IO uint32_t CNAK                      : 1;  /**< \brief 清零NAK位 */
    __IO uint32_t SNAK                      : 1;  /**< \brief 置为NAK位 */
    __IO uint32_t SD0PID                    : 1;  /**< \brief 设置DATA0 PID/SEVNFRM */
    __IO uint32_t SD1PID                    : 1;  /**< \brief 设置DATA1 PID/SEVNFRM */
    __IO uint32_t EPDIS                     : 1;  /**< \brief 端点禁止 */
    __IO uint32_t EPENA                     : 1;  /**< \brief 端点使能 */
} stc_usbfs_doepctl_field_t;

/**
 * \brief USBFS 设备OUT端点x传输大小寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t XFRSIZ                    :19;  /**< \brief 传输大小 */
    __IO uint32_t PKTCNT                    :10;  /**< \brief 数据包计数 */
    uint32_t RESERVED29                     : 3;  /**< \brief 保留 */
} stc_usbfs_doeptsiz_field_t;

/**
 * \brief USBFS 时钟门控控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t STPPCLK                   : 1;  /**< \brief 停止PHY时钟 */
    __IO uint32_t GATEHCLK                  : 1;  /**< \brief 门控HCLK */
    uint32_t RESERVED2                      :30;  /**< \brief 保留 */
} stc_usbfs_pcgcctl_field_t;

/**
 *  \brief USB 寄存器结构体定义
 */
typedef struct
{
    uint8_t RESERVED0[8];                   /**< \brief 保留*/
    union
    {
        __IO uint32_t GAHBCFG;              /**< \brief AHB控制寄存器*/
        stc_usbfs_gahbcfg_field_t GAHBCFG_f;
    };
    union
    {
        __IO uint32_t GUSBCFG;              /**< \brief USB配置寄存器*/
        stc_usbfs_gusbcfg_field_t GUSBCFG_f;
    };
    union
    {
        __IO uint32_t GRSTCTL;              /**< \brief 复位寄存器*/
        stc_usbfs_grstctl_field_t GRSTCTL_f;
    };
    union
    {
        __IO uint32_t GINTSTS;              /**< \brief 全局中断状态寄存器*/
        stc_usbfs_gintsts_field_t GINTSTS_f;
    };
    union
    {
        __IO uint32_t GINTMSK;              /**< \brief 全局中断屏蔽寄存器*/
        stc_usbfs_gintmsk_field_t GINTMSK_f;
    };
    union
    {
        __IO uint32_t GRXSTSR;              /**< \brief 接收状态调试读取寄存器*/
        stc_usbfs_grxstsr_field_t GRXSTSR_f;
    };
    union
    {
        __IO uint32_t GRXSTSP;              /**< \brief 接收状态读取和出栈寄存器*/
        stc_usbfs_grxstsp_field_t GRXSTSP_f;
    };
    union
    {
        __IO uint32_t GRXFSIZ;              /**< \brief USBFS接收FIFO大小寄存器*/
        stc_usbfs_grxfsiz_field_t GRXFSIZ_f;
    };
    union
    {
        __IO uint32_t HNPTXFSIZ;            /**< \brief USBFS主机非周期性发送FIFO大小寄存器*/
        stc_usbfs_hnptxfsiz_field_t HNPTXFSIZ_f;
    };
    union
    {
        __IO uint32_t HNPTXSTS;             /**< \brief USBFS非周期性发送状态寄存器*/
        stc_usbfs_hnptxsts_field_t HNPTXSTS_f;
    };
    uint8_t RESERVED10[12];                 /**< \brief 保留*/
    __IO uint32_t CID;                      /**< \brief USBFS模块ID寄存器*/
    uint8_t RESERVED11[192];                /**< \brief 保留*/
    union
    {
        __IO uint32_t HPTXFSIZ;             /**< \brief USBFS周期性发送FIFO大小寄存器*/
        stc_usbfs_hptxfsiz_field_t HPTXFSIZ_f;
    };
    union
    {
        __IO uint32_t DIEPTXF1;             /**< \brief USBFS设备IN端点1发送FIFO大小寄存器*/
        stc_usbfs_dieptxf_field_t DIEPTXF1_f;
    };
    union
    {
        __IO uint32_t DIEPTXF2;             /**< \brief USBFS设备IN端点2发送FIFO大小寄存器*/
        stc_usbfs_dieptxf_field_t DIEPTXF2_f;
    };
    union
    {
        __IO uint32_t DIEPTXF3;             /**< \brief USBFS设备IN端点3发送FIFO大小寄存器*/
        stc_usbfs_dieptxf_field_t DIEPTXF3_f;
    };
    union
    {
        __IO uint32_t DIEPTXF4;             /**< \brief USBFS设备IN端点4发送FIFO大小寄存器*/
        stc_usbfs_dieptxf_field_t DIEPTXF4_f;
    };
    union
    {
        __IO uint32_t DIEPTXF5;             /**< \brief USBFS设备IN端点5发送FIFO大小寄存器*/
        stc_usbfs_dieptxf_field_t DIEPTXF5_f;
    };
    uint8_t RESERVED17[744];                /**< \brief 保留*/
    union
    {
        __IO uint32_t HCFG;                 /**< \brief USBFS主机配置寄存器*/
        stc_usbfs_hcfg_field_t HCFG_f;
    };
    union
    {
        __IO uint32_t HFIR;                 /**< \brief USBFS主机帧时间间隔寄存器*/
        stc_usbfs_hfir_field_t HFIR_f;
    };
    union
    {
        __IO uint32_t HFNUM;                /**< \brief USBFS主机帧编号/剩余时间间隔寄存器*/
        stc_usbfs_hfnum_field_t HFNUM_f;
    };
    uint8_t RESERVED20[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HPTXSTS;              /**< \brief USBFS主机周期性发送FIFO/队列状态寄存器*/
        stc_usbfs_hptxsts_field_t HPTXSTS_f;
    };
    union
    {
        __IO uint32_t HAINT;                /**< \brief USBFS主机全体通道中断寄存器*/
        stc_usbfs_haint_field_t HAINT_f;
    };
    union
    {
        __IO uint32_t HAINTMSK;             /**< \brief USBFS主机全体通道中断屏蔽寄存器*/
        stc_usbfs_haintmsk_field_t HAINTMSK_f;
    };
    uint8_t RESERVED23[36];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t HPRT;                 /**< \brief 主机端口控制和状态寄存器*/
        stc_usbfs_hprt_field_t HPRT_f;
    };
    uint8_t RESERVED24[188];                /**< \brief 保留*/
    union
    {
        __IO uint32_t HCCHAR0;              /**< \brief 主机通道0特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR0_f;
    };
    uint8_t RESERVED25[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT0;               /**< \brief 主机通道0中断寄存器*/
        stc_usbfs_hcint_field_t HCINT0_f;
    };
    union
    {
        __IO uint32_t HCINTMSK0;            /**< \brief 主机通道0中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK0_f;
    };
    union
    {
        __IO uint32_t HCTSIZ0;              /**< \brief 主机通道0传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ0_f;
    };
    __IO uint32_t HCDMA0;                   /**< \brief 主机通道0DMA地址寄存器*/
    uint8_t RESERVED29[8];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCCHAR1;              /**< \brief 主机通道1特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR1_f;
    };
    uint8_t RESERVED30[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT1;               /**< \brief 主机通道1中断寄存器*/
        stc_usbfs_hcint_field_t HCINT1_f;
    };
    union
    {
        __IO uint32_t HCINTMSK1;            /**< \brief 主机通道1中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK1_f;
    };
    union
    {
        __IO uint32_t HCTSIZ1;              /**< \brief 主机通道1传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ1_f;
    };
    __IO uint32_t HCDMA1;                   /**< \brief 主机通道1DMA地址寄存器*/
    uint8_t RESERVED34[8];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCCHAR2;              /**< \brief 主机通道2特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR2_f;
    };
    uint8_t RESERVED35[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT2;               /**< \brief 主机通道2中断寄存器*/
        stc_usbfs_hcint_field_t HCINT2_f;
    };
    union
    {
        __IO uint32_t HCINTMSK2;            /**< \brief 主机通道2中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK2_f;
    };
    union
    {
        __IO uint32_t HCTSIZ2;              /**< \brief 主机通道2传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ2_f;
    };
    __IO uint32_t HCDMA2;                   /**< \brief 主机通道2DMA地址寄存器*/
    uint8_t RESERVED39[8];
    union
    {
        __IO uint32_t HCCHAR3;              /**< \brief 主机通道3特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR3_f;
    };
    uint8_t RESERVED40[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT3;               /**< \brief 主机通道3中断寄存器*/
        stc_usbfs_hcint_field_t HCINT3_f;
    };
    union
    {
        __IO uint32_t HCINTMSK3;            /**< \brief 主机通道3中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK3_f;
    };
    union
    {
        __IO uint32_t HCTSIZ3;              /**< \brief 主机通道3传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ3_f;
    };
    __IO uint32_t HCDMA3;                   /**< \brief 主机通道3DMA地址寄存器*/
    uint8_t RESERVED44[8];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCCHAR4;              /**< \brief 主机通道4特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR4_f;
    };
    uint8_t RESERVED45[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT4;               /**< \brief 主机通道4中断寄存器*/
        stc_usbfs_hcint_field_t HCINT4_f;
    };
    union
    {
        __IO uint32_t HCINTMSK4;            /**< \brief 主机通道4中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK4_f;
    };
    union
    {
        __IO uint32_t HCTSIZ4;              /**< \brief 主机通道4传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ4_f;
    };
    __IO uint32_t HCDMA4;                   /**< \brief 主机通道4DMA地址寄存器*/
    uint8_t RESERVED49[8];
    union
    {
        __IO uint32_t HCCHAR5;              /**< \brief 主机通道5特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR5_f;
    };
    uint8_t RESERVED50[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT5;               /**< \brief 主机通道5中断寄存器*/
        stc_usbfs_hcint_field_t HCINT5_f;
    };
    union
    {
        __IO uint32_t HCINTMSK5;            /**< \brief 主机通道5中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK5_f;
    };
    union
    {
        __IO uint32_t HCTSIZ5;              /**< \brief 主机通道5传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ5_f;
    };
    __IO uint32_t HCDMA5;                   /**< \brief 主机通道5DMA地址寄存器*/
    uint8_t RESERVED54[8];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCCHAR6;              /**< \brief 主机通道6特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR6_f;
    };
    uint8_t RESERVED55[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT6;               /**< \brief 主机通道6中断寄存器*/
        stc_usbfs_hcint_field_t HCINT6_f;
    };
    union
    {
        __IO uint32_t HCINTMSK6;            /**< \brief 主机通道6中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK6_f;
    };
    union
    {
        __IO uint32_t HCTSIZ6;              /**< \brief 主机通道6传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ6_f;
    };
    __IO uint32_t HCDMA6;                   /**< \brief 主机通道6DMA地址寄存器*/
    uint8_t RESERVED59[8];
    union
    {
        __IO uint32_t HCCHAR7;              /**< \brief 主机通道7特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR7_f;
    };
    uint8_t RESERVED60[4];
    union
    {
        __IO uint32_t HCINT7;               /**< \brief 主机通道7中断寄存器*/
        stc_usbfs_hcint_field_t HCINT7_f;
    };
    union
    {
        __IO uint32_t HCINTMSK7;            /**< \brief 主机通道7中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK7_f;
    };
    union
    {
        __IO uint32_t HCTSIZ7;              /**< \brief 主机通道7传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ7_f;
    };
    __IO uint32_t HCDMA7;                   /**< \brief 主机通道7DMA地址寄存器*/
    uint8_t RESERVED64[8];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCCHAR8;              /**< \brief 主机通道8特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR8_f;
    };
    uint8_t RESERVED65[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT8;               /**< \brief 主机通道8中断寄存器*/
        stc_usbfs_hcint_field_t HCINT8_f;
    };
    union
    {
        __IO uint32_t HCINTMSK8;            /**< \brief 主机通道8中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK8_f;
    };
    union
    {
        __IO uint32_t HCTSIZ8;              /**< \brief 主机通道8传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ8_f;
    };
    __IO uint32_t HCDMA8;                   /**< \brief 主机通道8DMA地址寄存器*/
    uint8_t RESERVED69[8];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCCHAR9;              /**< \brief 主机通道9特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR9_f;
    };
    uint8_t RESERVED70[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT9;               /**< \brief 主机通道9中断寄存器*/
        stc_usbfs_hcint_field_t HCINT9_f;
    };
    union
    {
        __IO uint32_t HCINTMSK9;            /**< \brief 主机通道9中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK9_f;
    };
    union
    {
        __IO uint32_t HCTSIZ9;              /**< \brief 主机通道9传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ9_f;
    };
    __IO uint32_t HCDMA9;                   /**< \brief 主机通道9DMA地址寄存器*/
    uint8_t RESERVED74[8];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCCHAR10;             /**< \brief 主机通道10特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR10_f;
    };
    uint8_t RESERVED75[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT10;              /**< \brief 主机通道10中断寄存器*/
        stc_usbfs_hcint_field_t HCINT10_f;
    };
    union
    {
        __IO uint32_t HCINTMSK10;           /**< \brief 主机通道10中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK10_f;
    };
    union
    {
        __IO uint32_t HCTSIZ10;             /**< \brief 主机通道10传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ10_f;
    };
    __IO uint32_t HCDMA10;                  /**< \brief 主机通道10DMA地址寄存器*/
    uint8_t RESERVED79[8];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCCHAR11;             /**< \brief 主机通道11特性寄存器*/
        stc_usbfs_hcchar_field_t HCCHAR11_f;
    };
    uint8_t RESERVED80[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t HCINT11;              /**< \brief 主机通道11中断寄存器*/
        stc_usbfs_hcint_field_t HCINT11_f;
    };
    union
    {
        __IO uint32_t HCINTMSK11;           /**< \brief 主机通道11中断屏蔽寄存器*/
        stc_usbfs_hcintmsk_field_t HCINTMSK11_f;
    };
    union
    {
        __IO uint32_t HCTSIZ11;             /**< \brief 主机通道11传输大小寄存器*/
        stc_usbfs_hctsiz_field_t HCTSIZ11_f;
    };
    __IO uint32_t HCDMA11;                  /**< \brief 主机通道11DMA地址寄存器*/
    uint8_t RESERVED84[392];                /**< \brief 保留*/
    union
    {
        __IO uint32_t DCFG;                 /**< \brief USBFS设备配置寄存器*/
        stc_usbfs_dcfg_field_t DCFG_f;
    };
    union
    {
        __IO uint32_t DCTL;                 /**< \brief USBFS设备控制寄存器*/
        stc_usbfs_dctl_field_t DCTL_f;
    };
    union
    {
        __IO uint32_t DSTS;                 /**< \brief USBFS设备状态寄存器*/
        stc_usbfs_dsts_field_t DSTS_f;
    };
    uint8_t RESERVED87[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPMSK;              /**< \brief USBFS设备IN端点通用中断屏蔽寄存器*/
        stc_usbfs_diepmsk_field_t DIEPMSK_f;
    };
    union
    {
        __IO uint32_t DOEPMSK;              /**< \brief USBFS设备OUT端点通用中断屏蔽寄存器*/
        stc_usbfs_doepmsk_field_t DOEPMSK_f;
    };
    union
    {
        __IO uint32_t DAINT;                /**< \brief USBFS设备全体端点中断寄存器*/
        stc_usbfs_daint_field_t DAINT_f;
    };
    union
    {
        __IO uint32_t DAINTMSK;             /**< \brief USBFS设备全体端点中断屏蔽寄存器*/
        stc_usbfs_daintmsk_field_t DAINTMSK_f;
    };
    uint8_t RESERVED91[20];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPEMPMSK;           /**< \brief USBFS设备IN端点FIFO空中断屏蔽寄存器*/
        stc_usbfs_diepempmsk_field_t DIEPEMPMSK_f;
    };
    uint8_t RESERVED92[200];                /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPCTL0;             /**< \brief USBFS设备IN端点0控制寄存器*/
        stc_usbfs_diepctl0_field_t DIEPCTL0_f;
    };
    uint8_t RESERVED93[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPINT0;             /**< \brief USBFS设备IN端点0中断寄存器*/
        stc_usbfs_diepint0_field_t DIEPINT0_f;
    };
    uint8_t RESERVED94[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPTSIZ0;            /**< \brief USBFS设备IN端点0传输大小寄存器*/
        stc_usbfs_dieptsiz0_field_t DIEPTSIZ0_f;
    };
    __IO uint32_t DIEPDMA0;                 /**< \brief USBFS设备IN端点0DMA地址寄存器*/
    union
    {
        __IO uint32_t DTXFSTS0;             /**< \brief USBFS设备IN端点0发送FIFO状态寄存器*/
        stc_usbfs_dtxfsts0_field_t DTXFSTS0_f;
    };
    uint8_t RESERVED97[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPCTL1;             /**< \brief USBFS设备IN端点1控制寄存器*/
        stc_usbfs_diepctl_field_t DIEPCTL1_f;
    };
    uint8_t RESERVED98[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPINT1;             /**< \brief USBFS设备IN端点1中断寄存器*/
        stc_usbfs_diepint_field_t DIEPINT1_f;
    };
    uint8_t RESERVED99[4];                  /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPTSIZ1;            /**< \brief USBFS设备IN端点1传输大小寄存器*/
        stc_usbfs_dieptsiz_field_t DIEPTSIZ1_f;
    };
    __IO uint32_t DIEPDMA1;                 /**< \brief USBFS设备IN端点1DMA地址寄存器*/
    union
    {
        __IO uint32_t DTXFSTS1;             /**< \brief USBFS设备IN端点1发送FIFO状态寄存器*/
        stc_usbfs_dtxfsts_field_t DTXFSTS1_f;
    };
    uint8_t RESERVED102[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPCTL2;             /**< \brief USBFS设备IN端点1控制寄存器*/
        stc_usbfs_diepctl_field_t DIEPCTL2_f;
    };
    uint8_t RESERVED103[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPINT2;             /**< \brief USBFS设备IN端点2中断寄存器*/
        stc_usbfs_diepint_field_t DIEPINT2_f;
    };
    uint8_t RESERVED104[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPTSIZ2;            /**< \brief USBFS设备IN端点2传输大小寄存器*/
        stc_usbfs_dieptsiz_field_t DIEPTSIZ2_f;
    };
    __IO uint32_t DIEPDMA2;                 /**< \brief USBFS设备IN端点2DMA地址寄存器*/
    union
    {
        __IO uint32_t DTXFSTS2;             /**< \brief USBFS设备IN端点2发送FIFO状态寄存器*/
        stc_usbfs_dtxfsts_field_t DTXFSTS2_f;
    };
    uint8_t RESERVED107[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPCTL3;             /**< \brief USBFS设备IN端点3控制寄存器*/
        stc_usbfs_diepctl_field_t DIEPCTL3_f;
    };
    uint8_t RESERVED108[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPINT3;             /**< \brief USBFS设备IN端点3中断寄存器*/
        stc_usbfs_diepint_field_t DIEPINT3_f;
    };
    uint8_t RESERVED109[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPTSIZ3;            /**< \brief USBFS设备IN端点3传输大小寄存器*/
        stc_usbfs_dieptsiz_field_t DIEPTSIZ3_f;
    };
    __IO uint32_t DIEPDMA3;                 /**< \brief USBFS设备IN端点3DMA地址寄存器*/
    union
    {
        __IO uint32_t DTXFSTS3;             /**< \brief USBFS设备IN端点3发送FIFO状态寄存器*/
        stc_usbfs_dtxfsts_field_t DTXFSTS3_f;
    };
    uint8_t RESERVED112[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPCTL4;             /**< \brief USBFS设备IN端点4控制寄存器*/
        stc_usbfs_diepctl_field_t DIEPCTL4_f;
    };
    uint8_t RESERVED113[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPINT4;             /**< \brief USBFS设备IN端点4中断寄存器*/
        stc_usbfs_diepint_field_t DIEPINT4_f;
    };
    uint8_t RESERVED114[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPTSIZ4;            /**< \brief USBFS设备IN端点4传输大小寄存器*/
        stc_usbfs_dieptsiz_field_t DIEPTSIZ4_f;
    };
    __IO uint32_t DIEPDMA4;                 /**< \brief USBFS设备IN端点4DMA地址寄存器*/
    union
    {
        __IO uint32_t DTXFSTS4;             /**< \brief USBFS设备IN端点4发送FIFO状态寄存器*/
        stc_usbfs_dtxfsts_field_t DTXFSTS4_f;
    };
    uint8_t RESERVED117[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPCTL5;             /**< \brief USBFS设备IN端点5控制寄存器*/
        stc_usbfs_diepctl_field_t DIEPCTL5_f;
    };
    uint8_t RESERVED118[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPINT5;             /**< \brief USBFS设备IN端点5中断寄存器*/
        stc_usbfs_diepint_field_t DIEPINT5_f;
    };
    uint8_t RESERVED119[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DIEPTSIZ5;            /**< \brief USBFS设备IN端点5传输大小寄存器*/
        stc_usbfs_dieptsiz_field_t DIEPTSIZ5_f;
    };
    __IO uint32_t DIEPDMA5;                 /**< \brief USBFS设备IN端点5DMA地址寄存器*/
    union
    {
        __IO uint32_t DTXFSTS5;             /**< \brief USBFS设备IN端点5发送FIFO状态寄存器*/
        stc_usbfs_dtxfsts_field_t DTXFSTS5_f;
    };
    uint8_t RESERVED122[324];               /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPCTL0;             /**< \brief 设备OUT端点0控制寄存器*/
        stc_usbfs_doepctl0_field_t DOEPCTL0_f;
    };
    uint8_t RESERVED123[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPINT0;             /**< \brief 设备OUT端点0中断寄存器*/
        stc_usbfs_doepint_field_t DOEPINT0_f;
    };
    uint8_t RESERVED124[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPTSIZ0;            /**< \brief 设备OUT端点0传输大小寄存器*/
        stc_usbfs_doeptsiz0_field_t DOEPTSIZ0_f;
    };
    __IO uint32_t DOEPDMA0;                 /**< \brief 设备OUT端点0DMA地址寄存器*/
    uint8_t RESERVED126[8];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPCTL1;             /**< \brief 设备OUT端点1控制寄存器*/
        stc_usbfs_doepctl_field_t DOEPCTL1_f;
    };
    uint8_t RESERVED127[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPINT1;             /**< \brief 设备OUT端点1中断寄存器*/
        stc_usbfs_doepint_field_t DOEPINT1_f;
    };
    uint8_t RESERVED128[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPTSIZ1;            /**< \brief 设备OUT端点1传输大小寄存器*/
        stc_usbfs_doeptsiz_field_t DOEPTSIZ1_f;
    };
    __IO uint32_t DOEPDMA1;                 /**< \brief 设备OUT端点1DMA地址寄存器*/
    uint8_t RESERVED130[8];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPCTL2;             /**< \brief 设备OUT端点2控制寄存器*/
        stc_usbfs_doepctl_field_t DOEPCTL2_f;
    };
    uint8_t RESERVED131[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPINT2;             /**< \brief 设备OUT端点2中断寄存器*/
        stc_usbfs_doepint_field_t DOEPINT2_f;
    };
    uint8_t RESERVED132[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPTSIZ2;            /**< \brief 设备OUT端点2传输大小寄存器*/
        stc_usbfs_doeptsiz_field_t DOEPTSIZ2_f;
    };
    __IO uint32_t DOEPDMA2;                 /**< \brief 设备OUT端点2DMA地址寄存器*/
    uint8_t RESERVED134[8];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPCTL3;             /**< \brief 设备OUT端点2控制寄存器*/
        stc_usbfs_doepctl_field_t DOEPCTL3_f;
    };
    uint8_t RESERVED135[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPINT3;             /**< \brief 设备OUT端点3中断寄存器*/
        stc_usbfs_doepint_field_t DOEPINT3_f;
    };
    uint8_t RESERVED136[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPTSIZ3;            /**< \brief 设备OUT端点3传输大小寄存器*/
        stc_usbfs_doeptsiz_field_t DOEPTSIZ3_f;
    };
    __IO uint32_t DOEPDMA3;                 /**< \brief 设备OUT端点3DMA地址寄存器*/
    uint8_t RESERVED138[8];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPCTL4;             /**< \brief 设备OUT端点2控制寄存器*/
        stc_usbfs_doepctl_field_t DOEPCTL4_f;
    };
    uint8_t RESERVED139[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPINT4;             /**< \brief 设备OUT端点4中断寄存器*/
        stc_usbfs_doepint_field_t DOEPINT4_f;
    };
    uint8_t RESERVED140[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPTSIZ4;            /**< \brief 设备OUT端点4传输大小寄存器*/
        stc_usbfs_doeptsiz_field_t DOEPTSIZ4_f;
    };
    __IO uint32_t DOEPDMA4;                 /**< \brief 设备OUT端点4DMA地址寄存器*/
    uint8_t RESERVED142[8];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPCTL5;             /**< \brief 设备OUT端点2控制寄存器*/
        stc_usbfs_doepctl_field_t DOEPCTL5_f;
    };
    uint8_t RESERVED143[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPINT5;             /**< \brief 设备OUT端点5中断寄存器*/
        stc_usbfs_doepint_field_t DOEPINT5_f;
    };
    uint8_t RESERVED144[4];                 /**< \brief 保留*/
    union
    {
        __IO uint32_t DOEPTSIZ5;            /**< \brief 设备OUT端点5传输大小寄存器*/
        stc_usbfs_doeptsiz_field_t DOEPTSIZ5_f;
    };
    __IO uint32_t DOEPDMA5;                 /**< \brief 设备OUT端点5DMA地址寄存器*/
    uint8_t RESERVED146[584];               /**< \brief 保留*/
    union
    {
        __IO uint32_t PCGCCTL;              /**< \brief USBFS电源和门控时钟控制急促器*/
        stc_usbfs_pcgcctl_field_t PCGCCTL_f;
    };
} amhw_hc32f460_usbd_t;


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
