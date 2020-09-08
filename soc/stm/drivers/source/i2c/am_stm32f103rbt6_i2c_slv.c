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
 * - 1.00 19-07-26  zc, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_stm32f103rbt6_i2c_slv.h"
#include "hw/amhw_stm32f103rbt6_i2c.h"
#include "am_stm32f103rbt6_clk.h"

/*******************************************************************************
  函数声明
*******************************************************************************/

/** \brief I2C硬件初始化 */
static int __i2c_slv_hard_init(am_stm32f103rbt6_i2c_slv_dev_t *p_dev);

/** \brief I2C中断处理函数 */
static void __stm32f103rbt6_i2c_slv_irq_handler (void *p_arg);

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
static int __i2c_slv_hard_init (am_stm32f103rbt6_i2c_slv_dev_t *p_dev)
{
    amhw_stm32f103rbt6_i2c_t *p_hw_i2c = NULL;
    uint16_t tmpreg = 0, i2c_freq_value = 0;
    uint32_t pclk1 = 0;
    uint16_t result = 0x04;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_stm32f103rbt6_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    /** <breif 配置 CR2 */
    tmpreg = p_hw_i2c->i2c_cr2;

    /** <brief 清零 freq[5:0] */
    tmpreg &= 0xFFC0;

    /** <brief 根据 pclk1  设置freq时钟 */
    pclk1 = am_clk_rate_get(p_dev->p_devinfo->clk_num);

    i2c_freq_value = (uint16_t)(pclk1/1000000);
    tmpreg |= i2c_freq_value;

    /** <brief 写入CR2 */
    p_hw_i2c->i2c_cr2 = tmpreg;

    /** <brief 配置 CCR */

    /**
     *  \brief 禁能 I2C  配置 TRISE 清零 F/S DUTY CCR[11:0]
     */
    amhw_stm32f103rbt6_i2c_disable(p_hw_i2c);
    tmpreg = 0;

    /** <brief 最高为 100khz */
    if (p_dev->i2c_clock_speed <= 100000) {
        /** <brief 标准模式速度计算 */
    result = (uint16_t)(pclk1 / (p_dev->i2c_clock_speed << 1));

        if (result < 0x04) {

            /** <brief 设置最小允许值 */
            result = 0x04;
        }
        tmpreg |= result;
        /** \brief 设置标准模式下最大的 rise值     */
        p_hw_i2c->i2c_trise = i2c_freq_value + 1;
        } else {

            /** <brief 最高为 400khz */
            if (p_dev->i2c_dutycycle == AMHW_STM32F103RBT6_I2C_DUTY_CYCLE_2 ) {
                result = (uint16_t)(pclk1 / (p_dev->i2c_clock_speed *3));
            } else {
                result = (uint16_t)(pclk1 / (p_dev->i2c_clock_speed * 25));
                result |= AMHW_STM32F103RBT6_I2C_DUTY_CYCLE_16_9;
            }

            /** < 测试 CCR[11:0] 是否小于 0x01 */
            if ((result & 0x0FFF) == 0) {

                /** <brief 设置最小的 CCR值 */
                result |= (uint16_t)0x0001;
            }

            /** <brief 设置speed值  和  F/S为 快速模式 */
            tmpreg |= (uint16_t)(result | (uint16_t)0x8000);
            /** <brief 设置快速模式的最大rise时间 */
            p_hw_i2c->i2c_trise = (uint16_t) (((i2c_freq_value *(uint16_t)300) /
                                              (uint16_t)1000) + (uint16_t)1) ;
        }

        /** <brief 写入 ccr */
        p_hw_i2c->i2c_ccr = tmpreg;

        /** <brief 启动I2C外设 */
        amhw_stm32f103rbt6_i2c_enable(p_hw_i2c);

        /** <brief 配置 CR1 */

        tmpreg = p_hw_i2c->i2c_cr1;

        /** \brief 清零 ACK,SMBTYPE,SMBUS */
        tmpreg &= 0xFBF5;

        /**
         *  <brief 配置 I2C 模式 和 acknowledgement
         *  根据I2C的模式值 设置 SMBTYPE 和 SMBUS
         *  根据I2C的ACK值 设置ACK位
         */
        tmpreg |= (uint16_t)((uint16_t)p_dev->i2c_mode | p_dev->i2c_ack);

        /** \brief 写入CR1 */
        p_hw_i2c->i2c_cr1 = tmpreg;

        /** \brief I2C OAR1 配置 */
        p_hw_i2c->i2c_oar1 = (p_dev->i2c_acknowledgedaddress |
                              ((p_dev->i2c_ownaddress1) << 1));

        /* 默认不使用 10bit地址*/
        amhw_stm32f103rbt6_i2c_endual(p_hw_i2c,DISABLE);
        amhw_stm32f103rbt6_i2c_iten_mode_set(p_hw_i2c,
                                      I2C_CR2_ITEVTEN |
                                      I2C_CR2_ITBUFEN |
                                      I2C_CR2_ITERREN,
                                      ENABLE);

        /** 启动 I2C外设  */
        amhw_stm32f103rbt6_i2c_enable(p_hw_i2c);

        return AM_OK;
}

