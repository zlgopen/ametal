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
 * \brief LoRa modem registers and bits definitions
 *
 * \internal
 * \par Modification history
 * - 1.0.0 20-03-25  enf,first implementation.
 * - 1.0.1 20-09-23  yg, adaptation zsl42x.
 * \endinternal
 */

#ifndef __AM_ZSL42X_LORA_REG_H
#define __AM_ZSL42X_LORA_REG_H

#include "am_types.h"
#include "am_zsl42x_lora.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief LoRa 操作码枚举
 */
typedef enum {

    /** \brief Commands Selecting the Operating Modes of the Radio */
    AM_ZSL42X_LORA_OPCODE_SET_SLEEP              = 0x84, /** \brief 睡眠模式设置 */
    AM_ZSL42X_LORA_OPCODE_SET_STANDBY            = 0x80, /** \brief 待机模式设置 */
    AM_ZSL42X_LORA_OPCODE_SET_FS                 = 0xC1, /** \brief FS模式设置 */
    AM_ZSL42X_LORA_OPCODE_SET_TX                 = 0x83, /** \brief 发送设置 */
    AM_ZSL42X_LORA_OPCODE_SET_RX                 = 0x82, /** \brief 接受设置 */
    AM_ZSL42X_LORA_OPCODE_STOPTIMERONPREAMBLE    = 0x9F, /** \brief 停止超时检测 */
    AM_ZSL42X_LORA_OPCODE_SET_RXDUTYCYCLE        = 0x94, /** \brief 周期唤醒设置 */
    AM_ZSL42X_LORA_OPCODE_SET_CAD                = 0xC5, /** \brief 信道检测设置 */
    AM_ZSL42X_LORA_OPCODE_SET_TXCONTINUOUSWAVE   = 0xD1, /** \brief 所有包的测试命令 */
    AM_ZSL42X_LORA_OPCODE_SET_TXINFINITEPREAMBLE = 0xD2, /** \brief 测试命令 */
    AM_ZSL42X_LORA_OPCODE_SET_REGULATORMODE      = 0x96, /** \brief 设置调制器供电选择 */
    AM_ZSL42X_LORA_OPCODE_CALIBRATE              = 0x89, /** \brief 校准使能 */
    AM_ZSL42X_LORA_OPCODE_CALIBRATEIMAGE         = 0x98, /** \brief 特定频段的镜频校准 */
    AM_ZSL42X_LORA_OPCODE_SET_PACONFIG           = 0x95, /** \brief PA配置 */
    AM_ZSL42X_LORA_OPCODE_SET_RXTXFALLBACKMODE   = 0x93, /** \brief 发送接收完成进入的模式 */

    /** \brief Register and Buffer Access Commands */
    AM_ZSL42X_LORA_OPCODE_REGISTER_WRITE         = 0x0D, /** \brief 写寄存器 */
    AM_ZSL42X_LORA_OPCODE_REGISTER_READ          = 0x1D, /** \brief 读寄存器 */
    AM_ZSL42X_LORA_OPCODE_BUFFER_WRITE           = 0x0E, /** \brief 写数据到FIFO中 */
    AM_ZSL42X_LORA_OPCODE_BUFFER_READ            = 0x1E, /** \brief 从FIFO中读取数据 */

    /** \brief DIO and IRQ Control */
    AM_ZSL42X_LORA_OPCODE_SET_DIOIRQ_PARAMS = 0x08, /** \brief DIOx中断使能 */
    AM_ZSL42X_LORA_OPCODE_GET_IRQSTATUS     = 0x12, /** \brief 获取DIO中断状态 */
    AM_ZSL42X_LORA_OPCODE_CLEAR_IRQSTATUS   = 0x02, /** \brief 清除DIO中断状态 */
    AM_ZSL42X_LORA_OPCODE_SET_DIO2_RFSWITCH = 0x9D, /** \brief DIO2用作RF切换开关 */
    AM_ZSL42X_LORA_OPCODE_SET_DIO3_TCXOCTRL = 0x97, /** \brief DIO3作为外部TCXO配置电压控制 */

    /** \brief RF, Modulation and Packet Commands */
    AM_ZSL42X_LORA_OPCODE_SET_RF_FREQUENCY       = 0x86, /** \brief 设置RF频率 */
    AM_ZSL42X_LORA_OPCODE_SET_PACKET_TYPE        = 0x8A, /** \brief 设置帧类型 */
    AM_ZSL42X_LORA_OPCODE_GET_PACKET_TYPE        = 0x11, /** \brief 获取帧类型 */
    AM_ZSL42X_LORA_OPCODE_SET_TX_PARAMS          = 0x8E, /** \brief 设置TX参数 */
    AM_ZSL42X_LORA_OPCODE_SET_MODULATION_PARAMS  = 0x8B, /** \brief 设置调制参数*/
    AM_ZSL42X_LORA_OPCODE_SET_PACKET_PARAMS      = 0x8C, /** \brief 设置数据包相关参数 */
    AM_ZSL42X_LORA_OPCODE_SET_CAD_PARAMS         = 0x88, /** \brief 设置CAD相关参数 */
    AM_ZSL42X_LORA_OPCODE_SET_BUFFER_BASEADDRESS = 0x8F, /** \brief 接收发送缓冲区的基地址 */
    AM_ZSL42X_LORA_OPCODE_SET_LORA_SYMBNUM_TIMEOUT = 0xA0, /** \brief 设置超时符号数量时间 */

    /** \brief  Status Commands */
    AM_ZSL42X_LORA_OPCODE_GET_STATUS          = 0xC0, /** \brief 获取芯片状态 */
    AM_ZSL42X_LORA_OPCODE_GET_RSSI_INST       = 0x15, /** \brief 获取瞬间RSSI数据 */
    AM_ZSL42X_LORA_OPCODE_GET_RXBUFFER_STATUS = 0x13, /** \brief 获取接受buffer状态  */
    AM_ZSL42X_LORA_OPCODE_GET_PACKET_STATUS   = 0x14, /** \brief 获取包信息 */
    AM_ZSL42X_LORA_OPCODE_GET_STATS           = 0x10, /** \brief 获取最后几个数据包信息数量 */
    AM_ZSL42X_LORA_OPCODE_RESET_STATS         = 0x00, /** \brief 复位0x10命令得到的值 */
    AM_ZSL42X_LORA_OPCODE_GET_DEVICE_ERRORS   = 0x17, /** \brief 获取器件已发生的错误 */
    AM_ZSL42X_LORA_OPCODE_CLEAR_DEVICE_ERRORS = 0x07, /** \brief 清除所有器件错误  */
} am_zsl42x_lora_opcode_t;

/**< \brief Syncword for Private LoRa networks */
#define AM_ZSL42X_LORA_MAC_PRIVATE_SYNCWORD              0x1424

/**< \brief Syncword for Public LoRa networks */
#define AM_ZSL42X_LORA_MAC_PUBLIC_SYNCWORD               0x3444

/**
 * \brief LoRa 睡眠模式选择   冷启动、热启动
 */
typedef enum {
    AM_ZSL42X_LORA_SLEEP_MODE_COLD_START = 0, /**< \brief 唤醒时不保留配置 */
    AM_ZSL42X_LORA_SLEEP_MODE_WARM_START = 1, /**< \brief 唤醒时保留配置 */
} am_zsl42x_lora_sleep_mode_t;

/**
 * \brief LoRa RTC自动唤醒
 */
typedef enum {
    AM_ZSL42X_LORA_SLEEP_MODE_RTC_WAKEUP_DISABLE = 0,
    AM_ZSL42X_LORA_SLEEP_MODE_RTC_WAKEUP_ENABLE  = 1,
} am_zsl42x_lora_sleep_mode_rtc_wakeup_t;

/**
 * \brief sleep模式设置
 *
 * \param[in] am_zsl42x_lora_dev_t                   ：lora设备
 * \param[in] am_zsl42x_lora_sleep_mode_t            ：睡眠模式选择
 * \param[in] am_zsl42x_lora_sleep_mode_rtc_wakeup_t ：RTC自动唤醒使能(主要用于周期唤醒功能)
 *
 * \note 此命令只能在SDBY下发送，在NSS上升沿后，除了备份调整器和sleepconfig指定的块（RC64k），
 *       其他块都将关闭
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setsleep (am_zsl42x_lora_dev_t                  *p_dev,
                             am_zsl42x_lora_sleep_mode_t            mode,
                             am_zsl42x_lora_sleep_mode_rtc_wakeup_t enable);

/**
 * \brief LoRa RTC自动唤醒
 */
typedef enum {
    AM_ZSL42X_LORA_STANDBY_MODE_STDBY_RC   = 0,
    AM_ZSL42X_LORA_STANDBY_MODE_STDBY_XOSC = 1,
} am_zsl42x_lora_standby_mode_t;

