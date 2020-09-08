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
 * \brief 定时器TIM接口
 *
 * 1. 16 位向上、向下、向上/下自动装载计数器
 * 2. 16 位可编程（可以实时修改）预分频器，计数器时钟频率的分频系数为 1 ~ 65536 之间的任意数值
 * 3. 多达 4个独立通道(仅限TIM1、TIM2、TIM3，其余只有一个通道)：
 *    -  输入捕获
 *    -  输出比较
 *    -  PWM 生成（边缘或中间对齐模式）
 *    -  单脉冲模式输出
 *    -  死区时间可编程的互补输出
 *    -  使用外部信号控制定时器和定时器互联的同步电路
 * 4. 允许在指定数目的计数器周期之后更新定时器寄存器的重复计数器
 * 5. 刹车输入信号可以将定时器输出信号置于复位状态或者一个已知状态
 * 6. 如下事件发生时产生中断/DMA：
 *    -  更新：计数器向上溢出/向下溢出，计数器初始化（通过软件或者内部/外部触发）
 *    -  触发事件（计数器启动、停止、初始化或者由内部/外部触发计数）
 *    -  输入捕获
 *    -  输出比较
 *    -  刹车信号输入
 * 7.  支持针对定位的增量（正交）编码器和霍尔传感器电路
 * 8.  触发输入作为外部时钟或者按周期的电流管理
 *
 * \note 通用定时器TIMX没有rcr及 bdtr寄存器
 *       通用定时器TIM14只有一个ccmr及ccr寄存器， 没有rcr及 bdtr以下的寄存器
 *       通用定时器TIM16、17只有一个ccmr及ccr寄存器
 *
 * \internal
 * \par History
 * - 1.00 17-04-17  nwt, first implementation
 * \endinternal
 */

