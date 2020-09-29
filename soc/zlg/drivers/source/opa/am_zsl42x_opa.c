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
 * - 1.00 19-10-08
 * \endinternal
 */

#include "ametal.h"
#include "am_zsl42x.h"
#include "am_delay.h"
#include "zsl42x_periph_map.h"
#include "am_zsl42x_opa.h"
#include "hw/amhw_zsl42x_opa.h"

/**
 * \brief OPA 初始化
 */
am_opa_handle_t am_zsl42x_opa_init(am_zsl42x_opa_dev_t           *p_dev,
                                   const am_zsl42x_opa_devinfo_t *p_devinfo)
{
    amhw_zsl42x_opa_t *p_hw_opa;

    if ((p_devinfo == NULL) || (p_dev == NULL)) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;

    p_hw_opa = (amhw_zsl42x_opa_t *)(p_dev->p_devinfo->reg_base);

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* 使能BGR */
    amhw_zsl42x_bgr_enable(AM_TRUE);

    if (p_dev->p_devinfo->opa_mode == AM_ZSL42x_OPA_GENERAL){

        /* 使能opa */
        amhw_zsl42x_opa_en (p_hw_opa);

        /* 自动校准使能 */
        amhw_zsl42x_opa_az_en (p_hw_opa);

        /* 自动校准脉冲宽度设置 */
        amhw_zsl42x_opa_clk_sel (p_hw_opa, p_dev->p_devinfo->clk_sel);

        /* 选择校准方式 */
        amhw_zsl42x_opa_az_way_sel (p_hw_opa, p_dev->p_devinfo->az_way);

        /* 启动校准 */
        switch (p_dev->p_devinfo->az_way){

            case AMHW_ZSL42x_AZ_SW_START:
                amhw_zsl42x_az_start (p_hw_opa,AMHW_ZSL42x_AZ_SW_START);
                break;
            case AMHW_ZSL42x_AZ_SW_TRI_START :
                amhw_zsl42x_az_start (p_hw_opa,AMHW_ZSL42x_AZ_SW_TRI_START);
                break;
            default :
                return NULL;
        }

        /* 延时 */
        am_udelay(20);
    }else {

        /* DAC使用OP3单位增加缓存使能 */
        amhw_zsl42x_opabuf_en (p_hw_opa);
    }

    return p_dev;
}

/**
 * \brief OPA 去初始化
 */
void am_zsl42x_opa_deinit (am_opa_handle_t handle)
{
    am_zsl42x_opa_dev_t *p_dev  = (am_zsl42x_opa_dev_t *)handle;
    amhw_zsl42x_opa_t *p_hw_opa =
                          (amhw_zsl42x_opa_t *)(p_dev->p_devinfo->reg_base);

    if (handle == NULL){

        return ;
    }

    /* 平台去初始化 */
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    /* 禁能OPA */
    amhw_zsl42x_opa_dis (p_hw_opa);
    amhw_zsl42x_opabuf_dis (p_hw_opa);
}

/* end of file */
