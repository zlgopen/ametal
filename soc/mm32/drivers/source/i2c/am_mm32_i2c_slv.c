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
 * \brief I2C从机驱动，服务I2C从机标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 17-09-06  vir, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_mm32_i2c_slv.h"
#include "hw/amhw_mm32_i2c.h"

/*******************************************************************************
  函数声明
*******************************************************************************/

/** \brief I2C硬件初始化 */
static int __i2c_slv_hard_init(am_mm32_i2c_slv_dev_t *p_dev);

/** \brief I2C中断处理函数 */
static void __i2c_slv_irq_handler (void *p_arg);

/** \brief 开启I2C从机设备 */
static int __i2c_slv_setup (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev);
/** \brief 关闭 I2C从机设备 */
static int __i2c_slv_shutdown (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev);
/** \brief 获取可用的从机设备个数 */
static int __i2c_slv_num_get(void *p_drv);

/**
 * \brief I2C从设备 驱动函数定义
 */
static am_const struct am_i2c_slv_drv_funcs __g_i2c_slv_drv_funcs = {
    __i2c_slv_setup,
    __i2c_slv_shutdown,
    __i2c_slv_num_get,
};

/**
 *  \brief I2C硬件初始化
 *  \note  1: 设置从设备的地址 及地址位数
 *         2：开启中断
 */
static int __i2c_slv_hard_init (am_mm32_i2c_slv_dev_t *p_dev)
{
    amhw_mm32_i2c_t *p_hw_i2c  = NULL;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_mm32_i2c_t *) (p_dev->p_devinfo->i2c_regbase);

    /* 关闭I2C控制器，配置参数 */
    amhw_mm32_i2c_disable(p_hw_i2c);

    amhw_mm32_i2c_rx_tl_set(p_hw_i2c,0 );
    amhw_mm32_i2c_tx_tl_set(p_hw_i2c, 1);

    /*
     * 一些中断的设置
     */

    /** 只开启 从机 接受中断 和  读请求中断 */
    amhw_mm32_i2c_intr_mask_clear(p_hw_i2c,0XFFF);
    amhw_mm32_i2c_intr_mask_set (p_hw_i2c,AMHW_MM32_INT_FLAG_RX_FULL   | 
	                                     AMHW_MM32_INT_FLAG_RD_REQ    |
                                         AMHW_MM32_INT_FLAG_START_DET);
    /* 清除中断状态 */
    amhw_mm32_i2c_clr_intr_get (p_hw_i2c);
    amhw_mm32_i2c_enable(p_hw_i2c);

    return AM_OK;
}

/**
 * \brief 开始从机设备
 */
static int __i2c_slv_setup (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev)
{
    int     i;
    uint8_t mm32_i2c_slv_dev_num ;
    am_mm32_i2c_slv_dev_t *p_dev      = NULL;
    amhw_mm32_i2c_t       *p_hw_i2c   = NULL;

    if ((p_drv         == NULL) ||
        (p_i2c_slv_dev == NULL)) {
        return -AM_EINVAL;
    }

    p_dev    = (am_mm32_i2c_slv_dev_t *)p_drv;
    p_hw_i2c = (amhw_mm32_i2c_t *) (p_dev->p_devinfo->i2c_regbase);

    /* 获取总的从设备个数 */
    mm32_i2c_slv_dev_num = p_dev->mm32_i2c_slv_dev_num;

    for ( i = 0; i < mm32_i2c_slv_dev_num; i++) {
        if( NULL == p_dev->p_i2c_slv_dev[i]) {
            p_dev->p_i2c_slv_dev[i] = p_i2c_slv_dev;
            break ;
        }
    }

    /* 从机设备都用关了，无法开启该从机设备 */
    if ( i == mm32_i2c_slv_dev_num) {
        return -AM_EAGAIN;
    }

    /* 关闭I2C控制器，配置参数 */
    amhw_mm32_i2c_disable (p_hw_i2c);

    /* 判断是否 需要响应 广播地址 */
    if ( p_i2c_slv_dev->dev_flags & AM_I2C_SLV_GEN_CALL_ACK) {
        amhw_mm32_i2c_gen_call_ack(p_hw_i2c);
        amhw_mm32_i2c_intr_mask_set(p_hw_i2c,AMHW_MM32_INT_FLAG_GEN_CALL);
    } else {
        amhw_mm32_i2c_gen_call_nack(p_hw_i2c);
    }

    /**
     * \brief 根据  i 存放从机设备地址。
     *         i = 0 ,存放到第1个从地址
     *         i = 1 ,存放到第2从地址 ，以此类推
     *
     * \note ZLG只有一个从机地址 ,直接存放即可 。
     */
    amhw_mm32_i2c_slave_addr_set (p_hw_i2c, p_i2c_slv_dev->dev_addr);

    /* 设置从机设备的地址位数 */
    if ( p_i2c_slv_dev->dev_flags  &  AM_I2C_SLV_ADDR_10BIT) {
        p_hw_i2c->ic_con = AMHW_MM32_I2C_SLAVE  |
                       AMHW_MM32_I2C_10BITADDR_SLAVE;
    } else {
        p_hw_i2c->ic_con = AMHW_MM32_I2C_SLAVE  |
                      AMHW_MM32_I2C_7BITADDR_SLAVE;
    }

    amhw_mm32_i2c_enable(p_hw_i2c);
    return AM_OK;
}

