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
 * \brief �⴫���� BH1730FVC ���̣�ͨ������ģʽʵ��
 *
 * - ʵ������
 *   1. �������ݸ������ʣ����ж��л�ȡ���ݣ���ͨ�����ڴ�ӡ
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-20  yrz, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_bh1730fvc.h"
#include "am_delay.h"

/** \brief BH1730FVC��������ID */
const static int __bh1730fvc_id[1] = {AM_BH1730FVC_CHAN_1};

/** \breif BH1730FVC���������� */
static am_sensor_val_t __bh1730fvc_data[1];

/**
 * \brief ͨ��1����ǿ�Ļص�����
 */
static void __pfn_light (void *p_arg, uint32_t trigger_src)
{
    am_sensor_handle_t handle = (am_sensor_handle_t)p_arg;

    if (trigger_src & AM_SENSOR_TRIGGER_DATA_READY) {
        am_sensor_data_get(handle, &__bh1730fvc_id[0], 1, &__bh1730fvc_data[0]);
        am_sensor_val_unit_convert(&__bh1730fvc_data[0], 1, AM_SENSOR_UNIT_MICRO);
        am_kprintf("The channel %d,type is light, value is: %d.%06d Lux!\r\n",
                   __bh1730fvc_id[0],
                   (__bh1730fvc_data[0].val)/1000000,
                   (__bh1730fvc_data[0].val)%1000000);
    }
}

/**
 * \brief �������
 */
void demo_std_bh1730fvc_int_entry (am_sensor_handle_t handle)
{
    /* ʹ����·ͨ�� */
    am_sensor_enable(handle, __bh1730fvc_id, 1, __bh1730fvc_data);

    /* ����ͨ��0�Ĵ����ص����� */
    am_sensor_trigger_cfg(handle,
                          __bh1730fvc_id[0],
                          AM_SENSOR_TRIGGER_DATA_READY,
                          __pfn_light,
                          (void*)handle);

    /* ��ͨ��0�Ĵ���ģʽ */
    am_sensor_trigger_on(handle, __bh1730fvc_id[0]);
}

/* end of file */
