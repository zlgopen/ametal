/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ACMP 硬件操作接口描述
 *
 * 1. 可选外部输入既可用作比较器的正输入，也可用作比较器的负输入；
 * 2. 内部基准电压（0.9V带隙基准电压）既可用作比较器的正输入，也可用作比较器的负输入；
 * 3. 32级阶梯电压既可用作比较器的正输入，也可用作比较器的负输入；
 * 4. 电压阶梯源的可选范围在电源引脚VDD或VDDCMP引脚之间；
 * 5. 电压阶梯在不需要时可单独掉电；
 * 6. 中断功能。
 *
 * \internal
 * \par Modification history
 * - 1.02 15-12-12  hgo, fix amhw_lpc82x_acmp_ladsel_set() bug.
 * - 1.01 15-12-04  hgo, modify macro definition.
 * - 1.00 15-07-16  bob, first implementation.
 * \endinternal
 */
#ifndef  __AMHW_LPC82X_ACMP_H
#define  __AMHW_LPC82X_ACMP_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amhw_lpc82x_if_acmp
 * \copydoc amhw_lpc82x_acmp.h
 * @{
 */

/**
 * \brief ACMP 寄存器块结构体
 */
typedef struct amhw_lpc82x_acmp {
    __IO uint32_t ctrl;            /**< \brief ACMP 控制寄存器          */
    __IO uint32_t lad;             /**< \brief ACMP 输入控制寄存器      */
} amhw_lpc82x_acmp_t;

/**
 * \name ACMP 控制寄存器位域定义
 * \anchor grp_amhw_lpc82x_acmp_ctrl
 * @{
 */

/** \brief COMPEDGE上升沿输出            */
#define AMHW_LPC82X_ACMP_CTRL_EDGESEL_FALL AM_SBF(0UL, 3)

/** \brief COMPEDGE上升沿输出            */
#define AMHW_LPC82X_ACMP_CTRL_EDGESEL_RISE AM_SBF(1UL, 3)

/** \brief COMPEDGE上升沿下降沿输出      */
#define AMHW_LPC82X_ACMP_CTRL_EDGESEL_BOTH AM_SBF(2UL, 3)

/** \brief 比较器输出与总线时钟同步输出  */
#define AMHW_LPC82X_ACMP_CTRL_COMPSA       AM_SBF(1UL, 6)

/** \brief 电压阶梯正输入  */
#define AMHW_LPC82X_ACMP_CTRL_VP_LADOUT    AM_SBF(0UL, 8)

/** \brief 比较器正输入1   */
#define AMHW_LPC82X_ACMP_CTRL_VP_CMP1      AM_SBF(1UL, 8)

/** \brief 比较器正输入2   */
#define AMHW_LPC82X_ACMP_CTRL_VP_CMP2      AM_SBF(2UL, 8)

/** \brief 比较器正输入3   */
#define AMHW_LPC82X_ACMP_CTRL_VP_CMP3      AM_SBF(3UL, 8)

/** \brief 比较器正输入4   */
#define AMHW_LPC82X_ACMP_CTRL_VP_CMP4      AM_SBF(4UL, 8)

/** \brief 基准电压正输入  */
#define AMHW_LPC82X_ACMP_CTRL_VP_BGAP      AM_SBF(5UL, 8)

/** \brief ADC通道0正输入  */
#define AMHW_LPC82X_ACMP_CTRL_VP_ADC0      AM_SBF(6UL, 8)

/** \brief 电压阶梯负输入  */
#define AMHW_LPC82X_ACMP_CTRL_VM_LADOUT    AM_SBF(0UL, 11)

/** \brief 比较器正输入1   */
#define AMHW_LPC82X_ACMP_CTRL_VM_CMP1      AM_SBF(1UL, 11)

/** \brief 比较器正输入2   */
#define AMHW_LPC82X_ACMP_CTRL_VM_CMP2      AM_SBF(2UL, 11)

/** \brief 比较器正输入3   */
#define AMHW_LPC82X_ACMP_CTRL_VM_CMP3      AM_SBF(3UL, 11)

/** \brief 比较器正输入4   */
#define AMHW_LPC82X_ACMP_CTRL_VM_CMP4      AM_SBF(4UL, 11)

/** \brief 基准电压负输入  */
#define AMHW_LPC82X_ACMP_CTRL_VM_BGAP      AM_SBF(5UL, 11)

/** \brief ADC通道0负输入  */
#define AMHW_LPC82X_ACMP_CTRL_VM_ADC0      AM_SBF(6UL, 11)

/** \brief 中断清零位      */
#define AMHW_LPC82X_ACMP_CTRL_EDGECLR      AM_SBF(1UL, 20)

/** \brief 比较器输出状态  */
#define AMHW_LPC82X_ACMP_CTRL_COMPSTAT     AM_SBF(1UL, 21)

/** \brief 边沿检测状态    */
#define AMHW_LPC82X_ACMP_CTRL_COMPEDGE     AM_SBF(0UL, 23)

/** \brief 比较器迟滞无    */
#define AMHW_LPC82X_ACMP_CTRL_HYS_NONE     AM_SBF(0UL, 25)

/** \brief 比较器迟滞5mV   */
#define AMHW_LPC82X_ACMP_CTRL_HYS_5MV      AM_SBF(1UL, 25)

/** \brief 比较器迟滞10mV  */
#define AMHW_LPC82X_ACMP_CTRL_HYS_10MV     AM_SBF(2UL, 25)

/** \brief 比较器迟滞20mV  */
#define AMHW_LPC82X_ACMP_CTRL_HYS_20MV     AM_SBF(3UL, 25)

/** @} */

/**
 * \name ACMP 电压阶梯寄存器位域定义
 * \anchor grp_amhw_lpc82x_acmp_ladder
 * @{
 */

/** \brief 阶梯电压使能         */
#define AMHW_LPC82X_ACMP_LADDER_LADEN      AM_SBF(1UL, 0)

/** \brief 基准电压为电源VDD    */
#define AMHW_LPC82X_ACMP_LADDER_VDD        AM_SBF(0UL, 6)

/** \brief 基准电压为VDDCMP引脚 */
#define AMHW_LPC82X_ACMP_LADDER_VDDCMP     AM_SBF(1UL, 6)

/** \brief 电压阶梯值    */
#define AMHW_LPC82X_ACMP_LADDER_LADSEL(n)  AM_SBF((n) & 0x1F, 1)

/** @} */


/**
 * \brief ACMP控制寄存器配置
 *
 * \param[in] p_hw_acmp     : 指向ACMP寄存器块的指针
 * \param[in] flags         : AMHW_LPC82X_ACMP_CTRL_* 宏值或
 *                            多个AMHW_LPC82X_ACMP_CTRL_*宏的或值
 *                            如(#AMHW_LPC82X_ACMP_CTRL_VM_CMP1)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_acmp_config(amhw_lpc82x_acmp_t *p_hw_acmp, uint32_t flags)
{
    p_hw_acmp->ctrl = flags;
}

/**
 * \brief ACMP控制寄存器位配置
 *
 * \param[in] p_hw_acmp     : 指向ACMP寄存器块的指针
 * \param[in] flags         : AMHW_LPC82X_ACMP_CTRL_* 宏值或
 *                            多个AMHW_LPC82X_ACMP_CTRL_*宏的或值
 *                           (#AMHW_LPC82X_ACMP_CTRL_VM_CMP1)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_acmp_ctl_set(amhw_lpc82x_acmp_t *p_hw_acmp, uint32_t flags)
{
    AM_BIT_SET_MASK(p_hw_acmp->ctrl, flags);
}

/**
 * \brief ACMP控制寄存器位清除
 *
 * \param[in] p_hw_acmp     : 指向ACMP寄存器块的指针
 * \param[in] flags         : AMHW_LPC82X_ACMP_CTRL_* 宏值或
 *                            多个AMHW_LPC82X_ACMP_CTRL_*宏的或值
 *                            (#AMHW_LPC82X_ACMP_CTRL_VM_CMP1)
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_acmp_ctl_clr(amhw_lpc82x_acmp_t *p_hw_acmp, uint32_t flags)
{
    AM_BIT_CLR_MASK(p_hw_acmp->ctrl, flags);
}

/**
 * \brief ACMP控制寄存器配置获取
 *
 * \param[in] p_hw_acmp    : 指向ACMP寄存器块的指针
 *
 * \return 控制寄存器的值
 */
am_static_inline
uint32_t amhw_lpc82x_acmp_ctl_get(amhw_lpc82x_acmp_t *p_hw_acmp)
{
    return p_hw_acmp->ctrl;
}

/**
 * \brief 清除ACMP中断
 *
 * \param[in] p_hw_acmp    : 指向ACMP寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_acmp_int_clr(amhw_lpc82x_acmp_t *p_hw_acmp)
{
    AM_BIT_SET_MASK(p_hw_acmp->ctrl, AMHW_LPC82X_ACMP_CTRL_EDGECLR);
    AM_BIT_CLR_MASK(p_hw_acmp->ctrl, AMHW_LPC82X_ACMP_CTRL_EDGECLR);
}

/**
 * \brief ACMP阶梯电压使能
 *
 * \param[in] p_hw_acmp    : 指向ACMP寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_acmp_ladder_enable(amhw_lpc82x_acmp_t *p_hw_acmp)
{
    AM_BIT_SET_MASK(p_hw_acmp->lad, AMHW_LPC82X_ACMP_LADDER_LADEN);
}

/**
 * \brief ACMP阶梯电压禁能
 *
 * \param[in] p_hw_acmp    : 指向ACMP寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_acmp_ladder_disable(amhw_lpc82x_acmp_t *p_hw_acmp)
{
    AM_BIT_CLR_MASK(p_hw_acmp->lad, AMHW_LPC82X_ACMP_LADDER_LADEN);
}

/**
 * \brief ACMP选择基准电压引脚（电源VDD引脚或VDDCMP引脚）
 *
 * \param[in] p_hw_acmp  : 指向ACMP寄存器块的指针
 * \param[in] io_flag    : 控制标志  #AMHW_LPC82X_ACMP_LADDER_VDD 或
 *                                   #AMHW_LPC82X_ACMP_LADDER_VDDCMP
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_acmp_ladref_set(amhw_lpc82x_acmp_t *p_hw_acmp, uint32_t io_flag)
{
    uint32_t lad = p_hw_acmp->lad;
    
    AM_BIT_CLR_MASK(lad, AMHW_LPC82X_ACMP_LADDER_VDDCMP);
    AM_BIT_SET_MASK(lad, io_flag);
    
    p_hw_acmp->lad = lad;
}

/**
 * \brief ACMP设置电压阶梯值
 *
 * \param[in] p_hw_acmp   : 指向ACMP寄存器块的指针
 * \param[in] value       : 电压阶梯值（0~31，基准电压 = 输入电压 * value / 31）
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_acmp_ladsel_set(amhw_lpc82x_acmp_t *p_hw_acmp, uint32_t value)
{
    uint32_t lad = p_hw_acmp->lad;

    AM_BIT_CLR_MASK(lad, AM_SBF(0x1F, 1));
    AM_BIT_SET_MASK(lad, AMHW_LPC82X_ACMP_LADDER_LADSEL(value));
    
    p_hw_acmp->lad = lad;
    
}

/**
 *@} 
 */

#ifdef __cplusplus
}
#endif

#endif /*__AMHW_LPC82X_ACMP_H */

/* end of file */

