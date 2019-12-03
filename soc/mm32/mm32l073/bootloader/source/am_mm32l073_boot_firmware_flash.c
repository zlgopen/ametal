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
 * \brief bootloader firmware store by flash drivers implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-15  yrh, first implementation
 * \endinternal
 */
#include "am_mm32l073_boot_firmware_flash.h"
#include "am_boot_firmware.h"
#include <string.h>

static int __firmware_flash_start(void *p_drv, uint32_t firmware_size);
static int __firmware_flash_bytes(void *p_drv, uint8_t *p_data, uint32_t firmware_size);
static int __firmware_flash_final(void *p_drv);
static int __firmware_verify(void *p_drv, am_boot_firmware_verify_info_t *p_verify_info);
/**
 * \brief BootLoader 固件存放到flash的标准接口的实现
 */
static struct am_boot_firmware_drv_funcs __g_firmware_flash_drv_funcs = {
    __firmware_flash_start,
    __firmware_flash_bytes,
    __firmware_flash_final,
    __firmware_verify,
};

/**
 * \brief 固件存放到flash的开始函数
 */
static int __firmware_flash_start (void *p_drv, uint32_t firmware_size)
{
    am_mm32l073_boot_firmware_flash_dev_t *p_dev = (am_mm32l073_boot_firmware_flash_dev_t *)p_drv;
    p_dev->firmware_size_is_unknow = AM_FALSE;
    int ret;
    /*如果用户调用开始函数时没有传入固件的大小，就需要边读边擦除flash，传入了固件大小就一次性擦除*/
    if( firmware_size <= 0) {
        p_dev->firmware_size_is_unknow = AM_TRUE;
    }

    am_boot_flash_info_t *flash_info = NULL;
    p_dev->flash_handle->p_funcs->pfn_flash_info_get(p_dev->flash_handle->p_drv,
                                                    &flash_info);

    uint32_t flash_end_addr = flash_info->flash_start_addr + \
        flash_info->flash_size - 1;

    if((p_dev->firmware_dst_addr + firmware_size) > flash_end_addr) {
        return AM_ENOMEM;
    }

    if(p_dev->firmware_dst_addr < flash_info->flash_start_addr) {
        return AM_EFAULT;
    }

    if(0 != ((p_dev->firmware_dst_addr - flash_info->flash_start_addr) &
            (flash_info->flash_sector_size - 1))) {
        return AM_EFAULT;
    }

    p_dev->firmware_total_size     = 0;
    p_dev->erase_sector_start_addr = p_dev->firmware_dst_addr;
    p_dev->curr_program_flash_addr = p_dev->firmware_dst_addr;
    p_dev->curr_buf_data_size      = 0;

    uint32_t erase_size;
    if( p_dev->firmware_size_is_unknow == AM_TRUE) {
        erase_size = flash_info->flash_sector_size;
    } else {
        erase_size = firmware_size;
    }
    ret = p_dev->flash_handle->p_funcs->pfn_flash_erase_region(
        p_dev->flash_handle->p_drv,
        p_dev->erase_sector_start_addr,
        erase_size);

    if(ret != AM_OK) {
        return AM_ERROR;
    }

    return AM_OK;
}

/**
 * \brief 存储固件
 */
