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
 * - 1.00 19-09-19
 * \endinternal
 */

/*******************************************************************************
  头文件
*******************************************************************************/
#include "ametal.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_zlg_i2c.h"
#include "hw/amhw_zlg118_i2c.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define __I2C_ST_IDLE             (0x10u)          /* 空闲状态 */
#define __I2C_ST_MSG_START        (0x11u)          /* 消息传输开始状态 */
#define __I2C_EVT_MSG_LAUNCH      (0xFEu)          /* 开始处理一个新的消息 */

/*******************************************************************************
  函数声明
*******************************************************************************/

/** \brief I2C 硬件初始化 */
static int __i2c_hard_init (am_zlg_i2c_dev_t *p_dev);

/** \brief I2C 中断处理函数 */
static void __i2c_irq_handler (void *p_arg);

/** \brief I2C 状态机函数 */
static int __i2c_mst_sm_event (am_zlg_i2c_dev_t *p_dev, uint32_t event);

/** \brief I2C 消息处理函数 */
static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_trans);

/** \brief 从控制器传输列表表头取出一条 message */
am_static_inline
struct am_i2c_message *__i2c_msg_out (am_zlg_i2c_dev_t *p_dev);

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
am_local int __i2c_hard_init (am_zlg_i2c_dev_t *p_dev)
{
    amhw_zlg118_i2c_t *p_hw_i2c = NULL;
    uint16_t count = ((am_clk_rate_get (p_dev->p_devinfo->clk_num) / \
                     p_dev->p_devinfo->speed) >> 3) - 1;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_zlg118_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c->i2c_cr = 0;

    if (p_dev->p_devinfo->speed < 100000) {
        amhw_zlg118_i2c_cr_clear(p_hw_i2c, AMHW_ZLG118_I2C_HIGH_SPEED);
    } else {
        amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_HIGH_SPEED);
    }
    amhw_zlg118_i2c_tm_set(p_hw_i2c, count);
    amhw_zlg118_i2c_tm_enable(p_hw_i2c);

    amhw_zlg118_i2c_enable(p_hw_i2c);

    return AM_OK;
}

/** \brief I2C 硬件重新初始化 */
am_local int __i2c_hard_re_init (am_zlg_i2c_dev_t *p_dev)
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
    am_zlg_i2c_dev_t *p_dev = (am_zlg_i2c_dev_t *)p_arg;
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
am_local void __softimer_start (am_zlg_i2c_dev_t *p_dev)
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
am_local void __softimer_stop (am_zlg_i2c_dev_t *p_dev)
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
void __i2c_msg_in (am_zlg_i2c_dev_t *p_dev, struct am_i2c_message *p_msg)
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
struct am_i2c_message *__i2c_msg_out (am_zlg_i2c_dev_t *p_dev)
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
    am_zlg_i2c_dev_t *p_dev = (am_zlg_i2c_dev_t *)p_arg;

    if (p_arg == NULL) {
        return;
    }

    __softimer_stop(p_dev);
    __i2c_re_init(p_arg);
    p_dev->busy = AM_FALSE;
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
    am_zlg_i2c_dev_t *p_dev    = NULL;
    amhw_zlg118_i2c_t   *p_hw_i2c = NULL;

    if (p_arg == NULL) {
        return;
    }

    p_dev    = (am_zlg_i2c_dev_t *)p_arg;
    p_hw_i2c = (amhw_zlg118_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    amhw_zlg118_i2c_status_get(p_hw_i2c);
}

/******************************************************************************/

static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_msg)
{
    am_zlg_i2c_dev_t *p_dev    = (am_zlg_i2c_dev_t *)p_drv;
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

        return __i2c_mst_sm_event(p_dev, __I2C_EVT_MSG_LAUNCH);
    }
}

/**
 * \brief I2C初始化
 */
am_i2c_handle_t am_zlg_i2c_init (am_zlg_i2c_dev_t           *p_dev,
                                 const am_zlg_i2c_devinfo_t *p_devinfo)
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

