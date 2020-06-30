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
 * \brief 高级定时器ADTIM接口
 *
 * \internal
 * \par History
 * - 1.00 19-10-08  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32F460_ADTIM_H
#define __AMHW_HC32F460_ADTIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "hc32f460_regbase.h"

/**
 * \addtogroup amhw_hc32f460_if_adtim
 * \copydoc amhw_hc32f460_adtim.h
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
 * \brief TMR6 - 通用计数值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CNT                       :16;  /**< \brief 当前定时器的计数值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_cnter_field_t;

/**
 * \brief TMR6 - 通用周期基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t PERA                      :16;  /**< \brief 计数周期值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_perar_field_t;

/**
 * \brief TMR6 - 通用周期基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t PERB                      :16;  /**< \brief 计数周期值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_perbr_field_t;

/**
 * \brief TMR6 - 通用周期基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t PERC                      :16;  /**< \brief 计数周期值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_percr_field_t;

/**
 * \brief TMR6 - 通用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t GCMA                      :16;  /**< \brief 计数比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_gcmar_field_t;

/**
 * \brief TMR6 - 通用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t GCMB                      :16;  /**< \brief 计数比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_gcmbr_field_t;

/**
 * \brief TMR6 - 通用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t GCMC                      :16;  /**< \brief 计数比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_gcmcr_field_t;

/**
 * \brief TMR6 - 通用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t GCMD                      :16;  /**< \brief 计数比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_gcmdr_field_t;

/**
 * \brief TMR6 - 通用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t GCME                      :16;  /**< \brief 计数比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_gcmer_field_t;

/**
 * \brief TMR6 - 通用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t GCMF                      :16;  /**< \brief 计数比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_gcmfr_field_t;

/**
 * \brief TMR6 - 专用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SCMA                      :16;  /**< \brief 专用比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_scmar_field_t;

/**
 * \brief TMR6 - 专用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SCMB                      :16;  /**< \brief 专用比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_scmbr_field_t;

/**
 * \brief TMR6 - 专用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SCMC                      :16;  /**< \brief 专用比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_scmcr_field_t;

/**
 * \brief TMR6 - 专用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SCMD                      :16;  /**< \brief 专用比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_scmdr_field_t;

/**
 * \brief TMR6 - 专用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SCME                      :16;  /**< \brief 专用比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_scmer_field_t;

/**
 * \brief TMR6 - 专用比较基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SCMF                      :16;  /**< \brief 专用比较基准值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_scmfr_field_t;

/**
 * \brief TMR6 - 死区时间基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DTUA                      :16;  /**< \brief 死区时间值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_dtuar_field_t;

/**
 * \brief TMR6 - 死区时间基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DTDA                      :16;  /**< \brief 死区时间值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_dtdar_field_t;

/**
 * \brief TMR6 - 死区时间基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DTUB                      :16;  /**< \brief 死区时间值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_dtubr_field_t;

/**
 * \brief TMR6 - 死区时间基准值寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DTDB                      :16;  /**< \brief 死区时间值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_tmr6_dtdbr_field_t;

/**
 * \brief TMR6 - 通用控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t START                     : 1;  /**< \brief 定时器启动*/
    __IO uint32_t MODE                      : 3;  /**< \brief 计数模式*/
    __IO uint32_t CKDIV                     : 3;  /**< \brief 计数时钟选择*/
    uint32_t RESERVED7                      : 1;  /**< \brief 保留*/
    __IO uint32_t DIR                       : 1;  /**< \brief 计数方向*/
    uint32_t RESERVED9                      : 7;  /**< \brief 保留*/
    __IO uint32_t ZMSKREV                   : 1;  /**< \brief Z相输入公转定时器选择*/
    __IO uint32_t ZMSKPOS                   : 1;  /**< \brief Z相输入位置定时器选择*/
    __IO uint32_t ZMSKVAL                   : 2;  /**< \brief Z相输入屏蔽周期数*/
    uint32_t RESERVED20                     :12;  /**< \brief 保留*/
} stc_tmr6_gconr_field_t;

/**
 * \brief TMR6 - 中断控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t INTENA                    : 1;  /**< \brief 计数匹配中断使能A*/
    __IO uint32_t INTENB                    : 1;  /**< \brief 计数匹配中断使能B*/
    __IO uint32_t INTENC                    : 1;  /**< \brief 计数匹配中断使能C*/
    __IO uint32_t INTEND                    : 1;  /**< \brief 计数匹配中断使能D*/
    __IO uint32_t INTENE                    : 1;  /**< \brief 计数匹配中断使能E*/
    __IO uint32_t INTENF                    : 1;  /**< \brief 计数匹配中断使能F*/
    __IO uint32_t INTENOVF                  : 1;  /**< \brief 上溢中断使能*/
    __IO uint32_t INTENUDF                  : 1;  /**< \brief 下溢中断使能*/
    __IO uint32_t INTENDTE                  : 1;  /**< \brief 死区时间错误中断使能*/
    uint32_t RESERVED9                      : 7;  /**< \brief 保留*/
    __IO uint32_t INTENSAU                  : 1;  /**< \brief 专用向上计数中断使能A*/
    __IO uint32_t INTENSAD                  : 1;  /**< \brief 专用向下计数中断使能A*/
    __IO uint32_t INTENSBU                  : 1;  /**< \brief 专用向上计数中断使能B*/
    __IO uint32_t INTENSBD                  : 1;  /**< \brief 专用向下计数中断使能B*/
    uint32_t RESERVED20                     :12;  /**< \brief 保留*/
} stc_tmr6_iconr_field_t;

/**
 * \brief TMR6 - 端口控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CAPMDA                    : 1;  /**< \brief 功能模式选择A*/
    __IO uint32_t STACA                     : 1;  /**< \brief 计数开始端口状态设定A*/
    __IO uint32_t STPCA                     : 1;  /**< \brief 计数停止端口状态A*/
    __IO uint32_t STASTPSA                  : 1;  /**< \brief 计数开始停止端口状态选择A*/
    __IO uint32_t CMPCA                     : 2;  /**< \brief 周期值匹配时端口设定A*/
    __IO uint32_t PERCA                     : 2;  /**< \brief 周期值匹配时端口设定A*/
    __IO uint32_t OUTENA                    : 1;  /**< \brief 输出使能A*/
    __IO uint32_t EMBSELA                   : 2;
    __IO uint32_t EMBVALA                   : 2;  /**< \brief EMB状态控制A*/
    uint32_t RESERVED13                     : 3;  /**< \brief 保留*/
    __IO uint32_t CAPMDB                    : 1;  /**< \brief 功能模式选择B*/
    __IO uint32_t STACB                     : 1;  /**< \brief 计数开始端口状态设定B*/
    __IO uint32_t STPCB                     : 1;  /**< \brief 计数停止端口状态B*/
    __IO uint32_t STASTPSB                  : 1;  /**< \brief 计数开始停止端口状态选择B*/
    __IO uint32_t CMPCB                     : 2;  /**< \brief 比较值匹配时端口设定B*/
    __IO uint32_t PERCB                     : 2;  /**< \brief 周期值匹配时端口设定B*/
    __IO uint32_t OUTENB                    : 1;  /**< \brief 输出使能B*/
    __IO uint32_t EMBSELB                   : 2;
    __IO uint32_t EMBVALB                   : 2;  /**< \brief EMB状态控制B*/
    uint32_t RESERVED29                     : 3;  /**< \brief 保留*/
} stc_tmr6_pconr_field_t;

/**
 * \brief TMR6 - 缓存控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t BENA                      : 1;  /**< \brief 通用比较值缓存传送A使能*/
    __IO uint32_t BSEA                      : 1;  /**< \brief 通用比较值缓存传送选择A*/
    __IO uint32_t BENB                      : 1;  /**< \brief 通用比较值缓存传送B使能*/
    __IO uint32_t BSEB                      : 1;  /**< \brief 通用比较值缓存传送选择B*/
    uint32_t RESERVED4                      : 4;  /**< \brief 保留*/
    __IO uint32_t BENP                      : 1;  /**< \brief 周期值缓存传送使能*/
    __IO uint32_t BSEP                      : 1;  /**< \brief 周期值缓存传送*/
    uint32_t RESERVED10                     : 6;  /**< \brief 保留*/
    __IO uint32_t BENSPA                    : 1;  /**< \brief 专用比较基准值缓存传送使能A*/
    __IO uint32_t BSESPA                    : 1;  /**< \brief 专用比较基准值缓存传送选择A*/
    uint32_t RESERVED18                     : 2;  /**< \brief 保留*/
    __IO uint32_t BTRSPA                    : 2;  /**< \brief 专用比较基准值缓存传送时间点A*/
    uint32_t RESERVED22                     : 2;  /**< \brief 保留*/
    __IO uint32_t BENSPB                    : 1;  /**< \brief 专用比较基准值缓存传送使能B*/
    __IO uint32_t BSESPB                    : 1;  /**< \brief 专用比较基准值缓存传送选择B*/
    uint32_t RESERVED26                     : 2;  /**< \brief 保留*/
    __IO uint32_t BTRSPB                    : 2;  /**< \brief 专用比较基准值缓存传达时间点B*/
    uint32_t RESERVED30                     : 2;  /**< \brief 保留*/
} stc_tmr6_bconr_field_t;

