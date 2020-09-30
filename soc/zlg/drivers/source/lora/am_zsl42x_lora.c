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
 *
 * \internal
 * \par Modification history
 * - 1.0.0 20-03-25  enf,first implementation.
 * - 1.0.1 20-09-23  yg, adaptation zsl42x.
 * \endinternal
 */

#include "am_zsl42x_lora.h"
#include "am_zsl42x_lora_reg.h"
#include "am_delay.h"

/**
 * \brief 数据包状态
 */
static am_radio_packet_status_t __radio_pktstatus;

/**
 * \brief FSK模式带宽配置索引列表
 */
static const struct {

    uint32_t bandwidth;
    uint8_t  reg_value;

} __g_zsl42x_lora_fsk_bandwidth[] = {

    {4800  , AM_ZSL42X_LORA_FSK_RX_BW_4800},
    {5800  , AM_ZSL42X_LORA_FSK_RX_BW_5800},
    {7300  , AM_ZSL42X_LORA_FSK_RX_BW_7300},
    {9700  , AM_ZSL42X_LORA_FSK_RX_BW_7300},
    {11700 , AM_ZSL42X_LORA_FSK_RX_BW_11700},
    {14600 , AM_ZSL42X_LORA_FSK_RX_BW_14600},
    {19500 , AM_ZSL42X_LORA_FSK_RX_BW_19500},
    {23400 , AM_ZSL42X_LORA_FSK_RX_BW_23400},
    {29300 , AM_ZSL42X_LORA_FSK_RX_BW_29300},
    {39000 , AM_ZSL42X_LORA_FSK_RX_BW_39000},
    {46900 , AM_ZSL42X_LORA_FSK_RX_BW_39000},
    {58600 , AM_ZSL42X_LORA_FSK_RX_BW_58600},
    {78200 , AM_ZSL42X_LORA_FSK_RX_BW_78200},
    {93800 , AM_ZSL42X_LORA_FSK_RX_BW_93800},
    {117300, AM_ZSL42X_LORA_FSK_RX_BW_117300},
    {156200, AM_ZSL42X_LORA_FSK_RX_BW_156200},
    {187200, AM_ZSL42X_LORA_FSK_RX_BW_187200},
    {234300, AM_ZSL42X_LORA_FSK_RX_BW_234300},
    {312000, AM_ZSL42X_LORA_FSK_RX_BW_234300},
    {373600, AM_ZSL42X_LORA_FSK_RX_BW_373600},
    {467000, AM_ZSL42X_LORA_FSK_RX_BW_373600},
    {500000, 0x00}, /* Invalid Bandwidth */
};

/* lora模式带宽配置索引列表 */
static const am_zsl42x_lora_bw_t __lora_bandwidth[] = {
    AM_ZSL42X_LORA_BW_125000,
    AM_ZSL42X_LORA_BW_250000,
    AM_ZSL42X_LORA_BW_500000,
};

/* 125KHz下，CAD配置索引列表 */
static const zsl42x_lora_cad_set_t __cadconfig[] = {
    {5,  10, 21, AM_ZSL42X_LORA_CAD_SYMBOLNUM_1},
    {6,  10, 21, AM_ZSL42X_LORA_CAD_SYMBOLNUM_1},
    {7,  10, 22, AM_ZSL42X_LORA_CAD_SYMBOLNUM_2},
    {8,  10, 22, AM_ZSL42X_LORA_CAD_SYMBOLNUM_2},
    {9,  10, 23, AM_ZSL42X_LORA_CAD_SYMBOLNUM_4},
    {10, 10, 24, AM_ZSL42X_LORA_CAD_SYMBOLNUM_4},
    {11, 10, 25, AM_ZSL42X_LORA_CAD_SYMBOLNUM_4},
    {12, 10, 28, AM_ZSL42X_LORA_CAD_SYMBOLNUM_4}
};

/* lora 符号时间 */
static double __radio_lora_symbtime[3][8] = {
/*     SF12    SF11    SF10    SF9    SF8    SF7  SF6    SF5 */
    { 32.768, 16.384, 8.192, 4.096, 2.048, 1.024, 0.512, 0.256},  /* 125 KHz */
    { 16.384, 8.192,  4.096, 2.048, 1.024, 0.512, 0.256, 0.128},  /* 250 KHz */
    { 8.192,  4.096,  2.048, 1.024, 0.512, 0.256, 0.128, 0.064}}; /* 500 KHz */

/******************************************************************************/

/** \brief 接收超时回调函数 */
static void __zsl42x_lora_inside_rx_timeout_irq (void            *p_drv);

/** \brief 发送超时回调函数 */
static void __zsl42x_lora_inside_tx_timeout_irq (void            *p_drv);

/** \brief 射频中断处理函数 */
static void __zsl42x_lora_dio1_irq              (void            *p_drv);

/** \brief 设置硬件为接收 */
static int  __zsl42x_lora_inside_hw_rx_set      (am_zsl42x_lora_dev_t *p_dev);

/** \brief 设置硬件为发送 */
static int  __zsl42x_lora_inside_hw_tx_set      (am_zsl42x_lora_dev_t *p_dev);

/** \brief 等到射频繁忙结束 */
static void __zsl42x_lora_inside_wait_on_busy   (am_zsl42x_lora_dev_t *p_dev);

/** \brief 清除所有中断标志 */
static int __zsl42x_lora_drv_int_flag_clr     (void              *p_drv);

/** \brief 复位操作 */
static int __zsl42x_lora_server_reset         (void              *p_drv);

/** \brief 唤醒设备 */
static int __zsl42x_lora_server_wakeup        (void              *p_drv);

/** \brief 设置射频睡眠 */
static int __zsl42x_lora_server_sleep         (void              *p_drv);

/** \brief 设置射频standby */
static int __zsl42x_lora_server_standby       (void              *p_drv);

/** \brief 设置射频开始CAD检测 */
static int __zsl42x_lora_server_cad_start     (void              *p_drv);

/** \brief 检查频率是否支持 */
static int __zsl42x_lora_server_rf_frequency_check (void     *p_drv,
                                                    uint32_t  freq,
                                                    uint8_t  *p_state);

/** \brief 获取符号超时时间 */
static int __zsl42x_lora_server_symbol_time_get (void *p_drv, float *p_time);

/** \brief 获取随机值 */
static int __zsl42x_lora_server_random        (void              *p_drv,
                                               uint32_t          *p_random);
/** \brief 设置射频信道 */
static int __zsl42x_lora_server_channel_set   (void              *p_drv,
                                               uint32_t           fre);
/** \brief 获取射频信道 */
static int __zsl42x_lora_server_channel_get  (void               *p_drv,
                                              uint32_t           *p_fre);
/** \brief 获取射频服务状态 */
static int __zsl42x_lora_server_status_get   (void               *p_drv,
                                              uint8_t            *p_status);
/** \brief dio 是否有效 */
static int __zsl42x_lora_drv_dio_is_valid     (void              *p_drv,
                                               int                dio);
/** \brief 循环接收设置 */
static int __zsl42x_lora_server_rxdutycycle_set (void           *p_drv,
                                                 uint32_t        rx_period,
                                                 uint32_t        sleep_period);

/** \brief calibration 设置 */
static int __zsl42x_lora_inside_calibration_image (void          *p_drv,
                                                   uint32_t       freq);

/** \brief 射频接收数据 */
static int __zsl42x_lora_server_receive (void                    *p_drv,
                                         uint32_t                 timeout);

/** \brief 射频发送数据 */
static int __zsl42x_lora_server_send    (void                    *p_drv,
                                         uint8_t                 *p_data,
                                         uint8_t                  len);

/** \brief 信道活跃状态 */
static int __zsl42x_lora_server_is_channel_free (
                                      void                   *p_drv,
                         am_zsl42x_lora_server_radio_modems_t mode,
                                      uint32_t                freq,
                                      int16_t                 rssi_thresh,
                                      uint8_t                *p_state);

/** \brief 写寄存器 */
static int __zsl42x_lora_server_reg_write (void                 *p_drv,
                                           uint16_t              addr,
                                           uint8_t              *p_data,
                                           uint8_t               size);

/** \brief 读寄存器 */
static int __zsl42x_lora_server_reg_read (void                  *p_drv,
                                          uint16_t               addr,
                                          uint8_t               *p_data,
                                          uint8_t                size);

/** \brief  获取数据包符号时间 */
static int __zsl42x_lora_server_time_pkt_symbol_get (void     *p_drv,
                                                     uint8_t   packet_len,
                                                     uint16_t *p_pkt_symbol_num);

/** \brief  获取数据包在空时间 */
static int __zsl42x_lora_server_time_on_air_get (
                                    void                   *p_drv,
                      am_zsl42x_lora_server_radio_modems_t  modem,
                                    uint8_t                 packet_len,
                                    uint64_t               *p_time);

/** \brief  设置射频  continuous wave 发送模式 */
static int __zsl42x_lora_server_tx_continuous_wave (void      *p_drv,
                                                    uint32_t   freq,
                                                    int8_t     power,
                                                    uint32_t   time);

/** \brief  设置射频同步字节 */
static int __zsl42x_lora_server_syncword_set (void            *p_drv,
                                              uint8_t         *p_syncword,
                                              uint8_t          num);

/** \brief  重复发送 */
static int __zsl42x_lora_server_tx_repeat_set (void           *p_drv,
                                               uint8_t        *p_txbuff,
                                               uint8_t         len,
                                               uint8_t         offset);

/** \brief  crc 种子数设置  */
static am_zsl42x_lora_fsk_crc_type_t __zsl42x_lora_crc_seed_set (void *p_drv);

/** \brief  设置发送配置信息 */
static int  __zsl42x_lora_tx_config_set (
    void *     p_drv,        am_zsl42x_lora_server_radio_modems_t  modem,
    int8_t     power,        uint32_t                freq_dev,
    uint32_t   bandwidth,    uint32_t                datarate,
    uint8_t    coderate,     uint16_t                preamble_len,
    uint8_t    fix_len,      uint8_t                 crc_on,
    uint8_t    freq_hop_on,  uint8_t                 hop_period,
    uint8_t    iq_inverted,  uint32_t                timeout);

/** \brief  设置接收配置信息 */
static int __zsl42x_lora_rx_config_set (
    void *     p_drv,          am_zsl42x_lora_server_radio_modems_t   modem,
    uint32_t   bandwidth,      uint32_t                 datarate,
    uint8_t    coderate,       uint32_t                 bandwidth_afc,
    uint16_t   preamble_len,   uint16_t                 symb_timeout,
    uint8_t    fix_len,        uint8_t                  payload_len,
    uint8_t    crc_on,         uint8_t                  freq_hop_on,
    uint8_t    hop_period,     uint8_t                  iq_inverted,
    uint8_t    rx_continuous);

/** \brief  射频模式设置 */
static int __zsl42x_lora_server_modem_set (void                  *p_drv,
                             am_zsl42x_lora_server_radio_modems_t modem);

/** \brief  射频模式获取 */
static int __zsl42x_lora_server_modem_get (void                   *p_drv,
                             am_zsl42x_lora_server_radio_modems_t *p_modem);

/** \brief  唤醒初始化设置 */
static int __zsl42x_lora_server_wakeup_init (void *p_drv, void *p_arg);

/** \brief  射频内存写操作 */
static int __zsl42x_lora_server_buffer_write (void     *p_drv,
                                              uint8_t   addr,
                                              uint8_t  *p_data,
                                              uint8_t   size);

/** \brief  射频内存读操作 */
static int __zsl42x_lora_server_buffer_read (void      *p_drv,
                                             uint8_t    addr,
                                             uint8_t   *p_data,
                                             uint8_t    size);

/** \brief  设置射频传输的数据长度 */
static int __zsl42x_lora_max_payload_length_set (void                  *p_drv,
                                   am_zsl42x_lora_server_radio_modems_t modem,
                                                 uint8_t                max);

/** \brief  公网设置 */
static int __zsl42x_lora_server_public_network_set (void *p_drv, uint8_t enable);

/** \brief  持续接收设置 */
static int __zsl42x_lora_rx_continues_set (void *p_drv, uint8_t enable);

/** \brief  符号超时时间设置 */
static int __zsl42x_lora_server_symbolnumtimeout_set (void *p_drv, uint32_t ms);

/** \brief  射频RSSI获取 */
static int __zsl42x_lora_server_rssi_get (void                  *p_drv,
                            am_zsl42x_lora_server_radio_modems_t modem,
                                          int16_t               *p_rssi_value);

/** \brief  获取接收到的数据字节个数 */
static int __zsl42x_lora_server_nbbytes_get (void    *p_drv,
                                             uint8_t *p_bufer,
                                             uint8_t  nbbytes);

