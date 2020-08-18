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
 * \addtogroup amhw_hc32f07x_if_opa
 * \copydoc amhw_hc32f07x_opa.h
 * @{
 */

/**
 * \brief OPA - 寄存器组
 */
typedef struct amhw_hc32f27x_opa {
         uint32_t  res1[12];     /**< \brief  保留 */
    __IO uint32_t  opacr0;       /**< \brief  OPA 配置寄存器0 */
    __IO uint32_t  opacr1;       /**< \brief  OPA 配置寄存器1 */
	  __IO uint32_t  opacr2;       /**< \brief  OPA 配置寄存器2 */
	  __IO uint32_t  opacr;        /**< \brief  OPA 控制寄存器 */ 
} amhw_hc32f07x_opa_t;

/**
 * \brief OPA 配置编号
 */
#define  AMHW_HC32F07X_OP0   0
#define  AMHW_HC32F07X_OP1   1
#define  AMHW_HC32F07X_OP2   2
#define  AMHW_HC32F07X_OP3   3
#define  AMHW_HC32F07X_OP4   4

/**
 * \brief OPA 输出使能掩码
 */
#define  AMHW_HC32F07X_OPA0_2_OEN0   (0U << 0U)
#define  AMHW_HC32F07X_OPA0_2_OEN1   (1U << 0U)
#define  AMHW_HC32F07X_OPA0_2_OEN2   (1U << 1U)
#define  AMHW_HC32F07X_OPA0_2_OEN3   (1U << 2U)
#define  AMHW_HC32F07X_OPA0_2_OEN4   (1U << 3U)
#define  AMHW_HC32F07X_OPA3_4_OEN    (0)

/**
 * \brief OPA0 输出使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] flag     : 输出x使能掩码，参考宏定义：OPA 输出使能掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa0_out_en (amhw_hc32f07x_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr0 |= flag;
}

/**
 * \brief OPA1 输出使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] flag     : 输出x使能掩码，参考宏定义：OPA 输出使能掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa1_out_en (amhw_hc32f07x_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr0 |= (flag << 4);
}

/**
 * \brief OPA2 输出使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] flag     : 输出x使能掩码，参考宏定义：OPA 输出使能掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa2_out_en (amhw_hc32f07x_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr0 |= (flag<<8);
}




/**
 * \brief OPA 输出禁能掩码
 */
#define  AMHW_HC32F07X_OPA_ODIS1   ~(1U << 0U)
#define  AMHW_HC32F07X_OPA_ODIS2   ~(1U << 1U)
#define  AMHW_HC32F07X_OPA_ODIS3   ~(1U << 2U)
#define  AMHW_HC32F07X_OPA_ODIS4   ~(1U << 3U)

/**
 * \brief OPA 输出禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] flag     : 输出x禁能掩码，参考宏定义：OPA 输出禁能掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa0_out_dis (amhw_hc32f07x_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr0 &= flag;
}

/**
 * \brief OPA 输出禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] flag     : 输出x禁能掩码，参考宏定义：OPA 输出禁能掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa1_out_dis (amhw_hc32f07x_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr0 &= (flag << 4);
}

/**
 * \brief OPA 输出禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] flag     : 输出x禁能掩码，参考宏定义：OPA 输出禁能掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa2_out_dis (amhw_hc32f07x_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr0 &= (flag << 8);
}

/**
 * \brief OPA DAC使用OP3单位增加缓存使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \note : 与OPA使能不能同时使能
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa3buf_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 10);
    p_hw_opa->opacr1 &= ~(1 << 8);
}

/**
 * \brief OPA DAC使用OP4单位增加缓存使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \note : 与OPA使能不能同时使能
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa4buf_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 11);
    p_hw_opa->opacr1 &= ~(1 << 9);
}

/**
 * \brief OPA DAC使用OP3单位增加缓存禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa3buf_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr0 &= ~(1 << 10);
}

/**
 * \brief OPA DAC使用OP4单位增加缓存禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa4buf_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr0 &= ~(1 << 11);
}

/**
 * \brief OPA0 自动校零使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa0_az_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 0);
}

/**
 * \brief OPA1 自动校零使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa1_az_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 1);
}

/**
 * \brief OPA2 自动校零使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa2_az_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 2);
}

/**
 * \brief OPA3 自动校零使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa3_az_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 3);
}

/**
 * \brief OPA4 自动校零使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa4_az_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 4);
}

/**
 * \brief OPA1 自动校零禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa0_az_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 0);
}

/**
 * \brief OPA0 自动校零禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa1_az_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 1);
}

/**
 * \brief OPA2 自动校零禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa2_az_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 2);
}

/**
 * \brief OPA3 自动校零禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa3_az_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 3);
}

/**
 * \brief OPA4 自动校零禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa4_az_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 4);
}

/**
 * \brief OPA0 使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \note : 与OPABUF使能不能同时使能
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa0_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 5);
}

/**
 * \brief OPA1 使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \note : 与OPABUF使能不能同时使能
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa1_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 6);
}

/**
 * \brief OPA2 使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \note : 与OPABUF使能不能同时使能
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa2_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 7);
}

/**
 * \brief OPA3 使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \note : 与OPABUF使能不能同时使能
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa3_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 8);
    p_hw_opa->opacr1 &= ~(1 << 10);
}

/**
 * \brief OPA4 使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \note : 与OPABUF使能不能同时使能
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa4_en (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 |= (1 << 9);
    p_hw_opa->opacr1 &= ~(1 << 11);
}

/**
 * \brief OPA0 禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa0_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 5);
}

/**
 * \brief OPA1 禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa1_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 6);
}

/**
 * \brief OPA2 禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa2_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 7);
}

/**
 * \brief OPA3 禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa3_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 8);
}

/**
 * \brief OPA4 禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa4_dis (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 9);
}

/**
 * \brief 自动校准脉冲宽度设置
 */
typedef enum amhw_hc32f07x_opa_clk_sel {
    AMHW_HC32F07X_OPA_CLK_1 = 0,
    AMHW_HC32F07X_OPA_CLK_2,
    AMHW_HC32F07X_OPA_CLK_4,
    AMHW_HC32F07X_OPA_CLK_8,
    AMHW_HC32F07X_OPA_CLK_16,
    AMHW_HC32F07X_OPA_CLK_32,
    AMHW_HC32F07X_OPA_CLK_64,
    AMHW_HC32F07X_OPA_CLK_128,
    AMHW_HC32F07X_OPA_CLK_256,
    AMHW_HC32F07X_OPA_CLK_512,
    AMHW_HC32F07X_OPA_CLK_1024,
    AMHW_HC32F07X_OPA_CLK_2048,
    AMHW_HC32F07X_OPA_CLK_4096,
}amhw_hc32f07x_opa_clk_sel_t;

/**
 * \brief OPA 自动校准脉冲宽度设置
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in]  flag    : 校准时钟周期，参考枚举定义：自动校准脉冲宽度设置
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa_clk_sel (amhw_hc32f07x_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr = (p_hw_opa->opacr & (~(0xf << 4))) | (flag << 4);
}

/**
 * \brief OPA 自动校零选择掩码
 */
#define  AMHW_HC32F07X_OPA_AZ_SW      1U /**< \brief 软件校准 */
#define  AMHW_HC32F07X_OPA_AZ_SW_TRI  2U /**< \brief 软件触发校准 */
#define  AMHW_HC32F07X_OPA_AZ_ADC     3U /**< \brief ADC触发校准 */

/**
 * \brief OPA 自动校零选择
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in]  flag    : 自动校零方式，参考宏定义：OPA 自动校零选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_opa_az_way_sel (amhw_hc32f07x_opa_t *p_hw_opa, uint32_t flag)
{
    if (flag == AMHW_HC32F07X_OPA_AZ_ADC){
        p_hw_opa->opacr |= (1 << 0);
    }else if (flag == AMHW_HC32F07X_OPA_AZ_SW){
        p_hw_opa->opacr |= (1 << 3);
    }else {
        p_hw_opa->opacr &= ~(1 << 3);
    }
}

/**
 * \brief OPA 校准启动掩码
 */
#define AMHW_HC32F07X_AZ_SW_START        (1U << 2U)  /**< \brief 软件校准启动 */
#define AMHW_HC32F07X_AZ_SW_TRI_START    (1U << 1U)  /**< \brief 软件触发校准启动 */

/**
 * \brief OPA 软件、软件触发校准启动
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in]  flag    : 自动校零方式，参考宏定义：OPA 校准启动掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_az_start (amhw_hc32f07x_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr |= flag;
}

/**
 * \brief OPA 软件触发校准停止
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f07x_az_stop (amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr &= ~(1 << 1);
}

/**
 * \brief OPA ADC启动触发OPA自动校准停止
 * 
 * \param[in] p_hw_opa :指向OPA寄存器组基地址指针
 *
* \retval : 无
 */

am_static_inline
void amhw_hc32f07x_az_sw_adc_stop(amhw_hc32f07x_opa_t *p_hw_opa)
{
    p_hw_opa->opacr &= ~(1 << 0);
}


/**
 * @} amhw_if_hc32f07x_opa
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F07X_OPA_H */

/* end of file */
