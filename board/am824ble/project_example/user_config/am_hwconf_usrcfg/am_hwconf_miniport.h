/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief MiniPort 用户配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 18-01-24  pea, first implementation
 * \endinternal
 */

#ifndef __AM_HWCONF_MINIPORT_H
#define __AM_HWCONF_MINIPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_hc595.h"

/*******************************************************************************
  MiniPort-595 实例初始化
*******************************************************************************/

/**
 * \brief MiniPort-595 实例初始化
 *
 * param 无
 *
 * \return HC595 标准服务句柄，若为 NULL，表明初始化失败
 */
am_hc595_handle_t am_miniport_595_inst_init (void);

/*******************************************************************************
  MiniPort-KEY 实例初始化
*******************************************************************************/

/**
 * \brief MiniPort-KEY 实例初始化（单独使用）
 *
 * param 无
 *
 * \retval AW_OK 初始化成功
 * \retval  < 0  初始化失败，请检查 am_hwconf_miniport_key.c 文件中的配置是否正确
 */
int am_miniport_key_inst_init (void);

/*******************************************************************************
  MiniPort-KEY 实例初始化
*******************************************************************************/

/**
 * \brief MiniPort-LED 实例初始化（单独使用）
 *
 * param 无
 *
 * \retval AW_OK 初始化成功
 * \retval  < 0  初始化失败，请检查 am_hwconf_miniport_led.c 文件中的配置是否正确
 */
int am_miniport_led_inst_init (void);

/**
 * \brief MiniPort-595 + MiniPort-LED 实例初始化
 *
 * param 无
 *
 * \retval AW_OK 初始化成功
 * \retval  < 0  初始化失败，请检查 am_hwconf_miniport_led.c 文件中的配置是否正确
 */
int am_miniport_led_595_inst_init (void);

/*******************************************************************************
  MiniPort-View KEY 实例初始化
*******************************************************************************/

/**
 * \brief MiniPort-KEY + MiniPort-View 实例初始化
 *
 * param 无
 *
 * \retval AW_OK 初始化成功
 * \retval  < 0  初始化失败，请检查 am_hwconf_miniport_view_key.c 文件中的配置
 *               是否正确
 */
int am_miniport_view_key_inst_init (void);

/**
 * \brief MiniPort-595 + MiniPort-KEY + MiniPort-View 实例初始化
 *
 * param 无
 *
 * \retval AW_OK 初始化成功
 * \retval  < 0  初始化失败，请检查 am_hwconf_miniport_view_key.c 文件中的配置
 *               是否正确
 */
int am_miniport_view_key_595_inst_init (void);

/*******************************************************************************
  MiniPort-View 实例初始化
*******************************************************************************/

/**
 * \brief MiniPort-View 实例初始化（单独使用）
 *
 * param 无
 *
 * \retval AW_OK 初始化成功
 * \retval  < 0  初始化失败，请检查 am_hwconf_miniport_view.c 文件中的配置是否正确
 */
int am_miniport_view_inst_init (void);

/**
 * \brief MiniPort-595 + MiniPort-View 实例初始化
 *
 * param 无
 *
 * \retval AW_OK 初始化成功
 * \retval  < 0  初始化失败，请检查 am_hwconf_miniport_view.c 文件中的配置是否正确
 */
int am_miniport_view_595_inst_init (void);

/*******************************************************************************
  MiniPort-ZLG72128 实例初始化
*******************************************************************************/

/**
 * \brief MiniPort-ZLG72128 实例初始化
 *
 * param 无
 *
 * \retval AW_OK 初始化成功
 * \retval  < 0  初始化失败，请检查 am_hwconf_miniport_zlg72128.c 文件中的配置
 *               是否正确
 */
int am_miniport_zlg72128_inst_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_HWCONF_MINIPORT_H */

/* end of file */
