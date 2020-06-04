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
 * \brief I2C操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-17  sdy, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG_I2C_H
#define __AMHW_ZLG_I2C_H

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
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/**
 * \addtogroup amhw_zlg_if_i2c
 * \copydoc amhw_zlg_i2c.h
 * @{
 */

/**
 * \brief I2C寄存器块结构体
 */
typedef struct amhw_zlg_i2c {
    __IO uint16_t  ic_con;                /**< \brief I2C控制寄存器 */
         uint16_t  reserved0;             /**< \brief 保留0 */
    __IO uint16_t  ic_tar;                /**< \brief I2C目标地址寄存器 */
         uint16_t  reserved1;             /**< \brief 保留1 */
    __IO uint16_t  ic_sar;                /**< \brief I2C从机地址寄存器 */
         uint16_t  reserved2;             /**< \brief 保留2 */
    __IO uint16_t  ic_hs_maddr;           /**< \brief 暂且保留 */
         uint16_t  reserved3;             /**< \brief 保留3 */
    __IO uint16_t  ic_data_cmd;           /**< \brief I2C数据命令寄存器 */
         uint16_t  reserved4;             /**< \brief 保留4 */
    __IO uint16_t  ic_ss_scl_hcnt;        /**< \brief 标准模式 I2C时钟高电平计数寄存器 */
         uint16_t  reserved5;             /**< \brief 保留5 */
    __IO uint16_t  ic_ss_scl_lcnt;        /**< \brief 标准模式 I2C时钟低电平计数寄存器 */
         uint16_t  reserved6;             /**< \brief 保留6 */
    __IO uint16_t  ic_fs_scl_hcnt;        /**< \brief 快速模式 I2C时钟高电平计数寄存器 */
         uint16_t  reserved7;             /**< \brief 保留7 */
    __IO uint16_t  ic_fs_scl_lcnt;        /**< \brief 快速模式 I2C时钟低电平计数寄存器 */
         uint16_t  reserved8;             /**< \brief 保留8 */
    __IO uint16_t  ic_hs_scl_hcnt;        /**< \brief 暂且保留 */
         uint16_t  reserved9;             /**< \brief 保留9 */
    __IO uint16_t  ic_hs_scl_lcnt;        /**< \brief 暂且保留 */
         uint16_t  reserved10;            /**< \brief 保留10 */
    __IO uint16_t  ic_intr_stat;          /**< \brief I2C中断状态寄存器 */
         uint16_t  reserved11;            /**< \brief 保留11 */
    __IO uint16_t  ic_intr_mask;          /**< \brief I2C中断屏蔽寄存器 */
         uint16_t  reserved12;            /**< \brief 保留12 */
    __IO uint16_t  ic_raw_intr_stat;      /**< \brief I2C RAW 中断寄存器 */
         uint16_t  reserved13;            /**< \brief 保留13 */
    __IO uint16_t  ic_rx_tl;              /**< \brief I2C接收阈值 */
         uint16_t  reserved14;            /**< \brief 保留14 */
    __IO uint16_t  ic_tx_tl;              /**< \brief I2C发送阈值 */
         uint16_t  reserved15;            /**< \brief 保留15 */

    __IO uint16_t  ic_clr_intr;           /**< \brief I2C组合和独立中断清除寄存器 */
         uint16_t  reserved16;            /**< \brief 保留16 */
    __IO uint16_t  ic_clr_rx_under;       /**< \brief I2C清除RX_UNDER中断寄存器 */
         uint16_t  reserved17;            /**< \brief 保留17 */
    __IO uint16_t  ic_clr_rx_over;        /**< \brief I2C清除RX_OVER中断寄存器 */
         uint16_t  reserved18;            /**< \brief 保留18 */
    __IO uint16_t  ic_clr_tx_over;        /**< \brief I2C清除TX_OVER中断寄存器 */
         uint16_t  reserved19;            /**< \brief 保留19 */
    __IO uint16_t  ic_clr_rd_req;         /**< \brief I2C清除RD_REQ中断寄存器 */
         uint16_t  reserved20;            /**< \brief 保留20 */
    __IO uint16_t  ic_clr_tx_abrt;        /**< \brief I2C清除TX_ABRT中断寄存器 */
         uint16_t  reserved21;            /**< \brief 保留21 */
    __IO uint16_t  ic_clr_rx_done;        /**< \brief I2C清除RX_DONE中断寄存器 */
         uint16_t  reserved22;            /**< \brief 保留22 */
    __IO uint16_t  ic_clr_activity;       /**< \brief I2C清除 ACTIVITY中断寄存器 */
         uint16_t  reserved23;            /**< \brief 保留23 */
    __IO uint16_t  ic_clr_stop_det;       /**< \brief I2C清除STOP_DET中断寄存器 */
         uint16_t  reserved24;            /**< \brief 保留24 */
    __IO uint16_t  ic_clr_start_det;      /**< \brief I2C清除START_DET中断寄存器 */
         uint16_t  reserved25;            /**< \brief 保留25 */
    __IO uint16_t  ic_clr_gen_call;       /**< \brief I2C清除 GEN_CALL 中断寄存器 */
         uint16_t  reserved26;            /**< \brief 保留26 */
    __IO uint16_t  ic_enable;             /**< \brief I2C使能寄存器 */
         uint16_t  reserved27;            /**< \brief 保留27 */

    __IO uint32_t  ic_status;             /**< \brief I2C状态寄存器 */
    __IO uint32_t  ic_txflr;              /**< \brief I2C发送缓冲水平寄存器 */
    __IO uint32_t  ic_rxflr;              /**< \brief I2C接收缓冲水平寄存器 */
    __IO uint32_t  ic_sda_hold;           /**< \brief I2C SDA保持时间寄存器 */
    __IO uint32_t  ic_tx_abrt_source;     /**< \brief 暂且保留 */
    __IO uint32_t  ic_slv_data_nack_only; /**< \brief 暂且保留 */
    __IO uint32_t  ic_dma_cr;             /**< \brief I2C DMA控制寄存器 */
    __IO uint32_t  ic_dma_tdlr;           /**< \brief 暂且保留 */
    __IO uint32_t  ic_dma_rdlr;           /**< \brief 暂且保留 */
    __IO uint32_t  ic_sda_setup;          /**< \brief I2C SDA建立时间寄存器 */
    __IO uint32_t  ic_ack_general_call;   /**< \brief I2C广播呼叫 ACK寄存器 */
    __IO uint32_t  ic_enable_status;      /**< \brief 暂且保留 */

    __IO uint32_t  ic_fs_spklen;          /**< \brief 暂且保留 */
    __IO uint32_t  ic_hs_spklen;          /**< \brief 暂且保留 */

    __IO uint16_t  ic_clr_restart_det;    /**< \brief 暂且保留 */
         uint16_t  reserved28;            /**< \brief 保留28 */
    __IO uint32_t  ic_comp_param_1;       /**< \brief 暂且保留 */
    __IO uint32_t  ic_comp_version;       /**< \brief 暂且保留 */
    __IO uint32_t  ic_comp_type;          /**< \brief 暂且保留 */
} amhw_zlg_i2c_t;

