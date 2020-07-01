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
 * \brief INTC Access Layer
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-16  cds, first implementation
 *
 * \endinternal
 */

#ifndef __HC32F460_INTCTRL_H
#define __HC32F460_INTCTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "hc32f460_regbase.h"

typedef struct amhw_intctrl_typedef {
        __IO uint32_t NMICR;
        __IO uint32_t NMIENR;
        __IO uint32_t NMIFR;
        __IO uint32_t NMICFR;
        __IO uint32_t EIRQCR[16];
        __IO uint32_t WUPEN;
        __IO uint32_t EIFR;
        __IO uint32_t EICFR;
        __IO uint32_t SEL0;
        __IO uint32_t SEL1;
        __IO uint32_t SEL2;
        __IO uint32_t SEL3;
        __IO uint32_t SEL4;
        __IO uint32_t SEL5;
        __IO uint32_t SEL6;
        __IO uint32_t SEL7;
        __IO uint32_t SEL8;
        __IO uint32_t SEL9;
        __IO uint32_t SEL10;
        __IO uint32_t SEL11;
        __IO uint32_t SEL12;
        __IO uint32_t SEL13;
        __IO uint32_t SEL14;
        __IO uint32_t SEL15;
        __IO uint32_t SEL16;
        __IO uint32_t SEL17;
        __IO uint32_t SEL18;
        __IO uint32_t SEL19;
        __IO uint32_t SEL20;
        __IO uint32_t SEL21;
        __IO uint32_t SEL22;
        __IO uint32_t SEL23;
        __IO uint32_t SEL24;
        __IO uint32_t SEL25;
        __IO uint32_t SEL26;
        __IO uint32_t SEL27;
        __IO uint32_t SEL28;
        __IO uint32_t SEL29;
        __IO uint32_t SEL30;
        __IO uint32_t SEL31;
        __IO uint32_t SEL32;
        __IO uint32_t SEL33;
        __IO uint32_t SEL34;
        __IO uint32_t SEL35;
        __IO uint32_t SEL36;
        __IO uint32_t SEL37;
        __IO uint32_t SEL38;
        __IO uint32_t SEL39;
        __IO uint32_t SEL40;
        __IO uint32_t SEL41;
        __IO uint32_t SEL42;
        __IO uint32_t SEL43;
        __IO uint32_t SEL44;
        __IO uint32_t SEL45;
        __IO uint32_t SEL46;
        __IO uint32_t SEL47;
        __IO uint32_t SEL48;
        __IO uint32_t SEL49;
        __IO uint32_t SEL50;
        __IO uint32_t SEL51;
        __IO uint32_t SEL52;
        __IO uint32_t SEL53;
        __IO uint32_t SEL54;
        __IO uint32_t SEL55;
        __IO uint32_t SEL56;
        __IO uint32_t SEL57;
        __IO uint32_t SEL58;
        __IO uint32_t SEL59;
        __IO uint32_t SEL60;
        __IO uint32_t SEL61;
        __IO uint32_t SEL62;
        __IO uint32_t SEL63;
        __IO uint32_t SEL64;
        __IO uint32_t SEL65;
        __IO uint32_t SEL66;
        __IO uint32_t SEL67;
        __IO uint32_t SEL68;
        __IO uint32_t SEL69;
        __IO uint32_t SEL70;
        __IO uint32_t SEL71;
        __IO uint32_t SEL72;
        __IO uint32_t SEL73;
        __IO uint32_t SEL74;
        __IO uint32_t SEL75;
        __IO uint32_t SEL76;
        __IO uint32_t SEL77;
        __IO uint32_t SEL78;
        __IO uint32_t SEL79;
        __IO uint32_t SEL80;
        __IO uint32_t SEL81;
        __IO uint32_t SEL82;
        __IO uint32_t SEL83;
        __IO uint32_t SEL84;
        __IO uint32_t SEL85;
        __IO uint32_t SEL86;
        __IO uint32_t SEL87;
        __IO uint32_t SEL88;
        __IO uint32_t SEL89;
        __IO uint32_t SEL90;
        __IO uint32_t SEL91;
        __IO uint32_t SEL92;
        __IO uint32_t SEL93;
        __IO uint32_t SEL94;
        __IO uint32_t SEL95;
        __IO uint32_t SEL96;
        __IO uint32_t SEL97;
        __IO uint32_t SEL98;
        __IO uint32_t SEL99;
        __IO uint32_t SEL100;
        __IO uint32_t SEL101;
        __IO uint32_t SEL102;
        __IO uint32_t SEL103;
        __IO uint32_t SEL104;
        __IO uint32_t SEL105;
        __IO uint32_t SEL106;
        __IO uint32_t SEL107;
        __IO uint32_t SEL108;
        __IO uint32_t SEL109;
        __IO uint32_t SEL110;
        __IO uint32_t SEL111;
        __IO uint32_t SEL112;
        __IO uint32_t SEL113;
        __IO uint32_t SEL114;
        __IO uint32_t SEL115;
        __IO uint32_t SEL116;
        __IO uint32_t SEL117;
        __IO uint32_t SEL118;
        __IO uint32_t SEL119;
        __IO uint32_t SEL120;
        __IO uint32_t SEL121;
        __IO uint32_t SEL122;
        __IO uint32_t SEL123;
        __IO uint32_t SEL124;
        __IO uint32_t SEL125;
        __IO uint32_t SEL126;
        __IO uint32_t SEL127;
        __IO uint32_t VSSEL[16];
        __IO uint32_t SWIER;
        __IO uint32_t EVTER;
        __IO uint32_t IER;
} amhw_intctrl_typedef_t;