/**
 * \brief standby模式设置
 *
 * \param[in] am_zsl42x_lora_dev_t          ：lora设备
 * \param[in] am_zsl42x_lora_standby_mode_t ：待机模式模式选择
 *
 * \note 此模式专门用于使用高级命令进行芯片配置。
 *       默认情况下，电池插入或复位操作发生的时候，芯片进入SDBY_RC模式，运行13MHz的RC时钟。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setstandby (am_zsl42x_lora_dev_t  *p_dev,
                               uint8_t                mode);
/**
 * \brief FS设置
 *
 * \param[in] am_zsl42x_lora_dev_t   :lora设备
 *
 * \note 用于测试PLL目的。从STDBY切换到TX或RX模式，将自动设置FS。
 *       在FS模式下，PLL的频率将被设置成Setrffrequency函数设置的频率。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setfs (am_zsl42x_lora_dev_t *p_dev);

/**
 * \brief TX设置
 *
 * \param[in] am_zsl42x_lora_dev_t     :lora设备
 * \param[in] data                     :超时时间设置（24bit数据）
 *
 * \note 从STDBY_RC模式开始，打开振荡器，打开PLL，打开PA，然后PA调节器根据Settxparams的设置完成配置，
 *       最后开始传输包数据。
 *       数据发送完成或失败，TX_DONE被置位或超时标志，PA调节器被降低，PA关闭，退回到STDBY_RC模式。
 *
 *       超时时间设置: Time = data * 15.625us (data:传入超时时间设置参数；Time:实际超时时间)
 *       当data设置为0时，代表禁用TX超时设置，只有传输完成，才能退回到STDBY_RC模式。
 *       当data设置不为0时，代表启用TX超时设置，传输完成或超时发生，退回到STDBY_RC模式。
 *
 *       data是一个24位的数据。Time超时时间最大为262s。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_settx (am_zsl42x_lora_dev_t *p_dev,
                          uint32_t              data);

/**
 * \brief RX设置
 *
 * \param[in] am_zsl42x_lora_dev_t     :lora设备
 * \param[in] data                     :超时时间设置（24bit数据）
 *
 * \note 设置为接收模式，等待一个或多个数据包的接收。
 *
 *       超时时间设置: Time = data * 15.625us (data:传入超时时间设置参数；Time:实际超时时间)
 *
 *       当data设置0x000000时，代表禁用RX超时设置，只有传输完成，才能退回到STDBY_RC模式。
 *       当data设置0xffffff时，代表启用RX连续模式，接收到数据或者超时，将继续尝试接收下一数据，
 *       不退回STDBY_RC模式。
 *       当data设置其他数值时，代表启用RX超时设置，接收完成或超时发生，退回到STDBY_RC模式。
 *
 *       如果启用了超时设置，在检测到前导符或同步字时（FSK）或者检测到包头（LoRa）时或直接调用stoptimeron
 *       preamble，停止超时时间的计时，直接接收整个数据。接受完成退回STDBY_RC模式。如果未检测到数据，则等待
 *       超时发生。
 *
 *       data是一个24位的数据。Time超时时间最大为262s。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setrx (am_zsl42x_lora_dev_t *p_dev,
                          uint32_t              data);

/**
 * \brief LoRa stoptimeronpreamble停止检测(接收超时)
 */
typedef enum {
    AM_ZSL42X_LORA_STOPTIMER_SYNCWORD_HEADER = 0x00,
    AM_ZSL42X_LORA_STOPTIMER_PREAMBLE        = 0x01,
} am_zsl42x_lora_stoptimeronpreamble_t;

/**
 * \brief ZSL42X LORA LowDataRateOptimize
 */
typedef enum {
    AM_ZSL42X_LORA_LOW_DATA_DATE_OPTIMIZE_OFF = 0X00,
    AM_ZSL42X_LORA_LOW_DATA_DATE_OPTIMIZE_ON  = 0X01,
} am_zsl42x_lora_lowdatarateoptimize_t;

/**
 * \brief stoptimeronpreamble设置
 *
 * \param[in] am_zsl42x_lora_dev_t    :lora设备
 * \param[in] type                    :停止接收检测（前导符、同步字和包头），直至数据接收完成
 *
 * \note 只针对接收超时有效时的检测，可调用此函数终结本次检测，直至数据接收完成
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_stoptimeronpreamble (am_zsl42x_lora_dev_t *p_dev,
                                        uint8_t               type);

/**
 * \brief 周期唤醒设置
 *
 * \param[in] am_zsl42x_lora_dev_t    :lora设备
 * \param[in] rx_period               :接收周期设定值（实际时间 = 设定值 * 15.625us）
 * \param[in] sleep_period            :睡眠周期设定值（实际时间 = 设定值 * 15.625us）
 *
 * \note 当设备处于STDBY_RC模式下，发送此命令，将保存设备配置，芯片进入一个循环，该循环由以下步骤定义：
 *          1.芯片进入RX模式，并监听数据包一段时间（时间由rx_period定义 ）
 *          2.检测到一个数据包开始（LoRa--包头、FSK--前导符或同步字）
 *          3.检测到一个数据包开始（LoRa--包头、FSK--前导符或同步字），将停止超时，并在2*rx_period +
 *            sleep_period的时间后，重新启动超时
 *          4.如果在rx_period的时间内未检测到数据包开始（LoRa--包头、FSK--前导符或同步字），则进入睡眠
 *            模式，睡眠时间由sleep_period设定。
 *          5.睡眠时间结束，重复前面步骤。
 *
 *       需要注意的是，主机在任何时刻都可以停止该循环过程。另外存在两种情况将打断循环：
 *          1.在rx_period时间内，检测到数据，通过RX_DONE标志给主机发送中断信号，并主动退回到STBY_RC模式。
 *          2.在rx_period时间内，主机发送setstandby命令（睡眠模式不受主机控制）。
 *
 *       时间计算方式：
 *          sleep = sleep_period * 15.625us
 *          rx    = rx_period * 15.625us
 *       在rx_period和sleep_period之间会插入500us的时间来保存配置以及模式的切换。这个时间不准确，跟硬件相关。
 *       如果使用自身的TCXO时，delay定义的启动延时也将在rx_period和sleep_period之间插入。
 *
 *       另外，因为检测到一个数据包开始后的接收时间为2*rx_period+sleep_period，为了防止伪序言检测需要满
 *       足以下情况
 *           T1(preamble)+T2(header) <= 2*rx_period+sleep_period
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setrxdutycycle (am_zsl42x_lora_dev_t  *p_dev,
                                   uint32_t               rx_period,
                                   uint32_t               sleep_period);

/**
 * \brief setcad信道活跃检测（LoRa特有的操作指令）
 *
 * \param[in] am_zsl42x_lora_dev_t : lora设备
 *
 * \note 根据setcadparams命令配置的长度，搜索LoRa信号，搜索完成后，返回到STDBY_RC模式。
 *
 *       在搜索结束时，如果启用irq_done，则会出发该信号。
 *
 *       如果有一个有效信号被检测到，会产生一个irq_caddetect信号。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setcad (am_zsl42x_lora_dev_t *p_dev);

/**
 * \brief settxcontinuouswave所有包的测试命令
 *
 * \param[in] am_zsl42x_lora_dev_t :lora设备
 *
 * \note 产生一个选定频率和输出功率的连续波（RX tone），设备保持TX连续波，直至主机发送配置命令。
 *
 *       实际中没有有价值的用例，但是可以为开发人员在TX模式下检查和监视性能有一定帮助。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_settxcontinuouswave (am_zsl42x_lora_dev_t *p_dev);

/**
 * \brief settxinfinitepreamble测试命令
 *
 * \param[in] am_zsl42x_lora_dev_t : lora设备
 *
 * \note 在FSK模式下，生成无限个交替的0和1的序列（0x55）。
 *       在LoRa模式下，只能不断调制前导符。
 *
 *       设备保持TX无限序言，不能发送任何数据，直至主机发送配置命令更改。
 *
 *       此命令可以帮助用户较为容易地检测到调制参数地光谱影响。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_settxinfinitepreamble (am_zsl42x_lora_dev_t *p_dev);

/**
 * \brief LoRa setregulatormode LDO、DC_DC选择
 */
typedef enum {
    AM_ZSL42X_LORA_STOPTIMER_LDO   = 0x00, /**< \brief 适用于所有模式 */
    AM_ZSL42X_LORA_STOPTIMER_DC_DC = 0x01, /**< \brief 适用于STBY_XOSC、FS、RX、TX模式 */
} am_zsl42x_lora_setregulatormode_t;

