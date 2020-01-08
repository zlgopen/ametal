/**
 * \file
 * \brief ZSN603组件相关头文件定义
 *
 */
#ifndef __ZSN603_H_
#define __ZSN603_H_
#include "zsn603_platform.h"

/**
 * \name 设备工作状态定义,用于表示当前设备工作状态
 * 用于ZSN603设备的工作模式设置
 * @{
 */
/** \brief ZSN603正常工作模式  命令-响应*/
#define  ZSN603_NORMAL_MODE                     0xA0

/** \brief  自动检测模式无中断有数据
 * 该模式为当前设备处于自动检测模式下,且当前通信模式为UART模式,并且当前自动检测模式配置为无中断且会主动回传数据
 */
#define  ZSN603_AUTO_MODE_NO_INT                0xA1

/** \brief  自动检测模式有中断有数据
 * 该模式为当前设备处于自动检测模式下,且当前通信模式为UART模式,并且当前自动检测模式配置为有中断且会主动回传数据
 */
#define  ZSN603_AUTO_MODE_INT_MESSAGE           0xA2

/** \brief  自动检测模式仅有中断
 * 该模式为当前设备处于自动检测模式下,且当前通信模式为UART模式,并且当前自动检测模式配置为有中断但不会主动回传数据
 * 或者当前通信模式处于I2C模式下
 */
#define  ZSN603_AUTO_MODE_INT                   0xA3

/**
 * \name 设备帧长度宏定义
 *
 * @{
 */
#define  ZSN603_FRAME_HEAD_LENGHT               0x08 /* 帧头长度,帧信息之前的字节长度总共为8字节*/
#define  ZSN603_FRAME_CHECK_LENGHT              0x02 /* 帧校验和长度,2字节 */
#define  ZSN603_FRAME_FRAME_MIN_LENGHT          0x0A /* 最小帧长度10字节 */
#define  ZSN603_BUFFER_SIZE_MAX                 282  /**< \brief 串口缓冲区最大情况为272 + 10 */
/**
 * \name 设备应用类定义
 *   设备命令帧类型标识
 * @{
 */
#define  ZSN603_DEVICE_CONTROL_CMD              0x01 /**< \brief 设备控制类命令*/
#define  ZSN603_MIFARE_CARD_CMD                 0x02 /**< \brief Mifare卡命令 */
#define  ZSN603_ISO7816_PROTOCOL_CMD            0x05 /**< \brief ISO7816协议命令 */
#define  ZSN603_ISO14443_PROTOCOL_CMD           0x06 /**< \brief ISO14443协议命令 */
#define  ZSN603_CPU_CARD_CMD                    0x07 /**< \brief CPU卡命令 */

/**
 * \name 设备控制命令定义
 *   用于ZSN603设备控制类的命令函数
 * @{
 */
#define  ZSN603_GET_DEVICE_INFO                 0x41 /**< \brief 获取设备信息*/
#define  ZSN603_CONFIG_ICC_INTERFACE            0x42 /**< \brief 配置IC卡接口 */
#define  ZSN603_CLOSE_ICC_INTERFACE             0x43 /**< \brief 关闭IC卡接口 */
#define  ZSN603_SET_ICC_INTERFACE_PROTOCOL      0x44 /**< \brief IC接口协议 */
#define  ZSN603_LOAD_ICC_KEY                    0x45 /**< \brief 装载IC密钥 */
#define  ZSN603_SET_ICC_INTERFACE_REG           0x46 /**< \brief 设置IC接口寄存器 */
#define  ZSN603_GET_ICC_INTERFACE_REG           0x47 /**< \brief 获取IC接口寄存器 */
#define  ZSN603_SET_BAUD_RATE                   0x48 /**< \brief 设置波特率 */
#define  ZSN603_SET_ICC_INTERFACE_OUTPUT        0x49 /**< \brief 设置天线驱动模式 */
#define  ZSN603_SET_ANTENNA_CHANNAL             0x4B /**< \brief 设置天线通道 */
#define  ZSN603_SET_LOCAL_ADDRESS               0x4C /**< \brief 设置从机地址 */
#define  ZSN603_LED_CONTROL                     0x4D /**< \brief LED控制 */
#define  ZSN603_BUZZER_CONTROL                  0x4E /**< \brief 蜂鸣器控制 */
#define  ZSN603_READ_EEPROM                     0x62 /**< \brief 读EEPROM */
#define  ZSN603_WRITE_EEPROM                    0x63 /**< \brief 写EEPROM */
#define  ZSN603_AUTO_TEST                       0x48 /**< \brief 执行自动测试 */
#define  ZSN603_READ_AUTO_TEST                  0x49 /**< \brief 读自动测试结果 */
/**
 * \name Mifare类卡命令定义
 *   用于ZSN603  Mifare类的命令函数
 * @{
 */
#define  ZSN603_MIFARE_REQUEST                  0x41 /**< \brief 卡请求*/
#define  ZSN603_MIFARE_ANTICOLL                 0x42 /**< \brief 卡防碰撞 */
#define  ZSN603_MIFARE_SELECT                   0x43 /**< \brief 卡选择 */
#define  ZSN603_MIFARE_HALT                     0x44 /**< \brief 卡挂起 */
#define  ZSN603_MIFARE_AUTH_E2                  0x45 /**< \brief 装载E2密钥 */
#define  ZSN603_MIFARE_AUTH_KEY                 0x46 /**< \brief 直接密钥验证 */
#define  ZSN603_MIFARE_READ                     0x47 /**< \brief Mifare卡读 */
#define  ZSN603_MIFARE_WRITE                    0x48 /**< \brief Mifare卡写 */
#define  ZSN603_MIFARE0_WRITE                   0x49 /**< \brief UltraLight卡写 */
#define  ZSN603_MIFARE_VALUE                    0x4A /**< \brief Mifare卡值操作 */
#define  ZSN603_MIFARE_RESET                    0x4C /**< \brief 卡复位 */
#define  ZSN603_MIFARE_ACTIVE                   0x4D /**< \brief 卡激活 */
#define  ZSN603_MIFARE_AUTO_DETECT              0x4E /**< \brief 自动检测 */
#define  ZSN603_MIFARE_GET_AUTO_DETECT          0x4F /**< \brief 读自动检测数据 */
#define  ZSN603_MIFARE_SET_VALUE                0x50 /**< \brief 设置值块的值 */
#define  ZSN603_MIFARE_GET_VALUE                0x51 /**< \brief 获取值块的值 */
#define  ZSN603_MIFARE_EXCHANGE_BLOCK1          0x58 /**< \brief 数据交互命令 */
#define  ZSN603_MIFARE_MF0_CMD_TRANS            0x53 /**< \brief MF0卡片与PCD之间数据交互扩展命令 */

/**
 * \name ISO7816-3协议类命令定义
 *   用于 ZSN603  ISO7816-3协议类的命令函数
 * @{
 */
#define  ZSN603_CICC_TPDU                       0x42 /**< \brief 接触式IC卡传输协议（自动处理T=0和T=1协议 */
#define  ZSN603_CICC_COLD_RESET                 0x43 /**< \brief 接触式IC卡冷复位 */
#define  ZSN603_CICC_WARM_RESET                 0x44 /**< \brief 接触式IC卡热复位 */
#define  ZSN603_CICC_DEACTIVATION               0x45 /**< \brief 接触式IC卡停活（关闭电源和时钟） */
#define  ZSN603_CICC_TP0                        0x47 /**< \brief 接触式IC卡T=0传输协议 */
#define  ZSN603_CICC_TP1                        0x48 /**< \brief 接触式IC卡T=1传输协议 */

/**
 * \name ISO1443(PICC)协议类命令定义
 *   用于 ZSN603  ISO1443(PICC)协议类的命令函数
 * @{
 */
