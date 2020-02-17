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
 * \brief UART驱动，服务UART标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-10  ari, first implementation
 * \endinternal
 */

#ifndef __AM_MM32_UART_H
#define __AM_MM32_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_uart.h"
#include "hw/amhw_mm32_uart.h"

/**
 * \addtogroup am_mm32_if_uart
 * \copydoc am_mm32_uart.h
 * @{
 */

/**
 * \name 错误编码号，错误回调函数的code参数，由驱动传递给用户，指明当前的错误类型
 *
 *  错误回调函数的参数原型：\n
 *  int uart_err(void *p_arg, int code, void *p_data, int size);
 *  p_arg是用户注册的用户参数。p_data和size不同的错误类型包含的信息有所不同。
 *  可以使用am_uart_callback_set() 函数设置串口错误回调函数。
 * @{
 */

/**
 * \brief 用户指定的其它中断发生,p_data的类型为uint32_t，值为串口的中断状态，方
 *        便用户判断具体的中断源，size为1。
 */
#define AM_MM32_UART_ERRCODE_UART_OTHER_INT  3

/** @} */

/**
 * \brief 串口设备信息结构体，该设备信息用于串口初始化
 */
typedef struct am_mm32_uart_devinfo {

    uint32_t uart_reg_base;     /**< \brief 指向UART寄存器块的指针 */

    uint8_t  inum;              /**< \brief 串口中断号 */

    int      clk_num;           /**<  \brief 时钟ID */

    /**
     * \brief 串口配置标志，AMHW_MM32_UART_DATA_*宏或AMHW_MM32_UART_PARITY_*宏
     *        或AMHW_MM32_UART_STOP_*的或值
     *        (如：AMHW_MM32_UART_DATA_8BIT|AMHW_MM32_UART_PARITY_NO
     *                                       |AMHW_MM32_UART_STOP_1BIT)
     */
    uint8_t  cfg_flags;

    uint32_t baud_rate;         /**< \brief 初始化波特率 */

    /**
     * \brief 指定使能的其它中断,AMHW_MM32_UART_INT_*宏值或多个AMHW_MM32_UART_INT_*宏的
     *        或值，除了(# AMHW_MM32_UART_INT_TX_EMPTY_ENABLE),
     *               (# AMHW_MM32_UART_INT_RX_VAL_ENABLE)这两个中断。
     */
    uint32_t other_int_enable;

    /** \brief RS485 方向控制函数, AM_TRUE: 发送模式， AM_FALSE: 接收模式 */
    void (*pfn_rs485_dir) (am_bool_t is_txmode);

    void (*pfn_plfm_init)(void);   /**< \brief 平台初始化函数 */

    void (*pfn_plfm_deinit)(void); /**< \brief 平台去初始化函数 */

} am_mm32_uart_devinfo_t;

/**
 * \brief 串口设备结构体定义
 */
typedef struct am_mm32_uart_dev {

    am_uart_serv_t  uart_serv; /**< \brief 标准UART服务 */

    /** \brief 指向用户注册的txchar_get函数 */
    int (*pfn_txchar_get)(void *, char *);

    /** \brief 指向用户注册的rxchar_put函数 */
    int (*pfn_rxchar_put)(void *, char);

    /** \brief 指向用户注册的错误回调函数 */
    int (*pfn_err)(void *, int, void *, int);

    void     *txget_arg;                    /**< \brief txchar_get函数参数 */
    void     *rxput_arg;                    /**< \brief rxchar_put函数参数 */
    void     *err_arg;                      /**< \brief 错误回调函数用户参数 */

    uint8_t   channel_mode;                 /**< \brief 串口模式 中断/查询 */
    uint32_t  baud_rate;                    /**< \brief 串口波特率 */
    uint16_t  options;                      /**< \brief 硬件设置选项 */

    uint32_t  other_int_enable;             /**< \brief 指定使能的其它中断 */

    am_bool_t rs485_en;                     /**< \brief 是否使能了 485 模式 */

    const am_mm32_uart_devinfo_t *p_devinfo; /**< \brief 指向设备信息常量的指针 */

} am_mm32_uart_dev_t;

/**
 * \brief 初始化UART，返回UART标准服务操作句柄
 *
 * \param[in] p_dev     : 指向串口设备的指针
 * \param[in] p_devinfo : 指向串口设备信息常量的指针
 *
 * \return UART标准服务操作句柄，值为NULL时表明初始化失败
 */
am_uart_handle_t am_mm32_uart_init(am_mm32_uart_dev_t              *p_dev,
                                     const am_mm32_uart_devinfo_t *p_devinfo);

/**
 * \brief 不使用UART时，解初始化UART，释放相关资源
 *
 * \param[in] p_dev : 指向串口设备的指针
 */
void am_mm32_uart_deinit (am_mm32_uart_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MM32_UART_H */

/* end of file */