/**
 * \brief setregulatormode设置调制器供电选择（LDO、DC_DC）
 *
 * \param[in] am_zsl42x_lora_dev_t     :lora设备
 * \param[in] type                     :供电选择
 *
 * \note 默认使用LDO，使用DC_DC较导致功率的提升。调节参数有regmodeparam设定
 *
 *       跟具体硬件有关。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setregulatormode (am_zsl42x_lora_dev_t *p_dev,
                                     uint8_t               type);

/** \brief setcalibratefunction 校准使能宏定义（可以使用或操作多个校准）*/
#define AM_ZSL42X_LORA_SETCALIBRATE_ENABLE_RC64K       (1 << 0)
#define AM_ZSL42X_LORA_SETCALIBRATE_ENABLE_RC13M       (1 << 1)
#define AM_ZSL42X_LORA_SETCALIBRATE_ENABLE_PLL         (1 << 2)
#define AM_ZSL42X_LORA_SETCALIBRATE_ENABLE_ADCPULSE    (1 << 3)
#define AM_ZSL42X_LORA_SETCALIBRATE_ENABLE_ADCBULK_N   (1 << 4)
#define AM_ZSL42X_LORA_SETCALIBRATE_ENABLE_ADCBULK_P   (1 << 5)
#define AM_ZSL42X_LORA_SETCALIBRATE_ENABLE_IMAGE       (1 << 6)
#define AM_ZSL42X_LORA_SETCALIBRATE_ENABLE_ALL         (0x7F)

/**
 * \brief setcalibratefunction校准使能（RC64K/RC13M/PLL/ADC/IMAGE）
 *
 * \param[in] am_zsl42x_lora_dev_t   :lora设备
 * \param[in] flag  :AM_ZSL42X_LORA_SETCALIBRATE_ENABLE_xx宏定义使能（可使用或操作）
 *
 * \note 电源启动时，会对RC64K/RC13M/PLL/ADC进行校准。在STDBY_RC模式下，可通过命令对一个或多个模块校准。
 *       本命令为功能使能，具体配置由calibparam定义。
 *
 *       如果校准所有模块，则总共耗时3.5ms，BUSY才会再次变低。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setcalibratefunction (am_zsl42x_lora_dev_t *p_dev,
                                         uint8_t               flag);

/**
 * \brief LoRa calibrateimage 特定频段的镜频校准范围枚举
 */
typedef enum {
    AM_ZSL42X_LORA_CALIBRATE_IMAGE_430MHz_440MHz = 0,
    AM_ZSL42X_LORA_CALIBRATE_IMAGE_470MHz_510MHz = 1,
    AM_ZSL42X_LORA_CALIBRATE_IMAGE_779MHz_787MHz = 2,
    AM_ZSL42X_LORA_CALIBRATE_IMAGE_863MHz_870MHz = 3,
    AM_ZSL42X_LORA_CALIBRATE_IMAGE_902MHz_928MHz = 4,
} am_zsl42x_lora_calibrate_image_t;

/**
 * \brief calibrateimage特定频段的镜频校准
 *
 * \param[in] am_zsl42x_lora_dev_t             : lora设备
 * \param[in] am_zsl42x_lora_calibrate_image_t : 频段枚举（其他频段需要联系芯片厂商咨询）
 *
 * \note 用户可以选择两个参数来覆盖所有的ISM频段。
 *
 *       当POR或者设备从睡眠模式冷启动时，镜频校准作为初始校准的一部分，并对默认902MHz_928MHz进行校准。但是
 *       在此阶段设备内部不知道是否安装了XTAL或TCXO，所以校准将失败，应该在调用setdio3astcxoctrl后再请
 *       求一次完整的镜频校准。
 *
 *       默认镜频校准的频段902MHz_928MHz，可设置其他频段，或者咨询厂商获得更多频段参数。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_calibrateimage (am_zsl42x_lora_dev_t             *p_dev,
                                   am_zsl42x_lora_calibrate_image_t  type);

/** \brief sx1261推荐PA配置（15dBm、14dBm、10dBm）  */
#define AM_ZSL42X_LORA_PACONFIG_SX1261_15dBm_paDutyCycle   0x06   /* PAS占空比（导通角）*/
#define AM_ZSL42X_LORA_PACONFIG_SX1261_15dBm_hpMax         0x00   /* PA大小 */
#define AM_ZSL42X_LORA_PACONFIG_SX1261_15dBm_devieSel      0x01   /* 1261/1262选择 */
#define AM_ZSL42X_LORA_PACONFIG_SX1261_15dBm_paLut         0x01   /* 保留为1 */

#define AM_ZSL42X_LORA_PACONFIG_SX1261_14dBm_paDutyCycle   0x04   /* PAS占空比（导通角）*/
#define AM_ZSL42X_LORA_PACONFIG_SX1261_14dBm_hpMax         0x00
#define AM_ZSL42X_LORA_PACONFIG_SX1261_14dBm_devieSel      0x01
#define AM_ZSL42X_LORA_PACONFIG_SX1261_14dBm_paLut         0x01

#define AM_ZSL42X_LORA_PACONFIG_SX1261_10dBm_paDutyCycle   0x04   /* PAS占空比（导通角）*/
#define AM_ZSL42X_LORA_PACONFIG_SX1261_10dBm_hpMax         0x00   /* PA大小 */
#define AM_ZSL42X_LORA_PACONFIG_SX1261_10dBm_devieSel      0x01   /* 1261/1262选择 */
#define AM_ZSL42X_LORA_PACONFIG_SX1261_10dBm_paLut         0x01   /* 保留为1 */

/** \brief sx1262推荐PA配置（22dBm、20dBm、17dBm、14dBm）  */
#define AM_ZSL42X_LORA_PACONFIG_SX1262_22dBm_paDutyCycle   0x04   /* PAS占空比（导通角）*/
#define AM_ZSL42X_LORA_PACONFIG_SX1262_22dBm_hpMax         0x07   /* PA大小 */
#define AM_ZSL42X_LORA_PACONFIG_SX1262_22dBm_devieSel      0x00   /* 1261/1262选择 */
#define AM_ZSL42X_LORA_PACONFIG_SX1262_22dBm_paLut         0x01   /* 保留为1 */

#define AM_ZSL42X_LORA_PACONFIG_SX1262_20dBm_paDutyCycle   0x03   /* PAS占空比（导通角）*/
#define AM_ZSL42X_LORA_PACONFIG_SX1262_20dBm_hpMax         0x05   /* PA大小 */
#define AM_ZSL42X_LORA_PACONFIG_SX1262_20dBm_devieSel      0x00   /* 1261/1262选择 */
#define AM_ZSL42X_LORA_PACONFIG_SX1262_20dBm_paLut         0x01   /* 保留为1 */

#define AM_ZSL42X_LORA_PACONFIG_SX1262_17dBm_paDutyCycle   0x02   /* PAS占空比（导通角）*/
#define AM_ZSL42X_LORA_PACONFIG_SX1262_17dBm_hpMax         0x03   /* PA大小 */
#define AM_ZSL42X_LORA_PACONFIG_SX1262_17dBm_devieSel      0x00   /* 1261/1262选择 */
#define AM_ZSL42X_LORA_PACONFIG_SX1262_17dBm_paLut         0x01   /* 保留为1 */

#define AM_ZSL42X_LORA_PACONFIG_SX1262_14dBm_paDutyCycle   0x02   /* PAS占空比（导通角）*/
#define AM_ZSL42X_LORA_PACONFIG_SX1262_14dBm_hpMax         0x02   /* PA大小 */
#define AM_ZSL42X_LORA_PACONFIG_SX1262_14dBm_devieSel      0x00   /* 1261/1262选择 */
#define AM_ZSL42X_LORA_PACONFIG_SX1262_14dBm_paLut         0x01   /* 保留为1 */

/**
 * \brief setpaconfig PA配置
 *
 * \param[in] am_zsl42x_lora_dev_t      : lora设备
 * \param[in] paDutyCycle               : PAS占空比（导通角
 * \param[in] hpMax                     : PA大小
 * \param[in] devieSel                  : 1261/1262选择(0:1262   1:1261)
 * \param[in] paLut                     : 保留为1
 *
 * \note  paDutyCycle: PAS占空比（导通角）。最大输出功率、功耗、谐波都随着paDutyCycle变化。因此应该选
 *                     择与给定的匹配网络协同工作。
 *        hpMax      : PA大小。只对1262有效。最大输出功率可以通过降低hpMax的值来降低。该值得范围为
 *                     0x07--0x00之间。将值增大到0x07以上将导致设备老化，在极端温度下使用讲损坏设备。
 *
 *        特别注意：
 *           对于1261，合成频率低于400MHz，paDutyCycle不应超过0x04
 *                   合成频率大于400MHz，paDutyCycle不应超过0x07
 *
 *           对于1262，paDutyCycle不应超过0x04
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setpaconfig (am_zsl42x_lora_dev_t *p_dev,
                                uint8_t               paDutyCycle,
                                uint8_t               hpMax,
                                uint8_t               devieSel,
                                uint8_t               paLut);

/**
 * \brief LoRa setrxtxfallbackmode 接收完成、发送完成，退回到的模式枚举
 */
