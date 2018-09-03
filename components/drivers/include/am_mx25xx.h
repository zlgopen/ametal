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
 * \brief 旺宏公司的MX25系列MX25XX芯片驱动 
 * 
 * 已知兼容芯片： 
 *   - MX25L1606E
 *   - MX25L8006E
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-09-14  tee, first implementation.
 * \endinternal
 */
 
#ifndef __AM_MX25XX_H
#define __AM_MX25XX_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "ametal.h"
#include "am_types.h"
#include "am_spi.h"
#include "am_gpio.h"
#include "am_mtd.h"
    
/**
 * \addtogroup am_if_mx25xx
 * \copydoc am_mx25xx.h
 * @{
 */

/**
 * \brief 定义一个芯片型号
 *
 * \param[in] page_size        : 页大小，使用2的幂表示，如页大小为256，则该值为8
 * \param[in] pages_in_sector  : 单个扇区中包含的页数目，使用2的幂表示
 * \param[in] sectors_in_block : 单个块中包含的扇区数目，使用2的幂表示
 * \param[in] nblocks          : 芯片的总块数
 * \param[in] id               : 芯片ID
 *
 * \return 芯片型号，可以直接作为实例信息中 type 成员的值
 */
#define AM_MX25XX_TYPE_DEF(page_size,         \
                           pages_in_sector,   \
                           sectors_in_block,  \
                           nblocks,           \
                           id)                \
        {page_size, pages_in_sector, sectors_in_block, nblocks, id},

/**
 * \name 已知的一些芯片型号定义
 *
 *     若使用的芯片型号此处未定义，可以使用 \sa AM_MX25XX_TYPE_DEF() 宏定义一
 * 个新的芯片。 这些宏可以直接作为实例信息中 type 成员的值。
 * 
 * @{
 */
#define AM_MX25XX_MX25L8006     AM_MX25XX_TYPE_DEF(8, 4, 4, 4, 0x1420C2)
#define AM_MX25XX_MX25L1606     AM_MX25XX_TYPE_DEF(8, 4, 4, 5, 0x1520C2)

/** @} */

/**
 * \brief 器件型号（包含页大小等信息）
 */
typedef struct am_mx25xx_type {

    uint8_t   page_size;         /**< \brief 页大小，使用2的幂表示  */
    uint8_t   pages_in_sector;   /**< \brief 单个扇区包含的页数，使用2的幂表示  */
    uint8_t   sectors_in_block;  /**< \brief 单个扇区包含的页数，使用2的幂表示  */
    uint8_t   nblocks;           /**< \brief 芯片总块数，使用2的幂表示  */

    /**
     * 该ID由：manufacture_id + memory type + memory density构成
     * 如：MX25L1606 manufacture_id、 memory type、 memory density分别为
     * 0xC2、0x20、0x15，则该值为：0x1520C2
     */
    uint32_t  id;
 
} am_mx25xx_type_t;
 
/**
 * \brief MX25XX 实例信息
 */
typedef struct am_mx25xx_devinfo {
 
    uint16_t          spi_mode;      /**< \brief 器件使用的SPI模式 */
    int               spi_cs_pin;    /**< \brief SPI片选引脚                */
    uint32_t          spi_speed;     /**< \brief 使用的SPI速率           */
    am_mx25xx_type_t  type;          /**< \brief 器件型号                        */
 
} am_mx25xx_devinfo_t;
     
/**
 * \brief MX25XX 实例
 */
typedef struct am_mx25xx_dev {
    am_spi_device_t            spi_dev;        /**< \brief SPI设备              */
    uint32_t                   addr_offset;    /**< \brief 保留地址空间  */
    const am_mx25xx_devinfo_t *p_devinfo;      /**< \brief 实例信息            */
} am_mx25xx_dev_t;

/** \brief 定义 MX25XX 的实例句柄类型 */
typedef struct am_mx25xx_dev *am_mx25xx_handle_t;


/** 
 * \brief 初始化 MX25XX，获取操作 MX25XX 的 handle
 * 
 * \param[in] p_dev      : 指向 MX25XX 实例的指针
 * \param[in] p_devinfo  : 指向 MX25XX 实例信息的指针
 * \param[in] handle     : SPI标准服务handle,使用该 handle 与 MX25XX 进行通信
 *
 * \return MX25XX 操作句柄，如果初始化失败，则返回值为NULL
 */
am_mx25xx_handle_t am_mx25xx_init(am_mx25xx_dev_t            *p_dev,
                                  const am_mx25xx_devinfo_t  *p_devinfo,
                                  am_spi_handle_t             handle);

