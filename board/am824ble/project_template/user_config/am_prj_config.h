/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 工程配置文件，包括系统，板级和硬件配置
 * \sa am_prj_config.h
 *
 * \internal
 * \par Modification history
 * - 1.00 14-12-23  tee, first implementation
 * \endinternal
 */

#ifndef __AM_PRJ_CONFIG_H
#define __AM_PRJ_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 * \addtogroup am_if_prj_config
 * \copydoc am_prj_config.h
 * @{
 */

/**
 * \brief 一些全局使用的单实例设备，默认在系统启动时初始化，以便全局使用
 *
 * 如 GPIO、DMA、INT 等。如果确定应用程序不使用这些资源，可以将对应的宏值修改为 0
 * 如无特殊情况，不建议禁能（将对应宏值修改为 0）这些默认初始化的单实例设备
 *
 * @{
 */

/** \brief 为 1，启动时，完成时钟初始化 */
#define AM_CFG_CLK_ENABLE                1

/** \brief 为 1，启动时，完成 GPIO 初始化 */
#define AM_CFG_GPIO_ENABLE               1

/** \brief 为 1，启动时，完成 INT 初始化 */
#define AM_CFG_INT_ENABLE                1

/** \brief 为 1，启动时，完成 DMA 初始化 */
#define AM_CFG_DMA_ENABLE                1

/** 
 * @}
 */

/**
 * \name 使能一些系统服务
 * @{
 */

/** \brief 是否使能 NVRAM 服务 */
#define AM_CFG_NVRAM_ENABLE              1

/** \brief 是否使能按键系统 */
#define AM_CFG_KEY_ENABLE                1

/** \brief 是否使能 isr defer 组件 */
#define AM_CFG_ISR_DEFER_ENABLE          1

/** 
 * @}
 */

/**
 * \name 板级初始化配置
 *
 * 默认可以使用板上的一些资源，如 LED，蜂鸣器等，如果不使用，应禁止相应的宏
 * @{
 */

/**
 * \brief 如果为 1，则初始化延时函数
 *
 * 初始化后可以使用 am_mdelay() 和 am_udelay() 延时函数
 *
 * \note 默认的延时函数由 am_delay.c 文件实现，如有特别需求，可以自行更新为其它
 *       实现方式
 */
#define AM_CFG_DELAY_ENABLE              1

/**
 * \brief 如果为 1，则初始化 LED 的相关功能，板上默认有两个 LED
 *
 * ID: 0 --- PIO0_8 （需要短接跳线帽 J9）
 * ID: 1 --- PIO0_9 （需要短接跳线帽 J10）
 */
#define AM_CFG_LED_ENABLE                1

/**
 * \brief 如果为 1，则初始化板载按键
 *
 * 默认键值为 KEY_KP0，使用时需要短接跳线帽 J14 的 KEY 和 PIO0_1，用作独立按键功能
 */
#define AM_CFG_KEY_GPIO_ENABLE           1

/**
 * \brief 如果为 1，则初始化蜂鸣器的相关功能
 *
 * 默认使用 SCT_OUT1 (PIO0_2) 输出 PWM （需要短接跳线帽 J7）
 */
#define AM_CFG_BUZZER_ENABLE             1

/**
 * \brief 如果为 1，则初始化系统滴答
 */
#define AM_CFG_SYSTEM_TICK_ENABLE        1

/**
 * \brief 如果为 1，则初始化软件定时器
 *
 * 软件定时器默认使用 MRT 定时器
 * 软件定时器的使用详见 am_softimer.h
 */
#define AM_CFG_SOFTIMER_ENABLE           1

/**
 * \brief 如果为 1，则初始化串口调试输出，调试串口默认为 USART0
 *
 * USART0 的 TXD 引脚默认为 PIO0_4 只需要将 PIO0_4 连接到串口模块的输入引脚，
 * 即可看到 AM_DBG_INFO() 打印的调试信息
 */
#define AM_CFG_DEBUG_ENABLE              1

/**
 * \brief 如果为 1，则系统会自动适配标准库
 *
 * 当系统适配标准库后，用户才可使用 printf()、malloc()、free() 等标准库函数
 *
 * \note 使用标准库时，将耗费系统更多的 FLASH 和 RAM 资源，用户应谨慎使用
 */
#define AM_CFG_STDLIB_ENABLE             1

/** 
 * @} 
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_PRJ_CONFIG_H */

/* end of file */
