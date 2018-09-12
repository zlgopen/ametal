/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief USART驱动，服务UART标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-03  bob, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_USART_H
#define __AM_LPC_USART_H

#include "ametal.h"
#include "am_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_lpc_if_usart
 * \copydoc am_lpc_usart.h
 * @{
 */

/**
 * \brief 用户指定的其它中断发生
 */
#define AM_LPC_UART_ERRCODE_USART_OTHER_INT  2
 
/**
 * \brief 用于软件流控的XON字符定义，默认0x11
 */ 
#define AM_LPC_UART_XON       0x11
  
/**
 * \brief 用于软件流控的XOFF字符定义，默认0x13
 */ 
#define AM_LPC_UART_XOFF      0x13


/**
 * \brief 串口设备信息结构体，该设备信息用于串口初始化
 */
typedef struct am_lpc_usart_devinfo {

    uint32_t usart_regbase;        /**< \brief USART寄存器基址 */

    uint8_t  inum;                 /**< \brief 串口中断号 */
  
  uint32_t  clk_id;               /**< \brief 串口时钟ID */
    
    void (*pfn_plfm_init)(void);   /**< \brief 平台初始化函数 */

    void (*pfn_plfm_deinit)(void); /**< \brief 平台解初始化函数 */

    /** \brief RS485方向控制函数, AM_TRUE: 发送模式， AM_FALSE: 接收模式 */
    void  (*pfn_rs485_dir) (am_bool_t is_txmode);
    
} am_lpc_usart_devinfo_t;

/**
 * \brief 串口设备结构体定义
 */
typedef struct am_lpc_usart_dev {

    /** \brief 标准UART服务                 */
    am_uart_serv_t       uart_serv;

    /** \brief 指向用户注册的txchar_get函数 */
    am_uart_txchar_get_t pfn_txchar_get;

    /** \brief 指向用户注册的rxchar_put函数 */
    am_uart_rxchar_put_t pfn_rxchar_put;
 
    /** \brief 指向用户注册的错误回调函数   */
    am_uart_err_t        pfn_err;

    /** \brief txchar_get函数参数           */
    void     *p_txget_arg;

    /** \brief rxchar_put函数参数           */
    void     *p_rxput_arg;

    /** \brief 错误回调函数用户参数         */
    void     *p_err_arg;
  
    /** \brief 是否使能了 485模式          */
    am_bool_t rs485_en;

    /** \brief 流控模式（无，软件，硬件）   */
    uint8_t   flowctl_mode;                   

    /** \brief 流控发送器状态（用于软流控） */
    uint8_t   flowctl_tx_stat;                

    /** \brief 串口模式 中断/查询           */
    uint8_t   channel_mode; 

    /** \brief 串口波特率                   */
    uint32_t  baud_rate; 

    /** \brief 硬件设置选项                 */
    uint16_t  options;                        

    /** \brief 指定使能的其它中断           */
    uint32_t  other_int_enable;
 
    /** \brief 指向设备信息常量的指针       */
    const am_lpc_usart_devinfo_t *p_devinfo;

} am_lpc_usart_dev_t;

/**
 * \brief 初始化USART，返回UART标准服务操作句柄
 *
 * \param[in] p_dev     : 指向串口设备的指针
 * \param[in] p_devinfo : 指向串口设备信息常量的指针
 *
 * \return UART标准服务操作句柄，值为NULL时表明初始化失败
 */
am_uart_handle_t am_lpc_usart_init (am_lpc_usart_dev_t           *p_dev,
                                    const am_lpc_usart_devinfo_t *p_devinfo);

/**
 * \brief 不使用USART时，解初始化USART，释放相关资源
 *
 * \param[in] handle : UART标准服务操作句柄
 *
 * \return 无
 */
void am_lpc_usart_deinit (am_uart_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_USART_H */

/* end of file */
