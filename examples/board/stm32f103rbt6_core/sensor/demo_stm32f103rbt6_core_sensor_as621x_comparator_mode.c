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
 * \brief �¶ȴ����� AS621x �Ƚ�ģʽ����
 *
 * - ʵ������
 *   1. ÿ��һ�룬ͨ�����ڴ�ӡͨ��ֵ
 *
 * \par Դ����
 * \snippet demo_stm32f103rbt6_core_sensor_as621x_comparator_mode.c src_stm32f103rbt6_core_sensor_as621x_comparator_mode
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation
 * \endinternal
 */

#include "am_sensor.h"
#include "am_hwconf_sensor_as621x.h"
#include "demo_std_entries.h"
#include "demo_stm32f103rbt6_core_entries.h"

/**
 * \brief �������������
 */
void demo_stm32f103rbt6_core_sensor_as621x_comparator_mode_entry (void)
{
    am_sensor_handle_t handle = am_sensor_as621x_inst_init();

    demo_std_as621x_comparator_mode_entry(handle);
}


/* end of file */