/**
 * \brief
 */

/** \brief 主机模式 */
#define AMHW_ZLG_I2C_MASTER                  (1ul | (1ul << 6))

/** \brief 从机模式 */
#define AMHW_ZLG_I2C_SLAVE                   (0ul | (0ul << 6))

/** \brief 标准模式(0 ~ 100Kbps),主机模式下有效 */
#define AMHW_ZLG_I2C_NORMAL_SPEED            (1ul << 1)

/** \brief 快速模式(<=400Kbps),主机模式下有效 */
#define AMHW_ZLG_I2C_HIGH_SPEED              (2ul << 1)

/** \brief 7 位的寻址地址,从机模式下有效 */
#define AMHW_ZLG_I2C_7BITADDR_SLAVE          (0ul << 3)

/** \brief 10 位的寻址地址,从机模式下有效 */
#define AMHW_ZLG_I2C_10BITADDR_SLAVE         (1ul << 3)

/** \brief 7 位的寻址地址,主机模式下有效 */
#define AMHW_ZLG_I2C_7BITADDR_MASTER         (0ul << 4)

/** \brief 10 位的寻址地址,主机模式下有效 */
#define AMHW_ZLG_I2C_10BITADDR_MASTER        (1ul << 4)

/** \brief 主机模式下使能发送RESTART条件 */
#define AMHW_ZLG_I2C_RESTART_ENABLE          (1ul << 5)

