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
 *        将串口用于485模式时，需要在中断模式下使用
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-15  nwt, make some changes.
 * - 1.00 15-10-19  xym, first implementation.
 * \endinternal
 */

#ifndef __AM_FSL_UART_H
#define __AM_FSL_UART_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_uart.h"
#include "hw/amhw_fsl_uart.h"
/**
 * \addtogroup am_fsl_if_uart
 * \copydoc am_fsl_uart.h
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
#define AM_FSL_UART_ERRCODE_UART_OTHER_INT  2

/** @} */

/**
 * \brief 485模式，发送/接收控制引脚方向函数，（中断模式下）
 * \param[in] flag  AM_TRUE:发送模式 AM_FALSE: 接收模式
 */
typedef void (*pfn_uart_int_485_send_t) (am_bool_t flag);

/**
 * \brief 串口驱动的版本号
 *  不同的版本号，设置波特率的函数，获取中断状态等函数也不同
 *  AM_FSL_UART_VER0 S1寄存器低5位可写，含有匹配地址的功能，（含有MA1，MA2，C5寄存器）
 *  AM_FSL_UART_VER0 S1寄存器低5位不可写 ,无匹配地址的功能。
 */

# define AM_FSL_UART_VER0 0
# define AM_FSL_UART_VER1 1
/**
 * \brief 串口设备信息结构体，该设备信息用于串口初始化
 */
typedef struct am_fsl_uart_devinfo {

    amhw_fsl_uart_t *p_hw_uart;     /**< \brief 指向UART寄存器块的指针. */

    uint8_t      inum;              /**< \brief 串口中断号.           */

    uint8_t      ver;               /**< \brief 串口驱动的版本号.           */

    uint32_t     clk_id;            /**< \brief uart 时钟号 */

    /**
     * \brief 串口配置标志，AMHW_FSL_UART_C1_M_*宏或AMHW_FSL_UART_C1_PARITY_*宏
     *        或AMHW_UART_BDH_SBNS_STOP_*的或值
     *        (如：AMHW_FSL_UART_C1_M_8BIT|AMHW_FSL_UART_C1_PARITY_NO
     *                                    |AMHW_FSL_UART_BDH_SBNS_STOP_1)
     */
    uint8_t     cfg_flags;


    uint32_t     baud_rate;           /**< \brief 初始化波特率.         */

    /**
     * \brief 指定使能的其它中断,AMHW_FSL_UART_INT_*宏值或多个AMHW_FSL_UART_INT_*宏的
     *        或值，除了(#AMHW_FSL_UART_INT_C2_TCIE),(#AMHW_FSL_UART_INT_C2_IRIE),
     *        (#AMHW_FSL_UART_INT_C2_ILIE)和(#AMHW_FSL_UART_INT_C2_TIE)这四个中断。
     */
    uint32_t     other_int_enable;

    /** \brief 485模式下发送/接收引脚控制，仅限中断模式 */
    pfn_uart_int_485_send_t uart_int_485_send;

    void (*pfn_plfm_init)(void);   /**< \brief 平台初始化函数.  */

    void (*pfn_plfm_deinit)(void); /**< \brief 平台去初始化函数. */

} am_fsl_uart_devinfo_t;

/**
 * \brief 串口设备结构体定义
 */
typedef struct am_fsl_uart_dev {

    am_uart_serv_t  uart_serv;             /**< \brief 标准UART服务          */

    /** \brief 指向用户注册的txchar_get函数 */
    int (*pfn_txchar_get)(void *, char *);  
    
    /** \brief 指向用户注册的rxchar_put函数 */
    int (*pfn_rxchar_put)(void *, char);   

    /** \brief 指向用户注册的错误回调函数   */    
    int (*pfn_err)(void *, int, void *, int); 

    void     *txget_arg;                   /**< \brief txchar_get函数参数    */
    void     *rxput_arg;                   /**< \brief rxchar_put函数参数    */
    void     *err_arg;                     /**< \brief 错误回调函数用户参数  */
    
    uint8_t   channel_mode;                /**< \brief 串口模式 中断/查询    */
    uint32_t  baud_rate;                   /**< \brief 串口波特率            */
    uint16_t  options;                     /**< \brief 硬件设置选项          */

    uint32_t  other_int_enable;            /**< \brief 指定使能的其它中断    */

    const am_fsl_uart_devinfo_t *p_devinfo;  /**< \brief 指向设备信息常量的指针*/

} am_fsl_uart_dev_t;

/**
 * \brief 初始化UART，返回UART标准服务操作句柄
 *
 * \param[in] p_dev     : 指向串口设备的指针
 * \param[in] p_devinfo : 指向串口设备信息常量的指针
 *
 * \return UART标准服务操作句柄，值为NULL时表明初始化失败
 */
am_uart_handle_t am_fsl_uart_init(am_fsl_uart_dev_t           *p_dev,
                                   const am_fsl_uart_devinfo_t *p_devinfo);

/**
 * \brief 不使用UART时，解初始化UART，释放相关资源
 * \param[in] p_dev : 指向串口设备的指针
 */
void am_fsl_uart_deinit(am_fsl_uart_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_FSL_UART_H */

/* end of file */