#define HC32F460_INTC   (*(amhw_intctrl_typedef_t *)HC32F460_INTC_BASE)


typedef enum amhw_hc32f460_intc_pin_ext_int_mode {
    AMHW_HC32F460_INTC_PIN_EXT_INT_FALL     = 0,
    AMHW_HC32F460_INTC_PIN_EXT_INT_RISE     = 1,
    AMHW_HC32F460_INTC_PIN_EXT_INT_BOTHEDGE = 2,
    AMHW_HC32F460_INTC_PIN_EXT_INT_LOW      = 3,
}amhw_hc32f460_intc_pin_ext_int_mode_t;

/**
 * \brief 外部中断触发模式设置
 *
 * \param[in] pin_pos : 引脚外部中断编号（0-15）
 * \param[in] mode    : 触发模式（amhw_hc32f460_intc_pin_ext_int_mode_t）
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_intc_pin_ext_int_trigger_cfg (int pin_pos, int mode)
{
    HC32F460_INTC.EIRQCR[pin_pos] &= ~(1 << 7);    /* 关闭数字滤波 */
    HC32F460_INTC.EIRQCR[pin_pos] &= ~(0x3);       /* 清除模式设置位 */
    HC32F460_INTC.EIRQCR[pin_pos] |= mode;         /* 设置触发模式 */
    HC32F460_INTC.EIRQCR[pin_pos] |= (1 << 7);     /* 开启滤波 */
}

/**
 * \brief 设置外部中断为下降沿触发
 *
 * \param[in] pin_pos : 引脚外部中断编号（0-15）
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_intc_pin_falling_int_enable (int pin_pos)
{
    HC32F460_INTC.EIRQCR[pin_pos] &= ~(1 << 7);    /* 关闭数字滤波 */
    HC32F460_INTC.EIRQCR[pin_pos] &= ~(0x3);       /* 清除模式设置位 */
    HC32F460_INTC.EIRQCR[pin_pos] |= AMHW_HC32F460_INTC_PIN_EXT_INT_FALL; /* 设置触发模式 */
    HC32F460_INTC.EIRQCR[pin_pos] |= (1 << 7);     /* 开启滤波 */
}

