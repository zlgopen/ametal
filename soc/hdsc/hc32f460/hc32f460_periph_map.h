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
 * \brief HC32F460 外设映射
 *
 * \internal
 * \par Modification history
 * - 1.00 20-01-16  cds, first implementation
 * \endinternal
 */

#ifndef __AMHW_HC32F460_PERIPH_MAP_H
#define __AMHW_HC32F460_PERIPH_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hc32f460_regbase.h"


#include "hw/amhw_hc32f460_gpio.h"
#include "hw/amhw_hc32f460_uart.h"

/**
 * \addtogroup amhw_hc32f460_if_periph_map
 * \copydoc amhw_hc32f460_periph_map.h
 * @{
 */

/**
 * \name 外设定义
 * @{
 */


#define HC32F460_GPIO           ((amhw_hc32f460_gpio_t *)HC32F460_GPIO_BASE)
#define HC32F460_UART1          ((amhw_hc32f460_uart_t *)HC32F460_UART1_BASE)
#define HC32F460_UART2          ((amhw_hc32f460_uart_t *)HC32F460_UART2_BASE)
#define HC32F460_UART3          ((amhw_hc32f460_uart_t *)HC32F460_UART3_BASE)
#define HC32F460_UART4          ((amhw_hc32f460_uart_t *)HC32F460_UART4_BASE)

/** @} */




/**
 * @} amhw_hc32f460_if_periph_map
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_REG_BASE_H */

/* end of file */
