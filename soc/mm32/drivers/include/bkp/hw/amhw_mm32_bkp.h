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
 * \brief 备份寄存器接口
 *
 * \internal
 * \par History
 * - 1.00 17-04-17  nwt, first implementation
 * \endinternal
 */

#ifndef __AMHW_MM32_BKP_H
#define __AMHW_MM32_BKP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup amhw_mm32_if_bkp
 * \copydoc amhw_mm32_bkp.h
 * @{
 */

/** \brief CSR register CTE bit mask */
#define AMHW_BKP_CSR_CTE_SET       ((uint16_t)0x0001)

/** \brief CSR register CTI bit mask */
#define AMHW_BKP_CSR_CTI_SET       ((uint16_t)0x0002)

/**
  * \brief 系统控制寄存器块结构体
  */
typedef struct amhw_mm32_bkp {
    __I  uint32_t reserve;        /**< \brief 保留 */
    __IO uint32_t reserve_dr[10]; /**< \brief 备份区域数据寄存器.(保留) */
    __IO uint32_t rtc_cr;         /**< \brief rtc控制寄存器 */
    __IO uint32_t cr;             /**< \brief BKP控制寄存器 */
    __IO uint32_t csr;            /**< \brief 时钟监控状态寄存器 */
    __I  uint32_t reserve1[6];    /**< \brief 保留 */
    __IO uint32_t dr[10];         /**< \brief 备份区域数据寄存器, offest 0x50 */
} amhw_mm32_bkp_t;

/**
 * \brief Clears Tamper Pin Event pending flag.
 *
 * \param[in] p_hw_bkp : 指向系统配置寄存器块的指针
 * \param[in] flag     : see AMHW_BKP_CSR_CTE_SET or AMHW_BKP_CSR_CTI_SET
 *
 * \return None
 */
am_static_inline
void amhw_mm32_bkp_clear_flag (amhw_mm32_bkp_t *p_hw_bkp, uint32_t flag)
{
    /* Set CTE bit to clear Tamper Pin Event flag */
    p_hw_bkp->csr |= flag;
}

/**
 * \brief 写备份区域数据寄存器
 *
 * \param[in] p_hw_bkp ：指向系统配置寄存器块的指针
 * \param[in] index    : 备份区域数据寄存器索引，值为 0 ~ 9
 * \param[in] value    : 写入备份区或数据寄存器的值，值为 0 ~ 65536
 *
 * \return 无
 */
am_static_inline
void amhw_mm32_bkp_dr_write (amhw_mm32_bkp_t    *p_hw_bkp,
                            uint8_t            index,
                            uint16_t           value)
{
    p_hw_bkp->dr[index] = value;
}

/**
 * \brief 写备份区域数据寄存器
 *
 * \param[in] index    : 备份区域数据寄存器索引，值为 0 ~ 9
 * \param[in] p_hw_bkp ：指向系统配置寄存器块的指针
 *
 * \return 返回对应备份寄存器的值
 */
am_static_inline
uint32_t amhw_mm32_bkp_dr_read (amhw_mm32_bkp_t *p_hw_bkp, uint8_t index)
{
    return (p_hw_bkp->dr[index]);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_MM32_BKP_H */

/* end of file */
