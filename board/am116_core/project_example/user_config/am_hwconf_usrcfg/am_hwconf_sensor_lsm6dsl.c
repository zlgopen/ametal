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
 * \brief ������ LSM6DSL �����ļ�
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-22  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_lsm6dsl.h"
#include "am_common.h"
#include "zlg116_pin.h"
#include "am_zlg116_inst_init.h"

/** \brief ������ LSM6DSL �豸��Ϣʵ�� */
am_const am_local struct am_sensor_lsm6dsl_devinfo __g_lsm6dsl_info = {
    PIOB_0,            /*< \brief �������Ŷ���    */
    0x6B               /*< \breif LSM6DSL I2C��ַ */
};

/** \breif ������ LSM6DSL �豸�ṹ�嶨�� */
am_local struct am_sensor_lsm6dsl_dev __g_lsm6dsl_dev;

/** \brief ������ LSM6DSL �豸ʵ���� */
am_sensor_handle_t am_sensor_lsm6dsl_inst_init (void)
{
    return am_sensor_lsm6dsl_init(&__g_lsm6dsl_dev,
                                  &__g_lsm6dsl_info,
                                  am_zlg116_i2c1_inst_init());
}

/** \brief ������ LSM6DSL ʵ�����ʼ�� */
am_err_t am_sensor_lsm6dsl_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_lsm6dsl_deinit(handle);
}

/* end of file */
