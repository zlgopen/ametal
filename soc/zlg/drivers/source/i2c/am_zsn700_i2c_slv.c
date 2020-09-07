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
 * - 1.00 20-03-18
 * \endinternal
 */

#include "ametal.h"
#include "am_zlg_i2c_slv.h"
#include "hw/amhw_zsn700_i2c.h"

/*******************************************************************************
  函数声明
*******************************************************************************/

/** \brief I2C硬件初始化 */
static int __i2c_slv_hard_init(am_zlg_i2c_slv_dev_t *p_dev);
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
static int __i2c_slv_hard_init (am_zlg_i2c_slv_dev_t *p_dev)
{
    amhw_zsn700_i2c_t *p_hw_i2c  = NULL;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_zsn700_i2c_t *) (p_dev->p_devinfo->i2c_regbase);

    /* 清空I2C控制寄存器 */
    p_hw_i2c->i2c_cr = 0;

    /* 从机模式，开启应答 */
    amhw_zsn700_i2c_cr_set(p_hw_i2c, AMHW_ZSN700_I2C_REPLY_ENABLE);

    /* 禁用波特率计数器 */
    amhw_zsn700_i2c_tm_disable(p_hw_i2c);

    return AM_OK;
}

/**
 * \brief 开始从机设备
 */
static int __i2c_slv_setup (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev)
{
    int                     i;
    uint8_t                 zlg_i2c_slv_dev_num ;
    am_zlg_i2c_slv_dev_t   *p_dev      = NULL;
    amhw_zsn700_i2c_t      *p_hw_i2c   = NULL;

    if ((p_drv == NULL) || (p_i2c_slv_dev == NULL)) {
        return -AM_EINVAL;
    }

    p_dev    = (am_zlg_i2c_slv_dev_t *)p_drv;
    p_hw_i2c = (amhw_zsn700_i2c_t *)(p_dev->p_devinfo->i2c_regbase);

    /* 获取总的从设备个数 */
    zlg_i2c_slv_dev_num = p_dev->zlg_i2c_slv_dev_num;

    for ( i = 0; i < zlg_i2c_slv_dev_num; i++) {
        if( NULL == p_dev->p_i2c_slv_dev[i]) {
            p_dev->p_i2c_slv_dev[i] = p_i2c_slv_dev;
            break ;
        }
    }

    /* 从机设备都用关了，无法开启该从机设备 */
    if ( i == zlg_i2c_slv_dev_num) {
        return -AM_EAGAIN;
    }

    /* 关闭I2C控制器，配置参数 */
    amhw_zsn700_i2c_disable (p_hw_i2c);

    /* 判断是否 需要响应 广播地址 */
    if ( p_i2c_slv_dev->dev_flags & AM_I2C_SLV_GEN_CALL_ACK) {
        amhw_zsn700_i2c_gen_call_ack(p_hw_i2c);
    } else {
        amhw_zsn700_i2c_gen_call_nack(p_hw_i2c);
    }

    /**
     * \brief 根据  i 存放从机设备地址。
     *         i = 0 ,存放到第1个从地址
     *         i = 1 ,存放到第2从地址 ，以此类推
     *
     * \note ZLG只有一个从机地址 ,直接存放即可 。
     */
    amhw_zsn700_i2c_slave_addr_set (p_hw_i2c, p_i2c_slv_dev->dev_addr);

    amhw_zsn700_i2c_enable(p_hw_i2c);
    am_int_enable(p_dev->p_devinfo->inum);

    return AM_OK;
}

/**
 * \brief 关闭 从机设备
 */
static int __i2c_slv_shutdown (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev)
{
    am_zlg_i2c_slv_dev_t *p_dev    = (am_zlg_i2c_slv_dev_t *)p_drv;
    amhw_zsn700_i2c_t   *p_hw_i2c_slv = NULL;

    if ( (p_dev              == NULL) ||
         (p_i2c_slv_dev      == NULL)) {
        return -AM_EINVAL;
    }
    p_hw_i2c_slv = (amhw_zsn700_i2c_t *) p_dev->p_devinfo->i2c_regbase;

    /**
     * \brief 失能对应从地址 ,根据从机地址可以确定该关闭哪一个从机设备
     *
     * \note ZLG只能生成一个从设备 ，直接关闭失能I2C即可
     */
    p_dev->p_i2c_slv_dev[0] = NULL;
    am_int_disable(p_dev->p_devinfo->inum);
    amhw_zsn700_i2c_disable (p_hw_i2c_slv);

    return AM_OK;
}