/**
 * \brief MX25xx的 MTD 功能初始化
 *
 * 初始化后，即可使用MTD标准接口对 MX25xx进行操作
 *
 * \param[in] handle         : MX25XX 操作句柄
 * \param[in] p_mtd          : 指向 MTD 实例的指针
 * \param[in] reserved_nblks : 保留的块数，起始的 reserved_nblks 块不会被MTD使用。
 *
 * \return MTD实例句柄
 *
 */
am_mtd_handle_t am_mx25xx_mtd_init(am_mx25xx_handle_t  handle,
                                   am_mtd_serv_t      *p_mtd,
                                   uint32_t            reserved_nblks);


/**
 * \brief 擦除
 *
 *    擦除的主要作用是将数据全部重置为0xFF（即所有存储单元的位设置为1）。擦除操作
 * 并不能直接擦除某个单一地址单元，擦除的最小单元为扇区，即每次只能擦除单个或多个
 * 扇区。由于擦除的最小单元为扇区，因此，擦除区域的起始地址必须为某扇区的起始地址、
 * 擦除长度必须为扇区大小的整数倍。
 *
 * \param[in] handle : MX25XX 操作句柄
 * \param[in] addr   : 擦除区域的首地址，必须为某扇区的起始地址
 * \param[in] len    : 擦除区域的长度，必须为扇区大小的整数倍
 *
 * \retval  AM_OK     : 擦除成功
 * \retval -AM_EINVAL : 擦除失败, 参数错误
 * \retval -AM_EIO    : 擦除失败, SPI通信出错
 */
int am_mx25xx_erase(am_mx25xx_handle_t  handle,
                    uint32_t            addr,
                    uint32_t            len);


/**
 * \brief 读取数据
 *
 * \param[in] handle : MX25XX 操作句柄
 * \param[in] addr   : 读取数据的首地址
 * \param[in] p_buf  : 读取数据存放的缓冲区
 * \param[in] len    : 数据读取的长度
 *
 * \retval  AM_OK     : 读取数据成功
 * \retval -AM_EINVAL : 读取数据失败, 参数错误
 * \retval -AM_EIO    : 读取数据失败, SPI通信出错
 */
int am_mx25xx_read(am_mx25xx_handle_t  handle,
                   uint32_t            addr,
                   uint8_t            *p_buf,
                   uint32_t            len);


/** 
 * \brief 写入数据
 *
 * \param[in] handle : MX25XX 操作句柄
 * \param[in] addr   : 写入数据的首地址
 * \param[in] p_buf  : 写入数据存放的缓冲区
 * \param[in] len    : 数据读取的长度
 *
 * \retval  AM_OK     : 读取数据成功
 * \retval -AM_EINVAL : 读取数据失败, 参数错误
 * \retval -AM_EIO    : 读取数据失败, SPI通信出错
 */
int am_mx25xx_write(am_mx25xx_handle_t  handle,
                    uint32_t            addr,
                    uint8_t            *p_buf,
                    uint32_t            len);

/**
 * \brief 读取 JEDEC ID
 *
 * 该ID由：manufacture_id + memory type + memory density构成
 * 
 * \param[in]  handle   : MX25XX 操作句柄
 * \param[out] p_id     : JEDEC ID
 *
 * \retval  AM_OK     : 读取成功
 * \retval -AM_EINVAL : 读取失败, 参数错误
 * \retval -AM_EIO    : 读取失败, SPI通信出错
 */ 
int am_mx25xx_id_read(am_mx25xx_handle_t handle, uint32_t *p_id);
                                        
/** 
 * \brief 使用REMS命令读取 厂家ID（1字节）和器件ID（1字节）
 * 
 * \param[in]  handle           : MX25XX 操作句柄
 * \param[out] p_manufacture_id : 厂家ID（1字节）
 * \param[out] p_device_id      : 设备ID（1字节）
 *
 * \retval  AM_OK     : 读取成功
 * \retval -AM_EINVAL : 读取失败, 参数错误
 * \retval -AM_EIO    : 读取失败, SPI通信出错
 */                                        
int am_mx25xx_id_rems_read(am_mx25xx_handle_t  handle, 
                           uint8_t            *p_manufacture_id,
                           uint8_t            *p_device_id);
                            
