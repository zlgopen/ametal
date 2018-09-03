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
 * \brief ZLG9021接口函数
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-06  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_ZLG9021_H
#define __AM_ZLG9021_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"
#include "am_i2c.h"
#include "am_softimer.h"
#include "am_uart_rngbuf.h"

/**
 * @addtogroup am_if_zlg9021
 * @copydoc am_zlg9021.h
 * @{
 */

/**
 * \name ZLG9021控制命令
 * 
 *     控制命令用于 am_zlg9021_ioctl() 函数的 cmd 参数。其p_arg参数的实际类型随着
 * 命令的不同而不同。
 *
 * @{
 */

/**
 * \brief 设置BLE连接间隔，单位ms
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，其值表示设置的BLE连接时间间隔，
 * 有效值有：20、50、100、200、300、400、500、1000、1500、2000
 */
#define  AM_ZLG9021_BLE_CONNECT_INTERVAL_SET     1

/**
 * \brief 获取BLE连接间隔，单位ms
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用于获取BLE连接时间间隔。
 */
#define  AM_ZLG9021_BLE_CONNECT_INTERVAL_GET     2

/**
 * \brief 模块重命名
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个表示新模块名的字符串，字符串
 * 长度需要在15个字符以内。
 */
#define  AM_ZLG9021_RENAME                       3

/**
 * \brief 模块重命名（自动在模块名后增加MAC地址）
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个表示新模块名的字符串，字符串
 * 长度需要在15个字符以内。更新后，模块会被命名为 NAME(xxxxxx)，括号内为模块的 MAC
 * 地址后 3 个字节的 Hex 码， 便于 iOS 系统识别模块
 */
#define  AM_ZLG9021_RENAME_MAC_STR              4


/**
 * \brief 获取模块名
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个转载模块名的缓冲区（数组），
 * 缓冲区大小至少为15字节。
 */
#define  AM_ZLG9021_NAME_GET                     5

/**
 * \brief 设置波特率
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，其值表示设置的波特率，
 * 有效值有：4800、9600、19200、38400、57600、115200
 */
#define  AM_ZLG9021_BAUD_SET                     6


/**
 * \brief 获取波特率
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用于获取比特率
 */
#define  AM_ZLG9021_BAUD_GET                     7



/**
 * \brief 获取MAC地址
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个13字符的缓冲区
 */
#define  AM_ZLG9021_MAC_GET                       8



/**
 * \brief 复位ZLG9021
 *
 * 此时， p_arg 参数未使用，值为NULL
 */
#define  AM_ZLG9021_RESET                         9


/**
 * \brief 设置BLE的广播周期（单位:ms）
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，其值表示广播周期
 * 有效值有：200、500、1000、1500、2000、2500、3000、4000、5000
 */
#define  AM_ZLG9021_BLE_ADV_PERIOD_SET            10


/**
 * \brief 获取BLE的广播周期（单位:ms）
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用以获取广播周期
 */
#define  AM_ZLG9021_BLE_ADV_PERIOD_GET            11


/**
 * \brief 设置发送功率（dBm）
 *
 * 此时， p_arg 参数的类型为  int 类型，其值表示发射功率
 * 有效值有：-20、-18、-16、-14、-12、-10、-8、-6、-4、-2、0、2、4
 */
#define  AM_ZLG9021_TX_POWER_SET                  12


/**
 * \brief 获取发送功率（dBm）
 *
 * 此时， p_arg 参数的类型为  int 指针类型，用以获取广播周期
 */
#define  AM_ZLG9021_TX_POWER_GET                  13


/**
 * \brief 设置BCTS输出低电平到串口数据之间的延时（单位:ms）
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，其值表示延时时间
 * 有效值有：0、10、20、30
 */
#define  AM_ZLG9021_BCTS_DELAY_SET                14


/**
 * \brief 获取BCTS输出低电平到串口数据之间的延时（单位:ms）
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用于获取延时时间
 */
#define  AM_ZLG9021_BCTS_DELAY_GET                15


/**
 * \brief 低功耗模式
 *
 * 此时， p_arg 参数为 am_bool_t，值为AM_TRUE时进入低功耗模式，值为AM_FALSE时退出低功耗模式。
 *
 * 若p_arg的值为 AM_TRUE，该函数使ZLG9021进入低功耗模式，此时，若EN引脚有效（初始化时
 * 实例信息提供），则拉高EN引脚，模块不工作，功耗最低；若EN引脚无效，则使用软件控制
 * 命令使ZLG9021进入低功耗模式。
 *
 * 若p_arg的值为FLASE，表示退出低功耗模式，模块恢复正常工作。
 */