/** \brief  唤醒时间获取 */
static int __zsl42x_lora_server_wakeup_time_get(void *p_drv);

static const am_zsl42x_lora_server_funcs_t __g_lora_server_funcs = {
    __zsl42x_lora_server_reset,
    __zsl42x_lora_server_wakeup_init,
    __zsl42x_lora_server_wakeup,
    __zsl42x_lora_server_modem_set,
    __zsl42x_lora_server_modem_get,
    __zsl42x_lora_server_status_get,
    __zsl42x_lora_tx_config_set,
    __zsl42x_lora_rx_config_set,
    __zsl42x_lora_server_rxdutycycle_set,
    __zsl42x_lora_server_receive,
    __zsl42x_lora_server_send,
    __zsl42x_lora_server_sleep,
    __zsl42x_lora_server_standby,
    __zsl42x_lora_server_cad_start,
    __zsl42x_lora_server_channel_set,
    __zsl42x_lora_server_channel_get,
    __zsl42x_lora_server_is_channel_free,
    __zsl42x_lora_server_rf_frequency_check,
    __zsl42x_lora_server_reg_write,
    __zsl42x_lora_server_reg_read,
    __zsl42x_lora_server_buffer_write,
    __zsl42x_lora_server_buffer_read,
    __zsl42x_lora_max_payload_length_set,
    __zsl42x_lora_server_public_network_set,
    __zsl42x_lora_rx_continues_set,
    __zsl42x_lora_server_random,
    __zsl42x_lora_server_time_pkt_symbol_get,
    __zsl42x_lora_server_time_on_air_get,
    __zsl42x_lora_server_symbolnumtimeout_set,
    __zsl42x_lora_server_symbol_time_get,
    __zsl42x_lora_server_tx_continuous_wave,
    __zsl42x_lora_server_rssi_get,
    __zsl42x_lora_server_syncword_set,
    __zsl42x_lora_server_tx_repeat_set,
    __zsl42x_lora_server_nbbytes_get,
    __zsl42x_lora_server_wakeup_time_get,
};

void am_zsl42x_lora_memset1 (uint8_t *p_dst, uint8_t value, uint32_t size)
{
    while (size--) {
        *p_dst++ = value;
    }
}

void am_zsl42x_lora_memcpy1 (uint8_t *p_dst, const uint8_t *p_src, uint32_t size)
{
    while (size--) {
        *p_dst++ = *p_src++;
    }
}

static uint8_t __zsl42x_lora_fsk_bandwidth_reg_value (uint32_t bandwidth)
{
    uint8_t i;

    if (bandwidth == 0) {
        return (0x1F);
    }

    for (i = 0; i < (AM_ZSL42X_LORA_NELEMENTS(__g_zsl42x_lora_fsk_bandwidth) - 1); i++) {
        if ((bandwidth >= __g_zsl42x_lora_fsk_bandwidth[i].bandwidth) &&
            (bandwidth <  __g_zsl42x_lora_fsk_bandwidth[i + 1].bandwidth)) {
            return __g_zsl42x_lora_fsk_bandwidth[i+1].reg_value;
        }
    }
    /* ERROR: Value not found */
    while (1);
}

static int __zsl42x_lora_server_wakeup_time_get(void *p_drv)
{
    (void)p_drv;

    return AM_ZSL42X_LORA_RADIO_WAKEUP_TIME;
}

static
int __zsl42x_lora_server_nbbytes_get (void    *p_drv,
                                      uint8_t *p_bufer,
                                      uint8_t  nbbytes)
{
    am_zsl42x_lora_dev_t *p_dev     = (am_zsl42x_lora_dev_t *)p_drv;
    uint16_t         irq_flgs  = 0;
    uint8_t          rx_size   = 0;
    uint8_t          rxpointer = 0;

    do {
        am_zsl42x_lora_getirqstatus(p_dev, &irq_flgs);
        irq_flgs &= (AM_ZSL42X_LORA_IRQ_MASK_RX_DONE |
                     AM_ZSL42X_LORA_IRQ_MASK_CRC_ERR);

        am_zsl42x_lora_getrxbufferstatus(p_dev, &rxpointer, &rx_size);

        if (irq_flgs && (rx_size < nbbytes)) {
            return AM_ZSL42X_LORA_RET_ERROR;
        }

    } while (rx_size < nbbytes);

    am_zsl42x_lora_read_buffer(p_dev,
                               rxpointer,
                               nbbytes,
                               p_bufer);
    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_wakeup_init (void *p_drv, void *p_arg)
{
    (void)p_drv;
    (void)p_arg;

    return -AM_ZSL42X_LORA_ENOTSUP;
}

static int __zsl42x_lora_server_modem_set (void                  *p_drv,
                             am_zsl42x_lora_server_radio_modems_t modem)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 进入STDBY_RC模式 */
    __zsl42x_lora_server_standby(p_drv);
    __zsl42x_lora_inside_wait_on_busy(p_dev);

    if (modem == AM_ZSL42X_LORA_MODEM_FSK) {
        am_zsl42x_lora_setpackettype(p_dev, AM_ZSL42X_LORA_MODEM_FSK);
        p_dev->setconfig.network_current = 0;
    } else if (modem == AM_ZSL42X_LORA_MODEM_LORA) {
        am_zsl42x_lora_setpackettype(p_dev, AM_ZSL42X_LORA_MODEM_LORA);
        if (p_dev->setconfig.network_current != p_dev->setconfig.network_previous) {
            p_dev->setconfig.network_current = p_dev->setconfig.network_previous;
            __zsl42x_lora_server_public_network_set(p_drv, p_dev->setconfig.network_current);
        }
    } else {
        return AM_ZSL42X_LORA_RET_ERROR;
    }

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_modem_get (void                   *p_drv,
                             am_zsl42x_lora_server_radio_modems_t *p_modem)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    *p_modem = (am_zsl42x_lora_server_radio_modems_t)p_dev->setconfig.packet_type;

    return AM_ZSL42X_LORA_RET_OK;
}

static
int __zsl42x_lora_server_reg_read (void      *p_drv,
                                   uint16_t   addr,
                                   uint8_t   *p_data,
                                   uint8_t    size)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    return am_zsl42x_lora_read_register(p_dev,
                                        (am_zsl42x_lora_regaddr_t)addr,
                                        p_data,
                                        size);
}

static
int __zsl42x_lora_server_reg_write (void      *p_drv,
                                    uint16_t   addr,
                                    uint8_t   *p_data,
                                    uint8_t    size)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    return am_zsl42x_lora_write_register(p_dev,
                                         (am_zsl42x_lora_regaddr_t)addr,
                                         p_data,
                                         size);
}

static
int __zsl42x_lora_server_buffer_read (void     *p_drv,
                                      uint8_t   addr,
                                      uint8_t  *p_data,
                                      uint8_t   size)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    return am_zsl42x_lora_read_buffer(p_dev, addr, size, p_data);
}

static
int __zsl42x_lora_server_buffer_write (void     *p_drv,
                                       uint8_t   addr,
                                       uint8_t  *p_data,
                                       uint8_t   size)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    return am_zsl42x_lora_write_buffer(p_dev, addr, p_data, size);
}