/** \brief 从机模式下使能产生STOP_DET中断 */
#define AMHW_ZLG_I2C_STOP_DET_IFADDRESSED    (1ul << 7)

/**
 * \brief I2C中断标志
 */
typedef enum {
   AMHW_ZLG_INT_FLAG_RX_UNDER = (1ul << 0),      /**< \brief 接收缓冲欠载 */
   AMHW_ZLG_INT_FLAG_RX_OVER  = (1ul << 1),      /**< \brief 接受缓冲过载 */
   AMHW_ZLG_INT_FLAG_RX_FULL  = (1ul << 2),      /**< \brief 接收缓冲非空 */
   AMHW_ZLG_INT_FLAG_TX_OVER  = (1ul << 3),      /**< \brief 发送缓冲过载 */
   AMHW_ZLG_INT_FLAG_TX_EMPTY = (1ul << 4),      /**< \brief 发送缓冲空 */
   AMHW_ZLG_INT_FLAG_RD_REQ   = (1ul << 5),      /**< \brief 读请求 */
   AMHW_ZLG_INT_FLAG_TX_ABRT  = (1ul << 6),      /**< \brief 发送中止 */
   AMHW_ZLG_INT_FLAG_RX_DONE  = (1ul << 7),      /**< \brief 从发送结束 */
   AMHW_ZLG_INT_FLAG_ACTIVITY = (1ul << 8),      /**< \brief I2C接口激活 */
   AMHW_ZLG_INT_FLAG_STOP_DET = (1ul << 9),      /**< \brief 停止条件检测 */
   AMHW_ZLG_INT_FLAG_START_DET= (1ul << 10),     /**< \brief 起始条件检测 */
   AMHW_ZLG_INT_FLAG_GEN_CALL = (1ul << 11),     /**< \brief 广播呼叫 */
}amhw_zlg_int_flag_t;

/**
 * \brief I2C状态标志
 */
typedef enum {
   AMHW_ZLG_STATUS_FLAG_ACTIVITY     = (1ul << 0),   /**< \brief I2C位活动状态 */
   AMHW_ZLG_STATUS_FLAG_TFNF         = (1ul << 1),   /**< \brief 发送缓冲未满 */
   AMHW_ZLG_STATUS_FLAG_TFE          = (1ul << 2),   /**< \brief 发送缓冲空 */
   AMHW_ZLG_STATUS_FLAG_RFNE         = (1ul << 3),   /**< \brief 接收缓冲非空 */
   AMHW_ZLG_STATUS_FLAG_RFF          = (1ul << 4),   /**< \brief 接收缓冲满 */
   AMHW_ZLG_STATUS_FLAG_MST_ACTIVITY = (1ul << 5),   /**< \brief 主机状态机活动状态位 */
   AMHW_ZLG_STATUS_FLAG_SLV_ACTIVITY = (1ul << 6),   /**< \brief 从机状态机活动状态位 */
}amhw_zlg_status_flag_t;