/**
 * \brief TMR6 - 死区控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t DTCEN                     : 1;  /**< \brief 死区功能使能*/
    uint32_t RESERVED1                      : 3;  /**< \brief 保留*/
    __IO uint32_t DTBENU                    : 1;  /**< \brief 死区时间缓存传送U*/
    __IO uint32_t DTBEND                    : 1;  /**< \brief 死区时间缓存传送D*/
    uint32_t RESERVED6                      : 2;  /**< \brief 保留*/
    __IO uint32_t SEPA                      : 1;  /**< \brief 分离设定*/
    uint32_t RESERVED9                      :23;  /**< \brief 保留*/
} stc_tmr6_dconr_field_t;

/**
 * \brief TMR6 - 滤波控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t NOFIENGA                  : 1;  /**< \brief 捕获输入端口滤波GA*/
    __IO uint32_t NOFICKGA                  : 2;  /**< \brief 滤波采样基准时钟选择GA*/
    uint32_t RESERVED3                      : 1;  /**< \brief 保留*/
    __IO uint32_t NOFIENGB                  : 1;  /**< \brief 捕获输入端口滤波GB*/
    __IO uint32_t NOFICKGB                  : 2;  /**< \brief 滤波采样基准时钟选择GB*/
    uint32_t RESERVED7                      : 9;  /**< \brief 保留*/
    __IO uint32_t NOFIENTA                  : 1;  /**< \brief 捕获输入端口滤波TA*/
    __IO uint32_t NOFICKTA                  : 2;  /**< \brief 滤波采样基准时钟选择TA*/
    uint32_t RESERVED19                     : 1;  /**< \brief 保留*/
    __IO uint32_t NOFIENTB                  : 1;  /**< \brief 捕获输入端口滤波TB*/
    __IO uint32_t NOFICKTB                  : 2;  /**< \brief 滤波采样基准时钟选择TB*/
    uint32_t RESERVED23                     : 9;  /**< \brief 保留*/
} stc_tmr6_fconr_field_t;

/**
 * \brief TMR6 - 有效周期寄存器位域结构体
 */
typedef struct
{
    uint32_t RESERVED0                      : 8;  /**< \brief 保留*/
    __IO uint32_t SPPERIA                   : 1;  /**< \brief 专用信号有效周期选择A*/
    __IO uint32_t SPPERIB                   : 1;  /**< \brief 专用信号有效周期选择B*/
    uint32_t RESERVED10                     : 6;  /**< \brief 保留*/
    __IO uint32_t PCNTE                     : 2;  /**< \brief 有效周期计数条件选择*/
    __IO uint32_t PCNTS                     : 3;  /**< \brief 有效周期选择*/
    uint32_t RESERVED21                     :11;  /**< \brief 保留*/
} stc_tmr6_vperr_field_t;

/**
 * \brief TMR6 - 状态标志寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CMAF                      : 1;  /**< \brief 计数匹配A*/
    __IO uint32_t CMBF                      : 1;  /**< \brief 计数匹配B*/
    __IO uint32_t CMCF                      : 1;  /**< \brief 计数匹配C*/
    __IO uint32_t CMDF                      : 1;  /**< \brief 计数匹配D*/
    __IO uint32_t CMEF                      : 1;  /**< \brief 计数匹配E*/
    __IO uint32_t CMFF                      : 1;  /**< \brief 计数匹配F*/
    __IO uint32_t OVFF                      : 1;  /**< \brief 上溢匹配*/
    __IO uint32_t UDFF                      : 1;  /**< \brief 下溢匹配*/
    __IO uint32_t DTEF                      : 1;  /**< \brief 死区时间错误*/
    __IO uint32_t CMSAUF                    : 1;  /**< \brief 向上计数专用比较基准值匹配A*/
    __IO uint32_t CMSADF                    : 1;  /**< \brief 向下计数专用比较基准值匹配A*/
    __IO uint32_t CMSBUF                    : 1;  /**< \brief 向上计数专用比较基准值匹配B*/
    __IO uint32_t CMSBDF                    : 1;  /**< \brief 向下计数专用比较基准值匹配B*/
    uint32_t RESERVED13                     : 8;  /**< \brief 保留*/
    __IO uint32_t VPERNUM                   : 3;  /**< \brief 周期次数*/
    uint32_t RESERVED24                     : 7;  /**< \brief 保留*/
    __IO uint32_t DIRF                      : 1;  /**< \brief 计数方向*/
} stc_tmr6_stflr_field_t;

/**
 * \brief TMR6 - 硬件启动事件选择寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t HSTA0                     : 1;  /**< \brief 硬件启动条件0*/
    __IO uint32_t HSTA1                     : 1;  /**< \brief 硬件启动条件1*/
    uint32_t RESERVED2                      : 2;  /**< \brief 保留*/
    __IO uint32_t HSTA4                     : 1;  /**< \brief 硬件启动条件4*/
    __IO uint32_t HSTA5                     : 1;  /**< \brief 硬件启动条件5*/
    __IO uint32_t HSTA6                     : 1;  /**< \brief 硬件启动条件6*/
    __IO uint32_t HSTA7                     : 1;  /**< \brief 硬件启动条件7*/
    __IO uint32_t HSTA8                     : 1;  /**< \brief 硬件启动条件8*/
    __IO uint32_t HSTA9                     : 1;  /**< \brief 硬件启动条件9*/
    __IO uint32_t HSTA10                    : 1;  /**< \brief 硬件启动条件10*/
    __IO uint32_t HSTA11                    : 1;  /**< \brief 硬件启动条件11*/
    uint32_t RESERVED12                     :19;  /**< \brief 保留*/
    __IO uint32_t STARTS                    : 1;  /**< \brief 硬件启动使能*/
} stc_tmr6_hstar_field_t;

/**
 * \brief TMR6 - 硬件停止事件选择寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t HSTP0                     : 1;  /**< \brief 硬件停止条件0*/
    __IO uint32_t HSTP1                     : 1;  /**< \brief 硬件停止条件1*/
    uint32_t RESERVED2                      : 2;
    __IO uint32_t HSTP4                     : 1;  /**< \brief 硬件停止条件4*/
    __IO uint32_t HSTP5                     : 1;  /**< \brief 硬件停止条件5*/
    __IO uint32_t HSTP6                     : 1;  /**< \brief 硬件停止条件6*/
    __IO uint32_t HSTP7                     : 1;  /**< \brief 硬件停止条件7*/
    __IO uint32_t HSTP8                     : 1;  /**< \brief 硬件停止条件8*/
    __IO uint32_t HSTP9                     : 1;  /**< \brief 硬件停止条件9*/
    __IO uint32_t HSTP10                    : 1;  /**< \brief 硬件停止条件10*/
    __IO uint32_t HSTP11                    : 1;  /**< \brief 硬件停止条件11*/
    uint32_t RESERVED12                     :19;  /**< \brief 保留*/
    __IO uint32_t STOPS                     : 1;  /**< \brief 硬件停止使能*/
} stc_tmr6_hstpr_field_t;

/**
 * \brief TMR6 - 硬件清零事件选择寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t HCLE0                     : 1;  /**< \brief 硬件清零条件0*/
    __IO uint32_t HCLE1                     : 1;  /**< \brief 硬件清零条件1*/
    uint32_t RESERVED2                      : 2;  /**< \brief 保留*/
    __IO uint32_t HCLE4                     : 1;  /**< \brief 硬件清零条件4*/
    __IO uint32_t HCLE5                     : 1;  /**< \brief 硬件清零条件5*/
    __IO uint32_t HCLE6                     : 1;  /**< \brief 硬件清零条件6*/
    __IO uint32_t HCLE7                     : 1;  /**< \brief 硬件清零条件7*/
    __IO uint32_t HCLE8                     : 1;  /**< \brief 硬件清零条件8*/
    __IO uint32_t HCLE9                     : 1;  /**< \brief 硬件清零条件9*/
    __IO uint32_t HCLE10                    : 1;  /**< \brief 硬件清零条件10*/
    __IO uint32_t HCLE11                    : 1;  /**< \brief 硬件清零条件11*/
    uint32_t RESERVED12                     :19;  /**< \brief 保留*/
    __IO uint32_t CLEARS                    : 1;  /**< \brief 硬件清零使能*/
} stc_tmr6_hclrr_field_t;

/**
 * \brief TMR6 - 硬件捕获事件选择寄存器A位域结构体
 */
typedef struct
{
    __IO uint32_t HCPA0                     : 1;  /**< \brief 硬件捕获A条件0*/
    __IO uint32_t HCPA1                     : 1;  /**< \brief 硬件捕获A条件1*/
    uint32_t RESERVED2                      : 2;  /**< \brief 保留*/
    __IO uint32_t HCPA4                     : 1;  /**< \brief 硬件捕获A条件4*/
    __IO uint32_t HCPA5                     : 1;  /**< \brief 硬件捕获A条件5*/
    __IO uint32_t HCPA6                     : 1;  /**< \brief 硬件捕获A条件6*/
    __IO uint32_t HCPA7                     : 1;  /**< \brief 硬件捕获A条件7*/
    __IO uint32_t HCPA8                     : 1;  /**< \brief 硬件捕获A条件8*/
    __IO uint32_t HCPA9                     : 1;  /**< \brief 硬件捕获A条件9*/
    __IO uint32_t HCPA10                    : 1;  /**< \brief 硬件捕获A条件10*/
    __IO uint32_t HCPA11                    : 1;  /**< \brief 硬件捕获A条件11*/
    uint32_t RESERVED12                     :20;  /**< \brief 保留*/
} stc_tmr6_hcpar_field_t;