static int __firmware_flash_bytes (void *p_drv, uint8_t *p_data, uint32_t firmware_size)
{
    if(p_data == NULL || firmware_size <= 0) {
        return AM_EINVAL;
    }
    int      leave_size;   /* 剩余 */
    int      ret, i;
    uint32_t program_size; /* 每次写入flash的的数据大小  */

    /* 当缓冲区里面还有数据时，需要将缓冲区用新传入的固件数据填满， data_program_start_index 指向填完缓冲区后，
     * 固件数据的起始下标，也即没放入缓冲区的数据的起始下标
     */
    uint32_t data_program_start_index = 0;

    /* 如果按照缓冲区大小的整数倍写入flash后，固件剩余的数据不足缓冲的大小，就要暂时放在缓冲区中，
     * 等待下次数据过来后凑满缓冲区大小的数据在写入flash，leave_firmware_index就是指向要暂时存入缓冲区的数据的起始下标
     */
    uint32_t leave_firmware_index;
    am_mm32l073_boot_firmware_flash_dev_t *p_dev = (am_mm32l073_boot_firmware_flash_dev_t *)p_drv;

    p_dev->firmware_total_size += firmware_size;

    am_boot_flash_info_t *flash_info = NULL;
    p_dev->flash_handle->p_funcs->pfn_flash_info_get(p_dev->flash_handle->p_drv,
                                                    &flash_info);

    uint32_t flash_end_addr = flash_info->flash_start_addr + \
        flash_info->flash_size - 1;

    if((p_dev->firmware_dst_addr + p_dev->firmware_total_size) > flash_end_addr) {
        return AM_ENOMEM;
    }

    if(p_dev->firmware_size_is_unknow == AM_TRUE) {
        /* 固件的大小加上已经擦除的大小， */
        uint32_t add_len = p_dev->curr_program_flash_addr - p_dev->erase_sector_start_addr + firmware_size;

        if(add_len >= flash_info->flash_sector_size) {
            uint32_t erase_sector_count;
            if(add_len & (flash_info->flash_sector_size - 1)) {
                erase_sector_count = add_len / flash_info->flash_sector_size;
            } else {
                erase_sector_count = add_len / flash_info->flash_sector_size - 1;
            }

            ret = p_dev->flash_handle->p_funcs->pfn_flash_erase_region(
                 p_dev->flash_handle->p_drv,
                 p_dev->erase_sector_start_addr + flash_info->flash_sector_size,
                 erase_sector_count * flash_info->flash_sector_size);
            if(ret != AM_OK) {
                return AM_ERROR;
            }
            p_dev->erase_sector_start_addr += erase_sector_count * \
                                              flash_info->flash_sector_size;
        }
    }
    /* 当缓冲区还剩余数据没被写入flash */
    if(p_dev->curr_buf_data_size != 0) {
        for(i = 0; (p_dev->curr_buf_data_size < p_dev->buf_data_size) && (i < firmware_size); i++) {
            p_dev->buf_data[p_dev->curr_buf_data_size++] = p_data[i];
        }

        if(p_dev->curr_buf_data_size == p_dev->buf_data_size) {
            ret = p_dev->flash_handle->p_funcs->pfn_flash_program(
                p_dev->flash_handle->p_drv,
                p_dev->curr_program_flash_addr,
                p_dev->buf_data,
                p_dev->buf_data_size);
            if(ret != AM_OK) {
                return AM_ERROR;
            }
            p_dev->curr_program_flash_addr += p_dev->buf_data_size;
            p_dev->curr_buf_data_size = 0;
        }

        if(i < firmware_size) {
            data_program_start_index = i;
        } else {
            return AM_OK;
        }
    }

    firmware_size = firmware_size - data_program_start_index;
    leave_size    = firmware_size & (p_dev->buf_data_size - 1);
    program_size  = firmware_size - leave_size;
    leave_firmware_index = program_size + data_program_start_index;

    ret = p_dev->flash_handle->p_funcs->pfn_flash_program(
        p_dev->flash_handle->p_drv,
        p_dev->curr_program_flash_addr,
       &p_data[data_program_start_index],
        program_size);
    if(ret != AM_OK) {
        return AM_ERROR;
    }
    p_dev->curr_program_flash_addr += program_size;

    for(i = 0; i < leave_size; i++) {
        p_dev->buf_data[p_dev->curr_buf_data_size++] = p_data[leave_firmware_index++];
    }

    return AM_OK;
}
/**
 * \brief 固件存储结束
 */
static int __firmware_flash_final(void *p_drv)
{
    am_mm32l073_boot_firmware_flash_dev_t *p_dev = (am_mm32l073_boot_firmware_flash_dev_t *)p_drv;
    int ret;
    if(p_dev->curr_buf_data_size != 0) {
        /* 如果结束固件存放时，缓冲区中还有数据 ，且不满时，要用0将缓冲区填满再将剩余的数据写入flash*/
        while(p_dev->curr_buf_data_size < p_dev->buf_data_size) {
            p_dev->buf_data[p_dev->curr_buf_data_size++] = 0xff;
        }
        ret = p_dev->flash_handle->p_funcs->pfn_flash_program(
            p_dev->flash_handle->p_drv,
            p_dev->curr_program_flash_addr,
            p_dev->buf_data,
            p_dev->buf_data_size);
        if(ret != AM_OK) {
            return AM_ERROR;
        }
    }
    p_dev->curr_buf_data_size      = 0;
    p_dev->firmware_total_size     = 0;
    p_dev->erase_sector_start_addr = p_dev->firmware_dst_addr;
    p_dev->curr_program_flash_addr = p_dev->firmware_dst_addr;

    return AM_OK;
}

/**
 * \brief 固件简单校验
 */
static int __firmware_verify(void *p_drv, am_boot_firmware_verify_info_t *p_verify_info)
{
    am_mm32l073_boot_firmware_flash_dev_t *p_dev = (am_mm32l073_boot_firmware_flash_dev_t *)p_drv;
    uint8_t *p_flash = (uint8_t *)p_dev->firmware_dst_addr;

    uint32_t verify = 0, i = 0;

    if (NULL == p_verify_info) {
        return -AM_ERROR;
    }

    for (i = 0 ; i < p_verify_info->len ; i++) {

        verify += p_flash[i];

    }

    if (verify == p_verify_info->verify_value) {
        return AM_OK;
    } else {
        return -AM_ERROR;
    }
}

/**
 * \brief 固件flash存储初始化
 */
am_boot_firmware_handle_t am_mm32l073_boot_firmware_flash_init (
    am_mm32l073_boot_firmware_flash_dev_t     *p_dev,
    am_mm32l073_boot_firmware_flash_devinfo_t *p_devinfo,
    am_boot_flash_handle_t            flash_handle)
{
    p_dev->firmware_flash_serv.pfn_funcs = &__g_firmware_flash_drv_funcs;
    p_dev->firmware_flash_serv.p_drv     = p_dev;
    p_dev->flash_handle                  = flash_handle;
    p_dev->firmware_dst_addr             = p_devinfo->firmware_dst_addr;
    p_dev->buf_data_size                 = p_devinfo->buf_size;
    p_dev->p_devinfo                     = p_devinfo;

    return &p_dev->firmware_flash_serv;
}
/**
 * \brief 固件flash存储解初始化
 */
void am_mm32l073_boot_firmware_flash_deint(void)
{

}
/* end of file */
