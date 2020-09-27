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
 * \brief GS200 用户配置文件
 * \sa am_hwconf_gs200.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-16  fzb, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_gs200.h"
#include "am_zlg116_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_gs200
 * \copydoc am_hwconf_gs200.c
 * @{
 */

am_local uint8_t __g_gs200_txbuf[128];      /**< \brief 发送缓冲区 */
am_local uint8_t __g_gs200_rxbuf[128];      /**< \brief 接收缓冲区 */
am_local uint8_t __g_gs200_cmd_txbuf[5];    /**< \brief 命令发送缓冲区 */

/** \brief 设备实例 */
am_local am_gs200_dev_t __g_gs200_dev;

/** \brief 设备信息 */
am_local am_const am_gs200_devinfo_t __g_gs200_devinfo = {
    9600,                       /**< \brief 模块当前使用的波特率 */
    __g_gs200_rxbuf,            /**< \brief 接收缓存 */
    __g_gs200_txbuf,            /**< \brief 发送缓存 */
    __g_gs200_cmd_txbuf,        /**< \brief 命令发送缓存 */
    sizeof(__g_gs200_rxbuf),    /**< \brief 接收缓存长度 */
    sizeof(__g_gs200_txbuf),    /**< \brief 发送缓存长度 */
    AM_GS200_COM_FRAME_LEN,     /**< \brief 命令发送缓存长度 */
};

/**
 * \brief  GS200 实例初始化，获得 GS200 标准服务操作句柄
 *
 * \return  GS200 标准服务操作句柄
 */
am_gs200_handle_t am_gs200_inst_init (void)
{
    am_uart_handle_t uart_handle = am_zlg116_uart2_inst_init();

    return am_gs200_init(&__g_gs200_dev,
                         &__g_gs200_devinfo,
                         uart_handle);
}

/**
 * @}
 */

/* end of file */