/** 
 * \brief 读取MX25XX状态寄存器的值
 * 
 * \param[in]  handle : MX25XX 操作句柄
 * \param[out] p_stat : 获取到的状态寄存器的值
 *
 * \retval  AM_OK     : 读取状态寄存器成功
 * \retval -AM_EINVAL : 读取失败, 参数错误
 * \retval -AM_EIO    : 读取失败, SPI通信出错
 */
int am_mx25xx_status_read(am_mx25xx_handle_t  handle,
                          uint8_t            *p_stat);

/** 
 * \brief 设置MX25XX状态寄存器的值
 * 
 * \param[in] handle : MX25XX 操作句柄
 * \param[in] val    : 写入状态寄存器的值
 *
 * \retval  AM_OK     : 写入状态寄存器成功
 * \retval -AM_EINVAL : 写入失败, 参数错误
 * \retval -AM_EIO    : 写入失败, SPI通信出错
 *
 * \note 除非特殊应用，否则不建议使用该函数直接写状态寄存器的值
 */
int am_mx25xx_status_write(am_mx25xx_handle_t  handle,
                           uint8_t             val);

/** 
 * \brief 进入掉电模式（低功耗）
 * 
 * \param[in] handle : MX25XX 操作句柄
 *
 * \retval  AM_OK     : 进入掉电模式成功
 * \retval -AM_EINVAL : 进入掉电模式失败, 参数错误
 * \retval -AM_EIO    : 进入掉电模式失败, SPI通信出错
 */                              
int am_mx25xx_deep_power_down_enter(am_mx25xx_handle_t  handle);

/** 
 * \brief 退出掉电模式
 * 
 * \param[in] handle : MX25XX 操作句柄
 *
 * \retval  AM_OK     : 进入掉电模式成功
 * \retval -AM_EINVAL : 进入掉电模式失败, 参数错误
 * \retval -AM_EIO    : 进入掉电模式失败, SPI通信出错
 */
int am_mx25xx_deep_power_down_exit(am_mx25xx_handle_t  handle);

/** 
 * \brief 进入安全区域（OTP数据区），进入后，可以使用读写函数操作OTP数据区
 * 
 * \param[in] handle : MX25XX 操作句柄
 *
 * \retval  AM_OK     : 进入安全区域成功
 * \retval -AM_EINVAL : 进入安全区域失败, 参数错误
 * \retval -AM_EIO    : 进入安全区域失败, SPI通信出错
 *
 * \note 如果该区域被锁定，将只能读取数值，不能修改数值
 */
int am_mx25xx_secured_otp_enter(am_mx25xx_handle_t handle);

/**
 * \brief 退出安全区域（OTP数据区）
 * 
 * \param[in] handle : MX25XX 操作句柄
 *
 * \retval  AM_OK     : 退出安全区域成功
 * \retval -AM_EINVAL : 退出安全区域失败, 参数错误
 * \retval -AM_EIO    : 退出安全区域失败, SPI通信出错
 */
int am_mx25xx_secured_otp_exit(am_mx25xx_handle_t handle);

/** 
 * \brief 检测安全区域（OTP数据区）是否已被锁定
 * 
 * \param[in]  handle     : MX25XX 操作句柄
 * \param[out] p_lockdown : 获取当前的锁定状态，AM_TRUE, 锁定，AM_FALSE,未锁定
 *
 * \retval  AM_OK     : 检测成功
 * \retval -AM_EINVAL : 检测失败, 参数错误
 * \retval -AM_EIO    : 检测失败, SPI通信出错
 */   
int am_mx25xx_secured_lockdown_check(am_mx25xx_handle_t   handle,
                                     am_bool_t           *p_lockdown);

/** 
 * \brief 锁定安全区域（OTP数据区），锁定后，将无法解锁，也无法修改OTP中的数据
 * 
 * \param[in] handle : MX25XX 操作句柄
 *
 * \retval  AM_OK     : 锁定成功
 * \retval -AM_EINVAL : 锁定失败, 参数错误
 * \retval -AM_EIO    : 锁定失败, SPI通信出错
 */   
int am_mx25xx_secured_lockdown(am_mx25xx_handle_t handle);


/** 
 * \brief MX25XX解初始化，释放相关资源
 * 
 *     解初始化后，MX25XX相关函数均不可使用，包括使用MX25XX获取
 * 到的NVRAM标准服务handle。
 * 
 * \param[in] p_dev : 指向 MX25XX 设备的指针
 *
 * \return 无
 */
void am_mx25xx_deinit(am_mx25xx_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
 
#endif /* __AM_MX25XX_H */

/* end of file */
