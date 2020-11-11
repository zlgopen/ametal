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
 * \brief LVD 底层操作接口
 * \internal
 * \par Modification History
 * - 1.01 20-04-27 zcb, Modify the error namespace ANHW_ to AMHW_
 * - 1.00 19-10-08
 * \endinternal
 */

#ifndef __AMHW_ZSL42x_LVD_H
#define __AMHW_ZSL42x_LVD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zsl42x_if_lvd
 * \copydoc amhw_zsl42x_lvd.h
 * @{
 */

/**
 * \brief LVD - 寄存器组
 */
typedef struct amhw_zsl42x_lvd {
         uint8_t   res[40];      /**< \brief  保留 */
    __IO uint32_t  lvccr;        /**< \brief  LVD 配置寄存器 */
    __IO uint32_t  lvcifr;       /**< \brief  LVD 中断寄存器 */
} amhw_zsl42x_lvd_t;

/**
 * \brief LVD 中断使能
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_int_enable (amhw_zsl42x_lvd_t *p_hw_lvd)
{
    p_hw_lvd->lvccr |= (1 << 15);
}

/**
 * \brief LVD 中断禁能
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_int_disable (amhw_zsl42x_lvd_t *p_hw_lvd)
{
    p_hw_lvd->lvccr &= ~(1 << 15);
}

/*
 * \brief LVD 触发使能掩码
 */
#define AMHW_ZSL42x_LVD_HTEN    (1U << 14)    /**< \brief 高电平触发使能 */
#define AMHW_ZSL42x_LVD_RTEN    (1U << 13)    /**< \brief 上升沿触发使能 */
#define AMHW_ZSL42x_LVD_FTEN    (1U << 12)    /**< \brief 下降沿触发使能 */

/**
 * \brief LVD 触发使能
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 * \param[in] flag    : 参考宏定义：LVD 触发使能掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_tri_enable (amhw_zsl42x_lvd_t *p_hw_lvd, uint32_t flag)
{
    switch (flag){

        case AMHW_ZSL42x_LVD_FTEN : p_hw_lvd->lvccr |= flag;
                                    p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_RTEN;
                                    p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_HTEN;
            break;
        case AMHW_ZSL42x_LVD_RTEN : p_hw_lvd->lvccr |= flag;
                                    p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_FTEN;
                                    p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_HTEN;
            break;
        case AMHW_ZSL42x_LVD_HTEN : p_hw_lvd->lvccr |= flag;
                                    p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_FTEN;
                                    p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_RTEN;
            break;
        case AMHW_ZSL42x_LVD_FTEN | AMHW_ZSL42x_LVD_RTEN :
            p_hw_lvd->lvccr |= AMHW_ZSL42x_LVD_FTEN;
            p_hw_lvd->lvccr |= AMHW_ZSL42x_LVD_RTEN;
            p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_HTEN;
            break;
        case AMHW_ZSL42x_LVD_FTEN | AMHW_ZSL42x_LVD_HTEN :
            p_hw_lvd->lvccr |= AMHW_ZSL42x_LVD_FTEN;
            p_hw_lvd->lvccr |= AMHW_ZSL42x_LVD_HTEN;
            p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_RTEN;
            break;
        case AMHW_ZSL42x_LVD_HTEN | AMHW_ZSL42x_LVD_RTEN :
            p_hw_lvd->lvccr |= AMHW_ZSL42x_LVD_HTEN;
            p_hw_lvd->lvccr |= AMHW_ZSL42x_LVD_RTEN;
            p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_FTEN;
            break;
        case AMHW_ZSL42x_LVD_HTEN | AMHW_ZSL42x_LVD_RTEN | AMHW_ZSL42x_LVD_FTEN:
            p_hw_lvd->lvccr |= AMHW_ZSL42x_LVD_HTEN;
            p_hw_lvd->lvccr |= AMHW_ZSL42x_LVD_RTEN;
            p_hw_lvd->lvccr |= AMHW_ZSL42x_LVD_FTEN;
            break;
        default:
            p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_HTEN;
            p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_RTEN;
            p_hw_lvd->lvccr &= ~AMHW_ZSL42x_LVD_FTEN;
    }
}

/* * \brief LVD 触发禁能掩码
 */
#define AMHW_ZSL42x_LVD_HTDIS    (1U << 14)    /**< \brief 高电平触发禁能 */
#define AMHW_ZSL42x_LVD_RTDIS    (1U << 13)    /**< \brief 上升沿触发禁能 */
#define AMHW_ZSL42x_LVD_FTDIS    (1U << 12)    /**< \brief 下降沿触发禁能 */

/**
 * \brief LVD 触发禁能
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 * \param[in] flag    : 参考宏定义：LVD 触发禁能掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_tri_disable (amhw_zsl42x_lvd_t *p_hw_lvd, uint32_t flag)
{
    p_hw_lvd->lvccr &= ~flag;
}

/**
 * \brief LVD 数字滤波时间
 */
typedef enum amhw_zsl42x_lvd_deb_time {
    AMHW_ZSL42x_LVD_DEB_TIME_7_US = 0,
    AMHW_ZSL42x_LVD_DEB_TIME_14_US,
    AMHW_ZSL42x_LVD_DEB_TIME_28_US,
    AMHW_ZSL42x_LVD_DEB_TIME_112_US,
    AMHW_ZSL42x_LVD_DEB_TIME_450_US,
    AMHW_ZSL42x_LVD_DEB_TIME_1P8_MS,
    AMHW_ZSL42x_LVD_DEB_TIME_7P2_MS,
    AMHW_ZSL42x_LVD_DEB_TIME_28P8_MS,
}amhw_zsl42x_lvd_deb_time_t;

/**
 * \brief LVD 数字滤波时间选择
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 * \param[in] flag    : 参考枚举：LVD 数字滤波时间
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_deb_time_sel (amhw_zsl42x_lvd_t *p_hw_lvd, uint32_t flag)
{
    p_hw_lvd->lvccr = (p_hw_lvd->lvccr & (~(0x7 << 9))) | (flag << 9);
}

/**
 * \brief LVD 数字滤波使能
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_deb_time_enable (amhw_zsl42x_lvd_t *p_hw_lvd)
{
    p_hw_lvd->lvccr |= (1 << 8);
}

/**
 * \brief LVD 数字滤波禁能
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_deb_time_disable (amhw_zsl42x_lvd_t *p_hw_lvd)
{
    p_hw_lvd->lvccr &= ~(1 << 8);
}

/**
 * \brief LVD 阈值电压
 */
typedef enum amhw_zsl42x_lvd_vids {
    AMHW_ZSL42x_LVD_VIDS_1P8_V = 0,
    AMHW_ZSL42x_LVD_VIDS_1P9_V,
    AMHW_ZSL42x_LVD_VIDS_2P0_V,
    AMHW_ZSL42x_LVD_VIDS_2P1_V,
    AMHW_ZSL42x_LVD_VIDS_2P2_V,
    AMHW_ZSL42x_LVD_VIDS_2P3_V,
    AMHW_ZSL42x_LVD_VIDS_2P4_V,
    AMHW_ZSL42x_LVD_VIDS_2P5_V,
    AMHW_ZSL42x_LVD_VIDS_2P6_V,
    AMHW_ZSL42x_LVD_VIDS_2P7_V,
    AMHW_ZSL42x_LVD_VIDS_2P8_V,
    AMHW_ZSL42x_LVD_VIDS_2P9_V,
    AMHW_ZSL42x_LVD_VIDS_3P0_V,
    AMHW_ZSL42x_LVD_VIDS_3P1_V,
    AMHW_ZSL42x_LVD_VIDS_3P2_V,
    AMHW_ZSL42x_LVD_VIDS_3P3_V,
}amhw_zsl42x_lvd_vids_t;

/**
 * \brief LVD 阈值电压选择
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 * \param[in] flag    : 参考枚举：LVD 阈值电压
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_vids_sel (amhw_zsl42x_lvd_t *p_hw_lvd, uint32_t flag)
{
    p_hw_lvd->lvccr = (p_hw_lvd->lvccr & (~(0xf << 4))) | (flag << 4);
}

/*
 * \brief LVD 监测来源选择掩码
 */
#define AMHW_ZSL42x_LVD_SRC_PB07 (3U) /**< \brief PB07端口输入电压 */
#define AMHW_ZSL42x_LVD_SRC_PB08 (2U) /**< \brief PB08端口输入电压 */
#define AMHW_ZSL42x_LVD_SRC_PC13 (1U) /**< \brief PC13端口输入电压 */
#define AMHW_ZSL42x_LVD_SRC_AVCC (0U) /**< \brief AVCC电源电压*/

/**
 * \brief LVD 检测来源选择
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 * \param[in] flag    : 参考宏定义：LVD 监测来源选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_src_sel (amhw_zsl42x_lvd_t *p_hw_lvd, uint32_t flag)
{
    p_hw_lvd->lvccr = (p_hw_lvd->lvccr & (~(0x3 << 2))) | (flag << 2);
}

/*
 * \brief LVD 触发动作选择掩码
 */
#define AMHW_ZSL42x_LVD_TRI_ACT_SYSTEM_RESET  (1U)   /**< \brief 系统复位 */
#define AMHW_ZSL42x_LVD_TRI_ACT_NVIC_INT      (0U)   /**< \brief NVIC中断 */

/**
 * \brief LVD 触发动作选择
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 * \param[in] flag    : 参考宏定义：LVD 触发动作选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_tri_act_sel (amhw_zsl42x_lvd_t *p_hw_lvd, uint32_t flag)
{
    p_hw_lvd->lvccr = (p_hw_lvd->lvccr & (~(0x1 << 1))) | (flag << 1);
}

/**
 * \brief LVD 使能
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_enable (amhw_zsl42x_lvd_t *p_hw_lvd)
{
    p_hw_lvd->lvccr |= (1 << 0);
}

/**
 * \brief LVD 禁能
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_disable (amhw_zsl42x_lvd_t *p_hw_lvd)
{
    p_hw_lvd->lvccr &= ~(1 << 0);
}

/**
 * \brief LVD cr寄存器清零
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_lvd_cr_clr (amhw_zsl42x_lvd_t *p_hw_lvd)
{
    p_hw_lvd->lvccr = 0;
}

/**
 * \brief LVD 滤波状态获取
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 *
 * \retval : LVD 滤波状态
 */
am_static_inline
int amhw_zsl42x_flt_status_check (amhw_zsl42x_lvd_t *p_hw_lvd)
{
    return (p_hw_lvd->lvcifr & (1 << 1));
}

/**
 * \brief LVD 中断状态获取
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 *
 * \retval : LVD 中断状态
 */
am_static_inline
int amhw_zsl42x_int_status_check (amhw_zsl42x_lvd_t *p_hw_lvd)
{
    return (p_hw_lvd->lvcifr & 0x1);
}

/**
 * \brief 中断状态标志清除
 *
 * \param[in] p_hw_lvd : 指向LVD寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_int_falg_clr (amhw_zsl42x_lvd_t *p_hw_lvd)
{
    p_hw_lvd->lvcifr &= ~(1 << 0);
}

/**
 * @} amhw_if_zsl42x_lvd
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZSL42x_LVD_H */

/* end of file */
