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
 * \brief ZLG52810接口函数
 *
 * \internal
 * \par Modification History
 * - 1.00 19-01-22  htf, first implementation.
 * \endinternal
 */

#ifndef __AM_ZLG52810_H
#define __AM_ZLG52810_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"
#include "am_i2c.h"
#include "am_softimer.h"
#include "am_uart_rngbuf.h"

/**
 * @addtogroup am_if_zlg52810
 * @copydoc am_zlg52810.h
 * @{
 */

/* \brief  ZLG52810 错误标识 */
#define  AM_ZLG52810_RECVIVE_INVALID       0x01  /* 接收内容无效 */
#define  AM_ZLG52810_PARAMETER_INVALID     0x02  /* 参数无效 */
#define  AM_ZLG52810_COMMAND_INVALID       0x03  /* 命令无效 */

/* \brief ZLG52810 波特率设置标识 */
#define  AM_ZLG52810_BAUD_4800             0
#define  AM_ZLG52810_BAUD_9600             1
#define  AM_ZLG52810_BAUD_19200            2
#define  AM_ZLG52810_BAUD_38400            3
#define  AM_ZLG52810_BAUD_57600            4
#define  AM_ZLG52810_BAUD_115200           5
#define  AM_ZLG52810_BAUD_230400           6
#define  AM_ZLG52810_BAUD_460800           7
#define  AM_ZLG52810_BAUD_1000000          8

/* \brief ZLG52810 流控设置标识 */
#define  AM_ZLG52810_FLOWC_DISABLE         0
#define  AM_ZLG52810_FLOWC_EN_NOFLOW       1
#define  AM_ZLG52810_FLOWC_EN_FLOW         2

/**
 * \name ZLG52810控制命令
 * 
 *     控制命令用于 am_zlg52810_ioctl() 函数的 cmd 参数。其p_arg参数的实际类型随着
 * 命令的不同而不同。
 *
 * @{
 */

/**
 * \brief 查询自动最大连接间隔，单位ms
 *
 * 此时， p_arg 参数的类型为  char 指针类型，其值表示获取的BLE自动最大连接间隔，
 * 其参数范围为 0006~0C80  长度为4个字符
 * 4 个字符代表一个 16 位数，用该16 位数乘以 1.25 即为希望设置的连接间隔，单位 ms
 */
#define  AM_ZLG52180_ACOMA_GET                    1

/**
 * \brief 获取BLE连接间隔，单位ms
 *
 * 此时， p_arg 参数的类型为 char 指针类型，其值表示设置的BLE自动最大连接间隔，
 * 其参数范围为 0006~0C80
 * 4 个字符代表一个 16 位数，用该16 位数乘以 1.25 即为希望设置的连接间隔，单位 ms
 */
#define  AM_ZLG52810_ACOMA_SET                     2

/**
 * \brief 查询自动更新连接参数
 *
 * 此时， p_arg 参数的类型为uint32_t 指针类型
 * 有效值有：0 禁能自动更新参数     1 使能自动更新参数
 */
#define  AM_ZLG52810_ACOUP_GET                    3

/**
 * \brief 设置自动更新连接参数
 *
 * 此时， p_arg 参数的类型为uint32_t 指针类型
 * 有效值有：0 禁能自动更新参数     1 使能自动更新参数
 */
#define  AM_ZLG52810_ACOUP_SET                    4

/**
 * \brief 查询设备地址
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个表示设备地址的字符串
 */
#define  AM_ZLG52810_ADDR_GET                         5

/**
 * \brief 查询广播包类型
 *
 * 参数的类型为  uint32_t 类型的指针  指向模块设置的广播包类型
 *  0 可发现可连接（默认）   1 可发现不可连接
 */
#define  AM_ZLG52810_ADTY_GET                         6

/**
 * \brief 设置广播包类型
 *
 * 参数的类型为  uint32_t 类型   设置的广播包类型
 *  0 可发现可连接（默认）   1 可发现不可连接
 */
#define  AM_ZLG52810_ADTY_SET                         7

