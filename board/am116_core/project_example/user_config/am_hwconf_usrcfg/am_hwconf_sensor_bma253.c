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
 * \brief ������ BMA253 �����ļ�
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-22  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_bma253.h"
#include "am_common.h"
#include "zlg116_pin.h"
#include "am_zlg116_inst_init.h"

/** \brief ������ BMA253 �豸��Ϣʵ�� */
am_const am_local struct am_sensor_bma253_devinfo __g_bma253_info = {
    PIOB_0,            /*< \brief �������Ŷ���    */
    0x18               /*< \breif BMA253 I2C��ַ */
};

/** \breif ������ BMA253 �豸�ṹ�嶨�� */
am_local struct am_sensor_bma253_dev __g_bma253_dev;

/** \brief ������ BMA253 �豸ʵ���� */
am_sensor_handle_t am_sensor_bma253_inst_init (void)
{
    return am_sensor_bma253_init(&__g_bma253_dev,
                                 &__g_bma253_info,
                                 am_zlg116_i2c1_inst_init());
}

/** \brief ������ BMA253 ʵ�����ʼ�� */
am_err_t am_sensor_bma253_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_bma253_deinit(handle);
}

/* end of file */
