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

#include "am_hc32f460_can.h"
#include "hw/amhw_hc32f460_can.h"
#include "string.h"
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
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;

    amhw_hc32f460_can_irq_cmd(p_hw_can, CanRxIrqEn, CanEnable);

    return AM_CAN_NOERROR;
}

/**
 * \brief CAN复位（停止）
 */
am_can_err_t __can_reset (void *p_drv)
{
    return AM_CAN_NOERROR;
}

/**
 * \brief CAN休眠
 */
am_can_err_t __can_sleep (void *p_drv)
{
    return AM_CAN_NOERROR;
}

/**
 * \brief CAN唤醒
 */
am_can_err_t __can_wakeup (void *p_drv)
{
    return AM_CAN_NOERROR;
}

/**
 * \brief 中断使能
 */
am_can_err_t __can_int_enable (void *p_drv, am_can_int_type_t int_mask)
{
    return AM_CAN_NOERROR;
}

/**
 * \brief 中断禁能
 */
am_can_err_t __can_int_disable (void *p_drv, am_can_int_type_t int_mask)
{
    return AM_CAN_NOERROR;
}

/**
 * \brief 模式设置
 */
am_can_err_t __can_mode_set (void *p_drv, am_can_mode_type_t mode)
{
    am_hc32f460_can_dev_t *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t   *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;

    if (AM_CAN_MODE_NROMAL == mode) {

        /* 正常模式 */
        amhw_hc32f460_can_mode_config(p_hw_can, CanListenOnlyMode, CanDisable);

    } else if ((AM_CAN_MODE_LISTEN_ONLY == mode) &&
                 (p_dev->p_devinfo->type == AMHW_HC32F460_CAN_PELI_CAN)) {

        /* 只听模式 */
        amhw_hc32f460_can_mode_config(p_hw_can, CanListenOnlyMode, CanEnable);
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
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t      *p_hw_can = NULL;
    stc_can_init_config_t   stcCanInitCfg;
    
    if (NULL == p_drv || NULL == p_can_baudrate) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;        
    memset(&stcCanInitCfg, 0, sizeof(stcCanInitCfg));

    stcCanInitCfg.stcCanBt.PRESC = p_can_baudrate->brp;
    stcCanInitCfg.stcCanBt.SEG_1 = p_can_baudrate->tesg1;
    stcCanInitCfg.stcCanBt.SEG_2 = p_can_baudrate->tesg2;
    stcCanInitCfg.stcCanBt.SJW   = p_can_baudrate->sjw;
    stcCanInitCfg.stcWarningLimit.CanErrorWarningLimitVal = 10u;
    stcCanInitCfg.stcWarningLimit.CanWarningLimitVal = 16u-1u;
    stcCanInitCfg.enCanRxBufAll  = CanRxNormal;
    stcCanInitCfg.enCanRxBufMode = CanRxBufNotStored;
    stcCanInitCfg.enCanSAck      = CanSelfAckEnable;
    stcCanInitCfg.enCanSTBMode   = CanSTBFifoMode;

    amhw_hc32f460_can_lowlevel_set(p_hw_can, &stcCanInitCfg);

    return AM_CAN_NOERROR;
}

/**
 * \brief 波特率获取
 */
am_can_err_t __can_baudrate_get (void               *p_drv,
                                 am_can_bps_param_t *p_can_baudrate)
{
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t      *p_hw_can = NULL;
    stc_can_init_config_t   stcCanInitCfg;
    
    if (NULL == p_drv || NULL == p_can_baudrate) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;
    memset(&stcCanInitCfg, 0, sizeof(stcCanInitCfg));

    amhw_hc32f460_can_lowlevel_get(p_hw_can, &stcCanInitCfg);
    
    p_can_baudrate->brp = stcCanInitCfg.stcCanBt.PRESC;
    p_can_baudrate->sjw = stcCanInitCfg.stcCanBt.SJW;
    p_can_baudrate->tesg1 = stcCanInitCfg.stcCanBt.SEG_1;
    p_can_baudrate->tesg2 = stcCanInitCfg.stcCanBt.SEG_2;

    return AM_CAN_NOERROR;
}

/**
 * \brief 获取错误计数
 */
am_can_err_t __can_err_cnt_get (void *p_drv, am_can_err_cnt_t *p_can_err_reg)
{
    am_hc32f460_can_dev_t *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t   *p_hw_can = NULL;

    if (NULL == p_drv || NULL == p_can_err_reg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;

    /* 读 CAN TX/RX错误计数器 */
    p_can_err_reg->rx_error_cnt = amhw_hc32f460_can_rx_error_cnt_get(p_hw_can);
    p_can_err_reg->tx_error_cnt = amhw_hc32f460_can_tx_error_cnt_get(p_hw_can);

    return AM_CAN_NOERROR;
}

/**
 * \brief 清除错误计数
 */
am_can_err_t __can_err_cnt_clr (void *p_drv)
{
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;

    amhw_hc32f460_can_rx_error_cnt_clr(p_hw_can);
    amhw_hc32f460_can_tx_error_cnt_clr(p_hw_can);
    
    return AM_CAN_NOERROR;
}

/**
 * \brief 发送信息
 */
am_can_err_t __can_msg_send (void *p_drv, am_can_message_t *p_txmsg)
{
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t      *p_hw_can = NULL;
    uint8_t u8Idx = 0u;
    stc_can_txframe_t       stcTxFrame;

    if (NULL == p_drv || NULL == p_txmsg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;

    memset(&stcTxFrame, 0, sizeof(stcTxFrame));

    stcTxFrame.TBUF32_0      = p_txmsg->id;
    stcTxFrame.Control_f.DLC = p_txmsg->msglen;

    if (p_txmsg->flags & AM_CAN_REMOTE_FLAG) {
        stcTxFrame.Control_f.RTR = 1;
    }

    if (p_txmsg->flags & AM_CAN_EXTERN_FLAG) {
        stcTxFrame.Control_f.IDE = 1;
    }
    
    for(u8Idx = 0u; u8Idx < p_txmsg->msglen; u8Idx++)
    {
        stcTxFrame.Data[u8Idx] = p_txmsg->msgdata[u8Idx];
    }
    
    amhw_hc32f460_can_set_frame(p_hw_can, &stcTxFrame);
    amhw_hc32f460_can_transmit_cmd(p_hw_can, CanPTBTxCmd);
    
    return AM_CAN_NOERROR;
}


/**
 * \brief 接收信息
 */
am_can_err_t __can_msg_recv (void *p_drv, am_can_message_t *p_rxmsg)
{
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t      *p_hw_can = NULL;
    uint8_t                  i;
    stc_can_rxframe_t       stcRxFrame;

    if (NULL == p_drv || NULL == p_rxmsg) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;

    /* 初始化 */
    p_rxmsg->id     = 0;
    p_rxmsg->flags  = 0;
    p_rxmsg->msglen = 0;
    memset(&stcRxFrame, 0, sizeof(stcRxFrame));
    
    /* 判断接收缓冲区是否为空 */
    if(AM_FALSE == amhw_hc32f460_can_irq_flag_get(p_hw_can, CanRxIrqFlg)) {
        return AM_CAN_NOERROR;
    }

    amhw_hc32f460_can_irq_flag_clr(p_hw_can, CanRxIrqFlg);
    
    amhw_hc32f460_can_receive(p_hw_can, &stcRxFrame);
    p_rxmsg->id = stcRxFrame.RBUF32_0;
    
    if(1u == stcRxFrame.Cst.Control_f.RTR)
    {
        p_rxmsg->flags |= AM_CAN_REMOTE_FLAG;
        p_rxmsg->msglen = 0;
        return AM_CAN_NOERROR;
    }  
    
    if(1u == stcRxFrame.Cst.Control_f.IDE)    {
        p_rxmsg->flags |= AM_CAN_EXTERN_FLAG;
    }  
    
    p_rxmsg->msglen = stcRxFrame.Cst.Control_f.DLC;
    /* 读取数据 */
    for (i = 0 ; i < p_rxmsg->msglen ; i++) {
        p_rxmsg->msgdata[i] = stcRxFrame.Data[i];
    }

    return AM_CAN_NOERROR;
}

/**
 * \brief 停止发送信息
 */
am_can_err_t __can_stop_msg_snd (void *p_drv)
{
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t      *p_hw_can = NULL;

    if (NULL == p_drv) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;
   
    amhw_hc32f460_can_transmit_cmd(p_hw_can, CanPTBTxAbortCmd);
    amhw_hc32f460_can_transmit_cmd(p_hw_can, CanSTBTxAbortCmd);
   
    return AM_CAN_NOERROR;
}

/**
 * \brief 设置滤波函数
 */
am_can_err_t __can_filter_tab_set (void    *p_drv,
                                   uint8_t *p_filterbuff,
                                   size_t   lenth)
{
    am_hc32f460_can_dev_t    *p_dev     = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t      *p_hw_can  = NULL;
    stc_can_filter_t        stcFilter;
    if (NULL == p_drv || NULL == p_filterbuff || 0 == lenth) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;

    /* Can filter config */
    stcFilter.enAcfFormat = CanAllFrames;
    stcFilter.enFilterSel = CanFilterSel1;
    stcFilter.u32CODE     = 0x00000000ul;
    stcFilter.u32MASK     = 0x1FFFFFFFul;
    amhw_hc32f460_can_filter_config(p_hw_can, &stcFilter, CanEnable);

    return AM_CAN_NOERROR;
}

/**
 * \brief 设置滤波函数(扩展) 只有一组滤波器 不支持扩展和标准的屏蔽位
 */
am_can_err_t __can_filter_tab_ext_set (void             *p_drv,
                                       am_can_filter_t  *p_filterbuff,
                                       size_t            lenth)
{
    return AM_CAN_NOERROR;
}

/**
 * \brief 获取滤波函数
 */
am_can_err_t __can_filter_tab_get (void    *p_drv,
                                   uint8_t *p_filterbuff,
                                   size_t  *p_lenth)
{
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
    return AM_CAN_NOERROR;
}

/**
 * \brief can中断服务函数
 */
static void __can_int_handle (void *p_drv)
{

}

/**
 * \brief CAN 状态
 */
am_can_err_t __can_status_get (void              *p_drv,
                               am_can_int_type_t *p_int_type,
                               am_can_bus_err_t  *p_bus_err)
{
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
    amhw_hc32f460_can_t      *p_hw_can = NULL;
    uint8_t                  int_flags = 0;  /* 中断标志 */
    uint8_t                  err_code  = 0;  /* 错误号 */

    if (NULL == p_drv || NULL == p_int_type || NULL == p_bus_err) {
        return AM_CAN_INVALID_PARAMETER;
    }

    p_hw_can = (amhw_hc32f460_can_t *)p_dev->p_devinfo->regbase;
    
    int_flags = amhw_hc32f460_can_int_status_get(p_hw_can);

    *p_int_type = 0;

    /* 接收中断 */
    if (int_flags & AMHW_HC32F460_CAN_INT_RECV) {
        *p_int_type |= AM_CAN_INT_RX;
    }

    /* 发送中断 */
    if ((int_flags & AMHW_HC32F460_CAN_INT_PTB_TRAN) || (int_flags & AMHW_HC32F460_CAN_INT_STB_TRAN)) {
        *p_int_type |= AM_CAN_INT_TX;
    }

    /* 数据溢出中断 */
    if (int_flags & AMHW_HC32F460_CAN_INT_OVER) {
        *p_int_type |= AM_CAN_INT_DATAOVER;
    }

    /* 错误中断、总线错误中断、总线仲裁丢失中断 */
    if ((int_flags & AMHW_HC32F460_CAN_INT_ERR)) {

        *p_int_type |= AM_CAN_INT_ERROR;
    }
    
    if (AM_TRUE == amhw_hc32f460_can_status_get(p_hw_can, CanBusoff)) {
        *p_int_type |= AM_CAN_INT_BUS_OFF;
    }
   
////////////////////////////////////////////////////////////////////////////////
    /* 从寄存器获取错误代码 */
    *p_bus_err = 0;
    err_code  = amhw_hc32f460_can_error_status_get(p_hw_can);

    /* 比特错误 */
    if (BIT_ERROR == err_code) {
        *p_bus_err |= AM_CAN_BUS_ERR_BIT;
    }

    /* 格式错误 */
    if (FORM_ERROR == err_code) {
        *p_bus_err |= AM_CAN_BUS_ERR_FORM;
    }

    /* 填充错误 */
    if (STUFF_ERROR == err_code) {
        *p_bus_err |= AM_CAN_BUS_ERR_STUFF;
    }

    /* 未知错误 */
    if (UNKOWN_ERROR == err_code) {
        *p_bus_err |= AM_CAN_BUS_ERR_UNKNOWN;
    }

    /* ACK错误 */
    if (ACK_ERROR == err_code) {
        *p_bus_err |= AM_CAN_BUS_ERR_ACK;
    }

    /* CRC错误 */
    if (CRC_ERROR == err_code) {
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
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;

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
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;

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
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
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
    am_hc32f460_can_dev_t    *p_dev    = (am_hc32f460_can_dev_t *)p_drv;
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
am_can_handle_t am_hc32f460_can_init (am_hc32f460_can_dev_t           *p_dev,
                               const am_hc32f460_can_devinfo_t *p_devinfo)
{
    uint8_t             i = 0;
    if (NULL == p_dev || NULL == p_devinfo) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo = p_devinfo;

    p_dev->handle.p_drv_funcs = &dev_func;
    p_dev->handle.p_drv       = (void *)p_dev;

    p_dev->mode               = AMHW_HC32F460_CAN_MODE_RESET;

    for (i = 0 ; i < p_devinfo->p_intcb_num ; i++) {
        p_devinfo->p_intcb_info[i].pfn_callback = NULL;
        p_devinfo->p_intcb_info[i].p_arg        = NULL;
    }

    return (am_can_handle_t)&(p_dev->handle);
}

/**
 * \brief 解初始化
 */
void am_hc32f460_can_deinit (am_can_handle_t handle)
{
    am_hc32f460_can_dev_t  *p_dev = (am_hc32f460_can_dev_t *)handle->p_drv;

    if (NULL == p_dev) {
        return;
    }

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    /* 中断禁能 */
    am_int_disable(p_dev->p_devinfo->int_num);
}

/* end of file */
