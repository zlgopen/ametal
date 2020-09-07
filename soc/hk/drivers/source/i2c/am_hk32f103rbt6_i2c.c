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
 * \brief hk32f103rbt6 I2C驱动接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-07-24  zc, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件
*******************************************************************************/
#include "ametal.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_hk32f103rbt6_i2c.h"
#include "hw/amhw_hk32f103rbt6_i2c.h"
#include "hk32f103rbt6_regbase.h"
#include "hk32f103rbt6_clk.h"
#include "hk32f103rbt6_pin.h"
#include "amhw_hk32f103rbt6_gpio.h"
/*******************************************************************************
  宏定义
*******************************************************************************/
#define __I2C_ST_IDLE             (0x10u)          /* 空闲状态 */
#define __I2C_ST_MSG_START        (0x11u)          /* 消息传输开始状态 */
#define __I2C_ST_TRANS_START      (0x12u)          /* 单个传输开始状态 */
#define __I2C_ST_SEND_SLA_ADDR    (0x13u)          /* 发送从机地址状态 */
#define __I2C_ST_M_SEND_DATA      (0x14u)          /* 发送数据状态 */
#define __I2C_ST_M_RECV_DATA      (0x15u)          /* 接收数据状态 */

#define __I2C_EVT_NONE            (0xFFu)          /* 无事件 */
#define __I2C_EVT_MSG_LAUNCH      (0xFEu)          /* 开始处理一个新的消息 */
#define __I2C_EVT_TRANS_LAUNCH    (0xFDu)          /* 开始处理一个新的传输 */

#define __I2C_EVT_MST_IDLE        AM_SBF(0x0, 1)  /* 主机空闲事件 */
#define __I2C_EVT_MST_START       AM_SBF(0X3, 1)  /* 传输就绪 */
#define __I2C_EVT_MST_ADDR        AM_SBF(0X4, 1)  /* 传输就绪 */
#define __I2C_EVT_MST_RX          AM_SBF(0x1, 1)  /* 可以接收数据 */
#define __I2C_EVT_MST_TX          AM_SBF(0x2, 1)  /* 可以发送数据 */

/* check if transfers empty */
#define __I2C_TRANS_EMPTY(p_dev) \
    ((p_dev)->p_cur_trans >= (p_dev)->p_cur_msg->p_transfers \
                             + (p_dev)->p_cur_msg->trans_num)

/*******************************************************************************
  函数声明
*******************************************************************************/

/** \brief I2C 硬件初始化 */
static int __i2c_hard_init (am_hk32f103rbt6_i2c_dev_t *p_dev);

/** \brief I2C 中断处理函数 */
static void __i2c_irq_handler (void *p_arg);

/** \brief I2C 状态机函数 */
static int __i2c_mst_sm_event (am_hk32f103rbt6_i2c_dev_t *p_dev, uint32_t event);

/** \brief I2C 消息处理函数 */
static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_trans);

/** \brief 从控制器传输列表表头取出一条 message */
am_static_inline
struct am_i2c_message *__i2c_msg_out (am_hk32f103rbt6_i2c_dev_t *p_dev);

/*******************************************************************************
  全局变量
*******************************************************************************/

/**
 * \brief I2C 驱动函数定义
 */
static am_const struct am_i2c_drv_funcs __g_i2c_drv_funcs = {
    __i2c_msg_start
};

/******************************************************************************/

/** \brief I2C 硬件初始化 */
am_local int __i2c_hard_init (am_hk32f103rbt6_i2c_dev_t *p_dev)
{
    amhw_hk32f103rbt6_i2c_t *p_hw_i2c = NULL;
    uint16_t tmpreg = 0, i2c_freq_value = 0;
    uint32_t pclk1 = 0;
    uint16_t result = 0x04;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_hk32f103rbt6_i2c_t *)p_dev->p_devinfo->i2c_regbase;

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
    amhw_hk32f103rbt6_i2c_disable(p_hw_i2c);
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
        if (p_dev->i2c_dutycycle == AMHW_HK32F103RBT6_I2C_DUTY_CYCLE_2 ) {
            result = (uint16_t)(pclk1 / (p_dev->i2c_clock_speed *3));
        } else {
            result = (uint16_t)(pclk1 / (p_dev->i2c_clock_speed * 25));
            result |= AMHW_HK32F103RBT6_I2C_DUTY_CYCLE_16_9;
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
    amhw_hk32f103rbt6_i2c_enable(p_hw_i2c);

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

    /** \breif 写入CR1 */
    p_hw_i2c->i2c_cr1 = tmpreg;

    /** \breif I2C OAR1 配置 */
    p_hw_i2c->i2c_oar1 = (p_dev->i2c_acknowledgedaddress |
                          (p_dev->i2c_ownaddress1 <<1 ) );

    /* 默认不使用 10bit地址*/
    amhw_hk32f103rbt6_i2c_endual(p_hw_i2c,DISABLE);
    amhw_hk32f103rbt6_i2c_iten_mode_set(p_hw_i2c,
                                  I2C_CR2_ITERREN |
                                  I2C_CR2_ITEVTEN |
                                  I2C_CR2_ITBUFEN,
                                  DISABLE);

    /** 启动 I2C外设  */
    amhw_hk32f103rbt6_i2c_enable(p_hw_i2c);

    return AM_OK;
}

