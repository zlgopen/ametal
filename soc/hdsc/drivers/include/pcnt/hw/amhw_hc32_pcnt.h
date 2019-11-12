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
 * \brief PCNT操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-11
 * \endinternal
 */

#ifndef __AMHW_HC32_PCNT_H
#define __AMHW_HC32_PCNT_H

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
 * \addtogroup amhw_hc32_if_pcnt
 * \copydoc amhw_hc32_pcnt.h
 * @{
 */

/**
 * \brief PCNT寄存器块结构体
 */
typedef struct amhw_hc32_pcnt {
    __IO uint32_t run;      /* 启动寄存器 */
    __IO uint32_t ctrl;     /* 控制寄存器 */
    __IO uint32_t flt;      /* 滤波控制寄存器 */
    __IO uint32_t tocr;     /* 超时控制寄存器 */
    __O  uint32_t cmd;      /* 命令寄存器 */
    __I  uint32_t sr1;      /* 状态寄存器1 */
    __I  uint32_t cnt;      /* 计数寄存器 */
    __I  uint32_t top;      /* 计数溢出寄存器 */
    __IO uint32_t buf;      /* 计数溢出缓存寄存器 */
    __I  uint32_t ifr;      /* 中断标识寄存器 */
    __O  uint32_t icr;      /* 中断清除寄存器 */
    __IO uint32_t ien;      /* 中断使能寄存器 */
    __I  uint32_t sr2;      /* 状态寄存器2 */
    __IO uint32_t dbg;      /* 输出选择寄存器 */
} amhw_hc32_pcnt_t;

/**
 * \brief PCNT启动
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_start (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    p_hw_pcnt->run |= 0x1ul;
}

/**
 * \brief PCNT停止
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_stop (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    p_hw_pcnt->run &= ~0x1ul;
}

/**
 * \brief PCNT启动/停止状态获取
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 0:停止, 1:启动
 */
am_static_inline
uint8_t amhw_hc32_pcnt_run_get (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    return p_hw_pcnt->run & 0x1ul;
}

/**
 * \brief S1通道极性选择
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  mode       : 极性选择， 1：取反， 0：不取反
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_s1p_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t mode)
{
        p_hw_pcnt->ctrl = (p_hw_pcnt->ctrl & ~(0x1ul << 6)) | \
                          ((mode & 0x1) << 6);
}

/**
 * \brief S0通道极性选择
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  mode       : 极性选择， 1：取反， 0：不取反
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_s0p_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t mode)
{
        p_hw_pcnt->ctrl = (p_hw_pcnt->ctrl & ~(0x1ul << 5)) | \
                          ((mode & 0x1) << 5);
}

/**
 * \brief 计数方向选择(仅对单通道模式和双通道非交模式有效)
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  dir        : 计数方向， 1：减计数， 0：加计数
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_dir_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t dir)
{
        p_hw_pcnt->ctrl = (p_hw_pcnt->ctrl & ~(0x1ul << 4)) | \
                          ((dir & 0x1) << 4);
}

/**
 * \brief 计数方向获取(仅对单通道模式和双通道非交模式有效)
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return  1：减计数， 0：加计数
 */
am_static_inline
uint8_t amhw_hc32_pcnt_dir_get (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    return (p_hw_pcnt->ctrl >> 4) & 0x1ul;
}

/**
 * \brief 时钟选择
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  clksel     : 时钟选择， 0:PCLK
 *                                     1:PCLK
 *                                     2:XTL
 *                                     3:RCL
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_clk_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t clksel)
{
    p_hw_pcnt->ctrl = (p_hw_pcnt->ctrl & ~(0x3ul << 2)) | \
                      ((clksel & 0x3ul)<< 2);
}

/**
 * \brief 计数模式选择
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  mode       : 时钟选择， 0:单通道脉冲计数模式
 *                                     1:单通道脉冲计数模式
 *                                     2:双通道非交脉冲计数模式
 *                                     3:双通道正交脉冲计数模式
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_mode_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t mode)
{
    p_hw_pcnt->ctrl = (p_hw_pcnt->ctrl & ~0x3ul) | (mode & 0x3ul);
}

/**
 * \brief 脉冲宽度滤波器使能
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_flt_enable (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    p_hw_pcnt->flt |= (0x1ul << 16);
}

/**
 * \brief 脉冲宽度滤波器禁能
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_flt_disable (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    p_hw_pcnt->flt &= ~(0x1ul << 16);
}

/**
 * \brief 滤波计数器阈值设置
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  top        : 计数器阈值,不能为0
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_debtop_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t top)
{
    if(top > 0) {
        p_hw_pcnt->flt = (p_hw_pcnt->flt & ~(0x7ul << 13)) | \
                         ((top & 0x7ul) << 13);
    }
}

/**
 * \brief 滤波时钟分频系数设置
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  div        : 分频系数
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_clkdiv_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint16_t div)
{
    p_hw_pcnt->flt = (p_hw_pcnt->flt & ~0x1ffful) | (div & 0x1ffful);
}

/**
 * \brief 超时功能使能
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_timeover_enable (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    p_hw_pcnt->tocr |= (0x1ul << 16);
}

/**
 * \brief 超时功能禁能
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_timeover_disable (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    p_hw_pcnt->tocr &= ~(0x1ul << 16);
}

/**
 * \brief 超时阈值设置
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  th         : 超时阈值
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_th_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint16_t th)
{
    p_hw_pcnt->tocr = (p_hw_pcnt->tocr & ~0xffful) | (th & 0xffful);
}

/**
 * \brief 命令寄存器设置
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  cmd        : 命令
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_cmd_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint32_t cmd)
{
    p_hw_pcnt->cmd |= cmd;
}

/**
 * \brief 双通道正交脉冲计数方向获取
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 0:加计数, 1:减计数
 */
am_static_inline
uint8_t amhw_hc32_pcnt_ddir_get (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    return p_hw_pcnt->sr1 & 0x1ul;
}

/**
 * \brief 计数器计数值获取
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 计数值
 */
am_static_inline
uint16_t amhw_hc32_pcnt_cnt_get (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    return p_hw_pcnt->cnt & 0xfffful;
}

/**
 * \brief 计数器上溢出值/下溢出重载值获取
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 溢出值
 */
am_static_inline
uint16_t amhw_hc32_pcnt_top_get (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    return p_hw_pcnt->top & 0xfffful;
}

/**
 * \brief 计数溢出缓存获取
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 *
 * \return 缓存的值
 */
am_static_inline
uint16_t amhw_hc32_pcnt_buf_get (amhw_hc32_pcnt_t *p_hw_pcnt)
{
    return p_hw_pcnt->buf & 0xfffful;
}

/**
 * \brief 缓存值设置
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  buf        : 缓存值
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_buf_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint16_t buf)
{
    p_hw_pcnt->buf = buf;
}

/**
 * \brief 中断标识获取
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  ie         : 中断标识
 *
 * \return 0:无中断标识, 非0:有中断标识
 */
am_static_inline
uint16_t amhw_hc32_pcnt_int_get (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t ie)
{
    return p_hw_pcnt->buf & ie;
}

/**
 * \brief 中断标识清除
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  ie         : 中断标识
 *
 * \return
 */
am_static_inline
void amhw_hc32_pcnt_int_clear (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t ie)
{
    p_hw_pcnt->buf &= ~ie;
}

/**
 * \brief 中断使能
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  ie         : 中断标识
 *
 * \return
 */
am_static_inline
void amhw_hc32_pcnt_int_enable (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t ie)
{
    p_hw_pcnt->ien |= ie;
}

/**
 * \brief 中断禁能
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  ie         : 中断标识
 *
 * \return
 */
am_static_inline
void amhw_hc32_pcnt_int_disable (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t ie)
{
    p_hw_pcnt->ien &= ~ie;
}

/**
 * \brief 同步状态获取
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  cmd         : 同步命令
 *
 * \return 0:同步完成, 非0:同步进行中
 */
am_static_inline
uint16_t amhw_hc32_pcnt_stat_get (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t cmd)
{
    return p_hw_pcnt->sr2 & cmd;
}

/**
 * \brief 输出选择
 *
 * \papcnt[in]  p_hw_pcnt  : 指向PCNT寄存器结构体的指针
 * \papcnt[in]  sel        : 输出选项
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pcnt_dbg_set (amhw_hc32_pcnt_t *p_hw_pcnt, uint8_t sel)
{
    p_hw_pcnt->buf = sel;
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
#endif /* __AMHW_HC32_PCNT_H */

#endif

/* end of file */
