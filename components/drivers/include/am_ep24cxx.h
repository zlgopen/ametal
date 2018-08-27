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
 * \brief ep24cxx 通用EEPROM驱动
 *
 * EP24CXX 是指与常见的CAT24C02、CAT24C08这一系列EEPROM兼容芯片。芯片容量从
 * 1kbit 到 1Mbit。
 *
 * \internal
 * \par modification history:
 * - 1.00 16-08-03  tee, first implementation
 * \endinternal
 */

#ifndef __AM_EP24CXX_H
#define __AM_EP24CXX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"
#include "am_i2c.h"
#include "am_nvram.h"

/**
 * @addtogroup am_if_ep24cxx
 * @copydoc am_ep24cxx.h
 * @{
 */

/**
 * \brief 根据 EEPROM 芯片属性定义一款芯片
 *
 *     不同芯片之间的一些属性（容量、页大小等）可能存在差异，通过定义各个属性，
 * 即可确定一款芯片或一系列属性相同的芯片。定义了如下6种属性：
 *
 * 1. 页大小
 *     如AT24C02有分页，页大小为8字节；而AT24C1024的页大小为256字节。无分页时，
 * 该属性无效。
 *
 * 2. 有无分页
 *     如AT24C02有分页，页大小为8字节；而铁电公司的FRAM无分页。
 *
 * 3. 数据地址在器件地址中的位数 
 *     一般地，如AT24C02，器件地址中不包含数据地址；而对于AT24C1024，
 * 由于容量较大，共计1024Kbit，即128K = 2 ^ 17，因此数据地址需要17 bits，芯片设
 * 计时，为了节省通信字节数，寄存器地址使用2字节（16 bits），用以表示数据地址的
 * 低16位，数据地址的最高位存放到器件地址中。此时，该属性的值即为1。
 *
 * 4. 寄存器地址的字节数
 *     如AT24C02，寄存器地址为1字节，用以表示数据地址，此时，该属性的值为1；而对
 * 于AT24C1024，寄存器地址为2字节，用以表示数据地址的低16位，此时，该属性的值则
 * 为2。
 *
 * 5. 芯片容量（byte）
 *     如AT24C02，容量为2K bit，即容量为 256 字节。而对于AT24C1024，容量1024K
 * bit，即128K 字节。由于容量呈现一定的规律，均为128字节的2的幂次整数倍。
 *     即：容量 = (2^n) * 128
 * 因此，实际表示容量时，仅使用整数值n来表示容量，对于AT24C02，则n值为1，对于
 * AT24C1024，则 n 值为 10。
 *
 * 6. 写入时间
 *     EEPROM的芯片写入时间各有差异，如AT24C02，写入时间为 5ms，而铁电公司的FRAM
 * 无分页写入时间为0,无需等待。
 *
 *     这6个属性即可确定的定义一个芯片或一系列相同属性的芯片。这里使用一个32位
 * 数据来统一的表示这些属性。32位数据的位分配如下：
 *
 * - bit[15:0]  ：页大小
 * - bit[18:16] ：数据地址在器件地址中的位数 
 * - bit[19]    ：寄存器地址的字节数，0: 单字节  1: 双字节
 * - bit[23:20] ：芯片容量, (2^n)*128 bytes, n = 0 ~ 15
 * - bit[31:24] ：写入时间，单位ms，有效值，0 ~ 255
 *
 * \param[in] page_size                  : 页大小。
 * \param[in] data_addr_bits_in_slv_addr : 数据地址在器件地址中的位数
 * \param[in] reg_addr_double_bytes      : 寄存器地址双字节。0，单字节；1:，双字节
 * \param[in] chip_size                  : 芯片容量。size = (2^n)*128 bytes
 * \param[in] write_time                 : 写入时间，单位ms 
 *
 * \return 32位数据，表示一款特定的芯片或一系列属性相同的芯片。
 * 
 * \note 一般地，用户无需直接使用该宏，应使用已经定义好的芯片型号。
 */
#define AM_EP24CXX_TYPE_DEF(page_size,                  \
                            data_addr_bits_in_slv_addr, \
                            reg_addr_double_bytes,      \
                            chip_size,                  \
                            write_time)                 \
       (((page_size) << 0)                   | \
        ((data_addr_bits_in_slv_addr) << 16) | \
        ((reg_addr_double_bytes) << 19)      | \
        ((chip_size)      << 20)             | \
        ((write_time)     << 24))
 
/**
 * \name 已知的一些芯片型号定义
 *
 *     若使用的芯片型号此处未定义，可以使用 \sa AM_EP24CXX_CHIP_DEF() 宏定义一
 * 个新的芯片。
 * 
 * @{
 */

