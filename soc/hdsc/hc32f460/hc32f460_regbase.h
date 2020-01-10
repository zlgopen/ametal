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
 * \brief HC32F460 芯片外设寄存器基址定义
 *
 * \internal
 * \par Modification history
 * - 1.00 19-012-23  cds, first implementation
 * \endinternal
 */

#ifndef __HC32F460_REGBASE_H
#define __HC32F460_REGBASE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup hc32f460_if_regbase
 * \copydoc hc32f460_regbase.h
 * @{
 */

#define HC32F460_SYSC_BASE    (0x40054000UL)  /** \brief SYSC基地址 */
#define HC32F460_MSTP_BASE    (0x40048000UL)  /** \brief MSTP基地址 */
#define HC32F460_GPIO_BASE    (0x40053800UL)  /** \brief GPIO基地址 */
#define HC32F460_EFM_BASE     (0x40010400UL)  /** \brief 嵌入式FLASH（EFM）基地址 */
#define HC32F460_INTC_BASE    (0x40051000UL)  /** \brief INTC基地址 */
#define HC32F460_UART1_BASE           (0x4001D000UL)  /** \brief UART1基地址 */
#define HC32F460_UART2_BASE           (0x4001D400UL)  /** \brief UART2基地址 */
#define HC32F460_UART3_BASE           (0x40021000UL)  /** \brief UART3基地址 */
#define HC32F460_UART4_BASE           (0x40021400UL)  /** \brief UART4基地址 */
#define HC32F460_EVENT_PORT_BASE      (0x40010800UL)  /** \brief EVENT_PORT基地址 */
#define HC32F460_SYSTICK_BASE         (0xE000E010UL)  /** \brief SYSTICK基地址 */

/**
 * \name 外设内存映射地址定义
 * @{
 */


/** @} */

/**
 * @} hc32f460_if_regbase
 */

#ifdef __cplusplus
}
#endif

#endif/* __HC32F460_REGBASE_H */

/* end of file */
