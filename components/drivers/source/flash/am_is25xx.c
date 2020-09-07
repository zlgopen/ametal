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
 * \brief ISSI公司的IS25系列SPI FLASH芯片驱动 
 * 
 * 已知兼容芯片： 
 *   - IS25WP032
 *   - IS25WP064 
 *   - IS25WP0128
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-03  yrz, first implementation.
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/
#include "ametal.h"
#include "am_vdebug.h"
#include "am_is25xx.h"
#include <string.h>

/*******************************************************************************
  宏定义
*******************************************************************************/

/**
 * \name 通过器件型号获取到实际的相关信息
 * @{
 */

/* obtain the page size   */
#define __IS25XX_PAGE_SIZE_GET(type)   (1ul << ((type).page_size))

/* obtain the sector size */
#define __IS25XX_SECTOR_SIZE_GET(type) (1ul << (((type).page_size) + \
                                               ((type).pages_in_sector)))

/* obtain the block size  */
#define __IS25XX_BLCOK_SIZE_GET(type)  (1ul << (((type).page_size)        + \
                                                ((type).pages_in_sector)  + \
                                                ((type).sectors_in_block)))

/* obtain chip size       */
#define __IS25XX_CHIP_SIZE_GET(type)   (1ul << (((type).page_size)        + \
                                                ((type).pages_in_sector)  + \
                                                ((type).sectors_in_block) + \
                                                ((type).nblocks)))

/** @} */

/**
 * \name SPI FLASH的各个命令
 * @{
 */

#define __IS25XX_CMD_WREN       0x06   /**< \brief 写使能                */
#define __IS25XX_CMD_WRDI       0x04   /**< \brief 写禁能                */
#define __IS25XX_CMD_WRSR       0x01   /**< \brief 写状态寄存器          */
#define __IS25XX_CMD_RDID       0x9F   /**< \brief 读ID号                */
#define __IS25XX_CMD_RDSR       0x05   /**< \brief 读状态寄存器          */
#define __IS25XX_CMD_READ       0x03   /**< \brief 读数据                */
#define __IS25XX_CMD_FAST_READ  0x0B   /**< \brief 读数据（快速）        */ 
#define __IS25XX_CMD_RES        0xAB   /**< \brief 读电子ID              */
#define __IS25XX_CMD_REMS       0x90   /**< \brief 读厂家ID和器件ID      */
#define __IS25XX_CMD_DREAD      0x3B   /**< \brief 双数据(2-bit)输出模式 */ 
#define __IS25XX_CMD_SE         0x20   /**< \brief 扇区擦除              */
#define __IS25XX_CMD_BE         0x52   /**< \brief 块擦除（或 0xD8）     */
#define __IS25XX_CMD_CE         0x60   /**< \brief 芯片擦除（或 0xC7）   */
#define __IS25XX_CMD_PP         0x02   /**< \brief 编写页数据            */

#define __IS25XX_CMD_DP         0xB9   /**< \brief 进入深度掉电模式      */
#define __IS25XX_CMD_RDP        0xAB   /**< \brief 退出深度掉电模式      */

/** @} */

/*******************************************************************************
  本地函数
*******************************************************************************/

static int __is25xx_wait_busy (am_is25xx_dev_t *p_dev)
{
    uint8_t status;
    int     ret;

    /* 直到最低位两位均为0 */
    do {
        ret = am_is25xx_status_read(p_dev, &status);
        
        if (ret != AM_OK) {
            return ret;
        }
    } while ((status & 0x01) != 0x00);
    
    return AM_OK;
}

/* 等待busy位和 WEL为均为0，确保写入、擦除等操作成功完成 */
static int __is25xx_wait_busy_and_wel (am_is25xx_dev_t *p_dev)
{
    uint8_t status;
    int     ret;

    /* 直到最低位两位均为0 */
    do {
        ret = am_is25xx_status_read(p_dev, &status);
        
        if (ret != AM_OK) {
            return ret;
        }
    } while ((status & 0x03) != 0x00);
    
    return AM_OK;
}

/******************************************************************************/
static int __is25xx_write_en (am_is25xx_dev_t *p_dev)
{
 
    uint8_t cmd = __IS25XX_CMD_WREN;

    return am_spi_write_then_read(&(p_dev->spi_dev),
                                  &cmd,
                                  1,
                                  NULL,
                                  0);
}

/******************************************************************************/

static int __is25xx_write_dis (am_is25xx_dev_t *p_dev)
{
 
    uint8_t cmd = __IS25XX_CMD_WRDI;

    return am_spi_write_then_read(&(p_dev->spi_dev),
                                  &cmd,
                                  1,
                                  NULL,
                                  0);
}

