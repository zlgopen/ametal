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
 * \brief VC中断触发例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   PA07接VCC或GND对应PA06输出高电平或低电平。当PA07 电压低于 PC00 (接DAC0_out 输出2400mv)
 *   产生中断，串口打印出 "vc trigger interrupt !"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_vc_int.c src_zlg118_hw_vc_int
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-30  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_vc_int
 * \copydoc demo_zlg118_hw_vc_int.c
 */

/** [src_zlg118_hw_vc_int] */

#include "ametal.h"
#include "am_zlg118.h"
#include "am_int.h"
#include "am_zlg118_vc.h"
#include "hw/amhw_zlg118_vc.h"
#include "hw/amhw_zlg118_dac.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define VC_INUM      INUM_VC0                           /**< \brief VC中断编号 */
#define VC_CHAN      AMHW_ZLG118_VC0                    /**< \brief VC通道 */
#define VC_VREF      AMHW_ZLG118_VC_REF2P5_VCC          /**< \brief 参考电压  */
#define VC_HYS_VOL   AMHW_ZLG118_VC0_HYS_10_MV          /**< \brief 迟滞电压*/
#define VC_BIAS      AMHW_ZLG118_VC0_BIAS_1_2_UA        /**< \brief 功耗*/
#define VC_DEB_TIME  AMHW_ZLG118_DEB_TIME_28_US         /**< \brief 滤波时间*/
#define VC_P_IN      AMHW_ZLG118_VC0_P_INPUT_PC0        /**< \brief P端输入*/
#define VC_N_IN      AMHW_ZLG118_VC0_N_INPUT_PA7        /**< \brief N端输入*/
#define VC_OUT_CFG   AMHW_ZLG118_VC_OUT_CFG_DISABLE     /**< \brief 输出配置 */
#define VC_INT_TYPE  AMHW_ZLG118_VC_OUT_TRI_INT_RISING  /**< \brief 中断触发类型 */

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_zlg118_vc_t       *gp_hw_vc   = NULL;  /**< \brief vc 外设 */
static amhw_zlg118_dac_t       *gp_hw_dac  = NULL;  /**< \brief dac 外设 */

static void (*gpfn_cb_t) (void *);               /**< \brief vc中断回调函数 */

/*
 * \brief VC初始化
 */
static void vc_init ()
{

    /* 使能BGR */
    amhw_zlg118_bgr_enable(AM_TRUE);

    /* 配置参考电压 */
    amhw_zlg118_vc_ref2p5_sel (gp_hw_vc, VC_VREF);

    /* 选择 迟滞电压、功耗、 滤波时间、  通道N、P端输入 、中断类型*/
    switch (VC_CHAN){

        case AMHW_ZLG118_VC0 :
                amhw_zlg118_vc0_hys_sel (gp_hw_vc, VC_HYS_VOL);
                amhw_zlg118_vc0_bias_sel (gp_hw_vc, VC_BIAS);
                amhw_zlg118_vc0_flt_time_sel (gp_hw_vc, VC_DEB_TIME);

                amhw_zlg118_vc0_n_sel (gp_hw_vc, VC_N_IN);
                amhw_zlg118_vc0_p_sel (gp_hw_vc, VC_P_IN);

                amhw_zlg118_vc0_out_tri_int_sel (gp_hw_vc,VC_INT_TYPE);
            break;

        case AMHW_ZLG118_VC1 :
                amhw_zlg118_vc1_hys_sel (gp_hw_vc, VC_HYS_VOL);
                amhw_zlg118_vc1_bias_sel (gp_hw_vc, VC_BIAS);
                amhw_zlg118_vc1_flt_time_sel (gp_hw_vc, VC_DEB_TIME);

                amhw_zlg118_vc1_n_sel (gp_hw_vc, VC_N_IN);
                amhw_zlg118_vc1_p_sel (gp_hw_vc, VC_P_IN);

                amhw_zlg118_vc1_out_tri_int_sel (gp_hw_vc, VC_INT_TYPE);
            break;
        case AMHW_ZLG118_VC2 :
                amhw_zlg118_vc2_hys_sel (gp_hw_vc, VC_HYS_VOL);
                amhw_zlg118_vc2_bias_sel (gp_hw_vc, VC_BIAS);
                amhw_zlg118_vc2_flt_time_sel (gp_hw_vc, VC_DEB_TIME);

                amhw_zlg118_vc2_n_sel (gp_hw_vc, VC_N_IN);
                amhw_zlg118_vc2_p_sel (gp_hw_vc, VC_P_IN);

                amhw_zlg118_vc1_out_tri_int_sel (gp_hw_vc, VC_INT_TYPE);
            break;
    }

    /* 配置输出 */
    amhw_zlg118_vc_outcfg_enable (gp_hw_vc,
                                  VC_OUT_CFG,
                                  VC_CHAN);

    if (VC_DEB_TIME != AMHW_ZLG118_DEB_TIME_NO){

        /* 滤波使能 */
        amhw_zlg118_vc_flt_enable (gp_hw_vc, VC_CHAN);
    }

}