/**
 * \brief TMR6 - 硬件捕获事件选择寄存器B位域结构体
 */
typedef struct
{
    __IO uint32_t HCPB0                     : 1;  /**< \brief 硬件捕获B条件0*/
    __IO uint32_t HCPB1                     : 1;  /**< \brief 硬件捕获B条件1*/
    uint32_t RESERVED2                      : 2;  /**< \brief 保留*/
    __IO uint32_t HCPB4                     : 1;  /**< \brief 硬件捕获B条件4*/
    __IO uint32_t HCPB5                     : 1;  /**< \brief 硬件捕获B条件5*/
    __IO uint32_t HCPB6                     : 1;  /**< \brief 硬件捕获B条件6*/
    __IO uint32_t HCPB7                     : 1;  /**< \brief 硬件捕获B条件7*/
    __IO uint32_t HCPB8                     : 1;  /**< \brief 硬件捕获B条件8*/
    __IO uint32_t HCPB9                     : 1;  /**< \brief 硬件捕获B条件9*/
    __IO uint32_t HCPB10                    : 1;  /**< \brief 硬件捕获B条件10*/
    __IO uint32_t HCPB11                    : 1;  /**< \brief 硬件捕获B条件11*/
    uint32_t RESERVED12                     :20;  /**< \brief 保留*/
} stc_tmr6_hcpbr_field_t;

/**
 * \brief TMR6 - 硬件递加事件选择寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t HCUP0                     : 1;  /**< \brief 硬件递加条件0*/
    __IO uint32_t HCUP1                     : 1;  /**< \brief 硬件递加条件1*/
    __IO uint32_t HCUP2                     : 1;  /**< \brief 硬件递加条件2*/
    __IO uint32_t HCUP3                     : 1;  /**< \brief 硬件递加条件3*/
    __IO uint32_t HCUP4                     : 1;  /**< \brief 硬件递加条件4*/
    __IO uint32_t HCUP5                     : 1;  /**< \brief 硬件递加条件5*/
    __IO uint32_t HCUP6                     : 1;  /**< \brief 硬件递加条件6*/
    __IO uint32_t HCUP7                     : 1;  /**< \brief 硬件递加条件7*/
    __IO uint32_t HCUP8                     : 1;  /**< \brief 硬件递加条件8*/
    __IO uint32_t HCUP9                     : 1;  /**< \brief 硬件递加条件9*/
    __IO uint32_t HCUP10                    : 1;  /**< \brief 硬件递加条件10*/
    __IO uint32_t HCUP11                    : 1;  /**< \brief 硬件递加条件11*/
    uint32_t RESERVED12                     : 4;  /**< \brief 保留*/
    __IO uint32_t HCUP16                    : 1;  /**< \brief 硬件递加条件16*/
    __IO uint32_t HCUP17                    : 1;  /**< \brief 硬件递加条件17*/
    uint32_t RESERVED18                     :14;  /**< \brief 保留*/
} stc_tmr6_hcupr_field_t;

/**
 * \brief TMR6 - 硬件递减事件选择寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t HCDO0                     : 1;  /**< \brief 硬件递减条件0*/
    __IO uint32_t HCDO1                     : 1;  /**< \brief 硬件递减条件1*/
    __IO uint32_t HCDO2                     : 1;  /**< \brief 硬件递减条件2*/
    __IO uint32_t HCDO3                     : 1;  /**< \brief 硬件递减条件3*/
    __IO uint32_t HCDO4                     : 1;  /**< \brief 硬件递减条件4*/
    __IO uint32_t HCDO5                     : 1;  /**< \brief 硬件递减条件5*/
    __IO uint32_t HCDO6                     : 1;  /**< \brief 硬件递减条件6*/
    __IO uint32_t HCDO7                     : 1;  /**< \brief 硬件递减条件7*/
    __IO uint32_t HCDO8                     : 1;  /**< \brief 硬件递减条件8*/
    __IO uint32_t HCDO9                     : 1;  /**< \brief 硬件递减条件9*/
    __IO uint32_t HCDO10                    : 1;  /**< \brief 硬件递减条件10*/
    __IO uint32_t HCDO11                    : 1;  /**< \brief 硬件递减条件11*/
    uint32_t RESERVED12                     : 4;  /**< \brief 保留*/
    __IO uint32_t HCDO16                    : 1;  /**< \brief 硬件递减条件16*/
    __IO uint32_t HCDO17                    : 1;  /**< \brief 硬件递减条件17*/
    uint32_t RESERVED18                     :14;  /**< \brief 保留*/
} stc_tmr6_hcdor_field_t;

/**
 * \brief TMR6 - 软件同步启动控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SSTA1                     : 1;  /**< \brief 单元1软件启动*/
    __IO uint32_t SSTA2                     : 1;  /**< \brief 单元2软件启动*/
    __IO uint32_t SSTA3                     : 1;  /**< \brief 单元3软件启动*/
    uint32_t RESERVED3                      :13;  /**< \brief 保留*/
    __IO uint32_t RESV0                     : 1;
    uint32_t RESERVED17                     : 7;  /**< \brief 保留*/
    __IO uint32_t RESV                      : 1;
    uint32_t RESERVED25                     : 7;  /**< \brief 保留*/
} stc_tmr6_cr_sstar_field_t;

/**
 * \brief TMR6 - 软件同步停止控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SSTP1                     : 1;  /**< \brief 单元1软件停止*/
    __IO uint32_t SSTP2                     : 1;  /**< \brief 单元2软件停止*/
    __IO uint32_t SSTP3                     : 1;  /**< \brief 单元3软件停止*/
    uint32_t RESERVED3                      :29;  /**< \brief 保留*/
} stc_tmr6_cr_sstpr_field_t;

/**
 * \brief TMR6 - 软件同步清零控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t SCLE1                     : 1;  /**< \brief 单元1软件清零*/
    __IO uint32_t SCLE2                     : 1;  /**< \brief 单元2软件清零*/
    __IO uint32_t SCLE3                     : 1;  /**< \brief 单元3软件清零*/
    uint32_t RESERVED3                      :29;  /**< \brief 保留*/
} stc_tmr6_cr_sclrr_field_t;

/**
 * \brief 定时器寄存器块结构体
 */
