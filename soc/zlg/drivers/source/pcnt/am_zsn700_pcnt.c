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
 * \brief PCNT驱动接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-18
 * \endinternal
 */

/*******************************************************************************
  头文件
*******************************************************************************/
#include "ametal.h"
#include "am_int.h"
#include "am_zsn700_pcnt.h"

/*******************************************************************************
  内部函数
*******************************************************************************/

void __pcnt_irq_handler (void *parg)
{
    am_zsn700_pcnt_dev_t *p_dev = (am_zsn700_pcnt_dev_t *)parg;
    if(amhw_zsn700_pcnt_int_get(p_dev->p_hw_pcnt, ZSN700_PCNT_INT_TO)) {
        amhw_zsn700_pcnt_int_clear(p_dev->p_hw_pcnt, ZSN700_PCNT_INT_TO);
    } else if (amhw_zsn700_pcnt_int_get(p_dev->p_hw_pcnt, ZSN700_PCNT_INT_OV)) {
        amhw_zsn700_pcnt_int_clear(p_dev->p_hw_pcnt, ZSN700_PCNT_INT_OV);
    } else if (amhw_zsn700_pcnt_int_get(p_dev->p_hw_pcnt, ZSN700_PCNT_INT_UF)) {
        amhw_zsn700_pcnt_int_clear(p_dev->p_hw_pcnt, ZSN700_PCNT_INT_UF);
    } else {
        ;
    }
}

/*******************************************************************************
  外部函数
*******************************************************************************/

/**
 * \brief PCNT初始化
 *
 * \param[in] p_dev     : 指向PCNT设备结构体的指针
 * \param[in] p_devinfo : 指向PCNT设备信息结构体的指针
 *
 * \return PCNT服务操作句柄
 */
am_zsn700_pcnt_handle_t am_zsn700_pcnt_init (
        am_zsn700_pcnt_dev_t     *p_dev,
        const am_zsn700_pcnt_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;
    p_dev->p_hw_pcnt = (amhw_zsn700_pcnt_t *)p_devinfo->pcnt_regbase;

    if(p_devinfo->pfn_plfm_init != NULL) {
        p_devinfo->pfn_plfm_init();
    }

    /* 清除中断标识 */
    amhw_zsn700_pcnt_int_clear(p_dev->p_hw_pcnt,
                               ZSN700_PCNT_INT_TO |
                               ZSN700_PCNT_INT_OV |
                               ZSN700_PCNT_INT_UF);

    /* 连接中断函数 */
    am_int_connect(p_devinfo->inum, __pcnt_irq_handler, (void *)p_dev);

    return p_dev;
}

/**
 * \brief 解除PCNT初始化
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 *
 * \return 无
 */
void am_zsn700_pcnt_deinit (am_zsn700_pcnt_handle_t handle)
{
    if(handle->p_devinfo->pfn_plfm_deinit != NULL) {
        handle->p_devinfo->pfn_plfm_deinit();
    }
    handle->p_devinfo = NULL;
    handle->p_hw_pcnt = NULL;
}

/**
 * \brief 采样时钟设置
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 * \param[in] clk    : 时钟
 *
 * \return 无
 */
void am_zsn700_pcnt_clk_sel (am_zsn700_pcnt_handle_t handle,
                             am_zsn700_pcnt_clk_t    clk)
{
    if (handle == NULL) {
        return ;
    }
    amhw_zsn700_pcnt_clk_set(handle->p_hw_pcnt, clk);
}

/**
 * \brief 通道极性设置
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 * \param[in] polar  : 通道极性选择
 *
 * \return 无
 */
void am_zsn700_pcnt_polar_set (am_zsn700_pcnt_handle_t handle,
                               am_zsn700_pcnt_polar_t  polar)
{
    if (handle == NULL) {
        return ;
    }
    switch(polar) {
    case ZSN700_PCNT_S0PNOINVERT:
        amhw_zsn700_pcnt_s0p_set(handle->p_hw_pcnt, 0);
        break;

    case ZSN700_PCNT_S0PINVERT:
        amhw_zsn700_pcnt_s0p_set(handle->p_hw_pcnt, 1);
        break;

    case ZSN700_PCNT_S1PNOINVERT:
        amhw_zsn700_pcnt_s1p_set(handle->p_hw_pcnt, 0);
        break;

    case ZSN700_PCNT_S1PINVERT:
        amhw_zsn700_pcnt_s1p_set(handle->p_hw_pcnt, 1);
        break;

    default:
        break;
    }
}

