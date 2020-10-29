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
 * \brief LoRa LoRa modem registers and bits definitions
 *
 * \internal
 * \par Modification history
 * - 1.0.0 19-10-29  zp,first implementation.
 * - 1.0.1 20-09-23  yg,adaptation zsl42x.
 * \endinternal
 */

#ifndef __AM_ZSL42X_LORA_H
#define __AM_ZSL42X_LORA_H

#include "am_types.h"
#include "am_zsl42x_lpsoftimer.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name LoRa error number
 * @{
 */
#define AM_ZSL42X_LORA_RET_OK          0       /**< \brief 正确 */
#define AM_ZSL42X_LORA_RET_ERROR       -1      /**< \brief 错误 */
#define AM_ZSL42X_LORA_ESRCH           3       /**< \brief 进程不存在 */
#define AM_ZSL42X_LORA_RET_EAGAIN      11      /**< \brief 资源不可用，需重试 */
#define AM_ZSL42X_LORA_ENOMEM          12      /**< \brief 空间（内存）不足 */
#define AM_ZSL42X_LORA_RET_EBUSY       16      /**< \brief 设备或资源忙 */
#define AM_ZSL42X_LORA_RET_EINVAL      22      /**< \brief 无效参数 */
#define AM_ZSL42X_LORA_ENOTSUP         35      /**< \brief 不支持 */
#define AM_ZSL42X_LORA_ENOBUFS         55      /**< \brief 缓冲空间不足 */
/** @} */

/**
 * \name LoRa bool type
 * @{
 */
#ifndef AM_ZSL42X_LORA_TRUE
#define AM_ZSL42X_LORA_TRUE    1
#endif

#ifndef AM_ZSL42X_LORA_FALSE
#define AM_ZSL42X_LORA_FALSE   0
#endif

/** \brief 求结构体成员的偏移*/
#define AM_ZSL42X_LORA_OFFSET(structure, member) ((size_t)(&(((structure *)0)->member)))

/**
 * \brief 通过结构体成员指针获取包含该结构体成员的结构体
 */
#define AM_ZSL42X_LORA_CONTAINER_OF(ptr, type, member) \
            ((type *)((char *)(ptr) - AM_ZSL42X_LORA_OFFSET(type,member)))

/**
 * \brief 计算数组元素个数
 */
#define AM_ZSL42X_LORA_NELEMENTS(array) (sizeof (array) / sizeof ((array) [0]))

/**
 * \brief 射频调制模式
 */
typedef enum {
    AM_ZSL42X_LORA_MODEM_FSK = 0,     /**< \brief FSK模式 */
    AM_ZSL42X_LORA_MODEM_LORA,        /**< \brief LORA模式 */
} am_zsl42x_lora_server_radio_modems_t;

/**
 * \brief 射频模块工作状态
 */
typedef enum {
    AM_ZSL42X_LORA_RF_IDLE_STATE = 0,     /**< \brief 空闲状态  */
    AM_ZSL42X_LORA_RF_RX_RUNNING_STATE,   /**< \brief 接收状态  */
    AM_ZSL42X_LORA_RF_TX_RUNNING_STATE,   /**< \brief 发送状态  */
    AM_ZSL42X_LORA_RF_CAD_STATE,          /**< \brief CAD检测状态  */
    AM_ZSL42X_LORA_RF_SLEEP_STATE         /**< \brief 睡眠状态  */
} am_zsl42x_lora_server_radio_state_t;

/**
 * \brief lora 带宽
 */
typedef enum {
    AM_ZSL42X_LORA_BW_125 = 0,    /**< \brief 125KHZ */
    AM_ZSL42X_LORA_BW_250,        /**< \brief 250KHZ */
    AM_ZSL42X_LORA_BW_500,        /**< \brief 500KHZ */
    AM_ZSL42X_LORA_BW_200,        /**< \brief 200KHz */
    AM_ZSL42X_LORA_BW_400,        /**< \brief 400KHz */
    AM_ZSL42X_LORA_BW_800,        /**< \brief 800KHZ */
    AM_ZSL42X_LORA_BW_1600,       /**< \brief 1600KHZ */
} am_zsl42x_lora_server_bw_t;

/**
 * \brief lora 扩频因子
 */
typedef enum {
    AM_ZSL42X_LORA_SERVER_SF5 = 0x05,        /**< \brief 扩频因子5 */
    AM_ZSL42X_LORA_SERVER_SF6,               /**< \brief 扩频因子6 */
    AM_ZSL42X_LORA_SERVER_SF7,               /**< \brief 扩频因子7 */
    AM_ZSL42X_LORA_SERVER_SF8,               /**< \brief 扩频因子8 */
    AM_ZSL42X_LORA_SERVER_SF9,               /**< \brief 扩频因子9 */
    AM_ZSL42X_LORA_SERVER_SF10,              /**< \brief 扩频因子10 */
    AM_ZSL42X_LORA_SERVER_SF11,              /**< \brief 扩频因子11 */
    AM_ZSL42X_LORA_SERVER_SF12               /**< \brief 扩频因子12 */
} am_zsl42x_lora_server_sf_t;

/**
 * \brief lora 编码率
 */
typedef enum {
    AM_ZSL42X_LORA_SERVER_CR_4_5    = 0x01,    /**< \brief 编码率4/5 */
    AM_ZSL42X_LORA_SERVER_CR_4_6,              /**< \brief 编码率4/6 */
    AM_ZSL42X_LORA_SERVER_CR_4_7,              /**< \brief 编码率4/7 */
    AM_ZSL42X_LORA_SERVER_CR_4_8,              /**< \brief 编码率4/8 */
    AM_ZSL42X_LORA_SERVER_CR_LI_4_5,           /**< \brief 长交错编码率4/5 */
    AM_ZSL42X_LORA_SERVER_CR_LI_4_6,           /**< \brief 长交错编码率4/6 */
    AM_ZSL42X_LORA_SERVER_CR_LI_4_7,           /**< \brief 长交错编码率4/7 */
} am_zsl42x_lora_server_cr_t;

/** \brief LoRa私网同步字 */
#define AM_ZSL42X_LORA_SERVER_MAC_PRIVATE_SYNCWORD        0x12

/** \brief LoRa公网同步字 */
#define AM_ZSL42X_LORA_SERVER_MAC_PUBLIC_SYNCWORD         0x34

/**
 * \brief 射频事件回调
 */
typedef struct {

    /**
     * \brief 传输完成回调函数原型
     *
     * \param[in] p_arg : 传入回调函数的参数
     *
     * \return    无
     */
    void (*pfn_tx_done_cb) (void *p_arg);

    /**< \brief 传输完成回调参数 */
    void *p_tx_done_arg;

    /**
     * \brief 传输超时回调函数原型
     *
     * \param[in] p_arg : 传入回调函数的参数
     *
     * \return    无
     */
    void (*pfn_tx_timeout_cb) (void *p_arg);

    /**< \brief 传输超时回调参数 */
    void *p_tx_timeout_arg;

    /**
     * \brief 接收完成回调函数原型
     *
     * \param[in] p_arg     : 传入回调函数的参数
     * \param[in] p_payload : 指向接收缓冲区
     * \param[in] size      : 接收到的数据大小
     * \param[in] rssi      : 接收该帧时的计算得的 RSSI 值 [dBm]
     * \param[in] snr       : 信噪比
     *                        FSK : N/A (set to 0)
     *                        LoRa: SNR value in dB
     * \return     无
     */
    void (*pfn_rx_done_cb) (void       *p_arg,
                            uint8_t    *p_payload,
                            uint16_t    size,
                            int16_t     rssi,
                            int8_t      snr);

    /**< \brief 接收完成回调参数 */
    void *p_rx_done_arg;

    /**
     * \brief 接收超时回调函数原型
     *
     * \param[in] p_arg : 传入回调函数的参数
     *
     * \return    无
     */
    void (*pfn_rx_timeout_cb) (void *p_arg);

    /**< \brief 接收超时回调参数 */
    void *p_rx_timeout_arg;

    /**
     * \brief 接收错误回调函数原型
     *
     * \param[in] p_arg : 传入回调函数的参数
     *
     * \return    无
     */
    void (*pfn_rx_error_cb) (void *p_arg);

    /**< \brief 接收错误回调参数 */
    void *p_rx_error_arg;

    /**
     * \brief FHSS跳频回调函数原型
     *
     * \param[in] p_arg           : 传入回调函数的参数
     * \param[in] current_channel : 当前信道的编号, 从1开始, 每跳频一次递增+1
     *
     * \return    无
     */
    void (*pfn_fhss_change_channel_cb) (void *p_arg, uint8_t current_channel);

    /**< \brief FHSS跳频回调参数 */
    void *p_fhss_change_channel_arg;

    /**
     * \brief 信道活跃检测完成回调函数原型
     *
     * \param[in] p_arg                     : 传入回调函数的参数
     * \param[in] channel_activity_detected : 是否活跃[1:信道活跃，0：信道不活跃]
     *
     * \return    无
     */
    void (*pfn_cad_done_cb) (void *p_arg, uint8_t channel_activity_detected);

    /**< \brief 信道活跃检测完成回调参数 */
    void *p_cad_done_arg;

    /**
     * \brief 有效报头回调函数原型
     *
     * \param[in] p_arg : 传入回调函数的参数
     *
     * \return    无
     */
    void (*pfn_valid_header_cb) (void *p_arg);

    /**< \brief 有效报头回调参数 */
    void *p_valid_header_arg;

    /**
     * \brief 有效同步字回调函数原型
     */
    void (*pfn_valid_syncword_cb) (void *p_arg);

    /** \brief 有效同步字回调参数 */
    void *p_valid_syncword_arg;

    /**
     * \brief 测距完成回调原型
     *
     * \param[in] p_arg      : 传入回调函数的参数
     * \param[in] raw_result : 测距原始结果
     * \param[in] rssi       : 接收该帧时的计算得的 RSSI 值 [dBm]
     * \param[in] snr        : 信噪比
     *                          FSK : N/A (set to 0)
     *                          LoRa: SNR value in dB
     *
     * \return    无
     */
    void (*pfn_ranging_done_cb) (void    *p_arg,
                                 float    raw_result,
                                 int16_t  rssi,
                                 int8_t   snr);

    /** \brief 测距完成回调参数 */
    void *p_ranging_done_arg;

    /**
     * \brief 测距错误回调原型
     * \param[in] p_arg : 传入回调函数的参数
     * \return 无
     */
    void (*pfn_ranging_error_cb) (void *p_arg);

    /** \brief 测距错误回调参数 */
    void *p_ranging_error_arg;

} am_zsl42x_lora_radio_events_t;

/**
 * \brief lora 服务函数定义
 */
typedef struct {

    /**
     * \brief 复位射频模块
     *
     * \param[in] p_drv  : lora驱动参数
     *
     * \retval AM_ZSL42X_LORA_TRUE  复位成功
     * \retval AM_ZSL42X_LORA_FALSE 复位失败
     */
    int (*pfn_reset) (void *p_drv);

    /**
     * \brief 唤醒复位初始化
     *
     * \param[in] p_drv  : lora驱动参数
     * \param[in] p_arg  : 复位参数
     *
     * \retval AM_ZSL42X_LORA_TRUE  : 复位成功
     * \retval AM_ZSL42X_LORA_FALSE : 复位失败
     */
    int (*pfn_wakeup_init) (void *p_dev, void *p_arg);

    /**
     * \brief 唤醒射频模块
     *
     * \param[in] p_drv  : lora驱动参数
     *
     * \retval AM_ZSL42X_LORA_TRUE  : 设置成功
     * \retval AM_ZSL42X_LORA_FALSE : 设置失败
     */
    int (*pfn_wakeup) (void *p_drv);

    /**
     * \brief 射频模式设置
     *
     * \param[in] p_drv  : lora驱动参数
     * \param[in] modem  : 射频模式 [0: FSK, 1: LoRa]
     *
     * \retval AM_ZSL42X_LORA_TRUE  : 设置成功
     * \retval AM_ZSL42X_LORA_FALSE : 设置成功
     */
    int (*pfn_mode_set) (void *p_drv, am_zsl42x_lora_server_radio_modems_t modem);

    /**
     * \brief 射频模式获取
     *
     * \param[in] p_drv    : lora驱动参数
     * \param[in] p_modem  : 射频模式
     *
     * \retval AM_ZSL42X_LORA_TRUE  : 获取成功
     * \retval AM_ZSL42X_LORA_FALSE : 获取失败
     */
    int (*pfn_mode_get) (void *p_drv, am_zsl42x_lora_server_radio_modems_t *p_modem);

    /**
     * \brief 获取射频状态
     *
     * \param[in]  p_drv     : lora驱动参数
     * \param[out] p_status  : 射频状态
     *
     * \retval AM_ZSL42X_LORA_TRUE  : 获取成功
     * \retval AM_ZSL42X_LORA_FALSE : 获取失败
     */
    int (*pfn_status_get) (void *p_drv, am_zsl42x_lora_server_radio_state_t *p_status);

    /**
     * \brief 发送传输参数设置
     *
     * \remark 使用LoRa调制模式时仅支持 125、250及500kHz
     *
     * \param[in] p_drv        : lora驱动参数
     * \param[in] modem        : 使用的射频模式 [0: FSK, 1: LoRa]
     * \param[in] power        : 设置功率 [dBm]
     * \param[in] freq_divider : 设置分频器 (FSK only)
     *                            FSK : [Hz]
     *                            LoRa: 0
     * \param[in] bandwidth    : 设置带宽 (LoRa only)
     *                            FSK : 0
     *                            LoRa: [0: 125 kHz, 1: 250 kHz,
     *                                   2: 500 kHz, 3: Reserved]
     * \param[in] datarate     : 设置数据速率
     *                            FSK : 600..300000 bits/s
     *                            LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
     *                                  10: 1024, 11: 2048, 12: 4096  chips]
     * \param[in] coderate     : 设置编码率 (LoRa only)
     *                            FSK : N/A (set to 0)
     *                            LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
     * \param[in] preamble_len : 设置前导码长度
     *                            FSK : Number of bytes
     *                            LoRa: Length in symbols (the hardware adds 4 more symbols)
     * \param[in] fix_len      : 固定包长使能 [0: variable, 1: fixed]
     * \param[in] crc_on       : CRC校验使能 [0: OFF, 1: ON]
     * \param[in] freq_hop_on  : 内部跳频使能
     *                            FSK : N/A (set to 0)
     *                            LoRa: [0: OFF, 1: ON]
     * \param[in] hop_period   : 每次跳频的符号周期
     *                            FSK : N/A (set to 0)
     *                            LoRa: Number of symbols
     * \param[in] iq_inverted  : 转变 IQ 信号 (LoRa only)
     *                            FSK : N/A (set to 0)
     *                            LoRa: [0: not inverted, 1: inverted]
     * \param[in] timeout      : 传输超时 [ms]
     *
     * \retval    AM_ZSL42X_LORA_TRUE   设置成功
     * \retval    AM_ZSL42X_LORA_FALSE  设置失败
     */
    int (*pfn_tx_config_set) (
        void    *p_drv, am_zsl42x_lora_server_radio_modems_t modem,
        int8_t   power,           uint32_t freq_divider,
        uint32_t bandwidth,       uint32_t datarate,
        uint8_t  coderate,        uint16_t preamble_len,
        uint8_t  fix_len,         uint8_t  crc_on,
        uint8_t  freq_hop_on,     uint8_t  hop_period,
        uint8_t  iq_inverted,     uint32_t timeout);

    /**
     * \brief 接收参数设置
     *
     * \remark 使用LoRa调制模式时仅支持 125、250及500kHz
     *
     * \param[in] p_drv         : lora驱动参数
     * \param[in] modem         : 使用的射频模式 [0: FSK, 1: LoRa]
     * \param[in] bandwidth     : 设置带宽
     *                             FSK : >= 2600 and <= 250000 Hz
     *                             LoRa: [0: 125 kHz, 1: 250 kHz,
     *                                    2: 500 kHz, 3: Reserved]
     * \param[in] datarate      : 设置数据速率
     *                             FSK : 600..300000 bits/s
     *                             LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
     *                                   10: 1024, 11: 2048, 12: 4096  chips]
     * \param[in] coderate      : 设置编码率 (LoRa only)
     *                             FSK : N/A (set to 0)
     *                             LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
     * \param[in] bandwidth_afc : 设置 AFC 带宽 (FSK only)
     *                             FSK : >= 2600 and <= 250000 Hz
     *                             LoRa: N/A (set to 0)
     * \param[in] preamble_len  : 设置前导码长度
     *                             FSK : Number of bytes
     *                             LoRa: Length in symbols (the hardware adds 4 more symbols)
     * \param[in] symb_timeout  : 设置单次接收超时值 (LoRa only)
     *                             FSK : N/A (set to 0)
     *                             LoRa: timeout in symbols
     * \param[in] fix_len       : 固有长度使能 [0: variable, 1: fixed]
     * \param[in] payload_len   : 设置负载长度 (当固有长度使能时)
     * \param[in] crc_on        : CRC校验使能 [0: OFF, 1: ON]
     * \param[in] freq_hop_on   : 内部包跳频使能
     *                             FSK : N/A (set to 0)
     *                             LoRa: [0: OFF, 1: ON]
     * \param[in] hop_period    : 每次跳频的符号周期
     *                             FSK : N/A (set to 0)
     *                             LoRa: Number of symbols
     * \param[in] iq_inverted   : 转变 IQ 信号 (LoRa only)
     *                             FSK : N/A (set to 0)
     *                             LoRa: [0: not inverted, 1: inverted]
     * \param[in] rx_continuous : 设置连续接收模式
     *                             [false: single mode, true: continuous mode]
     *
     * \retval    AM_ZSL42X_LORA_TRUE  : 设置成功
     * \retval    AM_ZSL42X_LORA_FALSE : 设置失败
     */
    int (*pfn_rx_config_set) (
        void    *p_drv,  am_zsl42x_lora_server_radio_modems_t modem,
        uint32_t bandwidth,       uint32_t datarate,
        uint8_t  coderate,        uint32_t bandwidth_afc,
        uint16_t preamble_len,    uint16_t symb_timeout,
        uint8_t  fix_len,         uint8_t  payload_len,
        uint8_t  crc_on,          uint8_t  freq_hop_on,
        uint8_t  hop_period,      uint8_t  iq_inverted,
        uint8_t  rx_continuous);

    /**
     * \brief 周期唤醒接收设置
     *
     * \param[in] p_drv         : lora驱动参数
     * \param[in] rx_period     : 接收时间[ms]
     * \param[in] sleep_period  : 睡眠时间[ms]
     *
     * \retval    AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval    AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_rxdutycycle) (void      *p_drv,
                            uint32_t   rx_period,
                            uint32_t   sleep_period);

    /**
     * \brief 启动射频接收(连续、单次)
     *
     * \param[in] p_drv    : lora驱动参数
     * \param[in] timeout  : 接收超时 [ms] (0:连续接收模式。others：单次超时接收模式)
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_receive) (void *p_drv, uint32_t timeout);

    /**
     * \brief 启动射频发送（单次）
     *
     * \param[in] p_drv   : lora驱动参数
     * \param[in] p_data  : 指向待发送数据的指针
     * \param[in] len     : 待发送数据长度
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_send) (void *p_drv, uint8_t *p_data, uint8_t len);

    /**
     * \brief 进入睡眠模式
     *
     * \param[in] p_drv        :lora驱动参数
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_sleep) (void *p_drv);

    /**
     * \brief 进入待机模式
     *
     * \param[in] p_drv        : lora驱动参数
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_standby) (void *p_drv);

    /**
     * \brief 启动一次信道活跃检测
     *
     * \param[in] p_drv        : lora驱动参数
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_cad_start) (void *p_drv);

    /**
     * \brief 设置RF频率(通信信道频率)
     *
     * \param[in] p_drv     : lora驱动参数
     * \param[in] frequency : RF频率(Hz)
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_channel_set) (void *p_drv, uint32_t frequency);

    /**
     * \brief 获取RF频率(通信信道频率)
     *
     * \param[in]  p_drv  : lora驱动参数
     * \param[out] p_freq : RF频率[Hz]
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_channel_get) (void *p_drv, uint32_t *p_freq);

    /**
     * \brief 检查通信信道是否空闲
     *
     * \param[in]  p_drv        : lora驱动参数
     * \param[in]  modem        : 射频调制模式
     * \param[in]  freq         : 信道频率
     * \param[in]  rssi_thresh  : rssi临界值（用来判断是否空闲的依据）
     * \param[out] p_state      : 空闲状态[0：忙碌, 1： 空闲]
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_is_channel_free) (void                   *p_drv,
                                am_zsl42x_lora_server_radio_modems_t  modem,
                                uint32_t                freq,
                                int16_t                 rssi_thresh,
                                uint8_t                *p_state);

    /**
     * \brief 检查硬件是否支持给出的射频频率
     *
     * \param[in]  p_drv      : lora驱动参数
     * \param[in]  frequency  : 频率
     * \param[out] p_state    : 是否支持[0：不支持, 1： 支持]
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_rf_frequency_check) (void     *p_drv,
                                   uint32_t  frequency,
                                   uint8_t  *p_state);

    /**
     * \brief 写特定地址的射频寄存器 (按字节写入)
     *
     * \param[in] p_drv    : lora驱动参数
     * \param[in] addr     : 写数据地址
     * \param[in] p_data   : 写入的数据
     * \param[in] size     : 写入数据的大小[单位：字节]
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_reg_write) (void      *p_drv,
                          uint16_t   addr,
                          uint8_t   *p_data,
                          uint8_t    size);

    /**
     * \brief 读特定地址的射频寄存器(按字节读取)
     *
     * \param[in]  p_drv    : lora驱动参数
     * \param[in]  addr     : 读取地址
     * \param[out] p_data   : 读取的数据
     * \param[in]  size     : 读取数据的大小[单位：字节]
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_reg_read) (void       *p_drv,
                         uint16_t    addr,
                         uint8_t    *p_data,
                         uint8_t     size);

    /**
     * \brief 从起始地址写多个寄存器
     *
     * \param[in] p_drv    : lora驱动参数
     * \param[in] addr     : 写数据地址
     * \param[in] p_data   : 写入的数据
     * \param[in] size     : 写入数据的大小[单位：字节]
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_buffer_write) (void      *p_drv,
                             uint8_t    addr,
                             uint8_t   *p_data,
                             uint8_t    size);

    /**
     * \brief 从起始地址读多个寄存器
     *
     * \param[in]  p_drv    : lora驱动参数
     * \param[in]  addr     : 读取地址
     * \param[out] p_data   : 读取的数据
     * \param[in]  size     : 读取数据的大小[单位：字节]
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_buffer_read) (void     *p_drv,
                            uint8_t   addr,
                            uint8_t  *p_data,
                            uint8_t   size);

    /**
     * \brief  设置最大负载长度
     *
     * \param[in] p_drv         : lora驱动参数
     * \param[in] modem         : 使用的射频模式 [0: FSK, 1: LoRa]
     * \param[in] payload_size  : 负载最大字节数
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 设置成功
     * \retval   AM_ZSL42X_LORA_FALSE : 设置失败
     */
    int (*pfn_max_payload_length_set) (void                   *p_dev,
                         am_zsl42x_lora_server_radio_modems_t  modem,
                                       uint8_t                 payload_size);

    /**
     * \brief 设置公网或私网
     *
     * \param[in] p_drv  : lora驱动参数
     * \param[in] enable : 1 公网， 0 私网
     *
     * \note     只适用于LoRa模式
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 设置成功
     * \retval   AM_ZSL42X_LORA_FALSE : 设置失败
     */
    int (*pfn_public_network_set) (void *p_drv, uint8_t enable);

    /**
     * \brief 设置连续接收
     *
     * \param[in] p_drv  : lora驱动参数
     * \param[in] enable : 1:使能， 0 禁能
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 设置成功
     * \retval   AM_ZSL42X_LORA_FALSE : 设置失败
     */
    int (*pfn_rx_continue_set) (void *p_drv, uint8_t enable);

    /**
     * \brief 生成32位随机数
     *
     * \param[in]  p_drv    : lora驱动参数
     * \param[out] p_random : 32位随机数值
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_random) (void *p_drv, uint32_t *p_random);

    /**
     * \brief 计算给出负载对应的符号数
     *
     * \param[in]  p_drv            : lora驱动参数
     * \param[in]  pkt_len          : 负载包长度
     * \param[out] p_pkt_symbol_num : 有效负载的符号数
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_time_pkt_symbol_get) (void      *p_drv,
                                    uint8_t    pkt_len,
                                    uint16_t  *p_pkt_symbol_num);

    /**
     * \brief 计算给出负载的在空时间（ms）
     *
     * \param[in]  p_drv        : lora驱动参数
     * \param[in]  modem        : 射频模式
     * \param[in]  packet_len   : 负载包长度
     * \param[out] p_time       : 在空时间（us）
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_time_on_air_get) (void                   *p_drv,
                  am_zsl42x_lora_server_radio_modems_t  modem,
                                uint8_t                 packet_len,
                                uint64_t               *p_time);

    /**
     * \brief 设置符号超时时间
     *
     * \param[in] p_drv   : lora驱动参数
     * \param[in] ms      :  符号超时时间[单位：ms]
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_symb_timeout_set) (void *p_drv, uint32_t ms);

    /**
     * \brief 获取当前射频配置参数情况下单个符号时间,LoRa模式
     *
     * \param[in]  p_drv   : lora驱动参数
     * \param[out] p_time  : 单个符号时间[单位：us]
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_symbol_time_get) (void *p_drv, float *p_time);

    /**
     * \brief 设置射频进入连续传输模式(测试使用)
     *
     * \param[in] p_drv  : lora驱动参数
     * \param[in] freq   : 信道频率
     * \param[in] power  : 发射功率 [dBm]
     * \param[in] time   : 传输超时 [us]
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_tx_continuous_wave) (void     *p_drv,
                                   uint32_t  freq,
                                   int8_t    power,
                                   uint32_t  time);

    /**
     * \brief 获取当前的RSSI值
     *
     * \param[in] p_drv         : lora驱动参数
     * \param[in] modem         : 射频调制模式
     * \param[in] p_rssi_value  : RSSI值
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_rssi_get) (void                   *p_drv,
           am_zsl42x_lora_server_radio_modems_t  modem,
                         int16_t                *p_rssi_value);

    /**
     * \brief 设置同步字(syncword)
     *
     * \param[in] p_drv       : lora驱动参数
     * \param[in] p_wordbuff  : 指向同步字数组的指针
     * \param[in] num         : 同步字个数
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_syncword_set) (void     *p_drv,
                             uint8_t  *p_syncword,
                             uint8_t   num);

    /**
     * \brief 重传(如果size不为0，转换一部分负载到FIFO中)
     *
     * \param[in] p_drv    : lora驱动参数
     * \param[in] p_txbuff : 重传的数据数组指针
     * \param[in] len      : 数据长度[单位：字节]
     * \param[in] offset   : 数据偏移
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_tx_repeat_set) (void      *p_drv,
                              uint8_t   *p_txbuff,
                              uint8_t    len,
                              uint8_t    offset);

    /**
     * \brief 获取lora数据的前几个字节数据
     *
     * \param[in] p_drv     : lora驱动参数
     * \param[in] p_buffer  : 获取的数据
     * \param[in] size      ： 数据长度
     *
     * \retval   AM_ZSL42X_LORA_TRUE  : 操作成功
     * \retval   AM_ZSL42X_LORA_FALSE : 操作失败
     */
    int (*pfn_recv_nbbytes_get) (void *p_drv, uint8_t *p_buffer, uint8_t size);

    /**
     * \brief 获取射频唤醒时间
     *
     * \param[in] p_drv     : lora驱动参数
     * \param[in] p_time    : 获取的唤醒时间
     *
     * \return              : 返回唤醒时间值
     */
    int (*pfn_wakeup_time_get) (void *p_drv);
} am_zsl42x_lora_server_funcs_t;

/**
 * \brief lora 标准服务设备结构体
 */
typedef struct am_zsl42x_lora_server_dev {

    /** \brief 指向射频服务函数 */
    am_zsl42x_lora_server_funcs_t        *p_server_funcs;

    /** \brief 指向射频回调事件 */
    const am_zsl42x_lora_radio_events_t  *p_radio_events;

    /** \brief 用于保存底层硬件驱动设备结构体的指针  */
    void                                 *p_hw_drv;
} am_zsl42x_lora_server_dev_t;

/**< \brief lora 标准设备句柄 */
typedef am_zsl42x_lora_server_dev_t *am_zsl42x_lora_server_handle_t;

/**
 * @addtogroup am_if_sx1266_regs_zsl42x_lora
 * @copydoc am_sx1266_regs_zsl42x_lora.h
 * @{
 */

/**
 * @addtogroup am_if_lora
 * @copydoc am_lora.h
 * @{
 */

/**
 * \brief 射频驱动支持的调制方式
 * @{
 */
#define AM_ZSL42X_LORA_RADIO_PACKET_FSK      0x00   /**< \brief FSK调制及包类型 */
#define AM_ZSL42X_LORA_RADIO_PACKET_LORA     0x01   /**< \brief LORA调制及包类型 */
/** @} */

/**
 * \brief 相关时间的定义
 * @{
 */
#define AM_ZSL42X_LORA_RADIO_OSC_STARTUP  0  /**< \brief 射频睡眠模式的唤醒时间[ms] */
#define AM_ZSL42X_LORA_RADIO_SLEEP_TO_RX  3  /**< \brief 射频PLL锁及模式准备时间(可能会随温度变化)[ms] */

/**< \brief 射频完成唤醒时间带温度补偿临界值[ms] */
#define AM_ZSL42X_LORA_RADIO_WAKEUP_TIME (AM_ZSL42X_LORA_RADIO_OSC_STARTUP + \
                                          AM_ZSL42X_LORA_RADIO_SLEEP_TO_RX)