/** \brief AT24C01 */
#define AM_EP24CXX_AT24C01      AM_EP24CXX_TYPE_DEF(8,   0,  0,  0,  5)

/** \brief AT24C02 */
#define AM_EP24CXX_AT24C02      AM_EP24CXX_TYPE_DEF(8,   0,  0,  1,  5)

/** \brief AT24C04 */
#define AM_EP24CXX_AT24C04      AM_EP24CXX_TYPE_DEF(16,  1,  0,  2,  5)

/** \brief AT24C08 */
#define AM_EP24CXX_AT24C08      AM_EP24CXX_TYPE_DEF(16,  2,  0,  3,  5)

/** \brief AT24C16 */
#define AM_EP24CXX_AT24C16      AM_EP24CXX_TYPE_DEF(16,  3,  0,  4,  5)

/** \brief AT24C32 */
#define AM_EP24CXX_AT24C32      AM_EP24CXX_TYPE_DEF(32,  0,  1,  5,  5)

/** \brief AT24C64 */
#define AM_EP24CXX_AT24C64      AM_EP24CXX_TYPE_DEF(32,  0,  1,  6,  5)

/** \brief AT24C128 */
#define AM_EP24CXX_AT24C128     AM_EP24CXX_TYPE_DEF(64,  0,  1,  7,  5)

/** \brief AT24C256 */
#define AM_EP24CXX_AT24C256     AM_EP24CXX_TYPE_DEF(64,  0,  1,  8,  5)

/** \brief AT24C512 */
#define AM_EP24CXX_AT24C512     AM_EP24CXX_TYPE_DEF(128, 0,  1,  9,  5)

/** \brief AT24CM01 */
#define AM_EP24CXX_AT24CM01     AM_EP24CXX_TYPE_DEF(256, 1,  1,  10, 5)

/** \brief CAT24C01 */
#define AM_EP24CXX_CAT24C01     AM_EP24CXX_TYPE_DEF(16,  0,  0,  0,  5)

/** \brief CAT24C02 */
#define AM_EP24CXX_CAT24C02     AM_EP24CXX_TYPE_DEF(16,  0,  0,  1,  5)

/** \brief CAT24C04 */
#define AM_EP24CXX_CAT24C04     AM_EP24CXX_TYPE_DEF(16,  1,  0,  2,  5)

/** \brief CAT24C08 */
#define AM_EP24CXX_CAT24C08     AM_EP24CXX_TYPE_DEF(16,  2,  0,  3,  5)

/** \brief CAT24C16 */
#define AM_EP24CXX_CAT24C16     AM_EP24CXX_TYPE_DEF(16,  3,  0,  4,  5)

/** \brief FM24C02 */
#define AM_EP24CXX_FM24C02      AM_EP24CXX_TYPE_DEF(8,   0,  0,  1,  5)

/** \brief FM24C04 */
#define AM_EP24CXX_FM24C04      AM_EP24CXX_TYPE_DEF(16,  1,  0,  2,  5)

/** \brief FM24C08 */
#define AM_EP24CXX_FM24C08      AM_EP24CXX_TYPE_DEF(16,  2,  0,  3,  5)

/** \brief FM24C16 */
#define AM_EP24CXX_FM24C16      AM_EP24CXX_TYPE_DEF(16,  3,  0,  4,  5)

/** \brief FM24C32 */
#define AM_EP24CXX_FM24C32      AM_EP24CXX_TYPE_DEF(32,  0,  1,  5,  5)

/** \brief FM24C64 */
#define AM_EP24CXX_FM24C64      AM_EP24CXX_TYPE_DEF(32,  0,  1,  6,  5)

/** \brief FM24C128 */
#define AM_EP24CXX_FM24C128     AM_EP24CXX_TYPE_DEF(64,  0,  1,  7,  5)

/** \brief FM24C256 */
#define AM_EP24CXX_FM24C256     AM_EP24CXX_TYPE_DEF(64,  0,  1,  8,  5)

/** \brief FM24C512 */
#define AM_EP24CXX_FM24C512     AM_EP24CXX_TYPE_DEF(128, 0,  1,  9,  5)

/** \brief FM24C1024 */
#define AM_EP24CXX_FM24C1024    AM_EP24CXX_TYPE_DEF(256, 1,  1,  10, 5)

/** @} */

/**
 * \brief ep24cxx设备信息结构体定义
 */
typedef struct am_ep24cxx_devinfo {
    
    /** \brief 设备 7-bit 从机地址 */
    uint8_t                        slv_addr;

    /** \brief 芯片型号，如： \sa #AM_EP24CXX_AT24C02 */
    uint32_t                       type;
    
} am_ep24cxx_devinfo_t;
 
