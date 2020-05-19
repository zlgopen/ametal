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
 * \brief LPUART驱动，服务LPUART标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-26  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HC32_LPUART_H
#define __AM_HC32_LPUART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_uart.h"
#include "hw/amhw_hc32_lpuart.h"

/**
 * \addtogroup am_hc32_if_lpuart
 * \copydoc am_hc32_lpuart.h
 * @{
 */

/**
 * \name 错误编码号，错误回调函数的code参数，由驱动传递给用户，指明当前的错误类型
 *
 *  错误回调函数的参数原型：\n
 *  int lpuart_err(void *p_arg, int code, void *p_data, int size);
 *  p_arg是用户注册的用户参数。p_data和size不同的错误类型包含的信息有所不同。
 *  可以使用am_lpuart_callback_set() 函数设置串口错误回调函数。
 * @{
 */

/**
 * \brief 用户指定的其它中断发生,p_data的类型为uint32_t，值为串口的中断状态，方
 *        便用户判断具体的中断源，size为1。
 */
#define AM_HC32_LPUART_ERRCODE_LPUART_OTHER_INT  3

/** @} */

/**
 * \brief 串口设备信息结构体，该设备信息用于串口初始化
 */
typedef struct am_hc32_lpuart_devinfo {

    uint32_t    lpuart_reg_base;    /**< \brief 指向LPUART寄存器块的指针 */

    uint8_t     inum;               /**< \brief 串口中断号 */

    amhw_hc32_lpuart_sclk_src_t sclk_src;   /**< \brief 通信传输时钟  */

    uint32_t    cfg_flags;          /**< \brief 串口配置标志 */

    am_bool_t   asyn_hd;            /**< \brief 异步半双工模式（单线）标志 */

    uint32_t    baud_rate;          /**< \brief 初始化波特率 */

    /**
     * \brief 指定使能的中断,AMHW_HC32_LPUART_INT_*宏值或多个AMHW_HC32_LPUART_INT_*宏
     */
    uint32_t    other_int_enable;

    /** \brief RS485 方向控制函数, AM_TRUE: 发送模式， AM_FALSE: 接收模式 */
    void (*pfn_rs485_dir) (am_bool_t is_txmode);

    void (*pfn_plfm_init)(void);    /**< \brief 平台初始化函数 */

    void (*pfn_plfm_deinit)(void);  /**< \brief 平台去初始化函数 */

} am_hc32_lpuart_devinfo_t;

/**
 * \brief 串口设备结构体定义
 */
typedef struct am_hc32_lpuart_dev {

    am_uart_serv_t  lpuart_serv; /**< \brief 标准LPUART服务 */

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
    uint32_t  clk_rate;                     /**< \brief 串口模式时钟频率 */
    uint32_t  baud_rate;                    /**< \brief 串口波特率 */
    uint16_t  options;                      /**< \brief 硬件设置选项 */

    uint32_t  other_int_enable;             /**< \brief 指定使能的其它中断 */

    am_bool_t rs485_en;                     /**< \brief 是否使能了 485 模式 */

    const am_hc32_lpuart_devinfo_t *p_devinfo; /**< \brief 指向设备信息常量的指针 */

} am_hc32_lpuart_dev_t;

/**
 * \brief 初始化LPUART，返回LPUART标准服务操作句柄
 *
 * \param[in] p_dev     : 指向串口设备的指针
 * \param[in] p_devinfo : 指向串口设备信息常量的指针
 *
 * \return LPUART标准服务操作句柄，值为NULL时表明初始化失败
 */
am_uart_handle_t am_hc32_lpuart_init(am_hc32_lpuart_dev_t           *p_dev,
                                       const am_hc32_lpuart_devinfo_t *p_devinfo);

/**
 * \brief 不使用LPUART时，解初始化LPUART，释放相关资源
 *
 * \param[in] p_dev : 指向串口设备的指针
 */
void am_hc32_lpuart_deinit (am_hc32_lpuart_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32_LPUART_H */

/* end of file */