/** @} */

/**
 * \brief 晶振频率
 * @{
 */
#define AM_ZSL42X_LORA_XTAL_FREQ           32000000
/** @} */

/**
 * \brief 其他宏定义
 * @{
 */
#define AM_ZSL42X_LORA_LORA_CR_4_5            1
#define AM_ZSL42X_LORA_LORA_CR_4_6            2
#define AM_ZSL42X_LORA_LORA_CR_4_7            3
#define AM_ZSL42X_LORA_LORA_CR_4_8            4

#define AM_ZSL42X_LORA_XTAL_FREQ              32000000
#define AM_ZSL42X_LORA_RX_BUFFER_SIZE         256
/** @} */

/**
 * \brief 射频中频临界
 */
#define __ZSL42X_LORA_RF_MID_BAND_THRESH      525000000

/**
 * \brief 计算 RSSI 需要用的常量 (RSSI偏移值)
 */
#define __ZSL42X_LORA_RSSI_OFFSET_LF          -164
#define __ZSL42X_LORA_RSSI_OFFSET_HF          -157

#define __ZSL42X_LORA_MAX_PAYLOAD_LRNGRH       0XFE


#define AM_ZSL42X_LORA_CRC_IBM_SEED     0xFFFF /** \brief 计算IBM类型CRC的LFSR初始化值 */
#define AM_ZSL42X_LORA_CRC_CCITT_SEED   0x1D0F /** \brief 计算CCIT类型CRC的LFSR初始化值 */

