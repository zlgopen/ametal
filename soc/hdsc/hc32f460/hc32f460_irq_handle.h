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
 * \brief HC32F460 中断处理函数
 *
 * \internal
 * \par Modification history
 * - 1.00 20-01-13  cds, first implementation
 * \endinternal
 */

#ifndef __HC32F460_IRQ_HANDLE_H
#define __HC32F460_IRQ_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif

void IRQ128_Handler(void *parg);
void IRQ129_Handler(void *parg);
void IRQ130_Handler(void *parg);
void IRQ131_Handler(void *parg);
void IRQ132_Handler(void *parg);
void IRQ136_Handler(void *parg);
void IRQ137_Handler(void *parg);
void IRQ138_Handler(void *parg);
void IRQ139_Handler(void *parg);
void IRQ141_Handler(void *parg);
void IRQ142_Handler(void *parg);
void IRQ143_Handler(void *parg);

typedef struct
{
    volatile uint32_t INTSEL                : 9;
    uint32_t RESERVED9                      :23;
} stc_intc_sel_field_t;

/**
 * \brief 自由中断注册函数
 *
 * \param[in] inum :    中断向量编号
 * \param[in] src_num : 中断源编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_int_sel0_31(uint8_t inum, uint16_t src_num)
{
    stc_intc_sel_field_t *stcIntSel;
    stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * inum));
    stcIntSel->INTSEL = src_num;
}

#ifdef __cplusplus
}
#endif

#endif /* __HC32F460_IRQ_HANDLE_H */

/* end of file */