/**
 * \brief 查询广播时间间隔
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向广播时间间隔设置
 * 有效参数 0~F  0:100ms   1:152.5ms   2:211.25ms(默认)   3:318.75ms
 *            4:417.5ms    5:546.25ms   6:760ms         7:852.5ms
 *            8:1022.5ms   9:1285ms     A:2000ms        B:3000ms
 *            C:4000ms     D:5000ms     E:6000ms        F:10000ms
 *
 */
#define  AM_ZLG52810_ADVI_GET                         8

/**
 * \brief 设置广播时间间隔  修改广播间隔可以有效降低功耗
 *
 * 此时， p_arg 参数的类型为  char 类型，表示广播时间间隔设置
 * 有效参数 0~F  0:100ms   1:152.5ms   2:211.25ms(默认)   3:318.75ms
 *            4:417.5ms    5:546.25ms   6:760ms         7:852.5ms
 *            8:1022.5ms   9:1285ms     A:2000ms        B:3000ms
 *            C:4000ms     D:5000ms     E:6000ms        F:10000ms
 *
 */
#define  AM_ZLG52810_ADVI_SET                         9

/**
 * \brief 获取波特率
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用于获取比特率
 * 有效值有：0:4800  1:9600  2:19200  3:38400  4:57600  5:115200
 *        6:230400  7:460800  8:1000000
 */
#define  AM_ZLG52810_BAUD_GET                         10

/**
 * \brief 设置波特率
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，其值表示设置的波特率，
 * 有效值有：0:4800  1:9600  2:19200  3:38400  4:57600  5:115200
 *        6:230400  7:460800  8:1000000
 */
#define  AM_ZLG52810_BAUD_SET                         11

/**
 * \brief 获取当前的连接状态
 *
 * 此时， p_arg 参数的类型为  am_bool_t 指针类型，用于获取连接状态，
 * 若值为 AM_TRUE，表明已连接，若值为 AM_FALSE，表明未连接
 */
#define  AM_ZLG52810_CONS_GET                         12

/**
 * \brief 断开当前连接
 *
 * 此时， p_arg 参数未使用，值为NULL
 */
#define  AM_ZLG52810_DISCONNECT                       13

/**
 * \brief 查询临时连接参数
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个4字符的缓冲区
 * 返回的4个字符代表一个16位数，用该数乘以1.25即为当前连接间隔，单位ms
 * 有效范围：  0006~0C80
 */
#define  AM_ZLG52810_COPS_GET                         14

/**
 * \brief 设置临时连接参数
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个4字符的设置值
 * 返回的4个字符代表一个16位数，用该数乘以1.25即为当前连接间隔，单位ms
 * 有效范围：  0006~0C80
 */
#define  AM_ZLG52810_COPS_SET                         15

/**
 * \brief 清除设备所有绑定信息
 *
 * 此时， p_arg 为NULL
 */
#define  AM_ZLG52810_ERASE                            16

/**
 * \brief 恢复出厂设置
 *
 * 此时， p_arg 为NULL
 */
#define  AM_ZLG52810_FARESET                          17

/**
 * \brief 查询串口流控功能
 *
 * 此时， p_arg 参数的类型为  int32_t 指针类型，用以获取当前串口流控功能状态
 * 有效值有： 0:关闭（默认）  1:打开
 */
#define  AM_ZLG52810_FLC_GET                          18

/**
 * \brief 设置串口流控功能
 *
 * 此时， p_arg 参数的类型为  int32_t 类型，用以设置    当前串口流控功能状态
 * 有效值有： 0:关闭（默认）  1:打开
 */
#define  AM_ZLG52810_FLC_SET                          19

/**
 * \brief 查询LE secure connection 功能开关
 *
 * 此时， p_arg 参数的类型为  int32_t 指针类型，用以获取LE secure connection 功能开关状态
 * 有效值有: 0:关闭（默认）   1:打开
 */
#define  AM_ZLG52810_LESC_GET                         20

/**
 * \brief 设置LE secure connection 功能开关
 *
 * 此时， p_arg 参数的类型为  int32_t 类型，用以设置LE secure connection 功能开关状态
 * 有效值有: 0:关闭（默认）   1:打开
 */
#define  AM_ZLG52810_LESC_SET                         21

/**
 * \brief 查询低功耗等级
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，其值用于获取低功耗等级
 * 有效值有：0:全速运行（默认）     1:低功耗1     2:低功耗2
 */
