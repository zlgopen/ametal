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
 * \brief 传感器 AS7262 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_as7262.h"

/**
 * \brief 传感器 AS7262 控制函数
 */
am_err_t am_sensor_as7262_ioctl (am_sensor_handle_t  handle,
                                 int                 cmd,
                                 void               *p_arg)
{
    am_sensor_as7262_dev_t   *p_dev = (am_sensor_as7262_dev_t *)handle->p_drv;

    am_err_t ret = AM_OK;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    ret = p_dev->pfn_ioctl(handle->p_drv, cmd, p_arg);

    return ret;
}

/* end of file */

