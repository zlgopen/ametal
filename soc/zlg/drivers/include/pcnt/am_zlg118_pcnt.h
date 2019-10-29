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
 * \brief PCNT驱动接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-11
 * \endinternal
 */

#ifndef __AM_ZLG118_PCNT_H
#define __AM_ZLG118_PCNT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "hw/amhw_zlg118_pcnt.h"

/* 通道极性选择枚举 */
typedef enum am_zlg118_pcnt_polar {
    ZLG118_PCNT_S0PNOINVERT = (0u),   /* S0通道极性不取反 */
    ZLG118_PCNT_S0PINVERT   = (1u),   /* S0通道取反 */
    ZLG118_PCNT_S1PNOINVERT = (2u),   /* S1通道极性不取反 */
    ZLG118_PCNT_S1PINVERT   = (3u)    /* S1通道取反 */
} am_zlg118_pcnt_polar_t;

/* 时钟选择枚举 */
typedef enum am_zlg118_pcnt_clk {
    ZLG118_PCNT_PCLK = (1u),
    ZLG118_PCNT_XTL  = (2u),
    ZLG118_PCNT_RCL  = (3u)
} am_zlg118_pcnt_clk_t;

/* 脉冲计数模式选择枚举 */
typedef enum am_zlg118_pcnt_mode {
    ZLG118_PCNT_SINGLE  = (1u), /* 单通道脉冲计数模式 */
    ZLG118_PCNT_SPECIAL = (2u), /* 双通道非正交脉冲计数模式 */
    ZLG118_PCNT_DOUBLE  = (3u)  /* 双通道正交脉冲计数模式 */
} am_zlg118_pcnt_mode_t;

/* 脉冲计数模式选择枚举 */
typedef enum am_zlg118_pcnt_dir {
    ZLG118_PCNT_UP   = (0u), /* 向上计数模式 */
    ZLG118_PCNT_DOWN = (1u), /* 向下计数模式 */
} am_zlg118_pcnt_dir_t;

/* 中断标志枚举 */
typedef enum am_zlg118_pcnt_int {
    ZLG118_PCNT_INT_S1E = (1u << 7), /* S1脉冲解码错误 */
    ZLG118_PCNT_INT_S0E = (1u << 6), /* S0脉冲解码错误 */
    ZLG118_PCNT_INT_BB  = (1u << 5), /* 中间无低状态 */
    ZLG118_PCNT_INT_FE  = (1u << 4), /* 不正确非交编码帧 */
    ZLG118_PCNT_INT_DIR = (1u << 3), /* 正交脉冲方向改变 */
    ZLG118_PCNT_INT_TO  = (1u << 2), /* 超时中断 */
    ZLG118_PCNT_INT_OV  = (1u << 1), /* 上溢出中断 */
    ZLG118_PCNT_INT_UF  = (1u << 0)  /* 下溢出中断 */
} am_zlg118_pcnt_int_t;

/* PCNT命令枚举 */
typedef enum am_zlg118_pcnt_cmd {
    ZLG118_PCNT_B2C = (4u), /* 立即将BUF中的值同步到CNT */
    ZLG118_PCNT_B2T = (2u), /* 立即将BUF中的值同步到TOP */
    ZLG118_PCNT_T2C = (1)   /* 立即将TOP中的值同步到CNT */
} am_zlg118_pcnt_cmd_t;

/* 输出选项 */
typedef enum am_zlg118_pcnt_dgb {
    ZLG118_PCNT_DGB_0   = (0),  /* 固定为0 */
    ZLG118_PCNT_DGB_SA  = (4u), /* 脉冲同步之后的输出（S0A/S1A） */
    ZLG118_PCNT_DGB_SP  = (2u), /* 脉冲极性选择之后的输出（S0P/S1P） */
    ZLG118_PCNT_DGB_SPF = (1)   /* 脉冲滤波之后的输出（S0PF/S1PF） */
} am_zlg118_pcnt_dgb_t;