#define AM_ZSL42X_LORA_CRC_POLYNOMIAL_IBM    0x8005  /**< \brief 计算IBM CRC的多项式 */
#define AM_ZSL42X_LORA_CRC_POLYNOMIAL_CCITT  0x1021  /**< \breif 计算CCIT CRC的多项式 */

/** \brief void (*pfn) (void *) */
typedef void (*am_zsl42x_lora_pfnvoid_t) (void *);

/**
 *  \brief 数据包射频调制类型--FSK相关配置
 */
typedef struct {

    uint32_t datarate;           /**< \brief 传输速率 */
    uint32_t bw;                 /**< \brief 带宽 */
    uint32_t bw_afc;             /**< \brief AFC带宽 */
    uint32_t freq_deviation;     /**< \brief 偏差频率 */

    uint8_t  addr_filter_enable; /**< \brief 地址过滤（AM_TRUE:使能, AM_FALSE:禁能） */
    uint8_t  node_addr;          /**< \brief 节点地址 */
    uint8_t  broadcast_addr;     /**< \brief 广播地址 */
    uint8_t  detector_length;    /**< \brief 检测器长度 */
    uint8_t  syncword_number;    /**< \brief 同步字数量  */
    uint8_t *p_syncword;         /**< \brief 指向同步字数组的指针  */

    uint8_t  whiteng_enable;  /**< \brief whiteng使能(AM_TRUE:使能, AM_FALSE:禁能)*/
    uint8_t  rx_continuous;   /**< \brief 连续接收 */
    uint8_t  tx_timeout;      /**< \brief 发送超时时间 */
    uint8_t  crc_length;      /**< \brief Size of the CRC */

    uint8_t  radio_modshapings;  /**< \brief  modulation shaping parameter */
} am_zsl42x_lora_settings_fsk_t;