typedef struct {
    union
    {
        __IO uint32_t CNTER;           /**< \brief 通用计数值寄存器 */
        stc_tmr6_cnter_field_t CNTER_f;
    };
    union
    {
        __IO uint32_t PERAR;           /**< \brief 通用周期基准值寄存器A */
        stc_tmr6_perar_field_t PERAR_f;
    };
    union
    {
        __IO uint32_t PERBR;          /**< \brief 通用周期基准值寄存器B */
        stc_tmr6_perbr_field_t PERBR_f;
    };
    union
    {
        __IO uint32_t PERCR;          /**< \brief 通用周期基准值寄存器C */
        stc_tmr6_percr_field_t PERCR_f;
    };
    union
    {
        __IO uint32_t GCMAR;          /**< \brief 通用比较基准值寄存器A */
        stc_tmr6_gcmar_field_t GCMAR_f;
    };
    union
    {
        __IO uint32_t GCMBR;          /**< \brief 通用比较基准值寄存器B */
        stc_tmr6_gcmbr_field_t GCMBR_f;
    };
    union
    {
        __IO uint32_t GCMCR;          /**< \brief 通用比较基准值寄存器C */
        stc_tmr6_gcmcr_field_t GCMCR_f;
    };
    union
    {
        __IO uint32_t GCMDR;          /**< \brief 通用比较基准值寄存器D */
        stc_tmr6_gcmdr_field_t GCMDR_f;
    };
    union
    {
        __IO uint32_t GCMER;          /**< \brief 通用比较基准值寄存器E */
        stc_tmr6_gcmer_field_t GCMER_f;
    };
    union
    {
        __IO uint32_t GCMFR;          /**< \brief 通用比较基准值寄存器F */
        stc_tmr6_gcmfr_field_t GCMFR_f;
    };
    union
    {
        __IO uint32_t SCMAR;          /**< \brief 专用比较基准值寄存器A */
        stc_tmr6_scmar_field_t SCMAR_f;
    };
    union
    {
        __IO uint32_t SCMBR;          /**< \brief 专用比较基准值寄存器B */
        stc_tmr6_scmbr_field_t SCMBR_f;
    };
    union
    {
        __IO uint32_t SCMCR;          /**< \brief 专用比较基准值寄存器C */
        stc_tmr6_scmcr_field_t SCMCR_f;
    };
    union
    {
        __IO uint32_t SCMDR;          /**< \brief 专用比较基准值寄存器D */
        stc_tmr6_scmdr_field_t SCMDR_f;
    };
    union
    {
        __IO uint32_t SCMER;          /**< \brief 专用比较基准值寄存器E */
        stc_tmr6_scmer_field_t SCMER_f;
    };
    union
    {
        __IO uint32_t SCMFR;          /**< \brief 专用比较基准值寄存器F */
        stc_tmr6_scmfr_field_t SCMFR_f;
    };
    union
    {
        __IO uint32_t DTUAR;          /**< \brief 死区时间基准值寄存器UA */
        stc_tmr6_dtuar_field_t DTUAR_f;
    };
    union
    {
        __IO uint32_t DTDAR;          /**< \brief 死区时间基准值寄存器DA */
        stc_tmr6_dtdar_field_t DTDAR_f;
    };
    union
    {
        __IO uint32_t DTUBR;          /**< \brief 死区时间基准值寄存器UB */
        stc_tmr6_dtubr_field_t DTUBR_f;
    };
    union
    {
        __IO uint32_t DTDBR;          /**< \brief 死区时间基准值寄存器DB */
        stc_tmr6_dtdbr_field_t DTDBR_f;
    };
    union
    {
        __IO uint32_t GCONR;          /**< \brief 通用控制寄存器 */
        stc_tmr6_gconr_field_t GCONR_f;
    };
    union
    {
        __IO uint32_t ICONR;          /**< \brief 中断控制寄存器 */
        stc_tmr6_iconr_field_t ICONR_f;
    };
    union
    {
        __IO uint32_t PCONR;          /**< \brief 端口控制寄存器 */
        stc_tmr6_pconr_field_t PCONR_f;
    };
    union
    {
        __IO uint32_t BCONR;          /**< \brief 缓存控制寄存器 */
        stc_tmr6_bconr_field_t BCONR_f;
    };
    union
    {
        __IO uint32_t DCONR;          /**< \brief 死区控制寄存器 */
        stc_tmr6_dconr_field_t DCONR_f;
    };
    uint8_t RESERVED25[4];            /**< \brief 保留 */
    union
    {
        __IO uint32_t FCONR;          /**< \brief 滤波控制寄存器 */
        stc_tmr6_fconr_field_t FCONR_f;
    };
    union
    {
        __IO uint32_t VPERR;          /**< \brief 有效周期寄存器 */
        stc_tmr6_vperr_field_t VPERR_f;
    };
    union
    {
        __IO uint32_t STFLR;          /**< \brief 状态标志寄存器 */
        stc_tmr6_stflr_field_t STFLR_f;
    };
    union
    {
        __IO uint32_t HSTAR;          /**< \brief 硬件启动事件选择寄存器 */
        stc_tmr6_hstar_field_t HSTAR_f;
    };
    union
    {
        __IO uint32_t HSTPR;          /**< \brief 硬件停止事件选择寄存器 */
        stc_tmr6_hstpr_field_t HSTPR_f;
    };
    union
    {
        __IO uint32_t HCLRR;          /**< \brief 硬件清零事件选择寄存器 */
        stc_tmr6_hclrr_field_t HCLRR_f;
    };
    union
    {
        __IO uint32_t HCPAR;          /**< \brief 硬件捕获事件选择寄存器A */
        stc_tmr6_hcpar_field_t HCPAR_f;
    };
    union
    {
        __IO uint32_t HCPBR;          /**< \brief 硬件捕获事件选择寄存器B */
        stc_tmr6_hcpbr_field_t HCPBR_f;
    };
    union
    {
        __IO uint32_t HCUPR;          /**< \brief 硬件递加事件选择寄存器 */
        stc_tmr6_hcupr_field_t HCUPR_f;
    };
    union
    {
        __IO uint32_t HCDOR;          /**< \brief 硬件递减事件选择寄存器 */
        stc_tmr6_hcdor_field_t HCDOR_f;
    };
} amhw_hc32f460_adtim_t;

/**
 * \brief ADT CHx通道定义
 */
typedef enum {
    AMHW_HC32F460_ADTIM_CHX_A = 0u,            /** <brief CHx A通道 */
    AMHW_HC32F460_ADTIM_CHX_B = 1u,            /** <brief CHx B通道 */
}amhw_hc32f460_adtim_chx_t;

/**
 * \brief ADT TRIG端口定义
 */
typedef enum {
    AMHW_HC32F460_ADTIM_TRIG_PORT_A = 0u,   /** <brief TIMx 触发A端口 */
    AMHW_HC32F460_ADTIM_TRIG_PORT_B = 1u,   /** <brief TIMx 触发B端口 */
    AMHW_HC32F460_ADTIM_TRIG_PORT_C = 2u,   /** <brief TIMx 触发C端口 */
    AMHW_HC32F460_ADTIM_TRIG_PORT_D = 3u,   /** <brief TIMx 触发D端口 */
}amhw_hc32f460_adtim_trig_port_t;

/**
 * \brief ADT通用控制 - Z相输入屏蔽周期数
 */
typedef enum {
    AMHW_HC32F460_ADTIM_ZMASK_NO = 0u,  /** <brief Z相输入屏蔽功能无效 */
    AMHW_HC32F460_ADTIM_ZMASK_4  = 1u,  /** <brief 位置计数上溢后或下溢后的4个计数周期内的Z相输入被屏蔽 */
    AMHW_HC32F460_ADTIM_ZMASK_8  = 2u,  /** <brief 位置计数上溢后或下溢后的8个计数周期内的Z相输入被屏蔽 */
    AMHW_HC32F460_ADTIM_ZMASK_16 = 3u,  /** <brief 位置计数上溢后或下溢后的16个计数周期内的Z相输入被屏蔽 */
}amhw_hc32f460_adtim_zmask_t;

/**
 * \brief ADT通用控制 - 计数时钟选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_PCLK_DIV1    = 0ul,  /** <brief PCLK0 */
    AMHW_HC32F460_ADTIM_PCLK_DIV2    = 1ul,  /** <brief PCLK0/2 */
    AMHW_HC32F460_ADTIM_PCLK_DIV4    = 2ul,  /** <brief PCLK0/4 */
    AMHW_HC32F460_ADTIM_PCLK_DIV8    = 3ul,  /** <brief PCLK0/8 */
    AMHW_HC32F460_ADTIM_PCLK_DIV16   = 4ul,  /** <brief PCLK0/16 */
    AMHW_HC32F460_ADTIM_PCLK_DIV64   = 5ul,  /** <brief PCLK0/64 */
    AMHW_HC32F460_ADTIM_PCLK_DIV256  = 6ul,  /** <brief PCLK0/256 */
    AMHW_HC32F460_ADTIM_PCLK_DIV1024 = 7ul,  /** <brief PCLK0/1024 */
}amhw_hc32f460_adtim_pclk_div_t;

/**
 * \brief ADT计数模式
 */
typedef enum {
    AMHW_HC32F460_ADTIM_CNT_MODE_SAWTOOTH   = 0u,  /** <brief 锯齿波模式 */
    AMHW_HC32F460_ADTIM_CNT_MODE_TRIANGLE_A = 4u,  /** <brief 三角波A模式 */
    AMHW_HC32F460_ADTIM_CNT_MODE_TRIANGLE_B = 5u,  /** <brief 三角波B模式 */
}amhw_hc32f460_adtim_cnt_mode_t;

/**
 * \brief ADT计数方向
 */
typedef enum {
    AMHW_HC32F460_ADTIM_CNT_DIR_DOWN = 0u,        /** <brief 递减计数 */
    AMHW_HC32F460_ADTIM_CNT_DIR_UP   = 1u,        /** <brief 递加计数 */
}amhw_hc32f460_adtim_cnt_dir_t;

/**
 * \brief ADT通用比较基准
 */
typedef enum {
    AMHW_HC32F460_ADTIM_COMPARE_A = 0u,           /** <brief 通用比较基准A */
    AMHW_HC32F460_ADTIM_COMPARE_B = 1u,           /** <brief 通用比较基准B */
    AMHW_HC32F460_ADTIM_COMPARE_C = 2u,           /** <brief 通用比较基准C */
    AMHW_HC32F460_ADTIM_COMPARE_D = 3u,           /** <brief 通用比较基准D */
    AMHW_HC32F460_ADTIM_COMPARE_E = 4u,           /** <brief 通用比较基准E */
    AMHW_HC32F460_ADTIM_COMPARE_F = 5u,           /** <brief 通用比较基准F */
}amhw_hc32f460_adtim_compare_x_t;

/**
 * \brief ADT专用比较基准
 */
typedef enum {
    AMHW_HC32F460_ADTIM_SPCL_COMP_A = 0u,         /** <brief 专用比较基准A */
    AMHW_HC32F460_ADTIM_SPCL_COMP_B = 1u,         /** <brief 专用比较基准B */
}amhw_hc32f460_adtim_special_compare_x_t;

/**
 * \brief ADT端口控制 - TIMx输出状态控制
 */
