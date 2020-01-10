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
 * \brief FTFC Peripheral Access Layer
 *
 *   The Miscellaneous System Control Module (FTFC) contains CPU configuration
 *   registers and on-chip memory controller registers.
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-04  zp, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_HC32_FLASH_H
#define __AMHW_HC32_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"
#include "hc32f460_regbase.h"

/**
 * \addtogroup amhw_hc32f460_if_flash
 * \copydoc amhw_hc32f460_flash.h
 * @{
 */

/**
 * \brief 使用匿名联合体段开始
 * @{
 */

#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)

  /* 默认使能匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/** @} */

typedef struct amhw_hc32f460_flash {

    __IO uint32_t FAPRT;

    __IO uint32_t FSTP;

    __IO uint32_t FRMC;

    __IO uint32_t FWMC;

    __IO uint32_t FSR;

    __IO uint32_t FSCLR;

    __IO uint32_t FITE;

    __IO uint32_t FSWP;

    __IO uint32_t FPMTSW;

    __IO uint32_t FPMTEW;

    uint8_t RESERVED10[40];

    __IO uint32_t UQID1;

    __IO uint32_t UQID2;

    __IO uint32_t UQID3;

    uint8_t RESERVED13[164];

    __IO uint32_t MMF_REMPRT;

    __IO uint32_t MMF_REMCR0;

    __IO uint32_t MMF_REMCR1;

} amhw_hc32f460_flash_t;

#define AM_HC32F460_FLASH    ((amhw_hc32f460_flash_t *)(HC32F460_EFM_BASE))

/**
 * \brief 读FLASH周期
 */
typedef enum {
    AMHW_HC32F460_FLASH_READ_WAITTIME_0 = 0,  /**< \brief  0~24MHz: 00/11: 1个周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_1 ,     /**< \brief 24~48MHz：           01： 2个周期  */
    AMHW_HC32F460_FLASH_READ_WAITTIME_2 ,     /**< \brief 48~72MHz：          10: 3个周期  */
    AMHW_HC32F460_FLASH_READ_WAITTIME_3 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_4 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_5 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_6 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_7 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_8 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_9 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_10 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_11 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_12 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_13 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_14 ,
    AMHW_HC32F460_FLASH_READ_WAITTIME_15 ,
} amhw_hc32f460_flash_read_waittime;

#define __AMHW_HC32F460_EFM_LOCK            (0x00000000u)
#define __AMHW_HC32F460_EFM_UNLOCK          (0x00000001u)
#define __AMHW_HC32F460_EFM_KEY1            (0x0123ul)
#define __AMHW_HC32F460_EFM_KEY2            (0x3210ul)

am_static_inline
void amhw_hc32f460_flash_lock(amhw_hc32f460_flash_t *p_hw_flash)
{
    if (__AMHW_HC32F460_EFM_UNLOCK== p_hw_flash->FAPRT)
    {
        p_hw_flash->FAPRT = __AMHW_HC32F460_EFM_KEY2;
        p_hw_flash->FAPRT = __AMHW_HC32F460_EFM_KEY2;
    }
}

am_static_inline
void amhw_hc32f460_flash_unlock(amhw_hc32f460_flash_t *p_hw_flash)
{
    if (__AMHW_HC32F460_EFM_LOCK== p_hw_flash->FAPRT)
    {
        p_hw_flash->FAPRT = __AMHW_HC32F460_EFM_KEY1;
        p_hw_flash->FAPRT = __AMHW_HC32F460_EFM_KEY2;
    }
}

am_static_inline
void amhw_hc32f460_flash_waitcycle(amhw_hc32f460_flash_t            *p_hw_flash,
                                   amhw_hc32f460_flash_read_waittime waitcycle)
{
    uint32_t reg_value;

    reg_value = p_hw_flash->FRMC;
    p_hw_flash->FRMC = ((reg_value & (0xF << 4)) | (waitcycle << 4));
}


/**
 * \brief 使用匿名联合体段结束
 * @{
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)

  /* 允许匿名联合体使能 */
#elif defined(__GNUC__)

  /* 默认使用匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif
/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32_FLASH_H */

/* end of file */