/**
 *  数据包射频调制类型--lora相关配置
 */
typedef struct {

    uint8_t  sf;                   /**< \brief 扩频因子 */
    uint32_t bw;                   /**< \brief 带宽 */
    uint8_t  cr;                   /**< \brief 编码率 */
    uint8_t  lowdatarate_optimize; /**< \brief 速率优化(AM_TRUE:使能, AM_FALSE:禁能)*/
    uint8_t  network_num;          /**< \brief 网络编码 */
    uint8_t  freq_hop_on;          /**< \brief 跳频使能(AM_TRUE:使能, AM_FALSE:禁能)*/
    uint8_t  hop_period;           /**< \brief 跳频周期 */
    uint8_t  symbol_timeout;       /**< \brief 符号超时时间 */
    uint32_t tx_timeout;           /**< \brief 发送超时时间 */
} am_zsl42x_lora_settings_lora_t;

/**
 * \brief zsl42x lora 配置信息
 */
typedef struct {

    uint8_t   packet_type;         /**< \brief 数据包射频调制类型(0：FKS, 1： LoRa) */
    uint32_t  frequency;           /**< \brief 信道频率 */
    int8_t    tx_power;            /**< \brief TX功耗 */
    uint16_t  tx_ramptime;         /**< \brief 斜坡时间 */
    uint8_t   tx_base_addr;        /**< \brief TX数据在FIFO中的缓存基地址 */
    uint8_t   rx_base_addr;        /**< \brief RX数据在FIFO中的缓存基地址 */
    uint8_t   idel_mode;           /**< \brief 设备空闲时所处模式设置（部分芯片特有） */

    uint8_t   iq_inverted_enable; /**< \brief 信号反转(AM_TRUE:使能, AM_FALSE:禁能)*/
    uint16_t  preamble_length;    /**< \brief 前导符长度（数量）*/

    uint8_t   header_type;        /**< \brief 头类型(0 :可变长度包, 1:固定长度包) */
    uint8_t   payloadlength;      /**< \brief 有效负载大小(数量)*/
    uint8_t   crc_enable;         /**< \brief CRC使能(AM_TRUE:使能, AM_FALSE:禁能)*/
    uint8_t   network_current;    /**< \brief 网络类型(公网、私网) */
    uint8_t   network_previous;   /**< \brief 网络类型(公网、私网) */

    am_zsl42x_lora_settings_fsk_t  fsk_setting;    /**< \brief FSK较为特有的相关设置 */
    am_zsl42x_lora_settings_lora_t lora_setting;   /**< \brief LORA较为特有的相关设置 */
} am_zsl42x_lora_settings_t;