/**
 * \brief 关闭 从机设备
 */
static int __i2c_slv_shutdown (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev)
{
    am_mm32_i2c_slv_dev_t *p_dev    = (am_mm32_i2c_slv_dev_t *)p_drv;
    amhw_mm32_i2c_t   *p_hw_i2c_slv = NULL;

    if ( (p_dev              == NULL) ||
         (p_i2c_slv_dev      == NULL)) {
        return -AM_EINVAL;
    }
    p_hw_i2c_slv = (amhw_mm32_i2c_t *) p_dev->p_devinfo->i2c_regbase;

    /**
     * \brief 失能对应从地址 ,根据从机地址可以确定该关闭哪一个从机设备
     *
     * \note ZLG只能生成一个从设备 ，直接关闭失能I2C即可
     */
    p_dev->p_i2c_slv_dev[0] = NULL;
    amhw_mm32_i2c_disable (p_hw_i2c_slv);

    return AM_OK;
}

/**
 * \brief 获取可用从机设备个数
 */
static int __i2c_slv_num_get (void *p_drv)
{
    int     i;
    int     count = 0;
    uint8_t mm32_i2c_slv_dev_num ;
    am_mm32_i2c_slv_dev_t *p_dev = (am_mm32_i2c_slv_dev_t *)p_drv;

    if ((p_dev == NULL)) {
        return -AM_EINVAL;
    }
    /* 获取总的从设备个数 */
    mm32_i2c_slv_dev_num = p_dev->mm32_i2c_slv_dev_num;

    for (i = 0; i < mm32_i2c_slv_dev_num; i++) {
        if (NULL == p_dev->p_i2c_slv_dev[i]) {
            count++;
        }
    }
    return count;
}

/**
 * \brief 软件定时器回调函数
 *
 * \note 在开始信号之后 ，若主机超过1秒还未发停止信号 ，则将从机进行复位
 */
static void __i2c_slv_timing_callback (void *p_arg)
{
    am_mm32_i2c_slv_dev_t *p_dev = ( am_mm32_i2c_slv_dev_t *) p_arg;

#if 1
    amhw_mm32_i2c_t     *p_hw_i2c  = NULL;
    p_hw_i2c  = (amhw_mm32_i2c_t *) (p_dev->p_devinfo->i2c_regbase);

    /* 平台初始化之后将 寄存器的值复位 */
    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    /* 关闭I2C控制器，配置参数 */
    amhw_mm32_i2c_disable (p_hw_i2c);

    /** 判断是否 需要响应 广播地址 */
    if ( p_dev->p_i2c_slv_dev[0]->dev_flags & AM_I2C_SLV_GEN_CALL_ACK) {
        amhw_mm32_i2c_gen_call_ack(p_hw_i2c);
        amhw_mm32_i2c_intr_mask_set(p_hw_i2c,AMHW_MM32_INT_FLAG_GEN_CALL);
    } else {
        amhw_mm32_i2c_gen_call_nack(p_hw_i2c);
    }

    amhw_mm32_i2c_slave_addr_set (p_hw_i2c, p_dev->p_i2c_slv_dev[0]->dev_addr);

    /* 设置从机设备的地址位数 */
    if ( p_dev->p_i2c_slv_dev[0]->dev_flags  &  AM_I2C_SLV_ADDR_10BIT) {
        p_hw_i2c->ic_con = AMHW_MM32_I2C_SLAVE  |
                       AMHW_MM32_I2C_10BITADDR_SLAVE;
    } else {
        p_hw_i2c->ic_con = AMHW_MM32_I2C_SLAVE  |
                      AMHW_MM32_I2C_7BITADDR_SLAVE;
    }

    amhw_mm32_i2c_enable(p_hw_i2c);
#endif

    __i2c_slv_hard_init(p_dev);

    /** 停止定时 */
    am_softimer_stop(&p_dev->softimer);
}

