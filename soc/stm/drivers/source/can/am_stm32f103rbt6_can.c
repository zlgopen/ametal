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
 * - 1.00 19-07-31  zc, first implementation
 * - 1.01 19-12-18  add filter table extern api
 * \endinternal
 */

#include "am_int.h"
#include "am_can.h"
#include "am_stm32f103rbt6_can.h"
#include "hw/amhw_stm32f103rbt6_can.h"
#include "stm32f103rbt6_inum.h"

/** \brief CAN启动 */
am_can_err_t __can_stm32f103rbt6_start (void *p_drv);
/** \brief CAN复位（停止） */
am_can_err_t __can_stm32f103rbt6_reset (void *p_drv);
/** \brief CAN休眠  */
am_can_err_t __can_stm32f103rbt6_sleep (void *p_drv);
/** \brief CAN唤醒 */
am_can_err_t __can_stm32f103rbt6_wakeup (void *p_drv);
/** \brief 中断使能 */
am_can_err_t __can_stm32f103rbt6_int_enable (void *p_drv, am_can_int_type_t int_mask);
/** \brief 中断禁能*/
am_can_err_t __can_stm32f103rbt6_int_disable (void *p_drv, am_can_int_type_t int_mask);
/** \brief 模式设置 */
am_can_err_t __can_stm32f103rbt6_mode_set (void *p_drv, am_can_mode_type_t mode);
/** \brief 波特率设置 */
am_can_err_t __can_stm32f103rbt6_baudrate_set (void               *p_drv,
                                 am_can_bps_param_t *p_can_baudrate);
/** \brief 波特率获取 */
am_can_err_t __can_stm32f103rbt6_baudrate_get (void               *p_drv,
                                 am_can_bps_param_t *p_can_baudrate);
/** \brief 获取错误计数 */
am_can_err_t __can_stm32f103rbt6_err_cnt_get (void         *p_drv,
                                 am_can_err_cnt_t   *p_can_err_reg);
/** \brief 发送信息   */
am_can_err_t __can_stm32f103rbt6_msg_send (void *p_drv, am_can_message_t *p_txmsg);
/** \brief 接收信息   */
am_can_err_t __can_stm32f103rbt6_msg_recv (void *p_drv, am_can_message_t *p_rxmsg);
/** \brief 停止发送信息   */
am_can_err_t __can_stm32f103rbt6_stop_msg_snd (void *p_drv);

/** \brief CAN 状态 */
am_can_err_t __can_stm32f103rbt6_status_get (void              *p_drv,
                               am_can_int_type_t *p_int_type,
                               am_can_bus_err_t  *p_bus_err);
/** \brief 中断连接 */
am_can_err_t __can_stm32f103rbt6_connect (void         *p_drv,
                            am_pfnvoid_t  pfn_isr,
                            void         *p_arg);
/** \brief 删除中断连接 */
am_can_err_t __can_stm32f103rbt6_disconnect (void         *p_drv,
                               am_pfnvoid_t  pfn_isr,
                               void         *p_arg);
/** \brief 中断回调函数的连接 */
am_can_err_t __can_stm32f103rbt6_intcb_connect (void              *p_drv,
                                  am_can_int_type_t  inttype,
                                  am_pfnvoid_t       pfn_callback,
                                  void              *p_arg);


/** \brief 删除中断回调函数的连接 */
am_can_err_t __can_stm32f103rbt6_intcb_disconnect (void              *p_drv,
                                            am_can_int_type_t  inttype);

/** \brief 设置滤波函数 (扩展)*/
am_can_err_t __can_stm32f103rbt6_filter_tab_ext_set (void            *p_drv,
                                              am_can_filter_t *p_filterbuff,
                                              size_t           lenth);
/** \brief 获取滤波函数 (扩展)*/
am_can_err_t __can_stm32f103rbt6_filter_tab_ext_get (void             *p_drv,
                                              am_can_filter_t  *p_filterbuff,
                                              size_t           *p_lenth);

struct am_can_drv_funcs __stm32f103rbt6_can_dev_func = {
    __can_stm32f103rbt6_start,
    __can_stm32f103rbt6_reset,
    __can_stm32f103rbt6_sleep,
    __can_stm32f103rbt6_wakeup,
    __can_stm32f103rbt6_int_enable,
    __can_stm32f103rbt6_int_disable,
    __can_stm32f103rbt6_mode_set,
    __can_stm32f103rbt6_baudrate_set,
    __can_stm32f103rbt6_baudrate_get,
    __can_stm32f103rbt6_err_cnt_get,
    NULL,
    __can_stm32f103rbt6_msg_send,
    __can_stm32f103rbt6_msg_recv,
    __can_stm32f103rbt6_stop_msg_snd,
    NULL,
    NULL,
    __can_stm32f103rbt6_status_get,
    __can_stm32f103rbt6_connect,
    __can_stm32f103rbt6_disconnect,
    __can_stm32f103rbt6_intcb_connect,
    __can_stm32f103rbt6_intcb_disconnect,
    __can_stm32f103rbt6_filter_tab_ext_set,
    __can_stm32f103rbt6_filter_tab_ext_get,
};

/**
 * \brief CAN初始化
 */