typedef enum {
    AMHW_HC32F460_ADTIM_DISVAL_NORM = 0u,  /** <brief 强制输出无效条件0~3中条件成立时，CHx端口正常输出 */
    AMHW_HC32F460_ADTIM_DISVAL_HIZ  = 1u,  /** <brief 强制输出无效条件0~3中条件成立时，CHx端口输出高阻态 */
    AMHW_HC32F460_ADTIM_DISVAL_LOW  = 2u,  /** <brief 强制输出无效条件0~3中条件成立时，CHx端口输出低电平 */
    AMHW_HC32F460_ADTIM_DISVAL_HIGH = 3u,  /** <brief 强制输出无效条件0~3中条件成立时，CHx端口输出高电平 */
}amhw_hc32f460_adtim_disval_t;

/**
 * \brief ADT端口控制 - CHx强制输出无效条件选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_DISSEL0 = 0u,      /** <brief 选择强制输出无效条件0 */
    AMHW_HC32F460_ADTIM_DISSEL1 = 1u,      /** <brief 选择强制输出无效条件1 */
    AMHW_HC32F460_ADTIM_DISSEL2 = 2u,      /** <brief 选择强制输出无效条件2 */
    AMHW_HC32F460_ADTIM_DISSEL3 = 3u,      /** <brief 选择强制输出无效条件3 */
}amhw_hc32f460_adtim_dissel_t;

/**
 * \brief ADT端口控制 - CHx周期值匹配时端口状态设定
 */
typedef enum {
    AMHW_HC32F460_ADTIM_PERIOD_LOW  = 0u,  /** <brief 计数器计数值与周期值相等时，CHx端口输出保持为低电平 */
    AMHW_HC32F460_ADTIM_PERIOD_HIGH = 1u,  /** <brief 计数器计数值与周期值相等时，CHx端口输出设定为高电平 */
    AMHW_HC32F460_ADTIM_PERIOD_KEEP = 2u,  /** <brief 计数器计数值与周期值相等时，CHx端口输出设定为先前状态 */
    AMHW_HC32F460_ADTIM_PERIOD_INV  = 3u,  /** <brief 计数器计数值与周期值相等时，CHx端口输出设定为反转电平 */
}amhw_hc32f460_adtim_perc_t;

/**
 * \brief ADT端口控制 - CHx比较值匹配时端口状态设定
 */
typedef enum {
    AMHW_HC32F460_ADTIM_COMPARE_LOW  = 0u, /** <brief 计数器计数与GCMxR相等时，CHx端口输出保持为低电平 */
    AMHW_HC32F460_ADTIM_COMPARE_HIGH = 1u, /** <brief 计数器计数与GCMxR相等时，CHx端口输出设定为高电平 */
    AMHW_HC32F460_ADTIM_COMPARE_KEEP = 2u, /** <brief 计数器计数与GCMxR相等时，CHx端口输出设定为先前状态 */
    AMHW_HC32F460_ADTIM_COMPARE_INV  = 3u, /** <brief 计数器计数与GCMxR相等时，CHx端口输出设定为反转电平 */
}amhw_hc32f460_adtim_cmpc_t;

/**
  * \brief ADT端口控制 - CHx端口输出
  */
typedef enum {
    AMHW_HC32F460_ADTIM_PORT_OUT_LOW  = 0u, /** <brief CHx端口输出设定为低电平 */
    AMHW_HC32F460_ADTIM_PORT_OUT_HIGH = 1u, /** <brief CHx端口输出设定为高电平 */
}amhw_hc32f460_adtim_port_out_t;

/**
 * \brief ADT端口控制 - CHx端口功能模式选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_PORT_MODE_CMP_OUTPUT = 0u, /** <brief CHx端口设定为比较输出功能 */
    AMHW_HC32F460_ADTIM_PORT_MODE_CAP_INPUT  = 1u, /** <brief CHx端口设定为捕获输入功能 */
}amhw_hc32f460_adtim_port_mode_t;

/**
 * \brief ADT端口控制 - CHx计数开始停止端口状态选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_STATE_SEL_SS   = 0u, /** <brief 计数开始或停止时，CHx端口输出由STACB、STPCB决定 */
    AMHW_HC32F460_ADTIM_STATE_SEL_KEEP = 1u, /** <brief 计数开始或停止时，CHx端口输出设定为先前状态 */
}amhw_hc32f460_adtim_stastps_t;

/**
 * \brief ADT死区控制 - CHx死区分离设定
 */
typedef enum {
    AMHW_HC32F460_ADTIM_DtSeperate = 0u,      /** <brief DTUAR和DTDAR分别设定 */
    AMHW_HC32F460_ADTIM_DtEqual    = 1u,      /** <brief DTDAR的值和DTUAR的值自动相等 */
}amhw_hc32f460_adtim_sepa_t;

/**
 * \brief ADT滤波控制 - TRIx/TIMxIx端口滤波采样基准时钟选择
 */
typedef enum
{
    AMHW_HC32F460_ADTIM_FLTCLK_PCLK_DIV1  = 0u, /** <brief PCLK0 */
    AMHW_HC32F460_ADTIM_FLTCLK_PCLK_DIV4  = 1u, /** <brief PCLK0/4 */
    AMHW_HC32F460_ADTIM_FLTCLK_PCLK_DIV16 = 2u, /** <brief PCLK0/16 */
    AMHW_HC32F460_ADTIM_FLTCLK_PCLK_DIV64 = 3u, /** <brief PCLK0/64 */
}amhw_hc32f460_adtim_fltclk_pclkdiv_t;

/**
 * \brief ADT有效周期 - TIMx有效周期选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_PeriodCnts0 = 0u,       /** <brief 有效周期选择功能无效 */
    AMHW_HC32F460_ADTIM_PeriodCnts1 = 1u,       /** <brief 每隔1个周期有效一次 */
    AMHW_HC32F460_ADTIM_PeriodCnts2 = 2u,       /** <brief 每隔2个周期有效一次 */
    AMHW_HC32F460_ADTIM_PeriodCnts3 = 3u,       /** <brief 每隔3个周期有效一次 */
    AMHW_HC32F460_ADTIM_PeriodCnts4 = 4u,       /** <brief 每隔4个周期有效一次 */
    AMHW_HC32F460_ADTIM_PeriodCnts5 = 5u,       /** <brief 每隔5个周期有效一次 */
    AMHW_HC32F460_ADTIM_PeriodCnts6 = 6u,       /** <brief 每隔6个周期有效一次 */
    AMHW_HC32F460_ADTIM_PeriodCnts7 = 7u,       /** <brief 每隔7个周期有效一次 */
}amhw_hc32f460_adtim_pcnts_t;

/**
 * \brief ADT有效周期 - 计数条件选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_PeriodCnteDisable = 0u,  /** <brief 有效周期选择功能无效 */
    AMHW_HC32F460_ADTIM_PeriodCnteMin     = 1u,  /** <brief 锯齿波计数上、下溢点或三角波波谷做为计数条件 */
    AMHW_HC32F460_ADTIM_PeriodCnteMax     = 2u,  /** <brief 锯齿波计数上、下溢点或三角波波峰做为计数条件 */
    AMHW_HC32F460_ADTIM_PeriodCnteBoth    = 3u,  /** <brief 锯齿波计数上、下溢点或三角波波峰，波谷做为计数条件 */
}amhw_hc32f460_adtim_pcnte_t;

/**
 * \brief ADT端口触发控制 - 触发源选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_TRIG_PA3  = 0u,    /** <brief PA3 */
    AMHW_HC32F460_ADTIM_TRIG_PB3  = 1u,    /** <brief PB3 */
    AMHW_HC32F460_ADTIM_TRIG_PC3  = 2u,    /** <brief PC3 */
    AMHW_HC32F460_ADTIM_TRIG_PD3  = 3u,    /** <brief PD3 */
    AMHW_HC32F460_ADTIM_TRIG_PA7  = 4u,    /** <brief PA7 */
    AMHW_HC32F460_ADTIM_TRIG_PB7  = 5u,    /** <brief PB7 */
    AMHW_HC32F460_ADTIM_TRIG_PC7  = 6u,    /** <brief PC7 */
    AMHW_HC32F460_ADTIM_TRIG_PD7  = 7u,    /** <brief PD7 */
    AMHW_HC32F460_ADTIM_TRIG_PA11 = 8u,    /** <brief PA11 */
    AMHW_HC32F460_ADTIM_TRIG_PB11 = 9u,    /** <brief PB11 */
    AMHW_HC32F460_ADTIM_TRIG_PC11 = 10u,   /** <brief PC11 */
    AMHW_HC32F460_ADTIM_TRIG_PD1  = 11u,   /** <brief PD1 */
    AMHW_HC32F460_ADTIM_TRIG_PA15 = 12u,   /** <brief PA15 */
    AMHW_HC32F460_ADTIM_TRIG_PB15 = 13u,   /** <brief PB15 */
    AMHW_HC32F460_ADTIM_TRIG_PC5  = 14u,   /** <brief PC5 */
    AMHW_HC32F460_ADTIM_TRIG_PD5  = 15u,   /** <brief PD5 */
}amhw_hc32f460_adtim_trigsel_t;