static int __zsl42x_lora_max_payload_length_set (
                                            void                   *p_drv,
                              am_zsl42x_lora_server_radio_modems_t  modem,
                                            uint8_t                 max)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 进入standby模式 */
    __zsl42x_lora_server_standby(p_drv);
    __zsl42x_lora_inside_wait_on_busy(p_dev);

    switch(modem) {

    case AM_ZSL42X_LORA_MODEM_FSK:
        if (p_dev->setconfig.header_type == AM_ZSL42X_LORA_FSK_PACKET_VARIABLE_LENGTH) {
            p_dev->setconfig.payloadlength = max;
            am_zsl42x_lora_setpacketparams_fsk(
                p_dev,
                p_dev->setconfig.preamble_length,
                (am_zsl42x_lora_fsk_detector_length_t)p_dev->setconfig.fsk_setting.detector_length,
                p_dev->setconfig.fsk_setting.syncword_number,
                (am_zsl42x_lora_fsk_addr_filter_t)p_dev->setconfig.fsk_setting.addr_filter_enable,
                (am_zsl42x_lora_fsk_packetsize_info_t)p_dev->setconfig.header_type,
                p_dev->setconfig.payloadlength,
                __zsl42x_lora_crc_seed_set(p_drv),
                p_dev->setconfig.fsk_setting.whiteng_enable);

        }

        break;

    case AM_ZSL42X_LORA_MODEM_LORA:

            p_dev->setconfig.payloadlength = max;

            am_zsl42x_lora_setpacketparams_lora(
                p_dev,
                p_dev->setconfig.preamble_length,
                (am_zsl42x_lora_header_type_t)p_dev->setconfig.header_type,
                p_dev->setconfig.payloadlength,
                (am_zsl42x_lora_crc_type_t)p_dev->setconfig.crc_enable,
                (am_zsl42x_lora_iq_type_t)p_dev->setconfig.iq_inverted_enable);

        break;

    default:
        break;
    }

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_public_network_set (void      *p_drv,
                                                    uint8_t    enable)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;
    uint8_t               data;

    p_dev->setconfig.network_current  = enable;
    p_dev->setconfig.network_previous = enable;

    __zsl42x_lora_server_modem_set(p_drv, AM_ZSL42X_LORA_MODEM_LORA);

    if (1 == enable) {

        data = (AM_ZSL42X_LORA_MAC_PUBLIC_SYNCWORD >> 8) & 0xFF;

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_write_register(
            p_dev, AM_ZSL42X_LORA_REGADDR_LORA_SYNCWORD_MSB, &data, 1);

        data = AM_ZSL42X_LORA_MAC_PUBLIC_SYNCWORD & 0xFF;
        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_write_register(
            p_dev, AM_ZSL42X_LORA_REGADDR_LORA_SYNCWORD_LSB, &data, 1);

    } else {
        data = (AM_ZSL42X_LORA_MAC_PRIVATE_SYNCWORD >> 8) & 0XFF;

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_write_register(
            p_dev, AM_ZSL42X_LORA_REGADDR_LORA_SYNCWORD_MSB, &data, 1);

        data = AM_ZSL42X_LORA_MAC_PRIVATE_SYNCWORD & 0xFF;
        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_write_register(
            p_dev, AM_ZSL42X_LORA_REGADDR_LORA_SYNCWORD_LSB, &data, 1);
    }
    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_rx_continues_set (void *p_drv, uint8_t enable)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    p_dev->rx_continuous = enable;

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_tx_repeat_set (void          *p_drv,
                                               uint8_t       *p_txbuff,
                                               uint8_t        len,
                                               uint8_t        offset)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    am_lpsoftimer_stop(&p_dev->tx_timeout_timer);
    am_lpsoftimer_stop(&p_dev->rx_timeout_timer);

    if (p_drv == NULL || p_txbuff == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if (p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    __zsl42x_lora_inside_wait_on_busy(p_dev);

    /* 选择硬件电路为发送 */
    __zsl42x_lora_inside_hw_tx_set(p_dev);

    /* 进入STDBY_RC模式 */
    __zsl42x_lora_server_standby(p_drv);
    /* 设置发送长度 */
    p_dev->setconfig.payloadlength = len + offset;

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_setpacketparams_lora(
        p_dev,
        p_dev->setconfig.preamble_length,
        (am_zsl42x_lora_header_type_t)p_dev->setconfig.header_type,
        p_dev->setconfig.payloadlength,
        (am_zsl42x_lora_crc_type_t)p_dev->setconfig.crc_enable,
        (am_zsl42x_lora_iq_type_t)p_dev->setconfig.iq_inverted_enable);

    /* 设置DIO1关联中断  */
    uint16_t irq_mask = AM_ZSL42X_LORA_IRQ_MASK_TX_DONE |
                        AM_ZSL42X_LORA_IRQ_MASK_TIMEOUT;

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    //am_zsl42x_lora_setdioirqparams(p_dev, irq_mask, irq_mask, 0, 0);

    am_zsl42x_lora_setbufferbaseaddress(p_dev,
                                   p_dev->setconfig.tx_base_addr,
                                   0X00);

    /* 往buffer缓冲区中填写有效负载 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_write_buffer(p_dev, offset, p_txbuff, len);

    /* 清除中断标志 */
    am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_ALL_ENABLE);
    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_TX_RUNNING_STATE;

    /* 启动发送模式 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_settx(p_dev, 0);

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_drv_int_flag_clr (void *p_drv)
{
    am_zsl42x_lora_server_handle_t  handle = (am_zsl42x_lora_server_handle_t)p_drv;
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)handle->p_hw_drv;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    /* 清楚中断标志 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
    return am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_ALL_ENABLE);
}

static int __zsl42x_lora_drv_dio_is_valid (void *p_drv, int dio)
{
    (void)p_drv;
    (void)dio;

    return -AM_ZSL42X_LORA_ENOTSUP;
}

/**
 * \brief 射频中断处理函数
 *
 * \note 在事件处理回调内禁止将射频模块设置为睡眠模式
 */
static void __zsl42x_lora_dio1_irq (void *p_drv)
{
    am_zsl42x_lora_server_handle_t  handle = (am_zsl42x_lora_server_handle_t)p_drv;
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)handle->p_hw_drv;

    uint8_t  uint8_get1 = 0;
    uint8_t  uint8_get2 = 0;
    uint16_t irg_flags  = 0;

    /* 防止射频睡眠时意外产生中断 */
    if (p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        return;
    }

    /* 获取中断状态标志 */
    am_zsl42x_lora_getirqstatus(p_dev, &irg_flags);

    do {
        /* 头错误 */
        if((irg_flags & AM_ZSL42X_LORA_IRQ_MASK_HEADER_ERR ) ==
            AM_ZSL42X_LORA_IRQ_MASK_HEADER_ERR ) {

            am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_HEADER_ERR);
            irg_flags &= ~AM_ZSL42X_LORA_IRQ_MASK_HEADER_ERR;

            am_lpsoftimer_stop(&p_dev->rx_timeout_timer);
            if(p_dev->rx_continuous == AM_FALSE ) {
                __zsl42x_lora_server_standby((void *)handle);
            }

            /* 判断是否是连续接收模式，更新当前lora设备工作状态 */
            if(p_dev->rx_continuous == AM_ZSL42X_LORA_TRUE) {
                p_dev->state = AM_ZSL42X_LORA_RF_RX_RUNNING_STATE;
            } else {
                p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;
            }

            if((handle->p_radio_events != NULL ) &&
               (handle->p_radio_events->pfn_rx_timeout_cb != NULL )) {
                handle->p_radio_events->pfn_rx_error_cb(
                    handle->p_radio_events->p_rx_error_arg);
            }
        }

        /* 头有效 */
        if ((irg_flags & AM_ZSL42X_LORA_IRQ_MASK_HEADER_VALID) ==
            AM_ZSL42X_LORA_IRQ_MASK_HEADER_VALID) {

            am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_HEADER_VALID);
            irg_flags &= ~AM_ZSL42X_LORA_IRQ_MASK_HEADER_VALID;


            if (handle->p_radio_events &&
                handle->p_radio_events->pfn_valid_header_cb) {
                handle->p_radio_events->pfn_valid_header_cb(
                    handle->p_radio_events->p_valid_header_arg);
            }
        }
        /* CRC 校验 */
        if ((irg_flags & AM_ZSL42X_LORA_IRQ_MASK_CRC_ERR) &&
            (p_dev->setconfig.crc_enable == 1)) {

            am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_CRC_ERR);
            irg_flags &= ~AM_ZSL42X_LORA_IRQ_MASK_CRC_ERR;

            /* 判断是否是连续接收模式，更新当前lora设备工作状态 */
            if(p_dev->rx_continuous == AM_ZSL42X_LORA_FALSE) {
                p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;
                __zsl42x_lora_server_standby((void *)handle);
            }

            if (handle->p_radio_events && handle->p_radio_events->pfn_rx_error_cb) {
                handle->p_radio_events->pfn_rx_error_cb(
                    handle->p_radio_events->p_rx_error_arg);
            }
        }

        /* 接收完成 */
        if(irg_flags & AM_ZSL42X_LORA_IRQ_MASK_RX_DONE) {

            am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_RX_DONE);
            irg_flags &= ~AM_ZSL42X_LORA_IRQ_MASK_RX_DONE;

            am_lpsoftimer_stop(&p_dev->rx_timeout_timer);

            /* 判断是否是连续接收模式，更新当前lora设备工作状态 */
            if(p_dev->rx_continuous == AM_ZSL42X_LORA_TRUE) {
                p_dev->state = AM_ZSL42X_LORA_RF_RX_RUNNING_STATE;
            } else {
                p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;
            }

            if((p_dev->rx_continuous == AM_ZSL42X_LORA_FALSE) &&
               (p_dev->setconfig.packet_type == AM_ZSL42X_LORA_MODEM_LORA)) {

                uint8_t write_data = 0;
                uint8_t read_data  = 0;

                __zsl42x_lora_server_standby((void *)handle);

                am_zsl42x_lora_write_register(p_dev,
                                         AM_ZSL42X_LORA_REGADDR_RTC_CTRL,
                                         &write_data,
                                         1);
                am_zsl42x_lora_read_register(p_dev,
                                        AM_ZSL42X_LORA_REGADDR_EVENT_MASK,
                                        &read_data,
                                        1);
                read_data |= (0x01 << 1);
                am_zsl42x_lora_write_register(p_dev,
                                         AM_ZSL42X_LORA_REGADDR_EVENT_MASK,
                                         &read_data,
                                         1);
            }

            am_zsl42x_lora_getrxbufferstatus(p_dev,
                                             &uint8_get1,
                                             &uint8_get2);

            /* 如果是隐式包头，则从寄存器中获取长度信息 */
            if((p_dev->setconfig.header_type == AM_ZSL42X_LORA_PACKET_FIXED_LENGTH) &&
               (p_dev->setconfig.packet_type == AM_ZSL42X_LORA_MODEM_LORA)) {
                am_zsl42x_lora_read_register(p_dev,
                                        AM_ZSL42X_LORA_REGADDR_PAYLOADLENGTH,
                                        &uint8_get2,
                                        1);
            }

            /* 从buffer读取数据到设备数据缓冲区 */
            am_zsl42x_lora_read_buffer(p_dev,
                                       uint8_get1,
                                       uint8_get2,
                                       p_dev->rx_tx_buffer);

            am_zsl42x_lora_getpacketstatus(p_dev, &__radio_pktstatus);

            if (__radio_pktstatus.packettype == AM_ZSL42X_LORA_MODEM_LORA) {

                p_dev->lora_packet_deal.size       = uint8_get2;
                p_dev->lora_packet_deal.snr_value  = __radio_pktstatus.params.lora.snrpkt;
                p_dev->lora_packet_deal.rssi_value = __radio_pktstatus.params.lora.rssipkt;

                if (handle->p_radio_events &&
                    handle->p_radio_events->pfn_rx_done_cb) {
                    handle->p_radio_events->pfn_rx_done_cb(
                        handle->p_radio_events->p_rx_done_arg,
                        p_dev->p_rx_tx_buffer,
                        p_dev->lora_packet_deal.size,
                        p_dev->lora_packet_deal.rssi_value,
                        p_dev->lora_packet_deal.snr_value);
                }

            } else if (__radio_pktstatus.packettype == AM_ZSL42X_LORA_MODEM_FSK) {

                p_dev->fsk_packet_deal.size       = uint8_get2;
                p_dev->fsk_packet_deal.rssi_value = __radio_pktstatus.params.fsk.rssiavg;

                if (handle->p_radio_events &&
                    handle->p_radio_events->pfn_rx_done_cb) {

                    handle->p_radio_events->pfn_rx_done_cb(
                            handle->p_radio_events->p_rx_done_arg,
                            p_dev->p_rx_tx_buffer,
                            p_dev->fsk_packet_deal.size,
                            p_dev->fsk_packet_deal.rssi_value,
                            0);
                }
            }
        }

        /* 发送完成 */
        if(irg_flags & AM_ZSL42X_LORA_IRQ_MASK_TX_DONE) {

            am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_TX_DONE);
            irg_flags &= ~AM_ZSL42X_LORA_IRQ_MASK_TX_DONE;

            am_lpsoftimer_stop(&p_dev->tx_timeout_timer);

            /* 判断是否是连续发送模式，更新当前lora设备工作状态 */
            if(p_dev->tx_continuous == 1) {
                p_dev->state = AM_ZSL42X_LORA_RF_TX_RUNNING_STATE;
            } else {
                __zsl42x_lora_server_standby((void *)handle);
                p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;
            }

            if (handle->p_radio_events &&
                handle->p_radio_events->pfn_tx_done_cb) {

                handle->p_radio_events->pfn_tx_done_cb(
                    handle->p_radio_events->p_tx_done_arg);
            }
        }

        /* CAD检测完成 */
        if(irg_flags & AM_ZSL42X_LORA_IRQ_MASK_CAD_DONE) {

            am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_CAD_DONE);
            irg_flags &= ~AM_ZSL42X_LORA_IRQ_MASK_CAD_DONE;

            __zsl42x_lora_server_standby((void *)handle);
            p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;

            /* 通道活跃标志设定 */
            if(irg_flags & AM_ZSL42X_LORA_IRQ_MASK_CAD_DET) {
                am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_CAD_DET);
                p_dev->cad_activity = 1;
            } else {
                p_dev->cad_activity = 0;
            }

            /* 获取包信息 */
            am_zsl42x_lora_getpacketstatus(p_dev, &__radio_pktstatus);

            p_dev->lora_packet_deal.snr_value  = __radio_pktstatus.params.lora.snrpkt;
            p_dev->lora_packet_deal.rssi_value = __radio_pktstatus.params.lora.rssipkt;

            /* cad回调函数 */
            if (handle->p_radio_events &&
                handle->p_radio_events->pfn_cad_done_cb) {

                handle->p_radio_events->pfn_cad_done_cb(
                    handle->p_radio_events->p_cad_done_arg,
                    p_dev->cad_activity);
            }
        }

        /* 超时 */
        if ((irg_flags & AM_ZSL42X_LORA_IRQ_MASK_TIMEOUT) ==
            AM_ZSL42X_LORA_IRQ_MASK_TIMEOUT) {

            am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_TIMEOUT);
            irg_flags &= ~AM_ZSL42X_LORA_IRQ_MASK_TIMEOUT;

            if (p_dev->state == AM_ZSL42X_LORA_RF_TX_RUNNING_STATE) {
                am_lpsoftimer_stop(&p_dev->tx_timeout_timer);

                if (p_dev->tx_continuous != AM_ZSL42X_LORA_TRUE) {
                    __zsl42x_lora_server_standby((void *)handle);
                    p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;
                }

                if (handle->p_radio_events && handle->p_radio_events->pfn_tx_timeout_cb) {
                    handle->p_radio_events->pfn_tx_timeout_cb(
                        handle->p_radio_events->p_tx_timeout_arg);
                }
            }

            if (p_dev->state == AM_ZSL42X_LORA_RF_RX_RUNNING_STATE) {

                am_lpsoftimer_stop(&p_dev->rx_timeout_timer);

                /* 防止在隐式报头模式下出现异常中断事件 */
                if (p_dev->setconfig.header_type == (uint8_t)AM_ZSL42X_LORA_PACKET_FIXED_LENGTH) {
                    uint8_t data[2] = {0, 0};
                    am_zsl42x_lora_write_register(p_dev,
                                             AM_ZSL42X_LORA_REGADDR_DIO3_OUT_VOLTAGE,
                                             &data[0],
                                             2);

                    am_zsl42x_lora_read_register(p_dev,
                                            AM_ZSL42X_LORA_REGADDR_EVENT_MASK,
                                            &data[0],
                                            2);
                    data[1] = data[1] | 0x02;

                    am_zsl42x_lora_write_register(p_dev,
                                             AM_ZSL42X_LORA_REGADDR_EVENT_MASK,
                                             data,
                                             2);
                }

                if (p_dev->rx_continuous != AM_ZSL42X_LORA_TRUE) {
                    __zsl42x_lora_server_standby((void *)handle);
                    p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;
                }

                if (handle->p_radio_events && handle->p_radio_events->pfn_rx_timeout_cb) {
                    handle->p_radio_events->pfn_rx_timeout_cb(
                        handle->p_radio_events->p_rx_timeout_arg);
                }
            }
        }

        if (p_dev->state != AM_ZSL42X_LORA_RF_SLEEP_STATE) {
            am_zsl42x_lora_getirqstatus(p_dev, &irg_flags); /* 防止在中断的时候射频又产生中断 */
        } else {
            return;    /* 射频处于睡眠状态就退出 */
        }

    } while (irg_flags);
}

static void __zsl42x_lora_inside_tx_timeout_irq (void *p_drv)
{
    am_zsl42x_lora_server_handle_t handle = (am_zsl42x_lora_server_handle_t)p_drv;
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)handle->p_hw_drv;

    /* 更新当前lora设备工作状态 */
    if (p_dev->tx_continuous != AM_ZSL42X_LORA_TRUE) {
        p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;
    }

    am_lpsoftimer_stop(&p_dev->tx_timeout_timer);
    am_lpsoftimer_stop(&p_dev->rx_timeout_timer);

    if (handle->p_radio_events &&
        handle->p_radio_events->pfn_tx_timeout_cb) {
        handle->p_radio_events->pfn_tx_timeout_cb(
            handle->p_radio_events->p_tx_timeout_arg);
    }
}

