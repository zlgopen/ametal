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
 * \brief PCA驱动接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-19
 * - 1.01 20-01-10
 *   修复16bit PWM异常
 * \endinternal
 */

/*******************************************************************************
  头文件
*******************************************************************************/
#include "ametal.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_hc32_pca.h"
#include "hc32_clk.h"
#include "hc32_inum.h"
#include "hc32_periph_map.h"

/*******************************************************************************
  宏定义
*******************************************************************************/


/*******************************************************************************
  内部函数
*******************************************************************************/

void __pca_irq_handler (void *parg)
{
    am_hc32_pca_dev_t *p_dev = (am_hc32_pca_dev_t *)parg;

    if(0 < amhw_hc32_pca_int_get(p_dev->p_hw_pca, HC32_PCA_CF)) {
        amhw_hc32_pca_int_clear(p_dev->p_hw_pca, HC32_PCA_CF);
    } else if (0 < amhw_hc32_pca_int_get(p_dev->p_hw_pca, HC32_PCA_CCF4)) {
        amhw_hc32_pca_int_clear(p_dev->p_hw_pca, HC32_PCA_CCF4);
    } else if (0 < amhw_hc32_pca_int_get(p_dev->p_hw_pca, HC32_PCA_CCF3)) {
        amhw_hc32_pca_int_clear(p_dev->p_hw_pca, HC32_PCA_CCF3);
    } else if (0 < amhw_hc32_pca_int_get(p_dev->p_hw_pca, HC32_PCA_CCF2)) {
        amhw_hc32_pca_int_clear(p_dev->p_hw_pca, HC32_PCA_CCF2);
    } else if (0 < amhw_hc32_pca_int_get(p_dev->p_hw_pca, HC32_PCA_CCF1)) {
        amhw_hc32_pca_int_clear(p_dev->p_hw_pca, HC32_PCA_CCF1);
    } else if (0 < amhw_hc32_pca_int_get(p_dev->p_hw_pca, HC32_PCA_CCF0)) {
        amhw_hc32_pca_int_clear(p_dev->p_hw_pca, HC32_PCA_CCF0);
    }
}

/*******************************************************************************
  外部函数
*******************************************************************************/

/**
 * \brief PCA初始化
 *
 * \return PCA服务操作句柄
 */
am_hc32_pca_handle_t am_hc32_pca_init (
        am_hc32_pca_dev_t             *p_dev,
        const am_hc32_pca_devinfo_t   *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;
    p_dev->p_hw_pca = (amhw_hc32_pca_t *)p_devinfo->pca_regbase;

    if(p_devinfo->pfn_plfm_init != NULL) {
        p_devinfo->pfn_plfm_init();
    }

    /* PCA时钟源选择 */
    amhw_hc32_pca_cps_set(p_dev->p_hw_pca, p_devinfo->clk_src);

    return p_dev;
}

/**
 * \brief 解除PCA初始化
 *
 * \param[in] handle : 与从设备关联的PCA标准服务操作句柄
 *
 * \return 无
 */
void am_hc32_pca_deinit (am_hc32_pca_handle_t handle)
{
    if(handle->p_devinfo->pfn_plfm_deinit != NULL) {
        handle->p_devinfo->pfn_plfm_deinit();
    }
    handle->p_devinfo = NULL;
    handle->p_hw_pca  = NULL;
}

/**
 * \brief PCA边沿捕获模式配置
 *
 * \param[in] handle : 与从设备关联的PCA标准服务操作句柄
 * \param[in] num    : PCA 模块编号
 * \param[in] chan   : 通道号
 * \param[in] cap    : 捕获边沿选择
 *
 * \return AM_OK:成功
 *         其它:失败
 */
