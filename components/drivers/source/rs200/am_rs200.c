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
 * \brief RS200 driver implement
 *
 * \internal
 * \par Modification History
 * - 1.00 19-12-12  RZ, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_rs200.h"
#include "am_gpio.h"
#include "am_uart.h"
#include "am_vdebug.h"

/*******************************************************************************
    type defines
*******************************************************************************/


/*******************************************************************************
    global variables
*******************************************************************************/


/*******************************************************************************
    local defines
*******************************************************************************/
/**
 * \brief   RS200命令
 */
#define   __RS200_VERSION_ID               0x00       /* 固件版本 */
#define   __RS200_RAIN_STA_ID              0x01       /* 雨量状态 */
#define   __RS200_SYS_STA_ID               0x02       /* 系统状态 */
#define   __RS200_OPTICAL_SYS_VALUE_ID     0x03       /* 光学系统校准值 */
#define   __RS200_RAIN_VALUE_ID            0x04       /* 雨量值（未使用） */
#define   __RS200_RAIN_STA_OUT_FRE_ID      0x05       /* 雨量状态输出频率 */

#define   __RS200_THRESHOLD_V1_ID          0x06       /* 无雨与小雨的阈值V1 */
#define   __RS200_THRESHOLD_V2_ID          0x07       /* 小雨与中雨的阈值V2 */
#define   __RS200_THRESHOLD_V3_ID          0x08       /* 中雨与大雨的阈值V3 */

#define   __RS200_THRESHOLD_S1_ID          0x09       /* 无雨与小雨的阈值S1 */
#define   __RS200_THRESHOLD_S2_ID          0x0A       /* 小雨与中雨的阈值S2 */
#define   __RS200_THRESHOLD_S3_ID          0x0B       /* 中雨与大雨的阈值S3 */

#define   __RS200_THRESHOLD_N1_ID          0x0C       /* 无雨与小雨的阈值N1 */
#define   __RS200_THRESHOLD_N2_ID          0x0D       /* 小雨与中雨的阈值N2 */
#define   __RS200_THRESHOLD_N3_ID          0x0E       /* 中雨与大雨的阈值N3 */

#define   __RS200_OPTICAL_TEST_MODE_ID     0x0F       /* 环境光测模式 */
#define   __RS200_TEMP_ID                  0x10       /* 主机读取RS200温度 */
#define   __RS200_LIGHT_SLEEP_MODE_ID      0x11       /* 光学睡眠模式 */

/**
 * \brief   RS200读写方向控制
 */
#define   __RS200_WRITE                    1          /* 写*/
#define   __RS200_READ                     0          /* 读 */

/**
 * \brief   CRC校验
 */
#define   __CRC_INIT                       0xFF       /* CRC计算初值 */
#define   __CRC_POLY                       0x31       /* 多项式 */
#define   __CRC_XOROUT                     0x00       /* 结果异或值 */