/**
 * \brief 设置控制寄存器值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  flag      : 标志
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_con_set (amhw_zlg_i2c_t    *p_hw_i2c,
                           uint16_t           flag)
{
    p_hw_i2c->ic_con |= flag;
}

/**
 * \brief 清除控制寄存器值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  flag      : 标志
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_con_clear (amhw_zlg_i2c_t    *p_hw_i2c,
                             uint16_t           flag)
{
    p_hw_i2c->ic_con &= ~((uint32_t)flag);
}

/**
 * \brief 设置主操作的目标地址.
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  addr      : 目标地址
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_tar_set (amhw_zlg_i2c_t    *p_hw_i2c,
                           uint16_t           addr)
{
    p_hw_i2c->ic_tar = (p_hw_i2c->ic_tar & ~0x3ff) | (addr & 0x3ff);
}

/**
 * \brief 设置从机地址.
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  addr      : 从机地址
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_slave_addr_set (amhw_zlg_i2c_t    *p_hw_i2c,
                                  uint16_t           addr)
{
    p_hw_i2c->ic_sar = (p_hw_i2c->ic_sar & ~0x3ff) | (addr & 0x3ff);
}

/**
 * \brief 写数据.
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  dat       : 数据
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_dat_write (amhw_zlg_i2c_t    *p_hw_i2c,
                             uint16_t           dat)
{
    p_hw_i2c->ic_data_cmd = dat;
}

/**
 * \brief 读数据.
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 数据
 */
am_static_inline
uint8_t amhw_zlg_i2c_dat_read (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_data_cmd);
}

/**
 * \brief 写I2C标准模式下时钟高电平计数值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  val       : 数据(6 ~ 65525)
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_ss_scl_high_set (amhw_zlg_i2c_t    *p_hw_i2c,
                                   uint16_t           val)
{
    p_hw_i2c->ic_ss_scl_hcnt = (p_hw_i2c->ic_ss_scl_hcnt & ~0xffff) | val;
}

/**
 * \brief 读I2C标准模式下时钟高电平计数值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return I2C标准模式下时钟高电平计数值(6 ~ 65525)
 */
am_static_inline
uint16_t amhw_zlg_i2c_ss_scl_high_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint16_t)(p_hw_i2c->ic_ss_scl_hcnt & 0xffff);
}

/**
 * \brief 写I2C标准模式下时钟低电平计数值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  val       : 数据(8 ~ 65535)
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_ss_scl_low_set (amhw_zlg_i2c_t    *p_hw_i2c,
                                  uint16_t           val)
{
    p_hw_i2c->ic_ss_scl_lcnt = (p_hw_i2c->ic_ss_scl_lcnt & ~0xffff) | val;
}

/**
 * \brief 读I2C标准模式下时钟低电平计数值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return I2C标准模式下时钟低电平计数值(8 ~ 65535)
 */
am_static_inline
uint16_t amhw_zlg_i2c_ss_scl_low_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint16_t)(p_hw_i2c->ic_ss_scl_lcnt & 0xffff);
}

/**
 * \brief 写I2C快速模式下时钟高电平计数值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  val       : 数据(6 ~ 65535)
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_fs_scl_high_set (amhw_zlg_i2c_t    *p_hw_i2c,
                                   uint16_t           val)
{
    p_hw_i2c->ic_fs_scl_hcnt = (p_hw_i2c->ic_fs_scl_hcnt & ~0xffff) | val;
}

/**
 * \brief 读I2C快速模式下时钟高电平计数值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return I2C速模式下时钟高电平计数值(6 ~ 65525)
 */
am_static_inline
uint16_t amhw_zlg_i2c_fs_scl_high_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint16_t)(p_hw_i2c->ic_fs_scl_hcnt & 0xffff);
}

/**
 * \brief 写I2C快速模式下时钟低电平计数值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  val       : 数据(8 ~ 65535)
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_fs_scl_low_set (amhw_zlg_i2c_t    *p_hw_i2c,
                                  uint16_t           val)
{
    p_hw_i2c->ic_fs_scl_lcnt = (p_hw_i2c->ic_fs_scl_lcnt & ~0xffff) | val;
}

/**
 * \brief 读I2C快速模式下时钟低电平计数值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return I2C快速模式下时钟低电平计数值(8 ~ 65535)
 */
am_static_inline
uint16_t amhw_zlg_i2c_fs_scl_low_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint16_t)(p_hw_i2c->ic_fs_scl_lcnt & 0xffff);
}

