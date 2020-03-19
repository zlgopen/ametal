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
 * \brief OPA驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-13
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32.h"
#include "am_delay.h"
#include "hc32_periph_map.h"
#include "am_hc32x3x_opa.h"
#include "hw/amhw_hc32x3x_opa.h"

/**
 * \brief OPA 初始化
 */
am_opa_handle_t am_hc32_opa_init(am_hc32_opa_dev_t           *p_dev,
                                 const am_hc32_opa_devinfo_t *p_devinfo)
{

    if ((p_devinfo == NULL) || (p_dev == NULL)) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* 使能BGR */
    amhw_hc32_bgr_enable(AM_TRUE);

    return p_dev;
}

/**
 * \brief 模式设置
 */
error_t am_hc32_opa_mode_set (am_opa_handle_t handle, uint8_t ch)
{
    am_hc32_opa_dev_t *p_dev  = (am_hc32_opa_dev_t *)handle;
    amhw_hc32_opa_t *p_hw_opa =
                          (amhw_hc32_opa_t *)(p_dev->p_devinfo->reg_base);

    if (p_dev == NULL){
        return -AM_ENXIO;
    }

    if (ch > AM_HC32_OPA_CH3){
        return -AM_EINVAL;
    }

    /* 模式配置 */
    switch (p_dev->p_devinfo->opa_mode){
        case AM_HC32_OPA_MODE_UNITY_GAIN:
            amhw_hc32_opa_unity_gain_mode(p_hw_opa, ch);
            break;
        case AM_HC32_OPA_MODE_FORWARD_IN:
            amhw_hc32_opa_forwar_in_mode(p_hw_opa, ch, p_dev->p_devinfo->gain);
            break;
        case AM_HC32_OPA_MODE_OPPOSITE:
            amhw_hc32_opa_opposite_mode(p_hw_opa, ch, p_dev->p_devinfo->gain);
            break;
        case AM_HC32_OPA_MODE_TWO_DIFF:
            amhw_hc32_opa_diff_mode(p_hw_opa, ch, p_dev->p_devinfo->gain);
            break;
        case AM_HC32_OPA_MODE_UNIVERSAL:
            amhw_hc32_uinversal_mode(p_hw_opa, ch);
            break;
        case AM_HC32_OPA_MODE_CAS_OPPO:
            amhw_hc32_cas_opposite_mode(p_hw_opa, ch, p_dev->p_devinfo->gain);
            break;
        case AM_HC32_OPA_MODE_CAS_FORWARD:
            amhw_hc32_cas_forward_mode(p_hw_opa, ch, p_dev->p_devinfo->gain);
            break;
        default:
            return -AM_EINVAL;
    }

    /* IO端口与内部连接控制 */
    amhw_hc32_opa_po_ctrl(p_hw_opa, ch, p_dev->p_devinfo->po);

    return AM_OK;
}

error_t am_hc32_opa_enable (am_opa_handle_t handle)
{
    am_hc32_opa_dev_t *p_dev  = (am_hc32_opa_dev_t *)handle;
    amhw_hc32_opa_t *p_hw_opa =
                          (amhw_hc32_opa_t *)(p_dev->p_devinfo->reg_base);

    if (p_dev == NULL){

        return -AM_ENXIO;
    }

    /* 使能OPA */
    amhw_hc32_opa_en(p_hw_opa);

    return AM_OK;
}

/**
 * \brief OPA 去初始化
 */
void am_hc32_opa_deinit (am_opa_handle_t handle)
{
    am_hc32_opa_dev_t *p_dev  = (am_hc32_opa_dev_t *)handle;
    amhw_hc32_opa_t *p_hw_opa =
                          (amhw_hc32_opa_t *)(p_dev->p_devinfo->reg_base);

    if (handle == NULL){

        return ;
    }

    /* 平台去初始化 */
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    /* 禁能OPA */
    amhw_hc32_opa_dis (p_hw_opa);
}

/* end of file */
