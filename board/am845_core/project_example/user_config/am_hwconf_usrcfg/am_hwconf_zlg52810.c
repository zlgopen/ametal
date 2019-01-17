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
 * \brief ZLG52810 用户配置文件
 * \sa am_hwconf_zlg52810.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-28  pea, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_zlg52810.h"
#include "am_lpc84x.h"
#include "am_lpc84x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_zlg52810
 * \copydoc am_hwconf_zlg52810.c
 * @{
 */

am_local uint8_t __g_zlg52810_txbuf[128]; /**< \brief 发送缓冲区 */
am_local uint8_t __g_zlg52810_rxbuf[128]; /**< \brief 接收缓冲区 */

/** \brief 设备实例 */
am_local am_zlg52810_dev_t __g_zlg52810_dev;

/** \brief 设备信息 */
am_local am_const am_zlg52810_devinfo_t __g_zlg52810_devinfo = {
    -1,                    /**< \brief RTS引脚   用作流控 */
    -1,                    /**< \brief CTS引脚   用作流控 */
    PIO0_28,                    /**< \brief 复位引脚 */
    -1,                         /**< \brief RESTORE 引脚，用于恢复出厂设置 */
    -1,                         /**< \brief low power wakeup 引脚，用于低功耗唤醒 */
    9600,                       /**< \brief 模块当前使用的波特率 */
    __g_zlg52810_rxbuf,         /**< \brief 接收缓存 */
    __g_zlg52810_txbuf,         /**< \brief 发送缓存 */
    sizeof(__g_zlg52810_rxbuf), /**< \brief 接收缓存长度 */
    sizeof(__g_zlg52810_txbuf)  /**< \brief 发送缓存长度 */
};

/**
 * \brief  ZLG52810 实例初始化，获得 ZLG52810 标准服务操作句柄
 *
 * \return  ZLG52810 标准服务操作句柄
 */
am_zlg52810_handle_t am_zlg52810_inst_init (void)
{
    am_uart_handle_t uart_handle = am_lpc84x_usart1_inst_init();

    return am_zlg52810_init(&__g_zlg52810_dev,
                            &__g_zlg52810_devinfo,
                             uart_handle);
}

/**
 * @}
 */

/* end of file */
