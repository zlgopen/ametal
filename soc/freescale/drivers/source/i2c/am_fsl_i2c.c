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
 * \brief I2C drivers implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 15-09-19   ari , first implementation.
 * - 1.00 18-01-03   sdq , make somd changes.
 * \endinternal
 */

#include "am_list.h"
#include "am_fsl_i2c.h"
#include "hw/amhw_fsl_i2c.h"
#include "am_clk.h"
#include "am_led.h"

#define __I2C_ST_IDLE             (0x10u)          /* 空闲状态             */
#define __I2C_ST_MSG_START        (0x11u)          /* 消息传输开始状态     */
#define __I2C_ST_TRANS_START      (0x12u)          /* 单个传输开始状态     */
#define __I2C_ST_SEND_SLA_ADDR    (0x13u)          /* 发送从机地址状态     */
#define __I2C_ST_M_SEND_DATA      (0x14u)          /* 发送数据状态         */
#define __I2C_ST_M_RECV_DATA      (0x15u)          /* 接收数据状态         */
#define __I2C_ST_ARBI_LOST        (0x16u)          /* 仲裁丢失状态         */
#define __I2C_ST_SEND_START_SIGNAL (0x17u)         /* 发送起始信号状态     */

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
  Functions declaration
*******************************************************************************/

/* I2C 硬件初始化配置函数   */
static int __i2c_speed_set(am_fsl_i2c_dev_t *p_dev , uint32_t speed);
static int __i2c_hard_init(am_fsl_i2c_dev_t *p_dev);

/* I2C 中断处理函数   */
void       __i2c_irq_handler(void *p_arg);

/* 消息处理 */
static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_msg);
static int __i2c_mst_sm_event(am_fsl_i2c_dev_t *p_arg, uint32_t event);

/**
 * \brief I2C 驱动函数定义
 */
static am_const struct am_i2c_drv_funcs __g_i2c_drv_funcs = {
    __i2c_msg_start
};

/**
 * \brief I2C时钟配置结构体
 */
typedef struct __i2c_div_table {
    uint8_t   icr;         /**< \brief I2C 乘数因子        */
    uint16_t  scl_div;     /**< \brief I2C 分频因子        */
} __i2c_div_table_t;

/******************************************************************************/


/**
 * \brief I2C 波特率设置函数
 */
static int __i2c_speed_set(am_fsl_i2c_dev_t *p_dev , uint32_t speed)
{
	  amhw_fsl_i2c_t  *p_hw_i2c   = p_dev->p_devinfo->p_hw_i2c;
    uint8_t  mult , i , multiplier;
    uint32_t source_clk , rate , abs_error;
    uint32_t min_error = 0xffffffff;
    uint32_t best_mult = 0;
    uint32_t best_icr = 0;
	
    /* I2C 时钟分频因子列表  */
    const __i2c_div_table_t div_table[50] = {
        { 0x00, 20 },   { 0x01, 22 },   { 0x02, 24 },   { 0x03, 26 },
        { 0x04, 28 },   { 0x05, 30 },   { 0x09, 32 },   { 0x06, 34 },
        { 0x0a, 36 },   { 0x07, 40 },   { 0x0c, 44 },   { 0x10, 48 },
        { 0x11, 56 },   { 0x12, 64 },   { 0x0f, 68 },   { 0x13, 72 },
        { 0x14, 80 },   { 0x15, 88 },   { 0x19, 96 },   { 0x16, 104 },
        { 0x1a, 112 },  { 0x17, 128 },  { 0x1c, 144 },  { 0x1d, 160 },
        { 0x1e, 192 },  { 0x22, 224 },  { 0x1f, 240 },  { 0x23, 256 },
        { 0x24, 288 },  { 0x25, 320 },  { 0x26, 384 },  { 0x2a, 448 },
        { 0x27, 480 },  { 0x2b, 512 },  { 0x2c, 576 },  { 0x2d, 640 },
        { 0x2e, 768 },  { 0x32, 896 },  { 0x2f, 960 },  { 0x33, 1024 },
        { 0x34, 1152 }, { 0x35, 1280 }, { 0x36, 1536 }, { 0x3a, 1792 },
        { 0x37, 1920 }, { 0x3b, 2048 }, { 0x3c, 2304 }, { 0x3d, 2560 },
        { 0x3e, 3072 }, { 0x3f, 3840 }
    };	
		
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /*  获取当前I2C运行时钟  */
    source_clk = am_clk_rate_get(p_dev->p_devinfo->clk_id);

    if(speed > 1000000) {
        return  -AM_EINVAL;
    }
    for (mult = 0 ; mult <= 2 ; mult++) {
        multiplier = 1 << mult;
        for (i = 0 ; i < 50 ; i++) {
            rate = source_clk / (multiplier * div_table[i].scl_div);
            abs_error = speed > rate ? speed - rate : rate - speed;
            if (abs_error < min_error) {
                best_mult = mult;
                best_icr  =  div_table[i].icr;
                min_error = abs_error;
            }
            if (min_error == 0) {
                break;
            }
        }
    }

    /* 写入分频因子，设置波特率  */
    amhw_fsl_i2c_clk_div_set(p_hw_i2c , (best_mult << 6)|best_icr);
    return AM_OK;
}

