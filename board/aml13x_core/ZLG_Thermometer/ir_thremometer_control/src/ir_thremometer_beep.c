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
 * \brief ·äÃùÆ÷Çý¶¯
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-10  yjx, first implementation
 * \endinternal
 */
#include "am_gpio.h"
#include "hc32x3x_pin.h"
#include "ir_thremometer_beep.h"
#include "ametal.h"
#include "am_hc32l13x_inst_init.h"

static am_hc32_pca_handle_t pca_handle = 0;
/** \brief ·äÃùÆ÷³õÊ¼»¯*/
void ir_thremometer_beep_init(void)
{
	pca_handle = am_hc32_pca1_inst_init();
	am_hc32_pca_pwm_set(pca_handle, 2, 2, HC32_PCA_PWM_16, 528, 264);
	am_hc32_pca_stop(pca_handle);
	ir_thremometer_beep_on(1, BEEP_SHORT_NOISE);
}
/** \brief ·äÃùÆ÷Ãù½Ðº¯Êý*/
void ir_thremometer_beep_on(uint32_t times, beep_nosie_t nosie)
{
	uint32_t time_ms = 0;
	if (nosie == BEEP_LONG_NOISE) {
		time_ms = 500;
	} else {
		time_ms = 100;
	}
	for(int i = 0; i < times; i++) {
		am_hc32_pca_start(pca_handle);
		am_mdelay(time_ms);
		am_hc32_pca_stop(pca_handle);
		am_mdelay(time_ms);
	}
}
/* end of file */
