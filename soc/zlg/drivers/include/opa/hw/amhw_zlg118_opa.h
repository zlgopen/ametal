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
 * \brief OPA 底层操作接口
 * \internal
 * \par Modification History
 * - 1.00 19-10-10
 * \endinternal
 */

#ifndef __AMHW_ZLG118_OPA_H
#define __AMHW_ZLG118_OPA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zlg118_if_opa
 * \copydoc amhw_zlg118_opa.h
 * @{
 */

/**
 * \brief OPA - 寄存器组
 */
typedef struct amhw_zlg118_opa {
         uint32_t  res1[12];     /**< \brief  保留 */
    __IO uint32_t  opacr0;       /**< \brief  OPA 配置寄存器0 */
         uint32_t  res2[2];      /**< \brief  保留 */
    __IO uint32_t  opacr1;       /**< \brief  OPA 配置寄存器1 */
} amhw_zlg118_opa_t;

/**
 * \brief OPA 输出使能掩码
 */
#define  AMHW_ZLG118_OPA_OEN1   (1U << 3U)
#define  AMHW_ZLG118_OPA_OEN2   (1U << 4U)
#define  AMHW_ZLG118_OPA_OEN3   (1U << 5U)
#define  AMHW_ZLG118_OPA_OEN4   (1U << 6U)

/**
 * \brief OPA 输出使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] flag     : 输出x使能掩码，参考宏定义：OPA 输出使能掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_opa_out_en (amhw_zlg118_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr0 |= flag;
}

/**
 * \brief OPA 输出禁能掩码
 */
#define  AMHW_ZLG118_OPA_ODIS1   ~(1U << 3U)
#define  AMHW_ZLG118_OPA_ODIS2   ~(1U << 4U)
#define  AMHW_ZLG118_OPA_ODIS3   ~(1U << 5U)
#define  AMHW_ZLG118_OPA_ODIS4   ~(1U << 6U)

/**
 * \brief OPA 输出禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in] flag     : 输出x禁能掩码，参考宏定义：OPA 输出禁能掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_opa_out_dis (amhw_zlg118_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr0 &= flag;
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
void amhw_zlg118_opabuf_en (amhw_zlg118_opa_t *p_hw_opa)
{
    p_hw_opa->opacr0 |= (1 << 2);
    p_hw_opa->opacr0 &= ~(1 << 0);
}

/**
 * \brief OPA DAC使用OP3单位增加缓存禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_opabuf_dis (amhw_zlg118_opa_t *p_hw_opa)
{
    p_hw_opa->opacr0 &= ~(1 << 2);
}

/**
 * \brief OPA 自动校零使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_opa_az_en (amhw_zlg118_opa_t *p_hw_opa)
{
    p_hw_opa->opacr0 |= (1 << 1);
}

/**
 * \brief OPA 自动校零禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_opa_az_dis (amhw_zlg118_opa_t *p_hw_opa)
{
    p_hw_opa->opacr0 &= ~(1 << 1);
}

/**
 * \brief OPA 使能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \note : 与OPABUF使能不能同时使能
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_opa_en (amhw_zlg118_opa_t *p_hw_opa)
{
    p_hw_opa->opacr0 |= (1 << 0);
    p_hw_opa->opacr0 &= ~(1 << 2);
}

/**
 * \brief OPA 禁能
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_opa_dis (amhw_zlg118_opa_t *p_hw_opa)
{
    p_hw_opa->opacr0 &= ~(1 << 0);
}

/**
 * \brief 自动校准脉冲宽度设置
 */
typedef enum amhw_zlg118_opa_clk_sel {
    AMHW_ZLG118_OPA_CLK_1 = 0,
    AMHW_ZLG118_OPA_CLK_2,
    AMHW_ZLG118_OPA_CLK_4,
    AMHW_ZLG118_OPA_CLK_8,
    AMHW_ZLG118_OPA_CLK_16,
    AMHW_ZLG118_OPA_CLK_32,
    AMHW_ZLG118_OPA_CLK_64,
    AMHW_ZLG118_OPA_CLK_128,
    AMHW_ZLG118_OPA_CLK_256,
    AMHW_ZLG118_OPA_CLK_512,
    AMHW_ZLG118_OPA_CLK_1024,
    AMHW_ZLG118_OPA_CLK_2048,
    AMHW_ZLG118_OPA_CLK_4096,
}amhw_zlg118_opa_clk_sel_t;

/**
 * \brief OPA 自动校准脉冲宽度设置
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in]  flag    : 校准时钟周期，参考枚举定义：自动校准脉冲宽度设置
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_opa_clk_sel (amhw_zlg118_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr1 = (p_hw_opa->opacr1 & (~(0xf << 4))) | (flag << 4);
}

/**
 * \brief OPA 自动校零选择掩码
 */
#define  AMHW_ZLG118_OPA_AZ_SW      1U /**< \brief 软件校准 */
#define  AMHW_ZLG118_OPA_AZ_SW_TRI  2U /**< \brief 软件触发校准 */
#define  AMHW_ZLG118_OPA_AZ_ADC     3U /**< \brief ADC触发校准 */

/**
 * \brief OPA 自动校零选择
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in]  flag    : 自动校零方式，参考宏定义：OPA 自动校零选择掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_opa_az_way_sel (amhw_zlg118_opa_t *p_hw_opa, uint32_t flag)
{
    if (flag == AMHW_ZLG118_OPA_AZ_ADC){
        p_hw_opa->opacr1 |= (1 << 0);
    }else if (flag == AMHW_ZLG118_OPA_AZ_SW){
        p_hw_opa->opacr1 |= (1 << 3);
    }else {
        p_hw_opa->opacr1 &= ~(1 << 3);
    }
}

/**
 * \brief OPA 校准启动掩码
 */
#define AMHW_ZLG118_AZ_SW_START        (1U << 2U)  /**< \brief 软件校准启动 */
#define AMHW_ZLG118_AZ_SW_TRI_START    (1U << 1U)  /**< \brief 软件触发校准启动 */

/**
 * \brief OPA 软件、软件触发校准启动
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 * \param[in]  flag    : 自动校零方式，参考宏定义：OPA 校准启动掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_az_start (amhw_zlg118_opa_t *p_hw_opa, uint32_t flag)
{
    p_hw_opa->opacr1 |= flag;
}

/**
 * \brief OPA 软件触发校准停止
 *
 * \param[in] p_hw_opa : 指向OPA寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zlg118_az_stop (amhw_zlg118_opa_t *p_hw_opa)
{
    p_hw_opa->opacr1 &= ~(1 << 1);
}


/**
 * @} amhw_if_zlg118_opa
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZLG118_OPA_H */

/* end of file */
