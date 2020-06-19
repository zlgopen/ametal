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
 * \brief Flash模块的驱动层实现
 *
 * \internal
 * \par Modification History
 * - 1.00 20-06-17  cds, first implementation
 * \endinternal
 */

#include "hw/amhw_hc32f460_flash.h"
#include "am_types.h"
#include "am_bitops.h"
#include "am_int.h"
#include "ametal.h"

/*******************************************************************************
  defines
*******************************************************************************/
#define __AM_HC32F460_FLASH_START_ADDR   0x0
#define __AM_HC32F460_FLASH_END_ADDR     0x7FFDF

/******************************************************************************
*   函数定义
******************************************************************************/

/**
 * \brief Flash模块初始化
 *
 * \param[in] p_hw_gpio 指向FLASH寄存器块的指针
 *
 * \return 无
 */
void am_hc32f460_flash_init (amhw_hc32f460_flash_t *p_hw_flash)
{
    /* 使能FLASH */
    amhw_hc32f460_flash_unlock(p_hw_flash);
    amhw_hc32f460_flash_enable(p_hw_flash, AM_TRUE);
    amhw_hc32f460_flash_lock(p_hw_flash);
}

/**
 * \brief 擦除扇区
 *
 * \param[in] p_hw_gpio  指向FLASH寄存器块的指针
 * \param[in] start_addr 扇区的起始地址
 *
 * \return > 0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_hc32f460_flash_sector_erase (amhw_hc32f460_flash_t *p_hw_flash,
                                        uint32_t               start_addr)
{
    amhw_hc32f460_flash_unlock(p_hw_flash);

    amhw_hc32f460_flash_fwmc_pemode_enable(p_hw_flash);

    amhw_hc32f460_flash_fwmc_pemod_set(p_hw_flash, AMHW_HC32F460_FLASH_ERASE_PGM_MD_SECTORERASE);

    *(volatile uint32_t *)start_addr = 0x12345678;

    while(!(amhw_hc32f460_flash_efm_status_get(p_hw_flash) & AMHW_HC32F460_EFM_STATUS_RDY));

    amhw_hc32f460_flash_efm_status_clr(p_hw_flash, AMHW_HC32F460_EFM_STATUS_OPTEND);

    amhw_hc32f460_flash_lock(p_hw_flash);

    return AM_OK;

}

/**
 * \brief 全擦除
 *
 * \param[in] p_hw_gpio  指向FLASH寄存器块的指针
 *
 * \return 执行结果
 */
int32_t am_hc32f460_flash_all_sector_erase (amhw_hc32f460_flash_t *p_hw_flash)
{
    amhw_hc32f460_flash_unlock(p_hw_flash);

    amhw_hc32f460_flash_fwmc_pemode_enable(p_hw_flash);

    amhw_hc32f460_flash_fwmc_pemod_set(p_hw_flash, AMHW_HC32F460_FLASH_ERASE_PGM_MD_MASSERASE);

    *(volatile uint32_t *)__AM_HC32F460_FLASH_START_ADDR = 0x12345678;

    while(!(amhw_hc32f460_flash_efm_status_get(p_hw_flash) & AMHW_HC32F460_EFM_STATUS_RDY));

    amhw_hc32f460_flash_efm_status_clr(p_hw_flash, AMHW_HC32F460_EFM_STATUS_OPTEND);

    amhw_hc32f460_flash_lock(p_hw_flash);

    return AM_OK;

}

/**
 * \brief 对flash编程
 *
 * \param[in] p_hw_gpio 指向FLASH寄存器块的指针
 * \param[in] dst_addr  写入到flash的起始地址
 * \param[in] p_src     要写入到flash中的数据的起始地址
 * \param[in] size      写入字(32bit)的个数
 *
 * \retval 0 实际成功写入的字数
 */
int32_t am_hc32f460_flash_sector_program (amhw_hc32f460_flash_t *p_hw_flash,
                                          uint32_t               dst_addr,
                                          uint32_t              *p_src,
                                          uint32_t               size)
{
    if ((dst_addr & 0x3) != 0) {
        return -AM_EINVAL;
    }

    if ((dst_addr + (size * 4)) > (__AM_HC32F460_FLASH_END_ADDR & 0xFFFFFFFC)) {
        return -AM_EINVAL;
    }

    uint32_t i = 0;

    amhw_hc32f460_flash_unlock(p_hw_flash);

    amhw_hc32f460_flash_fwmc_pemode_enable(p_hw_flash);

    amhw_hc32f460_flash_fwmc_pemod_set(p_hw_flash, AMHW_HC32F460_FLASH_ERASE_PGM_MD_SINGLEPROGRAM);

    /** 对flash编程 */
    for (i = 0; i < size; i++) {

        /** 写入一个字的数据 */
        *(uint32_t *)(dst_addr + i * 4)     = p_src[i];

        /** 等待写入完成 */
        while (!(amhw_hc32f460_flash_efm_status_get(p_hw_flash) & AMHW_HC32F460_EFM_STATUS_RDY)) {
           ;
        }
    }

    amhw_hc32f460_flash_efm_status_clr(p_hw_flash, AMHW_HC32F460_EFM_STATUS_OPTEND);

    amhw_hc32f460_flash_lock(p_hw_flash);

    return i;
}

/**
 * \brief 开启EFM寄存器写保护
 *
 * \param[in] p_hw_gpio 指向FLASH寄存器块的指针
 *
 * \retval 无
 */
 void am_hc32f460_flash_lock(amhw_hc32f460_flash_t *p_hw_flash)
{
    amhw_hc32f460_flash_lock(p_hw_flash);
}

 /**
  * \brief 关闭EFM寄存器写保护
  *
  * \param[in] p_hw_gpio 指向FLASH寄存器块的指针
  *
  * \retval 无
  */
 void am_hc32f460_flash_unlock(amhw_hc32f460_flash_t *p_hw_flash)
{
    amhw_hc32f460_flash_unlock(p_hw_flash);
}


 /**
  * \brief 设置FLASH读插入的等待周期
  *
  * \param[in] p_hw_gpio 指向FLASH寄存器块的指针
  * \param[in] waitcycle FLASH读插入的等待周期
  *
  * \retval 无
  */
void am_hc32f460_flash_waitcycle(amhw_hc32f460_flash_t *p_hw_flash,
                                 amhw_hc32f460_flash_read_waittime waitcycle)
{
    amhw_hc32f460_flash_waitcycle(p_hw_flash, waitcycle);
}



/* end of file */
