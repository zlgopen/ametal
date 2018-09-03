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
 * \brief A Flash Translation Layer memory card driver
 *
 * \internal
 * \par Modification history
 * - 1.00 16-08-23  tee, first implementation.
 * \endinternal
 */
#ifndef __AM_FTL_H
#define __AM_FTL_H

#include "ametal.h"
#include "am_mtd.h"
#include "am_nvram.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief FTL需要使用到的内存（RAM）空间大小
 *
 * \param[in] size           : 存储器的容量
 * \param[in] erase_size     : 擦除单元大小
 * \param[in] logic_blk_size : 逻辑块的大小
 * \param[in] nb_log_blocks  : 日志块个数
 *
 * \return FTL需要使用到的内存（RAM）空间大小
 */
#define AM_FTL_RAM_SIZE_GET(size, erase_size, logic_blk_size, nb_log_blocks) \
   (((logic_blk_size) + 4)                                                   \
     + ((size) / (erase_size) * 2)                                           \
     + ((nb_log_blocks) * (sizeof(struct log_buf)))                          \
     + ((nb_log_blocks) * ((erase_size) / (logic_blk_size)))                 \
     + (((((size) / (erase_size)) + 31) / 32) * sizeof(uint32_t)))
 
typedef struct am_ftl_info {

    /** \brief RAM 缓冲区   */
    uint8_t  *p_buf;

    /** \brief RAM 缓冲区的长度，必须为 AM_FTL_RAM_SIZE_GET() 宏得到的值  */
    size_t    len;

    /**
     * \brief 逻辑块的大小
     *
     * 一般与 MTD 设备的写入大小（如：SPI FLASH的页大小）相等或为写入大小的
     * 整数倍，使用 \sa am_ftl_read() 或者 \sa am_ftl_write() 函数读/写的数据
     * 时，每次读/写的数据量即为逻辑块大小
     */
    size_t     logic_blk_size;

    /**
     * \brief 日志块个数，即使用多少个物理块（擦除单元）来缓存数据，该值越大，效率越高
     * 但相应的实际数据块就会减少，一般情况下，可以设置在 2 ~ 10之间，不得小于2。
     */
    size_t     nb_log_blocks;

    /** \brief 保留的物理块（擦除单元）个数，起始的几个块将不会被使用     */
    size_t     reserved_blocks;

} am_ftl_info_t ;


/**
 * FTL 服务结构体定义
 */
typedef struct am_ftl_serv {

    /** \brief The MTD handle                                */
    am_mtd_handle_t mtd;

    /** \brief MAX lbn for read and write operation         */
    uint32_t        max_lbn;

    /** \brief The number of physical blocks(exclude reserved blocks) */
    unsigned int    nb_blocks;

     /** \brief free buf size, (nblock + 31 / 32)             */
    uint8_t         free_size;

    /** \brief sectors per block can be used store data      */
    uint8_t         sectors_per_blk;

    /** \brief The sectors use for header                    */
    uint8_t         sectors_hdr;

    /** \brief last used free block  */
    uint16_t        last_free;

     /** \brief The First EUN for each virtual unit          */
    uint16_t       *p_eun_table;

    /** \brief Store all free blocks                         */
    uint32_t       *p_free;

    /** \brief The 512 byte buffer for copy                  */
    uint8_t         *p_wr_buf;

    /** \brief Log buffer information */
    struct log_buf {
        uint16_t  used;        /**< \brief  used sectors           */
        uint16_t  lbn;         /**< \brief  related logic block    */
        uint16_t  pbn;         /**< \brief  related physical block */
        uint8_t  *p_map;       /**< \brief  sector map             */
    }*p_log_buf;

    const   am_ftl_info_t  *p_info;
 
} am_ftl_serv_t;

/** \brief FTL句柄类型定义  */
typedef am_ftl_serv_t *am_ftl_handle_t;
 
/**
 * \brief FTL初始化
 *
 * \param[in] p_ftl      : FTL 实例
 * \param[in] p_info     : FTL 实例信息
 * \param[in] mtd_handle : 关联的  MTD 存储设备
 *
 * \return FTL 服务句柄，值为NULL时表示初始化失败
 */
am_ftl_handle_t  am_ftl_init (am_ftl_serv_t          *p_ftl,
                              const am_ftl_info_t    *p_info,
                              am_mtd_handle_t         mtd_handle);

/**
 * \brief 获取用户可用的逻辑块个数（lbn_num）
 *
 * 读写逻辑块时，逻辑块编号的有效范围为 : 0 ~ (lbn_num - 1)
 *
 * 每个逻辑块的存储容量为初始化信息中指定的逻辑块大小。
 */
size_t am_ftl_max_lbn_get (am_ftl_serv_t *p_ftl);

/**
 * \brief 写入一块数据
 *
 * \param[in] handle : FTL 实例句柄
 * \param[in] lbn   : 逻辑块
 * \param[in] p_buf : 数据存放的缓冲区（长度固定为初始化时指定的逻辑块大小）
 *
 * \retval AM_OK : 写入数据成功
 * \retval  < 0  : 写入失败
 */
int am_ftl_write (am_ftl_handle_t handle, unsigned int lbn, void *p_buf);

/**
 * \brief 读取一块数据
 *
 * \param[in] handle : FTL 实例句柄
 * \param[in] lbn   : 逻辑块
 * \param[in] p_buf : 数据存放的缓冲区（长度固定为初始化时指定的逻辑块大小）
 *
 * \retval AM_OK : 写入数据成功
 * \retval  < 0  : 写入失败
 */
int am_ftl_read (am_ftl_handle_t handle, unsigned int lbn, void *p_buf);

/**
 * \brief 初始化 NVRAM 功能，以便使用NVRAM接口访问存储器
 *
 * \param[in] handle : FTL 实例句柄
 * \param[in] p_dev  : NVRAM 设备实例
 * \param[in] p_name : NVRAM 设备名，与 NVRAM存储段列表中的设备名对应
 *
 * \retval AM_OK : 初始化成功
 * \retval  < 0  : 初始化失败
 */
int am_ftl_nvram_init (am_ftl_handle_t   handle,
                       am_nvram_dev_t   *p_dev,
                       const char       *p_name);

#ifdef __cplusplus
}
#endif

#endif /* __AM_FTL_H */
