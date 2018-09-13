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
 * \brief components 所有例程函数入口声明
 * \sa    demo_components_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-07-30  pea, first implementation
 * \endinternal
 */

#ifndef __DEMO_COMPONENTS_ENTRIES_H
#define __DEMO_COMPONENTS_ENTRIES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_ftl.h"
#include "am_mtd.h"
#include "am_mx25xx.h"
#include "am_is25xx.h"    
#include "am_ep24cxx.h"
#include "am_fm175xx.h"

/**
 * \brief EP24CXX 器件例程
 *
 * \param[in] ep24cxx_handle EP24CXX 标准服务句柄
 * \param[in] test_lenth     测试字节数
 *
 * \return 无
 */
void demo_ep24cxx_entry (am_ep24cxx_handle_t ep24cxx_handle, int32_t test_lenth);

/**
 * \brief  MX25XX器件 例程
 *
 * \param[in] mx25xx_handle  MX25XX 标准服务句柄
 * \param[in] test_lenth     测试字节数
 *
 * \return 无
 */
void demo_mx25xx_entry (am_mx25xx_handle_t mx25xx_handle, int32_t test_lenth);

/**
 * \brief  IS25XX器件 例程
 *
 * \param[in] is25xx_handle  IS25XX 标准服务句柄
 * \param[in] test_lenth     测试字节数
 *
 * \return 无
 */
void demo_is25xx_entry (am_is25xx_handle_t is25xx_handle, int32_t test_lenth);

/**
 * \brief FTL 例程
 *
 * \param[in] ftl_handle     FTL 标准服务句柄
 * \param[in] test_lenth     测试字节数
 *
 * \return 无
 */
void demo_ftl_entry (am_ftl_handle_t ftl_handle, int32_t test_lenth);

/**
 * \brief MTD 例程
 *
 * \param[in] mtd_handle  MTD 标准服务句柄
 * \param[in] test_lenth  测试字节数
 *
 * \return 无
 */
void demo_mtd_entry (am_mtd_handle_t mtd_handle, int32_t test_lenth);

/**
 * \brief FM175XX 例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_picca_read_id (am_fm175xx_handle_t handle);

/**
 * \brief FM175XX LPCD模式例程
 * \param[in] handle  FM175XX 服务句柄
 * \return 无
 */
void demo_fm175xx_picca_lpcd_mode (am_fm175xx_handle_t handle);
#ifdef __cplusplus
}
#endif

#endif /* __DEMO_COMPONENTS_ENTRIES_H */

/* end of file */
