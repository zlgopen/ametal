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
 * \brief 纯GPIO驱动的动态扫描数码管驱动（位选和段码均为GPIO驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "am_digitron_scan.h"
#include "am_digitron_scan_gpio.h"
#include "am_digitron_seg_gpio.h"
#include "am_digitron_com_gpio.h"

/*******************************************************************************
  Public functions
*******************************************************************************/

int am_digitron_scan_gpio_init (am_digitron_scan_gpio_dev_t        *p_dev,
		                        const am_digitron_scan_gpio_info_t *p_info)
{
    if ((p_dev == NULL) || (p_info == NULL)) {
    	return -AM_EINVAL;
    }

    /* 段码和位码独立发送  */
    p_dev->scan_ops.p_send_seg = am_digitron_seg_gpio_init(&p_dev->seg_dev,
														   &p_info->base_info,
														    p_info->p_seg_pins);

    p_dev->scan_ops.p_sel_com  = am_digitron_com_gpio_init(&p_dev->com_dev,
														   &p_info->base_info,
														    p_info->p_com_pins);

    return am_digitron_scan_init(&p_dev->scan_dev,
    		                     &p_info->scan_info,
								 &p_info->base_info,
								 AM_TRUE,
								 &p_dev->scan_ops);
}

/******************************************************************************/
int am_digitron_scan_gpio_cb_set (am_digitron_scan_gpio_dev_t *p_dev,
		                          am_digitron_scan_cb_t        pfn_cb,
							      void                        *p_arg)
{
    if (p_dev == NULL) {
    	return -AM_EINVAL;
    }
    return am_digitron_scan_cb_set(&p_dev->scan_dev, pfn_cb, p_arg);
}

/******************************************************************************/
int am_digitron_scan_gpio_deinit (am_digitron_scan_gpio_dev_t *p_dev)
{
    if (p_dev == NULL) {
    	return -AM_EINVAL;
    }

	/* 删除标准的数码管设备  */
	return am_digitron_scan_deinit(&p_dev->scan_dev);
}

/* end of file */
