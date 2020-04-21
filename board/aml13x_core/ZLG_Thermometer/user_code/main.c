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
 * \brief HC32L13x 模板工程
 *
 * - 实验现象:
 *   模板例程:本demo为测温枪专用demo  ，必须使用我司配套的硬件，包含测温枪的逻辑功能，可适用于多种传感器。传感器切换直接切换VT和RT表参数即可，
 *						所给示例中包含多种传感器参数，已有传感器可以自由切换使用，未有传感器需自己添加。VT和RT表的参数存放于
 *					\board\aml13x_core\project_template\infrared_sensor_interpolation\include
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */

/**
 * \brief 例程入口
 */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "hc32x3x_pin.h"
#include "hc32_periph_map.h"
#include "ir_thremometer.h"

int am_main (void)
{
    AM_DBG_INFO("Start up successful!\r\n");
	
	ir_thremometer_init();
	
	ir_thremometer_run();
	
	return 0;

}

/* end of file */
