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
 * \brief PCA驱动接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-09
 * \endinternal
 */

#ifndef __AM_HC32_PCA_H
#define __AM_HC32_PCA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "hw/amhw_hc32_pca.h"

/* 中断标志枚举 */
typedef enum am_hc32_pca_int {
    HC32_PCA_CF   = (0x1 << 7),   /* 计数器溢出标志 */
    HC32_PCA_CCF4 = (0x1 << 4),   /* 计数器模块4比较/捕获标志位 */
    HC32_PCA_CCF3 = (0x1 << 3),   /* 计数器模块3比较/捕获标志位 */
    HC32_PCA_CCF2 = (0x1 << 2),   /* 计数器模块2比较/捕获标志位 */
    HC32_PCA_CCF1 = (0x1 << 1),   /* 计数器模块1比较/捕获标志位 */
    HC32_PCA_CCF0 = (0x1 << 0)    /* 计数器模块0比较/捕获标志位 */
} am_hc32_pca_int_t;

/* 时钟源枚举 */
typedef enum am_hc32_pca_clk {
    HC32_PCA_PCLK32  = (0x0),  /* PCLK/32 */
    HC32_PCA_PCLK16  = (0x1),  /* PCLK/16 */
    HC32_PCA_PCLK8   = (0x2),  /* PCLK/8 */
    HC32_PCA_PCLK4   = (0x3),  /* PCLK/4 */
    HC32_PCA_PCLK2   = (0x4),  /* PCLK/2 */
    HC32_PCA_TIM0_OF = (0x5),  /* timer0 overflow */
    HC32_PCA_TIM1_OF = (0x6),  /* timer1 overflow */
    HC32_PCA_EC1     = (0x7),  /* ECI 外部时钟，时钟PCLK四分频采样 */
} am_hc32_pca_clk_t;

/* 比较捕获模式控制枚举 */
typedef enum am_hc32_pca_ccapm {
    HC32_PCA_ECOM = (0x1 << 6),   /* 允许比较器功能控制位 */
    HC32_PCA_MAT  = (0x1 << 3),   /* 允许匹配控制位 */
    HC32_PCA_PWM  = (0x1 << 1),   /* 脉冲调制控制位 */
    HC32_PCA_CCIE = (0x1 << 0)    /* PCA使能中断 */
} am_hc32_pca_ccapm_t;

/* 边沿捕获枚举 */
typedef enum am_hc32_pca_capn {
    HC32_PCA_CAPP_EN  = (0x1 << 1),   /* 允许正沿捕获 */
    HC32_PCA_CAPP_DIS = (0x0 << 1),   /* 禁止正沿捕获 */
    HC32_PCA_CAPN_EN  = (0x1),        /* 允许负沿捕获 */
    HC32_PCA_CAPN_DIS = (0x0),        /* 禁止负沿捕获 */
} am_hc32_pca_capn_t;

/* 翻转控制枚举 */
typedef enum am_hc32_pca_tog {
    HC32_PCA_TOG_EN   = (0x1),   /* 使能翻转 */
    HC32_PCA_TOG_DIS  = (0x0),   /* 禁能翻转 */
} am_hc32_pca_tog_t;

/* PWM类型选择 */
typedef enum am_hc32_pca_pwm {
    HC32_PCA_PWM_8  = 0,   /* 8位PWM */
    HC32_PCA_PWM_16 = 1    /* 16位PWM */
} am_hc32_pca_pwm_t;

/**
 * \brief PCA对应通道的GPIO信息
 */
typedef struct {
    uint32_t gpio;          /**< \brief 对应的GPIO管脚号 */
    uint32_t pwmunc;        /**< \brief PWM模式对应的GPIO功能设置 */
    uint32_t capfunc;       /**< \brief CAP模式对应的GPIO功能默认设置 */
} am_hc32_pca_ioinfo_t;

/**
 * \brief PCNT设备信息参数结构体
 */