/**
 * \brief 滤波使能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 * \param[in] debtop : 滤波计数器阈值
 * \param[in] clkdiv : 滤波时钟分频系数
 *
 * \return 无
 */
void am_zsn700_pcnt_flt_enable (am_zsn700_pcnt_handle_t handle,
                                uint8_t                 debtop,
                                uint16_t                clkdiv)
{
    if (handle == NULL) {
        return ;
    }
    amhw_zsn700_pcnt_debtop_set(handle->p_hw_pcnt, debtop);
    amhw_zsn700_pcnt_clkdiv_set(handle->p_hw_pcnt, clkdiv);
    amhw_zsn700_pcnt_flt_enable(handle->p_hw_pcnt);
}

/**
 * \brief 滤波禁能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 *
 * \return 无
 */
void am_zsn700_pcnt_flt_disable (am_zsn700_pcnt_handle_t handle)
{
    if (handle == NULL) {
        return ;
    }
    amhw_zsn700_pcnt_flt_disable(handle->p_hw_pcnt);
}

/**
 * \brief 超时使能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 * \param[in] th     : 超时阈值
 *
 * \return 无
 */
void am_zsn700_pcnt_timeover_enable (am_zsn700_pcnt_handle_t handle,
                                     uint16_t                th)
{
    if (handle == NULL) {
        return ;
    }
    amhw_zsn700_pcnt_th_set(handle->p_hw_pcnt, th);
    amhw_zsn700_pcnt_timeover_enable(handle->p_hw_pcnt);
}

/**
 * \brief 超时禁能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 *
 * \return 无
 */
void am_zsn700_pcnt_timeover_disable (am_zsn700_pcnt_handle_t handle)
{
    if (handle == NULL) {
        return ;
    }
    amhw_zsn700_pcnt_timeover_disable(handle->p_hw_pcnt);
}

/**
 * \brief PCNT使能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 * \param[in] mode   : 计数模式
 * \param[in] dir    : 计数方向
 * \param[in] value  : 溢出值
 * \param[in] dgb    : 输出选项
 *
 * \return 无
 */
void am_zsn700_pcnt_start (am_zsn700_pcnt_handle_t handle,
                           am_zsn700_pcnt_mode_t   mode,
                           am_zsn700_pcnt_dir_t    dir,
                           uint16_t                value,
                           am_zsn700_pcnt_dgb_t    dgb)
{
    if (handle == NULL) {
        return ;
    }

    /* 输出设置 */
    amhw_zsn700_pcnt_dbg_set(handle->p_hw_pcnt, dgb);

    /* 模式设置 */
    amhw_zsn700_pcnt_mode_set(handle->p_hw_pcnt, mode);

    if(ZSN700_PCNT_DOUBLE != mode) {

        /* 计数方向设置 */
        amhw_zsn700_pcnt_dir_set(handle->p_hw_pcnt, dir);
    }

    /* 溢出值设置 */
    amhw_zsn700_pcnt_buf_set(handle->p_hw_pcnt, value);

    if(ZSN700_PCNT_UP == dir) {

        /* 立即同步数据 */
        amhw_zsn700_pcnt_cmd_set(handle->p_hw_pcnt,
                                 ZSN700_PCNT_B2T);

        /* 使能相应中断 */
        amhw_zsn700_pcnt_int_enable(handle->p_hw_pcnt,
                                    ZSN700_PCNT_INT_OV);
    } else {
        amhw_zsn700_pcnt_cmd_set(handle->p_hw_pcnt,
                                 ZSN700_PCNT_B2C);
        amhw_zsn700_pcnt_int_enable(handle->p_hw_pcnt,
                                    ZSN700_PCNT_INT_UF);
    }

    am_int_enable(handle->p_devinfo->inum);
    amhw_zsn700_pcnt_start(handle->p_hw_pcnt);
}

/**
 * \brief PCNT禁能
 *
 * \param[in] handle : 与从设备关联的PCNT标准服务操作句柄
 *
 * \return 无
 */
void am_zsn700_pcnt_stop (am_zsn700_pcnt_handle_t handle)
{
    if (handle == NULL) {
        return ;
    }

    am_int_disable(handle->p_devinfo->inum);
    amhw_zsn700_pcnt_stop(handle->p_hw_pcnt);
}

/* end of file */
