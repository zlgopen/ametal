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
 * \brief ������ SHTC1 �����ļ�
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-22  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_shtc1.h"
#include "am_common.h"
#include "zlg116_pin.h"
#include "am_zlg116_inst_init.h"

/** \brief ������ SHTC1 �豸��Ϣʵ�� */
am_const am_local struct am_sensor_shtc1_devinfo __g_shtc1_info = {
    0x70               /*< \breif SHTC1 I2C��ַ */
};

/** \breif ������ SHTC1 �豸�ṹ�嶨�� */
am_local struct am_sensor_shtc1_dev __g_shtc1_dev;

/** \brief ������ SHTC1 �豸ʵ���� */
am_sensor_handle_t am_sensor_shtc1_inst_init (void)
{
    return am_sensor_shtc1_init(&__g_shtc1_dev,
                                &__g_shtc1_info,
                                am_zlg116_i2c1_inst_init());
}

/** \brief ������ SHTC1 ʵ�����ʼ�� */
am_err_t am_sensor_shtc1_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_shtc1_deinit(handle);
}

/* end of file */
