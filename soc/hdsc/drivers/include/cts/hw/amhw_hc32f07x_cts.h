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
 * \brief CTS操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-11
 * \endinternal
 */

#ifndef __AMHW_HC32F07X_CTS_H
#define __AMHW_HC32F07X_CTS_H

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
#elif defined(__GNUC__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/**
 * \addtogroup amhw_hc32f07x_if_cts
 * \copydoc amhw_hc32f07x_cts.h
 * @{
 */

/**
 * \brief PCNT寄存器块结构体
 */
typedef struct amhw_hc32f07x_cts {
    __IO uint32_t cr;        /* 控制寄存器 */
    __IO uint32_t cfgr;      /* 配置寄存器 */	
    __IO uint32_t isr;       /* 中断和状态寄存器 */
    __IO uint32_t icr;       /* 中断标志清除寄存器 */	
} amhw_hc32f07x_cts_t;

/**
 * \brief CTS 定时器功能启动
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f07x_cts_tim_enable (amhw_hc32f07x_cts_t *p_hw_cts)
{
    p_hw_cts->cr |= (0x1ul << 15);
}

/**
 * \brief CTS 定时器功能停止
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f07x_cts_tim_disable (amhw_hc32f07x_cts_t *p_hw_cts)
{
    p_hw_cts->cr &= ~(0x1ul << 15);
}

/**
 * \brief CTS 振荡微调值设置
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  value     : 微调值
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f07x_cts_trim_set (amhw_hc32f07x_cts_t *p_hw_cts,
	                               uint8_t              value)
{
    p_hw_cts->cr = (p_hw_cts->cr & ~(0x7f << 8)) | 
	                  ((value & 0x7f) << 8);
}

/**
 * \brief CTS 振荡微调值获取
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 *
 * \return  value  微调值
 */
am_static_inline
uint8_t amhw_hc32f07x_cts_trim_get (amhw_hc32f07x_cts_t *p_hw_cts)
{
    return ((p_hw_cts->cr >> 8) & 0x7f );
}

/**
 * \brief CTS 软件同步设置
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_swsync_set (amhw_hc32f07x_cts_t *p_hw_cts)
{
    p_hw_cts->cr |= (0x01 << 7);
}

/**
 * \brief CTS 自动微调使能
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_auto_trim_enable (amhw_hc32f07x_cts_t *p_hw_cts)
{
    p_hw_cts->cr |= (0x01 << 6);
}

/**
 * \brief CTS 自动微调禁能
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_auto_trim_disable (amhw_hc32f07x_cts_t *p_hw_cts)
{
    p_hw_cts->cr &= ~(0x01 << 6);
}

/**
 * \brief CTS 频率误差计数器/定时器使能
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_cen_enable (amhw_hc32f07x_cts_t *p_hw_cts)
{
    p_hw_cts->cr |= (0x01 << 5);
}

/**
 * \brief CTS 频率误差计数器/定时器禁能
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_cen_disable (amhw_hc32f07x_cts_t *p_hw_cts)
{
    p_hw_cts->cr &= ~(0x01 << 5);
}

/**
 * \biref 中断使能控制
 */
#define AMHW_HC32F07X_CTS_INT_CAPIE  (0X01 << 4U)  /**< \brief 定时器模式捕获中断使能 */
#define AMHW_HC32F07X_CTS_INT_UDFIE  (0X01 << 3U)  /**< \brief 计数器下溢出中断使能 */
#define AMHW_HC32F07X_CTS_INT_ERRIE  (0X01 << 2U)  /**< \brief 校准错误中断使能 */
#define AMHW_HC32F07X_CTS_INT_WARNIE (0X01 << 1U)  /**< \brief 校准警告中断使能 */
#define AMHW_HC32F07X_CTS_INT_OKIE   (0X01 << 0U)  /**< \brief 校准正常中断使能 */

/**
 * \brief CTS 中断使能
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  ie        : 参考上述宏定义AMHW_HC32F07X_CTS_INT_
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_int_enable (amhw_hc32f07x_cts_t *p_hw_cts,
	                                 uint8_t              ie)
{
    p_hw_cts->cr |= ie;
}

/**
 * \brief CTS 中断禁能
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  ie        : 参考上述宏定义AMHW_HC32F07X_CTS_INT_
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_int_disable (amhw_hc32f07x_cts_t *p_hw_cts,
	                                 uint8_t              ie)
{
    p_hw_cts->cr &= ~ie;
}

/**
 * \brief CTS 校准时钟
 */
typedef enum amhw_hc32f07x_cts_clksrc {
    AMHW_HC32F07X_CTS_CLKSRC_RC48M = 0,
    AMHW_HC32F07X_CTS_CLKSRC_RCH ,
    AMHW_HC32F07X_CTS_CLKSRC_XTH ,
    AMHW_HC32F07X_CTS_CLKSRC_RFFCLK ,    /* 仅限定时器模式使用 */
}amhw_hc32f07x_cts_clksrc_t;

/**
 * \brief CTS 时钟选择
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  src        : 参考枚举量amhw_hc32f07x_cts_clksrc_t
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_clksrc_set (amhw_hc32f07x_cts_t        *p_hw_cts,
                                   amhw_hc32f07x_cts_clksrc_t  src)
{
    p_hw_cts->cfgr = (p_hw_cts->cfgr & ~(0x03u << 30)) | 
                      (src << 30);
}

/**
 * \brief CTS 信号源
 */
typedef enum amhw_hc32f07x_cts_refsrc {
    AMHW_HC32F07X_CTS_REFSRC_USB_SOF = 0,
    AMHW_HC32F07X_CTS_REFSRC_XTL,
    AMHW_HC32F07X_CTS_REFSRC_GPIO,
    AMHW_HC32F07X_CTS_REFSRC_LPTIM_TOG,
}amhw_hc32f07x_cts_refsrc_t;

/**
 * \brief CTS 信号源选择
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  src        : 参考枚举量amhw_hc32f07x_cts_refsrc_t
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_refsrc_set (amhw_hc32f07x_cts_t   *p_hw_cts,
                                   amhw_hc32f07x_cts_refsrc_t  src)
{
    p_hw_cts->cfgr = (p_hw_cts->cfgr & ~(0x03u << 28)) | 
                      (src << 28);
}

/**
 * \brief CTS 同步极性
 */
typedef enum amhw_hc32f07x_cts_pol {
    AMHW_HC32F07X_CTS_POL_UP = 0,
    AMHW_HC32F07X_CTS_POL_DOWN,
}amhw_hc32f07x_cts_pol_t;

/**
 * \brief CTS 信号源选择
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  src        : 参考枚举量amhw_hc32f07x_cts_pol_t
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_pol_set (amhw_hc32f07x_cts_t   *p_hw_cts,
	                              amhw_hc32f07x_cts_pol_t  src)
{
    p_hw_cts->cfgr = (p_hw_cts->cfgr & ~(0x01 << 27)) | 
	                   (src << 27);
}

/**
 * \brief CTS 预分频
 */
typedef enum amhw_hc32f07x_cts_prs {
    AMHW_HC32F07X_CTS_PRS_0DIV = 0,    /* 不分频 */
    AMHW_HC32F07X_CTS_PRS_2DIV,        /* 2分频 */
    AMHW_HC32F07X_CTS_PRS_4DIV,        /* 4分频 */
    AMHW_HC32F07X_CTS_PRS_8DIV,        /* 8分频 */
    AMHW_HC32F07X_CTS_PRS_16DIV,       /* 16分频 */
    AMHW_HC32F07X_CTS_PRS_32DIV,       /* 32分频 */
    AMHW_HC32F07X_CTS_PRS_64DIV,       /* 64分频 */
    AMHW_HC32F07X_CTS_PRS_256DIV,      /* 256分频 */
}amhw_hc32f07x_cts_prs_t;

/**
 * \brief CTS 预分频设置
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  src        : 参考枚举量amhw_hc32f07x_cts_pol_t
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_prs_set (amhw_hc32f07x_cts_t   *p_hw_cts,
                                amhw_hc32f07x_cts_prs_t  src)
{
    p_hw_cts->cfgr = (p_hw_cts->cfgr & ~(0x07 << 24)) | 
                      (src << 24);
}

/**
 * \brief CTS 频率误差限值设置
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  value     : 误差限值
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_felim_set (amhw_hc32f07x_cts_t   *p_hw_cts,
	                                uint8_t                value)
{
    p_hw_cts->cfgr = (p_hw_cts->cfgr & ~(0xff << 16)) | 
	                   (value << 16);
}

/**
 * \brief CTS 计数器重载值
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  value     : 重载值
 *
 * \return  无
 */
am_static_inline
void amhw_hc32f07x_cts_arr_set (amhw_hc32f07x_cts_t   *p_hw_cts,
	                                uint16_t                value)
{
    p_hw_cts->cfgr = (p_hw_cts->cfgr & ~(0xffff << 0)) | 
	                   (value << 0);
}

/**
 * \brief CTS 频率误差捕获值获取
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 *
 * \return  频率误差捕获值
 */
am_static_inline
uint16_t amhw_hc32f07x_cts_fecap_get (amhw_hc32f07x_cts_t   *p_hw_cts)
{
    return  ((p_hw_cts->isr >> 16) & 0xffff);
}

/**
 * \brief CTS 频率误差方向
 */
typedef enum amhw_hc32f07x_cts_dir {
    AMHW_HC32F07X_CTS_DIR_UP = 0,   /* 递增计数方向，实际频率高于目标频率 */
    AMHW_HC32F07X_CTS_DIR_DOWN ,    /* 递减计数方向，实际频率低于目标频率 */
}amhw_hc32f07x_cts_dir_t;

/**
 * \brief CTS 频率误差方向获取
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 *
 * \return  参考枚举量amhw_hc32f07x_cts_dir_t
 */
am_static_inline
amhw_hc32f07x_cts_dir_t 
amhw_hc32f07x_cts_dir_get (amhw_hc32f07x_cts_t   *p_hw_cts)
{
    return  (amhw_hc32f07x_cts_dir_t)((p_hw_cts->isr >> 15) & 0x01);
}

/**
 * \biref 标志定义
 */
#define AMHW_HC32F07X_CTS_FLAG_OVF  (10U)  /**< \brief TRIM溢出 */
#define AMHW_HC32F07X_CTS_FLAG_MISS (9U)  /**< \brief 校准参考同步丢失 */
#define AMHW_HC32F07X_CTS_FLAG_ERR  (8U)  /**< \brief 校准错误 */

#define AMHW_HC32F07X_CTS_FLAG_CAPF  (4U)  /**< \brief 捕获标志 */
#define AMHW_HC32F07X_CTS_FLAG_UDFF  (3U)  /**< \brief 计数器下溢出标志 */
#define AMHW_HC32F07X_CTS_FLAG_ERRF  (2U)  /**< \brief 错误标志 */
#define AMHW_HC32F07X_CTS_FLAG_WARNF (1U)  /**< \brief 校准警告标志 */
#define AMHW_HC32F07X_CTS_FLAG_OKF   (0U)  /**< \brief 校准正常标志 */

/**
 * \brief CTS 标志获取
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  flag      : 参考宏定义AMHW_HC32F07X_CTS_FLAG
 *
 * \return AM_TRUE :标志有效
 *         AM_FALSE:标志无效
 */
am_static_inline
am_bool_t amhw_hc32f07x_cts_flag_get (amhw_hc32f07x_cts_t   *p_hw_cts,
                                      uint16_t              flag)
{
    return  ((p_hw_cts->isr >> flag) & 0x01)? AM_TRUE : AM_FALSE;
}

/**
 * \biref 状态清除定义
 */
#define AMHW_HC32F07X_CTS_CLR_CAPC  (1U << 4)  /**< \brief 捕获标志清除 */
#define AMHW_HC32F07X_CTS_CLR_UDFC  (1U << 3)  /**< \brief 预期同步标志清除 */
#define AMHW_HC32F07X_CTS_CLR_ERRC  (1U << 2)  /**< \brief 校准错误标志清除 */
#define AMHW_HC32F07X_CTS_CLR_WARNC (1U << 1)  /**< \brief 校准警告标志清除 */
#define AMHW_HC32F07X_CTS_CLR_OKC   (1U << 0)  /**< \brief 校准正常标志清除 */

/**
 * \brief CTS 状态清除
 *
 * \papcnt[in]  p_hw_cts  : 指向CTS寄存器结构体的指针
 * \papcnt[in]  flag      : 参考宏定义AMHW_HC32F07X_CTS_CLR
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f07x_cts_flag_clr (amhw_hc32f07x_cts_t   *p_hw_cts,
                                uint8_t              clr)
{
    p_hw_cts->icr &= ~clr;
}

/**
 * @}
 */
/*
 * \brief 匿名结构体段的结束
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__GNUC__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

#ifdef __cplusplus
}
#endif /* __AMHW_HC32f07x_CTS_H */

#endif

/* end of file */