/******************************************************************************/

/* 读取FLASH 的 JEDEC_ID */
static int __is25xx_id_read (am_is25xx_dev_t *p_dev, uint32_t *p_id)
{

    uint8_t cmd = __IS25XX_CMD_RDID;  

    *p_id = 0;

    return am_spi_write_then_read(&(p_dev->spi_dev),
                                  &cmd,
                                  1,
                                  (uint8_t *)p_id,
                                  3);
}

/******************************************************************************/

/* 每次最多编写一页数据，超过一页数据后，仅最后的一页数据有效 */
static int __is25xx_page_write (am_is25xx_dev_t   *p_dev,
                                uint32_t           addr,
                                uint8_t           *p_buf,
                                uint32_t           len)
{
    uint8_t cmd_buf[4];
    int     ret;

    __is25xx_wait_busy(p_dev);
    __is25xx_write_en(p_dev);
    __is25xx_wait_busy(p_dev);

    cmd_buf[0] = __IS25XX_CMD_PP;
    cmd_buf[1] = (addr >> 16) & 0xFF;
    cmd_buf[2] = (addr >> 8 ) & 0xFF;
    cmd_buf[3] = addr & 0xFF;


    ret = am_spi_write_then_write(&(p_dev->spi_dev),
                                   cmd_buf,
                                   4,
                                   p_buf,
                                   len);
    if (ret != AM_OK) {
        return ret;
    }

    /* 等待写入数据完成 */
    return __is25xx_wait_busy_and_wel(p_dev);
}

/******************************************************************************/
static int __is25xx_read (am_is25xx_dev_t   *p_dev,
                          uint32_t           addr,
                          uint8_t           *p_buf,
                          uint32_t           len)
{
    uint8_t  cmd_buf[5];
    int      ret;

    cmd_buf[0] = __IS25XX_CMD_FAST_READ;
    cmd_buf[1] = (addr >> 16) & 0xFF;
    cmd_buf[2] = (addr >> 8 ) & 0xFF;
    cmd_buf[3] = addr & 0xFF;
    cmd_buf[4] = 0xFF;             /* Dummy Byte */

    /* 快速读前需要确保状态处于空闲状态 */
    ret = __is25xx_wait_busy(p_dev);

    if (ret != AM_OK) {
        return ret;
    }

    return am_spi_write_then_read(&(p_dev->spi_dev),
                                  cmd_buf,
                                  5,
                                  p_buf,
                                  len);
}

/******************************************************************************/
/* program is25xx */
static int __is25xx_program_data (am_is25xx_dev_t        *p_dev,
                                  uint32_t                 addr,
                                  uint8_t                 *p_buf,
                                  uint32_t                 len,
                                  am_bool_t                is_read)
{
    int ret;

    /* read/write flash */
    ret = (is_read == AM_TRUE) ?
             __is25xx_read(p_dev, addr, p_buf, len)
           : __is25xx_page_write(p_dev, addr, p_buf, len);

    return ret;
}

/******************************************************************************/
/* IS25xx read or write */
static int __is25xx_rw (am_is25xx_dev_t        *p_dev,
                        int                     start,
                        uint8_t                *p_buf,
                        size_t                  len,
                        am_bool_t               is_read)
{
    const am_is25xx_devinfo_t *p_devinfo = p_dev->p_devinfo;

    uint32_t maxsize = __IS25XX_CHIP_SIZE_GET(p_devinfo->type);
    uint32_t maxaddr = maxsize - 1;

    uint32_t page    = __IS25XX_PAGE_SIZE_GET(p_devinfo->type);
    uint32_t len_tmp;
    int      ret;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /* start address beyond this chip's capacity */
    if (start > maxaddr) {
        return -AM_ENXIO;
    }

    /* no data will be read or written */
    if (len == 0) {
        return AM_OK;
    }

    /* adjust len that will not beyond this chip's capacity */
    if ((start + len) > maxsize) {
        len = maxsize - start;
    }

    /* if page == 0 or read op. , this means there is no page limit */
    if ((is_read  == AM_TRUE) || (page == 0)) {
        page = maxsize;
    }

    /* write the unaligned data of the start */
    len_tmp =  AM_ROUND_DOWN(start + page, page) - start;
    if (len < len_tmp) {
        len_tmp = len;
    }

    /* lock the device */

    ret = AM_OK;
    ret = __is25xx_program_data(p_dev, start, p_buf, len_tmp, is_read);

    if (ret != AM_OK) {
        return -AM_EIO;
    }

    len   -= len_tmp;
    start += len_tmp;
    p_buf += len_tmp;

    /* write the rest data */

    while (len) {
        len_tmp = len > page ? page : len;
        ret = __is25xx_program_data(p_dev, start, p_buf, len_tmp, is_read);

        if (ret != AM_OK) {
            return -AM_EIO;
        }

        len   -= len_tmp;
        start += len_tmp;
        p_buf += len_tmp;
    }

    return AM_OK;
}