/**
 * \brief 从机接受数据
 */
void __i2c_slv_irq_rx_handler (am_mm32_i2c_slv_dev_t *p_dev)
{
    amhw_mm32_i2c_t *p_hw_i2c      = (amhw_mm32_i2c_t *)p_dev->p_devinfo->i2c_regbase;
    am_i2c_slv_device_t *p_i2c_slv = p_dev->p_i2c_slv_dev[0];

    /* 接受数据 */
    uint8_t rx_data = amhw_mm32_i2c_dat_read (p_hw_i2c);

    /* 接受回调 */
    if ( p_dev->is_gen_call == AM_TRUE) {
        /** 如果是广播呼叫 */
        if( p_i2c_slv->p_cb_funs->pfn_gen_call) {
            p_i2c_slv->p_cb_funs->pfn_gen_call(p_i2c_slv->p_arg, rx_data);
        }
    } else {

       if ( p_i2c_slv->p_cb_funs->pfn_rxbyte_put) {
            p_i2c_slv->p_cb_funs->pfn_rxbyte_put( p_i2c_slv->p_arg, rx_data);
       }
    }
}

/**
 * \brief 从机发送数据
 */
void __i2c_slv_irq_tx_handler (am_mm32_i2c_slv_dev_t *p_dev)
{
    amhw_mm32_i2c_t   *p_hw_i2c  = (amhw_mm32_i2c_t *)p_dev->p_devinfo->i2c_regbase;
    am_i2c_slv_device_t *p_i2c_slv = p_dev->p_i2c_slv_dev[0];

    uint8_t  tx_data = 0;

    /**
     * \brief 通过发送回调来获取数据 ，
     *        此FIFO有两个字节 + 移位寄存器放一个字节 ，因此可以连续取三个字节
     */
    if (NULL != p_i2c_slv->p_cb_funs->pfn_txbyte_get) {
         p_i2c_slv->p_cb_funs->pfn_txbyte_get( p_i2c_slv->p_arg, &tx_data);
    }
    amhw_mm32_i2c_dat_write(p_hw_i2c, tx_data);

    /* 清除读请求 */
    amhw_mm32_i2c_clr_rd_req_get(p_hw_i2c);
}

/**
 * \brief I2C 中断处理函数
 *
 * \param[in] p_arg : 指向I2C设备结构体的指针
 *
 * \return 无
 */
