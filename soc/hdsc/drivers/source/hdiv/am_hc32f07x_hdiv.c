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
 * \brief hdiv模块的驱动层实现
 *
 * \internal
 * \par Modification History
 * \endinternal
 */

#include "hw/amhw_hc32f07x_hdiv.h"
#include "am_types.h"
#include "am_bitops.h"
#include "ametal.h"

int am_hc32f07x_hdiv_unsigned(amhw_hc32f07x_hdiv_t *p_hw_hdiv, 
                              amhw_hc32f07x_hdiv_unsigned_t *p_undivd)
{
    if(NULL == p_undivd) {
        return -AM_EINVAL;
    }
    amhw_hc32f07x_hdiv_unsigned_set (p_hw_hdiv);

    p_hw_hdiv->divdend = p_undivd->undividend;
    p_hw_hdiv->divisor = p_undivd->undivisor;

    if(amhw_hc32f07x_hdiv_zerostat_get(p_hw_hdiv) == AM_TRUE){
        return -AM_EINVAL;
    }

    while(amhw_hc32f07x_hdiv_endstat_get(p_hw_hdiv) == AM_FALSE){
        ;
    }

    p_undivd->quotient  = p_hw_hdiv->quotient;
    p_undivd->remainder = p_hw_hdiv->remainder;

    return AM_OK;
}

int am_hc32f07x_hdiv_signed(amhw_hc32f07x_hdiv_t *p_hw_hdiv, 
                            amhw_hc32f07x_hdiv_signed_t *p_divd)
{
    if(NULL == p_divd) {
        return -AM_EINVAL;
    }
    amhw_hc32f07x_hdiv_signed_set (p_hw_hdiv);

    p_hw_hdiv->divdend = p_divd->dividend;
    p_hw_hdiv->divisor = p_divd->divisor;

    if(amhw_hc32f07x_hdiv_zerostat_get(p_hw_hdiv) == AM_TRUE){
        return -AM_EINVAL;
    }

    while(amhw_hc32f07x_hdiv_endstat_get(p_hw_hdiv) == AM_FALSE){
        ;
    }

    p_divd->quotient  = p_hw_hdiv->quotient;
    p_divd->remainder = p_hw_hdiv->remainder;

    return AM_OK;
}

/* end of file */