/**
 * \brief 射频FSK包处理结构
 */
typedef struct {
    uint8_t  preamble_detected;
    uint8_t  sync_word_detected;
    int8_t   rssi_value;
    int32_t  afc_value;
    uint8_t  rx_gain;
    uint16_t size;
    uint16_t nb_bytes;
    uint8_t  fifo_thresh;
    uint8_t  chunk_size;
} am_zsl42x_lora_packet_fsk_deal_t;

/**
 * \brief 射频LoRa包处理结构
 */
typedef struct {
    int8_t   snr_value;
    int16_t  rssi_value;
    uint8_t  size;
} am_zsl42x_lora_packet_lora_deal_t;

/**
 * \brief zsl42x lora 射频芯片类型
 */
typedef enum {
    AM_ZSL42X_LORA_CORE_TYPE_SX1261 = 0,
    AM_ZSL42X_LORA_CORE_TYPE_SX1262,
    AM_ZSL42X_LORA_CORE_TYPE_SX1268
} am_zsl42x_lora_core_type_t;

typedef struct {

    am_zsl42x_lora_core_type_t core_type;   /**< \brief 芯片器件编号 */

//  float    xta_pf;         /**< \brief XTA引脚电容选择(11.3pF~33.4pF,步进0.47pF) */
//  float    xtb_pf;         /**< \brief XTB引脚电容选择(11.3pF~33.4pF,步进0.47pF) */
    uint32_t nss_pin;        /**< \brief 片选引脚 */
    uint32_t reset_pin;      /**< \brief 复位引脚 */
    uint32_t busy_pin;       /**< \brief BUSY信号引脚 */
    uint32_t dio1_pin;       /**< \brief DIO1引脚 */
    uint32_t dio2_pin;       /**< \brief DIO2引脚 */
    uint32_t dio3_pin;       /**< \brief DIO3引脚 */
    uint32_t txen_pin;       /**< \brief txen引脚 */
    uint32_t rxen_pin;       /**< \brief rxen引脚 */
    uint32_t frequency;      /**< \brief ZSL42X_LORA使用的晶振频率 */

    uint8_t  calibrate_image;

    uint32_t irq_mask;
    uint32_t dio1_mask;
    uint32_t dio2_mask;
    uint32_t dio3_mask;

    void (*pfn_plfm_init)   (void *p_arg);          /**< \brief 相关引脚初始化函数 */
    void (*pfn_plfm_deinit) (void *p_arg);          /**< \brief 相关引脚去初始化函数 */
    int  (*pfn_gpio_set)    (int   pin, int value); /**< \brief 引脚电平设置函数指针 */
    int  (*pfn_gpio_get)    (int   pin);            /**< \brief 引脚电平获取函数指针 */

    int  (*pfn_dio_irq_enable)  (int pin);          /**< \brief 引脚中断使能 */
    int  (*pfn_dio_irq_disable) (int pin);          /**< \brief 引脚中断禁能 */
} am_zsl42x_lora_devinfo_t;

/** \brief ZSL42X LORA 驱动函数前置声明 */
typedef struct am_zsl42x_lora_drv_funcs  am_zsl42x_lora_drv_funcs_t;

/**
 * \brief lora 设备结构体
 */
typedef struct am_zsl42x_lora_dev {

    am_zsl42x_lora_server_dev_t     lora_dev;        /**< \brief 基类lora 实例 */
    am_zsl42x_lora_settings_t       setconfig;       /**< \brief lora设置参数 */
    am_zsl42x_lora_drv_funcs_t     *p_drv_funcs;     /**< \brief 底层驱动函数 */
    am_zsl42x_lora_pfnvoid_t        p_dio_irq;       /**< \brief DIO中断函数表 */
    const am_zsl42x_lora_devinfo_t *p_devinfo;       /**< \brief 指向设备信息常量的指针 */

    am_lpsoftimer_t            tx_timeout_timer;           /**< \brief 发送超时定时器 */
    am_lpsoftimer_t            rx_timeout_timer;           /**< \brief 接收超时定时器 */
    am_lpsoftimer_t            rx_timeout_sync_word_timer; /**< \brief 同步字超时定时器 */

    am_zsl42x_lora_packet_lora_deal_t lora_packet_deal;  /**< \brief lora包处理结构 */
    am_zsl42x_lora_packet_fsk_deal_t  fsk_packet_deal;   /**< \brief fsk包处理结构 */

    uint8_t                    rx_continuous; /**< \brief 连续接收  */
    uint8_t                    tx_continuous; /**< \brief 连续发送  */
    uint8_t                    cad_activity;  /**< \brief CAD检测结果 0：空闲  1：活跃  */
    uint8_t                    fhss_channel;  /**< \brief 跳频当前信道的编号  */

    am_zsl42x_lora_server_radio_state_t    state;        /**< \brief 射频工作状态 */

    /** \brief 收发缓冲区(保留5个字节校验有效帧头) */
    uint8_t                    rx_tx_buffer[AM_ZSL42X_LORA_RX_BUFFER_SIZE + 5];
    uint8_t                   *p_rx_tx_buffer;
} am_zsl42x_lora_dev_t;

/** \brief lora操作句柄定义 */
typedef am_zsl42x_lora_dev_t *am_zsl42x_lora_handle_t;

/**
 * \brief lora 底层驱动函数定义
 */
struct am_zsl42x_lora_drv_funcs {

    /**
     * \brief  引脚是否有效
     *
     * \retval 1:有效;0:无效
     */
    int (*pfn_dio_is_valid) (void *p_drv, int dio);

    /**
     * \brief  清除所有DIO引脚中断标志
     */
    int (*pfn_int_flag_clr) (void *p_drv);

    /**
     * \brief  复位
     */
    int (*pfn_reset) (void *p_drv);

    /**
     * \brief  读寄存器
     */
    int (*pfn_buf_read) (am_zsl42x_lora_dev_t  *p_dev,
                         uint8_t               *p_addr,
                         uint32_t               addr_len,
                         uint8_t               *p_buff,
                         uint32_t               buff_len);

    /**
     * \brief  写寄存器
     */
    int (*pfn_buf_write) (am_zsl42x_lora_dev_t  *p_dev,
                          uint8_t               *p_addr,
                          uint32_t               addr_len,
                          const uint8_t         *p_buff,
                          uint32_t               buff_len);
};

/*******************************************************************************
    ZSL42X LORA API
*******************************************************************************/

/**
 * \brief zsl42x lora 射频模块初始化
 *
 * \param[in] p_dev       : lora 设备结构体的指针
 * \param[in] p_devinfo   : 指向  lora 设备信息
 * \param[in] p_drv_funcs : 指向 驱动函数列表
 *
 * \return lora 服务操作句柄,如果为 NULL，表明初始化失败
 */
am_zsl42x_lora_handle_t am_zsl42x_lora_init (
                                  am_zsl42x_lora_dev_t         *p_dev,
                            const am_zsl42x_lora_devinfo_t     *p_devinfo,
                                  am_zsl42x_lora_drv_funcs_t   *p_drv_funcs);