/******************************************************************************/
static int __is25xx_sector_erase (am_is25xx_handle_t handle, uint32_t addr)
{
    uint8_t cmd_buf[4];
    int     ret;

    __is25xx_wait_busy(handle);
    __is25xx_write_en(handle);
    __is25xx_wait_busy(handle);

    cmd_buf[0] = __IS25XX_CMD_SE;

    /* 扇区地址，一个扇区的大小为4K，则低12位全为0 */
    cmd_buf[1] = (addr >> 16) & 0xFF;
    cmd_buf[2] = (addr >> 8 ) & 0xFF;
    cmd_buf[3] = addr & 0xFF;


    ret = am_spi_write_then_read(&(handle->spi_dev),
                                  cmd_buf,
                                  4,
                                  NULL,
                                  0);


    if (ret != AM_OK) {
        return ret;
    }

    /* 等待擦除完成 */
    return 0;
}

/******************************************************************************/
static int __is25xx_block_erase (am_is25xx_handle_t handle, uint32_t addr)
{

    uint8_t cmd_buf[4];
    int     ret;

    __is25xx_wait_busy(handle);
    __is25xx_write_en(handle);
    __is25xx_wait_busy(handle);

    cmd_buf[0] = __IS25XX_CMD_BE;

    /* 扇区地址，一个扇区的大小为4K，则低12位全为0 */
    cmd_buf[1] = (addr >> 16) & 0xFF;
    cmd_buf[2] = (addr >> 8 ) & 0xFF;
    cmd_buf[3] = addr & 0xFF;


    ret = am_spi_write_then_read(&(handle->spi_dev),
                                  cmd_buf,
                                  4,
                                  NULL,
                                  0);


    if (ret != AM_OK) {
        return ret;
    }

    /* 等待擦除完成 */
    return __is25xx_wait_busy_and_wel(handle);
}

/******************************************************************************/
static int __is25xx_chip_erase (am_is25xx_handle_t handle)
{

    uint8_t cmd = __IS25XX_CMD_CE;
    int     ret;

    __is25xx_wait_busy(handle);
    __is25xx_write_en(handle);
    __is25xx_wait_busy(handle);

    ret = am_spi_write_then_read(&(handle->spi_dev),
                                  &cmd,
                                  1,
                                  NULL,
                                  0);


    if (ret != AM_OK) {
        return ret;
    }

    /* 等待擦除完成 */
    return __is25xx_wait_busy_and_wel(handle);
}

/*******************************************************************************
  公共函数
*******************************************************************************/
am_is25xx_handle_t am_is25xx_init (am_is25xx_dev_t           *p_dev,
                                   const am_is25xx_devinfo_t *p_devinfo,
                                   am_spi_handle_t            spi_handle)
{
    uint32_t id;

    if ((p_dev == NULL) || (spi_handle == NULL)) {
        return NULL;
    }
    
    am_gpio_pin_cfg(p_devinfo->spi_cs_pin, AM_GPIO_OUTPUT_INIT_HIGH);
    
    p_dev->p_devinfo = p_devinfo;

    am_spi_mkdev(&(p_dev->spi_dev),
                 spi_handle,
                 8,
                 p_devinfo->spi_mode,
                 p_devinfo->spi_speed,
                 p_devinfo->spi_cs_pin,
                 NULL);
 
    if (am_spi_setup(&(p_dev->spi_dev)) != AM_OK) {
        return NULL;
    }
 
    if (__is25xx_write_dis(p_dev) != AM_OK) {
        return NULL;
    }
    
    if (__is25xx_id_read(p_dev, &id) != AM_OK) {
        return NULL;
    }
    
    if (id != p_devinfo->type.id) {

        AM_DBG_INFO("The id is %x, not your choice chip(id=%x)",
                    id,
                    p_devinfo->type.id);

        return NULL;
    }

    /* 设备地址作为 handle 返回 */
    return p_dev;
}

/******************************************************************************/
void am_is25xx_deinit (am_is25xx_dev_t *p_dev)
{
    return;
}