uint8_t __can_stm32f103rbt6_init (void *p_drv)
{
    am_stm32f103rbt6_can_dev_t        *p_dev       = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t          *p_hw_can    = NULL;
    am_stm32f103rbt6_can_init_info_t  *p_can_info  = NULL;
    uint8_t                     init_staus  = 0;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_can_info =(am_stm32f103rbt6_can_init_info_t *)p_dev->p_devinfo->init_info;
    p_hw_can   =(amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    /* 退出睡眠   请求进入初始化模式 */
    if (amhw_stm32f103rbt6_can_operating_mode_request(
            p_hw_can, AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT)) {

        /* 设置时间触发通信模式  */
        if (p_can_info->can_ttcm) {
            amhw_stm32f103rbt6_can_ttcom_mode_set(p_hw_can, 1);
        } else {
            amhw_stm32f103rbt6_can_ttcom_mode_set(p_hw_can, 0);
        }

        /* 设置软件自动离线管理  */
        if (p_can_info->can_abom) {
            p_hw_can->mcr |= AMHW_STM32F103RBT6_CAN_MCR_ABOM;
        } else {
            p_hw_can->mcr &= (~AMHW_STM32F103RBT6_CAN_MCR_ABOM);
        }

        /* 设置自动唤醒模式 */
        if (p_can_info->can_awum) {
            p_hw_can->mcr |= AMHW_STM32F103RBT6_CAN_MCR_AWUM;
        } else {
            p_hw_can->mcr &= (~AMHW_STM32F103RBT6_CAN_MCR_AWUM);
        }

        /* 设置 禁止报文自动重传  */
        if (p_can_info->can_nart) {
            p_hw_can->mcr |= AMHW_STM32F103RBT6_CAN_MCR_NART;
        } else {
            p_hw_can->mcr &= (~AMHW_STM32F103RBT6_CAN_MCR_NART);
        }

        /* 设置 接收FIFO锁定模式 */
        if (p_can_info->can_rflm) {
            p_hw_can->mcr |= AMHW_STM32F103RBT6_CAN_MCR_RFLM;
        } else {
            p_hw_can->mcr &= (~AMHW_STM32F103RBT6_CAN_MCR_RFLM);
        }

        /* 设置 发送FIFP优先级 */
        if (p_can_info->can_txfp) {
            p_hw_can->mcr |= AMHW_STM32F103RBT6_CAN_MCR_TXFP;
        } else {
            p_hw_can->mcr &= (~AMHW_STM32F103RBT6_CAN_MCR_TXFP);
        }


        p_hw_can->btr |= (uint32_t)(((uint32_t) p_can_info->can_mode << 30));

        /* 清除唤醒挂号标志位  */
        p_hw_can->msr |= (uint32_t)0x08;

        /* 进入正常模式 */
        if (amhw_stm32f103rbt6_can_operating_mode_request(
                    p_hw_can, AMHW_STM32F103RBT6_CAN_OPERATING_NORMAL)) {
            init_staus = 1;
        } else {
            init_staus = 0;
        }

    } else {
        init_staus = 0;
    }

    return init_staus;
}


/**
 * \brief CAN启动
 */
am_can_err_t __can_stm32f103rbt6_start (void *p_drv)
{

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    /* 初始化CAN控制信息 */
    __can_stm32f103rbt6_init(p_drv);

    /* 使能中断 */
    am_int_enable(INUM_USB_HP_CAN_TX);
    am_int_enable(INUM_USB_HP_CAN_RX);
    am_int_enable(INUM_CAN1_RX1);
    am_int_enable(INUM_CAN1_SCE);

    return AM_CAN_NOERROR;
}

/**
 * \brief CAN复位（停止）
 */
am_can_err_t __can_stm32f103rbt6_reset (void *p_drv)
{
    am_stm32f103rbt6_can_dev_t    *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    /* 对CAN强制复位  复位后CAN进入睡眠模式 */
    p_hw_can->mcr |= AMHW_STM32F103RBT6_CAN_MCR_RESET;


    /* 禁能中断 */
    am_int_disable(INUM_USB_HP_CAN_TX);
    am_int_disable(INUM_USB_HP_CAN_RX);
    am_int_disable(INUM_CAN1_RX1);
    am_int_disable(INUM_CAN1_SCE);

    return AM_CAN_NOERROR;
}

/**
 * \brief CAN休眠
 */
am_can_err_t __can_stm32f103rbt6_sleep (void *p_drv)
{
    am_stm32f103rbt6_can_dev_t    *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    /* 切换到睡眠模式*/
    amhw_stm32f103rbt6_can_operating_mode_request(
            p_hw_can, AMHW_STM32F103RBT6_CAN_OPERATING_SLEEP);

    return AM_CAN_NOERROR;
}

/**
 * \brief CAN唤醒
 */
am_can_err_t __can_stm32f103rbt6_wakeup (void *p_drv)
{
    am_stm32f103rbt6_can_dev_t    *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    /* 切换到睡眠模式*/
    amhw_stm32f103rbt6_can_operating_mode_request(
            p_hw_can, AMHW_STM32F103RBT6_CAN_OPERATING_NORMAL);

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断使能
 */
am_can_err_t __can_stm32f103rbt6_int_enable (void *p_drv, am_can_int_type_t int_mask)
{
    am_stm32f103rbt6_can_dev_t             *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t               *p_hw_can = NULL;
    amhw_stm32f103rbt6_can_mode_t           mode     = p_dev->mode;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    /* 接收中断 */
    if (int_mask & AM_CAN_INT_RX) {

        /* 使能 FIFO 0 1 消息挂号中断使能 */
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_FMPIE0, 1);
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_FMPIE1, 1);

        int_mask &= ~AM_CAN_INT_RX;
    }

    /* 发送中断 */
    if (int_mask & AM_CAN_INT_TX) {

        /* 发送邮箱空中断使能*/
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_TMEIE, 1);

        int_mask &= ~AM_CAN_INT_TX;
    }

    /* 错误\警告中断  */
    if ((int_mask & AM_CAN_INT_ERROR)) {

        /* 错误警告 和 错误中断使能*/
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_ERRIE, 1);

        int_mask &= ~AM_CAN_INT_ERROR;
    }

    /* 警告中断  */
    if ((int_mask & AM_CAN_INT_WARN)) {

        /* 错误警告 和 错误中断使能*/
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_ERRIE, 1);

        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_EWGIE, 1);

        int_mask &= ~AM_CAN_INT_WARN;
    }

    /* 错误被动中断  */
    if ((int_mask & AM_CAN_INT_ERROR_PASSIVE)) {

        /* 错误被动 和 错误中断使能*/
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_ERRIE, 1);

        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_EPVIE, 1);


        int_mask &= ~AM_CAN_INT_ERROR_PASSIVE;
    }

    /* 总线关闭中断  */
    if ((int_mask & AM_CAN_INT_BUS_OFF)) {

        /* 离线中断 和 错误中断使能*/
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_ERRIE, 1);

        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_BOFIE, 1);

        int_mask &= ~AM_CAN_INT_BUS_OFF;
    }

    /* 唤醒中断 */
    if (int_mask & AM_CAN_INT_WAKE_UP) {

        /* 使能唤醒中断 */
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_WKUIE, 1);

        int_mask &= ~AM_CAN_INT_WAKE_UP;
    }

    /* 总线超载中断 */
    if (int_mask & AM_CAN_INT_DATAOVER) {

        /* 使能数据溢出中断  FIFO 0 1 */
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_FOVIE0, 1);
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_FOVIE1, 1);

        int_mask &= ~AM_CAN_INT_DATAOVER;
    }

    /* 恢复CAN状态 */
    if (mode != AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT) {

        amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can, mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断禁能
 */
am_can_err_t __can_stm32f103rbt6_int_disable (void *p_drv, am_can_int_type_t int_mask)
{
    am_stm32f103rbt6_can_dev_t         *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t           *p_hw_can = NULL;
    amhw_stm32f103rbt6_can_mode_t       mode     = p_dev->mode;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;


    /* 接收中断 */
    if (int_mask & AM_CAN_INT_RX) {

        /* 使能 FIFO 0 1 消息挂号中断使能 */
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_FMPIE0, 0);
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_FMPIE1, 0);

        int_mask &= ~AM_CAN_INT_RX;
    }

    /* 发送中断 */
    if (int_mask & AM_CAN_INT_TX) {

        /* 发送邮箱空中断使能*/
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_TMEIE, 0);

        int_mask &= ~AM_CAN_INT_TX;
    }

    /* 错误\警告中断  */
    if ((int_mask & AM_CAN_INT_ERROR)) {

        /* 错误警告 和 错误中断使能*/
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_ERRIE, 0);

        int_mask &= ~AM_CAN_INT_ERROR;
    }

    /* 警告中断  */
    if ((int_mask & AM_CAN_INT_WARN)) {

        /* 错误警告 和 错误中断使能*/
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_ERRIE, 0);
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_EWGIE, 0);

        int_mask &= ~AM_CAN_INT_WARN;
    }

    /* 错误被动中断  */
    if ((int_mask & AM_CAN_INT_ERROR_PASSIVE)) {

        /* 错误被动 和 错误中断使能*/
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_ERRIE, 0);
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_EPVIE, 0);

        int_mask &= ~AM_CAN_INT_ERROR_PASSIVE;
    }

    /* 总线关闭中断  */
    if ((int_mask & AM_CAN_INT_BUS_OFF)) {

        /* 离线中断 和 错误中断使能*/
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_ERRIE, 0);
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_BOFIE, 0);

        int_mask &= ~AM_CAN_INT_BUS_OFF;
    }

    /* 唤醒中断 */
    if (int_mask & AM_CAN_INT_WAKE_UP) {
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_WKUIE, 0);

        int_mask &= ~AM_CAN_INT_WAKE_UP;
    }

    /* 总线超载中断 */
    if (int_mask & AM_CAN_INT_DATAOVER) {
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_FOVIE0, 0);
        amhw_stm32f103rbt6_can_itconfig(p_hw_can,
                AMHW_STM32F103RBT6_CAN_INT_FOVIE1, 0);

        int_mask &= ~AM_CAN_INT_DATAOVER;
    }
    /* 恢复CAN状态 */
    if (mode != AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT) {

        amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can, mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 模式设置
 */
am_can_err_t __can_stm32f103rbt6_mode_set (void *p_drv, am_can_mode_type_t mode)
{
    am_stm32f103rbt6_can_dev_t *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t   *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    if (AM_CAN_MODE_NROMAL == mode ) {

        /* 正常模式 */
        amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can,
                AMHW_STM32F103RBT6_CAN_OPERATING_NORMAL);
    } else if (AM_CAN_MODE_LISTEN_ONLY == mode) {

        /* 静默模式*/
        amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can,
                AMHW_STM32F103RBT6_CAN_MODE_SILENT);
    } else {
        /* 回环 和 回环静默未设置 */

        return AM_CAN_INVALID_PARAMETER;
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 波特率设置
 */
am_can_err_t __can_stm32f103rbt6_baudrate_set (void           *p_drv,
                                 am_can_bps_param_t    *p_can_baudrate)
{
    am_stm32f103rbt6_can_dev_t         *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t           *p_hw_can = NULL;
    amhw_stm32f103rbt6_can_mode_t       mode     = p_dev->mode;

    if (NULL == p_drv || NULL == p_can_baudrate) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    if (AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT != p_dev->mode) {

        /* 波特率寄存器只在初始化模式下可进行访问，设置前进入初始化模式  */
        amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can,
                AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT);
    }

    /* 设置 波特率分频器   同步跳转宽度   和相位段 */
    amhw_stm32f103rbt6_can_brp_set(p_hw_can, p_can_baudrate->brp);
    amhw_stm32f103rbt6_can_sjw_set(p_hw_can, p_can_baudrate->sjw);

    amhw_stm32f103rbt6_can_tseg1_set(p_hw_can, p_can_baudrate->tesg1);
    amhw_stm32f103rbt6_can_tseg2_set(p_hw_can, p_can_baudrate->tesg2);

    if (mode != AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT) {

        /* 切换到正常模式 */
        amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can,
                mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 波特率获取
 */
am_can_err_t __can_stm32f103rbt6_baudrate_get (void           *p_drv,
                                 am_can_bps_param_t    *p_can_baudrate)
{
    am_stm32f103rbt6_can_dev_t         *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t           *p_hw_can = NULL;
    amhw_stm32f103rbt6_can_mode_t       mode     = p_dev->mode;

    if (NULL == p_drv || NULL == p_can_baudrate) {
        return AM_CAN_INVALID_PARAMETER;
    }

    if (AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT != p_dev->mode) {
        /* 波特率寄存器只在初始化模式下可进行访问，设置前进入初始化模式  */
        amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can,
                AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT);
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    p_can_baudrate->brp = amhw_stm32f103rbt6_can_brp_get(p_hw_can);
    p_can_baudrate->sjw = amhw_stm32f103rbt6_can_sjw_get(p_hw_can);

    p_can_baudrate->tesg1 = amhw_stm32f103rbt6_can_tseg1_get(p_hw_can);
    p_can_baudrate->tesg2 = amhw_stm32f103rbt6_can_tseg2_get(p_hw_can);


    if (mode != AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT) {

        /* 切换到正常模式 */
        amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can,
                mode);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 获取错误计数
 */
am_can_err_t __can_stm32f103rbt6_err_cnt_get (void        *p_drv,
                               am_can_err_cnt_t    *p_can_err_reg)
{
    am_stm32f103rbt6_can_dev_t *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t   *p_hw_can = NULL;

    if (NULL == p_drv || NULL == p_can_err_reg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    /* 读 TX/RX错误计数器 */
    p_can_err_reg->rx_error_cnt = amhw_stm32f103rbt6_can_get_recverrorcount(p_hw_can);
    p_can_err_reg->tx_error_cnt = amhw_stm32f103rbt6_can_get_transerrorcount(p_hw_can);

    return AM_CAN_NOERROR;
}


/**
 * \brief 发送信息
 */
am_can_err_t __can_stm32f103rbt6_msg_send (void *p_drv, am_can_message_t *p_txmsg)
{
    am_stm32f103rbt6_can_dev_t    *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t      *p_hw_can = NULL;
    am_bool_t               is_remote = AM_FALSE;
    am_bool_t               is_extern = AM_FALSE;

    uint8_t transmit_mailbox = 0;

    if (NULL == p_drv || NULL == p_txmsg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;


    if ((p_hw_can->tsr & AMHW_STM32F103RBT6_CAN_TSR_TME0) ==
                         AMHW_STM32F103RBT6_CAN_TSR_TME0) {

        transmit_mailbox = 0;
    } else if ((p_hw_can->tsr & AMHW_STM32F103RBT6_CAN_TSR_TME1) ==
                                AMHW_STM32F103RBT6_CAN_TSR_TME1) {
        transmit_mailbox = 1;
    } else if ((p_hw_can->tsr & AMHW_STM32F103RBT6_CAN_TSR_TME2) ==
                                AMHW_STM32F103RBT6_CAN_TSR_TME2) {
        transmit_mailbox = 2;
    } else {

        /* 发送邮箱不为空  */
        transmit_mailbox = STM32F103RBT6_CAN_TX_STATUS_NOMAILBOX;
    }

    is_remote = ((p_txmsg->flags & AM_CAN_REMOTE_FLAG) ? AM_TRUE : AM_FALSE);
    is_extern = ((p_txmsg->flags & AM_CAN_EXTERN_FLAG) ? AM_TRUE : AM_FALSE);

    /* stm32f103rbt6 can 没有can fd功能 */
    if (p_txmsg->flags & AM_CAN_FD_CTRL_FLAG) {
        return AM_CAN_INVALID_PARAMETER;
    }

    /* 8字节数据 */
     if (p_txmsg->msglen > 8) {
         p_txmsg->msglen = 8;
     }

    if (transmit_mailbox != STM32F103RBT6_CAN_TX_STATUS_NOMAILBOX) {

        /* 清空寄存器*/
        p_hw_can->tx_mail[transmit_mailbox].tir &= AMHW_STM32F103RBT6_CAN_TIR_TXRQ;

        /* 扩展标识符 */
        if (is_extern) {

            /* 设置扩展标识符ID 和 IDE位 */
            p_hw_can->tx_mail[transmit_mailbox].tir |= ((p_txmsg->id << 3)|
                                                        (uint32_t)0x4);
        } else {

            /* 设置标准标识符ID 和 IDE位 */
            p_hw_can->tx_mail[transmit_mailbox].tir |= (p_txmsg->id << 21);
            p_hw_can->tx_mail[transmit_mailbox].tir &= (~(uint32_t)0x04);
        }

        /* 远程标志 */
        if (is_remote) {
            /* 远程帧 */
            p_hw_can->tx_mail[transmit_mailbox].tir |= (uint32_t)0x02;
        } else {
            /* 数据帧 */
            p_hw_can->tx_mail[transmit_mailbox].tir &= (~(uint32_t)0x02);
        }

        /* 设置 数据长度*/
        p_hw_can->tx_mail[transmit_mailbox].tdtr &= (uint32_t)0xfffffff0;
        p_hw_can->tx_mail[transmit_mailbox].tdtr |= p_txmsg->msglen ;

            p_hw_can->tx_mail[transmit_mailbox].tdlr =
                    (((uint32_t)p_txmsg->msgdata[3] << 24) |
                     ((uint32_t)p_txmsg->msgdata[2] << 16) |
                     ((uint32_t)p_txmsg->msgdata[1] << 8)  |
                     ((uint32_t)p_txmsg->msgdata[0]));

            p_hw_can->tx_mail[transmit_mailbox].tdhr =
                    (((uint32_t)p_txmsg->msgdata[7] << 24) |
                     ((uint32_t)p_txmsg->msgdata[6] << 16) |
                     ((uint32_t)p_txmsg->msgdata[5] << 8)  |
                     ((uint32_t)p_txmsg->msgdata[4]));

        /* 请求传输 */
        p_hw_can->tx_mail[transmit_mailbox].tir |= AMHW_STM32F103RBT6_CAN_TIR_TXRQ;
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 接收信息
 */
am_can_err_t __can_stm32f103rbt6_msg_recv (void *p_drv, am_can_message_t *p_rxmsg)
{
    am_stm32f103rbt6_can_dev_t       *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_filter_t  *p_filter = NULL;
    amhw_stm32f103rbt6_can_t         *p_hw_can = NULL;
    uint8_t         rx_mailbox = 0;

    if (NULL == p_drv || NULL == p_rxmsg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_filter= (amhw_stm32f103rbt6_can_filter_t *)p_dev->p_devinfo->filter;
    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    /* 初始化 */
    p_rxmsg->id     = 0;
    p_rxmsg->flags  = 0;
    p_rxmsg->msglen = 0;

    /* 判断接收缓冲区是否为空 */
    if (p_filter->can_fliter_fifo  == AMHW_STM32F103RBT6_CAN_FILTER_FIFO0 ) {
        if ((p_hw_can->rf_0r & (uint32_t)0x3)) {
            rx_mailbox = 0;
        } else {
            return AM_CAN_NOERROR;
        }
    } else if (p_filter->can_fliter_fifo  == AMHW_STM32F103RBT6_CAN_FILTER_FIFO1) {
        if ((p_hw_can->rf_1r & (uint32_t)0x3)) {
            rx_mailbox = 1;
        } else {
            return AM_CAN_NOERROR;
        }
    }

    if (p_hw_can->rx_mail[rx_mailbox].rir & (uint8_t)0x04 ) {

        /* 使用扩展标识符  */
        p_rxmsg->flags |= AM_CAN_EXTERN_FLAG;

        /* 获取ID */
        p_rxmsg->id     = p_hw_can->rx_mail[rx_mailbox].rir >> 3;

    } else {
        /* 使用标准标识符  */

        /* 获取ID */
        p_rxmsg->id     = p_hw_can->rx_mail[rx_mailbox].rir >> 21;
    }

    /* 远程帧处理 */
    if (p_hw_can->rx_mail[rx_mailbox].rir & 0x02) {

        /* 远程帧 */
        p_rxmsg->flags |= AM_CAN_REMOTE_FLAG;
        p_rxmsg->msglen = 0;
    }

        /* 获取发送数据长度*/
    p_rxmsg->msglen = (uint8_t)0x0f &
                       p_hw_can->rx_mail[rx_mailbox].rdtr;



    /* 获取过滤器匹配序号 */
    p_dev->fmi = (uint8_t)0xff & (p_hw_can->rx_mail[rx_mailbox].rdtr >> 8);

        /* 获取数据 */
        p_rxmsg->msgdata[0] = (uint8_t)0xff &
                              (p_hw_can->rx_mail[rx_mailbox].rdlr);
        p_rxmsg->msgdata[1] = (uint8_t)0xff &
                              (p_hw_can->rx_mail[rx_mailbox].rdlr >> 8);
        p_rxmsg->msgdata[2] = (uint8_t)0xff &
                              (p_hw_can->rx_mail[rx_mailbox].rdlr >> 16);
        p_rxmsg->msgdata[3] = (uint8_t)0xff &
                              (p_hw_can->rx_mail[rx_mailbox].rdlr >> 24);

        p_rxmsg->msgdata[4] = (uint8_t)0xff &
                              (p_hw_can->rx_mail[rx_mailbox].rdhr);
        p_rxmsg->msgdata[5] = (uint8_t)0xff &
                              (p_hw_can->rx_mail[rx_mailbox].rdhr >> 8);
        p_rxmsg->msgdata[6] = (uint8_t)0xff &
                              (p_hw_can->rx_mail[rx_mailbox].rdhr >> 16);
        p_rxmsg->msgdata[7] = (uint8_t)0xff &
                              (p_hw_can->rx_mail[rx_mailbox].rdhr >> 24);

    /* 释放FIFO  */
    /* 释放FIFO 0 */
    if (rx_mailbox == 0) {
        p_hw_can->rf_0r |= AMHW_STM32F103RBT6_CAN_RF0R_RFOM0;
    } else {
    /* 释放FIFO 1 */
        p_hw_can->rf_1r |= AMHW_STM32F103RBT6_CAN_RF1R_RFOM1;
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 停止发送信息
 */
am_can_err_t __can_stm32f103rbt6_stop_msg_snd (void *p_drv)
{
    am_stm32f103rbt6_can_dev_t    *p_dev            = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t      *p_hw_can         = NULL;
    uint8_t                 transmit_mailbox = 0;
    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    if ((p_hw_can->tsr & AMHW_STM32F103RBT6_CAN_TSR_TME0) !=
                         AMHW_STM32F103RBT6_CAN_TSR_TME0) {

        transmit_mailbox = 0;
    } else if ((p_hw_can->tsr & AMHW_STM32F103RBT6_CAN_TSR_TME1) !=
                                AMHW_STM32F103RBT6_CAN_TSR_TME1) {
        transmit_mailbox = 1;
    } else if ((p_hw_can->tsr & AMHW_STM32F103RBT6_CAN_TSR_TME2) !=
                                AMHW_STM32F103RBT6_CAN_TSR_TME2) {
        transmit_mailbox = 2;
    } else {
        /* 所有邮箱为空  */
        return AM_CAN_INVALID_PARAMETER;
    }

    switch (transmit_mailbox) {

    case (0):
        p_hw_can->tsr |= AMHW_STM32F103RBT6_CAN_TSR_ABRQ0;
        break;

    case (1):
        p_hw_can->tsr |= AMHW_STM32F103RBT6_CAN_TSR_ABRQ1;
        break;

    case (2):
        p_hw_can->tsr |= AMHW_STM32F103RBT6_CAN_TSR_ABRQ2;
        break;

    default:
        break;
    }

    return AM_CAN_NOERROR;
}

am_can_err_t __can_stm32f103rbt6_filter_32bit_idmask(uint32_t         *reg_1,
                                              uint32_t         *reg_2,
                                              am_can_filter_t  *p_filterbuff)
{
    uint32_t reg_f_r1 = 0;
    uint32_t reg_f_r2 = 0;

    uint8_t  state = AM_CAN_NOERROR;
    if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_DATA ) {
        /* 数据帧 */
        reg_f_r1 &= (~(uint32_t)(1 << 1));
        reg_f_r2 |= ( (uint32_t)(1 << 1));
    } else if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_REMOTE) {
        /* 远程帧 */
        reg_f_r1 |= ((uint32_t)(1 << 1));
        reg_f_r2 |= ((uint32_t)(1 << 1));
    } else if(p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_NOCARE) {
        /* 都可以接收 */
        reg_f_r2 &= (~(uint32_t)(1 << 1));
    } else {
        /* 无此选项 */
        state = AM_CAN_INVALID_PARAMETER;
        return state;
    }

    if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_STD) {
        /* 只接收标准帧 */
        reg_f_r1 &= (~(uint32_t)(1 << 2));
        reg_f_r2 |= ( (uint32_t)(1 << 2));

        /* 标准ID STID[10:0] */
        if ((p_filterbuff->id[0] >0x7ff) || (p_filterbuff->mask[0] >0x7ff) ) {
            state = AM_CAN_INVALID_PARAMETER;
            return state;
        }

        reg_f_r1 |= ((uint32_t)p_filterbuff->id[0] << 21);
        reg_f_r2 |= ((uint32_t)p_filterbuff->mask[0] << 21);
    } else if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_EXT) {
        /* 只接收扩展帧 */
        reg_f_r1 |= ((uint32_t)(1 << 2));
        reg_f_r2 |= ((uint32_t)(1 << 2));

        /* EXID[28:0] */
        if ((p_filterbuff->id[0] > 0x1fffffff) ||
            (p_filterbuff->mask[0] > 0x1fffffff)) {
            state = AM_CAN_INVALID_PARAMETER;
            return state;
        }

        reg_f_r1 |= ((uint32_t)p_filterbuff->id[0] << 3);
        reg_f_r2 |= ((uint32_t)p_filterbuff->mask[0] << 3);
    } else {
        /* 无此选项 */
        state = AM_CAN_INVALID_PARAMETER;
        return state;
    }

    *reg_1 = reg_f_r1;
    *reg_2 = reg_f_r2;

    return state;
}

am_can_err_t __can_stm32f103rbt6_filter_32bit_idlist(uint32_t        *reg_1,
                                              uint32_t        *reg_2,
                                              am_can_filter_t *p_filterbuff)
{
    uint32_t reg_f_r1 = 0;
    uint32_t reg_f_r2 = 0;
    uint8_t  state    = AM_CAN_NOERROR;

    if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_DATA ) {
        /* 数据帧 */
        reg_f_r1 &= (~(uint32_t)(1 << 1));
        reg_f_r2 &= (~(uint32_t)(1 << 1));
    } else if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_REMOTE) {
        /* 远程帧 */
        reg_f_r1 |= ((uint32_t)(1 << 1));
        reg_f_r2 |= ((uint32_t)(1 << 1));
    } else {
        /* 列表模式下 返回参数错误 */
        state = AM_CAN_INVALID_PARAMETER;
        return state;
    }

    if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_STD) {
        /* 只接收标准帧 */
        reg_f_r1 &= (~(uint32_t)(1 << 2));
        reg_f_r2 &= (~(uint32_t)(1 << 2));

        /* 标准ID STID[10:0] */
        if ((p_filterbuff->id[0] >0x7ff) || (p_filterbuff->id[1] > 0x7ff)) {
            state = AM_CAN_INVALID_PARAMETER;
            return state;
        }

        reg_f_r1 |= ((uint32_t)p_filterbuff->id[0] << 21);
        reg_f_r2 |= ((uint32_t)p_filterbuff->id[1] << 21);
    } else if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_EXT) {
        /* 只接收扩展帧 */
        reg_f_r1 |= ((uint32_t)(1 << 2));
        reg_f_r2 |= ((uint32_t)(1 << 2));

        /* EXID[28:0] */
        if ((p_filterbuff->id[0] > 0x1fffffff) ||
            (p_filterbuff->id[1] > 0x1fffffff) ) {
            state = AM_CAN_INVALID_PARAMETER;
            return state;
        }

        reg_f_r1 |= ((uint32_t)p_filterbuff->id[0] << 3);
        reg_f_r2 |= ((uint32_t)p_filterbuff->id[1] << 3);
    } else {
        /* 无此选项 */
        state = AM_CAN_INVALID_PARAMETER;
        return state;
    }

    *reg_1 = reg_f_r1;
    *reg_2 = reg_f_r2;

    return state;
}

am_can_err_t __can_stm32f103rbt6_filter_16bit_idmask(uint32_t        *reg_1,
                                              uint32_t        *reg_2,
                                              am_can_filter_t *p_filterbuff)
{
    uint16_t reg_f_r1_h = 0;
    uint16_t reg_f_r1_l = 0;
    uint16_t reg_f_r2_h = 0;
    uint16_t reg_f_r2_l = 0;
    uint16_t state      = AM_CAN_NOERROR;

    if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_DATA ) {
        /* 数据帧 */
        reg_f_r1_h &= (~(uint16_t)(1 << 4));
        reg_f_r1_l |= ((uint16_t)(1 << 4)); /* mask */
        reg_f_r2_h &= (~(uint16_t)(1 << 4));
        reg_f_r2_l |= ((uint16_t)(1 << 4)); /* mask */
    } else if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_REMOTE) {
        /* 远程帧 */
        reg_f_r1_h |= ((uint16_t)(1 << 4));
        reg_f_r1_l |= ((uint16_t)(1 << 4));
        reg_f_r2_h |= ((uint16_t)(1 << 4));
        reg_f_r2_l |= ((uint16_t)(1 << 4));
    } else if(p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_NOCARE) {
        /* 都可以接收 */
        reg_f_r1_l &= (~(uint16_t)(1 << 4));
        reg_f_r2_l &= (~(uint16_t)(1 << 4));
    } else {
        state = AM_CAN_INVALID_PARAMETER;
        return state;
    }

    if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_STD) {
        /* 只接收标准帧 */
        reg_f_r1_h &= (~(uint16_t)(1 << 3));
        reg_f_r1_l |= ((uint16_t)(1 << 3));
        reg_f_r2_h &= (~(uint16_t)(1 << 3));
        reg_f_r2_l |= ((uint16_t)(1 << 3));

        if ((p_filterbuff->id[0] >0x7ff)   ||
            (p_filterbuff->mask[0] >0x7ff) ||
            (p_filterbuff->id[1] >0x7ff)   ||
            (p_filterbuff->mask[1] >0x7ff)) {
            state = AM_CAN_INVALID_PARAMETER;
            return state;
        }

        reg_f_r1_h |= (p_filterbuff->id[0] & 0x7ff) << 5;
        reg_f_r1_l |= (p_filterbuff->mask[0] & 0x7ff) << 5;
        reg_f_r2_h |= (p_filterbuff->id[1] & 0x7ff) << 5;
        reg_f_r2_l |= (p_filterbuff->mask[1] & 0x7ff) << 5;
    } else if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_EXT) {
        /* 只接收扩展帧 */
        reg_f_r1_h |= ((uint16_t)(1 << 3));
        reg_f_r1_l |= ((uint16_t)(1 << 3));
        reg_f_r2_h |= ((uint16_t)(1 << 3));
        reg_f_r2_l |= ((uint16_t)(1 << 3));

        if ((p_filterbuff->id[0] > 0x3ffff)   ||
            (p_filterbuff->mask[0] > 0x3ffff) ||
            (p_filterbuff->id[1] > 0x3ffff)   ||
            (p_filterbuff->mask[1] > 0x3ffff)) {
            state = AM_CAN_INVALID_PARAMETER;
            return state;
        }

        reg_f_r1_h |= (p_filterbuff->id[0] >> 15) |
                     ((p_filterbuff->id[0] & 0x7ff) << 5);
        reg_f_r1_l |= (p_filterbuff->mask[0] >> 15) |
                     ((p_filterbuff->mask[0] & 0x7ff) << 5);
        reg_f_r2_h |= (p_filterbuff->id[1] >> 15) |
                     ((p_filterbuff->id[1] & 0x7ff) << 5);
        reg_f_r2_l |= (p_filterbuff->mask[1] >> 15) |
                     ((p_filterbuff->mask[1] & 0x7ff) << 5);
    } else {
        state = AM_CAN_INVALID_PARAMETER;
        return state;
    }

    *reg_1 = ((uint32_t)reg_f_r1_h <<16) |
            ((uint32_t)reg_f_r1_l);
    *reg_2 = ((uint32_t)reg_f_r2_h <<16) |
            ((uint32_t)reg_f_r2_l);

    return state;
}