static void __zsl42x_lora_inside_rx_timeout_irq (void *p_drv)
{
    am_zsl42x_lora_server_handle_t  handle = (am_zsl42x_lora_server_handle_t)p_drv;
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)handle->p_hw_drv;

    /* 更新当前lora设备工作状态 */
    if (p_dev->rx_continuous != AM_ZSL42X_LORA_TRUE) {
        p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;
    }

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_ALL_ENABLE);

    am_lpsoftimer_stop(&p_dev->rx_timeout_timer);

    if (handle->p_radio_events &&
        handle->p_radio_events->pfn_rx_timeout_cb) {
        handle->p_radio_events->pfn_rx_timeout_cb(
            handle->p_radio_events->p_rx_timeout_arg);
    }
}

static int __zsl42x_lora_inside_hw_rx_set (am_zsl42x_lora_dev_t *p_dev)
{
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->txen_pin, 0);
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->rxen_pin, 1);

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_inside_hw_tx_set (am_zsl42x_lora_dev_t *p_dev)
{
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->txen_pin, 1);
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->rxen_pin, 0);

    return AM_ZSL42X_LORA_RET_OK;
}

static void __zsl42x_lora_inside_wait_on_busy (am_zsl42x_lora_dev_t *p_dev)
{
    while(p_dev->p_devinfo->pfn_gpio_get(p_dev->p_devinfo->busy_pin) == 1);
}

static int __zsl42x_lora_inside_calibration_all (void *p_drv)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    /* 校准所有 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_setcalibratefunction(p_dev, AM_ZSL42X_LORA_SETCALIBRATE_ENABLE_ALL);

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_inside_calibration_image (void *p_drv, uint32_t freq)
{
    am_zsl42x_lora_dev_t            *p_dev    = (am_zsl42x_lora_dev_t *)p_drv;
    am_zsl42x_lora_calibrate_image_t fre_band = AM_ZSL42X_LORA_CALIBRATE_IMAGE_430MHz_440MHz;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(freq > 900000000) {
        fre_band = AM_ZSL42X_LORA_CALIBRATE_IMAGE_902MHz_928MHz;
    }
    else if(freq > 850000000) {
        fre_band = AM_ZSL42X_LORA_CALIBRATE_IMAGE_863MHz_870MHz;
    }
    else if(freq > 770000000) {
        fre_band = AM_ZSL42X_LORA_CALIBRATE_IMAGE_779MHz_787MHz;
    }
    else if(freq > 460000000) {
        fre_band = AM_ZSL42X_LORA_CALIBRATE_IMAGE_470MHz_510MHz;
    }
    else if(freq > 425000000) {
        fre_band = AM_ZSL42X_LORA_CALIBRATE_IMAGE_430MHz_440MHz;
    }

    if (fre_band == p_dev->p_devinfo->calibrate_image) {
        return AM_ZSL42X_LORA_RET_OK;
    }

    /* 镜频校准，切换频段 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_calibrateimage(p_dev, fre_band);

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_reset (void *p_drv)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    /* 持续一定时长的低电平 */
    am_mdelay(10);
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->reset_pin, 0);
    am_mdelay(20);
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->reset_pin, 1);
    am_mdelay(10);

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_wakeup (void *p_drv)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->nss_pin, 0);
    am_mdelay(1);
    p_dev->p_devinfo->pfn_gpio_set(p_dev->p_devinfo->nss_pin, 1);

    __zsl42x_lora_inside_wait_on_busy(p_dev);

    if (p_dev->p_devinfo->core_type == AM_ZSL42X_LORA_CORE_TYPE_SX1268) {
        // 使能DIO3 引脚给有源晶振供电
        am_zsl42x_lora_setdio3tcxoctrl(p_dev, AM_ZSL42X_LORA_DIO3TCXOCTRL_1700MV, 100);
    }

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_status_get (void                  *p_drv,
                              am_zsl42x_lora_server_radio_state_t *p_status)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL || p_status == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    *p_status = p_dev->state;

    return AM_ZSL42X_LORA_RET_OK;
}

void __zsl42x_lora_whitening_seed_set (void *p_drv, uint16_t seed)
{
    uint8_t                 reg_val = 0;
    am_zsl42x_lora_dev_t   *p_dev   = (am_zsl42x_lora_dev_t *)p_drv;

    am_zsl42x_lora_read_register(p_dev,
                            AM_ZSL42X_LORA_REGADDR_FSK_WHITEN_INIT_MSB,
                            &reg_val,
                            1);
    reg_val &= 0xFE;
    reg_val = ((seed >> 8) & 0x01) | reg_val;
    am_zsl42x_lora_write_register(p_dev,
                             AM_ZSL42X_LORA_REGADDR_FSK_WHITEN_INIT_MSB,
                             &reg_val,
                             1);

    am_zsl42x_lora_write_register(p_dev,
                             AM_ZSL42X_LORA_REGADDR_FSK_WHITEN_INIT_LSB,
                             (uint8_t *)&seed,
                             1);
}

static
am_zsl42x_lora_fsk_crc_type_t __zsl42x_lora_crc_seed_set (void  *p_drv)
{
    am_zsl42x_lora_fsk_crc_type_t  crc_val;
    uint8_t                   buf[2];
    uint16_t                  seed;
    uint16_t                  crc_polynomia;
    am_zsl42x_lora_dev_t          *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    if (p_dev->setconfig.fsk_setting.crc_length == AM_ZSL42X_LORA_FSK_CRC_2_BYTES_IBM) {
        seed            = AM_ZSL42X_LORA_CRC_IBM_SEED;
        crc_polynomia   = AM_ZSL42X_LORA_CRC_POLYNOMIAL_IBM;
        crc_val         = AM_ZSL42X_LORA_FSK_CRC_2_BYTE;
    } else if (p_dev->setconfig.fsk_setting.crc_length == AM_ZSL42X_LORA_FSK_CRC_2_BYTES_CCIT) {
        seed            = AM_ZSL42X_LORA_CRC_CCITT_SEED;
        crc_polynomia   = AM_ZSL42X_LORA_CRC_POLYNOMIAL_CCITT;
        crc_val         = AM_ZSL42X_LORA_FSK_CRC_2_BYTE_INV;
    } else {
        return (am_zsl42x_lora_fsk_crc_type_t)p_dev->setconfig.fsk_setting.crc_length;
    }

    buf[0] = (uint8_t)((seed >> 8 ) & 0xFF);
    buf[1] = (uint8_t)(seed & 0xFF);

    am_zsl42x_lora_write_register(p_dev,
                             AM_ZSL42X_LORA_REGADDR_FSK_CRC_INIT_MSB,
                             &buf[0],
                             1);
    am_zsl42x_lora_write_register(p_dev,
                             AM_ZSL42X_LORA_REGADDR_FSK_CRC_INIT_LSB,
                             &buf[1],
                             1);
    buf[0] = (uint8_t)((crc_polynomia >> 8) & 0xFF);
    buf[1] = (uint8_t)(crc_polynomia & 0xFF);

    am_zsl42x_lora_write_register(p_dev,
                             AM_ZSL42X_LORA_REGADDR_FSK_CRC_POLY_MSB,
                             &buf[0],
                             0);
    am_zsl42x_lora_write_register(p_dev,
                             AM_ZSL42X_LORA_REGADDR_FSK_CRC_POLY_LSB,
                             &buf[1],
                             1);
    return crc_val;
}