/******************************************************************************/
int am_is25xx_erase (am_is25xx_handle_t  handle,
                     uint32_t            addr,
                     uint32_t            len)
{
    uint32_t sector_size, block_size, chip_size;

    int ret = 0;

    if (handle == NULL) {
        return -AM_EINVAL;
    }
    
    sector_size = __IS25XX_SECTOR_SIZE_GET(handle->p_devinfo->type);
    block_size  = __IS25XX_BLCOK_SIZE_GET(handle->p_devinfo->type);
    chip_size   = __IS25XX_CHIP_SIZE_GET(handle->p_devinfo->type);
       
    /* Start address must align on sector boundary */
    if (addr & (sector_size - 1)) {
        return -AM_EINVAL;
    }
    
    /* calculate erase sector num */
    len = ((len / sector_size) + (1 && (len % sector_size))) * sector_size; 
    
    /* Do not allow past end of device */
    if (addr + len > chip_size) {
        return -AM_EINVAL;
    }

    /* erase full chip */
    if ((addr == 0) && (len == chip_size)) {
       return __is25xx_chip_erase(handle);
    }

    while (len) {

        /* check if could erase a full block */
        if (((addr & (block_size - 1)) == 0) && (len & (block_size - 1)) == 0) {

            ret = __is25xx_block_erase(handle, addr);

            addr += block_size;
            len  -= block_size;

        } else {
            ret = __is25xx_sector_erase(handle, addr);

            addr += sector_size;
            len  -= sector_size;
        }

        if (ret != AM_OK) {
            break;
        }
    }
    return ret;
}

/******************************************************************************/
int am_is25xx_read (am_is25xx_handle_t  handle,
                    uint32_t            addr,
                    uint8_t            *p_buf,
                    uint32_t            len)
{
    return __is25xx_rw(handle, addr, p_buf, len, AM_TRUE);
}

/******************************************************************************/
int am_is25xx_write (am_is25xx_handle_t  handle,
                     uint32_t            addr,
                     uint8_t            *p_buf,
                     uint32_t            len)
{
    return __is25xx_rw(handle, addr, p_buf, len, AM_FALSE);
}

/*******************************************************************************
  提供MTD初始化接口函数
*******************************************************************************/

static int __is25xx_mtd_erase (void                        *p_cookie,
                               struct am_mtd_erase_info    *p_info)
{
    am_is25xx_dev_t *p_dev = (am_is25xx_dev_t *)p_cookie;

    int            err;
    size_t         len;
    int            addr;

    if (p_cookie == NULL || p_info == NULL) {
        return -AM_EINVAL;
    }

    p_info->fail_addr    = AM_MTD_ERASE_FAIL_ADDR_UNKNOWN;
    p_info->state        = AM_MTD_ERASE_PROCESSING;

    /* get the start address */
    addr = (int)(p_info->addr + p_dev->addr_offset);

    len = p_info->len;

    err = am_is25xx_erase(p_dev, addr, len);

    p_info->state = AM_MTD_ERASE_DONE;

    if (p_info->pfn_callback) {
        p_info->pfn_callback(p_info);
    }

    return err;
}

/******************************************************************************/
static int __is25xx_mtd_read (void                 *p_cookie,
                              uint32_t              addr,
                              void                 *p_buf,
                              uint32_t              len)
{
    am_is25xx_dev_t *p_dev = (am_is25xx_dev_t *)p_cookie;

    int   err;
    int   from;

    if (p_cookie == NULL || p_buf == NULL) {
        return -AM_EINVAL;
    }

    if (!len) {
        return AM_OK;
    }

    from = addr + p_dev->addr_offset;

    err = am_is25xx_read(p_dev, from, p_buf, len);

    return err;
}

/******************************************************************************/
static int __is25xx_mtd_write (void                 *p_cookie,
                               uint32_t              addr,
                               const void           *p_buf,
                               uint32_t              len)
{
    am_is25xx_dev_t *p_dev = (am_is25xx_dev_t *)p_cookie;

    int   err;
    int   to;

    if (p_cookie == NULL || p_buf == NULL) {
        return -AM_EINVAL;
    }

    if (!len) {
        return AM_OK;
    }

    to = addr + p_dev->addr_offset;

    err = am_is25xx_write(p_dev, to, (uint8_t *)p_buf, len);

    return err;
}

/******************************************************************************/
static const struct am_mtd_ops __g_mtd_ops = {
    __is25xx_mtd_erase,       /* mtd_erase */
    __is25xx_mtd_read,        /* mtd_read */
    __is25xx_mtd_write,       /* mtd_write */

};

