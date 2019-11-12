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
 * \brief CRC驱动，服务CRC标准接口
 *
 * 只能支持4字节对齐的序列计算
 * 即： 序列的字节数 % 4 == 0
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-20
 * \endinternal
 */

#ifndef  __AM_HC32_CRC_H
#define  __AM_HC32_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_crc.h"

#include "hw/amhw_hc32_crc.h"

/**
 * \addtogroup am_hc32_if_crc
 * \copydoc am_hc32_crc.h
 * @{
 */

#define AM_HC32_CRC_16  0 /**< \brief CRC-16 校验方式*/
#define AM_HC32_CRC_32  1 /**< \brief CRC-32 校验方式 */

/**
 * \brief CRC设备信息
 */
typedef struct am_hc32_crc_devinfo {

    /** \brief CRC寄存器块的基地址 */
    uint32_t   crc_reg_base;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_hc32_crc_devinfo_t;

/**
 * \brief CRC设备实例
 */
typedef struct am_hc32_crc_dev {

    /** \brief CRC标准服务 */
    am_crc_serv_t                  crc_serve;

    /** \brief 指向CRC设备信息的指针 */
    const am_hc32_crc_devinfo_t *p_devinfo;

    /** \brief 指向CRC模型的指针 */
    am_crc_pattern_t              *p_pattern;

} am_hc32_crc_dev_t;

/**
 * \brief CRC初始化
 *
 * \param[in] p_dev     : 指向CRC设备的指针
 * \param[in] p_devinfo : 指向CRC设备信息的指针
 *
 * \return CRC标准服务操作句柄，如果为 NULL，表明初始化失败
 */
am_crc_handle_t am_hc32_crc_init (am_hc32_crc_dev_t           *p_dev,
                                    const am_hc32_crc_devinfo_t *p_devinfo);

/**
 * \brief CRC去初始化
 *
 * \param[in] handle : am_hc32_crc_init() 初始化函数获得的CRC服务句柄
 *
 * \return 无
 */
void am_hc32_crc_deinit (am_crc_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32_CRC_H */

/* end of file */