am_can_err_t __can_stm32f103rbt6_filter_16bit_idlist(uint32_t        *reg_1,
                                              uint32_t        *reg_2,
                                              am_can_filter_t *p_filterbuff)
{
    uint16_t reg_f_r1_h = 0;
    uint16_t reg_f_r1_l = 0;
    uint16_t reg_f_r2_h = 0;
    uint16_t reg_f_r2_l = 0;
    uint8_t  state      = AM_CAN_NOERROR;

    if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_DATA ) {
        /* 数据帧 */
        reg_f_r1_h &= (~(uint16_t)(1 << 4));
        reg_f_r1_l &= (~(uint16_t)(1 << 4));
        reg_f_r2_h &= (~(uint16_t)(1 << 4));
        reg_f_r2_l &= (~(uint16_t)(1 << 4));
    } else if (p_filterbuff->rtr == AM_CAN_FRAME_FORMAT_REMOTE) {
        /* 远程帧 */
        reg_f_r1_h |= ((uint16_t)(1 << 4));
        reg_f_r1_l |= ((uint16_t)(1 << 4));
        reg_f_r2_h |= ((uint16_t)(1 << 4));
        reg_f_r2_l |= ((uint16_t)(1 << 4));
    } else {
        /* 列表模式下 返回参数错误 */
        state = AM_CAN_INVALID_PARAMETER;
        return state;
    }

    if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_STD) {
        /* 只接收标准帧 */
        reg_f_r1_h &= (~(uint16_t)(1 << 3));
        reg_f_r1_l &= (~(uint16_t)(1 << 3));
        reg_f_r2_h &= (~(uint16_t)(1 << 3));
        reg_f_r2_l &= (~(uint16_t)(1 << 3));
        if ((p_filterbuff->id[0] > 0x7ff) ||
            (p_filterbuff->id[1] > 0x7ff) ||
            (p_filterbuff->id[2] > 0x7ff) ||
            (p_filterbuff->id[3] > 0x7ff) ) {
            state = AM_CAN_INVALID_PARAMETER;
            return state;
        }
        reg_f_r1_h |= (p_filterbuff->id[0] & 0x7ff) << 5;
        reg_f_r1_l |= (p_filterbuff->id[1] & 0x7ff) << 5;
        reg_f_r2_h |= (p_filterbuff->id[2] & 0x7ff) << 5;
        reg_f_r2_l |= (p_filterbuff->id[3] & 0x7ff) << 5;

    } else if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_EXT) {
        /* 只接收扩展帧 */
        reg_f_r1_h |= ((uint16_t)(1 << 3));
        reg_f_r1_l |= ((uint16_t)(1 << 3));
        reg_f_r2_h |= ((uint16_t)(1 << 3));
        reg_f_r2_l |= ((uint16_t)(1 << 3));

        if ((p_filterbuff->id[0] > 0x3ffff) ||
            (p_filterbuff->id[1] > 0x3ffff) ||
            (p_filterbuff->id[2] > 0x3ffff) ||
            (p_filterbuff->id[3] > 0x3ffff) ) {
            state = AM_CAN_INVALID_PARAMETER;
            return state;
        }

        reg_f_r1_h |= ((p_filterbuff->id[0] & 0x7ff) << 5) |
                       (p_filterbuff->id[0] >> 15);
        reg_f_r1_l |= ((p_filterbuff->id[1] & 0x7ff) << 5) |
                       (p_filterbuff->id[1] >> 15);
        reg_f_r2_h |= ((p_filterbuff->id[2] & 0x7ff) << 5) |
                       (p_filterbuff->id[2] >> 15);
        reg_f_r2_l |= ((p_filterbuff->id[3] & 0x7ff) << 5) |
                       (p_filterbuff->id[3] >> 15);
    } else {
        state = AM_CAN_INVALID_PARAMETER;
        return state;
    }

    *reg_1 = ((uint32_t)reg_f_r1_h <<16) |
            ((uint32_t)reg_f_r1_l);
    *reg_2 = ((uint32_t)reg_f_r2_h <<16) |
            ((uint32_t)reg_f_r2_l);
    return state;
}