/*******************************************************************************
    Local functions
*******************************************************************************/
/* 每个字节的CRC列表 */
static const unsigned char crc_table[] =
{
    0x00,0x31,0x62,0x53,0xc4,0xf5,0xa6,0x97,0xb9,0x88,0xdb,0xea,0x7d,0x4c,0x1f,0x2e,
    0x43,0x72,0x21,0x10,0x87,0xb6,0xe5,0xd4,0xfa,0xcb,0x98,0xa9,0x3e,0x0f,0x5c,0x6d,
    0x86,0xb7,0xe4,0xd5,0x42,0x73,0x20,0x11,0x3f,0x0e,0x5d,0x6c,0xfb,0xca,0x99,0xa8,
    0xc5,0xf4,0xa7,0x96,0x01,0x30,0x63,0x52,0x7c,0x4d,0x1e,0x2f,0xb8,0x89,0xda,0xeb,
    0x3d,0x0c,0x5f,0x6e,0xf9,0xc8,0x9b,0xaa,0x84,0xb5,0xe6,0xd7,0x40,0x71,0x22,0x13,
    0x7e,0x4f,0x1c,0x2d,0xba,0x8b,0xd8,0xe9,0xc7,0xf6,0xa5,0x94,0x03,0x32,0x61,0x50,
    0xbb,0x8a,0xd9,0xe8,0x7f,0x4e,0x1d,0x2c,0x02,0x33,0x60,0x51,0xc6,0xf7,0xa4,0x95,
    0xf8,0xc9,0x9a,0xab,0x3c,0x0d,0x5e,0x6f,0x41,0x70,0x23,0x12,0x85,0xb4,0xe7,0xd6,
    0x7a,0x4b,0x18,0x29,0xbe,0x8f,0xdc,0xed,0xc3,0xf2,0xa1,0x90,0x07,0x36,0x65,0x54,
    0x39,0x08,0x5b,0x6a,0xfd,0xcc,0x9f,0xae,0x80,0xb1,0xe2,0xd3,0x44,0x75,0x26,0x17,
    0xfc,0xcd,0x9e,0xaf,0x38,0x09,0x5a,0x6b,0x45,0x74,0x27,0x16,0x81,0xb0,0xe3,0xd2,
    0xbf,0x8e,0xdd,0xec,0x7b,0x4a,0x19,0x28,0x06,0x37,0x64,0x55,0xc2,0xf3,0xa0,0x91,
    0x47,0x76,0x25,0x14,0x83,0xb2,0xe1,0xd0,0xfe,0xcf,0x9c,0xad,0x3a,0x0b,0x58,0x69,
    0x04,0x35,0x66,0x57,0xc0,0xf1,0xa2,0x93,0xbd,0x8c,0xdf,0xee,0x79,0x48,0x1b,0x2a,
    0xc1,0xf0,0xa3,0x92,0x05,0x34,0x67,0x56,0x78,0x49,0x1a,0x2b,0xbc,0x8d,0xde,0xef,
    0x82,0xb3,0xe0,0xd1,0x46,0x77,0x24,0x15,0x3b,0x0a,0x59,0x68,0xff,0xce,0x9d,0xac
};

/**
 * \breif   根据查表法计算CRC
 *
 * \param   *ptr[in]    目标地址
 * \param   len[in]     目标长度
 *
 * \retval  CRC计算结果
 *
 */
static uint8_t __cal_crc_table (uint8_t *ptr, uint32_t len)
{
    uint8_t crc = __CRC_INIT;

    while (len--)
    {
        crc = crc_table[crc ^ *ptr++];
    }

    crc ^= __CRC_XOROUT;
    return (crc);
}

/**
 * \brief   校验从RS200接收到的数据是否正确
 *
 * \param   id[in]        接收数据的ID
 * \param   *p_buf[in]    接收到的数据地址
 *
 * \reval   AM_OK：校验正确    其他：校验错误
 */
static int __check_rs200_value(uint8_t id, uint8_t *p_buf)
{
    uint8_t crc = 0;

    crc = __cal_crc_table((uint8_t *)(p_buf + 1), 3);

    if ((crc == *(p_buf + 4)) && (id == (*(p_buf + 1) & 0x7F)))
    {
        return AM_OK;
    } else {
        return AM_ERROR;
    }
}

/**
 * \brief   传输发送给RS200的数据
 *
 * \param   cmd[in]    命令：1为写，0为读
 * \param   id[in]     帧ID
 * \param   data[in]   需要发送的数据
 */
static int __rs200_transmit (am_rs200_dev_t    *p_this, 
                              uint8_t            cmd, 
                              uint8_t            id, 
                              uint16_t           data)
{
    uint8_t txbuf[5];

    txbuf[0] = 0x3A;
    txbuf[1] = (cmd << 7) | id;
    txbuf[2] = data  & 0x00FF;
    txbuf[3] = data >> 8;
    txbuf[4] = __cal_crc_table(&txbuf[1], 3);

    if (am_uart_rngbuf_send(p_this->rngbuf_handle, txbuf, 5) == 5) {
        return AM_OK;
    } else {
        return AM_ERROR;
    }
}

/**
 * \brief   向RS200写入数据
 *
 * \param   *p_this[in] RS200服务句柄
 * \param   id[in]      帧ID
 * \param   id[in]      需要发送的数据
 *
 * \reval   AM_OK：写入成功   其他：写入失败
 */
static int __rs200_write (am_rs200_dev_t *p_this, uint8_t id, uint16_t data)
{
    int ret = AM_OK;
    uint8_t rs200_rxbuf[5];

    __rs200_transmit(p_this, __RS200_WRITE, id, data);

    am_uart_rngbuf_receive(p_this->rngbuf_handle, rs200_rxbuf, 5);

    ret = __check_rs200_value(id, rs200_rxbuf);

    if ((ret == AM_OK) && (data == (rs200_rxbuf[2] | rs200_rxbuf[3] << 8))) {

    } else {
        ret = AM_ERROR;
    }

    return ret;
}