/**
 * \brief PCNT设备信息参数结构体
 */
typedef struct am_zlg118_pcnt_devinfo {
    uint32_t                pcnt_regbase;   /* 寄存器基址 */
    uint32_t                clk_num;        /* 时钟ID */
    uint16_t                inum;           /* 中断号 */
    void                    (*pfn_plfm_init)(void);         /*平台初始化函数 */
    void                    (*pfn_plfm_deinit)(void);       /* 平台解初始化函数 */
} am_zlg118_pcnt_devinfo_t;

typedef struct am_zlg118_pcnt_dev {
    amhw_zlg118_pcnt_t             *p_hw_pcnt;
    const am_zlg118_pcnt_devinfo_t *p_devinfo;
} am_zlg118_pcnt_dev_t;

typedef am_zlg118_pcnt_dev_t* am_zlg118_pcnt_handle_t;

/**
 * \addtogroup am_zlg_if_pcnt
 * \copydoc am_zlg_pcnt.h
 * @{
 */

/**
 * \brief PCNT初始化
 *
 * \param[in] p_dev     : 指向PCNT设备结构体的指针
 * \param[in] p_devinfo : 指向PCNT设备信息结构体的指针
 *
 * \return PCNT服务操作句柄
 */
am_zlg118_pcnt_handle_t am_zlg118_pcnt_init (
        am_zlg118_pcnt_dev_t           *p_dev,
        const am_zlg118_pcnt_devinfo_t *p_devinfo);

/**
 * \brief 解除PCNT初始化
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 *
 * \return 无
 */
void am_zlg118_pcnt_deinit (am_zlg118_pcnt_handle_t handle);

/**
 * \brief 采样时钟设置
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 * \param[in] clk    : 时钟
 *
 * \return 无
 */
void am_zlg118_pcnt_clk_sel (am_zlg118_pcnt_handle_t handle,
                             am_zlg118_pcnt_clk_t    clk);

/**
 * \brief 通道极性设置
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 * \param[in] polar  : 通道极性选择
 *
 * \return 无
 */
void am_zlg118_pcnt_polar_set (am_zlg118_pcnt_handle_t handle,
                               am_zlg118_pcnt_polar_t  polar);

/**
 * \brief 滤波使能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 * \param[in] debtop : 滤波计数器阈值
 * \param[in] clkdiv : 滤波时钟分频系数
 *
 * \return 无
 */
void am_zlg118_pcnt_flt_enable (am_zlg118_pcnt_handle_t handle,
                                uint8_t                 debtop,
                                uint16_t                clkdiv);

/**
 * \brief 滤波禁能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 *
 * \return 无
 */
void am_zlg118_pcnt_flt_disable (am_zlg118_pcnt_handle_t handle);

/**
 * \brief 超时使能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 * \param[in] th     : 超时阈值
 *
 * \return 无
 */
void am_zlg118_pcnt_timeover_enable (am_zlg118_pcnt_handle_t handle,
                                     uint16_t                th);

/**
 * \brief 超时禁能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 *
 * \return 无
 */
void am_zlg118_pcnt_timeover_disable (am_zlg118_pcnt_handle_t handle);

/**
 * \brief PCNT使能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 * \param[in] mode   : 计数模式
 * \param[in] dir    : 计数方向
 * \param[in] value  : 溢出值
 * \param[in] dgb    : 输出选项
 *
 * \return 无
 */
void am_zlg118_pcnt_start (am_zlg118_pcnt_handle_t handle,
                           am_zlg118_pcnt_mode_t   mode,
                           am_zlg118_pcnt_dir_t    dir,
                           uint16_t                value,
                           am_zlg118_pcnt_dgb_t    dgb);

/**
 * \brief PCNT禁能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 *
 * \return 无
 */
void am_zlg118_pcnt_stop (am_zlg118_pcnt_handle_t handle);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG118_PCNT_H */

/* end of file */