#define  ZSN603_PICCA_REQUEST                   0x41 /**< \brief A型卡请求 */
#define  ZSN603_PICCA_ANTICOLL                  0x42 /**< \brief A型卡防碰撞 */
#define  ZSN603_PICCA_SELECT                    0x43 /**< \brief A型卡选择 */
#define  ZSN603_PICCA_HALT                      0x44 /**< \brief A型卡挂起 */
#define  ZSN603_PICCA_RATS                      0x45 /**< \brief A型卡RATS */
#define  ZSN603_PICCA_PPS                       0x46 /**< \brief A型卡PPS */
#define  ZSN603_PICCA_DESELECT                  0x47 /**< \brief DESELECT */
#define  ZSN603_PICCA_TPCL                      0x48 /**< \brief T=CL */
#define  ZSN603_PICCA_EXCHANGE_BLOCK2           0x4A /**< \brief 数据交换*/
#define  ZSN603_PICCA_RESET                     0x4C /**< \brief A型卡复位（若支持ISO14443-4, 则自动执行RATS）*/
#define  ZSN603_PICCA_ACTIVE                    0x4D /**< \brief A型卡激活*/
#define  ZSN603_PICCB_ACTIVE                    0x4E /**< \brief B型卡激活*/
#define  ZSN603_PICCB_RESET                     0x4F /**< \brief B型卡复位（执行ATTRIB）*/
#define  ZSN603_PICCB_REQUEST                   0x50 /**< \brief B型卡请求*/
#define  ZSN603_PICCB_ATTRIB                    0x52 /**< \brief B型卡修改传输属性（选择卡）*/
#define  ZSN603_PICCB_HALT                      0x53 /**< \brief B型卡挂起*/
#define  ZSN603_PICCB_GETUID                    0x54 /**< \brief 获取二代证UID*/


/**
 * \name PLUS CPU卡类命令定义
 *   用于 ZSN603  PLUS CPU协议类的命令函数
 * @{
 */
#define  ZSN603_SL0_WRITE_PERSO                  0x42 /**< \brief SL0个人化数据更新 */
#define  ZSN603_SL0_COMMIT_PERSO                 0x43 /**< \brief SL0提交个人化 */
#define  ZSN603_SL3_FIRST_AUTH                   0x4A /**< \brief SL3首次验证(直接密钥验证) */
#define  ZSN603_SL3_FIRST_AUTH_E2                0x4B /**< \brief SL3首次验证(E2密钥验证) */
#define  ZSN603_SL3_FOLLOWING_AUTH               0x4C /**< \brief SL3跟随验证(直接密钥验证) */
#define  ZSN603_SL3_FOLLOWING_AUTH_E2            0x4D /**< \brief SL3跟随验证(E2密钥验证)*/
#define  ZSN603_SL3_RESET_AUTH                   0x4E /**< \brief SL3复位验证 */
#define  ZSN603_SL3_READ                         0x4F /**< \brief SL3读数据块 */
#define  ZSN603_SL3_WRITE                        0x50 /**< \brief SL3写数据块*/
#define  ZSN603_SL3_VALUE                        0x53 /**< \brief SL3值块操作*/

/**
 * \name 设备   标识宏定义
 * 用于ZSN603设备的工作时的一些相关标识
 * @{
 */
#define   ZSN603_EXECUTE_SUCCESS                 0x00 /**< \brief 设备操作成功  */
#define   ZSN603_DEV_SERVER_ERR                  0xA1 /**< \brief 设备服务错误 设备为空  */
#define   ZSN603_FRAME_LENGHT_RX_ERR             0xA2 /**< \brief 获取回应帧总长度错误  */
#define   ZSN603_FRAME_INFO_RX_ERR               0xA3 /**< \brief 获取回应帧信息错误  */
#define   ZSN603_FRAME_LENGHT_ERR                0xA4 /**< \brief 解析获取信息时,帧长度错误  */
#define   ZSN603_FRAME_NO_SLV_ADDRS_ERR          0xA5 /**< \brief 解析获取信息时,无local address错误  */
#define   ZSN603_FRAME_CHECK_SUM_ERR             0xA6 /**< \brief 解析获取信息时,校验和错误  */
#define   ZSN603_IIC_TRANS_ERR                   0xA7 /**< \brief 发送时 IIC 通信返回错误  */
#define   ZSN603_WORKMODE_INVALID                0xA8 /**< \brief 发送数据时  设备处于无效工作模式  */
#define   ZSN603_UART_RX_TIMEOUT                 0xA9 /**< \brief 串口接收数据时超时  */
#define   ZSN603_I2C_SEMB_TIMEOUT                0xAA /**< \brief I2C中断等待超时  */
#define   ZSN603_INPUT_PARA_ERROR                0xAB /**< \brief 帧命令输入参数错误  */
/**
 * \name 设备工作工作模式
 * 用于ZSN603设备的工作模式设置。
 * @{
 */
#define   ZSN603_WORK_MODE_IIC                   0x00 /**< \brief IIC工作模式  */
#define   ZSN603_WORK_MODE_UART                  0x01 /**< \brief UART工作模式  */

/**
 * \name 设备控制类指令  IC卡协议设置参数宏定义
 * 用于ZSN603设备的设备控制类指令中的IC卡协议设置命令
 * @{
 */
#define   ZSN603_ICC_ISO_TYPE_A                  0x00 /**< \brief iso14443-3A协议  */
#define   ZSN603_ICC_ISO_TYPE_B                  0x04 /**< \brief iso14443-3B协议  */

/**
 * \name 设备控制类指令  IC卡密钥类型参数宏定义
 *   用于ZSN603设备的密钥操作相关参数
 * @{
 */
#define   ZSN603_ICC_KEY_TYPE_A                  0x60 /**< \brief 密钥 A  */
#define   ZSN603_ICC_KEY_TYPE_B                  0x61 /**< \brief 密钥 B  */

/**
 * \name 设备控制类指令  波特率设置参数宏定义
 *   用于ZSN603设备的波特率设置相关操作参数
 * @{
 */
#define  ZSN603_BAUD_230400                      0x07 /**< \brief 230400 */
#define  ZSN603_BAUD_172800                      0x06 /**< \brief 172800 */
#define  ZSN603_BAUD_115200                      0x05 /**< \brief 115200 */
#define  ZSN603_BAUD_57600                       0x04 /**< \brief 57600 */
#define  ZSN603_BAUD_38400                       0x03 /**< \brief 38400 */
#define  ZSN603_BAUD_28800                       0x02 /**< \brief 28800 */
#define  ZSN603_BAUD_19200                       0x01 /**< \brief 19200 */
#define  ZSN603_BAUD_9600                        0x00 /**< \brief 9600 */

/**
 * \name 设备控制类指令  天线模式设置参数宏定义
 *   用于ZSN603设备的天线模式设置相关操作参数
 * @{
 */
#define  ZSN603_ANT_MODE_TX12_LATERNATE          0x01 /**< \brief 天线TX1、TX2交替驱动 */
#define  ZSN603_ANT_MODE_TX1                     0x01 /**< \brief 天线仅TX1驱动 */
#define  ZSN603_ANT_MODE_TX2                     0x02 /**< \brief 天线仅TX2驱动 */
#define  ZSN603_ANT_MODE_TX12                    0x03 /**< \brief 天线TX1、TX2同时驱动 */

/**
 * \name 设备控制类指令  控制LED设置参数宏定义
 *   用于ZSN603设备的控制LED设置相关操作参数
 * @{
 */
#define  ZSN603_CONTROL_LED_ON                   0x01 /**< \brief LED关闭 */
#define  ZSN603_CONTROL_LED_OFF                  0x00 /**< \brief LED开启 */

/**
 * \name A卡 请求 指令参数宏定义
 *   用于ZSN603设备的A卡请求命令参数
 * @{
 */
#define  ZSN603_MIFARE_REQUEST_IDLE              0x26 /**< \brief LED关闭 */
#define  ZSN603_MIFARE_REQUEST_ALL               0x52 /**< \brief LED开启 */

/**
 * \name A卡 自动检卡模式  参数宏定义
 *   用于ZSN603设备的A卡自动检卡模式命令参数
 * @{
 */
#define  ZSN603_AUTO_DETECT_EXECUTE_HALT         0x08 /**< \brief 最后执行Halt() */
#define  ZSN603_AUTO_DETECT_CONTINUE             0x04 /**< \brief 输出后继续执行 */
#define  ZSN603_AUTO_DETECT_INTERRUPT            0x02 /**< \brief 产生中断 */
#define  ZSN603_AUTO_DETECT_SEND                 0x01 /**< \brief 主动发送 */

#define  ZSN603_AUTO_DETECT_NO_AUTH              0x00 /**< \brief 不进行验证 */
#define  ZSN603_AUTO_DETECT_E2_AUTH              0x45 /**< \brief 使用E2密钥进行验证 */
#define  ZSN603_AUTO_DETECT_KEY_AUTH             0x46 /**< \brief 使用用户输入密钥进行验证 */

