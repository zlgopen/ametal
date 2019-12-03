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
 * \brief I2C驱动，服务I2C标准接口
 *
 * \internal
 * \par Modification History
 * - 1.01 17-12-28  pea, add a timeout mechanism, enable TX_ABRT and STOP_DET
 *                  interrupt
 * - 1.00 17-04-24  sdy, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件
*******************************************************************************/
#include "ametal.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_mm32_i2c.h"
#include "hw/amhw_mm32_i2c.h"

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

#define __I2C_EVT_MST_IDLE         AM_SBF(0x0, 1)  /* 主机空闲事件 */
#define __I2C_EVT_MST_RX           AM_SBF(0x1, 1)  /* 可以接收数据 */
#define __I2C_EVT_MST_TX           AM_SBF(0x2, 1)  /* 可以发送数据 */

/* check if transfers empty */
#define __I2C_TRANS_EMPTY(p_dev) \
    ((p_dev)->p_cur_trans >= (p_dev)->p_cur_msg->p_transfers \
                             + (p_dev)->p_cur_msg->trans_num)

/*******************************************************************************
  函数声明
*******************************************************************************/

/** \brief I2C 硬件初始化 */
static int __i2c_hard_init (am_mm32_i2c_dev_t *p_dev);

/** \brief I2C 中断处理函数 */
static void __i2c_irq_handler (void *p_arg);

/** \brief I2C 状态机函数 */
static int __i2c_mst_sm_event (am_mm32_i2c_dev_t *p_dev, uint32_t event);

/** \brief I2C 消息处理函数 */
static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_trans);

/** \brief 从控制器传输列表表头取出一条 message */
am_static_inline
struct am_i2c_message *__i2c_msg_out (am_mm32_i2c_dev_t *p_dev);

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
am_local int __i2c_hard_init (am_mm32_i2c_dev_t *p_dev)
{
    amhw_mm32_i2c_t *p_hw_i2c = NULL;
    uint16_t mode  = AMHW_MM32_I2C_NORMAL_SPEED;
    uint16_t count = (am_clk_rate_get (p_dev->p_devinfo->clk_num) /
                                       p_dev->p_devinfo->speed) /
                                       2;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_mm32_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    amhw_mm32_i2c_disable (p_hw_i2c);

    if (p_dev->p_devinfo->speed <= 100000) {
        amhw_mm32_i2c_ss_scl_low_set(p_hw_i2c, count);
        amhw_mm32_i2c_ss_scl_high_set(p_hw_i2c, count);
        mode = AMHW_MM32_I2C_NORMAL_SPEED;
    } else {
        amhw_mm32_i2c_fs_scl_low_set(p_hw_i2c, count);
        amhw_mm32_i2c_fs_scl_high_set(p_hw_i2c, count);
        mode = AMHW_MM32_I2C_HIGH_SPEED;
    }

    p_hw_i2c->ic_con = (mode |
                        AMHW_MM32_I2C_MASTER |
                        AMHW_MM32_I2C_RESTART_ENABLE);

    amhw_mm32_i2c_rx_tl_set(p_hw_i2c, 0);
    amhw_mm32_i2c_tx_tl_set(p_hw_i2c, 1);

    amhw_mm32_i2c_intr_mask_clear (p_hw_i2c, 0xfff);

    return AM_OK;
}

