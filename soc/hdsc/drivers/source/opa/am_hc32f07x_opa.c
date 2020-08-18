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
#include "am_hc32f07x.h"
#include "am_delay.h"
#include "hc32f07x_periph_map.h"
#include "am_hc32f07x_opa.h"
#include "hw/amhw_hc32f07x_opa.h"
#include "am_gpio.h"
/**
 * \brief OPA 初始化
 */
am_opa_handle_t am_hc32f07x_opa_init(am_hc32f07x_opa_dev_t           *p_dev,
                                   const am_hc32f07x_opa_devinfo_t *p_devinfo)
{
    amhw_hc32f07x_opa_t *p_hw_opa;
    int chan=0;
    if ((p_devinfo == NULL) || (p_dev == NULL)) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;

    p_hw_opa = (amhw_hc32f07x_opa_t *)(p_dev->p_devinfo->reg_base);
     
    chan = p_dev->p_devinfo ->chan ;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* 使能BGR */
    amhw_hc32_bgr_enable(AM_TRUE);

    if (p_dev->p_devinfo->opa_mode == AM_HC32F07X_OPA0_4_GENERAL){
        switch (chan) {

            case AMHW_HC32F07X_OP0:
                /* 使能opa0 */
                amhw_hc32f07x_opa0_en (p_hw_opa);

                /* 使能opa0输出通道 */
                am_gpio_pin_cfg (p_dev->p_devinfo ->p_ioinfo ->gpio,
                                 p_dev->p_devinfo ->p_ioinfo ->func );
                amhw_hc32f07x_opa0_out_en (p_hw_opa,
                                           p_dev->p_devinfo->p_ioinfo->chan_sel);

                /* 自动校准使能 */
                amhw_hc32f07x_opa0_az_en (p_hw_opa);
                break;
            case AMHW_HC32F07X_OP1:

                /* 使能opa1 */
                amhw_hc32f07x_opa1_en (p_hw_opa);

                /* 使能opa1输出通道 */
                am_gpio_pin_cfg (p_dev->p_devinfo ->p_ioinfo ->gpio,
                                 p_dev->p_devinfo ->p_ioinfo ->func );
                amhw_hc32f07x_opa1_out_en (p_hw_opa,
                                           p_dev->p_devinfo->p_ioinfo->chan_sel);
                /* 自动校准使能 */
                amhw_hc32f07x_opa1_az_en (p_hw_opa);
                break;
            case AMHW_HC32F07X_OP2:
                /* 使能opa2 */
                amhw_hc32f07x_opa2_en (p_hw_opa);
                /* 使能opa2输出通道 */
                am_gpio_pin_cfg (p_dev->p_devinfo ->p_ioinfo ->gpio,
                                 p_dev->p_devinfo ->p_ioinfo ->func );
                amhw_hc32f07x_opa2_out_en (p_hw_opa,
                                 p_dev->p_devinfo->p_ioinfo->chan_sel);
                /* 自动校准使能 */
                amhw_hc32f07x_opa2_az_en (p_hw_opa);
                break;
            case AMHW_HC32F07X_OP3:

                /* 使能opa3 */
                amhw_hc32f07x_opa3_en (p_hw_opa);

                /* 使能opa3输出通道 */
                am_gpio_pin_cfg (p_dev->p_devinfo ->p_ioinfo ->gpio,
                                 p_dev->p_devinfo ->p_ioinfo ->func );

                /* 自动校准使能 */
                amhw_hc32f07x_opa3_az_en (p_hw_opa);
                break;
            case AMHW_HC32F07X_OP4:

                /* 使能opa4 */
                amhw_hc32f07x_opa4_en (p_hw_opa);

                /* 使能opa4输出通道 */
                am_gpio_pin_cfg (p_dev->p_devinfo ->p_ioinfo ->gpio,
                                 p_dev->p_devinfo ->p_ioinfo ->func );
                /* 自动校准使能 */
                amhw_hc32f07x_opa4_az_en (p_hw_opa);
                break;
            default :
                break;
        }
       
        /* 自动校准脉冲宽度设置 */
        amhw_hc32f07x_opa_clk_sel (p_hw_opa, p_dev->p_devinfo->clk_sel);

        /* 选择校准方式 */
        amhw_hc32f07x_opa_az_way_sel (p_hw_opa, p_dev->p_devinfo->az_way);

        /* 启动校准 */
        switch (p_dev->p_devinfo->az_way){

            case AMHW_HC32F07X_AZ_SW_START:
                amhw_hc32f07x_az_start (p_hw_opa,AMHW_HC32F07X_AZ_SW_START);
                break;
            case AMHW_HC32F07X_AZ_SW_TRI_START :
                amhw_hc32f07x_az_start (p_hw_opa,AMHW_HC32F07X_AZ_SW_TRI_START);
                break;
            default :
                return NULL;
        }

        /* 延时 */
        am_udelay(20);
    }else {
        if (chan == AMHW_HC32F07X_OP3 ) {

            /* DAC使用OP3单位增加缓存使能 */
            amhw_hc32f07x_opa3buf_en (p_hw_opa);
        }else {

            /* DAC使用OP4单位增加缓存使能 */
            amhw_hc32f07x_opa4buf_en (p_hw_opa);
        }
    }

    return p_dev;
}



/**
 * \brief OPA 去初始化
 */
void am_hc32f07x_opa_deinit (am_opa_handle_t handle)
{
    am_hc32f07x_opa_dev_t *p_dev  = (am_hc32f07x_opa_dev_t *)handle;
    amhw_hc32f07x_opa_t *p_hw_opa =
                          (amhw_hc32f07x_opa_t *)(p_dev->p_devinfo->reg_base);

    int chan = 0;
    if (handle == NULL){

        return ;
    }

    chan = p_dev->p_devinfo ->p_ioinfo ->chan_sel ;

    /* 平台去初始化 */
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    switch(chan) {
    
        case AMHW_HC32F07X_OP0 :
            /* 禁能OPA */
            amhw_hc32f07x_opa0_dis (p_hw_opa);
            break;
        case AMHW_HC32F07X_OP1 :
            /* 禁能OPA */
            amhw_hc32f07x_opa1_dis (p_hw_opa);
            break;
        case AMHW_HC32F07X_OP2 :
            /* 禁能OPA */
            amhw_hc32f07x_opa2_dis (p_hw_opa);
            break;
        case AMHW_HC32F07X_OP3 :
            /* 禁能OPA */
            amhw_hc32f07x_opa3_dis (p_hw_opa);
            amhw_hc32f07x_opa3buf_dis (p_hw_opa);
            break;
        case AMHW_HC32F07X_OP4 :
            /* 禁能OPA */
            amhw_hc32f07x_opa4_dis (p_hw_opa);
            amhw_hc32f07x_opa4buf_dis (p_hw_opa);
            break;
        default :
            break;
    }
}

/* end of file */