static void __i2c_slv_irq_handler (void *p_arg)
{
        
    am_mm32_i2c_slv_dev_t  *p_dev              = (am_mm32_i2c_slv_dev_t *)p_arg;
    amhw_mm32_i2c_t        *p_hw_i2c_slv       = (amhw_mm32_i2c_t *)p_dev->p_devinfo->i2c_regbase;
    am_i2c_slv_device_t   *p_i2c_slv_dev      = p_dev->p_i2c_slv_dev[0];
    amhw_mm32_int_flag_t    i2c_slv_int_status = amhw_mm32_i2c_intr_stat_get(p_hw_i2c_slv);

    /**
     * \brief 开始信号
     *
     * \note  只要总线上有开始信号就会触发该中断
     */
    if ( i2c_slv_int_status & AMHW_MM32_INT_FLAG_START_DET) {
        /** 清除开始中断 */
        amhw_mm32_i2c_clr_start_det_get(p_hw_i2c_slv);

        /**
         *  \brief 判断是不是 与本设备进行通信
         *
         *  \note 只有从机地址匹配了  从机状态机活动状态位 才会置1
         */
        if ( p_hw_i2c_slv->ic_status & AMHW_MM32_STATUS_FLAG_SLV_ACTIVITY ) {
            /* 地址匹配回调 ,是读还是写该MCU无法确定 */
            if( NULL != p_i2c_slv_dev->p_cb_funs->pfn_addr_match) {
                p_i2c_slv_dev->p_cb_funs->pfn_addr_match(p_i2c_slv_dev->p_arg, AM_TRUE);
            }

            /** 打开停止中断 */
            amhw_mm32_i2c_intr_mask_set (p_hw_i2c_slv,AMHW_MM32_INT_FLAG_STOP_DET);
            /* 开始用软件定时器计时 1000ms */
            am_softimer_start(&p_dev->softimer, 1000);
        }
    }

    /* 判断是否是广播呼叫 */
    if( i2c_slv_int_status & AMHW_MM32_INT_FLAG_GEN_CALL ) {
        amhw_mm32_i2c_clr_gen_call_get(p_hw_i2c_slv );
        p_dev->is_gen_call = AM_TRUE;
    }

    /**
     * \brief 接受缓存非空时 ，从机准备接受数据
     */
    if ( i2c_slv_int_status & AMHW_MM32_INT_FLAG_RX_FULL) {
        /* 开始接收数据 */
        __i2c_slv_irq_rx_handler (p_dev);
    }

    /**
     * \brief 发送数据
     */
    /** 当有读请求时 */
    if ( i2c_slv_int_status & AMHW_MM32_INT_FLAG_RD_REQ) {
        /* 开始处理数据 */
        __i2c_slv_irq_tx_handler (p_dev);
        /**
         * \brief
         * \note 发送阈值设定的为1时，    FIFO含有的字节数 小于等于 1就就会触发发送空中断
         *       发送阈值设定的为0时，    FIFO含有的字节数 小于等于 0就就会触发发送空中断
         */
        amhw_mm32_i2c_intr_mask_set(p_hw_i2c_slv, AMHW_MM32_INT_FLAG_TX_EMPTY);
    }
    /* 缓存区有空则发送数据 */
    if (i2c_slv_int_status & AMHW_MM32_INT_FLAG_TX_EMPTY ) {
        __i2c_slv_irq_tx_handler (p_dev);
    }

    /**
     * \brief 传输数据结束时
     */
    if( i2c_slv_int_status & AMHW_MM32_INT_FLAG_STOP_DET) {

        /** 若主机不需接受数据时 ，缓存区可能还有数据，发送终止将置1,及时清除 */
        if ( p_hw_i2c_slv->ic_raw_intr_stat & AMHW_MM32_INT_FLAG_TX_ABRT ) {
            amhw_mm32_i2c_clr_tx_abrt_get(p_hw_i2c_slv);
        }

        /** 清除停止中断 */
        amhw_mm32_i2c_clr_stop_det_get(p_hw_i2c_slv);
        /** 关闭停止中断 */
        amhw_mm32_i2c_intr_mask_clear(p_hw_i2c_slv,AMHW_MM32_INT_FLAG_STOP_DET);
        p_dev->is_gen_call = AM_FALSE;

        /* 执行传输停止回调 */
        if ( NULL != p_i2c_slv_dev->p_cb_funs->pfn_tran_stop) {
            p_i2c_slv_dev->p_cb_funs->pfn_tran_stop(p_i2c_slv_dev->p_arg);
        }

        amhw_mm32_i2c_intr_mask_clear(p_hw_i2c_slv, AMHW_MM32_INT_FLAG_TX_EMPTY);

        /* 传输结束停止计时 */
        am_softimer_stop(&p_dev->softimer);
    }
}

/**
 * \brief I2C初始化
 *
 * \note 硬件初始化 通过用户调用开始从设备来初始化
 */
am_i2c_slv_handle_t am_mm32_i2c_slv_init (am_mm32_i2c_slv_dev_t           *p_dev,
                                         const am_mm32_i2c_slv_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL ) {
        return NULL;
    }

    p_dev->i2c_slv_serv.p_funcs = (struct am_i2c_slv_drv_funcs *)&__g_i2c_slv_drv_funcs;
    p_dev->i2c_slv_serv.p_drv   = p_dev;

    p_dev->p_devinfo   = p_devinfo;
    p_dev->mm32_i2c_slv_dev_num =1;  /* 只能生成一个从机设备 */

    /* p_dev->current_matching_num =0; 默认当前匹配为 0 */

    p_dev->is_gen_call = AM_FALSE;

    p_dev->p_i2c_slv_dev[0] = NULL;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* 初始化软件定时器 */
    am_softimer_init(&p_dev->softimer, __i2c_slv_timing_callback, p_dev);

    __i2c_slv_hard_init(p_dev);

    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum, __i2c_slv_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->i2c_slv_serv);
}

/**
 * \brief I2C解初始化
 */
void am_mm32_i2c_slv_deinit (am_i2c_slv_handle_t handle)
{
    amhw_mm32_i2c_t       *p_hw_i2c_slv = NULL;
    am_mm32_i2c_slv_dev_t *p_dev    = NULL;

    if (NULL == handle) {
        return ;
    }

    p_dev    = (am_mm32_i2c_slv_dev_t *)handle;
    p_hw_i2c_slv = (amhw_mm32_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    amhw_mm32_i2c_disable (p_hw_i2c_slv);

    /* 去除中断连接 */
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __i2c_slv_irq_handler, (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