/**
 * \brief zsl42x lora射频模块去初始化
 *
 * \param[in] handle : lora 服务句柄
 *
 * \return 无
 */
void am_zsl42x_lora_deinit (am_zsl42x_lora_handle_t handle);

/**
 * \brief Set size elements of dst array with value
 *
 * \remark avoid standard memset function only works on pointers that are aligned
 *
 * \param [out] dst   : Destination array
 * \param [in]  value : Default value
 * \param [in]  size  : Number of bytes to be copied
 */
void am_zsl42x_lora_memset1 (uint8_t *p_dst, uint8_t value, uint32_t size);

/**
 * \brief Copies size elements of src array to dst array
 *
 * \remark avoid standard memset function only works on pointers that are aligned
 *
 * \param [out] dst  :  Destination array
 * \param [in]  src  : Source array
 * \param [in]  size : Number of bytes to be copied
 */
void am_zsl42x_lora_memcpy1 (uint8_t *p_dst, const uint8_t *p_src, uint32_t size);

/**
 * \brief 回调事件设置
 *
 * \param[in] handle   : lora句柄
 * \param[in] p_events : 指向回调事件的指针
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 *
 */
am_static_inline
int am_zsl42x_lora_server_radio_events_set (am_zsl42x_lora_handle_t   handle,
                                const am_zsl42x_lora_radio_events_t  *p_events)
{
    if (p_events == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    handle->lora_dev.p_radio_events = p_events;

    return AM_ZSL42X_LORA_TRUE;
}

/**
 * \brief 复位
 *
 * \param[in] handle : lora句柄
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_reset (am_zsl42x_lora_handle_t handle)
{
    if (handle->lora_dev.p_server_funcs->pfn_reset == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_reset(handle->lora_dev.p_hw_drv);
}

/**
 * \brief 唤醒初始化
 *
 * \param[in] handle : lora句柄
 * \param[in] p_arg  : 初始化参数
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_wakeup_init(am_zsl42x_lora_handle_t handle, void *p_arg)
{
    if (handle->lora_dev.p_server_funcs->pfn_wakeup_init == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_wakeup_init(handle->lora_dev.p_hw_drv, p_arg);
}

/**
 * \brief 唤醒射频模块
 *
 * \param[in] handle : lora句柄
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_wakeup (am_zsl42x_lora_handle_t handle)
{
    if (handle->lora_dev.p_server_funcs->pfn_wakeup == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_wakeup(handle->lora_dev.p_hw_drv);
}

/**
 * \brief 射频模式设置
 *
 * \param[in] handle : lora句柄
 * \param[in] modem  : 射频模式 [0: FSK, 1: LoRa]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_modem_set(am_zsl42x_lora_handle_t  handle,
                       am_zsl42x_lora_server_radio_modems_t  modem)
{
    if (handle->lora_dev.p_server_funcs->pfn_mode_set == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_mode_set(handle->lora_dev.p_hw_drv, modem);
}

/**
 * \brief 射频模式设置
 *
 * \param[in] handle   : lora句柄
 * \param[in] p_modem  : 射频模式 [0: FSK, 1: LoRa]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_modem_get(am_zsl42x_lora_handle_t    handle,
                       am_zsl42x_lora_server_radio_modems_t   *p_modem)
{
    if (handle->lora_dev.p_server_funcs->pfn_mode_get == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_mode_get(handle->lora_dev.p_hw_drv, p_modem);
}

/**
 * \brief 获取射频模块状态
 *
 * \param[in]  handle   : lora句柄
 * \param[out] p_status : 状态数据
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_status_get (am_zsl42x_lora_handle_t    handle,
                        am_zsl42x_lora_server_radio_state_t     *p_status)
{
    if (handle->lora_dev.p_server_funcs->pfn_status_get == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_status_get(handle->lora_dev.p_hw_drv, p_status);
}

/**
 * \brief 接收参数设置
 *
 * \remark 使用LoRa调制模式时仅支持 125、250及500kHz
 *
 * \param[in] handle        : lora驱动参数
 * \param[in] modem         : 使用的射频模式 [0: FSK, 1: LoRa]
 * \param[in] bandwidth     : 设置带宽
 *                             FSK : >= 2600 and <= 250000 Hz
 *                             LoRa: [0: 125 kHz, 1: 250 kHz,
 *                                    2: 500 kHz, 3: Reserved]
 * \param[in] datarate      : 设置数据速率
 *                             FSK : 600..300000 bits/s
 *                             LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
 *                                   10: 1024, 11: 2048, 12: 4096  chips]
 * \param[in] coderate      : 设置编码率 (LoRa only)
 *                             FSK : N/A (set to 0)
 *                             LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
 * \param[in] bandwidth_afc : 设置 AFC 带宽 (FSK only)
 *                             FSK : >= 2600 and <= 250000 Hz
 *                             LoRa: N/A (set to 0)
 * \param[in] preamble_len  : 设置前导码长度
 *                             FSK : Number of bytes
 *                             LoRa: Length in symbols (the hardware adds 4 more symbols)
 * \param[in] symb_timeout  : 设置单次接收超时值 (LoRa only)
 *                             FSK : N/A (set to 0)
 *                             LoRa: timeout in symbols
 * \param[in] fix_len       : 固有长度使能 [0: variable, 1: fixed]
 * \param[in] payload_len   : 设置负载长度 (当固有长度使能时)
 * \param[in] crc_on        : CRC校验使能 [0: OFF, 1: ON]
 * \param[in] freq_hop_on   : 内部包跳频使能
 *                             FSK : N/A (set to 0)
 *                             LoRa: [0: OFF, 1: ON]
 * \param[in] hop_period    : 每次跳频的符号周期
 *                             FSK : N/A (set to 0)
 *                             LoRa: Number of symbols
 * \param[in] iq_inverted   : 转变 IQ 信号 (LoRa only)
 *                             FSK : N/A (set to 0)
 *                             LoRa: [0: not inverted, 1: inverted]
 * \param[in] rx_continuous : 设置连续接收模式
 *                             [false: single mode, true: continuous mode]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_rx_config_set(
    am_zsl42x_lora_handle_t handle,  am_zsl42x_lora_server_radio_modems_t  modem,
    uint32_t         bandwidth,      uint32_t           datarate,
    uint8_t          coderate,       uint32_t           bandwidth_afc,
    uint16_t         preamble_len,   uint16_t           symb_timeout,
    uint8_t          fix_len,        uint8_t            payload_len,
    uint8_t          crc_on,         uint8_t            freq_hop_on,
    uint8_t          hop_period,     uint8_t            iq_inverted,
    uint8_t          rx_continuous)
{
    if (handle->lora_dev.p_server_funcs->pfn_rx_config_set == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_rx_config_set(
        handle->lora_dev.p_hw_drv, modem,
        bandwidth,                 datarate,
        coderate,                  bandwidth_afc,
        preamble_len,              symb_timeout,
        fix_len,                   payload_len,
        crc_on,                    freq_hop_on,
        hop_period,                iq_inverted,
        rx_continuous);
}

/**
 * \brief 发送传输参数设置
 *
 * \remark 使用LoRa调制模式时仅支持 125、250及500kHz
 *
 * \param[in] handle       : lora驱动参数
 * \param[in] modem        : 使用的射频模式 [0: FSK, 1: LoRa]
 * \param[in] power        : 设置功率 [dBm]
 * \param[in] freq_divider : 设置分频器 (FSK only)
 *                            FSK : [Hz]
 *                            LoRa: 0
 * \param[in] bandwidth    : 设置带宽 (LoRa only)
 *                            FSK : 0
 *                            LoRa: [0: 125 kHz, 1: 250 kHz,
 *                                   2: 500 kHz, 3: Reserved]
 * \param[in] datarate     : 设置数据速率
 *                            FSK : 600..300000 bits/s
 *                            LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
 *                                  10: 1024, 11: 2048, 12: 4096  chips]
 * \param[in] coderate     : 设置编码率 (LoRa only)
 *                            FSK : N/A (set to 0)
 *                            LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
 * \param[in] preamble_len : 设置前导码长度
 *                            FSK : Number of bytes
 *                            LoRa: Length in symbols (the hardware adds 4 more symbols)
 * \param[in] fix_len      : 固定包长使能 [0: variable, 1: fixed]
 * \param[in] crc_on       : CRC校验使能 [0: OFF, 1: ON]
 * \param[in] freq_hop_on  : 内部跳频使能
 *                            FSK : N/A (set to 0)
 *                            LoRa: [0: OFF, 1: ON]
 * \param[in] hop_period   : 每次跳频的符号周期
 *                            FSK : N/A (set to 0)
 *                            LoRa: Number of symbols
 * \param[in] iq_inverted  : 转变 IQ 信号 (LoRa only)
 *                            FSK : N/A (set to 0)
 *                            LoRa: [0: not inverted, 1: inverted]
 * \param[in] timeout      : 传输超时 [ms]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_tx_config_set(
    am_zsl42x_lora_handle_t handle, am_zsl42x_lora_server_radio_modems_t modem,
    int8_t           power,         uint32_t          freq_divider,
    uint32_t         bandwidth,     uint32_t          datarate,
    uint8_t          coderate,      uint16_t          preamble_len,
    uint8_t          fix_len,       uint8_t           crc_on,
    uint8_t          freq_hop_on,   uint8_t           hop_period,
    uint8_t          iq_inverted,   uint32_t          timeout)
{
    if (handle->lora_dev.p_server_funcs->pfn_tx_config_set == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_tx_config_set(
        handle->lora_dev.p_hw_drv, modem,
        power,            freq_divider,
        bandwidth,        datarate,
        coderate,         preamble_len,
        fix_len,          crc_on,
        freq_hop_on,      hop_period,
        iq_inverted,      timeout);
}

/**
 * \brief 周期唤醒接收设置
 *
 * \param[in] handle       : lora句柄
 * \param[in] rx_period    : 接收时间(ms)
 * \param[in] sleep_period : 睡眠时间(ms)
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_rxdutycycle_set (am_zsl42x_lora_handle_t handle,
                                           uint32_t                rx_period,
                                           uint32_t                sleep_period)
{
    if (handle->lora_dev.p_server_funcs->pfn_rxdutycycle == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_rxdutycycle(handle->lora_dev.p_hw_drv,
                                                            rx_period,
                                                            sleep_period);
}

/**
 * \brief 启动射频接收(连续、单次)
 *
 * \param[in] handle  : lora句柄
 * \param[in] timeout : 接收超时 [ms] (0:连续接收模式。others：单次超时接收模式)
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_receive (am_zsl42x_lora_handle_t handle, uint32_t timeout)
{
    if (handle->lora_dev.p_server_funcs->pfn_receive == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_receive(handle->lora_dev.p_hw_drv,
                                                        timeout);
}

/**
 * \brief 发送射频数据（单次）
 *
 * \param[in] handle  : lora句柄
 * \param[in] p_data  : 指向待发送数据的指针
 * \param[in] len     : 待发送数据长度
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_send (am_zsl42x_lora_handle_t handle,
                                uint8_t                *p_data,
                                uint8_t                 len)
{
    if (handle->lora_dev.p_server_funcs->pfn_send == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_send(handle->lora_dev.p_hw_drv,
                                                     p_data, len);
}

/**
 * \brief 进入睡眠模式
 *
 * \param[in] handle     : lora句柄
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_sleep (am_zsl42x_lora_handle_t handle)
{
    if (handle->lora_dev.p_server_funcs->pfn_sleep == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_sleep(handle->lora_dev.p_hw_drv);
}

/**
 * \brief 进入待机模式
 * \param[in] handle   : lora句柄
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_standby (am_zsl42x_lora_handle_t handle)
{
    if (handle->lora_dev.p_server_funcs->pfn_standby == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_standby(handle->lora_dev.p_hw_drv);
}

/**
 * \brief 启动一次信道活跃检测
 *
 * \param[in] handle  : lora句柄
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */

am_static_inline
int am_zsl42x_lora_server_cad_start (am_zsl42x_lora_handle_t handle)
{
    if (handle->lora_dev.p_server_funcs->pfn_cad_start == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_cad_start(handle->lora_dev.p_hw_drv);
}

/**
 * \brief 设置RF频率(通信信道频率)
 *
 * \param[in] handle           : lora句柄
 * \param[in] frequency        : RF频率[单位：Hz]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_channel_set (am_zsl42x_lora_handle_t handle,
                                       uint32_t                frequency)
{
    if (handle->lora_dev.p_server_funcs->pfn_channel_set == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_channel_set(handle->lora_dev.p_hw_drv,
                                                            frequency);
}

/**
 * \brief 获取RF频率(通信信道频率)
 *
 * \param[in]  handle           : lora句柄
 * \param[out] p_freq           : RF频率[单位：Hz]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_channel_get (am_zsl42x_lora_handle_t handle,
                                       uint32_t               *p_freq)
{
    if (handle->lora_dev.p_server_funcs->pfn_channel_get == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_channel_get(handle->lora_dev.p_hw_drv,
                                                            p_freq);
}

/**
 * \brief 检查通信信道是否空闲
 *
 * \param[in]  handle           : lora句柄
 * \param[in]  modem            : 射频调制模式
 * \param[in]  freq             : 信道频率
 * \param[in]  rssi_thresh      : rssi临界值（用来判断是否空闲的依据）
 * \param[out] p_state          : 空闲状态[0：忙碌, 1： 空闲]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_is_channel_free (am_zsl42x_lora_handle_t    handle,
                             am_zsl42x_lora_server_radio_modems_t     modem,
                                           uint32_t                   freq,
                                           int16_t                    rssi_thresh,
                                           uint8_t                   *p_state)
{
    if (handle->lora_dev.p_server_funcs->pfn_is_channel_free == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_is_channel_free(handle->lora_dev.p_hw_drv,
                                                                modem,
                                                                freq,
                                                                rssi_thresh,
                                                                p_state);
}

/**
 * \brief 检查硬件是否支持给出的射频频率
 *
 * \param[in]  handle           : lora句柄
 * \param[in]  frequency        : 频率
 * \param[out] p_state          : 是否支持[0：不支持, 1： 支持]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_rf_frequency_check (am_zsl42x_lora_handle_t  handle,
                                              uint32_t                 frequency,
                                              uint8_t                 *p_state)
{
    if (handle->lora_dev.p_server_funcs->pfn_rf_frequency_check == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_rf_frequency_check(handle->lora_dev.p_hw_drv,
                                                                   frequency,
                                                                   p_state);
}

/**
 * \brief 写特定地址的射频寄存器
 *
 * \param[in] handle  : lora句柄
 * \param[in] address : 写数据的地址
 * \param[in] p_data  : 需要写入的数据
 * \param[in] size    : 写入数据的大小[单位：字节]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_reg_write(am_zsl42x_lora_handle_t  handle,
                                    uint16_t                 address,
                                    uint8_t                 *p_data,
                                    uint8_t                  size)
{
    if (handle->lora_dev.p_server_funcs->pfn_reg_write == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_reg_write(handle->lora_dev.p_hw_drv,
                                                          address,
                                                          p_data,
                                                          size);
}

/**
 * \brief 读特定地址的射频寄存器
 *
 * \param[in] handle  : lora句柄
 * \param[in] address : 读数据的地址
 * \param[in] p_data  : 读取的数据
 * \param[in] size    : 读取数据的大小[单位：字节]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_reg_read(am_zsl42x_lora_handle_t  handle,
                                   uint16_t                 address,
                                   uint8_t                 *p_data,
                                   uint8_t                  size)
{
    if (handle->lora_dev.p_server_funcs->pfn_reg_read == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_reg_read(handle->lora_dev.p_hw_drv,
                                                         address,
                                                         p_data,
                                                         size);
}

/**
 * \brief 从起始地址写多个寄存器
 *
 * \param[in] handle  : lora句柄
 * \param[in] address : 读数据的地址
 * \param[in] p_data  : 读取的数据
 * \param[in] size    : 读取数据的大小[单位：字节]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_buffer_write (am_zsl42x_lora_handle_t  handle,
                                        uint8_t                  address,
                                        uint8_t                 *p_data,
                                        uint8_t                  size)
{
    if (handle->lora_dev.p_server_funcs->pfn_buffer_write == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_buffer_write(handle->lora_dev.p_hw_drv,
                                                             address,
                                                             p_data,
                                                             size);
}

/**
 * \brief 从起始地址读多个寄存器
 *
 * \param[in] handle  : lora句柄
 * \param[in] address : 读数据的地址
 * \param[in] p_data  : 读取的数据
 * \param[in] size    : 读取数据的大小[单位：字节]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_buffer_read (am_zsl42x_lora_handle_t  handle,
                                       uint8_t                  address,
                                       uint8_t                 *p_data,
                                       uint8_t                  size)
{
    if (handle->lora_dev.p_server_funcs->pfn_buffer_read == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_buffer_read(handle->lora_dev.p_hw_drv,
                                                            address,
                                                            p_data,
                                                            size);
}

/**
 * \brief 设置最大荷载长度
 *
 * \param[in] handle     : lora句柄
 * \param[in] modem      : 使用的射频模式 [0: FSK, 1: LoRa]
 * \param[in] length_max : 负载最大字节数[单位：字节]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_max_payload_length_set(am_zsl42x_lora_handle_t  handle,
                                   am_zsl42x_lora_server_radio_modems_t   modem,
                                                 uint8_t                  length_max)
{
    if (handle->lora_dev.p_server_funcs->pfn_max_payload_length_set == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_max_payload_length_set(handle->lora_dev.p_hw_drv,
                                                                       modem,
                                                                       length_max);
}

/**
 * \brief 网络类型设置 (公网、私网)
 *
 * \param[in] handle  : lora句柄
 * \param[in] enable  : 1 : 公网, 0 : 私网
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_public_network_set(am_zsl42x_lora_handle_t   handle,
                                             uint8_t                   enable)

{
    if (handle->lora_dev.p_server_funcs->pfn_public_network_set == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_public_network_set(handle->lora_dev.p_hw_drv,
                                                                   enable);
}

/**
 * \brief 设置连续接收
 *
 * \param[in] handle  : lora句柄
 * \param[in] enable  : 1 允许连续接收， 0不允许连续接收
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_rx_continuous_set (am_zsl42x_lora_handle_t handle,
                                             uint8_t                 enable)
{
    if (handle->lora_dev.p_server_funcs->pfn_rx_continue_set == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_rx_continue_set(handle->lora_dev.p_hw_drv,
                                                                enable);
}

/**
 * \brief 生成32位随机数
 *
 * \param[in]  handle    : lora句柄
 * \param[out] p_random  : 32位随机数值
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_random (am_zsl42x_lora_handle_t handle, uint32_t *p_random)
{
    if (handle->lora_dev.p_server_funcs->pfn_random == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_random(handle->lora_dev.p_hw_drv,
                                                       p_random);
}

/**
 * \brief 计算给出负载对应的符号数
 *
 * \param[in]  handle           : lora句柄
 * \param[in]  pkt_len          : 负载包长度
 * \param[out] p_pkt_symbol_num : 有效负载的符号数
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_time_pkt_symbol_get (am_zsl42x_lora_handle_t  handle,
                                               uint8_t                  pkt_len,
                                               uint16_t       *p_pkt_symbol_num)
{
    if (handle->lora_dev.p_server_funcs->pfn_time_pkt_symbol_get == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_time_pkt_symbol_get(handle->lora_dev.p_hw_drv,
                                                                    pkt_len,
                                                                    p_pkt_symbol_num);
}

/**
 * \brief 计算给出负载的在空时间（ms）
 *
 * \param[in]  handle      : lora句柄
 * \param[in]  modem       : 射频模式
 * \param[in]  packet_len  : 负载包长度
 * \param[out] p_time      : 在空时间（ms）
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_time_on_air_get (am_zsl42x_lora_handle_t handle,
                             am_zsl42x_lora_server_radio_modems_t  modem,
                                           uint8_t                 packet_len,
                                           uint64_t               *p_time)
{
    if (handle->lora_dev.p_server_funcs->pfn_time_on_air_get == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_time_on_air_get(handle->lora_dev.p_hw_drv,
                                                                modem,
                                                                packet_len,
                                                                p_time);
}

/**
 * \brief 设置符号超时时间
 *
 * \param[in] handle   : lora句柄
 * \param[in] timeout  : 符号超时时间[单位：ms]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_symb_timeout_set (am_zsl42x_lora_handle_t handle,
                                            uint32_t                timeout)
{
    if (handle->lora_dev.p_server_funcs->pfn_symb_timeout_set== NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_symb_timeout_set(handle->lora_dev.p_hw_drv,
                                                                 timeout);
}

/**
 * \brief 获取当前射频配置参数情况下单个符号时间,LoRa模式
 *
 * \param[in]  handle : lora句柄
 * \param[out] p_time : 单个符号时间[单位：ms]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_symbol_time_get (am_zsl42x_lora_handle_t handle,
                                           float                  *p_time)
{
    if (handle->lora_dev.p_server_funcs->pfn_symbol_time_get == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_symbol_time_get(handle->lora_dev.p_hw_drv,
                                                                p_time);
}

/**
 * \brief 设置射频进入连续传输模式
 *
 * \param[in] handle : lora句柄
 * \param[in] freq   : 信道频率
 * \param[in] power  : 发射功率 [dBm]
 * \param[in] time   : 传输超时 [ms]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_tx_continuous_wave (am_zsl42x_lora_handle_t handle,
                                              uint32_t                freq,
                                              int8_t                  power,
                                              uint32_t                time)
{
    if (handle->lora_dev.p_server_funcs->pfn_tx_continuous_wave == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_tx_continuous_wave(handle->lora_dev.p_hw_drv,
                                                                   freq,
                                                                   power,
                                                                   time);
}

/**
 * \brief 获取当前的RSSI值
 *
 * \param[in] handle        : lora句柄
 * \param[in] modem         : 射频调制模式
 * \param[in] p_rssi_value  : RSSI值
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_rssi_get (am_zsl42x_lora_handle_t  handle,
                      am_zsl42x_lora_server_radio_modems_t   modem,
                                    int16_t                 *p_rssi_value)
{
    if (handle->lora_dev.p_server_funcs->pfn_rssi_get == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_rssi_get(handle->lora_dev.p_hw_drv,
                                                         modem,
                                                         p_rssi_value);
}

/**
 * \brief 设置同步字(syncword)
 *
 * \param[in] handle     : lora句柄
 * \param[in] p_syncword : 指向同步字数组的指针
 * \param[in] num        : 同步字个数[单位：字节]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_syncword_set (am_zsl42x_lora_handle_t handle,
                                        uint8_t                *p_syncword,
                                        uint8_t                 num)
{
    if (handle->lora_dev.p_server_funcs->pfn_syncword_set == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_syncword_set(handle->lora_dev.p_hw_drv,
                                                             p_syncword,
                                                             num);
}

/**
 * \brief 重传(如果size不为0，转换一部分负载到FIFO中)
 *
 * \param[in] handle   : lora句柄
 * \param[in] p_txbuff : 重传的数据数组指针
 * \param[in] len      : 数据数量[单位：字节]
 * \param[in] offset   : 数据偏移
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_tx_repeat_set (am_zsl42x_lora_handle_t handle,
                                         uint8_t                *p_txbuff,
                                         uint8_t                 len,
                                         uint8_t                 offset)
{
    if (handle->lora_dev.p_server_funcs->pfn_tx_repeat_set == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_tx_repeat_set(handle->lora_dev.p_hw_drv,
                                                              p_txbuff,
                                                              len,
                                                              offset);
}

/**
 * \brief 获取fifo中的前几个字节数据(loranet协议栈使用)
 *
 * \param[in] handle    : lora驱动参数
 * \param[in] p_buffer  : 获取的数据
 * \param[in] nbbytes   ： 数据长度[单位：字节]
 *
 * \retval   AM_ZSL42X_LORA_TRUE        : 操作成功
 * \retval   AM_ZSL42X_LORA_FALSE       : 操作失败
 * \retval   -AM_ZSL42X_LORA_ENOTSUP    ： 不支持
 * \retval   -AM_ZSL42X_LORA_RET_EINVAL : 无效参数
 */
am_static_inline
int am_zsl42x_lora_server_recv_nbbytes_get(am_zsl42x_lora_handle_t  handle,
                                           uint8_t                 *p_buffer,
                                           uint8_t                  nbbytes)
{
    if (handle->lora_dev.p_server_funcs->pfn_recv_nbbytes_get == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_recv_nbbytes_get(handle->lora_dev.p_hw_drv,
                                                                 p_buffer,
                                                                 nbbytes);
}

/**
 * \brief 获取唤醒时间
 *
 * \param[in] handle : lora驱动参数
 *
 * \return 唤醒时间，-AM_LORA_ENOTSUP值表示不支持该操作
 */
am_static_inline
int am_zsl42x_lora_server_wakeup_time_get(am_zsl42x_lora_handle_t handle)
{
    if (handle->lora_dev.p_server_funcs->pfn_wakeup_time_get == NULL) {
        return -AM_ZSL42X_LORA_ENOTSUP;
    }

    return handle->lora_dev.p_server_funcs->pfn_wakeup_time_get(handle->lora_dev.p_hw_drv);
}

/*******************************************************************************
    API END
*******************************************************************************/

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42X_LORA_H */

/* end of file */
