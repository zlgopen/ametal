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
 * \brief CLKTRIM��׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-29
 * \endinternal
 */

#ifndef  __AM_ZSL42x_CLKTRIM_H
#define  __AM_ZSL42x_CLKTRIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "hw/amhw_zsl42x_clktrim.h"

/**
 * \addtogroup am_zsl42x_if_clktrim
 * \copydoc am_zsl42x_clktrim.h
 * @{
 */

/**
 * \brief CLKTRIM���ģʽ����
 *
 * \param[in] rcntval    : �ο���������ֵ
 * \param[in] rcntval    : У׼���������ֵ
 * \param[in] refclk_sel : �ο�ʱ��ѡ��
 * \param[in] calclk_sel : �����ʱ��ѡ��
 *
 * \return <0�������ʱ��ʧЧ
 *        AM_OK����������
 */
uint8_t am_zsl42x_clktrim_monitor (uint32_t                     rcntval,
                                   uint32_t                     ccntval,
                                   amhw_clktrim_refclk_sel_t    refclk_sel,
                                   amhw_clktrim_calclk_sel_t    calclk_sel);

/**
 * \brief CLKTRIMУ׼ģʽ����
 *
 * \param[in] rcntval    : �ο���������ֵ
 * \param[in] refclk_sel : �ο�ʱ��ѡ��
 * \param[in] calclk_sel : ��У׼ʱ��ѡ��
 *
 * \return ��У׼ʱ�Ӽ�������ֵ
 */
uint32_t am_zsl42x_clktrim_calibrate (uint32_t                     rcntval,
                                      amhw_clktrim_refclk_sel_t    refclk_sel,
                                      amhw_clktrim_calclk_sel_t    calclk_sel);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42x_CLKTRIM_H */

/* end of file */