/**
 * \brief 开始从机设备
 */
static int __i2c_slv_setup (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev)
{
    int     i;
    uint8_t stm32f103rbt6_i2c_slv_dev_num ;
    am_stm32f103rbt6_i2c_slv_dev_t *p_dev      = NULL;
    amhw_stm32f103rbt6_i2c_t       *p_hw_i2c   = NULL;

    if ((p_drv         == NULL) ||
        (p_i2c_slv_dev == NULL)) {
        return -AM_EINVAL;
    }

    p_dev    = (am_stm32f103rbt6_i2c_slv_dev_t *)p_drv;
    p_hw_i2c = (amhw_stm32f103rbt6_i2c_t *) (p_dev->p_devinfo->i2c_regbase);

    /* 获取总的从设备个数 */
    stm32f103rbt6_i2c_slv_dev_num            = p_dev->stm32f103rbt6_i2c_slv_dev_num;
    p_dev->i2c_ack                 = AMHW_STM32F103RBT6_I2C_ACK_ENABLE;
    p_dev->i2c_acknowledgedaddress = AMHW_STM32F103RBT6_I2C_ACK_ADDRESS_7_BIT;
    p_dev->i2c_clock_speed         = 100000;
    p_dev->i2c_dutycycle           = AMHW_STM32F103RBT6_I2C_DUTY_CYCLE_2;
    p_dev->i2c_mode                = AMHW_STM32F103RBT6_I2C_MODE_I2C;

    for ( i = 0; i < stm32f103rbt6_i2c_slv_dev_num; i++) {
        if( NULL == p_dev->p_i2c_slv_dev[i]) {
            p_dev->p_i2c_slv_dev[i] = p_i2c_slv_dev;
            break ;
        }
    }

    /* 从机设备都用关了，无法开启该从机设备 */
    if ( i == stm32f103rbt6_i2c_slv_dev_num) {
        return -AM_EAGAIN;
    }

//    /* 关闭I2C控制器，配置参数 */
//    amhw_stm32f103rbt6_i2c_disable (p_hw_i2c);

    /* 判断是否 需要响应 广播地址 */
    if ( p_i2c_slv_dev->dev_flags & AM_I2C_SLV_GEN_CALL_ACK) {
        amhw_stm32f103rbt6_i2c_engc(p_hw_i2c, ENABLE);
    } else {
        amhw_stm32f103rbt6_i2c_engc(p_hw_i2c, DISABLE);
    }

    /**
     * \brief 根据  i 存放从机设备地址。
     *         i = 0 ,存放到第1个从地址
     *         i = 1 ,存放到第2从地址 ，以此类推
     *
     * \note STM32F103RBT6只有一个从机地址 ,直接存放即可 。
     */
    p_hw_i2c->i2c_oar1 = (uint16_t)(p_i2c_slv_dev->dev_addr << 1);

    /* 设置从机设备的地址位数 */
    if ( p_i2c_slv_dev->dev_flags  &  AM_I2C_SLV_ADDR_10BIT) {
        p_hw_i2c->i2c_oar1 |= 0x8000;
    } else {
        p_hw_i2c->i2c_oar1 &= 0x0fff;
    }

//    amhw_stm32f103rbt6_i2c_enable(p_hw_i2c);
    return AM_OK;
}