/**
 * \brief 获取可屏蔽中断状态寄存器值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 可屏蔽中断状态寄存器值(或关系)
 */
am_static_inline
amhw_zlg_int_flag_t amhw_zlg_i2c_intr_stat_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (amhw_zlg_int_flag_t)(p_hw_i2c->ic_intr_stat & 0xffff);
}

/**
 * \brief 设置中断屏蔽寄存器值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  flag      : 具体标志，或关系
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_intr_mask_set (amhw_zlg_i2c_t    *p_hw_i2c,
                                 uint16_t           flag)
{
    p_hw_i2c->ic_intr_mask |= flag;
}

/**
 * \brief 设置中断屏蔽寄存器值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  flag      : 具体标志，或关系
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_intr_mask_clear (amhw_zlg_i2c_t    *p_hw_i2c,
                                   uint16_t           flag)
{
    p_hw_i2c->ic_intr_mask &= ~((uint32_t)flag);
}

/**
 * \brief 获取中断屏蔽寄存器值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 中断屏蔽寄存器值(或关系)
 */
am_static_inline
amhw_zlg_int_flag_t amhw_zlg_i2c_intr_mask_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (amhw_zlg_int_flag_t)(p_hw_i2c->ic_intr_mask & 0xffff);
}

/**
 * \brief 获取不可屏蔽中断状态寄存器值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 不可屏蔽中断状态寄存器值(或关系)
 */
am_static_inline
amhw_zlg_int_flag_t amhw_zlg_i2c_raw_intr_stat_get (
                                                    amhw_zlg_i2c_t *p_hw_i2c)
{
    return (amhw_zlg_int_flag_t)(p_hw_i2c->ic_raw_intr_stat & 0xffff);
}

/**
 * \brief 设置接收阈值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  val       : 阈值
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_rx_tl_set (amhw_zlg_i2c_t *p_hw_i2c, uint8_t val)
{
    p_hw_i2c->ic_rx_tl = val & 0xff;
}

/**
 * \brief 获取接收阈值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 接收阈值
 */
am_static_inline
uint8_t amhw_zlg_i2c_rx_tl_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_rx_tl & 0xff);
}

/**
 * \brief 设置发送阈值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  val       : 阈值
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_tx_tl_set (amhw_zlg_i2c_t *p_hw_i2c, uint8_t val)
{
    p_hw_i2c->ic_tx_tl = val & 0xff;
}

/**
 * \brief 获取发送阈值
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 发送阈值
 */
am_static_inline
uint8_t amhw_zlg_i2c_tx_tl_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_tx_tl & 0xff);
}

/**
 * \brief 读取I2C组合和独立中断清除寄存器值，清除I2C组合和独立中断
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return I2C组合和独立中断清除寄存器值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_intr_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_intr & 0x1);
}

/**
 * \brief I2C清除RX_UNDER中断寄存器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_rx_under_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_rx_under & 0x1);
}

/**
 * \brief I2C清除RX_OVER中断寄存器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_rx_over_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_rx_over & 0x1);
}

/**
 * \brief I2C清除TX_OVER中断寄存器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_tx_over_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_tx_over & 0x1);
}

/**
 * \brief I2C清除RD_REQ中断寄存器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_rd_req_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_rd_req & 0x1);
}

/**
 * \brief I2C清除TX_ABRT中断寄存器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_tx_abrt_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_tx_abrt & 0x1);
}

/**
 * \brief I2C清除RX_DONE中断寄存器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_rx_done_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_rx_done & 0x1);
}

/**
 * \brief I2C清除 ACTIVITY中断寄存器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_activity_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_activity & 0x1);
}

/**
 * \brief I2C清除STOP_DET中断寄存器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_stop_det_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_stop_det & 0x1);
}

/**
 * \brief I2C清除START_DET中断寄存器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_start_det_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_start_det & 0x1);
}

/**
 * \brief I2C清除 GEN_CALL 中断寄存器
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 读取值
 */
am_static_inline
uint8_t amhw_zlg_i2c_clr_gen_call_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_clr_gen_call & 0x1);
}