/**
 * \brief ADT AOS触发控制 - AOSx触发源选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_AosxTrigSelTim0Int   = 0u,    /** <brief TIM0_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelTim1Int   = 1u,    /** <brief TIM1_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelTim2Int   = 2u,    /** <brief TIM2_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelLpTimInt  = 3u,    /** <brief LPTIMER_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelTim4Int   = 4u,    /** <brief TIM4_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelTim5Int   = 5u,    /** <brief TIM5_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelTim6Int   = 6u,    /** <brief TIM6_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelUart0Int  = 7u,    /** <brief UART0_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelUart1Int  = 8u,    /** <brief UART1_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelLpUartInt = 9u,    /** <brief LPUART_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelVc0Int    = 10u,   /** <brief VC0_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelVc1Int    = 11u,   /** <brief VC1_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelRtcInt    = 12u,   /** <brief RTC_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelPcaInt    = 13u,   /** <brief PCA_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelSpiInt    = 14u,   /** <brief SPI_INT */
    AMHW_HC32F460_ADTIM_AosxTrigSelAdcInt    = 15u,   /** <brief ADC_INT */
}amhw_hc32f460_adtim_iaosxs_t;

/**
 * \brief ADT硬件(启动/停止/清零/捕获)事件触发选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_HwTrigAos0        = 0u,       /** <brief 从AOS来的事件触发0有效 */
    AMHW_HC32F460_ADTIM_HwTrigAos1        = 1u,       /** <brief 从AOS来的事件触发1有效 */
    AMHW_HC32F460_ADTIM_HwTrigAos2        = 2u,       /** <brief 从AOS来的事件触发2有效 */
    AMHW_HC32F460_ADTIM_HwTrigAos3        = 3u,       /** <brief 从AOS来的事件触发3有效 */
    AMHW_HC32F460_ADTIM_HwTrigCHxARise    = 4u,       /** <brief CHxA端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwTrigCHxAFall    = 5u,       /** <brief CHxA端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwTrigCHxBRise    = 6u,       /** <brief CHxB端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwTrigCHxBFall    = 7u,       /** <brief CHxB端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwTrigTimTriARise = 8u,       /** <brief TIMTRIA端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwTrigTimTriAFall = 9u,       /** <brief TIMTRIA端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwTrigTimTriBRise = 10u,      /** <brief TIMTRIB端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwTrigTimTriBFall = 11u,      /** <brief TIMTRIB端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwTrigTimTriCRise = 12u,      /** <brief TIMTRIC端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwTrigTimTriCFall = 13u,      /** <brief TIMTRIC端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwTrigTimTriDRise = 14u,      /** <brief TIMTRID端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwTrigTimTriDFall = 15u,      /** <brief TIMTRID端口上采样到下降沿 */
}amhw_hc32f460_adtim_hw_trig_t;

/**
 * \brief ADT硬件(递加/递减)事件触发选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_HwCntCHxALowCHxBRise   = 0u,  /** <brief CHxA端口为低电平时，CHxB端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwCntCHxALowCHxBFall   = 1u,  /** <brief CHxA端口为低电平时，CHxB端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwCntCHxAHighCHxBRise  = 2u,  /** <brief CHxA端口为高电平时，CHxB端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwCntCHxAHighCHxBFall  = 3u,  /** <brief CHxA端口为高电平时，CHxB端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwCntCHxBLowCHxARise   = 4u,  /** <brief CHxB端口为低电平时，CHxA端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwCntCHxBLowCHxAFall   = 5u,  /** <brief CHxB端口为低电平时，CHxA端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwCntCHxBHighChxARise  = 6u,  /** <brief CHxB端口为高电平时，CHxA端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwCntCHxBHighCHxAFall  = 7u,  /** <brief CHxB端口为高电平时，CHxA端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwCntTimTriARise       = 8u,  /** <brief TIMTRIA端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwCntTimTriAFall       = 9u,  /** <brief TIMTRIA端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwCntTimTriBRise       = 10u, /** <brief TIMTRIB端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwCntTimTriBFall       = 11u, /** <brief TIMTRIB端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwCntTimTriCRise       = 12u, /** <brief TIMTRIC端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwCntTimTriCFall       = 13u, /** <brief TIMTRIC端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwCntTimTriDRise       = 14u, /** <brief TIMTRID端口上采样到上升沿 */
    AMHW_HC32F460_ADTIM_HwCntTimTriDFall       = 15u, /** <brief TIMTRID端口上采样到下降沿 */
    AMHW_HC32F460_ADTIM_HwCntAos0              = 16u, /** <brief 从AOS来的事件触发0有效 */
    AMHW_HC32F460_ADTIM_HwCntAos1              = 17u, /** <brief 从AOS来的事件触发1有效 */
    AMHW_HC32F460_ADTIM_HwCntAos2              = 18u, /** <brief 从AOS来的事件触发2有效 */
    AMHW_HC32F460_ADTIM_HwCntAos3              = 19u, /** <brief 从AOS来的事件触发3有效 */
}amhw_hc32f460_adtim_hw_cnt_t;

/**
 * \brief ADT端口刹车极性控制
 */
typedef enum {
    AMHW_HC32F460_ADTIM_PtBrkHigh = 0u,              /** <brief 端口刹车极性高电平有效 */
    AMHW_HC32F460_ADTIM_PtBrkLow  = 1u,              /** <brief 端口刹车极性低电平有效 */
}amhw_hc32f460_adtim_ptbrk_polarity_t;

/**
 * \brief ADT PWM展频计数选择
 */
typedef enum {
    AMHW_HC32F460_ADTIM_PwmDitherUnderFlow = 0u,     /** <brief PWM展频计数下溢出 */
    AMHW_HC32F460_ADTIM_PwmDitherOverFlow  = 1u,     /** <brief PWM展频计数上溢出 */
}amhw_hc32f460_adtim_pwm_dither_type_t;

/**
 * \brief ADT中断类型
 */
typedef enum {
    AMHW_HC32F460_ADTIM_CMAIrq  = 0u,                /** <brief 计数匹配A（或捕获输入）中断 */
    AMHW_HC32F460_ADTIM_CMBIrq  = 1u,                /** <brief 计数匹配B（或捕获输入）中断 */
    AMHW_HC32F460_ADTIM_CMCIrq  = 2u,                /** <brief 计数匹配C中断 */
    AMHW_HC32F460_ADTIM_CMDIrq  = 3u,                /** <brief 计数匹配D中断 */
    AMHW_HC32F460_ADTIM_CMEIrq  = 4u,                /** <brief 计数匹配E中断 */
    AMHW_HC32F460_ADTIM_CMFIrq  = 5u,                /** <brief 计数匹配F中断 */
    AMHW_HC32F460_ADTIM_OVFIrq  = 6u,                /** <brief 上溢匹配中断 */
    AMHW_HC32F460_ADTIM_UDFIrq  = 7u,                /** <brief 下溢匹配中断 */
    AMHW_HC32F460_ADTIM_DTEIrq  = 8u,                /** <brief 死区时间错误中断 */
    AMHW_HC32F460_ADTIM_INTENSAUIrq = 16u,               
    AMHW_HC32F460_ADTIM_INTENSADIrq = 17u,              
    AMHW_HC32F460_ADTIM_INTENSBUIrq = 18u,               
    AMHW_HC32F460_ADTIM_INTENSBDIrq = 19u,               
}amhw_hc32f460_adtim_irq_type_t;

/**
 * \brief ADT标志类型
 */
typedef enum {
    AMHW_HC32F460_ADTIM_CMAF_FLAG = 0,                    /** <brief 计数匹配A标志 */
    AMHW_HC32F460_ADTIM_CMBF_FLAG = 1,                    /** <brief 计数匹配B标志 */
    AMHW_HC32F460_ADTIM_CMCF_FLAG = 2,                    /** <brief 计数匹配C标志 */
    AMHW_HC32F460_ADTIM_CMDF_FLAG = 3,                    /** <brief 计数匹配D标志 */
    AMHW_HC32F460_ADTIM_OVFF_FLAG = 6,                    /** <brief 上溢匹配标志 */
    AMHW_HC32F460_ADTIM_UDFF_FLAG = 7,                    /** <brief 下溢匹配标志 */
    AMHW_HC32F460_ADTIM_DTEF_FLAG = 8,                    /** <brief 死区时间错误标志 */
    AMHW_HC32F460_ADTIM_CMSAUF_FLAG = 9,                  /** <brief 向上计数专用比较基准值匹配A标志 */
    AMHW_HC32F460_ADTIM_CMSADF_FLAG = 10,                 /** <brief 向下计数专用比较基准值匹配B标志 */
    AMHW_HC32F460_ADTIM_CMSBUF_FLAG = 11,                 /** <brief 向上计数专用比较基准值匹配A标志 */
    AMHW_HC32F460_ADTIM_CMSBDF_FLAG = 12,                 /** <brief 向下计数专用比较基准值匹配B标志 */
}amhw_hc32f460_adtim_state_type_t;

/**
 * \brief ADT软件同步配置
 */
typedef struct {
    am_bool_t adtim4;                              /** <brief Timer 4 */
    am_bool_t adtim5;                              /** <brief Timer 5 */
    am_bool_t adtim6;                              /** <brief Timer 6 */
}amhw_hc32f460_adtim_sw_sync_t;

/**
 * \brief ADT AOS触发配置
 */
