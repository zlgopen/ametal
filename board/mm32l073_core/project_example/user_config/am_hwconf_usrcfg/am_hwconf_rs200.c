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
 * \brief RS200 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-17  RZ, first implementation.
 * \endinternal
 */

#include "am_rs200.h"
#include "am_common.h"
#include "am_mm32l073_inst_init.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define  RS200_RX_BUF_SIZE  16   /**< \brief 接收环形缓冲区大小，应该为2^n  */
#define  RS200_TX_BUF_SIZE  16   /**< \brief 发送环形缓冲区大小，应该为2^n  */

/*******************************************************************************
  全局变量
*******************************************************************************/
/** \brief UART 接收环形缓冲区  */
am_local uint8_t __rs200_rxbuf[RS200_RX_BUF_SIZE];

/** \brief UART 发送环形缓冲区  */
am_local uint8_t __rs200_txbuf[RS200_TX_BUF_SIZE];

/** \brief RS200设备信息实例 */
am_const am_local struct am_rs200_devinfo __g_rs200_info = {
    __rs200_rxbuf,
    __rs200_txbuf,
    RS200_RX_BUF_SIZE,
    RS200_TX_BUF_SIZE,
    200
};

/** \breif RS200 设备结构体定义 */
am_local struct am_rs200_dev __g_rs200_dev;

/** \brief RS200 设备实例化 */
am_rs200_handle_t am_rs200_inst_init (void)
{
    return am_rs200_init(&__g_rs200_dev,
                         &__g_rs200_info,
                          am_mm32l073_uart2_inst_init());
}

/* end of file */