#ifndef __AMHW_STM32F103RBT6_TIM_H
#define __AMHW_STM32F103RBT6_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_stm32f103rbt6_if_tim
 * \copydoc amhw_stm32f103rbt6_tim.h
 * @{
 */

/**
  * \brief 定时器寄存器块结构体
  */
typedef struct amhw_stm32f103rbt6_tim {
    __IO uint32_t cr[2];         /**< \brief 控制寄存器 */
    __IO uint32_t smcr;          /**< \brief 从模式控制寄存器 */
    __IO uint32_t dier;          /**< \brief DMA/中断使能寄存器 */
    __IO uint32_t sr;            /**< \brief 状态寄存器 */
    __O  uint32_t egr;           /**< \brief 事件产生寄存器 */
    __IO uint32_t ccmr[2];       /**< \brief 捕捉/比较模式寄存器 */
    __O  uint32_t ccer;          /**< \brief 捕捉/比较使能寄存器 */
    __IO uint32_t cnt;           /**< \brief 计数器 */
    __IO uint32_t psc;           /**< \brief 预分频器 */
    __IO uint32_t arr;           /**< \brief 自动装载寄存器 */
    __IO uint32_t rcr;           /**< \brief 重复计数寄存器 */
    __IO uint32_t ccr[4];        /**< \brief 捕获/比较寄存器 */
    __IO uint32_t bdtr;          /**< \brief 刹车和死区寄存器 */
    __IO uint32_t dcr;           /**< \brief 定时器 DMA控制寄存器 */
    __IO uint32_t dmar;          /**< \brief 连续模式的 DMA 地址 */
} amhw_stm32f103rbt6_tim_t;

/**
 * \brief 得到当前计数器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 当前计数器的值
 */
am_static_inline
uint32_t amhw_stm32f103rbt6_tim_count_get (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    return p_hw_tim->cnt;
}

/**
 * \brief 设置当前计数器的值
 *
 * \param[in] p_hw_tim :    指向TIM定时器寄存器块的指针
 * \param[in] value    :    设置的计数器的值
 *
 * \note: 通用定时器TIM2、TIM3的值为32位, 其它的为16位
 *
 *\ return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_count_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint32_t value)
{
    p_hw_tim->cnt = value;
}

/**
 * \brief 设置预分频寄存器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] value    : 设置的预分频寄存器值（最终的分频为 value+1）
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_prescale_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint16_t value)
{
    p_hw_tim->psc = value;
}

/**
 * \brief 获取预分频寄存器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 预分频寄存器的值
 */
am_static_inline
uint32_t amhw_stm32f103rbt6_tim_prescale_get (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    return p_hw_tim->psc;
}

/**
 * \brief 设置自动装载寄存器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] value    : 自动装载寄存器的值
 *
 * \note: 通用定时器TIM2、TIM3的值为32位, 其它的为16位, 该值一般会在下一次更新事件来临时才会真正生效
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_arr_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint32_t value)
{
    p_hw_tim->arr = value;
}

/**
 * \brief 获取自动装载寄存器的值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 自动装载寄存器的值
 */
am_static_inline
uint32_t amhw_stm32f103rbt6_tim_arr_get (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    return p_hw_tim->arr;
}

/**
 * \brief 使能定时器(开始计数)
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_enable (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    AM_BIT_MODIFY(p_hw_tim->cr[0], 0 , 1);
}

/**
 * \brief 禁止定时器(停止计数)
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_disable (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    AM_BIT_MODIFY(p_hw_tim->cr[0], 0, 0);
}

/**
 * \brief 清除UDIS位，允许更新事件UEV发生
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_udis_enable (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    AM_BIT_MODIFY(p_hw_tim->cr[0], 1 , 0);
}

/**
 * \brief 置UDIS位，禁止更新事件UEV发生
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_udis_disable (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    AM_BIT_MODIFY(p_hw_tim->cr[0], 1, 1);
}

/**
 * \brief 自动重装载预装载允许使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_arpe_enable (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    AM_BIT_MODIFY(p_hw_tim->cr[0], 7 , 1);
}

/**
 * \brief 自动重装载预装载允许禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_arpe_disable (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    AM_BIT_MODIFY(p_hw_tim->cr[0], 7, 0);
}

/**
 * \brief 设置更新源请求
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] urs_opt  : 更新源请求的值
 *                       0：设置 UG位、  从模式控制器产生的更新、溢出产生更新中断/DMA请求
 *                       1： 只有溢出产生更新中断/DMA请求
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_urs_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t urs_opt)
{
    AM_BIT_MODIFY(p_hw_tim->cr[0], 2, urs_opt);
}

/**
 * \brief 设置更新源请求
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] opm_opt  : 是否选择单脉冲模式
 *                       0：在发生更新事件时，计数器不停止
 *                       1： 在发生下一次更新事件（清除 CEN 位）时，计数器停止
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_opm_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t opm_opt)
{
    AM_BIT_MODIFY(p_hw_tim->cr[0], 3, opm_opt);
}

/**
 * \brief 设置计数的方向
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] dir_opt  : 计数方向选择
 *                       0：计数器向上计数
 *                       1：计数器向下计数
 *
 *\note :当计数器配置为中央对齐模式或编码器模式时，该位为只读
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_dir_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t dir_opt)
{
    AM_BIT_MODIFY(p_hw_tim->cr[0], 4, dir_opt);
}

/**
 * \brief 对齐模式设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] cms_opt  : 对齐模式选择
 *                       0: 边沿对齐模式
 *                       1: 中央对齐模式 1
 *                       2: 中央对齐模式 2
 *                       3: 中央对齐模式 3
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_cms_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t cms_opt)
{
    AM_BITS_SET(p_hw_tim->cr[0], 5, 2, cms_opt);
}

/**
 * \brief 时钟分频因子设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ckd_opt  : 时钟分频因子选择
 *                       0: tDTS = tCK_INT(定时器计数时钟)
 *                       1: tDTS = 2 * tCK_INT(定时器计数时钟)
 *                       2: tDTS = 4 * tCK_INT(定时器计数时钟)
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ckd_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t ckd_opt)
{
    AM_BITS_SET(p_hw_tim->cr[0], 8, 2, ckd_opt);
}

/**
 * \brief 捕获/比较预装载控制使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note : 该位只对具有互补输出的通道起作用。
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccpc_enable (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    AM_BIT_MODIFY(p_hw_tim->cr[1], 1, 0);
}

/**
 * \brief 捕获/比较预装载控制禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note : 该位只对具有互补输出的通道起作用。
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccpc_disable (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    AM_BIT_MODIFY(p_hw_tim->cr[1], 0, 0);
}

/**
 * \brief 捕获/比较控制更新设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ccus_opt : 捕获/比较控制更新选择
 *                       0: 捕获/比较控制位是预装载的（CCPC = 1），只能通过设置 COM 位更新
 *                       1: 捕获/比较控制位是预装载的（CCPC = 1），可以通过设置 COM 位或 TRGI上的一个上升沿更新
 *
 * \note : 该位只对具有互补输出的通道起作用。
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccus_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t ccus_opt)
{
    AM_BIT_MODIFY(p_hw_tim->cr[1], 2, ccus_opt);
}

/**
 * \brief 捕获/比较的 DMA设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ccds_opt : 捕获/比较的 DMA 选择选择
 *                       0: 当发生 CCx事件时，送出 CCx的 DMA请求
 *                       1: 当发生更新事件时，送出 CCx的 DMA请求
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccds_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t ccds_opt)
{
    AM_BIT_MODIFY(p_hw_tim->cr[1], 3, ccds_opt);
}

/**
 * \brief 主模式设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] mms_opt  : 主模式选择
 *                       0: 复位; 1: 使能; 2: 更新; 3: 比较脉冲
 *                       4: 比较  – OC1REF 信号被用于作为触发输出（TRGO）
 *                       5: 比较  – OC2REF 信号被用于作为触发输出（TRGO）
 *                       6: 比较  – OC3REF 信号被用于作为触发输出（TRGO）
 *                       7: 比较  – OC4REF 信号被用于作为触发输出（TRGO）
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_mms_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t mms_opt)
{
    AM_BITS_SET(p_hw_tim->cr[1], 4, 3, mms_opt);
}

/**
 * \brief TI1设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ti1s_opt : TI1选择
 *                       0: TIMx_CH1 管脚连到 TI1 输入
 *                       1: TIMx_CH1、TIMx_CH2 和 TIMx_CH3 管脚经异或后连到 TI1 输入
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_tils_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t ti1s_opt)
{
    AM_BIT_MODIFY(p_hw_tim->cr[1], 7, ti1s_opt);
}

/**
 * \brief 输出空闲状态设置枚举类型
 */
typedef enum amhw_stm32f103rbt6_tim_ois {
    AMHW_STM32F103RBT6_TIM_OIS4   = (1UL << 14),  /**< \brief 输出空闲状态 4(OC4 输出) */
    AMHW_STM32F103RBT6_TIM_OIS3N  = (1UL << 13),  /**< \brief 输出空闲状态 3(OC3N 输出) */
    AMHW_STM32F103RBT6_TIM_OIS3   = (1UL << 12),  /**< \brief 输出空闲状态 3(OC3 输出) */
    AMHW_STM32F103RBT6_TIM_OIS2N  = (1UL << 11),  /**< \brief 输出空闲状态 2(OC2N 输出) */
    AMHW_STM32F103RBT6_TIM_OIS2   = (1UL << 10),  /**< \brief 输出空闲状态 2(OC2 输出) */
    AMHW_STM32F103RBT6_TIM_OIS1N  = (1UL << 9),   /**< \brief 输出空闲状态 1(OC1N输出) */
    AMHW_STM32F103RBT6_TIM_OIS1   = (1UL << 8),   /**< \brief 输出空闲状态 1(OC1输出) */
} amhw_stm32f103rbt6_tim_ois_t;

/**
 * \brief 输出空闲状态置1
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ois_opt  : 输出空闲状态选择, 值为 amhw_stm32f103rbt6_tim_ois_t 这一类型枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ois_set (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                              amhw_stm32f103rbt6_tim_ois_t ois_opt)
{
    p_hw_tim->cr[1] |= ois_opt;
}

/**
 * \brief 输出空闲状态清0
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ois_opt  : 出空闲状态选择, 值为 amhw_stm32f103rbt6_tim_ois_t 这一类型枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ois_clr (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                              amhw_stm32f103rbt6_tim_ois_t ois_opt)
{
    p_hw_tim->cr[1] &= ~ois_opt;
}

/**
 * \brief 从模式选择枚举类型
 */
typedef enum amhw_stm32f103rbt6_tim_sms {
    AMHW_STM32F103RBT6_TIM_SLAVE_MODE_DISABLED  = 0,  /**< \brief 关闭从模式 */
    AMHW_STM32F103RBT6_TIM_ENCODER_MODE1        = 1,  /**< \brief 编码器模式 1 */
    AMHW_STM32F103RBT6_TIM_ENCODER_MODE2        = 2,  /**< \brief 编码器模式 2 */
    AMHW_STM32F103RBT6_TIM_ENCODER_MODE3        = 3,  /**< \brief 编码器模式 3 */
    AMHW_STM32F103RBT6_TIM_RESET_MODE           = 4,  /**< \brief 复位模式 */
    AMHW_STM32F103RBT6_TIM_GATED_MODE           = 5,  /**< \brief 门控模式 */
    AMHW_STM32F103RBT6_TIM_TRIGGER_MODE         = 6,  /**< \brief 触发模式 */
    AMHW_STM32F103RBT6_TIM_EXTERNAL_CLOCK_MODE1 = 7,  /**< \brief 外部时钟模式 1 */
} amhw_stm32f103rbt6_tim_sms_t;

/**
 * \brief 从模式设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] sms_opt  : 从模式选择, 值为 amhw_stm32f103rbt6_tim_sms_t 这一类枚举类型
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_sms_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t sms_opt)
{
    AM_BITS_SET(p_hw_tim->smcr, 0, 3, sms_opt);
}

/**
 * \brief 触发选择枚举类型
 */
typedef enum amhw_stm32f103rbt6_tim_ts {
    AMHW_STM32F103RBT6_TIM_ITR0       = 0,  /**< \brief 内部触发 0 */
    AMHW_STM32F103RBT6_TIM_ITR1       = 1,  /**< \brief 内部触发 1 */
    AMHW_STM32F103RBT6_TIM_ITR2       = 2,  /**< \brief 内部触发 2 */
    AMHW_STM32F103RBT6_TIM_ITR3       = 3,  /**< \brief 内部触发 3 */
    AMHW_STM32F103RBT6_TIM_TI1F_ED    = 4,  /**< \brief TI1的边沿检测器 */
    AMHW_STM32F103RBT6_TIM_TI1FP1     = 5,  /**< \brief 滤波后的定时器输入1 */
    AMHW_STM32F103RBT6_TIM_TI2FP2     = 6,  /**< \brief 滤波后的定时器输入2 */
    AMHW_STM32F103RBT6_TIM_ETRF       = 7,  /**< \brief 外部触发输入 */
} amhw_stm32f103rbt6_tim_ts_t;

/**
 * \brief 定时器类型
 */
typedef enum amhw_stm32f103rbt6_tim_type {
    AMHW_STM32F103RBT6_TIM_TYPE0       = 0,  /**< \brief TIM1 */
    AMHW_STM32F103RBT6_TIM_TYPE1       = 1,  /**< \brief TIM2、TIM3 */
    AMHW_STM32F103RBT6_TIM_TYPE2       = 2,  /**< \brief TIM14 */
    AMHW_STM32F103RBT6_TIM_TYPE3       = 3,  /**< \brief TIM16、TIM17 */
} amhw_stm32f103rbt6_tim_type_t;

/**
 * \brief 触发类型设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ts_opt   : 触发类型选择, 值为  amhw_stm32f103rbt6_tim_ts_t 这一类枚举类型
 *
 * \note : 这些位只能在未用到（如 SMS = 000）时被改变，以避免在改变时产生错误的边沿检测
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ts_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t ts_opt)
{
    AM_BITS_SET(p_hw_tim->smcr, 4, 3, ts_opt);
}

/**
 * \brief 主/从模式设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] msm_opt  : 主/从模式选择
 *                       0： 无作用
 *                       1： 触发输入（TRGI）上的事件被延迟了，以允许在当前定时器（通过 TRGO）与它的从定时
 *                             器间的完美同步。这对要求把几个定时器同步到一个单一的外部事件时是非常有用的
 *
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_msm_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t msm_opt)
{
    AM_BIT_MODIFY(p_hw_tim->smcr, 7, msm_opt);
}

/**
 * \brief 外部触发采样滤波选择枚举类型
 */
typedef enum amhw_stm32f103rbt6_tim_etf {
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING0   = 0,  /**< \brief 无滤波器 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING1   = 1,  /**< \brief 采样频率 fSAMPLING = fCK_INT，N = 2 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING2   = 2,  /**< \brief 采样频率 fSAMPLING = fCK_INT，N = 4 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING3   = 3,  /**< \brief 采样频率 fSAMPLING = fCK_INT，N = 8 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING4   = 4,  /**< \brief 采样频率 fSAMPLING = fDTS/2，N = 6 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING5   = 5,  /**< \brief 采样频率 fSAMPLING = fDTS/2，N = 8 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING6   = 6,  /**< \brief 采样频率 fSAMPLING = fDTS/4，N = 6 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING7   = 7,  /**< \brief 采样频率 fSAMPLING = fDTS/4，N = 8 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING8   = 8,  /**< \brief 采样频率 fSAMPLING = fDTS/8，N = 6 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING9   = 9,  /**< \brief 采样频率 fSAMPLING = fDTS/8，N = 8 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING10  = 10, /**< \brief 采样频率 fSAMPLING = fDTS/16，N = 5 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING11  = 11, /**< \brief 采样频率 fSAMPLING = fDTS/16，N = 6 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING12  = 12, /**< \brief 采样频率 fSAMPLING = fDTS/16，N = 8 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING13  = 13, /**< \brief 采样频率 fSAMPLING = fDTS/32，N = 5 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING14  = 14, /**< \brief 采样频率 fSAMPLING = fDTS/32，N = 6 */
    AMHW_STM32F103RBT6_TIM_ETF_FSAMPLING15  = 15, /**< \brief 采样频率 fSAMPLING = fDTS/32，N = 8 */
} amhw_stm32f103rbt6_tim_etf_t;

/**
 * \brief 外部触发滤波设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] etf_opt  : 部触发滤波选择, 值为 amhw_stm32f103rbt6_tim_etf_t 这一类型
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_etf_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t etf_opt)
{
    AM_BITS_SET(p_hw_tim->smcr, 8, 4, etf_opt);
}

/**
 * \brief 外部触发预分频设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] etps_opt : 外部触发预分频选择,
 *                       0: 关闭预分频;     1: ETRP频率除以 2;
 *                       2: ETRP频率除以 4; 3: ETRP频率除以 8;
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_etps_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t etps_opt)
{
    AM_BITS_SET(p_hw_tim->smcr, 12, 2, etps_opt);
}

/**
 * \brief 外部时钟使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note : 该位只对具有互补输出的通道起作用。
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ece_enable (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    AM_BIT_MODIFY(p_hw_tim->smcr, 14, 1);
}

/**
 * \brief 外部时钟禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \note : 该位只对具有互补输出的通道起作用。
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ece_disable (amhw_stm32f103rbt6_tim_t *p_hw_tim)
{
    AM_BIT_MODIFY(p_hw_tim->smcr, 14, 0);
}

/**
 * \brief 外部触发极性设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] etp_opt  : 外部触发极性选择
 *                       0: ETR 不反相，高电平或上升沿有效;
 *                       1: ETR 被反相，低电平或下降沿有效;
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_etp_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t etp_opt)
{
    AM_BIT_MODIFY(p_hw_tim->smcr, 15, etp_opt);
}

/**
 * \brief 定时器相关的DMA请求类型枚举
 */
typedef enum amhw_stm32f103rbt6_tim_dma_req {
    AMHW_STM32F103RBT6_TIM_UDE     = (1UL << 8),   /**< \brief 允许触发 DMA 请求 */
    AMHW_STM32F103RBT6_TIM_CC1DE   = (1UL << 9),   /**< \brief 允许捕获/比较 1的 DMA请求 */
    AMHW_STM32F103RBT6_TIM_CC2DE   = (1UL << 10),  /**< \brief 允许捕获/比较 2的 DMA请求 */
    AMHW_STM32F103RBT6_TIM_CC3DE   = (1UL << 11),  /**< \brief 允许捕获/比较 3的 DMA请求 */
    AMHW_STM32F103RBT6_TIM_CC4DE   = (1UL << 12),  /**< \brief 允许捕获/比较 4的 DMA请求 */
    AMHW_STM32F103RBT6_TIM_COMDE   = (1UL << 13),  /**< \brief 允许 COM的 DMA请求 */
    AMHW_STM32F103RBT6_TIM_TDE     = (1UL << 14),  /**< \brief 允许触发的DMA请求 */
} amhw_stm32f103rbt6_tim_dma_req_t;

/**
 * \brief 定时器相关的DMA请求类型使能
 *
 * \param[in] p_hw_tim   : 指向TIM定时器寄存器块的指针
 * \param[in] dmareq_opt : 定时器相关的DMA请求类型选择, 值为 amhw_stm32f103rbt6_tim_dma_req_t 这一枚举类型
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_dma_req_enable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                  uint8_t            dmareq_opt)
{
    p_hw_tim->dier |= dmareq_opt;
}

/**
 * \brief 定时器相关的DMA请求类型禁能
 *
 * \param[in] p_hw_tim   : 指向TIM定时器寄存器块的指针
 * \param[in] dmareq_opt : 定时器相关的DMA请求类型选择, 值为 amhw_stm32f103rbt6_tim_dma_req_t 这一枚举类型
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_dma_req_disable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                   uint8_t            dmareq_opt)
{
    p_hw_tim->dier &= ~dmareq_opt;
}

/**
 * \brief 定时器相关中断类型枚举
 */
typedef enum amhw_stm32f103rbt6_tim_int {
    AMHW_STM32F103RBT6_TIM_UIE     = (1UL << 0),  /**< \brief 允许更新中断 */
    AMHW_STM32F103RBT6_TIM_CC1IE   = (1UL << 1),  /**< \brief 允许捕获/比较 1 中断 */
    AMHW_STM32F103RBT6_TIM_CC2IE   = (1UL << 2),  /**< \brief 允许捕获/比较 2 中断 */
    AMHW_STM32F103RBT6_TIM_CC3IE   = (1UL << 3),  /**< \brief 允许捕获/比较 3 中断 */
    AMHW_STM32F103RBT6_TIM_CC4IE   = (1UL << 4),  /**< \brief 允许捕获/比较 4 中断 */
    AMHW_STM32F103RBT6_TIM_COMIE   = (1UL << 5),  /**< \brief 允许COM中断 */
    AMHW_STM32F103RBT6_TIM_TIE     = (1UL << 6),  /**< \brief 允许触发中断 */
    AMHW_STM32F103RBT6_TIM_BIE     = (1UL << 7),  /**< \brief 允许刹车中断 */
} amhw_stm32f103rbt6_tim_int_t;

/**
 * \brief 定时器相关中断类型使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] int_opt  : 定时器相关中断求类型选择, 值为 amhw_stm32f103rbt6_tim_int_t 这一枚举类型
 *                       可以或组合 (# (AMHW_STM32F103RBT6_TIM_UIE | AMHW_STM32F103RBT6_TIM_CC1IE))
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_int_enable (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t int_opt)
{
    p_hw_tim->dier |= int_opt;
}

/**
 * \brief 定时器相关中断类型禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] int_opt  : 定时器相关中断类型选择, 值为 amhw_stm32f103rbt6_tim_int_t 这一枚举类型
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_int_disable (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t int_opt)
{
    p_hw_tim->dier &= ~int_opt;
}

/**
 * \brief 定时器相关状态标记位枚举类型
 */
typedef enum amhw_stm32f103rbt6_tim_status_flag {
    AMHW_STM32F103RBT6_TIM_UIF     = (1UL << 0),  /**< \brief 更新中断标记 */
    AMHW_STM32F103RBT6_TIM_CC1IF   = (1UL << 1),  /**< \brief 捕获/比较 1 中断标记 */
    AMHW_STM32F103RBT6_TIM_CC2IF   = (1UL << 2),  /**< \brief 捕获/比较 2 中断标记 */
    AMHW_STM32F103RBT6_TIM_CC3IF   = (1UL << 3),  /**< \brief 捕获/比较 3 中断标记 */
    AMHW_STM32F103RBT6_TIM_CC4IF   = (1UL << 4),  /**< \brief 捕获/比较 4 中断标记 */
    AMHW_STM32F103RBT6_TIM_COMIF   = (1UL << 5),  /**< \brief COM中断标记 */
    AMHW_STM32F103RBT6_TIM_TIF     = (1UL << 6),  /**< \brief 触发中断标记 */
    AMHW_STM32F103RBT6_TIM_BIF     = (1UL << 7),  /**< \brief 刹车中断标记 */
    AMHW_STM32F103RBT6_TIM_CC1OF   = (1UL << 9),  /**< \brief 捕获/比较1重复捕获标记 */
    AMHW_STM32F103RBT6_TIM_CC2OF   = (1UL << 10), /**< \brief 捕获/比较2重复捕获标记 */
    AMHW_STM32F103RBT6_TIM_CC3OF   = (1UL << 11), /**< \brief 捕获/比较3重复捕获标记 */
    AMHW_STM32F103RBT6_TIM_CC4OF   = (1UL << 12), /**< \brief 捕获/比较4重复捕获标记 */
} amhw_stm32f103rbt6_tim_status_flag_t;

/**
 * \brief 取得定时器状态标记
 *
 * \param[in] p_hw_tim   : 指向TIM定时器寄存器块的指针
 * \param[in] status_flg : 定时器相关状态标记选择, 值为  amhw_stm32f103rbt6_tim_status_flag_t 这一枚举类型
 *
 * \return amhw_stm32f103rbt6_tim_status_flag_t 这一枚举类型的值(可以返回或组合)
 */
am_static_inline
uint32_t amhw_stm32f103rbt6_tim_status_flg_get (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                      uint32_t           status_flg)
{
    return (p_hw_tim->sr & status_flg);
}

/**
 * \brief 清除定时器相关标志
 *
 * \param[in] p_hw_tim   : 指向TIM定时器寄存器块的指针
 * \param[in] status_flg : 定时器相关状态标记选择, 值为  amhw_stm32f103rbt6_tim_status_flag_t 这一枚举类型
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_status_flg_clr (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                  uint32_t           status_flg)
{
    p_hw_tim->sr = ~status_flg;
}

/**
 * \brief 定时器相关事件产生枚举类型
 */
typedef enum amhw_stm32f103rbt6_tim_egr {
    AMHW_STM32F103RBT6_TIM_UG     = (1UL << 0),  /**< \brief 更新事件产生 */
    AMHW_STM32F103RBT6_TIM_CC1G   = (1UL << 1),  /**< \brief 捕获/比较 1事件产生 */
    AMHW_STM32F103RBT6_TIM_CC2G   = (1UL << 2),  /**< \brief 捕获/比较 2事件产生 */
    AMHW_STM32F103RBT6_TIM_CC3G   = (1UL << 3),  /**< \brief 捕获/比较 3事件产生 */
    AMHW_STM32F103RBT6_TIM_CC4G   = (1UL << 4),  /**< \brief 捕获/比较 4事件产生 */
    AMHW_STM32F103RBT6_TIM_COMG   = (1UL << 5),  /**< \brief COM事件产生 */
    AMHW_STM32F103RBT6_TIM_TG     = (1UL << 6),  /**< \brief 触发事件产生 */
    AMHW_STM32F103RBT6_TIM_BG     = (1UL << 7),  /**< \brief 刹车事件产生 */
} amhw_stm32f103rbt6_tim_egr_t;

/**
 * \brief 定时器相关事件产生设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] egr_opt  : 定时器相关事件产生选择, 值为  amhw_stm32f103rbt6_tim_egr_t 这一枚举类型
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_egr_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t egr_opt)
{
    p_hw_tim->egr |= egr_opt;
}

/**
 * \brief 死区发生器设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] utg_opt  : 死区发生器选择, 可能死区时间为
 *
 *                       0 到 15875nS，若步长时间为 125nS；
 *                       6uS 到 31750nS，若步长时间为 250nS；
 *                       32uS 到 63uS，若步长时间为 1uS；
 *                       64uS 到 126uS，若步长时间为 2 uS。
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_utg_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t utg_opt)
{
    AM_BITS_SET(p_hw_tim->bdtr, 0, 7, utg_opt);
}

/**
 * \brief 锁定设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] lock_opt : 锁定选择
 *                       0: 锁定关闭，寄存器无写保护；
 *                       1: 锁定级别 1，不能写入 TIMx_BDTR寄存器的 DTG、 BKE、 BKP、 AOE位和
 *                          TIMx_CR2寄存器的 OISx/OISxN 位；
 *                       2: 锁定级别 2，不能写入锁定级别 1中的各位，也不能写入 CC 极性位（一旦相关通道
 *                          通过；CCxS位设为输出，CC极性位是 TIMx_CCER寄存器的 CCxP/CCNxP位）以
 *                          及OSSR/OSSI 位；
 *                       3:锁定级别 3，不能写入锁定级别 2中的各位，也不能写入CC控制位（一旦相关通道通
 *                         过  CCxS位设为输出，CC控制位是 TIMx_CCMRx 寄存器的 OCxM/OCxPE位）。
 *
 * \note：在系统复位后，只能写一次 LOCK位，一旦写入TIMx_BDTR寄存器，则其内容冻结直至复位。
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_lock_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t lock_opt)
{
    AM_BITS_SET(p_hw_tim->bdtr, 8, 2, lock_opt);
}

/**
 * \brief 空闲模式下‘关闭状态’设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ossi_opt : 空闲模式下‘关闭状态’选择
 *                       0: 当定时器不工作时，禁止OC/OCN输出（OC/OCN使能输出信号  = 0）
 *                       1: 当定时器不工作时，一旦 CCxE = 1 或 CCxNE = 1，首先开启 OC/OCN 并输出无效
 *                              电平，然后置 OC/OCN 使能输出信号  = 1。
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ossi_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t ossi_opt)
{
    AM_BIT_MODIFY(p_hw_tim->bdtr, 10, ossi_opt);
}

/**
 * \brief 运行模式下‘关闭状态’设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ossr_opt : 运行模式下‘关闭状态’选择
 *                       0: 当定时器不工作时，禁止OC/OCN输出（OC/OCN使能输出信号  = 0）
 *                       1: 当定时器不工作时，一旦 CCxE = 1 或 CCxNE = 1，首先开启 OC/OCN 并输出无效
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ossr_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t ossr_opt)
{
    AM_BIT_MODIFY(p_hw_tim->bdtr, 11, ossr_opt);
}

/**
 * \brief 定时器死区使能类型枚举
 */
typedef enum amhw_stm32f103rbt6_tim_bdtr_en {
    AMHW_STM32F103RBT6_TIM_MOE   = (1UL << 15),  /**< \brief 主输出使能 */
    AMHW_STM32F103RBT6_TIM_AOE   = (1UL << 14),  /**< \brief 自动输出使能 */
    AMHW_STM32F103RBT6_TIM_BKE   = (1UL << 12),  /**< \brief 刹车功能使能 */
} amhw_stm32f103rbt6_tim_bdtr_en_t;

/**
 * \brief 定时器死区设置相关类型使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] bdtr_opt : 定时器死区设置相关类型使能选择, 值为  amhw_stm32f103rbt6_tim_bdtr_en_t 这一枚举类型
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_bdtr_enable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                               uint32_t           bdtr_opt)
{
    p_hw_tim->bdtr |= bdtr_opt;
}

/**
 * \brief 定时器死区设置相关类型禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] bdtr_opt : 定时器死区设置相关类型禁能选择, 值为  amhw_stm32f103rbt6_tim_bdtr_en_t 这一枚举类型
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_bdtr_disable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                uint32_t           bdtr_opt)
{
    p_hw_tim->bdtr &= ~bdtr_opt;
}

/**
 * \brief 刹车输入极性设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] bkp_opt  : 刹车输入极性选择, 0: 刹车输入低电平有效; 1: 刹车输入高电平有效
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_bkp_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t bkp_opt)
{
    AM_BIT_MODIFY(p_hw_tim->bdtr, 13, bkp_opt);
}

/**
 * \brief 定时器DMA基地址设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] dba_opt  : 定时器DMA基地址选择, 值为0 ~ 31
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_dba_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t dba_opt)
{
    AM_BITS_SET(p_hw_tim->dcr, 0, 5, dba_opt);
}

/**
 * \brief 定时器DMA 连续传送长度
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] dbl_opt  : DMA 连续传送长度选择, 值为0 ~ 31
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_dbl_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint8_t dbl_opt)
{
    AM_BITS_SET(p_hw_tim->dcr, 8, 5, dbl_opt);
}

/**
 * \brief 定时器DMA连续模式的基地址设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] dmab_opt : 定时器DMA连续模式的基地址选择, 值为0 ~ 65536
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_dmab_set (amhw_stm32f103rbt6_tim_t *p_hw_tim, uint16_t dmab_opt)
{
    p_hw_tim->dcr = dmab_opt;
}

/**
 * \brief 定时器输入捕获/比较输出通道枚举类型
 */
typedef enum amhw_stm32f103rbt6_tim_ccs_chan {
    AMHW_STM32F103RBT6_TIM_CC1S   = 0,  /**< \brief CC1S */
    AMHW_STM32F103RBT6_TIM_CC2S   = 1,  /**< \brief CC2S */
    AMHW_STM32F103RBT6_TIM_CC3S   = 2,  /**< \brief CC3S */
    AMHW_STM32F103RBT6_TIM_CC4S   = 3,  /**< \brief CC4S */

} amhw_stm32f103rbt6_tim_ccs_chan_t;

/**
 * \brief 捕获/比较ccs通道设置
 *
 * \param[in] p_hw_tim  : 指向TIM定时器寄存器块的指针
 * \param[in] ccs_opt   : 捕获/比较ccs 通道选择
 *                        0: CC*通道被配置为输出
 *                        1: CC*通道被配置为输入，IC1 映射在 TI1上
 *                        2: CC*通道被配置为输入，IC2 映射在 TI1上
 *                        3: CC*通道被配置为输入，IC* 映射在 TRC上
 *
 * \note :  When ccs_opt is 1, TIM Input 1, 2, 3 or 4 is selected to be
 *          connected  to IC1, IC2, IC3 or IC4, respectively
 *
 * \note :  When ccs_opt is 2, TIM Input 1, 2, 3 or 4 is selected to be
 *          connected  to IC2, IC1, IC4 or IC3, respectively
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \note : CC*S 仅在通道关闭时（TIMx_CCER 寄存器的 CC*E = 0）才是可写的
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccs_set (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                           uint8_t            ccs_opt,
                           uint32_t           ccs_chan)
{
    AM_BITS_SET(p_hw_tim->ccmr[ccs_chan >> 1], (8 * (ccs_chan & 0x01)) , 2, ccs_opt);
}

/*******************************************************************************
                   以下函数仅在ccmr寄存器设置cc*s位设置为 输出比较模式时起作用
*******************************************************************************/

/**
 * \brief 通道输出比较快速使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccs_ocfe_enable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                   uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccmr[ccs_chan >> 1], (8 * (ccs_chan & 0x01) + 2), 1);
}

/**
 * \brief 通道输出比较快速禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccs_ocfe_disable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                    uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccmr[ccs_chan >> 1], (8 * (ccs_chan & 0x01) + 2), 0);
}

/**
 * \brief 通道输出比较预装载使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccs_ocpe_enable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                   uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccmr[ccs_chan >> 1], (8 * (ccs_chan & 0x01) + 3), 1);
}

/**
 * \brief 通道输出比较预装载禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccs_ocpe_disable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                    uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccmr[ccs_chan >> 1], (8 * (ccs_chan & 0x01) + 3), 0);
}

/**
 * \brief 通道输出比较清0使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccs_occe_enable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                   uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccmr[ccs_chan >> 1], (8 * (ccs_chan & 0x01) + 7), 1);
}

/**
 * \brief 通道输出比较清0禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccs_occe_disable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                    uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccmr[ccs_chan >> 1], (8 * (ccs_chan & 0x01) + 7), 0);
}

/**
 * \brief 定时器输出比较枚举类型
 */
typedef enum amhw_stm32f103rbt6_tim_output_cmp_mode {
    AMHW_STM32F103RBT6_TIM_FROZEN                   = 0,  /**< \brief 冻结 */
    AMHW_STM32F103RBT6_TIM_MATCH_CHANNAL_SETACTIVE  = 1,  /**< \brief 匹配时设置通道为有效电平 */
    AMHW_STM32F103RBT6_TIM_MATCH_CHANNAL_SETINACTIV = 2,  /**< \brief 匹配时设置通道 为无效电平 */
    AMHW_STM32F103RBT6_TIM_TOGGLE                   = 3,  /**< \brief 翻转 */
    AMHW_STM32F103RBT6_TIM_FORCE_INACTIVE_LEVEL     = 4,  /**< \brief 强制为无效电平 */
    AMHW_STM32F103RBT6_TIM_FORCE_ACTIVE_LEVEL       = 5,  /**< \brief 强制为有效电平 */
    AMHW_STM32F103RBT6_TIM_PWM_MODE1                = 6,  /**< \brief PWM模式1 */
    AMHW_STM32F103RBT6_TIM_PWM_MODE2                = 7,  /**< \brief PWM模式2 */
} amhw_stm32f103rbt6_tim_output_cmp_mode_t;

/**
 * \brief 通道输出比较模式
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ocm_opt  : 输出比较模式选择, 值为 amhw_stm32f103rbt6_tim_output_cmp_mode_t 这一枚举类型
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \note : 1. OC*M 一旦 LOCK 级别设为 3 （TIMx_BDTR 寄存器中的 LOCK 位）并且 CC*S = 00 （该
 *           通道配置成输出）则该位不能被修改
 *         2. 在 PWM 模式 1 或 PWM 模式 2 中，只有当比较结果改变了或在输出比较模式中从
 *           冻结模式切换到 PWM 模式时，OC*REF 电平才改变
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ocm_set (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                           uint8_t            ocm_opt,
                           uint8_t            ccs_chan)
{
    AM_BITS_SET(p_hw_tim->ccmr[ccs_chan >> 1], (8 * (ccs_chan & 0x01) + 4) , 3, ocm_opt);
}

/**
 * \brief 通道比较输出电平极性
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ccp_opt  : 通道比较输出电平极性选择, 0: OC1高电平有效; 1: OC1低电平有效
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccp_output_set (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                  uint8_t            ccp_opt,
                                  uint32_t           ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccer, (4 * ccs_chan + 1), ccp_opt);
}

/**
 * \brief 比较输出通道使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_cce_output_enable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                     uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccer, (4 * ccs_chan ), 1);
}

/**
 * \brief 比较输出通道禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_cce_output_disable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                      uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccer, (4 * ccs_chan ), 0);
}

/**
 * \brief 获取设置比较输出通道的预装载值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ch       : 匹配通道编号  有效值0-3
 *
 * \note:  通用定时器TIM2、TIM3该寄存器为32位，其它为16位

 * \return 比较输出通道的预装载的值
 */
am_static_inline
uint32_t amhw_stm32f103rbt6_tim_ouput_reload_val_get (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                            uint8_t            ch)
{
    return p_hw_tim->ccr[ch];
}

/**
 * \brief 设置比较输出通道的预装载值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] value    : 比较输出通道的预装载值
 * \param[in] ch       : 捕获通道，有效值0-3
 *
 *\ note:  通用定时器TIM2、TIM3该寄存器为32位，其它为16位
 *
 * \return 指定捕获通道的捕获值
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccr_ouput_reload_val_set (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                            uint32_t           value,
                                            uint8_t            ch)
{
    p_hw_tim->ccr[ch] = value;
}

/*******************************************************************************
                   以下函数仅在ccmr寄存器设置cc*s位设置为输入比较模式时起作用
*******************************************************************************/

/**
 * \brief 通道输入/捕获预分频器
 *
 * \param[in] p_hw_tim  : 指向TIM定时器寄存器块的指针
 * \param[in] icpsc_opt : 捕获/比较 分频选择
 *                        0: 无分频;              1: 每 2个事件触发一次捕获
 *                        2: 每 4个事件触发一次捕获;   3: 每 8个事件触发一次捕获
 *
 * \param[in] ccs_chan  : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_icpsc_set (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                             uint8_t            icpsc_opt,
                             uint8_t            ccs_chan)
{
    AM_BITS_SET(p_hw_tim->ccmr[ccs_chan >> 1], (8 * (ccs_chan & 0x01) + 2) , 2, icpsc_opt);
}

/**
 * \brief 通道输入捕获采样滤波选择枚举类型
 */
typedef enum amhw_stm32f103rbt6_tim_icf {
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING0   = 0x00,  /**< \brief 无滤波器 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING1   = 0x08,  /**< \brief 采样频率 fSAMPLING = fDTS/8，N = 6 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING2   = 0x01,  /**< \brief 采样频率 fSAMPLING = fCK_INT，N = 2 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING3   = 0x09,  /**< \brief 采样频率 fSAMPLING = fCK_INT，N = 4 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING4   = 0x02,  /**< \brief 采样频率 fSAMPLING = fDTS/16，N = 5 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING5   = 0x0A,  /**< \brief 采样频率 fSAMPLING = fCK_INT，N = 8 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING6   = 0x03,  /**< \brief 采样频率 fSAMPLING = fDTS/16，N = 6 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING7   = 0x0B,  /**< \brief 采样频率 fSAMPLING = fDTS/2，N = 6 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING8   = 0x04,  /**< \brief 采样频率 fSAMPLING = fDTS/16，N = 8 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING9   = 0x0C,  /**< \brief 采样频率 fSAMPLING = fDTS/2，N = 8 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING10  = 0x05,  /**< \brief 采样频率 fSAMPLING = fDTS/16，N = 5 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING11  = 0x0D,  /**< \brief 采样频率 fSAMPLING = fDTS/4，N = 6 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING12  = 0x06,  /**< \brief 采样频率 fSAMPLING = fDTS/32，N = 6 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING13  = 0x0E,  /**< \brief 采样频率 fSAMPLING = fDTS/4，N = 8 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING14  = 0x07,  /**< \brief 采样频率 fSAMPLING = fDTS/4，N = 8 */
    AMHW_STM32F103RBT6_TIM_ICF_FSAMPLING15  = 0x0F,  /**< \brief 采样频率 fSAMPLING = fDTS/32，N = 8 */
} amhw_stm32f103rbt6_tim_icf_t;

/**
 * \brief 通道输入捕获采样滤波频率设置
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] icf_opt  : 输入捕获采样滤波频率选择，值为 amhw_stm32f103rbt6_tim_icf_t 这一枚举类型
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_icf_set (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                           uint8_t            icf_opt,
                           uint8_t            ccs_chan)
{
    AM_BITS_SET(p_hw_tim->ccmr[ccs_chan >> 1], (8 * (ccs_chan & 0x01) + 4) , 4, icf_opt);
}

/**
 * \brief 输入捕获(外部触发)电平极性
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] type     : 定时器类型
 * \param[in] ccp_opt  : 输入捕获通道电平极性选择
 *                       1: 不反相：捕获发生在 ICx 的上升沿；当用作外部触发器时，ICx不反相;
 *                       2: 反相：捕获发生在 ICx 的下降沿；当用作外部触发器时， ICx反相
 *                       其中 x代表捕获通道，根据定时器的不同而通道数量也有所不同，最多为4个通道
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \note : 一旦 LOCK 级别（TIMx_BDTR 寄存器中的 LCCK 位）设为 3 或 2，则该位不能被修改
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccp_captrigger_set (amhw_stm32f103rbt6_tim_t        *p_hw_tim,
                                      amhw_stm32f103rbt6_tim_type_t    type,
                                      uint8_t                ccp_opt,
                                      uint8_t                ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccer, (4 * ccs_chan + 1), ccp_opt >> 1);
}

/**
 * \brief 输入捕获通道使能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_cce_cap_enable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                  uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccer, (4 * ccs_chan), 1);
}

/**
 * \brief 输入捕获/比较通道禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_cce_cap_disable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                   uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccer, (4 * ccs_chan), 0);
}

/**
 * \brief 输入捕获互补输出极性
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ccnp_opt : 输入捕获互补输出极性选择
 *                       0: OCN高电平有效;
 *                       1: OCN低电平有效;
 *
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \note : 一旦 LOCK 级别（TIMx_BDTR 寄存器中的 LCCK 位）设为 3 或 2，则该位不能被修改
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccne_set (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                            uint8_t            ccnp_opt,
                            uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccer, (4 * ccs_chan + 3), ccnp_opt);
}

/**
 * \brief 输入捕获互补输出通道使能
 *
 * \param[in] p_hw_tim :    指向TIM定时器寄存器块的指针
 * \param[in] ccs_chan :  ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccne_enable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                               uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccer, (4 * ccs_chan + 2), 1);
}

/**
 * \brief 输入捕获互补输出通道禁能
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ccs_chan : ccs通道选择，值为 amhw_stm32f103rbt6_tim_ccs_chan_t 这一类型的枚举
 *
 * \return 无
 */
am_static_inline
void amhw_stm32f103rbt6_tim_ccne_cap_disable (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                    uint8_t            ccs_chan)
{
    AM_BIT_MODIFY(p_hw_tim->ccer, (4 * ccs_chan + 2), 0);
}

/**
 * \brief 从一个捕获通道获取捕获值
 *
 * \param[in] p_hw_tim : 指向TIM定时器寄存器块的指针
 * \param[in] ch       : 捕获通道，有效值0-3
 *
 * \return 返回指定捕获通道的捕获值
 */
am_static_inline
uint32_t amhw_stm32f103rbt6_tim_ccr_cap_val_get (amhw_stm32f103rbt6_tim_t    *p_hw_tim,
                                       uint8_t            ch)
{
    return p_hw_tim->ccr[ch];
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_STM32F103RBT6_TIM_H */

/* end of file */