/**
 * \brief 获取可用从机设备个数
 */
static int __i2c_slv_num_get (void *p_drv)
{
    int     i;
    int     count = 0;
    uint8_t zlg_i2c_slv_dev_num ;
    am_zlg_i2c_slv_dev_t *p_dev = (am_zlg_i2c_slv_dev_t *)p_drv;

    if ((p_dev == NULL)) {
        return -AM_EINVAL;
    }
    /* 获取总的从设备个数 */
    zlg_i2c_slv_dev_num = p_dev->zlg_i2c_slv_dev_num;

    for (i = 0; i < zlg_i2c_slv_dev_num; i++) {
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
    am_zlg_i2c_slv_dev_t  *p_dev     = ( am_zlg_i2c_slv_dev_t *) p_arg;
    amhw_zsn700_i2c_t     *p_hw_i2c  = NULL;
    p_hw_i2c  = (amhw_zsn700_i2c_t *) (p_dev->p_devinfo->i2c_regbase);

    /* 平台初始化之后将 寄存器的值复位 */
    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    /* 关闭I2C控制器，配置参数 */
    amhw_zsn700_i2c_disable (p_hw_i2c);

    /** 判断是否 需要响应 广播地址 */
    if ( p_dev->p_i2c_slv_dev[0]->dev_flags & AM_I2C_SLV_GEN_CALL_ACK) {
        amhw_zsn700_i2c_gen_call_ack(p_hw_i2c);
    } else {
        amhw_zsn700_i2c_gen_call_nack(p_hw_i2c);
    }

    amhw_zsn700_i2c_slave_addr_set(p_hw_i2c, p_dev->p_i2c_slv_dev[0]->dev_addr);

    amhw_zsn700_i2c_enable(p_hw_i2c);

    __i2c_slv_hard_init(p_dev);

    /** 停止定时 */
    am_softimer_stop(&p_dev->softimer);
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
    static uint8_t           rx_len = 0;
    uint8_t                  state;
    uint8_t                  rx_data;
    uint8_t                  tx_data;
    amhw_zsn700_i2c_t       *p_hw_i2c_slv;
    am_zlg_i2c_slv_dev_t    *p_dev          = (am_zlg_i2c_slv_dev_t *)p_arg;
    am_i2c_slv_device_t     *p_i2c_slv_dev  = p_dev->p_i2c_slv_dev[0];

    p_hw_i2c_slv = (amhw_zsn700_i2c_t *)p_dev->p_devinfo->i2c_regbase;
    state = amhw_zsn700_i2c_status_get(p_hw_i2c_slv);

    switch(state) {
    case 0xa0:  /* 接收到停止条件或重复起始条件 */
        if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_tran_stop) {
            p_i2c_slv_dev->p_cb_funs->pfn_tran_stop(p_i2c_slv_dev->p_arg); /* 主机read开始 */
        }
        rx_len = 0;
        amhw_zsn700_i2c_cr_set(p_hw_i2c_slv, AMHW_ZSN700_I2C_REPLY_ENABLE);
        break;

    case 0x60:  /* 已接收到(与自身匹配的)SLA+W；已接收ACK */
    case 0x70:  /* 已接收通用调用地址（0x00）；已接收ACK */
        rx_len = 0;
        amhw_zsn700_i2c_cr_set(p_hw_i2c_slv, AMHW_ZSN700_I2C_REPLY_ENABLE);
        break;

    case 0x68:  /* 主控时在SLA+读写丢失仲裁；已接收自身的SLA+W；已返回ACK */
    case 0x78:  /* 主控时在SLA+读写中丢失仲裁；已接收通用调用地址；已返回ACK */
    case 0x88:  /* 前一次寻址使用自身从地址；已接收数据字节；已返回非ACK */
        amhw_zsn700_i2c_cr_set(p_hw_i2c_slv, AMHW_ZSN700_I2C_REPLY_ENABLE);
        break;

    case 0x80:  /* 前一次寻址使用自身从地址；已接收数据字节；已返回ACK */
    case 0x98:  /* 前一次寻址使用通用调用地址；已接收数据；已返回非ACK */
    case 0x90:  /* 前一次寻址使用通用调用地址；已接收数据；已返回ACK */
        amhw_zsn700_i2c_cr_set(p_hw_i2c_slv, AMHW_ZSN700_I2C_REPLY_ENABLE);
        rx_data = amhw_zsn700_i2c_dat_read(p_hw_i2c_slv);//接收数据
        if(rx_len) {    /* 忽略接收的第一个数据(从机设备子地址) */
            /* 接受回调 */
            if (p_dev->is_gen_call == AM_TRUE) {
                /** 如果是广播呼叫 */
                if(p_i2c_slv_dev->p_cb_funs->pfn_gen_call) {
                    p_i2c_slv_dev->p_cb_funs->pfn_gen_call(p_i2c_slv_dev->p_arg, rx_data);
                }
            } else {
                if(p_i2c_slv_dev->p_cb_funs->pfn_rxbyte_put) {
                    p_i2c_slv_dev->p_cb_funs->pfn_rxbyte_put(p_i2c_slv_dev->p_arg, rx_data);
                }
            }
        }
        rx_len++;
        break;

    case 0xa8:  /* 已接收自身的SLA+R；已返回ACK,将要发出数据并将收到ACK位 */
    case 0xb0:  /* 当主控时在SLA+读写中丢失仲裁；已接收自身SLA+R；已返回ACK */
    case 0xc8:  /* 装入的数据字节已被发送；已接收ACK */
    case 0xb8:  /* 已发送数据；已接收ACK */
        if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_txbyte_get) {
            p_i2c_slv_dev->p_cb_funs->pfn_txbyte_get(p_i2c_slv_dev->p_arg, &tx_data);
        }
        amhw_zsn700_i2c_dat_write(p_hw_i2c_slv,tx_data);
        break;

    case 0xc0:  /* 发送数据，接收非ACKn */
        if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_tran_stop) {
            p_i2c_slv_dev->p_cb_funs->pfn_tran_stop(p_i2c_slv_dev->p_arg); /* 主机read完 */
        }
        break;
    }

    /* 清除中断标志 */
    amhw_zsn700_i2c_cr_clear(p_hw_i2c_slv, AMHW_ZSN700_I2C_INT_FLAG);
}

