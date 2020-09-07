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
 * \brief OPA 底层操作接口
 * \internal
 * \par Modification History
 * - 1.00 19-10-10
 * \endinternal
 */

#ifndef __AMHW_HC32_OPA_H
#define __AMHW_HC32_OPA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_hc32_if_opa
 * \copydoc amhw_hc32_opa.h
 * @{
 */

/**
 * \brief OPA - 寄存器组
 */
typedef struct amhw_hc32_opa {
    __IO uint32_t  opa_cr1;      /**< \brief  OPA通道1控制寄存器 */
    __IO uint32_t  opa_cr2;      /**< \brief  OPA通道2控制寄存器 */
    __IO uint32_t  opa_cr3;      /**< \brief  OPA通道3控制寄存器 */
} amhw_hc32_opa_t;

/**
 * \brief OPA 通道选择掩码
 */
#define  AMHW_HC32_OPA_CH1   (1U)
#define  AMHW_HC32_OPA_CH2   (2U)
#define  AMHW_HC32_OPA_CH3   (3U)

/**
 * \brief OPA工作模式选择掩码
 */
#define AMHW_HC32_OPA_MODE_UNITY_GAIN         (1U)    /**< \biref 单位增益模式 */
#define AMHW_HC32_OPA_MODE_FORWARD_IN         (2U)    /**< \biref 正向输入模式 */
#define AMHW_HC32_OPA_MODE_OPPOSITE           (3U)    /**< \biref 反向输入模式 */
#define AMHW_HC32_OPA_MODE_TWO_DIFF           (4U)    /**< \biref 两差分运放模式 */
#define AMHW_HC32_OPA_MODE_UNIVERSAL          (5U)    /**< \biref 通用模式 */
#define AMHW_HC32_OPA_MODE_CAS_OPPO           (6U)    /**< \biref 级联反向模式 */
#define AMHW_HC32_OPA_MODE_CAS_FORWARD        (7U)    /**< \biref 级联正向模式 */

/**
 * \brief OPA 反向增益选择掩码
 */
#define AMHW_HC32_OPA_GAIN_14    (0U)   /**< \brief 14 Gain */
#define AMHW_HC32_OPA_GAIN_7     (1U)   /**< \brief 7 Gain */
#define AMHW_HC32_OPA_GAIN_13_3  (2U)   /**< \brief 13/3 Gain */
#define AMHW_HC32_OPA_GAIN_3     (3U)   /**< \brief 3 Gain */
#define AMHW_HC32_OPA_GAIN_5_3   (4U)   /**< \brief 5/3 Gain */
#define AMHW_HC32_OPA_GAIN_1     (5U)   /**< \brief 1 Gain */
#define AMHW_HC32_OPA_GAIN_1_3   (6U)   /**< \brief 1/3 Gain */
#define AMHW_HC32_OPA_GAIN_11_5  (7U)   /**< \brief 11/5 Gain */

#define AWHW_HC32_OPA_NO_GAIN    (8u)   /**< \brief 无增益 */
/**
 * \brief OPA 正向增益选择掩码
 */
#define AMHW_HC32_OPA_NONGAIN_16    (0U)   /**< \brief 14 Non-Gain */
#define AMHW_HC32_OPA_NONGAIN_8     (1U)   /**< \brief 8 Non-Gain */
#define AMHW_HC32_OPA_NONGAIN_16_3  (2U)   /**< \brief 16/3 Non-Gain */
#define AMHW_HC32_OPA_NONGAIN_4     (3U)   /**< \brief 4 Non-Gain */
#define AMHW_HC32_OPA_NONGAIN_8_3   (4U)   /**< \brief 8/3 Non-Gain */
#define AMHW_HC32_OPA_NONGAIN_2     (5U)   /**< \brief 2 Non-Gain */
#define AMHW_HC32_OPA_NONGAIN_4_3   (6U)   /**< \brief 4/3 Non-Gain */
#define AMHW_HC32_OPA_NONGAIN_16_5  (7U)   /**< \brief 16/5 Non-Gain */

/**
 * \brief OPA 单位增益PGA模式配置
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] chnel    : OPA通道
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_opa_unity_gain_mode (amhw_hc32_opa_t *p_hw_opa, uint8_t chnel)
{
    switch(chnel){

        case AMHW_HC32_OPA_CH1:
            {
                p_hw_opa->opa_cr1 |=  (0x3 << 10);
                p_hw_opa->opa_cr1 &= ~(0x3 << 8);
                p_hw_opa->opa_cr1 &= ~(0x3 << 16);
                p_hw_opa->opa_cr1 |=  (0x1 << 3);
                p_hw_opa->opa_cr1 &= ~(0x1 << 4);
                p_hw_opa->opa_cr1 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH2:
            {
                p_hw_opa->opa_cr2 |=  (0x3 << 10);
                p_hw_opa->opa_cr2 &= ~(0x3 << 8);
                p_hw_opa->opa_cr2 &= ~(0x3 << 16);
                p_hw_opa->opa_cr2 |=  (0x1 << 3);
                p_hw_opa->opa_cr2 &= ~(0x1 << 4);
                p_hw_opa->opa_cr2 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH3:
            {
                p_hw_opa->opa_cr3 |=  (0x3 << 10);
                p_hw_opa->opa_cr3 &= ~(0x3 << 8);
                p_hw_opa->opa_cr3 &= ~(0x3 << 16);
                p_hw_opa->opa_cr3 |=  (0x1 << 3);
                p_hw_opa->opa_cr3 &= ~(0x1 << 4);
                p_hw_opa->opa_cr3 &= ~(0x1 << 2);
            }
            break;
        default :
            break;
    }

}

/**
 * \brief OPA 正向输入PGA模式配置
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] chnel    : OPA通道
 * \param[in] nongain  : 正向增益
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_opa_forwar_in_mode (amhw_hc32_opa_t *p_hw_opa,
                                   uint8_t          chnel,
                                   uint8_t          nongain)
{
    switch(chnel){

        case AMHW_HC32_OPA_CH1:
            {
                p_hw_opa->opa_cr1 |=  (0x3 << 10);
                p_hw_opa->opa_cr1 |=  (0x1 << 8);
                p_hw_opa->opa_cr1 &= ~(0x3 << 16);
                p_hw_opa->opa_cr1 &= ~(0x1 << 3);
                p_hw_opa->opa_cr1 |=  (0x1 << 4);
                p_hw_opa->opa_cr1 |=  (nongain << 12);
                p_hw_opa->opa_cr1 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH2:
            {
                p_hw_opa->opa_cr2 |=  (0x3 << 10);
                p_hw_opa->opa_cr2 |=  (0x1 << 8);
                p_hw_opa->opa_cr2 &= ~(0x3 << 16);
                p_hw_opa->opa_cr2 &= ~(0x1 << 3);
                p_hw_opa->opa_cr2 |=  (0x1 << 4);
                p_hw_opa->opa_cr2 |=  (nongain << 12);
                p_hw_opa->opa_cr2 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH3:
            {
                p_hw_opa->opa_cr3 |=  (0x3 << 10);
                p_hw_opa->opa_cr3 |=  (0x1 << 8);
                p_hw_opa->opa_cr3 &= ~(0x3 << 16);
                p_hw_opa->opa_cr3 &= ~(0x1 << 3);
                p_hw_opa->opa_cr3 |=  (0x1 << 4);
                p_hw_opa->opa_cr3 |=  (nongain << 12);
                p_hw_opa->opa_cr3 &= ~(0x1 << 2);
            }
            break;
        default :
            break;
    }

}

/**
 * \brief OPA 反向输入PGA模式配置
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] chnel    : OPA通道
 * \param[in] gain     : 反向增益
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_opa_opposite_mode (amhw_hc32_opa_t *p_hw_opa,
                                   uint8_t          chnel,
                                   uint8_t          gain)
{
    switch(chnel){

        case AMHW_HC32_OPA_CH1:
            {
                p_hw_opa->opa_cr1 |=  (0x3 << 10);
                p_hw_opa->opa_cr1 |=  (0x1 << 8);
                p_hw_opa->opa_cr1 |=  (0x2 << 16);
                p_hw_opa->opa_cr1 &= ~(0x1 << 3);
                p_hw_opa->opa_cr1 |=  (0x1 << 4);
                p_hw_opa->opa_cr1 |=  (gain << 12);
                p_hw_opa->opa_cr1 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH2:
            {
                p_hw_opa->opa_cr2 |=  (0x3 << 10);
                p_hw_opa->opa_cr2 |=  (0x1 << 8);
                p_hw_opa->opa_cr2 |=  (0x2 << 16);
                p_hw_opa->opa_cr2 &= ~(0x1 << 3);
                p_hw_opa->opa_cr2 |=  (0x1 << 4);
                p_hw_opa->opa_cr2 |=  (gain << 12);
                p_hw_opa->opa_cr2 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH3:
            {
                p_hw_opa->opa_cr3 |=  (0x3 << 10);
                p_hw_opa->opa_cr3 |=  (0x1 << 8);
                p_hw_opa->opa_cr3 |=  (0x2 << 16);
                p_hw_opa->opa_cr3 &= ~(0x1 << 3);
                p_hw_opa->opa_cr3 |=  (0x1 << 4);
                p_hw_opa->opa_cr3 |=  (gain << 12);
                p_hw_opa->opa_cr3 &= ~(0x1 << 2);
            }
            break;
        default :
            break;
    }

}

/**
 * \brief OPA 两运放差分PGA模式配置
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] chnel    : OPA通道
 * \param[in] gain     : 反向增益
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_opa_diff_mode (amhw_hc32_opa_t *p_hw_opa,
                              uint8_t          chnel,
                              uint8_t          gain)
{
    switch(chnel){

        case AMHW_HC32_OPA_CH1:
            {
                p_hw_opa->opa_cr1 |=  (0x3 << 10);
                p_hw_opa->opa_cr1 &= ~(0x3 << 8);
                p_hw_opa->opa_cr1 &= ~(0x3 << 16);
                p_hw_opa->opa_cr1 |=  (0x1 << 3);
                p_hw_opa->opa_cr1 |=  (0x1 << 4);
                p_hw_opa->opa_cr1 |=  (gain << 12);
                p_hw_opa->opa_cr1 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH2:
            {
                p_hw_opa->opa_cr2 |=  (0x3 << 10);
                p_hw_opa->opa_cr2 |=  (0x1 << 8);
                p_hw_opa->opa_cr2 |=  (0x1 << 16);
                p_hw_opa->opa_cr2 &= ~(0x1 << 3);
                p_hw_opa->opa_cr2 |=  (0x1 << 4);
                p_hw_opa->opa_cr2 &= ~(0x7 << 12);
                p_hw_opa->opa_cr2 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH3:
            {
                p_hw_opa->opa_cr3 |=  (0x3 << 10);
                p_hw_opa->opa_cr3 |=  (0x1 << 8);
                p_hw_opa->opa_cr3 &= ~(0x3 << 16);
                p_hw_opa->opa_cr3 &= ~(0x1 << 3);
                p_hw_opa->opa_cr3 &= ~(0x1 << 4);
                p_hw_opa->opa_cr3 &= ~(0x7 << 12);
                p_hw_opa->opa_cr3 &= ~(0x1 << 2);
            }
            break;
        default :
            break;
    }

}

/**
 * \brief OPA 通用运放模式配置
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] chnel    : OPA通道
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_uinversal_mode (amhw_hc32_opa_t *p_hw_opa,
                              uint8_t          chnel)
{
    switch(chnel){

        case AMHW_HC32_OPA_CH1:
            {
                p_hw_opa->opa_cr1 |=  (0x3 << 10);
                p_hw_opa->opa_cr1 |=  (0x3 << 8);
                p_hw_opa->opa_cr1 &= ~(0x3 << 16);
                p_hw_opa->opa_cr1 &= ~(0x1 << 3);
                p_hw_opa->opa_cr1 &= ~(0x1 << 4);
                p_hw_opa->opa_cr1 |=  (0x5 << 12);
                p_hw_opa->opa_cr1 |= (0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH2:
            {
                p_hw_opa->opa_cr2 |=  (0x3 << 10);
                p_hw_opa->opa_cr2 |=  (0x3 << 8);
                p_hw_opa->opa_cr2 &= ~(0x3 << 16);
                p_hw_opa->opa_cr2 &= ~(0x1 << 3);
                p_hw_opa->opa_cr2 &= ~(0x1 << 4);
                p_hw_opa->opa_cr2 |=  (0x5 << 12);
                p_hw_opa->opa_cr2 |= (0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH3:
            {
                p_hw_opa->opa_cr3 |=  (0x3 << 10);
                p_hw_opa->opa_cr3 |=  (0x3 << 8);
                p_hw_opa->opa_cr3 &= ~(0x3 << 16);
                p_hw_opa->opa_cr3 &= ~(0x1 << 3);
                p_hw_opa->opa_cr3 &= ~(0x1 << 4);
                p_hw_opa->opa_cr3 |=  (0x5 << 12);
                p_hw_opa->opa_cr3 |= (0x1 << 2);
            }
            break;
        default :
            break;
    }
}

/**
 * \brief OPA 级联反向PGA模式配置
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] chnel    : OPA通道
 * \param[in] gain     : 反向增益
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_cas_opposite_mode  (amhw_hc32_opa_t *p_hw_opa,
                                   uint8_t          chnel,
                                   uint8_t          gain)
{
    switch(chnel){

        case AMHW_HC32_OPA_CH1:
            {
                p_hw_opa->opa_cr1 |=  (0x3 << 10);
                p_hw_opa->opa_cr1 |=  (0x1 << 8);
                p_hw_opa->opa_cr1 |=  (0x2 << 16);
                p_hw_opa->opa_cr1 &= ~(0x1 << 3);
                p_hw_opa->opa_cr1 |=  (0x1 << 4);
                p_hw_opa->opa_cr1 |=  (gain << 12);
                p_hw_opa->opa_cr1 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH2:
            {
                p_hw_opa->opa_cr2 |=  (0x3 << 10);
                p_hw_opa->opa_cr2 |=  (0x1 << 8);
                p_hw_opa->opa_cr2 |=  (0x1 << 16);
                p_hw_opa->opa_cr2 &= ~(0x1 << 3);
                p_hw_opa->opa_cr2 |=  (0x1 << 4);
                p_hw_opa->opa_cr2 |=  (gain << 12);
                p_hw_opa->opa_cr2 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH3:
            {
                p_hw_opa->opa_cr3 |=  (0x3 << 10);
                p_hw_opa->opa_cr3 |=  (0x1 << 8);
                p_hw_opa->opa_cr3 |=  (0x1 << 16);
                p_hw_opa->opa_cr3 &= ~(0x1 << 3);
                p_hw_opa->opa_cr3 |=  (0x1 << 4);
                p_hw_opa->opa_cr3 |=  (gain << 12);
                p_hw_opa->opa_cr3 &= ~(0x1 << 2);
            }
            break;
        default :
            break;
    }
}

/**
 * \brief OPA 级联正向PGA模式配置
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] chnel    : OPA通道
 * \param[in] nongain  : 正向增益
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_cas_forward_mode  (amhw_hc32_opa_t *p_hw_opa,
                                   uint8_t          chnel,
                                   uint8_t          nongain)
{
    switch(chnel){

        case AMHW_HC32_OPA_CH1:
            {
                p_hw_opa->opa_cr1 |=  (0x3 << 10);
                p_hw_opa->opa_cr1 |=  (0x1 << 8);
                p_hw_opa->opa_cr1 &= ~(0x3 << 16);
                p_hw_opa->opa_cr1 &= ~(0x1 << 3);
                p_hw_opa->opa_cr1 |=  (0x1 << 4);
                p_hw_opa->opa_cr1 |=  (nongain << 12);
                p_hw_opa->opa_cr1 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH2:
            {
                p_hw_opa->opa_cr2 |=  (0x2 << 10);
                p_hw_opa->opa_cr2 |=  (0x1 << 8);
                p_hw_opa->opa_cr2 &= ~(0x3 << 16);
                p_hw_opa->opa_cr2 &= ~(0x1 << 3);
                p_hw_opa->opa_cr2 |=  (0x1 << 4);
                p_hw_opa->opa_cr2 |=  (nongain << 12);
                p_hw_opa->opa_cr2 &= ~(0x1 << 2);
            }
            break;
        case AMHW_HC32_OPA_CH3:
            {
                p_hw_opa->opa_cr3 |=  (0x2 << 10);
                p_hw_opa->opa_cr3 |=  (0x1 << 8);
                p_hw_opa->opa_cr3 &= ~(0x3 << 16);
                p_hw_opa->opa_cr3 &= ~(0x1 << 3);
                p_hw_opa->opa_cr3 |=  (0x1 << 4);
                p_hw_opa->opa_cr3 |=  (nongain << 12);
                p_hw_opa->opa_cr3 &= ~(0x1 << 2);
            }
            break;
        default :
            break;
    }

}

/**
 * \brief OPA IO端口与内部连接控制宏
 */
#define AWHW_HC32_OPA_PO_EN   (1U)     /* 使能IO端口与内部连接 */
#define AWHW_HC32_OPA_PO_DIS  (0U)     /* 禁能IO端口与内部连接 */

/**
 * \brief OPA IO端口与内部连接控制
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] chnel    : OPA通道
 * \param[in] opt      : 使能或者禁能，参考上面宏定义
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_opa_po_ctrl(amhw_hc32_opa_t *p_hw_opa, uint8_t chnel, uint8_t opt)
{
    switch (chnel){

        case AMHW_HC32_OPA_CH1:
            if (opt){
                p_hw_opa->opa_cr1 |= (0x1 << 15);
            }else{
                p_hw_opa->opa_cr1 &= ~(0x1 << 15);
            }
            break;
        case AMHW_HC32_OPA_CH2:
            if (opt){
                p_hw_opa->opa_cr2 |= (0x1 << 15);
            }else{
                p_hw_opa->opa_cr2 &= ~(0x1 << 15);
            }
            break;
        case AMHW_HC32_OPA_CH3:
            if (opt){
                p_hw_opa->opa_cr3 |= (0x1 << 15);
            }else{
                p_hw_opa->opa_cr3 &= ~(0x1 << 15);
            }
            break;
        default:
            break;
    }
}

/**
 * \brief OPA 使能
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_opa_en (amhw_hc32_opa_t *p_hw_opa)
{
    p_hw_opa->opa_cr1 |= (0x1 << 0);
    p_hw_opa->opa_cr2 |= (0x1 << 0);
    p_hw_opa->opa_cr3 |= (0x1 << 0);
}

/**
 * \brief OPA 禁能
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_opa_dis (amhw_hc32_opa_t *p_hw_opa)
{
    p_hw_opa->opa_cr1 &= ~(0x1 << 0);
    p_hw_opa->opa_cr2 &= ~(0x1 << 0);
    p_hw_opa->opa_cr3 &= ~(0x1 << 0);
}

/**
 * \brief OPA 偏置电流选择
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32_bias_set (amhw_hc32_opa_t *p_hw_opa)
{
    p_hw_opa->opa_cr1 &= ~(0x7 << 5);
    p_hw_opa->opa_cr2 &= ~(0x7 << 5);
    p_hw_opa->opa_cr3 &= ~(0x7 << 5);
}

/**
 * @} amhw_if_hc32_opa
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32_OPA_H */

/* end of file */