static int  __zsl42x_lora_tx_config_set (
    void      *p_drv,        am_zsl42x_lora_server_radio_modems_t   modem,
    int8_t     power,        uint32_t                 freq_dev,
    uint32_t   bandwidth,    uint32_t                 datarate,
    uint8_t    coderate,     uint16_t                 preamble_len,
    uint8_t    fix_len,      uint8_t                  crc_on,
    uint8_t    freq_hop_on,  uint8_t                  hop_period,
    uint8_t    iq_inverted,  uint32_t                 timeout)
{
    am_zsl42x_lora_dev_t *p_dev       = (am_zsl42x_lora_dev_t *)p_drv;
    uint8_t          syncword[8] = {
        0xC1, 0x94, 0xC1, 0x02, 0x04, 0x06, 0x04, 0x06};
    uint8_t          val_tx_mod  = 0;

    (void)freq_dev;
    (void)freq_hop_on;
    (void)hop_period;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 进入standby模式 */
    __zsl42x_lora_server_standby(p_drv);
    __zsl42x_lora_inside_wait_on_busy(p_dev);

    am_zsl42x_lora_setbufferbaseaddress(p_dev, 0X00, 0X00);
    p_dev->setconfig.tx_base_addr = 0X00;
    p_dev->setconfig.rx_base_addr = 0X00;

//    am_zsl42x_lora_settxparam(p_dev, power, AM_ZSL42X_LORA_RAMPTIME_40US);
    p_dev->setconfig.tx_power = power;

    switch (modem) {

    case AM_ZSL42X_LORA_MODEM_FSK:

        p_dev->setconfig.packet_type                    = AM_ZSL42X_LORA_RADIO_PACKET_FSK;
//        p_dev->setconfig.fsk_setting.freq_deviation     = freq_dev; /* 接收配置也需要配置，所以在默认初始值设置了 */
        p_dev->setconfig.fsk_setting.syncword_number    = 3 << 3; /* convert byte into bit */
        p_dev->setconfig.fsk_setting.datarate           = datarate;
        p_dev->setconfig.fsk_setting.bw                 = bandwidth;
        p_dev->setconfig.fsk_setting.detector_length    = AM_ZSL42X_LORA_FSK_PREAMBLE_DETECTOR_BITS_8;
        p_dev->setconfig.fsk_setting.addr_filter_enable = AM_ZSL42X_LORA_FSK_ADDR_FILTER_DISABLE;
        p_dev->setconfig.fsk_setting.radio_modshapings  = AM_ZSL42X_LORA_FSK_FILTER_GS_BT_10;
        p_dev->setconfig.fsk_setting.whiteng_enable     = AM_TRUE;
        p_dev->setconfig.preamble_length                = (preamble_len << 3); /* convert byte into bit */
        p_dev->setconfig.payloadlength                  = __ZSL42X_LORA_MAX_PAYLOAD_LRNGRH;
        p_dev->setconfig.crc_enable                     = crc_on;

         if (p_dev->setconfig.crc_enable == AM_TRUE) {
             p_dev->setconfig.fsk_setting.crc_length = AM_ZSL42X_LORA_FSK_CRC_2_BYTES_CCIT;
         } else {
             p_dev->setconfig.fsk_setting.crc_length = AM_ZSL42X_LORA_FSK_CRC_OFF;
         }

        p_dev->setconfig.header_type  = (fix_len == AM_TRUE) ? \
            AM_ZSL42X_LORA_FSK_PACKET_FIXED_LENGTH : AM_ZSL42X_LORA_FSK_PACKET_VARIABLE_LENGTH;


        __zsl42x_lora_inside_wait_on_busy(p_dev);
        __zsl42x_lora_server_modem_set(
            p_dev,
            ((p_dev->setconfig.packet_type == AM_ZSL42X_LORA_RADIO_PACKET_LORA) ? AM_ZSL42X_LORA_MODEM_LORA : AM_ZSL42X_LORA_MODEM_FSK));

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setmodulationparams_fsk(
            p_dev,
            p_dev->setconfig.fsk_setting.datarate,
            (am_zsl42x_lora_fsk_filter_t)p_dev->setconfig.fsk_setting.radio_modshapings,
            __zsl42x_lora_fsk_bandwidth_reg_value(p_dev->setconfig.fsk_setting.bw),
            p_dev->setconfig.fsk_setting.freq_deviation,
            p_dev->p_devinfo->frequency);

        __zsl42x_lora_inside_wait_on_busy(p_dev);

        /* 设置数据包参数 */
        am_zsl42x_lora_setpacketparams_fsk(
            p_dev,
            p_dev->setconfig.preamble_length,
            (am_zsl42x_lora_fsk_detector_length_t)p_dev->setconfig.fsk_setting.detector_length,
            p_dev->setconfig.fsk_setting.syncword_number,
            (am_zsl42x_lora_fsk_addr_filter_t)p_dev->setconfig.fsk_setting.addr_filter_enable,
            (am_zsl42x_lora_fsk_packetsize_info_t)p_dev->setconfig.header_type,
            p_dev->setconfig.payloadlength,
            __zsl42x_lora_crc_seed_set(p_drv),
            p_dev->setconfig.fsk_setting.whiteng_enable);
        __zsl42x_lora_whitening_seed_set(p_dev, 0x01FF);

        p_dev->setconfig.fsk_setting.p_syncword = &syncword[0];
        __zsl42x_lora_server_syncword_set(p_drv,
                                     p_dev->setconfig.fsk_setting.p_syncword,
                                     8);

        /* 设置需要过滤的节点地址、广播地址 */
        __zsl42x_lora_inside_wait_on_busy(p_dev);
        if(p_dev->setconfig.fsk_setting.addr_filter_enable == 1) {
            am_zsl42x_lora_write_register(p_dev,
                                     AM_ZSL42X_LORA_REGADDR_FSK_NODE_ADDR,
                                     &p_dev->setconfig.fsk_setting.node_addr,
                                     1);
        } else if(p_dev->setconfig.fsk_setting.addr_filter_enable == 2) {
            am_zsl42x_lora_write_register(p_dev,
                                     AM_ZSL42X_LORA_REGADDR_FSK_BROADCAST_ADDR,
                                     &p_dev->setconfig.fsk_setting.broadcast_addr,
                                     1);
        }
        break;

    case AM_ZSL42X_LORA_MODEM_LORA:

        p_dev->setconfig.packet_type     = AM_ZSL42X_LORA_RADIO_PACKET_LORA;
        p_dev->setconfig.lora_setting.bw = bandwidth;
        p_dev->setconfig.lora_setting.sf = datarate;
        p_dev->setconfig.lora_setting.cr = coderate;
        p_dev->setconfig.preamble_length = preamble_len;

        if(((bandwidth == 0) && ((datarate == 11) || (datarate == 12))) ||
          ((bandwidth == 1) && (datarate == 12))) {
            p_dev->setconfig.lora_setting.lowdatarate_optimize = AM_ZSL42X_LORA_LOW_DATA_DATE_OPTIMIZE_ON;
        } else {
            p_dev->setconfig.lora_setting.lowdatarate_optimize = AM_ZSL42X_LORA_LOW_DATA_DATE_OPTIMIZE_OFF;
        }

        /*
         * 对于ZSL42X_LORA,当SF设置为SF5/SF5时，发送速度很快，为了防止接收方接收不到数据，前导码建议设置
         * 12 symbols 或 更长
         */
        if((p_dev->setconfig.lora_setting.sf == AM_ZSL42X_LORA_SF5 ) ||
           (p_dev->setconfig.lora_setting.sf == AM_ZSL42X_LORA_SF6 )) {

            if ( p_dev->setconfig.preamble_length < 12) {
                p_dev->setconfig.preamble_length = 12;
            }
        }

        p_dev->setconfig.header_type   = (am_zsl42x_lora_header_type_t)fix_len;
        p_dev->setconfig.payloadlength = __ZSL42X_LORA_MAX_PAYLOAD_LRNGRH;
        p_dev->setconfig.crc_enable    = (am_zsl42x_lora_crc_type_t)crc_on;
        p_dev->setconfig.iq_inverted_enable = (am_zsl42x_lora_iq_type_t)iq_inverted;

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        __zsl42x_lora_server_modem_set(
            p_dev,
            ((p_dev->setconfig.packet_type == AM_ZSL42X_LORA_RADIO_PACKET_LORA) ? AM_ZSL42X_LORA_MODEM_LORA : AM_ZSL42X_LORA_MODEM_FSK));

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setmodulationparams_lora(p_dev,
                                           (am_zsl42x_lora_sf_t)p_dev->setconfig.lora_setting.sf,
                                           __lora_bandwidth[p_dev->setconfig.lora_setting.bw],
                                           (am_zsl42x_lora_cr_t)p_dev->setconfig.lora_setting.cr,
                                           (am_bool_t)p_dev->setconfig.lora_setting.lowdatarate_optimize);

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        /* 设置数据包参数 */
        am_zsl42x_lora_setpacketparams_lora(
            p_dev,
            p_dev->setconfig.preamble_length,
            (am_zsl42x_lora_header_type_t)p_dev->setconfig.header_type,
            p_dev->setconfig.payloadlength,
            (am_zsl42x_lora_crc_type_t)p_dev->setconfig.crc_enable,
            (am_zsl42x_lora_iq_type_t)p_dev->setconfig.iq_inverted_enable);

        p_dev->setconfig.lora_setting.tx_timeout = timeout;

        /* Modulation Quality */
        {
            am_zsl42x_lora_read_register(p_dev,
                                    AM_ZSL42X_LORA_REGADDR_TX_MODULATION,
                                    &val_tx_mod,
                                    1);

            if ((modem == AM_ZSL42X_LORA_MODEM_LORA) &&
                (__lora_bandwidth[bandwidth] == AM_ZSL42X_LORA_BW_500000)) {
                val_tx_mod &= ~(1 << 2 );
            } else {
                val_tx_mod |= (1 << 2 );
            }

            am_zsl42x_lora_write_register(p_dev,
                                     AM_ZSL42X_LORA_REGADDR_TX_MODULATION,
                                     &val_tx_mod,
                                     1);

          /*
           * during the chip initialization,
           * the register TxClampConfigshould be modified to optimize the PA clamping
           * threshold. Bits 4-1 must be set to “1111” (default value “0100”).
           */
            if (p_dev->p_devinfo->core_type == AM_ZSL42X_LORA_CORE_TYPE_SX1262) {
                uint8_t val_ant_mis;
                am_zsl42x_lora_read_register(p_dev,
                                        AM_ZSL42X_LORA_REGADDR_TX_CLAMPCONFIG,
                                        &val_ant_mis,
                                        1);
                val_ant_mis |= ( 0x0F << 1 );
                am_zsl42x_lora_write_register(p_dev,
                                         AM_ZSL42X_LORA_REGADDR_TX_CLAMPCONFIG,
                                         &val_ant_mis,
                                         1);
            }
        }

        break;

    default:
        break;
    }

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_rx_config_set (
    void *     p_drv,          am_zsl42x_lora_server_radio_modems_t  modem,
    uint32_t   bandwidth,      uint32_t                datarate,
    uint8_t    coderate,       uint32_t                bandwidth_afc,
    uint16_t   preamble_len,   uint16_t                symb_timeout,
    uint8_t    fix_len,        uint8_t                 payload_len,
    uint8_t    crc_on,         uint8_t                 freq_hop_on,
    uint8_t    hop_period,     uint8_t                 iq_inverted,
    uint8_t    rx_continuous)
{
    am_zsl42x_lora_dev_t *p_dev       = (am_zsl42x_lora_dev_t *)p_drv;
    uint8_t          syncword[8] = {
        0xC1, 0x94, 0xC1, 0x02, 0x04, 0x06, 0x04, 0x06};

    (void)bandwidth_afc;
    (void)freq_hop_on;
    (void)hop_period;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 进入standby模式 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
    __zsl42x_lora_server_standby(p_drv);

    switch (modem) {

    case AM_ZSL42X_LORA_MODEM_FSK:
        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_stoptimeronpreamble(p_dev, AM_ZSL42X_LORA_STOPTIMER_SYNCWORD_HEADER);

        p_dev->setconfig.packet_type = AM_ZSL42X_LORA_RADIO_PACKET_FSK;

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        __zsl42x_lora_server_modem_set(
            p_dev,
            ((p_dev->setconfig.packet_type == AM_ZSL42X_LORA_RADIO_PACKET_LORA) ? AM_ZSL42X_LORA_MODEM_LORA : AM_ZSL42X_LORA_MODEM_FSK));

        p_dev->setconfig.fsk_setting.datarate           = datarate;
        p_dev->setconfig.fsk_setting.bw                 = bandwidth;
        p_dev->setconfig.fsk_setting.radio_modshapings  = AM_ZSL42X_LORA_FSK_FILTER_GS_BT_10;
        p_dev->setconfig.fsk_setting.detector_length    = AM_ZSL42X_LORA_FSK_PREAMBLE_DETECTOR_BITS_8;
        p_dev->setconfig.fsk_setting.addr_filter_enable = AM_ZSL42X_LORA_FSK_ADDR_FILTER_DISABLE;
        p_dev->setconfig.fsk_setting.whiteng_enable     = AM_TRUE;
        p_dev->setconfig.fsk_setting.syncword_number    = (3 << 3); /* convert byte into bit */

        p_dev->setconfig.preamble_length    = (preamble_len << 3); /* convert from byte to bit */
        p_dev->setconfig.payloadlength      = __ZSL42X_LORA_MAX_PAYLOAD_LRNGRH;
        p_dev->setconfig.crc_enable         = crc_on;
        p_dev->setconfig.iq_inverted_enable = iq_inverted;

        if (p_dev->setconfig.crc_enable == AM_TRUE) {
            p_dev->setconfig.fsk_setting.crc_length = AM_ZSL42X_LORA_FSK_CRC_2_BYTES_CCIT;
        } else {
            p_dev->setconfig.fsk_setting.crc_length = AM_ZSL42X_LORA_FSK_CRC_OFF;
        }

        p_dev->setconfig.header_type    = (fix_len == AM_TRUE) ? \
            AM_ZSL42X_LORA_FSK_PACKET_FIXED_LENGTH : AM_ZSL42X_LORA_FSK_PACKET_VARIABLE_LENGTH;

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setmodulationparams_fsk(
            p_dev,
            p_dev->setconfig.fsk_setting.datarate,
            (am_zsl42x_lora_fsk_filter_t)p_dev->setconfig.fsk_setting.radio_modshapings,
            __zsl42x_lora_fsk_bandwidth_reg_value(p_dev->setconfig.fsk_setting.bw),
            p_dev->setconfig.fsk_setting.freq_deviation,
            p_dev->p_devinfo->frequency);

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setpacketparams_fsk(
            p_dev,
            p_dev->setconfig.preamble_length,
            (am_zsl42x_lora_fsk_detector_length_t)p_dev->setconfig.fsk_setting.detector_length,
            p_dev->setconfig.fsk_setting.syncword_number,
            (am_zsl42x_lora_fsk_addr_filter_t)p_dev->setconfig.fsk_setting.addr_filter_enable,
            (am_zsl42x_lora_fsk_packetsize_info_t)p_dev->setconfig.header_type,
            p_dev->setconfig.payloadlength,
            __zsl42x_lora_crc_seed_set(p_drv),
            p_dev->setconfig.fsk_setting.whiteng_enable);

        p_dev->setconfig.fsk_setting.p_syncword =&syncword[0];
        __zsl42x_lora_server_syncword_set(p_drv,
                                     p_dev->setconfig.fsk_setting.p_syncword,
                                     8);

        __zsl42x_lora_whitening_seed_set(p_drv, 0x01FF);

        break;

    case AM_ZSL42X_LORA_MODEM_LORA:

        __zsl42x_lora_inside_wait_on_busy(p_dev);
//        am_zsl42x_lora_stoptimeronpreamble(p_dev, AM_ZSL42X_LORA_STOPTIMER_SYNCWORD_HEADER);

        p_dev->setconfig.lora_setting.symbol_timeout = symb_timeout;

        p_dev->setconfig.packet_type = AM_ZSL42X_LORA_RADIO_PACKET_LORA;
        __zsl42x_lora_inside_wait_on_busy(p_dev);
        __zsl42x_lora_server_modem_set(
            p_dev,
            ((p_dev->setconfig.packet_type == AM_ZSL42X_LORA_RADIO_PACKET_LORA) ? AM_ZSL42X_LORA_MODEM_LORA : AM_ZSL42X_LORA_MODEM_FSK));

        if (bandwidth > 10) {
            bandwidth = 0;
        }

        p_dev->setconfig.lora_setting.bw = bandwidth;
        p_dev->setconfig.lora_setting.sf = datarate;
        p_dev->setconfig.lora_setting.cr = coderate;

        if(((bandwidth == 0) && ((datarate == 11) || (datarate == 12))) ||
          ((bandwidth == 1)  && (datarate == 12))) {
            p_dev->setconfig.lora_setting.lowdatarate_optimize = AM_ZSL42X_LORA_LOW_DATA_DATE_OPTIMIZE_ON;
        } else {
            p_dev->setconfig.lora_setting.lowdatarate_optimize = AM_ZSL42X_LORA_LOW_DATA_DATE_OPTIMIZE_OFF;
        }

        if((p_dev->setconfig.lora_setting.sf == AM_ZSL42X_LORA_SF5 ) ||
           (p_dev->setconfig.lora_setting.sf == AM_ZSL42X_LORA_SF6 )) {

            if( preamble_len < 12 ) {
                p_dev->setconfig.preamble_length = 12;
            } else {
                p_dev->setconfig.preamble_length = preamble_len;
            }
        } else {
            p_dev->setconfig.preamble_length = preamble_len;
        }

        p_dev->setconfig.header_type = (am_zsl42x_lora_header_type_t)fix_len;
        p_dev->setconfig.payloadlength = payload_len;
        p_dev->setconfig.crc_enable    = (am_zsl42x_lora_crc_type_t)crc_on;
        p_dev->setconfig.iq_inverted_enable = (am_zsl42x_lora_iq_type_t)iq_inverted;

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setmodulationparams_lora(p_dev,
                                           (am_zsl42x_lora_sf_t)p_dev->setconfig.lora_setting.sf,
                                           __lora_bandwidth[p_dev->setconfig.lora_setting.bw],
                                           (am_zsl42x_lora_cr_t)p_dev->setconfig.lora_setting.cr,
                                           (am_bool_t)p_dev->setconfig.lora_setting.lowdatarate_optimize);

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        /* 设置数据包参数 */
        am_zsl42x_lora_setpacketparams_lora(
            p_dev,
            p_dev->setconfig.preamble_length,
            (am_zsl42x_lora_header_type_t)p_dev->setconfig.header_type,
            p_dev->setconfig.payloadlength,
            (am_zsl42x_lora_crc_type_t)p_dev->setconfig.crc_enable,
            (am_zsl42x_lora_iq_type_t)p_dev->setconfig.iq_inverted_enable);

        uint8_t data;
        am_zsl42x_lora_read_register(p_dev, AM_ZSL42X_LORA_REGADDR_IQ_POLARITY_SETUP, &data, 1);
        if (p_dev->setconfig.iq_inverted_enable == AM_ZSL42X_LORA_IQ_INVERTED) {
            data &= ~(1 << 2);
        } else {
            data |= (1 << 2);
        }
        am_zsl42x_lora_write_register(p_dev, AM_ZSL42X_LORA_REGADDR_IQ_POLARITY_SETUP, &data, 1);

        break;

    default:
        break;
    }

    p_dev->rx_continuous = rx_continuous;

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_rxdutycycle_set (void    *p_drv,
                                                 uint32_t rx_period,
                                                 uint32_t sleep_period)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    am_lpsoftimer_stop(&p_dev->tx_timeout_timer);
    am_lpsoftimer_stop(&p_dev->rx_timeout_timer);

    /* 选择硬件电路为接收 */
    __zsl42x_lora_inside_hw_rx_set(p_dev);

    /* 进入STDBY_RC模式 */
    __zsl42x_lora_server_standby(p_drv);

    /* 前导码检测停止接收定时器 */
//    am_zsl42x_lora_stoptimeronpreamble(p_dev, AM_ZSL42X_LORA_STOPTIMER_SYNCWORD_HEADER);

    /* 查找lora带宽索引判断，超出索引范围，使用默认配置  */
    if(p_dev->setconfig.lora_setting.bw > 10) {
        p_dev->setconfig.lora_setting.bw = 0;
    }

    /* 设置lora调制参数 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_setmodulationparams_lora(p_dev,
                                       (am_zsl42x_lora_sf_t)p_dev->setconfig.lora_setting.sf,
                                       __lora_bandwidth[p_dev->setconfig.lora_setting.bw],
                                       (am_zsl42x_lora_cr_t)p_dev->setconfig.lora_setting.cr,
                                       (am_bool_t)p_dev->setconfig.lora_setting.lowdatarate_optimize);

    /* 设置数据包参数 */
    am_zsl42x_lora_setpacketparams_lora(
        p_dev,
        p_dev->setconfig.preamble_length,
        (am_zsl42x_lora_header_type_t)p_dev->setconfig.header_type,
        p_dev->setconfig.payloadlength,
        (am_zsl42x_lora_crc_type_t)p_dev->setconfig.crc_enable,
        (am_zsl42x_lora_iq_type_t)p_dev->setconfig.iq_inverted_enable);

    /* 设置DIO1关联中断  */
//    uint16_t irq_mask = AM_ZSL42X_LORA_IRQ_MASK_RX_DONE;
//    __zsl42x_lora_inside_wait_on_busy(p_dev);
//    am_zsl42x_lora_setdioirqparams(p_dev,
//                             irq_mask,
//                             irq_mask,
//                             0,
//                             0);

    __zsl42x_lora_inside_wait_on_busy(p_dev);

    /* 清除中断标志 */
    am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_ALL_ENABLE);

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_RX_RUNNING_STATE;

    __zsl42x_lora_inside_wait_on_busy(p_dev);

//    rxduration (us) = rx_period * 15.625
//    sleepduration (us) = sleep_period * 15.625

    return am_zsl42x_lora_setrxdutycycle(p_dev, rx_period, sleep_period);
}

