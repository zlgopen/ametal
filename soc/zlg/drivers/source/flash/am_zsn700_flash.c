/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
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
 * - 1.00 20-03-18  yrz, first implementation
 * \endinternal
 */

#include "am_zsn700_flash.h"
#include "am_types.h"
#include "am_bitops.h"
#include "am_int.h"
#include "ametal.h"


/*******************************************************************************
  defines
*******************************************************************************/

#define AM_ZSN700_FLASH_END_ADDR      (FLASH_SIZE - 1)

#define AM_ZSN700_FLASH_BYPASS(p_hw)  amhw_zsn700_flash_key_set(p_hw, 0x5A5A);\
                                      amhw_zsn700_flash_key_set(p_hw, 0xA5A5)

#define AM_ZSN700_FLASH_TIMEOUT_INIT  (0xFFu)
#define AM_ZSN700_FLASH_TIMEOUT_PGM   (0xFFu)
#define AM_ZSN700_FLASH_TIMEOUT_ERASE (0xFFu)
#define AM_ZSN700_FLASH_LOCK_ALL      (0x00u)
#define AM_ZSN700_FLASH_UNLOCK_ALL    (0xFFFFFFFFu)

am_local const uint32_t __g_zsn700_flash_pcgtimer_4mhz[] = \
    {0x20u, 0x17u, 0x1Bu, 0x4650u, 0x222E0u, 0x18u, 0xF0u, 0x3E8u};

/******************************************************************************/

/**
 * \brief 设置所有扇区写保护
 */
am_local void __zsn700_flash_lockall(amhw_zsn700_flash_t *p_hw_flash)
{
    AM_ZSN700_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock0 = AM_ZSN700_FLASH_LOCK_ALL;
    AM_ZSN700_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock0 = AM_ZSN700_FLASH_LOCK_ALL;
    AM_ZSN700_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock2 = AM_ZSN700_FLASH_LOCK_ALL;
    AM_ZSN700_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock3 = AM_ZSN700_FLASH_LOCK_ALL;
}

/**
 * \brief 取消所有扇区写保护
 */
am_local void __zsn700_flash_unlockall(amhw_zsn700_flash_t *p_hw_flash)
{
    AM_ZSN700_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock0 = AM_ZSN700_FLASH_UNLOCK_ALL;
    AM_ZSN700_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock1 = AM_ZSN700_FLASH_UNLOCK_ALL;
    AM_ZSN700_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock2 = AM_ZSN700_FLASH_UNLOCK_ALL;
    AM_ZSN700_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock3 = AM_ZSN700_FLASH_UNLOCK_ALL;
}

/**
 * \brief 设置指定扇区写保护
 */
am_local void __zsn700_flash_locksector(amhw_zsn700_flash_t *p_hw_flash,
                                        uint32_t             addr)
{
    uint8_t     slock;
    uint32_t    bit;

    AM_ZSN700_FLASH_BYPASS(p_hw_flash);

    slock = addr >> 16;
    bit   = ~(1 << ((addr & 0xFFFF) >> 11));

    switch (slock) {

    case 0:
        p_hw_flash->slock0 &= bit;
        break;

    case 1:
        p_hw_flash->slock1 &= bit;
        break;

    case 2:
        p_hw_flash->slock2 &= bit;
        break;

    case 3:
        p_hw_flash->slock3 &= bit;
        break;

    default:
        break;
    }
}

/**
 * \brief 取消指定扇区写保护
 */
am_local void __zsn700_flash_unlocksector(amhw_zsn700_flash_t *p_hw_flash,
                                          uint32_t             addr)
{
    uint8_t     slock;
    uint32_t    bit;

    AM_ZSN700_FLASH_BYPASS(p_hw_flash);

    slock = addr >> 16;
    bit   = 1 << ((addr & 0xFFFF) >> 11);

    switch (slock) {

    case 0:
        p_hw_flash->slock0 |= bit;
        break;

    case 1:
        p_hw_flash->slock1 |= bit;
        break;

    case 2:
        p_hw_flash->slock2 |= bit;
        break;

    case 3:
        p_hw_flash->slock3 |= bit;
        break;

    default:
        break;
    }
}

