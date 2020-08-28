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
 */

#include "am_int.h"
#include "am_can.h"
#include "am_hc32f07x_can.h"
#include "hw/amhw_hc32f07x_can.h"
#include "hc32f07x_inum.h"
#include "am_vdebug.h"
/** \brief CAN启动 */
am_can_err_t __can_hc32f07x_start (void *p_drv);
/** \brief CAN复位（停止） */
am_can_err_t __can_hc32f07x_reset (void *p_drv);
///** \brief CAN休眠  */
//am_can_err_t __can_hc32f07x_sleep (void *p_drv);
///** \brief CAN唤醒 */
//am_can_err_t __can_hc32f07x_wakeup (void *p_drv);
/** \brief 中断使能 */
am_can_err_t __can_hc32f07x_int_enable (void *p_drv, am_can_int_type_t int_mask);
/** \brief 中断禁能*/
am_can_err_t __can_hc32f07x_int_disable (void *p_drv, am_can_int_type_t int_mask);
/** \brief 模式设置 */
am_can_err_t __can_hc32f07x_mode_set (void *p_drv, am_can_mode_type_t mode);
/** \brief 波特率设置 */
am_can_err_t __can_hc32f07x_baudrate_set (void               *p_drv,
                                 am_can_bps_param_t *p_can_baudrate);
/** \brief 波特率获取 */
am_can_err_t __can_hc32f07x_baudrate_get (void               *p_drv,
                                 am_can_bps_param_t *p_can_baudrate);
/** \brief 获取错误计数 */
am_can_err_t __can_hc32f07x_err_cnt_get (void         *p_drv,
                                 am_can_err_cnt_t   *p_can_err_reg);
/** \brief 发送信息   */
am_can_err_t __can_hc32f07x_msg_send (void *p_drv, am_can_message_t *p_txmsg);
/** \brief 接收信息   */
am_can_err_t __can_hc32f07x_msg_recv (void *p_drv, am_can_message_t *p_rxmsg);
/** \brief 停止发送信息   */
am_can_err_t __can_hc32f07x_stop_msg_snd (void *p_drv);

/** \brief CAN 状态 */
am_can_err_t __can_hc32f07x_status_get (void              *p_drv,
                               am_can_int_type_t *p_int_type,
                               am_can_bus_err_t  *p_bus_err);
/** \brief 中断连接 */
am_can_err_t __can_hc32f07x_connect (void         *p_drv,
                            am_pfnvoid_t  pfn_isr,
                            void         *p_arg);
/** \brief 删除中断连接 */
am_can_err_t __can_hc32f07x_disconnect (void         *p_drv,
                               am_pfnvoid_t  pfn_isr,
                               void         *p_arg);
/** \brief 中断回调函数的连接 */
am_can_err_t __can_hc32f07x_intcb_connect (void              *p_drv,
                                  am_can_int_type_t  inttype,
                                  am_pfnvoid_t       pfn_callback,
                                  void              *p_arg);


/** \brief 删除中断回调函数的连接 */
am_can_err_t __can_hc32f07x_intcb_disconnect (void              *p_drv,
                                            am_can_int_type_t  inttype);

/** \brief 设置滤波函数 (扩展)*/
am_can_err_t __can_hc32f07x_filter_tab_ext_set (void            *p_drv,
                                              am_can_filter_t *p_filterbuff,
                                              size_t           lenth);
/** \brief 获取滤波函数 (扩展)*/
am_can_err_t __can_hc32f07x_filter_tab_ext_get (void             *p_drv,
                                              am_can_filter_t  *p_filterbuff,
                                              size_t           *p_lenth);

struct am_can_drv_funcs __hc32f07x_can_dev_func = {
    __can_hc32f07x_start,
    __can_hc32f07x_reset,
    NULL,
    NULL,
    __can_hc32f07x_int_enable,
    __can_hc32f07x_int_disable,
    __can_hc32f07x_mode_set,
    __can_hc32f07x_baudrate_set,
    __can_hc32f07x_baudrate_get,
    __can_hc32f07x_err_cnt_get,
    NULL,
    __can_hc32f07x_msg_send,
    __can_hc32f07x_msg_recv,
    __can_hc32f07x_stop_msg_snd,
    NULL,
    NULL,
    __can_hc32f07x_status_get,
    __can_hc32f07x_connect,
    __can_hc32f07x_disconnect,
    __can_hc32f07x_intcb_connect,
    __can_hc32f07x_intcb_disconnect,
    __can_hc32f07x_filter_tab_ext_set,
    __can_hc32f07x_filter_tab_ext_get,
};

/**
 * \brief CAN初始化
 */