typedef enum {
    AM_ZSL42X_LORA_RXTXFALLBACKMODE_FS         = 0x40,
    AM_ZSL42X_LORA_RXTXFALLBACKMODE_STDBY_XOSC = 0x30,
    AM_ZSL42X_LORA_RXTXFALLBACKMODE_STDBY_RC   = 0x20,
} am_zsl42x_lora_rxtxfallbackmode_t;

/**
 * \brief setrxtxfallbackmode接收完成、发送完成，退回到的模式设置
 *
 * \param[in] am_zsl42x_lora_dev_t              : lora设备
 * \param[in] am_zsl42x_lora_rxtxfallbackmode_t : 接收完成、发送完成，退回到的模式
 *
 * \note 默认为STDBY_RC
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setrxtxfallbackmode (am_zsl42x_lora_dev_t *p_dev,
                                        uint8_t               type);

/**
 * \brief LoRa 寄存器地址枚举
 */
typedef enum {

    /** \brief DIO相关寄存器 */
    AM_ZSL42X_LORA_REGADDR_DIOX_OUT_ENABLE     = 0x0580,   /**< \brief DIOx输出使能 */
    AM_ZSL42X_LORA_REGADDR_DIOX_IN_ENABLE      = 0x0583,   /**< \brief DIOx输入使能 */
    AM_ZSL42X_LORA_REGADDR_DIOX_PU_CTRL        = 0x0584,   /**< \brief DIOx上拉控制 */
    AM_ZSL42X_LORA_REGADDR_DIOX_PD_CTRL        = 0x0585,   /**< \brief DIOx下拉控制 */

    /** \brief FSK模式下相关寄存器 */
    AM_ZSL42X_LORA_REGADDR_FSK_WHITEN_INIT_MSB = 0x06B8, /**< \brief CRC白化初始值 MSB */
    AM_ZSL42X_LORA_REGADDR_FSK_WHITEN_INIT_LSB = 0x06B9, /**< \brief CRC白化初始值 LSB */
    AM_ZSL42X_LORA_REGADDR_FSK_CRC_INIT_MSB    = 0x06BC, /**< \brief CRC初始值 MSB */
    AM_ZSL42X_LORA_REGADDR_FSK_CRC_INIT_LSB    = 0x06BD, /**< \brief CRC初始值 LSB */
    AM_ZSL42X_LORA_REGADDR_FSK_CRC_POLY_MSB    = 0x06BE, /**< \brief 多项式值MSB */
    AM_ZSL42X_LORA_REGADDR_FSK_CRC_POLY_LSB    = 0x06BF, /**< \brief 多项式值LSB */
    AM_ZSL42X_LORA_REGADDR_FSK_SYNCWORD0       = 0x06C0, /**< \brief 同步字0 */
    AM_ZSL42X_LORA_REGADDR_FSK_SYNCWORD1       = 0x06C1, /**< \brief 同步字1 */
    AM_ZSL42X_LORA_REGADDR_FSK_SYNCWORD2       = 0x06C2, /**< \brief 同步字2 */
    AM_ZSL42X_LORA_REGADDR_FSK_SYNCWORD3       = 0x06C3, /**< \brief 同步字3 */
    AM_ZSL42X_LORA_REGADDR_FSK_SYNCWORD4       = 0x06C4, /**< \brief 同步字4 */
    AM_ZSL42X_LORA_REGADDR_FSK_SYNCWORD5       = 0x06C5, /**< \brief 同步字5 */
    AM_ZSL42X_LORA_REGADDR_FSK_SYNCWORD6       = 0x06C6, /**< \brief 同步字6 */
    AM_ZSL42X_LORA_REGADDR_FSK_SYNCWORD7       = 0x06C7, /**< \brief 同步字7 */
    AM_ZSL42X_LORA_REGADDR_FSK_NODE_ADDR       = 0x06CD, /**< \brief 节点地址 */
    AM_ZSL42X_LORA_REGADDR_FSK_BROADCAST_ADDR  = 0x06CE, /**< \brief 广播地址 */

    /** \brief lora模式下相关寄存器 */
    AM_ZSL42X_LORA_REGADDR_LORA_SYNCWORD_MSB   = 0x0740,   /**< \brief 同步字MSB */
    AM_ZSL42X_LORA_REGADDR_LORA_SYNCWORD_LSB   = 0x0741,   /**< \brief 同步字LSB */

    /** \brief 其他寄存器 */
    AM_ZSL42X_LORA_REGADDR_RANDOMNUMBERGEN0    = 0x0819,   /**< \brief 随机数0 */
    AM_ZSL42X_LORA_REGADDR_RANDOMNUMBERGEN1    = 0x081A,   /**< \brief 随机数1 */
    AM_ZSL42X_LORA_REGADDR_RANDOMNUMBERGEN2    = 0x081B,   /**< \brief 随机数2 */
    AM_ZSL42X_LORA_REGADDR_RANDOMNUMBERGEN3    = 0x081C,   /**< \brief 随机数3 */
    AM_ZSL42X_LORA_REGADDR_RX_GAIN             = 0x08AC,   /**< \brief 接收增益   */
    AM_ZSL42X_LORA_REGADDR_OCP_CONFIG          = 0x08E7,   /**< \brief 过电流保护级别  */
    AM_ZSL42X_LORA_REGADDR_XTA_TRIM            = 0x0911,   /**< \brief XTA引脚调整 */
    AM_ZSL42X_LORA_REGADDR_XTB_TRIM            = 0x0912,   /**< \brief XTB引脚调整 */
    AM_ZSL42X_LORA_REGADDR_RTC_CTRL            = 0x0902,   /**< \brief RTC控制 */
    AM_ZSL42X_LORA_REGADDR_IQ_POLARITY_SETUP   = 0x0736,   /**< \brief IQ信号设置 */

    AM_ZSL42X_LORA_REGADDR_TX_MODULATION    = 0x0889, /**< \brief 带宽500kHz调制质量提升 */
    AM_ZSL42X_LORA_REGADDR_TX_CLAMPCONFIG   = 0x08D8, /**< \brief 最大功率天线匹配寄存器 */
    AM_ZSL42X_LORA_REGADDR_DIO3_OUT_VOLTAGE = 0x0920, /**< \brief DIO3输出电压控制*/
    AM_ZSL42X_LORA_REGADDR_EVENT_MASK       = 0x0944, /**< \brief 事件清除 */

    /* 手册未未给出的寄存器 */
    AM_ZSL42X_LORA_REGADDR_PACKETPARAMS  = 0x0704, /**< \brief 保存数据包配置的寄存器地址 */
    AM_ZSL42X_LORA_REGADDR_PAYLOADLENGTH = 0x0702, /**< \brief 保存有效负载长度的寄存器地址  */
} am_zsl42x_lora_regaddr_t;

/**
 * \brief write register 写寄存器
 *
 * \param[in] am_zsl42x_lora_dev_t        : lora设备
 * \param[in] am_zsl42x_lora_regaddr_t    : 寄存器地址
 * \param[in] data                        : 待写入寄存器数据
 * \param[in] len                         : 待写入数据长度
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_write_register (am_zsl42x_lora_dev_t     *p_dev,
                                   am_zsl42x_lora_regaddr_t  reg_addr,
                                   uint8_t                  *p_data,
                                   uint32_t                  len);

/**
 * \brief read register 读寄存器
 *
 * \param[in] am_zsl42x_lora_dev_t        : lora设备
 * \param[in] am_zsl42x_lora_regaddr_t    : 寄存器地址
 * \param[in] data                        : 读出数据缓存地址指针
 * \param[in] len                         : 读出数据长度
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_read_register (am_zsl42x_lora_dev_t      *p_dev,
                                  am_zsl42x_lora_regaddr_t   reg_addr,
                                  uint8_t                   *p_data,
                                  uint32_t                   len);

/**
 * \brief write buffer 写FIFO
 *
 * \param[in] am_zsl42x_lora_dev_t        : lora设备
 * \param[in] offset                      : 偏移地址（0——255）
 * \param[in] data                        : 待写入寄存器数据
 * \param[in] len                         : 待写入数据长度（256——1）
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_write_buffer (am_zsl42x_lora_dev_t  *p_dev,
                                 uint8_t                offset,
                                 uint8_t               *p_data,
                                 uint32_t               len);

/**
 * \brief write register 读FIFO
 *
 * \param[in] am_zsl42x_lora_dev_t        : lora设备
 * \param[in] offset                      : 偏移地址（0——255）
 * \param[in] len                         : 读出数据长度(256——1)
 * \param[in] data                        : 读出数据缓存地址指针
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_read_buffer (am_zsl42x_lora_dev_t  *p_dev,
                                uint8_t                offset,
                                uint32_t               len,
                                uint8_t               *p_data);

/** \brief setdioirqparams dio中断使能宏定义（可以使用或操作多个使能）*/
#define AM_ZSL42X_LORA_IRQ_MASK_TX_DONE        (1 << 0) /**< \brief 发送完成 */
#define AM_ZSL42X_LORA_IRQ_MASK_RX_DONE        (1 << 1) /**< \brief 接收完成 */
#define AM_ZSL42X_LORA_IRQ_MASK_PREAMBLE_DET   (1 << 2) /**< \brief 检测到前导符 */
#define AM_ZSL42X_LORA_IRQ_MASK_SYNCWORD_VALID (1 << 3) /**< \brief 同步字有效(FSK) */
#define AM_ZSL42X_LORA_IRQ_MASK_HEADER_VALID   (1 << 4) /**< \brief 包头有效(LoRa) */
#define AM_ZSL42X_LORA_IRQ_MASK_HEADER_ERR     (1 << 5) /**< \brief 包头错误(LoRa) */
#define AM_ZSL42X_LORA_IRQ_MASK_CRC_ERR        (1 << 6) /**< \brief CRC校验错误 */
#define AM_ZSL42X_LORA_IRQ_MASK_CAD_DONE       (1 << 7) /**< \brief CAD完成(LoRa) */
#define AM_ZSL42X_LORA_IRQ_MASK_CAD_DET        (1 << 8) /**< \brief CAD检测到信号(LoRa)*/
#define AM_ZSL42X_LORA_IRQ_MASK_TIMEOUT        (1 << 9) /**< \brief 超时 */
#define AM_ZSL42X_LORA_IRQ_MASK_ALL_DISABLE    (0x000)  /**< \brief 关闭所有 */
#define AM_ZSL42X_LORA_IRQ_MASK_ALL_ENABLE     (0xFFFF) /**< \brief 开启所有 */