#define  AM_ZLG9021_POWERDOWN                     16

/**
 * \brief 获取固件版本号
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用于获取版本号
 * 获取的值为整数：如：100（对应1.00版本），101（对应1.01版本）
 */
#define  AM_ZLG9021_VERSION_GET                   17

/**
 * \brief 获取当前的连接状态
 *
 * 此时， p_arg 参数的类型为  am_bool_t 指针类型，用于获取连接状态，
 * 若值为 AM_TRUE，表明已连接，若值为 AM_FALSE，表明未连接
 */
#define  AM_ZLG9021_CONSTATE_GET                  18

/**
 * \brief 断开当前连接
 *
 * 此时， p_arg 参数未使用，值为NULL
 */
#define  AM_ZLG9021_DISCONNECT                    19

/**
 * \brief 设置模块的配对码
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个表示配对码的字符串，字符串
 * 长度需要在15个字符以内。更新后，模块会被命名为 NAME(xxxxxx)，括号内为模块的 MAC
 * 地址后 3 个字节的 Hex 码， 便于 iOS 系统识别模块
 */
#define  AM_ZLG9021_PWD_SET                       20

/**
 * \brief 获取模块的配对码
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个转载配对码（字符串）的缓冲区，
 * 缓冲区大小至少为15字节。
 */
#define  AM_ZLG9021_PWD_GET                       21

/**
 * \brief 设置是否开启加密传输
 *
 * 此时， p_arg 参数的类型为  am_bool_t 类型， 值为AM_TRUE时开启传输加密功能，值为AM_FALSE
 * 时关闭传输加密功能。
 */
#define  AM_ZLG9021_ENC_SET                       22

/**
 * \brief 获取模块的加密状态
 *
 * 此时， p_arg 参数的类型为  am_bool_t 指针类型，  用于获取当前传输是否加密。
 */
#define  AM_ZLG9021_ENC_GET                       23

/**
 * \brief 恢复出厂设置
 *
 * p_arg参数为NULL
 *
 * 该命令必须在系统启动后 30s 内执行，方可正确恢复出厂设置，否则，可能恢复出厂设置
 * 失败。使用该命令时，需要确保实例信息中的 RESTORE 引脚有效（不为-1）
 */
#define  AM_ZLG9021_RESTORE                       30

/** @} */

/**
 * \name ZLG9021支持一些串口控制命令
 * @{
 */

/**
 * \brief 获取接收缓冲区中已接收的数据个数
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用于获取接收缓冲区中已接收的数据个数
 */
#define AM_ZLG9021_NREAD                         24

/**
 * \brief 获取发送缓冲区中已写入的数据个数
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用于获取发送缓冲区中已写入的数据个数
 */
#define AM_ZLG9021_NWRITE                        25

/**
 * \brief 清空发送缓冲区和接收缓冲区
 *
 * 此时， p_arg 参数未使用，值为NULL
 */
#define AM_ZLG9021_FLUSH                         26

/**
 * \brief 清空发送缓冲区
 *
 * 此时， p_arg 参数未使用，值为NULL
 */
#define AM_ZLG9021_WFLUSH                        27

/**
 * \brief 清空接收缓冲区
 *
 * 此时， p_arg 参数未使用，值为NULL
 */
#define AM_ZLG9021_RFLUSH                       28

/**
 * \brief 设置接收超时时间（单位：ms）
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型
 */
#define AM_ZLG9021_TIMEOUT                      29

/** @} */
 
/**
 * \brief ZLG9021实例信息结构体定义
 */
typedef struct am_zlg9021_devinfo {

    /** \brief EN引脚，若EN引脚固定接低电平，则无需MCU控制，此时，该值设置为 -1 */
    int          pin_en;

    /** \brief BRTS引脚，该引脚在每次发送数据至ZLG9021前被拉低，结束后拉高  */
    int          pin_brts;

    /** \brief 复位引脚  */
    int          pin_rst;

    /** \brief RESTORE引脚，用于恢复出厂设置   */
    int          pin_restore;

    /** \brief 模块当前使用的波特率        */
    uint32_t     baudrate;

    /** \brief 用于串口接收的缓冲区，建议大小在32以上     */
    uint8_t     *p_uart_rxbuf;

    /** \brief 用于串口发送的缓冲区，建议大小在32以上     */
    uint8_t     *p_uart_txbuf;

    /** \brief 用于串口接收的缓冲区大小 */
    uint16_t     rxbuf_size;

    /** \brief 用于串口发送的缓冲区大小 */
    uint16_t     txbuf_size;

} am_zlg9021_devinfo_t;

