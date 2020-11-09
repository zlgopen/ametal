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
 * \brief TRNG 底层操作接口
 * \internal
 * \par Modification History
 * - 1.00 19-10-08
 * \endinternal
 */

#ifndef __AMHW_ZSL42x_TRNG_H
#define __AMHW_ZSL42x_TRNG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zsl42x_if_trng
 * \copydoc amhw_zsl42x_trng.h
 * @{
 */
/**
 * \brief TRNG - 寄存器组
 */
typedef struct amhw_zsl42x_trng {
    __IO uint32_t trngcr;            /**< \brief 控制寄存器 */
    __IO uint32_t trngmode;          /**< \brief 模式寄存器 */
         uint32_t res;               /**< \brief 保留 */
    __IO uint32_t trngdat0;          /**< \brief 数据寄存器0 */
    __IO uint32_t trngdat1;          /**< \brief 数据寄存器1 */
} amhw_zsl42x_trng_t;

/**
 * \brief TRNG 启动随机数产生
 *
 * \note :硬件清零
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_trng_run (amhw_zsl42x_trng_t *p_hw_trng)
{
    p_hw_trng->trngcr |= (1 << 1);
}

/**
 * \brief TRNG 随机数产生状态读取
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : 1 正在产生随机数 0 随机数产生完成
 */
am_static_inline
int amhw_zsl42x_trng_stu_check (amhw_zsl42x_trng_t *p_hw_trng)
{
    return ((p_hw_trng->trngcr & (0x2)) >> 1);
}

/**
 * \brief 随机源电路使能
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_trng_cir_enable (amhw_zsl42x_trng_t *p_hw_trng)
{
    p_hw_trng->trngcr |= (1 << 0);
}

/**
 * \brief 随机源电路禁能
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_trng_cir_disable (amhw_zsl42x_trng_t *p_hw_trng)
{
    p_hw_trng->trngcr &= ~(1 << 0);
}

/**
 * \brief RNG 反馈的移位次数
 */
typedef enum amhw_zsl42x_trng_cnt {
    AMHW_ZSL42x_TRNG_CNT_0 = 0,
    AMHW_ZSL42x_TRNG_CNT_8,
    AMHW_ZSL42x_TRNG_CNT_16,
    AMHW_ZSL42x_TRNG_CNT_32,
    AMHW_ZSL42x_TRNG_CNT_64,
    AMHW_ZSL42x_TRNG_CNT_128,
    AMHW_ZSL42x_TRNG_CNT_256,
    AMHW_ZSL42x_TRNG_CNT_RES
}amhw_zsl42x_trng_cnt_t;

/**
 * \brief RNG反馈移位次数选择
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 * \param[in] flag      : 反馈移位次数，参考枚举定义：RNG 反馈的移位次数
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_trng_cnt_sel (amhw_zsl42x_trng_t *p_hw_trng ,uint32_t flag)
{
    p_hw_trng->trngmode = (p_hw_trng->trngmode & (~(0x7 << 2))) | (flag << 2);
}

/**
 * \brief RNG反馈信号是否与随机源进行异或操作设置
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 * \param[in] flag      : 是否设置设置标志 （AM_TRUE是,AM_FALSE否）
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_trng_fdbk_set (amhw_zsl42x_trng_t *p_hw_trng ,am_bool_t flag)
{
    if (flag == 1){
        p_hw_trng->trngmode |= (1 << 1);
    }else {
        p_hw_trng->trngmode &= ~(1 << 1);
    }
}

/**
 * \brief 在产生新的随机数时，64bits RNG是否从随机源获得新的初始值
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 * \param[in] flag      : 是否设置设置标志 （AM_TRUE是,AM_FALSE否）
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_trng_load_set (amhw_zsl42x_trng_t *p_hw_trng, am_bool_t flag)
{
    if (flag == 1){
        p_hw_trng->trngmode |= (1 << 0);
    }else {
        p_hw_trng->trngmode &= ~(1 << 0);
    }
}

/**
 * \brief TRNG 读取低32位随机数
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : 低32为随机数
 */
am_static_inline
uint32_t amhw_zsl42x_trng_low32_data_read (amhw_zsl42x_trng_t *p_hw_trng)
{
    return (p_hw_trng->trngdat0);
}

/**
 * \brief TRNG 读取高32位随机数
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : 高32为随机数
 */
am_static_inline
uint32_t amhw_zsl42x_trng_high32_data_read (amhw_zsl42x_trng_t *p_hw_trng)
{
    return (p_hw_trng->trngdat1);
}

/**
 * @} amhw_if_zsl42x_trng
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZSL42x_TRNG_H */

/* end of file */