uint8_t __can_hc32f07x_init (void *p_drv)
{
    am_hc32f07x_can_dev_t        *p_dev       = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t          *p_hw_can    = NULL;
    am_hc32f07x_can_init_info_t  *p_can_info  = NULL;
    uint8_t                     init_staus  = 0;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_can_info =(am_hc32f07x_can_init_info_t *)p_dev->p_devinfo->init_info;
    p_hw_can   =(amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    /* 时间触发通信方式设定 */
    if(p_can_info->can_ttcm){
        amhw_hc32f07x_can_tt_enable(p_hw_can);
    }else {
        amhw_hc32f07x_can_tt_disable(p_hw_can);
    }

    /* 设置接收缓冲器数据存储模式 */
    if (p_can_info->rx_all) {
        amhw_hc32f07x_can_rxbuf_stores_all_set(p_hw_can,
                                               AMHW_HC32F07X_CAN_RXBUF_ALL);
    } else {
        amhw_hc32f07x_can_rxbuf_stores_all_set(p_hw_can,
                                              AMHW_HC32F07X_CAN_RXBUF_NORMAL);
    }

    /* 设置接收缓冲器上溢模式 */
    if (p_can_info->over_mode) {
        amhw_hc32f07x_can_rxbuf_over_mode_set(p_hw_can,
                                            AMHW_HC32F07X_CAN_RXBUF_NOTSTORED);
    } else {
        amhw_hc32f07x_can_rxbuf_over_mode_set (p_hw_can,
                                          AMHW_HC32F07X_CAN_RXBUF_OVERWRITTEN);
    }

    /* 设置缓冲器发送模式 */
    if (p_can_info->can_stb_mode) {
        amhw_hc32f07x_can_stb_mode_set(p_hw_can, 
                                       AMHW_HC32F07X_CAN_STB_MODE_PRIM);
    } else {
        amhw_hc32f07x_can_stb_mode_set(p_hw_can, 
                                       AMHW_HC32F07X_CAN_STB_MODE_FIFO);
    }

    /* 接收缓冲器将满设定值 */
    amhw_hc32f07x_can_rxbuf_warning_set(p_hw_can, 
                                        p_can_info->warning_limitval);

    /* 错误警告限定值 */
    amhw_hc32f07x_can_err_warning_set (p_hw_can, 
                                       p_can_info->error_warning_limitval);
    /* 关闭所有中断 */
    amhw_hc32f07x_can_int_all_disable (p_hw_can);

    return init_staus;
}


/**
 * \brief CAN启动
 */
am_can_err_t __can_hc32f07x_start (void *p_drv)
{
am_hc32f07x_can_dev_t    *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    /* 初始化CAN控制信息 */
    __can_hc32f07x_init(p_drv);

    /* 使能中断 */
    am_int_enable(p_dev->p_devinfo->int_num );

    return AM_CAN_NOERROR;
}

/**
 * \brief CAN复位（停止）
 */
am_can_err_t __can_hc32f07x_reset (void *p_drv)
{
    am_hc32f07x_can_dev_t    *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    /* 对CAN强制复位 */
    amhw_hc32f07x_can_reset_set (p_hw_can);


    /* 禁能中断 */
    am_int_disable(INUM_CAN);

    return AM_CAN_NOERROR;
}


/**
 * \brief 中断使能
 */
am_can_err_t __can_hc32f07x_int_enable (void *p_drv, am_can_int_type_t int_mask)
{
    am_hc32f07x_can_dev_t             *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t               *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    /* 接收中断 */
    if (int_mask & AM_CAN_INT_RX) {

        /* 使能接收中断 */
        amhw_hc32f07x_can_int_enable (p_hw_can, AMHW_HC32F07X_CAN_INT_RECV);

        int_mask &= ~AM_CAN_INT_RX;
    }

    /* 发送中断 */
    if (int_mask & AM_CAN_INT_TX) {
			
        /* 使能PTB发送中断 */
        amhw_hc32f07x_can_int_enable (p_hw_can, AMHW_HC32F07X_CAN_INT_TX_PTB);

        /* 使能STB发送中断 */
        amhw_hc32f07x_can_int_enable (p_hw_can, AMHW_HC32F07X_CAN_INT_TX_STB);

        int_mask &= ~AM_CAN_INT_TX;
    }

    /* 错误\警告中断  */
    if ((int_mask & AM_CAN_INT_ERROR) || (int_mask & AM_CAN_INT_WARN)) {

        /* 使能错误中断 */
        amhw_hc32f07x_can_int_enable(p_hw_can, AMHW_HC32F07X_CAN_INT_ERROR);

        /* 使能总线错误中断 */
        amhw_hc32f07x_can_int_enable(p_hw_can, AMHW_HC32F07X_CAN_INT_BUS_ERR);

        /* 使能仲裁失败中断 */
        amhw_hc32f07x_can_int_enable(p_hw_can, AMHW_HC32F07X_CAN_INT_ARBILOST);

        int_mask &= ~AM_CAN_INT_ERROR;
    }

    /* 错误被动中断  */
    if ((int_mask & AM_CAN_INT_ERROR_PASSIVE)) {

        /* 使能错误被动中断  */
        amhw_hc32f07x_can_int_enable (p_hw_can, AMHW_HC32F07X_CAN_INT_PASSIVE);

        int_mask &= ~AM_CAN_INT_ERROR_PASSIVE;
    }

    /* 总线超载中断 */
    if (int_mask & AM_CAN_INT_DATAOVER) {

        /* 使能接收上溢中断  */
        amhw_hc32f07x_can_int_enable(p_hw_can, AMHW_HC32F07X_CAN_INT_RX_OVER);

        /* 使能接收满中断  */
        amhw_hc32f07x_can_int_enable(p_hw_can, AMHW_HC32F07X_CAN_INT_RX_FULL);

        /* 使能接收将满中断  */
        amhw_hc32f07x_can_int_enable(p_hw_can, 
                                     AMHW_HC32F07X_CAN_INT_RX_ALMOSTFULL);

        int_mask &= ~AM_CAN_INT_DATAOVER;
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断禁能
 */
am_can_err_t __can_hc32f07x_int_disable (void *p_drv, am_can_int_type_t int_mask)
{
    am_hc32f07x_can_dev_t         *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t           *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;


   /* 接收中断 */
    if (int_mask & AM_CAN_INT_RX) {

        /* 禁能接收中断 */
        amhw_hc32f07x_can_int_disable (p_hw_can, AMHW_HC32F07X_CAN_INT_RECV);

        int_mask &= ~AM_CAN_INT_RX;
    }

    /* 发送中断 */
    if (int_mask & AM_CAN_INT_TX) {

        /* 禁能PTB发送中断 */
        amhw_hc32f07x_can_int_disable (p_hw_can, AMHW_HC32F07X_CAN_INT_TX_PTB);

        /* 使能STB发送中断 */
        amhw_hc32f07x_can_int_disable (p_hw_can, AMHW_HC32F07X_CAN_INT_TX_STB);

        int_mask &= ~AM_CAN_INT_TX;
    }

    /* 错误\警告中断  */
   if ((int_mask & AM_CAN_INT_ERROR) || (int_mask & AM_CAN_INT_WARN)) {

        /* 禁能错误中断 */
        amhw_hc32f07x_can_int_disable(p_hw_can, AMHW_HC32F07X_CAN_INT_ERROR);

        /* 禁能总线错误中断 */
        amhw_hc32f07x_can_int_disable(p_hw_can, AMHW_HC32F07X_CAN_INT_BUS_ERR);

        /* 禁能仲裁失败中断 */
        amhw_hc32f07x_can_int_disable(p_hw_can, AMHW_HC32F07X_CAN_INT_ARBILOST);

        int_mask &= ~AM_CAN_INT_ERROR;
    }

    /* 错误被动中断  */
    if ((int_mask & AM_CAN_INT_ERROR_PASSIVE)) {

        /* 禁能错误被动中断  */
        amhw_hc32f07x_can_int_disable(p_hw_can, AMHW_HC32F07X_CAN_INT_PASSIVE);

        int_mask &= ~AM_CAN_INT_ERROR_PASSIVE;
    }

    /* 总线超载中断 */
    if (int_mask & AM_CAN_INT_DATAOVER) {

        /* 禁能接收上溢中断  */
        amhw_hc32f07x_can_int_disable(p_hw_can, AMHW_HC32F07X_CAN_INT_RX_OVER);

        /* 禁能接收满中断  */
        amhw_hc32f07x_can_int_disable(p_hw_can, AMHW_HC32F07X_CAN_INT_RX_FULL);

        /* 禁能接收将满中断  */
        amhw_hc32f07x_can_int_disable(p_hw_can, 
                                      AMHW_HC32F07X_CAN_INT_RX_ALMOSTFULL);

        int_mask &= ~AM_CAN_INT_DATAOVER;
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 模式设置
 */
am_can_err_t __can_hc32f07x_mode_set (void *p_drv, am_can_mode_type_t mode)
{
    am_hc32f07x_can_dev_t *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t   *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    if (AM_CAN_MODE_NROMAL == mode ) {

        /* 正常模式 */
        amhw_hc32f07x_can_mode_disable (p_hw_can,
                                        AMHW_HC32F07X_CAN_LISTEN_ONLY_MODE);
        amhw_hc32f07x_can_mode_disable (p_hw_can,
                                        AMHW_HC32F07X_CAN_EXT_LOOPBACK_MODE);
        amhw_hc32f07x_can_mode_disable (p_hw_can,
                                        AMHW_HC32F07X_CAN_INT_LOOPBACK_MODE);
        amhw_hc32f07x_can_mode_disable (p_hw_can,
                                        AMHW_HC32F07X_CAN_SIG_PRIARY_MODE);
        amhw_hc32f07x_can_mode_disable (p_hw_can,
                                        AMHW_HC32F07X_CAN_SIG_SECONDARY_MODE);
    }else if (AM_CAN_MODE_LISTEN_ONLY == mode) {

        /* 静默模式*/
        amhw_hc32f07x_can_mode_enable (p_hw_can,
                                       AMHW_HC32F07X_CAN_LISTEN_ONLY_MODE);
    }else {

        /* 回环 和 回环静默未设置 */
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 波特率设置
 */
am_can_err_t __can_hc32f07x_baudrate_set (void           *p_drv,
                                 am_can_bps_param_t    *p_can_baudrate)
{
    am_hc32f07x_can_dev_t         *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t           *p_hw_can = NULL;

    if (NULL == p_drv || NULL == p_can_baudrate) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    /* 波特率寄存器只在复位模式下可进行访问，设置前进入复位模式  */
    amhw_hc32f07x_can_reset_set(p_hw_can);

    /* 设置 波特率分频器   同步跳转宽度   和相位段 */
    amhw_hc32f07x_can_brp_set(p_hw_can, p_can_baudrate->brp);
    amhw_hc32f07x_can_sjw_set(p_hw_can, p_can_baudrate->sjw);

    amhw_hc32f07x_can_tseg1_set(p_hw_can, p_can_baudrate->tesg1);
    amhw_hc32f07x_can_tseg2_set(p_hw_can, p_can_baudrate->tesg2);

    /* 退出复位模式 */
    amhw_hc32f07x_can_reset_clr(p_hw_can);

    return AM_CAN_NOERROR;
}

/**
 * \brief 波特率获取
 */
am_can_err_t __can_hc32f07x_baudrate_get (void           *p_drv,
                                 am_can_bps_param_t    *p_can_baudrate)
{
    am_hc32f07x_can_dev_t         *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t           *p_hw_can = NULL;

    if (NULL == p_drv || NULL == p_can_baudrate) {
        return AM_CAN_INVALID_PARAMETER;
    }

    /* 波特率寄存器只在初始化模式下可进行访问，设置前进入初始化模式  */
    amhw_hc32f07x_can_reset_set(p_hw_can);

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    p_can_baudrate->brp = amhw_hc32f07x_can_brp_get(p_hw_can);
    p_can_baudrate->sjw = amhw_hc32f07x_can_sjw_get(p_hw_can);

    p_can_baudrate->tesg1 = amhw_hc32f07x_can_tseg1_get(p_hw_can);
    p_can_baudrate->tesg2 = amhw_hc32f07x_can_tseg2_get(p_hw_can);

    amhw_hc32f07x_can_reset_set(p_hw_can);

    return AM_CAN_NOERROR;
}

/**
 * \brief 获取错误计数
 */
am_can_err_t __can_hc32f07x_err_cnt_get (void        *p_drv,
                               am_can_err_cnt_t    *p_can_err_reg)
{
    am_hc32f07x_can_dev_t *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t   *p_hw_can = NULL;

    if (NULL == p_drv || NULL == p_can_err_reg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    /* 读 TX/RX错误计数器 */
    p_can_err_reg->rx_error_cnt = amhw_hc32f07x_can_get_recverrorcount(p_hw_can);
    p_can_err_reg->tx_error_cnt = amhw_hc32f07x_can_get_transerrorcount(p_hw_can);

    return AM_CAN_NOERROR;
}


/**
 * \brief 发送信息
 */
am_can_err_t __can_hc32f07x_msg_send (void *p_drv, am_can_message_t *p_txmsg)
{
    
    am_hc32f07x_can_dev_t    *p_dev     = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t      *p_hw_can  = NULL;
    amhw_hc32f07x_can_type_t  can_type;
    am_bool_t                 is_remote = AM_FALSE;
    am_bool_t                 is_extern = AM_FALSE;

    if (NULL == p_drv || NULL == p_txmsg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;
    can_type = p_dev->p_devinfo->type;

    /* 判断发送缓冲区是否被释放 */
    if (AMHW_HC32F07X_CAN_TB_FULL == amhw_hc32f07x_can_get_tb_status(p_hw_can)){
        return AM_CAN_INVALID_PARAMETER;
    }

    is_remote = ((p_txmsg->flags & AM_CAN_REMOTE_FLAG) ? AM_TRUE : AM_FALSE);
    is_extern = ((p_txmsg->flags & AM_CAN_EXTERN_FLAG) ? AM_TRUE : AM_FALSE);

    /* 8字节数据 */
    if (p_txmsg->msglen > 8) {
        p_txmsg->msglen = 8;
    }
    amhw_hc32f07x_can_txbuf_sel (p_hw_can,
                                 AMHW_HC32F07X_CAN_TXBUF_PTB);

    /* 扩展标识符 */
    if (is_extern) {

        /* 设置扩展标识符ID和IDE位 */
        p_hw_can->tbuf.tbuf0      = p_txmsg->id;
        p_hw_can->tbuf.txctrl.ide = 1;
    }else {

        /* 设置标准标识符ID和IDE位 */
        p_hw_can->tbuf.tbuf0      = p_txmsg->id;
        p_hw_can->tbuf.txctrl.ide = 0;
    }

    /* 远程标志 */
    if (is_remote) {

        /* 远程帧 */
        p_hw_can->tbuf.txctrl.rtr = 1;
    }else {

        /* 数据帧 */
        p_hw_can->tbuf.txctrl.rtr = 0;
    }

    /* 写入数据长度 */
    p_hw_can->tbuf.txctrl.dlc = p_txmsg->msglen;

    /* 写入数据 */
    p_hw_can->tbuf.tbuf2 = p_txmsg->msgdata[3] << 24 |
                           p_txmsg->msgdata[2] << 16 |
                           p_txmsg->msgdata[1] << 8  |
                           p_txmsg->msgdata[0];
    p_hw_can->tbuf.tbuf3 = p_txmsg->msgdata[7] << 24 |
                           p_txmsg->msgdata[6] << 16 |
                           p_txmsg->msgdata[5] << 8  |
                           p_txmsg->msgdata[4];


    /* 发送请求 */
    if(can_type == AMHW_HC32F07X_CAN_PTB){

        /* PTB发送请求 */
        amhw_hc32f07x_can_ptb_send_enable(p_hw_can);
    }else {

        /* STB发送请求 */
        if(p_txmsg->flags & AM_CAN_ONCE_SEND_FLAG){
            amhw_hc32f07x_can_stb_sendone_enable(p_hw_can);
        }else {
            amhw_hc32f07x_can_stb_sendall_enable(p_hw_can);
        }
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 接收信息
 */
am_can_err_t __can_hc32f07x_msg_recv (void *p_drv, am_can_message_t *p_rxmsg)
{
    am_hc32f07x_can_dev_t       *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t         *p_hw_can = NULL;

    if (NULL == p_drv || NULL == p_rxmsg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    /* 初始化 */
    p_rxmsg->id     = 0;
    p_rxmsg->flags  = 0;
    p_rxmsg->msglen = 0;

    /* 判断接收缓冲区是否为空 */
    if (amhw_hc32f07x_can_get_rxbuf_status (p_hw_can)  == 
                                           AMHW_HC32F07X_CAN_RXBUF_FULL ) {
            return AM_CAN_NOERROR;
        }

    if (p_hw_can->rbuf.cst.ide == AMHW_HC32F07X_CAN_IDE_EXTENDED) {

        /* 使用扩展标识符  */
        p_rxmsg->flags |= AM_CAN_EXTERN_FLAG;

        /* 获取ID */
        p_rxmsg->id     = p_hw_can->rbuf.rbuf0;

    } else {
        /* 使用标准标识符  */

        /* 获取ID */
        p_rxmsg->id     = p_hw_can->rbuf.rbuf0;
    }

    /* 远程帧处理 */
    if (p_hw_can->rbuf.cst.rtr == AMHW_HC32F07X_CAN_RTR_REMOTE) {

        /* 远程帧 */
        p_rxmsg->flags |= AM_CAN_REMOTE_FLAG;
        p_rxmsg->msglen = 0;
    }

        /* 获取发送数据长度*/
    p_rxmsg->msglen = (uint8_t)0x0f &
                       p_hw_can->rbuf.cst.dlc;

    p_rxmsg->msgdata[3] = (uint8_t)0xff &
                              p_hw_can->rbuf.rbuf2>>24;
    p_rxmsg->msgdata[2] = (uint8_t)0xff &
                              p_hw_can->rbuf.rbuf2>>16;
    p_rxmsg->msgdata[1] = (uint8_t)0xff &
                              p_hw_can->rbuf.rbuf2>>8;
    p_rxmsg->msgdata[0] = (uint8_t)0xff &
                              p_hw_can->rbuf.rbuf2;

    p_rxmsg->msgdata[7] = (uint8_t)0xff &
                              p_hw_can->rbuf.rbuf3>>24;
    p_rxmsg->msgdata[6] = (uint8_t)0xff &
                              p_hw_can->rbuf.rbuf3>>16;
    p_rxmsg->msgdata[5] = (uint8_t)0xff &
                              p_hw_can->rbuf.rbuf3>>8;
    p_rxmsg->msgdata[4] = (uint8_t)0xff &
                              p_hw_can->rbuf.rbuf3;
 
    /* 释放接收BUF */
    amhw_hc32f07x_can_rxbuf_release (p_hw_can);

    return AM_CAN_NOERROR;
}

/**
 * \brief 停止发送信息
 */
am_can_err_t __can_hc32f07x_stop_msg_snd (void *p_drv)
{
    am_hc32f07x_can_dev_t    *p_dev            = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t      *p_hw_can         = NULL;
    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    if(p_dev->p_devinfo->type == AMHW_HC32F07X_CAN_PTB){
        amhw_hc32f07x_can_ptb_abort_enable (p_hw_can);	
    }else {
        amhw_hc32f07x_can_stb_abort_enable (p_hw_can);
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 设置滤波函数(扩展)
 */
am_can_err_t __can_hc32f07x_filter_tab_ext_set (void              *p_drv,
                                                am_can_filter_t   *p_filterbuff,
                                                size_t             lenth)
{
    am_hc32f07x_can_dev_t       *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t         *p_hw_can = NULL;
    uint8_t                      filt_num = 0;
    uint8_t                      filt_pos = 0;
    uint32_t                     id       = 0;
    uint32_t                     id_mask  = 0;
    am_can_err_t                 state    = AM_CAN_NOERROR;
    int                          i        = 0;
    if (NULL == p_drv || NULL == p_filterbuff || 0 == lenth || (lenth > 8)) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    amhw_hc32f07x_can_reset_set(p_hw_can);

    for ( i = 0; i < lenth; i++) {

        /* 滤波器组号  */
        filt_num = i;

        if (filt_num > 7 ) {
            /* 跳出当前 */
            state = AM_CAN_ILLEGAL_CHANNEL_NO;
            break;
        } else {
            /* 滤波器组 */
            filt_pos = (uint8_t)(1ul << filt_num);
        }

        /* 禁用滤波器  */
        p_hw_can->acfen &= ~(uint32_t)filt_pos;

        if(p_filterbuff->ide == AM_CAN_FRAME_TYPE_STD){
            id      = (p_filterbuff->id[0] & 0x7ff);
            id_mask = (p_filterbuff->mask[0] & 0x7ff);
        }else {
            id      = (p_filterbuff->id[0] & 0x1fffffff);
            id_mask = (p_filterbuff->mask[0] & 0x1fffffff);
        }

        amhw_hc32f07x_can_filter_addr (p_hw_can,
                                       (amhw_hc32f07x_can_filter_t)filt_num);

        if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_STD) {

            /* 只接收标准帧 */
            amhw_hc32f07x_can_ide_set (p_hw_can,
                                       AMHW_HC32F07X_CAN_AIDE_STD);
        }else if(p_filterbuff->ide == AM_CAN_FRAME_TYPE_EXT) {
            amhw_hc32f07x_can_ide_set (p_hw_can,
                                       AMHW_HC32F07X_CAN_AIDE_EXT);
        }else {
            state = AM_CAN_INVALID_PARAMETER;
            return state;
        }
        /* 滤波器id设置 */
        amhw_hc32f07x_can_acf_select_set (p_hw_can, 
                                          AMHW_HC32F07X_CAN_ACF_ID);
        amhw_hc32f07x_can_filter_code_set (p_hw_can, id);

        /* 滤波器mask设置 */
        amhw_hc32f07x_can_acf_select_set (p_hw_can, 
                                          AMHW_HC32F07X_CAN_ACF_ID);
        amhw_hc32f07x_can_filter_code_set (p_hw_can, id_mask);

        /* 激活滤波器  */
            p_hw_can->acfen |= (uint32_t)filt_pos;

        /* 滤波器组后移 */
        p_filterbuff++;
    }

    /* 切换到正常模式 */
    amhw_hc32f07x_can_reset_clr(p_hw_can);

    return state;
}


/**
 * \brief 获取滤波函数
 */
am_can_err_t __can_hc32f07x_filter_tab_ext_get (void              *p_drv,
                                              am_can_filter_t   *p_filterbuff,
                                              size_t            *p_lenth)
{
    am_hc32f07x_can_dev_t         *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t           *p_hw_can = NULL;
    uint8_t                     filter_group = 0;
    int                         i = 0;
    if (NULL == p_drv ) {
        return AM_CAN_INVALID_PARAMETER;
    }


    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    filter_group = p_hw_can->acfen & 0xff;

    for (i = 0;i < 8;i++) {

        /* 判断当前滤波器组是否激活 */
        if ( ( filter_group & ( (uint8_t)1ul<< i) ) ) {
            *p_lenth = *p_lenth + 1;
        } else {
            continue;
        }

        uint16_t reg_f_ide     = p_hw_can->acf >> 29;
        uint16_t reg_f_id      = p_hw_can->acf & 0x1fffffff;
        uint16_t reg_f_id_mask = p_hw_can->acf & 0x1fffffff;

        /* 帧格式 帧类型 */
        p_filterbuff->ide = (reg_f_ide & 0x01) ?
                            AM_CAN_FRAME_TYPE_EXT :
                            AM_CAN_FRAME_TYPE_STD;

        p_filterbuff->rtr = AM_CAN_FRAME_FORMAT_NOCARE;

        /* 扩展帧 */
        if (p_filterbuff->ide == AM_CAN_FRAME_TYPE_EXT) {
            p_filterbuff->id[0]   = (reg_f_id );
            p_filterbuff->mask[0] = (reg_f_id_mask );
        } else {
           /* 标准帧 */
           p_filterbuff->id[0]   = reg_f_id & 0x7ff;
           p_filterbuff->mask[0] = reg_f_id_mask & 0x7ff;
        }

        p_filterbuff++;
    }
    return AM_CAN_NOERROR;
}


static void __can_hc32f07x_intcb_func_run (am_hc32f07x_can_dev_t  *p_dev,
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
static void __can_hc32f07x_int_handle (void *p_drv)
{
    am_hc32f07x_can_dev_t    *p_dev     = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t      *p_hw_can  = NULL;

    if (NULL == p_drv) {
        return;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;


    /* 接收中断产生，并且rxfifo不为空 */
    if (amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_RECV)) {

        /* 执行接收回调函数 */
        __can_hc32f07x_intcb_func_run(p_dev, AM_CAN_INT_RX);

        /* 标志清除 */
        amhw_hc32f07x_can_irq_status_clr (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_RECV);
    }

    /* 发送中断产生，并且发送完成 */
    if (amhw_hc32f07x_can_get_irq_status (p_hw_can,
                           AMHW_HC32F07X_CAN_FLAG_TX_PTB)||
        amhw_hc32f07x_can_get_irq_status (p_hw_can,
                           AMHW_HC32F07X_CAN_FLAG_TX_STB)) {

        /* 执行发送回调函数 */
        __can_hc32f07x_intcb_func_run(p_dev, AM_CAN_INT_TX);

        /* 标志清除 */
        amhw_hc32f07x_can_irq_status_clr (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_TX_PTB);
        amhw_hc32f07x_can_irq_status_clr (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_TX_STB);
    }

    /* 数据溢出中断产生 */
    if (amhw_hc32f07x_can_get_irq_status (p_hw_can,
                           AMHW_HC32F07X_CAN_FLAG_RX_OVER))  {

        /* 执行数据溢出中断回调函数 */
        __can_hc32f07x_intcb_func_run(p_dev, AM_CAN_INT_DATAOVER);

        amhw_hc32f07x_can_irq_status_clr (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_RX_OVER);
    }

    /* 错误被动中断 */
    if (amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                        AMHW_HC32F07X_CAN_ERR_FLAG_PASSIVE)) {

        /* 执行警告中断回调函数 */
        __can_hc32f07x_intcb_func_run(p_dev, AM_CAN_INT_ERROR_PASSIVE);

        amhw_hc32f07x_can_irq_status_clr (p_hw_can,
                                          AMHW_HC32F07X_CAN_ERR_FLAG_PASSIVE);
    }

    /* 错误中断、总线错误中断、总线仲裁丢失处理 */
    if (amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_ERROR) ||
        amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                        AMHW_HC32F07X_CAN_ERR_FLAG_ARBILOST) ||
        amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                        AMHW_HC32F07X_CAN_ERR_FLAG_BUS_ERR) ) {

        /* 判断总线是否离线 */
        if (amhw_hc32f07x_can_status_get (p_hw_can, 
                                       AMHW_HC32F07X_CAN_FLAG_BUSOFF_ACTIVE)) {

            /* 执行总线关闭中断回调函数 */
            __can_hc32f07x_intcb_func_run(p_dev, AM_CAN_INT_BUS_OFF);

            amhw_hc32f07x_can_err_status_clr (p_hw_can,
                                          AMHW_HC32F07X_CAN_ERR_FLAG_BUS_ERR);
        } else {

           /* 执行错误中断回调函数 */
            __can_hc32f07x_intcb_func_run(p_dev, AM_CAN_INT_ERROR);

            amhw_hc32f07x_can_irq_status_clr (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_ERROR);
            amhw_hc32f07x_can_err_status_clr (p_hw_can,
                                          AMHW_HC32F07X_CAN_ERR_FLAG_ARBILOST);
        }
    }
}

/**
 * \brief CAN 状态
 */
am_can_err_t __can_hc32f07x_status_get (void              *p_drv,
                               am_can_int_type_t *p_int_type,
                               am_can_bus_err_t  *p_bus_err)
{
    am_hc32f07x_can_dev_t    *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
    amhw_hc32f07x_can_t      *p_hw_can = NULL;
    uint8_t                 err      = 0;  /* 错误寄存器值 */

    if (NULL == p_drv || NULL == p_int_type || NULL == p_bus_err) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    *p_int_type = 0;

    /* 接收中断 */
    if (amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_RECV)) {

        *p_int_type |= AM_CAN_INT_RX;
    }

    /* 发送中断 */
    if (amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_TX_PTB) ||
        amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_TX_STB)) {
        *p_int_type |= AM_CAN_INT_TX;
    }

    /* 数据溢出中断 */
    if (amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_RX_OVER)) {

        *p_int_type |= AM_CAN_INT_DATAOVER;
    }

    /* 错误中断、总线错误中断、总线仲裁丢失中断 */
    if (amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                          AMHW_HC32F07X_CAN_FLAG_ERROR) ||
        amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                   AMHW_HC32F07X_CAN_ERR_FLAG_ARBILOST) ||
        amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                    AMHW_HC32F07X_CAN_ERR_FLAG_PASSIVE) ||
        amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                    AMHW_HC32F07X_CAN_ERR_FLAG_BUS_ERR) ||
        amhw_hc32f07x_can_get_irq_status (p_hw_can,
                                    AMHW_HC32F07X_CAN_ERR_FLAG_EWARN))  {

        *p_int_type |= AM_CAN_INT_ERROR;

        /* 判断是否是因为错误计数器达到限制而引起的总线关闭 */
        if (amhw_hc32f07x_can_status_get (p_hw_can, 
                                  AMHW_HC32F07X_CAN_FLAG_BUSOFF_ACTIVE)) {
            *p_int_type |= AM_CAN_INT_BUS_OFF;
        }

        /* 判断是否是因为错误计数器达到限制而引起的错误警告 */
        if (amhw_hc32f07x_can_get_err_status (p_hw_can,
                                 AMHW_HC32F07X_CAN_ERR_FLAG_EWARN)) {
            *p_int_type |= AM_CAN_INT_WARN;
        }

        /* 判断是否是因为错误计数器达到限制而引起的错误被动 */
        if (amhw_hc32f07x_can_get_irq_status (p_hw_can,
                           AMHW_HC32F07X_CAN_ERR_FLAG_PASSIVE)) {
            *p_int_type |= AM_CAN_INT_ERROR_PASSIVE;
        }
    }

    /* 从寄存器获取错误代码 */
    *p_bus_err = 0;
    err        = amhw_hc32f07x_can_error_status_get (p_hw_can );

    /* 格式错误 */
    if (AMHW_HC32F07X_CAN_ERROR_FORM == err) {
        *p_bus_err |= AM_CAN_BUS_ERR_FORM;
    }

    /* 填充错误 */
    if (AMHW_HC32F07X_CAN_ERROR_STUFF == err) {
        *p_bus_err |= AM_CAN_BUS_ERR_STUFF;
    }

    /* 未知错误     显性错误   隐性错误  由软件设置错误*/
    if (AMHW_HC32F07X_CAN_ERROR_UNKOWN  == err) {
        *p_bus_err |= AM_CAN_BUS_ERR_UNKNOWN;
    }
		
		/* 位错误 */
    if (AMHW_HC32F07X_CAN_ERROR_BIT == err) {
        *p_bus_err |= AM_CAN_BUS_ERR_BIT;
    }

    /* ACK错误 */
    if (err == AMHW_HC32F07X_CAN_ERROR_ACK) {
        *p_bus_err |= AM_CAN_BUS_ERR_ACK;
    }

    /* CRC错误 */
    if (err == AMHW_HC32F07X_CAN_ERROR_CRC) {
        *p_bus_err |= AM_CAN_BUS_ERR_CRC;
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断连接
 */
am_can_err_t __can_hc32f07x_connect (void         *p_drv,
                            am_pfnvoid_t  pfn_isr,
                            void         *p_arg)
{
    am_hc32f07x_can_dev_t    *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    if (NULL != pfn_isr) {
        /* 连接传入函数 */
        am_int_connect(p_dev->p_devinfo->int_num, pfn_isr, p_arg);
    } else {
        /* 连接默认函数 */
        am_int_connect(p_dev->p_devinfo->int_num, __can_hc32f07x_int_handle, p_dev);

    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 删除中断连接
 */
am_can_err_t __can_hc32f07x_disconnect (void         *p_drv,
                                        am_pfnvoid_t  pfn_isr,
                                        void         *p_arg)
{
    am_hc32f07x_can_dev_t    *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;

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
        am_int_disconnect(p_dev->p_devinfo->int_num, __can_hc32f07x_int_handle, p_dev);

    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 中断回调函数的连接
 */
am_can_err_t __can_hc32f07x_intcb_connect (void              *p_drv,
                                  am_can_int_type_t  inttype,
                                  am_pfnvoid_t       pfn_callback,
                                  void              *p_arg)
{
    am_hc32f07x_can_dev_t    *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
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
am_can_err_t __can_hc32f07x_intcb_disconnect (void              *p_drv,
                                            am_can_int_type_t  inttype)
{
    am_hc32f07x_can_dev_t    *p_dev    = (am_hc32f07x_can_dev_t *)p_drv;
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
am_can_handle_t am_hc32f07x_can_init (am_hc32f07x_can_dev_t      *p_dev,
                               const am_hc32f07x_can_devinfo_t *p_devinfo)
{
    uint8_t             i = 0;

    if (NULL == p_dev || NULL == p_devinfo) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo = p_devinfo;

    p_dev->handle.p_drv_funcs = &__hc32f07x_can_dev_func;
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
void am_hc32f07x_can_deinit (am_can_handle_t handle)
{
    amhw_hc32f07x_can_t *p_hw_can = NULL;
    am_hc32f07x_can_dev_t  *p_dev = (am_hc32f07x_can_dev_t *)handle->p_drv;

    if (NULL == p_dev) {
        return;
    }

    p_hw_can = (amhw_hc32f07x_can_t *)p_dev->p_devinfo->regbase;

    amhw_hc32f07x_can_reset_set(p_hw_can);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    /* 中断禁能 */
    am_int_disable(p_dev->p_devinfo->int_num);

}

/* end of file */