//    /* 连接中断 */
//    am_int_connect(p_dev->p_devinfo->inum, __i2c_irq_handler, (void *)p_dev);
//    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->i2c_serv);
}

/**
 * \brief I2C解初始化
 */
void am_zlg_i2c_deinit (am_i2c_handle_t handle)
{
    amhw_zlg118_i2c_t   *p_hw_i2c = NULL;
    am_zlg_i2c_dev_t *p_dev    = NULL;

    if (NULL == handle) {
        return ;
    }

    p_dev    = (am_zlg_i2c_dev_t *)handle;
    p_hw_i2c = (amhw_zlg118_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    p_dev->i2c_serv.p_funcs = NULL;
    p_dev->i2c_serv.p_drv   = NULL;

    amhw_zlg118_i2c_disable (p_hw_i2c);

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

/**
 * \brief I2C 状态机函数
 */
static int __i2c_mst_sm_event (am_zlg_i2c_dev_t *p_dev, uint32_t event)
{
    int                     key;
    uint8_t                 state;
    amhw_zlg118_i2c_t      *p_hw_i2c    = NULL;
    struct am_i2c_message  *p_cur_msg   = NULL;
    struct am_i2c_transfer *p_cur_trans = NULL;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    key = am_int_cpu_lock();

    p_cur_msg        = __i2c_msg_out(p_dev);
    p_dev->p_cur_msg = p_cur_msg;

    if (p_cur_msg) {
        p_cur_msg->status = -AM_EINPROGRESS;
    } else {
        p_dev->busy = AM_FALSE;
    }

    am_int_cpu_unlock(key);

    p_dev->p_cur_trans  = p_cur_msg->p_transfers;
    p_cur_trans = p_dev->p_cur_trans;
    p_hw_i2c = (amhw_zlg118_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    __softimer_start(p_dev);

    p_dev->data_ptr = 0;

    /* 发送起始条件 */
    amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_START_ENABLE);

    while(1) {
        /* 获取中断标志 */
        while(0 == amhw_zlg118_i2c_irq_get(p_hw_i2c)) {
            if(p_dev->busy == AM_FALSE) {
                p_dev->state = __I2C_ST_IDLE;
                amhw_zlg118_i2c_cr_clear(p_hw_i2c,
                                         AMHW_ZLG118_I2C_START_ENABLE);
                break;
            } else {
                p_dev->state = __I2C_ST_MSG_START;
            }
        }

        /* 获取I2C状态 */
        state = amhw_zlg118_i2c_status_get(p_hw_i2c);

        if(p_cur_trans[p_cur_msg->done_num].flags & AM_I2C_M_RD) {  /* 读操作 */

            /* 判断i2c状态 */
            switch (state) {
            case 0x08: /* 已发送起始条件 */
            case 0x10: /* 已发送重复起始条件 */

                /* 清除起始条件 */
                amhw_zlg118_i2c_cr_clear(p_hw_i2c,
                                         AMHW_ZLG118_I2C_START_ENABLE);

                /* 传输从机地址和读命令 */
                amhw_zlg118_i2c_dat_write(p_hw_i2c,
                                          ((p_cur_trans->addr << 1) | 0x1));
                break;

            case 0x18: /* 已发送SLA+W，已接收ACK */

                /* 传输目标地址 */
                amhw_zlg118_i2c_dat_write(p_hw_i2c,
                        p_cur_trans[p_cur_msg->done_num - 1].\
                                    p_buf[p_dev->data_ptr]);
                break;

            case 0x28: /* 已发送数据，已接收ACK */

                /* 发送重复起始条件 */
                amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_START_ENABLE);
                break;

            case 0x40: /* 已发送SLA +R,已接收ACK */

                /* 使能应答 */
                if(p_cur_trans[p_cur_msg->done_num].nbytes > 1) {
                    amhw_zlg118_i2c_cr_set(p_hw_i2c,
                                           AMHW_ZLG118_I2C_REPLY_ENABLE);
                }
                break;

            case 0x48: /* 已发送SLA +R,已接收非ACK */
                amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_STOP_ENABLE);
                amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_START_ENABLE);
                break;

            case 0x50:/* 已接收数据字节，ACK已返回 */

                /* 接收数据 */
                p_cur_trans[p_cur_msg->done_num].p_buf[p_dev->data_ptr++] = \
                    amhw_zlg118_i2c_dat_read(p_hw_i2c);

                if(p_dev->data_ptr ==
                        p_cur_trans[p_cur_msg->done_num].nbytes - 1) {

                    /* 禁能应答 */
                    amhw_zlg118_i2c_cr_clear(p_hw_i2c,
                                             AMHW_ZLG118_I2C_REPLY_ENABLE);
                }
                break;

            case 0x58:/* 已接收数据字节，非ACK已返回 */

                /* 接收最后一个数据 */
                p_cur_trans[p_cur_msg->done_num].p_buf[p_dev->data_ptr++] = \
                    amhw_zlg118_i2c_dat_read(p_hw_i2c);
                p_dev->state = __I2C_ST_IDLE;
                break;

            case 0x38: /* 在SLA+ 读写或写数据字节时丢失仲裁 */
            default:
                p_dev->state = __I2C_ST_IDLE;
                break;
            }
        } else {    /* 写操作 */

            /* 判断i2c状态 */
            switch (state) {
            case 0x08: /* 已发送起始条件 */

                /* 清除起始条件 */
                amhw_zlg118_i2c_cr_clear(p_hw_i2c,
                                         AMHW_ZLG118_I2C_START_ENABLE);

                /* 传输从机地址和写命令 */
                amhw_zlg118_i2c_dat_write(p_hw_i2c, (p_cur_trans->addr << 1));
                break;

            case 0x18: /* 已发送SLA+W，已接收ACK */
            case 0x28: /* 已发送数据，已接收ACK */
                if(p_cur_msg->done_num <= p_cur_msg->trans_num) {

                /* 传输数据 */
                amhw_zlg118_i2c_dat_write(p_hw_i2c,
                                          p_cur_trans[p_cur_msg->done_num].\
                                          p_buf[p_dev->data_ptr++]);
                }
                break;

            case 0x20: /* 已发送SLA+W，已接收非ACK */
            case 0x30: /* 已发送数据 */
            case 0x38: /* 在SLA+ 读写或写数据字节时丢失仲裁 */
            default:
                p_dev->state = __I2C_ST_IDLE;
                break;
            }
        }

        /* 判断消息是否传输完成 */
        if(p_dev->data_ptr >= p_cur_trans[p_cur_msg->done_num].nbytes) {
            p_cur_msg->done_num++;
            p_dev->data_ptr = 0;
        }

        if(p_dev->state == __I2C_ST_IDLE ||
           p_cur_msg->done_num > p_cur_msg->trans_num) {
            /* 设置停止停止标志 */
            amhw_zlg118_i2c_cr_set(p_hw_i2c, AMHW_ZLG118_I2C_STOP_ENABLE);

            /* 清除中断标志位 */
            amhw_zlg118_i2c_cr_clear(p_hw_i2c, AMHW_ZLG118_I2C_INT_FLAG);
            break;
        } else if(p_cur_msg->done_num == p_cur_msg->trans_num) {
            p_cur_msg->done_num++;
        }

        /* 清除中断标志位 */
        amhw_zlg118_i2c_cr_clear(p_hw_i2c, AMHW_ZLG118_I2C_INT_FLAG);
    }

    p_dev->busy = AM_FALSE;
    p_dev->state = __I2C_ST_IDLE;
    p_cur_msg->status = AM_OK;

    __softimer_stop(p_dev);

    /* 回调消息完成函数 */
    if (p_cur_msg->pfn_complete != NULL) {
        p_cur_msg->pfn_complete(p_dev->p_cur_msg->p_arg);
    }

    return AM_OK;
}

/* end of file */