/**
 * \brief   读取RS200数据
 *
 * \param   *p_this[in]    RS200服务句柄
 * \param   id[in]         帧ID
 * \param   *p_data[out]   RS200数据
 *
 * \reval   AM_OK：读取成功    其他：读取失败
 */
static int __rs200_read (am_rs200_dev_t *p_this, uint8_t id, uint16_t *p_data)
{
    uint8_t  ret = AM_OK;
    uint8_t rs200_rxbuf[5];

    __rs200_transmit(p_this, __RS200_READ, id, 0x0);

    am_uart_rngbuf_receive(p_this->rngbuf_handle, rs200_rxbuf, 5);

    ret = __check_rs200_value(id, rs200_rxbuf);

    if (ret == AM_OK)
    {
        *p_data = rs200_rxbuf[2] | rs200_rxbuf[3] << 8;
    }

    return ret;
}

/**
 * \brief   获取固件版本
 *
 * \param   *p_this[in]    RS200服务句柄
 * \param   *p_data[out]   版本数据
 *
 * \reval   AM_OK：读取成功    其他：读取失败
 */
static int __rs200_version_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_VERSION_ID, p_data);
    
}

/**
 * \brief   读取雨量状态
 *
 * \param   *p_this[in]    RS200服务句柄
 * \param   *p_data[out]   雨量状态数据
 *
 * \reval   AM_OK：读取成功    其他：读取失败
 */
static int __rain_sta_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_RAIN_STA_ID, p_data);
}

/**
 * \brief   读取系统状态
 *
 * \param   *p_this[in]    RS200服务句柄
 * \param   *p_data[out]   系统状态
 *
 * \reval   AM_OK：读取成功    其他：读取失败
 */
static int __sys_sta_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_SYS_STA_ID, p_data);
}

/**
 * \brief   读取光学系统校准值
 *
 * \param   *p_this[in]    RS200服务句柄
 * \param   *p_data[out]   光学系统校准值
 *
 * \reval   AM_OK：读取成功    其他：读取失败
 */
static int __optical_sys_value_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_OPTICAL_SYS_VALUE_ID, p_data);
}

/**
 * \brief   设置雨量状态输出频率
 *
 * \param   *p_this[in]    RS200服务句柄
 * \param   data[in]       雨量状态输出频率
 *
 * \reval   AM_OK：设置成功    其他：设置失败
 */
static int __rain_sta_out_fre_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_write(p_this, __RS200_RAIN_STA_OUT_FRE_ID, data);
}

/**
 * \brief   读取雨量状态输出频率
 *
 * \param   *p_this[in]    RS200服务句柄
 * \param   *p_data[out]   雨量状态输出频率
 *
 * \reval   AM_OK：读取成功    其他：读取失败
 */
static int __rain_sta_out_fre_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_RAIN_STA_OUT_FRE_ID, p_data);
}

/**
 * \brief   设置无雨与小雨的阈值V1
 *
 * \param   *p_this[in] RS200服务句柄
 * \param   data[in]    无雨与小雨的阈值V1
 *
 * \reval   AM_OK：设置成功    其他：设置失败
 */
static int __threshold_v1_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_write(p_this, __RS200_THRESHOLD_V1_ID, data);
}

/**
 * \brief   读取无雨与小雨的阈值V1
 *
 * \param   *p_this[in]  RS200服务句柄
 * \param   *p_data[out] 无雨与小雨的阈值V1
 *
 * \reval   AM_OK：读取成功    其他：读取失败
 */
static int __threshold_v1_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_THRESHOLD_V1_ID, p_data);
}

/**
 * \brief   设置小雨与中雨的阈值V2
 *
 * \param   *p_this[in]  RS200服务句柄
 * \param   data[in]     小雨与中雨的阈值V2
 *
 * \reval   AM_OK：设置成功    其他：设置失败
 */
static int __threshold_v2_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_write(p_this, __RS200_THRESHOLD_V2_ID, data);
}

/**
 * \brief   读取小雨与中雨的阈值V2
 *
 * \param   *p_data[out]    小雨与中雨的阈值V2
 *
 * \reval   AM_OK：读取成功    其他：读取失败
 */