#define  AM_ZLG52810_LOWL_GET                         22

/**
 * \brief 设置低功耗等级
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，其值用于设置低功耗等级
 * 有效值有：0:全速运行（默认）     1:低功耗1     2:低功耗2
 */
#define  AM_ZLG52810_LOWL_SET                         23

/**
 * \brief 查询广播包自定义字段数据
 *
 * 此时， p_arg 参数的类型为  char 指针类型，用于获取广播包自定义字段数据
 */
#define  AM_ZLG52810_MFSD_GET                         24

/**
 * \brief 设置广播包自定义字段数据
 *
 * 此时， p_arg 参数的类型为  char 指针类型，用于设置广播包自定义字段数据
 * 有效范围为  0~52个字符         26个字节拆成52个16进制的字符
 */
#define  AM_ZLG52810_MFSD_SET                         25

/**
 * \brief 查询设备名
 *
 * 此时， p_arg 参数为char 指针类型，其用于存放查询的设备名
 */
#define  AM_ZLG52810_NAME_GET                         26

/**
 * \brief 设置设备名
 *
 * 此时， p_arg 参数为char 指针类型，其用于存放设置的设备名
 */
#define  AM_ZLG52810_NAME_SET                         27

/**
 * \brief 设置模块的配对码
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个表示配对码的字符串，字符串
 * 必须是‘0’~‘9’中的6个随意组合字符，（默认123456）。
 */
#define  AM_ZLG52810_PASS_SET                          28

/**
 * \brief 获取模块的配对码
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向一个转载配对码（字符串）的缓冲区，
 * 缓冲区大小为6字节（默认为123456）。
 */
#define  AM_ZLG52810_PASS_GET                          29

/**
 * \brief 查询发射功率
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，指向查询的发射功率，
 * 有效值有:  0:-20dbm   1:-12dbm   2:-4dbm  3:0dbm(默认)  4:4dbm
 */
#define  AM_ZLG52810_POWE_GET                         30

/**
 * \brief 查询发射功率
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，指向设置的发射功率，
 * 有效值有:  0:-20dbm   1:-12dbm   2:-4dbm  3:0dbm(默认)  4:4dbm
 */
#define  AM_ZLG52810_POWE_SET                         31

/**
 * \brief 软件复位功能
 *
 * 此时， p_arg 参数未使用，值为NULL
 */
#define  AM_ZLG52810_RESET                            32

/**
 * \brief 查询模块打印开关
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，指向获取的模块打印开关
 * 有效值有: 0:关闭       1:打开(默认)
 */
#define  AM_ZLG52810_RSPI_GET                         33

/**
 * \brief 设置模块打印开关
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，表示设置的模块打印开关
 * 有效值有: 0:关闭       1:打开(默认)
 */
#define  AM_ZLG52810_RSPI_SET                         34

/**
 * \brief 查询URATE发送延时
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，指向获取的URATE发送延时
 * 有效值有: 0:10ms     1:20ms  3:30ms   4:40ms  5:50ms  6:100ms  7:500ms
 */
#define  AM_ZLG52810_UTDT_GET                         35

/**
 * \brief 设置URATE发送延时
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，表示设置的URATE发送延时
 * 有效值有: 0:10ms     1:20ms  3:30ms   4:40ms  5:50ms  6:100ms  7:500ms
 */
#define  AM_ZLG52810_UTDT_SET                         36

/**
 * \brief 查询设备安全等级
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，指向获取的设备安全等级，
 * 有效值有: 0:不认证、不绑定        1:认证、不绑定          3:不认证、绑定          4:认证、绑定
 */
#define  AM_ZLG52810_SECL_GET                         37

/**
 * \brief 设置设备安全等级
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型，表示设置的设备安全等级，
 * 有效值有: 0:不认证、不绑定        1:认证、不绑定          3:不认证、绑定          4:认证、绑定
 */
#define  AM_ZLG52810_SECL_SET                         38

/**
 * \brief 查询TX characteristic UUID
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向获取的TX  UUID，
 * 有效值有: 0000~FFFF
 */
#define  AM_ZLG52810_UUTX_GET                         39

