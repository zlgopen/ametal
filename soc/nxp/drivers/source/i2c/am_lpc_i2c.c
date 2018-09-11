/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief I2C drivers implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-01-10  jon, first implementation.
 * \endinternal
 */

#include "am_lpc_i2c.h"
#include "am_clk.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

#define __I2C_ST_IDLE             (0x10u)          /* 空闲状态             */
#define __I2C_ST_MSG_START        (0x11u)          /* 消息传输开始状态     */
#define __I2C_ST_TRANS_START      (0x12u)          /* 单个传输开始状态     */
#define __I2C_ST_SEND_SLA_ADDR    (0x13u)          /* 发送从机地址状态     */
#define __I2C_ST_M_SEND_DATA      (0x14u)          /* 发送数据状态         */
#define __I2C_ST_M_RECV_DATA      (0x15u)          /* 接收数据状态         */
#define __I2C_ST_ARBI_LOST        (0x16u)          /* 仲裁丢失状态         */
#define __I2C_ST_TIMEOUT          (0x17u)          /* 超时状态            */

#define __I2C_EVT_NONE            (0xFFu)          /* 无事件               */
#define __I2C_EVT_MSG_LAUNCH      (0xFEu)          /* 开始处理一个新的消息 */
#define __I2C_EVT_TRANS_LAUNCH    (0xFDu)          /* 开始处理一个新的传输 */

#define __I2C_EVT_MST_IDLE         AM_SBF(0x0, 1)  /* 主机空闲事件         */
#define __I2C_EVT_MST_RX           AM_SBF(0x1, 1)  /* 可以接收数据         */
#define __I2C_EVT_MST_TX           AM_SBF(0x2, 1)  /* 可以发送数据         */

#define __I2C_EVT_MST_ADDR_NO_ACK  AM_SBF(0x3, 1)  /* 地址无应答           */
#define __I2C_EVT_MST_DATA_NO_ACK  AM_SBF(0x4, 1)  /* 数据无应答           */

#define __I2C_EVT_MST_BERR         AM_SBF(0x5, 1)  /* 总线错误             */

#define __I2C_EVT_MST_TIMEOUT      AM_SBF(0x6, 1)  /* 超时错误             */

/*******************************************************************************
  函数声明
*******************************************************************************/

/** \brief I2C 速率设置函数 */
static int __i2c_speed_set (am_lpc_i2c_dev_t *p_dev, uint32_t bus_speed);

/** \brief I2C 硬件初始化 */
static int __i2c_hard_init (am_lpc_i2c_dev_t *p_dev);

/** \brief I2C 中断处理函数 */
static void __i2c_irq_handler (void *p_arg);

/** \brief I2C 状态机函数 */
static int __i2c_mst_sm_event (am_lpc_i2c_dev_t *p_dev, uint32_t event);

/** \brief I2C 消息处理函数 */
static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_trans);


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

/**
 * \brief I2C 总线速率设置
 *
 * \param[in] p_dev     : 指向I2C设备结构体
 * \param[in] bus_speed : 总线速率
 *
 * \retval  AM_OK     : 设置成功
 * \retval -AM_EINVAL : 参数非法
 */