/*
 * \brief VC中断服务函数
 */
static void __vc_irq_handler (void *p_cookie)
{

    if (amhw_zlg118_vc_int_status_check (gp_hw_vc, AMHW_ZLG118_VC0_FLT_INT_FLAG)){

        amhw_zlg118_vc_int_status_clr(gp_hw_vc,AMHW_ZLG118_VC0_FLT_INT_FLAG_CLR);
    }else if (amhw_zlg118_vc_int_status_check (gp_hw_vc,
                                               AMHW_ZLG118_VC1_FLT_INT_FLAG)){

        amhw_zlg118_vc_int_status_clr(gp_hw_vc,AMHW_ZLG118_VC1_FLT_INT_FLAG_CLR);
    }else if (amhw_zlg118_vc_int_status_check (gp_hw_vc,
                                               AMHW_ZLG118_VC2_FLT_INT_FLAG)){

        amhw_zlg118_vc_int_status_clr(gp_hw_vc,AMHW_ZLG118_VC2_FLT_INT_FLAG_CLR);
    }else{

        return ;
    }

    gpfn_cb_t(NULL);
}

/**
 * \breif VC中断触发配置
 *
 */
static void vc_int_tri_cfg ()
{
    if (gp_hw_vc == NULL){

        return ;
    }

    /* 输出触发中断使能 */
    amhw_zlg118_vc_int_enable (gp_hw_vc, VC_CHAN);

    am_int_connect(VC_INUM,
                   __vc_irq_handler,
                   NULL);

    am_int_enable (VC_INUM);
}

/**
 * \brief DAC初始化
 */
static void dac_hw_init(amhw_zlg118_dac_t *p_hw_dac,
                        uint32_t           verf)
{

    if (p_hw_dac == NULL){
        return ;
    }

    /* 设置参考电压源 */
    amhw_zlg118_dac_chan_sref_sel(p_hw_dac, verf);

    /* 失能输出缓冲器*/
    amhw_zlg118_dac_chan_output_buf_disable(p_hw_dac);

    /* 触发使能 */
    amhw_zlg118_dac_chan_trg_enable(p_hw_dac);

    /* 使能dac通道 */
    amhw_zlg118_dac_chan_enable(p_hw_dac);

}

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_vc_int_entry (void    *p_hw_vc,
                                  void   (*pfn_cb_t) (void *),
                                  void    *p_hw_dac,
                                  uint16_t mv_val)
{
    uint16_t vol_val = 0;

    gp_hw_vc  = (amhw_zlg118_vc_t *)p_hw_vc;
    gp_hw_dac = (amhw_zlg118_dac_t *)p_hw_dac;

    gpfn_cb_t = pfn_cb_t;

    /* 电压值转换为数字量 */
    vol_val = mv_val * 4096 / 3300;

    /* DAC 初始化 默认参考电压类型 外部参考电压源  PB01*/
    dac_hw_init(gp_hw_dac, AMHW_ZLG118_DAC_CHAN_MASK_EXTER_REF);

    /* 设置DAC电压值 数据格式：12位右对齐 */
    amhw_zlg118_dac_chan_12bit_right_aligned_data(gp_hw_dac, vol_val);

    /* VC初始化 */
    vc_init ();

    /* VC触发中断配置  */
    vc_int_tri_cfg ();

    /*使能电压比较器*/
    amhw_zlg118_vc_volt_cmp_enable(p_hw_vc, VC_CHAN);

    while (1)
    {
        ;
    }
}