typedef struct am_hc32_pca_devinfo {
    uint32_t                pca_regbase;        /* 寄存器基址 */
    uint32_t                clk_num;            /* 时钟ID */
    uint16_t                inum;               /* 中断号 */
    am_hc32_pca_clk_t     clk_src;            /* 时钟源选择 */
    am_hc32_pca_ioinfo_t  *ioinfo;            /* 引脚配置信息 */
    void                    (*pfn_plfm_init)(void);         /*平台初始化函数 */
    void                    (*pfn_plfm_deinit)(void);       /* 平台解初始化函数 */
} am_hc32_pca_devinfo_t;

typedef struct am_hc32_pca_dev {
    amhw_hc32_pca_t             *p_hw_pca;
    const am_hc32_pca_devinfo_t *p_devinfo;
} am_hc32_pca_dev_t;

typedef am_hc32_pca_dev_t* am_hc32_pca_handle_t;

/**
 * \addtogroup am_zlg_if_pca
 * \copydoc am_zlg_pca.h
 * @{
 */

/**
 * \brief PCA初始化
 *
 * \return PCA服务操作句柄
 */
am_hc32_pca_handle_t am_hc32_pca_init (
        am_hc32_pca_dev_t             *p_dev,
        const am_hc32_pca_devinfo_t   *p_devinfo);

/**
 * \brief 解除PCA初始化
 *
 * \param[in] handle : 与从设备关联的PCA标准服务操作句柄
 *
 * \return 无
 */
void am_hc32_pca_deinit (am_hc32_pca_handle_t handle);

/**
 * \brief PCA边沿捕获模式配置
 *
 * \param[in] handle : 与从设备关联的PCA标准服务操作句柄
 * \param[in] num    : PCA 模块编号
 * \param[in] chan   : 通道号
 * \param[in] cap    : 捕获边沿选择
 *
 * \return AM_OK:成功
 *         其它:失败
 */
int am_hc32_pca_cap_set (am_hc32_pca_handle_t handle,
                           uint8_t                num,
                           uint8_t                chan,
                           am_hc32_pca_capn_t   cap);

/**
 * \brief PCA比较计数模式配置
 *
 * \param[in] handle : 与从设备关联的PCA标准服务操作句柄
 * \param[in] num    : PCA 模块编号
 * \param[in] carr   : 周期寄存器值
 * \param[in] ccap   : 比较捕获寄存器值
 *
 * \return AM_OK:成功
 *         其它:失败
 */
int am_hc32_pca_cmp_cnt_set (am_hc32_pca_handle_t handle,
                               uint8_t                num,
                               uint16_t               carr,
                               uint16_t               ccap);

/**
 * \brief PCA脉宽调制模式配置
 *
 * \param[in] handle  : 与从设备关联的PCA标准服务操作句柄
 * \param[in] num     : PCA 模块编号
 * \param[in] chan    : 通道号
 * \param[in] pwm_sel : PWM类型选择
 * \param[in] carr    : 周期寄存器值
 * \param[in] ccap    : 比较捕获寄存器值
 *
 * 8bit占空比  = (255 - ccap) / 100 * 100%
 * 16bit占空比= (carr - ccap) / carr * 100%
 *
 * \return AM_OK:成功
 *         其它:失败
 */
int am_hc32_pca_pwm_set (am_hc32_pca_handle_t handle,
                           uint8_t                num,
                           uint8_t                chan,
                           am_hc32_pca_pwm_t    pwm_sel,
                           uint16_t               carr,
                           uint16_t               ccap);

/**
 * \brief PCA开始工作
 *
 * \param[in] handle : 与从设备关联的PCA标准服务操作句柄
 *
 * \return
 */
void am_hc32_pca_start(am_hc32_pca_handle_t handle);

/**
 * \brief PCA停止工作
 *
 * \param[in] handle : 与从设备关联的PCA标准服务操作句柄
 *
 * \return
 */
void am_hc32_pca_stop(am_hc32_pca_handle_t handle);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32_PCA_H */

/* end of file */
