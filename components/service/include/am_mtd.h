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
 * \brief AMetal MTD (Memory Technology Device) driver.
 *
 * For Now, Only support the Nor FLASH.
 *
 * \internal
 * \par modification history:
 * - 15-09-15 tee, first implementation.
 * \endinternal
 */

#ifndef __AM_MTD_H
#define __AM_MTD_H

#include "ametal.h"
#include "am_types.h"
#include "am_list.h"
#include "am_errno.h"

/**
 * \addtogroup am_if_mtd
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif


/** \brief MTD 设备的类型 */
enum am_mtd_type {
    
    AM_MTD_TYPE_ABSENT          = 0,  
    AM_MTD_TYPE_RAM             = 1,
    AM_MTD_TYPE_ROM             = 2,
    AM_MTD_TYPE_NOR_FLASH       = 3,
    AM_MTD_TYPE_NAND_FLASH      = 4,
    AM_MTD_TYPE_DATA_FLASH      = 5,
    AM_MTD_TYPE_UBI_VOLUME      = 6,
    AM_MTD_TYPE_MLC_NAND_FLASH  = 7
};

/**
 * \name capabilities flags
 * @{
 */
#define AM_MTD_FLAG_WRITEABLE     0x400  /**< device is writeable */
#define AM_MTD_FLAG_BIT_WRITEABLE 0x800  /**< single bits can be flipped */
#define AM_MTD_FLAG_NO_ERASE      0x1000 /**< no erase necessary */
#define AM_MTD_FLAG_POWERUP_LOCK  0x2000 /**< always locked after reset */
/** @} */

/**
 * \name some common devices / combinations of capabilities
 * @{
 */
#define AM_MTD_FLAGS_ROM            0

#define AM_MTD_FLAGS_RAM           (AM_MTD_FLAG_WRITEABLE      | \
                                    AM_MTD_FLAG_BIT_WRITEABLE  | \
                                    AM_MTD_FLAG_NO_ERASE)

#define AM_MTD_FLAGS_NOR_FLASH     (AM_MTD_FLAG_WRITEABLE      | \
                                    AM_MTD_FLAG_BIT_WRITEABLE)

#define AM_MTD_FLAGS_NAND_FLASH    (AM_MTD_FLAG_WRITEABLE)

#define AM_MTD_FLAGS_SDCARD        (AM_MTD_FLAG_WRITEABLE      | \
                                    AM_MTD_FLAG_NO_ERASE)
/** @} */

/** \brief MTD device erase status */
enum am_mtd_erase_status {
    AM_MTD_ERASE_PENDING    = 0x01,
    AM_MTD_ERASE_PROCESSING = 0x02,
    AM_MTD_ERASE_SUSPEND    = 0x04,
    AM_MTD_ERASE_DONE       = 0x08,
    AM_MTD_ERASE_FAILED     = 0x10,
};

/** \brief 擦除失败时， 失败地址未知  */
#define AM_MTD_ERASE_FAIL_ADDR_UNKNOWN  -1LL

/**
 * If the erase fails, \a fail_addr might indicate exactly which block failed.
 * If \a fail_addr = am_MTD_ERASE_FAIL_ADDR_UNKNOWN, the failure was not at the
 * device level or was not specific to any particular block.
 */
struct am_mtd_erase_info {
    off_t                       addr;
    size_t                      len;
    off_t                       fail_addr;
    unsigned int                retries;
    void                      (*pfn_callback) (struct am_mtd_erase_info *self);
    void                       *priv;
    enum am_mtd_erase_status    state;
};

/** \brief MTD driver operations */
struct am_mtd_ops {
    
    /** \brief 擦除函数 */
    int (*pfn_mtd_erase) (void *p_drv, struct am_mtd_erase_info *i);

    /** \brief 读数据函数 */
    int (*pfn_mtd_read)  (void               *p_drv,
                          uint32_t            addr,
                          void               *p_buf,
                          uint32_t            len);

    /** \brief 写入数据函数 */
    int (*pfn_mtd_write) (void               *p_drv,
                          uint32_t            addr,
                          const void         *p_buf,
                          uint32_t            len);
};

/** 
 * \brief MTD 服务结构体类型定义
 */
typedef struct am_mtd_serv {

    enum am_mtd_type    type;            /**< \brief MTD设备的类型      */
    uint32_t            flags;           /**< \brief MTD设备的标志      */
    size_t              size;            /**< \brief MTD总容量大小      */
    unsigned int        erase_size;      /**< \brief 擦除大小           */
    
    /**
     * \brief 最小的写入单元大小
     * 注意，0为非法值，有效值为1或更大。
     */
    unsigned int        write_size;
    
    /**
     * Size of the write buffer used by the MTD. MTD devices having a write
     * buffer can write multiple \a writesize chunks at a time. E.g. while
     * writing 4 * \a writesize bytes to a device with 2 * \a writesize bytes
     * buffer the MTD driver can (but doesn't have to) do 2 \a writesize
     * operations, but not 4. Currently, all NANDs have \a write_buf_size
     * equivalent to \a write_size (NAND page size). Some NOR flashes do have
     * \a write_buf_size greater than \a write_size.
     */
    unsigned int        write_buf_size;

    /** \brief 驱动函数                           */
    const struct am_mtd_ops *p_ops;

    /** \brief 驱动程序私有数据       */
    void                    *p_drv;

} am_mtd_serv_t;

/** \brief MTD 服务句柄  */
typedef am_mtd_serv_t *am_mtd_handle_t;

/**
 * \brief 获取擦除单元的大小（字节数）
 * \param[in] handle : MX25XX 实例句柄
 * \return 擦除单元的大小（字节数）
 */
#define AM_MTD_ERASE_UNIT_SIZE_GET(handle) \
    (handle->erase_size)

/**
 * \brief 获取MTD的容量大小（字节数）
 * \param[in] handle : MX25XX 实例句柄
 * \return MTD的容量大小（字节数）
 */
#define AM_MTD_CHIP_SIZE_GET(handle) \
    (handle->size)

/**
 * \brief 擦除
 *
 * \param[in] handle : MX25XX 操作句柄
 * \param[in] addr   : 擦除区域的首地址，必须为某扇区的起始地址
 * \param[in] len    : 擦除区域的长度，必须为扇区大小的整数倍
 *
 * \return  AM_OK，擦除成功; 其它值，擦除失败
 */
int am_mtd_erase (am_mtd_handle_t     handle,
                  uint32_t            addr,
                  uint32_t            len);

/**
 * \brief 读取数据
 *
 * \param[in] handle : MTD标准设备实例的句柄
 * \param[in] addr   : 读取数据的首地址
 * \param[in] p_buf  : 读取数据存放的缓冲区
 * \param[in] len    : 读取数据的长度
 *
 * \retval  非负数 : 成功读取的字节数
 * \retval  负数      : 读取失败
 */
int am_mtd_read (am_mtd_handle_t     handle,
                 uint32_t            addr,
                 void               *p_buf,
                 uint32_t            len);

/**
 * \brief 写入数据
 *
 * \param[in] handle : MTD标准设备实例的句柄
 * \param[in] addr   : 写入数据的首地址
 * \param[in] p_buf  : 写入数据存放的缓冲区
 * \param[in] len    : 写入数据的长度
 *
 * \retval  非负数 : 成功写入的字节数
 * \retval  负数      : 写入失败
 */
int am_mtd_write (am_mtd_handle_t     handle,
                  uint32_t            addr,
                  const void         *p_buf,
                  uint32_t            len);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* __AM_MTD_H */

/* end of file */