/**
 * \brief setdioirqparams 设置DIO中断（可以使用或操作多个使能）
 *
 * \param[in] am_zsl42x_lora_dev_t   : lora设备
 * \param[in] irq_mask               : 中断使能
 * \param[in] dio1_mask              : DIO1中断选择（irq_mask和dio1_mask对应位同时置1，则中断有效）
 * \param[in] dio2_mask              : DIO2中断选择
 * \param[in] dio3_mask              : DIO3中断选择
 *
 * \note  dio2和dio3作为RF SW或TCXO时，即使设置了dio2_mask、dio3_mask也不会有中断信号输出。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setdioirqparams (am_zsl42x_lora_dev_t *p_dev,
                                    uint16_t              irq_mask,
                                    uint16_t              dio1_mask,
                                    uint16_t              dio2_mask,
                                    uint16_t              dio3_mask);

/**
 * \brief getirqstatus 获取中断状态标志
 *
 * \param[in] am_zsl42x_lora_dev_t        : lora设备
 * \param[in] p_status                    : 中断状态标志（OUT）
 *
 * \note  status & AM_ZSL42X_LORA_SETDIO_IRQ_MASK_xx 可得出某一中断标志是否有效
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_getirqstatus (am_zsl42x_lora_dev_t *p_dev,
                                 uint16_t             *p_status);

/**
 * \brief clearirqstatus 清除中断状态标志（可以使用或操作多个使能）
 *
 * \param[in] am_zsl42x_lora_dev_t    : lora设备
 * \param[in] clear_mask              : 清除中断状态（1：清除，0：保持）
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_clearirqstatus (am_zsl42x_lora_dev_t *p_dev,
                                   uint16_t              clear_mask);

/**
 * \brief setdio2rfswitch DIO2作为RF控制开关
 *
 * \param[in] am_lora_p_dev_t  : lora设备
 * \param[in] enable           : AM_TRUE  ：RF控制开关
 *                               AM_FALSE ：用作IRQ的DIO
 *
 * \note DIO2作为RF控制开关时，需要注意DIO2的输出电平关系：
 *           在SLEEP、STDBY_RC、STDBY_XOSC、FS、RX模式下，DIO2 = 0。
 *           在TX模式下，DIO2 = 1。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setdio2rfswitch (am_zsl42x_lora_dev_t *p_dev,
                                    uint8_t               enable);

/**
 * \brief LoRa setdio3tcxoctrl DIO3作为外部TCXO配置电压控制枚举
 */
typedef enum {
    AM_ZSL42X_LORA_DIO3TCXOCTRL_1600MV = 0x00,
    AM_ZSL42X_LORA_DIO3TCXOCTRL_1700MV = 0x01,
    AM_ZSL42X_LORA_DIO3TCXOCTRL_1800MV = 0x02,
    AM_ZSL42X_LORA_DIO3TCXOCTRL_2200MV = 0x03,
    AM_ZSL42X_LORA_DIO3TCXOCTRL_2400MV = 0x04,
    AM_ZSL42X_LORA_DIO3TCXOCTRL_2700MV = 0x05,
    AM_ZSL42X_LORA_DIO3TCXOCTRL_3000MV = 0x06,
    AM_ZSL42X_LORA_DIO3TCXOCTRL_3300MV = 0x07,
} am_zsl42x_lora_dio3tcxoctrl_t;

/**
 * \brief setdio3tcxoctrl DIO3作为外部TCXO配置电压控制
 *
 * \param[in] am_lora_p_dev_t    : lora设备
 * \param[in] voltage            : 电压选择
 * \param[in] data               : 24bit的延时时间设定值（延时时间 = 设定值 * 15.625us）
 *
 * \note 使用此命令时，设备通过DIO3控制TCXO。需要时（处于STDBY_XOSC、FS、TX、RX），内部状态机设置DIO3
 *       预设的电压值。在内部，时钟控制器等待32MHz出现，然后再释放内部状态机。32MHz出现和稳定时间可以通过data
 *       设置。如果延时结束时仍未出现32MHz，XOSC_START_ERR错误将被标记在错误控制器中。当设备从POR或冷启动
 *       的条件下从睡眠模式唤醒，且使用了TCXO，XOSC_START_ERR标志将会有效。用户应该使用cleardeviceer
 *       rors指令来清除这个标志。
 *
 *       voltage被设置成低于电压电压200mV，即使设置电压高于电源电压，也会被限制在 Vdd > Vtcxo + 200mv
 *
 *       延时时间 = 设定值 * 15.625us
 *
 *       大多数TCXO不会立即在需要的频率准备好，并且会遇到初始设置时间问题，缓慢想需要的频率靠近。延时结束，内部
 *       将停止控制时钟，无线电继续工作。
 *
 *       特别注意：
 *          从STDBY_RC模式切换到RX、TX模式时，需要考虑延迟时间问题。
 *          为避免增加模式切换的时间。用户可以现在STDBY_RC模式配置设备，打开TCXO，等待延时，
 *          再切换到RX、TX模式。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setdio3tcxoctrl (am_zsl42x_lora_dev_t          *p_dev,
                                    am_zsl42x_lora_dio3tcxoctrl_t  voltage,
                                    uint32_t                       data);

/**
 * \brief setrffrequency 设置RF频率
 *
 * \param[in] am_lora_p_dev_t  : lora设备
 * \param[in] rffreq           : RF频率（Hz）
 * \param[in] fxtal            : 晶振频率（Hz）
 *
 * \note 定义在FS、TX、RX的晶片频率。
 *       在RX模式下，自动配置所需频率的偏移量
 *
 *       计算式     RF（频率） = (设定值 * Fxtal) / 2^25
 *
 *             设定值 =  RF（频率）* 2^25 / Fxtal
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setrffrequency (am_zsl42x_lora_dev_t *p_dev,
                                   uint32_t              rffreq,
                                   uint32_t              fxtal);

/**
 * \brief setpackettype 设置帧类型（FKS、LoRa）
 *
 * \param[in] am_lora_p_dev_t    : lora设备
 * \param[in] type               : 0：FKS      1： LoRa
 *
 * \note 前一模式的配置不保留，必须在STDBY_RC模式下切换
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setpackettype (am_zsl42x_lora_dev_t *p_dev,
                                  uint8_t               type);

/**
 * \brief setpackettype 获取帧类型（FKS、LoRa）
 *
 * \param[in] am_lora_p_dev_t    : lora设备
 * \param[in] type               : 0：FKS  1： LoRa
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_getpackettype (am_zsl42x_lora_dev_t *p_dev,
                                  uint8_t              *p_type);

/**
 * \brief LoRa settxparams TX斜坡时间(us)
 */
typedef enum {
    AM_ZSL42X_LORA_RAMPTIME_10US   = 0x00,
    AM_ZSL42X_LORA_RAMPTIME_20US   = 0x01,
    AM_ZSL42X_LORA_RAMPTIME_40US   = 0x02,
    AM_ZSL42X_LORA_RAMPTIME_80US   = 0x03,
    AM_ZSL42X_LORA_RAMPTIME_200US  = 0x04,
    AM_ZSL42X_LORA_RAMPTIME_800US  = 0x05,
    AM_ZSL42X_LORA_RAMPTIME_1700US = 0x06,
    AM_ZSL42X_LORA_RAMPTIME_3400US = 0x07,
} am_zsl42x_lora_ramptime_t;

