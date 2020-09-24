/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg116.cn/
*******************************************************************************/

/**
 * \file
 * \brief device操作接口
 * \internal
 * \par Modification History
 * - 1.00 20-09-22
 * \endinternal
 */

#ifndef __AMHW_ZLG116_DEVICE_H
#define __AMHW_ZLG116_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"



/**
 * \brief device - 寄存器组
 */
typedef struct amhw_zlg116_device {
    uint16_t f_size;        /**< \brief 闪存容量寄存器 */
    uint16_t reset[3];
    uint16_t uid[6];        /**< \brief 产品唯一标识寄存器 (96 位) */     
} amhw_zlg116_device_t;



/**
 * \brief 获取UID
 *
 * \param[in] p_hw_dac : 指向device寄存器组基地址指针
 * \param[in] p_uid : 指向UID被存储的位置，可定义类型为uint16_t [6];
 * \retval : 无
 */
am_static_inline
void amhw_zlg116_get_uid (amhw_zlg116_device_t *p_hw_device, uint16_t *p_uid)
{
    p_uid[0] = (uint16_t)(p_hw_device->uid[0]);
    p_uid[1] = (uint16_t)(p_hw_device->uid[1]);
    p_uid[2] = (uint16_t)(p_hw_device->uid[2]);
    p_uid[3] = (uint16_t)(p_hw_device->uid[3]);
    p_uid[4] = (uint16_t)(p_hw_device->uid[4]);
    p_uid[5] = (uint16_t)(p_hw_device->uid[5]);
}




#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZLG116_DEVICE_H */

/* end of file */
