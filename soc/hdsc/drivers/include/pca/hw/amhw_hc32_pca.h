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
 * \brief PCA操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-08
 * \endinternal
 */

#ifndef __AMHW_HC32_PCA_H
#define __AMHW_HC32_PCA_H

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
 * \addtogroup amhw_hc32_if_pca
 * \copydoc amhw_hc32_pca.h
 * @{
 */

/**
 * \brief PCA寄存器块结构体
 */
typedef struct amhw_hc32_pca {
    __IO uint32_t ccon;     /* 控制寄存器 */
    __IO uint32_t cmod;     /* 模式寄存器 */
    __IO uint32_t cnt;      /* 计数寄存器 */
    __O  uint32_t iclr;     /* 中断清除寄存器 */
    __IO uint32_t ccapm0;   /* 比较/捕获模块0模式寄存器 */
    __IO uint32_t ccapm1;   /* 比较/捕获模块1模式寄存器 */
    __IO uint32_t ccapm2;   /* 比较/捕获模块2模式寄存器 */
    __IO uint32_t ccapm3;   /* 比较/捕获模块3模式寄存器 */
    __IO uint32_t ccapm4;   /* 比较/捕获模块4模式寄存器 */
    __IO uint32_t ccap0h;   /* 比较/捕获模块0高8位寄存器 */
    __IO uint32_t ccap0l;   /* 比较/捕获模块0低8位寄存器 */
    __IO uint32_t ccap1h;   /* 比较/捕获模块1高8位寄存器 */
    __IO uint32_t ccap1l;   /* 比较/捕获模块1低8位寄存器 */
    __IO uint32_t ccap2h;   /* 比较/捕获模块2高8位寄存器 */
    __IO uint32_t ccap2l;   /* 比较/捕获模块2低8位寄存器 */
    __IO uint32_t ccap3h;   /* 比较/捕获模块3高8位寄存器 */
    __IO uint32_t ccap3l;   /* 比较/捕获模块3低8位寄存器 */
    __IO uint32_t ccap4h;   /* 比较/捕获模块4高8位寄存器 */
    __IO uint32_t ccap4l;   /* 比较/捕获模块4低8位寄存器 */
    __IO uint32_t ccapo;    /* PWM与高速输出标志寄存器 */
    __IO uint32_t ccap0;    /* 比较/捕获模块0的16位寄存器 */
    __IO uint32_t ccap1;    /* 比较/捕获模块1的16位寄存器 */
    __IO uint32_t ccap2;    /* 比较/捕获模块2的16位寄存器 */
    __IO uint32_t ccap3;    /* 比较/捕获模块3的16位寄存器 */
    __IO uint32_t ccap4;    /* 比较/捕获模块4的16位寄存器 */
    __IO uint32_t carr;     /* 周期装载寄存器 */
    __IO uint32_t epwm;     /* PWM增强寄存器 */
} amhw_hc32_pca_t;

/**
 * \brief 计数器开始计数
 *
 * \papca[in]  p_hw_pca  : 指向PCA寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_start (amhw_hc32_pca_t *p_hw_pca)
{
    p_hw_pca->ccon |= (0x1ul << 6);
}

/**
 * \brief 计数器停止计数
 *
 * \papca[in]  p_hw_pca  : 指向PCA寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_stop (amhw_hc32_pca_t *p_hw_pca)
{
    p_hw_pca->ccon &= ~(0x1ul << 6);
}

/**
 * \brief PCA空闲模式工作配置
 *
 * \papca[in]  p_hw_pca  : 指向PCA寄存器结构体的指针
 * \papca[in]  cidl      : 工作模式选择, 1:休眠模式停止工作
 *                                      0:休眠模式继续工作
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_cidl_set (amhw_hc32_pca_t    *p_hw_pca,
                               uint8_t               cidl)
{
    if(cidl > 1) {
        cidl = 1;
    }
    p_hw_pca->cmod &= ~(1ul << 7);
    p_hw_pca->cmod |= cidl << 7;
}

/**
 * \brief PCA WDT功能配置
 *
 * \papca[in]  p_hw_pca  : 指向PCA寄存器结构体的指针
 * \papca[in]  wdte      : 工作模式选择, 1:启动模块4的WDT功能
 *                                      0:关闭模块4的WDT功能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_wdte_set (amhw_hc32_pca_t    *p_hw_pca,
                               uint8_t               wdte)
{
    if(wdte > 1) {
        wdte = 1;
    }
    p_hw_pca->cmod &= ~(1ul << 6);
    p_hw_pca->cmod |= wdte << 6;
}

/**
 * \brief PCA 时钟分频及时钟源配置
 *
 * \papca[in]  p_hw_pca  : 指向PCA寄存器结构体的指针
 * \papca[in]  cps       : 工作模式选择, 1:启动模块4的WDT功能
 *                                      0:关闭模块4的WDT功能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_cps_set (amhw_hc32_pca_t    *p_hw_pca,
                              uint8_t               cps)
{
    if(cps > 7) {
        cps = 7;
    }
    p_hw_pca->cmod &= ~(7ul << 1);
    p_hw_pca->cmod |= cps << 1;
}

/**
 * \brief PCA 计数器中断使能配置
 *
 * \papca[in]  p_hw_pca  : 指向PCA寄存器结构体的指针
 * \papca[in]  cfie      : 工作模式选择, 1:使能中断
 *                                      0:关闭中断
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_cfie_set (amhw_hc32_pca_t    *p_hw_pca,
                               uint8_t               cfie)
{
    if(cfie > 1) {
        cfie = 1;
    }
    p_hw_pca->cmod &= ~1ul;
    p_hw_pca->cmod |= cfie;
}

/**
 * \brief PCA计数寄存器配置
 *
 * \papca[in]  p_hw_pca  : 指向PCA寄存器结构体的指针
 * \papca[in]  data      : 计数值
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_cnt_set (amhw_hc32_pca_t *p_hw_pca, uint8_t data)
{
    p_hw_pca->cnt = data;
}

/**
 * \brief PCA中断获取
 *
 * \papca[in]  p_hw_pca  : 指向PCA寄存器结构体的指针
 * \papca[in]  flag     : 中断标志
 *
 * \return 0:标志位未置位, 1:标志位置位
 */
am_static_inline
uint8_t amhw_hc32_pca_int_get (amhw_hc32_pca_t   *p_hw_pca,
                                 uint8_t              flag)
{
    return (p_hw_pca->ccon & flag);
}

/**
 * \brief PCA中断清除
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  flag     : 中断标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_int_clear (amhw_hc32_pca_t  *p_hw_pca,
                                uint8_t             flag)
{
    p_hw_pca->iclr &= ~(uint32_t)flag;
}

/**
 * \brief PCA 允许比较器功能控制
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 * \papca[in]  ecom     : 配置, 1:使能, 0:禁能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_ecom_set (amhw_hc32_pca_t  *p_hw_pca,
                               uint8_t             num,
                               uint8_t             ecom)
{
    if(ecom > 1) {
        ecom = 1;
    }

    switch(num) {
    case 0:
        p_hw_pca->ccapm0 &= ~(1 << 6);
        p_hw_pca->ccapm0 |= ecom << 6;
        break;

    case 1:
        p_hw_pca->ccapm1 &= ~(1 << 6);
        p_hw_pca->ccapm1 |= ecom << 6;
        break;

    case 2:
        p_hw_pca->ccapm2 &= ~(1 << 6);
        p_hw_pca->ccapm2 |= ecom << 6;
        break;

    case 3:
        p_hw_pca->ccapm3 &= ~(1 << 6);
        p_hw_pca->ccapm3 |= ecom << 6;
        break;

    case 4:
        p_hw_pca->ccapm4 &= ~(1 << 6);
        p_hw_pca->ccapm4 |= ecom << 6;
        break;

    default:
        break;
    }
}

/**
 * \brief PCA 正沿捕获功能控制
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 * \papca[in]  capp     : 配置, 1:使能, 0:禁能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_capp_set (amhw_hc32_pca_t  *p_hw_pca,
                               uint8_t             num,
                               uint8_t             capp)
{
    if(capp > 1) {
        capp = 1;
    }

    switch(num) {
    case 0:
        p_hw_pca->ccapm0 &= ~(1 << 5);
        p_hw_pca->ccapm0 |= capp << 5;
        break;

    case 1:
        p_hw_pca->ccapm1 &= ~(1 << 5);
        p_hw_pca->ccapm1 |= capp << 5;
        break;

    case 2:
        p_hw_pca->ccapm2 &= ~(1 << 5);
        p_hw_pca->ccapm2 |= capp << 5;
        break;

    case 3:
        p_hw_pca->ccapm3 &= ~(1 << 5);
        p_hw_pca->ccapm3 |= capp << 5;
        break;

    case 4:
        p_hw_pca->ccapm4 &= ~(1 << 5);
        p_hw_pca->ccapm4 |= capp << 5;
        break;

    default:
        break;
    }
}

/**
 * \brief PCA 负沿捕获功能控制
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 * \papca[in]  capn     : 配置, 1:使能, 0:禁能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_capn_set (amhw_hc32_pca_t  *p_hw_pca,
                               uint8_t             num,
                               uint8_t             capn)
{
    if(capn > 1) {
        capn = 1;
    }

    switch(num) {
    case 0:
        p_hw_pca->ccapm0 &= ~(1 << 4);
        p_hw_pca->ccapm0 |= capn << 4;
        break;

    case 1:
        p_hw_pca->ccapm1 &= ~(1 << 4);
        p_hw_pca->ccapm1 |= capn << 4;
        break;

    case 2:
        p_hw_pca->ccapm2 &= ~(1 << 4);
        p_hw_pca->ccapm2 |= capn << 4;
        break;

    case 3:
        p_hw_pca->ccapm3 &= ~(1 << 4);
        p_hw_pca->ccapm3 |= capn << 4;
        break;

    case 4:
        p_hw_pca->ccapm4 &= ~(1 << 4);
        p_hw_pca->ccapm4 |= capn << 4;
        break;

    default:
        break;
    }
}

/**
 * \brief PCA 匹配功能控制
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 * \papca[in]  mat      : 配置, 1:使能, 0:禁能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_mat_set (amhw_hc32_pca_t  *p_hw_pca,
                              uint8_t             num,
                              uint8_t             mat)
{
    if(mat > 1) {
        mat = 1;
    }

    switch(num) {
    case 0:
        p_hw_pca->ccapm0 &= ~(1 << 3);
        p_hw_pca->ccapm0 |= mat << 3;
        break;

    case 1:
        p_hw_pca->ccapm1 &= ~(1 << 3);
        p_hw_pca->ccapm1 |= mat << 3;
        break;

    case 2:
        p_hw_pca->ccapm2 &= ~(1 << 3);
        p_hw_pca->ccapm2 |= mat << 3;
        break;

    case 3:
        p_hw_pca->ccapm3 &= ~(1 << 3);
        p_hw_pca->ccapm3 |= mat << 3;
        break;

    case 4:
        p_hw_pca->ccapm4 &= ~(1 << 3);
        p_hw_pca->ccapm4 |= mat << 3;
        break;

    default:
        break;
    }
}

/**
 * \brief PCA 翻转功能控制
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 * \papca[in]  tog      : 配置, 1:使能, 0:禁能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_tog_set (amhw_hc32_pca_t  *p_hw_pca,
                              uint8_t             num,
                              uint8_t             tog)
{
    if(tog > 1) {
        tog = 1;
    }

    switch(num) {
    case 0:
        p_hw_pca->ccapm0 &= ~(1 << 2);
        p_hw_pca->ccapm0 |= tog << 2;
        break;

    case 1:
        p_hw_pca->ccapm1 &= ~(1 << 2);
        p_hw_pca->ccapm1 |= tog << 2;
        break;

    case 2:
        p_hw_pca->ccapm2 &= ~(1 << 2);
        p_hw_pca->ccapm2 |= tog << 2;
        break;

    case 3:
        p_hw_pca->ccapm3 &= ~(1 << 2);
        p_hw_pca->ccapm3 |= tog << 2;
        break;

    case 4:
        p_hw_pca->ccapm4 &= ~(1 << 2);
        p_hw_pca->ccapm4 |= tog << 2;
        break;

    default:
        break;
    }
}

/**
 * \brief PCA 脉宽调制功能控制
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 * \papca[in]  pwm      : 配置, 1:使能, 0:禁能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_pwm_set (amhw_hc32_pca_t  *p_hw_pca,
                              uint8_t             num,
                              uint8_t             pwm)
{
    if(pwm > 1) {
        pwm = 1;
    }

    switch(num) {
    case 0:
        p_hw_pca->ccapm0 &= ~(1 << 1);
        p_hw_pca->ccapm0 |= pwm << 1;
        break;

    case 1:
        p_hw_pca->ccapm1 &= ~(1 << 1);
        p_hw_pca->ccapm1 |= pwm << 1;
        break;

    case 2:
        p_hw_pca->ccapm2 &= ~(1 << 1);
        p_hw_pca->ccapm2 |= pwm << 1;
        break;

    case 3:
        p_hw_pca->ccapm3 &= ~(1 << 1);
        p_hw_pca->ccapm3 |= pwm << 1;
        break;

    case 4:
        p_hw_pca->ccapm4 &= ~(1 << 1);
        p_hw_pca->ccapm4 |= pwm << 1;
        break;

    default:
        break;
    }
}

/**
 * \brief PCA 中断使能控制
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 * \papca[in]  ccie     : 配置, 1:使能, 0:禁能
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_ccie_set (amhw_hc32_pca_t  *p_hw_pca,
                               uint8_t             num,
                               uint8_t             ccie)
{
    if(ccie > 1) {
        ccie = 1;
    }

    switch(num) {
    case 0:
        p_hw_pca->ccapm0 &= ~(1);
        p_hw_pca->ccapm0 |= ccie;
        break;

    case 1:
        p_hw_pca->ccapm1 &= ~(1);
        p_hw_pca->ccapm1 |= ccie;
        break;

    case 2:
        p_hw_pca->ccapm2 &= ~(1);
        p_hw_pca->ccapm2 |= ccie;
        break;

    case 3:
        p_hw_pca->ccapm3 &= ~(1);
        p_hw_pca->ccapm3 |= ccie;
        break;

    case 4:
        p_hw_pca->ccapm4 &= ~(1);
        p_hw_pca->ccapm4 |= ccie;
        break;

    default:
        break;
    }
}

/**
 * \brief PCA比较捕获数据寄存器高八位配置
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  data     : 计数值
 * \papca[in]  num      : 模块号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_ccaph_set (amhw_hc32_pca_t  *p_hw_pca,
                                uint8_t             num,
                                uint8_t             data)
{
    switch(num) {
    case 0:
        p_hw_pca->ccap0h = data;
        break;

    case 1:
        p_hw_pca->ccap1h = data;
        break;

    case 2:
        p_hw_pca->ccap2h = data;
        break;

    case 3:
        p_hw_pca->ccap3h = data;
        break;

    case 4:
        p_hw_pca->ccap4h = data;
        break;

    default:
        break;
    }
}

/**
 * \brief PCA比较捕获数据寄存器高八位获取
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_hc32_pca_ccaph_get (amhw_hc32_pca_t  *p_hw_pca,
                                   uint8_t             num)
{
    switch(num) {
    case 0:
        return p_hw_pca->ccap0h & 0xff;

    case 1:
        return p_hw_pca->ccap1h & 0xff;

    case 2:
        return p_hw_pca->ccap2h & 0xff;

    case 3:
        return p_hw_pca->ccap3h & 0xff;

    case 4:
        return p_hw_pca->ccap4h & 0xff;

    default:
        break;
    }

    return 0;
}

/**
 * \brief PCA比较捕获数据寄存器低八位配置
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  data     : 计数值
 * \papca[in]  num      : 模块号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_ccapl_set (amhw_hc32_pca_t  *p_hw_pca,
                                uint8_t             num,
                                uint8_t             data)
{
    switch(num) {
    case 0:
        p_hw_pca->ccap0l = data;
        break;

    case 1:
        p_hw_pca->ccap1l = data;
        break;

    case 2:
        p_hw_pca->ccap2l = data;
        break;

    case 3:
        p_hw_pca->ccap3l = data;
        break;

    case 4:
        p_hw_pca->ccap4l = data;
        break;

    default:
        break;
    }
}

/**
 * \brief PCA比较捕获数据寄存器低八位获取
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_hc32_pca_ccapl_get (amhw_hc32_pca_t  *p_hw_pca,
                                   uint8_t             num)
{
    switch(num) {
    case 0:
        return p_hw_pca->ccap0l & 0xff;

    case 1:
        return p_hw_pca->ccap1l & 0xff;

    case 2:
        return p_hw_pca->ccap2l & 0xff;

    case 3:
        return p_hw_pca->ccap3l & 0xff;

    case 4:
        return p_hw_pca->ccap4l & 0xff;

    default:
        break;
    }

    return 0;
}

/**
 * \brief PCA比较捕获16位寄存器配置
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  data     : 计数值
 * \papca[in]  num      : 模块号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_ccap_set (amhw_hc32_pca_t  *p_hw_pca,
                               uint8_t             num,
                               uint16_t            data)
{
    switch(num) {
    case 0:
        p_hw_pca->ccap0 = data;
        break;

    case 1:
        p_hw_pca->ccap1 = data;
        break;

    case 2:
        p_hw_pca->ccap2 = data;
        break;

    case 3:
        p_hw_pca->ccap3 = data;
        break;

    case 4:
        p_hw_pca->ccap4 = data;
        break;

    default:
        break;
    }
}

/**
 * \brief PCA比较捕获16位寄存器获取
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 *
 * \return 无
 */
am_static_inline
uint16_t amhw_hc32_pca_ccap_get (amhw_hc32_pca_t  *p_hw_pca,
                                   uint8_t             num)
{
    switch(num) {
    case 0:
        return p_hw_pca->ccap0 & 0xffff;

    case 1:
        return p_hw_pca->ccap1 & 0xffff;

    case 2:
        return p_hw_pca->ccap2 & 0xffff;

    case 3:
        return p_hw_pca->ccap3 & 0xffff;

    case 4:
        return p_hw_pca->ccap4 & 0xffff;

    default:
        break;
    }

    return 0;
}

/**
 * \brief PCA比较高速输出使能
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_ccapo_enable (amhw_hc32_pca_t  *p_hw_pca,
                                   uint8_t             num)
{
    p_hw_pca->ccapo |= (1 << num);
}

/**
 * \brief PCA比较高速输出禁能
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 * \papca[in]  num      : 模块号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_ccapo_disable (amhw_hc32_pca_t  *p_hw_pca,
                                    uint8_t             num)
{
    p_hw_pca->ccapo &= ~(1 << num);
}

/**
 * \brief PCA周期寄存器配置
 *
 * \papca[in]  p_hw_pca  : 指向PCA寄存器结构体的指针
 * \papca[in]  data      : 计数值
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_carr_set (amhw_hc32_pca_t *p_hw_pca, uint16_t data)
{
    p_hw_pca->carr = data;
}

/**
 * \brief PCA增强PWM控制使能
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_epwm_enable (amhw_hc32_pca_t  *p_hw_pca)
{
    p_hw_pca->epwm = 1;
}

/**
 * \brief PCA增强PWM控制禁能
 *
 * \papca[in]  p_hw_pca : 指向PCA寄存器结构体的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32_pca_epwm_disable (amhw_hc32_pca_t  *p_hw_pca)
{
    p_hw_pca->epwm = 0;
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
#endif /* __AMHW_HC32_PCA_H */

#endif

/* end of file */