/**
 * \brief 设置TX characteristic UUID
 *
 * 此时， p_arg 参数的类型为  char 类型，表示设置的TX  UUID，
 *  有效值有: 0000~FFFF
 */
#define  AM_ZLG52810_UUTX_SET                         40

/**
 * \brief 查询RX characteristic UUID
 *
 * 此时， p_arg 参数的类型为  char 指针类型，指向获取的RX  UUID，
 * 有效值有: 0000~FFFF
 */
#define  AM_ZLG52810_UURX_GET                         41

/**
 * \brief 设置RX characteristic UUID
 *
 * 此时， p_arg 参数的类型为  char 指针类型，表示设置的RX  UUID，
 *  有效值有: 0000~FFFF
 */
#define  AM_ZLG52810_UURX_SET                         42

/**
 * \brief 查询透传服务UUID
 *
 * 此时， p_arg 参数的类型为  char 指针类型，表示查询的透传服务的  UUID，
 */
#define  AM_ZLG52810_UUSE_GET                         43

/**
 * \brief 设置透传服务UUID
 *
 * 此时， p_arg 参数的类型为  char 指针类型，表示设置的透传服务的  UUID，
 */
#define  AM_ZLG52810_UUSE_SET                         44

/**
 * \brief 获取固件版本号
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用于获取版本号
 */
#define  AM_ZLG52810_VERS_GET                         45

/**
 * \name ZLG52810支持一些串口控制命令
 * @{
 */

/**
 * \brief 获取接收缓冲区中已接收的数据个数
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用于获取接收缓冲区中已接收的数据个数
 */
#define AM_ZLG52810_NREAD                             46

/**
 * \brief 获取发送缓冲区中已写入的数据个数
 *
 * 此时， p_arg 参数的类型为  uint32_t 指针类型，用于获取发送缓冲区中已写入的数据个数
 */
#define AM_ZLG52810_NWRITE                            47

/**
 * \brief 清空发送缓冲区和接收缓冲区
 *
 * 此时， p_arg 参数未使用，值为NULL
 */
#define AM_ZLG52810_FLUSH                             48

/**
 * \brief 清空发送缓冲区
 *
 * 此时， p_arg 参数未使用，值为NULL
 */
#define AM_ZLG52810_WFLUSH                            49

/**
 * \brief 清空接收缓冲区
 *
 * 此时， p_arg 参数未使用，值为NULL
 */
#define AM_ZLG52810_RFLUSH                            50

/**
 * \brief 设置接收超时时间（单位：ms）
 *
 * 此时， p_arg 参数的类型为  uint32_t 类型
 */
#define AM_ZLG52810_TIMEOUT                           51

/** @} */
 
/**
 * \brief ZLG52810实例信息结构体定义
 */
typedef struct am_zlg52810_devinfo {
    /** \brief RTS引脚，用作流控
     * 低电平:表示模块能够接受MCU发出的串口数据，MCU可继续发送
     * 高电平:表示模块不能够接受MCU发出的串口数据，MCU应停止发送数据
     */
    int          pin_rts;

    /** \brief CTS引脚，用作流控
     * 低电平:若当前用户MCU不能接收数据时，应将该引脚拉高
     * 高电平:若当前用户MCU能接受数据时，应将引脚拉低
     */
    int          pin_cts;

    /** \brief 硬件复位引脚   低电平有效 */
    int          pin_rst;

    /** \brief RESTORE引脚，用于恢复出厂设置   需在全速运行模式下拉低5s恢复出厂设置 */
    int          pin_restore;

    /** \brief WAKEUP引脚，用于唤醒低功耗模式，下降沿有效 */
    int          pin_wakeup;

    /** \brief 模块初始化波特率        */
    uint32_t     baudrate;

    /** \brief 用于串口接收的缓冲区，建议大小在32以上     */
    uint8_t     *p_uart_rxbuf;

    /** \brief 用于串口发送的缓冲区，建议大小在32以上     */
    uint8_t     *p_uart_txbuf;

    /** \brief 用于串口接收的缓冲区大小 */
    uint16_t     rxbuf_size;

    /** \brief 用于串口发送的缓冲区大小 */
    uint16_t     txbuf_size;

} am_zlg52810_devinfo_t;

/**
 * \brief ZLG52810设备结构体定义
 */
