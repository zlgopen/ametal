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
 * \brief ZM516X zigbee配置文件
 * \sa am_hwconf_zm516x.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  win, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x_inst_init.h"
#include "am_zm516x.h"
#include "am_lpc82x.h"

/**
 * \addtogroup am_if_src_hwconf_zm516x
 * \copydoc am_hwconf_zm516x.c
 * @{
 */

static uint8_t __g_zm516x_txbuf[256];
static uint8_t __g_zm516x_rxbuf[256];

/* 定义 ZM516X 实例 */
static am_zm516x_dev_t __g_zm516x_dev;

/* 定义 ZM516X 实例信息 */
static const am_zm516x_dev_info_t __g_zm516x_devinfo = {
    PIO0_28,                        /**< \brief 复位引脚 */
    PIO0_25,                        /**< \brief ACK 引脚 */
    10,                             /**< \brief ACK 超时时间，单位为毫秒 */
    __g_zm516x_txbuf,               /**< \brief 发送缓存 */
    sizeof(__g_zm516x_txbuf),       /**< \brief 发送缓存长度 */
    __g_zm516x_rxbuf,               /**< \brief 接收缓存 */
    sizeof(__g_zm516x_rxbuf)        /**< \brief 接收缓存长度 */
};

/**
 * \brief  ZM516X实例初始化，获得ZM516X模块标准服务操作句柄
 *
 * \return  ZM516X模块标准服务操作句柄
 */
am_zm516x_handle_t am_zm516x_inst_init (void)
{
    return am_zm516x_init(&__g_zm516x_dev, &__g_zm516x_devinfo, am_lpc82x_usart1_inst_init());
}

/**
 * @}
 */

/* end of file */