/*****************************************************************
 *  ZSN603  函数指针类型定义
 *****************************************************************/
/** \brief  ZSN603命令传输函数指针类型定义*/
typedef uint8_t (*zsn603_cmd_tx_func_t) (void          *p_drv,
                                         uint8_t       *p_cmd_data,
                                         uint32_t       cmd_lenght);
/** \brief  ZSN603命令接收解析函数指针类型定义*/
typedef int (*zsn603_cmd_rx_analysis_func_t) (void      *p_drv,
                                              uint32_t   buffer_len,
                                              uint8_t   *p_data,
                                              uint32_t  *p_data_lenght);
/** \brief  ZSN603中断回调函数指针类型定义*/
typedef void (*zsn603_int_cb) (void   *p_arg);

/** \brief  自动检测模式下检测到卡中断回调函数指针类型定义*/
typedef void (*zsn603_card_input) (void   *p_arg);


/** \brief 回应帧信息结构体 */
typedef struct zsn603_frame_info {
    uint32_t  status;                      /** \brief 执行状态 */
    uint32_t  info_lenght;                 /** \brief 信息长度 */
}zsn603_frame_info_t;

/** \brief ZSN603通用设备  无论使用何种通信方式  都会使用到该设备*/
typedef struct zsn603_dev {
    uint8_t               work_mode;       /** \brief 设备地址  */
    uint8_t               local_addr;      /** \brief 设备地址  */

    /* 以下四个成员用于自动检测模式下获取数据 */
    volatile uint8_t      status;          /** \brief 设备状态*/
    uint8_t               is_continue;     /** \brief 自动检测 检测到卡后是否继续检测*/
    zsn603_card_input     pfn_card_cb;     /** \brief 自动检测 检测到卡后的用户回调*/
    void                 *p_arg;           /** \brief 自动检测 检测到卡后的用户回调参数*/

    zsn603_cmd_tx_func_t  pfn_cmd_tx;      /** \brief 命令传输函数*/
    /** \brief 命令传输函数*/
    zsn603_cmd_rx_analysis_func_t pfn_cmd_rx_analysis;
}zsn603_dev_t;

/** \brief 定义ZSN603操作句柄  所有的接口函数都会使用到该句柄 */
typedef zsn603_dev_t *   zsn603_handle_t;

/*****************************************************************
 *  ZSN603  UART通信方式设备相关定义
 *****************************************************************/
/** \brief zsn603 设备结构体(UART模式) */
typedef struct zsn603_uart_devinfo {
    uint8_t  local_addr;                                  /** \brief 设备地址  */
    zsn603_platform_uart_devinfo_t  platform_info;        /** \brief 平台相关设备信息  */
}zsn603_uart_devinfo_t;


/** \brief zsn603 设备结构体(UART模式) */
typedef struct zsn603_uart_dev {
    zsn603_dev_t               zsn603_dev;                /** \brief ZSN603 设备*/
    uint8_t                    is_wait ;                  /** \brief 目前等待数据的个数*/
    uint8_t                    rx_count;                  /** \brief 接收到的数据的个数*/
    /** \brief 串口接收数据缓冲区*/
    uint8_t                    buffer[ZSN603_BUFFER_SIZE_MAX];
    ZSN603_SEMB_DECL(uart_semb);                          /** \brief 串口信号量，用于获取串口数据的超时*/
    zsn603_platform_uart_t     platform_dev;              /** \brief I2C模式平台设备*/
    }zsn603_uart_dev_t;

/*****************************************************************
 *  ZSN603 I2C通信方式设备相关定义
 *****************************************************************/
/** \brief zsn603 设备信息结构体(I2C模式) */
typedef struct zsn603_i2c_devinfo {
    uint8_t  local_addr;                          /** \brief 设备地址  */
    zsn603_platform_i2c_devinfo_t platform_info;  /** \brief 平台相关设备信息  */
}zsn603_i2c_devinfo_t;

/** \brief zsn603设备结构体(I2C模式) */
typedef struct zsn603_i2c_dev {
    zsn603_dev_t              zsn603_dev;         /** \brief ZSN603 设备*/
    ZSN603_SEMB_DECL(int_semb);                   /** \brief 中断信号量定义*/
    zsn603_platform_i2c_t     platform_dev;       /** \brief I2C模式平台设备*/
}zsn603_i2c_dev_t;

/**************************************************************
 * 自动检测模式结构体参数定义
 **************************************************************/
/** \brief zsn603自动检测模式设置参数结构体定义 */
typedef struct zsn603_auto_detect_ctrl {
    uint8_t             ad_mode;         /** \brief 自动检测模式(ADMode)*/
    uint8_t             tx_mode;         /** \brief 天线驱动方式(TXMode)*/
    uint8_t             req_code;        /** \brief 请求代码(ReqCode)*/
    uint8_t             auth_mode;       /** \brief 验证模式(AuthMode)*/
    uint8_t             key_type;        /** \brief 密钥类型(KeyType)*/
    uint8_t             key_num;         /** \brief E2密钥区号*/
    uint8_t            *p_key;           /** \brief 密钥(Key)*/
    uint8_t             key_len;         /** \brief 密钥长度(KeyLen)*/
    uint8_t             block;           /** \brief 验证的卡块号(Block)*/
    zsn603_card_input   pfn_card_input;  /** \brief 检测到卡时的回调函数*/
    void               *p_arg;           /** \brief 检测到卡时的回调函数参数*/
}zsn603_auto_detect_ctrl_t;

/** \brief zsn603自动检测模式设置参数结构体定义 */
typedef struct zsn603_auto_detect_data {
    uint8_t             ant_mode;        /** \brief 天线模式(TXMode)*/
    uint16_t            atq;             /** \brief 请求应答(ATQ)*/
    uint8_t             sak;             /** \brief 选择应答(SAK)*/
    uint8_t             id_len;          /** \brief 序列号长度(UIDLen)*/
    uint8_t             uid[10];         /** \brief 序列号(UID)*/
    uint8_t             data_len;        /** \brief 块数据长度*/
    uint8_t             data[16];        /** \brief 块数据缓存地址*/
}zsn603_auto_detect_data_t;
/**
 * \brief  ZSN603 初始化(UART通信模式)
 *
 * \param[in] p_dev           : ZSN603设备
 * \param[in] local_address   : ZSN603设备local address
 * \param[in] p_trans_arg     : 传输函数参数
 *
 * \retval NULL   : 初始化错误
 * \retval handle : 执行成功
 */
zsn603_handle_t  zsn603_uart_init(zsn603_uart_dev_t            *p_dev,
                                  const zsn603_uart_devinfo_t  *p_devinfo);

/**
 * \brief  ZSN603 解初始化(UART通信模式)
 *
 * \param[in] handle       : ZSN603句柄
 * \retval NULL   : 初始化错误
 * \retval handle : 执行成功
 */
int  zsn603_uart_deinit(zsn603_handle_t handle);

/**
 * \brief  ZSN603 初始化(I2C通信模式)
 *
 * \param[in] p_dev           : ZSN603设备
 * \param[in] p_devinfo       : ZSN603设备设备信息
 *
 * \retval NULL   : 初始化错误
 * \retval handle : 执行成功
 */
zsn603_handle_t  zsn603_i2c_init(zsn603_i2c_dev_t           *p_dev,
                                 const zsn603_i2c_devinfo_t *p_devinfo);

/**
 * \brief  ZSN603 解初始化(I2C通信模式)
 *
 * \param[in] handle       : ZSN603句柄
 * \retval NULL   : 初始化错误
 * \retval handle : 执行成功
 */
int  zsn603_i2c_deinit(zsn603_handle_t handle);

