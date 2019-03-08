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
 * \brief ��ʪ�ȴ����� SHTC1 ���̣�ͨ����׼�ӿ�ʵ��
 *
 * - ʵ������
 *   1. ÿ��һ�룬ͨ�����ڴ�ӡʪ�Ⱥ��¶ȵ�ֵ
 *
 * \par Դ����
 * \snippet demo_std_shtc1.c src_std_shtc1
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-20  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_shtc1
 * \copydoc demo_std_shtc1.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_shtc1.h"
#include "am_delay.h"

/**
 * \brief �������
 */
void demo_std_shtc1_entry (am_sensor_handle_t handle)
{
    /* SHTC1�ṩ������ͨ��ID�о� */
    const int       id[2] = {AM_SHTC1_CHAN_1, AM_SHTC1_CHAN_2};

    /* ��������ͨ�����ݵĻ��� */
    am_sensor_val_t data[2];

    int             i;

    /*
     * �г�����ͨ����ʪ�Ⱥ��¶ȣ����ݵ����ֺ͵�λ�ַ��������ڴ�ӡ
     */
    const char *data_name_string[] = {"humidity", "temperature"};
    const char *data_unit_string[] = {"%rH", "��"};

    am_sensor_enable(handle, id, 2, data);

    while(1) {
        am_sensor_data_get(handle, id, 2, data);
        for(i=0; i<2; i++) {
            if (AM_SENSOR_VAL_IS_VALID(data[i])) { /* ��ͨ��������Ч����������ʹ�� */
                /* ��λת��Ϊ AM_SENSOR_UNIT_MICRO���Դ�ӡ��ʾ6λС�� */
                am_sensor_val_unit_convert(&data[i], 1, AM_SENSOR_UNIT_MICRO);
                am_kprintf("The %s is : %d.%06d %s.\r\n", data_name_string[i],
                                                          (data[i].val)/1000000,
                                                          (data[i].val)%1000000,
                                                          data_unit_string[i]);
            } else {       //��ͨ��������Ч�����ݻ�ȡʧ��
                am_kprintf("The %s get failed!\r\n", data_name_string[i]);
            }
        }
        am_mdelay(1000);
    }
}

/* end of file */
