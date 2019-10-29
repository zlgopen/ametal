/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg118.cn/
*******************************************************************************/

/**
 * \file
 * \brief VC 底层操作接口
 * \internal
 * \par Modification History
 * - 1.00 19-09-26
 * \endinternal
 */

#ifndef __AMHW_ZLG118_VC_H
#define __AMHW_ZLG118_VC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zlg118_if_vc
 * \copydoc amhw_zlg118_vc.h
 * @{
 */

/**
 * \brief VC - 寄存器组
 */
typedef struct amhw_zlg118_vc {
    __IO uint32_t  vccr;        /**< \brief  VC 配置寄存器 */
    __IO uint32_t  vc0cr;       /**< \brief  VC0 配置寄存器 */
    __IO uint32_t  vc1cr;       /**< \brief  VC1 配置寄存器 */
    __IO uint32_t  vc0outcfg;   /**< \brief  VC0 输出配置寄存器 */
    __IO uint32_t  vc1outcfg;   /**< \brief  VC1 输出配置寄存器 */
    __IO uint32_t  vcifr;       /**< \brief  VC 中断寄存器 */
         uint32_t  res2[74];    /**< \brief  保留 */
    __IO uint32_t  vc2cr;       /**< \brief  VC2 配置寄存器 */
    __IO uint32_t  vc2outcfg;   /**< \brief  VC2 输出配置寄存器 */
} amhw_zlg118_vc_t;

/*******************************************************************************
 *  VC 配置寄存器
*******************************************************************************/
/*
 * \brief VC0-2 迟滞电压选择掩码
 */
#define AMHW_ZLG118_VC2_HYS_10_MV    (1U << 18)
#define AMHW_ZLG118_VC2_HYS_20_MV    (2U << 18)
#define AMHW_ZLG118_VC2_HYS_30_MV    (3U << 18)
#define AMHW_ZLG118_VC1_HYS_10_MV    (1U << 14)
#define AMHW_ZLG118_VC1_HYS_20_MV    (2U << 14)
#define AMHW_ZLG118_VC1_HYS_30_MV    (3U << 14)
#define AMHW_ZLG118_VC0_HYS_10_MV    (1U << 10)
#define AMHW_ZLG118_VC0_HYS_20_MV    (2U << 10)
#define AMHW_ZLG118_VC0_HYS_30_MV    (3U << 10)

/**
 * \brief VC0-2迟滞电压选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag    : 参考宏定义：VC0-2 迟滞电压选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc2_hys_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vccr = (p_hw_vc->vccr & (~(0x3 << 18))) | flag;
}

am_static_inline
void amhw_zlg118_vc1_hys_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vccr = (p_hw_vc->vccr & (~(0x3 << 14))) | flag;
}

am_static_inline
void amhw_zlg118_vc0_hys_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vccr = (p_hw_vc->vccr & (~(0x3 << 10))) | flag;
}

/*
 * \brief VC0-2 功耗选择掩码
 */
#define AMHW_ZLG118_VC2_BIAS_300_NA    (0U << 16)
#define AMHW_ZLG118_VC2_BIAS_1_2_UA    (1U << 16)
#define AMHW_ZLG118_VC2_BIAS_10_UA     (2U << 16)
#define AMHW_ZLG118_VC2_BIAS_20_UA     (3U << 16)
#define AMHW_ZLG118_VC1_BIAS_300_NA    (0U << 12)
#define AMHW_ZLG118_VC1_BIAS_1_2_UA    (1U << 12)
#define AMHW_ZLG118_VC1_BIAS_10_UA     (2U << 12)
#define AMHW_ZLG118_VC1_BIAS_20_UA     (3U << 12)
#define AMHW_ZLG118_VC0_BIAS_300_NA    (0U << 8)
#define AMHW_ZLG118_VC0_BIAS_1_2_UA    (1U << 8)
#define AMHW_ZLG118_VC0_BIAS_10_UA     (2U << 8)
#define AMHW_ZLG118_VC0_BIAS_20_UA     (3U << 8)

/**
 * \brief VC0-2 功耗选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag    : 参考宏定义：VC0-2 功耗选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc2_bias_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vccr = (p_hw_vc->vccr & (~(0x3 << 16))) | flag;
}

am_static_inline
void amhw_zlg118_vc1_bias_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vccr = (p_hw_vc->vccr & (~(0x3 << 12))) | flag;
}

am_static_inline
void amhw_zlg118_vc0_bias_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vccr = (p_hw_vc->vccr & (~(0x3 << 8))) | flag;
}

/*
 * \brief VC_DIV 参考电压Vref选择掩码
 */
#define AMHW_ZLG118_VC_REF2P5_VCC           (0U)
#define AMHW_ZLG118_VC_REF2P5_ADC_CR0_VREF  (1U)

/**
 * \brief VC_DIV 参考电压Vref选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag    : 参考宏定义：VC_DIV 参考电压Vref选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_ref2p5_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vccr = (p_hw_vc->vccr & (~(1 << 7))) | (flag << 7);
}

/**
 * \brief VC 参考电压分频使能
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_div_enable (amhw_zlg118_vc_t *p_hw_vc)
{
    p_hw_vc->vccr |= (1 << 6);
}

/**
 * \brief VC 参考电压分频禁能
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_div_disable (amhw_zlg118_vc_t *p_hw_vc)
{
    p_hw_vc->vccr &= ~(1 << 6);
}

/*
 * \brief VC 参考电压分频系数
 */
typedef enum amhw_zlg118_vc_div {
    AMHW_ZLG118_VC_DIV_1 = 0,
    AMHW_ZLG118_VC_DIV_2,
    AMHW_ZLG118_VC_DIV_3,
    AMHW_ZLG118_VC_DIV_4,
    AMHW_ZLG118_VC_DIV_5,
    AMHW_ZLG118_VC_DIV_6,
    AMHW_ZLG118_VC_DIV_7,
    AMHW_ZLG118_VC_DIV_8,
    AMHW_ZLG118_VC_DIV_9,
    AMHW_ZLG118_VC_DIV_10,
    AMHW_ZLG118_VC_DIV_11,
    AMHW_ZLG118_VC_DIV_12,
    AMHW_ZLG118_VC_DIV_13,
    AMHW_ZLG118_VC_DIV_14,
    AMHW_ZLG118_VC_DIV_15,
    AMHW_ZLG118_VC_DIV_16,
    AMHW_ZLG118_VC_DIV_17,
    AMHW_ZLG118_VC_DIV_18,
    AMHW_ZLG118_VC_DIV_19,
    AMHW_ZLG118_VC_DIV_20,
    AMHW_ZLG118_VC_DIV_21,
    AMHW_ZLG118_VC_DIV_22,
    AMHW_ZLG118_VC_DIV_23,
    AMHW_ZLG118_VC_DIV_24,
    AMHW_ZLG118_VC_DIV_25,
    AMHW_ZLG118_VC_DIV_26,
    AMHW_ZLG118_VC_DIV_27,
    AMHW_ZLG118_VC_DIV_28,
    AMHW_ZLG118_VC_DIV_29,
    AMHW_ZLG118_VC_DIV_30,
    AMHW_ZLG118_VC_DIV_31,
    AMHW_ZLG118_VC_DIV_32,
    AMHW_ZLG118_VC_DIV_33,
    AMHW_ZLG118_VC_DIV_34,
    AMHW_ZLG118_VC_DIV_35,
    AMHW_ZLG118_VC_DIV_36,
    AMHW_ZLG118_VC_DIV_37,
    AMHW_ZLG118_VC_DIV_38,
    AMHW_ZLG118_VC_DIV_39,
    AMHW_ZLG118_VC_DIV_40,
    AMHW_ZLG118_VC_DIV_41,
    AMHW_ZLG118_VC_DIV_42,
    AMHW_ZLG118_VC_DIV_43,
    AMHW_ZLG118_VC_DIV_44,
    AMHW_ZLG118_VC_DIV_45,
    AMHW_ZLG118_VC_DIV_46,
    AMHW_ZLG118_VC_DIV_47,
    AMHW_ZLG118_VC_DIV_48,
    AMHW_ZLG118_VC_DIV_49,
    AMHW_ZLG118_VC_DIV_50,
    AMHW_ZLG118_VC_DIV_51,
    AMHW_ZLG118_VC_DIV_52,
    AMHW_ZLG118_VC_DIV_53,
    AMHW_ZLG118_VC_DIV_54,
    AMHW_ZLG118_VC_DIV_55,
    AMHW_ZLG118_VC_DIV_56,
    AMHW_ZLG118_VC_DIV_57,
    AMHW_ZLG118_VC_DIV_58,
    AMHW_ZLG118_VC_DIV_59,
    AMHW_ZLG118_VC_DIV_60,
    AMHW_ZLG118_VC_DIV_61,
    AMHW_ZLG118_VC_DIV_62,
    AMHW_ZLG118_VC_DIV_63,
    AMHW_ZLG118_VC_DIV_64,
}amhw_zlg118_vc_div_t;

/**
 * \brief VC 参考电压分频系数选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag    : 参考枚举：amhw_zlg118_vc_div_t
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_div_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vccr = (p_hw_vc->vccr & (~(0x3f << 0))) | flag;
}

/*******************************************************************************
 *  VC0-2 CR寄存器配置
*******************************************************************************/
/**
 * \brief 模拟比较器通道选择掩码
 *
 */
#define AMHW_ZLG118_VC0   (1U)
#define AMHW_ZLG118_VC1   (2U)
#define AMHW_ZLG118_VC2   (4U)

/**
 * \brief VC0-2 电压比较器使能
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] vc_flag : 表示需要使用的模拟比较器，参考： 模拟比较器通道选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_volt_cmp_enable (amhw_zlg118_vc_t *p_hw_vc,
                                     uint32_t         vc_flag)
{
    switch (vc_flag){
        case 1 : p_hw_vc->vc0cr |= (1 << 16);
            break;
        case 2 : p_hw_vc->vc1cr |= (1 << 16);
            break;
        case 3 : p_hw_vc->vc0cr |= (1 << 16);
                 p_hw_vc->vc1cr |= (1 << 16);
            break;
        case 4 : p_hw_vc->vc2cr |= (1 << 16);
            break;
        case 5 : p_hw_vc->vc0cr |= (1 << 16);
                 p_hw_vc->vc2cr |= (1 << 16);
            break;
        case 6 : p_hw_vc->vc1cr |= (1 << 16);
                 p_hw_vc->vc2cr |= (1 << 16);
            break;
        case 7 : p_hw_vc->vc0cr |= (1 << 16);
                 p_hw_vc->vc1cr |= (1 << 16);
                 p_hw_vc->vc2cr |= (1 << 16);
            break;
        default:
            return ;
    }
}

/**
 * \brief VC0 电压比较器禁能
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] vc_flag : 表示需要禁能的模拟比较器，参考： 模拟比较器通道选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_volt_cmp_disable (amhw_zlg118_vc_t *p_hw_vc,
                                      uint32_t          vc_flag)
{
    switch (vc_flag){
        case 1 : p_hw_vc->vc0cr &= ~(1 << 16);
            break;
        case 2 : p_hw_vc->vc1cr &= ~(1 << 16);
            break;
        case 3 : p_hw_vc->vc0cr &= ~(1 << 16);
                 p_hw_vc->vc1cr &= ~(1 << 16);
            break;
        case 4 : p_hw_vc->vc2cr &= ~(1 << 16);
            break;
        case 5 : p_hw_vc->vc0cr &= ~(1 << 16);
                 p_hw_vc->vc2cr &= ~(1 << 16);
            break;
        case 6 : p_hw_vc->vc1cr &= ~(1 << 16);
                 p_hw_vc->vc2cr &= ~(1 << 16);
            break;
        case 7 : p_hw_vc->vc0cr &= ~(1 << 16);
                 p_hw_vc->vc1cr &= ~(1 << 16);
                 p_hw_vc->vc2cr &= ~(1 << 16);
            break;
        default:
            return ;
    }
}

/**
 * \brief VC0-2 中断使能
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] vc_flag : 表示需要使用的模拟比较器，参考： 模拟比较器通道选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_int_enable (amhw_zlg118_vc_t *p_hw_vc,
                                uint32_t          vc_flag)
{
    switch (vc_flag){
        case 1 : p_hw_vc->vc0cr |= (1 << 15);
            break;
        case 2 : p_hw_vc->vc1cr |= (1 << 15);
            break;
        case 3 : p_hw_vc->vc0cr |= (1 << 15);
                 p_hw_vc->vc1cr |= (1 << 15);
            break;
        case 4 : p_hw_vc->vc2cr |= (1 << 15);
            break;
        case 5 : p_hw_vc->vc0cr |= (1 << 15);
                 p_hw_vc->vc2cr |= (1 << 15);
            break;
        case 6 : p_hw_vc->vc1cr |= (1 << 15);
                 p_hw_vc->vc2cr |= (1 << 15);
            break;
        case 7 : p_hw_vc->vc0cr |= (1 << 15);
                 p_hw_vc->vc1cr |= (1 << 15);
                 p_hw_vc->vc2cr |= (1 << 15);
            break;
        default:
            return ;
    }
}

/**
 * \brief VC0-2 中断禁能
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] vc_flag : 表示需要禁能的模拟比较器，参考：模拟比较器通道选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_int_disable (amhw_zlg118_vc_t *p_hw_vc,
                                 uint32_t          vc_flag)
{
    switch (vc_flag){
        case 1 : p_hw_vc->vc0cr &= ~(1 << 15);
            break;
        case 2 : p_hw_vc->vc1cr &= ~(1 << 15);
            break;
        case 3 : p_hw_vc->vc0cr &= ~(1 << 15);
                 p_hw_vc->vc1cr &= ~(1 << 15);
            break;
        case 4 : p_hw_vc->vc2cr &= ~(1 << 15);
            break;
        case 5 : p_hw_vc->vc0cr &= ~(1 << 15);
                 p_hw_vc->vc2cr &= ~(1 << 15);
            break;
        case 6 : p_hw_vc->vc1cr &= ~(1 << 15);
                 p_hw_vc->vc2cr &= ~(1 << 15);
            break;
        case 7 : p_hw_vc->vc0cr &= ~(1 << 15);
                 p_hw_vc->vc1cr &= ~(1 << 15);
                 p_hw_vc->vc2cr &= ~(1 << 15);
            break;
        default:
            return ;
    }
}

/**
 * \brief VC0-2 输出信号触发中断选择掩码
 */
#define AMHW_ZLG118_VC_OUT_TRI_INT_HIGH_LEVEL (4U << 12)
#define AMHW_ZLG118_VC_OUT_TRI_INT_RISING     (2U << 12)
#define AMHW_ZLG118_VC_OUT_TRI_INT_FALLING    (1U << 12)

/**
 * \brief VC0-2 输出信号触发中断选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : 参考宏定义：VC0-2 输出信号触发中断选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc0_out_tri_int_sel (amhw_zlg118_vc_t *p_hw_vc,
                                      uint32_t          flag)
{
    p_hw_vc->vc0cr |= flag;
}

am_static_inline
void amhw_zlg118_vc1_out_tri_int_sel (amhw_zlg118_vc_t *p_hw_vc,
                                      uint32_t          flag)
{
   p_hw_vc->vc1cr |= flag;
}

am_static_inline
void amhw_zlg118_vc2_out_tri_int_sel (amhw_zlg118_vc_t *p_hw_vc,
                                      uint32_t          flag)
{
   p_hw_vc->vc2cr |= flag;
}

/**
 * \brief VC 滤波时间
 */
typedef enum amhw_zlg118_vc_deb_time {
    AMHW_ZLG118_DEB_TIME_7_US = 0,
    AMHW_ZLG118_DEB_TIME_14_US,
    AMHW_ZLG118_DEB_TIME_28_US,
    AMHW_ZLG118_DEB_TIME_112_US,
    AMHW_ZLG118_DEB_TIME_450_US,
    AMHW_ZLG118_DEB_TIME_1_8_MS,
    AMHW_ZLG118_DEB_TIME_7_2_MS,
    AMHW_ZLG118_DEB_TIME_28_2_MS,
    AMHW_ZLG118_DEB_TIME_NO,
}amhw_zlg118_vc_deb_time_t;

/**
 * \brief VC0-2 输出滤波时间配置
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : 参考枚举：amhw_zlg118_vc_deb_time_t
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc0_flt_time_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    if (flag != AMHW_ZLG118_DEB_TIME_NO){
        p_hw_vc->vc0cr = (p_hw_vc->vc0cr & (~(0x7 << 9))) | (flag << 9);
    }
}

am_static_inline
void amhw_zlg118_vc1_flt_time_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    if (flag != AMHW_ZLG118_DEB_TIME_NO){
        p_hw_vc->vc1cr = (p_hw_vc->vc1cr & (~(0x7 << 9))) | (flag << 9);
    }
}

am_static_inline
void amhw_zlg118_vc2_flt_time_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    if (flag != AMHW_ZLG118_DEB_TIME_NO){
        p_hw_vc->vc2cr = (p_hw_vc->vc2cr & (~(0x7 << 9))) | (flag << 9);
    }
}

/**
 * \brief VC0-2 滤波使能
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] vc_flag : 表示需要使用的模拟比较器，参考： 模拟比较器通道选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_flt_enable (amhw_zlg118_vc_t *p_hw_vc, uint32_t vc_flag)
{
    switch (vc_flag){
        case 1 : p_hw_vc->vc0cr |= (1 << 8);
            break;
        case 2 : p_hw_vc->vc1cr |= (1 << 8);
            break;
        case 3 : p_hw_vc->vc0cr |= (1 << 8);
                 p_hw_vc->vc1cr |= (1 << 8);
            break;
        case 4 : p_hw_vc->vc2cr |= (1 << 8);
            break;
        case 5 : p_hw_vc->vc0cr |= (1 << 8);
                 p_hw_vc->vc2cr |= (1 << 8);
            break;
        case 6 : p_hw_vc->vc1cr |= (1 << 8);
                 p_hw_vc->vc2cr |= (1 << 8);
            break;
        case 7 : p_hw_vc->vc0cr |= (1 << 8);
                 p_hw_vc->vc1cr |= (1 << 8);
                 p_hw_vc->vc2cr |= (1 << 8);
            break;
        default:
            return ;
    }
}

/**
 * \brief VC0-2 滤波禁能
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] vc_flag : 表示需要禁能的模拟比较器，参考：模拟比较器通道选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_flt_disable (amhw_zlg118_vc_t *p_hw_vc, uint32_t vc_flag)
{
    switch (vc_flag){
        case 1 : p_hw_vc->vc0cr &= ~(1 << 8);
            break;
        case 2 : p_hw_vc->vc1cr &= ~(1 << 8);
            break;
        case 3 : p_hw_vc->vc0cr &= ~(1 << 8);
                 p_hw_vc->vc1cr &= ~(1 << 8);
            break;
        case 4 : p_hw_vc->vc2cr &= ~(1 << 8);
            break;
        case 5 : p_hw_vc->vc0cr &= ~(1 << 8);
                 p_hw_vc->vc2cr &= ~(1 << 8);
            break;
        case 6 : p_hw_vc->vc1cr &= ~(1 << 8);
                 p_hw_vc->vc2cr &= ~(1 << 8);
            break;
        case 7 : p_hw_vc->vc0cr &= ~(1 << 8);
                 p_hw_vc->vc1cr &= ~(1 << 8);
                 p_hw_vc->vc2cr &= ~(1 << 8);
            break;
        default:
            return ;
    }
}

/**
 * \brief VC0 电压比较器 “-”端输入选择
 */
typedef enum amhw_zlg118_vc0_n_sel {
    AMHW_ZLG118_VC0_N_INPUT_PA0 = 0,
    AMHW_ZLG118_VC0_N_INPUT_PA1,
    AMHW_ZLG118_VC0_N_INPUT_PA2,
    AMHW_ZLG118_VC0_N_INPUT_PA3,
    AMHW_ZLG118_VC0_N_INPUT_PA4,
    AMHW_ZLG118_VC0_N_INPUT_PA5,
    AMHW_ZLG118_VC0_N_INPUT_PA6,
    AMHW_ZLG118_VC0_N_INPUT_PA7,
    AMHW_ZLG118_VC0_N_INPUT_PC4,
    AMHW_ZLG118_VC0_N_INPUT_PC5,
    AMHW_ZLG118_VC0_N_INPUT_DAC0,
    AMHW_ZLG118_VC0_N_INPUT_RES_DIV_OUT_VOL,
    AMHW_ZLG118_VC0_N_INPUT_INTER_TEMP_SENSOR_OUT_VOL,
    AMHW_ZLG118_VC0_N_INPUT_INTER_BASE_1_2_OUT_VOL,
    AMHW_ZLG118_VC0_N_INPUT_ADC_REF,
    AMHW_ZLG118_VC0_N_INPUT_LDO_OUT_VOL,
}amhw_zlg118_vc0_n_sel_t;

/**
 * \brief VC0 “-”端输入选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : “-”端输入选择，参考枚举定义： VC0 电压比较器 “-”端输入选择
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc0_n_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vc0cr = (p_hw_vc->vc0cr & (~(0xf << 4))) | (flag << 4);
}

/**
 * \brief VC1 电压比较器 “-”端输入选择
 */
typedef enum amhw_zlg118_vc1_n_sel {
    AMHW_ZLG118_VC1_N_INPUT_PC0 = 0,
    AMHW_ZLG118_VC1_N_INPUT_PC1,
    AMHW_ZLG118_VC1_N_INPUT_PC2,
    AMHW_ZLG118_VC1_N_INPUT_PC3,
    AMHW_ZLG118_VC1_N_INPUT_PA0,
    AMHW_ZLG118_VC1_N_INPUT_PA1,
    AMHW_ZLG118_VC1_N_INPUT_PB0,
    AMHW_ZLG118_VC1_N_INPUT_PB1,
    AMHW_ZLG118_VC1_N_INPUT_PB2,
    AMHW_ZLG118_VC1_N_INPUT_PB3,
    AMHW_ZLG118_VC1_N_INPUT_DAC1,
    AMHW_ZLG118_VC1_N_INPUT_RES_DIV_OUT_VOL,
    AMHW_ZLG118_VC1_N_INPUT_INTER_TEMP_SENSOR_OUT_VOL,
    AMHW_ZLG118_VC1_N_INPUT_INTER_BASE_1_2_OUT_VOL,
    AMHW_ZLG118_VC1_N_INPUT_ADC_REF,
    AMHW_ZLG118_VC1_N_INPUT_LDO_OUT_VOL,
}amhw_zlg118_vc1_n_sel_t;

/**
 * \brief VC1 “-”端输入选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : “-”端输入选择，参考枚举定义： VC1 电压比较器 “-”端输入选择
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc1_n_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vc1cr = (p_hw_vc->vc1cr & (~(0xf << 4))) | (flag << 4);
}

/**
 * \brief VC2 电压比较器 “-”端输入选择
 */
typedef enum amhw_zlg118_vc2_n_sel {
    AMHW_ZLG118_VC2_N_INPUT_PA5 = 0,
    AMHW_ZLG118_VC2_N_INPUT_PB1,
    AMHW_ZLG118_VC2_N_INPUT_PE11,
    AMHW_ZLG118_VC2_N_INPUT_PE15,
    AMHW_ZLG118_VC2_N_INPUT_PB11,
    AMHW_ZLG118_VC2_N_INPUT_PB14,
    AMHW_ZLG118_VC2_N_INPUT_PD10,
    AMHW_ZLG118_VC2_N_INPUT_PD11,
    AMHW_ZLG118_VC2_N_INPUT_PC7,
    AMHW_ZLG118_VC2_N_INPUT_DAC0_0,
    AMHW_ZLG118_VC2_N_INPUT_DAC0_1,
    AMHW_ZLG118_VC2_N_INPUT_NC,
    AMHW_ZLG118_VC2_N_INPUT_INTER_TEMP_SENSOR_OUT_VOL,
    AMHW_ZLG118_VC2_N_INPUT_INTER_BASE_1_2_OUT_VOL,
    AMHW_ZLG118_VC2_N_INPUT_ADC_REF,
    AMHW_ZLG118_VC2_N_INPUT_LDO_OUT_VOL,
}amhw_zlg118_vc2_n_sel_t;

/**
 * \brief VC2 “-”端输入选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : “-”端输入选择，参考枚举定义： VC2 电压比较器 “-”端输入选择
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc2_n_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vc2cr = (p_hw_vc->vc2cr & (~(0xf << 4))) | (flag << 4);
}

/**
 * \brief VC0 电压比较器 “+”端输入选择
 */
typedef enum amhw_zlg118_vc0_p_sel {
    AMHW_ZLG118_VC0_P_INPUT_PC0 = 0,
    AMHW_ZLG118_VC0_P_INPUT_PC1,
    AMHW_ZLG118_VC0_P_INPUT_PC2,
    AMHW_ZLG118_VC0_P_INPUT_PC3,
    AMHW_ZLG118_VC0_P_INPUT_PA0,
    AMHW_ZLG118_VC0_P_INPUT_PA1,
    AMHW_ZLG118_VC0_P_INPUT_PA2,
    AMHW_ZLG118_VC0_P_INPUT_PA3,
    AMHW_ZLG118_VC0_P_INPUT_PA4,
    AMHW_ZLG118_VC0_P_INPUT_PA5,
    AMHW_ZLG118_VC0_P_INPUT_PA6,
    AMHW_ZLG118_VC0_P_INPUT_PA7,
    AMHW_ZLG118_VC0_P_INPUT_PB4,
    AMHW_ZLG118_VC0_P_INPUT_PB5,
    AMHW_ZLG118_VC0_P_INPUT_PB6,
    AMHW_ZLG118_VC0_P_INPUT_DAC0,
}amhw_zlg118_vc0_p_sel_t;

/**
 * \brief VC0 “+”端输入选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : “-”端输入选择，参考枚举定义： VC0 电压比较器 “+”端输入选择
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc0_p_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vc0cr = (p_hw_vc->vc0cr & (~(0xf))) | flag;
}

/**
 * \brief VC1 电压比较器 “+”端输入选择
 */
typedef enum amhw_zlg118_vc1_p_sel {
    AMHW_ZLG118_VC1_P_INPUT_PA0 = 0,
    AMHW_ZLG118_VC1_P_INPUT_PA1,
    AMHW_ZLG118_VC1_P_INPUT_PA2,
    AMHW_ZLG118_VC1_P_INPUT_PA3,
    AMHW_ZLG118_VC1_P_INPUT_PA4,
    AMHW_ZLG118_VC1_P_INPUT_PA5,
    AMHW_ZLG118_VC1_P_INPUT_PB1,
    AMHW_ZLG118_VC1_P_INPUT_PB2,
    AMHW_ZLG118_VC1_P_INPUT_PB10,
    AMHW_ZLG118_VC1_P_INPUT_PB12,
    AMHW_ZLG118_VC1_P_INPUT_PB13,
    AMHW_ZLG118_VC1_P_INPUT_PB14,
    AMHW_ZLG118_VC1_P_INPUT_PB4,
    AMHW_ZLG118_VC1_P_INPUT_DAC0,
    AMHW_ZLG118_VC1_P_INPUT_PB6,
    AMHW_ZLG118_VC1_P_INPUT_PB7,
}amhw_zlg118_vc1_p_sel_t;

/**
 * \brief VC1 “+”端输入选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : “-”端输入选择，参考枚举定义： VC1 电压比较器 “+”端输入选择
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc1_p_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vc1cr = (p_hw_vc->vc1cr & (~(0xf))) | flag;
}

/**
 * \brief VC2 电压比较器 “+”端输入选择
 */
typedef enum amhw_zlg118_vc2_p_sel {
    AMHW_ZLG118_VC2_P_INPUT_PA5 = 0,
    AMHW_ZLG118_VC2_P_INPUT_PB1,
    AMHW_ZLG118_VC2_P_INPUT_PE9,
    AMHW_ZLG118_VC2_P_INPUT_PE10,
    AMHW_ZLG118_VC2_P_INPUT_PE11,
    AMHW_ZLG118_VC2_P_INPUT_PE13,
    AMHW_ZLG118_VC2_P_INPUT_PE14,
    AMHW_ZLG118_VC2_P_INPUT_PE15,
    AMHW_ZLG118_VC2_P_INPUT_PB11,
    AMHW_ZLG118_VC2_P_INPUT_PB14,
    AMHW_ZLG118_VC2_P_INPUT_PD9,
    AMHW_ZLG118_VC2_P_INPUT_PD10,
    AMHW_ZLG118_VC2_P_INPUT_PD11,
    AMHW_ZLG118_VC2_P_INPUT_PC7,
    AMHW_ZLG118_VC2_P_INPUT_DAC0_0,
    AMHW_ZLG118_VC2_P_INPUT_DAC0_1,
}amhw_zlg118_vc2_p_sel_t;

/**
 * \brief VC2 “+”端输入选择
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : “-”端输入选择，参考枚举定义： VC2 电压比较器 “+”端输入选择
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc2_p_sel (amhw_zlg118_vc_t *p_hw_vc, uint32_t flag)
{
    p_hw_vc->vc2cr = (p_hw_vc->vc2cr & (~(0xf))) | flag;
}

/*******************************************************************************
 *  VC0-2 输出寄存器配置
*******************************************************************************/
/**
 * \brief VC0-2 输出寄存器功能使能宏
 */
/**< \brief 结果输出禁能*/
#define AMHW_ZLG118_VC_OUT_CFG_DISABLE                              (1U << 16)
/**< \brief 结果作为Advanced Timer刹车控制*/
#define AMHW_ZLG118_VC_OUT_CFG_BREAK_EN                             (1U << 15)
/**< \brief 结果输出到Timer6捕获输入CHX使能 */
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_TIM6_EN                     (1U << 14)
/**< \brief 结果输出到Timer6反向使能 */
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_INV_TIM6_EN                 (1U << 13)
/**< \brief 结果输出到Timer5捕获输入CHX使能 */
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_TIM5_EN                     (1U << 12)
/**< \brief 结果输出到Timer5反向使能 */
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_INV_TIM5_EN                 (1U << 11)
/**< \brief 结果输出到Timer4捕获输入CHX使能*/
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_TIM4_EN                     (1U << 10)
/**< \brief 结果输出到Timer4反向使能*/
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_INV_TIM4_EN                 (1U << 9)
/**< \brief 结果输出到Timer0,1,2,3刹车控制*/
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_TIM0_1_2_3_EN               (1U << 5)
/**< \brief 结果输出到TIM3 REFCLR使能控制*/
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_TIM3_REFCLR_EN              (1U << 4)
/**< \brief 结果输出到TIM2 REFCLR使能控制*/
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_TIM2_REFCLR_EN              (1U << 3)
/**< \brief 结果输出到TIM1 REFCLR使能控制*/
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_TIM1_REFCLR_EN              (1U << 2)
/**< \brief 结果输出到TIM0 REFCLR使能控制*/
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_TIM0_REFCLR_EN              (1U << 1)
/**< \brief 结果输出反向到各Timer0,1,2,3 REFCLR*/
#define AMHW_ZLG118_VC_OUT_CFG_FIT_DATA_INV_TIM0_1_2_3_REFCLR_EN    (1U << 0)

/**
 * \brief VC0-2 输出寄存器配置使能
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : 输出寄存器功能使能，参考枚宏定义：输出寄存器功能使能宏
 * \param[in] vc_flag : 表示需要使用的模拟比较器，参考： 模拟比较器通道选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_outcfg_enable (amhw_zlg118_vc_t *p_hw_vc,
                                   uint32_t         flag,
                                   uint32_t         vc_flag)
{
    switch (vc_flag){
        case 1 : p_hw_vc->vc0outcfg |=  flag;
                 p_hw_vc->vc1outcfg &= ~flag;
                 p_hw_vc->vc2outcfg &= ~flag;
            break;
        case 2 : p_hw_vc->vc0outcfg &= ~flag;
                 p_hw_vc->vc1outcfg |=  flag;
                 p_hw_vc->vc2outcfg &= ~flag;
            break;
        case 4 : p_hw_vc->vc0outcfg &= ~flag;
                 p_hw_vc->vc1outcfg &= ~flag;
                 p_hw_vc->vc2outcfg |=  flag;
            break;
        default:
            return ;
    }
}

/**
 * \brief VC0-2 输出寄存器配置禁能
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] vc_flag : 表示需要禁能的模拟比较器，参考：模拟比较器通道选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_outcfg_disable (amhw_zlg118_vc_t *p_hw_vc,
                                    uint32_t         vc_flag)
{
    switch (vc_flag){
        case 1 : p_hw_vc->vc0outcfg &= ~(0xffff);
            break;
        case 2 : p_hw_vc->vc1outcfg &= ~(0xffff);
            break;
        case 3 : p_hw_vc->vc0outcfg &= ~(0xffff);
                 p_hw_vc->vc1outcfg &= ~(0xffff);
            break;
        case 4 : p_hw_vc->vc2outcfg &= ~(0xffff);
            break;
        case 5 : p_hw_vc->vc0outcfg &= ~(0xffff);
                 p_hw_vc->vc2outcfg &= ~(0xffff);
            break;
        case 6 : p_hw_vc->vc1outcfg &= ~(0xffff);
                 p_hw_vc->vc2outcfg &= ~(0xffff);
            break;
        case 7 : p_hw_vc->vc0outcfg &= ~(0xffff);
                 p_hw_vc->vc1outcfg &= ~(0xffff);
                 p_hw_vc->vc2outcfg &= ~(0xffff);
            break;
        default:
            return ;
    }
}

/*******************************************************************************
 *  VC 中断寄存器
*******************************************************************************/
/**
 * \brief VC0-2 滤波后状态宏
 */
#define AMHW_ZLG118_VC2_FLT_STATUS    (5U)
#define AMHW_ZLG118_VC1_FLT_STATUS    (3U)
#define AMHW_ZLG118_VC0_FLT_STATUS    (2U)

/**
 * \brief VC0-2 滤波状态读取
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : vc0-2滤波状态检查，参考枚宏定义： VC0-2 滤波后状态宏
 *
 * \retval : VC0-2 滤波状态值
 */
am_static_inline
int amhw_zlg118_vc_flt_status_check (amhw_zlg118_vc_t *p_hw_vc, uint32_t   flag)
{
    return (p_hw_vc->vcifr & (1 << flag)) >> flag;
}

/**
 * \brief VC0-2 中断状态标志宏
 */
#define AMHW_ZLG118_VC2_FLT_INT_FLAG    (4U)
#define AMHW_ZLG118_VC1_FLT_INT_FLAG    (1U)
#define AMHW_ZLG118_VC0_FLT_INT_FLAG    (0U)

/**
 * \brief VC0-2 中断状态读取
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : vc0-2中断状态检查，参考枚宏定义： VC0-2 中断状态标志宏
 *
 * \retval : VC0-2 滤波状态值
 */
am_static_inline
int amhw_zlg118_vc_int_status_check (amhw_zlg118_vc_t *p_hw_vc, uint32_t   flag)
{
    return (p_hw_vc->vcifr & (1 << flag)) >> flag;
}

/**
 * \brief VC0-2 中断状态标志清除宏
 */
#define AMHW_ZLG118_VC2_FLT_INT_FLAG_CLR    (4U)
#define AMHW_ZLG118_VC1_FLT_INT_FLAG_CLR    (1U)
#define AMHW_ZLG118_VC0_FLT_INT_FLAG_CLR    (0U)

/**
 * \brief VC0-2 中断状态标志清除
 *
 * \param[in] p_hw_vc : 指向VC寄存器组基地址指针
 * \param[in] flag : vc0-2中断状态标志清除，参考枚宏定义： VC0-2 中断状态标志清除宏
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_vc_int_status_clr (amhw_zlg118_vc_t *p_hw_vc, uint32_t   flag)
{
    p_hw_vc->vcifr &= ~(1 << flag);
}

/**
 * @} amhw_if_zlg118_vc
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZLG118_VC_H */

/* end of file */

