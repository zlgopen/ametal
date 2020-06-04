/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ZigBee 模块 ZM516X 操作接口
 *
 * 使用本服务需要包含头文件 am_zm516x.h
 *
 * \par 使用示例
 * \code
 * #include "am_zm516x.h"
 * \endcode
 *
 * \internal
 * \par modification history
 * - 1.01 18-01-16  pea, update API to v1.03 manual
 * - 1.00 15-09-06  afm, first implementation
 * \endinternal
 */

#ifndef __AM_ZM516X_H
#define __AM_ZM516X_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_zm516x
 * \copydoc am_zm516x.h
 * @{
 */

#include "am_types.h"
#include "am_wait.h"
#include "am_uart.h"
#include "am_rngbuf.h"

#define ZM516X_UART_RNGBUF_NREAD     0x0100   /**< \brief 可读数据个数        */
#define ZM516X_UART_RNGBUF_NWRITE    0x0200   /**< \brief 已经写入的数据个数  */
#define ZM516X_UART_RNGBUF_FLUSH     0x0300   /**< \brief 清空读/写缓冲区     */
#define ZM516X_UART_RNGBUF_WFLUSH    0x0400   /**< \brief 清空写缓冲区        */
#define ZM516X_UART_RNGBUF_RFLUSH    0x0500   /**< \brief 清空读缓冲区        */
#define ZM516X_UART_RNGBUF_TIMEOUT   0x0600   /**< \brief 设置等待超时        */

typedef struct am_zm516x_rngbuf_dev {

    /** \brief UART标准服务操作句柄    */
    am_uart_handle_t  handle;

    /** \brief 接收数据环形缓冲区      */
    struct am_rngbuf  rx_rngbuf;

    /** \brief 发送数据环形缓冲区      */
    struct am_rngbuf  tx_rngbuf;

    /** \brief 读超时                 */
    uint32_t          timeout_ms;

    /** \brief 用于接收等待           */
    am_wait_t         rx_wait;
} am_zm516x_rngbuf_dev_t;

/** \brief UART（带ring buffer的中断模式）标准服务操作句柄类型定义 */
typedef struct am_zm516x_rngbuf_dev * am_zm516x_rngbuf_handle_t;

#define __ZM516X_CMD_BUF_LEN       100    /**< \brief 命令缓存长度 */
#define __ZM516X_RSP_BUF_LEN       100    /**< \brief 应答缓存长度 */

/** \brief ZM516X 模块实例信息
 * IO 字节的bit0~bit3 为IO1~IO4，bit0 表示IO1，bit1 表示IO2，bit2 表示IO3，bit3 表示 IO4。
 * 该字节返回模块IO 当前的电平值，1 为高电平，0 为低电平。
 *
 * 在自组网模式下，IO1 和IO2 被用作自组网的功能管脚，不能设置为IO 触发。
 * AD0~AD3 表示返回模块4 路的AD 转换值，返回为10 位的AD 转换值，需要自行转换 为电压值，
 * 模块ADC 的参考电压为2.47V。
 */
typedef struct am_zm516x_io_adc_info {
    uint8_t  io;     /**< \brief IO状态  */
    uint16_t adc0;   /**< \brief AD0 */
    uint16_t adc1;   /**< \brief AD1 */
    uint16_t adc2;   /**< \brief AD2 */
    uint16_t adc3;   /**< \brief AD3  */
} *am_zm516x_io_adc_info_t;

typedef void (*pfn_upload_callback_t) (void                   *p_arg,
                                       am_zm516x_io_adc_info_t p_info);

/** \brief ZM516X 缓冲区信息 */
typedef struct am_zm516x_buf_info {
    uint8_t                 cmd_buf[__ZM516X_CMD_BUF_LEN]; /**< \brief 命令缓冲区 */
    uint8_t                 rsp_buf[__ZM516X_RSP_BUF_LEN]; /**< \brief 应答缓冲区 */
    uint32_t                cur_rsp_len;       /**< \brief 当前应答长度 */
    uint32_t                rsp_len;           /**< \brief 期望应答长度 */
} am_zm516x_buf_info_t;

/** \brief ZM516X 模块实例信息 */
typedef struct am_zm516x_dev_info {
    int      rst_pin;       /**< \brief 复位引脚 */
    int      ack_pin;       /**< \brief ACK 引脚 */
    uint16_t ack_timeout;   /**< \brief ACK 超时时间 */
    uint8_t *p_txbuf;       /**< \brief 发送缓冲区 */
    uint32_t txbuf_size;    /**< \brief 发送缓冲区大小（字节数） */
    uint8_t *p_rxbuf;       /**< \brief 接收缓冲区 */
    uint32_t rxbuf_size;    /**< \brief 接收缓冲区大小（字节数） */
} am_zm516x_dev_info_t;

/** \brief ZM516X 模块操作句柄 */
typedef struct am_zm516x_dev {
    am_zm516x_rngbuf_handle_t uart_handle;     /**< \brief UART 句柄(环形缓冲区) */
    am_zm516x_rngbuf_dev_t    uart_rngbuf_dev; /**< \brief UART 设备(环形缓冲区) */
    am_zm516x_buf_info_t      buf_info;        /**< \brief ZM516X 缓冲区信息 */

    am_wait_t                 ack_wait;        /**< \brief ACK 等待信号 */
    am_zm516x_dev_info_t     *p_devinfo;       /**< \brief 设备实例信息 */
    uint8_t                   prtl_type[2];    /**< \brief 协议类型 */

    uint8_t                   is_rsp;          /**< \brief 当前rsp接收状态 */
} am_zm516x_dev_t;

/** \brief ZM516X 模块标准服务操作句柄类型定义 */
typedef struct am_zm516x_dev *am_zm516x_handle_t;

/** \brief ZM516X 模块的配置信息 */
typedef struct am_zm516x_cfg_info {
    char    dev_name[16];     /**< \brief 设备名称 */
    char    dev_pwd[16];      /**< \brief 设备密码 */
    uint8_t dev_mode;         /**< \brief 设备类型，0: 终端设备 1: 路由设备 */
    uint8_t chan;             /**< \brief 通道号 */
    uint8_t panid[2];         /**< \brief 网络 ID (PanID) */
    uint8_t my_addr[2];       /**< \brief 本地网络地址 */
    uint8_t my_mac[8];        /**< \brief 本地物理地址(MAC) */
    uint8_t dst_addr[2];      /**< \brief 目标网络地址 */
    uint8_t dst_mac[8];       /**< \brief 目标物理地址(保留) */
    uint8_t reserve;          /**< \brief 保留 */

    /**
     * \brief 发射功率，值为 0~3，分别对应发射功率：
     *        -32dBm、-20.5dBm、-9dBm、+2.5dBm
     */
    uint8_t power_level;
    uint8_t retry_num;        /**< \brief 发送数据重试次数 */
    uint8_t tran_timeout;     /**< \brief 发送数据重试时间间隔(10ms) */

    /**
     * \brief 串口波特率，值为 1~7，分别对应波特率：
     *        2400、4800、9600、19200、38400、57600、115200
     */
    uint8_t serial_rate;
    uint8_t serial_data;      /**< \brief 串口数据位，值为 5~8 */
    uint8_t serial_stop;      /**< \brief 串口停止位，值为 1~2 */
    uint8_t serial_parity;    /**< \brief 串口校验位 0: 无校验 1: 奇校验 2: 偶校验 */
    uint8_t send_mode;        /**< \brief 发送模式 0: 单播  1: 广播 */
} am_zm516x_cfg_info_t;

/** \brief ZM516X 模块的基本信息(通过搜索命令 D4 获得) */
typedef struct am_zm516x_base_info {
    uint8_t dev_mode[2];      /**< \brief 设备类型 */
    uint8_t chan;             /**< \brief 通道号 */
    uint8_t data_rate;        /**< \brief RF通讯速率  */
    uint8_t panid[2];         /**< \brief 网络 ID (PanID) */
    uint8_t addr[2];          /**< \brief 网络地址 */
    uint8_t state;            /**< \brief 运行状态 */
} am_zm516x_base_info_t;

/** \brief ZM516X 远程模块的配置信息(通过获取远程配置信息命令 D5 获得) */
typedef struct am_zm516x_remote_info {
    am_zm516x_cfg_info_t cfg_info;       /**< \brief 配置信息 */
    uint8_t              state;          /**< \brief 运行状态 */
    uint8_t              dev_mode[2];    /**< \brief 设备类型 */
    uint8_t              version[2];     /**< \brief 固件版本 */
} am_zm516x_remote_info_t;

/**
 * \name ZM516X 模块的 AD 通道号
 * @{
 */
#define AM_ZM516X_AD_CHANNEL0         0
#define AM_ZM516X_AD_CHANNEL1         1
#define AM_ZM516X_AD_CHANNEL2         2
#define AM_ZM516X_AD_CHANNEL3         3
/** @} */

/** \brief ZM516X 模块的通讯模式 */
typedef enum am_zm516x_comm_mode {
    AM_ZM516X_COMM_UNICAST = 0,    /**< \brief 单播 */
    AM_ZM516X_COMM_BROADCAST,      /**< \brief 广播 */
} am_zm516x_comm_mode_t;

/** \brief ZM516X 模块的 MAC 地址 */
typedef struct am_zm516x_mac_addr {
    uint8_t mac[8];                /**< \brief MAC 地址 */
}am_zm516x_mac_addr_t;

/** \brief ZM516X 从机模块信息(通过查询主机模块存储的从机信息命令 E7 获得) */
typedef struct am_zm516x_slave_info {
    uint8_t mac[8];                /**< \brief MAC 地址 */
    uint8_t addr[2];               /**< \brief 网络地址 */
}am_zm516x_slave_info_t;

/**
 * \brief ZM516X 模块的地址
 *
 * 地址分为 2 字节的网络地址和 8 字节的 MAC 地址，使用 addr_size
 * 地址长度来区分地址类型
 */
typedef struct am_zm516x_addr {
    uint8_t *p_addr;               /**< \brief 地址 */
    uint8_t  addr_size;            /**< \brief 地址长度（字节数） */
}am_zm516x_addr_t;

/**
 * \brief ZM516X 模块的 PWM 信息
 */
typedef struct am_zm516x_pwm {

    /**
     * \brief PWM 频率，四个字节组合为一个 32 位变量，小端模式，单位为 Hz，通道
     *        1、2、4 的频率范围可设定从 62Hz~800000Hz，通道 3 的频率范围可设定
     *        从 62Hz~60000Hz。当 PWM 信号的频率为 0 时，表示不输出 PWM 信号
     */
    uint8_t freq[4];

    /**
     * \brief PWM 占空比，占空比的值可设定从 1~99，表示占空比的百分比
     */
    uint8_t duty_cycle;
}am_zm516x_pwm_t;

/**
 * \brief 向 ZM516X 模块发送 nbytes 个字节数据
 *
 * 这个接口向 ZM516X 模块发送 nbytes 个字节数据。若设备的内部缓冲不够，将做如下处理：
 * 剩余的数据不会被写入，返回值为已经发送的数据个数
 *
 * \param[in] handle ZM516X 模块操作句柄
 * \param[in] p_buf  要写入模块的数据缓冲区指针
 * \param[in] nbytes 要写入的字节个数
 *
 * \return 成功发送的数据个数，小于 0 为失败，失败原因可查看 errno
 *
 * \attention 该发送函数为异步操作函数，ZM516X 的配置命令是根据串口字符帧间隔时间
 *            来判断配置命令的结束，如果在调用了 am_zm516x_send 发送函数后，要紧
 *            接着调用 ZM516X 的配置命令，需要延时等待发送缓存数据完全发送的时间
 *            加上 ZM516X 的帧间隔时间后，才能调用 ZM516X 的配置命令，否则 ZM516X
 *            的配置命令会当作是数据发到 ZM516X 网络上，延时等待时间建议是：
 *            T(s) = (1 / 波特率) * 10 * 字节数 + 0.05
 */
am_err_t am_zm516x_send (am_zm516x_handle_t handle,
                         const void        *p_buf,
                         size_t             nbytes);

/**
 * \brief 向 ZigBee 模块发送 nbytes 个字节数据，并等待 ACK
 *
 * 这个接口向 ZM516X 模块发送 nbytes 个字节数据。若设备的内部缓冲不够，将做如下处理：
 * 剩余的数据不会被写入，返回值为已经发送的数据个数
 *
 * \param[in] handle ZM516X 模块操作句柄
 * \param[in] p_buf  要写入模块的数据缓冲区指针
 * \param[in] nbytes 要写入的字节个数
 *
 * \retval  > 0          成功发送的数据个数
 * \retval -AM_ENOTSUP   没有配置 ACK 引脚
 * \retval -AM_ETIMEDOUT 发送无应答
 *
 * \attention 1. 该发送函数为同步操作函数，发送数据后将等待 ACK，当收到 ACK 或者
 *               超时时才会返回；
 *            2. 发送模式为广播模式时不会产生 ACK。
 */
am_err_t am_zm516x_send_with_ack (am_zm516x_handle_t handle,
                                  const void        *p_buf,
                                  size_t             nbytes);

/**
 * \brief 从 ZM516X 模块接收 maxbytes 个字节数据
 *
 * - 这个接口从 ZM516X 模块接收最多 maxbytes 个字节数据
 * - 若设备可接收数据个数为 0，将立即返回
 *
 * \param[in]  handle   ZM516X 模块操作句柄
 * \param[out] p_buf    存放读取数据的缓冲区指针
 * \param[in]  maxbytes 最多要读取的字节个数
 *
 * \return 成功接收的数据个数，小于 0 为失败，失败原因可查看 errno
 */
am_err_t am_zm516x_receive (am_zm516x_handle_t handle,
                            void              *p_buf,
                            size_t             maxbytes);

/**
 * \brief 获取 ZM516X 模块的配置信息（永久命令：D1）
 *
 * \param[in]  handle ZM516X 模块操作句柄
 * \param[out] p_info 读取 ZM516X 模块的配置信息
 *
 * \retval  AM_OK    读取成功
 * \retval -AM_EPERM 读取失败
 */
am_err_t am_zm516x_cfg_info_get (am_zm516x_handle_t    handle,
                                 am_zm516x_cfg_info_t *p_info);

/**
 * \brief 设置 ZigBee 模块通道号（永久命令：D2）
 *
 * 模块可工作在 16 个物理通道上，根据模块的载波频率不同，不同通道的模块彼此间物
 * 理通道不通，因此在设计上可以实现物理上划分网段的效果
 *
 * \param[in] handle ZM516X 模块操作句柄
 * \param[in] chan   通道号 11~26
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EPERM  设置失败
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_zm516x_cfg_channel_set (am_zm516x_handle_t handle, uint8_t chan);

/**
 * \brief 搜索其它模块（永久命令：D4）
 *
 * 模块接收到本命令后，会向本网段内的相同通道的其他模块发出广播搜索包，运行本公
 * 司固件的 ZigBee 模块都会应答此广播，应答内容会将自己的相关基本信息返回到搜索
 * 发起目标节点。所以搜索前需要设置通道号才能搜索到对应通道的模块。
 * 此函数为同步接口，将强制等待timeout时间以接收应答
 *
 * \param[in]  handle      ZM516X 模块操作句柄
 * \param[in]  buf_size    p_base_info 指向的缓冲区大小，单位为
 *                         sizeof(am_zm516x_base_info_t)
 * \param[out] p_base_info 存储接收到的从机信息的缓冲区
 * \param[out] p_get_size  实际搜索到的模块数量
 * \param[in]  timeout     超时时间，此时间段外的应答将被忽略
 *
 * \retval  AM_OK     读取成功
 * \retval -AM_EPERM  读取失败
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_zm516x_discover (am_zm516x_handle_t     handle,
                             uint8_t                buf_size,
                             am_zm516x_base_info_t *p_base_info,
                             uint8_t               *p_get_size,
                             uint32_t               timeout);

/**
 * \brief 获取远程配置信息（永久命令：D5）
 *
 * \param[in]  handle        ZM516X 模块操作句柄
 * \param[in]  p_zb_addr     目标地址
 * \param[out] p_remote_info 搜索到的 ZM516X 模块的配置信息
 *
 * \retval  AM_OK     读取成功
 * \retval -AM_EPERM  读取失败
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EFAULT 地址错误
 *
 * \attention 串口数据位：5～8，数据位如果设置为 5、6、7 位，则不可以获取配置信息
 */
am_err_t am_zm516x_remote_info_get (am_zm516x_handle_t       handle,
                                    am_zm516x_addr_t        *p_zb_addr,
                                    am_zm516x_remote_info_t *p_remote_info);

/**
 * \brief 修改 ZM516X 模块的配置信息（永久命令：D6），设置成功需复位
 *
 * \param[in] handle ZM516X 模块操作句柄
 * \param[in] p_info ZM516X 模块的配置信息
 *
 * \retval  AM_OK    修改成功
 * \retval -AM_EPERM 修改失败
 *
 * \attention AMetal 平台操作 ZM516X 模块的串口参数只支持波特率配置，数据位固定为
 *            8 位，校验为无校验，停止位为 1 位，在更改模块的数据位，校验位，停止
 *            位时需要注意，如果不小心更改了，需要把模块恢复出厂默认才能操作
 */
am_err_t am_zm516x_cfg_info_set (am_zm516x_handle_t    handle,
                                 am_zm516x_cfg_info_t *p_info);

/**
 * \brief 使 ZM516X 模块复位（永久命令：D9）
 *
 * 复位帧无应答
 *
 * \param[in] handle ZM516X 模块操作句柄
 *
 * \return 无
 */
void am_zm516x_reset (am_zm516x_handle_t handle);

/**
 * \brief 恢复 ZM516X 模块出厂设置（永久命令：DA），设置成功需复位
 *
 * \param[in] handle ZM516X 模块操作句柄
 *
 * \retval  AM_OK    设置成功
 * \retval -AM_EPERM 设置失败
 */
am_err_t am_zm516x_default_set (am_zm516x_handle_t handle);

/**
 * \brief 设置 ZigBee 模块接收的数据包包头是否显示源地址（永久命令：DC），
 *        设置成功需复位
 *
 * \param[in] handle    ZM516X 模块操作句柄
 * \param[in] p_zb_addr 目标地址
 * \param[in] flag      包头是否显示源地址 AM_TRUE: 显示，AM_FALSE: 不显示
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EPERM  设置失败
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EFAULT 地址错误
 */
am_err_t am_zm516x_cfg_display_head_set (am_zm516x_handle_t handle,
                                         am_zm516x_addr_t  *p_zb_addr,
                                         am_bool_t          flag);

/**
 * \brief 设置指定地址 ZigBee 模块的 GPIO 输入输出方向（永久命令：E1）
 *
 * \param[in] handle    ZM516X 模块操作句柄
 * \param[in] p_zb_addr 目标地址
 * \param[in] dir       管脚方向，该字节各个位 1 为输出，0 为输入；
 *                      bit0 对应 GPIO1，bit3 对应 GPIO4
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EPERM  设置失败
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EFAULT 地址错误
 */
am_err_t am_zm516x_cfg_gpio_dir_set (am_zm516x_handle_t handle,
                                     am_zm516x_addr_t  *p_zb_addr,
                                     uint8_t            dir);

/**
 * \brief 获取指定地址 ZigBee 模块的 GPIO 输入输出方向（永久命令：E1）
 *
 * \param[in]  handle    ZM516X 模块操作句柄
 * \param[in]  p_zb_addr 目标地址
 * \param[out] p_dir     获取到的管脚方向，该字节各个位 1 为输出，0 为输入；
 *                       bit0 对应 GPIO1，bit3 对应 GPIO4
 *
 * \retval  AM_OK     获取成功
 * \retval -AM_EPERM  获取失败
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EFAULT 地址错误
 */
am_err_t am_zm516x_cfg_gpio_dir_get (am_zm516x_handle_t handle,
                                     am_zm516x_addr_t  *p_zb_addr,
                                     uint8_t           *p_dir);

/**
 * \brief IO/AD 采集设置（永久命令：E2），设置成功需复位
 *
 * AW516x 系列 ZigBee 模块有 4 路的 IO 和 4 路的 AD，IO/AD 采集设置命令可设定模块
 * IO 触发上传 IO 和 AD 状态及根据配置的周期时间定时上传 IO 和 AD 状态
 *
 * \param[in] handle     ZM516X 模块操作句柄
 * \param[in] p_zb_addr  目标地址
 * \param[in] dir        IO 触发边沿，bit0~bit3 对应 IO1~IO4，当这些位为 1 时，
 *                       表示上升沿触发，为 0 时，表示下降沿触发
 * \param[in] period     上传周期，单位为 10ms，最大可设置的值为 65535，
 *                       即设置的最大周期为 655350ms
 * \param[in] is_dormant 是否休眠，指示模块是否处于休眠状态，该字节为 1 表示模块处
 *                       于休眠状态，IO 根据配置的状态发生变化时唤醒模块，并向目标
 *                       节点地址发送 IO/AD 采集帧，如果周期大于 0，模块除了可以 IO
 *                       唤醒外，还会根据周期定时定时唤醒，然后向目标节点地址发送
 *                       IO/AD 采集帧，当发送完 IO/AD 采集帧后，模块进入休眠状态，
 *                       该字节为 0 表示模块不休眠
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EPERM  设置失败
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EFAULT 地址错误
 */
am_err_t am_zm516x_cfg_io_adc_upload_set (am_zm516x_handle_t handle,
                                          am_zm516x_addr_t  *p_zb_addr,
                                          uint8_t            dir,
                                          uint16_t           period,
                                          am_bool_t          is_dormant);

/**
 * \brief IO/AD 采集配置获取（永久命令：E2）
 * \param[in] handle        ZM516X 模块操作句柄
 * \param[in] p_zb_addr     目标地址
 * \param[out] p_dir        获取到的 IO 触发边沿配置，如果不需要获取，传入 NULL 即
 *                          可。bit0~bit3 对应 IO1~IO4，当这些位为 1 时，表示上升沿
 *                          触发，为 0 时，表示下降沿触发
 * \param[out] p_period     获取到的上传周期配置，单位为 10ms，如果不需要获取，传
 *                          入 NULL 即可
 * \param[out] p_is_dormant 获取到的是否休眠配置，如果不需要获取，传入 NULL 即可。
 *                          指示模块是否处于休眠状态，该字节为 1 表示模块处于休眠状
 *                          态，IO 根据配置的状态发生变化时唤醒模块，并向目标节点地
 *                          址发送 IO/AD 采集帧，如果周期大于 0，模块除了可以 IO唤醒
 *                          外，还会根据周期定时定时唤醒，然后向目标节点地址发送
 *                          IO/AD 采集帧，当发送完 IO/AD 采集帧后，模块进入休眠状
 *                          态，该字节为 0 表示模块不休眠
 *
 * \retval  AM_OK     获取成功
 * \retval -AM_EPERM  获取失败
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EFAULT 地址错误
 */
am_err_t am_zm516x_cfg_io_adc_upload_get (am_zm516x_handle_t handle,
                                          am_zm516x_addr_t  *p_zb_addr,
                                          uint8_t           *p_dir,
                                          uint16_t          *p_period,
                                          uint8_t           *p_is_dormant);

/**
 * \brief 设置指定地址 ZigBee 模块的 GPIO 输出值（永久命令：E3）
 *
 * 在调用该命令前需要先调用 I/O 方向设置命令把相应的 IO 设置为输出
 *
 * \param[in] handle    ZM516X 模块操作句柄
 * \param[in] p_zb_addr 要设置的 ZM516X 模块的目标地址
 * \param[in] value     GPIO 输出值，IO 字节的 bit0~bit3 对应 IO1~IO4，其相应位为
 *                      1，表示 I/O 输出高电平；其相应位为 0，表示 I/O 输出低电平
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EPERM  设置失败
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EFAULT 地址错误
 */
am_err_t am_zm516x_cfg_gpio_set (am_zm516x_handle_t handle,
                                 am_zm516x_addr_t  *p_zb_addr,
                                 uint8_t            value);


/**
 * \brief 获取指定地址 ZigBee 模块的 GPIO 输入值（永久命令：E3）
 *
 * 在调用该命令前需要先调用 I/O 方向设置命令把相应的 IO 设置为输入
 *
 * \param[in]  handle    ZM516X 模块操作句柄
 * \param[in]  p_zb_addr 要设置的 ZM516X 模块的目标地址
 * \param[out] p_value   获取到的 GPIO 输入值，IO 字节的 bit0~bit3 对应 IO1~IO4，
 *                       其相应位为 1，表示 I/O 输入高电平；其相应位为 0，表示 I/O
 *                       输入低电平
 *
 * \retval  AM_OK     获取成功
 * \retval -AM_EPERM  获取失败
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EFAULT 地址错误
 */
am_err_t am_zm516x_cfg_gpio_get (am_zm516x_handle_t handle,
                                 am_zm516x_addr_t  *p_zb_addr,
                                 uint8_t           *p_value);

/**
 * \brief 设置指定地址 ZigBee 模块的 PWM 输出值（永久命令：E4）
 *
 * \param[in] handle    ZM516X 模块操作句柄
 * \param[in] p_zb_addr 要设置的 ZM516X 模块的目标地址
 * \param[in] p_pwm     PWM 参数指针，指针指向的内存必须为 sizeof(am_zm516x_pwm_t)
 *                      的 4 倍大小，这样才能存储下 4 个 PWM 通道的参数
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EPERM  设置失败
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EFAULT 地址错误
 */
am_err_t am_zm516x_cfg_pwm_set (am_zm516x_handle_t handle,
                                am_zm516x_addr_t  *p_zb_addr,
                                am_zm516x_pwm_t   *p_pwm);


/**
 * \brief 获取指定地址 ZigBee 模块的 PWM 输出值配置（永久命令：E4）
 *
 * \param[in]  handle    ZM516X 模块操作句柄
 * \param[in]  p_zb_addr 要设置的 ZM516X 模块的目标地址
 * \param[out] p_pwm     PWM 参数指针，指针指向的内存必须为 sizeof(am_zm516x_pwm_t)
 *                       的 4 倍大小，这样才能存储下 4 个 PWM 通道的参数
 *
 * \retval  AM_OK     获取成功
 * \retval -AM_EPERM  获取失败
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EFAULT 地址错误
 */
am_err_t am_zm516x_cfg_pwm_get (am_zm516x_handle_t handle,
                                am_zm516x_addr_t  *p_zb_addr,
                                am_zm516x_pwm_t   *p_pwm);

/**
 * \brief 设置自组网功能（永久命令：E5），设置成功需复位
 *
 * 自组网功能为一主多从的星形网络拓扑结构，在主机到从机信号不可达时，可加入中继节点
 * 进行信号传递。模块在自组网模式下，主机模块会自动选择周围没有被使用的 PANID 和通
 * 道号形成一个独立的网络，并能自动分配一个唯一的本地网络地址给从机模块，从机模块使
 * 用时在使能了自组网功能后就不需要进行任何的配置操作，从机模块在加入网络后就能跟主
 * 机进行通讯。一个主机模块最多可连 200 个从机模块。详细流程请参考文档
 * AW516x_ZigBee.pdf
 *
 * \param[in] handle ZM516X 模块操作句柄
 * \param[in] state  自组网使能，0: 失能 1: 使能
 * \param[in] type   节点类型，0: 主机节点 1: 从机节点
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EPERM  设置失败
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_zm516x_cfg_network_set (am_zm516x_handle_t handle,
                                    am_bool_t          state,
                                    uint8_t            type);

/**
 * \brief 主机允许从机加入网络（永久命令：E6）
 *
 * 允许加入网络开启的窗口时间为当给主机发送了允许加入网络命令后，主机开始接受从机的
 * 入网请求，到达这个窗口时间后，主机不再接受从机的入网请求，窗口时间结束后，主机进
 * 入正常的工作状态
 *
 * \param[in] handle ZM516X 模块操作句柄
 * \param[in] time   允许加入网络开启的窗口时间，单位为秒
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EPERM  设置失败
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_zm516x_cfg_join_enable (am_zm516x_handle_t handle,
                                    uint16_t           time);

/**
 * \brief 查询主机模块存储的从机信息（永久命令：E7）
 *
 * \param[in]  handle       ZM516X 模块操作句柄
 * \param[in]  buf_size     p_slave_info 指向的缓冲区大小，单位为
 *                          sizeof(am_zm516x_slave_info_t)
 * \param[out] p_slave_info 存储接收到的从机信息的缓冲区
 * \param[out] p_get_size   实际获取到的从机信息数量
 *
 * \retval  AM_OK     查询成功
 * \retval -AM_EPERM  查询失败
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_zm516x_cfg_slave_get (am_zm516x_handle_t      handle,
                                  uint8_t                 buf_size,
                                  am_zm516x_slave_info_t *p_slave_info,
                                  uint8_t                *p_get_size);

/**
 * \brief 查询主从机状态（永久命令：E8）
 *
 * \param[in]  handle           ZM516X 模块操作句柄
 * \param[out] p_state          获取到的状态，0: 主机节点 1: 从机节点，如不需要获
 *                              取该参数，传入 NULL 即可
 * \param[out] p_response_state 获取到的响应状态，当模块配置为主机时，
 *                                  0: 主机空闲
 *                                  1: 主机正在检测网络
 *                                  2: 主机允许从机加入网络
 *                              当模块配置为从机时，
 *                                  0: 从机正在加入网
 *                                  1: 从机已加入网络
 *                                  2: 从机已退出网络
 *                              如不需要获取该参数，传入 NULL 即可
 *
 * \retval  AM_OK     查询成功
 * \retval -AM_EPERM  查询失败
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_zm516x_cfg_state_get (am_zm516x_handle_t handle,
                                  uint8_t           *p_state,
                                  uint8_t           *p_response_state);

/**
 * \brief 设置 ZM516X 模块通道号（临时命令：D1）
 *
 * \param[in] handle ZM516X 模块操作句柄
 * \param[in] chan   通道号
 *
 * \retval  AM_OK    设置成功
 * \retval -AM_EPERM 设置失败
 */
am_err_t am_zm516x_channel_set (am_zm516x_handle_t handle, uint8_t chan);

/**
 * \brief 设置 ZM516X 模块目标地址（临时命令：D2）
 *
 * \param[in] handle    ZM516X 模块操作句柄
 * \param[in] p_zb_addr 目标地址
 *
 * \retval  AM_OK    设置成功
 * \retval -AM_EPERM 设置失败
 */
am_err_t am_zm516x_dest_addr_set (am_zm516x_handle_t handle,
                                  am_zm516x_addr_t  *p_zb_addr);

/**
 * \brief 设置 ZM516X 模块接收的数据包包头是否显示源地址（临时命令：D3）
 *
 * \param[in] handle ZM516X 模块操作句柄
 * \param[in] flag   包头是否显示源地址 AM_TRUE: 显示，AM_FALSE: 不显示
 *
 * \retval  AM_OK    设置成功
 * \retval -AM_EPERM 设置失败
 */
am_err_t am_zm516x_display_head_set (am_zm516x_handle_t handle, am_bool_t flag);

/**
 * \brief 设置指定地址 ZM516X 模块的 GPIO 输入输出方向（临时命令：D4）
 *
 * \param[in] handle    ZM516X 模块操作句柄
 * \param[in] p_zb_addr 要设置的 ZM516X 模块的目标地址
 * \param[in] dir       管脚方向，该字节各个位 1 为输出，0 为输入；
 *                      bit0 对应 GPIO0，bit7 对应 GPIO7
 *
 * \retval  AM_OK    设置成功
 * \retval -AM_EPERM 设置失败
 */
am_err_t am_zm516x_gpio_dir (am_zm516x_handle_t handle,
                             am_zm516x_addr_t  *p_zb_addr,
                             uint8_t            dir);

/**
 * \brief 读取指定地址 ZM516X 模块的 GPIO 输入值（临时命令：D5）
 *
 * \param[in]  handle    ZM516X 模块操作句柄
 * \param[in]  p_zb_addr 要读取的 ZM516X 模块的目标地址
 * \param[out] p_value   读取到的管脚输入值，该字节各个位 1 为高电平，0 为低电平；
 *                       bit0 对应 GPIO0，bit7 对应 GPIO7
 *
 * \retval  AM_OK    读取成功
 * \retval -AM_EPERM 读取失败
 */
am_err_t am_zm516x_gpio_get (am_zm516x_handle_t handle,
                             am_zm516x_addr_t  *p_zb_addr,
                             uint8_t           *p_value);

/**
 * \brief 设置指定地址 ZM516X 模块的 GPIO 输出值（临时命令：D6）
 *
 * \param[in] handle    ZM516X 模块操作句柄
 * \param[in] p_zb_addr 要设置的 ZM516X 模块的目标地址
 * \param[in] value     管脚输出值，该字节各个位 1 为高电平，0 为低电平；
 *                      bit0 对应 GPIO0，bit7 对应 GPIO7
 *
 * \retval  AM_OK    设置成功
 * \retval -AM_EPERM 设置失败
 */
am_err_t am_zm516x_gpio_set (am_zm516x_handle_t handle,
                             am_zm516x_addr_t  *p_zb_addr,
                             uint8_t            value);

/**
 * \brief 读取指定地址 ZM516X 模块的 AD 转换值（临时命令：D7）
 *
 * 读取到的 AD 值为 AD 的转换值，需要参考 ZM516X 模块用户手册
 * 根据 ZM516X 模块 AD 的位数及参考电压转换为电压值
 *
 * \param[in]  handle    ZM516X 模块操作句柄
 * \param[in]  p_zb_addr 要读取的 ZM516X 模块的目标地址
 * \param[in]  chan      AD 的通道号
 * \param[out] p_value   AD 的转换值
 *
 * \retval  AM_OK    读取成功
 * \retval -AM_EPERM 读取失败
 */
am_err_t am_zm516x_ad_get (am_zm516x_handle_t handle,
                           am_zm516x_addr_t  *p_zb_addr,
                           uint8_t            chan,
                           uint16_t          *p_value);

/**
 * \brief 设置 ZM516X 模块进入睡眠模式（临时命令：D8）
 *
 * \param[in] handle ZM516X 模块操作句柄
 *
 * \return 无
 */
void am_zm516x_enter_sleep (am_zm516x_handle_t handle);

/**
 * \brief 设置ZM516X模块的通讯模式（临时命令：D9）
 *
 * \param[in] handle ZM516X模块操作句柄
 * \param[in] mode   模块的通讯模式
 *
 * \retval  AM_OK    设置成功
 * \retval -AM_EPERM 设置失败
 */
am_err_t am_zm516x_mode_set (am_zm516x_handle_t    handle,
                             am_zm516x_comm_mode_t mode);

/**
 * \brief 读取指定地址 ZM516X 模块的信号强度（临时命令：DA）
 *
 * \param[in]  handle    ZM516X 模块操作句柄
 * \param[in]  p_zb_addr 要读取的本机 ZM516X 模块与目标地址之间的信号强度
 * \param[out] p_signal  信号强度
 *
 * \retval  AM_OK    读取成功
 * \retval -AM_EPERM 读取失败
 */
am_err_t am_zm516x_sigal_get (am_zm516x_handle_t handle,
                              am_zm516x_addr_t  *p_zb_addr,
                              uint8_t           *p_signal);

/**
 * \brief 初始化 ZM516X 模块驱动
 *
 * \param[in] p_dev       ZM516X 模块设备结构体
 * \param[in] p_info      ZM516X 模块实例信息
 * \param[in] uart_handle 操作 ZM516X 模块的串口句柄
 *
 * \return ZM516X 标准服务句柄，若为 NULL，表明初始化失败
 */
am_zm516x_handle_t am_zm516x_init (am_zm516x_dev_t            *p_dev,
                                   const am_zm516x_dev_info_t *p_info,
                                   am_uart_handle_t            uart_handle);

/**
 * \brief 解初始化 ZM516X 模块驱动
 *
 * \param[in] handle 通过am_zm516x_init函数获取的ZM516X句柄
 *
 * \retval  AM_OK     解初始化成功
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_zm516x_deinit (am_zm516x_handle_t handle);

/** @} am_if_zm516x */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZM516X_H */

/* end of file */