/**
 * \brief 写一个字的数据
 */
am_local int32_t __zsn700_flash_writeword(amhw_zsn700_flash_t  *p_hw_flash,
                                          uint32_t              addr,
                                          uint32_t              data)
{
    volatile uint32_t timeout = AM_ZSN700_FLASH_TIMEOUT_PGM;

    /* busy? */
    timeout = AM_ZSN700_FLASH_TIMEOUT_PGM;
    while (AM_TRUE == amhw_zsn700_flash_busy_check(p_hw_flash)) {
        if(0 == timeout--) {
            return -AM_EAGAIN;
        }
    }

    __zsn700_flash_unlocksector(p_hw_flash, addr);

    /* set op */
    timeout = AM_ZSN700_FLASH_TIMEOUT_PGM;
    while(AMHW_ZSN700_FLASH_WRITE != amhw_zsn700_flash_opt_get(p_hw_flash)) {
        if(timeout--) {
            amhw_zsn700_flash_opt_set(p_hw_flash, AMHW_ZSN700_FLASH_WRITE);
        } else {
            return -AM_EAGAIN;
        }
    }

    /* write data */
    *((volatile uint32_t*)addr) = data;

    /* busy? */
    timeout = AM_ZSN700_FLASH_TIMEOUT_PGM;
    while (AM_TRUE == amhw_zsn700_flash_busy_check(p_hw_flash)) {
        if(0 == timeout--) {
            return -AM_EAGAIN;
        }
    }

    __zsn700_flash_locksector(p_hw_flash, addr);

    return AM_OK;
}

/******************************************************************************/

/**
 * \brief 设置读flash周期
 */
int32_t am_zsn700_flash_waitcycle(amhw_zsn700_flash_t              *p_hw_flash,
                                  amhw_zsn700_flash_read_waittime   waitcycle)
{
    AM_ZSN700_FLASH_BYPASS(p_hw_flash);
    amhw_zsn700_flash_read_waittime_set(p_hw_flash, waitcycle);

    return AM_OK;
}

/**
 * \brief 擦除扇区
 */
int32_t am_zsn700_flash_sector_erase (amhw_zsn700_flash_t *p_hw_flash,
                                      uint32_t             addr)
{
    volatile uint32_t  timeout = AM_ZSN700_FLASH_TIMEOUT_ERASE;

    if (AM_ZSN700_FLASH_END_ADDR < addr) {
        return -AM_EINVAL;
    }

    /* busy? */
    timeout = AM_ZSN700_FLASH_TIMEOUT_ERASE;
    while (AM_TRUE == amhw_zsn700_flash_busy_check(p_hw_flash)) {
        if(0 == timeout--) {
            return -AM_EAGAIN;
        }
    }

    __zsn700_flash_unlockall(p_hw_flash);

    /* set op */
    timeout = AM_ZSN700_FLASH_TIMEOUT_ERASE;
    while(AMHW_ZSN700_FLASH_SECTOR_ERASE != \
            amhw_zsn700_flash_opt_get(p_hw_flash)) {
        if(timeout--) {
            amhw_zsn700_flash_opt_set(p_hw_flash,
                                      AMHW_ZSN700_FLASH_SECTOR_ERASE);
        } else {
            return -AM_EAGAIN;
        }
    }

    /* write data */
    *((volatile uint8_t*)addr) = 0;

    /* busy? */
    timeout = AM_ZSN700_FLASH_TIMEOUT_ERASE;
    while (AM_TRUE == amhw_zsn700_flash_busy_check(p_hw_flash)) {
        if(0 == timeout--) {
            return -AM_EAGAIN;
        }
    }

    __zsn700_flash_lockall(p_hw_flash);

    return AM_OK;
}

