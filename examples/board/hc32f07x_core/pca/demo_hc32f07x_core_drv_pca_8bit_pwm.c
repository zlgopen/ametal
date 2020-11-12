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
 * \brief PCA 8bit PWM例程，通过标准接口和中间驱动层函数实现
 *
 * - 实验现象：
 *   1. PCA通道0，PB4引脚输出PWM，占空比为20%。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f07x_drv_pca_8bit_pwm.c src_hc32f07x_drv_pca_8bit_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 19-11-22  RZ, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f07x_drv_pca_8bit_pwm
 * \copydoc demo_hc32f07x_drv_pca_8bit_pwm.c
 */

/** [src_hc32f07x_drv_pca_8bit_pwm] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_hc32_pca.h"
#include "am_hc32f07x_inst_init.h"
#include "demo_amf07x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f07x_core_drv_pca_8bit_pwm_entry (void)
{
    am_hc32_pca_handle_t handle;

    AM_DBG_INFO("demo amf07x_core pca 8bit pwm entry!\r\n");

    handle = am_hc32_pca_inst_init();

    am_hc32_pca_pwm_set(handle, 0, 0, HC32_PCA_PWM_8, 0, 50);

    am_hc32_pca_start(handle);

    while(1) {

    }
}
/** [src_hc32f07x_drv_pca_8bit_pwm] */

/* end of file */