am_local int __i2c_hard_re_init (am_hk32f103rbt6_i2c_dev_t *p_dev)
/** \brief I2C 硬件重新初始化 */
{
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }
    return __i2c_hard_init(p_dev);
}

/**
 * \brief 重新初始化 I2C
 *
 * \param[in] p_arg 指向I2C设备结构体的指针
 *
 * \return 无
 */
am_local void __i2c_re_init (void *p_arg)
{
    am_hk32f103rbt6_i2c_dev_t *p_dev = (am_hk32f103rbt6_i2c_dev_t *)p_arg;
    uint32_t          key   = 0;

    if (p_arg == NULL) {
        return;
    }

    key = am_int_cpu_lock();

    p_dev->is_abort = AM_TRUE;

    do {

        /* 回调消息完成函数 */
        if ((p_dev->p_cur_msg != NULL) &&
            (p_dev->p_cur_msg->pfn_complete != NULL)) {
            p_dev->p_cur_msg->status = -AM_ETIMEDOUT;
            p_dev->p_cur_msg->pfn_complete(p_dev->p_cur_msg->p_arg);
        }

        p_dev->p_cur_msg = __i2c_msg_out(p_dev);
        if (NULL == p_dev->p_cur_msg) {
            break;
        }
    } while (1);

    p_dev->p_cur_trans = NULL;
    p_dev->p_cur_msg   = NULL;
    p_dev->is_complete = AM_FALSE;
    p_dev->data_ptr    = 0;
    p_dev->trans_state = 0;
    p_dev->busy        = AM_FALSE;
    p_dev->state       = __I2C_ST_IDLE; /* 初始化为空闲状态 */
    p_dev->slave_addr  = 0;

    p_dev->i2c_clock_speed         = p_dev->p_devinfo->speed;
    p_dev->i2c_mode                = AMHW_HK32F103RBT6_I2C_MODE_I2C;
    p_dev->i2c_dutycycle           = AMHW_HK32F103RBT6_I2C_DUTY_CYCLE_2;
    p_dev->i2c_ownaddress1         = 0x01;
    p_dev->i2c_ack                 = AMHW_HK32F103RBT6_I2C_ACK_ENABLE;
    p_dev->i2c_acknowledgedaddress = AMHW_HK32F103RBT6_I2C_ACK_ADDRESS_7_BIT;

    am_list_head_init(&(p_dev->msg_list));

    __i2c_hard_re_init(p_dev);

    if (p_dev->p_devinfo->pfn_bus_clean) {
        p_dev->p_devinfo->pfn_bus_clean();
    }
    am_int_cpu_unlock(key);
}

/**
 * \brief 使能软件定时器
 *
 * \param[in] p_arg 指向I2C设备结构体的指针
 *
 * \return 无
 */
am_local void __softimer_start (am_hk32f103rbt6_i2c_dev_t *p_dev)
{
    p_dev->is_abort = AM_FALSE;

    if (p_dev->p_devinfo->timeout_ms == 0) {
        return;
    }

    if (p_dev->softimer_state == AM_FALSE) {
        p_dev->softimer_state = AM_TRUE;
        am_softimer_start(&p_dev->softimer,
                           p_dev->p_devinfo->timeout_ms);
    } else {
        am_softimer_stop(&p_dev->softimer);
        am_softimer_start(&p_dev->softimer,
                           p_dev->p_devinfo->timeout_ms);
    }
}

/**
 * \brief 停止软件定时器
 *
 * \param[in] p_arg 指向I2C设备结构体的指针
 *
 * \return 无
 */
am_local void __softimer_stop (am_hk32f103rbt6_i2c_dev_t *p_dev)
{
    if (p_dev->p_devinfo->timeout_ms == 0) {
        return;
    }

    am_softimer_stop(&p_dev->softimer);

    p_dev->softimer_state = AM_FALSE;
}

/******************************************************************************/

