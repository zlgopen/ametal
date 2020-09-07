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
 * - 1.00 20-05-26
 * \endinternal
 */

#ifndef __AMHW_HC32F460_TRNG_H
#define __AMHW_HC32F460_TRNG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_hc32f460_if_trng
 * \copydoc amhw_hc32f460_trng.h
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
 * \brief TRNG 控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t EN                        : 1;  /**< \brief 模拟发振器使能*/
    __IO uint32_t RUN                       : 1;  /**< \brief 随机数运算开始*/
    uint32_t RESERVED2                      :30;  /**< \brief 保留*/
} stc_trng_cr_field_t;

/**
 * \brief TRNG 模式寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t LOAD                      : 1;  /**< \brief 装载控制位*/
    uint32_t RESERVED1                      : 1;  /**< \brief 保留*/
    __IO uint32_t CNT                       : 3;  /**< \brief 移位次数控制位*/
    uint32_t RESERVED5                      :27;  /**< \brief 保留*/
} stc_trng_mr_field_t;

typedef struct amhw_hc32f460_trng {
    union
    {
        __IO uint32_t CR;           /**< \brief TRNG控制寄存器*/
        stc_trng_cr_field_t CR_f;
    };
    union
    {
        __IO uint32_t MR;           /**< \brief TRNG模式寄存器*/
        stc_trng_mr_field_t MR_f;
    };
    uint8_t RESERVED2[4];           /**< \brief 保留*/
    __IO uint32_t DR0;              /**< \brief 数据寄存器0*/
    __IO uint32_t DR1;              /**< \brief 数据寄存器1*/
} amhw_hc32f460_trng_t;

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
void amhw_hc32f460_trng_run (amhw_hc32f460_trng_t *p_hw_trng)
{
    p_hw_trng->CR_f.RUN = 1;
}

/**
 * \brief TRNG 随机数产生状态读取
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : AM_TRUE: 正在产生随机数 / AM_FASLE 随机数产生完成
 */
am_static_inline
am_bool_t amhw_hc32f460_trng_stu_check (amhw_hc32f460_trng_t *p_hw_trng)
{
    return p_hw_trng->CR_f.RUN;
}

/**
 * \brief 随机源电路使能
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_trng_cir_enable (amhw_hc32f460_trng_t *p_hw_trng)
{
    p_hw_trng->CR_f.EN = 1;
}

/**
 * \brief 随机源电路禁能
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_trng_cir_disable (amhw_hc32f460_trng_t *p_hw_trng)
{
	p_hw_trng->CR_f.EN = 0;
}

/**
 * \brief RNG 反馈的移位次数
 */
typedef enum amhw_hc32f460_trng_cnt {
    AMHW_HC32F460_TRNG_CNT_32  = 3u,
    AMHW_HC32F460_TRNG_CNT_64  = 4u,
    AMHW_HC32F460_TRNG_CNT_128 = 5u,
    AMHW_HC32F460_TRNG_CNT_256 = 6u,
}amhw_hc32f460_trng_cnt_t;

/**
 * \brief RNG反馈移位次数选择
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 * \param[in] flag      : 反馈移位次数，参考枚举定义：RNG 反馈的移位次数
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_trng_cnt_sel (amhw_hc32f460_trng_t *p_hw_trng ,uint32_t flag)
{
    p_hw_trng->MR_f.CNT = flag;
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
void amhw_hc32f460_trng_load_set (amhw_hc32f460_trng_t *p_hw_trng,
                                  am_bool_t             flag)
{
    p_hw_trng->MR_f.LOAD = flag;
}

/**
 * \brief TRNG 读取低32位随机数
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : 低32为随机数
 */
am_static_inline
uint32_t amhw_hc32f460_trng_low32_data_read (amhw_hc32f460_trng_t *p_hw_trng)
{
    return (p_hw_trng->DR0);
}

/**
 * \brief TRNG 读取高32位随机数
 *
 * \param[in] p_hw_trng : 指向TRNG寄存器组指针
 *
 * \retval : 高32为随机数
 */
am_static_inline
uint32_t amhw_hc32f460_trng_high32_data_read (amhw_hc32f460_trng_t *p_hw_trng)
{
    return (p_hw_trng->DR1);
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
 * @} amhw_if_hc32f460_trng
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_TRNG_H */

/* end of file */
