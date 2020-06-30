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
 * \brief HASH驱动，服务HASH标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-05-22
 * \endinternal
 */
#ifndef __AM_HC32F460_HASH_H
#define __AM_HC32F460_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"



/**
 * \addtogroup am_hc32f460_if_hash
 * \copydoc am_hc32f460_hash.h
 * @{
 */
/**
 * \brief aes设备信息结构体
 */
typedef struct am_hc32f460_hash_info {

    uint32_t    reg_base;  /**< \brief HASH寄存器块基地址 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);
}am_hc32f460_hash_devinfo_t;

/**
 * \brief HASH设备实例
 */
typedef struct am_hc32f460_hash_dev {

    /** \brief 指向HASH设备信息的指针 */
    const am_hc32f460_hash_devinfo_t *p_devinfo;

} am_hc32f460_hash_dev_t;

/** \brief hash标准服务操作句柄类型定义 */
typedef am_hc32f460_hash_dev_t *am_hash_handle_t;

/**
 * \brief HASH 初始化
 *
 * \param[in] p_dev     :HASH设备初始化实例
 * \param[in] p_devinfo :HASH设备信息初始化实例
 *
 * \retval :无
 *
 */
am_hash_handle_t am_hc32f460_hash_init(am_hc32f460_hash_dev_t           *p_dev,
                                       const am_hc32f460_hash_devinfo_t *p_devinfo);

/**
 * \brief HASH 去始化
 *
 * \param[in] handle     :HASH标准设备服务句柄
 *
 * \retval :无
 *
 */
void am_hc32f460_hash_deinit (am_hash_handle_t handle);

/**
 * \brief hash 加密
 *
 * \param[in] : handle 标准HASH服务句柄
 * \param[in] : p_hash_data 原始消息
 * \param[in] : data_length 原始消息长度
 * \param[in] : p_hash_digest 存放计算后的消息摘要的缓冲区
 *
 * \retval : AM_ERROR 加密运算成功
 *           AM_ERROR 加密运算失败
 */
uint32_t am_hc32f460_hash_compute (am_hash_handle_t handle,
                                   uint8_t         *p_hash_data,
                                   uint32_t         data_length,
                                   uint8_t         *p_hash_digest);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_HASH_H */

/* end of file */