/**
 * \brief 添加一条 message 到控制器传输列表末尾
 *
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
void __i2c_msg_in (am_hk32f103rbt6_i2c_dev_t *p_dev, struct am_i2c_message *p_msg)
{
    am_list_add_tail((struct am_list_head *)(&p_msg->ctlrdata),
                    &(p_dev->msg_list));
}

/**
 * \brief 从控制器传输列表表头取出一条 message
 *
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
struct am_i2c_message *__i2c_msg_out (am_hk32f103rbt6_i2c_dev_t *p_dev)
{
    if (am_list_empty_careful(&(p_dev->msg_list))) {
        return NULL;
    } else {
        struct am_list_head *p_node = p_dev->msg_list.next;
        am_list_del(p_node);
        return am_list_entry(p_node, struct am_i2c_message, ctlrdata);
    }
}

/******************************************************************************/

/**
 * \brief 超时软件定时器回调函数
 *
 * \param[in] p_arg 指向I2C设备结构体的指针
 *
 * \return 无
 */
am_local void __softimer_callback (void *p_arg)
{
    am_hk32f103rbt6_i2c_dev_t *p_dev = (am_hk32f103rbt6_i2c_dev_t *)p_arg;

    if (p_arg == NULL) {
        return;
    }

    __softimer_stop(p_dev);
    __i2c_re_init(p_arg);
}

/**
 * \brief I2C 中断处理函数
 *
 * \param[in] p_arg : 指向I2C设备结构体的指针
 *
 * \return 无
 */
static void __i2c_irq_handler (void *p_arg)
{
    am_hk32f103rbt6_i2c_dev_t *p_dev          = NULL;
    amhw_hk32f103rbt6_i2c_t   *p_hw_i2c       = NULL;
    uint16_t             i2c_int_status = 0;

    p_dev    = (am_hk32f103rbt6_i2c_dev_t *)p_arg;
    p_hw_i2c = (amhw_hk32f103rbt6_i2c_t *)p_dev->p_devinfo->i2c_regbase;
    struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;
    if (p_arg == NULL) {
        return;
    }

     i2c_int_status = amhw_hk32f103rbt6_i2c_getitstatus(p_hw_i2c);


     /* 等待BTF事件中断 来发送和读取数据 */
     if (i2c_int_status & (uint16_t)AMHW_HK32F103RBT6_INT_FLAG__BTF) {
         if (i2c_int_status & (uint16_t)AMHW_HK32F103RBT6_INT_FLAG__RXNE) {
             __i2c_mst_sm_event(p_dev, __I2C_EVT_MST_RX);
             return ;
         } else if (i2c_int_status & (uint16_t)AMHW_HK32F103RBT6_INT_FLAG__TXE) {
             __i2c_mst_sm_event(p_dev, __I2C_EVT_MST_TX);
             return;
         }
     }
     /* 当只读取一个数据时 RXNE = 1 读取数据 */
     if ( p_cur_trans->flags & AM_I2C_M_RD) {
         if (p_cur_trans->nbytes == 1)
        /* 接收非空，进入接收状态 */
        if (i2c_int_status & (uint16_t)AMHW_HK32F103RBT6_INT_FLAG__RXNE) {

            /** \brief 接收模式  读取 DR寄存器  清除中断标志位 */
            __i2c_mst_sm_event(p_dev, __I2C_EVT_MST_RX);
        }
    }

}

/******************************************************************************/

static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_msg)
{
    am_hk32f103rbt6_i2c_dev_t  *p_dev    = (am_hk32f103rbt6_i2c_dev_t *)p_drv;

    int key;

    /** \brief 判断设备、 消息 、组成消息的传输 、请求处理的传输个数是否为空 */
    if ( (p_dev              == NULL) ||
         (p_msg              == NULL) ||
         (p_msg->p_transfers == NULL) ||
         (p_msg->trans_num   == 0)) {

        return -AM_EINVAL;
    }
    /** \brief 关闭本地CPU中断 */
    key = am_int_cpu_lock();

    /* 当前正在处理消息，只需要将新的消息加入链表即可 */
    if (p_dev->busy == AM_TRUE) {

        __i2c_msg_in(p_dev, p_msg);

        am_int_cpu_unlock(key);

        return AM_OK;

    } else {
        /** \brief 总线空闲 */
        p_dev->busy = AM_TRUE;
        __i2c_msg_in(p_dev, p_msg);
        p_msg->status = -AM_EISCONN; /* 正在排队中 */
        am_int_cpu_unlock(key);

        /** \brief 设备状态  转换为  开始处理一个新的消息 */
        return __i2c_mst_sm_event(p_dev, __I2C_EVT_MSG_LAUNCH);
    }
}