/**
 * \brief settxpower 设置TX参数
 *
 * \param[in] am_zsl42x_lora_dev_t    : lora设备
 * \param[in] power                   : 输出功率
 * \param[in] ramptime                : TX斜坡时间
 *
 * \note 低功耗PA时，power范围为   -17  ———— +14dBm  ， 步进1dBm
 *       高功耗PA时，power范围为   -9   ———— +22dBm  ， 步进1dBm
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_settxparam (am_zsl42x_lora_dev_t     *p_dev,
                               int8_t                    power,
                               am_zsl42x_lora_ramptime_t ramptime );

/**
 * \brief LoRa setmodulationparams_fsk 滤波选择枚举
 */
typedef enum {
    AM_ZSL42X_LORA_FSK_FILTER_NO       = 0x00,   /**< \brief 无滤波 */
    AM_ZSL42X_LORA_FSK_FILTER_GS_BT_03 = 0x08,   /**< \brief 高斯脉冲成型滤波器 BT=0.3*/
    AM_ZSL42X_LORA_FSK_FILTER_GS_BT_05 = 0x09,   /**< \brief 高斯脉冲成型滤波器 BT=0.5*/
    AM_ZSL42X_LORA_FSK_FILTER_GS_BT_07 = 0x0A,   /**< \brief 高斯脉冲成型滤波器 BT=0.7*/
    AM_ZSL42X_LORA_FSK_FILTER_GS_BT_10 = 0x0B,   /**< \brief 高斯脉冲成型滤波器 BT=1.0*/
} am_zsl42x_lora_fsk_filter_t;

/**
 * \brief LoRa setmodulationparams_fsk 带宽枚举
 */
typedef enum {
    AM_ZSL42X_LORA_FSK_RX_BW_4800   = 0x1F,   /**< \brief 接收带宽4.8kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_5800   = 0x17,   /**< \brief 接收带宽5.8kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_7300   = 0x0F,   /**< \brief 接收带宽7.3Hz */
    AM_ZSL42X_LORA_FSK_RX_BW_9700   = 0x1E,   /**< \brief 接收带宽9.7kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_11700  = 0x16,   /**< \brief 接收带宽11.7kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_14600  = 0x0E,   /**< \brief 接收带宽14.6kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_19500  = 0x1D,   /**< \brief 接收带宽19.5kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_23400  = 0x15,   /**< \brief 接收带宽23.4kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_29300  = 0x0D,   /**< \brief 接收带宽29.3kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_39000  = 0x1C,   /**< \brief 接收带宽39.0kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_46900  = 0x14,   /**< \brief 接收带宽46.9kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_58600  = 0x0C,   /**< \brief 接收带宽58.6kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_78200  = 0x1B,   /**< \brief 接收带宽78.2kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_93800  = 0x13,   /**< \brief 接收带宽93.8kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_117300 = 0x0B,   /**< \brief 接收带宽117.3kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_156200 = 0x1A,   /**< \brief 接收带宽156.2kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_187200 = 0x12,   /**< \brief 接收带宽187.2kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_234300 = 0x0A,   /**< \brief 接收带宽234.3kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_312000 = 0x19,   /**< \brief 接收带宽312.0kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_373600 = 0x11,   /**< \brief 接收带宽373.6kHz */
    AM_ZSL42X_LORA_FSK_RX_BW_467000 = 0x09,   /**< \brief 接收带宽467.0kHz */
} am_zsl42x_lora_fsk_bandwidth_t;

/**
 * \brief setmodulationparams_fsk 设置FSK调制参数
 *
 * \param[in] p_dev          : lora设备
 * \param[in] bitrate        : 目标比特率（600bps--300kbps）
 * \param[in] filter_type    : 滤波选择
 * \param[in] bandwidth      : 带宽选择
 * \param[in] freq_deviation : 目标偏移频率（Hz）
 * \param[in] fxtal          : 晶振频率（Hz）
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setmodulationparams_fsk (
                                am_zsl42x_lora_dev_t        *p_dev,
                                uint32_t                     bitrate,
                                am_zsl42x_lora_fsk_filter_t  filter_type,
                                uint8_t                      bandwidth,
                                uint32_t                     freq_deviation,
                                uint32_t                     fxtal);

/**
 * \brief LoRa setmodulationparams_zsl42x_lora 扩频因子枚举
 */
typedef enum {
    AM_ZSL42X_LORA_SF5  = 0x05,
    AM_ZSL42X_LORA_SF6  = 0x06,
    AM_ZSL42X_LORA_SF7  = 0x07,
    AM_ZSL42X_LORA_SF8  = 0x08,
    AM_ZSL42X_LORA_SF9  = 0x09,
    AM_ZSL42X_LORA_SF10 = 0x0A,
    AM_ZSL42X_LORA_SF11 = 0x0B,
    AM_ZSL42X_LORA_SF12 = 0x0C,
} am_zsl42x_lora_sf_t;

/**
 * \brief LoRa setmodulationparams_zsl42x_lora 带宽枚举
 */
typedef enum {
    AM_ZSL42X_LORA_BW_125000 = 0x04,  /**< \brief 125.0kHz */
    AM_ZSL42X_LORA_BW_250000 = 0x05,  /**< \brief 250.0kHz */
    AM_ZSL42X_LORA_BW_500000 = 0x06,  /**< \brief 500.0kHz */
} am_zsl42x_lora_bw_t;

/**
 * \brief LoRa setmodulationparams_zsl42x_lora 编码率枚举
 */
typedef enum {
    AM_ZSL42X_LORA_CR_4_5  = 0x01,  /**< \brief 4/5  1.25 */
    AM_ZSL42X_LORA_CR_4_6  = 0x02,  /**< \brief 4/6  1.5  */
    AM_ZSL42X_LORA_CR_4_7  = 0x03,  /**< \brief 4/7  1.75 */
    AM_ZSL42X_LORA_CR_4_8  = 0x04,  /**< \brief 4/8  2.0  */
} am_zsl42x_lora_cr_t;

/**
 * \brief setmodulationparams_lora 设置LoRa调制参数
 *
 * \param[in] p_dev              : lora设备
 * \param[in] sf_type            : 扩频因子
 * \param[in] bw_type            : 带宽
 * \param[in] cr_type            : 编码率
 * \param[in] lowdatarate_enable : 低数据速率优化使能（AM_TRUE:使能, AM_FALSE:禁能）
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setmodulationparams_lora (
                               am_zsl42x_lora_dev_t   *p_dev,
                               am_zsl42x_lora_sf_t     sf_type,
                               am_zsl42x_lora_bw_t     bw_type,
                               am_zsl42x_lora_cr_t     cr_type,
                               am_bool_t               lowdatarate_enable);

/**
 * \brief LoRa setpacketparams_fsk 前导符检测器长度
 */
typedef enum {
    AM_ZSL42X_LORA_FSK_PREAMBLE_DETECTOR_OFF     = 0x00, /**< \brief  关闭 */
    AM_ZSL42X_LORA_FSK_PREAMBLE_DETECTOR_BITS_8  = 0x04, /**< \brief 8 bits */
    AM_ZSL42X_LORA_FSK_PREAMBLE_DETECTOR_BITS_16 = 0x05, /**< \brief 16 bits */
    AM_ZSL42X_LORA_FSK_PREAMBLE_DETECTOR_BITS_24 = 0x06, /**< \brief 24 bits */
    AM_ZSL42X_LORA_FSK_PREAMBLE_DETECTOR_BITS_32 = 0x07, /**< \brief 32 bits */
} am_zsl42x_lora_fsk_detector_length_t;

/**
 * \brief LoRa setpacketparams_fsk 前导符检测器长度表
 */
typedef struct {
    uint32_t                             data;
    am_zsl42x_lora_fsk_detector_length_t detector_length;
} zsl42x_lora_fsk_detector_length_find_table_t;

/**
 * \brief LoRa setpacketparams_fsk 地址过滤功能选择
 */
typedef enum {
    AM_ZSL42X_LORA_FSK_ADDR_FILTER_DISABLE        = 0x00, /**< \brief 关闭 */
    AM_ZSL42X_LORA_FSK_ADDR_FILTER_NODE           = 0x01, /**< \brief 开启--节点地址  */
    AM_ZSL42X_LORA_FSK_ADDR_FILTER_NODE_BROADCAST = 0x02, /**< \brief 开启--节点、广播地址 */
} am_zsl42x_lora_fsk_addr_filter_t;

/**
 * \brief LoRa setpacketparams_fsk 数据包长度是否可变，是否将包场信息加载到包结构中
 */
