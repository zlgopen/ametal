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
 * \brief ZLG237 CAN 用户配置文件
 * \sa am_hwconf_zlg237_can.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-08-12  zc, first implementation
 * \endinternal
 */

#include "hc32f07x_periph_map.h"
#include "am_hc32f07x.h"
#include "hc32f07x_inum.h"
#include "am_gpio.h"
#include "am_hc32f07x_can.h"
#include "am_clk.h"

/**
 * \addtogroup am_if_src_hwconf_zlg237_can
 * \copydoc am_hwconf_zlg237_can.c
 * @{
 */

#define MAX     10

/** \brief CAN平台初始化 */
static void __hc32f07x_plfm_can_init (void)
{
    am_clk_enable(CLK_CAN);
 
    am_gpio_pin_cfg(PIOD_0, PIOD_0_CAN_RX | PIOD_0_INPUT_FLOAT );
    am_gpio_pin_cfg(PIOD_1, PIOD_1_CAN_TX | PIOD_1_OUT_PP);
    am_gpio_pin_cfg(PIOD_5,  PIOD_5_OUT_PP);
    am_gpio_set(PIOD_5, 0);
}

/** \brief 解除CAN平台初始化 */
static void __hc32f07x_plfm_can_deinit (void)
{
    am_clk_disable (CLK_CAN);
}

static am_hc32f07x_can_intcb_info_t    __g_can_intcb_info[MAX];

am_hc32f07x_can_init_info_t __g_can_initfo = {

    /** \brief 0 普通模式   1 存储所有*/
    AMHW_HC32F07X_CAN_RXBUF_NORMAL,

    /** \brief 时间触发通信模式  0 非触发  1 触发 */
	  0,
	
    AMHW_HC32F07X_CAN_RXBUF_NOTSTORED,

    AMHW_HC32F07X_CAN_STB_MODE_FIFO,

    10,

    15,
};

/** \brief 设备信息 */
static const am_hc32f07x_can_devinfo_t __g_can_devinfo = {

    HC32F07X_CAN_BASE,                 /**< \brief CAN */
    INUM_CAN,              /**< \brief CAN的中断编号  此处不可修改*/
    AMHW_HC32F07X_CAN_PTB,
    __g_can_intcb_info,              /**< \brief 回调信息 */
    MAX,                             /**< \brief 回调信息内存大小*/
    __hc32f07x_plfm_can_init,          /**< \brief CAN的平台初始化 */
    __hc32f07x_plfm_can_deinit,        /**< \brief CAN的平台去初始化 */
//    &__g_can_filter,
    &__g_can_initfo

};

/**< \brief 设备实例 */
static am_hc32f07x_can_dev_t  __g_can_dev;

/** \brief CAN实例初始化，获得CAN标准服务句柄 */
am_can_handle_t am_hc32f07x_can_inst_init (void)
{
    return am_hc32f07x_can_init(&__g_can_dev, &__g_can_devinfo);
}

/** \brief CAN实例解初始化 */
void am_hc32f07x_can_inst_deinit (am_can_handle_t handle)
{
    am_hc32f07x_can_deinit(handle);
}

/**
 * @}
 */

/* end of file */