/**
 * \brief I2C初始化
 */
am_i2c_handle_t am_hk32f103rbt6_i2c_init (am_hk32f103rbt6_i2c_dev_t           *p_dev,
                                 const am_hk32f103rbt6_i2c_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->i2c_serv.p_funcs = (struct am_i2c_drv_funcs *)&__g_i2c_drv_funcs;
    p_dev->i2c_serv.p_drv   = p_dev;

    p_dev->p_devinfo = p_devinfo;

    p_dev->p_cur_trans             = NULL;
    p_dev->p_cur_msg               = NULL;
    p_dev->is_complete             = AM_FALSE;
    p_dev->data_ptr                = 0;
    p_dev->trans_state             = 0;
    p_dev->busy                    = AM_FALSE;
    p_dev->state                   = __I2C_ST_IDLE; /* 初始化为空闲状态 */
    p_dev->slave_addr              = 0;

    p_dev->i2c_clock_speed         = p_devinfo->speed;
    p_dev->i2c_mode                = AMHW_HK32F103RBT6_I2C_MODE_I2C;
    p_dev->i2c_dutycycle           = AMHW_HK32F103RBT6_I2C_DUTY_CYCLE_2;
    p_dev->i2c_ownaddress1         = 0x01;
    p_dev->i2c_ack                 = AMHW_HK32F103RBT6_I2C_ACK_ENABLE;
    p_dev->i2c_acknowledgedaddress = AMHW_HK32F103RBT6_I2C_ACK_ADDRESS_7_BIT;


    if (p_devinfo->timeout_ms != 0) {
        p_dev->softimer_state = AM_FALSE;
        if (am_softimer_init(&p_dev->softimer,
                              __softimer_callback,
                              p_dev) != AM_OK) {
            return NULL;
        }
    }

    p_dev->is_abort = AM_FALSE;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    am_list_head_init(&(p_dev->msg_list));

    __i2c_hard_init(p_dev);

//    if (p_dev->p_devinfo->pfn_bus_clean) {
//        p_dev->p_devinfo->pfn_bus_clean();
//    }

    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum, __i2c_irq_handler, (void *)p_dev);
    am_int_connect(p_dev->p_devinfo->inum + 1, __i2c_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    am_int_enable(p_dev->p_devinfo->inum + 1);

    return &(p_dev->i2c_serv);
}

/**
 * \brief I2C解初始化
 */
