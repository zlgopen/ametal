/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief  CAN驱动层实现
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-05  zcb, first implementation.
 * - 1.01 19-12-18  zc , add filter table extern api
 * \endinternal
 */

#include "am_int.h"

#include "am_zmf159_can.h"
#include "hw/amhw_zmf159_can.h"

/** \brief CAN启动 */
am_can_err_t __can_start (void *p_drv);
/** \brief CAN复位（停止） */
am_can_err_t __can_reset (void *p_drv);
/** \brief CAN休眠  */
am_can_err_t __can_sleep (void *p_drv);
/** \brief CAN唤醒 */
am_can_err_t __can_wakeup (void *p_drv);
/** \brief 中断使能 */
am_can_err_t __can_int_enable (void *p_drv, am_can_int_type_t int_mask);
/** \brief 中断禁能*/
am_can_err_t __can_int_disable (void *p_drv, am_can_int_type_t int_mask);
/** \brief 模式设置 */
am_can_err_t __can_mode_set (void *p_drv, am_can_mode_type_t mode);
/** \brief 波特率设置 */
am_can_err_t __can_baudrate_set (void               *p_drv,
                                 am_can_bps_param_t *p_can_baudrate);
/** \brief 波特率获取 */
am_can_err_t __can_baudrate_get (void               *p_drv,
                                 am_can_bps_param_t *p_can_baudrate);
/** \brief 获取错误计数 */
am_can_err_t __can_err_cnt_get (void *p_drv, am_can_err_cnt_t *p_can_err_reg);
/** \brief 清除错误计数 */
am_can_err_t __can_err_cnt_clr (void *p_drv);
/** \brief 发送信息   */
am_can_err_t __can_msg_send (void *p_drv, am_can_message_t *p_txmsg);
/** \brief 接收信息   */
am_can_err_t __can_msg_recv (void *p_drv, am_can_message_t *p_rxmsg);
/** \brief 停止发送信息   */
am_can_err_t __can_stop_msg_snd (void *p_drv);
/** \brief 设置滤波函数 */
am_can_err_t __can_filter_tab_set (void    *p_drv,
                                   uint8_t *p_filterbuff,
                                   size_t   lenth);
/** \brief 获取滤波函数*/
am_can_err_t __can_filter_tab_get (void    *p_drv,
                                   uint8_t *p_filterbuff,
                                   size_t  *p_lenth);
/** \brief CAN 状态 */
am_can_err_t __can_status_get (void              *p_drv,
                               am_can_int_type_t *p_int_type,
                               am_can_bus_err_t  *p_bus_err);
/** \brief 中断连接 */
am_can_err_t __can_connect (void         *p_drv,
                            am_pfnvoid_t  pfn_isr,
                            void         *p_arg);
/** \brief 删除中断连接 */
am_can_err_t __can_disconnect (void         *p_drv,
                               am_pfnvoid_t  pfn_isr,
                               void         *p_arg);
/** \brief 中断回调函数的连接 */
am_can_err_t __can_intcb_connect (void              *p_drv,
                                  am_can_int_type_t  inttype,
                                  am_pfnvoid_t       pfn_callback,
                                  void              *p_arg);
/** \brief 删除中断回调函数的连接 */
am_can_err_t __can_intcb_disconnect (void *p_drv, am_can_int_type_t  inttype);

/** \brief 设置滤波函数(扩展) */
am_can_err_t __can_filter_tab_ext_set (void             *p_drv,
                                       am_can_filter_t  *p_filterbuff,
                                       size_t            lenth);

/** \brief 获取滤波函数(扩展) */
am_can_err_t __can_filter_tab_ext_get (void             *p_drv,
                                       am_can_filter_t  *p_filterbuff,
                                       size_t           *p_lenth);
struct am_can_drv_funcs dev_func = {
    __can_start,
    __can_reset,
    __can_sleep,
    __can_wakeup,
    __can_int_enable,
    __can_int_disable,
    __can_mode_set,
    __can_baudrate_set,
    __can_baudrate_get,
    __can_err_cnt_get,
    __can_err_cnt_clr,
    __can_msg_send,
    __can_msg_recv,
    __can_stop_msg_snd,
    __can_filter_tab_set,
    __can_filter_tab_get,
    __can_status_get,
    __can_connect,
    __can_disconnect,
    __can_intcb_connect,
    __can_intcb_disconnect,
    __can_filter_tab_ext_set,
    __can_filter_tab_ext_get
};


/**
 * \brief CAN启动
 */
am_can_err_t __can_start (void *p_drv)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    amhw_zmf159_peli_can_ewlr_write(p_hw_can, 0x15);

    amhw_zmf159_can_mode_set(p_hw_can,
                               p_dev->p_devinfo->type,
                               &(p_dev->mode),
                               AMHW_ZMF159_CAN_MODE_RUN);

    am_int_enable(p_dev->p_devinfo->int_num);

    return AM_CAN_NOERROR;
}

/**
 * \brief CAN复位（停止）
 */
am_can_err_t __can_reset (void *p_drv)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    amhw_zmf159_can_mode_set(p_hw_can,
                               p_dev->p_devinfo->type,
                               &(p_dev->mode),
                               AMHW_ZMF159_CAN_MODE_RESET);
    /* 禁能中断 */
    am_int_disable(p_dev->p_devinfo->int_num);

    return AM_CAN_NOERROR;
}

/**
 * \brief CAN休眠
 */
am_can_err_t __can_sleep (void *p_drv)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    amhw_zmf159_can_mode_set(p_hw_can,
                               p_dev->p_devinfo->type,
                               &(p_dev->mode),
                               AMHW_ZMF159_CAN_MODE_SLEEP);

    return AM_CAN_NOERROR;
}

/**
 * \brief CAN唤醒
 */
am_can_err_t __can_wakeup (void *p_drv)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    amhw_zmf159_can_mode_set(p_hw_can,
                               p_dev->p_devinfo->type,
                               &(p_dev->mode),
                               AMHW_ZMF159_CAN_MODE_RUN);

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断使能
 */