am_mtd_handle_t am_is25xx_mtd_init(am_is25xx_handle_t  handle,
                                   am_mtd_serv_t      *p_mtd,
                                   uint32_t            reserved_nblks)
{
    uint32_t block_size;
    uint32_t nblocks;

    if ((handle == NULL) || (p_mtd == NULL)) {
        return NULL;
    }

    nblocks = (1 << handle->p_devinfo->type.nblocks);

    /* No available blocks */
    if (reserved_nblks >= nblocks) {
        return NULL;
    }

    block_size = __IS25XX_BLCOK_SIZE_GET(handle->p_devinfo->type);

    /* reserved space not will not used for MTD */
    handle->addr_offset = reserved_nblks * block_size;

    p_mtd->p_ops = &__g_mtd_ops;
    p_mtd->p_drv = (void *)handle;


    p_mtd->size           = (nblocks - reserved_nblks) * block_size;
    p_mtd->type           = AM_MTD_TYPE_NOR_FLASH;
    p_mtd->flags          = AM_MTD_FLAGS_NOR_FLASH;
    p_mtd->erase_size     = __IS25XX_SECTOR_SIZE_GET(handle->p_devinfo->type);
    p_mtd->write_size     = __IS25XX_PAGE_SIZE_GET(handle->p_devinfo->type);
    p_mtd->write_buf_size = 0;

    return p_mtd;
}

/*******************************************************************************
  其它一些器件相关的接口函数
*******************************************************************************/

/* 读取状态 */
int am_is25xx_status_read (am_is25xx_handle_t  handle,
                           uint8_t            *p_stat)
{
    uint8_t cmd = __IS25XX_CMD_RDSR;
 
    return am_spi_write_then_read(&(handle->spi_dev),
                                  &cmd,
                                  1,
                                  p_stat,
                                  1);
}

/******************************************************************************/
/* 写状态寄存器，bit6、bit1、bit0 自动设置，不能使用写命令修改 */
int am_is25xx_status_write (am_is25xx_handle_t handle,
                            uint8_t            val)
{
    uint8_t cmd = __IS25XX_CMD_WRSR;
    
    __is25xx_wait_busy(handle);
    __is25xx_write_en(handle);
    __is25xx_wait_busy(handle);
    
    return am_spi_write_then_read(&(handle->spi_dev),
                                  &cmd,
                                  1,
                                  &val,
                                  1);
}

/******************************************************************************/
int am_is25xx_deep_power_down_enter (am_is25xx_handle_t handle)
{
 
    uint8_t cmd = __IS25XX_CMD_DP;
 
    return am_spi_write_then_write(&(handle->spi_dev),
                                   &cmd,
                                   1,
                                   NULL,
                                   0);
}

/******************************************************************************/
int am_is25xx_deep_power_down_exit (am_is25xx_handle_t handle)
{
 
    uint8_t cmd = __IS25XX_CMD_RDP;
 
    return am_spi_write_then_write(&(handle->spi_dev),
                                   &cmd,
                                   1,
                                   NULL,
                                   0);
}

/******************************************************************************/

/* 读取FLASH 的 JEDEC_ID */
int am_is25xx_id_read (am_is25xx_handle_t handle, uint32_t *p_id)
{
 
    uint8_t cmd = __IS25XX_CMD_RDID;
    
    *p_id = 0;
    
    return am_spi_write_then_read(&(handle->spi_dev),
                                  &cmd,
                                  1,
                                  (uint8_t *)p_id,
                                  3);
}

/******************************************************************************/
int am_is25xx_id_rems_read (am_is25xx_handle_t  handle, 
                            uint8_t            *p_manufacture_id,
                            uint8_t            *p_device_id)
{
 
    uint8_t cmd_buf[4];
    int ret;

    __is25xx_wait_busy(handle);
    
    cmd_buf[0] = __IS25XX_CMD_REMS;
    cmd_buf[1] = 0xFF;
    cmd_buf[2] = 0xFF;
    cmd_buf[3] = 0x00;     /* Manufacturer ID + Device ID */
    
    ret = am_spi_write_then_read(&(handle->spi_dev),
                                   cmd_buf,     /* 先发送4个字节 */
                                   4,
                                   cmd_buf,     /* 再接收2个字节 */
                                   3);
    
    if (ret != AM_OK) {
        return ret;
    }
    
    *p_manufacture_id = cmd_buf[0];
    *p_device_id      = cmd_buf[1];
    
    return AM_OK;
}

/* end of file */