/**
 * \brief 设置滤波函数(扩展)
 */
am_can_err_t __can_stm32f103rbt6_filter_tab_ext_set (void              *p_drv,
                                              am_can_filter_t   *p_filterbuff,
                                              size_t             lenth)
{
    am_stm32f103rbt6_can_dev_t         *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_filter_t    *p_filter = NULL;
    amhw_stm32f103rbt6_can_t           *p_hw_can = NULL;
    uint8_t                      filt_num = 0;
    uint32_t                     filt_pos = 0;
    amhw_stm32f103rbt6_can_mode_t       mode     = p_dev->mode;
    am_can_err_t                 state    = AM_CAN_NOERROR;

    if (NULL == p_drv || NULL == p_filterbuff || 0 == lenth || (lenth > 14)) {
        return AM_CAN_INVALID_PARAMETER;
    }



    p_filter= (amhw_stm32f103rbt6_can_filter_t *)p_dev->p_devinfo->filter;
    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    if (AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT != p_dev->mode) {
        /* 波特率寄存器只在初始化模式下可进行访问，设置前进入初始化模式  */
        amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can,
                AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT);
    }


    /* 滤波器工作在滤波器初始化模式  */
    p_hw_can->fmr |= (uint32_t)0x01;
    for (int i = 0; i < lenth; i++) {

        /* 滤波器组号  */
        filt_num = i;

        if (filt_num > 13 ) {
            /* 跳出当前 */
            state = AM_CAN_ILLEGAL_CHANNEL_NO;
            break;
        } else {
            /* 滤波器组 */
            filt_pos = (uint32_t)(1ul << filt_num);
        }

        /* 禁用滤波器  */
        p_hw_can->fa_1r &= ~(uint32_t)filt_pos;

        if (p_filter->can_filter_mode) {

            /* 标识符列表 */
            p_hw_can->fm_1r |= (uint32_t)filt_pos;
            if (p_filter->can_filter_scale) {
                uint32_t reg_f_r1 = 0;
                uint32_t reg_f_r2 = 0;

                reg_f_r1 = p_hw_can->fi_rx[filt_num].f_r1;
                reg_f_r2 = p_hw_can->fi_rx[filt_num].f_r2;
                /* 32位 */
                p_hw_can->fs_1r |= (uint32_t)filt_pos;
                state = __can_stm32f103rbt6_filter_32bit_idlist(&reg_f_r1,
                                                         &reg_f_r2,
                                                          p_filterbuff);
                if (state != AM_CAN_NOERROR) {
                    return state;
                } else {
                    p_hw_can->fi_rx[filt_num].f_r1 = reg_f_r1;
                    p_hw_can->fi_rx[filt_num].f_r2 = reg_f_r2;
                }


            } else {

                /* 16位 */
                p_hw_can->fs_1r &= ~(uint32_t)filt_pos;

                uint32_t reg_f_r1 = 0;
                uint32_t reg_f_r2 = 0;

                reg_f_r1 = p_hw_can->fi_rx[filt_num].f_r1;
                reg_f_r2 = p_hw_can->fi_rx[filt_num].f_r2;

                state = __can_stm32f103rbt6_filter_16bit_idlist(&reg_f_r1,
                                                         &reg_f_r2,
                                                          p_filterbuff);

                if (state != AM_CAN_NOERROR) {
                    return state;
                } else {
                    p_hw_can->fi_rx[filt_num].f_r1 = reg_f_r1;
                    p_hw_can->fi_rx[filt_num].f_r2 = reg_f_r2;
                }
            }
        } else {

            /* 标识符屏蔽位 */
            p_hw_can->fm_1r &= ~(uint32_t)filt_pos;

            if (p_filter->can_filter_scale) {

                /* 32位 */
                p_hw_can->fs_1r |= (uint32_t)filt_pos;

                uint32_t reg_f_r1 = 0;
                uint32_t reg_f_r2 = 0;

                reg_f_r1 = p_hw_can->fi_rx[filt_num].f_r1;
                reg_f_r2 = p_hw_can->fi_rx[filt_num].f_r2;

                state = __can_stm32f103rbt6_filter_32bit_idmask(
                                                 &reg_f_r1,
                                                 &reg_f_r2,
                                                 p_filterbuff);
                if (state != AM_CAN_NOERROR) {
                    return state;
                } else {
                    p_hw_can->fi_rx[filt_num].f_r1 = reg_f_r1;
                    p_hw_can->fi_rx[filt_num].f_r2 = reg_f_r2;
                }
            } else {

                /* 16位 */
                p_hw_can->fs_1r &= ~(uint32_t)filt_pos;

                uint32_t reg_f_r1 = 0;
                uint32_t reg_f_r2 = 0;

                state = __can_stm32f103rbt6_filter_16bit_idmask(
                            &reg_f_r1,
                            &reg_f_r2,
                            p_filterbuff);

                if (state != AM_CAN_NOERROR) {
                    return state;
                } else {
                    p_hw_can->fi_rx[filt_num].f_r1 = reg_f_r1;
                    p_hw_can->fi_rx[filt_num].f_r2 = reg_f_r2;
                }

            }
        }

        /* 滤波器FIFO关联  */
        if (p_filter->can_fliter_fifo == AMHW_STM32F103RBT6_CAN_FILTER_FIFO0) {

            /* 关联到FIFO 0  */
            p_hw_can->ffa_1r &= ~(uint32_t)filt_pos;
        }

        if (p_filter->can_fliter_fifo == AMHW_STM32F103RBT6_CAN_FILTER_FIFO1) {

            /* 关联到FIFO 1  */
            p_hw_can->ffa_1r |= (uint32_t)filt_pos;
        }

        /* 激活滤波器  */
        if (p_filter->filter_state == 1) {

            p_hw_can->fa_1r |= (uint32_t)filt_pos;
        }

        /* 滤波器组后移 */
        p_filterbuff++;
    }

    /* 滤波器组回到正常模式 */
    p_hw_can->fmr &= ~0x01;

    if (mode != AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT) {

        /* 切换到正常模式 */
        amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can,
                mode);
    }

    return state;

}