/**
 * \brief ep24cxx设备结构体定义
 */
typedef struct am_ep24cxx_dev {

    /** \brief I2C从机设备             */
    am_i2c_device_t                i2c_dev;
 
    /** \brief ep24cxx设备提供的 NVRAM 标准服务 */
    am_nvram_dev_t                *p_serv;
    
    /** \brief 指向设备信息的指针      */
    const am_ep24cxx_devinfo_t    *p_devinfo;
    
} am_ep24cxx_dev_t;

/** \brief ep24cxx 操作句柄定义 */
typedef am_ep24cxx_dev_t *am_ep24cxx_handle_t;
 
/**
 * \brief ep24cxx初始化函数
 *
 * ep24xx可以提供标准的 NVRAM 服务，该初始化函数返回标准的 NVRAM 服务句柄。
 *
 * \param[in] p_dev      : 指向ep24cxx设备的指针
 * \param[in] p_devinfo  : 指向ep24cxx设备信息的指针
 * \param[in] i2c_handle : I2C标准服务操作句柄（使用该I2C句柄与ep24cxx通信）
 *
 * \return 操作ep24cxx的handle，若初始化失败，则返回值为NULL。
 *
 * \note 设备指针p_dev指向的设备只需要定义，不需要在调用本函数前初始化。
 */
am_ep24cxx_handle_t am_ep24cxx_init (am_ep24cxx_dev_t           *p_dev,
                                     const am_ep24cxx_devinfo_t *p_devinfo,
                                     am_i2c_handle_t             i2c_handle);
                                   
/**
 * \brief 初始化EP24CXX的NVRAM功能，向系统提供  NVRAM 标准服务
 * 
 *   该函数将EP24CXX设备作为标准的NVRAM设备注册到系统中
 * 
 * \param[in] handle      : ep24cxx操作句柄
 * \param[in] p_dev       : NVRAM 标准设备
 * \param[in] p_dev_name  : NVRAM 标准设备的设备名
 *
 * \return AM_OK, 提供NVRAM服务成功；其它值，提供NVRAM服务失败。
 *
 * \note 使用该函数后，即可使用NVRAM标准接口 am_nvram_set() 和 am_nvram_get() 访问
 * EP24CXX存储器，使用的段必须在 am_nvram_cfg.c文件中定义，存储段依赖的设备名即可
 * 填写为此处 p_name 指定的名字，如 p_name 为  "fm24c02", 这可以向g_nvram_segs[]存
 * 储段列表中增加如下5个存储段（仅作为示例）：
 *    {"ip",         0,  0,  4,   "fm24c02"},
 *    {"ip",         1,  4,  4,   "fm24c02"},
 *    {"temp_limit", 0,  8,  4,   "fm24c02"},
 *    {"system",     0,  12, 50,  "fm24c02"},
 *    {"test",       0,  62, 178, "fm24c02"},
 */
int am_ep24cxx_nvram_init (am_ep24cxx_handle_t   handle,
                           am_nvram_dev_t       *p_dev,
                           const char           *p_dev_name);

/**
 * \brief 数据写入
 * 
 * \param[in] handle     : ep24cxx操作句柄
 * \param[in] start_addr : 数据写入的起始地址
 * \param[in] p_buf      : 待写入的数据
 * \param[in] len        ：写入数据长度
 *
 * \return AM_OK, 数据写入成功；其它值，数据写入失败。
 */                      
int am_ep24cxx_write (am_ep24cxx_handle_t  handle, 
                      int                  start_addr, 
                      uint8_t             *p_buf, 
                      int                  len);

  
/**
 * \brief 数据读取
 * 
 * \param[in]  handle    : ep24cxx操作句柄
 * \param[in]  start_addr: 数据读取的起始地址
 * \param[out] p_buf     : 存放读取数据的缓冲区
 * \param[in]  len       ：读取数据的长度
 *
 * \return AM_OK, 数据读取成功；其它值，数据读取失败。
 */
int am_ep24cxx_read (am_ep24cxx_handle_t handle,
                     int                 start_addr, 
                     uint8_t            *p_buf, 
                     int                 len);
                              
/**
 * \brief ep24cxx解初始化函数
 *
 * 当不再使用指定的ep24cxx设备时，可以使用该函数解初始化该设备，以释放相关资源
 *
 * \param[in] handle : ep24cxx操作句柄
 *
 * \return AM_OK, 解初始化成功；其它值，解初始化失败。
 */                              
int am_ep24cxx_deinit (am_ep24cxx_handle_t handle);
                              
/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_EP24CXX_H */

/* end of file */