/** \brief I2C 硬件重新初始化 */
am_local int __i2c_hard_re_init (am_mm32_i2c_dev_t *p_dev)
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
    am_mm32_i2c_dev_t *p_dev = (am_mm32_i2c_dev_t *)p_arg;
    uint32_t             key   = 0;

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
am_local void __softimer_start (am_mm32_i2c_dev_t *p_dev)
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
am_local void __softimer_stop (am_mm32_i2c_dev_t *p_dev)
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
void __i2c_msg_in (am_mm32_i2c_dev_t *p_dev, struct am_i2c_message *p_msg)
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
struct am_i2c_message *__i2c_msg_out (am_mm32_i2c_dev_t *p_dev)
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
    am_mm32_i2c_dev_t *p_dev = (am_mm32_i2c_dev_t *)p_arg;

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
    am_mm32_i2c_dev_t *p_dev    = NULL;
    amhw_mm32_i2c_t   *p_hw_i2c = NULL;
    amhw_mm32_int_flag_t  i2c_int_status;

    if (p_arg == NULL) {
        return;
    }

    p_dev    = (am_mm32_i2c_dev_t *)p_arg;
    p_hw_i2c = (amhw_mm32_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    i2c_int_status = amhw_mm32_i2c_intr_stat_get (p_hw_i2c);

    /* 清除中断标志 */
    amhw_mm32_i2c_clr_intr_get(p_hw_i2c);

    /* 发送中止 */
    if (i2c_int_status & AMHW_MM32_INT_FLAG_TX_ABRT) {
        amhw_mm32_i2c_intr_mask_clear(p_hw_i2c,
                                     AMHW_MM32_INT_FLAG_TX_ABRT);
        if (!__I2C_TRANS_EMPTY(p_dev)) {
            __softimer_stop(p_dev);
            __i2c_re_init(p_dev);
            return;
        }
    }

    /* 接收非空，进入接收状态 */
    if (i2c_int_status & AMHW_MM32_INT_FLAG_RX_FULL) {
        amhw_mm32_i2c_intr_mask_clear(p_hw_i2c,
                                     AMHW_MM32_INT_FLAG_RX_FULL);
        __i2c_mst_sm_event(p_dev, __I2C_EVT_MST_RX);
    }

    /* 发送缓冲空，进入发送状态 */
    if (i2c_int_status & AMHW_MM32_INT_FLAG_TX_EMPTY) {
        amhw_mm32_i2c_intr_mask_clear(p_hw_i2c,
                                     AMHW_MM32_INT_FLAG_TX_EMPTY);
        __i2c_mst_sm_event(p_dev, __I2C_EVT_MST_TX);
    }

    /* 检测到停止位 */
    if (i2c_int_status & AMHW_MM32_INT_FLAG_STOP_DET) {
        amhw_mm32_i2c_intr_mask_clear(p_hw_i2c,
                                     AMHW_MM32_INT_FLAG_STOP_DET);
        __softimer_stop(p_dev);

        if (AM_FALSE != p_dev->is_complete) {
            p_dev->is_complete = AM_FALSE;

            amhw_mm32_i2c_intr_mask_clear(p_hw_i2c, 0);
            amhw_mm32_i2c_clr_intr_get(p_hw_i2c);

            /* 回调消息完成函数 */
            if (p_dev->p_cur_msg->pfn_complete != NULL) {
                p_dev->p_cur_msg->pfn_complete(p_dev->p_cur_msg->p_arg);
            }
            __i2c_mst_sm_event(p_dev, __I2C_EVT_MSG_LAUNCH);
        } else {
            __i2c_re_init(p_dev);
        }
    }
}

/******************************************************************************/

static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_msg)
{
    am_mm32_i2c_dev_t *p_dev    = (am_mm32_i2c_dev_t *)p_drv;
    amhw_mm32_i2c_t   *p_hw_i2c = (amhw_mm32_i2c_t *)p_dev->p_devinfo->i2c_regbase;
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

        /* 清除所有中断 */
        amhw_mm32_i2c_intr_mask_clear(p_hw_i2c, 0xfff);

        return __i2c_mst_sm_event(p_dev, __I2C_EVT_MSG_LAUNCH);
    }
}

/**
 * \brief I2C初始化
 */
am_i2c_handle_t am_mm32_i2c_init (am_mm32_i2c_dev_t           *p_dev,
                                 const am_mm32_i2c_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->i2c_serv.p_funcs = (struct am_i2c_drv_funcs *)&__g_i2c_drv_funcs;
    p_dev->i2c_serv.p_drv   = p_dev;

    p_dev->p_devinfo = p_devinfo;

    p_dev->p_cur_trans = NULL;
    p_dev->p_cur_msg   = NULL;
    p_dev->is_complete = AM_FALSE;
    p_dev->data_ptr    = 0;
    p_dev->trans_state = 0;
    p_dev->busy        = AM_FALSE;
    p_dev->state       = __I2C_ST_IDLE; /* 初始化为空闲状态 */
    p_dev->slave_addr  = 0;

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

    if (p_dev->p_devinfo->pfn_bus_clean) {
        p_dev->p_devinfo->pfn_bus_clean();
    }

    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum, __i2c_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->i2c_serv);
}

/**
 * \brief I2C解初始化
 */