/**
 * \brief 设置外部中断为上升沿触发
 *
 * \param[in] pin_pos : 引脚外部中断编号（0-15）
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_intc_pin_rising_int_enable (int pin_pos)
{
    HC32F460_INTC.EIRQCR[pin_pos] &= ~(1 << 7);    /* 关闭数字滤波 */
    HC32F460_INTC.EIRQCR[pin_pos] &= ~(0x3);       /* 清除模式设置位 */
    HC32F460_INTC.EIRQCR[pin_pos] |= AMHW_HC32F460_INTC_PIN_EXT_INT_RISE; /* 设置触发模式 */
    HC32F460_INTC.EIRQCR[pin_pos] |= (1 << 7);     /* 开启滤波 */
}

/**
 * \brief 设置外部中断为双边沿触发
 *
 * \param[in] pin_pos : 引脚外部中断编号（0-15）
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_intc_pin_double_edge_int_enable (int pin_pos)
{
    HC32F460_INTC.EIRQCR[pin_pos] &= ~(1 << 7);    /* 关闭数字滤波 */
    HC32F460_INTC.EIRQCR[pin_pos] &= ~(0x3);       /* 清除模式设置位 */
    HC32F460_INTC.EIRQCR[pin_pos] |= AMHW_HC32F460_INTC_PIN_EXT_INT_BOTHEDGE; /* 设置触发模式 */
    HC32F460_INTC.EIRQCR[pin_pos] |= (1 << 7);     /* 开启滤波 */
}

/**
 * \brief 设置外部中断为低电平触发
 *
 * \param[in] pin_pos : 引脚外部中断编号（0-15）
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_intc_pin_low_int_enable (int pin_pos)
{
    HC32F460_INTC.EIRQCR[pin_pos] &= ~(1 << 7);    /* 关闭数字滤波 */
    HC32F460_INTC.EIRQCR[pin_pos] &= ~(0x3);       /* 清除模式设置位 */
    HC32F460_INTC.EIRQCR[pin_pos] |= AMHW_HC32F460_INTC_PIN_EXT_INT_LOW; /* 设置触发模式 */
    HC32F460_INTC.EIRQCR[pin_pos] |= (1 << 7);     /* 开启滤波 */
}

/**
 * \brief 使能外设共享中断位
 *
 * \param[in] inum : 中断编号
 * \param[in] bit  : 要使能的位
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_intc_int_vssel_bit_set (int inum, int bit)
{
    HC32F460_INTC.VSSEL[inum - 128] |= (1 << bit);
}

/**
 * \brief 禁能外设共享中断位
 *
 * \param[in] inum : 中断编号
 * \param[in] bit  : 要禁能的位
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_intc_int_vssel_bit_clr (int inum, int bit)
{
    HC32F460_INTC.VSSEL[inum - 128] &= ~(1 << bit);
}

/**
 * \brief 使能外设共享中断位
 *
 * \param[in] inum  : 中断编号
 * \param[in] mask  : 要使能的位的掩码
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_intc_int_vssel_bits_set (int inum, uint32_t mask)
{
    HC32F460_INTC.VSSEL[inum - 128] |= mask;
}

/**
 * \brief 禁能外设共享中断位
 *
 * \param[in] inum  : 中断编号
 * \param[in] mask  : 要禁能的位的掩码
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_intc_int_vssel_bits_clr (int inum, uint32_t mask)
{
    HC32F460_INTC.VSSEL[inum - 128] &= ~mask;
}

/**
 * \brief 获取中断共享中断位使能状态
 *
 * \param[in] inum  : 中断编号
 *
 * \return 该中断号对应的共享中断使能状况
 */
am_static_inline
uint32_t amhw_hc32f460_intc_int_vssel_get (int inum)
{
    return HC32F460_INTC.VSSEL[inum - 128];
}

#ifdef __cplusplus
}
#endif

#endif /* __HC32F460_INTCTRL_H */

/* end of file */