/**
 * \brief 获取滤波函数
 */
am_can_err_t __can_stm32f103rbt6_filter_tab_ext_get (void              *p_drv,
                                              am_can_filter_t   *p_filterbuff,
                                              size_t            *p_lenth)
{
    am_stm32f103rbt6_can_dev_t         *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t           *p_hw_can = NULL;
    uint32_t                     filter_group = 0;

    /* 1 list  0 mask */
    uint8_t                      filter_mode = 0;

    /* 1 32  0 16 */
    uint8_t                      filter_scale = 0;
    if (NULL == p_drv ) {
        return AM_CAN_INVALID_PARAMETER;
    }


    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    filter_group = p_hw_can->fa_1r & 0x3fff;

    for (int i = 0;i < 14;i++) {

        /* 判断当前滤波器组是否激活 */
        if ( ( filter_group & ( (uint32_t)1ul<< i) ) ) {
            *p_lenth = *p_lenth + 1;
        } else {
            continue;
        }

        /* 模式判定 */
        filter_mode  = (p_hw_can->fm_1r >> i) & 0x01;
        filter_scale = (p_hw_can->fs_1r >> i) & 0x01;

        if (filter_mode == AMHW_STM32F103RBT6_CAN_FILTER_MODE_IDMASK) {
                /* 掩码模式 16位 */
            if (filter_scale == AMHW_STM32F103RBT6_CAN_FILTER_SCALE_16BIT) {
                uint16_t reg_f_r1_h = p_hw_can->fi_rx[i].f_r1 >> 16;
                uint16_t reg_f_r1_l = p_hw_can->fi_rx[i].f_r1 & 0xffff;
                uint16_t reg_f_r2_h = p_hw_can->fi_rx[i].f_r2 >> 16;
                uint16_t reg_f_r2_l = p_hw_can->fi_rx[i].f_r2 & 0xffff;

                /* 帧格式 帧类型 */
                p_filterbuff->ide = ((reg_f_r1_h >> 3) & 0x01) ?
                                        AM_CAN_FRAME_TYPE_EXT :
                                        AM_CAN_FRAME_TYPE_STD;
                p_filterbuff->rtr = ((reg_f_r1_l >> 4) & 0x01) ?
                                        (((reg_f_r1_h >> 4) & 0x01) ?
                                                AM_CAN_FRAME_FORMAT_REMOTE :
                                                AM_CAN_FRAME_FORMAT_DATA):
                                        AM_CAN_FRAME_FORMAT_NOCARE;

                p_filterbuff->id[0] = (reg_f_r1_h >> 5) |
                                        ((reg_f_r1_h & 0x07) << 15);
                p_filterbuff->id[1] = (reg_f_r2_h >> 5) | ((reg_f_r2_h & 0x07) << 15);

                p_filterbuff->mask[0] = (reg_f_r1_l >> 5) | ((reg_f_r1_l & 0x07) << 15);
                p_filterbuff->mask[1] = (reg_f_r2_l >> 5) | ((reg_f_r2_l & 0x07) << 15);

            } else {
                /* 掩码模式 32位 */
                uint32_t reg_f_r1 = p_hw_can->fi_rx[i].f_r1;
                uint32_t reg_f_r2 = p_hw_can->fi_rx[i].f_r2;

                /* 帧格式 帧类型 */
                p_filterbuff->ide = ((reg_f_r1 >> 2) & 0x01) ?
                                        AM_CAN_FRAME_TYPE_EXT :
                                        AM_CAN_FRAME_TYPE_STD;

                p_filterbuff->rtr = ((reg_f_r2 >> 1) & 0x01) ?
                                        (((reg_f_r1 >> 1) & 0x01) ?
                                                AM_CAN_FRAME_FORMAT_REMOTE :
                                                AM_CAN_FRAME_FORMAT_DATA):
                                        AM_CAN_FRAME_FORMAT_NOCARE;

                    /* 扩展帧 */
                if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_EXT) {
                    p_filterbuff->id[0]    = (reg_f_r1 >> 3);
                    p_filterbuff->mask[0]  = (reg_f_r1 >> 3);
                } else {
                    /* 标准帧 */
                    p_filterbuff->id[0]    = reg_f_r1 >> 21;
                    p_filterbuff->mask[0]  = reg_f_r2 >> 21;
                }
            }
        } else {
            if (filter_scale == AMHW_STM32F103RBT6_CAN_FILTER_SCALE_16BIT) {
                /* 列表 16位 */
                uint32_t reg_f_r1_h = p_hw_can->fi_rx[i].f_r1 >> 16;
                uint32_t reg_f_r1_l = p_hw_can->fi_rx[i].f_r1 & 0xffff;
                uint32_t reg_f_r2_h = p_hw_can->fi_rx[i].f_r2 >> 16;
                uint32_t reg_f_r2_l = p_hw_can->fi_rx[i].f_r2 & 0xffff;

                p_filterbuff->ide = ((reg_f_r1_h >> 3) & 0x01) ?
                                        AM_CAN_FRAME_TYPE_EXT :
                                        AM_CAN_FRAME_TYPE_STD;
                p_filterbuff->rtr = ((reg_f_r1_h >> 4) & 0x01) ?
                                        AM_CAN_FRAME_FORMAT_REMOTE :
                                        AM_CAN_FRAME_FORMAT_DATA;

                if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_EXT) {
                    p_filterbuff->id[0] = ((reg_f_r1_h & 0x07) << 15) |
                                           (reg_f_r1_h >> 5);
                    p_filterbuff->id[1] = ((reg_f_r1_l & 0x07) << 15) |
                                           (reg_f_r1_l >> 5);
                    p_filterbuff->id[2] = ((reg_f_r2_h & 0x07) << 15) |
                                           (reg_f_r2_h >> 5);
                    p_filterbuff->id[3] = ((reg_f_r2_l & 0x07) << 15) |
                                           (reg_f_r2_l >> 5);
                } else {
                    p_filterbuff->id[0] = reg_f_r1_h >> 5;
                    p_filterbuff->id[1] = reg_f_r1_l >> 5;
                    p_filterbuff->id[2] = reg_f_r2_h >> 5;
                    p_filterbuff->id[3] = reg_f_r2_l >> 5;
                }

            } else {
                /* 列表 32位 */
                uint32_t reg_f_r1 = p_hw_can->fi_rx[i].f_r1;
                uint32_t reg_f_r2 = p_hw_can->fi_rx[i].f_r2;

                p_filterbuff->ide = ((reg_f_r1 >> 2) & 0x01) ?
                                        AM_CAN_FRAME_TYPE_EXT :
                                        AM_CAN_FRAME_TYPE_STD;
                p_filterbuff->rtr = ((reg_f_r2 >> 1) & 0x01) ?
                                        AM_CAN_FRAME_FORMAT_REMOTE:
                                        AM_CAN_FRAME_FORMAT_DATA;

                if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_EXT) {
                    p_filterbuff->id[0] = reg_f_r1 >> 3;
                    p_filterbuff->id[1] = reg_f_r2 >> 3;
                } else {
                    p_filterbuff->id[0] = reg_f_r1 >> 21;
                    p_filterbuff->id[1] = reg_f_r2 >> 21;
                }
            }
        }
        p_filterbuff++;
    }
    return AM_CAN_NOERROR;
}