static int __i2c_speed_set (am_lpc_i2c_dev_t *p_dev, uint32_t bus_speed)
{
    amhw_lpc_i2c_t *p_hw_i2c  = NULL;

    uint32_t    scl, div, div_top;
    uint32_t    scl_h, scl_l;
    uint32_t    sum_div, cur_div;
    uint32_t    f_scl = 0, f_div = 0;

    uint32_t    err, min_err;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c  = (amhw_lpc_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    if (bus_speed > 1000000) {
        return -AM_EINVAL;        /* 支持的最大频率为1MHz */
    }

    /*
     * I2C时钟的高电平和低电平均在2 ~ 9之间，即分频后，
     * 应该使到4 ~ 18个时钟组成的时钟可以得到需要的频率
     */
    sum_div = am_clk_rate_get(p_dev->p_devinfo->clk_id) / bus_speed;

    /* 因为SCL最小值为4，所以分频值最大只需要为：总分频值 / 4 */
    div_top = sum_div / 4 + 1;

    /* 总的分频小于 65536 */
    if (div_top >= 65536) {
        div_top = 65536;    /* 分频器的分频值最大为65536 */
    }

    min_err = 0xFFFFFFFF;

    for (div = 1; div <= div_top; div++) {
        for (scl = 4; scl <= 18; scl += 2) {

            cur_div = div * scl;

            if (cur_div > sum_div) {
                err = cur_div - sum_div;
            } else {
                err = sum_div - cur_div;
            }

            /* 找到了最佳分频值  */
            if (err == 0) {
                f_scl = scl;
                f_div = div;
                break;
            }

            if (err < min_err) {     /* 本次误差小于上次误差 */
                f_scl   = scl;
                f_div   = div;
                min_err = err;
            }
        }

        /* 找到了最佳分频值  */
        if (err == 0) {
            break;
        }
    }

    amhw_lpc_i2c_clkdiv_set(p_hw_i2c, f_div);

    p_dev->i2c_clkrate = am_clk_rate_get(p_dev->p_devinfo->clk_id) / f_div;

    scl_h = f_scl >> 1;               /* 占空比设置默认为50% */
    scl_l = (f_scl - scl_h);

    if (scl_h < 2) {
        scl_h = 2;
    } else if (scl_h > 9) {
        scl_h = 9;
    }

    if (scl_l < 2) {
        scl_l = 2;
    } else if (scl_l > 9) {
        scl_l = 9;
    }

    amhw_lpc_i2c_msttime_set(p_hw_i2c, scl_h, scl_l);

    return AM_OK;
}

/******************************************************************************/

static int __i2c_slv_timeout_set (am_lpc_i2c_dev_t *p_dev,
                                  uint32_t             timeout_ms)
{
    amhw_lpc_i2c_t *p_hw_i2c = NULL;
    uint32_t    ticks;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_lpc_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    /* 不需要超时 */
    if (timeout_ms == 0) {
        amhw_lpc_i2c_mode_disable(p_hw_i2c, AM_I2C_TIMEOUT_MODE);
        amhw_lpc_i2c_int_disable(p_hw_i2c,
                                    AMHW_LPC_I2C_INTENSET_EVTIMEOUT |
                                    AMHW_LPC_I2C_INTENSET_SCLTIMEOUT);
        return AM_OK;
    }

    /* 计算需要定时的ticks */
    ticks = p_dev->i2c_clkrate / 1000 * timeout_ms;

    /* 最大tick为65536 */
    if (ticks > 65536) {
        ticks = 65536;
    }

    amhw_lpc_i2c_mode_disable(p_hw_i2c, AM_I2C_TIMEOUT_MODE);

    amhw_lpc_i2c_timeout_set(p_hw_i2c, ticks >> 4);

    amhw_lpc_i2c_mode_enable(p_hw_i2c, AM_I2C_TIMEOUT_MODE);

    amhw_lpc_i2c_int_enable(p_hw_i2c, AMHW_LPC_I2C_INTENSET_EVTIMEOUT |
                                         AMHW_LPC_I2C_INTENSET_SCLTIMEOUT);
    return AM_OK;
}

/******************************************************************************/

/** \brief I2C 硬件初始化 */
static int __i2c_hard_init(am_lpc_i2c_dev_t *p_dev)
{
    amhw_lpc_i2c_t *p_hw_i2c = NULL;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_lpc_i2c_t *)p_dev->p_devinfo->i2c_regbase;
    
    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }
    
    amhw_lpc_i2c_mode_disable(p_hw_i2c, AM_I2C_MASTER_MODE);

    __i2c_speed_set(p_dev, p_dev->p_devinfo->bus_speed);

    amhw_lpc_i2c_mode_enable(p_hw_i2c, AM_I2C_MASTER_MODE);

    __i2c_slv_timeout_set(p_dev, p_dev->p_devinfo->timeout_ms);

    return AM_OK;
}

/******************************************************************************/

/**
 * \brief 添加一条 message 到控制器传输列表末尾
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
void __i2c_msg_in (am_lpc_i2c_dev_t *p_dev, struct am_i2c_message *p_msg)
{
    am_list_add_tail((struct am_list_head *)(&p_msg->ctlrdata),
                     &(p_dev->msg_list));
}

/**
 * \brief 从控制器传输列表表头取出一条 message
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
struct am_i2c_message *__i2c_msg_out (am_lpc_i2c_dev_t *p_dev)
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
 * \brief I2C 中断处理函数
 *
 * \param[in] p_arg : 指向I2C设备结构体的指针
 *
 * \return 无
 */
