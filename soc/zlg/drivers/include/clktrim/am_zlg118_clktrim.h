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
 * \brief CLKTRIM标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-29
 * \endinternal
 */

#ifndef  __AM_ZLG118_CLKTRIM_H
#define  __AM_ZLG118_CLKTRIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "hw/amhw_zlg118_clktrim.h"

/**
 * \addtogroup am_zlg118_if_clktrim
 * \copydoc am_zlg118_clktrim.h
 * @{
 */

/**
 * \brief CLKTRIM监测模式配置
 *
 * \param[in] rcntval    : 参考计数器初值
 * \param[in] rcntval    : 校准计数器溢出值
 * \param[in] refclk_sel : 参考时钟选择
 * \param[in] calclk_sel : 待监测时钟选择
 *
 * \return <0，被监控时钟失效
 *        AM_OK，工作正常
 */
uint8_t am_zlg118_clktrim_monitor (uint16_t                     rcntval,
                                   uint16_t                     ccntval,
                                   amhw_clktrim_refclk_sel_t    refclk_sel,
                                   amhw_clktrim_calclk_sel_t    calclk_sel);

/**
 * \brief CLKTRIM校准模式配置
 *
 * \param[in] rcntval    : 参考计数器初值
 * \param[in] refclk_sel : 参考时钟选择
 * \param[in] calclk_sel : 待校准时钟选择
 *
 * \return <0，需调整校准时间
 *         >0，校准时钟计数器的值 / 参考时钟计数器的值 * 1000
 */
uint8_t am_zlg118_clktrim_calibrate (uint16_t                     rcntval,
                                     amhw_clktrim_refclk_sel_t    refclk_sel,
                                     amhw_clktrim_calclk_sel_t    calclk_sel);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG118_CLKTRIM_H */

/* end of file */