/**
 * \brief ZLG9021设备结构体定义
 */
typedef struct am_zlg9021_dev {

    /** \brief 数据是否在发送之中  */
    volatile am_bool_t          in_send;

    /** \brief brts 是否需要设置为高电平 */
    volatile am_bool_t          is_brts;

    /** \brief BLE is connected? */
    volatile int                connect_state;

    /** \brief 命令处理的状态  */
    volatile int                cmd_proc_state;

    /** \brief 获取 "TIM:CONNECT"的状态  */
    volatile int                stat_rep_type;

    /** \brief 获取 "TIM:DISCONNECT"的状态  */
    volatile int                stat_rep_cnt;

    /** \brief UART实例句柄                    */
    am_uart_handle_t            uart_handle;

    /** \brief 接收数据环形缓冲区      */
    struct am_rngbuf            rx_rngbuf;

    /** \brief 发送数据环形缓冲区      */
    struct am_rngbuf            tx_rngbuf;

    /** \brief 用于保存设备信息指针 */
    const am_zlg9021_devinfo_t *p_devinfo;

    /** \brief 用于接收命令应答的等待结构体       */
    am_wait_t                   ack_wait;

    /** \brief 用于接收数据的等待结构体   */
    am_wait_t                   rx_wait;

    /** \brief 接收超时时间                */
    volatile uint32_t           timeout_ms;

    /** \brief 命令回应接收缓冲区  */
    uint8_t                    *p_cmd_rxbuf;

    /** \brief 命令回应接收缓冲区的长度  */
    volatile int                cmd_rxbuf_len;

    /** \brief 命令回应已经接收的长度  */
    volatile int                cmd_rx_len;

    /** \brief The Timer for timeout */
    am_softimer_t               timer;

    /** \brief The Timer for BRTS */
    am_softimer_t               timer_brts;

} am_zlg9021_dev_t;


/** \brief 定义ZLG9021操作句柄  */
typedef struct am_zlg9021_dev *am_zlg9021_handle_t;
                                      
/**
 * \brief ZLG9021初始化函数
 *
 * 使用ZLG9021前，应该首先调用该初始化函数，以获取操作ZLG9021的handle。
 *
 * \param[in] p_dev      : 指向ZLG9021设备的指针
 * \param[in] p_devinfo  : 指向ZLG9021设备信息的指针
 * \param[in] uart_handle : UART标准服务操作句柄（使用该UART句柄与ZLG9021通信）
 *
 * \return 操作ZLG9021的handle，若初始化失败，则返回值为NULL。
 *
 * \note 设备指针p_dev指向的设备只需要定义，不需要在调用本函数前初始化。
 */
am_zlg9021_handle_t am_zlg9021_init (am_zlg9021_dev_t           *p_dev,
                                     const am_zlg9021_devinfo_t *p_devinfo,
                                     am_uart_handle_t            uart_handle);


/**
 * \brief ZLG9021控制函数
 *
 * \param[in] handle    : ZLG9021实例句柄
 * \param[in] cmd       : 控制指令，例如：#AM_ZLG9021_BAUD_SET
 * \param[in,out] p_arg : 命令对应的参数，例如： 115200
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_EIO     : 执行错误
 * \retval -AM_ENOTSUP : 指令不支持
 *
 * \note 处理
 */
int am_zlg9021_ioctl(am_zlg9021_handle_t  handle,
                     int                  cmd,
                     void                *p_arg);


/**
 * \brief 发送透传数据
 *
 * \param[in] handle : ZLG9021实例句柄
 * \param[in] p_buf  : 待发送的数据
 * \param[in] len    : 数据长度
 *
 * \return 非负数，实际发送的字节数，负数，发送失败
 */
int am_zlg9021_send (am_zlg9021_handle_t  handle,
                     const uint8_t       *p_buf,
                     int                  len);


/**
 * \brief 接收透传数据
 *
 * \param[in] handle : ZLG9021实例句柄
 * \param[in] p_buf  : 待发送的数据
 * \param[in] len    : 数据长度
 *
 * \return 非负数，实际接收的字节数，负数，接收失败
 */
int am_zlg9021_recv (am_zlg9021_handle_t  handle,
                     uint8_t             *p_buf,
                     int                  len);
 
/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG9021_H */

/* end of file */
