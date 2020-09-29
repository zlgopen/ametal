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
 * \brief HC32F460 CAN 用户配置文件
 * \sa am_hwconf_hc32f460_can.c
 *
 * \internal
 * \par Modification history
 * - 1.00 
 * \endinternal
 */

#include "hc32f460_periph_map.h"
#include "am_hc32f460.h"
#include "hc32f460_inum.h"
#include "am_gpio.h"
#include "am_hc32f460_can.h"
#include "am_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_adc
 * \copydoc am_hwconf_hc32f460_adc.c
 * @{
 */

#define MAX     10

/** \brief CAN平台初始化 */
static void __hc32f460_plfm_can_init (void)
{
    am_gpio_pin_cfg(PIOB_6, GPIO_AFIO(AMHW_HC32F460_AFIO_CAN1_RX));
    am_gpio_pin_cfg(PIOB_7, GPIO_AFIO(AMHW_HC32F460_AFIO_CAN1_TX));
    am_gpio_pin_cfg(PIOD_15, AM_GPIO_OUTPUT_INIT_LOW);
    
    am_clk_enable(CLK_CAN);
}

/** \brief 解除CAN平台初始化 */
static void __hc32f460_plfm_can_deinit (void)
{
    am_clk_disable (CLK_CAN);
}

static am_hc32f460_can_intcb_info_t    __g_can_intcb_info[MAX];

/** \brief 设备信息 */
static const am_hc32f460_can_devinfo_t __g_can_devinfo = {

    HC32F460_CAN_BASE,                 /**< \brief CAN */
    INUM_CAN,                          /**< \brief CAN的中断编号 */
    AMHW_HC32F460_CAN_PELI_CAN,        /**< \brief CAN模式定义 */
    __g_can_intcb_info,                /**< \brief 回调信息 */
    MAX,                               /**< \brief 回调信息内存大小*/
    __hc32f460_plfm_can_init,          /**< \brief CAN1的平台初始化 */
    __hc32f460_plfm_can_deinit,        /**< \brief CAN1的平台去初始化 */

};

/** \brief 设备实例 */
am_hc32f460_can_dev_t  __g_can_dev;

/** \brief CAN实例初始化，获得CAN标准服务句柄 */
am_can_handle_t am_hc32f460_can_inst_init (void)
{
    return am_hc32f460_can_init(&__g_can_dev, &__g_can_devinfo);
}

/** \brief CAN实例解初始化 */
void am_hc32f460_can_inst_deinit (am_can_handle_t handle)
{
    am_hc32f460_can_deinit(handle);
}

/**
 * @}
 */

/* end of file */
