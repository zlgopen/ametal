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
 * \brief VC驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.01 20-04-27 zcb, Add callback function parameters
 * - 1.00 19-09-27
 * \endinternal
 */

#include "ametal.h"
#include "am_int.h"
#include "am_zsl42x.h"
#include "zsl42x_periph_map.h"
#include "am_zsl42x_vc.h"
#include "hw/amhw_zsl42x_vc.h"


/*VC中断服务函数*/
void __vc_irq_handler (void *p_cookie)
{
    amhw_zsl42x_vc_t *p_hw_vc;
    am_zsl42x_vc_dev_t *p_dev = (am_zsl42x_vc_dev_t *)p_cookie;
    p_hw_vc = (amhw_zsl42x_vc_t *)(p_dev->p_devinfo->vc_reg_base);



    if (amhw_zsl42x_vc_int_status_check (p_hw_vc, AMHW_ZSL42x_VC0_FLT_INT_FLAG)){

        amhw_zsl42x_vc_int_status_clr(p_hw_vc, AMHW_ZSL42x_VC0_FLT_INT_FLAG_CLR);

    }else if (amhw_zsl42x_vc_int_status_check (
                   p_hw_vc, AMHW_ZSL42x_VC1_FLT_INT_FLAG)){

        amhw_zsl42x_vc_int_status_clr(p_hw_vc, AMHW_ZSL42x_VC1_FLT_INT_FLAG_CLR);

    }else if (amhw_zsl42x_vc_int_status_check (
                   p_hw_vc, AMHW_ZSL42x_VC2_FLT_INT_FLAG)){

        amhw_zsl42x_vc_int_status_clr(p_hw_vc,AMHW_ZSL42x_VC2_FLT_INT_FLAG_CLR);

    }else{

        return ;
    }

    if (p_dev->pfn_trigger_cb) {
        p_dev->pfn_trigger_cb(p_dev->p_arg);
    }
}

/**
 * \brief VC通道使能
 */
int am_zsl42x_vc_chan_enable (am_zsl42x_vc_handle_t    handle)
{

    amhw_zsl42x_vc_t *p_hw_vc;

    if (handle == NULL){
       return AM_ERROR;
    }

    p_hw_vc = (amhw_zsl42x_vc_t *)(handle->p_devinfo->vc_reg_base);

    /*使能电压比较器*/
    amhw_zsl42x_vc_volt_cmp_enable(p_hw_vc, handle->chan);

    return AM_OK;
}

/**
 * \brief 中断触发函数连接
 */
int am_zsl42x_tri_set (am_zsl42x_vc_handle_t           handle,
                     am_pfnvoid_t                  pfn_cb,
                     void                         *p_arg)
{
    int key;

    if (handle == NULL) {
        return AM_ERROR;
    }

    key = am_int_cpu_lock();

    handle->pfn_trigger_cb = pfn_cb;
    handle->p_arg          = p_arg;

    am_int_cpu_unlock(key);

    return AM_OK;
}

/**
 * \brief VC初始化
 */