/**
 * \brief 关闭 从机设备
 */
static int __i2c_slv_shutdown (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev)
{
    am_stm32f103rbt6_i2c_slv_dev_t *p_dev    = (am_stm32f103rbt6_i2c_slv_dev_t *)p_drv;
    amhw_stm32f103rbt6_i2c_t    *p_hw_i2c_slv = NULL;

    if ( (p_dev              == NULL) ||
         (p_i2c_slv_dev      == NULL)) {
        return -AM_EINVAL;
    }
    p_hw_i2c_slv = (amhw_stm32f103rbt6_i2c_t *) p_dev->p_devinfo->i2c_regbase;

    /**
     * \brief 失能对应从地址 ,根据从机地址可以确定该关闭哪一个从机设备
     *
     * \note STM32F103RBT6只能生成一个从设备 ，直接关闭失能I2C即可
     */
    p_dev->p_i2c_slv_dev[0] = NULL;
    amhw_stm32f103rbt6_i2c_disable(p_hw_i2c_slv);

    return AM_OK;
}

/**
 * \brief 获取可用从机设备个数
 */
static int __i2c_slv_num_get (void *p_drv)
{
    int     i;
    int     count = 0;
    uint8_t stm32f103rbt6_i2c_slv_dev_num ;
    am_stm32f103rbt6_i2c_slv_dev_t *p_dev = (am_stm32f103rbt6_i2c_slv_dev_t *)p_drv;

    if ((p_dev == NULL)) {
        return -AM_EINVAL;
    }
    /* 获取总的从设备个数 */
    stm32f103rbt6_i2c_slv_dev_num = p_dev->stm32f103rbt6_i2c_slv_dev_num;

    for (i = 0; i < stm32f103rbt6_i2c_slv_dev_num; i++) {
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
    am_stm32f103rbt6_i2c_slv_dev_t *p_dev = ( am_stm32f103rbt6_i2c_slv_dev_t *) p_arg;

#if 1

    amhw_stm32f103rbt6_i2c_t     *p_hw_i2c  = NULL;
    p_hw_i2c  = (amhw_stm32f103rbt6_i2c_t *) (p_dev->p_devinfo->i2c_regbase);

    /* 平台初始化之后将 寄存器的值复位 */
    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    __i2c_slv_hard_init(p_dev);

    /** 判断是否 需要响应 广播地址 */
    if ( p_dev->p_i2c_slv_dev[0]->dev_flags & AM_I2C_SLV_GEN_CALL_ACK) {

        amhw_stm32f103rbt6_i2c_engc(p_hw_i2c, ENABLE);
    } else {
        amhw_stm32f103rbt6_i2c_engc(p_hw_i2c, DISABLE);
    }

    /* 设置从机地址 */
    p_hw_i2c->i2c_oar1 = (uint16_t)(p_dev->p_i2c_slv_dev[0]->dev_addr << 1);


    /* 设置从机设备的地址位数 */
    if ( p_dev->p_i2c_slv_dev[0]->dev_flags  &  AM_I2C_SLV_ADDR_10BIT) {
        p_hw_i2c->i2c_oar1 |= 0x8000;
    } else {
        p_hw_i2c->i2c_oar1 &= 0x0fff;
    }

#endif

    /** 停止定时 */
    am_softimer_stop(&p_dev->softimer);
}

/**
 * \brief 从机接受数据
 */
void __stm32f103rbt6_i2c_slv_irq_rx_handler (am_stm32f103rbt6_i2c_slv_dev_t *p_dev)
{
    amhw_stm32f103rbt6_i2c_t   *p_hw_i2c  = (amhw_stm32f103rbt6_i2c_t *)
                                      p_dev->p_devinfo->i2c_regbase;
    am_i2c_slv_device_t *p_i2c_slv =  p_dev->p_i2c_slv_dev[0];

    /* 接受数据 */
    uint8_t rx_data = amhw_stm32f103rbt6_i2c_recv_data (p_hw_i2c);

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
void __stm32f103rbt6_i2c_slv_irq_tx_handler (am_stm32f103rbt6_i2c_slv_dev_t *p_dev)
{
    amhw_stm32f103rbt6_i2c_t   *p_hw_i2c  = (amhw_stm32f103rbt6_i2c_t *)
                                     p_dev->p_devinfo->i2c_regbase;
    am_i2c_slv_device_t *p_i2c_slv = p_dev->p_i2c_slv_dev[0];

    uint8_t  tx_data = 0;

    /**
     * \brief 通过发送回调来获取数据 ，
     *        此FIFO有两个字节 + 移位寄存器放一个字节 ，因此可以连续取三个字节
     */
    if (NULL != p_i2c_slv->p_cb_funs->pfn_txbyte_get) {
         p_i2c_slv->p_cb_funs->pfn_txbyte_get( p_i2c_slv->p_arg, &tx_data);
    }
    amhw_stm32f103rbt6_i2c_send_data(p_hw_i2c, tx_data);

}

/**
 * \brief I2C 中断处理函数
 *
 * \param[in] p_arg : 指向I2C设备结构体的指针
 *
 * \return 无
 */
static void __stm32f103rbt6_i2c_slv_irq_handler (void *p_arg)
{
        
    am_stm32f103rbt6_i2c_slv_dev_t *p_dev              = (am_stm32f103rbt6_i2c_slv_dev_t *)
                                                   p_arg;
    amhw_stm32f103rbt6_i2c_t       *p_hw_i2c_slv       = (amhw_stm32f103rbt6_i2c_t *)
                                                  p_dev->p_devinfo->i2c_regbase;
    am_i2c_slv_device_t     *p_i2c_slv_dev      = p_dev->p_i2c_slv_dev[0];
    uint16_t                 i2c_slv_int_status = amhw_stm32f103rbt6_i2c_getitstatus(
                                                      p_hw_i2c_slv);


    /* EV1 事件: 收到从地址*/
    if (i2c_slv_int_status & (uint16_t)AMHW_STM32F103RBT6_INT_FLAG__ADDR) {

        /* 读sr2 清除中断标志位 */
        amhw_stm32f103rbt6_i2c_read_reg(p_hw_i2c_slv->i2c_sr2);

        if( NULL != p_i2c_slv_dev->p_cb_funs->pfn_addr_match) {
            p_i2c_slv_dev->p_cb_funs->pfn_addr_match(p_i2c_slv_dev->p_arg,
                                                         AM_TRUE);
        }

        /* 收到广播呼叫从地址  */
        if (amhw_stm32f103rbt6_i2c_checkflagstaus(p_hw_i2c_slv,
                AMHW_STM32F103RBT6_I2C_FLAG_GENCALL)) {
            p_dev->is_gen_call = AM_TRUE;
        }
        /* 开始用软件定时器计时 1000ms */
        am_softimer_start(&p_dev->softimer, 1000);
    }


    if (i2c_slv_int_status & (uint16_t)AMHW_STM32F103RBT6_INT_FLAG__BTF) {
        if (i2c_slv_int_status & (uint16_t)AMHW_STM32F103RBT6_INT_FLAG__RXNE) {

            /* 开始接收数据 */
            __stm32f103rbt6_i2c_slv_irq_rx_handler (p_dev);
        } else if (i2c_slv_int_status & (uint16_t)AMHW_STM32F103RBT6_INT_FLAG__TXE) {

            /* 开始发送数据 */
            __stm32f103rbt6_i2c_slv_irq_tx_handler (p_dev);
        }
    }

    /**
     * \brief 接收缓存非空  EV2
     */
    if (i2c_slv_int_status & (uint16_t)AMHW_STM32F103RBT6_INT_FLAG__RXNE) {

        /* 开始接收数据 */
        __stm32f103rbt6_i2c_slv_irq_rx_handler (p_dev);
    }

    /**
     * \brief 接收缓存非空   EV3 EV3_1
     */
    if (i2c_slv_int_status & (uint16_t)AMHW_STM32F103RBT6_INT_FLAG__TXE) {

        /* 开始发送数据 */
        __stm32f103rbt6_i2c_slv_irq_tx_handler (p_dev);
    }

    /* 传输数据结束时 */
    if (i2c_slv_int_status & (uint16_t)AMHW_STM32F103RBT6_INT_FLAG__STOPF) {
        if (i2c_slv_int_status & (uint16_t)AMHW_STM32F103RBT6_INT_FLAG__AF) {
            /* EV3_2 从发送器清除 AF */
            amhw_stm32f103rbt6_i2c_clearflag(p_hw_i2c_slv, AMHW_STM32F103RBT6_I2C_FLAG_AF);
        } else {

            /* EV4  */
            amhw_stm32f103rbt6_i2c_read_reg(p_hw_i2c_slv->i2c_sr1);
            p_hw_i2c_slv->i2c_cr1 &=0xffff;
        }

        p_dev->is_gen_call = AM_FALSE;

        /* 执行传输停止回调 */
        if ( NULL != p_i2c_slv_dev->p_cb_funs->pfn_tran_stop) {
            p_i2c_slv_dev->p_cb_funs->pfn_tran_stop(p_i2c_slv_dev->p_arg);
        }

        /* 传输结束停止计时 */
        am_softimer_stop(&p_dev->softimer);
    }

}

/**
 * \brief I2C初始化
 *
 * \note 硬件初始化 通过用户调用开始从设备来初始化
 */
am_i2c_slv_handle_t am_stm32f103rbt6_i2c_slv_init (am_stm32f103rbt6_i2c_slv_dev_t     *p_dev,
                                      const am_stm32f103rbt6_i2c_slv_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL ) {
        return NULL;
    }

    p_dev->i2c_slv_serv.p_funcs = (struct am_i2c_slv_drv_funcs *)
                                       &__g_i2c_slv_drv_funcs;
    p_dev->i2c_slv_serv.p_drv   = p_dev;

    p_dev->p_devinfo   = p_devinfo;
    p_dev->stm32f103rbt6_i2c_slv_dev_num =1;  /* 只能生成一个从机设备 */



    /* p_dev->current_matching_num =0; 默认当前匹配为 0 */

    p_dev->is_gen_call = AM_FALSE;

    p_dev->p_i2c_slv_dev[0] = NULL;

    p_dev->i2c_clock_speed         = 100000;
    p_dev->i2c_mode                = AMHW_STM32F103RBT6_I2C_MODE_I2C;
    p_dev->i2c_dutycycle           = AMHW_STM32F103RBT6_I2C_DUTY_CYCLE_2;
    p_dev->i2c_ownaddress1         = 0;
    p_dev->i2c_ack                 = AMHW_STM32F103RBT6_I2C_ACK_ENABLE;
    p_dev->i2c_acknowledgedaddress = AMHW_STM32F103RBT6_I2C_ACK_ADDRESS_7_BIT;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* 初始化软件定时器 */
    am_softimer_init(&p_dev->softimer, __i2c_slv_timing_callback, p_dev);

    __i2c_slv_hard_init(p_dev);

    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum, __stm32f103rbt6_i2c_slv_irq_handler,
                       (void *)p_dev);

    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->i2c_slv_serv);
}

/**
 * \brief I2C解初始化
 */
void am_stm32f103rbt6_i2c_slv_deinit (am_i2c_slv_handle_t handle)
{
    amhw_stm32f103rbt6_i2c_t       *p_hw_i2c_slv = NULL;
    am_stm32f103rbt6_i2c_slv_dev_t *p_dev        = NULL;

    if (NULL == handle) {
        return ;
    }

    p_dev    = (am_stm32f103rbt6_i2c_slv_dev_t *)handle;
    p_hw_i2c_slv = (amhw_stm32f103rbt6_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    amhw_stm32f103rbt6_i2c_disable (p_hw_i2c_slv);

    /* 去除中断连接 */
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __stm32f103rbt6_i2c_slv_irq_handler,
                         (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