typedef struct {
    amhw_hc32f460_adtim_iaosxs_t   aos0trig_src;     /** <brief AOS0触发源选择 */
    amhw_hc32f460_adtim_iaosxs_t   aos1trig_src;     /** <brief AOS1触发源选择 */
    amhw_hc32f460_adtim_iaosxs_t   aos2trig_src;     /** <brief AOS2触发源选择 */
    amhw_hc32f460_adtim_iaosxs_t   aos3trig_src;     /** <brief AOS3触发源选择 */
}amhw_hc32f460_adtim_aos_trig_cfg_t;

/**
 * \brief ADT 中断触发配置
 */
typedef struct {
    am_bool_t   specilmatch_B_trigdma;  /** <brief 专用比较基准值匹配B使能触发DMA */
    am_bool_t   specilmatch_A_trigdma;  /** <brief 专用比较基准值匹配A使能触发DMA */
    am_bool_t   underflow_trigdma;      /** <brief 下溢匹配使能触发DMA */
    am_bool_t   overflow_trigdma;       /** <brief 上溢匹配使能触发DMA */
    am_bool_t   cntmatch_D_trigdma;     /** <brief 计数匹配D使能触发DMA */
    am_bool_t   cntmatch_C_trigdma;     /** <brief 计数匹配C使能触发DMA */
    am_bool_t   cntmatch_B_trigdma;     /** <brief 计数匹配B使能触发DMA */
    am_bool_t   cntmatch_A_trigdma;     /** <brief 计数匹配A使能触发DMA */
    am_bool_t   specilmatch_B_trig;     /** <brief 专用比较基准值匹配B使能触发ADC */
    am_bool_t   specilmatch_A_trig;     /** <brief 专用比较基准值匹配A使能触发ADC */
    am_bool_t   underflow_trig;         /** <brief 下溢匹配使能触发ADC */
    am_bool_t   overflow_trig;          /** <brief 上溢匹配使能触发ADC */
    am_bool_t   cntmatch_D_trig;        /** <brief 计数匹配D使能触发ADC */
    am_bool_t   cntmatch_C_trig;        /** <brief 计数匹配C使能触发ADC */
    am_bool_t   cntmatch_B_trig;        /** <brief 计数匹配B使能触发ADC */
    am_bool_t   cntmatch_A_trig;        /** <brief 计数匹配A使能触发ADC */
}amhw_hc32f460_adtim_irq_trig_cfg_t;

/**
 * \brief ADT Trig端口配置
 */
typedef struct {
    amhw_hc32f460_adtim_trigsel_t        trigsrc;    /** <brief 触发源选择 */
    am_bool_t                          flt_enable;   /** <brief 触发源捕获输入滤波使能 */
    amhw_hc32f460_adtim_fltclk_pclkdiv_t fltclk;     /** <brief 滤波采样基准时钟 */
}amhw_hc32f460_adtim_port_trig_cfg_t;

/**
 * \brief ADT Z相输入屏蔽功能配置
 */
typedef struct {
    amhw_hc32f460_adtim_zmask_t zmask_cycle; /** <brief Z相输入屏蔽计数周期选择 */
    am_bool_t                 fltposcntmask; /** <brief Z相输入时的屏蔽周期内，位置计数器的清零功能不屏蔽（FALSE）或屏蔽(TRUE) */
    am_bool_t                 fltrevcntmask; /** <brief Z相输入时的屏蔽周期内，公转计数器的计数功能不屏蔽（FALSE）或屏蔽(TRUE) */
}amhw_hc32f460_adtim_zmask_cfg_t;

/**
 * \brief ADT TIMxX端口配置
 */
typedef struct {
    amhw_hc32f460_adtim_port_mode_t portmode;    /** <brief 端口功能模式 */
    am_bool_t                     outenable;     /** <brief 输出使能 */
    amhw_hc32f460_adtim_perc_t      perc;        /** <brief 周期值匹配时端口状态 */
    amhw_hc32f460_adtim_cmpc_t      cmpc;        /** <brief 比较值匹配时端口状态 */
    amhw_hc32f460_adtim_stastps_t   stastps;     /** <brief 计数开始停止端口状态选择 */
    amhw_hc32f460_adtim_port_out_t  staout;      /** <brief 计数开始端口输出状态 */
    amhw_hc32f460_adtim_port_out_t  stpout;      /** <brief 计数停止端口输出状态 */
    amhw_hc32f460_adtim_disval_t    disval;      /** <brief 强制输出无效时输出状态控制 */
    amhw_hc32f460_adtim_dissel_t    dissel;      /** <brief 强制输出无效条件选择 */
    am_bool_t                     fltenable;     /** <brief 端口捕获输入滤波使能 */
    amhw_hc32f460_adtim_fltclk_pclkdiv_t fltclk; /** <brief 端口滤波采样基准时钟 */
}amhw_hc32f460_adtim_timx_chx_port_cfg_t;

/**
 * \brief ADT刹车端口配置
 */
typedef struct {
    am_bool_t                          portenable;    /** <brief 端口使能 */
    amhw_hc32f460_adtim_ptbrk_polarity_t polaritysel; /** <brief 极性选择 */
}amhw_hc32f460_adtim_break_port_cfg_t;

/**
 * \brief ADT无效条件3配置
 */
typedef struct amhw_hc32f460_adtim_disable_3_cfg
{
    amhw_hc32f460_adtim_break_port_cfg_t stcbrkptcfg[16]; /** <brief 刹车端口配置 */
    am_bool_t                          fltenable;         /** <brief 刹车端口滤波使能 */
    amhw_hc32f460_adtim_fltclk_pclkdiv_t fltclk;          /** <brief 滤波采样基准时钟 */
}amhw_hc32f460_adtim_disable_3_cfg_t;

/**
 * \brief ADT无效条件1配置
 */
typedef struct {
    am_bool_t   tim6_outsh;     /** <brief TIM6输出同高 */
    am_bool_t   tim5_outsh;     /** <brief TIM5输出同高 */
    am_bool_t   tim4_outsh;     /** <brief TIM4输出同高 */
    am_bool_t   tim6_outsl;     /** <brief TIM6输出同低 */
    am_bool_t   tim5_outsl;     /** <brief TIM5输出同低 */
    am_bool_t   tim4_outsl;     /** <brief TIM4输出同低 */
}amhw_hc32f460_adtim_disable_1_cfg_t;

/**
 * \brief ADT PWM展频计数配置
 */
typedef struct {
    amhw_hc32f460_adtim_pwm_dither_type_t enAMHW_HC32F460_ADTIM_PDType;/** <brief PWM展频计数选择 */
    am_bool_t   bTimxBPDEn;                      /** <brief PWM通道B展频使能 */
    am_bool_t   bTimxAPDEn;                      /** <brief PWM通道A展频使能 */
}amhw_hc32f460_adtim_pwm_dither_cfg_t;

/**
 * \brief ADT基本计数配置
 */
typedef struct {
    amhw_hc32f460_adtim_cnt_mode_t cntmode;        /** <brief 计数模式 */
    amhw_hc32f460_adtim_cnt_dir_t  cntdir;         /** <brief 计数方向 */
    amhw_hc32f460_adtim_pclk_div_t cntclkdiv;      /** <brief 计数时钟选择 */
}amhw_hc32f460_adtim_basecnt_cfg_t;

/**
 * \brief ADT计数状态
 */
typedef struct {
    uint16_t  u16counter;                  /** <brief 当前计数器的计数值 */
    am_bool_t cntdir;                      /** <brief 计数方向 */
    uint8_t   u8validperiod;               /** <brief 有效周期计数 */
    am_bool_t cmsbdf;                      /** <brief 向下计数专用比较基准值匹配B标志 */
    am_bool_t cmsbuf;                      /** <brief 向上计数专用比较基准值匹配A标志 */
    am_bool_t cmsadf;                      /** <brief 向下计数专用比较基准值匹配B标志 */
    am_bool_t cmsauf;                      /** <brief 向上计数专用比较基准值匹配A标志 */
    am_bool_t dtff;                        /** <brief 死区时间错误标志 */
    am_bool_t udff;                        /** <brief 下溢匹配标志 */
    am_bool_t ovff;                        /** <brief 上溢匹配标志 */
    am_bool_t cmdf;                        /** <brief 计数匹配D标志 */
    am_bool_t cmcf;                        /** <brief 计数匹配C标志 */
    am_bool_t cmbf;                        /** <brief 计数匹配B标志 */
    am_bool_t cmaf;                        /** <brief 计数匹配A标志 */
}amhw_hc32f460_adtim_cntstate_cfg_t;

/**
 * \brief ADT有效计数周期
 */
typedef struct {
    amhw_hc32f460_adtim_pcnts_t  validcnt;     /** <brief 有效周期选择 */
    amhw_hc32f460_adtim_pcnte_t  validcdt;     /** <brief 有效周期计数条件 */
    am_bool_t                    period_D;     /** <brief 通用信号有效周期选择D */
    am_bool_t                    period_C;     /** <brief 通用信号有效周期选择C */
    am_bool_t                    period_B;     /** <brief 通用信号有效周期选择B */
    am_bool_t                    period_A;     /** <brief 通用信号有效周期选择A */
} amhw_hc32f460_adtim_validper_cfg_t;


