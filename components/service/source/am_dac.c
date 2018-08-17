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
 * \brief DAC implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-04-27  tee, first implementation.
 * \endinternal
 */
#include "am_dac.h"


/*******************************************************************************
  public functions
*******************************************************************************/

/**
 * \brief 设置DAC通道的电压值（单位：mV）
 *
 * \param[in] handle : DAC标准服务操作句柄
 * \param[in] chan   : DAC通道号
 *
 * \retval   AM_OK      操作成功
 * \retval  -AM_ENXIO   DAC通道号不存在
 */
int am_dac_mv_set (am_dac_handle_t handle, int chan, uint32_t mv)
{
    uint32_t bits = am_dac_bits_get(handle);
    uint32_t vref = am_dac_vref_get(handle);
    
    
    return am_dac_val_set(handle,chan, mv * (1 << bits) / vref);
}

/* end of file */