/**
 * \brief I2C 超时设置
 */
static int __i2c_set_timeout (am_fsl_i2c_dev_t *p_dev, uint16_t time_out)
{
    amhw_fsl_i2c_t  *p_hw_i2c = NULL;
    uint32_t clk;
    uint32_t reg_val;

    if (p_dev == NULL) {
        return  -AM_EINVAL;
    }

    p_hw_i2c = p_dev->p_devinfo->p_hw_i2c;

    amhw_fsl_i2c_timeout_clk_sel(p_hw_i2c, AMHW_FSL_I2C_MODULE_CLOCK_64);
    clk = am_clk_rate_get(p_dev->p_devinfo->clk_id);
    reg_val = time_out  * (clk / (64 * 1000));
    if (reg_val > 65535) {
        reg_val = 65535;
    }
    amhw_fsl_i2c_scl_timeout_val_set(p_hw_i2c, reg_val);
    return AM_OK;
}
/**
 * \brief I2C 硬件初始化函数
 */
static int __i2c_hard_init (am_fsl_i2c_dev_t *p_dev)
{
    amhw_fsl_i2c_t  *p_hw_i2c = NULL;

    if (p_dev == NULL) {
        return  -AM_EINVAL;
    }

    p_hw_i2c = p_dev->p_devinfo->p_hw_i2c;

    if (p_hw_i2c == NULL) {
        return  -AM_EINVAL;
    }

    amhw_fsl_i2c_disable(p_hw_i2c);

    __i2c_set_timeout(p_dev, p_dev->p_devinfo->timeout_ms);

    /* I2C 波特率设置  */
    __i2c_speed_set(p_dev, p_dev->p_devinfo->bus_speed);

    /* 滤波设置 */
    amhw_fsl_i2c_flt_fact(p_hw_i2c, 15);

    amhw_fsl_i2c_enable(p_hw_i2c);

    return AM_OK;
}


/**
 * \brief 添加一条 message 到控制器传输列表末尾
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
void __i2c_msg_in (am_fsl_i2c_dev_t *p_dev, struct am_i2c_message *p_msg)
{
    am_list_add_tail((struct am_list_head *)(&p_msg->ctlrdata),
                     &(p_dev->msg_list));
}

/**
 * \brief 从控制器传输列表表头取出一条 message
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
struct am_i2c_message *__i2c_msg_out (am_fsl_i2c_dev_t *p_dev)
{
    if (am_list_empty_careful(&(p_dev->msg_list))) {
        return NULL;
    } else {
        struct am_list_head *p_node = p_dev->msg_list.next;
        am_list_del(p_node);
        return am_list_entry(p_node, struct am_i2c_message, ctlrdata);
    }
}

/**
 * \brief 消息开始
 */