/**
 * \brief 配置A通道硬件捕获事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] choice     : amhw_hc32f460_adtim_hw_trig_t枚举量
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_cfghwcaptureA(amhw_hc32f460_adtim_t        *p_hw_adtim,
                                          amhw_hc32f460_adtim_hw_trig_t choice) {

    if(choice > AMHW_HC32F460_ADTIM_HwTrigTimTriDFall) {
        return AM_FALSE;
    }

    p_hw_adtim->HCPAR |= (1ul << choice);
    p_hw_adtim->PCONR |= (1ul << 0 );

    return AM_TRUE;
}

/**
 * \brief 配置B通道硬件捕获事件
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] choice     : amhw_hc32f460_adtim_hw_trig_t枚举量
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_cfghwcaptureB(amhw_hc32f460_adtim_t        *p_hw_adtim,
                                          amhw_hc32f460_adtim_hw_trig_t choice) {

    if(choice > AMHW_HC32F460_ADTIM_HwTrigTimTriDFall) {
        return AM_FALSE;
    }

    p_hw_adtim->HCPBR |= (1ul << choice);
    p_hw_adtim->PCONR |= (1ul << 16 );

    return AM_TRUE;
}

/**
 * \brief CHxX端口配置
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] port       : amhw_hc32f460_adtim_chx_t通道枚举量
 * \param[in] p_cfg      : 指向TIMxX端口配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_hc32f460_adtim_timxchxportcfg(
              amhw_hc32f460_adtim_t                   *p_hw_adtim,
              amhw_hc32f460_adtim_chx_t                port,
              amhw_hc32f460_adtim_timx_chx_port_cfg_t *p_cfg);

/**
 * \brief ADTIM初始化
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] p_cfg      : 指向ADT基本计数配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_init(amhw_hc32f460_adtim_t             *p_hw_adtim,
                                 amhw_hc32f460_adtim_basecnt_cfg_t *p_cfg) {
    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    if((p_cfg->cntmode) > AMHW_HC32F460_ADTIM_CNT_MODE_TRIANGLE_B) {
        return AM_FALSE;
    }

    p_hw_adtim->GCONR &= ~(7ul << 1);
    p_hw_adtim->GCONR &= ~(7ul << 4);
    p_hw_adtim->GCONR &= ~(1ul << 8);

    p_hw_adtim->GCONR |= (p_cfg->cntmode << 1);
    p_hw_adtim->GCONR |= (p_cfg->cntclkdiv << 4);
    p_hw_adtim->GCONR |= (p_cfg->cntdir << 8);

    return AM_TRUE;
}

am_static_inline
amhw_hc32f460_adtim_pclk_div_t
amhw_hc32f460_adtim_clkdiv_get(amhw_hc32f460_adtim_t *p_hw_adtim) {

    return (amhw_hc32f460_adtim_pclk_div_t)((p_hw_adtim->GCONR >> 4) & 7ul);
}


/**
 * \brief ADTIM去初始化
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_hc32f460_adtim_deinit(amhw_hc32f460_adtim_t *p_hw_adtim);

/**
 * \brief ADTIM启动
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_startcount(amhw_hc32f460_adtim_t *p_hw_adtim) {

    p_hw_adtim->GCONR |= (1ul << 0);

    return AM_TRUE;
}

/**
 * \brief ADTIM停止
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_stopcount(amhw_hc32f460_adtim_t *p_hw_adtim) {

    p_hw_adtim->GCONR &= ~(1ul << 0);

    return AM_TRUE;
}

/**
 * \brief 设置计数值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] cnt        ： 计数值设定
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_setcount(amhw_hc32f460_adtim_t *p_hw_adtim,
                                     uint16_t             cnt) {

    p_hw_adtim->CNTER= cnt;

    return AM_TRUE;
}

/**
 * \brief 获取计数值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return 计数值
 */
am_static_inline
uint16_t amhw_hc32f460_adtim_getcount(amhw_hc32f460_adtim_t *p_hw_adtim) {
    return p_hw_adtim->CNTER;
}

/**
 * \brief 清除计数值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_clearcount(amhw_hc32f460_adtim_t *p_hw_adtim) {

    p_hw_adtim->CNTER = 0;

    return AM_TRUE;
}

typedef enum adtim_period_x
{
    ADTIM_PERIODA = 0u,         
    ADTIM_PERIODB = 1u,         
    ADTIM_PERIODC = 2u,         
}adtim_period_x_t;

/**
 * \brief 配置计数周期
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] data       : 计数周期设定值
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_setperiod(amhw_hc32f460_adtim_t *p_hw_adtim,
                                        adtim_period_x_t       pri,
                                        uint16_t               data) {
    switch (pri)
    {
        case ADTIM_PERIODA:
            p_hw_adtim->PERAR = data;
            break;
        case ADTIM_PERIODB:
            p_hw_adtim->PERBR = data;
            break;
        case ADTIM_PERIODC:
            p_hw_adtim->PERCR = data;
            break;
        default:
            break;
    }
    return AM_TRUE;
}

/**
 * \brief 获取计数周期
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return 计数周期
 */
am_static_inline
uint16_t amhw_hc32f460_adtim_getperiod(amhw_hc32f460_adtim_t *p_hw_adtim, adtim_period_x_t pri) {
    uint16_t value = 0;
    switch (pri)
    {
        case ADTIM_PERIODA:
            value = p_hw_adtim->PERAR;
            break;
        case ADTIM_PERIODB:
            value = p_hw_adtim->PERBR;
            break;
        case ADTIM_PERIODC:
            value = p_hw_adtim->PERCR;
            break;
        default:
            break;
    }

    return value;
}

/**
 * \brief 配置比较输出计数基准值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] compare    : amhw_hc32f460_adtim_compare_x_t枚举量
 * \param[in] data       : 设定值
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_setcomparevalue(
              amhw_hc32f460_adtim_t          *p_hw_adtim,
              amhw_hc32f460_adtim_compare_x_t compare,
              uint16_t                      data) {

    switch(compare) {
    case AMHW_HC32F460_ADTIM_COMPARE_A:
        p_hw_adtim->GCMAR = data;
        break;
    case AMHW_HC32F460_ADTIM_COMPARE_B:
        p_hw_adtim->GCMBR = data;
        break;
    case AMHW_HC32F460_ADTIM_COMPARE_C:
        p_hw_adtim->GCMCR = data;
        break;
    case AMHW_HC32F460_ADTIM_COMPARE_D:
        p_hw_adtim->GCMDR = data;
        break;
    case AMHW_HC32F460_ADTIM_COMPARE_E:
        p_hw_adtim->GCMER = data;
        break;
    case AMHW_HC32F460_ADTIM_COMPARE_F:
        p_hw_adtim->GCMFR = data;
        break;        
    default:
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * \brief 获取捕获值
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] port       : amhw_hc32f460_adtim_chx_t枚举量
 *
 * \return 捕获值
 */
am_static_inline
uint16_t amhw_hc32f460_adtim_getcapturevalue(amhw_hc32f460_adtim_t    *p_hw_adtim,
                                           amhw_hc32f460_adtim_chx_t port) {
    switch(port) {
    case AMHW_HC32F460_ADTIM_CHX_A:
        return p_hw_adtim->GCMAR;
    case AMHW_HC32F460_ADTIM_CHX_B:
        return p_hw_adtim->GCMBR;
    default:
        return AM_FALSE;
    }
}

/**
 * \brief 配置中断
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] type       : amhw_hc32f460_adtim_irq_type_t中断类型枚举量
 * \param[in] equal      : 相应中断使能（AM_FALSE或AM_TRUE）
 *
 * \return AM_FALSE:设置失败
 *         AM_TRUE :设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_cfgirq(amhw_hc32f460_adtim_t         *p_hw_adtim,
                                   amhw_hc32f460_adtim_irq_type_t type,
                                   am_bool_t                    enable) {
    if(enable == AM_TRUE) {
        p_hw_adtim->ICONR |= (1ul << type);
    } else {
        p_hw_adtim->ICONR &= ~(1ul << type);
    }

    return AM_TRUE;
}

/**
 * \brief 获取中断标志
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] type       : amhw_hc32f460_adtim_irq_type_t中断类型枚举量
 *
 * \return AM_FALSE:标志无效
 *         AM_TRUE :标志有效
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_get_status_flag(amhw_hc32f460_adtim_t         *p_hw_adtim,
                                       amhw_hc32f460_adtim_state_type_t type) {
    return ((p_hw_adtim->STFLR >> type) & 1ul) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除中断标志
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] type       : amhw_hc32f460_adtim_irq_type_t中断类型枚举量
 *
 * \return AM_FALSE:设置失败
 *         AM_TRUE :设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_clear_status_flag(amhw_hc32f460_adtim_t         *p_hw_adtim,
                                         amhw_hc32f460_adtim_state_type_t type) {
    p_hw_adtim->STFLR &= ~(1ul << type);

    return AM_TRUE;
}

/**
 * \brief 清除所有中断标志
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE:设置失败
 *         AM_TRUE :设置成功
 */
am_static_inline
am_bool_t amhw_hc32f460_adtim_clearallirqflag(amhw_hc32f460_adtim_t *p_hw_adtim) {

    p_hw_adtim->STFLR = 0;

    return AM_TRUE;
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
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_ADTIM_H */

/* end of file */