static int __threshold_v2_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_THRESHOLD_V2_ID, p_data);
}

/**
 * \brief   设置中雨与大雨的阈值V3
 *
 * \param   data[in]    中雨与大雨的阈值V3
 *
 * \reval   AM_OK：设置成功    其他：设置失败
 */
static int __threshold_v3_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_write(p_this, __RS200_THRESHOLD_V3_ID, data);
}

/**
 * \brief   读取中雨与大雨的阈值V3
 *
 * \param   *p_data[out]    中雨与大雨的阈值V3
 *
 * \reval   AM_OK：读取成功    其他：读取失败
 */
static int __threshold_v3_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_THRESHOLD_V3_ID, p_data);
}

/**
 * \brief   设置无雨与小雨的阈值S1
 *
 * \param   data[in]    无雨与小雨的阈值S1
 *
 * \reval   AM_OK：设置成功    其他：设置失败
 */
static int __threshold_s1_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_write(p_this, __RS200_THRESHOLD_S1_ID, data);
}

/**
 * \brief   读取无雨与小雨的阈值S1
 *
 * \param   *p_data[out]    无雨与小雨的阈值S1
 *
 * \reval   true：读取成功，false：读取失败
 */
static int __threshold_s1_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_THRESHOLD_S1_ID, p_data);
}


/**
 * \brief   设置小雨与中雨的阈值S2
 *
 * \param   data[in]    小雨与中雨的阈值S2
 *
 * \reval   AM_OK：设置成功    其他：设置失败
 */
static int __threshold_s2_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_write(p_this, __RS200_THRESHOLD_S2_ID, data);
}

/**
 * \brief   读取小雨与中雨的阈值S2
 *
 * \param   *p_data[out]    小雨与中雨的阈值S2
 *
 * \reval   true：读取成功，false：读取失败
 */
static int __threshold_s2_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_THRESHOLD_S2_ID, p_data);
}

/**
 * \brief   设置中雨与大雨的阈值S3
 *
 * \param   data[in]    中雨与大雨的阈值S3
 *
 * \reval   AM_OK：设置成功    其他：设置失败
 */
static int __threshold_s3_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_write(p_this, __RS200_THRESHOLD_S3_ID, data);
}

/**
 * \brief   读取中雨与大雨的阈值S3
 *
 * \param   *p_data[out]    中雨与大雨的阈值S3
 *
 * \reval   true：读取成功，false：读取失败
 */
static int __threshold_s3_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_THRESHOLD_S3_ID, p_data);
}

/**
 * \brief   设置10次判定为大雨的阈值N1
 *
 * \param   data[in]    10次判定为大雨的N1
 *
 * \reval   AM_OK：设置成功    其他：设置失败
 */
static int __threshold_n1_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_write(p_this, __RS200_THRESHOLD_N1_ID, data);
}

/**
 * \brief   读取10次判定为大雨的阈值N1
 *
 * \param   *p_data[out]    10次判定为大雨的N1
 *
 * \reval   true：读取成功，false：读取失败
 */
static int __threshold_n1_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_THRESHOLD_N1_ID, p_data);
}

/**
 * \brief   设置10次判定为中雨的阈值N2
 *
 * \param   data[in]    10次判定为中雨的N2
 *
 * \reval   AM_OK：设置成功    其他：设置失败
 */
static int __threshold_n2_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_write(p_this, __RS200_THRESHOLD_N2_ID, data);
}

/**
 * \brief   读取10次判定为中雨的阈值N2
 *
 * \param   *p_data[out]    10次判定为中雨的N2
 *
 * \reval   true：读取成功，false：读取失败
 */
static int __threshold_n2_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_THRESHOLD_N2_ID, p_data);
}

/**
 * \brief   设置10次判定为大雨的阈值N3
 *
 * \param   data[in]    10次判定为大雨的N3
 *
 * \reval   AM_OK：设置成功    其他：设置失败
 */
static int __threshold_n3_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_write(p_this, __RS200_THRESHOLD_N3_ID, data);
}

/**
 * \brief   读取10次判定为大雨的阈值N3
 *
 * \param   *p_data[out]    10次判定为大雨的N3
 *
 * \reval   true：读取成功，false：读取失败
 */