/**
 * \brief 对扇区编程或部分扇区编程
 */
int32_t am_zsn700_flash_sector_program (amhw_zsn700_flash_t *p_hw_flash,
                                        uint32_t             dst_addr,
                                        uint32_t            *p_src,
                                        uint32_t             size)
{
    uint32_t i = 0;

    if (AM_ZSN700_FLASH_END_ADDR < (dst_addr + size)) {
        return -AM_EINVAL;
    }

    for(i = 0; i < size; i++) {
        __zsn700_flash_writeword(p_hw_flash, dst_addr + i * 4, p_src[i]);
    }

    for (i = 0; i < size; i++) {
      if (p_src[i] != *(uint32_t *)(dst_addr + i * 4)) {
          break;
      }
    }
    return i;
}

/**
 * \brief 全片擦除
 */
int32_t am_zsn700_flash_all_sector_erase (amhw_zsn700_flash_t *p_hw_flash)
{
    volatile uint32_t timeout = AM_ZSN700_FLASH_TIMEOUT_ERASE;

    /* busy? */
    timeout = AM_ZSN700_FLASH_TIMEOUT_ERASE;
    while (AM_TRUE == amhw_zsn700_flash_busy_check(p_hw_flash)) {
        if(0 == timeout--) {
            return -AM_EAGAIN;
        }
    }

    __zsn700_flash_unlockall(p_hw_flash);

    /* set op */
    timeout = AM_ZSN700_FLASH_TIMEOUT_ERASE;
    while(AMHW_ZSN700_FLASH_CHIP_ERASE != \
            amhw_zsn700_flash_opt_get(p_hw_flash)) {
        if(timeout--) {
            amhw_zsn700_flash_opt_set(p_hw_flash, AMHW_ZSN700_FLASH_CHIP_ERASE);
        } else {
            return -AM_EAGAIN;
        }
    }

    /* write data */
    *((volatile uint8_t*)0) = 0;

    /* busy? */
    timeout = AM_ZSN700_FLASH_TIMEOUT_ERASE;
    while (AM_TRUE == amhw_zsn700_flash_busy_check(p_hw_flash)) {
        if(0 == timeout--) {
            return -AM_EAGAIN;
        }
    }

    __zsn700_flash_lockall(p_hw_flash);

    return AM_OK;
}

/**
 * \brief Flash模块初始化
 */
int32_t am_zsn700_flash_init(amhw_zsn700_flash_t   *p_hw_flash,
                             uint8_t                freqcfg,
                             am_bool_t              dpstb_able)
{
    uint32_t           index        = 0;
    volatile uint32_t  timeout      = AM_ZSN700_FLASH_TIMEOUT_INIT;
    uint32_t           prgtimer[8]  = {0};
    volatile uint32_t *pprgtimerreg = (volatile uint32_t*)p_hw_flash;

    if ((1  != freqcfg) &&
        (2  != freqcfg) &&
        (4  != freqcfg) &&
        (6  != freqcfg) &&
        (8  != freqcfg) &&
        (12 != freqcfg)) {
        return -AM_EINVAL;
    }

    /* dpstb 使能控制 */
    AM_BITS_SET(p_hw_flash->cr, 9, 1, dpstb_able);

    /* flash时间参数配置值计算 */
    for (index = 0; index < 8; index++) {
        prgtimer[index] = freqcfg * __g_zsn700_flash_pcgtimer_4mhz[index];
    }

    /* flash时间参数寄存器配置 */
    for (index = 0; index < 8; index++) {
        timeout = AM_ZSN700_FLASH_TIMEOUT_INIT;
        while(pprgtimerreg[index] != prgtimer[index]) {
            if(timeout--) {
                AM_ZSN700_FLASH_BYPASS(p_hw_flash);
                pprgtimerreg[index] = prgtimer[index];
            } else {
                return AM_ERROR;
            }
        }
    }

    return AM_OK;
}


/* end of file */
