/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ZLG9021 用户配置文件
 * \sa am_hwconf_zlg9021.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-28  pea, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_zlg9021.h"
#include "am_lpc82x.h"
#include "am_lpc82x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_zlg9021
 * \copydoc am_hwconf_zlg9021.c
 * @{
 */

am_local uint8_t __g_zlg9021_txbuf[128]; /**< \brief 发送缓冲区 */
am_local uint8_t __g_zlg9021_rxbuf[128]; /**< \brief 接收缓冲区 */

/** \brief 设备实例 */
am_local am_zlg9021_dev_t __g_zlg9021_dev;

/** \brief 设备信息 */
am_local am_const am_zlg9021_devinfo_t __g_zlg9021_devinfo = {
    PIO0_16,                   /**< \brief EN 引脚，若无需 MCU 控制，该值设置为 -1 */
    PIO0_25,                   /**< \brief BRTS 引脚，发送数据至 ZLG9021 前被拉低，结束后拉高 */
    PIO0_28,                   /**< \brief 复位引脚 */
    -1,                        /**< \brief RESTORE 引脚，用于恢复出厂设置 */
    9600,                      /**< \brief 模块当前使用的波特率 */
    __g_zlg9021_rxbuf,         /**< \brief 接收缓存 */
    __g_zlg9021_txbuf,         /**< \brief 发送缓存 */
    sizeof(__g_zlg9021_rxbuf), /**< \brief 接收缓存长度 */
    sizeof(__g_zlg9021_txbuf)  /**< \brief 发送缓存长度 */
};

/**
 * \brief  ZLG9021 实例初始化，获得 ZLG9021 标准服务操作句柄
 *
 * \return  ZLG9021 标准服务操作句柄
 */
am_zlg9021_handle_t am_zlg9021_inst_init (void)
{
    am_uart_handle_t uart_handle = am_lpc82x_usart2_inst_init();

    return am_zlg9021_init(&__g_zlg9021_dev,
                           &__g_zlg9021_devinfo,
                           uart_handle);
}

/**
 * @}
 */

/* end of file */
