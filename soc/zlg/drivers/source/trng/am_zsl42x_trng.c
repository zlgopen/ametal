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
 * \brief trng驱动实现
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
#include "am_zsl42x_trng.h"
#include "hw/amhw_zsl42x_trng.h"

/**
 * \brief 随机数生成
 */
int am_zsl42x_trng_generate (am_trng_handle_t handle,
                             uint8_t          env_flag,
                             uint32_t         *p_data)
{
    amhw_zsl42x_trng_t *p_hw_trng;

    if (handle == NULL || p_data == NULL){
        return AM_ERROR;
    }

    p_hw_trng = (amhw_zsl42x_trng_t *)(handle->p_devinfo->reg_base);

    /* 打开随机源电路 */
    amhw_zsl42x_trng_cir_enable (p_hw_trng);

    if (env_flag == AM_ZSL42x_TRNG_POWER_ON){
        /* 选择重新装载初始值 */
        amhw_zsl42x_trng_load_set (p_hw_trng, AM_TRUE);
    }else{
        /* 选择不重新装载初始值 */
        amhw_zsl42x_trng_load_set (p_hw_trng, AM_FALSE);
    }

    /* 选择反馈方式 */
    amhw_zsl42x_trng_load_set (p_hw_trng, AM_TRUE);

    /* 选择移位次数 */
    amhw_zsl42x_trng_load_set (p_hw_trng, handle->p_devinfo->rng_cnt);

    /* 随机数生成启动 */
    amhw_zsl42x_trng_run (p_hw_trng);

    /* 等待生成完成 */
    while (amhw_zsl42x_trng_stu_check (p_hw_trng)){

        ;
    }

    if (env_flag == AM_ZSL42x_TRNG_POWER_ON){
        /* 选择不重新装载初始值 */
        amhw_zsl42x_trng_load_set (p_hw_trng, AM_FALSE);
    }else{
        ;
    }

    /* 选择反馈方式 */
    amhw_zsl42x_trng_load_set (p_hw_trng, AM_FALSE);

    /* 选择移位次数 */
    amhw_zsl42x_trng_load_set (p_hw_trng, AMHW_ZSL42x_TRNG_CNT_64);

    /* 随机数生成启动 */
    amhw_zsl42x_trng_run (p_hw_trng);

    /* 等待生成完成 */
    while (amhw_zsl42x_trng_stu_check (p_hw_trng)){

        ;
    }

    /* 读取64位真随机数 */
    p_data[0] = amhw_zsl42x_trng_low32_data_read (p_hw_trng);
    p_data[1] = amhw_zsl42x_trng_high32_data_read (p_hw_trng);

    /* 关闭随机源电路 */
    amhw_zsl42x_trng_cir_disable (p_hw_trng);

    return AM_OK;
}

/**
 * \brief TRNG 初始化
 */
am_trng_handle_t am_zsl42x_trng_init(am_zsl42x_trng_dev_t           *p_dev,
                                   const am_zsl42x_trng_devinfo_t *p_devinfo)
{
    if ((p_devinfo == NULL) || (p_dev == NULL)) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    return p_dev;
}

/**
 * \brief TRNG 去初始化
 */
void am_zsl42x_trng_deinit (am_trng_handle_t handle)
{
    am_zsl42x_trng_dev_t *p_dev  = (am_zsl42x_trng_dev_t *)handle;

    if (handle == NULL){

        return ;
    }

    /* 平台去初始化 */
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

}

/* end of file */