/**
 * \brief  若ZSN603 初始化使用IIC模式     必须将该终端函数作为中断引脚的回调函数
 *
 *         在IIC模式下，ZSN603完成命令执行之后，中断引脚将会被拉低
 *         所以用户可以将 int_pin 配置为下降沿触发，并设置引脚中断回调函数为此函数，函数参数为 ZSN603 设备句柄 即可
 *
 * \param[in] p_arg           : 中断回调函数参数
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
void zsn603_int_pin_hander(void *p_arg);


/**
 * \brief 获取设备信息  将会返回固件版本号
 *
 * \param[in]  handle          : ZSN603设备服务句柄
 * \param[in]  buffer_len      : 输入缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_rx_data       : 保存设备信息的缓冲区
 * \param[out] p_rx_data_count : 接收到的设备信息
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_get_device_info(zsn603_handle_t      handle,
                               uint32_t             buffer_len,
                               uint8_t             *p_rx_data,
                               uint32_t            *p_rx_data_count);


/**
 * \brief 配置IC卡接口
 *
 * \param[in] handle  : ZSN603设备服务句柄
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_config_icc_interface(zsn603_handle_t      handle);

/**
 * \brief 关闭IC卡接口
 *
 * \param[in] handle  : ZSN603设备服务句柄
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_close_icc_interface(zsn603_handle_t      handle);

/**
 * \brief 设置IC卡协议
 *
 * \param[in] handle       : ZSN603设备服务句柄
 * \param[in] isotype      : 协议类型   ZSN603_ICC_ISO_TYPE_A
 *                                      ZSN603_ICC_ISO_TYPE_B
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_set_ios_type(zsn603_handle_t  handle,
                            uint8_t          isotype);
/**
 * \brief 装载IC卡密钥
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \param[in] key_type    : 密钥类型(1字节):   0x60——密钥A
 *                                             0x61——密钥B
 * \param[in] key_block   :密钥区号(1字节):  取值范围0~15
 * \param[in] p_key       :密钥(6字节或16字节)
 * \param[in] key_length  : 密钥长度     6 /16
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_load_icc_key(zsn603_handle_t  handle,
                            uint8_t          key_type,
                            uint8_t          key_block,
                            uint8_t         *p_key,
                            uint32_t         key_length);

/**
 * \brief 设置IC卡接口的寄存器值
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \param[in] reg_addr    : 寄存器地址(1字节):  0x00~0x3F
 * \param[in] reg_val     : 寄存器值(1字节)
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_set_icc_reg(zsn603_handle_t handle,
                           uint8_t         reg_addr,
                           uint8_t         reg_val);

/**
 * \brief 获取IC卡接口的寄存器值
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \param[in] p_info      : 寄存器地址(1字节):  0x00~0x3F
 * \param[out] p_val      : 寄存器值(1字节)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_get_icc_reg(zsn603_handle_t  handle,
                           uint8_t          reg_addr,
                           uint8_t         *p_val);

/**
 * \brief 设置串口模式工作波特率
 *
 * \param[in] handle            : ZSN603设备服务句柄
 * \param[in] baudrate_flag     : 波特率编号(1字节):
 *                      ZSN603_BAUD_172800     ZSN603_BAUD_115200
 *                      ZSN603_BAUD_57600      ZSN603_BAUD_38400
 *                      ZSN603_BAUD_28800      ZSN603_BAUD_19200
 *                      ZSN603_BAUD_230400     ZSN603_BAUD_9600
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功   注
 *  需要值得注意    若执行该函数成功之后，用户调整串口对应的波特率
 */
uint8_t zsn603_set_baud_rate(zsn603_handle_t  handle,
                             uint8_t          baudrate_flag);

/**
 * \brief 设置天线驱动模式
 *
 * \param[in] handle        : ZSN603设备服务句柄
 * \param[in] antmode_flag  : 天线驱动模式(1字节):
 *                ZSN603_ANT_MODE_TX12_LATERNATE     ZSN603_ANT_MODE_TX1
 *                ZSN603_ANT_MODE_TX2                ZSN603_ANT_MODE_TX12
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_set_ant_mode(zsn603_handle_t  handle,
                            uint8_t          antmode_flag);

/**
 * \brief 切换天线通道
 *
 * \param[in] handle       : ZSN603设备服务句柄
 * \param[in] ant_channel  : 天线编号(1字节): 0x00~0x07
 *
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_set_ant_channel(zsn603_handle_t  handle,
                               uint8_t          ant_channel);

/**
 * \brief  设置设备从机地址
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \param[in] slv_addr    : 从机地址(1字节)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功  若执行成功后  ZSN603设备地址会自动变为新的设置的值
 */
uint8_t zsn603_set_local_addr(zsn603_handle_t  handle,
                              uint8_t          slv_addr);

/**
 * \brief  LED灯控制
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \param[in] control_led : 控制指令(1字节)  :ZSN603_CONTROL_LED_ON
 *                                            ZSN603_CONTROL_LED_OFF
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_control_led(zsn603_handle_t    handle,
                           uint8_t            control_led);

/**
 * \brief  蜂鸣器控制
 *
 * \param[in] handle       : ZSN603设备服务句柄
 * \param[in] control_byte : 控制指令(1字节)   后四位控制鸣叫次数    前四位控制鸣叫时间
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_control_buzzer(zsn603_handle_t  handle,
                              uint8_t          control_byte);

/**
 * \brief  读EEPROM
 *
 * \param[in]  handle      : ZSN603设备服务句柄
 * \param[in]  eeprom_addr : E2PROM址址(1字节):范围0~255
 * \param[in]  nbytes:     : 读取数据的长度(1字节)
 * \param[in]  buffer_len  : 输入缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_buf       : 返回数据的缓冲区
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_read_eeprom(zsn603_handle_t  handle,
                           uint8_t          eeprom_addr,
                           uint8_t          nbytes,
                           uint32_t         buffer_len,
                           uint8_t         *p_buf);

/**
 * \brief  写EEPROM
 *
 * \param[in] handle       : ZSN603设备服务句柄
 * \param[in] eeprom_addr  : E2PROM址址(1字节):范围0~255
 * \param[in] nbytes       : 写数据的长度(1字节)
 * \param[in] p_buf        : 要写入的数据信息(nbytes字节)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_write_eeprom(zsn603_handle_t      handle,
                                  uint8_t        eeprom_addr,
                                  uint8_t        nbytes,
                                  uint8_t       *p_buf);

/**
 * \brief  用于Mifare卡的请求操作
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] req_mode   : 请求模式(1字节): 0x26——IDLE模式
 *                                          0x52——ALL模式
 * \param[out] p_atq     : 返回的atq信息    (2字节)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_request(zsn603_handle_t   handle,
                              uint8_t           req_mode,
                              uint16_t         *p_atq);

/**
 * \brief  用于Mifare卡的防碰撞操作,需要执行成功一次请求命令,并返回请求成功,才能进行防碰撞操作,否则返回错误。
 *
 * \param[in] handle         : ZSN603设备服务句柄
 * \param[in] anticoll_level : 防碰撞等级(1字节):  0x93——第一级防碰撞
 *                                                 0x95——第二级防碰撞
 *                                                 0x97——第三级防碰撞
 * \param[in] p_know_uid     : 序列号(n字节):  (若位计数≠0)
 * \param[in] nbit_cnt       : 位计数(1字节):   已知的序列号的长度n
 * \param[out] p_uid         : 返回的UID（4字节，低字节在先），若UID不完整，则最低字节为级联标志0x88，需要进行更高一级的防碰撞。
 * \param[out] p_uid_cnt     : 返回的UID长度
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_anticoll(zsn603_handle_t  handle,
                              uint8_t           anticoll_level,
                              uint8_t          *p_know_uid,
                              uint8_t           nbit_cnt,
                              uint8_t          *p_uid,
                              uint32_t         *p_uid_cnt);

/**
 * \brief  该命令用于Mifare卡的选择操作。
 *
 *   需要成功执行一次防碰撞命令，并返回成功，才能进行卡选择操作，否则返回错误。
 *
 * \param[in]  handle     : ZSN603设备服务句柄
 * \param[in]  p_info     : 选择代码(1字节)   0x93——第一级防碰撞
 *                                            0x95——第二级防碰撞
 *                                            0x97——第三级防碰撞
 * \param[in]  p_uid      : UID(4字节)      前一个防碰撞命令返回的UID
 * \param[out] p_sak      : 返回的信息，若bit2为1，则表明UID不完整
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_select(zsn603_handle_t  handle,
                             uint8_t          anticoll_level,
                             uint8_t         *p_uid,
                             uint8_t         *p_sak);

/**
 * \brief  该命令用于Mifare卡的挂起操作,使所选择的卡进入HALT状态,在HALT状态下,
 *         卡将不响应读卡器发出的IDLE模式的请求,除非将卡复位或离开天线感应区后再进入。
 *         但它会响应读卡器发出的ALL请求。
 *
 * \param[in] handle      : ZSN603设备服务句柄
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */

uint8_t zsn603_mifare_halt(zsn603_handle_t      handle);
/**
 * \brief  该命令用芯片内部已存入的密钥与卡的密钥进行验证,所以使用该命令前,
 *         应事先用“装载IC卡密钥”命令把密钥成功载入芯片内,
 *         另外,需要验证的卡的扇区号不必与芯片内密钥区号相等。
 * \param[in] key_type     : 密钥类型(1字节)  - 0x60——密钥A
 *                                            - 0x61——密钥B
 * \param[in] p_uid      : 卡序列号，4字节
 * \param[in] key_sec    : 密钥区号:0 ~ 7
 * \param[in] nblock     : 需要验证的卡块号，取值范围与卡类型有关，
 *                         - S50:0 ~ 63
 *                         - S70:0 ~ 255
 *                         - PLUS CPU 2K:0 ~ 127
 *                         - PLUS CPU 4K:0 ~ 255
 * \param[in] data_count : 数据长度      7
 * \param[out] p_rx_frame_info : 保存回应帧的结构体
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_eeprom_auth(zsn603_handle_t  handle,
                           uint8_t          key_type,
                           uint8_t         *p_uid,
                           uint8_t          key_sec,
                           uint8_t          nblock);
/**
 * \brief  该命令将密码作为参数传递,因此在此之前不需用“装载IC卡密钥”命令。
 *         若当前卡为PLUS CPU卡的等级2或等级3,且输入的密码只有6字节,
 *         则芯片自动将输入的密码复制2次,取前16字节作为当前验证密钥。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] key_type   : 密钥类型，可以使用下列值：
 *                          - ZSN603_ICC_KEY_TYPE_A
 *                          - ZSN603_ICC_KEY_TYPE_B
 * \param[in] p_uid      : 卡序列号，4字节
 * \param[in] p_key      : 密钥缓冲区
 * \param[in] key_len    : 密钥的长度，只能为6（6字节密钥）或16（16字节密钥）
 * \param[in] nblock     : 需要验证的卡块号，取值范围与卡类型有关，
 *                         - S50：0 ~ 63
 *                         - S70：0 ~ 255
 *                         - PLUS CPU 2K：0 ~ 127
 *                         - PLUS CPU 4K：0 ~ 255
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_key_auth(zsn603_handle_t   handle,
                        uint8_t           key_type,
                        uint8_t          *p_uid,
                        uint8_t          *p_key,
                        uint8_t           key_len,
                        uint8_t           nblock);


/**
 * \brief  该命令对Mifare卡进行读操作,读之前必需成功进行密钥验证。
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \param[in] nblock      : 卡块号(1字节) - S50(0~63)
 *                                        - S70(0~255)
 *                                        - PLUS CPU 2K(0~127)
 *                                        - PLUS CPU 4K(0~255)
 * \param[in]  buffer_len : 输入缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_buf      : 保存读取的块的数据
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_read(zsn603_handle_t  handle,
                           uint8_t          nblock,
                           uint32_t         buffer_len,
                           uint8_t         *p_buf);

/**
 * \brief  该命令对Mifare卡进行写操作,写之前必需成功进行密钥验证。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] p_info     : 卡块号(1字节)  - S50(0~63)
 *                                        - S70(0~255)
 *                                        - PLUS CPU 2K(0~127)
 *                                        - PLUS CPU 4K(0~255)
 * \param[in] p_buf      : 待写入的数据(16字节)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_write(zsn603_handle_t  handle,
                            uint8_t          nblock,
                            uint8_t         *p_buf);

/**
 * \brief  该命令对UltraLight卡进行写操作。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] nblock     : 卡块号(1字节):1~15
 * \param[in] p_buf      : 待写入的数据(4字节)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_ultralight_write(zsn603_handle_t   handle,
                                uint8_t           nblock,
                                uint8_t          *p_buf);

/**
 * \brief  该命令对Mifare卡的值块进行加减操作并写入指定块
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] mode       : 模式(1字节):    0xC0~减
 *                                         0xC1~加
 * \param[in] nblock     : 卡块号(1字节):  S50(0~63)
 *                                         S70(0~255)
 *                                         PLUS CPU 2K(0~127)
 *                                         PLUS CPU 4K(0~255)
 * \param[in] value      : 值(4字节有符号数,低字节在先)
 * \param[in] ntransblk  : 传输块号(1字节)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_value(zsn603_handle_t   handle,
                            uint8_t           mode,
                            uint8_t           nblock,
                            uint8_t           ntransblk,
                            uint32_t          value);

/**
 * \brief  该命令是通过将载波信号关闭指定的时间,再开启来实现卡片复位。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] time_ms    : 时间(1字节),以毫秒为单位,0为一直关闭
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_card_reset(zsn603_handle_t  handle,
                          uint8_t          time_ms);

/**
 * \brief  该命令用于激活卡片,是请求、防碰撞和选择三条命令的组合。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] req_mode   : 请求代码(1字节):  0x26~IDLE
 *                                           0x52~ALL
 *
 * \param[out] p_atq     : 返回的atq信息
 * \param[out] p_saq     : 返回的saq信息
 * \param[out] p_len     : 返回的UID长度
 * \param[out] p_uid     : 返回的UID
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_card_active(zsn603_handle_t   handle,
                                  uint8_t           req_mode,
                                  uint16_t         *p_atq,
                                  uint8_t          *p_saq,
                                  uint8_t          *p_len,
                                  uint8_t          *p_uid);

/**
 * \brief  该命令用于卡片的自动检测,执行该命令成功后,在UART模式下,芯片将主动发送读取到卡片的数据。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] admode     : 自动检测模式ADMode控制字节(1字节)
 *                bit7 ~ bit4:保留
 *                bit3       :执行完之后的动作.
 *                            0-无动作;          1 - 最后执行HALT命令(ZSN603_AUTO_DETECT_EXECUTE_HALT)
 *                bit2       :数据输出后.
 *                            0-不继续检测;      1 - 继续检测(ZSN603_AUTO_DETECT_CONTINUE)
 *                bit1       :当UART接口检测到有卡时，是否产生中断,I2C模式设置无效，必定会产生中断
 *                            0-不产生中断;      1 - 产生中断(ZSN603_AUTO_DETECT_INTERRUPT)
 *                bit0       :当UART接口时检测到有卡时，是否主动发送数据,I2C模式设置无效,从机不能主动发送数据
 *                            0不主动发送数据;   1 - 主动发送数据(ZSN603_AUTO_DETECT_SEND)
 * \param[in] txmode     : 天线模式TxMode控制字节.  - 0x00:Tx1 Tx2 交替驱动
 *                                                  - 0x01:仅Tx1驱动
 *                                                  - 0x02:仅Tx2驱动
 *                                                  - 0x03:Tx1 Tx2 同时驱动
 * \param[in] reqcode    : 请求代码ReqCode(1字节)   - 0x26~IDLE
 *                                                  - 0x52~ALL
 * \param[in] authmode   : 验证模式AuthMode(1字节)  -'E'~用E2密钥验证 (ZSN603_AUTO_DETECT_E2_AUTH)
 *                                                  -'F'~用直接密钥验证(ZSN603_AUTO_DETECT_KEY_AUTH)
 *                                                  - 0 ~不验证  (ZSN603_AUTO_DETECT_NO_AUTH)
 * \param[in] key_type   : 密钥AB KeyType(1字节):   0x60~密钥A  (ZSN603_ICC_KEY_TYPE_A)
 *                                                  0x61~密钥B (ZSN603_ICC_KEY_TYPE_B)
 * \param[in] p_key      : 密钥Key:  若验证模式为'E',则为密钥区号(1字节)
 *                                   若验证模式为'F',则为密钥(6或16字节)
 * \param[in] block      : 密钥长度   (6/16字节)
 * \param[in] block      : 卡块号Block(1字节): S50(0~63)
 *                                             S70(0~255)
 *                                             PLUS CPU 2K(0~127)
 *                                             PLUS CPU 4K(0~255)
 *
 * \param[in] data_count : 若验证模式 = 'E', 则为7
 *                         若验证模式 = 'F', 则为12或22
 *                         若验证模式 =  0,  则为4
 * \param[out] p_rx_frame_info : 保存回应帧的结构体
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_auto_detect(zsn603_handle_t              handle,
                           zsn603_auto_detect_ctrl_t   *p_ctrl);
/**
 * \brief  该命令用于读取自动检测的数据,特别适合于I2C通信模式下使用。
 *         通过该读取自动检测数据命令,可以决定读取数据后是否继续检测。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] ctrl_mode  : 读模式(1字节) - 0x00 取消检测
 *                                       - 0x01 继续检测
 * \param[out] p_data    : 保存返回数据的结构体指针
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_get_auto_detect(zsn603_handle_t             handle,
                               uint8_t                     ctrl_mode,
                               zsn603_auto_detect_data_t  *p_data);

/**
 * \brief  该命令用于设置值块的值。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] block      : 块地址(1字节):将要写入数值的块地址
 * \param[in] data       : 块值(4字节):有符号的32位数据,低字节在前
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_set_value(zsn603_handle_t  handle,
                                uint8_t          block,
                                int              data);
/**
 * \brief  该命令用于获取值块的值,值块里面的数据只有是按照值格式存储时,才能通过该命令读取成功,否则返回失败。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] block      : 块地址(1字节):将要读取数值的块地址
 * \param[out] p_value   : 保存获取值的指针
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_get_value(zsn603_handle_t  handle,
                                uint8_t          block,
                                int             *p_value);

/**
 * \brief  该命令属于芯片扩展功能,用于芯片向卡片发送任意长度组合的数据串,
 *         例如针对NXP新推出的NTAG213F是属于Ultralight C系列卡片,
 *         但是该卡片又新添加了扇区数据读写密钥保护功能。而这个密钥验证命令即可利用此命名传输命令来实现。。
 *
 * \param[in]  handle      : ZSN603设备服务句柄
 * \param[in]  len         : 数据长度(1字节):实际数据长度
 * \param[in]  p_data_buf  : 数据(n-1字节):实际传输的命令数据串
 * \param[in]  data_count  : 0x02
 * \param[in]  wtxm_crc    : b7~b2,wtxm；b1,RFU(0); b0,0,CRC禁止能，1,CRC使能。
 * \param[in]  fwi         : 超时等待时间编码, 超时时间=（（0x01 << fwi）ms
 * \param[in]  buffer_len  : 输入缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_rx_buf    : 数据交互回应帧信息
 * \param[out] p_real_len  : 用于获取接收的信息长度
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_exchange_block(zsn603_dev_t  *handle,
                                     uint8_t       *p_data_buf,
                                     uint8_t        len,
                                     uint8_t        wtxm_crc,
                                     uint8_t        fwi,
                                     uint32_t       buffer_len,
                                     uint8_t       *p_rx_buf,
                                     uint32_t      *p_len);
/**
 * \brief  该命令用读写器与卡片的数据交互,通过该命令可以实现读写卡器的所有功能。
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \param[in] p_tx_buf    : 发送数据的缓冲区
 * \param[in] tx_nbytes   : 发送字节数
 * \param[in]  buffer_len : 输入缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[in] p_rx_buf    : 接收数据的缓冲区
 * \param[in] p_rx_nbytes : 接收字节数
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_mifare_cmd_trans(zsn603_handle_t   handle,
                                uint8_t          *p_tx_buf,
                                uint8_t           tx_nbytes,
                                uint32_t          buffer_len,
                                uint8_t          *p_rx_buf,
                                uint32_t         *p_rx_nbytes);
/**
 * \brief  该命令根据接触式IC卡的复位信息,自动选择T = 0或T = 1传输协议,整个过程不需要使用者干预。该命令用于传输APDU数据流。
 *
 * \param[in] handle        : ZSN603设备服务句柄
 * \param[in] p_tx_buf      : 发送缓冲区
 * \param[in] tx_bufsize    : 发送缓冲区的长度， 1 ~ 272
 * \param[in] buffer_len    : 接收缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[in] p_rx_buf      : 接收缓冲区
 * \param[in] p_rx_len      : 接收数据的长度
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_cicc_tpdu(zsn603_handle_t   handle,
                         uint8_t          *p_tx_buf,
                         uint32_t          tx_bufsize,
                         uint32_t          buffer_len,
                         uint8_t          *p_rx_buf,
                         uint32_t         *p_rx_len);

/**
 * \brief  该命令是冷复位,执行了接触式IC卡上电时序,执行成功后会自动根据IC卡的复位信息来选择‘B’命令使用的传输协议(T = 0 或T = 1)。
 *
 * \param[in] handle        : ZSN603设备服务句柄
 * \param[in] buffer_len    : 复位信息缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[in] p_rx_buf      : 存放复位信息的缓冲区
 * \param[in] p_rx_len      : 获取复位信息长度的指针。
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_cicc_cold_reset(zsn603_handle_t   handle,
                               uint32_t          buffer_len,
                               uint8_t          *p_rx_buf,
                               uint32_t         *p_rx_len);

/**
 * \brief  该命令是热复位,没有执行了接触式IC卡上电时序,
 *         执行成功后会自动根据IC卡的复位信息来选择‘B’命令使用的传输协议(T = 0 或T = 1)。
 *         该命令和冷复位相比较只是没有执行IC卡上电操作。
 *
 * \param[in] handle        : ZSN603设备服务句柄
 * \param[in] buffer_len    : 复位信息缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[in] p_rx_buf      : 存放复位信息的缓冲区
 * \param[in] p_rx_len      : 获取复位信息长度的指针。
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_cicc_warm_reset(zsn603_handle_t    handle,
                               uint32_t           buffer_len,
                               uint8_t           *p_rx_buf,
                               uint32_t          *p_rx_len);

/**
 * \brief  该命令是关闭接触式IC卡的电源和时钟。
 *
 * \param[in] handle      : ZSN603设备服务句柄
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_cicc_deactivation(zsn603_handle_t  handle);

/**
 * \brief  该命令用于T = 0传输协议。
 *
 * \param[in]  handle        : ZSN603设备服务句柄
 * \param[in]  p_tx_buf      : 发送缓冲区
 * \param[in]  tx_bufsize    : 发送缓冲区的长度， 1 ~ 272
 * \param[in]  buffer_len    : 接收缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_rx_buf      : 接收缓冲区
 * \param[out] p_rx_len      : 接收数据的长度
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_cicc_tp0(zsn603_handle_t    handle,
                        uint8_t           *p_tx_buf,
                        uint32_t           tx_bufsize,
                        uint32_t           buffer_len,
                        uint8_t           *p_rx_buf,
                        uint32_t          *p_rx_len);

/**
 * \brief  该命令用于T=1传输协议。
 *
 * \param[in]  handle        : ZSN603设备服务句柄
 * \param[in]  p_tx_buf      : 发送缓冲区
 * \param[in]  tx_bufsize    : 发送缓冲区的长度， 1 ~ 272
 * \param[in]  buffer_len    : 接收缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_rx_buf      : 接收缓冲区
 * \param[out] p_rx_len      : 接收数据的长度
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_cicc_tp1(zsn603_handle_t   handle,
                        uint8_t          *p_tx_buf,
                        uint32_t          tx_bufsize,
                        uint32_t          buffer_len,
                        uint8_t          *p_rx_buf,
                        uint32_t         *p_rx_len);

/**
 * \brief  该命令用于A型卡的请求操作,该命令的操作与Mifare S50/S70卡类的请求操作时一样的。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] req_mode   : 请求模式(1字节): 0x26-IDLE模式
 *                                          0x52-ALL模式
 * \param[out] p_atq     : 返回的atq信息    (2字节)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_request(zsn603_handle_t  handle,
                             uint8_t          req_mode,
                             uint16_t        *p_atq);

/**
 * \brief  该命令用于A型卡的防碰撞操作,该命令的操作与Mifare S50/S70卡类的防碰撞操作时一致的。
 *
 * \param[in] handle         : ZSN603设备服务句柄
 * \param[in] anticoll_level : 防碰撞等级(1字节):  0x93——第一级防碰撞
 *                                            0x95——第二级防碰撞
 *                                            0x97——第三级防碰撞
 * \param[in] p_know_uid     : 序列号(n字节):  (若位计数≠0)
 * \param[in] nbit_cnt       : 位计数，表明已知的序列号uid的位数。其值小于32。
 *                               - 若nbit_cnt值为0,则表明没有已知序列号，序列号的所有位
 *                                 都要从本函数获得。
 *                               - 若nbit_cnt不为0，则序列号中有已知的序列号的值，表示
 *                                 序列号的前nbit_cnt位为已知序列号中所示前nbit_cnt位的
 *                                 值，其余位需要本函数获得。
 * \param[in]  buffer_len    : 接收UID缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_uid         : 保存防碰撞后获取到的卡序列号的缓冲
 * \param[out] p_uid_cnt     : 返回的UID长度
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_anticoll(zsn603_handle_t   handle,
                              uint8_t           anticoll_level,
                              uint8_t          *p_know_uid,
                              uint8_t           nbit_cnt,
                              uint32_t          buffer_len,
                              uint8_t          *p_uid,
                              uint32_t         *p_uid_cnt);

/**
 * \brief  该命令用于A型卡的选择,该命令的操作与Mifare S50/S70卡类的选择操作时一致的。
 *
 * \param[in]  handle     : ZSN603设备服务句柄
 * \param[in]  p_info     : 选择代码(1字节) :  0x93——第一级防碰撞
 *                                             0x95——第二级防碰撞
 *                                             0x97——第三级防碰撞
 * \param[in]  p_uid      : UID(4字节)   : 前一个防碰撞命令返回的UID
 * \param[in]  uid_cnt    : UID长度(4字节): 前一个防碰撞命令返回的UID的长度
 * \param[out] p_sak      : 返回的信息，若bit2为1，则表明UID不完整
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_select(zsn603_handle_t  handle,
                            uint8_t          anticoll_level,
                            uint8_t         *p_uid,
                            uint8_t          uid_cnt,
                            uint8_t         *p_sak);

/**
 * \brief  该命令用于A型卡的挂起,该命令的操作与Mifare S50/S70卡类的挂起命令一致。
 *
 * \param[in] handle      : ZSN603设备服务句柄
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_halt(zsn603_handle_t      handle);

/**
 * \brief  RATS(request for answer to select)是ISO14443-4协议的命令,
 *         芯片发送RATS,卡片发出ATS(answer to select)作为RATS的应答,在执行该命令前,
 *         必需先进行一次卡选择操作,且执行过一次RATS命令后,想再次执行RATS命令,必需先解除激活。
 *
 * \param[in]  handle     : ZSN603设备服务句柄
 * \param[in]  cid        : CID(1字节):  卡标识符(card IDentifier,取值范围0x00~0x0E)
 * \param[in]  buffer_len : 存放接收数据缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_ats_buf  : 用于接收回应的信息，不同卡，字节数会有不同
 * \param[in]  p_rx_len   : 用于获取接收的信息长度
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_rats(zsn603_handle_t   handle,
                          uint8_t           cid,
                          uint32_t          buffer_len,
                          uint8_t          *p_ats_buf,
                          uint32_t         *p_rx_len);

/**
 * \brief  PPS(protocal and parameter selection)是ISO14443-4协议的命令,
 *         用于改变有关的专用协议参数,该命令不是必需的,命令只支持默认参数,即该命令的参数设置为0即可。
 *         在执行该命令前,必需先成功执行一次RATS命令。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] dsi_dri    : DSI_DRI(1字节): 芯片与卡通信波特率,设置为0(106Kb/s)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_pps(zsn603_handle_t  handle,
                         uint8_t          dsi_dri);

/**
 * \brief  该命令是ISO14443-4协议的命令,用于将卡片置为挂起(HALT)状态,
 *         处于挂起(HALT)状态的卡可以用“请求”命令(请求代码为ALL)来重新激活卡,只有执行“RATS”命令的卡才用该命令。
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_deselect(zsn603_handle_t      handle);


/**
 * \brief  T=CL是半双工分组传输协议,ISO14443-4协议命令,用于读写器与卡片之间的数据交互,
 *         一般符合ISO14443协议的CPU卡均用该协议与读写器通信。
 *         调用该命令时只需要将CPU卡COS命令的数据作为输入即可,其他的如分组类型、卡标识符CID、帧等待时间FWT、
 *         等待时间扩展倍增因子WTXM(waiting time extensionmultiplier)等等由该命令自动完成。
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \param[in] p_cos_buf   : COS命令
 * \param[in] cos_bufsize : COS命令的长度
 * \param[in]  buffer_len : 存放接收信息缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_res_buf  : 用于接收回应的信息，不同COS命令，字节数会有不同
 * \param[out] p_rx_len   : 用于获取接收的信息长度
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_tpcl(zsn603_handle_t   handle,
                          uint8_t          *p_cos_buf,
                          uint8_t           cos_bufsize,
                          uint32_t          buffer_len,
                          uint8_t          *p_res_buf,
                          uint32_t         *p_rx_len);

/**
 * \brief  该命令用读写器与卡片的数据交互,通过该命令可以实现读写卡器的所有功能。
 *
 * \param[in]  handle      : ZSN603设备服务句柄
 * \param[in]  len         : 数据长度(1字节):实际数据长度
 * \param[in]  p_data_buf  : 数据(n-1字节):实际传输的命令数据串
 * \param[in]  data_count  : 0x02
 * \param[in]  wtxm_crc    : b7~b2,wtxm；b1,RFU(0); b0,0,CRC禁止能，1,CRC使能。
 * \param[in]  fwi         : 超时等待时间编码, 超时时间=((0x01 << fwi)ms)
 * \param[in]  buffer_len  : 存放信息缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_rx_buf    : 数据交互回应帧信息
 * \param[out] p_real_len  : 用于获取接收的信息长度
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_exchange_block(zsn603_handle_t   handle,
                                    uint8_t          *p_data_buf,
                                    uint8_t           len,
                                    uint8_t           wtxm_crc,
                                    uint8_t           fwi,
                                    uint32_t          buffer_len,
                                    uint8_t          *p_rx_buf,
                                    uint32_t         *p_rx_len);

/**
 * \brief  该命令是通过将载波信号关闭指定的时间,再开启来实现卡片复位。
 *
 * \param[in] handle      : ZSN603设备服务句柄
 * \param[in] time_ms     : 时间(1字节),以毫秒为单位,0为一直关闭
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_reset(zsn603_handle_t  handle,
                           uint8_t          time_ms);

/**
 * \brief  该命令用于激活卡片,是请求、防碰撞和选择三条命令的组合。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] req_mode   : 请求代码(1字节):  0x26~IDLE
 *                                           0x52~ALL
 *
 * \param[out] p_atq     : 返回的atq信息(2字节)
 * \param[out] p_saq     : 返回的saq信息(1字节)
 * \param[out] p_len     : 返回的UID长度(1字节)
 * \param[out] p_uid     : 返回的UID(*p_len个字节  最多10字节)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_picca_active(zsn603_handle_t  handle,
                            uint8_t          req_mode,
                            uint16_t        *p_atq,
                            uint8_t         *p_saq,
                            uint8_t         *p_len,
                            uint8_t         *p_uid);

/**
 * \brief  该命令用于激活B型卡片,在调用该命令前,需要先执行设备控制类的“设置IC卡接口协议(工作模式)(Cmd = D)”,把芯片先配置成TypeB模式。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] req_mode   : 请求代码(1字节): 0x00~IDLE
 *                                          0x08~ALL
 * \param[out] p_uid     : 返回UID   长度为12字节
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_piccb_active(zsn603_handle_t  handle,
                            uint8_t          req_mode,
                            uint8_t         *p_info);

/**
 * \brief  该命令是通过将载波信号关闭指定的时间,再开启来实现卡片复位,其实现方式与A型卡复位一样。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] time_ms    : 时间(1字节),以毫秒为单位,0为一直关闭
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_piccb_reset(zsn603_handle_t  handle,
                           uint8_t          time_ms);

/**
 * \brief  该命令用于B型卡请求。
 *
 * \param[in]  handle     : ZSN603设备服务句柄
 * \param[in]  req_mode   : 请求代码(1字节): 0x00~IDLE
 *                                       0x08~ALL
 * \param[in]  slot_time  : 时隙总数(1字节): 范围0~4
 * \param[in]  buffer_len : 保存返回UID缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_uid      : 返回的UID相关信息，长度为12
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_piccb_request(zsn603_handle_t   handle,
                             uint8_t           req_mode,
                             uint8_t           slot_time,
                             uint32_t          buffer_len,
                             uint8_t          *p_uid);
/**
 * \brief  该命令用于B型卡修改传输属性(卡选择)。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] p_pupi     : PUPI(4字节):   卡片标识符
 * \param[in] cid        : CID(1字节) :   取值范围为 0 - 14, 若不支持CID,则设置为0
 *
 * \param[in] protype    : proType(1字节):支持的协议,由请求回应中的ProtocolType指定
 *                         proType.3:PCD与PICC是否继续通信
 *                                   1:PCD中止与PICC继续通信
 *                                   0:PCD 与PICC继续通信
 *                         proType.2~1:PICC EOF和PCD SOF间的最小延迟
 *                                    11: etu + 512 / fs
 *                                    10: etu + 256 / fs
 *                                    01: etu + 128 / fs
 *                                    00: etu + 32 / fs
 *                         proType.0: 是否遵循ISO14443-4
 *                                    1:遵循ISO14443-4;
 *                                    0:不遵循ISO14443-4. (二代证必须为1)
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_piccb_attrib(zsn603_handle_t  handle,
                            uint8_t         *p_pupi,
                            uint8_t          cid,
                            uint8_t          protype);
/**
 * \brief  该命令用于B型卡挂起,在执行挂起命令前,必需先执行成功过一次请求命令。
 *         执行挂起命令成功后,卡片处于挂起状态,芯片必需通过ALL方式请求卡片,而不能用IDLE方式请求。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] p_pupi     : PUPI(4字节):   4字节标识符
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_piccb_halt(zsn603_handle_t      handle,
                          uint8_t       *p_pupi);
/**
 * \brief  该命令用于读取二代身份ID。该命令包括了卡请求、卡选择、以及获取ID指令。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] req_mode   : 请求代码(1字节):  0x00~IDLE
 *                                           0x08~ALL
 * \param[in] p_uid      : 返回的身份证ID(8字节)
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_piccb_getid(zsn603_handle_t  handle,
                           uint8_t          req_mode,
                           uint8_t         *p_uid);

/**
 * \brief  该命令SL0更新个人化数据。该命令用于SL0（Security Level 0，安全等级0）的PLUS CPU卡个人化，
 * PLUS CPU卡出厂时的安全等级为SL0，该等级下，不需要任何验证就可以向卡里写数据，写入的数据是作为其它安全等级的初始值
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] addr       : PLUS CPU卡存储器地址(2字节):
 * \param[in] p_data     : 数据/AES密钥/配置字(16字节)
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_plus_cpu_write_perso(zsn603_handle_t  handle,
                                    uint16_t         addr,
                                    uint8_t         *p_data);

/**
 * \brief  该命令用于SL0（Security Level 0，安全等级0）的PLUS CPU卡提交个人化数据，
 * 命令“SL0个人化更新数据”只是更新卡中的数据，但该数据还未生效，用户还不能直接使用。
 * “SL0提交个人化”使更新的个人化数据生效。执行该命令后，PLUS CPU卡的安全等级提高到SL1或者SL3（若是支持SL1的卡，
 * 则执行该命令后卡片安全等级提高到SL1；若是只支持SL0和SL3的卡，则执行该命令后卡片安全等级提高到SL3）。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_plus_cpu_commit_perso(zsn603_handle_t      handle);


/**
 * \brief  该命令用于SL3 PLUS CPU卡的密钥验证，验证的密钥通过该命令的参数输入。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] addr       : AES密钥地址（2字节）
 * \param[out] p_data    : AES密钥（16字节）
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_plus_cpu_first_auth(zsn603_handle_t  handle,
                                   uint16_t         addr,
                                   uint8_t         *p_data);

/**
 * \brief  该命令用于SL3 PLUS CPU卡的首次密钥验证，验证的密钥来自芯片内部，掉电不丢失的数据。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] addr       : AES密钥地址（2字节）
 * \param[in] key_block  : 密钥区号（1字节）
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_plus_cpu_first_auth_e2(zsn603_handle_t      handle,
                                      uint16_t             addr,
                                      uint8_t              key_block);

/**
 * \brief  该命令用于SL3 PLUS CPU卡的跟随密钥验证，验证的密钥来自命令参数，
 *         只有执行过“首次验证”命令成功后才能使用该命令。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] addr       : AES密钥地址（2字节）
 * \param[out] p_data    : AES密钥（16字节）
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_plus_cpu_follow_auth(zsn603_handle_t      handle,
                                    uint16_t             addr,
                                    uint8_t             *p_data);

/**
 * \brief  该命令用于SL3 PLUS CPU卡的跟随密钥验证，验证的密钥来自芯片内部掉电不丢失的数据，
 *         只有执行过“首次验证”命令成功后才能使用该命令。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] addr       : AES密钥地址（2字节）
 * \param[in] key_block  : 密钥区号（1字节）
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_plus_cpu_follow_auth_e2(zsn603_handle_t      handle,
                                       uint16_t             addr,
                                       uint8_t              key_block);


/**
 * \brief  该命令用于PLUS CPU卡通过首次验证后的使用过程中，复位读写计数器和验证等信息。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_plus_cpu_sl3_reset_auth(zsn603_handle_t      handle);

/**
 * \brief  该命令用于读取SL3的数据块，在读数据块之前必需成功执行一次密钥验证。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] read_mode  : 读模式(1字节):0x30～命令有MAC；数据密文；回应无MAC
 *                                    0x31～命令有MAC；数据密文；回应有MAC
 *                                    0x32～命令有MAC；数据明文；回应无MAC
 *                                    0x33～命令有MAC；数据明文；回应有MAC
 *                                    0x34～命令无MAC；数据密文；回应无MAC
 *                                    0x35～命令无MAC；数据密文；回应有MAC
 *                                    0x36～命令无MAC；数据明文；回应无MAC
 *                                    0x37～命令无MAC；数据明文；回应有MAC
 *
 * \param[in] start_addr  : 起始块号(2字节)
 * \param[in] block_num   : 读的块数(1字节):    范围1～3
 * \param[in]  buffer_len : 存放读取数据缓冲区的大小，若缓冲区的大小小于设备信息长度，则只会返回缓冲区大小数据
 * \param[out] p_rx_data  : 读取数据的缓冲区
 * \param[out] p_rx_lenght: 读取数据的长度
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_plus_cpu_sl3_read(zsn603_handle_t   handle,
                                 uint8_t           read_mode,
                                 uint16_t          start_addr,
                                 uint8_t           block_num,
                                 uint32_t          buffer_len,
                                 uint8_t          *p_rx_data,
                                 uint32_t         *p_rx_lenght);

/**
 * \brief  该命令用于读取SL3的数据块，在读数据块之前必需成功执行一次密钥验证。
 *
 * \param[in] handle     : ZSN603设备服务句柄
 * \param[in] read_mode  : 写模式(1字节):0xA0～命令有MAC；数据密文；回应无MAC
 *                                    0xA1～命令有MAC；数据密文；回应有MAC
 *                                    0xA2～命令有MAC；数据明文；回应无MAC
 *                                    0xA3～命令有MAC；数据明文；回应有MAC
 *
 * \param[in] start_addr  : 起始块号(2字节)
 * \param[in] block_num   : 写的块数(1字节):    范围1～3
 * \param[out] p_rx_data  : 保存待写入数据的缓冲区
 * \param[out] p_rx_lenght: 写入数据的长度  (block_num * 16)
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_plus_cpu_sl3_write(zsn603_handle_t  handle,
                                  uint8_t          write_mode,
                                  uint16_t         start_addr,
                                  uint8_t          block_num,
                                  uint8_t         *p_tx_data,
                                  uint8_t          tx_lenght);
/**
 * \brief  该命令用于对SL3的进行值操作 可将目标块的数据进行加减值操作后写入目标块。
 *
 * \param[in] handle       : ZSN603设备服务句柄
 * \param[in] write_mode   : 值操作模式(1字节):    0xB7～加值
 *                                                 0xB9～减值
 * \param[in] src_addr     : 源块号(2字节)
 * \param[in] dst_addr     : 目的块号(2字节)
 * \param[in] data         : 值数据(4字节):  4字节有符号数，低字节在前，高字节的符号位被忽略
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
uint8_t zsn603_plus_cpu_sl3_value_opr(zsn603_handle_t  handle,
                                      uint8_t          write_mode,
                                      uint16_t         src_addr,
                                      uint16_t         dst_addr,
                                      int              data);
#endif