void am_hk32f103rbt6_i2c_deinit (am_i2c_handle_t handle)
{
    amhw_hk32f103rbt6_i2c_t   *p_hw_i2c = NULL;
    am_hk32f103rbt6_i2c_dev_t *p_dev    = NULL;

    if (NULL == handle) {
        return ;
    }

    p_dev    = (am_hk32f103rbt6_i2c_dev_t *)handle;
    p_hw_i2c = (amhw_hk32f103rbt6_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    p_dev->i2c_serv.p_funcs = NULL;
    p_dev->i2c_serv.p_drv   = NULL;

    amhw_hk32f103rbt6_i2c_disable(p_hw_i2c);

    /* 去除中断连接 */
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum,
                      __i2c_irq_handler,
                      (void *)p_dev);

    __softimer_stop(p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/******************************************************************************/

/*  状态机内部状态切换 */
#define __I2C_NEXT_STATE(s, e) \
    do { \
        p_dev->state = (s); \
        new_event = (e); \
    } while(0)

/*
 * \brief 拉低SCL
 */
am_static_inline
void i2c_pulldown_scl (am_hk32f103rbt6_i2c_dev_t *p_dev) {

    int i2c_num = p_dev->p_devinfo->clk_num;
    amhw_hk32f103rbt6_gpio_reg_t *p_hw_gpio = NULL;
    p_hw_gpio = (amhw_hk32f103rbt6_gpio_reg_t *)HK32F103RBT6_GPIOB_BASE;
    if (i2c_num == CLK_I2C1) {
        p_hw_gpio->brr |= (uint32_t)(1 << 6);
        p_hw_gpio->crl &= ~0x8000000;
    } else if (i2c_num == CLK_I2C2) {
        p_hw_gpio->brr |= (uint32_t)(1 << 10);
        p_hw_gpio->crh &= ~0x800;
    }
}

/*
 * \brief 释放SCL
 */
am_static_inline
void i2c_release_scl (am_hk32f103rbt6_i2c_dev_t *p_dev) {
    int i2c_num = p_dev->p_devinfo->clk_num;
    amhw_hk32f103rbt6_gpio_reg_t *p_hw_gpio = NULL;
    p_hw_gpio = (amhw_hk32f103rbt6_gpio_reg_t *)HK32F103RBT6_GPIOB_BASE;
    if (i2c_num == CLK_I2C1) {
        p_hw_gpio->crl |= 0x8000000;
    } else if (i2c_num == CLK_I2C2) {
        p_hw_gpio->crh |= 0x800;
    }
}
/**
 * \brief I2C 状态机函数
 */
static int __i2c_mst_sm_event (am_hk32f103rbt6_i2c_dev_t *p_dev, uint32_t event)
{
    volatile uint32_t  new_event = __I2C_EVT_NONE;
    amhw_hk32f103rbt6_i2c_t *p_hw_i2c = NULL;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_hk32f103rbt6_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    /* 消除警告 */
    (void)event;

    while (1) {

        /* 检查是否有新的事件在状态机内部发生 */
        if (new_event != __I2C_EVT_NONE) {
            event     = new_event;
            new_event = __I2C_EVT_NONE;
        }

        /* 以设备的状态为基准进行状态的设置 */
        switch (p_dev->state) {

        /*
         * 空闲状态中设置I2C模式为主机模式
         * 空闲状态和开始消息传输状态要处理的事情是一样，事件只应是：
         * __I2C_EVT_TRANS_LAUNCH
         */

        case __I2C_ST_IDLE:
        {
            /* 检 测BUSY位是否等  0*/
            while(amhw_hk32f103rbt6_i2c_checkflagstaus(p_hw_i2c,
                                                 AMHW_HK32F103RBT6_I2C_FLAG_BUSY));
        }
        case __I2C_ST_MSG_START:
        {
            am_i2c_message_t *p_cur_msg = NULL;
            int key;

            /** 从消息队列中取出待处理的消息 */
            key = am_int_cpu_lock();

            p_cur_msg        = __i2c_msg_out(p_dev);
            p_dev->p_cur_msg = p_cur_msg;

            /* 若本次传输设置的从机地址 与 p_dev中的值不匹配   更新从地址*/
            if (p_dev->p_cur_msg->p_transfers->addr != p_dev->slave_addr) {
                p_dev->slave_addr = p_dev->p_cur_msg->p_transfers->addr;
            }

            /* 判断取出消息的完整性 */
            if (p_cur_msg) {

                /* 跟新消息的状态  */
                p_cur_msg->status = -AM_EINPROGRESS;
            } else {

                p_dev->busy = AM_FALSE;
            }
            am_int_cpu_unlock(key);

            /* 无需处理的消息 */
            if (p_cur_msg == NULL) {

                __I2C_NEXT_STATE(__I2C_ST_IDLE, __I2C_EVT_NONE);

                break;
            } else {

                /*第一次处理数据*/
                p_cur_msg->done_num = 0;
                p_dev->p_cur_trans  = p_cur_msg->p_transfers;
                p_dev->data_ptr     = 0;


                __I2C_NEXT_STATE(__I2C_ST_TRANS_START,
                        __I2C_EVT_TRANS_LAUNCH);

                /* 直接进入下一个状态，开始一个传输，此处无需break */
                event     = new_event;
                new_event = __I2C_EVT_NONE;

                __softimer_start(p_dev);
            }
        }
        /* no break */
        case __I2C_ST_TRANS_START:
        {
            /* 事件需与自身状态匹配 */
            if (event != __I2C_EVT_TRANS_LAUNCH) {
                return -AM_EINVAL;
            }

            struct am_i2c_message *p_cur_msg = p_dev->p_cur_msg;

            if (NULL == p_cur_msg) {
                __softimer_stop(p_dev);
                __i2c_re_init(p_dev);

                return -AM_EINVAL;
            }

            /* 当前消息传输完成  */
            if (__I2C_TRANS_EMPTY(p_dev)) {

                /* 消息正在处理中 */
                if (p_cur_msg->status == -AM_EINPROGRESS) {
                    p_cur_msg->status = AM_OK;
                }

                p_dev->is_complete = AM_TRUE;

                /*判断是处于主接收还是发送模式*/
                am_bool_t is_send = amhw_hk32f103rbt6_i2c_checkflagstaus(
                    p_hw_i2c, AMHW_HK32F103RBT6_I2C_FLAG_TRA);

                /* 发送EV8_2 事件时   产生停止条件   主发送*/
                if (is_send == AM_TRUE) {

                    /* 主发送模式下产生停止信号后，I2C自动回到从机模式 */
                    amhw_hk32f103rbt6_i2c_genstop(p_hw_i2c, ENABLE);

                    /* 关闭中断TXE RXNE */
                    amhw_hk32f103rbt6_i2c_iten_mode_set(p_hw_i2c,
                                                  I2C_CR2_ITEVTEN |
                                                  I2C_CR2_ITBUFEN,
                                                  DISABLE);

                    __softimer_stop(p_dev);

                    if (AM_FALSE != p_dev->is_complete) {
                        __I2C_NEXT_STATE(__I2C_ST_IDLE, __I2C_EVT_NONE);
                        p_dev->is_complete = AM_FALSE;
                        p_dev->busy = AM_FALSE;

                        /* 回调消息完成函数 */
                        if (p_dev->p_cur_msg->pfn_complete != NULL) {
                            p_dev->p_cur_msg->pfn_complete(
                                p_dev->p_cur_msg->p_arg);
                        }

                    } else {
                        __i2c_re_init(p_dev);
                    }
                    break;

                } else {

                    /* 接收模式  产生停止条件前  还应在倒数第二个数据 是设置    EV7: RXE = 1 读DR清除该事件 */

                    amhw_hk32f103rbt6_i2c_iten_mode_set(p_hw_i2c,
                                                  I2C_CR2_ITEVTEN |
                                                  I2C_CR2_ITBUFEN,
                                                  DISABLE);

                    __softimer_stop(p_dev);

                    if (AM_FALSE != p_dev->is_complete) {
                        __I2C_NEXT_STATE(__I2C_ST_IDLE, __I2C_EVT_NONE);
                        p_dev->is_complete = AM_FALSE;
                        p_dev->busy = AM_FALSE;

                        /* 回调消息完成函数 */
                        if (p_dev->p_cur_msg->pfn_complete != NULL) {
                            p_dev->p_cur_msg->pfn_complete(
                                p_dev->p_cur_msg->p_arg);
                        }

                    } else {
                        __i2c_re_init(p_dev);
                    }
                    break;
                }

            } else {    /* 获取到一个传输，正确处理该传输即可 */

                    struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;

                    if (NULL == p_cur_trans) {
                        __softimer_stop(p_dev);
                        __i2c_re_init(p_dev);

                        return -AM_EINVAL;
                    }

                    p_dev->data_ptr = 0;

                    /*
                      * 不需要启动信号，直接传输，必须同时满足以下三个条件：
                      * 1.设置了标志 AM_I2C_M_NOSTART
                      * 2./当前传输不是一个消息中的第一个传输
                      * 3.当前传输与上一个传输的方向一致
                      */
                    if ((p_cur_trans->flags & AM_I2C_M_NOSTART) &&
                        ((p_cur_trans->flags & AM_I2C_M_RD) ==
                        ((p_cur_trans - 1)->flags & AM_I2C_M_RD))) {

                        am_bool_t is_read = (p_cur_trans->flags & AM_I2C_M_RD) ?
                                         AM_TRUE : AM_FALSE;

                        /* 当前传输状态和上一状态 从机地址在空闲状态以传输*/
                        p_dev->trans_state = 1;

                        if (is_read == AM_TRUE) {

                            /* 继续接受数据 */
                            __I2C_NEXT_STATE(__I2C_ST_M_RECV_DATA,
                                    __I2C_EVT_NONE);
                            break;
                        } else {
                            /* 继续发送数据 */
                            __I2C_NEXT_STATE(__I2C_ST_M_SEND_DATA,
                                    __I2C_EVT_NONE);
                            break;
                        }
                    /* 发送从机地址 */
                    } else {
                        /* 此处传输了从机地址 */
                        p_dev->trans_state = 0;
                        /* 下一步操作是发送地址 */
                        __I2C_NEXT_STATE(__I2C_ST_SEND_SLA_ADDR,
                                __I2C_EVT_MST_START);
                        event     = new_event;
                        new_event = __I2C_EVT_NONE;
                    }
            }
        }

        case __I2C_ST_SEND_SLA_ADDR:
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;
            struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;

            if ((NULL == p_cur_msg) || (NULL == p_cur_trans)) {
                __softimer_stop(p_dev);
                __i2c_re_init(p_dev);
                return -AM_EINVAL;
            }

            /* 暂不支持10bit 模式 */
            if (p_cur_trans->flags & AM_I2C_ADDR_10BIT) {

                /* 更新消息状态 */
                p_cur_msg->status = -AM_ENOTSUP;

                /* 忽略剩下的transfer */
                p_dev->p_cur_trans = p_cur_msg->p_transfers +
                                     p_cur_msg->trans_num;

                /* 结束当前的消息传输 */
                __I2C_NEXT_STATE(__I2C_ST_TRANS_START,
                                 __I2C_EVT_TRANS_LAUNCH);

                break;
            }

            /* 产生起始条件 切换至主模式 */
            amhw_hk32f103rbt6_i2c_genstrat(p_hw_i2c, ENABLE);

            /* EV5事件 ： 等待SB */
            while (!amhw_hk32f103rbt6_i2c_checkevent(p_hw_i2c,
                    I2C_EVENT_MASTER_MODE_SELECT ));

            /* 清除SB */
            amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);

            /* 发送从机地址 */
            if ( p_cur_trans->flags & AM_I2C_M_RD) {

                __I2C_NEXT_STATE(__I2C_ST_M_RECV_DATA,
                        __I2C_EVT_NONE);
                /* 读*/
                amhw_hk32f103rbt6_i2c_send7bit_address(
                    p_hw_i2c, p_dev->slave_addr, 1);
            } else {

                __I2C_NEXT_STATE(__I2C_ST_M_SEND_DATA,
                        __I2C_EVT_NONE);
                /* 写 */
                amhw_hk32f103rbt6_i2c_send7bit_address(
                    p_hw_i2c, p_dev->slave_addr, 0);
            }

            /* 等待EV6事件标志位 */
            while (1) {
                if (p_hw_i2c->i2c_sr1 & 0x0002) {
                    break;
                }
            }

            if ( p_cur_trans->flags & AM_I2C_M_RD) {
                if (p_cur_trans->nbytes == 1) {
                    i2c_pulldown_scl(p_dev);

                    amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);
                    amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr2);

                    amhw_hk32f103rbt6_i2c_ack_en(p_hw_i2c, DISABLE);
                    amhw_hk32f103rbt6_i2c_genstop(p_hw_i2c, ENABLE);
                    i2c_release_scl(p_dev);
                } else if (p_cur_trans->nbytes == 2) {
                    amhw_hk32f103rbt6_i2c_ack_en(p_hw_i2c, ENABLE);

                    /* 设置POS位 */
                    amhw_hk32f103rbt6_i2c_pos(p_hw_i2c, ENABLE);

                    i2c_pulldown_scl(p_dev);

                    amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);
                    amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr2);
                    amhw_hk32f103rbt6_i2c_ack_en(p_hw_i2c, DISABLE);

                    i2c_release_scl(p_dev);
                } else {

                    /* 读取sr2寄存器清除EV6事件 */
                    amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);
                    amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr2);
                }
            } else {

                /* 读取sr2寄存器清除EV6事件 */
                amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);
                amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr2);
                if ( ((p_cur_trans->flags & 0x01) == 0) && (p_dev->data_ptr == 0)) {
                    amhw_hk32f103rbt6_i2c_send_data(p_hw_i2c,
                         (p_cur_trans->p_buf)[p_dev->data_ptr++]);
                }
            }

            /* 打开TXE RXNE中断使能 */
            amhw_hk32f103rbt6_i2c_iten_mode_set(p_hw_i2c,
                                          I2C_CR2_ITEVTEN |
                                          I2C_CR2_ITBUFEN ,
                                          ENABLE);
            break;

        }
        case __I2C_ST_M_RECV_DATA:
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;
            struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;

            if ((NULL == p_cur_msg) || (NULL == p_cur_trans)) {
                __softimer_stop(p_dev);
                __i2c_re_init(p_dev);

                return -AM_EINVAL;
            }

            if (p_cur_trans->nbytes ==1) {

                /* 读取一个数据 */
                (p_cur_trans->p_buf)[p_dev->data_ptr++] = \
                        amhw_hk32f103rbt6_i2c_recv_data(p_hw_i2c);
            } else if (p_cur_trans->nbytes ==2) {

                /* 读取两个数据 */
                i2c_pulldown_scl(p_dev);

                /* 清除POS位 */
                amhw_hk32f103rbt6_i2c_pos(p_hw_i2c, DISABLE);

                amhw_hk32f103rbt6_i2c_ack_en(p_hw_i2c, ENABLE);
                amhw_hk32f103rbt6_i2c_genstop(p_hw_i2c, ENABLE);
                (p_cur_trans->p_buf)[p_dev->data_ptr++] = \
                        amhw_hk32f103rbt6_i2c_recv_data(p_hw_i2c);

                i2c_release_scl(p_dev);
                (p_cur_trans->p_buf)[p_dev->data_ptr++] = \
                        amhw_hk32f103rbt6_i2c_recv_data(p_hw_i2c);
            }else if (p_dev->data_ptr == (p_cur_trans->nbytes - 3) &&
                     (p_cur_trans->nbytes !=1) &&
                     (p_cur_trans->nbytes !=2)) {

                /* 读取第N-2 个数据 */
                i2c_pulldown_scl(p_dev);
                amhw_hk32f103rbt6_i2c_ack_en(p_hw_i2c, DISABLE);
                i2c_release_scl(p_dev);
                amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);
                (p_cur_trans->p_buf)[p_dev->data_ptr++] = \
                        amhw_hk32f103rbt6_i2c_recv_data(p_hw_i2c);
            } else if (p_dev->data_ptr == (p_cur_trans->nbytes - 2) &&
                      (p_cur_trans->nbytes !=1) &&
                      (p_cur_trans->nbytes !=2)) {

                /* 读取第N-1 第N 个数据 */
                i2c_pulldown_scl(p_dev);
                amhw_hk32f103rbt6_i2c_ack_en(p_hw_i2c, ENABLE);
                amhw_hk32f103rbt6_i2c_genstop(p_hw_i2c, ENABLE);
                amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);
                (p_cur_trans->p_buf)[p_dev->data_ptr++] = \
                        amhw_hk32f103rbt6_i2c_recv_data(p_hw_i2c);
                i2c_release_scl(p_dev);
                amhw_hk32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);
                (p_cur_trans->p_buf)[p_dev->data_ptr++] = \
                        amhw_hk32f103rbt6_i2c_recv_data(p_hw_i2c);
            } else {

                /* 读取多个数据 */
                (p_cur_trans->p_buf)[p_dev->data_ptr++] = \
                        amhw_hk32f103rbt6_i2c_recv_data(p_hw_i2c);
            }


            if (p_dev->data_ptr != p_cur_trans->nbytes) {

                __I2C_NEXT_STATE(__I2C_ST_M_RECV_DATA,
                        __I2C_EVT_NONE);
            } else { /* 接收数据完毕，该传输完成 */

                /* 保存从机地址 */
                p_dev->slave_addr =p_dev->p_cur_trans->addr;

                p_cur_msg->done_num++;
                p_dev->p_cur_trans++;

                /* 关闭 TXE RXNE中断 */
                amhw_hk32f103rbt6_i2c_iten_mode_set(p_hw_i2c,
                                              I2C_CR2_ITEVTEN |
                                              I2C_CR2_ITBUFEN ,
                                              DISABLE);
                /* 等待硬件清除 stop 位 */
                while((p_hw_i2c->i2c_cr1 & 0x200));

                /* 在读数据结束后，使能ACK位*/
                amhw_hk32f103rbt6_i2c_ack_en(p_hw_i2c, ENABLE);

                /* 本传输完毕，进入下一传输 */
                __I2C_NEXT_STATE(__I2C_ST_TRANS_START,
                                 __I2C_EVT_TRANS_LAUNCH);
            }
            break;
        }
        case __I2C_ST_M_SEND_DATA:
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;
            struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;
            if (event != __I2C_EVT_MST_TX) {
                return -AM_EINVAL;    /* 事件需与自身状态匹配 */
            }
            if ((NULL == p_cur_msg) ||(NULL == p_cur_trans)) {
                __softimer_stop(p_dev);
                __i2c_re_init(p_dev);

                return -AM_EINVAL;
            }

            if (p_dev->data_ptr < p_cur_trans->nbytes) {

                amhw_hk32f103rbt6_i2c_send_data(p_hw_i2c,
                     (p_cur_trans->p_buf)[p_dev->data_ptr++]);

            } else {
                /* 保存从机地址 */
                p_dev->slave_addr =p_dev->p_cur_trans->addr;

                p_cur_msg->done_num++;
                p_dev->p_cur_trans++;

                /*
                 * 上一次传输为 write 本次传输为 read时 关闭中断使能 避免发送从地址 产生addr中断
                 */
                if((p_dev->p_cur_trans->flags & AM_I2C_M_RD) &&
                    (!((p_dev->p_cur_trans - 1)->flags & AM_I2C_M_RD))) {
                    amhw_hk32f103rbt6_i2c_genstop(p_hw_i2c, ENABLE);
                    amhw_hk32f103rbt6_i2c_iten_mode_set(p_hw_i2c,
                            I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN , DISABLE);

                }
                /* 等待硬件清除 stop 位 */
                while((p_hw_i2c->i2c_cr1 & 0x200));
                /* 本传输完毕，进入下一传输 */
                __I2C_NEXT_STATE(__I2C_ST_TRANS_START,
                                 __I2C_EVT_TRANS_LAUNCH);
            }
            break;
        }

        default:
            break;
        }

        if (p_dev->is_abort != AM_FALSE) {
            p_dev->is_abort = AM_FALSE;
            return -AM_FALSE;
        }

        if (new_event == __I2C_EVT_NONE) {
            break;
        }
    }

    return AM_OK;
}
/* end of file */
