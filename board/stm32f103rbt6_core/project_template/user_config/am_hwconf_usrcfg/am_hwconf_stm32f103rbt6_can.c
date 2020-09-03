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
 * \brief STM32F103RBT6 CAN 用户配置文件
 * \sa am_hwconf_stm32f103rbt6_can.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-08-12  zc, first implementation
 * \endinternal
 */

#include "stm32f103rbt6_periph_map.h"
#include "am_stm32f103rbt6.h"
#include "stm32f103rbt6_inum.h"
#include "am_gpio.h"
#include "am_stm32f103rbt6_can.h"
#include "am_clk.h"

/**
 * \addtogroup am_if_src_hwconf_stm32f103rbt6_can
 * \copydoc am_hwconf_stm32f103rbt6_can.c
 * @{
 */

#define MAX     10

/** \brief CAN平台初始化 */
static void __stm32f103rbt6_plfm_can_init (void)
{
    am_clk_enable(CLK_CAN);

    am_gpio_pin_cfg(PIOA_11, PIOA_11_CAN_RX_REMAP0 | PIOA_11_INPUT_PU | PIOA_11_SPEED_50MHz);
    am_gpio_pin_cfg(PIOA_12, PIOA_12_CAN_TX_REMAP0 | PIOA_12_AF_PP |PIOA_12_SPEED_50MHz );


}

/** \brief 解除CAN平台初始化 */
static void __stm32f103rbt6_plfm_can_deinit (void)
{
    am_clk_disable (CLK_CAN);
}

static am_stm32f103rbt6_can_intcb_info_t    __g_can_intcb_info[MAX];

amhw_stm32f103rbt6_can_filter_t __g_can_filter = {

    /* 关联FIFO */
    AMHW_STM32F103RBT6_CAN_FILTER_FIFO0,

    /* 滤波器模式 */
    AMHW_STM32F103RBT6_CAN_FILTER_MODE_IDMASK,

    /* 滤波器位宽 */
    AMHW_STM32F103RBT6_CAN_FILTER_SCALE_32BIT,

    /* 滤波器激活状态 */
    1
};

am_stm32f103rbt6_can_init_info_t __g_can_initfo = {

    /** \brief 0 普通模式   1 回环模式 */
    AMHW_STM32F103RBT6_CAN_MODE_NORMAL,

    /** \brief 时间触发通信模式  0 非触发  1 触发 */
    0,

    /** \brief 软件自动离线管理
     *   0 软件对MCR的INRQ位进行置‘1’ 后 清‘0’ 一旦硬件检测到128次11位连续隐性位则退出离线模式
     *   1 一旦硬件检测到128次11位连续隐性位则退出离线模式
     */
    1,

    /** \brief 自动唤醒模式
     *   0 睡眠模式通过清除CAN_MCR的SLEEP位  由软件唤醒
     *   1 睡眠模式通过检测CAN报文，由硬件自动唤醒。唤醒的同时，硬件自动对CAN_MSR 的SLEEP和SLAK清‘0’
     */
    1,

    /** \brief 禁止报文自动重传
     *   0 硬件在发送报文失败时会一直重传到发送成功
     *   1 CAN报文只发送一次
     */
    0,

    /** \brief 接收FIFO锁定模式
     *   0 在接收溢出时FIFO未被锁定，当接收FIFO的报文未被读出，下一个收到的报文会覆盖原有的报文
     *   1 在接受溢出时FIFO被锁定，当接收FIFO的报文未被读出，下一个收到的报文会被丢弃
     */
    0,

    /** \brief 发送FIFO优先级
     *   0 优先级由报文的标识符来决定
     *   1 优先级由发送请求的顺序来决定
     */
    0
};

/** \brief 设备信息 */
static const am_stm32f103rbt6_can_devinfo_t __g_can_devinfo = {

    STM32F103RBT6_CAN_BASE,                 /**< \brief CAN */
    INUM_USB_HP_CAN_TX,              /**< \brief CAN的中断编号  此处不可修改*/
    __g_can_intcb_info,              /**< \brief 回调信息 */
    MAX,                             /**< \brief 回调信息内存大小*/
    __stm32f103rbt6_plfm_can_init,          /**< \brief CAN1的平台初始化 */
    __stm32f103rbt6_plfm_can_deinit,        /**< \brief CAN1的平台去初始化 */
    &__g_can_filter,
    &__g_can_initfo

};

/**< \brief 设备实例 */
static am_stm32f103rbt6_can_dev_t  __g_can_dev;

/** \brief CAN实例初始化，获得CAN标准服务句柄 */
am_can_handle_t am_stm32f103rbt6_can_inst_init (void)
{
    return am_stm32f103rbt6_can_init(&__g_can_dev, &__g_can_devinfo);
}

/** \brief CAN实例解初始化 */
void am_stm32f103rbt6_can_inst_deinit (am_can_handle_t handle)
{
    am_stm32f103rbt6_can_deinit(handle);
}

/**
 * @}
 */

/* end of file */
