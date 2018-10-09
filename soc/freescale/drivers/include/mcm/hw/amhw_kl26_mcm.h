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
 * \brief MCM模块的硬件层接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-14  mkr, first implementation.
 * \endinternal
 */

#ifndef __KL26_MCM_H
#define __KL26_MCM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_kl26_if_mcm
 * \copydoc amhw_kl26_mcm.h
 * @{
 */

#define KL26_MCM_ESFC       (1UL << 16)  /**< \brief FLASH忙延时标志位      */

#define KL26_MCM_CFCC       (1UL << 10)  /**< \brief FLASH高速缓存清除位    */

#define KL26_MCM_ARB        (1UL << 9 )  /**< \brief 仲裁方式选择位         */

#define KL26_MCM_CPOWOI     (1UL << 2 )  /**< \brief 中断唤醒计算操作使能位 */

#define KL26_MCM_CPOACK     (1UL << 1 )  /**< \brief 计算操作应答标志位     */

#define KL26_MCM_CPOREQ     (1UL << 0 )  /**< \brief 计算操作请求标志位     */

/**
 *  \name 仲裁模式
 *  @{
 */
#define AMHW_KL26_FIX_PRIO            (0)          /**< \brief 固定优先权 */
#define AMHW_KL26_ROUND_ROBIN         (1)          /**< \brief 循环模式   */
/**
 *  @}
 */

/**
 *  \name 预测缓存模式
 *  @{
 */
#define AMHW_KL26_SEECULATE_CODE      (0)         /**< \brief 开启指令预测缓存             */
#define AMHW_KL26_SPECULATE_CODE_DATA (1)         /**< \brief 开启指令和数据预测缓存 */
#define AMHW_KL26_SPECULATE_NONE      (2)         /**< \brief 无预测缓存                          */
/**
 *  @}
 */

/**
 *  \brief MCM 寄存器结构体定义
 */
typedef struct amhw_kl26_mcm {
    __I  uint32_t reserve0[2];      /**< \brief 保留                        */
    __I  uint16_t plasc;            /**< \brief 交叉开关从机配置寄存器 (RO) */
    __I  uint16_t plamc;            /**< \brief 交叉开关主机配置寄存器 (RO) */
    __IO uint32_t placr;            /**< \brief 平台控制寄存器              */
    __I  uint32_t reserve1[12];     /**< \brief 保留                        */
    __IO uint32_t cpo;              /**< \brief 计算操作寄存器              */
} amhw_kl26_mcm_t;

/**
 *  \brief 获取从机axbs的配置情况
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *  \return 无
 */
am_static_inline
uint16_t amhw_kl26_mcm_axbs_slave_config_get (amhw_kl26_mcm_t *p_hw_mcm)
{
    return p_hw_mcm->plasc & 0xfful;
}

/**
 *  \brief 获取主机axbs的配置情况
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *  \return 无
 */
am_static_inline
uint16_t amhw_kl26_mcm_axbs_master_config_get (amhw_kl26_mcm_t *p_hw_mcm)
{
    return p_hw_mcm->plamc & 0xfful;
}

/**
 *  \brief 使能flash忙时延时
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *  \return 无
 */
am_static_inline
void amhw_kl26_mcm_flash_stalling_enable (amhw_kl26_mcm_t *p_hw_mcm)
{
    p_hw_mcm->placr |= KL26_MCM_ESFC;
}

/**
 *  \brief 关闭flash忙时延时
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *  \return 无
 */
am_static_inline
void amhw_kl26_mcm_flash_stalling_disable (amhw_kl26_mcm_t *p_hw_mcm)
{
    p_hw_mcm->placr &= ~KL26_MCM_ESFC;
}

/**
 *  \brief 配置FLASH预测缓存
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *  \param config 配置模式
 *      - SEECULATE_CODE:      开启指令预测缓存但关闭数据预测缓存
 *      - SPECULATE_CODE_DATA: 开启指令和数据预测缓存
 *      - SPECULATE_NONE:      关闭预测缓存
 *  \return 无
 */
am_static_inline
void amhw_kl26_mcm_speculation_buffer_config (amhw_kl26_mcm_t *p_hw_mcm,
                                              uint32_t         config)
{
    p_hw_mcm->placr = (p_hw_mcm->placr & (~(0x3 << 14))) | (config << 14);
}

/**
 *  \brief 配置FLASH高速缓存
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *  \param config 配置模式
 *      - 0: 缓存指令和数据
 *      - 1: 缓存指令、不缓存数据
 *      - 2: 不缓存指令、缓存数据
 *      - 3: 指令和数据都不缓存
 *      - 4: 关闭缓存
 *  \return 无
 */
am_static_inline
void amhw_kl26_mcm_cache_config (amhw_kl26_mcm_t *p_hw_mcm, uint32_t config)
{
    p_hw_mcm->placr = ((p_hw_mcm->placr & (~(0x7 << 11)))) | (config << 11);
}

/**
 *  \brief 清除高速缓存(cache)
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *  \return 无
 */
am_static_inline
void amhw_mcm_cache_clear (amhw_kl26_mcm_t *p_hw_mcm)
{
    p_hw_mcm->placr |= KL26_MCM_CFCC;
}

/**
 *  \brief 选择交叉开关主机仲裁方式
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *  \param sel 仲裁方式
 *          - AMHW_KL26_FIX_PRIO 固定优先级
 *          - AMHW_KL26_ROUND_ROBIN 循环模式
 *  \return 无
 */
am_static_inline
void amhw_kl26_mcm_arbiration_select (amhw_kl26_mcm_t *p_hw_mcm, uint8_t sel)
{
    p_hw_mcm->placr = (p_hw_mcm->placr & (~KL26_MCM_ARB)) | (sel << 9);
}

/**
 *  \brief 中断唤醒计算操作使能
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *  \return 无
 */
am_static_inline
void amhw_kl26_mcm_compute_operation_enable (amhw_kl26_mcm_t *p_hw_mcm)
{
    p_hw_mcm->cpo |= KL26_MCM_CPOWOI;
}

/**
 *  \brief 中断唤醒计算操作除能
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *  \return 无
 */
am_static_inline
void amhw_kl26_mcm_compute_operation_disable (amhw_kl26_mcm_t *p_hw_mcm)
{
    p_hw_mcm->cpo &= ~KL26_MCM_CPOWOI;
}

/**
 *  \brief 计算操作状态获取
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *
 *  \retval 0: 计算操作未进入或以结束
 *  \retval AMHW_MCM_CPOACK: 已进入计算操作或操作未结束
 */
am_static_inline
uint8_t amhw_kl26_mcm_compute_status_get (amhw_kl26_mcm_t *p_hw_mcm)
{
    return p_hw_mcm->cpo & KL26_MCM_CPOACK;
}

/**
 *  \brief 计算操作状态请求状态获取
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *
 *  \retval 0: 请求被清除
 *  \retval AMHW_MCM_CPOREQ: 请求计算操作
 */
am_static_inline
uint8_t amhw_kl26_mcm_compute_request_get (amhw_kl26_mcm_t *p_hw_mcm)
{
    return (p_hw_mcm->cpo & KL26_MCM_CPOREQ);
}

/**
 *  \brief 请求计算操作
 *
 *  \param p_hw_mcm 指向MCM模块寄存器的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_kl26_mcm_compute_request (amhw_kl26_mcm_t *p_hw_mcm)
{
    p_hw_mcm->cpo |= KL26_MCM_CPOREQ;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __KL26_MCM_H */