static int __threshold_n3_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_THRESHOLD_N3_ID, p_data);
}

/**
 * \brief   设置环境光测模式
 *
 * \param   data[in]    0：退出环境光测模式，1：进入环境光测模式
 *
 * \note    进入环境光测模式，RS01会不停的发生当前测量到的光值
 * \note    发生该指令不会产生返回数据
 */
static int __optical_test_mode_set (am_rs200_dev_t *p_this, uint16_t data)
{
    return __rs200_transmit(p_this,
                            __RS200_WRITE,
                            __RS200_OPTICAL_TEST_MODE_ID,
                            data);
}
/**
 * \brief   读取环境光值
 *
 * \param   *p_data[out]    环境光值
 *
 * \reval   true：读取成功，false：读取失败
 *
 * \note    只有在RS01进入环境光测模式下才能读取环境光值
 */
static int __optical_test_value_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    int ret = AM_OK;
    uint8_t rs01_rxbuf[5];

    am_uart_rngbuf_receive(p_this->rngbuf_handle, rs01_rxbuf, 5);

    ret = __check_rs200_value(__RS200_OPTICAL_TEST_MODE_ID, rs01_rxbuf);

    if (ret == AM_OK)
    {
        *p_data = rs01_rxbuf[2] | rs01_rxbuf[3] << 8;
    }

    return ret;
}

/**
 * \brief   读取芯片温度
 *
 * \param   *p_data[out]    芯片温度
 *
 * \reval   true：读取成功，false：读取失败
 *
 * \note    在进入光学睡眠模式下，不能读取芯片温度
 */
static int __rs200_temp_get (am_rs200_dev_t *p_this, uint16_t *p_data)
{
    return __rs200_read(p_this, __RS200_TEMP_ID, p_data);
}

/**
 * \brief   设置光学睡眠状态
 *
 * \param   data[in]    0：退出光学睡眠，1：进入光学睡眠
 *
 * \note    退出光学模式，RS01会发出版本数据，需要接收该返回数据
 * \note    在进入光学睡眠模式下，不能读取芯片温度
 */
static int __light_sleep_mode_set (am_rs200_dev_t *p_this, uint16_t data)
{
    int ret = AM_ERROR;
    uint8_t rs200_rxbuf[5];
    
    ret = __rs200_transmit(p_this,
                           __RS200_WRITE,
                           __RS200_LIGHT_SLEEP_MODE_ID,
                           data);

    if (data == 0) {
        am_uart_rngbuf_receive(p_this->rngbuf_handle, rs200_rxbuf, 5);
    }
    return ret;
}

/*******************************************************************************
    public functions
*******************************************************************************/

am_rs200_handle_t am_rs200_init (am_rs200_dev_t           *p_dev,
                                 const am_rs200_devinfo_t *p_devinfo,
                                 am_uart_handle_t          handle)
{
    if ((p_dev == NULL) || (handle == NULL)) {
        return NULL;
    }

    p_dev->uart_handle = handle;
    p_dev->p_devinfo   = p_devinfo;
    
    /* UART 初始化为环形缓冲区模式 */
    p_dev->rngbuf_handle = am_uart_rngbuf_init(&(p_dev->rngbuf_dev),
                                               handle,
                                               (uint8_t*)p_devinfo->p_uart_rxbuf,
                                               p_devinfo->rxbuf_size,
                                               (uint8_t*)p_devinfo->p_uart_txbuf,
                                               p_devinfo->txbuf_size);

    am_uart_rngbuf_ioctl(p_dev->rngbuf_handle,
                         AM_UART_RNGBUF_TIMEOUT,
                         (void*)(p_devinfo->timeout_ms));
    return p_dev;
}