static void __i2c_irq_handler (void *p_arg)
{
    am_lpc_i2c_dev_t *p_dev    = NULL;
    amhw_lpc_i2c_t   *p_hw_i2c = NULL;
    uint32_t i2c_status;

    if (p_arg == NULL) {
        return;
    }

    p_dev    = (am_lpc_i2c_dev_t *)p_arg;
    p_hw_i2c = (amhw_lpc_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    i2c_status = amhw_lpc_i2c_intstat_get(p_hw_i2c);


    /* 主机挂起状态       */
    if ((i2c_status & AMHW_LPC_I2C_STAT_MSTPEND)           &&
        ((i2c_status & AMHW_LPC_I2C_STAT_EVTIMEOUT)  == 0) &&
        ((i2c_status & AMHW_LPC_I2C_STAT_SCLTIMEOUT) == 0) &&
        ((i2c_status & AMHW_LPC_I2C_STAT_MSTARBLOSS) == 0) &&
        ((i2c_status & AMHW_LPC_I2C_STAT_MSTSSERR)   == 0)) {

        __i2c_mst_sm_event(p_dev, (amhw_lpc_i2c_stat_get(p_hw_i2c) &
                                   AMHW_LPC_I2C_MASTER_STATE_MASK));
    }

    /* 超时 */
    if  ((i2c_status & AMHW_LPC_I2C_STAT_EVTIMEOUT) ||
         (i2c_status & AMHW_LPC_I2C_STAT_SCLTIMEOUT)) {

        /* 清除状态          */
        amhw_lpc_i2c_stat_clr(p_hw_i2c,
                                 AMHW_LPC_I2C_STAT_EVTIMEOUT  |
                                 AMHW_LPC_I2C_STAT_SCLTIMEOUT |
                                 AMHW_LPC_I2C_STAT_MSTARBLOSS |
                                 AMHW_LPC_I2C_STAT_MSTSSERR);

        __i2c_mst_sm_event(p_dev, __I2C_EVT_MST_TIMEOUT);

        /* 禁能后再使能超时功能，复位超时内部逻辑 */
        amhw_lpc_i2c_mode_disable(p_hw_i2c, AM_I2C_TIMEOUT_MODE);

        amhw_lpc_i2c_mode_enable(p_hw_i2c, AM_I2C_TIMEOUT_MODE);


      /* 总线仲裁丢失、启动/停止错误 */
    } else if ((i2c_status & AMHW_LPC_I2C_STAT_MSTARBLOSS)||
               (i2c_status & AMHW_LPC_I2C_STAT_MSTSSERR)) {

       /* 清除状态          */
        amhw_lpc_i2c_stat_clr(p_hw_i2c,
                                 AMHW_LPC_I2C_STAT_MSTARBLOSS |
                                 AMHW_LPC_I2C_STAT_MSTSSERR);
        __i2c_mst_sm_event(p_dev, __I2C_EVT_MST_BERR);
    }
}

/******************************************************************************/

static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_msg)
{
    am_lpc_i2c_dev_t *p_dev    = (am_lpc_i2c_dev_t *)p_drv;

    amhw_lpc_i2c_t   *p_hw_i2c = (amhw_lpc_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    int key;

    if ( (p_dev              == NULL) ||
         (p_msg              == NULL) ||
         (p_msg->p_transfers == NULL) ||
         (p_msg->trans_num   == 0)) {

        return -AM_EINVAL;
    }

    key = am_int_cpu_lock();

    /* 当前正在处理消息，只需要将新的消息加入链表即可 */
    if (p_dev->busy == AM_TRUE) {

        __i2c_msg_in(p_dev, p_msg);

        am_int_cpu_unlock(key);

        return AM_OK;

    } else {


        p_dev->busy = AM_TRUE;
        __i2c_msg_in(p_dev, p_msg);
        p_msg->status = -AM_EISCONN; /* 正在排队中 */

        am_int_cpu_unlock(key);

        /* 打开挂起、仲裁丢失、启动/停止错误中断 */
        amhw_lpc_i2c_int_enable(p_hw_i2c,
                                   AMHW_LPC_I2C_INTENSET_MSTPEND    |
                                   AMHW_LPC_I2C_INTENSET_MSTARBLOSS |
                                   AMHW_LPC_I2C_INTENSET_MSTSSERR);

        return AM_OK;
    }
}

/**
 * \brief I2C 初始化
 */
am_i2c_handle_t am_lpc_i2c_init (am_lpc_i2c_dev_t           *p_dev,
                                    const am_lpc_i2c_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->i2c_serv.p_funcs = (struct am_i2c_drv_funcs *)&__g_i2c_drv_funcs;
    p_dev->i2c_serv.p_drv   = p_dev;
  
    p_dev->p_devinfo = p_devinfo;
    
    p_dev->busy        = AM_FALSE;
    p_dev->p_cur_trans = NULL;
    p_dev->p_cur_msg   = NULL;
    p_dev->data_ptr    = 0;
    p_dev->state       = __I2C_ST_IDLE;                   /* 初始化为空闲状态 */

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    am_list_head_init(&(p_dev->msg_list));

    __i2c_hard_init(p_dev);

    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum, __i2c_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->i2c_serv);
}