static int __zsl42x_lora_server_receive (void *p_drv, uint32_t timeout)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    am_lpsoftimer_stop(&p_dev->tx_timeout_timer);
    am_lpsoftimer_stop(&p_dev->rx_timeout_timer);

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 选择硬件电路为接收 */
    __zsl42x_lora_inside_hw_rx_set(p_dev);

    /* 进入standby模式 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
    __zsl42x_lora_server_standby(p_drv);

    /* 前导码检测停止接收定时器 */
//    am_zsl42x_lora_stoptimeronpreamble(handle, AM_ZSL42X_LORA_STOPTIMER_SYNCWORD_HEADER);

    /* 设置FSK参数 */
    if(p_dev->setconfig.packet_type == AM_ZSL42X_LORA_RADIO_PACKET_FSK) {

        /* 设置FSK调制参数 */
//        __zsl42x_lora_inside_wait_on_busy(p_dev);
//        am_zsl42x_lora_setmodulationparams_fsk(
//            p_dev,
//            p_dev->setconfig.fsk_setting.datarate,
//            p_dev->setconfig.fsk_setting.radio_modshapings,
//            __zsl42x_lora_fsk_bandwidth_reg_value(p_dev->setconfig.fsk_setting.bw),
//            p_dev->setconfig.fsk_setting.freq_deviation,
//            p_dev->p_devinfo->frequency);

        /* 设置数据包相关配置 */
        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setpacketparams_fsk(
            p_dev,
            p_dev->setconfig.preamble_length,
            (am_zsl42x_lora_fsk_detector_length_t)p_dev->setconfig.fsk_setting.detector_length,
            p_dev->setconfig.fsk_setting.syncword_number,
            (am_zsl42x_lora_fsk_addr_filter_t)p_dev->setconfig.fsk_setting.addr_filter_enable,
            (am_zsl42x_lora_fsk_packetsize_info_t)((p_dev->setconfig.header_type)),
            p_dev->setconfig.payloadlength,
            __zsl42x_lora_crc_seed_set(p_drv),
            p_dev->setconfig.fsk_setting.whiteng_enable);
    } else {
        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setmodulationparams_lora(p_dev,
                                           (am_zsl42x_lora_sf_t)p_dev->setconfig.lora_setting.sf,
                                           __lora_bandwidth[p_dev->setconfig.lora_setting.bw],
                                           (am_zsl42x_lora_cr_t)p_dev->setconfig.lora_setting.cr,
                                           (am_bool_t)p_dev->setconfig.lora_setting.lowdatarate_optimize);

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        /* 设置数据包参数 */
        am_zsl42x_lora_setpacketparams_lora(
            p_dev,
            p_dev->setconfig.preamble_length,
            (am_zsl42x_lora_header_type_t)p_dev->setconfig.header_type,
            p_dev->setconfig.payloadlength,
            (am_zsl42x_lora_crc_type_t)p_dev->setconfig.crc_enable,
            (am_zsl42x_lora_iq_type_t)p_dev->setconfig.iq_inverted_enable);
    }

    /* 设置DIO1关联中断  */
    uint16_t irq_mask = (AM_ZSL42X_LORA_IRQ_MASK_RX_DONE      |
                         AM_ZSL42X_LORA_IRQ_MASK_HEADER_ERR   |
                         AM_ZSL42X_LORA_IRQ_MASK_CRC_ERR      |
                         AM_ZSL42X_LORA_IRQ_MASK_HEADER_VALID);

    if (p_dev->rx_continuous != AM_ZSL42X_LORA_TRUE) {
        irq_mask |= AM_ZSL42X_LORA_IRQ_MASK_TIMEOUT;
    }