typedef struct am_zlg52810_dev {

    /** \brief 串口发送是否开启流控功能 */
    volatile uint8_t            flowc_state;

    /** \brief BLE is connected? */
    volatile int                connect_state;

    /** \brief 命令处理的状态  */
    volatile int                cmd_proc_state;

    /** \brief 此次已经接收数据的个数*/
    volatile int                stat_rep_cnt;

    /** \brief UART实例句柄                    */
    am_uart_handle_t            uart_handle;

    /** \brief 接收数据环形缓冲区      */
    struct am_rngbuf            rx_rngbuf;

    /** \brief 发送数据环形缓冲区      */
    struct am_rngbuf            tx_rngbuf;

    /** \brief 用于保存设备信息指针 */
    const am_zlg52810_devinfo_t *p_devinfo;

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

    /** \brief 软件定时器用于判断一帧是否接收完成 */
    am_softimer_t               timer;
} am_zlg52810_dev_t;


/** \brief 定义ZLG52810操作句柄  */
typedef struct am_zlg52810_dev *am_zlg52810_handle_t;
                                      
/**
 * \brief ZLG52810初始化函数
 *
 * 使用ZLG52810前，应该首先调用该初始化函数，以获取操作ZLG52810的handle。
 *
 * \param[in] p_dev      : 指向ZLG52810设备的指针
 * \param[in] p_devinfo  : 指向ZLG52810设备信息的指针
 * \param[in] uart_handle : UART标准服务操作句柄（使用该UART句柄与ZLG52810通信）
 *
 * \return 操作ZLG52810的handle，若初始化失败，则返回值为NULL。
 *
 * \note 设备指针p_dev指向的设备只需要定义，不需要在调用本函数前初始化。
 */
am_zlg52810_handle_t am_zlg52810_init (am_zlg52810_dev_t           *p_dev,
                                       const am_zlg52810_devinfo_t *p_devinfo,
                                       am_uart_handle_t             uart_handle);


/**
 * \brief ZLG52810控制函数
 *
 * \param[in] handle    : ZLG52810实例句柄
 * \param[in] cmd       : 控制指令，例如：#AM_ZLG52810_BAUD_SET
 * \param[in,out] p_arg : 命令对应的参数，例如： 115200
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_EIO     : 执行错误
 * \retval -AM_ENOTSUP : 指令不支持
 *
 * \note 处理
 */
int am_zlg52810_ioctl(am_zlg52810_handle_t  handle,
                      int                   cmd,
                      void                 *p_arg);


/**
 * \brief 发送透传数据
 *
 * \param[in] handle : ZLG52810实例句柄
 * \param[in] p_buf  : 待发送的数据
 * \param[in] len    : 数据长度
 *
 * \return 非负数，实际发送的字节数，负数，发送失败
 */
int am_zlg52810_send (am_zlg52810_handle_t  handle,
                     const uint8_t         *p_buf,
                     int                    len);


/**
 * \brief 接收透传数据
 *
 * \param[in] handle : ZLG52810实例句柄
 * \param[in] p_buf  : 接收数据的缓冲区
 * \param[out] len   : 接收数据长度
 *
 * \return 非负数，实际接收的字节数，负数，接收失败
 */
int am_zlg52810_recv (am_zlg52810_handle_t  handle,
                     uint8_t               *p_buf,
                     int                    len);
 
/**
 * \brief ZLG52810 低功耗模式唤醒
 *
 * \param[in] handle : ZLG52810实例句柄
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval AM_FALSE    : 执行错误  无相应控制引脚
 */
int am_zlg52810_lpower_wakeup(am_zlg52810_handle_t  handle);

/**
 * \brief ZLG52810 恢复出厂设置
 *
 * \param[in] handle : ZLG52810实例句柄
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval AM_FALSE    : 执行错误  无相应控制引脚
 */
int am_zlg52810_fareset(am_zlg52810_handle_t  handle);

/**
 * \brief ZLG52810 硬件复位
 *
 * \param[in] handle : ZLG52810实例句柄
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval AM_FALSE    : 执行错误  无相应控制引脚
 */
int am_zlg52810_reset(am_zlg52810_handle_t  handle);
/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG52810_H */

/* end of file */