static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_msg)
{
    am_fsl_i2c_dev_t *p_dev  = (am_fsl_i2c_dev_t *)p_drv;

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
        p_dev->stop_signal = AM_FALSE;
        am_int_cpu_unlock(key);
        

        __i2c_mst_sm_event(p_dev, __I2C_ST_MSG_START);

        /* 开启中断（中断要在状态机退出后开启，避免状态机嵌套） */
        amhw_fsl_i2c_int_enable((amhw_fsl_i2c_t *)p_dev->p_devinfo->p_hw_i2c,
                                 AMHW_FSL_I2C_INT_IICIE |
                                 AMHW_FSL_I2C_INT_SHTF2IE |
                                 AMHW_FSL_I2C_INT_SSIE);

        return AM_OK;
    }
}

/**
 * \brief 软件定时器回调函数
 *
 * \param[in] p_arg 指向I2C 设备结构体的指针
 *
 * \return 无
 */
am_local void __softimer_callback (void *p_arg)
{
	am_fsl_i2c_dev_t *p_dev = (am_fsl_i2c_dev_t *)p_arg;

    if (p_arg == NULL) {
        return;
    }
    __i2c_mst_sm_event(p_dev,__I2C_EVT_MST_TIMEOUT);
}

/**
 * \brief I2C 中断函数
 */
void __i2c_irq_handler (void *p_arg)
{
    am_fsl_i2c_dev_t  *p_dev    = (am_fsl_i2c_dev_t *)p_arg;
    amhw_fsl_i2c_t    *p_hw_i2c = p_dev->p_devinfo->p_hw_i2c;

    volatile uint8_t i2c_status  = amhw_fsl_i2c_stat_get(p_hw_i2c);
    volatile uint8_t i2c_timeout = amhw_fsl_i2c_tmout_flag_get(p_hw_i2c);
	
    if (p_arg == NULL) {
        return;
    }


 
    if (i2c_status == 0) {
        return;
    }

    /* 起始信号中断 */
    if (amhw_fsl_i2c_start_is_dec(p_hw_i2c)) {
        amhw_fsl_i2c_start_dec_clr(p_hw_i2c);
        amhw_fsl_i2c_stat_clr(p_hw_i2c , AMHW_FSL_I2C_STAT_IICIF);

        __i2c_mst_sm_event(p_dev, __I2C_EVT_TRANS_LAUNCH);
        return;
    }

    /* 停止信号中断 */
    if (amhw_fsl_i2c_stop_is_dec(p_hw_i2c)) {
        amhw_fsl_i2c_stop_dec_clr(p_hw_i2c);
        amhw_fsl_i2c_stat_clr(p_hw_i2c , AMHW_FSL_I2C_STAT_IICIF);

        __i2c_mst_sm_event(p_dev, __I2C_ST_MSG_START);
        return;
    }

    /* 清除中断标识这行代码应该放在处理下列各个状态之前，否则可能导致状态机死掉 */
    /* 应该放在清除起始标识和停止标识之后，否则中断标识会再次被置位 */
    amhw_fsl_i2c_stat_clr(p_hw_i2c , AMHW_FSL_I2C_STAT_IICIF);

    if (i2c_status & AMHW_FSL_I2C_STAT_IICIF) {
        if (i2c_status & AMHW_FSL_I2C_STAT_ARBL) {
            amhw_fsl_i2c_stat_clr (p_hw_i2c , AMHW_FSL_I2C_STAT_ARBL);
            __i2c_mst_sm_event(p_dev, __I2C_ST_MSG_START);
        } else if (i2c_timeout & AMHW_FSL_I2C_TMOUT_FLAG_SCL_LOW) {
            amhw_fsl_i2c_tmout_flag_clr(p_hw_i2c , AMHW_FSL_I2C_TMOUT_FLAG_SCL_LOW);
            __i2c_mst_sm_event(p_dev, __I2C_EVT_MST_TIMEOUT);
        } else if (i2c_timeout & AMHW_FSL_I2C_TMOUT_FLAG_SDA_LOW) {
            amhw_fsl_i2c_tmout_flag_clr(p_hw_i2c , AMHW_FSL_I2C_TMOUT_FLAG_SDA_LOW);
            __i2c_mst_sm_event(p_dev, __I2C_EVT_MST_TIMEOUT);
        } else if(i2c_status & AMHW_FSL_I2C_STAT_TCF) {
            __i2c_mst_sm_event(p_dev, __I2C_ST_MSG_START);
        }
    }
}

