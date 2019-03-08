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
 * \brief ������ٶȴ����� LSM6DSL ���̣�ͨ����׼�ӿ�ʵ��
 *
 * - ʵ������
 *   1. ÿ��һ�룬ͨ�����ڴ�ӡ������ٶȺ��¶ȵ�ֵ
 *
 * \par Դ����
 * \snippet demo_std_lsm6dsl.c src_std_lsm6dsl
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-20  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_lsm6dsl
 * \copydoc demo_std_lsm6dsl.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_sensor.h"
#include "am_sensor_lsm6dsl.h"
#include "am_delay.h"

/**
 * \brief �������
 */
void demo_std_lsm6dsl_entry (am_sensor_handle_t handle)
{
    uint8_t i = 0;

    /* LSM6DSL�ṩ������ͨ��ID�о� */
    const int id[4] = {AM_LSM6DSL_CHAN_1, AM_LSM6DSL_CHAN_2,
                       AM_LSM6DSL_CHAN_3, AM_LSM6DSL_CHAN_4};

    /* �����ĸ�ͨ�����ݵĻ��� */
    am_sensor_val_t data[4];

    /*
     * �г��ĸ�ͨ�������ٶȺ��¶ȣ����ݵ����ֺ͵�λ�ַ��������ڴ�ӡ
     */
    const char *data_name_string[] = {"ACC_X", "ACC_Y", "ACC_Z", "temperature"};
    const char *data_unit_string[] = {"m/s^2", "m/s^2", "m/s^2", "��"};
    
    am_sensor_enable(handle, id, 4, data);

    while(1) {
        am_sensor_data_get(handle, id, 4, data);

        for (i = 0; i < 4; i++) {
            if (AM_SENSOR_VAL_IS_VALID(data[i])) { /* ��ͨ��������Ч����������ʹ�� */

                /* ��λת��Ϊ AM_SENSOR_UNIT_MICRO���Դ�ӡ��ʾ6λС�� */
                am_sensor_val_unit_convert(&data[i], 1, AM_SENSOR_UNIT_MICRO);
                am_kprintf("The %s is : %d.%06d %s.\r\n",
                              data_name_string[i],
                              (data[i].val)/1000000,
                              (uint32_t)(data[i].val)%1000000,
                              data_unit_string[i]);
            } else {       //��ͨ��������Ч�����ݻ�ȡʧ��
                am_kprintf("The %s get failed!\r\n", data_name_string[i]);
            }
        }
        am_mdelay(1000);
    }
}

/* end of file */
