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

#ifndef  __AM_HC32F07X_CTS_H
#define  __AM_HC32F07X_CTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "hw/amhw_hc32f07x_cts.h"

/**
 * \addtogroup am_hc32f07x_if_cts
 * \copydoc am_hc32f07x_cts.h
 * @{
 */

/**
 * \brief cts时钟校准
 *
 * \param[in] fclk   : 校准时钟频率
 * \param[in] fref   : 参考时钟频率
 * \param[in] clksrc : 校准时钟
 * \param[in] refsrc : 参考时钟
 *
 * \return AM_OK 校准成功
 *          <0   时钟频率错误或对应时钟未使能
 */
uint8_t am_hc32f07x_cts_calibrate( uint32_t                    fclk,
	                                 uint32_t                    fref,
	                                 amhw_hc32f07x_cts_clksrc_t clksrc,
	                                 amhw_hc32f07x_cts_refsrc_t refsrc);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F07X_CTS_H */

/* end of file */

