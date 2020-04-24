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
 * \brief ZSN700_READER底层驱动库头文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-01  htf, first implementation.
 * \endinternal
 */

#ifndef __AM_ZSN700_READER_H
#define __AM_ZSN700_READER_H

#include "am_spi.h"
#include "am_types.h"
#include "am_softimer.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_zsn700_reader
 * \copydoc am_zsn700_reader.h
 * @{
 */

/**
 * \name ZSN700_READER Command Code  ZSN700_READER命令码定义
 * @{
 */

#define AM_ZSN700_READER_CMD_IDLE              0x0  /**< \brief 无动作,取消当前指令执行 */
#define AM_ZSN700_READER_CMD_MEM               0x1  /**< \brief 存储25字节数据到内部缓冲器 */
#define AM_ZSN700_READER_CMD_RANDOM_ID         0x2  /**< \brief 产生一个10字节随机数 */
#define AM_ZSN700_READER_CMD_CALC_CRC          0x3  /**< \brief 激活CRC协处理器 */
#define AM_ZSN700_READER_CMD_TRANSMIT          0x4  /**< \brief 发射FIFO缓冲器中的数据 */
#define AM_ZSN700_READER_CMD_NO_CMD_CHANGE     0x7  /**< \brief 指令不改变,可以用来在不影响指令的情况下修改CMD寄存器,如PD位 */
#define AM_ZSN700_READER_CMD_RECEIVE           0x8  /**< \brief 激活接收器电路 */
#define AM_ZSN700_READER_CMD_TRANSCEIVE        0xC  /**< \brief 发射FIFO缓冲器中的数据到天线,并在发射后自动激活接收器 */
#define AM_ZSN700_READER_CMD_AUTHENT           0xE  /**< \brief 作为对泄气模式执行的M1安全认证 */
#define AM_ZSN700_READER_CMD_SOFT_RESET        0xF  /**< \brief 复位ZSN700_READER */

/** @} */


/**
 * \name 状态码定义
 * @{
 */
#define AM_ZSN700_READER_STATUS_SUCCESS                  (0)     /**< \brief 操作成功 */
#define AM_ZSN700_READER_STATUS_COMM_OK                  0x00    /**< \brief 成功 */
#define AM_ZSN700_READER_STATUS_COMM_TIMEOUT             0x01    /**< \brief 通信超时 */
#define AM_ZSN700_READER_STATUS_COMM_DVC_NOT_ONLINE      0x02    /**< \brief 设备不在线 */
#define AM_ZSN700_READER_STATUS_COMM_ERROR               0x03    /**< \brief 通信错误 */
#define AM_ZSN700_READER_STATUS_CMD_ERROR                0x04    /**< \brief 命令错误 */
#define AM_ZSN700_READER_STATUS_FAIL                     0x05    /**< \brief 操作失败，无帧数据 */
#define AM_ZSN700_READER_STATUS_SOFTIMER_ERROR           0x06    /**< \brief 操作失败，软件定时器初始化失败 */

#define AM_ZSN700_READER_STATUS_I2CMODE                  0x10    /**< \brief 模块是I2C通讯模式 */
#define AM_ZSN700_READER_STATUS_UARTMODE                 0x11    /**< \brief 模块是UART通讯模式 */

#define AM_ZSN700_READER_STATUS_IDLE                     0x8A    /**< \brief 空闲 */
#define AM_ZSN700_READER_STATUS_ABORT                    0x8B    /**< \brief 中止（非正常停止） */
#define AM_ZSN700_READER_STATUS_BUSY                     0x8C    /**< \brief 忙（正在执行其他命令） */
#define AM_ZSN700_READER_STATUS_EXECUTING                0x8D    /**< \brief 正在执行（命令未执行完） */
#define AM_ZSN700_READER_STATUS_SENDING                  0x8E    /**< \brief 正在发送（数据未发送完） */
#define AM_ZSN700_READER_STATUS_RECEIVING                0x8F    /**< \brief 正在接收（未超时，数据未接收完） */
        
#define AM_ZSN700_READER_STATUS_RFU90                    0x90    /**< \brief RFU 0x90 */
#define AM_ZSN700_READER_STATUS_RFU91                    0x91    /**< \brief RFU 0x91 */
#define AM_ZSN700_READER_STATUS_RFU92                    0x92    /**< \brief RFU 0x92 */
#define AM_ZSN700_READER_STATUS_SEQ_ERROR                0x93    /**< \brief 数据分包错误 */
#define AM_ZSN700_READER_STATUS_ADR_ERROR                0x94    /**< \brief 不是本机地址，或子地址错误 */
#define AM_ZSN700_READER_STATUS_RCV_LENGTH_ERR           0x95    /**< \brief 接收的长度有误 */
#define AM_ZSN700_READER_STATUS_MAC_ERR                  0x96    /**< \brief MAC校验有误 */
#define AM_ZSN700_READER_STATUS_USER_DATA_LEN_ERR        0x97    /**< \brief 用户输入数据长度有误 */
#define AM_ZSN700_READER_STATUS_USER_PARAMETER_ERR       0x98    /**< \brief 用户输入的参数有误 */
#define AM_ZSN700_READER_STATUS_USER_DATA_TOO_MANY       0x99    /**< \brief 用户数据过多 */
#define AM_ZSN700_READER_STATUS_USER_BUF_TOO_SMALL       0x9A    /**< \brief 用户缓冲区太小 */
#define AM_ZSN700_READER_STATUS_FUNCTION_ERR             0x9B    /**< \brief 功能错误 */
#define AM_ZSN700_READER_STATUS_INVALID_COMMAND          0x9C    /**< \brief 无效命令/不支持的命令 */
#define AM_ZSN700_READER_STATUS_NOT_SAME_CARD            0x9D    /**< \brief 不是同一张卡 */
#define AM_ZSN700_READER_STATUS_TRAN_NOT_COMPLETED       0x9E    /**< \brief 交易未完成 */
#define AM_ZSN700_READER_STATUS_APP_NOT_ENABLED_ERR      0x9F    /**< \brief 应用未启用 */
#define AM_ZSN700_READER_STATUS_IC_POWER_ERR             0x60    /**< \brief 读卡IC未上电配置 */

#define AM_ZSN700_READER_STATUS_CARD_TYPE_ERR            0xA0    /**< \brief 卡类型错误 */
#define AM_ZSN700_READER_STATUS_RFUA1                    0xA1    /**< \brief RFU 0xA1 */
#define AM_ZSN700_READER_STATUS_RFUA2                    0xA2    /**< \brief RFU 0xA2 */
#define AM_ZSN700_READER_STATUS_RFUA3                    0xA3    /**< \brief RFU 0xA3 */
#define AM_ZSN700_READER_STATUS_RFUA4                    0xA4    /**< \brief RFU 0xA4 */
#define AM_ZSN700_READER_STATUS_RFUA5                    0xA5    /**< \brief RFU 0xA5 */
#define AM_ZSN700_READER_STATUS_RFUA6                    0xA6    /**< \brief RFU 0xA6 */
#define AM_ZSN700_READER_STATUS_RFUA7                    0xA7    /**< \brief RFU 0xA7 */
#define AM_ZSN700_READER_STATUS_RFUA8                    0xA8    /**< \brief RFU 0xA8 */
#define AM_ZSN700_READER_STATUS_RFUA9                    0xA9    /**< \brief RFU 0xA9 */
#define AM_ZSN700_READER_STATUS_RFUAA                    0xAA    /**< \brief RFU 0xAA */
#define AM_ZSN700_READER_STATUS_RFUAB                    0xAB    /**< \brief RFU 0xAB */
#define AM_ZSN700_READER_STATUS_IID_ERROR                0xAC    /**< \brief IID错误(不支持的IID) */
#define AM_ZSN700_READER_STATUS_VPC_TIMEOUT              0xAD    /**< \brief 存在中继攻击 */
#define AM_ZSN700_READER_STATUS_NOT_SUPPORT_VC           0xAE    /**< \brief 不支持虚拟卡操作 */
#define AM_ZSN700_READER_STATUS_PCD_CAP_ERROR            0xAF    /**< \brief PCD能力有误 */

#define AM_ZSN700_READER_STATUS_RFUB0                    0xB0    /**< \brief RFU 0xB0 */
#define AM_ZSN700_READER_STATUS_RFUB1                    0xB1    /**< \brief RFU 0xB1 */
#define AM_ZSN700_READER_STATUS_RFUB2                    0xB2    /**< \brief RFU 0xB2 */
#define AM_ZSN700_READER_STATUS_RFUB3                    0xB3    /**< \brief RFU 0xB3 */
#define AM_ZSN700_READER_STATUS_RFUB4                    0xB4    /**< \brief RFU 0xB4 */
#define AM_ZSN700_READER_STATUS_RFUB5                    0xB5    /**< \brief RFU 0xB5 */
#define AM_ZSN700_READER_STATUS_AUTH_ERROR               0xB6    /**< \brief 验证错误 */
#define AM_ZSN700_READER_STATUS_CMD_OVERFLOW             0xB7    /**< \brief 命令(数据)溢出 */
#define AM_ZSN700_READER_STATUS_INVALID_MAC              0xB8    /**< \brief 无效的MAC */
#define AM_ZSN700_READER_STATUS_INVALID_BLOCK            0xB9    /**< \brief 无效的块 */
#define AM_ZSN700_READER_STATUS_INEXISTENT_BLOCK         0xBA    /**< \brief 不存在的块 */
#define AM_ZSN700_READER_STATUS_NOT_SATISFIED            0xBB    /**< \brief 安全等级不足 Security level is insufficient */
#define AM_ZSN700_READER_STATUS_MORE_THAN_RETRIES        0xBC    /**< \brief 超过重试次数 */
#define AM_ZSN700_READER_STATUS_HANDLE_ERROR             0xBD    /**< \brief 句柄错误 */
#define AM_ZSN700_READER_STATUS_INTERNAL_COMM_ERRPR      0xBE    /**< \brief 设备内部通信错误 */
#define AM_ZSN700_READER_STATUS_GENERAL_ERROR            0xBF    /**< \brief 普通操作错误 */

#define AM_ZSN700_READER_STATUS_NO_DATA_ERROR            0xC0    /**< \brief 无数据错误(需要发送数据时,FIFO中无数据) */
#define AM_ZSN700_READER_STATUS_INTEG_ERROR              0xC1    /**< \brief 数据完整性错误(包括CRC或奇偶校验错误) */
#define AM_ZSN700_READER_STATUS_NAD_ERROR                0xC2    /**< \brief NAD错误 */
#define AM_ZSN700_READER_STATUS_INVALID_VALUE            0xC3    /**< \brief 值块格式错误 */
#define AM_ZSN700_READER_STATUS_PERORM_ERROR             0xC4    /**< \brief 命令执行结果错误 */
#define AM_ZSN700_READER_STATUS_EDC_ERROR                0xC5    /**< \brief 校验字节有误(通信校验) */
#define AM_ZSN700_READER_STATUS_APDU_ERROR               0xC6    /**< \brief APDU命令错误 */
#define AM_ZSN700_READER_STATUS_SEND_ERROR               0xC7    /**< \brief 发送错误 */
#define AM_ZSN700_READER_STATUS_LENGTH_ERROR             0xC8    /**< \brief 长度错误 */
#define AM_ZSN700_READER_STATUS_NAK_ERROR                0xC9    /**< \brief NAK错误 */
#define AM_ZSN700_READER_STATUS_PCB_ERROR                0xCA    /**< \brief PCB错误 */
#define AM_ZSN700_READER_STATUS_CID_ERROR                0xCB    /**< \brief CID错误 */
#define AM_ZSN700_READER_STATUS_BLOCK_FORMAT_ERROR       0xCC    /**< \brief 分组帧格式错误  */
#define AM_ZSN700_READER_STATUS_PPS_ERROR                0xCD    /**< \brief PPSS错误 */
#define AM_ZSN700_READER_STATUS_ATS_ERROR                0xCE    /**< \brief ATS错误(ISO14443-4); 复位应答错(ISO7816-3) */
#define AM_ZSN700_READER_STATUS_CARD_COMM_ERROR          0xCF    /**< \brief 与卡片之间的通信出错 */

#define AM_ZSN700_READER_STATUS_EXECUTE_FAILED           0xD0    /**< \brief 执行失败 */
#define AM_ZSN700_READER_STATUS_COMPARE_ERROR            0xD1    /**< \brief 比较错误 */
#define AM_ZSN700_READER_STATUS_ACCESS_BANNED            0xD2    /**< \brief 禁止访问 */
#define AM_ZSN700_READER_STATUS_ACCESS_ERROR             0xD3    /**< \brief 存储器访问错误 */
#define AM_ZSN700_READER_STATUS_BCC_ERROR                0xD4    /**< \brief BCC校验错误 */
#define AM_ZSN700_READER_STATUS_NO_TARGET                0xD5    /**< \brief 无目标 */
#define AM_ZSN700_READER_STATUS_UNDEFINE_ERROR           0xD6    /**< \brief 未定义的错误 */
#define AM_ZSN700_READER_STATUS_NO_RF_FIELD              0xD7    /**< \brief 无RF场错误 */
#define AM_ZSN700_READER_STATUS_POWER_SHORTAGE           0xD9    /**< \brief 功率不足 */
#define AM_ZSN700_READER_STATUS_SERNR_ERROR              0xDA    /**< \brief 不支持的防碰撞等级 */
#define AM_ZSN700_READER_STATUS_NO_BITWISE_ANTICOLL      0xDB    /**< \brief 不支持Bit帧防冲突 */
#define AM_ZSN700_READER_STATUS_INIT_ERROR               0xDC    /**< \brief 初始化错误 */
#define AM_ZSN700_READER_STATUS_TEMP_ERROR               0xDD    /**< \brief 温度有误 */
#define AM_ZSN700_READER_STATUS_INVALID_DEVICE_STATE     0xDE    /**< \brief 驱动错误 */
#define AM_ZSN700_READER_STATUS_INSUFFICIENT_RESOURCES   0xDF    /**< \brief 系统资源不足 */

#define AM_ZSN700_READER_STATUS_OTHER_ERROR              0xE0    /**< \brief 其他错误 */
#define AM_ZSN700_READER_STATUS_NO_ID_AVAILABLE          0xE1    /**< \brief 无空闲ID可用 */
#define AM_ZSN700_READER_STATUS_ID_NOT_IN_USE            0xE2    /**< \brief 指定的ID不在使用 */
#define AM_ZSN700_READER_STATUS_INSTANCE_ALREADY_IN_USE  0xE3    /**< \brief INSTANCE 已被使用 */
#define AM_ZSN700_READER_STATUS_ID_ALREADY_IN_USE        0xE4    /**< \brief ID已被使用 */
#define AM_ZSN700_READER_STATUS_SLOT_ALREADY_IN_USE      0xE4    /**< \brief 卡座已被使用 */
#define AM_ZSN700_READER_STATUS_WRONG_HALT_FORMAT        0xE5    /**< \brief 挂起格式错误 */
#define AM_ZSN700_READER_STATUS_WRONG_UID_CHECKBYTE      0xE6    /**< \brief 目标UID检测错误 */
#define AM_ZSN700_READER_STATUS_TARGET_RESET_TOX         0xE7    /**< \brief 目标复位 */
#define AM_ZSN700_READER_STATUS_TARGET_SET_TOX           0xE8    /**< \brief 目标设置超时 */
#define AM_ZSN700_READER_STATUS_TARGET_DEADLOCKED        0xE9    /**< \brief 目标死锁 */
#define AM_ZSN700_READER_STATUS_BLOCKNR_NOT_EQUAL        0xEA    /**< \brief 通信块错误 */
#define AM_ZSN700_READER_STATUS_ACK_SUPPOSED             0xEB    /**< \brief ACK错误 */
#define AM_ZSN700_READER_STATUS_AUTHENT_ERROR            0xEC    /**< \brief 验证错误 */
#define AM_ZSN700_READER_STATUS_INTERFACE_NOT_ENABLED    0xED    /**< \brief 接口未激活 */
#define AM_ZSN700_READER_STATUS_INVALID_FORMAT           0xEE    /**< \brief 无效格式 */
#define AM_ZSN700_READER_STATUS_INTERFACE_ERROR          0xEF    /**< \brief 主机接口错误 */

#define AM_ZSN700_READER_STATUS_UNSUPPORTED_COMMAND      0xF0    /**< \brief 不支持的命令 */
#define AM_ZSN700_READER_STATUS_UNSUPPORTED_PARAMETER    0xF1    /**< \brief 不支持的参数 */
#define AM_ZSN700_READER_STATUS_INVALID_PARAMETER        0xF2    /**< \brief 无效参数 */
#define AM_ZSN700_READER_STATUS_USER_BUFFER_FULL         0xF3    /**< \brief 用户缓冲区满 */
#define AM_ZSN700_READER_STATUS_FIFO_WRITE_ERROR         0xF4    /**< \brief FIFO写错误 */
#define AM_ZSN700_READER_STATUS_NOT_YET_IMPLEMENTED      0xF5    /**< \brief 尚未生效 */
#define AM_ZSN700_READER_STATUS_PROTOCOL_ERROR           0xF6    /**< \brief 协议有误 */
#define AM_ZSN700_READER_STATUS_BUFFER_OVERFLOW          0xF7    /**< \brief BUF溢出错误 */
#define AM_ZSN700_READER_STATUS_FIFO_OVERFLOW            0xF7    /**< \brief FIFO溢出错误 */
#define AM_ZSN700_READER_STATUS_ACCESS_DENIED            0xF8    /**< \brief 写禁止 */
#define AM_ZSN700_READER_STATUS_BUFFER_TOO_SMALL         0xF9    /**< \brief 缓冲区过小 */
#define AM_ZSN700_READER_STATUS_COLLISION_ERROR          0xFA    /**< \brief 位冲突错误 */
#define AM_ZSN700_READER_STATUS_FRAMING_ERROR            0xFB    /**< \brief 帧错误 */
#define AM_ZSN700_READER_STATUS_BIT_COUNT_ERROR          0xFC    /**< \brief 接收位计数器错误 */
#define AM_ZSN700_READER_STATUS_PARITY_ERROR             0xFD    /**< \brief 奇偶校验错误 */
#define AM_ZSN700_READER_STATUS_CRC_ERROR                0xFE    /**< \brief CRC校验错误 */
#define AM_ZSN700_READER_STATUS_IO_TIMEOUT               0xFF    /**< \brief 无应答错误 */

/** @} */


/**
 * \name 定义延时常量
 *
 * \note 延时时间不宜超过100ms，否则可能导致MCU内部定时器无效。
 * @{
 */
#define AM_ZSN700_READER_RIC_DELAY1MS                2           /**< \brief 超时1ms */
#define AM_ZSN700_READER_RIC_DELAY2MS                3           /**< \brief 超时2ms */
#define AM_ZSN700_READER_RIC_DELAY5MS                6           /**< \brief 延时4.832ms */
#define AM_ZSN700_READER_RIC_DELAY10MS               11          /**< \brief 延时9.966ms */
#define AM_ZSN700_READER_RIC_DELAY20MS               21          /**< \brief 延时19.932ms */
#define AM_ZSN700_READER_RIC_DELAY50MS               51          /**< \brief 延时50.132ms */
#define AM_ZSN700_READER_RIC_DELAY100MS              101         /**< \brief 延时99.962ms */

/** @} */


/**
 * \name LPCD检测休眠阶段时间配置
 *
 * \note 用户可根据低功耗要求进行修改
 * @{
 */
#define AM_ZSN700_READER_LCPD_SLEEP0MS               0          /**< \brief 0ms */
#define AM_ZSN700_READER_LCPD_SLEEP200MS             1          /**< \brief 200ms */
#define AM_ZSN700_READER_LCPD_SLEEP400MS             2          /**< \brief 400ms */
#define AM_ZSN700_READER_LCPD_SLEEP500MS             3          /**< \brief 500ms */
#define AM_ZSN700_READER_LCPD_SLEEP600MS             4          /**< \brief 600ms */
#define AM_ZSN700_READER_LCPD_SLEEP700MS             5          /**< \brief 700ms */
#define AM_ZSN700_READER_LCPD_SLEEP800MS             6          /**< \brief 800ms */
#define AM_ZSN700_READER_LCPD_SLEEP900MS             7          /**< \brief 900ms */
#define AM_ZSN700_READER_LCPD_SLEEP1000MS            8          /**< \brief 1s */
#define AM_ZSN700_READER_LCPD_SLEEP1100MS            9          /**< \brief 1.1s */
#define AM_ZSN700_READER_LCPD_SLEEP1200MS            10         /**< \brief 1.2ms */
#define AM_ZSN700_READER_LCPD_SLEEP1300MS            11         /**< \brief 1.3ms */
#define AM_ZSN700_READER_LCPD_SLEEP1400MS            12         /**< \brief 1.4ms */
#define AM_ZSN700_READER_LCPD_SLEEP1500MS            13         /**< \brief 1.5ms */
#define AM_ZSN700_READER_LCPD_SLEEP1600MS            14         /**< \brief 1.6ms */
#define AM_ZSN700_READER_LCPD_SLEEP1700MS            15         /**< \brief 1.7ms */
/** @} */


/**
 * \name LPCD检测阶段时间配置
 *
 * \note 用户可根据低功耗要求进行修改
 * @{
 */
#define AM_ZSN700_READER_LCPD_DETECT0US              1           /**< \brief 0us */
#define AM_ZSN700_READER_LCPD_DETECT4US              2           /**< \brief 4.7us */
#define AM_ZSN700_READER_LCPD_DETECT9US              3           /**< \brief 9.4us */
#define AM_ZSN700_READER_LCPD_DETECT14US             4           /**< \brief 14.1us */
#define AM_ZSN700_READER_LCPD_DETECT18US             5           /**< \brief 18.8us */
#define AM_ZSN700_READER_LCPD_DETECT23US             6           /**< \brief 23.5us */
#define AM_ZSN700_READER_LCPD_DETECT28US             7           /**< \brief 28.2us */
#define AM_ZSN700_READER_LCPD_DETECT32US             8           /**< \brief 32.9us */
#define AM_ZSN700_READER_LCPD_DETECT37US             9           /**< \brief 37.6us */
#define AM_ZSN700_READER_LCPD_DETECT42US             10          /**< \brief 42.3us */
#define AM_ZSN700_READER_LCPD_DETECT47US             11          /**< \brief 47us */
#define AM_ZSN700_READER_LCPD_DETECT51US             12          /**< \brief 51.7us */
#define AM_ZSN700_READER_LCPD_DETECT56US             13          /**< \brief 56.4us */
#define AM_ZSN700_READER_LCPD_DETECT61US             14          /**< \brief 61.1us */
#define AM_ZSN700_READER_LCPD_DETECT65US             15          /**< \brief 65.8us */
#define AM_ZSN700_READER_LCPD_DETECT70US             16          /**< \brief 70.5us */
#define AM_ZSN700_READER_LCPD_DETECT75US             17          /**< \brief 75.2us */
#define AM_ZSN700_READER_LCPD_DETECT79US             18          /**< \brief 79.9us */
#define AM_ZSN700_READER_LCPD_DETECT84US             19          /**< \brief 84.6us */
#define AM_ZSN700_READER_LCPD_DETECT89US             20          /**< \brief 89.3us */
#define AM_ZSN700_READER_LCPD_DETECT94US             21          /**< \brief 94us */
#define AM_ZSN700_READER_LCPD_DETECT98US             22          /**< \brief 98.7us */
#define AM_ZSN700_READER_LCPD_DETECT103US            23          /**< \brief 103.4us */
#define AM_ZSN700_READER_LCPD_DETECT108US            24          /**< \brief 108.1us */
#define AM_ZSN700_READER_LCPD_DETECT112US            25          /**< \brief 112.8us */
#define AM_ZSN700_READER_LCPD_DETECT117US            26          /**< \brief 117.5us */
#define AM_ZSN700_READER_LCPD_DETECT122US            27          /**< \brief 122.2us */
#define AM_ZSN700_READER_LCPD_DETECT126US            28          /**< \brief 126.9us */
#define AM_ZSN700_READER_LCPD_DETECT131US            29          /**< \brief 131.6us */
#define AM_ZSN700_READER_LCPD_DETECT136US            30          /**< \brief 136.3us */
#define AM_ZSN700_READER_LCPD_DETECT141US            31          /**< \brief 141us */
/** @} */

/**
 * \name VMID 准备时间配置
 *
 * @{
 */
#define AM_ZSN700_READER_LPCD_VMID_0MS                0          /**< \brief 0ms */
#define AM_ZSN700_READER_LPCD_VMID_0_4MS              2          /**< \brief 0.4ms */
#define AM_ZSN700_READER_LPCD_VMID_0_5MS              3          /**< \brief 0.5ms */
#define AM_ZSN700_READER_LPCD_VMID_0_6MS              4          /**< \brief 0.6ms */
#define AM_ZSN700_READER_LPCD_VMID_0_7MS              5          /**< \brief 0.7ms */
#define AM_ZSN700_READER_LPCD_VMID_0_8MS              6          /**< \brief 0.8ms */
#define AM_ZSN700_READER_LPCD_VMID_0_9MS              7          /**< \brief 0.9ms */
#define AM_ZSN700_READER_LPCD_VMID_1_0MS              8          /**< \brief 1.0ms */
#define AM_ZSN700_READER_LPCD_VMID_1_1MS              9          /**< \brief 1.1ms */
#define AM_ZSN700_READER_LPCD_VMID_1_2MS              10         /**< \brief 1.2ms */
#define AM_ZSN700_READER_LPCD_VMID_1_3MS              11         /**< \brief 1.3ms */
#define AM_ZSN700_READER_LPCD_VMID_1_4MS              12         /**< \brief 1.4ms */
#define AM_ZSN700_READER_LPCD_VMID_1_5MS              13         /**< \brief 1.5ms */
#define AM_ZSN700_READER_LPCD_VMID_1_6MS              14         /**< \brief 1.6ms */
#define AM_ZSN700_READER_LPCD_VMID_1_7MS              15         /**< \brief 1.7ms */
#define AM_ZSN700_READER_LPCD_VMID_1_8MS              16         /**< \brief 1.8ms */
#define AM_ZSN700_READER_LPCD_VMID_1_9MS              17         /**< \brief 1.9ms */
#define AM_ZSN700_READER_LPCD_VMID_2_0MS              18         /**< \brief 2.0ms */
#define AM_ZSN700_READER_LPCD_VMID_2_1MS              19         /**< \brief 2.1ms */
#define AM_ZSN700_READER_LPCD_VMID_2_2MS              20         /**< \brief 2.2ms */
#define AM_ZSN700_READER_LPCD_VMID_2_3MS              21         /**< \brief 2.3ms */
#define AM_ZSN700_READER_LPCD_VMID_2_4MS              22         /**< \brief 2.4ms */
#define AM_ZSN700_READER_LPCD_VMID_2_5MS              23         /**< \brief 2.5ms */
#define AM_ZSN700_READER_LPCD_VMID_2_6MS              24         /**< \brief 2.6ms */
#define AM_ZSN700_READER_LPCD_VMID_2_7MS              25         /**< \brief 2.7ms */
#define AM_ZSN700_READER_LPCD_VMID_2_8MS              26         /**< \brief 2.8ms */
#define AM_ZSN700_READER_LPCD_VMID_2_9MS              27         /**< \brief 2.9ms */
#define AM_ZSN700_READER_LPCD_VMID_3_0MS              28         /**< \brief 3.0ms */
#define AM_ZSN700_READER_LPCD_VMID_3_1MS              29         /**< \brief 3.1ms */
#define AM_ZSN700_READER_LPCD_VMID_3_2MS              30         /**< \brief 3.2ms */
#define AM_ZSN700_READER_LPCD_VMID_3_3MS              31         /**< \brief 3.3ms */
/** @} */


/**
 * \name LPCD发送驱动配置
 *
 * @{
 */
#define AM_ZSN700_READER_LCPD_TX2_RF_ENABLE          1           /**< \brief LPCD TX2发射使能 */
#define AM_ZSN700_READER_LCPD_TX2_RF_DISABLE         0           /**< \brief LPCD TX2发射禁能 */

#define AM_ZSN700_READER_LCPD_RF_NMOS_46             0           /**< \brief LPCD 发射NMOS驱动配置为46欧姆 */
#define AM_ZSN700_READER_LCPD_RF_NMOS_23             1           /**< \brief LPCD 发射NMOS驱动配置为23欧姆 */

#define AM_ZSN700_READER_LCPD_RF_PMOS_180            0           /**< \brief LPCD 发射NMOS驱动配置为180欧姆 */
#define AM_ZSN700_READER_LCPD_RF_PMOS_90             1           /**< \brief LPCD 发射PMOS驱动配置为90欧姆 */
#define AM_ZSN700_READER_LCPD_RF_PMOS_46             2           /**< \brief LPCD 发射PMOS驱动配置为46欧姆 */
#define AM_ZSN700_READER_LCPD_RF_PMOS_23             3           /**< \brief LPCD 发射PMOS驱动配置为23欧姆 */
#define AM_ZSN700_READER_LCPD_RF_PMOS_12             4           /**< \brief LPCD 发射PMOS驱动配置为12欧姆 */
#define AM_ZSN700_READER_LCPD_RF_PMOS_6              5           /**< \brief LPCD 发射PMOS驱动配置为6欧姆 */
#define AM_ZSN700_READER_LCPD_RF_PMOS_3              6           /**< \brief LPCD 发射PMOS驱动配置为3欧姆 */
#define AM_ZSN700_READER_LCPD_RF_PMOS_1              7           /**< \brief LPCD 发射PMOS驱动配置为1.5欧姆 */
/** @} */


/**
 * \name 偏置电流设置
 *
 * @{
 * */
#define AM_ZSN700_READER_BIAS_CURRENT_50NA           1          /**< \brief LPCD 偏置电流50nA */
#define AM_ZSN700_READER_BIAS_CURRENT_100NA          2          /**< \brief LPCD 偏置电流150nA */
#define AM_ZSN700_READER_BIAS_CURRENT_150NA          3          /**< \brief LPCD 偏置电流200nA */
#define AM_ZSN700_READER_BIAS_CURRENT_200NA          4          /**< \brief LPCD 偏置电流250nA */
#define AM_ZSN700_READER_BIAS_CURRENT_250NA          5          /**< \brief LPCD 偏置电流300nA */
#define AM_ZSN700_READER_BIAS_CURRENT_300NA          6          /**< \brief LPCD 偏置电流350nA */
#define AM_ZSN700_READER_BIAS_CURRENT_350NA          7          /**< \brief LPCD 偏置电流400nA */
/** @} */


/**
 * \name LPCD检测参数配置
 *
 * @{
 */
#define AM_ZSN700_READER_LCPD_AUTO_DETECT_TIMES_3    0           /**< \brief LPCD 自动检测三次后产生中断 */
#define AM_ZSN700_READER_LCPD_AUTO_DETECT_TIMES_1    1           /**< \brief LPCD 自动检测一次后产生中断  */

#define AM_ZSN700_READER_LCPD_INT_DISABLE            0           /**< \brief LPCD 不产生中断 */
#define AM_ZSN700_READER_LCPD_INT_ENABLE             1           /**< \brief LPCD 产生中断 */

#define AM_ZSN700_READER_LCPD_AUTO_WUP_DISANLE       0           /**< \brief LPCD 禁能自动唤醒模式 */
#define AM_ZSN700_READER_LCPD_AUTO_WUP_ENABLE        1           /**< \brief LPCD 使能自动唤醒模式*/

#define AM_ZSN700_READER_LCPD_AUTO_WUP_TIME_6S       0           /**< \brief LPCD 配置自动唤醒时间  6s*/
#define AM_ZSN700_READER_LCPD_AUTO_WUP_TIME_12S      1           /**< \brief LPCD 配置自动唤醒时间 12s*/
#define AM_ZSN700_READER_LCPD_AUTO_WUP_TIME_15MIN    2           /**< \brief LPCD 配置自动唤醒时间 15minutes*/
#define AM_ZSN700_READER_LCPD_AUTO_WUP_TIME_30MIN    3           /**< \brief LPCD 配置自动唤醒时间 30minutes*/
#define AM_ZSN700_READER_LCPD_AUTO_WUP_TIME_60MIN    4           /**< \brief LPCD 配置自动唤醒时间 1hours*/
#define AM_ZSN700_READER_LCPD_AUTO_WUP_TIME_108MIN   5           /**< \brief LPCD 配置自动唤醒时间 1.8hours*/
#define AM_ZSN700_READER_LCPD_AUTO_WUP_TIME_216MIN   6           /**< \brief LPCD 配置自动唤醒时间 3.6hours*/
#define AM_ZSN700_READER_LCPD_AUTO_WUP_TIME_432MIN   7           /**< \brief LPCD 配置自动唤醒时间 7.2hours*/

/** @} */
/** \brief  LPCD模式初始化返回状态码*/
#define  AM_ZSN700_READER_INIT_SUCCESS               0x00        /**< \brief 设备LPCD模式初始化成功  */
#define  AM_ZSN700_READER_REG_INIT_ERROR             0x01        /**< \brief 设备LPCD模式寄存器初始化失败  */
#define  AM_ZSN700_READER_CALIBRA_INIT_ERROR         0x02        /**< \brief 设备LPCD模式调校初始化失败  */

/**
 * \name  T=CL 结构帧常亮定义
 * @{
 */
#define AM_ZSN700_READER_PCB_TYPE_MASK               0xC0        /**< \brief PCB类型掩码 */
#define AM_ZSN700_READER_PCB_BLOCK_NUM_MASK          0x01        /**< \brief PCB块数掩码 */
#define AM_ZSN700_READER_PCB_I_BLOCK                 0x00        /**< \brief I_BLOCK */
#define AM_ZSN700_READER_PCB_R_BLOCK                 0x80        /**< \brief R_BLOCK */
#define AM_ZSN700_READER_PCB_S_BLOCK                 0xC0        /**< \brief S_BLOCK */
#define AM_ZSN700_READER_PCB_CID_MASK                0x08        /**< \brief PCB中CID掩码 */
#define AM_ZSN700_READER_PCB_NAD_MASK                0x04        /**< \brief PCB中NAD掩码 */
#define AM_ZSN700_READER_PCB_CID                     0x08        /**< \brief PCB中CID */
#define AM_ZSN700_READER_PCB_NAD                     0x04        /**< \brief PCB中NAD */
#define AM_ZSN700_READER_PCB_CHAINING                0x10        /**< \brief 数据分链 */

#define AM_ZSN700_READER_PCB_I_RFU                   0x02        /**< \brief I_BLOCK 保留位 */

#define AM_ZSN700_READER_PCB_R_TYPE_MASK             0x10        /**< \brief R_BLOCK 类型掩码 */
#define AM_ZSN700_READER_PCB_R_NAK                   0x10        /**< \brief R_BLOCK NAK */
#define AM_ZSN700_READER_PCB_R_ACK                   0x00        /**< \brief R_BLOCK ACK */
#define AM_ZSN700_READER_PCB_R_RFU                   0x22        /**< \brief R_BLOCK 保留位 */

#define AM_ZSN700_READER_PCB_S_TYPE_MASK             0x30        /**< \brief S_BLOCK 类型掩码 */
#define AM_ZSN700_READER_PCB_S_WTX                   0x30        /**< \brief S_BLOCK WTX */
#define AM_ZSN700_READER_PCB_S_DESELECT              0x00        /**< \brief S_BLOCK DESELECT */
#define AM_ZSN700_READER_PCB_S_RFU                   0x02        /**< \brief S_BLOCK 保留位 */

/** @} */


#define AM_ZSN700_READER_LPCD_IRQ                    0x12        /**< \brief LPCD模式中断寄存器*/
#define AM_ZSN700_READER_LPCD_INT_IDLE               0x00        /**< \brief 无中断*/
#define AM_ZSN700_READER_LPCD_CARD_IRQ               0x01        /**< \brief 卡进场中断*/
#define AM_ZSN700_READER_LPCD_CALIB_IRQ              0x04        /**< \brief 调校完成中断 */
#define AM_ZSN700_READER_LPCD_WUP_IRQ                0x10        /**< \brief 自动唤醒中断 */
/** @} */

/**
 * \name  函数配置
 * @{
 */
#define AM_ZSN700_READER_MAX_FS_TAB_NO               9    /**< \brief 接口设备能接收的字节数表 */
#define AM_ZSN700_READER_MAX_FSDI                    8    /**< \brief 接口设备最大能接收缓冲区为256字节 */
#define AM_ZSN700_READER_MAX_FSCI                    8    /**< \brief 接口设备最大能发送缓冲区为256字节  */
                                         /**< \brief 0 -> 16字节; 1 -> 24字节;  2 -> 32字节; */
                                         /**< \brief 3 -> 40字节; 4 -> 48字节;  5 -> 64字节; */
                                         /**< \brief 6 -> 96字节; 7 -> 128字节; 8 -> 256字节 */

#define AM_ZSN700_READER_FSDI_BUF_SIZE    262  /**< \brief 定义接口设备缓冲区字节数 */

/**  @} */

/** \brief  定义ISO14443命令 */
#define AM_ZSN700_READER_PICC_RATS                   0xE0        /**< \brief ATS请求命令码 */
#define AM_ZSN700_READER_PICC_PPS                    0xD0        /**< \brief PPS命令 */

/** \brief  通信参数结构掩码定义 */
#define AM_ZSN700_READER_PARA_FO_CID_MASK            0x02        /**< \brief 通信参数中是否有CID掩码 */
#define AM_ZSN700_READER_PARA_FO_NAD_MASK            0x01        /**< \brief 通信参数中是否有NAD掩码 */

/**
 * \brief 定义协议类型和速度
 */
typedef enum am_zsn700_reader_prot_type
{
    /* ISO14443A协议，106kbit/s  */
    AM_ZSN700_READER_PROT_TYPE_ISO14443A_106 = 0x00,

    /**< \brief ISO14443B协议，106kbit/s */
    AM_ZSN700_READER_PROT_TYPE_ISO14443B_106 = 0x04,
} am_zsn700_reader_prot_type_t;


/**
 * \brief am_zsn700_reader_cmd_input_para zsn700_reader命令输入参数结构体定义
 */
typedef struct am_zsn700_reader_cmd_input_para
{
    uint8_t         cmd_code;       /**< \brief 命令类型 */
    uint16_t        tmo;            /**< \brief 超时时间 */
    uint16_t        nbytes;         /**< \brief 发送字节数 */
    const void     *pbuf;           /**< \brief 发送数据 */
} am_zsn700_reader_cmd_input_para_t;


/**
 * \brief am_zsn700_reader_cmd_output_para zsn700_reader命令输出参数结构体定义
 */
typedef struct am_zsn700_reader_cmd_output_para
{
    uint8_t         nlastbits;      /**< \brief 最后字节接收的位 */
    uint16_t        nbufsize;       /**< \brief 接收缓冲区大小 */
    uint16_t        nbytes;         /**< \brief 接收的字节数 */
    void           *pbuf;           /**< \brief 接收缓冲区 */
} am_zsn700_reader_cmd_output_para_t;

/**
 * \brief am_zsn700_reader_cmd_info 命令处理过程信息结构体定义
 */
typedef struct am_zsn700_reader_cmd_info
{
    uint8_t         status;         /**< \brief 状态 */
    uint8_t         dorcv;          /**< \brief 是否允许接收标志 */
    uint8_t         ncollpos;       /**< \brief 碰撞位置 */
    uint8_t         nlastbits;      /**< \brief 最后字节接收位 */

    uint8_t         irqen;          /**< \brief 请求的中断 */
    uint8_t         waitirq;        /**< \brief 等待的中断 */
    uint8_t         irqsource;      /**< \brief 所有的中断 */

    uint16_t        nbytesreceived; /**< \brief 已接收的字节数 */
    uint16_t        nbytessent;     /**< \brief 已发送的字节数 */
    uint16_t        nbytestosend;   /**< \brief 将要发送的字节数 */
    uint16_t        nmaxbufsize;    /**< \brief 最大用户缓冲区长度 */

    const void     *psbuf;          /**< \brief 发送数据缓冲区 */
    void           *prbuf;          /**< \brief 接收数据缓冲区 */
} am_zsn700_reader_cmd_info_t;


/**
 * \brief T=CL协议参数
 */
typedef struct am_zsn700_reader_tpcl_prot_para {
    uint8_t pcb_num;                /**< \brief 分包标志 */
    uint8_t cid;                    /**< \brief CID */
    uint8_t fsdi;                   /**< \brief PCD  能接收的最大帧 */
    uint8_t fsci;                   /**< \brief PICC 能接收的最大帧 */
    uint8_t ta1;                    /**< \brief 含DR、DS */
    uint8_t bit_rate;               /**< \brief 位速率（同TA1，含DR、DS） */
    uint8_t fwi;                    /**< \brief 帧等待时间因子 */
    uint8_t sfgi;                   /**< \brief 帧保护时间因子 */
    uint8_t tc1;                    /**< \brief 含是否支持CID、NID */
    uint8_t fo;                     /**< \brief 选择是否支持CID、NID（同TC1） */
} am_zsn700_reader_tpcl_prot_para_t;

/**
 * \brief ZSN700_READER LPCD模式参数配置信息定义
 */
typedef struct am_zsn700_reader_lpcd_cfginfo {
    uint8_t         sleep_time;                      /**< \brief LPCD检测休眠阶段时间配置 */
    uint8_t         detect_time;                     /**< \brief LPCD检测阶段时间配置 */
    uint8_t         vmid_time;                       /**< \brief VMID准备时间配置 */

    uint8_t         detect_range;                    /**< \brief LPCD检测范围*/
    uint8_t         detect_threshold;                /**< \brief LPCD检测阈值*/

    uint8_t         ie;                              /**< \brief LPCD中断使能配置*/
    uint8_t         auto_detect_times;               /**< \brief LPCD自动检测次数配置*/
    uint8_t         wake_up_enable;                  /**< \brief LPCD自动唤醒使能*/
    uint8_t         wake_up_time;                    /**< \brief LPCD自动唤醒时间配置*/

    uint8_t         bias_current;                    /**< \brief 偏置电流设置  */

} am_zsn700_reader_lpcd_cfginfo_t;

/**
 * \brief ZSN700_READER 设备信息定义
 */
typedef struct am_zsn700_reader_devinfo {
    am_zsn700_reader_prot_type_t             iso_type;       /**< \brief 初始使用的协议类型 */
    const  am_zsn700_reader_lpcd_cfginfo_t  *p_lpcd_cfg_info;  /**< \brief lpcd模式配置信息 */
} am_zsn700_reader_devinfo_t;


/**
 * \brief ZSN700_READER LPCD 模式信息
 */
typedef struct am_zsn700_reader_lpcd_modeinfo {
    int8_t                                    int_flag;       /**< \brief LPCD模式中断标识*/
    const am_zsn700_reader_lpcd_cfginfo_t    *p_cfg_info;   /**< \brief LPCD模式配置信息*/
} am_zsn700_reader_lpcd_modeinfo_t;;


/**
 * \brief ZSN700_READER LPCD模式调校变量信息定义
 */
typedef struct am_zsn700_reader_lpcd_calibrate_info {
    uint8_t        detect_timer_clkdivk;       /**< \brief LPCD 检测定时器分频数 */
    uint8_t        bias_current;               /**< \brief LPCD LPCD基准电流配置 */
    uint8_t        gain_amplify;               /**< \brief LPCD 放大器倍数 */
    uint8_t        adc_reference;              /**< \brief LPCD ADC电压参照电平 */

    uint8_t        sleep_time;                 /**< \brief LPCD 睡眠时间 */
    uint8_t        ready_time;                 /**< \brief LPCD 检测准备时间 */
    uint8_t        detect_time;                /**< \brief LPCD 检测时间  */
    uint8_t        detect_time_offset;         /**< \brief LPCD 检测时间偏移值  */

    uint8_t        adc_full_scale;             /**< \brief LPCD 定时器满幅下ADC值 */
    uint8_t        adc_threshold;              /**< \brief LPCD 检测幅度，设置为相对值 */
    uint8_t        lpcd_threshold_l;           /**< \brief LPCD 幅度低阈值 */
    uint8_t        lpcd_threshold_h;           /**< \brief LPCD 幅度高阈值 */
    uint8_t        adc_center;                 /**< \brief LPCD 幅度中心阈值 */

    uint8_t        detect_threshold;           /**< \brief LPCD 电压检测阈值范围 */

    uint8_t        cwn;
    uint8_t        cwp;
    uint8_t        tx2_en;

}am_zsn700_reader_calibrate_info_t;

/**
 * \brief ZSN700_READER 设备定义
 */
typedef struct am_zsn700_reader_dev {
    am_softimer_t                  timer;              /**< \brief 软件定时器，用于超时  */
    volatile uint32_t              tmo_cnt;            /**< \brief 超时计数器 */
    am_zsn700_reader_prot_type_t   iso_type;           /**< \brief 保存读卡芯片协议 */
    am_zsn700_reader_cmd_info_t    cmd_info;           /**< \brief 命令信息 */
    volatile uint8_t               read_flag;          /**< \brief 天线复位后读卡标志 */
    volatile uint8_t               tx_state;           /**< \brief 天线状态 */
    volatile am_bool_t             power_down;         /**< \brief 掉电标志 */

    am_zsn700_reader_tpcl_prot_para_t  cur_prot_para;  /**< \brief T=CL通信协议参数 */

    const am_zsn700_reader_devinfo_t  *p_devinfo;      /**< \brief 设备信息 */

    void                      (*lpcd_int_cb)(void *p_arg);
    void                       *p_lpcd_cb_arg;
} am_zsn700_reader_dev_t;

/* ZSN700_READER 设备句柄 */
typedef am_zsn700_reader_dev_t* am_zsn700_reader_handle_t;

/**
 * \name 通用函数接口声明
 * @{
 */

/**
 * \brief zsn700_reader 初始化，得到zsn700_reader设备
 *
 * \param[out] p_dev       ：zsn700_reader设备
 * \param[in]  p_devinfo   ：设备信息
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
am_zsn700_reader_handle_t am_zsn700_reader_init (am_zsn700_reader_dev_t           *p_dev,
                                                 const am_zsn700_reader_devinfo_t *p_info);
/**
 * \brief 写zsn700_reader 寄存器
 *
 * \param[in] p_dev    : zsn700_reader设备
 * \param[in] nregaddr : 寄存器地址
 * \param[in] nregval  : 写入的值
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
am_err_t am_zsn700_reader_set_reg (am_zsn700_reader_dev_t *p_dev,
                                   uint8_t                 nregadr,
                                   uint8_t                 nregval);


/**
 * \brief 写zsn700_reader 拓展寄存器
 *
 * \param[in] p_dev    : zsn700_reader设备
 * \param[in] nregaddr : 拓展寄存器地址
 * \param[in] nregval  : 拓展寄存器写入的值
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
am_err_t am_zsn700_reader_set_reg_ext (am_zsn700_reader_dev_t *p_dev,
                                       uint8_t                 nregadr,
                                       uint8_t                 nregval);


/**
 * \brief 读zsn700_reader 寄存器
 *
 * \param[in]  p_dev    : zsn700_reader设备
 * \param[in]  nregaddr : 寄存器地址
 *
 * \retval zsn700_reader设备寄存器的值
 */
uint8_t am_zsn700_reader_get_reg (am_zsn700_reader_dev_t *p_dev, uint8_t nregadr);


/**
 * \brief 读zsn700_reader 拓展寄存器
 *
 * \param[in]  p_dev    : zsn700_reader设备
 * \param[in]  nregaddr : 拓展寄存器地址
 *
 * \retval zsn700_reader设备拓展寄存器的值
 */
uint8_t am_zsn700_reader_get_reg_ext (am_zsn700_reader_dev_t *p_dev, uint8_t nregadr);


/**
 * \brief 将zsn700_reader寄存器的mask中为1的位 置位
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
am_err_t am_zsn700_reader_set_bit (am_zsn700_reader_dev_t *p_dev,
                                   uint8_t                 nregadr,
                                   uint8_t                 mask);


/**
 * \brief 将zsn700_reader寄存器的mask中为1的位清0
 *
 * \param[in] p_dev    : zsn700_reader设备
 * \param[in] nregaddr : 寄存器地址
 * \param[in] mask     : 修改的位掩码
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
am_err_t am_zsn700_reader_clr_bit (am_zsn700_reader_dev_t *p_dev,
                                   uint8_t                 nregadr,
                                   uint8_t                 mask);


/**
 * \brief 修改zsn700_reader寄存器的位
 *
 * \param[in] p_dev     : zsn700_reader设备
 * \param[in] nregaddr  : 寄存器地址
 * \param[in] modifyval : 修改的位掩码(0--将mask置1的位清0;其他--将mask置1的位置1)
 * \param[in] mask      : 修改的位掩码
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
am_err_t am_zsn700_reader_modify_reg (am_zsn700_reader_dev_t *p_dev,
                                      uint8_t                 nregaddr,
                                      uint8_t                 modifyval,
                                      uint8_t                 mask);


/**
 * \brief 写zsn700_reader 扩展寄存器
 *
 * \param[in] p_dev    : zsn700_reader设备
 * \param[in] nregaddr : 扩展寄存器子地址
 * \param[in] nregval  : 写入扩展寄存器的值
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
am_err_t am_zsn700_reader_set_extend_reg (am_zsn700_reader_dev_t *p_dev,
                                          uint8_t                 nregaddr,
                                          uint8_t                 nregval);


/**
 * \brief 读zsn700_reader 扩展寄存器
 *
 * \param[in] p_dev    : zsn700_reader设备
 * \param[in] nregaddr : 扩展寄存器子地址
 *
 * \retval zsn700_reader扩展寄存器的值
 */
uint8_t am_zsn700_reader_get_extend_reg (am_zsn700_reader_dev_t *p_dev, uint8_t nregaddr);


/**
 * \brief zsn700_reader工作状态检测，如果检测到读卡芯片寄存器数值异常变化则重新配置读卡芯片
 *
 * \param[in] p_dev    : zsn700_reader设备
 *
 * \retval AM_ZSN700_READER_STATUS_SUCCESS：检测通过
 * \retval AM_ZSN700_READER_STATUS_FAIL   ：检查不通过
 */
uint8_t am_zsn700_reader_check (am_zsn700_reader_dev_t *p_dev);


/**
 * \brief zsn700_reader关闭，关闭的模式与驱动的设置有关
 *
 * \param[in] p_dev    : zsn700_reader设备
 *
 * \retval 成功返回AM_OK，失败返回AM_ERROR
 */
am_err_t am_zsn700_reader_close (am_zsn700_reader_dev_t *p_dev);


/**
 * \brief 设置接收延时
 *
 * \param[in] p_dev : zsn700_reader设备
 * \param[in] _1ms  : 延时时间（单位为毫秒），其值应使用延时常量宏。
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
am_err_t am_zsn700_reader_set_tmo (am_zsn700_reader_dev_t *p_dev, uint16_t _1ms);


/**
 * \brief 设置卡片协议类型和速度，同时打开天线
 *
 * \param[in] p_dev : zsn700_reader设备
 * \param[in] type  : 卡片协议类型和速度
 *                   - AM_ZSN700_READER_PROT_TYPE_ISO14443A_106
 *                   - AM_ZSN700_READER_PROT_TYPE_ISO14443B_106
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_set_iso_type (am_zsn700_reader_dev_t       *p_dev,
                                       am_zsn700_reader_prot_type_t  type);


/**
 * \brief ZSN700_READER复位
 *
 * \param[in] p_dev : zsn700_reader设备
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_reset (am_zsn700_reader_dev_t *p_dev);


/**
 * \brief zsn700_reader配置
 *
 * \param[in] p_dev : zsn700_reader设备
 * \param[in] type  : 协议类型和速度
 *                   - AM_ZSN700_READER_PROT_TYPE_ISO14443A_106
 *                   - AM_ZSN700_READER_PROT_TYPE_ISO14443B_106
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_config (am_zsn700_reader_dev_t         *p_dev,
                                 am_zsn700_reader_prot_type_t    type);


/**
 * \brief 设置最后字节发送的位数
 *
 * \param[in] p_dev : zsn700_reader设备
 * \param[in] ntxlastbits : 最后字节发送的位数
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
am_err_t am_zsn700_reader_set_tx_last_bits (am_zsn700_reader_dev_t *p_dev,
                                            uint8_t                 ntxlastbits);


/**
 * \brief 设置接收数据存放的起始位置(位)
 *
 * \param[in] p_dev    : zsn700_reader设备
 * \param[in] nrxalign : 接收数据存放的起始位置
 *
 * \retval AM_OK       : 消息处理成功
 * \retval -AM_EINVAL  : 参数错误
 * \retval -AM_EIO     : 传输出错
 */
am_err_t am_zsn700_reader_set_rx_align (am_zsn700_reader_dev_t *p_dev, uint8_t nrxalign);

/**
 * \brief 设置冲突位后的数据是保持还是清0
 *
 * \param[in] p_dev : zsn700_reader设备
 * \param[in] keep_or_clr : 冲突位后的数据状态
 *                        - 0    -- 清0
 *                        - 其他    -- 保持
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
am_err_t am_zsn700_reader_set_keep_after_coll (am_zsn700_reader_dev_t *p_dev,
                                               uint8_t                 keep_or_clr);


/**
 * \brief 获取碰撞位置
 *
 * \param[in] p_dev : zsn700_reader设备
 *
 * \retval 碰撞位置
 *
 * \note 只有am_zsn700_reader_cmd()返回‘AM_ZSN700_READER_STATUS_COLLISION_ERROR’错误，才能通过该
 *       函数获取碰撞位置
 */
uint8_t am_zsn700_reader_get_coll_pos (am_zsn700_reader_dev_t *p_dev);


/**
 * \name CRC控制
 * @{
 */

#define AM_ZSN700_READER_TX_CRC_EN       0x80      /**< \brief 发送CRC使能 */
#define AM_ZSN700_READER_RX_CRC_EN       0x40      /**< \brief 接收CRC使能 */
#define AM_ZSN700_READER_TX_CRC_DIS      0x00      /**< \brief 发送CRC禁止 */
#define AM_ZSN700_READER_RX_CRC_DIS      0x00      /**< \brief 接收CRC禁止 */

 /** @} */

/**
 * \brief 使能/禁止CRC
 *
 * \param[in] p_dev     : zsn700_reader设备
 * \param[in] en_or_dis : 冲突位后的数据状态
 *                        - en_or_dis.7 发送CRC设置
 *                             - 1 -- 发送使能CRC;
 *                             - 0 -- 发送禁止CRC;
 *                        - en_or_dis.6 接收CRC设置
 *                             - 1 -- 接收使能CRC;
 *                             - 0 -- 接收禁止CRC.
 *
 * \retval AM_OK        : 消息处理成功
 * \retval -AM_EINVAL   : 参数错误
 * \retval -AM_EIO      : 传输出错
 */
am_err_t am_zsn700_reader_set_crc (am_zsn700_reader_dev_t *p_dev, uint8_t en_or_dis);


/**
 * \brief zsn700_reader禁止Crypto1加密
 *
 * \param[in] p_dev   : zsn700_reader设备
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
am_err_t am_zsn700_reader_dis_crypto1 (am_zsn700_reader_dev_t *p_dev);


/**
 * \brief zsn700_reader执行命令
 *
 * \param[in]  p_dev : zsn700_reader设备
 * \param[in]  p_in  : 指向zsn700_reader命令输入参数
 * \param[out] p_out : 指向zsn700_reader输出参数
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_cmd (am_zsn700_reader_dev_t                    *p_dev,
                              const am_zsn700_reader_cmd_input_para_t   *p_in,
                              am_zsn700_reader_cmd_output_para_t        *p_out);


/**
 * \brief 设置天线驱动模式
 *
 * \param[in] p_dev : zsn700_reader设备
 * \param[in] mode  : 天线驱动模式
 *                   - mode.0    -- TX1控制
 *                               - 0        -- TX1关闭
 *                               - 1        -- TX1打开
 *                   - mode.1    -- TX2控制
 *                               - 0        -- TX2关闭
 *                               - 1        -- TX2打开
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 *
 */
am_err_t am_zsn700_reader_set_tx_mode (am_zsn700_reader_dev_t *p_dev, uint8_t mode);


/**
 * \brief 获取天线当前驱动状态
 *
 * \param[in]  p_dev : zsn700_reader设备
 *
 * \retval 天线驱动状态，参见am_zsn700_reader_set_tx_mode()
 */
uint8_t am_zsn700_reader_get_tx_mode (am_zsn700_reader_dev_t *p_dev);


/**
 * \brief 打开/关闭载波
 *
 * \param[in]  p_dev : zsn700_reader设备
 * \param[in]  moon_or_off : AM_TRUE -- 打开载波；AM_FALSE -- 关闭载波
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 *
 * \note 打开载波只延迟了5ms,若需要更长的延时，则在外部加延迟。
 *       该函数不会改变天线驱动模式，即打开载波只会打开am_zsn700_reader_set_tx_mode()
 *       设置的天线驱动端，而不是打开所有的天线驱动端
 */
am_err_t am_zsn700_reader_turn_carrier_wave (am_zsn700_reader_dev_t *p_dev,
                                             am_bool_t               on_or_off);


/**
 * \brief 让RF场(载波)产生1个暂停，使卡复位
 *
 * \param[in] p_dev    : zsn700_reader设备
 * \param[in] pause_ms : 暂停时间，关闭RF场该时间后重新打开，若为0则不重新打开
 * \param[in] wait_ms  : RF场重新打开后持续等待该时间，若为0则不等待
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 *
 * \note 等待时间可根据卡的功耗而定，如Mifare1卡的功耗较小，等待2~5毫秒即可，
 *       而CPU卡功耗较大，需要等待10毫秒左右。
 */
am_err_t am_zsn700_reader_pause_carrier (am_zsn700_reader_dev_t *p_dev,
                                         uint8_t                 pause_ms,
                                         uint8_t                 wait_ms);


/**
 * \brief MF1卡的Crypto1认证
 *
 * \param[in] p_dev  : zsn700_reader设备
 * \param[in] mode   : 验证模式
 *                     - mode.6-0        -- 密钥类型
 *                         - KEY_TYPE_A    -- 密钥A
 *                         - KEY_TYPE_B    -- 密钥B
 *                     - mode.7          -- 密钥来源
 *                         - KEY_SOURCE_EXT    -- 使用外部输入的密钥验证
 *                         - KEY_SOURCE_E2     -- 使用内部E2的密钥验证
 * \param[in] p_key  : 6字节密钥（使用内部密钥时，第1字节为密钥存放扇区）
 * \param[in] p_uid  : 4字节UID
 * \param[in] nblock : 验证的块号
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note 执行成功后，以后的通信被加密
 */
uint8_t am_zsn700_reader_crypto1 (am_zsn700_reader_dev_t *p_dev,
                                  uint8_t                 mode,
                                  const uint8_t           p_key[6],
                                  const uint8_t           p_uid[4],
                                  uint8_t                 nblock);


/**
 * \name Mifare卡请求模式
 * @{
 */

#define AM_ZSN700_READER_PICCA_REQ_IDLE 0x26 /**< \brief IDLE模式，请求空闲的卡 */
#define AM_ZSN700_READER_PICCA_REQ_ALL  0x52 /**< \brief ALL模式，请求所有的卡  */

/** @}*/

/**
 * \brief Mifare卡请求操作
 *
 *  该函数作为卡的请求操作，只要符合ISO14443A标准的卡都应能发出响应。
 *
 * \param[in] p_dev       : zsn700_reader设备
 * \param[in] req_mode    : 请求模式，可使用下列宏：
 *                           - #AM_ZSN700_READER_PICCA_REQ_IDLE
 *                           - #AM_ZSN700_READER_PICCA_REQ_ALL
 * \param[out] p_atq      : 获取请求应答信息（ATQ）的指针，其值为16位。
 *                          若不想接收ATQ，则传入NULL。
 *                          不同卡类型返回的信息不同,常见类型返回的ATQ如下：
 *
 *            Mifare1 S50    | Mifare1 S70 | Mifare1 Light | Mifare0 UltraLight
 *       --------------------|-------------|---------------|-------------------
 *              0x0004       |    0x0002   |    0x0010     |      0x0044
 *       ----------------------------------------------------------------------
 *          Mifare3 DESFire  |   SHC1101   |    SHC1102    |      11RF32
 *       --------------------|-------------|---------------|-------------------
 *               0x0344      |    0x0004   |    0x3300     |      0x0004
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note 卡进入天线后，从射频场中获取能量，从而得电复位，复位后卡处于IDLE模式，
 * 用两种请求模式的任一种请求时，此时的卡均能响应；若对某一张卡成功进行了挂起
 * 操作（Halt命令或DeSelect命令），则进入了Halt模式，此时的卡只响应ALL（0x52）
 * 模式的请求，除非将卡离开天线感应区后再进入。
 */
uint8_t am_zsn700_reader_picca_request (am_zsn700_reader_dev_t  *p_dev,
                                        uint8_t                  req_mode,
                                        uint8_t                  p_atq[2]);


/**
 * \name Mifare卡防碰撞等级
 * @{
 */
#define AM_ZSN700_READER_PICCA_ANTICOLL_1  0x93  /**< \brief 第一级防碰撞  */
#define AM_ZSN700_READER_PICCA_ANTICOLL_2  0x95  /**< \brief 第二级防碰撞  */
#define AM_ZSN700_READER_PICCA_ANTICOLL_3  0x97  /**< \brief 第三级防碰撞  */
/** @}*/

/**
 * \brief Mifare卡的防碰撞操作
 *
 *  需要成功执行一次请求命令，并返回请求成功，才能进行防碰撞操作，否则返回错误。
 *
 * \param[in] p_dev          : zsn700_reader设备
 * \param[in] anticoll_level : 防碰撞等级，可使用下列宏：
 *                             - #AM_ZSN700_READER_PICCA_ANTICOLL_1
 *                             - #AM_ZSN700_READER_PICCA_ANTICOLL_2
 *                             - #AM_ZSN700_READER_PICCA_ANTICOLL_3
 * \param[out] p_uid   : 防碰撞后获取到的卡序列号

 * \param[out] p_real_uid_len : 卡序列号的实际大小（字节数）。
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note  符合ISO14443A标准卡的序列号都是全球唯一的，正是这种唯一性，才能实现防
 * 碰撞的算法逻辑，若有若干张卡同时在天线感应区内则这个函数能够找到一张序列号较
 * 大的卡来操作。
 */
uint8_t am_zsn700_reader_picca_anticoll (am_zsn700_reader_dev_t  *p_dev,
                                         uint8_t                  anticoll_level,
                                         uint8_t                 *p_uid,
                                         uint8_t                 *p_real_uid_len);

/**
 * \brief Mifare卡的选择操作
 *
 *  需要成功执行一次防碰撞命令，并返回成功，才能进行卡选择操作，否则返回错误。
 *
 * \param[in]  p_dev          : zsn700_reader设备
 * \param[in]  anticoll_level : 防碰撞等级，可使用下列宏：
 *                             - #AM_ZSN700_READER_PICCA_ANTICOLL_1
 *                             - #AM_ZSN700_READER_PICCA_ANTICOLL_2
 *                             - #AM_ZSN700_READER_PICCA_ANTICOLL_3
 * \param[in]  uid     : 前一个防碰撞函数获取的UID
 * \param[in]  uid_len : 前一个防碰撞函数获取的UID的长度（字节数）
 * \param[out] p_sak   : 返回的信息，若bit2为1，则表明UID不完整。若不想接收SAK则传入NULL。
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note  卡的序列号长度有三种：4字节、7字节和10字节。 4字节的只要用一级选择即可
 * 得到完整的序列号，如Mifare1 S50/S70等；7字节的要用二级选择才能得到完整的序列
 * 号，前一级所得到的序列号的最低字节为级联标志0x88，在序列号内只有后3字节可用，
 * 后一级选择能得到4字节序列号，两者按顺序连接即为7字节序列号，如UltraLight
 * 和DesFire等；10字节的以此类推，但至今还未发现此类卡。
 */
uint8_t am_zsn700_reader_picca_select (am_zsn700_reader_dev_t  *p_dev,
                                       uint8_t                  anticoll_level,
                                       const uint8_t           *p_uid,
                                       uint8_t                  uid_len,
                                       uint8_t                 *p_sak);

/**
 * \brief Mifare卡的挂起操作，使所选择的卡进入HALT状态
 *
 *  在HALT状态下，卡将不响应读卡器发出的IDLE模式的请求，除非将卡复位或离开天线感
 *  应区后再进入。但它会响应读卡器发出的ALL请求。
 *
 * \param[in]  p_dev          : zsn700_reader设备
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_picca_halt (am_zsn700_reader_dev_t  *p_dev);

/**
 * \brief Mifare卡激活，该函数用于激活卡片，是请求、防碰撞和选择三条命令的组合。
 *
 * \param[in]  p_dev      : zsn700_reader设备
 * \param[in]  req_mode   : 请求模式，可使用下列宏：
 *                           - #AM_ZSN700_READER_PICCA_REQ_IDLE
 *                           - #AM_ZSN700_READER_PICCA_REQ_ALL
 * \param[out] p_atq      : 卡片类型，若不关心则传入NULL。
 * \param[out] p_uid      : 存放序列号的缓冲区，长度应该与序列号长度保持一致，
 *                          如不确定，建议先使用最大长度10。不能传入NULL。

 * \param[out] p_uid_real_len   :序列号的实际长度，4字节、7字节或10字节，若不关心则传入NULL。
 * \param[out] p_sak      : 最后一次选择应答SAK，若不关心则传入NULL。
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_picca_active (am_zsn700_reader_dev_t   *p_dev,
                                       uint8_t                   req_mode,
                                       uint8_t                   p_atq[2],
                                       uint8_t                   p_uid[10],
                                       uint8_t                  *p_uid_real_len,
                                       uint8_t                   p_sak[1]);

/**
 * \name IC卡密匙类型
 * @{
 */

#define  AM_ZSN700_READER_IC_KEY_TYPE_A      0x60    /**< \brief 类型 A  */
#define  AM_ZSN700_READER_IC_KEY_TYPE_B      0x61    /**< \brief 类型 B  */

/** @}*/

/**
 * \brief Mifare卡密钥验证，将传入的密钥与卡的密钥进行验证
 *
 * \param[in] p_dev      : zsn700_reader设备
 * \param[in] key_type   : 密钥类型，可以使用下列值：
 *                          - #AM_ZSN700_READER_IC_KEY_TYPE_A
 *                          - #AM_ZSN700_READER_IC_KEY_TYPE_B
 * \param[in] p_uid      : 卡序列号，4字节
 * \param[in] p_key      : 密钥，6字节。默认的密钥都是全1，即6个0xff
 * \param[in] nblock     : 需要验证的卡块号，取值范围与卡类型有关，
 *                         - S50：0 ~ 63
 *                         - S70：0 ~ 255
 *                         - PLUS CPU 2K：0 ~ 127
 *                         - PLUS CPU 4K：0 ~ 255
 *
 * \retval 验证成功返回AM_ZSN700_READER_STATUS_SUCCESS，否则返回去其他状态码。
 *         (详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note PLUS CPU系列的卡的卡号有4字节和7字节之分，对于7字节卡号的卡，
 *       只需要将卡号的高4字节（等级2防碰撞得到的卡号）作为验证的卡号即可。
 */
uint8_t am_zsn700_reader_picca_authent (am_zsn700_reader_dev_t  *p_dev,
                                        uint8_t                  key_type,
                                        const uint8_t            p_uid[4],
                                        const uint8_t            p_key[6],
                                        uint8_t                  nblock);

/**
 * \brief Mifare卡数据读取
 *
 *     在验证成功之后，才能读相应的块数据，所验证的块号与读块号必须在同一个扇区内，
 * Mifare1卡从块号0开始按顺序每4个块1个扇区，若要对一张卡中的多个扇区进行操作，在
 * 对某一扇区操作完毕后，必须进行一条读命令才能对另一个扇区直接进行验证命令，否则
 * 必须从请求开始操作。对于PLUS CPU卡，若下一个读扇区的密钥和当前扇区的密钥相同，
 * 则不需要再次验证密钥，直接读即可。
 *
 * \param[in]  p_dev      : zsn700_reader设备
 * \param[in]  nblock     : 读取数据的块号
 *                          - S50：0 ~ 63
 *                          - S70：0 ~ 255
 *                          - PLUS CPU 2K：0 ~ 127
 *                          - PLUS CPU 4K：0 ~ 255
 * \param[out] p_buf      : 存放读取的数据，共16bytes
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_picca_read (am_zsn700_reader_dev_t  *p_dev,
                                     uint8_t                  nblock,
                                     uint8_t                  p_buf[16]);


/**
 * \brief Mifare卡写数据，写之前必需成功进行密钥验证。
 *
 *      对卡内某一块进行验证成功后，即可对同一扇区的各个块进行写操作（只要访问条件允许），
 *  其中包括位于扇区尾的密码块，这是更改密码的唯一方法。对于PLUS CPU卡等级2、3的AES密
 *  钥则是在其他位置修改密钥。
 *
 * \param[in] p_dev      : zsn700_reader设备
 * \param[in] nblock     : 读取数据的块号
 *                         - S50：0 ~ 63
 *                         - S70：0 ~ 255
 *                         - PLUS CPU 2K：0 ~ 127
 *                         - PLUS CPU 4K：0 ~ 255
 * \param[in] p_buf      : 写入数据缓冲区，大小必须为 16
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_picca_write (am_zsn700_reader_dev_t  *p_dev,
                                      uint8_t                  nblock,
                                      const uint8_t            p_buf[16]);

/**
 * \name Mifare卡值操作模式
 * @{
 */

#define AM_ZSN700_READER_PICCA_PICC_ADD       0xC1   /**< \brief 加值 */
#define AM_ZSN700_READER_PICCA_PICC_SUB       0xC0   /**< \brief 减值 */

/** @}*/

/**
 * \brief Mifare值操作，对Mifare卡的值块进行加减操作
 *
 * \param[in] p_dev      : zsn700_reader设备
 * \param[in] mode       : 值操作的模式，可以是加或减，使用下列宏：
 *                          - AM_ZSN700_READER_PICCA_PICC_ADD
 *                          - AM_ZSN700_READER_PICCA_PICC_SUB
 * \param[in] nblock     : 进行值操作的块号
 *                          - S50：0 ~ 63
 *                          - S70：0 ~ 255
 *                          - PLUS CPU 2K：0 ~ 127
 *                          - PLUS CPU 4K：0 ~ 255
 * \param[in] ntransblk  : 传输块号，计算结果值存放的块号
 * \param[in] value      : 4字节有符号数
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note 要进行此类操作，块数据必须要有值块的格式，可以使用
 * am_zsn700_reader_picca_val_set()函数将数据块初始化为值块的格式。可参考NXP的相关文
 * 档。若卡块 * 号与传输块号相同，则将操作后的结果写入原来的块内；若卡块号与传输块号不相同，则将操作后的
 * 结果写入传输块内，结果传输块内的数据被覆盖，原块内的值不变。处于等级2的PLUS CPU卡不支持值块操作，等
 * 级1、3支持。
 *       进行加值操作必须先验证KEYB，进行减值操作必须先验证KEYA。
 *       可以通过传入不同的块，并将value的值设为0来备份数据。
 */
uint8_t am_zsn700_reader_picca_val_operate (am_zsn700_reader_dev_t  *p_dev,
                                            uint8_t                  mode,
                                            uint8_t                  nblock,
                                            uint8_t                  ntransblk,
                                            int32_t                  value);

/**
 * \brief Mifare值操作，设置值块的值
 *
 * \param[in] p_dev      : zsn700_reader设备
 * \param[in] nblock     : 读取数据的值块地址
 * \param[in] value      : 设置的值
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note 该函数使用直接写数据的方式，将指定的块格式转换为数值块，并初始化数值块的值，也可以使用该函数改
 *       变数值块的值。对卡内某一块进行验证成功后，并且访问条件允许才能进行此操作。
 */
uint8_t am_zsn700_reader_picca_val_set (am_zsn700_reader_dev_t  *p_dev,
                                         uint8_t                 nblock,
                                         int32_t                 value);


/**
 * \brief Mifare值操作，获取值块的值
 *
 * \param[in]  p_dev      : zsn700_reader设备
 * \param[in]  nblock     : 读取数据的值块地址
 * \param[out] p_value    : 获取值的指针
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note 该函数使用直接写数据的方式，将指定的块格式转换为数值块，并初始化数值块的值，也可以使用该函数改
 *       变数值块的值。对卡内某一块进行验证成功后，并且访问条件允许才能进行此操作。
 */
uint8_t am_zsn700_reader_picca_val_get (am_zsn700_reader_dev_t  *p_dev,
                                        uint8_t                  nblock,
                                        int32_t                 *p_value);

/**
 * \brief A卡协议分析
 *
 * \param[in] p_dev : zsn700_reader设备
 * \param[in] cid   : 卡识别号，取值范围0--14
 * \param[in] p_ats : RATS
 *
 * \return 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_picca_prot_parse (am_zsn700_reader_dev_t  *p_dev,
                                           uint8_t                  cid,
                                           const uint8_t           *p_ats);

/**
 * \brief RATS
 *
 * \param[in]  p_dev    : zsn700_reader设备
 * \param[in]  cid      : 卡识别号，取值范围0--14
 * \param[out] p_rats   : 请求应答以响应信息
 * \param[out] buf_size : 响应信息的缓冲区大小
 * \param[out] p_nbytes : 请求应答以响应信息的字节数
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_picca_rats_get (am_zsn700_reader_dev_t *p_dev,
                                         uint8_t                 cid,
                                         void                   *p_rats,
                                         uint32_t                buf_size,
                                         uint32_t               *p_nbytes);

/**
 * \brief A卡协议和参数选择请求
 *
 * \param[in]  p_dev    : zsn700_reader设备
 * \param[in]  flags    : 位率编码
 *                      - b3:2 -- PCD  -> PICC的位率编码
 *                      - b1:0 -- PICC -> PCD 的位率编码
 *                          - (00)b -> 106Kb
 *                          - (01)b -> 212Kb
 *                          - (10)b -> 424Kb
 *                          - (11)b -> 847Kb
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_picca_pps_set (am_zsn700_reader_dev_t *p_dev,
                                        uint8_t                 flags);

/**
 * \brief A卡取消选择
 *
 * \param[in]  p_dev    : zsn700_reader设备
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_picca_deselect (am_zsn700_reader_dev_t *p_dev);

/**
 * \brief 传输（T=CL）
 *
 * \param[in]  p_dev  : zsn700_reader设备
 * \param[in] p_txbuf : 传输的数据缓冲区
 * \param[in] n_tx    : 需要传输的数据字节数
 * \param[in] p_rxbuf : 接收数据缓冲区
 * \param[in] n_rx    : 需要接收的字节数
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_picca_transfer (am_zsn700_reader_dev_t *p_dev,
                                         const void             *p_txbuf,
                                         uint32_t                n_tx,
                                         void                   *p_rxbuf,
                                         uint32_t                buf_size,
                                         uint32_t               *p_n_rx);

/**
 * \brief zsn700_reader ultralight 卡 传输
 *
 * \param[in] p_dev   : zsn700_reader设备
 * \param[in] p_txbuf : 发送缓冲区
 * \param[out] p_rxbuf: 接收缓冲区
 * \param[out] p_rx_nbytes: 接收字节数
 */
int am_zsn700_reader_mifare_ultralight_transfer (am_zsn700_reader_handle_t    handle,
                                                 const void                  *p_txbuf,
                                                 void                        *p_rxbuf,
                                                 uint32_t                    *p_rx_nbytes);

/**
 * \brief 读写器与IC卡数据交互
 *
 * \param[in]  p_dev       : zsn700_reader设备
 * \param[in]  sendbuf     : 待发送的数据
 * \param[in]  sendlen     : 发送数据的长度
 * \param[out] recvbuf     : 待接收的数据
 * \param[in]  recvbufsize : 需要接收的数据个数
 * \param[out] recvlen     : 共接收到的字节数
 * \param[in]  nwtxm_crc   : crc校验参数
 * \param[in]  nfwi        : 延时
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_exchange_block (am_zsn700_reader_dev_t *p_dev,
                                         uint8_t                *sendbuf,
                                         uint32_t                sendlen,
                                         uint8_t                *recvbuf,
                                         uint32_t                recvbufsize,
                                         uint16_t               *recvlen,
                                         uint8_t                 nwtxm_crc,
                                         uint8_t                 nfwi);

/**
 * \name PICCB卡请求模式
 * @{
 */

#define AM_ZSN700_READER_PICCB_REQ_IDLE 0x00 /**< \brief IDLE模式,请求空闲的卡 */
#define AM_ZSN700_READER_PICCB_REQ_ALL  0x08 /**< \brief ALL模式,请求所有的卡  */

/** @}*/

/**
 * \brief PICCB卡请求操作
 *
 *  该函数作为卡的请求操作，只要符合ISO14443B标准的卡都应能发出响应。
 *
 * \param[in]  p_dev       : zsn700_reader设备
 * \param[in]  req_mode    : 请求模式，可使用下列宏：
 *                           - #AM_ZSN700_READER_PICCB_REQ_IDLE
 *                           - #AM_ZSN700_READER_PICCB_REQ_ALL
 * \param[in]  afi         : 应用标识，一般情况下为0x00即可
 * \param[in]  slot_time   : 时隙总数，0 ~ 4
 * \param[out] atqb        : 返回对REQB命令的响应，长度为12
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_piccb_request (am_zsn700_reader_dev_t *p_dev,
                                        uint8_t                 req_mode,
                                        uint8_t                 afi,
                                        uint8_t                 slot_time,
                                        uint8_t                 atqb[12]);

/**
 * \brief B类卡的ATTRIB命令
 *
 * \param[in] p_dev      : zsn700_reader设备
 * \param[in] p_pupi     : PUPI,大小为4字节的标识符
 * \param[in] cid        : 取值范围为 0 - 14, 若不支持CID，则设置为0，CID由PCD规定并对所有在
 *                         同一时刻处于ACTIVE状态的PICC是唯一的
 * \param[in] protype    : 支持的协议
 *                          - bit3 : 1-PCD中止与PICC继续通信,0-PCD与PICC继续通信
 *                          - bit2 ~ bit1 :
 *                              - 11 : 10 etu + 512 / fs
 *                              - 10 : 10 etu + 256 / fs
 *                              - 01 : 10 etu + 128 / fs
 *                              - 00 : 10 etu + 32 / fs
 *                          - bit0 : 1-遵循ISO14443-4,0-不遵循ISO14443-4
 *                                   (二代身份证必须该位必须为1)
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note 执行该命令时必须先成功执行一次request命令
 */
uint8_t am_zsn700_reader_piccb_attrib (am_zsn700_reader_dev_t *p_dev,
                                       const uint8_t           pupi[4],
                                       uint8_t                 cid,
                                       uint8_t                 protype);

/**
 * \brief 获取B类卡的UID
 *
 * \param[in]  p_dev   : zsn700_reader设备
 * \param[out] uid     : 获取到的8字节UID
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note 执行该命令时必须先成功执行一次attrib命令
 */
uint8_t am_zsn700_reader_piccb_guid (am_zsn700_reader_dev_t *p_dev, uint8_t uid[8]);

/**
 * \brief PICC B卡激活，获取UID
 *
 * \param[in]  p_dev      : zsn700_reader设备
 * \param[in]  req_mode   : 请求模式，可使用下列宏：
 *                         - #AM_ZSN700_READER_PICCB_REQ_IDLE
 *                         - #AM_ZSN700_READER_PICCB_REQ_ALL
 * \param[in]  afi        : 应用标识，一般情况下为0x00即可
 * \param[out] p_uid      : UID相关信息，前4字节为 PUPI 中间4字节为应用参数  后四字节为参数信息
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 *
 * \note 在调用该函数前，需要确保IC工作模式已经配置为TypeB模式。
 */
uint8_t am_zsn700_reader_piccb_active (am_zsn700_reader_dev_t *p_dev,
                                       uint8_t                 req_mode,
                                       uint8_t                 afi,
                                       uint8_t                 uid[12]);

/**
 * \brief PICC B卡的挂起操作，使所选择的卡进入HALT状态
 *
 *     该函数用于B型卡挂起，在执行挂起命令前，必需先执行成功过一次请求命令。执行
 * 挂起命令成功后，卡片处于挂起状态，模块必需通过ALL方式请求卡片，而不能用IDLE方
 * 式请求。
 *
 * \param[in] p_dev   : zsn700_reader设备
 * \param[in] pupi    : PUPI,大小为4字节的标识符
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
uint8_t am_zsn700_reader_piccb_halt (am_zsn700_reader_dev_t *p_dev,
                                     const uint8_t           pupi[4]);

/**
 * \brief PCD进入LPCD模式的初始化
 *
 *    设备在此函数中完成了  寄存器的初始化   调校参数初始化   中断设置初始化
 *调校过程（包括GPA倍数设置    参考电压设置   阈值设置等）。
 *
 * \param[in] p_dev   : zsn700_reader设备
 *
 * \retval 操作结果(详细说明见am_zsn700_reader.h中的状态码定义)
 */
int8_t am_zsn700_reader_lpcd_init(am_zsn700_reader_dev_t *p_dev);

/**
 * \brief PCD进入LPCD模式
 *
 *    进入之前需调用am_zsn700_reader_lpcd_init函数进行LPCD模式寄存器进行初始化
 *
 * \param[in] handle   : zsn700_reader设备句柄
 *
 */
void  am_zsn700_reader_lpcd_mode_entry(am_zsn700_reader_handle_t handle);

/**
 * \brief PCD退出LPCD模式
 *
 *    退出LPCD模式之后需要延时1ms，确保设备晶振起振，使用此函数退出后，
 *需调用am_zsn700_reader_exit_lpcd_config 函数对未保存的数据进行重新配置，确保设备正常工作
 *
 * \param[in] handle   : zsn700_reader设备句柄
 *
 */
void  am_zsn700_reader_lpcd_mode_exit(am_zsn700_reader_handle_t handle);
/**
 * \brief PCD退出LPCD模式后对未保存的数据进行重新配置
 *
 * \param[in] p_dev   : zsn700_reader设备
 *
 */
void  am_zsn700_reader_exit_lpcd_config(am_zsn700_reader_dev_t   *p_dev);

/**
 * \brief PCD模式产生卡进场中断回调函数设置
 *
 * \param[in] p_dev   : zsn700_reader设备
 * \param[in] pfn_callback   : 回调函数
 * \param[in] p_lpcd_cb_arg   : 回调函数参数
 *
 */
void  am_zsn700_reader_lpcd_cb_set(am_zsn700_reader_dev_t   *p_dev,
                                   am_pfnvoid_t              pfn_callback,
                                   void                     *p_lpcd_cb_arg);

/**
 * \brief zsn700_reader  fm11rf005m 卡请求操作
 *
 * \param[in] p_dev   : zsn700_reader设备
 * \param[in] reqa_mode   : 卡请求模式（0x26与0x52没有区别）
 *
 */
int8_t am_zsn700_reader_fm11rf005m_reqa(am_zsn700_reader_handle_t handle,
                                        uint8_t                   reqa_mode);

/**
 * \brief zsn700_reader  fm11rf005m 卡读块操作
 *
 * \param[in] p_dev   : zsn700_reader设备
 * \param[in] block_num   : 块号
 * \param[in] p_buff  : 读取数据缓冲区
 *
 */
int8_t am_zsn700_reader_fm11rf005m_read_block(am_zsn700_reader_handle_t handle,
                                              uint8_t                   block_num,
                                              uint8_t                  *p_buff);
/**
 * \brief zsn700_reader  fm11rf005m 卡写块操作
 *
 * \param[in] p_dev   : zsn700_reader设备
 * \param[in] block_num   : 块号
 * \param[in] p_buff  : 读取数据缓冲区
 *
 */
int8_t am_zsn700_reader_fm11rf005m_write_block(am_zsn700_reader_handle_t handle,
                                               uint8_t                   block_num,
                                               uint8_t                  *p_buff);

/**
 * \brief zsn700_reader  身份证物理ID读取
 *
 * \param[in] p_dev   : zsn700_reader设备
 * \param[in] id      : ID读取缓冲区
 */
uint8_t am_zsn700_reader_idcard_readr_id (am_zsn700_reader_dev_t *p_dev,
                                          uint8_t                 id[10]);

/** @} */

/**
 * @} am_if_zsn700_reader
 */






#ifdef __cplusplus
}
#endif

#endif /* __ZSN700_READER_H */

/* end of file */