/**
 * \brief I2C初始化
 *
 * \note 硬件初始化 通过用户调用开始从设备来初始化
 */
am_i2c_slv_handle_t am_zlg_i2c_slv_init (
        am_zlg_i2c_slv_dev_t           *p_dev,
        const am_zlg_i2c_slv_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL ) {
        return NULL;
    }

    p_dev->i2c_slv_serv.p_funcs = \
            (struct am_i2c_slv_drv_funcs *)&__g_i2c_slv_drv_funcs;
    p_dev->i2c_slv_serv.p_drv   = p_dev;
    p_dev->p_devinfo            = p_devinfo;
    p_dev->zlg_i2c_slv_dev_num  = 1;  /* 只能生成一个从机设备 */
    p_dev->is_gen_call          = AM_FALSE;
    p_dev->p_i2c_slv_dev[0]     = NULL;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* 初始化软件定时器 */
    am_softimer_init(&p_dev->softimer, __i2c_slv_timing_callback, p_dev);

    __i2c_slv_hard_init(p_dev);

    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum,
                   __i2c_slv_irq_handler,
                   (void *)p_dev);

    return &(p_dev->i2c_slv_serv);
}

/**
 * \brief I2C解初始化
 */
void am_zlg_i2c_slv_deinit (am_i2c_slv_handle_t handle)
{
    amhw_zsn700_i2c_t    *p_hw_i2c_slv = NULL;
    am_zlg_i2c_slv_dev_t *p_dev        = NULL;

    if (NULL == handle) {
        return ;
    }

    p_dev        = (am_zlg_i2c_slv_dev_t *)handle;
    p_hw_i2c_slv = (amhw_zsn700_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    amhw_zsn700_i2c_disable (p_hw_i2c_slv);

    /* 去除中断连接 */
    am_int_disable(p_dev->p_devinfo->inum);

    am_int_disconnect(p_dev->p_devinfo->inum,
                      __i2c_slv_irq_handler,
                      (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