int am_hc32_pca_cap_set (am_hc32_pca_handle_t handle,
                         uint8_t              num,
                         uint8_t              chan,
                         am_hc32_pca_capn_t   cap)
{
    am_hc32_pca_int_t int_clr = HC32_PCA_CCF0;

    if(num > 4 || handle == NULL || handle->p_devinfo->ioinfo == NULL) {
        return -AM_EINVAL;
    }

    am_gpio_pin_cfg(handle->p_devinfo->ioinfo[chan].gpio,
                    handle->p_devinfo->ioinfo[chan].capfunc);

    /* 禁能比较器 */
    amhw_hc32_pca_ecom_set(handle->p_hw_pca, num, 0);

    /* 捕获边沿设置 */
    amhw_hc32_pca_capp_set(handle->p_hw_pca, num, (cap >> 1) & 0x1);
    amhw_hc32_pca_capn_set(handle->p_hw_pca, num, cap & 0x1);

    /* 禁用匹配功能 */
    amhw_hc32_pca_mat_set(handle->p_hw_pca, num, 0);

    /* 禁能引脚翻转 */
    amhw_hc32_pca_tog_set(handle->p_hw_pca, num, HC32_PCA_TOG_DIS);

    /* 禁用PWM */
    amhw_hc32_pca_pwm_set(handle->p_hw_pca, num, 0);
    amhw_hc32_pca_epwm_disable(handle->p_hw_pca);

    switch(num) {
    case 0:
        int_clr = HC32_PCA_CCF0;
        break;
    case 1:
        int_clr = HC32_PCA_CCF1;
        break;
    case 2:
        int_clr = HC32_PCA_CCF2;
        break;
    case 3:
        int_clr = HC32_PCA_CCF3;
        break;
    case 4:
        int_clr = HC32_PCA_CCF4;
        break;

    default:
        break;
    }
    amhw_hc32_pca_int_clear(handle->p_hw_pca, int_clr);

    /* 允许比较/捕获中断 */
    amhw_hc32_pca_ccie_set(handle->p_hw_pca, num, 1);

    /* 连接中断函数 */
    am_int_connect(handle->p_devinfo->inum, __pca_irq_handler, (void *)handle);
    am_int_enable(handle->p_devinfo->inum);

    return AM_OK;
}

/**
 * \brief PCA比较计数模式配置
 *
 * \param[in] handle : 与从设备关联的PCA标准服务操作句柄
 * \param[in] num    : PCA 模块编号
 * \param[in] carr   : 周期寄存器值
 * \param[in] ccap   : 比较捕获寄存器值
 *
 * \return AM_OK:成功
 *         其它:失败
 */
int am_hc32_pca_cmp_cnt_set (am_hc32_pca_handle_t handle,
                             uint8_t              num,
                             uint16_t             carr,
                             uint16_t             ccap)
{
    am_hc32_pca_int_t int_clr = HC32_PCA_CCF0;

    if(num > 4 || handle == NULL) {
        return -AM_EINVAL;
    }

    /* 使能比较器 */
    amhw_hc32_pca_ecom_set(handle->p_hw_pca, num, 1);

    /* 捕获边沿设置 */
    amhw_hc32_pca_capp_set(handle->p_hw_pca, num, HC32_PCA_CAPP_DIS >> 1);
    amhw_hc32_pca_capn_set(handle->p_hw_pca, num, HC32_PCA_CAPN_DIS);

    /* 使能匹配功能 */
    amhw_hc32_pca_mat_set(handle->p_hw_pca, num, 1);

    /* 禁能引脚翻转 */
    amhw_hc32_pca_tog_set(handle->p_hw_pca, num, HC32_PCA_TOG_DIS);

    /* PWM设置 */
    amhw_hc32_pca_pwm_set(handle->p_hw_pca, num, 0);
    amhw_hc32_pca_epwm_enable(handle->p_hw_pca);

    /* 周期寄存器配置 */
    amhw_hc32_pca_carr_set(handle->p_hw_pca, carr);

    /* 比较捕获寄存器配置 */
    amhw_hc32_pca_ccap_set(handle->p_hw_pca, num, ccap);

    switch(num) {
    case 0:
        int_clr = HC32_PCA_CCF0;
        break;
    case 1:
        int_clr = HC32_PCA_CCF1;
        break;
    case 2:
        int_clr = HC32_PCA_CCF2;
        break;
    case 3:
        int_clr = HC32_PCA_CCF3;
        break;
    case 4:
        int_clr = HC32_PCA_CCF4;
        break;

    default:
        break;
    }
    amhw_hc32_pca_int_clear(handle->p_hw_pca, int_clr);

    /* 允许比较/捕获中断 */
    amhw_hc32_pca_ccie_set(handle->p_hw_pca, num, 1);
    amhw_hc32_pca_cfie_set(handle->p_hw_pca, 1);

    /* 连接中断函数 */
    am_int_connect(handle->p_devinfo->inum, __pca_irq_handler, (void *)handle);
    am_int_enable(handle->p_devinfo->inum);

    return AM_OK;
}