static void __can_stm32f103rbt6_intcb_func_run (am_stm32f103rbt6_can_dev_t  *p_dev,
                                        am_can_int_type_t      can_int)
{
    uint32_t      temp      = (uint32_t)can_int;
    am_pfnvoid_t  intcb_fun = NULL;
    void         *intcb_arg = NULL;
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
static void __can_stm32f103rbt6_int_handle (void *p_drv)
{
    am_stm32f103rbt6_can_dev_t    *p_dev     = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t      *p_hw_can  = NULL;

    if (NULL == p_drv) {
        return;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;


    /* 接收中断产生，并且rxfifo不为空 */
    if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_FMPIE0)) ||
        amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_FMPIE1))) {

        /* 执行接收回调函数 */
        __can_stm32f103rbt6_intcb_func_run(p_dev, AM_CAN_INT_RX);

        /*  */
        amhw_stm32f103rbt6_can_clear_itstatus(p_hw_can, AMHW_STM32F103RBT6_CAN_INT_FMPIE0);
        amhw_stm32f103rbt6_can_clear_itstatus(p_hw_can, AMHW_STM32F103RBT6_CAN_INT_FMPIE1);
    }

    /* 发送中断产生，并且发送完成 */
    if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_TMEIE))) {

        /* 执行发送回调函数 */
        __can_stm32f103rbt6_intcb_func_run(p_dev, AM_CAN_INT_TX);
        amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can, AMHW_STM32F103RBT6_CAN_INT_TMEIE);
    }



    /* 唤醒中断产生 */
    if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_WKUIE))) {
        /* 执行唤醒中断回调函数 */
        __can_stm32f103rbt6_intcb_func_run(p_dev, AM_CAN_INT_WAKE_UP);

        amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can, AMHW_STM32F103RBT6_CAN_INT_WKUIE);
    }

    /* 数据溢出中断产生 */
    if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_FOVIE0)) ||
        amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_FOVIE1)))  {

        /* 执行数据溢出中断回调函数 */
        __can_stm32f103rbt6_intcb_func_run(p_dev, AM_CAN_INT_DATAOVER);

        /* 释放FIFO  在中断函数中处理或者  在此处理 FIFO  清空FIFO邮箱 */
        for (int i = 0 ;i <3 ; i++) {
            p_hw_can->rf_0r |= AMHW_STM32F103RBT6_CAN_RF0R_RFOM0;
            p_hw_can->rf_1r |= AMHW_STM32F103RBT6_CAN_RF1R_RFOM1;
        }

        amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can, AMHW_STM32F103RBT6_CAN_INT_FOVIE0);
        amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can, AMHW_STM32F103RBT6_CAN_INT_FOVIE1);
    }

    /* 错误中断、总线错误中断、总线仲裁丢失处理 */
    if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_ERRIE)))  {

        /* 判断总线是否离线 */
        if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
                (AMHW_STM32F103RBT6_CAN_INT_BOFIE))) {

            /* 执行总线关闭中断回调函数 */
            __can_stm32f103rbt6_intcb_func_run(p_dev, AM_CAN_INT_BUS_OFF);

            amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can,
                                            AMHW_STM32F103RBT6_CAN_INT_ERRIE);
        } else if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
                (AMHW_STM32F103RBT6_CAN_INT_EWGIE))) {

           /* 执行警告中断回调函数 */
           __can_stm32f103rbt6_intcb_func_run(p_dev, AM_CAN_INT_WARN);

           amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can,
                                           AMHW_STM32F103RBT6_CAN_INT_ERRIE);
        } else if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
                (AMHW_STM32F103RBT6_CAN_INT_EPVIE))) {

           /* 执行警告中断回调函数 */
           __can_stm32f103rbt6_intcb_func_run(p_dev, AM_CAN_INT_ERROR_PASSIVE);

           amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can,
                                           AMHW_STM32F103RBT6_CAN_INT_ERRIE);
        }
    }
}