typedef enum {

    /** \brief 有效负载大小固定，双方已知 */
    AM_ZSL42X_LORA_FSK_PACKET_FIXED_LENGTH     = 0x00,

    /** \brief 有效负载大小不固定，双方未知，有效负载大小信息加载到有效负载的第一字节 */
    AM_ZSL42X_LORA_FSK_PACKET_VARIABLE_LENGTH  = 0x01,
} am_zsl42x_lora_fsk_packetsize_info_t;

/**
 * \brief LoRa setpacketparams_fsk CRC功能选择
 */
typedef enum {
    AM_ZSL42X_LORA_FSK_CRC_OFF          = 0x01, /**< \brief CRC功能关闭 */
    AM_ZSL42X_LORA_FSK_CRC_1_BYTE       = 0x00, /**< \brief 1字节的CRC计算 */
    AM_ZSL42X_LORA_FSK_CRC_2_BYTE       = 0x02, /**< \brief 2字节的CRC计算 */
    AM_ZSL42X_LORA_FSK_CRC_1_BYTE_INV   = 0x04, /**< \brief 1字节的CRC计算并反转数据 */
    AM_ZSL42X_LORA_FSK_CRC_2_BYTE_INV   = 0x06, /**< \brief 2字节的CRC计算并反转数据 */
    AM_ZSL42X_LORA_FSK_CRC_2_BYTES_IBM  = 0xF1,
    AM_ZSL42X_LORA_FSK_CRC_2_BYTES_CCIT = 0xF2,
} am_zsl42x_lora_fsk_crc_type_t;

/**
 * \brief setpacketparams_fsk 设置FSK数据包相关参数
 *
 * \param[in] p_dev           : lora设备
 * \param[in] preamble_length : 前导符长度（0-65535）
 * \param[in] detector_length : 前导符检测器长度
 * \param[in] syncword_number : 同步字个数
 * \param[in] addr_filter     : 地址过滤选择
 * \param[in] sizeinfo_add    : 数据包长度信息（是否已知，是否将有效负载大小信息添加到有效负载的第一字节）
 * \param[in] payloadlength   : 有效负载大小（0-255）
 * \param[in] crc_type        : CRC类型选择
 * \param[in] whiteng_enable  : whiteng编码使能（AM_TRUE：使能       AM_FALSE：禁能）
 *
 * \note preamble_length---表示无线电发送序言的字节数，每个字节编码0x55。
 *       detector_length---检测器成功接收到设置长度的前导符bit，才会激活包控制器。
 *       syncword_number---0-8个同步字。另外，需要同步字写入制定寄存器中。
 *       addr_filter    ---需要8bit地址写入制定寄存器（节点、广播）中。
 *       sizeinfo_add   ---双方都知道有效负载的大小，则不添加有效负载大小信息到有效负载的第一字节。
 *                         有效负载的大小不固定，则添加有效负载大小信息到有效负载的第一字节。
 *       payloadlength  ---有效负载大小（0-255）
 *       crc_type       ---可以使用默认的CRC配置值，也可以通过写寄存器手动配置。
 *       whiteng_enable ---whiteng编码使能，通过写寄存器手动配置或使用默认配置。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setpacketparams_fsk (
                    am_zsl42x_lora_dev_t                  *p_dev,
                    uint16_t                               preamble_length,
                    am_zsl42x_lora_fsk_detector_length_t   detector_length,
                    uint8_t                                syncword_number,
                    am_zsl42x_lora_fsk_addr_filter_t       addr_filter,
                    am_zsl42x_lora_fsk_packetsize_info_t   sizeinfo_add,
                    uint8_t                                payloadlength,
                    am_zsl42x_lora_fsk_crc_type_t          crc_type,
                    am_bool_t                              whiteng_enable);

/**
 * \brief LoRa setpacketparams_lora 包头类型枚举
 */
typedef enum {
    AM_ZSL42X_LORA_PACKET_VARIABLE_LENGTH = 0x00,  /**< \brief 可变包长度 */
    AM_ZSL42X_LORA_PACKET_FIXED_LENGTH    = 0x01,  /**< \brief 固定包长度 */
} am_zsl42x_lora_header_type_t;

/**
 * \brief LoRa setpacketparams_lora CRC选择
 */
typedef enum {
    AM_ZSL42X_LORA_CRC_OFF = 0x00,
    AM_ZSL42X_LORA_CRC_ON  = 0x01,
} am_zsl42x_lora_crc_type_t;

/**
 * \brief LoRa setpacketparams_lora CRC选择
 */
typedef enum {
    AM_ZSL42X_LORA_IQ_STANDARD = 0x00,    /**< \brief 标准IQ设置 */
    AM_ZSL42X_LORA_IQ_INVERTED = 0x01,    /**< \brief 反转IQ设置 */
} am_zsl42x_lora_iq_type_t;

/**
 * \brief setpacketparams_fsk 设置FSK数据包相关参数
 *
 * \param[in] p_dev           : lora设备
 * \param[in] preamble_length : 前导符长度（0-65535）
 * \param[in] header_type     : 头类型
 * \param[in] payloadlength   : 有效负载大小（0-255）
 * \param[in] crc_type        : CRC使能
 * \param[in] iq_type         : IQ设置
 *
 * \note preamble_length---表示无线电发送序言的字节数。
 *       detector_length---检测器成功接收到设置长度的前导符bit，才会激活包控制器。
 *       header_type    ---显式头（可变长度包），将会把有效负载大小、编码速率、CRC方式添加到包头中。
 *       payloadlength  ---有效负载大小（0-255）
 *       crc_type       ---开启或关闭。
 *       iq_type        ---标准IQ信号设置、反转IQ信号设置。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setpacketparams_lora (
                           am_zsl42x_lora_dev_t        *p_dev,
                           uint16_t                     preamble_length,
                           am_zsl42x_lora_header_type_t header_type,
                           uint8_t                      payloadlength,
                           am_zsl42x_lora_crc_type_t    crc_type,
                           am_zsl42x_lora_iq_type_t     iq_type);

/**
 * \brief LoRa setcadparams_lora 符号数量
 */
typedef enum {
    AM_ZSL42X_LORA_CAD_SYMBOLNUM_1  = 0x00,
    AM_ZSL42X_LORA_CAD_SYMBOLNUM_2  = 0x01,
    AM_ZSL42X_LORA_CAD_SYMBOLNUM_4  = 0x02,
    AM_ZSL42X_LORA_CAD_SYMBOLNUM_8  = 0x03,
    AM_ZSL42X_LORA_CAD_SYMBOLNUM_16 = 0x04,
} am_zsl42x_lora_cad_symbolnum_t;

/**
 * \brief LoRa setcadparams_lora CAD检测到活动时退回的模式
 */
typedef enum {

    /** \brief CAD完成，直接退回到STDBY_RC模式 */
    AM_ZSL42X_LORA_CAD_EXTIMODE_ONLY = 0x00,

    /** \brief CAD检测到活动，停留在RX模式，直至数据接收完成或超时，再退回到STDBY_RC模式 */
    AM_ZSL42X_LORA_CAD_EXTIMODE_RX   = 0x01,
} am_zsl42x_lora_cad_extimode_t;

typedef struct {
    uint8_t  sf;
    uint8_t  detmin;
    uint8_t  detpeak;
    am_zsl42x_lora_cad_symbolnum_t  symbolnum;
} zsl42x_lora_cad_set_t;

/**
 * \brief setcadparams_lora 设置CAD相关参数
 *
 * \param[in] p_dev     : lora设备
 * \param[in] symbolnum : 符号数量
 * \param[in] detpeak   :
 * \param[in] detmin    :
 * \param[in] extimode  : CAD检测到活动时退回的模式选择
 * \param[in] data      : 超时时间设定值 （实际超时时间  = 设定值  *15.625us）
 *
 * \note detpeak、detmin---定义了LoRa调制解调器在尝试关联到实际的LoRa序言符号的灵敏度。这两个参数取决于
 *                        扩频因子和带宽，也取决于用于验证或不检测的符号数量。所选的值需要经过仔细的测试，
 *                        以确保在灵敏度较好的情况下有着良好检测，并限制错误检测的数量。应用笔记AN1200.
 *                        48提供了这些参数设置的指导
 *
 *       data          ---当extimode选择AM_ZSL42X_LORA_LORA_CAD_EXTIMODE_RX时，CAD检测完成
 *                        后进入单次接收模式，data即为单次接收模式中停留的的超时时间。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setcadparams_lora (
                                am_zsl42x_lora_dev_t          *p_dev,
                                am_zsl42x_lora_cad_symbolnum_t symbolnum,
                                uint8_t                        detpeak,
                                uint8_t                        detmin,
                                am_zsl42x_lora_cad_extimode_t  extimode,
                                uint32_t                       data);

/**
 * \brief setbufferbaseaddress 设置接收、发送缓冲区的基地址
 *
 * \param[in] p_dev          : lora设备
 * \param[in] tx_baseaddress : 数据发送地址
 * \param[in] rx_baseaddress : 数据接收地址
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setbufferbaseaddress (am_zsl42x_lora_dev_t *p_dev,
                                         uint8_t               tx_baseaddress,
                                         uint8_t               rx_baseaddress);

/**
 * \brief setsymbolnumtimeout_lora 设置调制解调器用于验证成功接收的符号数量
 *
 * \param[in] p_dev     : lora设备
 * \param[in] symbolnum : 符号数量
 *
 * \note 在lora模式下，进入RX时，一旦检测到LoRa符号，调制解调器将被锁定，这可能导致错误检测。此命令可用于验证包
 *       正确接收的符号数量。
 *
 *       symbolnum设置为0，调制解调器在检测到lora符号后立即验证接收。
 *
 *       symbolnum设置不为0，将等待总共symbolnum数量的符号正确验证，如果正确验证的符号数量不满足，将触发RX
 *       超时IRQ信号。
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_setsymbolnumtimeout_lora (am_zsl42x_lora_dev_t *p_dev,
                                             uint8_t               symbolnum);

/**
 * \brief LoRa getstatus 获取芯片状态（可在任意时候发出）
 * @{
 */