/**
 * \brief 使能 I2C 模块
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_enable (amhw_zlg_i2c_t *p_hw_i2c)
{
    p_hw_i2c->ic_enable |= 0x1ul;
}

/**
 * \brief 禁止 I2C 模块（发送和接收缓冲保持擦除状态）
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_disable (amhw_zlg_i2c_t *p_hw_i2c)
{
    p_hw_i2c->ic_enable &= ~0x1ul;
}

/**
 * \brief 获取I2C状态
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 状态（或关系）
 */
am_static_inline
amhw_zlg_status_flag_t amhw_zlg_i2c_status_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (amhw_zlg_status_flag_t)(p_hw_i2c->ic_status);
}

/**
 * \brief 获取发送缓冲有效数据个数
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 发送缓冲有效数据个数
 */
am_static_inline
uint8_t amhw_zlg_i2c_txflr_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_txflr);
}

/**
 * \brief 获取接收缓冲有效数据个数
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 接收缓冲有效数据个数
 */
am_static_inline
uint8_t amhw_zlg_i2c_rxflr_get (amhw_zlg_i2c_t *p_hw_i2c)
{
    return (uint8_t)(p_hw_i2c->ic_rxflr);
}

/**
 * \brief 使能 I2C 发送DMA
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_txdma_enable (amhw_zlg_i2c_t *p_hw_i2c)
{
    p_hw_i2c->ic_dma_cr |= 0x2ul;
}

/**
 * \brief 禁能I2C 发送DMA
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_txdma_disable (amhw_zlg_i2c_t *p_hw_i2c)
{
    p_hw_i2c->ic_dma_cr &= ~0x2ul;
}

/**
 * \brief 使能 I2C 收DMA
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_rxdma_enable (amhw_zlg_i2c_t *p_hw_i2c)
{
    p_hw_i2c->ic_dma_cr |= 0x1ul;
}

/**
 * \brief 禁能I2C 接收DMA
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_rxdma_disable (amhw_zlg_i2c_t *p_hw_i2c)
{
    p_hw_i2c->ic_dma_cr &= ~0x1ul;
}

/**
 * \brief I2C SDA 建立时间设置
 *
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 * \param[in]  val       : 建立时间值
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_sda_setup_set (amhw_zlg_i2c_t *p_hw_i2c, uint8_t val)
{
    p_hw_i2c->ic_sda_setup = (p_hw_i2c->ic_sda_setup & ~0xfful) | val;
}

/**
 * \brief I2C 传输终止
 *        note:  ZML165芯片在读结束时,需要调用该函数进行传输终止,产生停止位,停止位产生后会触发TX_ABRT中断,
 *               需要在中断产生后在ic_clr_tx_abrt寄存器中写0以保证下次I2C正常传输。
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_trans_abort (amhw_zlg_i2c_t *p_hw_i2c)
{
    p_hw_i2c->ic_enable |= 0x02;
}

/**
 * \brief I2C 清除tx_abrt寄存器
 * \param[in]  p_hw_i2c  : 指向I2C寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg_i2c_clr_tx_abort (amhw_zlg_i2c_t *p_hw_i2c)
{
    p_hw_i2c->ic_clr_tx_abrt = 0;
}


/**
 * \brief I2C接受广播呼叫 响应 ACK
 *
 */
am_static_inline
void amhw_zlg_i2c_gen_call_ack (amhw_zlg_i2c_t *p_hw_i2c)
{
    p_hw_i2c->ic_ack_general_call |= 0x1ul;
}

/**
 * \brief I2C接受广播呼叫 不响应
 *
 */
am_static_inline
void amhw_zlg_i2c_gen_call_nack (amhw_zlg_i2c_t *p_hw_i2c)
{
    p_hw_i2c->ic_ack_general_call &= ~0x1ul;
}

/**
 * @}
 */
/*
 * \brief 匿名结构体段的结束
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

#ifdef __cplusplus
}
#endif /* __AMHW_ZLG_I2C_H */

#endif

/* end of file */