am_zsl42x_vc_handle_t am_zsl42x_vc_init (am_zsl42x_vc_dev_t           *p_dev,
                                     const am_zsl42x_vc_devinfo_t *p_devinfo)
{
    amhw_zsl42x_vc_t *p_hw_vc;

    if ((p_devinfo == NULL) || (p_dev == NULL)) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;
    p_dev->chan      = p_devinfo->vc_chan;

    p_hw_vc = (amhw_zsl42x_vc_t *)(p_dev->p_devinfo->vc_reg_base);

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* 使能BGR */
    amhw_zsl42x_bgr_enable(AM_TRUE);

    /* 配置参考电压 */
    amhw_zsl42x_vc_ref2p5_sel (p_hw_vc, p_dev->p_devinfo->vc_vref);

    /* 选择 迟滞电压、功耗、 滤波时间、  通道N、P端输入 、中断类型*/
    switch (p_dev->chan){

    case AMHW_ZSL42x_VC0 :
        amhw_zsl42x_vc0_hys_sel(p_hw_vc, p_dev->p_devinfo->vc_delay);
        amhw_zsl42x_vc0_bias_sel(p_hw_vc, p_dev->p_devinfo->vc_bias);
        amhw_zsl42x_vc0_flt_time_sel(p_hw_vc,
                                   p_dev->p_devinfo->vc_flt_time);
        amhw_zsl42x_vc0_n_sel(p_hw_vc, p_dev->p_devinfo->vc_n_in);
        amhw_zsl42x_vc0_p_sel(p_hw_vc, p_dev->p_devinfo->vc_p_in);
        amhw_zsl42x_vc0_out_tri_int_sel(p_hw_vc,
                                      p_dev->p_devinfo->vc_out_int);
        break;

    case AMHW_ZSL42x_VC1 :
        amhw_zsl42x_vc1_hys_sel(p_hw_vc, p_dev->p_devinfo->vc_delay);
        amhw_zsl42x_vc1_bias_sel(p_hw_vc, p_dev->p_devinfo->vc_bias);
        amhw_zsl42x_vc1_flt_time_sel(p_hw_vc,
                                   p_dev->p_devinfo->vc_flt_time);
        amhw_zsl42x_vc1_n_sel(p_hw_vc, p_dev->p_devinfo->vc_n_in);
        amhw_zsl42x_vc1_p_sel(p_hw_vc, p_dev->p_devinfo->vc_p_in);
        amhw_zsl42x_vc1_out_tri_int_sel(p_hw_vc,
                                      p_dev->p_devinfo->vc_out_int);
        break;
    case AMHW_ZSL42x_VC2 :
        amhw_zsl42x_vc2_hys_sel(p_hw_vc, p_dev->p_devinfo->vc_delay);
        amhw_zsl42x_vc2_bias_sel(p_hw_vc, p_dev->p_devinfo->vc_bias);
        amhw_zsl42x_vc2_flt_time_sel(p_hw_vc,
                                   p_dev->p_devinfo->vc_flt_time);
        amhw_zsl42x_vc2_n_sel(p_hw_vc, p_dev->p_devinfo->vc_n_in);
        amhw_zsl42x_vc2_p_sel(p_hw_vc, p_dev->p_devinfo->vc_p_in);
        amhw_zsl42x_vc1_out_tri_int_sel(p_hw_vc,
                                      p_dev->p_devinfo->vc_out_int);
        break;
    }

    /* 配置输出 */
    amhw_zsl42x_vc_outcfg_enable(p_hw_vc,
                               p_dev->p_devinfo->vc_out_cfg,
                               p_dev->chan);

    if (p_dev->p_devinfo->vc_flt_time != AMHW_ZSL42x_DEB_TIME_NO){

        /* 滤波使能 */
        amhw_zsl42x_vc_flt_enable (p_hw_vc, p_dev->chan);
    }

    /* 输出触发中断使能 */
    amhw_zsl42x_vc_int_enable (p_hw_vc, p_dev->chan);

    am_int_connect(p_dev->p_devinfo->vc_inum,
                   __vc_irq_handler,
                   (void *)p_dev);

    am_int_enable (p_dev->p_devinfo->vc_inum);

    return p_dev;

}

/**
 * \brief VC去初始化
 */
void am_zsl42x_vc_deinit (am_zsl42x_vc_handle_t        handle)
{
    am_zsl42x_vc_dev_t *p_dev  = (am_zsl42x_vc_dev_t *)handle;
    amhw_zsl42x_vc_t *p_hw_vc  =
                          (amhw_zsl42x_vc_t *)(p_dev->p_devinfo->vc_reg_base);

    if (handle == NULL){

        return ;
    }

    /* 滤波禁能 */
    amhw_zsl42x_vc_flt_disable (p_hw_vc, p_dev->chan);

    /*禁能电压比较器*/
    amhw_zsl42x_vc_volt_cmp_disable(p_hw_vc, p_dev->chan);

    /* 输出触发中断禁能 */
    amhw_zsl42x_vc_int_disable (p_hw_vc, p_dev->chan);

    am_int_disconnect(p_dev->p_devinfo->vc_inum,
                      __vc_irq_handler,
                      (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}
