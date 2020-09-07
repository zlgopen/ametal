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
 * \brief SDIO驱动，服务SDIO标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-04-29  cds, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_SDIOC_H
#define __AM_HC32F460_SDIOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_int.h"
#include "am_sdio.h"
#include "am_list.h"
#include "am_wait.h"
#include "am_softimer.h"

#include "am_vdebug.h"
#include "hw/amhw_hc32f460_sdioc.h"

/**
 * \addtogroup am_hc32f460_if_sdioc
 * \copydoc am_hc32f460_sdioc.h
 * @{
 */

/**
 * \brief SDIOC设备信息参数结构体
 */
typedef struct am_hc32f460_sdioc_devinfo {

    /** \brief SDIO寄存器基地址 */
    uint32_t                         regbase;

    /** \brief SDIO设备ID */
    uint8_t                          dev_id;

    /** \brief SDIO中断号 */
    uint32_t                         inum;

    /** \brief SDIO时钟ID */
    int                              clk_num;

    /** \brief SDIO速率模式 */
    amhw_hc32f460_sdioc_speed_mode_t speed_mode;

    /** \brief 平台初始化函数 */
    void                           (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void                           (*pfn_plfm_deinit)(void);

} am_hc32f460_sdioc_devinfo_t;

/**
 * \brief SDIO设备结构体
 */
typedef struct am_hc32f460_sdioc_dev {

    /** \brief SDIO标准服务  */
    am_sdio_serv_t                     sdio_serv;

    /** \brief 指向SDIO设备信息  */
    const am_hc32f460_sdioc_devinfo_t *p_devinfo;

    /** \brief wait等待  */
    am_wait_t                          wait;

    /** \brief 保存普通中断状态  */
    uint16_t                           int_normal_status;

    /** \brief 保存错误中断状态  */
    uint16_t                           int_err_status;

    /** \brief 是否检查应答中的CRC校验码  */
    am_bool_t                          cmd_crc_enable;

} am_hc32f460_sdioc_dev_t;

/**
 * \brief SDIO初始化
 *
 * \param[in] p_dev     : 指向SDIO设备结构体的指针
 * \param[in] p_devinfo : 指向SDIO设备信息结构体的指针
 *
 * \return SDIO标准服务操作句柄
 */
am_sdio_handle_t am_hc32f460_sdioc_init (am_hc32f460_sdioc_dev_t           *p_dev,
                                         const am_hc32f460_sdioc_devinfo_t *p_devinfo);

/**
 * \brief 解除SDIO初始化
 *
 * \param[in] handle : 与从设备关联的SDIO标准服务操作句柄
 *
 * \return 无
 */
void am_hc32f460_sdioc_deinit (am_sdio_handle_t handle);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_SDIOC_H */

/* end of file */