/******************************************************************************/
int am_rs200_ioctl (am_rs200_handle_t  handle,
                    int                cmd,
                    void              *p_arg)
{
    int ret = -AM_EIO;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    switch(cmd) {

    case  AM_RS200_VERSION_GET:
        ret = __rs200_version_get(handle, (uint16_t*)p_arg);
        break;

    case  AM_RS200_RAIN_STA_GET:
        ret = __rain_sta_get(handle, (uint16_t*)p_arg);
        break;

    case  AM_RS200_SYS_STA_GET:
        ret = __sys_sta_get(handle, (uint16_t*)p_arg);
        break;

    case  AM_RS200_OPTICAL_SYS_VALUE_GET:
        ret = __optical_sys_value_get(handle, (uint16_t*)p_arg);
        break;

    case  AM_RS200_RAIN_STA_OUT_FRE_SET:
        ret = __rain_sta_out_fre_set(handle, (uint32_t)p_arg);
        break;

    case  AM_RS200_RAIN_STA_OUT_FRE_GET:
        ret = __rain_sta_out_fre_get(handle, (uint16_t*)p_arg);
        break;

    case  AM_RS200_THRESHOLD_V1_SET:
        ret = __threshold_v1_set(handle, (uint32_t)p_arg);
        break;

    case  AM_RS200_THRESHOLD_V1_GET:
        ret = __threshold_v1_get(handle, (uint16_t*)p_arg);
        break;

    case  AM_RS200_THRESHOLD_V2_SET:
        ret = __threshold_v2_set(handle, (uint32_t)p_arg);
        break;

    case  AM_RS200_THRESHOLD_V2_GET:
        ret = __threshold_v2_get(handle, (uint16_t*)p_arg);
        break;

    case  AM_RS200_THRESHOLD_V3_SET:
        ret = __threshold_v3_set(handle, (uint32_t)p_arg);
        break;

    case  AM_RS200_THRESHOLD_V3_GET:
        ret = __threshold_v3_get(handle, (uint16_t*)p_arg);
        break;

    case  AM_RS200_THRESHOLD_S1_SET:
        ret = __threshold_s1_set(handle, (uint32_t)p_arg);
        break;

    case  AM_RS200_THRESHOLD_S1_GET:
        ret = __threshold_s1_get(handle, (uint16_t*)p_arg);
        break;
    
    case  AM_RS200_THRESHOLD_S2_SET:
        ret = __threshold_s2_set(handle, (uint32_t)p_arg);
        break;
    
    case  AM_RS200_THRESHOLD_S2_GET:
        ret = __threshold_s2_get(handle, (uint16_t*)p_arg);
        break;
    
    case  AM_RS200_THRESHOLD_S3_SET:
        ret = __threshold_s3_set(handle, (uint32_t)p_arg);
        break;
    
    case  AM_RS200_THRESHOLD_S3_GET:
        ret = __threshold_s3_get(handle, (uint16_t*)p_arg);
        break;
    
    case  AM_RS200_THRESHOLD_N1_SET:
        ret = __threshold_n1_set(handle, (uint32_t)p_arg);
        break;
    
    case  AM_RS200_THRESHOLD_N1_GET:
        ret = __threshold_n1_get(handle, (uint16_t*)p_arg);
        break;
    
    case  AM_RS200_THRESHOLD_N2_SET:
        ret = __threshold_n2_set(handle, (uint32_t)p_arg);
        break;

    case  AM_RS200_THRESHOLD_N2_GET:
        ret = __threshold_n2_get(handle, (uint16_t*)p_arg);
        break;
    
    case  AM_RS200_THRESHOLD_N3_SET:
        ret = __threshold_n3_set(handle, (uint32_t)p_arg);
        break;
    
    case  AM_RS200_THRESHOLD_N3_GET:
        ret = __threshold_n3_get(handle, (uint16_t*)p_arg);
        break;
    
    case  AM_RS200_OPTICAL_TEST_MODE_SET:
        ret = __optical_test_mode_set(handle, (uint32_t)p_arg);
        break;
    
    case  AM_RS200_OPTICAL_TEST_VALUE_GET:
        ret = __optical_test_value_get(handle, (uint16_t*)p_arg);
        break;
    
    case  AM_RS200_TEMP_GET:
        ret = __rs200_temp_get(handle, (uint16_t*)p_arg);
        break;
    
    case  AM_RS200_LIGHT_SLEEP_MODE_SET:
        ret = __light_sleep_mode_set(handle, (uint32_t)p_arg);
        break;

    default:
        break;
    }

    return ret;
}

/******************************************************************************/
int am_rs200_rain_sta_get (am_rs200_handle_t handle, uint16_t *p_data)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }
    return __rain_sta_get(handle, p_data);
}

/******************************************************************************/
int am_rs200_recv (am_rs200_handle_t  handle,
                   uint8_t           *p_buf,
                   uint32_t           len)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return am_uart_rngbuf_receive(handle->rngbuf_handle, p_buf, len);
}

/* end of file */
