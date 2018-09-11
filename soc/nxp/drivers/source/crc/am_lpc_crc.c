/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief CRC drivers implementation
 *
 * The CRC can support follow patterns:
 * 
 * 1. width     = 16;
 *    poly      = 0x1021;
 *    initvalue = can be any value;
 *    refin     = AM_TRUE or AM_FALSE;
 *    refout    = AM_TRUE or AM_FALSE;
 *    xorout    = 0x0000 or 0xFFFF;
 *
 * 2. width     = 16;
 *    poly      = 0x8005;
 *    initvalue = can be any value;
 *    refin     = AM_TRUE or AM_FALSE;
 *    refout    = AM_TRUE or AM_FALSE;
 *    xorout    = 0x0000 or 0xFFFF;
 *
 * 3. width     = 32;
 *    poly      = 0x04C11DB7;
 *    initvalue = can be any value;
 *    refin     = AM_TRUE or AM_FALSE;
 *    refout    = AM_TRUE or AM_FALSE;
 *    xorout    = 0x00000000 or 0xFFFFFFFF;
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#include "am_lpc_crc.h"

/** \bruef Pointer to CRC device */
am_lpc_crc_dev_t *__gp_crc_dev;

/*******************************************************************************
* Functions declaration
*******************************************************************************/

static int __crc_init(void *p_drv, am_crc_pattern_t *p_pattern);

static int __crc_cal(void *p_drv, const uint8_t *p_data, uint32_t nbytes);

static int __crc_final (void *p_drv, uint32_t *p_value);


/** 
 * \brief timer for timing driver functions 
 */
static const struct am_crc_drv_funcs __g_crc_drv_funcs = {
    __crc_init,
    __crc_cal,
    __crc_final,
};

/******************************************************************************/

static int __crc_init (void *p_drv, am_crc_pattern_t *p_pattern)
{
    am_lpc_crc_dev_t *p_dev = (am_lpc_crc_dev_t *)p_drv;
    uint8_t         width;
    uint32_t        poly;
    amhw_lpc_crc_poly_t crc_poly = AMHW_LPC_CRC_POLY_CRC16;
    uint32_t        crc_mode_flags = 0;
    amhw_lpc_crc_t     *p_hw_crc   = NULL;
    
    if (p_pattern == NULL || p_dev == NULL) {
        return -AM_EINVAL;
    }
    
    width           = p_pattern->width;
    poly            = p_pattern->poly;
    p_dev->p_patern = p_pattern;
    
    if ((width != 16 || (poly != 0x1021 && poly != 0x8005)) &&
        (width != 32 || (poly != 0x04C11DB7))) {
             
        return -AM_ENOTSUP;                      /* Not support */
    }
    
    p_hw_crc = (amhw_lpc_crc_t   *)p_dev->p_devinfo->crc_regbase;
        
    if (p_pattern->refin == AM_TRUE) {
        crc_mode_flags |= AMHW_LPC_CRC_MODE_WRDATA_BIT_RVS;
    }
        
    if (p_pattern->refout == AM_TRUE) {
        crc_mode_flags |= AMHW_LPC_CRC_MODE_SUM_BIT_RVS;
    }

    if (width == 16) {
        if (poly == 0x1021) {
            crc_poly = AMHW_LPC_CRC_POLY_CCITT;
        } else if (poly == 0x8005) {
            crc_poly = AMHW_LPC_CRC_POLY_CRC16;
        }

        if (p_pattern->xorout == 0xFFFF) {
            crc_mode_flags |= AMHW_LPC_CRC_MODE_SUM_CMPL;
        }
        
        amhw_lpc_crc_mode_set(p_hw_crc, crc_poly, crc_mode_flags);

        amhw_lpc_crc_seed_set(p_hw_crc, p_pattern->initvalue);
        
        return AM_OK;
    }
    
    if (width == 32) {
        if (poly == 0x04C11DB7) {
            crc_poly = AMHW_LPC_CRC_POLY_CRC32;
        }

        if (p_pattern->xorout == 0xFFFFFFFF) {
            crc_mode_flags |= AMHW_LPC_CRC_MODE_SUM_CMPL;
        }
        
        amhw_lpc_crc_mode_set(p_hw_crc, crc_poly, crc_mode_flags);

        amhw_lpc_crc_seed_set(p_hw_crc, p_pattern->initvalue);
        
        return AM_OK;
    }
    
    return -AM_ENOTSUP;                      /* Not support */
}

static int __crc_cal (void *p_drv, const uint8_t *p_data, uint32_t nbytes)
{
    am_lpc_crc_dev_t *p_dev    = (am_lpc_crc_dev_t *)p_drv;

    amhw_lpc_crc_t   *p_hw_crc = (amhw_lpc_crc_t   *)p_dev->p_devinfo->crc_regbase;
    
    if (p_dev->p_patern == NULL) {
        return -AM_EINVAL;                /* The CRC calculate must after init */
    }
    
    amhw_lpc_crc_data8_write(p_hw_crc,
                             p_data,
                             nbytes);
    
    return AM_OK;
}

static int __crc_final (void *p_drv, uint32_t *p_value)
{
    am_lpc_crc_dev_t *p_dev    = (am_lpc_crc_dev_t *)p_drv;
    amhw_lpc_crc_t   *p_hw_crc = (amhw_lpc_crc_t   *)p_dev->p_devinfo->crc_regbase;
 
    if (p_dev->p_patern == NULL) {
        return -AM_EINVAL;                /* The CRC not initialization */
    }

   *p_value = amhw_lpc_crc_sum_get(p_hw_crc);
    
    p_dev->p_patern = NULL;
    
    return AM_OK;
}

am_crc_handle_t am_lpc_crc_init (am_lpc_crc_dev_t           *p_dev,
                               const am_lpc_crc_devinfo_t *p_devinfo)
{
    if (p_devinfo == NULL || p_dev == NULL) {
        return NULL;
    }
    
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo        = p_devinfo;

    p_dev->p_patern         = NULL;
    
    p_dev->crc_serv.p_funcs = (struct am_crc_drv_funcs *)&__g_crc_drv_funcs;
    p_dev->crc_serv.p_drv   = p_dev; 

    __gp_crc_dev = p_dev;
 
    return &(p_dev->crc_serv);
}

void am_lpc_crc_deinit (void)
{
    am_lpc_crc_dev_t *p_dev = __gp_crc_dev;

    if (p_dev == NULL) {
      return;
    }

    p_dev->crc_serv.p_funcs = NULL;
    p_dev->crc_serv.p_drv   = NULL;
    
    p_dev->p_patern = NULL;

    if (__gp_crc_dev->p_devinfo->pfn_plfm_deinit) {
        __gp_crc_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