/**
 * \brief PCA脉宽调制模式配置
 *
 * \param[in] handle  : 与从设备关联的PCA标准服务操作句柄
 * \param[in] num     : PCA 模块编号
 * \param[in] chan    : 通道号
 * \param[in] pwm_sel : PWM类型选择
 * \param[in] carr    : 周期寄存器值
 * \param[in] ccap    : 比较捕获寄存器值
 *
 * 8bit占空比  = ccap / 255 * 100%
 * 16bit占空比= (carr - ccap) / carr * 100%
 *
 * \return AM_OK:成功
 *         其它:失败
 */
int am_hc32_pca_pwm_set (am_hc32_pca_handle_t handle,
                         uint8_t              num,
                         uint8_t              chan,
                         am_hc32_pca_pwm_t    pwm_sel,
                         uint16_t             carr,
                         uint16_t             ccap)
{
    if(num > 4 || handle == NULL || handle->p_devinfo->ioinfo == NULL) {
        return -AM_EINVAL;
    }

    am_gpio_pin_cfg(handle->p_devinfo->ioinfo[chan].gpio,
                    handle->p_devinfo->ioinfo[chan].pwmunc);

    /* 使能比较器 */
    amhw_hc32_pca_ecom_set(handle->p_hw_pca, num, 1);

    /* 捕获边沿设置 */
    amhw_hc32_pca_capp_set(handle->p_hw_pca, num, HC32_PCA_CAPP_DIS >> 1);
    amhw_hc32_pca_capn_set(handle->p_hw_pca, num, HC32_PCA_CAPN_DIS);

    /* PWM设置 */
    if(pwm_sel) {
        amhw_hc32_pca_mat_set(handle->p_hw_pca, num, 1);
        amhw_hc32_pca_tog_set(handle->p_hw_pca, num, HC32_PCA_TOG_EN);

        amhw_hc32_pca_pwm_set(handle->p_hw_pca, num, 0);
        amhw_hc32_pca_epwm_enable(handle->p_hw_pca);

        amhw_hc32_pca_carr_set(handle->p_hw_pca, carr);
        amhw_hc32_pca_ccap_set(handle->p_hw_pca, num, ccap);
    } else {
        amhw_hc32_pca_mat_set(handle->p_hw_pca, num, 0);
        amhw_hc32_pca_tog_set(handle->p_hw_pca, num, HC32_PCA_TOG_DIS);

        amhw_hc32_pca_pwm_set(handle->p_hw_pca, num, 1);
        amhw_hc32_pca_epwm_disable(handle->p_hw_pca);

        amhw_hc32_pca_ccapl_set(handle->p_hw_pca, num, 255 - ccap);
        amhw_hc32_pca_ccaph_set(handle->p_hw_pca, num, 255 - ccap);
    }

    return AM_OK;
}

/* 启动PCA模块 */
void am_hc32_pca_start(am_hc32_pca_handle_t handle)
{
    amhw_hc32_pca_start(handle->p_hw_pca);
}

/* 停止PCA模块 */
void am_hc32_pca_stop(am_hc32_pca_handle_t handle)
{
    amhw_hc32_pca_stop(handle->p_hw_pca);
}

/* end of file */