//    __zsl42x_lora_inside_wait_on_busy(p_dev);
//    am_zsl42x_lora_setdioirqparams(p_dev,
//                             irq_mask,
//                             irq_mask,
//                             0,
//                             0);

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    /* 清除中断标志 */
    am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_ALL_ENABLE);

    if (p_dev->rx_continuous == AM_ZSL42X_LORA_TRUE) {

        am_zsl42x_lora_setsymbolnumtimeout_lora(p_dev, 0);
        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setrx(p_dev, 0xFFFFFF);

    } else {

        am_zsl42x_lora_setsymbolnumtimeout_lora(p_dev, p_dev->setconfig.lora_setting.symbol_timeout);
        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setrx(p_dev, 0);
    }

    if(timeout != 0) {
        am_lpsoftimer_value_set(&p_dev->rx_timeout_timer, timeout);
        am_lpsoftimer_start(&p_dev->rx_timeout_timer);
    }

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_RX_RUNNING_STATE;

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_send (void     *p_drv,
                                      uint8_t  *p_data,
                                      uint8_t   len)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    am_lpsoftimer_stop(&p_dev->tx_timeout_timer);
    am_lpsoftimer_stop(&p_dev->rx_timeout_timer);

    if (p_drv == NULL || p_data == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if (p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 选择硬件电路为发送 */
    __zsl42x_lora_inside_hw_tx_set(p_dev);

    /* 进入STDBY_RC模式 */
    __zsl42x_lora_server_standby((void *)p_drv);

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_setbufferbaseaddress(p_dev, 0X00, 0X00);

    p_dev->setconfig.payloadlength = len;

    /* 设置FSK参数 */
    if(p_dev->setconfig.packet_type == AM_ZSL42X_LORA_RADIO_PACKET_FSK) {

        /* 设置FSK调制参数 */
//        __zsl42x_lora_inside_wait_on_busy(p_dev);
//        am_zsl42x_lora_setmodulationparams_fsk(
//            p_dev,
//            p_dev->setconfig.fsk_setting.datarate,
//            p_dev->setconfig.fsk_setting.radio_modshapings,
//            __zsl42x_lora_fsk_bandwidth_reg_value(p_dev->setconfig.fsk_setting.bw),
//            p_dev->setconfig.fsk_setting.freq_deviation,
//            p_dev->p_devinfo->frequency);

        /* 设置数据包相关配置 */
        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setpacketparams_fsk(
            p_dev,
            p_dev->setconfig.preamble_length,
            (am_zsl42x_lora_fsk_detector_length_t)p_dev->setconfig.fsk_setting.detector_length,
            p_dev->setconfig.fsk_setting.syncword_number,
            (am_zsl42x_lora_fsk_addr_filter_t)p_dev->setconfig.fsk_setting.addr_filter_enable,
            (am_zsl42x_lora_fsk_packetsize_info_t)p_dev->setconfig.header_type,
            p_dev->setconfig.payloadlength,
            __zsl42x_lora_crc_seed_set(p_drv),
            p_dev->setconfig.fsk_setting.whiteng_enable);
    } else {

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_setmodulationparams_lora(p_dev,
                                           (am_zsl42x_lora_sf_t)p_dev->setconfig.lora_setting.sf,
                                           __lora_bandwidth[p_dev->setconfig.lora_setting.bw],
                                           (am_zsl42x_lora_cr_t)p_dev->setconfig.lora_setting.cr,
                                           (am_bool_t)p_dev->setconfig.lora_setting.lowdatarate_optimize);

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        /* 设置数据包参数 */
        am_zsl42x_lora_setpacketparams_lora(
            p_dev,
            p_dev->setconfig.preamble_length,
            (am_zsl42x_lora_header_type_t)p_dev->setconfig.header_type,
            p_dev->setconfig.payloadlength,
            (am_zsl42x_lora_crc_type_t)p_dev->setconfig.crc_enable,
            (am_zsl42x_lora_iq_type_t)p_dev->setconfig.iq_inverted_enable);
    }

    /* 设置DIO1关联中断  */
    uint16_t irq_mask = AM_ZSL42X_LORA_IRQ_MASK_TX_DONE      |
                        AM_ZSL42X_LORA_IRQ_MASK_HEADER_ERR   |
                        AM_ZSL42X_LORA_IRQ_MASK_CRC_ERR      |
                        AM_ZSL42X_LORA_IRQ_MASK_HEADER_VALID;

//    __zsl42x_lora_inside_wait_on_busy(p_dev);
//    am_zsl42x_lora_setdioirqparams(p_dev,
//                              irq_mask,
//                              irq_mask,
//                              0,
//                              0);

    /* 往buffer缓冲区中填写有效负载 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_write_buffer(p_dev,
                           p_dev->setconfig.tx_base_addr,
                           p_data,
                           p_dev->setconfig.payloadlength);

    /* 清除中断标志 */
    am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_ALL_ENABLE);

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_TX_RUNNING_STATE;

    if (p_dev->setconfig.lora_setting.tx_timeout != 0) {
        /* 设置软件定时器 */
        am_lpsoftimer_value_set(&p_dev->tx_timeout_timer,
                                p_dev->setconfig.lora_setting.tx_timeout);
        am_lpsoftimer_start(&p_dev->tx_timeout_timer);
    }

    /* 启动发送模式 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);

    am_zsl42x_lora_settx(p_dev, 0);

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_sleep (void *p_drv)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        return AM_ZSL42X_LORA_RET_OK;
    }

    am_lpsoftimer_stop(&p_dev->rx_timeout_timer);
    am_lpsoftimer_stop(&p_dev->tx_timeout_timer);

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_SLEEP_STATE;

    __zsl42x_lora_inside_wait_on_busy(p_dev);

    am_zsl42x_lora_setsleep (p_dev,
                             AM_ZSL42X_LORA_SLEEP_MODE_WARM_START,
                             AM_ZSL42X_LORA_SLEEP_MODE_RTC_WAKEUP_DISABLE);
    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_standby (void *p_drv)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    am_lpsoftimer_stop(&p_dev->rx_timeout_timer);
    am_lpsoftimer_stop(&p_dev->tx_timeout_timer);

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;

    __zsl42x_lora_inside_wait_on_busy(p_dev);

    return am_zsl42x_lora_setstandby (p_dev, AM_ZSL42X_LORA_STANDBY_MODE_STDBY_RC);
}

static int __zsl42x_lora_server_cad_start (void *p_drv)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    uint8_t i = 0, find_flag = 0;

    am_lpsoftimer_stop(&p_dev->tx_timeout_timer);
    am_lpsoftimer_stop(&p_dev->rx_timeout_timer);

    if((p_dev->setconfig.packet_type == AM_ZSL42X_LORA_RADIO_PACKET_FSK) ||
       (p_drv == NULL)) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 进入 standby模式 */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
    __zsl42x_lora_server_standby(p_drv);

//    /* 查找CAD索引表 */
//    find_flag = 0;
//    for(i = 0; i < 5; i++) {
//        if((p_dev->setconfig.lora_setting.sf == __cadconfig[i].sf)) {
//
//            find_flag                   = 1;
//            break;
//        }
//    }
//
//    /* 查找CAD索引表失败，设置参数异常，使用默认配置  */
//    if(find_flag == 0) {
//        i = 0;
//    }

    i = p_dev->setconfig.lora_setting.sf - 5;

    /* 选择硬件电路为接收 */
    __zsl42x_lora_inside_hw_rx_set(p_dev);

    /* 前导码检测停止接收定时器 */
    am_zsl42x_lora_stoptimeronpreamble(p_dev, AM_ZSL42X_LORA_STOPTIMER_SYNCWORD_HEADER);

    /* 查找lora带宽索引判断，超出索引范围，使用默认配置  */
    if(p_dev->setconfig.lora_setting.bw > 10) {
        p_dev->setconfig.lora_setting.bw = 0;
    }

//    /* 设置lora调制参数 */
//    __zsl42x_lora_inside_wait_on_busy(p_dev);
//    am_zsl42x_lora_setmodulationparams_lora(p_dev,
//                                       (am_zsl42x_lora_sf_t)p_dev->setconfig.lora_setting.sf,
//                                       __lora_bandwidth[p_dev->setconfig.lora_setting.bw],
//                                       (am_zsl42x_lora_cr_t)p_dev->setconfig.lora_setting.cr,
//                                       (am_bool_t)p_dev->setconfig.lora_setting.lowdatarate_optimize);
//
//    /* 设置数据包参数 */
//    __zsl42x_lora_inside_wait_on_busy(p_dev);
//    am_zsl42x_lora_setpacketparams_lora(
//        p_dev,
//        p_dev->setconfig.preamble_length,
//        (am_zsl42x_lora_header_type_t)p_dev->setconfig.header_type,
//        p_dev->setconfig.payloadlength,
//        (am_zsl42x_lora_crc_type_t)p_dev->setconfig.crc_enable,
//        (am_zsl42x_lora_iq_type_t)p_dev->setconfig.iq_inverted_enable);

    /* 清除中断标志 */
    am_zsl42x_lora_clearirqstatus(p_dev, AM_ZSL42X_LORA_IRQ_MASK_ALL_ENABLE);

//    am_lpsoftimer_timeout_set(timeout);

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_setcadparams_lora(p_dev,
                                __cadconfig[i].symbolnum,
                                __cadconfig[i].detpeak,
                                __cadconfig[i].detmin,
                                AM_ZSL42X_LORA_CAD_EXTIMODE_ONLY,
                                0);

//    am_lpsoftimer_start(&handle->rx_timeout_timer);

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_CAD_STATE;

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_setcad(p_dev);

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_channel_set (void *p_drv, uint32_t fre)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 进入STDBY_RC模式 */
    __zsl42x_lora_server_standby(p_drv);
    __zsl42x_lora_inside_wait_on_busy(p_dev);

    __zsl42x_lora_inside_calibration_image(p_drv, fre);

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_setrffrequency(p_dev, fre, p_dev->p_devinfo->frequency);

    p_dev->setconfig.frequency = fre;

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_channel_get (void *p_drv, uint32_t *p_fre)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL || p_fre == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    *p_fre = p_dev->setconfig.frequency;

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_is_channel_free (
                                            void                   *p_drv,
                              am_zsl42x_lora_server_radio_modems_t  mode,
                                            uint32_t                freq,
                                            int16_t                 rssi_thresh,
                                            uint8_t                *p_state)
{
    int16_t          rssi   = 0;
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    __zsl42x_lora_server_sleep(p_drv);

    am_zsl42x_lora_setpackettype(p_dev, (uint8_t)mode);

    __zsl42x_lora_server_channel_set(p_drv, freq);

    __zsl42x_lora_server_receive(p_drv, 0);

    am_udelay(100);

    __zsl42x_lora_server_rssi_get(p_drv, mode, &rssi);

    if(rssi < rssi_thresh) {
        *p_state = 1;
    } else {
        *p_state = 0;
    }

    __zsl42x_lora_server_sleep(p_drv);

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_rf_frequency_check (void     *p_drv,
                                                    uint32_t  freq,
                                                    uint8_t  *p_state)
{
    (void)p_drv;
    (void)freq;

    // Implement check. Currently all frequencies are supported
    *p_state = 1;

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_random (void *p_drv, uint32_t *p_random)
{
    uint8_t           data_buff[4] = {0, 0, 0, 0};
    am_zsl42x_lora_dev_t  *p_dev        = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL || p_random == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 进入STDBY_RC模式 */
    __zsl42x_lora_server_standby(p_drv);

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    if(am_zsl42x_lora_read_register (
            p_dev, AM_ZSL42X_LORA_REGADDR_RANDOMNUMBERGEN0, &data_buff[0], 1) != 0) {
        return AM_ZSL42X_LORA_RET_ERROR;
    }

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    if(am_zsl42x_lora_read_register (
            p_dev, AM_ZSL42X_LORA_REGADDR_RANDOMNUMBERGEN1, &data_buff[1], 1) != 0) {
        return AM_ZSL42X_LORA_RET_ERROR;
    }

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    if(am_zsl42x_lora_read_register (
            p_dev, AM_ZSL42X_LORA_REGADDR_RANDOMNUMBERGEN2, &data_buff[2], 1) != 0) {
        return AM_ZSL42X_LORA_RET_ERROR;
    }

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    if(am_zsl42x_lora_read_register (
            p_dev, AM_ZSL42X_LORA_REGADDR_RANDOMNUMBERGEN3, &data_buff[3], 1) != 0) {
        return AM_ZSL42X_LORA_RET_ERROR;
    }

    *p_random = (data_buff[0] << 24) |
                (data_buff[1] << 16) |
                (data_buff[2] << 8 ) |
                (data_buff[3] << 0);

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_time_pkt_symbol_get (void     *p_drv,
                                                     uint8_t   packet_len,
                                                     uint16_t *p_pkt_symbol_num)
{
    uint64_t          tmp;
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    tmp = (8 * packet_len                             +
           16 * (p_dev->setconfig.crc_enable ? 1 : 0) -
           4 * p_dev->setconfig.lora_setting.sf       +
           8                                          +
           (p_dev->setconfig.header_type ? 0 : 20)) * (uint64_t)(1000000);

    /* max(x,0) */
    if (tmp > 0) {
        tmp = tmp / (4 * (p_dev->setconfig.lora_setting.sf -
              ((p_dev->setconfig.lora_setting.lowdatarate_optimize > 0) ? 2 : 0)));
    } else {
        tmp = 0;
    }

    /* ceil() */
    if ((tmp % (uint64_t)1000000) < 10000) {
        tmp = ((tmp / (uint64_t)1000000) *
              (p_dev->setconfig.lora_setting.cr + 4)) * (uint64_t)1000000;
    } else {
        tmp = ((1 + (tmp / (uint64_t)1000000)) *
              (p_dev->setconfig.lora_setting.cr % 4 + 4)) * (uint64_t)1000000;
    }

    tmp /= (uint64_t)1000000;

    *p_pkt_symbol_num = 8 + tmp;

    return  AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_time_on_air_get (
                                            void                   *p_drv,
                              am_zsl42x_lora_server_radio_modems_t  modem,
                                            uint8_t                 packet_len,
                                            uint64_t               *p_time)
{
    am_zsl42x_lora_dev_t  *p_dev = (am_zsl42x_lora_dev_t *)p_drv;
    uint32_t          airTime = 0;

    switch (modem) {
    case AM_ZSL42X_LORA_MODEM_FSK: {
        airTime = (uint32_t )rint((8 * (p_dev->setconfig.preamble_length +
                  (p_dev->setconfig.fsk_setting.syncword_number >> 3 ) +
                  ((p_dev->setconfig.header_type == AM_ZSL42X_LORA_FSK_PACKET_FIXED_LENGTH ) ? 0.0 : 1.0) +
                   packet_len +
                  ((p_dev->setconfig.fsk_setting.crc_length == AM_ZSL42X_LORA_FSK_CRC_2_BYTE ) ? 2.0 : 0)) /
                   p_dev->setconfig.fsk_setting.datarate ) * 1000 );
    }
        break;

    case AM_ZSL42X_LORA_MODEM_LORA: {

        uint8_t  bw = __lora_bandwidth[p_dev->setconfig.lora_setting.bw];
        uint8_t  sf = p_dev->setconfig.lora_setting.sf;

        double ts = __radio_lora_symbtime[bw - 4][12 - sf];
        // time of preamble
        double tPreamble = (p_dev->setconfig.preamble_length + 4.25 ) * ts;
        // Symbol length of payload and time
        double tmp = ceil((8 * packet_len - 4 * sf +
                          28 + 16 * p_dev->setconfig.crc_enable -
                         ((p_dev->setconfig.header_type == AM_ZSL42X_LORA_PACKET_FIXED_LENGTH) ? 20 : 0)) /
                         (double)( 4 * (sf -
                         ((p_dev->setconfig.lora_setting.lowdatarate_optimize > 0 ) ? 2 : 0 )))) *
                         ((p_dev->setconfig.lora_setting.cr % 4 ) + 4 );

        double nPayload = 8 + ((tmp > 0 ) ? tmp : 0 );
        double tPayload = nPayload * ts;
        // Time on air
        double tOnAir = tPreamble + tPayload;
        // return milli seconds
        airTime = (uint32_t )floor(tOnAir + 0.999 );
        }

        break;

    default:
        break;
    }

    *p_time = airTime;

    return  AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_symbolnumtimeout_set (void *p_drv, uint32_t ms)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;
    uint32_t         bw, sf, symb_timeout;

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 进入STDBY_RC模式 */
    __zsl42x_lora_server_standby(p_drv);
    __zsl42x_lora_inside_wait_on_busy(p_dev);

    /* calculate symb */
    if (4 == __lora_bandwidth[p_dev->setconfig.lora_setting.bw]) {
        bw = 125000;
    } else if (5 == __lora_bandwidth[p_dev->setconfig.lora_setting.bw]) {
        bw = 250000;
    } else if  (6 == __lora_bandwidth[p_dev->setconfig.lora_setting.bw]) {
        bw = 500000;
    } else {
        bw = 125000;
    }

    sf           = p_dev->setconfig.lora_setting.sf;
    symb_timeout = ms * bw / ((1 << sf) * 1000);

    p_dev->setconfig.lora_setting.symbol_timeout = symb_timeout;

    am_zsl42x_lora_setsymbolnumtimeout_lora(p_dev, symb_timeout);

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_symbol_time_get (void *p_drv, float *p_time)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)p_drv;

    float datarate = 0.0;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    uint32_t bandwidth = 0;
    switch(__lora_bandwidth[p_dev->setconfig.lora_setting.bw]) {
    case AM_ZSL42X_LORA_BW_125000: bandwidth = 125000; break;
    case AM_ZSL42X_LORA_BW_250000: bandwidth = 250000; break;
    case AM_ZSL42X_LORA_BW_500000: bandwidth = 500000; break;
    default: break;
    }

    datarate = (float)(bandwidth >> p_dev->setconfig.lora_setting.sf);

    /* 时间（ms） */
    *p_time = (float)1000 / datarate;

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_tx_continuous_wave (void     *p_drv,
                                                    uint32_t  freq,
                                                    int8_t    power,
                                                    uint32_t  time)
{
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 选择硬件电路为发送 */
    __zsl42x_lora_inside_hw_tx_set(p_dev);

    /* 进入STDBY_RC模式 */
    __zsl42x_lora_server_standby(p_drv);
    __zsl42x_lora_inside_wait_on_busy(p_dev);

    /* 设置信道频率 */
    __zsl42x_lora_server_channel_set(p_drv, freq);

    /* TX功耗、斜坡时间  */
    __zsl42x_lora_inside_wait_on_busy(p_dev);
//    am_zsl42x_lora_settxparam(p_dev, power, AM_ZSL42X_LORA_RAMPTIME_40US);
    p_dev->setconfig.tx_power = power;

    am_lpsoftimer_value_set(&p_dev->tx_timeout_timer, time);
    am_lpsoftimer_start(&p_dev->tx_timeout_timer);

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_TX_RUNNING_STATE;

    __zsl42x_lora_inside_wait_on_busy(p_dev);
    am_zsl42x_lora_settxcontinuouswave(p_dev);

    return AM_ZSL42X_LORA_RET_OK;
}

static int __zsl42x_lora_server_rssi_get (
                                     void                   *p_drv,
                       am_zsl42x_lora_server_radio_modems_t  modem,
                                     int16_t                *p_rssi_value)
{
    (void)modem;
    am_zsl42x_lora_dev_t  *p_dev  = (am_zsl42x_lora_dev_t *)p_drv;

    if(p_drv == NULL || p_rssi_value == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    /* 处于接收模式才能获取rssi的瞬时值 */
    if(p_dev->state != AM_ZSL42X_LORA_RF_RX_RUNNING_STATE) {
        return AM_ZSL42X_LORA_RET_ERROR;
    }

    am_zsl42x_lora_getrssiinst(p_dev, (int8_t *)p_rssi_value);

    *p_rssi_value = -(*p_rssi_value) / 2;

    return AM_ZSL42X_LORA_RET_OK;
}

/**
 * \brief FSK  模式有独立的同步字节寄存器8字节
 *        lora 模式有两个字节的同步字节
 */
static int __zsl42x_lora_server_syncword_set (void    *p_drv,
                                              uint8_t *p_syncword,
                                              uint8_t  num)
{
    am_zsl42x_lora_dev_t  *p_dev     = (am_zsl42x_lora_dev_t *)p_drv;
    uint8_t           datalsb   = 0x14;     /* 默认初始值 */
    uint8_t           datamsb   = 0x24;     /* 默认初始值 */

    if(p_drv == NULL || p_syncword == NULL) {
        return -AM_ZSL42X_LORA_RET_EINVAL;
    }

    if(p_dev->state == AM_ZSL42X_LORA_RF_SLEEP_STATE) {
        __zsl42x_lora_server_wakeup(p_drv);
    }

    /* 进入STDBY_RC模式 */
    __zsl42x_lora_server_standby(p_drv);

    if(p_dev->setconfig.packet_type == AM_ZSL42X_LORA_RADIO_PACKET_FSK) {

        if(num > 8) {
            num = 8;
        }

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_write_register(
                p_dev, AM_ZSL42X_LORA_REGADDR_FSK_SYNCWORD0, &p_syncword[0], num);
    } else {

        if (num < 1) {
            return AM_ZSL42X_LORA_RET_ERROR;
        } else if (num >= 2) {

            datalsb = p_syncword[0];
            datamsb = p_syncword[1];

        } else {
            datalsb = p_syncword[0];
        }

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_write_register(
            p_dev, AM_ZSL42X_LORA_REGADDR_LORA_SYNCWORD_LSB, &datalsb, 1);

        __zsl42x_lora_inside_wait_on_busy(p_dev);
        am_zsl42x_lora_write_register(
            p_dev, AM_ZSL42X_LORA_REGADDR_LORA_SYNCWORD_MSB, &datamsb, 1);
    }

    return AM_ZSL42X_LORA_RET_OK;
}

am_zsl42x_lora_handle_t am_zsl42x_lora_init (
                                 am_zsl42x_lora_dev_t           *p_dev,
                                 const am_zsl42x_lora_devinfo_t *p_devinfo,
                                 am_zsl42x_lora_drv_funcs_t     *p_drv_funcs)
{
    am_zsl42x_lora_server_handle_t  lora_handle = (am_zsl42x_lora_server_handle_t)(&p_dev->lora_dev);

    /* 验证参数有效性 */
    if ((NULL == p_dev) || (NULL == p_devinfo) || (p_drv_funcs == NULL)) {
        return NULL;
    }

    /* 设备结构体赋值 */
    p_dev->p_devinfo               = p_devinfo;
    p_dev->p_dio_irq               = __zsl42x_lora_dio1_irq;
    p_dev->lora_dev.p_server_funcs = (am_zsl42x_lora_server_funcs_t *)&__g_lora_server_funcs;
    lora_handle->p_hw_drv          = (void *)p_dev;

    p_dev->p_drv_funcs = p_drv_funcs;
    /* 获取余下的底层驱动函数 */
    p_dev->p_drv_funcs->pfn_dio_is_valid = __zsl42x_lora_drv_dio_is_valid;
    p_dev->p_drv_funcs->pfn_int_flag_clr = __zsl42x_lora_drv_int_flag_clr;

    /* 平台硬件（引脚）初始化 */
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init((void *)p_dev);
    }

    /* 初始化配置参数 */
    p_dev->setconfig.packet_type        = AM_ZSL42X_LORA_RADIO_PACKET_LORA;
    p_dev->setconfig.frequency          = 480000000;
    p_dev->setconfig.tx_power           = 20;
    p_dev->setconfig.tx_ramptime        = 10;
    p_dev->setconfig.tx_base_addr       = 0;
    p_dev->setconfig.rx_base_addr       = 0;
    p_dev->setconfig.idel_mode          = AM_ZSL42X_LORA_RXTXFALLBACKMODE_STDBY_RC;
    p_dev->setconfig.iq_inverted_enable = AM_FALSE;
    p_dev->setconfig.preamble_length    = 8;
    p_dev->setconfig.header_type        = 0;
    p_dev->setconfig.payloadlength      = __ZSL42X_LORA_MAX_PAYLOAD_LRNGRH;
    p_dev->setconfig.crc_enable         = AM_FALSE;

    p_dev->setconfig.fsk_setting.datarate            = 50000;
    p_dev->setconfig.fsk_setting.bw                  = 0;
    p_dev->setconfig.fsk_setting.bw_afc              = 0;
    p_dev->setconfig.fsk_setting.freq_deviation      = 10000;
    p_dev->setconfig.fsk_setting.addr_filter_enable  = AM_FALSE;
    p_dev->setconfig.fsk_setting.node_addr           = 0;
    p_dev->setconfig.fsk_setting.broadcast_addr      = 0;
    p_dev->setconfig.fsk_setting.detector_length     = 8;
    p_dev->setconfig.fsk_setting.syncword_number     = 0;
    p_dev->setconfig.fsk_setting.p_syncword          = NULL;
    p_dev->setconfig.fsk_setting.whiteng_enable      = AM_FALSE;

    p_dev->setconfig.lora_setting.bw                   = 0;
    p_dev->setconfig.lora_setting.sf                   = 12;
    p_dev->setconfig.lora_setting.cr                   = AM_ZSL42X_LORA_LORA_CR_4_5;
    p_dev->setconfig.lora_setting.lowdatarate_optimize = AM_ZSL42X_LORA_LOW_DATA_DATE_OPTIMIZE_OFF;
    p_dev->setconfig.lora_setting.network_num          = AM_ZSL42X_LORA_SERVER_MAC_PUBLIC_SYNCWORD;
    p_dev->setconfig.lora_setting.freq_hop_on          = AM_FALSE;
    p_dev->setconfig.lora_setting.hop_period           = 0;
    p_dev->setconfig.lora_setting.symbol_timeout       = 0;

    /* 软件复位芯片 */
    __zsl42x_lora_server_reset((void *)p_dev);

    /* 唤醒芯片 */
    __zsl42x_lora_server_wakeup((void *)p_dev);

    /* 进入STDBY_RC模式 */
    __zsl42x_lora_server_standby((void *)p_dev);

    /* 设置调制器供电模式 */
    am_zsl42x_lora_setregulatormode(p_dev, AM_ZSL42X_LORA_STOPTIMER_DC_DC);

    /* SPI通信检测 */
    {
        uint8_t test = 0x11;

        __zsl42x_lora_server_reg_write(p_dev, AM_ZSL42X_LORA_REGADDR_PAYLOADLENGTH, &test, 1);
        test = 0;
        __zsl42x_lora_server_reg_read(p_dev, AM_ZSL42X_LORA_REGADDR_PAYLOADLENGTH, &test, 1);
        if(test != 0x11) {
            while (1);
        }
    }

    /* 设置接收、发送缓冲区的基地址 */
    am_zsl42x_lora_setbufferbaseaddress(p_dev, 0X00, 0X00);

    if (p_dev->p_devinfo->core_type == AM_ZSL42X_LORA_CORE_TYPE_SX1268) {
        // 使能DIO3 引脚给有源晶振供电
        am_zsl42x_lora_setdio3tcxoctrl(p_dev, AM_ZSL42X_LORA_DIO3TCXOCTRL_1700MV, 100);
    }

    am_zsl42x_lora_settxparam(p_dev, 0, AM_ZSL42X_LORA_RAMPTIME_40US);

    /* 校准所有 */
    __zsl42x_lora_inside_calibration_all((void *)p_dev);

    __zsl42x_lora_inside_wait_on_busy(p_dev);

    am_zsl42x_lora_calibrateimage(p_dev, p_devinfo->calibrate_image);

    __zsl42x_lora_inside_wait_on_busy(p_dev);

    /* 设置DIO1关联中断  */
    am_zsl42x_lora_setdioirqparams(p_dev,
                              p_devinfo->irq_mask,
                              p_devinfo->dio1_mask,
                              p_devinfo->dio1_mask,
                              p_devinfo->dio1_mask);

    /* 使能dio1中断 */
    p_dev->p_devinfo->pfn_dio_irq_enable(p_dev->p_devinfo->dio1_pin);

    /* 初始化超时定时器 */
    am_lpsoftimer_init(&p_dev->tx_timeout_timer,
                        __zsl42x_lora_inside_tx_timeout_irq,
                        (void *)lora_handle);
    am_lpsoftimer_init(&p_dev->rx_timeout_timer,
                        __zsl42x_lora_inside_rx_timeout_irq,
                        (void *)lora_handle);

    p_dev->p_rx_tx_buffer = &p_dev->rx_tx_buffer[0];

    /* 更新当前lora设备工作状态 */
    p_dev->state = AM_ZSL42X_LORA_RF_IDLE_STATE;

    return p_dev;
}

void am_zsl42x_lora_deinit (am_zsl42x_lora_handle_t handle)
{
    am_zsl42x_lora_dev_t *p_dev = (am_zsl42x_lora_dev_t *)handle;

    /* 复位芯片 */
    __zsl42x_lora_server_reset((void *)handle);

    /* 天线相关电路设置为接收状态 */
    __zsl42x_lora_inside_hw_rx_set(p_dev);

    /* 平台去初始化 */
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit((void *)p_dev);
    }

    /* 取消超时定时器初始化 */
    am_lpsoftimer_init(&p_dev->tx_timeout_timer, NULL, NULL);
    am_lpsoftimer_init(&p_dev->rx_timeout_timer, NULL, NULL);
}

/* end of file */
