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
 * \brief bootloader flash drivers implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-15  yrh, first implementation
 * \endinternal
 */
#include "am_zlg237_boot_flash.h"
#include "am_int.h"
#include "am_zlg237_flash.h"
#include "am_boot_flash.h"

static int __boot_flash_erase_region(void     *p_drv,
                                     uint32_t  start_addr,
                                     uint32_t  length);
static int __boot_flash_program(void      *p_drv,
                                uint32_t   dst_addr,
                                void      *p_src,
                                uint32_t   size);
static void __boot_flash_info_get(void *p_drv, am_boot_flash_info_t **pp_flash_info);
/**
 * \brief BootLoader flash标准层接口实现
 */
static struct am_boot_flash_drv_funcs __g_flash_drv_funcs = {
    __boot_flash_erase_region,
    __boot_flash_program,
    __boot_flash_info_get,
    NULL,
};
/**
 * \brief flash区域擦除
 */
static int __boot_flash_erase_region(void     *p_drv,
                                     uint32_t  start_addr,
                                     uint32_t  length)
{
    am_zlg237_boot_flash_dev_t *p_dev = (am_zlg237_boot_flash_dev_t *)p_drv;
    uint32_t page_size = p_dev->p_devinfo->base_info.flash_page_size;

    uint32_t erase_page_count = 0;
    if((length & (page_size - 1)) != 0) {
        erase_page_count = length / page_size + 1;
    } else {
        erase_page_count = length / page_size;
    }

     uint32_t flash_addr_end = p_dev->p_devinfo->base_info.flash_start_addr+ \
         p_dev->p_devinfo->base_info.flash_size;

     if ((start_addr + length) > flash_addr_end) {
         return AM_ERROR;
     }

     int  k, i, ret;
     for (i = 0 ; i < erase_page_count; i++) {

         k = am_int_cpu_lock();

         ret = am_zlg237_flash_page_erase(
             (amhw_zlg237_flash_t *)p_dev->amhw_zlg237_flash, i * page_size + start_addr);

         am_int_cpu_unlock(k);

         if (0 != ret) {
             break;
         }
     }

     if (i < erase_page_count) {
         return i;
     }

     return AM_OK;
}

/**
 * \brief 对flash的写操作
 */
static int __boot_flash_program(void      *p_drv,
                                uint32_t   dst_addr,
                                void      *p_src,
                                uint32_t   size)
{
    am_zlg237_boot_flash_dev_t *p_dev = (am_zlg237_boot_flash_dev_t *)p_drv;
    if(p_src == NULL) {
        return AM_ERROR;
    }

    if(size & (sizeof(uint32_t) - 1)) {
        return AM_ERROR;
    }
    uint32_t key, i = 0, ret, program_size;
    uint32_t flash_addr_end = p_dev->p_devinfo->base_info.flash_start_addr + \
    p_dev->p_devinfo->base_info.flash_size;

    if ((dst_addr + size) > flash_addr_end) {
        return AM_ERROR;
    }

    uint32_t page_size = p_dev->p_devinfo->base_info.flash_page_size;

    uint32_t program_page_count = size / page_size;
    uint32_t left_size = size & (page_size - 1);

    program_size = page_size / sizeof(uint32_t);

    for(i = 0; i < program_page_count; i++) {

        key = am_int_cpu_lock();
        ret = am_zlg237_flash_sector_program((amhw_zlg237_flash_t *)p_dev->amhw_zlg237_flash,
                                              dst_addr + i * page_size,
                                             (uint32_t *)((uint32_t)p_src+ i * page_size),
                                              program_size);
        am_int_cpu_unlock(key);
        if(ret != program_size) {
            return AM_ERROR;
        }
    }

    if(left_size != 0) {

        program_size = left_size / sizeof(uint32_t);

        key = am_int_cpu_lock();
        ret = am_zlg237_flash_sector_program((amhw_zlg237_flash_t *)p_dev->amhw_zlg237_flash,
                                              dst_addr + i * page_size,
                                             (uint32_t *)((uint32_t)p_src+ i * page_size),
                                              program_size);
        am_int_cpu_unlock(key);
        if(ret != program_size) {
            return AM_ERROR;
        }
    }
    return AM_OK;
}

/**
 * \brief 获取flash的信息
 */
static void __boot_flash_info_get(void *p_drv, am_boot_flash_info_t **pp_flash_info)
{
    am_zlg237_boot_flash_dev_t *p_dev = (am_zlg237_boot_flash_dev_t *)p_drv;

    *pp_flash_info = &p_dev->p_devinfo->base_info;
}

/**
 * \brief BootLoader flash初始化函数
 */
am_boot_flash_handle_t am_zlg237_boot_flash_init(am_zlg237_boot_flash_dev_t     *p_dev,
                                                 am_zlg237_boot_flash_devinfo_t *p_devinfo)
{
    if(p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }
    p_dev->amhw_zlg237_flash = (amhw_zlg237_flash_t *)p_devinfo->flash_reg_rebese;
    p_dev->flash_serv.p_funcs = &__g_flash_drv_funcs;
    p_dev->flash_serv.p_drv   = p_dev;

    p_dev->p_devinfo = p_devinfo;

    am_zlg237_flash_init(p_dev->amhw_zlg237_flash);

    return &p_dev->flash_serv;
}

/* end of file */