/** \brief 成功接收到数据包，且可读取 */
#define AM_ZSL42X_LORA_STATUS_FLAG_DATA_AVAILABLE       0x04

/** \brief 主机命令耗时太长，设置失败 */
#define AM_ZSL42X_LORA_STATUS_FLAG_COMMAND_TIMEOUT      0x06

/** \brief 主机命令错误，或参数错误 */
#define AM_ZSL42X_LORA_STATUS_FLAG_COMMAND_ERROR        0x08

/** \brief 主机命令有效，但无法执行命令 */
#define AM_ZSL42X_LORA_STATUS_FLAG_COMMAND_NOT_EXECUTE  0x0A

/** \brief 主机命令发送完成 */
#define AM_ZSL42X_LORA_STATUS_FLAG_COMMAND_TX_DONE      0x0C

/** \brief 当前处于STBY_RC模式  */
#define AM_ZSL42X_LORA_STATUS_MODE_STBY_RC              0x20

/** \brief 当前处于STBY_XOSC模式  */
#define AM_ZSL42X_LORA_STATUS_MODE_STBY_XOSC            0x30

/** \brief 当前处于FS模式  */
#define AM_ZSL42X_LORA_STATUS_MODE_FS                   0x40

/** \brief 当前处于RX模式  */
#define AM_ZSL42X_LORA_STATUS_MODE_RX                   0x50

/** \brief 当前处于TX模式  */
#define AM_ZSL42X_LORA_STATUS_MODE_TX                   0x60
/** @} */

/**
 * \brief getstatus 获取芯片状态（可在任意时候发出）
 *
 * \param[in] p_dev  : lora设备
 * \param[in] status : 状态数据
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_getstatus (am_zsl42x_lora_dev_t *p_dev,
                              uint8_t              *p_status);

/**
 * \brief getrxbufferstatus 获取接收buffer信息
 *
 * \param[in] p_dev                : lora设备
 * \param[in] rxstartbufferpointer : 接收数据的起始指针（OUT）
 * \param[in] payloadlength        : 接收数据长度（OUT）
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_getrxbufferstatus (am_zsl42x_lora_dev_t *p_dev,
                                      uint8_t              *p_rxstartbufferpointer,
                                      uint8_t              *p_payloadlength);


/**
 * \brief LoRa getpacketstatus_fsk 获取FSK状态
 */
typedef enum {
    AM_ZSL42X_LORA_FSK_STATUS_PREAMBLE_ERR = 0x80, /**< \brief 前导符错误 */
    AM_ZSL42X_LORA_FSK_STATUS_SYNC_ERR     = 0x40, /**< \brief 同步错误 */
    AM_ZSL42X_LORA_FSK_STATUS_ADRS_ERR     = 0x20, /**< \brief 地址错误 */
    AM_ZSL42X_LORA_FSK_STATUS_CRC_ERR      = 0x10, /**< \brief CRC错误 */
    AM_ZSL42X_LORA_FSK_STATUS_LENGTH_ERR   = 0x08, /**< \brief 长度错误 */
    AM_ZSL42X_LORA_FSK_STATUS_ABORT_ERR    = 0x04, /**< \brief 停止错误 */
    AM_ZSL42X_LORA_FSK_STATUS_PKT_RECEIVED = 0x02, /**< \brief 包已接收 */
    AM_ZSL42X_LORA_FSK_STATUS_PKT_SENT     = 0x01, /**< \brief 包已发送 */
} am_zsl42x_lora_fsk_status_t;

/**
 * \brief 数据包信道状态信息
 */
typedef struct {
    uint8_t   packettype;

    struct {
        struct {
            uint8_t rxstatus;
            int8_t rssiavg;      /**< \brief The averaged RSSI */
            int8_t rssisync;     /**< \brief The RSSI measured on last packet */
            uint32_t freqerror;
        } fsk;

        struct {
            int8_t rssipkt;        /**< \brief The RSSI of the last packet */
            int8_t snrpkt;         /**< \brief The SNR of the last packet */
            int8_t signalrssipkt;
            uint32_t freqerror;
        } lora;
    } params;
} am_radio_packet_status_t;

/**
 * \brief getpacketstatus_lora 获取lora包信息
 *
 * \param[in] p_dev         : lora设备
 * \param[in] p_pktstate    : 数据包信号状态信息（OUT）
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_getpacketstatus (am_zsl42x_lora_dev_t       *p_dev,
                                    am_radio_packet_status_t   *p_pktstate);

/**
 * \brief getrssiinst 处于接收模式时，获取RSSI的瞬间值
 *
 * \param[in] p_dev      : lora设备
 * \param[in] p_rssiinst : 处于接收模式时，获取RSSI的瞬间值（OUT）
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_getrssiinst (am_zsl42x_lora_dev_t *p_dev,
                                int8_t               *p_rssiinst);

/**
 * \brief getstats 获取最后几个数据包接收到的信息数量
 *
 * \param[in] p_dev                        : lora设备
 * \param[in] nbpkt_received               : nbpkt_received
 * \param[in] nbpkt_crcerror               : nbpkt_crcerror
 * \param[in] nbpkt_length_header_err      : nbpkt_length_err
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_getstats (am_zsl42x_lora_dev_t *p_dev,
                             uint8_t              *nbpkt_received,
                             uint8_t              *nbpkt_crcerror,
                             uint8_t              *nbpkt_length_header_err);

/**
 * \brief resetstats 重置getstats命令获取的数据
 *
 * \param[in] p_dev : lora设备
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_resetstats (am_zsl42x_lora_dev_t *p_dev);

/**
 * \brief LoRa getdeviceerrors 获取硬件错误状态
 */
typedef enum {

    AM_ZSL42X_LORA_DEVICE_ERROR_RC64K_CALIB = 0x001, /**< \brief RC64K校准失败 */
    AM_ZSL42X_LORA_DEVICE_ERROR_RC13M_CALIB = 0x002, /**< \brief RC13M校准失败 */
    AM_ZSL42X_LORA_DEVICE_ERROR_PLL_CALIB   = 0x004, /**< \brief PLL校准失败 */
    AM_ZSL42X_LORA_DEVICE_ERROR_ADC_CALIB   = 0x008, /**< \brief ADC校准失败 */
    AM_ZSL42X_LORA_DEVICE_ERROR_IMG_CALIB   = 0x010, /**< \brief IMG校准失败 */
    AM_ZSL42X_LORA_DEVICE_ERROR_XOSC_START  = 0x020, /**< \brief XOSC启动失败 */
    AM_ZSL42X_LORA_DEVICE_ERROR_PLL_LOCK    = 0x040, /**< \brief PLL锁定失败 */
    AM_ZSL42X_LORA_DEVICE_ERROR_PA_RAMP     = 0x100, /**< \brief PA上升失败*/
} am_zsl42x_lora_getdeviceerrors_t;

/**
 * \brief getdeviceerrors 获取硬件错误状态
 *
 * \param[in] p_dev  : lora设备
 * \param[in] status : 状态数据（OUT）--可以与am_zsl42x_lora_getdeviceerrors_t枚举相与判断具体错误
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_getdeviceerrors (am_zsl42x_lora_dev_t *p_dev,
                                    uint16_t             *p_status);

/**
 * \brief resetstats 重置getstats命令获取的数据
 *
 * \param[in] p_dev : lora设备
 *
 * \return 成功返回 AW_OK, 其它值表示失败
 */
int am_zsl42x_lora_cleardeviceerrors (am_zsl42x_lora_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42X_LORA_REG_H */

/* end of file */