void am_mm32_i2c_deinit (am_i2c_handle_t handle)
{
    amhw_mm32_i2c_t   *p_hw_i2c = NULL;
    am_mm32_i2c_dev_t *p_dev    = NULL;

    if (NULL == handle) {
        return ;
    }

    p_dev    = (am_mm32_i2c_dev_t *)handle;
    p_hw_i2c = (amhw_mm32_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    p_dev->i2c_serv.p_funcs = NULL;
    p_dev->i2c_serv.p_drv   = NULL;

    amhw_mm32_i2c_disable (p_hw_i2c);

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

/**
 * \brief I2C 状态机函数
 */
static int __i2c_mst_sm_event (am_mm32_i2c_dev_t *p_dev, uint32_t event)
{
    volatile uint32_t  new_event = __I2C_EVT_NONE;
    amhw_mm32_i2c_t *p_hw_i2c  = NULL;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_mm32_i2c_t *)p_dev->p_devinfo->i2c_regbase;
    
    /* 消除警告 */
    (void)event;
    
    while (1) {

        /* 检查是否有新的事件在状态机内部产生 */
        if (new_event != __I2C_EVT_NONE) {
            event     = new_event;
            new_event = __I2C_EVT_NONE;
        }

        /* 以设备的状态为基准进行状态的设置 */
        switch (p_dev->state) {

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
                amhw_mm32_i2c_intr_mask_clear (p_hw_i2c, 0xfff);

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
            struct am_i2c_message *p_cur_msg = p_dev->p_cur_msg;

            if (NULL == p_cur_msg) {
                __softimer_stop(p_dev);
                __i2c_re_init(p_dev);

                return -AM_EINVAL;
            }

            /* 当前消息传输完成 */
            if (__I2C_TRANS_EMPTY(p_dev)) {

                /* 消息正在处理中 */
                if (p_cur_msg->status == -AM_EINPROGRESS) {
                    p_cur_msg->status = AM_OK;
                }

                p_dev->is_complete = AM_TRUE;
                amhw_mm32_i2c_con_set(p_hw_i2c, 1 << 9); /* 产生一个停止信号 */
                __I2C_NEXT_STATE(__I2C_ST_IDLE,
                                 __I2C_EVT_NONE);

            } else {                    /* 获取到一个传输，正确处理该传输即可 */

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
                 * 2.当前传输不是一个消息中的第一个传输
                 * 3.当前传输与上一个传输的方向一致
                 */
                if ((p_cur_trans->flags & AM_I2C_M_NOSTART) &&
                    (p_cur_trans != p_cur_msg->p_transfers) &&
                    ((p_cur_trans->flags & AM_I2C_M_RD) ==
                    ((p_cur_trans - 1)->flags & AM_I2C_M_RD))) {

                    am_bool_t is_read = (p_cur_trans->flags & AM_I2C_M_RD) ?
                                     AM_TRUE : AM_FALSE;

                    p_dev->trans_state = 1;
                    if (is_read == AM_TRUE) {

                        /* 继续接受数据 */
                        __I2C_NEXT_STATE(__I2C_ST_M_RECV_DATA,
                                         __I2C_EVT_NONE);

                        amhw_mm32_i2c_intr_mask_set(p_hw_i2c,
                                                  AMHW_MM32_INT_FLAG_RX_FULL);
                    } else {

                        /* 继续发送数据 */
                        __I2C_NEXT_STATE(__I2C_ST_M_SEND_DATA,
                                         __I2C_EVT_NONE);
                        amhw_mm32_i2c_intr_mask_set(p_hw_i2c,
                                                   AMHW_MM32_INT_FLAG_TX_EMPTY);
                    }

                /* 发送从机地址 */
                } else {
                    p_dev->trans_state = 0;

                    /* 下一步操作是发送从机地址 */
                    __I2C_NEXT_STATE(__I2C_ST_SEND_SLA_ADDR,
                                     __I2C_EVT_MST_IDLE);
                }
            }
            break;
        }

        case __I2C_ST_SEND_SLA_ADDR:         /* 发送从机地址 */
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;
            struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;

            if ((NULL == p_cur_msg) ||(NULL == p_cur_trans)) {
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

            if (p_cur_trans->addr != p_dev->slave_addr) {
                __i2c_hard_re_init(p_dev);

                amhw_mm32_i2c_disable(p_hw_i2c);

                /* 发送设备地址 */
                amhw_mm32_i2c_tar_set(p_hw_i2c, p_cur_trans->addr);

                /* 设置7bit地址格式 */
                amhw_mm32_i2c_con_clear(p_hw_i2c,
                                       AMHW_MM32_I2C_7BITADDR_MASTER);

                amhw_mm32_i2c_enable(p_hw_i2c);
            }

            amhw_mm32_i2c_intr_mask_set(p_hw_i2c,
                                       AMHW_MM32_INT_FLAG_TX_ABRT);
            amhw_mm32_i2c_intr_mask_set(p_hw_i2c,
                                       AMHW_MM32_INT_FLAG_STOP_DET);

            /* 发送启动信号 + 从机地址 */
            if (p_cur_trans->flags & AM_I2C_M_RD) {

                /* 下一步是接收数据 */
                __I2C_NEXT_STATE(__I2C_ST_M_RECV_DATA, __I2C_EVT_NONE);
                if (p_cur_trans->nbytes <= 1) {

                    amhw_mm32_i2c_dat_write(p_hw_i2c, 0x0300);
                } else {
                    amhw_mm32_i2c_dat_write(p_hw_i2c, 0x0100);
                }
                amhw_mm32_i2c_intr_mask_set(p_hw_i2c,
                                           AMHW_MM32_INT_FLAG_RX_FULL);
            } else {

                /* 进入主机发送模式 */
                __I2C_NEXT_STATE(__I2C_ST_M_SEND_DATA, __I2C_EVT_NONE);
                amhw_mm32_i2c_intr_mask_set(p_hw_i2c,
                                           AMHW_MM32_INT_FLAG_TX_EMPTY);
            }

            break;
        }

        /* 当前处于接收数据状态 */
        case __I2C_ST_M_RECV_DATA:
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;
            struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;

            if ((NULL == p_cur_msg) ||(NULL == p_cur_trans)) {
                __softimer_stop(p_dev);
                __i2c_re_init(p_dev);

                return -AM_EINVAL;
            }

            if (p_dev->data_ptr < p_cur_trans->nbytes) {
                (p_cur_trans->p_buf)[p_dev->data_ptr++] = \
                                            amhw_mm32_i2c_dat_read(p_hw_i2c);
                if (p_dev->data_ptr < (p_cur_trans->nbytes - 1)){
                    amhw_mm32_i2c_dat_write(p_hw_i2c, 0x0100);
                    amhw_mm32_i2c_intr_mask_set(p_hw_i2c,
                                               AMHW_MM32_INT_FLAG_RX_FULL);
                } else if (p_dev->data_ptr == (p_cur_trans->nbytes - 1)) {

                    amhw_mm32_i2c_dat_write(p_hw_i2c, 0x0300);
                    amhw_mm32_i2c_intr_mask_set(p_hw_i2c,
                                               AMHW_MM32_INT_FLAG_RX_FULL);
                } else {
                    __I2C_NEXT_STATE(__I2C_ST_M_RECV_DATA,
                                        __I2C_EVT_MST_RX);
                    amhw_mm32_i2c_intr_mask_clear(p_hw_i2c,
                                                 AMHW_MM32_INT_FLAG_RX_FULL);
                }
            } else {  /* 数据接收完毕， 该传输完成 */

                /* 保存从机地址 */
                p_dev->slave_addr =p_dev->p_cur_trans->addr;

                p_cur_msg->done_num++;
                p_dev->p_cur_trans++;

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

            if ((NULL == p_cur_msg) ||(NULL == p_cur_trans)) {
                __softimer_stop(p_dev);
                __i2c_re_init(p_dev);

                return -AM_EINVAL;
            }

            if (p_dev->data_ptr < p_cur_trans->nbytes) {

                if ((p_dev->data_ptr == (p_cur_trans->nbytes - 1)) &&
                     p_dev->trans_state) {

                    amhw_mm32_i2c_dat_write(p_hw_i2c,
                               (p_cur_trans->p_buf)[p_dev->data_ptr++] | 0x200);
                } else {
                    amhw_mm32_i2c_dat_write(p_hw_i2c,
                                       (p_cur_trans->p_buf)[p_dev->data_ptr++]);
                }

                amhw_mm32_i2c_intr_mask_set(p_hw_i2c,
                                           AMHW_MM32_INT_FLAG_TX_EMPTY);
            } else {
                amhw_mm32_i2c_intr_mask_clear(p_hw_i2c,
                                             AMHW_MM32_INT_FLAG_TX_EMPTY);

                /* 保存从机地址 */
                p_dev->slave_addr =p_dev->p_cur_trans->addr;

                p_cur_msg->done_num++;
                p_dev->p_cur_trans++;

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
            return -AM_EINVAL;
        }

        /* 内部没有新事件产生，跳出 */
        if (new_event == __I2C_EVT_NONE) {
            break;
        }
    }

    return AM_OK;
}

/* end of file */