am_can_err_t __can_int_enable (void *p_drv, am_can_int_type_t int_mask)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;
    amhw_zmf159_can_type_t can_type;
    amhw_zmf159_can_mode_t mode = p_dev->mode;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;
    can_type = p_dev->p_devinfo->type;

    /* peli can中断使能只能在复位模式下进行 */
    if ((AMHW_ZMF159_CAN_PELI_CAN == can_type) &&
            (AMHW_ZMF159_CAN_MODE_RESET != p_dev->mode)) {

        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   AMHW_ZMF159_CAN_MODE_RESET);
    }

    /* 接收中断 */
    if (int_mask & AM_CAN_INT_RX) {
        /* 禁能接收中断 */
        amhw_zmf159_can_int_enable(p_hw_can,
                                     can_type,
                                     AMHW_ZMF159_CAN_INT_RECV);
        int_mask &= ~AM_CAN_INT_RX;
    }

    /* 发送中断 */
    if (int_mask & AM_CAN_INT_TX) {
        /* 禁能发送中断 */
        amhw_zmf159_can_int_enable(p_hw_can,
                                     can_type,
                                     AMHW_ZMF159_CAN_INT_TRAN);
        int_mask &= ~AM_CAN_INT_TX;
    }

    /* 错误\警告中断  = 错误、数据溢出 */
    if ((int_mask & AM_CAN_INT_ERROR) || (int_mask & AM_CAN_INT_WARN)) {
        /* 禁能错误中断 */
        amhw_zmf159_can_int_enable(p_hw_can,
                                     can_type,
                                     AMHW_ZMF159_CAN_INT_ERR);
        /* 如果处于peli can模式 */
        if (AMHW_ZMF159_CAN_PELI_CAN == can_type) {
            /* 禁能总线错误中断 */
            amhw_zmf159_can_int_enable(p_hw_can,
                                         can_type,
                                         AMHW_ZMF159_CAN_INT_BUS_ERR);
            /* 禁能总线总裁丢失中断 */
            amhw_zmf159_can_int_enable(p_hw_can,
                                         can_type,
                                        AMHW_ZMF159_CAN_INT_ARBITRATION_LOST);
        }
        int_mask &= ~AM_CAN_INT_ERROR;
    }

    /* 唤醒中断 */
    if (int_mask & AM_CAN_INT_WAKE_UP) {
        /* 禁能唤醒中断 */
        amhw_zmf159_can_int_enable(p_hw_can,
                                     can_type,
                                     AMHW_ZMF159_CAN_INT_WAKEUP);
        int_mask &= ~AM_CAN_INT_WAKE_UP;
    }

    /* 总线超载中断 */
    if (int_mask & AM_CAN_INT_DATAOVER) {
        /* 禁能数据溢出中断 */
        amhw_zmf159_can_int_enable(p_hw_can,
                                     can_type,
                                     AMHW_ZMF159_CAN_INT_OVER);
        int_mask &= ~AM_CAN_INT_WAKE_UP;
    }

    if ((0 != int_mask) && (p_dev->p_devinfo->type ==
                                     AMHW_ZMF159_CAN_BASIC_CAN)) {
        /* 运行至这里表明处于BASIC CAN模式使能BASIC CAN不支持的中断 */
        return AM_CAN_INVALID_PARAMETER;

    } else if (0 == int_mask) {

        return AM_CAN_NOERROR;
    }

    if (int_mask & AM_CAN_INT_ERROR_PASSIVE) {
        /* 使能peli can 消极错误中断 */
        amhw_zmf159_can_int_enable(p_hw_can,
                                     can_type,
                                     AMHW_ZMF159_CAN_INT_ERR_PASSIVE);
    }

    if (int_mask & AM_CAN_INT_BUS_OFF) {
        /* 禁能总线错误中断 */
        amhw_zmf159_can_int_enable(p_hw_can,
                                     can_type,
                                     AMHW_ZMF159_CAN_INT_BUS_ERR);
    }

    /* 恢复CAN状态 */
    if (mode != AMHW_ZMF159_CAN_MODE_RESET) {

        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断禁能
 */
am_can_err_t __can_int_disable (void *p_drv, am_can_int_type_t int_mask)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;
    amhw_zmf159_can_type_t can_type;
    amhw_zmf159_can_mode_t mode = p_dev->mode;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;
    can_type = p_dev->p_devinfo->type;

    /* peli can中断使能只能在复位模式下进行 */
    if ((AMHW_ZMF159_CAN_PELI_CAN == can_type) &&
            (AMHW_ZMF159_CAN_MODE_RESET != p_dev->mode)) {

        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   AMHW_ZMF159_CAN_MODE_RESET);
    }

    /* 接收中断 */
    if (int_mask & AM_CAN_INT_RX) {
        /* 禁能接收中断 */
        amhw_zmf159_can_int_disable(p_hw_can,
                                      can_type,
                                      AMHW_ZMF159_CAN_INT_RECV);
        int_mask &= ~AM_CAN_INT_RX;
    }

    /* 发送中断 */
    if (int_mask & AM_CAN_INT_TX) {
        /* 禁能发送中断 */
        amhw_zmf159_can_int_disable(p_hw_can,
                                      can_type,
                                      AMHW_ZMF159_CAN_INT_TRAN);
        int_mask &= ~AM_CAN_INT_TX;
    }

    /* 错误\警告中断  = 错误、数据溢出 */
    if ((int_mask & AM_CAN_INT_ERROR) || (int_mask & AM_CAN_INT_WARN)) {
        /* 禁能错误中断 */
        amhw_zmf159_can_int_disable(p_hw_can,
                                      can_type,
                                      AMHW_ZMF159_CAN_INT_ERR);
        /* 如果处于peli can模式 */
        if (AMHW_ZMF159_CAN_PELI_CAN == can_type) {
            /* 禁能总线错误中断 */
            amhw_zmf159_can_int_disable(p_hw_can,
                                          can_type,
                                          AMHW_ZMF159_CAN_INT_BUS_ERR);
            /* 禁能总线总裁丢失中断 */
            amhw_zmf159_can_int_disable(p_hw_can,
                                          can_type,
                                        AMHW_ZMF159_CAN_INT_ARBITRATION_LOST);
        }
        int_mask &= ~AM_CAN_INT_ERROR;
    }

    /* 唤醒中断 */
    if (int_mask & AM_CAN_INT_WAKE_UP) {
        /* 禁能唤醒中断 */
        amhw_zmf159_can_int_disable(p_hw_can,
                                      can_type,
                                      AMHW_ZMF159_CAN_INT_WAKEUP);
        int_mask &= ~AM_CAN_INT_WAKE_UP;
    }

    /* 总线超载中断 */
    if (int_mask & AM_CAN_INT_DATAOVER) {
        /* 禁能数据溢出中断 */
        amhw_zmf159_can_int_disable(p_hw_can,
                                      can_type,
                                      AMHW_ZMF159_CAN_INT_OVER);
        int_mask &= ~AM_CAN_INT_WAKE_UP;
    }

    if ((0 != int_mask) && (p_dev->p_devinfo->type ==
                                     AMHW_ZMF159_CAN_BASIC_CAN)) {
        /* 运行至这里表明处于BASIC CAN模式使能BASIC CAN不支持的中断 */
        return AM_CAN_INVALID_PARAMETER;

    } else if (0 == int_mask) {

        return AM_CAN_NOERROR;
    }

    if (int_mask & AM_CAN_INT_ERROR_PASSIVE) {
        /* 使能peli can 消极错误中断 */
        amhw_zmf159_can_int_disable(p_hw_can,
                                      can_type,
                                      AMHW_ZMF159_CAN_INT_ERR_PASSIVE);
    }

    if (int_mask & AM_CAN_INT_BUS_OFF) {
        /* 禁能总线错误中断 */
        amhw_zmf159_can_int_disable(p_hw_can,
                                      can_type,
                                      AMHW_ZMF159_CAN_INT_BUS_ERR);
    }

    /* 恢复CAN状态 */
    if (mode != AMHW_ZMF159_CAN_MODE_RESET) {

        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 模式设置
 */
am_can_err_t __can_mode_set (void *p_drv, am_can_mode_type_t mode)
{
    am_zmf159_can_dev_t *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t   *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    if (AM_CAN_MODE_NROMAL == mode) {

        /* 正常模式 */
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   AMHW_ZMF159_CAN_MODE_RUN);

    } else if ((AM_CAN_MODE_LISTEN_ONLY == mode) &&
                 (p_dev->p_devinfo->type == AMHW_ZMF159_CAN_PELI_CAN)) {

        /* 只听模式 */
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   AMHW_ZMF159_CAN_MODE_ONLY_LISTEN);
    } else {
        /* basic can无只听模式 */
        return AM_CAN_INVALID_PARAMETER;
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 波特率设置
 */
am_can_err_t __can_baudrate_set (void               *p_drv,
                                 am_can_bps_param_t *p_can_baudrate)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;
    amhw_zmf159_can_mode_t mode = p_dev->mode;

    if (NULL == p_drv || NULL == p_can_baudrate) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    if (AMHW_ZMF159_CAN_MODE_RESET != p_dev->mode) {
        /* 波特率寄存器只能在复位模式下进行访问作，设置前进入复位模式*/
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   AMHW_ZMF159_CAN_MODE_RESET);
    }

    amhw_zmf159_can_brp_set(p_hw_can, p_can_baudrate->brp);
    amhw_zmf159_can_sjw_set(p_hw_can, p_can_baudrate->sjw);
    amhw_zmf159_can_sam_set(p_hw_can, p_can_baudrate->smp);

    amhw_zmf159_can_tseg1_set(p_hw_can, p_can_baudrate->tesg1);
    amhw_zmf159_can_tseg2_set(p_hw_can, p_can_baudrate->tesg2);

    if (mode != AMHW_ZMF159_CAN_MODE_RESET) {
        /* 切换回正常模式 */
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 波特率获取
 */
am_can_err_t __can_baudrate_get (void               *p_drv,
                                 am_can_bps_param_t *p_can_baudrate)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;
    amhw_zmf159_can_mode_t mode = p_dev->mode;

    if (NULL == p_drv || NULL == p_can_baudrate) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    if (AMHW_ZMF159_CAN_MODE_RESET != p_dev->mode) {
        /* 波特率寄存器只能在复位模式下进行访问作，设置前进入复位模式*/
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   AMHW_ZMF159_CAN_MODE_RESET);
    }

    p_can_baudrate->brp = amhw_zmf159_can_brp_get(p_hw_can);
    p_can_baudrate->sjw = amhw_zmf159_can_sjw_get(p_hw_can);
    p_can_baudrate->smp = amhw_zmf159_can_sam_get(p_hw_can);

    p_can_baudrate->tesg1 = amhw_zmf159_can_tseg1_get(p_hw_can);
    p_can_baudrate->tesg2 = amhw_zmf159_can_tseg2_get(p_hw_can);

    if (mode != AMHW_ZMF159_CAN_MODE_RESET) {
        /* 切换回正常模式 */
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 获取错误计数
 */
am_can_err_t __can_err_cnt_get (void *p_drv, am_can_err_cnt_t *p_can_err_reg)
{
    am_zmf159_can_dev_t *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t   *p_hw_can = NULL;

    if (NULL == p_drv || NULL == p_can_err_reg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    /* basic CAN没有错误计数器 */
    if (AMHW_ZMF159_CAN_PELI_CAN != p_dev->p_devinfo->regbase) {
        return AM_CAN_INCOMPATIBLE_TYPE;
    }

    /* 读peli CAN TX/RX错误计数器 */
    p_can_err_reg->rx_error_cnt = amhw_zmf159_peli_can_rxerr_read(p_hw_can);
    p_can_err_reg->tx_error_cnt = amhw_zmf159_peli_can_txerr_read(p_hw_can);

    return AM_CAN_NOERROR;
}

/**
 * \brief 清除错误计数
 */
am_can_err_t __can_err_cnt_clr (void *p_drv)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;
    amhw_zmf159_can_mode_t mode = p_dev->mode;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    if (AMHW_ZMF159_CAN_BASIC_CAN == p_dev->p_devinfo->type) {
        return AM_CAN_INVALID_PARAMETER;
    }

    /* 错误计数器寄存器只能在复位模式下进行访问作，设置前进入复位模式*/
    if (AMHW_ZMF159_CAN_MODE_RESET != p_dev->mode) {

        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   AMHW_ZMF159_CAN_MODE_RESET);
    }

    amhw_zmf159_peli_can_rxerr_write(p_hw_can, 0);
    amhw_zmf159_peli_can_txerr_write(p_hw_can, 0);

    if (mode != AMHW_ZMF159_CAN_MODE_RESET) {
        /* 切换回正常模式 */
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 发送信息
 */
am_can_err_t __can_msg_send (void *p_drv, am_can_message_t *p_txmsg)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;
    amhw_zmf159_can_type_t can_type;
    am_bool_t                is_remote = AM_FALSE;
    am_bool_t                is_extern = AM_FALSE;
    uint8_t                  peli_sff  = 0;
    uint8_t                  i = 0;

    if (NULL == p_drv || NULL == p_txmsg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;
    can_type = p_dev->p_devinfo->type;

    /* 判断发送缓冲区是否被释放 */
    if (0 == (amhw_zmf159_can_status_get(p_hw_can) &
                                              AMHW_ZMF159_CAN_SR_TBS)) {
        return AM_CAN_INVALID_PARAMETER;
    }

    is_remote = ((p_txmsg->flags & AM_CAN_REMOTE_FLAG) ? AM_TRUE : AM_FALSE);
    is_extern = ((p_txmsg->flags & AM_CAN_EXTERN_FLAG) ? AM_TRUE : AM_FALSE);

    /* zmf159 can 没有can fd功能 */
    if (p_txmsg->flags & AM_CAN_FD_CTRL_FLAG) {
        return AM_CAN_INVALID_PARAMETER;
    }

    /* basic can 没有自接收请求 */
    if (((p_txmsg->flags & AM_CAN_SELF_SEND_FLAG) ||
            (p_txmsg->flags & AM_CAN_SELFONCE_SEND_FLAG)) &&
                     (AMHW_ZMF159_CAN_BASIC_CAN == can_type)) {
        return AM_CAN_INVALID_PARAMETER;
    }

    /* basic can 不能发送扩展帧 */
    if ((AMHW_ZMF159_CAN_BASIC_CAN == can_type) && (AM_TRUE == is_extern)) {
        return AM_CAN_INVALID_PARAMETER;
    }

    /* 8字节数据 */
    if (p_txmsg->msglen > 8) {
        p_txmsg->msglen = 8;
    }

    /* basic can 发送数据 */
    if (AMHW_ZMF159_CAN_BASIC_CAN == can_type) {

        /* 11bit id */
        p_txmsg->id &= 0x3ff;

        /* 写入id,rtr,len */
        amhw_zmf159_basic_can_tx_id_write(p_hw_can, p_txmsg->id);
        amhw_zmf159_basic_can_tx_rtr_write(p_hw_can, is_remote);
        amhw_zmf159_basic_can_tx_data_len_set(p_hw_can, p_txmsg->msglen);

        /* 写入数据 */
        for (i = 0 ; i < p_txmsg->msglen ; i++) {
            amhw_zmf159_basic_can_tx_data_write(p_hw_can,
                                                  i,
                                                  p_txmsg->msgdata[i]);
        }
    } else {
        /* peli can 发送数据 */

        if (is_extern) {
            /* 扩展标志 */
            peli_sff |= AMHW_ZMF159_CAN_PELI_SFF_FF;
        }

        if (is_remote) {
            /* 远程标志 */
            peli_sff |= AMHW_ZMF159_CAN_PELI_SFF_RTR;
        }

        peli_sff |= p_txmsg->msglen;
        /* 写入sff寄存器 */
        amhw_zmf159_peli_can_sff_set(p_hw_can, peli_sff);
        /* 设置ID值 */
        amhw_zmf159_peli_can_id_set(p_hw_can, is_extern, p_txmsg->id);
        /* 写入数据 */
        for (i = 0 ; i < p_txmsg->msglen ; i++) {
            amhw_zmf159_peli_can_data_write(p_hw_can,
                                              is_extern,
                                              i,
                                              p_txmsg->msgdata[i]);
        }

        if ((p_txmsg->flags & AM_CAN_SELF_SEND_FLAG) ||
                (p_txmsg->flags & AM_CAN_SELFONCE_SEND_FLAG)) {
            /* 自接收请求 */
            amhw_zmf159_can_cmr_set(p_hw_can, AMHW_ZMF159_CAN_CMR_SRR);
        }
    }

    /* 发送请求 */
    amhw_zmf159_can_cmr_set(p_hw_can, AMHW_ZMF159_CAN_CMR_TR);

    return AM_CAN_NOERROR;
}

/**
 * \brief 接收信息
 */
am_can_err_t __can_msg_recv (void *p_drv, am_can_message_t *p_rxmsg)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;
    amhw_zmf159_can_type_t can_type;
    am_bool_t                is_extern = AM_FALSE;
    uint8_t                  peli_sff  = 0;
    uint8_t                  i;

    if (NULL == p_drv || NULL == p_rxmsg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;
    can_type = p_dev->p_devinfo->type;

    /* 初始化 */
    p_rxmsg->id     = 0;
    p_rxmsg->flags  = 0;
    p_rxmsg->msglen = 0;

    /* 判断接收缓冲区是否为空 */
    if (0 == (amhw_zmf159_can_status_get(p_hw_can) &
                                           AMHW_ZMF159_CAN_SR_RBS)) {
        return AM_CAN_NOERROR;
    }

    /* sasic can接收数据 */
    if (AMHW_ZMF159_CAN_BASIC_CAN == can_type) {

        p_rxmsg->id     = amhw_zmf159_basic_can_rx_id_read(p_hw_can);
        /* 获取远程标志位 */
        if (amhw_zmf159_basic_can_rx_rtr_read(p_hw_can)) {
            p_rxmsg->flags |= AM_CAN_REMOTE_FLAG;
            p_rxmsg->msglen = 0;
            return AM_CAN_NOERROR;
        }
        p_rxmsg->msglen = amhw_zmf159_basic_can_rx_data_len_read(p_hw_can);
        /* 读取数据 */
        for (i = 0 ; i < p_rxmsg->msglen ; i++) {
            p_rxmsg->msgdata[i] =
                     amhw_zmf159_basic_can_rx_data_read(p_hw_can, i);
        }

        /*清除溢出 */
        amhw_zmf159_can_cmr_set(p_hw_can, AMHW_ZMF159_CAN_CMR_RRB);
    } else {
        /* 获取SFF寄存器值 */
        peli_sff = amhw_zmf159_peli_can_sff_get(p_hw_can);
        /* 扩展帧处理 */
        if (peli_sff & AMHW_ZMF159_CAN_PELI_SFF_FF) {
            p_rxmsg->flags |= AM_CAN_EXTERN_FLAG;
            is_extern = AM_TRUE;
        }

        p_rxmsg->id     = amhw_zmf159_peli_can_id_get(p_hw_can, is_extern);

        /* 远程帧处理 */
        if (peli_sff & AMHW_ZMF159_CAN_PELI_SFF_RTR) {
            p_rxmsg->flags |= AM_CAN_REMOTE_FLAG;
            p_rxmsg->msglen = 0;
        }
        /* 获取数据长度 */
        p_rxmsg->msglen = (peli_sff >> AMHW_ZMF159_CAN_PELI_SFF_DLC_SHIFT) &
                                       AMHW_ZMF159_CAN_PELI_SFF_DLC_MASK;
        /* 读数据 */
        for (i = 0 ; i < p_rxmsg->msglen ; i++) {
            p_rxmsg->msgdata[i] =
                    amhw_zmf159_peli_can_data_read(p_hw_can, is_extern, i);
        }

        /*清除溢出 */
        amhw_zmf159_can_cmr_set(p_hw_can, AMHW_ZMF159_CAN_CMR_RRB);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 停止发送信息
 */
am_can_err_t __can_stop_msg_snd (void *p_drv)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    amhw_zmf159_can_cmr_set(p_hw_can, AMHW_ZMF159_CAN_CMR_AT);

    return AM_CAN_NOERROR;
}

/**
 * \brief 设置滤波函数
 */
am_can_err_t __can_filter_tab_set (void    *p_drv,
                                   uint8_t *p_filterbuff,
                                   size_t   lenth)
{
    am_zmf159_can_dev_t    *p_dev     = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can  = NULL;
    amhw_zmf159_can_mode_t  mode       = p_dev->mode;

    if (NULL == p_drv || NULL == p_filterbuff || 0 == lenth) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    /* 滤波设置只能在复位模式下进行 */
    if (AMHW_ZMF159_CAN_MODE_RESET != p_dev->mode) {

        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   AMHW_ZMF159_CAN_MODE_RESET);
    }

    if (AMHW_ZMF159_CAN_BASIC_CAN == p_dev->p_devinfo->type) {

        amhw_zmf159_basic_can_ac_set(p_hw_can, *p_filterbuff);
        /* 如果没有传入掩码则掩码设置为全0 */
        if (1 == lenth) {
            amhw_zmf159_basic_can_am_set(p_hw_can, 0);
        } else {
            amhw_zmf159_basic_can_am_set(p_hw_can, *(p_filterbuff+1));
        }

        return AM_CAN_NOERROR;
    }

    /* peli can */

    if (lenth < 7) {
        return AM_CAN_INVALID_PARAMETER;
    }

    amhw_zmf159_peli_filter_mod_set(p_hw_can, AMHW_ZMF159_CAN_PELI_SINGLE_FILTER);

    amhw_zmf159_peli_can_ac_set(p_hw_can, *(uint32_t *)p_filterbuff);
    amhw_zmf159_peli_can_am_set(p_hw_can, *(uint32_t *)(p_filterbuff+4));

    if (mode != AMHW_ZMF159_CAN_MODE_RESET) {
        /* 切换回正常模式 */
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 设置滤波函数(扩展) 只有一组滤波器 不支持扩展和标准的屏蔽位
 */
am_can_err_t __can_filter_tab_ext_set (void             *p_drv,
                                       am_can_filter_t  *p_filterbuff,
                                       size_t            lenth)
{
    am_zmf159_can_dev_t    *p_dev     = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can  = NULL;
    amhw_zmf159_can_mode_t  mode      = p_dev->mode;
    uint32_t                id        = 0;
    uint32_t                id_mask   = 0;

    /* 只能设置一组滤波器 */
    if (NULL == p_drv || NULL == p_filterbuff || 0 == lenth || lenth > 1) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    /* 滤波设置只能在复位模式下进行 */
    if (AMHW_ZMF159_CAN_MODE_RESET != p_dev->mode) {

        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   AMHW_ZMF159_CAN_MODE_RESET);
    }

    if (AMHW_ZMF159_CAN_BASIC_CAN == p_dev->p_devinfo->type) {

        if ((p_filterbuff->id[0] > 0x7ff) ||
            (p_filterbuff->mask[0] > 0x7ff) ||
            (p_filterbuff->ide != AM_CAN_FRAME_TYPE_STD) ||
            (p_filterbuff->rtr != AM_CAN_FRAME_FORMAT_NOCARE)) {

            /* basic_can下只能设置标准帧 */
            return AM_CAN_ILLEGAL_DATA_LENGTH;
        }

        /* 此模式下mask和id的低三位设置无效 */
        amhw_zmf159_basic_can_ac_set(
                p_hw_can, (uint8_t)(p_filterbuff->id[0] & 0x7f8) >> 3);
        amhw_zmf159_basic_can_am_set(
                p_hw_can, (uint8_t)(((~p_filterbuff->mask[0]) & 0x7f8) >> 3));

        return AM_CAN_NOERROR;
    }

    /* peli can */

    amhw_zmf159_peli_filter_mod_set(p_hw_can, AMHW_ZMF159_CAN_PELI_SINGLE_FILTER);

    /* 根据掩码长度来决定掩码是设置标准帧 还是扩展帧 */
    if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_STD) {
        /* 标准帧 */
        if ((p_filterbuff->id[0] > 0x7ff) ||
            (p_filterbuff->mask[0] > 0x7ff) ) {

            return AM_CAN_ILLEGAL_DATA_LENGTH;
        }
        if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_DATA) {
            id = ((p_filterbuff->id[0] & 0x7ff) << 21);
            id_mask = (((~p_filterbuff->mask[0]) & 0x7ff) << 21) | 0x000fffff;
        } else if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_REMOTE) {
            id = ((p_filterbuff->id[0] & 0x7ff) << 21) | 0x00100000;
            id_mask = (((~p_filterbuff->mask[0]) & 0x7ff) << 21) | 0x000fffff;
        } else if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_NOCARE) {
            id = ((p_filterbuff->id[0] & 0x7ff) << 21);
            id_mask = (((~p_filterbuff->mask[0]) & 0x7ff) << 21) | 0x001fffff;
        } else {
            /* 无效参数 */
            return AM_CAN_ILLEGAL_CONFIG;
        }
    } else if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_EXT) {
        /* 扩展帧 */
        if ((p_filterbuff->id[0] > 0x1fffffff) ||
            (p_filterbuff->mask[0] > 0x1fffffff) ) {

            return AM_CAN_ILLEGAL_DATA_LENGTH;
        }
        if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_DATA) {
            id =((p_filterbuff->id[0] & 0x1fffffff) << 3);
            id_mask = (((~p_filterbuff->mask[0]) & 0x1fffffff) << 3) | 0x03;
        } else if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_REMOTE) {
            id = ((p_filterbuff->id[0] & 0x1fffffff) << 3) | 0x04;
            id_mask = (((~p_filterbuff->mask[0]) & 0x1fffffff) << 3) | 0x03;
        } else if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_NOCARE) {
            id = ((p_filterbuff->id[0] & 0x1fffffff) << 3);
            id_mask = (((~p_filterbuff->mask[0]) & 0x1fffffff) <<3) | 0x07;
        } else {
            return AM_CAN_ILLEGAL_CONFIG;
        }

    } else {
        return AM_CAN_ILLEGAL_CONFIG;
    }
    amhw_zmf159_peli_can_ac_set(p_hw_can,id);
    amhw_zmf159_peli_can_am_set(p_hw_can,id_mask);

    if (mode != AMHW_ZMF159_CAN_MODE_RESET) {
        /* 切换回正常模式 */
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 获取滤波函数
 */
am_can_err_t __can_filter_tab_get (void    *p_drv,
                                   uint8_t *p_filterbuff,
                                   size_t  *p_lenth)
{
    am_zmf159_can_dev_t    *p_dev     = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can  = NULL;
    amhw_zmf159_can_mode_t  mode      = p_dev->mode;
    uint32_t                 *filter    = (uint32_t *)p_filterbuff;

    if (NULL == p_drv || NULL == p_filterbuff || NULL == p_lenth) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    /* 滤波设置只能在复位模式下进行 */
    if (AMHW_ZMF159_CAN_MODE_RESET != p_dev->mode) {

        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   AMHW_ZMF159_CAN_MODE_RESET);
    }

    if (AMHW_ZMF159_CAN_BASIC_CAN == p_dev->p_devinfo->type) {

        *p_filterbuff++ = amhw_zmf159_basic_can_ac_get(p_hw_can);
        *p_filterbuff   = amhw_zmf159_basic_can_am_get(p_hw_can);
        *p_lenth        = 2;

        return AM_CAN_NOERROR;
    }

    /* peli can */

    filter[0] = amhw_zmf159_peli_can_ac_get(p_hw_can);
    filter[1] = amhw_zmf159_peli_can_am_get(p_hw_can);

    *p_lenth = 8;

    if (mode != AMHW_ZMF159_CAN_MODE_RESET) {
        /* 切换回正常模式 */
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 获取滤波函数(扩展) 由于无法判别具体是扩展还是标准帧
 *  采用标准帧和扩展帧分别解析
 */

am_can_err_t __can_filter_tab_ext_get (void             *p_drv,
                                       am_can_filter_t  *p_filterbuff,
                                       size_t           *p_lenth)
{
    am_zmf159_can_dev_t    *p_dev     = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can  = NULL;
    amhw_zmf159_can_mode_t  mode      = p_dev->mode;
    uint32_t                acr_reg   = 0;
    uint32_t                amr_reg   = 0;

    if (NULL == p_drv || NULL == p_filterbuff || NULL == p_lenth) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    /* 滤波设置只能在复位模式下进行 */
    if (AMHW_ZMF159_CAN_MODE_RESET != p_dev->mode) {

        amhw_zmf159_can_mode_set(p_hw_can,
                                 p_dev->p_devinfo->type,
                               &(p_dev->mode),
                                 AMHW_ZMF159_CAN_MODE_RESET);
    }

    if (AMHW_ZMF159_CAN_BASIC_CAN == p_dev->p_devinfo->type) {

        acr_reg = amhw_zmf159_basic_can_ac_get(p_hw_can);
        amr_reg = amhw_zmf159_basic_can_am_get(p_hw_can);

        p_filterbuff->id[0]     = ((acr_reg & 0xff) << 3);
        p_filterbuff->mask[0]   = (((~amr_reg) & 0xff) << 3);
        p_filterbuff->ide = AM_CAN_FRAME_TYPE_STD;
        p_filterbuff->rtr = AM_CAN_FRAME_FORMAT_NOCARE;
       *p_lenth = 1;

        return AM_CAN_NOERROR;
    }

    /* peli can */

    acr_reg = amhw_zmf159_peli_can_ac_get(p_hw_can);
    amr_reg = amhw_zmf159_peli_can_am_get(p_hw_can);

    if (AM_CAN_FRAME_TYPE_STD) {
        p_filterbuff->id[0]   = acr_reg >> 21;
        p_filterbuff->mask[0] = ((~amr_reg) >> 21);
        p_filterbuff->rtr     = ((amr_reg >> 20) & 0x01) ?
                                 (AM_CAN_FRAME_FORMAT_NOCARE):
                                 (((acr_reg >> 20) & 0x01 ) ?
                                         AM_CAN_FRAME_FORMAT_REMOTE:
                                         AM_CAN_FRAME_FORMAT_DATA);
        p_filterbuff->ide     = AM_CAN_FRAME_TYPE_STD;
    }

    if (AM_CAN_FRAME_TYPE_EXT) {
        p_filterbuff++;
        p_filterbuff->id[0]   = acr_reg >> 3;
        p_filterbuff->mask[0] = ((~amr_reg) >> 3);
        p_filterbuff->rtr     = ((amr_reg >> 2) & 0x01) ?
                                 (AM_CAN_FRAME_FORMAT_NOCARE):
                                 (((acr_reg >> 20) & 0x01 ) ?
                                         AM_CAN_FRAME_FORMAT_REMOTE:
                                         AM_CAN_FRAME_FORMAT_DATA);
        p_filterbuff->ide     = AM_CAN_FRAME_TYPE_EXT;
    }

    *p_lenth = 2;

    if (mode != AMHW_ZMF159_CAN_MODE_RESET) {
        /* 切换回正常模式 */
        amhw_zmf159_can_mode_set(p_hw_can,
                                   p_dev->p_devinfo->type,
                                   &(p_dev->mode),
                                   mode);
    }

    return AM_CAN_NOERROR;
}

static void __can_intcb_func_run (am_zmf159_can_dev_t  *p_dev,
                                  am_can_int_type_t      can_int)
{
    uint32_t      temp = (uint32_t)can_int;
    am_pfnvoid_t  intcb_fun = NULL;
    void          *intcb_arg = NULL;
    uint8_t       i = 0;

    if ((NULL == p_dev) || (AM_CAN_INT_ALL == can_int)) {
        return ;
    }

    while (temp >> 1) {
        i++;
        temp = temp >> 1;
    }

    intcb_fun = p_dev->p_devinfo->p_intcb_info[i].pfn_callback;
    intcb_arg = p_dev->p_devinfo->p_intcb_info[i].p_arg;

    if (NULL != intcb_fun) {
        intcb_fun(intcb_arg);
    }
}

/**
 * \brief can中断服务函数
 */
static void __can_int_handle (void *p_drv)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;
    amhw_zmf159_can_type_t  can_type;
    uint8_t                   int_flags = 0;  /* 中断标志 */
    uint8_t                   status    = 0;  /* CAN状态 */

    if (NULL == p_drv) {
        return;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;
    can_type = p_dev->p_devinfo->type;

    int_flags = amhw_zmf159_can_int_status_get(p_hw_can);
    status    = amhw_zmf159_can_status_get(p_hw_can);

    if (AMHW_ZMF159_CAN_BASIC_CAN == can_type) {
        /* basic can 无此中断，且此三位恒为1 */
        int_flags &= ~(AMHW_ZMF159_CAN_INT_ERR_PASSIVE);
        int_flags &= ~(AMHW_ZMF159_CAN_INT_ARBITRATION_LOST);
        int_flags &= ~(AMHW_ZMF159_CAN_INT_BUS_ERR);
    }

    /* 接收中断产生，并且rxfifo不为空 */
    if ((int_flags & AMHW_ZMF159_CAN_INT_RECV) &&
            (status & AMHW_ZMF159_CAN_SR_RBS)) {

        /* 执行接收回调函数 */
        __can_intcb_func_run(p_dev, AM_CAN_INT_RX);
        /* 释放接收缓冲区 */
        amhw_zmf159_can_cmr_set(p_hw_can, AMHW_ZMF159_CAN_CMR_RRB);
    }

    /* 发送中断产生，并且发送完成 */
    if ((int_flags & AMHW_ZMF159_CAN_INT_TRAN) &&
            (status & AMHW_ZMF159_CAN_SR_TCS)) {

        /* 执行接收回调函数 */
        __can_intcb_func_run(p_dev, AM_CAN_INT_TX);
    }

    /* 数据溢出中断产生 */
    if (int_flags & AMHW_ZMF159_CAN_INT_OVER) {

        /* 执行数据溢出中断回调函数 */
        __can_intcb_func_run(p_dev, AM_CAN_INT_DATAOVER);
    }

    /* 唤醒中断产生 */
    if (int_flags & AMHW_ZMF159_CAN_INT_WAKEUP) {
        /* 执行唤醒中断回调函数 */
        __can_intcb_func_run(p_dev, AM_CAN_INT_WAKE_UP);
    }

    /* 消极错误中断产生 */
    if (int_flags & AMHW_ZMF159_CAN_INT_ERR_PASSIVE) {
        /* 执行消极错误中断回调函数 */
        __can_intcb_func_run(p_dev, AM_CAN_INT_ERROR_PASSIVE);
    }

    /* 错误中断、总线错误中断、总线仲裁丢失处理 */
    if ((int_flags & AMHW_ZMF159_CAN_INT_ERR) ||
            (int_flags & AMHW_ZMF159_CAN_INT_BUS_ERR) ||
                (int_flags & AMHW_ZMF159_CAN_INT_ARBITRATION_LOST)) {

        /* 判断总线是否关闭 */
        if (status & AMHW_ZMF159_CAN_SR_BS) {
            /* 执行总线关闭错误中断回调函数 */
            __can_intcb_func_run(p_dev, AM_CAN_INT_BUS_OFF);
        } else {
            /* 执行错误中断回调函数 */
            __can_intcb_func_run(p_dev, AM_CAN_INT_ERROR);
        }
    }
}

/**
 * \brief CAN 状态
 */
am_can_err_t __can_status_get (void              *p_drv,
                               am_can_int_type_t *p_int_type,
                               am_can_bus_err_t  *p_bus_err)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    amhw_zmf159_can_t      *p_hw_can = NULL;
    amhw_zmf159_can_type_t can_type;
    uint8_t                  int_flags = 0;  /* 中断标志 */
    uint8_t                  err_code  = 0;  /* 错误号 */
    uint8_t                  status    = 0;  /* CAN状态 */
    uint8_t                  err       = 0;  /* 错误寄存器值 */

    if (NULL == p_drv || NULL == p_int_type || NULL == p_bus_err) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;
    can_type = p_dev->p_devinfo->type;

    int_flags = amhw_zmf159_can_int_status_get(p_hw_can);
    status    = amhw_zmf159_can_status_get(p_hw_can);

    if (AMHW_ZMF159_CAN_BASIC_CAN == can_type) {
        int_flags = int_flags & 0x1f;
    }

    *p_int_type = 0;

    /* 接收中断 */
    if (int_flags & AMHW_ZMF159_CAN_INT_RECV) {
        *p_int_type |= AM_CAN_INT_RX;
    }

    /* 发送中断 */
    if (int_flags & AMHW_ZMF159_CAN_INT_TRAN) {
        *p_int_type |= AM_CAN_INT_TX;
    }

    /* 数据溢出中断 */
    if (int_flags & AMHW_ZMF159_CAN_INT_OVER) {
        *p_int_type |= AM_CAN_INT_DATAOVER;
    }

    /* 唤醒中断 */
    if (int_flags & AMHW_ZMF159_CAN_INT_WAKEUP) {
        *p_int_type |= AM_CAN_INT_WAKE_UP;
    }

    /* 消极错误中断 */
    if (int_flags & AMHW_ZMF159_CAN_INT_ERR_PASSIVE) {
        *p_int_type |= AM_CAN_INT_ERROR_PASSIVE;
    }

    /* 错误中断、总线错误中断、总线仲裁丢失中断 */
    if ((int_flags & AMHW_ZMF159_CAN_INT_ERR)     ||
        (int_flags & AMHW_ZMF159_CAN_INT_BUS_ERR) ||
        (int_flags & AMHW_ZMF159_CAN_INT_ARBITRATION_LOST)) {

        *p_int_type |= AM_CAN_INT_ERROR;

        /* 判断是否是因为错误计数器达到限制而引起的总线关闭 */
        if ((status & AMHW_ZMF159_CAN_SR_BS)) {
            *p_int_type &= ~AM_CAN_INT_ERROR;
            *p_int_type |= AM_CAN_INT_BUS_OFF;
        }
    }

    /* basic can 不能获取错误代码 */
    if (AMHW_ZMF159_CAN_BASIC_CAN == can_type) {

        /* basic can 发生错误则返回未知错误 */
        if (int_flags & AMHW_ZMF159_CAN_INT_ERR) {
            *p_bus_err |= AM_CAN_BUS_ERR_UNKNOWN;
        } else {
            *p_bus_err  = 0;
        }

        return AM_CAN_NOERROR;
    }

    /* 从寄存器获取错误代码 */
    *p_bus_err = 0;
    err        = amhw_zmf159_peli_can_ecc_get(p_hw_can);
    err_code = (err >> AMHW_ZMF159_CAN_PELI_ECC_ERRC_SHIFT) &
                       AMHW_ZMF159_CAN_PELI_ECC_ERRC_MASK;

    /* 格式错误 */
    if (AMHW_ZMF159_CAN_PELI_ECC_ERRC_MODE == err_code) {
        *p_bus_err |= AM_CAN_BUS_ERR_FORM;
    }

    /* 填充错误 */
    if (AMHW_ZMF159_CAN_PELI_ECC_ERRC_FILL == err_code) {
        *p_bus_err |= AM_CAN_BUS_ERR_STUFF;
    }

    /* 未知错误 */
    if (AMHW_ZMF159_CAN_PELI_ECC_ERRC_OTHER == err_code) {
        *p_bus_err |= AM_CAN_BUS_ERR_UNKNOWN;
    }

    err_code = (err >> AMHW_ZMF159_CAN_PELI_ECC_CODE_SHIFT) &
                       AMHW_ZMF159_CAN_PELI_ECC_CODE_MASK;

    /* ACK错误 */
    if ((err_code == AMHW_ZMF159_CAN_PELI_ECC_CODE_ACK) ||
            (err_code == AMHW_ZMF159_CAN_PELI_ECC_CODE_ACK_CH)) {
        *p_bus_err |= AM_CAN_BUS_ERR_ACK;
    }

    /* CRC错误 */
    if ((err_code == AMHW_ZMF159_CAN_PELI_ECC_CODE_CRC) ||
            (err_code == AMHW_ZMF159_CAN_PELI_ECC_CODE_CRC_ARRAY)) {
        *p_bus_err |= AM_CAN_BUS_ERR_CRC;
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断连接
 */
am_can_err_t __can_connect (void         *p_drv,
                            am_pfnvoid_t  pfn_isr,
                            void         *p_arg)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    if (NULL != pfn_isr) {
        /* 连接传入函数 */
        am_int_connect(p_dev->p_devinfo->int_num, pfn_isr, p_arg);
    } else {
        /* 连接默认函数 */
        am_int_connect(p_dev->p_devinfo->int_num, __can_int_handle, p_dev);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 删除中断连接
 */
am_can_err_t __can_disconnect (void         *p_drv,
                               am_pfnvoid_t  pfn_isr,
                               void         *p_arg)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    /* 中断禁能 */
    am_int_disable(p_dev->p_devinfo->int_num);

    if (NULL != pfn_isr) {
        /* 删除连接函数 */
        am_int_disconnect(p_dev->p_devinfo->int_num, pfn_isr, p_arg);
    } else {
        /* 删除连接函数 */
        am_int_disconnect(p_dev->p_devinfo->int_num, __can_int_handle, p_dev);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断回调函数的连接
 */
am_can_err_t __can_intcb_connect (void              *p_drv,
                                  am_can_int_type_t  inttype,
                                  am_pfnvoid_t       pfn_callback,
                                  void              *p_arg)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    uint8_t  i = 0;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    while (inttype >> 1) {
        i++;
        inttype = inttype >> 1;
    }

    if (i >= p_dev->p_devinfo->p_intcb_num) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_dev->p_devinfo->p_intcb_info[i].pfn_callback = pfn_callback;
    p_dev->p_devinfo->p_intcb_info[i].p_arg        = p_arg;

    return AM_CAN_NOERROR;
}

/**
 * \brief 删除中断回调函数的连接
 */
am_can_err_t __can_intcb_disconnect (void *p_drv, am_can_int_type_t  inttype)
{
    am_zmf159_can_dev_t    *p_dev    = (am_zmf159_can_dev_t *)p_drv;
    uint8_t  i = 0;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    while (inttype >> 1) {
        i++;
        inttype = inttype >> 1;
    }

    if (i >= p_dev->p_devinfo->p_intcb_num) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_dev->p_devinfo->p_intcb_info[i].pfn_callback = NULL;
    p_dev->p_devinfo->p_intcb_info[i].p_arg        = NULL;

    return AM_CAN_NOERROR;
}

/**
 * \brief  CAN初始化
 */
am_can_handle_t am_zmf159_can_init (am_zmf159_can_dev_t           *p_dev,
                               const am_zmf159_can_devinfo_t *p_devinfo)
{
    amhw_zmf159_can_t *p_hw_can = NULL;
    uint8_t             i = 0;

    if (NULL == p_dev || NULL == p_devinfo) {
        return NULL;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_devinfo->regbase;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo = p_devinfo;

    p_dev->handle.p_drv_funcs = &dev_func;
    p_dev->handle.p_drv       = (void *)p_dev;

    p_dev->mode               = AMHW_ZMF159_CAN_MODE_RESET;

    amhw_zmf159_can_type_set(p_hw_can, p_devinfo->type);

    p_hw_can->cmr = 0x00;

    for (i = 0 ; i < p_devinfo->p_intcb_num ; i++) {
        p_devinfo->p_intcb_info[i].pfn_callback = NULL;
        p_devinfo->p_intcb_info[i].p_arg        = NULL;
    }

    return (am_can_handle_t)&(p_dev->handle);
}

/**
 * \brief 解初始化
 */
void am_zmf159_can_deinit (am_can_handle_t handle)
{
    amhw_zmf159_can_t *p_hw_can = NULL;
    am_zmf159_can_dev_t  *p_dev = (am_zmf159_can_dev_t *)handle->p_drv;

    if (NULL == p_dev) {
        return;
    }

    p_hw_can = (amhw_zmf159_can_t *)p_dev->p_devinfo->regbase;

    amhw_zmf159_can_mode_set(p_hw_can,
                               p_dev->p_devinfo->type,
                               &(p_dev->mode),
                               AMHW_ZMF159_CAN_MODE_RESET);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    /* 中断禁能 */
    am_int_disable(p_dev->p_devinfo->int_num);
}

/* end of file */