/**
 * \brief I2C 初始化
 */
am_i2c_handle_t am_fsl_i2c_init(am_fsl_i2c_dev_t            *p_dev ,
                                 const am_fsl_i2c_devinfo_t  *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    /* PMU平台初始化 */
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }
    p_dev->i2c_serv.p_funcs = (struct am_i2c_drv_funcs *)&__g_i2c_drv_funcs;
    p_dev->i2c_serv.p_drv   = p_dev;

    p_dev->p_devinfo = p_devinfo;

    p_dev->busy         = AM_FALSE;
    p_dev->p_cur_trans  = NULL;
    p_dev->p_cur_msg    = NULL;
    p_dev->data_ptr     = 0;
    p_dev->state        = __I2C_ST_IDLE;

    am_list_head_init(&(p_dev->msg_list));

    __i2c_hard_init(p_dev);

    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum, __i2c_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    if (am_softimer_init(&p_dev->softimer,
                          __softimer_callback,
                          p_dev) != AM_OK) {
        return NULL;
    }

    return &(p_dev->i2c_serv);
}

/**
 * \brief I2C 解初始化
 */
void am_fsl_i2c_deinit (am_i2c_handle_t handle)
{

    am_fsl_i2c_dev_t *p_dev = (am_fsl_i2c_dev_t *)handle;

    amhw_fsl_i2c_t *p_hw_i2c  = p_dev->p_devinfo->p_hw_i2c;

    p_dev->i2c_serv.p_funcs = NULL;
    p_dev->i2c_serv.p_drv   = NULL;
	
    if (handle == NULL) {
        return;
    }
    amhw_fsl_i2c_disable(p_hw_i2c);
    am_int_disable(p_dev->p_devinfo->inum);

    amhw_fsl_i2c_disable(p_hw_i2c);

    am_int_disconnect(p_dev->p_devinfo->inum, __i2c_irq_handler, (void *)p_dev);
    am_int_disable(p_dev->p_devinfo->inum);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/*  状态机内部状态切换 */
#define __I2C_NEXT_STATE(s, e) \
    do {                     \
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
static int __i2c_mst_sm_event (am_fsl_i2c_dev_t *p_dev, uint32_t event)
{
    volatile uint32_t new_event = __I2C_EVT_NONE;
    amhw_fsl_i2c_t *p_hw_i2c = NULL;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }


    p_hw_i2c  = (amhw_fsl_i2c_t *)p_dev->p_devinfo->p_hw_i2c;

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

            /* 关闭软件定时器 */
            am_softimer_stop(&p_dev->softimer);

            key = am_int_cpu_lock();
            p_cur_msg        = __i2c_msg_out(p_dev);
            p_dev->p_cur_msg = p_cur_msg;

            if (p_cur_msg) {
                p_cur_msg->status = -AM_EINPROGRESS;

                am_int_cpu_unlock(key);

                p_cur_msg->done_num = 0;
                p_dev->p_cur_trans  = p_cur_msg->p_transfers;
                p_dev->data_ptr     = 0;

                __I2C_NEXT_STATE(__I2C_ST_TRANS_START, __I2C_EVT_TRANS_LAUNCH);

                /* 直接进入下一个状态，开始一个传输，此处无需break */
                event     = new_event;
                new_event = __I2C_EVT_NONE;
            } else {

                /* 关闭中断 */
                amhw_fsl_i2c_int_disable(p_hw_i2c,
                                         AMHW_FSL_I2C_INT_IICIE  |
                                         AMHW_FSL_I2C_INT_SHTF2IE |
                                         AMHW_FSL_I2C_INT_SSIE);

                p_dev->busy = AM_FALSE;

                am_int_cpu_unlock(key);

                __I2C_NEXT_STATE(__I2C_ST_IDLE, __I2C_EVT_NONE);

                break;
            }
        }
        /* no break */

        case __I2C_ST_TRANS_START :
        {
            struct am_i2c_message *p_cur_msg = p_dev->p_cur_msg;
            struct am_i2c_transfer *p_cur_trans;
            /* 当前消息传输完成 */
            if (__I2C_TRANS_EMPTY(p_dev)) {

                /* 消息正在处理中 */
                if (p_cur_msg->status == -AM_EINPROGRESS) {
                    p_cur_msg->status = AM_OK;
                }

                amhw_fsl_i2c_stop_signal_send(p_hw_i2c);
                p_dev->stop_signal = AM_TRUE;

                p_cur_trans = p_cur_msg->p_transfers + (p_cur_msg->trans_num - 1);

                /* 接收数据时 最后一个数据在停止信号之后接收  */
                if ((p_dev->data_ptr < p_cur_trans->nbytes) &&
                    (p_cur_trans->flags & AM_I2C_M_RD)) {
                    p_cur_trans->p_buf[p_dev->data_ptr++] =  amhw_fsl_i2c_data_read(p_hw_i2c);
                    p_dev->dummy_rx_flg = 0;
                }

                /* 回调消息完成函数  */
                if (p_cur_msg->pfn_complete != NULL) {
                    p_cur_msg->pfn_complete(p_cur_msg->p_arg);
                }

                /** 开启软件超时定时器 */
                am_softimer_start(&p_dev->softimer,20);

                __I2C_NEXT_STATE(__I2C_ST_MSG_START, __I2C_EVT_NONE);
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

                        /* 继续接受数据 */
                        __I2C_NEXT_STATE(__I2C_ST_M_RECV_DATA, __I2C_EVT_TRANS_LAUNCH);
                    } else {

                        /* 继续发送数据 */
                        __I2C_NEXT_STATE(__I2C_ST_M_SEND_DATA, __I2C_EVT_TRANS_LAUNCH);
                    }
                } else {

                    /* 下一步操作是发送起始信号   */
                    __I2C_NEXT_STATE(__I2C_ST_SEND_START_SIGNAL, __I2C_EVT_TRANS_LAUNCH);
                }
            }
            break;
        }

        case __I2C_ST_SEND_SLA_ADDR :    /* 发送从机地址 */
        {
            struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;
            uint8_t dev_addr = (p_cur_trans->addr) << 1;

            /* 关闭软件定时器 */
            am_softimer_stop(&p_dev->softimer);

            if (p_cur_trans->flags & AM_I2C_M_RD) {
                dev_addr |= 0x1;            /* 读操作 */
                /* 下一步是接收数据 */
                __I2C_NEXT_STATE(__I2C_ST_M_RECV_DATA, __I2C_EVT_NONE);
            } else {
                /* 进入主机发送模式  */
                __I2C_NEXT_STATE(__I2C_ST_M_SEND_DATA, __I2C_EVT_NONE);
            }

            /** 开启软件超时定时器 */
            am_softimer_start(&p_dev->softimer,20);

            /* 写入从设备地址   */
            amhw_fsl_i2c_data_write(p_hw_i2c , dev_addr);
        }
        break;

        case __I2C_ST_SEND_START_SIGNAL :    /* 发送起始信号 */
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

            /** 开启软件超时定时器 */
            am_softimer_start(&p_dev->softimer,20);

            amhw_fsl_i2c_transmode_set(p_hw_i2c, AMHW_FSL_I2C_TRANSMODE_SEND);
            /* 发送启动信号  */
            if (p_cur_trans->flags & AM_I2C_M_RD) {

                dev_addr |= 0x1;            /* 读操作 */
                if((p_cur_trans != p_cur_msg->p_transfers) && (p_dev->stop_signal == AM_FALSE)) {
                    amhw_fsl_i2c_restart_signal_send(p_hw_i2c);
                } else {
                    amhw_fsl_i2c_start_signal_send(p_hw_i2c);
                    p_dev->stop_signal = AM_FALSE;
                }
                /* 开启虚拟读取 */
                p_dev->dummy_rx_flg = 1;

            } else {
                dev_addr &= ~0x1;            /* 写操作 */

                amhw_fsl_i2c_start_signal_send(p_hw_i2c);
                p_dev->stop_signal = AM_FALSE;
            }

            /* 下一状态，发送从机地址 */
            __I2C_NEXT_STATE(__I2C_ST_SEND_SLA_ADDR, __I2C_EVT_NONE);
        }
        break;

        /* 当前处于接收数据状态 */
        case __I2C_ST_M_RECV_DATA:
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;
            struct am_i2c_transfer *p_cur_trans = p_dev->p_cur_trans;

            /* 关闭软件定时器 */
            am_softimer_stop(&p_dev->softimer);

            if (((amhw_fsl_i2c_stat_get(p_hw_i2c) & AMHW_FSL_I2C_STAT_RXAK)) &&
                (p_dev->data_ptr < p_cur_trans->nbytes - 1)) {

                event = __I2C_EVT_MST_DATA_NO_ACK;

            }
            /* 接收数据，由主机应答，因此只可能产生地址无应答 */
            if ((event == __I2C_EVT_MST_ADDR_NO_ACK) ||
                (event == __I2C_EVT_MST_DATA_NO_ACK)) {

                if (!(p_cur_trans->flags & AM_I2C_M_IGNORE_NAK)) {

                    p_cur_msg->status = -AM_ENODEV;

                    /* 忽略剩下的transfer */
                    p_dev->p_cur_trans = p_cur_msg->p_transfers +
                                         p_cur_msg->trans_num;

                    __I2C_NEXT_STATE(__I2C_ST_TRANS_START, __I2C_EVT_TRANS_LAUNCH);
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

            amhw_fsl_i2c_transmode_set(p_hw_i2c, AMHW_FSL_I2C_TRANSMODE_RECV);

            /* 第一次读取为写入的设备地址 做一次假读取  */

            if (1 == p_dev->dummy_rx_flg) {
                if (p_cur_trans->nbytes > 1) {
                     amhw_fsl_i2c_ack_respond(p_hw_i2c);
                 } else {
                     amhw_fsl_i2c_nak_respond(p_hw_i2c);
                 }
                /* 假读*/
                (void)amhw_fsl_i2c_data_read(p_hw_i2c);
                p_dev->dummy_rx_flg = 0;
                break;
            }

            if (p_dev->data_ptr < p_cur_trans->nbytes - 1) {
                if ((p_cur_trans->nbytes > 1) &&
                    (p_dev->data_ptr  == p_cur_trans->nbytes - 2)) {
                     amhw_fsl_i2c_nak_respond(p_hw_i2c);
                }

                /* 继续接收数据，状态保持不变 */
                (p_cur_trans->p_buf)[p_dev->data_ptr++] = \
                                              amhw_fsl_i2c_data_read(p_hw_i2c);
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

            /* 关闭软件定时器 */
            am_softimer_stop(&p_dev->softimer);

            amhw_fsl_i2c_transmode_set(p_hw_i2c, AMHW_FSL_I2C_TRANSMODE_SEND);

            /* 无应答 */
            if (amhw_fsl_i2c_stat_get(p_hw_i2c) & AMHW_FSL_I2C_STAT_RXAK) {
               if (p_cur_trans == p_cur_msg->p_transfers) {
                   event = __I2C_EVT_MST_ADDR_NO_ACK;     /* 第一条传输 为地址不应答 */

               } else {
                   event = __I2C_EVT_MST_DATA_NO_ACK;
               }
            }

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
                amhw_fsl_i2c_data_write(p_hw_i2c,
                                     (p_cur_trans->p_buf)[p_dev->data_ptr++]);
            } else {

                p_cur_msg->done_num++;
                p_dev->p_cur_trans++;

                /* 本传输完毕，进入下一传输 */
                __I2C_NEXT_STATE(__I2C_ST_TRANS_START, __I2C_EVT_TRANS_LAUNCH);
            }

            break;
        }

        case __I2C_ST_ARBI_LOST:
        {
            struct am_i2c_message  *p_cur_msg   = p_dev->p_cur_msg;

            amhw_fsl_i2c_disable(p_hw_i2c);
            amhw_fsl_i2c_enable(p_hw_i2c);

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