/**
 * \brief I2C解初始化
 */
void am_lpc_i2c_deinit (am_i2c_handle_t handle)
{
    amhw_lpc_i2c_t   *p_hw_i2c = NULL;
    am_lpc_i2c_dev_t *p_dev    = NULL;

    if (NULL == handle) {
        return ;
    }

    p_dev    = (am_lpc_i2c_dev_t *)handle;
    p_hw_i2c = (amhw_lpc_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    p_dev->i2c_serv.p_funcs = NULL;
    p_dev->i2c_serv.p_drv   = NULL;

    amhw_lpc_i2c_mode_disable(p_hw_i2c, AM_I2C_MASTER_MODE);
    
    /* 去除中断连接 */
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __i2c_irq_handler, (void *)p_dev);

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

/* check if transfers empty */
#define __I2C_TRANS_EMPTY(p_dev) \
    ((p_dev)->p_cur_trans >= (p_dev)->p_cur_msg->p_transfers \
                             + (p_dev)->p_cur_msg->trans_num)

/**
 * \brief I2C 状态机函数
 */
static int __i2c_mst_sm_event (am_lpc_i2c_dev_t *p_dev, uint32_t event)
{
    volatile uint32_t new_event       = __I2C_EVT_NONE;


    amhw_lpc_i2c_t *p_hw_i2c = NULL;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c  = (amhw_lpc_i2c_t *)p_dev->p_devinfo->i2c_regbase;


    while (1) {

        /* 检查是否有新的事件在状态机内部产生 */
        if (new_event != __I2C_EVT_NONE) {
            event     = new_event;
            new_event = __I2C_EVT_NONE;
        }

        /* 以设备的状态为基准进行状态的设置 */
        switch(p_dev->state) {

        /*
         * 空闲状态和开始消息传输状态要处理的事情是一样，事件只应是：
         * __I2C_EVT_TRANS_LAUNCH
         */
        case __I2C_ST_IDLE:
        case __I2C_ST_MSG_START:
        {

            am_i2c_message_t *p_cur_msg = NULL;

            int key;

            key = am_int_cpu_lock();
            p_cur_msg        = __i2c_msg_out(p_dev);
            p_dev->p_cur_msg = p_cur_msg;

            if (p_cur_msg) {

                p_cur_msg->status = -AM_EINPROGRESS;

            } else {

                /* 关闭中断 */
                amhw_lpc_i2c_int_disable(p_hw_i2c,
                                            AMHW_LPC_I2C_INTENSET_MSTPEND    |
                                            AMHW_LPC_I2C_INTENSET_MSTARBLOSS |
                                            AMHW_LPC_I2C_INTENSET_MSTSSERR);

                p_dev->busy = AM_FALSE;
            }

            am_int_cpu_unlock(key);

            /* 无需要处理的消息 */
            if (p_cur_msg == NULL) {

                __I2C_NEXT_STATE(__I2C_ST_IDLE, __I2C_EVT_NONE);

                break;

            } else {

                p_cur_msg->done_num = 0;
                p_dev->p_cur_trans  = p_cur_msg->p_transfers;
                p_dev->data_ptr     = 0;

                __I2C_NEXT_STATE(__I2C_ST_TRANS_START, __I2C_EVT_TRANS_LAUNCH);

                /* 直接进入下一个状态，开始一个传输，此处无需break */
                event     = new_event;
                new_event = __I2C_EVT_NONE;
            }
        }
        /* no break */
        case __I2C_ST_TRANS_START :
        {
            struct am_i2c_message *p_cur_msg = p_dev->p_cur_msg;

            if (event != __I2C_EVT_TRANS_LAUNCH) {
                return -AM_EINVAL;  /* 传输开始状态等待的消息必须是启动传输 */
            }


            /* 当前消息传输完成 */
            if (__I2C_TRANS_EMPTY(p_dev)) {

                /* 消息正在处理中 */
                if (p_cur_msg->status == -AM_EINPROGRESS) {
                    p_cur_msg->status = AM_OK;
                }

                /* 回调消息完成函数  */
                if (p_cur_msg->pfn_complete != NULL) {
                    p_cur_msg->pfn_complete(p_cur_msg->p_arg);
                }


                /* 停止传输 */
                amhw_lpc_i2c_mst_stop(p_hw_i2c);

                __I2C_NEXT_STATE(__I2C_ST_MSG_START, __I2C_EVT_MSG_LAUNCH);

            } else {                    /* 获取到一个传输，正确处理该传输即可 */

                struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;

                p_dev->data_ptr = 0;

                /*
                 * 不需要启动信号，直接传输，必须同时满足以下三个条件：
                 * 1.设置了标志 AM_I2C_M_NOSTART
                 * 2.当前传输不是一个消息中的第一个传输
                 * 3.当前传输与上一个传输的方向一致
                 */
                if ((p_cur_trans->flags & AM_I2C_M_NOSTART) &&
                    (p_cur_trans != p_cur_msg->p_transfers) &&
                    ((p_cur_trans->flags & AM_I2C_M_RD) ==
                     ((p_cur_trans - 1)->flags & AM_I2C_M_RD))) {

                    am_bool_t is_read = (p_cur_trans->flags & AM_I2C_M_RD) ?
                                          AM_TRUE : AM_FALSE;

                    if (is_read == AM_TRUE) {

                        /* 如果是接收，需要自行一次continue，以便继续接收数据 */
                        if (amhw_lpc_i2c_stat_get(p_hw_i2c) &
                            AMHW_LPC_I2C_STAT_MSTPEND) {

                            amhw_lpc_i2c_mst_continue(p_hw_i2c);
                        }

                        /* 继续接受数据 */
                        __I2C_NEXT_STATE(__I2C_ST_M_RECV_DATA, __I2C_EVT_MSG_LAUNCH);
                    } else {

                        /* 继续发送数据 */
                        __I2C_NEXT_STATE(__I2C_ST_M_SEND_DATA, __I2C_EVT_MSG_LAUNCH);
                    }

                /* 发送从机地址 */
                } else {
                    /* 下一步操作是发送从机地址 */
                    __I2C_NEXT_STATE(__I2C_ST_SEND_SLA_ADDR, __I2C_EVT_MSG_LAUNCH);
                }
            }

            break;
        }
        
        case __I2C_ST_SEND_SLA_ADDR :                         /* 发送从机地址 */
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;
            struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;

            uint8_t dev_addr = (p_cur_trans->addr) << 1;

            /* 暂不支持10bit 模式 */
            if (p_cur_trans->flags & AM_I2C_ADDR_10BIT) {

                /* 更新消息状态 */
                p_cur_msg->status = -AM_ENOTSUP;

                /* 忽略剩下的transfer */
                p_dev->p_cur_trans = p_cur_msg->p_transfers +
                                     p_cur_msg->trans_num;

                /* 结束当前的消息传输 */
                __I2C_NEXT_STATE(__I2C_ST_TRANS_START, __I2C_EVT_TRANS_LAUNCH);

                break;
            }


            /* 发送启动信号 + 从机地址 */
            if (p_cur_trans->flags & AM_I2C_M_RD) {
                dev_addr |= 0x1;            /* 读操作 */

                /* 下一步是接收数据 */
                __I2C_NEXT_STATE(__I2C_ST_M_RECV_DATA, __I2C_EVT_NONE);

            } else {
                dev_addr |= 0x0;            /* 写操作 */

                /* 进入主机发送模式 */
                __I2C_NEXT_STATE(__I2C_ST_M_SEND_DATA, __I2C_EVT_NONE);
            }

            /* 发送设备地址 */
            amhw_lpc_i2c_mstdat_write(p_hw_i2c, dev_addr);
            amhw_lpc_i2c_mst_start(p_hw_i2c);

            break;
        }

        /* 当前处于接收数据状态 */
        case __I2C_ST_M_RECV_DATA:
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;
            struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;

            /* 接收数据，由主机应答，因此只可能产生地址无应答 */
            if ((event == __I2C_EVT_MST_ADDR_NO_ACK) ||
                (event == __I2C_EVT_MST_DATA_NO_ACK)) {

                if (!(p_cur_trans->flags & AM_I2C_M_IGNORE_NAK)) {

                    p_cur_msg->status = -AM_ENODEV;

                    /* 忽略剩下的transfer */
                    p_dev->p_cur_trans = p_cur_msg->p_transfers +
                                         p_cur_msg->trans_num;

                    __I2C_NEXT_STATE(__I2C_ST_TRANS_START,
                                     __I2C_EVT_TRANS_LAUNCH);
                    break;

                /* 忽略这个错误 */
                } else {
                    event = __I2C_EVT_MST_RX;
                }
            }

            /* 仲裁丢失、启动或停止错误 */
            if (event == __I2C_EVT_MST_BERR || event == __I2C_EVT_MST_TIMEOUT) {
                __I2C_NEXT_STATE(__I2C_ST_ARBI_LOST, __I2C_EVT_MST_BERR);
                break;
            }

            (p_cur_trans->p_buf)[p_dev->data_ptr++] = \
                                         amhw_lpc_i2c_mstdat_read(p_hw_i2c);

            if (p_dev->data_ptr < p_cur_trans->nbytes) {
                /* 继续接收数据，状态保持不变 */
                amhw_lpc_i2c_mst_continue(p_hw_i2c);

            } else {  /* 数据接收完毕， 该传输完成 */

                p_cur_msg->done_num++;
                p_dev->p_cur_trans++;

                /* 本传输完毕，进入下一传输 */
                __I2C_NEXT_STATE(__I2C_ST_TRANS_START, __I2C_EVT_TRANS_LAUNCH);
            }

            break;
        }

        case __I2C_ST_M_SEND_DATA :
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;
            struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;

            /* 地址或数据无应答 */
            if ( (event == __I2C_EVT_MST_ADDR_NO_ACK) ||
                 (event == __I2C_EVT_MST_DATA_NO_ACK)) {

                if (!(p_cur_trans->flags & AM_I2C_M_IGNORE_NAK)) {

                    p_cur_msg->status = (event == __I2C_EVT_MST_ADDR_NO_ACK) \
                                        ? -AM_ENODEV : -AM_ENOENT;

                    /* 忽略剩下的transfer */
                    p_dev->p_cur_trans = p_cur_msg->p_transfers +
                                         p_cur_msg->trans_num;

                    __I2C_NEXT_STATE(__I2C_ST_TRANS_START,
                                     __I2C_EVT_TRANS_LAUNCH);
                    break;

                /* 忽略这个错误 */
                } else {
                    event = __I2C_EVT_MST_TX;
                }

            }

            /* 仲裁丢失、启动或停止错误 */
            if (event == __I2C_EVT_MST_BERR || event == __I2C_EVT_MST_TIMEOUT) {
                __I2C_NEXT_STATE(__I2C_ST_ARBI_LOST, __I2C_EVT_MST_BERR);
                break;
            }

            if (p_dev->data_ptr < p_cur_trans->nbytes) {

                amhw_lpc_i2c_mstdat_write(p_hw_i2c,
                                     (p_cur_trans->p_buf)[p_dev->data_ptr++]);

                amhw_lpc_i2c_mst_continue(p_hw_i2c);
            } else {

                p_cur_msg->done_num++;
                p_dev->p_cur_trans++;

                /* 本传输完毕，进入下一传输 */
                __I2C_NEXT_STATE(__I2C_ST_TRANS_START, __I2C_EVT_TRANS_LAUNCH);
            }

            break;
        }

        /* 仲裁错误或超时，表明总线异常，直接结束这一次传输 */
        case __I2C_ST_ARBI_LOST:
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;

            /* 禁能再使能，复位硬件的内部逻辑 */
            amhw_lpc_i2c_mode_disable(p_hw_i2c, AM_I2C_MASTER_MODE);
            amhw_lpc_i2c_mode_enable(p_hw_i2c, AM_I2C_MASTER_MODE);

            /* 更新消息状态 */
            p_cur_msg->status = -AM_EIO;

            /* 忽略剩下的transfer */
            p_dev->p_cur_trans = p_cur_msg->p_transfers +
                                     p_cur_msg->trans_num;

            /* 结束当前消息传输 */
            __I2C_NEXT_STATE(__I2C_ST_TRANS_START, __I2C_EVT_TRANS_LAUNCH);

            break;
        }


        default:
            break;
        }

        /* 内部没有新事件产生，跳出 */
        if (new_event == __I2C_EVT_NONE) {
            break;
        }
    }

    return AM_OK;
}

/* end of file */
