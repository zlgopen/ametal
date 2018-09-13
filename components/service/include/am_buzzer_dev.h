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
 * \brief 蜂鸣器设备管理
 *
 * \internal
 * \par Modification history
 * - 1.01 17-07-17  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_BUZZER_DEV_H
#define __AM_BUZZER_DEV_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_common.h"
#include "am_softimer.h"
    
/**
 * \addtogroup am_if_buzzer_dev
 * \copydoc am_buzzer_dev.h
 * @{
 */

/**
 * \brief 蜂鸣器驱动函数
 */
typedef struct am_buzzer_drv_funcs {

    /* 打开或关闭蜂鸣器 */
    int (*pfn_buzzer_set) (void *p_cookie, am_bool_t on);

} am_buzzer_drv_funcs_t;

/**
 *  \brief 蜂鸣器设备
 */
typedef struct am_buzzer_dev {
    const am_buzzer_drv_funcs_t     *p_funcs;   /**< \brief 驱动函数      */
    void                            *p_cookie;  /**< \brief 驱动函数参数  */
    am_softimer_t                    timer;     /**< \brief 软件定时器  */
} am_buzzer_dev_t;

/**
 * \brief 注册蜂鸣器设备，当前仅支持单个蜂鸣器设备
 * \param[in] p_dev ： 注册的设备
 * \return AM_OK : 注册成功
 */
int am_buzzer_dev_register (am_buzzer_dev_t             *p_dev,
                            const am_buzzer_drv_funcs_t *p_funcs,
                            void                        *p_cookie);

/** 
 * @}  
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_BUZZER_DEV_H */

/* end of file */