/**
 * \brief CAN 状态
 */
am_can_err_t __can_stm32f103rbt6_status_get (void              *p_drv,
                               am_can_int_type_t *p_int_type,
                               am_can_bus_err_t  *p_bus_err)
{
    am_stm32f103rbt6_can_dev_t    *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
    amhw_stm32f103rbt6_can_t      *p_hw_can = NULL;
    uint8_t                 err_code = 0;  /* 错误号 */
    uint8_t                 err      = 0;  /* 错误寄存器值 */

    if (NULL == p_drv || NULL == p_int_type || NULL == p_bus_err) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    *p_int_type = 0;

    /* 接收中断 */
    if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_FMPIE0)) ||
        amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            ( AMHW_STM32F103RBT6_CAN_INT_FMPIE1))) {

        *p_int_type |= AM_CAN_INT_RX;
    }

    /* 发送中断 */
    if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_TMEIE))){
        *p_int_type |= AM_CAN_INT_TX;
    }

    /* 数据溢出中断 */
    if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            ( AMHW_STM32F103RBT6_CAN_INT_FOVIE0)) ||
        amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            ( AMHW_STM32F103RBT6_CAN_INT_FOVIE1))) {

        *p_int_type |= AM_CAN_INT_DATAOVER;
    }

    /* 唤醒中断 */
    if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_WKUIE))) {
        *p_int_type |= AM_CAN_INT_WAKE_UP;
    }

    /* 错误中断、总线错误中断、总线仲裁丢失中断 */
    if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            ( AMHW_STM32F103RBT6_CAN_INT_ERRIE)) ||
        amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            ( AMHW_STM32F103RBT6_CAN_INT_EWGIE)) ||
        amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_EPVIE)) ||
        amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            (AMHW_STM32F103RBT6_CAN_INT_LECIE)) ||
        amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
            ( AMHW_STM32F103RBT6_CAN_INT_BOFIE)) )  {

        *p_int_type |= AM_CAN_INT_ERROR;

        /* 判断是否是因为错误计数器达到限制而引起的总线关闭 */
        if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
                (AMHW_STM32F103RBT6_CAN_INT_BOFIE))) {
            *p_int_type |= AM_CAN_INT_BUS_OFF;
        }

        /* 判断是否是因为错误计数器达到限制而引起的错误警告 */
        if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
                (AMHW_STM32F103RBT6_CAN_INT_EWGIE))) {
            *p_int_type |= AM_CAN_INT_WARN;
        }

        /* 判断是否是因为错误计数器达到限制而引起的错误被动 */
        if (amhw_stm32f103rbt6_can_get_itstatus(p_hw_can,
                (AMHW_STM32F103RBT6_CAN_INT_EPVIE))) {
            *p_int_type |= AM_CAN_INT_ERROR_PASSIVE;
        }
    }

    /* 从寄存器获取错误代码 */
    *p_bus_err = 0;
    err        = amhw_stm32f103rbt6_can_get_lasterrorcode(p_hw_can);
    err_code = (err >> 4);

    /* 格式错误 */
    if (AMHW_STM32F103RBT6_CAN_LEC_FORMER == err_code) {
        *p_bus_err |= AM_CAN_BUS_ERR_FORM;
    }

    /* 填充错误 */
    if (AMHW_STM32F103RBT6_CAN_LEC_STUFER == err_code) {
        *p_bus_err |= AM_CAN_BUS_ERR_STUFF;
    }

    /* 未知错误     显性错误   隐性错误  由软件设置错误*/
    if ((AMHW_STM32F103RBT6_CAN_LEC_SOFTER == err_code) ||
        (AMHW_STM32F103RBT6_CAN_LEC_BRCER  == err_code) ||
        (AMHW_STM32F103RBT6_CAN_LEC_BDERR  == err_code)) {
        *p_bus_err |= AM_CAN_BUS_ERR_UNKNOWN;
    }

    /* ACK错误 */
    if (err_code == AMHW_STM32F103RBT6_CAN_LEC_ACKER) {
        *p_bus_err |= AM_CAN_BUS_ERR_ACK;
    }

    /* CRC错误 */
    if (err_code == AMHW_STM32F103RBT6_CAN_LEC_CRCER) {
        *p_bus_err |= AM_CAN_BUS_ERR_CRC;
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断连接
 */
am_can_err_t __can_stm32f103rbt6_connect (void         *p_drv,
                            am_pfnvoid_t  pfn_isr,
                            void         *p_arg)
{
    am_stm32f103rbt6_can_dev_t    *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    if (NULL != pfn_isr) {
        /* 连接传入函数 */
        am_int_connect(INUM_USB_HP_CAN_TX, pfn_isr, p_arg);
        am_int_connect(INUM_USB_HP_CAN_RX, pfn_isr, p_arg);
        am_int_connect(INUM_CAN1_RX1, pfn_isr, p_arg);
        am_int_connect(INUM_CAN1_SCE, pfn_isr, p_arg);
    } else {
        /* 连接默认函数 */
        am_int_connect(INUM_USB_HP_CAN_TX, __can_stm32f103rbt6_int_handle, p_dev);
        am_int_connect(INUM_USB_HP_CAN_RX, __can_stm32f103rbt6_int_handle, p_dev);
        am_int_connect(INUM_CAN1_RX1, __can_stm32f103rbt6_int_handle, p_dev);
        am_int_connect(INUM_CAN1_SCE, __can_stm32f103rbt6_int_handle, p_dev);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 删除中断连接
 */
am_can_err_t __can_stm32f103rbt6_disconnect (void         *p_drv,
                               am_pfnvoid_t  pfn_isr,
                               void         *p_arg)
{
    am_stm32f103rbt6_can_dev_t    *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    /* 中断禁能 */
    am_int_disable(INUM_USB_HP_CAN_TX);
    am_int_disable(INUM_USB_HP_CAN_RX);
    am_int_disable(INUM_CAN1_RX1);
    am_int_disable(INUM_CAN1_SCE);
    if (NULL != pfn_isr) {
        /* 删除连接函数 */
        am_int_disconnect(INUM_USB_HP_CAN_TX, pfn_isr, p_arg);
        am_int_disconnect(INUM_USB_HP_CAN_RX, pfn_isr, p_arg);
        am_int_disconnect(INUM_CAN1_RX1, pfn_isr, p_arg);
        am_int_disconnect(INUM_CAN1_SCE, pfn_isr, p_arg);
    } else {
        /* 删除连接函数 */
        am_int_disconnect(INUM_USB_HP_CAN_TX, __can_stm32f103rbt6_int_handle, p_dev);
        am_int_disconnect(INUM_USB_HP_CAN_RX, __can_stm32f103rbt6_int_handle, p_dev);
        am_int_disconnect(INUM_CAN1_RX1, __can_stm32f103rbt6_int_handle, p_dev);
        am_int_disconnect(INUM_CAN1_SCE, __can_stm32f103rbt6_int_handle, p_dev);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断回调函数的连接
 */
am_can_err_t __can_stm32f103rbt6_intcb_connect (void              *p_drv,
                                  am_can_int_type_t  inttype,
                                  am_pfnvoid_t       pfn_callback,
                                  void              *p_arg)
{
    am_stm32f103rbt6_can_dev_t    *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
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
am_can_err_t __can_stm32f103rbt6_intcb_disconnect (void              *p_drv,
                                            am_can_int_type_t  inttype)
{
    am_stm32f103rbt6_can_dev_t    *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_drv;
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
am_can_handle_t am_stm32f103rbt6_can_init (am_stm32f103rbt6_can_dev_t      *p_dev,
                               const am_stm32f103rbt6_can_devinfo_t *p_devinfo)
{
    uint8_t             i = 0;

    if (NULL == p_dev || NULL == p_devinfo) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo = p_devinfo;

    p_dev->handle.p_drv_funcs = &__stm32f103rbt6_can_dev_func;
    p_dev->handle.p_drv       = (void *)p_dev;

    for (i = 0 ; i < p_devinfo->p_intcb_num ; i++) {
        p_devinfo->p_intcb_info[i].pfn_callback = NULL;
        p_devinfo->p_intcb_info[i].p_arg        = NULL;
    }

    return (am_can_handle_t)&(p_dev->handle);
}

/**
 * \brief 解初始化
 */
void am_stm32f103rbt6_can_deinit (am_can_handle_t handle)
{
    amhw_stm32f103rbt6_can_t *p_hw_can = NULL;
    am_stm32f103rbt6_can_dev_t  *p_dev = (am_stm32f103rbt6_can_dev_t *)handle->p_drv;

    if (NULL == p_dev) {
        return;
    }

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    amhw_stm32f103rbt6_can_operating_mode_request(p_hw_can,
            AMHW_STM32F103RBT6_CAN_OPERATING_MODE_INIT);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    /* 中断禁能 */
    am_int_disable(INUM_USB_HP_CAN_TX);
    am_int_disable(INUM_USB_HP_CAN_RX);
    am_int_disable(INUM_CAN1_RX1);
    am_int_disable(INUM_CAN1_SCE);
}

/* end of file */
