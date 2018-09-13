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
 * \brief ZLG600底层驱动程序
 *
 * \internal
 * \par Modification history
 * - 1.01 16-08-03  sky, modified(add card auto detect).
 * - 1.00 15-09-03  tee, first implementation.
 * \endinternal
 */

#include "am_zlg600.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_delay.h"
#include <string.h>

/*******************************************************************************
  类型定义
*******************************************************************************/
typedef struct am_zlg600_info_buf_desc {
    uint8_t   *p_buf;
    uint16_t   len;
} am_zlg600_info_buf_desc_t;

/**
 * \brief 定义旧帧结构体
 */
typedef struct am_zlg600_frame_old {
    uint8_t                    frame_len;   /** \brief 帧长                   */
    uint8_t                    cmd_type;    /** \brief 命令类型               */
    uint8_t                    cmd_status;  /** \brief 命令/状态              */
    uint8_t                    info_len;    /** \brief 信息长度               */
    am_zlg600_info_buf_desc_t *p_info_desc; /** \brief 信息描述结构体指针     */
    uint8_t                    buf_num;     /** \brief 信息缓冲区的个数       */
    uint8_t                    bcc;         /** \brief 校验和                 */
    uint8_t                    ext;         /** \brief 帧结束符               */
} am_zlg600_frame_old_t;

/**
 * \brief 定义新帧结构体
 */
typedef struct am_zlg600_frame_new {
    uint8_t                    local_addr;  /** \brief 从机地址               */
    uint8_t                    slot_index;  /** \brief 卡槽索引               */
    uint8_t                    smc_seq;     /** \brief 安全报文/包号          */
    uint8_t                    cmd_class;   /** \brief 命令类型               */
    uint16_t                   cmd_status;  /** \brief 命令/状态码            */
    uint16_t                   info_len;    /** \brief 信息长度               */
    am_zlg600_info_buf_desc_t *p_info_desc; /** \brief 信息描述结构体指针     */
    uint8_t                    buf_num;     /** \brief 信息缓冲区的个数       */
    uint16_t                   check_sum;   /** \brief 校验和                 */
} am_zlg600_frame_new_t;

/*******************************************************************************
  串口发送和接收函数
*******************************************************************************/

/* 发送一个旧帧命令 */
static int __zlg600_uart_old_frame_cmd_send (void                   *p_cookie,
                                             am_zlg600_frame_old_t  *p_frame)
{
    am_zlg600_uart_dev_t *p_dev = (am_zlg600_uart_dev_t *)p_cookie;
    
    int i;
    uint8_t *p_buf;
    uint8_t  info_len;

    /* 发送帧长 + 命令类型 + 命令 + 信息长度 （共计4个字节）*/
    am_uart_rngbuf_send(p_dev->uart_ringbuf_handle, (uint8_t *)p_frame, 4);

    /* 发送各个信息缓冲区中的信息 */
    for (i = 0; i < p_frame->buf_num; i++) {
        p_buf     = p_frame->p_info_desc[i].p_buf;
        info_len  = p_frame->p_info_desc[i].len;
        if (info_len != 0) {                        /* 存在信息，则发送 */
            am_uart_rngbuf_send(p_dev->uart_ringbuf_handle, p_buf, info_len);
        }
    }

    /* 发送校验和   */
    am_uart_rngbuf_send(p_dev->uart_ringbuf_handle, &(p_frame->bcc), 1);

    /* 发送帧结束符 */
    am_uart_rngbuf_send(p_dev->uart_ringbuf_handle, &(p_frame->ext), 1);

    return AM_OK;
}

/* 接收一个旧回应帧 */
static int __zlg600_uart_old_frame_respond_recv (void                  *p_cookie,
                                                 am_zlg600_frame_old_t *p_frame)
{
    am_zlg600_uart_dev_t *p_dev = (am_zlg600_uart_dev_t *)p_cookie;
    
    int      len,i,j;
    uint8_t  info_len;
    uint8_t *p_buf;
    uint8_t  bcc = 0;             /* 接收时，可能存在数据丢弃，直接边读边校验 */

    uint8_t  remain_len;
    uint8_t  discard_data;        /* 用于将接收的无用数据丢弃 */

    /* 接收帧长 + 命令类型 + 状态 + 信息长度 （共计4个字节）*/
    len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle,
                                 (uint8_t *)p_frame,
                                 4);
    if (len != 4) {
        return -AM_ETIME;
    }
    
    bcc ^= p_frame->frame_len;
    bcc ^= p_frame->cmd_type;
    bcc ^= p_frame->cmd_status;
    bcc ^= p_frame->info_len;

    remain_len = p_frame->info_len;      /* 剩余待接收信息的字符总数 */

    /* 接收各个信息缓冲区中的数据 */
    for (i = 0; i < p_frame->buf_num; i++) {
        p_buf     = p_frame->p_info_desc[i].p_buf;
        info_len  = p_frame->p_info_desc[i].len;

        /* 读取的字节数不能大于剩余字节 */
        info_len = info_len > remain_len ? remain_len : info_len;

        remain_len -= info_len;

        p_frame->p_info_desc[i].len = info_len;

        if (info_len != 0) {                       /* 存在信息，则接收 */

            if (p_buf == NULL) {                   /* 接收的数据丢弃   */
                while (info_len != 0) {
                    len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle,
                                                 &discard_data,
                                                 1);
                    if (len != 1) {
                        return -AM_ETIME;
                    }
                    info_len--;
                    bcc ^= discard_data;
                }
            } else {
                len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle,
                                             p_buf,
                                             info_len);
                if (len != info_len) {
                    return -AM_ETIME;
                }

                for (j = 0; j < info_len; j++) {
                    bcc ^= p_buf[j];
                }
            }
        }
    }

    /* 未接收的信息丢弃 */
    while (remain_len != 0) {
        len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle,
                                     &discard_data,
                                     1);
        if (len != 1) {
            return -AM_ETIME;
        }
        remain_len--;
        bcc ^= discard_data;
    }

    /* 接收校验和 */
    len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle, &(p_frame->bcc), 1);
    if (len != 1) {
        return -AM_ETIME;
    }

    /* 接收帧结束符 */
    len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle, &(p_frame->ext), 1);
    if (len != 1) {
        return -AM_ETIME;
    }

    bcc = ~bcc;

    if (bcc != p_frame->bcc) {
        return -AM_EIO;
    }
    return AM_OK;
}

/******************************************************************************/

/* 发送一个新命令帧 */
static int __zlg600_uart_new_frame_cmd_send (void                  *p_cookie,
                                             am_zlg600_frame_new_t *p_frame)
{
    am_zlg600_uart_dev_t *p_dev = (am_zlg600_uart_dev_t *)p_cookie;

    int i;
    uint8_t   *p_buf;
    uint16_t  info_len;

    /* 发送地址(1) + 卡槽索引(1) + 安全报文/包号(1) + 命令类型 （共计4个字节）*/
    am_uart_rngbuf_send(p_dev->uart_ringbuf_handle, (uint8_t *)p_frame, 4);

    /* 发送命令/状态码(2) + 信息长度(2)（共计4个字节）                        */
    am_uart_rngbuf_send(p_dev->uart_ringbuf_handle,
                        (uint8_t *)(&p_frame->cmd_status),
                        4);

    /* 发送各缓冲区中的信息 */
    for (i = 0; i < p_frame->buf_num; i++) {
        p_buf     = p_frame->p_info_desc[i].p_buf;
        info_len  = p_frame->p_info_desc[i].len;
        if (info_len != 0) {
            am_uart_rngbuf_send(p_dev->uart_ringbuf_handle, p_buf, info_len);
        }
    }

    /* 发送校验和,2字节 */
    am_uart_rngbuf_send(p_dev->uart_ringbuf_handle,
                        (uint8_t *)&(p_frame->check_sum),
                        2);

    return AM_OK;
}

/* 接收一个新回应帧 */
static int __zlg600_uart_new_frame_respond_recv (void                  *p_cookie,
                                                 am_zlg600_frame_new_t *p_frame)
{
    am_zlg600_uart_dev_t *p_dev = (am_zlg600_uart_dev_t *)p_cookie;

    int       i, j, len;
    uint8_t  *p_buf;
    uint16_t  info_len;
    uint16_t  remain_len;
    uint8_t   discard_data;
    uint16_t  sum = 0;         /* 接收时，可能存在数据丢弃，直接边读边校验 */



    /* 接收地址(1) + 卡槽索引(1) + 安全报文/包号(1) + 命令类型 （共计4个字节）*/
    len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle,
                                 (uint8_t *)p_frame,
                                 4);
    if (len != 4) {
        return -AM_ETIME;
    }

    sum += p_frame->local_addr;
    sum += p_frame->slot_index;
    sum += p_frame->smc_seq;
    sum += p_frame->cmd_class;


    /* 接收命令/状态码(2) + 信息长度(2)（共计4个字节） */
    len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle,
                                 (uint8_t *)(&p_frame->cmd_status),
                                 4);
    if (len != 4) {
        return -AM_ETIME;
    }

    sum += p_frame->cmd_status & 0xFF;            /* 低字节 */
    sum += p_frame->cmd_status >> 8;              /* 高字节 */

    sum += p_frame->info_len & 0xFF;              /* 低字节 */
    sum += p_frame->info_len >> 8;                /* 低字节 */

    remain_len = p_frame->info_len;               /* 剩余待接收信息的字符总数 */

    for (i = 0; i < p_frame->buf_num; i++) {
        p_buf     = p_frame->p_info_desc[i].p_buf;
        info_len  = p_frame->p_info_desc[i].len;

        /* 读取的字节数不能大于剩余字节 */
        info_len = info_len > remain_len ? remain_len : info_len;

        remain_len -= info_len;

        p_frame->p_info_desc[i].len = info_len;   /* 本缓冲区实际读取字符长度 */

        if (info_len != 0) {                      /* 存在信息，则接收 */

            if (p_buf == NULL) {                  /* 接收的数据丢弃   */
                while (info_len != 0) {
                    len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle,
                                                 &discard_data,
                                                 1);
                    if (len != 1) {
                        return -AM_ETIME;
                    }
                    info_len--;
                    sum += discard_data;
                }
            } else {
                len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle,
                                             p_buf,
                                             info_len);
                if (len != info_len) {
                    return -AM_ETIME;
                }

                for (j = 0; j < info_len; j++) {
                    sum += p_buf[j];
                }
            }
        }
    }

    while (remain_len != 0) {
        len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle,
                                     &discard_data,
                                     1);
        if (len != 1) {
            return -AM_ETIME;
        }
        remain_len--;
        sum += discard_data;
    }
    sum = ~sum;

    /* 接收校验和 */
    len = am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle,
                                 (uint8_t *)&(p_frame->check_sum),
                                 2);

    if (len != 2) {
        return -AM_ETIME;
    }

    if (sum != p_frame->check_sum) {
        return -AM_EIO;
    }

    return AM_OK;
}

/*******************************************************************************
  I2C发送和接收函数
*******************************************************************************/

static void __pin_int_callback (void *p_arg)
{
    am_zlg600_i2c_dev_t *p_dev = (am_zlg600_i2c_dev_t *)p_arg;
    am_zlg600_serv_t    *p_serv = &p_dev->zlg600_serv;
    
//    /* 如果未启动自动检测 */
//    if (p_serv->auto_detect == 0) {
//        am_gpio_trigger_off(p_dev->pin);
//    }

    am_gpio_trigger_off(p_dev->pin);

    if ((p_serv->work_mode & 0xF0) == AM_ZLG600_MODE_MASTER) {
        if (p_serv->pfn_callback != NULL) {
            p_serv->pfn_callback(p_serv->p_arg);
        }
    } else {
        am_wait_done(&p_dev->wait);
    }
}

static int __wait_can_recv (am_zlg600_i2c_dev_t *p_dev)
{
    int ret;

    /* 已经是低电平 */
    if (am_gpio_get(p_dev->pin) == 0) {
        return AM_OK;
    }
    
    am_gpio_trigger_on(p_dev->pin);
    
    /* 超时时间内没有响应，超时退出 */
    ret = am_wait_on_timeout(&p_dev->wait, p_dev->zlg600_serv.timeout_ms);

    /* 如果超时 */
    if (-AM_ETIME == ret) {
        am_gpio_trigger_off(p_dev->pin);
    }

    return ret;
}

/* I2C数据传输完成回调函数 */
static void __i2c_trans_data_complete (void *p_arg)
{
    am_wait_t *p_wait = (am_wait_t *)p_arg;
    am_wait_done(p_wait);
}

/* 完成一次I2C数据传输 */
static int __i2c_trans_data (am_zlg600_i2c_dev_t *p_dev,
                             uint16_t             flags,
                             uint8_t             *p_buf,
                             uint32_t             nbytes)
{
    am_i2c_message_t msg;
    uint8_t          trans_addr = 0;
    am_wait_t        wait;

    am_wait_init(&wait);

    am_i2c_mktrans(&p_dev->p_trans[trans_addr++],
                   (p_dev->zlg600_serv.cur_addr),// >> 1,
                   flags,
                   p_buf,
                   nbytes);

    am_i2c_mkmsg(&msg,
                 p_dev->p_trans,
                 trans_addr,
                 __i2c_trans_data_complete,
                 (void *)&wait);

    am_i2c_msg_start(p_dev->i2c_handle, &msg);

    am_wait_on(&wait);
 
    return msg.status;
}

/* 发送一个旧帧命令 */
static int __zlg600_i2c_old_frame_cmd_send (void                   *p_cookie,
                                            am_zlg600_frame_old_t  *p_frame)
{
    am_zlg600_i2c_dev_t *p_dev = (am_zlg600_i2c_dev_t *)p_cookie;
    am_i2c_message_t     msg;
    uint8_t              trans_addr = 0;
    uint8_t              i;
    uint8_t             *p_buf;
    uint8_t              info_len;
    uint8_t              temp[2];
    am_wait_t            wait;

    am_wait_init(&wait);

    /* 发送帧前4个字节 */
    am_i2c_mktrans(&p_dev->p_trans[trans_addr++],
                   (p_dev->zlg600_serv.cur_addr),// >> 1,
                   AM_I2C_M_7BIT | AM_I2C_M_WR,
                   (uint8_t *)p_frame,
                   4);

    /* 发送各个信息缓冲区中的信息 */
    for (i = 0; i < p_frame->buf_num; i++) {
        p_buf     = p_frame->p_info_desc[i].p_buf;
        info_len  = p_frame->p_info_desc[i].len;
        if (info_len != 0) {                        /* 存在信息，则发送 */
            /* 中间的数据发送均无需启动信号 */
            am_i2c_mktrans(&p_dev->p_trans[trans_addr++],
                           (p_dev->zlg600_serv.cur_addr),// >> 1,
                           AM_I2C_M_7BIT | AM_I2C_M_WR | AM_I2C_M_NOSTART,
                           p_buf,
                           info_len);
        }
    }

    /* 便于发送校验和和帧结束符 */
    temp[0] = p_frame->bcc;
    temp[1] = p_frame->ext;
    
    /* 最后一次发送需要停止 */
    am_i2c_mktrans(&p_dev->p_trans[trans_addr++],
                   (p_dev->zlg600_serv.cur_addr),// >> 1,
                   AM_I2C_M_7BIT | AM_I2C_M_WR | AM_I2C_M_NOSTART,
                   temp,
                   2);

    am_i2c_mkmsg(&msg,
                 p_dev->p_trans,
                 trans_addr,
                 __i2c_trans_data_complete,
                 (void *)&wait);

    am_i2c_msg_start(p_dev->i2c_handle, &msg);

    am_wait_on(&wait);

    return msg.status;
}

/* 接收一个旧回应帧 */
static int __zlg600_i2c_old_frame_respond_recv (void                  *p_cookie,
                                                am_zlg600_frame_old_t *p_frame)
{
    am_zlg600_i2c_dev_t *p_dev = (am_zlg600_i2c_dev_t *)p_cookie;
    
    int      i,j;
    uint8_t  info_len;
    uint8_t *p_buf;
    uint8_t  bcc = 0;             /* 接收时，可能存在数据丢弃，直接边读边校验 */

    uint8_t  remain_len;
    uint8_t  discard_data;        /* 用于将接收的无用数据丢弃 */
    uint8_t  temp[2];
    int      ret;

    /* 等待可以接收 */
    ret = __wait_can_recv(p_dev);
    if (ret != AM_OK) {
        return ret;
    }
    
    /* 如果是帧格式切换，则直接返回 */
    if (((p_frame->cmd_type & 0xF) == 0x01) && (p_frame->cmd_status == 'K')) {
        p_frame->cmd_status = 0;
        return AM_OK;
    }

    /* 接收帧长 + 命令类型 + 状态 + 信息长度 （共计4个字节）*/
    ret = __i2c_trans_data(p_dev, 
                           AM_I2C_M_7BIT | AM_I2C_M_RD,
                           (uint8_t *)p_frame,
                           4);
    if (ret != AM_OK) {
        return ret;
    }

    bcc ^= p_frame->frame_len;
    bcc ^= p_frame->cmd_type;
    bcc ^= p_frame->cmd_status;
    bcc ^= p_frame->info_len;
 
    remain_len = p_frame->info_len;      /* 剩余待接收信息的字符总数 */
    
    /* 接收各个信息缓冲区中的数据 */
    for (i = 0; i < p_frame->buf_num; i++) {
        p_buf     = p_frame->p_info_desc[i].p_buf;
        info_len  = p_frame->p_info_desc[i].len;

        /* 读取的字节数不能大于剩余字节 */
        info_len = info_len > remain_len ? remain_len : info_len;

        remain_len -= info_len;

        p_frame->p_info_desc[i].len = info_len;

        if (info_len != 0) {                       /* 存在信息，则接收 */

            if (p_buf == NULL) {                   /* 接收的数据丢弃   */

                while (info_len != 0) {
                    
                    ret = __i2c_trans_data(p_dev, 
                                           AM_I2C_M_7BIT | AM_I2C_M_RD | AM_I2C_M_NOSTART,
                                           &discard_data, 
                                           1);
                    
                    if (ret != AM_OK) {
                        return ret;
                    }
                    
                    info_len--;
                    bcc ^= discard_data;
                }
            } else {
                
                ret = __i2c_trans_data(p_dev, 
                                       AM_I2C_M_7BIT | AM_I2C_M_RD | AM_I2C_M_NOSTART,
                                       p_buf, 
                                       info_len);
                
                if (ret != AM_OK) {
                    return ret;
                }
                
                for (j = 0; j < info_len; j++) {
                    bcc ^= p_buf[j];
                }
            }
        }
    }

    /* 未接收的信息丢弃 */
    while (remain_len != 0) {
        
        ret = __i2c_trans_data(p_dev, 
                               AM_I2C_M_7BIT | AM_I2C_M_RD | AM_I2C_M_NOSTART,
                               &discard_data, 
                               1);
  
        if (ret != AM_OK) {
            return ret;
        }
        remain_len--;
        bcc ^= discard_data;
    }
 
    /* 最后一次读取需要停止 */
    ret = __i2c_trans_data(p_dev, 
                           AM_I2C_M_7BIT | AM_I2C_M_RD | AM_I2C_M_NOSTART,
                           temp, 
                           2);
    
    if (ret != AM_OK) {
        return ret;
    }

    /* 便于发送校验和和帧结束符 */
    p_frame->bcc = temp[0];
    p_frame->ext = temp[1];
    
    bcc = ~bcc;
    
    if (bcc != p_frame->bcc) {
        return -AM_EIO;
    }
    
    return AM_OK;
}

/******************************************************************************/

/* 完成一次I2C数据传输 */
static int __i2c_trans_new_frame_data (am_zlg600_i2c_dev_t *p_dev,
                                       uint16_t             flags,
                                       uint16_t             sub_addr,
                                       uint8_t             *p_buf,
                                       uint32_t             nbytes)
{
    am_i2c_message_t msg;
    uint8_t          trans_addr = 0;
    am_wait_t        wait;

    am_wait_init(&wait);

    sub_addr = ((sub_addr & 0x00FF) << 8) | ((sub_addr & 0xFF00) >> 8);

    am_i2c_mktrans(&p_dev->p_trans[trans_addr++],
                   (p_dev->zlg600_serv.cur_addr),// >> 1,
                   AM_I2C_M_7BIT | AM_I2C_M_WR,
                   (uint8_t *)(&sub_addr),
                   2);

    am_i2c_mktrans(&p_dev->p_trans[trans_addr++],
                   (p_dev->zlg600_serv.cur_addr),// >> 1,
                   flags | AM_I2C_M_NOSTART,
                   p_buf,
                   nbytes);

    am_i2c_mkmsg(&msg,
                 p_dev->p_trans,
                 trans_addr,
                 __i2c_trans_data_complete,
                 (void *)&wait);

    am_i2c_msg_start(p_dev->i2c_handle, &msg);

    am_wait_on(&wait);

    return msg.status;
}

/* 发送一个新命令帧 */
static int __zlg600_i2c_new_frame_cmd_send (void                   *p_cookie,
                                            am_zlg600_frame_new_t  *p_frame)
{
    am_zlg600_i2c_dev_t *p_dev = (am_zlg600_i2c_dev_t *)p_cookie;

    int i;
    int ret;

    uint16_t   start_addr = 0x0102;
    uint8_t    status_write = 0x8D;

    uint8_t   *p_buf;
    uint16_t  info_len;

//    p_dev->zlg600_serv.cur_addr = p_frame->local_addr;

    /* 首先求得命令帧的总长度 */
    info_len = 0;
    for (i = 0; i < p_frame->buf_num; i++) {
        info_len += p_frame->p_info_desc[i].len;
    }

    info_len += 10;  /* 其余信息长度为10 */

    /* 写入命令帧长度 */
    ret = __i2c_trans_new_frame_data(p_dev,
                                     AM_I2C_M_7BIT,
                                     start_addr,
                                     (uint8_t *)&info_len,
                                     2);
    if (ret != AM_OK) {
        return ret;
    }

    start_addr += 2;

    /* 发送地址(1) + 卡槽索引(1) + 安全报文/包号(1) + 命令类型 （共计4个字节）*/
    ret = __i2c_trans_new_frame_data(p_dev,
                                     AM_I2C_M_7BIT,
                                     start_addr,
                                     (uint8_t *)p_frame,
                                     4);
    if (ret != AM_OK) {
        return ret;
    }

    start_addr += 4;

    /* 发送命令/状态码(2) + 信息长度(2)（共计4个字节） */
    ret = __i2c_trans_new_frame_data(p_dev,
                                     AM_I2C_M_7BIT,
                                     start_addr,
                                     (uint8_t *)(&p_frame->cmd_status),
                                     4);
    if (ret != AM_OK) {
        return ret;
    }

    start_addr += 4;

    /* 发送各缓冲区中的信息 */
    for (i = 0; i < p_frame->buf_num; i++) {
        p_buf     = p_frame->p_info_desc[i].p_buf;
        info_len  = p_frame->p_info_desc[i].len;
        if (info_len != 0) {
            ret = __i2c_trans_new_frame_data(p_dev,
                                             AM_I2C_M_7BIT,
                                             start_addr,
                                             p_buf,
                                             info_len);

            if (ret != AM_OK) {
                return ret;
            }

            start_addr += info_len;
        }
    }

    /* 发送校验和，2字节 */
    ret = __i2c_trans_new_frame_data(p_dev,
                                     AM_I2C_M_7BIT,
                                     start_addr,
                                     (uint8_t *)&(p_frame->check_sum),
                                     2);

    if (ret != AM_OK) {
        return ret;
    }

    start_addr = 0x0101;

    ret = __i2c_trans_new_frame_data(p_dev,
                                     AM_I2C_M_7BIT,
                                     start_addr,
                                     (uint8_t *)&status_write,
                                     1);

    return ret;
}

/* 接收一个新回应帧 */
static int __zlg600_i2c_new_frame_respond_recv (void                  *p_cookie,
                                                am_zlg600_frame_new_t *p_frame)
{
    am_zlg600_i2c_dev_t *p_dev = (am_zlg600_i2c_dev_t *)p_cookie;
    
    int       i, j;
    int       ret;
    uint8_t  *p_buf;
    uint16_t  info_len;
    uint16_t  remain_len;
    uint16_t  start_addr   = 0x0104;
    uint8_t   discard_data;
    uint16_t  sum = 0;         /* 接收时，可能存在数据丢弃，直接边读边校验 */

//    p_dev->zlg600_serv.cur_addr = p_frame->local_addr;
    
    /* 等待可以接收 */
    ret = __wait_can_recv(p_dev);
    if (ret != AM_OK) {
        return ret;
    }
    
    /* 如果是帧格式切换，则直接返回 */
    if ((p_frame->cmd_class == 0x01) && (p_frame->cmd_status == 'K')) {
        return AM_OK;
    }

    /* 接收地址(1) + 卡槽索引(1) + 安全报文/包号(1) + 命令类型 （共计4个字节）*/
    ret = __i2c_trans_new_frame_data(p_dev,
                                     AM_I2C_M_7BIT | AM_I2C_M_RD,
                                     start_addr,
                                     (uint8_t *)p_frame,
                                     4);
    if (ret != AM_OK) {
        return ret;
    }

    start_addr += 4;

    sum += p_frame->local_addr;
    sum += p_frame->slot_index;
    sum += p_frame->smc_seq;
    sum += p_frame->cmd_class;

    /* 接收命令/状态码(2) + 信息长度(2)（共计4个字节） */
    ret = __i2c_trans_new_frame_data(p_dev,
                                     AM_I2C_M_7BIT | AM_I2C_M_RD,
                                     start_addr,
                                     (uint8_t *)(&p_frame->cmd_status),
                                     4);
    if (ret != AM_OK) {
        return ret;
    }

    start_addr += 4;

    sum += p_frame->cmd_status & 0xFF;            /* 低字节 */
    sum += p_frame->cmd_status >> 8;              /* 高字节 */

    sum += p_frame->info_len & 0xFF;              /* 低字节 */
    sum += p_frame->info_len >> 8;                /* 低字节 */

    remain_len = p_frame->info_len;               /* 剩余待接收信息的字符总数 */
    
    /* 接收各个信息缓冲区中的数据 */
    for (i = 0; i < p_frame->buf_num; i++) {
        p_buf     = p_frame->p_info_desc[i].p_buf;
        info_len  = p_frame->p_info_desc[i].len;

        /* 读取的字节数不能大于剩余字节 */
        info_len = info_len > remain_len ? remain_len : info_len;

        remain_len -= info_len;

        p_frame->p_info_desc[i].len = info_len;

        if (info_len != 0) {                       /* 存在信息，则接收 */

            if (p_buf == NULL) {                   /* 接收的数据丢弃   */

                while (info_len != 0) {
                    
                    ret = __i2c_trans_new_frame_data(p_dev,
                                                     AM_I2C_M_7BIT | AM_I2C_M_RD,
                                                     start_addr,
                                                     &discard_data,
                                                     1);
                    
                    if (ret != AM_OK) {
                        return ret;
                    }
                    
                    start_addr += 1;

                    info_len--;
                    sum += discard_data;
                }
            } else {
                
                ret = __i2c_trans_new_frame_data(p_dev,
                                                 AM_I2C_M_7BIT | AM_I2C_M_RD,
                                                 start_addr,
                                                 p_buf,
                                                 info_len);
                
                if (ret != AM_OK) {
                    return ret;
                }
                
                start_addr += info_len;

                for (j = 0; j < info_len; j++) {
                    sum += p_buf[j];
                }
            }
        }
    }
    
    /* 未接收的信息丢弃 */
    while (remain_len != 0) {
        
        ret = __i2c_trans_new_frame_data(p_dev,
                                         AM_I2C_M_7BIT | AM_I2C_M_RD,
                                         start_addr,
                                         &discard_data,
                                         1);
  
        if (ret != AM_OK) {
            return ret;
        }

        start_addr += 1;

        remain_len--;
        sum += discard_data;
    }
    
    sum = ~sum;

    /* 接收校验和 */
    ret = __i2c_trans_new_frame_data(p_dev,
                                     AM_I2C_M_7BIT | AM_I2C_M_RD,
                                     start_addr,
                                     (uint8_t *)&(p_frame->check_sum),
                                     2);

    if (ret != AM_OK) {
        return ret;
    }
    
    if (sum != p_frame->check_sum) {
        return -AM_EIO;
    }

    return AM_OK;
}

/*******************************************************************************
  公共的处理函数
*******************************************************************************/

/* 计算旧帧的校验和值 */
static uint8_t __zlg600_old_frame_bcc_cal (am_zlg600_frame_old_t *p_frame)
{
    uint8_t  i,j;
    uint8_t  bcc = 0;
    uint8_t *p_buf;
    uint8_t  info_len;

    bcc ^= p_frame->frame_len;
    bcc ^= p_frame->cmd_type;
    bcc ^= p_frame->cmd_status;
    bcc ^= p_frame->info_len;

    for (i = 0; i < p_frame->buf_num; i++) {
        p_buf     = p_frame->p_info_desc[i].p_buf;
        info_len  = p_frame->p_info_desc[i].len;
        for(j = 0; j < info_len; j++) {
            bcc ^= p_buf[j];
        }
    }
    bcc = ~bcc;                     /* 将异或值取反后作为校验和 */

    return  bcc;
}

/* 计算新帧的校验和值 */
static uint16_t __zlg600_new_frame_sum_cal (am_zlg600_frame_new_t *p_frame)
{
    uint16_t  i,j;
    uint16_t  sum = 0;
    uint8_t  *p_buf;
    uint16_t  info_len;

    sum += p_frame->local_addr;
    sum += p_frame->slot_index;
    sum += p_frame->smc_seq;
    sum += p_frame->cmd_class;

    sum += p_frame->cmd_status & 0xFF;          /* 低字节 */
    sum += p_frame->cmd_status >> 8;            /* 高字节 */

    sum += p_frame->info_len & 0xFF;            /* 低字节 */
    sum += p_frame->info_len >> 8;              /* 低字节 */

    for (i = 0; i < p_frame->buf_num; i++) {
        p_buf     = p_frame->p_info_desc[i].p_buf;
        info_len  = p_frame->p_info_desc[i].len;
        for(j = 0; j < info_len; j++) {
            sum += p_buf[j];
        }
    }

    sum = ~sum;                                 /* 将得到的各个数据的和值取反 */

    return  sum;
}

/******************************************************************************/

/* 校验接收的旧回应帧是否出现错误 */
static int __zlg600_old_frame_respond_check (am_zlg600_frame_old_t *p_frame,
                                             uint8_t                cmd_type)
{

    /* 状态不为0x00，表示命令执行失败 */
    if (p_frame->cmd_status != 0x00) {
        return -AM_EIO;
    }

    /* 回应帧和命令帧的 命令类型 不同，表示出现错误 */
    if (p_frame->cmd_type != cmd_type) {
        return -AM_EIO;
    }

    return AM_OK;
}

/* 校验接收的新回应帧是否出现错误 */
static int __zlg600_new_frame_respond_check (am_zlg600_frame_new_t *p_frame,
                                             uint8_t                cmd_type,
                                             uint8_t                frame_seq)
{

    /* 状态不为0x00，表示命令执行失败 */
    if (p_frame->cmd_status != 0x00) {
        return -AM_EIO;
    }

    /* 回应帧和命令帧的 命令类型 不同，表示出现错误 */
    if (p_frame->cmd_class != cmd_type) {
        return -AM_EIO;
    }

    /* 回应帧和命令帧的 包号 不同，表示出现错误     */
    if (p_frame->smc_seq != frame_seq ) {
        return -AM_EIO;
    }

    return  AM_OK;
}

/******************************************************************************/

/* 处理一个UART旧帧 */
static int __zlg600_uart_old_frame_proc (am_zlg600_handle_t           handle,
                                         uint8_t                      cmd_type,
                                         uint8_t                      cmd,
                                         am_zlg600_info_buf_desc_t   *p_cmd_desc,
                                         uint8_t                      cmd_buf_num,
                                         am_zlg600_info_buf_desc_t   *p_res_desc,
                                         uint8_t                      res_buf_num,
                                         uint16_t                    *p_res_info_len)
{
    am_zlg600_frame_old_t   frame;
    am_zlg600_serv_t       *p_serv = (am_zlg600_serv_t *)handle;

    int (*pfn_temp)(void *, am_zlg600_frame_old_t *);

    int      i;
    uint16_t total_len;
    int      ret;

    if (cmd_buf_num != 0 && p_cmd_desc == NULL) {
        return -AM_EINVAL;
    }

    if (res_buf_num != 0 && p_res_desc == NULL) {
        return -AM_EINVAL;
    }

    total_len = 0;
    for (i = 0; i < cmd_buf_num; i++) {
        total_len += p_cmd_desc[i].len;
    }

    p_serv->frame_seq = (p_serv->frame_seq + 1) & 0xF;  /* 帧包号自增 */

    frame.frame_len    = 0x06 + total_len;     /* 旧帧的长度为0x06 + 信息长度 */
    frame.cmd_type     = cmd_type | (p_serv->frame_seq << 4);    /* 帧包号在高四位 */
    frame.cmd_status   = cmd;
    frame.info_len     = total_len;
    frame.p_info_desc  = p_cmd_desc;
    frame.buf_num      = cmd_buf_num;
    frame.bcc          = __zlg600_old_frame_bcc_cal(&frame);
    frame.ext          = 0x03;

    if (1 != p_serv->auto_detect_read) {

        pfn_temp = __zlg600_uart_old_frame_cmd_send;

        /* 发送命令帧，并判断是否成功 */
        ret = pfn_temp(handle->p_cookie, &frame);
        if (ret != AM_OK) {
            return ret;
        }
    }

    /* 用于接收回应帧中的信息      */
    frame.p_info_desc  = p_res_desc;
    frame.buf_num      = res_buf_num;

    pfn_temp = __zlg600_uart_old_frame_respond_recv;

    /* 接收回应帧，并判断是否成功 */
    ret = pfn_temp(handle->p_cookie, &frame);
    if (ret != AM_OK) {
        return ret;
    }

    /* 检查回应帧是否出现错误 */
    ret = __zlg600_old_frame_respond_check(&frame, cmd_type | (p_serv->frame_seq << 4));
    if (ret != AM_OK) {
        return ret;
    }

    if (p_res_info_len != NULL) {
        *p_res_info_len = frame.info_len;
    }

    return AM_OK;
}

/******************************************************************************/

/* 处理一个UART新帧 */
static int __zlg600_uart_new_frame_proc (am_zlg600_handle_t         handle,
                                         uint8_t                    cmd_class,
                                         uint16_t                   cmd,
                                         am_zlg600_info_buf_desc_t *p_cmd_desc,
                                         uint8_t                    cmd_buf_num,
                                         am_zlg600_info_buf_desc_t *p_res_desc,
                                         uint8_t                    res_buf_num,
                                         uint16_t                  *p_res_info_len)

{
    am_zlg600_frame_new_t   frame;
    am_zlg600_serv_t       *p_serv = handle;

    int (*pfn_temp)(void *, am_zlg600_frame_new_t *);

    int i;
    uint16_t total_len;
    int ret;

    if (cmd_buf_num != 0 && p_cmd_desc == NULL) {
        return -AM_EINVAL;
    }

    if (res_buf_num != 0 && p_res_desc == NULL) {
        return -AM_EINVAL;
    }

    total_len = 0;
    for (i = 0; i < cmd_buf_num; i++) {
        total_len += p_cmd_desc[i].len;
    }

    frame.local_addr   = p_serv->cur_addr << 1;
    frame.slot_index   = 0x00;
//    frame.smc_seq      = p_serv->frame_seq;
    frame.cmd_class    = cmd_class;
    frame.cmd_status   = cmd;
    frame.info_len     = total_len;
    frame.p_info_desc  = p_cmd_desc;
    frame.buf_num      = cmd_buf_num;
//    frame.check_sum    = __zlg600_new_frame_sum_cal(&frame);

    if (1 != p_serv->auto_detect_read) {

        /* 包号加1，仅低4位有效 */
        p_serv->frame_seq = (p_serv->frame_seq + 1) & 0xF;

        frame.smc_seq      = p_serv->frame_seq;
        frame.check_sum    = __zlg600_new_frame_sum_cal(&frame);

        pfn_temp = __zlg600_uart_new_frame_cmd_send;

        /* 发送命令帧，并判断是否成功 */
        ret = pfn_temp(p_serv->p_cookie, &frame);
        if (ret != AM_OK) {
            return ret;
        }
    }

    /* 用于接收回应帧中的信息      */
    frame.p_info_desc  = p_res_desc;
    frame.buf_num      = res_buf_num;

    pfn_temp = __zlg600_uart_new_frame_respond_recv;

     /* 接收回应帧，并判断是否成功 */
    ret = pfn_temp(p_serv->p_cookie, &frame);
    if (ret != AM_OK) {
        return ret;
    }

    /* 检查回应帧是否出现错误 */
    ret = __zlg600_new_frame_respond_check(&frame, cmd_class, p_serv->frame_seq);
    if (ret != AM_OK) {
        return ret;
    }

    if (p_res_info_len != NULL) {
        *p_res_info_len = frame.info_len;
    }
    return AM_OK;
}

/******************************************************************************/

/* 处理一个UART帧 */
static int __zlg600_uart_frame_proc (am_zlg600_handle_t         handle,
                                     uint8_t                    cmd_class,
                                     uint16_t                   cmd,
                                     am_zlg600_info_buf_desc_t *p_cmd_desc,
                                     uint8_t                    cmd_buf_num,
                                     am_zlg600_info_buf_desc_t *p_res_desc,
                                     uint8_t                    res_buf_num,
                                     uint16_t                  *p_res_info_len)

{
    am_zlg600_uart_dev_t *p_dev = (am_zlg600_uart_dev_t *)(handle->p_cookie);

    return ((int (*)(am_zlg600_handle_t,
                     uint8_t,
                     uint16_t,
                     am_zlg600_info_buf_desc_t *,
                     uint8_t,
                     am_zlg600_info_buf_desc_t *,
                     uint8_t,
                     uint16_t *))
            (p_dev->pfn_frame_proc))(handle,
                                     cmd_class,
                                     cmd,
                                     p_cmd_desc,
                                     cmd_buf_num,
                                     p_res_desc,
                                     res_buf_num,
                                     p_res_info_len);
}

/******************************************************************************/

/* 处理一个I2C旧帧 */
static int __zlg600_i2c_old_frame_proc (am_zlg600_handle_t           handle,
                                        uint8_t                      cmd_type,
                                        uint8_t                      cmd,
                                        am_zlg600_info_buf_desc_t   *p_cmd_desc,
                                        uint8_t                      cmd_buf_num,
                                        am_zlg600_info_buf_desc_t   *p_res_desc,
                                        uint8_t                      res_buf_num,
                                        uint16_t                    *p_res_info_len)
{
    am_zlg600_frame_old_t   frame;
    am_zlg600_serv_t       *p_serv = (am_zlg600_serv_t *)handle;

    int (*pfn_temp)(void *, am_zlg600_frame_old_t *);

    int      i;
    uint16_t total_len;
    int      ret;

    if (cmd_buf_num != 0 && p_cmd_desc == NULL) {
        return -AM_EINVAL;
    }

    if (res_buf_num != 0 && p_res_desc == NULL) {
        return -AM_EINVAL;
    }

    total_len = 0;
    for (i = 0; i < cmd_buf_num; i++) {
        total_len += p_cmd_desc[i].len;
    }

    p_serv->frame_seq = (p_serv->frame_seq + 1) & 0xF;  /* 帧包号自增 */

    frame.frame_len   = 0x06 + total_len;     /* 旧帧的长度为0x06 + 信息长度 */
    frame.cmd_type    = cmd_type | (p_serv->frame_seq << 4);    /* 帧包号在高四位 */
    frame.cmd_status  = cmd;
    frame.info_len    = total_len;
    frame.p_info_desc = p_cmd_desc;
    frame.buf_num     = cmd_buf_num;
    frame.bcc         = __zlg600_old_frame_bcc_cal(&frame);
    frame.ext         = 0x03;

    if (1 != p_serv->auto_detect_read) {

        pfn_temp = __zlg600_i2c_old_frame_cmd_send;

        /* 发送命令帧，并判断是否成功 */
        ret = pfn_temp(handle->p_cookie, &frame);
        if (ret != AM_OK) {
            return ret;
        }
    }

    /* 用于接收回应帧中的信息      */
    frame.p_info_desc  = p_res_desc;
    frame.buf_num      = res_buf_num;

    pfn_temp = __zlg600_i2c_old_frame_respond_recv;

    /* 接收回应帧，并判断是否成功 */
    ret = pfn_temp(handle->p_cookie, &frame);
    if (ret != AM_OK) {
        return ret;
    }

    /* 检查回应帧是否出现错误 */
    ret = __zlg600_old_frame_respond_check(&frame, cmd_type | (p_serv->frame_seq << 4));
    if (ret != AM_OK) {
        return ret;
    }

    if (p_res_info_len != NULL) {
        *p_res_info_len = frame.info_len;
    }

    return AM_OK;
}

/******************************************************************************/

/* 处理一个I2C新帧 */
static int __zlg600_i2c_new_frame_proc (am_zlg600_handle_t         handle,
                                        uint8_t                    cmd_class,
                                        uint16_t                   cmd,
                                        am_zlg600_info_buf_desc_t *p_cmd_desc,
                                        uint8_t                    cmd_buf_num,
                                        am_zlg600_info_buf_desc_t *p_res_desc,
                                        uint8_t                    res_buf_num,
                                        uint16_t                  *p_res_info_len)

{
    am_zlg600_frame_new_t   frame;
    am_zlg600_serv_t       *p_serv = handle;
    
    int (*pfn_temp)(void *, am_zlg600_frame_new_t *);
    
    int i;
    uint16_t total_len;
    int ret;

    if (cmd_buf_num != 0 && p_cmd_desc == NULL) {
        return -AM_EINVAL;
    }

    if (res_buf_num != 0 && p_res_desc == NULL) {
        return -AM_EINVAL;
    }

    total_len = 0;
    for (i = 0; i < cmd_buf_num; i++) {
        total_len += p_cmd_desc[i].len;
    }
    
    frame.local_addr  = p_serv->cur_addr << 1;
    frame.slot_index  = 0x00;
//    frame.smc_seq     = p_serv->frame_seq;
    frame.cmd_class   = cmd_class;
    frame.cmd_status  = cmd;
    frame.info_len    = total_len;
    frame.p_info_desc = p_cmd_desc;
    frame.buf_num     = cmd_buf_num;
//    frame.check_sum   = __zlg600_new_frame_sum_cal(&frame);

    if (1 != p_serv->auto_detect_read) {

        /* 包号加1，仅低4位有效 */
        p_serv->frame_seq = (p_serv->frame_seq + 1) & 0xF;

        frame.smc_seq     = p_serv->frame_seq;
        frame.check_sum   = __zlg600_new_frame_sum_cal(&frame);

        pfn_temp = __zlg600_i2c_new_frame_cmd_send;

        /* 发送命令帧，并判断是否成功 */
        ret = pfn_temp(p_serv->p_cookie, &frame);
        if (ret != AM_OK) {
            return ret;
        }
    }

    /* I2C模式下设置新旧帧格式，切换格式后无需再接收回应帧 */
    if (cmd_class == 0x01 && cmd == 'K') {
        return AM_OK;
    }
    
    /* 用于接收回应帧中的信息      */
    frame.p_info_desc  = p_res_desc;
    frame.buf_num      = res_buf_num;

    pfn_temp = __zlg600_i2c_new_frame_respond_recv;
    
     /* 接收回应帧，并判断是否成功 */
    ret = pfn_temp(p_serv->p_cookie, &frame);
    if (ret != AM_OK) {
        return ret;
    }

    /* 检查回应帧是否出现错误 */
    ret = __zlg600_new_frame_respond_check(&frame, cmd_class, p_serv->frame_seq);
    if (ret != AM_OK) {
        return ret;
    }

    if (p_res_info_len != NULL) {
        *p_res_info_len = frame.info_len;
    }
    return AM_OK;
}

/* 处理一个I2C帧 */
static int __zlg600_i2c_frame_proc (am_zlg600_handle_t         handle,
                                    uint8_t                    cmd_class,
                                    uint16_t                   cmd,
                                    am_zlg600_info_buf_desc_t *p_cmd_desc,
                                    uint8_t                    cmd_buf_num,
                                    am_zlg600_info_buf_desc_t *p_res_desc,
                                    uint8_t                    res_buf_num,
                                    uint16_t                  *p_res_info_len)

{
    am_zlg600_i2c_dev_t *p_dev = (am_zlg600_i2c_dev_t *)(handle->p_cookie);

    /* 如果指令为设置波特率(I2C不支持串口波特率设置) */
    if ((cmd_class == 0x01) && (cmd == 'H')) {
        return -AM_ENOTSUP;
    }

    return ((int (*)(am_zlg600_handle_t,
                     uint8_t,
                     uint16_t,
                     am_zlg600_info_buf_desc_t *,
                     uint8_t,
                     am_zlg600_info_buf_desc_t *,
                     uint8_t,
                     uint16_t *))
            (p_dev->pfn_frame_proc))(handle,
                                     cmd_class,
                                     cmd,
                                     p_cmd_desc,
                                     cmd_buf_num,
                                     p_res_desc,
                                     res_buf_num,
                                     p_res_info_len);
}

/******************************************************************************/

/* 根据设备中的frame_fmt处理一个帧 */
static int __zlg600_frame_proc (am_zlg600_handle_t           handle,
                                uint8_t                      cmd_class,
                                uint16_t                     cmd,
                                am_zlg600_info_buf_desc_t   *p_cmd_desc,
                                uint8_t                      cmd_buf_num,
                                am_zlg600_info_buf_desc_t   *p_res_desc,
                                uint8_t                      res_buf_num,
                                uint16_t                    *p_res_info_len)

{
    return ((int (*)(am_zlg600_handle_t,
                     uint8_t,
                     uint16_t,
                     am_zlg600_info_buf_desc_t *,
                     uint8_t,
                     am_zlg600_info_buf_desc_t *,
                     uint8_t,
                     uint16_t *))
            (handle->pfn_frame_proc))(handle,
                                      cmd_class,
                                      cmd,
                                      p_cmd_desc,
                                      cmd_buf_num,
                                      p_res_desc,
                                      res_buf_num,
                                      p_res_info_len);
}

/* 设置UART指令超时时间 */
static int __zlg600_uart_cmd_timeout_set (am_zlg600_handle_t handle,
                                          uint32_t           timeout_ms)
{
    int ret;

    am_zlg600_uart_dev_t *p_dev = (am_zlg600_uart_dev_t *)(handle->p_cookie);

    handle->timeout_ms = timeout_ms;

    /* 设置读超时时间 */
    ret = am_uart_rngbuf_ioctl(p_dev->uart_ringbuf_handle,
                               AM_UART_RNGBUF_TIMEOUT,
                               (void *)timeout_ms);

    return ret;
}

/* 设置I2C指令超时时间 */
static int __zlg600_i2c_cmd_timeout_set (am_zlg600_handle_t handle,
                                         uint32_t           timeout_ms)
{
    handle->timeout_ms = timeout_ms;
    return AM_OK;
}

/* Mifare卡UART自动检测中断回调函数(前向声明) */
static void __auto_detect_int_callback (void *p_arg);

/* UART设置自动检测触发 */
static void __zlg600_uart_auto_detect_trigger_set (am_zlg600_handle_t handle,
                                                   am_bool_t          enable)
{
    am_zlg600_uart_dev_t *p_dev = (am_zlg600_uart_dev_t *)(handle->p_cookie);

    /* 使能 */
    if (enable) {

        /* 连接触发函数 */
        am_uart_rngbuf_rx_trigger_cfg(p_dev->uart_ringbuf_handle,
                                      3,
                                      __auto_detect_int_callback,
                                      handle);

        /* 使能触发函数 */
        am_uart_rngbuf_rx_trigger_enable(p_dev->uart_ringbuf_handle);

    /* 禁能 */
    } else {

        /* 禁能触发函数 */
        am_uart_rngbuf_rx_trigger_disable(p_dev->uart_ringbuf_handle);
    }
}

/* I2C设置自动检测触发 */
static void __zlg600_i2c_auto_detect_trigger_set (am_zlg600_handle_t handle,
                                                  am_bool_t          enable)
{
    am_zlg600_i2c_dev_t *p_dev = (am_zlg600_i2c_dev_t *)(handle->p_cookie);

    /* 使能 */
    if (enable) {

        /* 关闭引脚触发(清除中断) */
        am_gpio_trigger_off(p_dev->pin);

        /* 启动引脚触发 */
        am_gpio_trigger_on(p_dev->pin);

    /* 禁能 */
    } else {

        /* 关闭引脚触发 */
        am_gpio_trigger_off(p_dev->pin);
    }
}

/*******************************************************************************
   初始化函数
*******************************************************************************/

/* zlg600 uart 初始化 */
static am_zlg600_handle_t __zlg600_uart_init (am_zlg600_uart_dev_t            *p_dev,
                                              am_uart_handle_t                 uart_handle,
                                              const am_zlg600_uart_dev_info_t *p_uart_info,
                                              am_pfnvoid_t                     p_op_func)
{
    uint8_t           tx_data, rx_data;
    am_zlg600_serv_t *p_serv = NULL;

    if (p_dev == NULL || p_uart_info == NULL) {
        return NULL;
    }

    /* 串口通信初始化函数不支持使用I2C模式，应直接使用I2C通信的初始化函数  */
    if (p_uart_info->now_mode == AM_ZLG600_MODE_I2C) {
        return NULL;
    }

    /* 设置波特率为  baudrate */
    am_uart_ioctl(uart_handle, AM_UART_BAUD_SET, (void *)p_uart_info->baudrate);

    /* 设置数据位为8位，无校验，1停止位 */
    am_uart_ioctl(uart_handle, AM_UART_OPTS_SET, (void *)AM_UART_CS8);

    /* 串口（带RingBuf）初始化*/
    p_dev->uart_ringbuf_handle = am_uart_rngbuf_init(&(p_dev->uart_ringbuf_dev),
                                                      uart_handle,
                                                      p_uart_info->p_uart_rxbuf,
                                                      p_uart_info->rxbuf_size,
                                                      p_uart_info->p_uart_txbuf,
                                                      p_uart_info->txbuf_size);

    /* 设置读超时时间为 200ms */
    am_uart_rngbuf_ioctl(p_dev->uart_ringbuf_handle,
                         AM_UART_RNGBUF_TIMEOUT,
                         (void *)200);

    /* 当前处于自动侦测模式，连续发送两次0x20 */
    if (p_uart_info->now_mode == AM_ZLG600_MODE_AUTO_CHECK ) {
        tx_data = 0x20;
        am_uart_rngbuf_send(p_dev->uart_ringbuf_handle, &tx_data, 1);
        am_mdelay(2);
        am_uart_rngbuf_send(p_dev->uart_ringbuf_handle, &tx_data, 1);

        rx_data = 0x00;

        /* 接收一个数据 */
         am_uart_rngbuf_receive(p_dev->uart_ringbuf_handle, &rx_data, 1);

//        /* 没有接收到一个数据或接收的数据不为0x06 */
//        if ((len != 1)  || (rx_data != 0x06)) {
//            return NULL;
//        }
    }

    /* 已经处于串口模式，无需再发送两次0x20 */
    if (p_uart_info->now_mode == AM_ZLG600_MODE_UART) {
    }

    p_serv = &p_dev->zlg600_serv;
    
    /* 初始化ZLG600标准服务 */
    p_serv->frame_seq  = 0;
    p_serv->p_cookie   = p_dev;
    p_serv->cur_addr   = p_uart_info->local_addr;
    p_serv->timeout_ms = 200;
    
    /* 加载指令超时函数 */
    p_serv->pfn_cmd_timeout_set = (am_pfnvoid_t)(__zlg600_uart_cmd_timeout_set);

    /* 加载帧处理函数 */
    p_serv->pfn_frame_proc = p_op_func;

    /* 加载自动检测触发函数 */
    p_serv->pfn_auto_detect_trigger_set =
                    (am_pfnvoid_t)__zlg600_uart_auto_detect_trigger_set;

    /* 清除自动检测读标志 */
    p_serv->auto_detect_read = 0;

    return p_serv;
}

/* zlg600 uart 初始化（旧帧） */
am_zlg600_handle_t am_zlg600_uart_old_init (am_zlg600_uart_dev_t            *p_dev,
                                            am_uart_handle_t                 uart_handle,
                                            const am_zlg600_uart_dev_info_t *p_uart_info)
{

    p_dev->pfn_frame_proc = (am_pfnvoid_t)__zlg600_uart_old_frame_proc;

    return __zlg600_uart_init(p_dev,
                              uart_handle,
                              p_uart_info,
                              (am_pfnvoid_t)(__zlg600_uart_frame_proc));
}

/* zlg600 uart 初始化（新帧） */
am_zlg600_handle_t am_zlg600_uart_new_init (am_zlg600_uart_dev_t            *p_dev,
                                            am_uart_handle_t                 uart_handle,
                                            const am_zlg600_uart_dev_info_t *p_uart_info)
{
    p_dev->pfn_frame_proc = (am_pfnvoid_t)__zlg600_uart_new_frame_proc;

    return __zlg600_uart_init(p_dev,
                              uart_handle,
                              p_uart_info,
                              (am_pfnvoid_t)(__zlg600_uart_frame_proc));
}

/* zlg600 i2c 初始化 */
static am_zlg600_handle_t __zlg600_i2c_init (am_zlg600_i2c_dev_t            *p_dev,
                                             am_i2c_handle_t                 i2c_handle,
                                             const am_zlg600_i2c_dev_info_t *p_i2c_info,
                                             am_pfnvoid_t                    p_op_func)
{
    am_zlg600_serv_t *p_serv = NULL;

    if (p_dev == NULL || p_i2c_info == NULL) {
        return NULL;
    }

    /* I2C通信初始化函数不支持使用UART模式，应直接使用UART通信的初始化函数  */
    if (p_i2c_info->now_mode == AM_ZLG600_MODE_UART) {
        return NULL;
    }

    /* I2C模式可以直接进行数据通信，无需向串口那样连续发送两次0x20 */

    /* 配置I2C中断引脚 */
    am_gpio_pin_cfg(p_i2c_info->pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);

    /* 连接GPIO引脚中断 */
    if (am_gpio_trigger_connect(p_i2c_info->pin, __pin_int_callback, p_dev) != AM_OK) {
        return NULL;
    }

    /* 下降沿触发 */
    if (am_gpio_trigger_cfg(p_i2c_info->pin, AM_GPIO_TRIGGER_FALL) != AM_OK) {
        return NULL;
    }

    p_dev->i2c_handle = i2c_handle;
    p_dev->pin        = p_i2c_info->pin;
    p_dev->p_trans    = p_i2c_info->p_trans;
    p_dev->trans_len  = p_i2c_info->trans_len;

    p_serv            = &p_dev->zlg600_serv;

    /* 初始化ZLG600标准服务 */
    p_serv->frame_seq   = 0;
    p_serv->p_cookie    = p_dev;
    p_serv->work_mode   = AM_ZLG600_MODE_SLAVE;
    p_serv->auto_detect = 0;
    p_serv->cur_addr    = p_i2c_info->local_addr;
    p_serv->timeout_ms  = 500;

    /* 加载指令超时函数 */
    p_serv->pfn_cmd_timeout_set = (am_pfnvoid_t)(__zlg600_i2c_cmd_timeout_set);

    /* 加载帧处理函数 */
    p_serv->pfn_frame_proc = p_op_func;

    /* 加载自动检测触发函数 */
    p_serv->pfn_auto_detect_trigger_set =
                    (am_pfnvoid_t)__zlg600_i2c_auto_detect_trigger_set;

    am_wait_init(&p_dev->wait);

    /* 清除自动检测读标志 */
    p_serv->auto_detect_read = 0;

    return p_serv;
}

/* zlg600 i2c 初始化（旧帧） */
am_zlg600_handle_t am_zlg600_i2c_old_init (am_zlg600_i2c_dev_t            *p_dev,
                                           am_i2c_handle_t                 i2c_handle,
                                           const am_zlg600_i2c_dev_info_t *p_i2c_info)
{
    p_dev->pfn_frame_proc = (am_pfnvoid_t)__zlg600_i2c_old_frame_proc;

    return __zlg600_i2c_init(p_dev,
                             i2c_handle,
                             p_i2c_info,
                             (am_pfnvoid_t)(__zlg600_i2c_frame_proc));
}

/* zlg600 i2c 初始化（新帧） */
am_zlg600_handle_t am_zlg600_i2c_new_init (am_zlg600_i2c_dev_t            *p_dev,
                                           am_i2c_handle_t                 i2c_handle,
                                           const am_zlg600_i2c_dev_info_t *p_i2c_info)
{
    p_dev->pfn_frame_proc = (am_pfnvoid_t)__zlg600_i2c_new_frame_proc;

    return __zlg600_i2c_init(p_dev,
                             i2c_handle,
                             p_i2c_info,
                             (am_pfnvoid_t)(__zlg600_i2c_frame_proc));
}

/*******************************************************************************
   设备控制类函数
*******************************************************************************/

/* 读设备信息 */
int am_zlg600_info_get (am_zlg600_handle_t  handle,
                        uint8_t            *p_info)
{
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_info == NULL) {
        return -AM_EINVAL;
    }

    rx_desc[0].p_buf = p_info;
    rx_desc[0].len   = 0x14;          /* 固定接收长度0x14 */

    return __zlg600_frame_proc(handle, 1, 'A', NULL, 0, rx_desc, 1, NULL);
}

/******************************************************************************/

/* 配置IC卡接口，该命令执行后，启动了IC卡接口，启动后默认支持TypeA卡 */
int am_zlg600_ic_port_config (am_zlg600_handle_t handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_frame_proc(handle, 1, 'B', NULL, 0, NULL, 0, NULL);
}

/* 关闭IC卡接口 */
int am_zlg600_ic_port_close (am_zlg600_handle_t handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_frame_proc(handle, 1, 'C', NULL, 0, NULL, 0, NULL);
}

/* 设置IC卡接口协议（工作模式）TypeA(0)或者TypeB(4) */
int am_zlg600_ic_isotype_set (am_zlg600_handle_t handle,
                              uint8_t            isotype)
{
    am_zlg600_info_buf_desc_t tx_desc[1];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &isotype;
    tx_desc[0].len   = 1;

    return __zlg600_frame_proc(handle, 1, 'D', tx_desc, 1, NULL, 0, NULL);
}

/******************************************************************************/

/* 装载IC卡密钥（工作模式）TypeA(0)或者TypeB(4) */
int am_zlg600_ic_key_load (am_zlg600_handle_t  handle,
                           uint8_t             key_type,
                           uint8_t             key_sec,
                           uint8_t            *p_key,
                           uint8_t             key_length)
{
    am_zlg600_info_buf_desc_t  tx_desc[3];

    if (handle == NULL || p_key == NULL) {
        return -AM_EINVAL;
    }

    /* 密钥类型 */
    tx_desc[0].p_buf = &key_type;
    tx_desc[0].len   = 1;

    /* 密钥区号 */
    tx_desc[1].p_buf = &key_sec;
    tx_desc[1].len   = 1;

    /* 密钥    */
    tx_desc[2].p_buf = p_key;
    tx_desc[2].len   = key_length;

    return __zlg600_frame_proc(handle, 1, 'E', tx_desc, 3, NULL, 0, NULL);
}

/******************************************************************************/

/* 设置IC卡接口的寄存器值 */
int am_zlg600_ic_reg_set (am_zlg600_handle_t handle,
                          uint8_t            reg_addr,
                          uint8_t            reg_val)
{
    am_zlg600_info_buf_desc_t tx_desc[2];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    /* 寄存器地址 */
    tx_desc[0].p_buf = &reg_addr;
    tx_desc[0].len   = 1;

    /* 寄存器值  */
    tx_desc[1].p_buf = &reg_val;
    tx_desc[1].len   = 1;

    return __zlg600_frame_proc(handle, 1, 'F', tx_desc, 2, NULL, 0, NULL);
}

/* 获取IC卡接口的寄存器值 */
int am_zlg600_ic_reg_get (am_zlg600_handle_t  handle,
                          uint8_t             reg_addr,
                          uint8_t            *p_val)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_val == NULL) {
        return -AM_EINVAL;
    }

    /* 寄存器地址     */
    tx_desc[0].p_buf = &reg_addr;
    tx_desc[0].len   = 1;

    /* 获取的寄存器值 */
    rx_desc[0].p_buf = p_val;
    rx_desc[0].len   = 1;

    return __zlg600_frame_proc(handle, 1, 'G', tx_desc, 1, rx_desc, 1, NULL);
}

/******************************************************************************/

/* 设置波特率 */
int am_zlg600_baudrate_set (am_zlg600_handle_t handle,
                            uint32_t           baudrate_flag)
{
    am_zlg600_info_buf_desc_t  tx_desc[1];
    uint8_t                    num = 0;     /* 波特率编码，对应0 ~ 7 */

    int ret;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    switch(baudrate_flag) {

    case AM_ZLG600_BAUDRATE_9600:
        num = 0;
        break;
    case AM_ZLG600_BAUDRATE_19200:
        num = 1;
        break;
    case AM_ZLG600_BAUDRATE_28800:
        num = 2;
        break;
    case AM_ZLG600_BAUDRATE_38400:
        num = 3;
        break;
    case AM_ZLG600_BAUDRATE_57600:
        num = 4;
        break;
    case AM_ZLG600_BAUDRATE_115200:
        num = 5;
        break;
    case AM_ZLG600_BAUDRATE_172800:
        num = 6;
        break;
    case AM_ZLG600_BAUDRATE_230400:
        num = 7;
        break;
    default:
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &num;
    tx_desc[0].len   = 1;

    ret = __zlg600_frame_proc(handle, 1, 'H', tx_desc, 1, NULL, 0, NULL);

    if (ret != AM_OK) {
        return ret;
    }

    /* 修改串口使用的比特率 */
    am_uart_rngbuf_ioctl(((am_zlg600_uart_dev_t *)handle)->uart_ringbuf_handle,
                          AM_UART_BAUD_SET,
                         (void *)baudrate_flag);

    return AM_OK;
}

/******************************************************************************/

/* 设置天线驱动方式 */
int am_zlg600_ant_mode_set (am_zlg600_handle_t handle,
                            uint8_t            antmode_flag)
{
    am_zlg600_info_buf_desc_t  tx_desc[1];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &antmode_flag;
    tx_desc[0].len   = 1;

    return __zlg600_frame_proc(handle, 1, 'I', tx_desc, 1, NULL, 0, NULL);
}

/******************************************************************************/

/* 设置新旧帧格式 */
int am_zlg600_frame_fmt_set (am_zlg600_handle_t handle,
                             uint8_t            flag)
{
    int ret;

    am_zlg600_info_buf_desc_t tx_desc[1];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    flag = flag & 0x01;     /* 帧类型只有最低位有效 #AM_ZLG600_FRAME_FMT_NEW */

    tx_desc[0].p_buf = &flag;
    tx_desc[0].len   = 1;

    ret = __zlg600_frame_proc(handle, 1, 'K', tx_desc, 1, NULL, 0, NULL);

    return ret;
}

/******************************************************************************/

/* 设置模式的从机地址 */
int am_zlg600_mode_addr_set (am_zlg600_handle_t handle,
                             uint8_t            mode,
                             uint8_t            new_addr)
{
    struct am_zlg600_serv *p_serv = handle;

    int     ret;
    uint8_t temp_addr = new_addr << 1;

    am_zlg600_info_buf_desc_t  tx_desc[2];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &mode;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = &temp_addr;
    tx_desc[1].len   = 1;

    ret = __zlg600_frame_proc(handle, 1, 'U', tx_desc, 2, NULL, 0, NULL);

    if (ret == AM_OK) {
        p_serv->cur_addr = new_addr;
    }

    return ret;
}

/* 获取模式和从机地址 */
int am_zlg600_mode_addr_get (am_zlg600_handle_t  handle,
                             uint8_t            *p_mode,
                             uint8_t            *p_addr)
{
    int     ret;

    am_zlg600_info_buf_desc_t  rx_desc[2];

    if (handle == NULL || p_mode == NULL || p_addr == NULL) {
        return -AM_EINVAL;
    }

    rx_desc[0].p_buf = p_mode;
    rx_desc[0].len   = 1;

    rx_desc[1].p_buf = p_addr;
    rx_desc[1].len   = 1;

    ret = __zlg600_frame_proc(handle, 1, 'V', NULL, 0, rx_desc, 2, NULL);

    *p_addr = *p_addr >> 1;

    return ret;
}

/******************************************************************************/

/* 装载用户密匙 */
int am_zlg600_user_key_load (am_zlg600_handle_t handle,
                             uint8_t            key_sec,
                             uint8_t           *p_key)
{
    am_zlg600_info_buf_desc_t  tx_desc[2];

    if (handle == NULL || p_key == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &key_sec;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = p_key;
    tx_desc[1].len   = 16;              /* 长度固定为16字节  */

    return __zlg600_frame_proc(handle, 1, 'a', tx_desc, 2, NULL, 0, NULL);
}

/******************************************************************************/

/* 读EEPROM */
int am_zlg600_eeprom_read (am_zlg600_handle_t handle,
                           uint8_t            eeprom_addr,
                           uint8_t           *p_buf,
                           uint8_t            nbytes)
{
    am_zlg600_info_buf_desc_t  tx_desc[2];
    am_zlg600_info_buf_desc_t  rx_desc[1];

    if (handle == NULL || p_buf == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &eeprom_addr;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = &nbytes;
    tx_desc[1].len   = 1;              /* 长度固定为16字节  */

    rx_desc[0].p_buf = p_buf;
    rx_desc[0].len   = nbytes;

    return __zlg600_frame_proc(handle, 1, 'b', tx_desc, 2, rx_desc, 1, NULL);
}

/* 写EEPROM */
int am_zlg600_eeprom_write (am_zlg600_handle_t handle,
                            uint8_t            eeprom_addr,
                            uint8_t           *p_buf,
                            uint8_t            nbytes)
{
    am_zlg600_info_buf_desc_t  tx_desc[3];

    if (handle == NULL || p_buf == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &eeprom_addr;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = &nbytes;
    tx_desc[1].len   = 1;              /* 长度固定为16字节  */

    tx_desc[2].p_buf = p_buf;
    tx_desc[2].len   = nbytes;

    return __zlg600_frame_proc(handle, 1, 'c', tx_desc, 3, NULL, 0, NULL);
}


/*******************************************************************************
   ISO14443通用函数（本地使用）
*******************************************************************************/

/*
 *     卡请求操作。以下命令是ISO14443协议规定好的，Mifare 卡 和 PICC卡均是一样的
 * 操作,这两种卡仅命令类型不同。
 */
static int __zlg600_iso14443_request (am_zlg600_handle_t handle,
                                      uint8_t            cmd_type,
                                      uint8_t            req_mode,
                                      uint16_t          *p_atq)
{
    am_zlg600_info_buf_desc_t  tx_desc[1];
    am_zlg600_info_buf_desc_t  rx_desc[1];

    tx_desc[0].p_buf = &req_mode;
    tx_desc[0].len   = 1;

    rx_desc[0].p_buf = (uint8_t *)p_atq;
    rx_desc[0].len   = 2;

    return __zlg600_frame_proc(handle, cmd_type, 'A', tx_desc, 1, rx_desc, 1, NULL);
}

/* 防碰撞 */
static int __zlg600_iso14443_anticoll (am_zlg600_handle_t handle,
                                       uint8_t            cmd_type,
                                       uint8_t            anticoll_level,
                                       uint32_t           know_uid,
                                       uint8_t            nbit_cnt,
                                       uint32_t          *p_uid)
{
    am_zlg600_info_buf_desc_t  tx_desc[3];
    am_zlg600_info_buf_desc_t  rx_desc[1];

    tx_desc[0].p_buf = &anticoll_level;
    tx_desc[0].len   = 1;

    /* 已知序列号的长度 */
    tx_desc[1].p_buf = &nbit_cnt;
    tx_desc[1].len   = 1;

    /* 已知UID，已知UID的位数由 nbit_cnt 决定 */
    tx_desc[2].p_buf = (uint8_t *)&know_uid;
    tx_desc[2].len   = 4;

    /* 接收的4字节UID */
    rx_desc[0].p_buf = (uint8_t *)p_uid;
    rx_desc[0].len   = 4;

    if (nbit_cnt == 0) {

        return __zlg600_frame_proc(handle,
                                   cmd_type,
                                   'B',
                                   tx_desc,
                                   2,
                                   rx_desc,
                                   1,
                                   NULL);
    } else {

        return __zlg600_frame_proc(handle,
                                   cmd_type,
                                   'B',
                                   tx_desc,
                                   3,
                                   rx_desc,
                                   1,
                                   NULL);
    }
}

/* 卡选择 */
static int __zlg600_iso14443_select (am_zlg600_handle_t handle,
                                     uint8_t            cmd_type,
                                     uint8_t            anticoll_level,
                                     uint32_t           uid,
                                     uint8_t           *p_sak)
{
    am_zlg600_info_buf_desc_t  tx_desc[2];
    am_zlg600_info_buf_desc_t  rx_desc[1];

    tx_desc[0].p_buf = &anticoll_level;
    tx_desc[0].len   = 1;

    /* 已知序列号     */
    tx_desc[1].p_buf = (uint8_t *)&uid;
    tx_desc[1].len   = 4;

    /* 接收的1字节SAK */
    rx_desc[0].p_buf = p_sak;
    rx_desc[0].len   = 1;

    return __zlg600_frame_proc(handle, cmd_type, 'C', tx_desc, 2, rx_desc, 1, NULL);
}

/* 卡挂起 */
static int __zlg600_iso14443_halt (am_zlg600_handle_t handle,
                                   uint8_t            cmd_type)
{
    return __zlg600_frame_proc(handle, cmd_type, 'D', NULL, 0, NULL, 0, NULL);
}

/******************************************************************************/

/* 卡复位 */
static int __zlg600_iso14443_reset (am_zlg600_handle_t  handle,
                                    uint8_t             cmd_type,
                                    uint8_t             time_ms)
{
    am_zlg600_info_buf_desc_t  tx_desc[1];

    tx_desc[0].p_buf = &time_ms;
    tx_desc[0].len   = 1;

    return __zlg600_frame_proc(handle, cmd_type, 'L', tx_desc, 1, NULL, 0, NULL);
}

/* 卡激活*/
static int __zlg600_iso14443_active (am_zlg600_handle_t  handle,
                                     uint8_t             cmd_type,
                                     uint8_t             req_mode,
                                     uint16_t           *p_atq,
                                     uint8_t            *p_saq,
                                     uint8_t            *p_len,
                                     uint8_t            *p_uid,
                                     uint8_t             uid_len)
{
    am_zlg600_info_buf_desc_t  tx_desc[2];
    am_zlg600_info_buf_desc_t  rx_desc[4];

    uint8_t       data = 0x00;                  /* 保留字节，设置为0x00 */

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    /* 第一字节为保留 */
    data = 0x00;
    tx_desc[0].p_buf = &data;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = &req_mode;
    tx_desc[1].len   = 1;

    rx_desc[0].p_buf = (uint8_t *)p_atq;   /* 请求应答（2字节）          */
    rx_desc[0].len   = 2;

    rx_desc[1].p_buf = p_saq;              /* 最后一级选择应答SAK（1字节）*/
    rx_desc[1].len   = 1;

    rx_desc[2].p_buf = p_len;              /* 序列号长度（1字节）        */
    rx_desc[2].len   = 1;

    rx_desc[3].p_buf = p_uid;              /* 序列号（N字节，由序列号长度决定） */
    rx_desc[3].len   = uid_len;            /* 最大10字节，直接按照10字节读取    */

    return __zlg600_frame_proc(handle, cmd_type, 'M', tx_desc, 2, rx_desc, 4, NULL);
}

/*******************************************************************************
   Mifare卡函数
*******************************************************************************/

/* 卡请求 */
int am_zlg600_mifare_card_request (am_zlg600_handle_t handle,
                                   uint8_t            req_mode,
                                   uint16_t          *p_atq)
{
    if (handle == NULL || p_atq == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_request(handle,
                                     0x02,
                                     req_mode,
                                     p_atq);
}

/* 防碰撞 */
int am_zlg600_mifare_card_anticoll(am_zlg600_handle_t handle,
                                   uint8_t            anticoll_level,
                                   uint32_t           know_uid,
                                   uint8_t            nbit_cnt,
                                   uint32_t          *p_uid)
{
    if (handle == NULL || p_uid == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_anticoll(handle,
                                      0x02,
                                      anticoll_level,
                                      know_uid,
                                      nbit_cnt,
                                      p_uid);
}

/* 卡选择 */
int am_zlg600_mifare_card_select(am_zlg600_handle_t handle,
                                 uint8_t            anticoll_level,
                                 uint32_t           uid,
                                 uint8_t           *p_sak)
{
    if (handle == NULL || p_sak == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_select(handle,
                                    0x02,
                                    anticoll_level,
                                    uid,
                                    p_sak);
}

/* 卡挂起 */
int am_zlg600_mifare_card_halt (am_zlg600_handle_t handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_halt(handle, 0x02);
}

/******************************************************************************/

/* E2密钥验证 */
int am_zlg600_mifare_card_e2_authent (am_zlg600_handle_t  handle,
                                      uint8_t             key_type,
                                      uint8_t            *p_uid,
                                      uint8_t             key_sec,
                                      uint8_t             nblock)
{
    am_zlg600_info_buf_desc_t tx_desc[4];

    if (handle == NULL || p_uid == NULL || (key_sec > 7)) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &key_type;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = p_uid;
    tx_desc[1].len   = 4;

    tx_desc[2].p_buf = &key_sec;
    tx_desc[2].len   = 1;

    tx_desc[3].p_buf = &nblock;
    tx_desc[3].len   = 1;

    return __zlg600_frame_proc(handle, 2, 'E', tx_desc, 4, NULL, 0, NULL);
}

/* 直接密钥验证 */
int am_zlg600_mifare_card_direct_authent (am_zlg600_handle_t  handle,
                                          uint8_t             key_type,
                                          uint8_t            *p_uid,
                                          uint8_t            *p_key,
                                          uint8_t             key_len,
                                          uint8_t             nblock)
{
    am_zlg600_info_buf_desc_t tx_desc[4];

    if (handle == NULL || p_uid == NULL || p_key == NULL || ((key_len != 6) && (key_len != 16))) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &key_type;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = p_uid;
    tx_desc[1].len   = 4;

    tx_desc[2].p_buf = p_key;
    tx_desc[2].len   = key_len;

    tx_desc[3].p_buf = &nblock;
    tx_desc[3].len   = 1;

    return __zlg600_frame_proc(handle, 2, 'F', tx_desc, 4, NULL, 0, NULL);
}

/******************************************************************************/

/* 读数据 */
int am_zlg600_mifare_card_read (am_zlg600_handle_t  handle,
                                uint8_t             nblock,
                                uint8_t            *p_buf)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_buf == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &nblock;
    tx_desc[0].len   = 1;

    rx_desc[0].p_buf = p_buf;
    rx_desc[0].len   = 16;

    return __zlg600_frame_proc(handle, 2, 'G', tx_desc, 1, rx_desc, 1, NULL);

}

/* 写数据 */
int am_zlg600_mifare_card_write (am_zlg600_handle_t  handle,
                                 uint8_t             nblock,
                                 uint8_t            *p_buf)
{
    am_zlg600_info_buf_desc_t tx_desc[2];

    if (handle == NULL || p_buf == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &nblock;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = p_buf;
    tx_desc[1].len   = 16;

    return __zlg600_frame_proc(handle, 2, 'H', tx_desc, 2, NULL, 0, NULL);
}

/******************************************************************************/

/* ultralight卡命令, 写数据 */
int am_zlg600_ultralight_card_write (am_zlg600_handle_t  handle,
                                     uint8_t             nblock,
                                     uint8_t            *p_buf)
{
    am_zlg600_info_buf_desc_t tx_desc[2];

    if (handle == NULL || p_buf == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &nblock;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = p_buf;
    tx_desc[1].len   = 4;

    return __zlg600_frame_proc(handle, 2, 'I', tx_desc, 2, NULL, 0, NULL);
}

/******************************************************************************/

/* mifare卡命令——值操作 */
int am_zlg600_mifare_card_val_operate (am_zlg600_handle_t  handle,
                                       uint8_t             mode,
                                       uint8_t             nblock,
                                       uint8_t             ntransblk,
                                       int32_t             value)
{
    am_zlg600_info_buf_desc_t tx_desc[4];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &mode;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = &nblock;
    tx_desc[1].len   = 1;

    tx_desc[2].p_buf = (uint8_t *)&value;
    tx_desc[2].len   = 4;

    tx_desc[3].p_buf = &ntransblk;
    tx_desc[3].len   = 4;

    return __zlg600_frame_proc(handle, 2, 'J', tx_desc, 4, NULL, 0, NULL);
}

/******************************************************************************/

/* 卡复位 */
int am_zlg600_mifare_card_reset (am_zlg600_handle_t  handle,
                                 uint8_t             time_ms)

{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_reset(handle, 0x02, time_ms);
}

/* 卡激活 */
int am_zlg600_mifare_card_active (am_zlg600_handle_t  handle,
                                  uint8_t             req_mode,
                                  uint16_t           *p_atq,
                                  uint8_t            *p_saq,
                                  uint8_t            *p_len,
                                  uint8_t            *p_uid,
                                  uint8_t             uid_len)
{
    if (handle == NULL || p_atq == NULL || p_saq == NULL || p_len == NULL || p_uid == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_active(handle,
                                    0x02,
                                    req_mode,
                                    p_atq,
                                    p_saq,
                                    p_len,
                                    p_uid,
                                    uid_len);
}

/******************************************************************************/

/* mifare卡命令——设置块值的值 */
int am_zlg600_mifare_card_val_set (am_zlg600_handle_t  handle,
                                   uint8_t             nblock,
                                   int32_t             value)
{
    am_zlg600_info_buf_desc_t tx_desc[2];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &nblock;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = (uint8_t *)&value;
    tx_desc[1].len   = 4;

    return __zlg600_frame_proc(handle, 2, 'P', tx_desc, 2, NULL, 0, NULL);
}

/******************************************************************************/

/* 获取块值的值 */
int am_zlg600_mifare_card_val_get (am_zlg600_handle_t  handle,
                                   uint8_t             nblock,
                                   int32_t            *p_value)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_value == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &nblock;
    tx_desc[0].len   = 1;

    rx_desc[0].p_buf = (uint8_t *)p_value;
    rx_desc[0].len   = 4;

    return __zlg600_frame_proc(handle, 2, 'Q', tx_desc, 1, rx_desc, 1, NULL);
}

/******************************************************************************/

/* mifare卡命令——命令传输  */
int am_zlg600_mifare_card_cmd_trans (am_zlg600_handle_t  handle,
                                     uint8_t            *p_tx_buf,
                                     uint8_t             tx_nbytes,
                                     uint8_t            *p_rx_buf,
                                     uint8_t             rx_nbytes)
{
    am_zlg600_info_buf_desc_t tx_desc[2];
    am_zlg600_info_buf_desc_t rx_desc[2];

    if (handle == NULL || p_tx_buf == NULL || p_rx_buf == NULL) {
        return -AM_EINVAL;
    }

    /* 实际传输的命令数据串 */
    tx_desc[1].p_buf = p_tx_buf;
    tx_desc[1].len   = tx_nbytes;

    /* 第一字节为实际数据长度，应该是（实际信息长度 + 1）*/
    tx_nbytes++;
    tx_desc[0].p_buf = &tx_nbytes;
    tx_desc[0].len   = 1;

    /* 实际接收到的信息 */
    rx_desc[0].p_buf = p_rx_buf;
    rx_desc[0].len   = rx_nbytes;

    return __zlg600_frame_proc(handle, 2, 'S', tx_desc, 2, rx_desc, 1, NULL);
}

/* 数据交换 */
int am_zlg600_mifare_card_data_exchange (am_zlg600_handle_t  handle,
                                         uint8_t            *p_data_buf,
                                         uint8_t             len,
                                         uint8_t             wtxm_crc,
                                         uint8_t             fwi,
                                         uint8_t            *p_rx_buf,
                                         uint8_t             buf_size,
                                         uint16_t           *p_real_len)
{
    am_zlg600_info_buf_desc_t tx_desc[3];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_data_buf == NULL || p_rx_buf == NULL || p_real_len == NULL) {
        return -AM_EINVAL;
    }

    /* 交互数据 */
    tx_desc[0].p_buf = p_data_buf;
    tx_desc[0].len   = len;

    tx_desc[1].p_buf = &wtxm_crc;
    tx_desc[1].len   = 1;

    tx_desc[2].p_buf = &fwi;
    tx_desc[2].len   = 1;

    /* 实际接收到的信息 */
    rx_desc[0].p_buf = p_rx_buf;
    rx_desc[0].len   = buf_size;

    return __zlg600_frame_proc(handle, 2, 'X', tx_desc, 3, rx_desc, 1, p_real_len);
}

/* Mifare卡UART自动检测中断回调函数 */
static void __auto_detect_int_callback (void *p_arg)
{
    am_zlg600_serv_t     *p_serv = (am_zlg600_serv_t *)p_arg;
    am_zlg600_uart_dev_t *p_dev  = (am_zlg600_uart_dev_t *)(p_serv->p_cookie);

    /* 禁能触发函数 */
    am_uart_rngbuf_rx_trigger_disable(p_dev->uart_ringbuf_handle);

    /* 主机模式，回调用户函数 */
    if ((p_serv->work_mode & 0xF0) == AM_ZLG600_MODE_MASTER) {
        if (p_serv->pfn_callback != NULL) {
            p_serv->pfn_callback(p_serv->p_arg);
        }
    }
}

/**
 * \brief Mifare卡自动检测回调函数设置
 */
int am_zlg600_mifare_card_auto_detect_cb_set (am_zlg600_handle_t  handle,
                                              am_pfnvoid_t        pfn_callback,
                                              void               *p_arg)
{
    struct am_zlg600_serv *p_serv = handle;

    if (p_serv == NULL || p_arg == NULL) {
        return -AM_ENXIO;
    }

    p_serv->pfn_callback = pfn_callback;
    p_serv->p_arg        = p_arg;

    return AM_OK;
}

/**
 * \brief Mifare卡自动检测断开连接回调函数
 */
int am_zlg600_mifare_card_auto_detect_disconnect (am_zlg600_handle_t  handle,
                                                  am_pfnvoid_t        pfn_callback,
                                                  void               *p_arg)
{
    struct am_zlg600_serv *p_serv = handle;

    if (p_serv == NULL || p_arg == NULL) {
        return -AM_ENXIO;
    }

    if (p_serv->pfn_callback == pfn_callback &&
        p_serv->p_arg        == p_arg) {
        p_serv->pfn_callback = NULL;
        p_serv->p_arg        = NULL;
    }

    return AM_OK;
}

/**
 * \brief 启动Mifare卡自动检测
 * \note 在自动检测期间，若主机发送任何除读自动检测数据外的，且数据长度小于3
 *       的命令，将退出自动检测模式，如请求am_zlg600_mifare_card_active()
 *       命令，在此期间，模块将不接收数据长度大于2的命令
 */
int am_zlg600_mifare_card_auto_detect_start (am_zlg600_handle_t                  handle,
                                             const am_zlg600_auto_detect_info_t *p_info)
{
    am_zlg600_info_buf_desc_t tx_desc[7];

    uint8_t temp_buf;

    uint8_t  tx_desc_num;
    int ret;

    struct am_zlg600_serv *p_serv = handle;

    if (p_serv == NULL || p_info == NULL) {
        return -AM_ENXIO;
    }

    p_serv->p_auto_detect_info = (am_zlg600_auto_detect_info_t *)p_info;

    /* 不自动检测、UART不产生中断、UART主动发送数据 */
    temp_buf = (p_info->ad_mode & 0x80) | 0x01;
    tx_desc[0].p_buf = (uint8_t *)&temp_buf;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = (uint8_t *)&p_info->tx_mode;
    tx_desc[1].len   = 1;

    tx_desc[2].p_buf = (uint8_t *)&p_info->req_mode;
    tx_desc[2].len   = 1;

    tx_desc[3].p_buf = (uint8_t *)&p_info->auth_mode;
    tx_desc[3].len   = 1;

    tx_desc[4].p_buf = (uint8_t *)&p_info->key_type;
    tx_desc[4].len   = 1;

    switch (p_info->auth_mode) {

    case AM_ZLG600_MIFARE_CARD_AUTH_DIRECT :
        tx_desc[5].p_buf = (uint8_t *)p_info->key;
        tx_desc[5].len   = p_info->key_len;

        tx_desc[6].p_buf = (uint8_t *)&p_info->nblock;
        tx_desc[6].len   = 1;

        tx_desc_num = 7;
        break;

    case AM_ZLG600_MIFARE_CARD_AUTH_E2 :
        tx_desc[5].p_buf = (uint8_t *)&p_info->key[0];
        tx_desc[5].len   = 1;

        tx_desc[6].p_buf = (uint8_t *)&p_info->nblock;
        tx_desc[6].len   = 1;

        tx_desc_num = 7;
        break;

    case AM_ZLG600_MIFARE_CARD_AUTH_NO :
        tx_desc_num = 4;
        break;

    default :
        return -AM_EINVAL;
    }

    ret = __zlg600_frame_proc(handle, 2, 'N', tx_desc, tx_desc_num, NULL, 0, NULL);

    p_serv->work_mode = (p_serv->work_mode & 0x0F) | AM_ZLG600_MODE_MASTER;

    /* 使能自动检测触发 */
    ((void (*)(am_zlg600_handle_t, am_bool_t))
            (p_serv->pfn_auto_detect_trigger_set))(p_serv, AM_TRUE);

    /* 标记自动检测 */
    p_serv->auto_detect = 1;

    return ret;
}

/**
 * \brief 关闭Mifare卡自动检测
 */
int am_zlg600_mifare_card_auto_detect_stop (am_zlg600_handle_t handle)
{
    uint8_t  mode, addr;
    int ret;

    struct am_zlg600_serv *p_serv = handle;

    if (p_serv == NULL) {
        return -AM_ENXIO;
    }

    /* 暂时退出主机模式 */
    p_serv->work_mode = (p_serv->work_mode & 0x0F) | AM_ZLG600_MODE_SLAVE;

    ret = am_zlg600_mode_addr_get(handle, &mode, &addr);

    if (ret == AM_OK) {

        /* 禁能自动检测触发 */
        ((void (*)(am_zlg600_handle_t, am_bool_t))
                (p_serv->pfn_auto_detect_trigger_set))(p_serv, AM_FALSE);

    } else {
        p_serv->work_mode = (p_serv->work_mode & 0x0F) | AM_ZLG600_MODE_MASTER;
    }

    /* 去掉自动检测标记 */
    p_serv->auto_detect = 0;

    return ret;
}

/**
 * \brief Mifare卡读取自动检测数据命令
 */
int am_zlg600_mifare_card_auto_detect_read (am_zlg600_handle_t            handle,
                                            am_zlg600_mifare_card_info_t *p_card_info)
{
    am_zlg600_info_buf_desc_t rx_desc[5];
    uint8_t                   abandon[3];

    int ret;
    uint16_t real_len = 0;
    uint8_t  i, j, k;

    struct am_zlg600_serv *p_serv = handle;

    if (p_serv == NULL || p_card_info == NULL) {
        return -AM_ENXIO;
    }

    rx_desc[0].p_buf = (uint8_t *)&p_card_info->tx_mode;
    rx_desc[0].len   = 1;

    rx_desc[1].p_buf = abandon;
    rx_desc[1].len   = 3;

    rx_desc[2].p_buf = (uint8_t *)&p_card_info->uid_len;
    rx_desc[2].len   = 1;

    rx_desc[3].p_buf = (uint8_t *)&p_card_info->uid;
    rx_desc[3].len   = 10;

    rx_desc[4].p_buf = (uint8_t *)&p_card_info->card_data;
    rx_desc[4].len   = 16;

    /* 读自动检测数据命令会暂时退出主机模式 */
    p_serv->work_mode = (p_serv->work_mode & 0x0F) | AM_ZLG600_MODE_SLAVE;

    p_serv->auto_detect_read = 1;
    ret = __zlg600_frame_proc(handle, 2, 'O', NULL, 0, rx_desc, 5, &real_len);
    p_serv->auto_detect_read = 0;

    if (p_serv->p_auto_detect_info->auth_mode == AM_ZLG600_MIFARE_CARD_AUTH_NO) {
        goto __WORK_MODE_UPDATE;
    }

    i = real_len - 1;
    j = sizeof(am_zlg600_mifare_card_info_t) - 1;

    for (k = 0; k < sizeof(p_card_info->card_data); k++) {
        ((uint8_t *)p_card_info)[j] = ((uint8_t *)p_card_info)[i];
        j--;
        i--;
    }

    for (i = p_card_info->uid_len; i < sizeof(p_card_info->uid); i++) {
        p_card_info->uid[i] = 0;
    }

__WORK_MODE_UPDATE :

    /* 禁能自动检测触发 */
    ((void (*)(am_zlg600_handle_t, am_bool_t))
            (p_serv->pfn_auto_detect_trigger_set))(p_serv, AM_FALSE);

    return ret;
}

/*******************************************************************************
  ISO7816-3类命令，接触式IC卡
*******************************************************************************/

/* IC卡复位 */
int am_zlg600_cicc_reset (am_zlg600_handle_t  handle,
                          uint8_t             baudrate_flag,
                          uint8_t            *p_rx_buf,
                          uint8_t             rx_bufsize,
                          uint16_t           *p_real_len)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    int           ret;

    if (handle == NULL || p_rx_buf == NULL || p_real_len == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &baudrate_flag;
    tx_desc[0].len   = 1;

    /* 保留信息16字节丢弃 */
    rx_desc[0].p_buf = NULL;
    rx_desc[0].len   = 16;

    /* 实际信息 */
    rx_desc[0].p_buf = p_rx_buf;
    rx_desc[0].len   = rx_bufsize;

    ret = __zlg600_frame_proc(handle, 5, 'A', tx_desc, 1, rx_desc, 2, p_real_len);

    if (p_real_len != NULL) {
        *p_real_len = *p_real_len - 16;     /* 复位信息的实际长度要减去前面保留的16字节*/
    }

    return ret;
}

/******************************************************************************/

/* IC卡传输协议 */
int am_zlg600_cicc_tpdu (am_zlg600_handle_t  handle,
                         uint8_t            *p_tx_buf,
                         uint16_t            tx_bufsize,
                         uint8_t            *p_rx_buf,
                         uint8_t             rx_bufsize,
                         uint16_t           *p_rx_len)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_tx_buf == NULL || p_rx_buf == NULL || p_rx_len == NULL) {
        return -AM_EINVAL;
    }

    /* 发送数据 */
    tx_desc[0].p_buf = p_tx_buf;
    tx_desc[0].len   = tx_bufsize;

    /* 接收信息 */
    rx_desc[0].p_buf = p_rx_buf;
    rx_desc[0].len   = rx_bufsize;

    return __zlg600_frame_proc(handle, 5, 'B', tx_desc, 1, rx_desc, 1, p_rx_len);
}

/******************************************************************************/

/* IC卡冷复位 */
int am_zlg600_cicc_coldreset (am_zlg600_handle_t  handle,
                              uint8_t             baudrate_flag,
                              uint8_t            *p_rx_buf,
                              uint8_t             rx_bufsize,
                              uint16_t           *p_rx_len)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_rx_buf == NULL || p_rx_len == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &baudrate_flag;
    tx_desc[0].len   = 1;

    rx_desc[0].p_buf = p_rx_buf;
    rx_desc[0].len   = rx_bufsize;

    return __zlg600_frame_proc(handle, 5, 'C', tx_desc, 1, rx_desc, 1, p_rx_len);
}

/******************************************************************************/

/* ISO7816-3类命令-IC卡热复位 */
int am_zlg600_cicc_warmreset (am_zlg600_handle_t  handle,
                              uint8_t             baudrate_flag,
                              uint8_t            *p_rx_buf,
                              uint8_t             rx_bufsize,
                              uint16_t           *p_rx_len)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &baudrate_flag;
    tx_desc[0].len   = 1;

    rx_desc[0].p_buf = p_rx_buf;
    rx_desc[0].len   = rx_bufsize;

    return __zlg600_frame_proc(handle, 5, 'D', tx_desc, 1, rx_desc, 1, p_rx_len);
}

/* IC卡停活   */
int am_zlg600_cicc_deactive (am_zlg600_handle_t  handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_frame_proc(handle, 5, 'E', NULL, 0, NULL, 0, NULL);
}

/******************************************************************************/

/* PPS */
int am_zlg600_cicc_pps (am_zlg600_handle_t  handle,
                        uint8_t            *p_pps)
{
    am_zlg600_info_buf_desc_t tx_desc[1];

    if (handle == NULL || p_pps == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = p_pps;
    tx_desc[0].len   = 4;

    return __zlg600_frame_proc(handle, 5, 'F', tx_desc, 1, NULL, 0, NULL);
}

/******************************************************************************/

/* ISO7816-3类命令-IC卡传输协议(T = 0) */
int am_zlg600_cicc_tpdu_tp0 (am_zlg600_handle_t  handle,
                             uint8_t            *p_tx_buf,
                             uint16_t            tx_bufsize,
                             uint8_t            *p_rx_buf,
                             uint8_t             rx_bufsize,
                             uint16_t           *p_rx_len)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_tx_buf == NULL || p_rx_buf == NULL || p_rx_len == NULL) {
        return -AM_EINVAL;
    }
    /* 发送数据 */
    tx_desc[0].p_buf = p_tx_buf;
    tx_desc[0].len   = tx_bufsize;

    /* 接收信息 */
    rx_desc[0].p_buf = p_rx_buf;
    rx_desc[0].len   = rx_bufsize;

    return __zlg600_frame_proc(handle, 5, 'G', tx_desc, 1, rx_desc, 1, p_rx_len);
}

/* ISO7816-3类命令-IC卡传输协议(T = 1) */
int am_zlg600_cicc_tpdu_tp1 (am_zlg600_handle_t  handle,
                             uint8_t            *p_tx_buf,
                             uint16_t            tx_bufsize,
                             uint8_t            *p_rx_buf,
                             uint8_t             rx_bufsize,
                             uint16_t           *p_rx_len)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_tx_buf == NULL || p_rx_buf == NULL || p_rx_len == NULL) {
        return -AM_EINVAL;
    }

    /* 发送数据 */
    tx_desc[0].p_buf = p_tx_buf;
    tx_desc[0].len   = tx_bufsize;

    /* 接收信息 */
    rx_desc[0].p_buf = p_rx_buf;
    rx_desc[0].len   = rx_bufsize;

    return __zlg600_frame_proc(handle, 5, 'H', tx_desc, 1, rx_desc, 1, p_rx_len);
}

/*******************************************************************************
   PICCA卡类函数
*******************************************************************************/

/* PICCA卡命令——请求    */
int am_zlg600_picca_request (am_zlg600_handle_t handle,
                             uint8_t            req_mode,
                             uint16_t          *p_atq)
{
    if (handle == NULL || p_atq == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_request(handle, 6, req_mode, p_atq);
}

/* PICCA卡命令——防碰撞  */
int am_zlg600_picca_anticoll (am_zlg600_handle_t handle,
                              uint8_t            anticoll_level,
                              uint32_t           know_uid,
                              uint8_t            nbit_cnt,
                              uint32_t          *p_uid)
{
    if (handle == NULL || p_uid == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_anticoll(handle,
                                      0x06,
                                      anticoll_level,
                                      know_uid,
                                      nbit_cnt,
                                      p_uid);
}

/* PICCA卡命令——卡选择   */
int am_zlg600_picca_select (am_zlg600_handle_t handle,
                            uint8_t            anticoll_level,
                            uint32_t           uid,
                            uint8_t           *p_sak)
{
    if (handle == NULL || p_sak == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_select(handle,
                                    0x06,
                                    anticoll_level,
                                    uid,
                                    p_sak);
}

/* PICCA卡命令——卡挂起   */
int am_zlg600_picca_halt (am_zlg600_handle_t handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_halt(handle, 0x06);
}


/* PICC A卡命令——RATS */
int am_zlg600_picca_rats (am_zlg600_handle_t  handle,
                          uint8_t             cid,
                          uint8_t            *p_ats_buf,
                          uint8_t             bufsize,
                          uint16_t           *p_rx_len)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_ats_buf == NULL || p_rx_len == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &cid;
    tx_desc[0].len   = 1;

    rx_desc[0].p_buf = p_ats_buf;
    rx_desc[0].len   = bufsize;

    return __zlg600_frame_proc(handle, 6, 'E', tx_desc, 1, rx_desc, 1, p_rx_len);
}

/* PICC A卡命令——PPS */
int am_zlg600_picca_pps (am_zlg600_handle_t  handle,
                         uint8_t             dsi_dri)
{
    am_zlg600_info_buf_desc_t tx_desc[1];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &dsi_dri;
    tx_desc[0].len   = 1;

    return __zlg600_frame_proc(handle, 6, 'F', tx_desc, 1, NULL, 0, NULL);
}

/* PICC A卡命令——解除卡激活 */
int am_zlg600_picca_deselect (am_zlg600_handle_t  handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_frame_proc(handle, 6, 'G', NULL, 0, NULL, 0, NULL);
}

/******************************************************************************/

/* PICC A卡命令——T=CL */
int am_zlg600_picc_tpcl (am_zlg600_handle_t  handle,
                         uint8_t            *p_cos_buf,
                         uint8_t             cos_bufsize,
                         uint8_t            *p_res_buf,
                         uint8_t             res_bufsize,
                         uint16_t           *p_rx_len)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_cos_buf == NULL || p_res_buf == NULL || p_rx_len == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = p_cos_buf;
    tx_desc[0].len   = cos_bufsize;

    rx_desc[0].p_buf = p_res_buf;
    rx_desc[0].len   = res_bufsize;

    return __zlg600_frame_proc(handle, 6, 'H', tx_desc, 1, rx_desc, 1, p_rx_len);
}

/* PICC A卡命令——数据交换 */
int am_zlg600_picc_data_exchange (am_zlg600_handle_t  handle,
                                  uint8_t            *p_data_buf,
                                  uint8_t             len,
                                  uint8_t             wtxm_crc,
                                  uint8_t             fwi,
                                  uint8_t            *p_rx_buf,
                                  uint8_t             bufsize,
                                  uint16_t           *p_rx_len)
{
    am_zlg600_info_buf_desc_t tx_desc[3];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_data_buf == NULL || p_rx_buf == NULL || p_rx_len == NULL) {
        return -AM_EINVAL;
    }

    /* 交互数据 */
    tx_desc[0].p_buf = p_data_buf;
    tx_desc[0].len   = len;

    tx_desc[1].p_buf = &wtxm_crc;
    tx_desc[1].len   = 1;

    tx_desc[2].p_buf = &fwi;
    tx_desc[2].len   = 1;

    /* 实际接收到的信息 */
    rx_desc[0].p_buf = p_rx_buf;
    rx_desc[0].len   = bufsize;

    return __zlg600_frame_proc(handle, 6, 'J', tx_desc, 3, rx_desc, 1, p_rx_len);
}

/* PICC A卡命令——卡复位 */
int am_zlg600_picca_reset (am_zlg600_handle_t  handle,
                           uint8_t             time_ms)

{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_reset(handle, 0x06, time_ms);
}

/* PICC A卡命令——卡激活 */
int am_zlg600_picca_active (am_zlg600_handle_t  handle,
                            uint8_t             req_mode,
                            uint16_t           *p_atq,
                            uint8_t            *p_saq,
                            uint8_t            *p_len,
                            uint8_t            *p_uid,
                            uint8_t             uid_len)
{
    if (handle == NULL || p_atq == NULL || p_saq == NULL || p_len == NULL || p_uid == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_iso14443_active(handle,
                                    0x06,
                                    req_mode,
                                    p_atq,
                                    p_saq,
                                    p_len,
                                    p_uid,
                                    uid_len);
}

/*******************************************************************************
  PICC B型卡
*******************************************************************************/

/* B型卡激活 */
int am_zlg600_piccb_active (am_zlg600_handle_t  handle,
                            uint8_t             req_mode,
                            uint8_t            *p_uid)
{
    am_zlg600_info_buf_desc_t tx_desc[2];
    am_zlg600_info_buf_desc_t rx_desc[1];

    uint8_t  data = 0x00;  /* 应用标识（1字节）：默认为0x00 */

    if (handle == NULL || p_uid == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &req_mode;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = &data;
    tx_desc[1].len   = 1;

    rx_desc[0].p_buf = p_uid;
    rx_desc[0].len   = 12;

    return __zlg600_frame_proc(handle, 6, 'N', tx_desc, 2, rx_desc, 1, NULL);
}

/* PICC B卡命令——卡复位 */
int am_zlg600_piccb_reset (am_zlg600_handle_t  handle,
                           uint8_t             time_ms)

{
    am_zlg600_info_buf_desc_t tx_desc[1];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &time_ms;
    tx_desc[0].len   = 1;

    return __zlg600_frame_proc(handle, 6, 'O', tx_desc, 1, NULL, 0, NULL);
}

/* PICC B卡命令——卡请求 */
int am_zlg600_piccb_request (am_zlg600_handle_t handle,
                             uint8_t            req_mode,
                             uint8_t            slot_time,
                             uint8_t           *p_uid)
{
    am_zlg600_info_buf_desc_t tx_desc[3];
    am_zlg600_info_buf_desc_t rx_desc[1];

    uint8_t data = 0x00;  /* 应用标识（1字节）：默认为0x00 */

    if (handle == NULL || p_uid == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &req_mode;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = &data;
    tx_desc[1].len   = 1;

    tx_desc[2].p_buf = &slot_time;
    tx_desc[2].len   = 1;

    rx_desc[0].p_buf = p_uid;
    rx_desc[0].len   = 12;

    return __zlg600_frame_proc(handle, 6, 'P', tx_desc, 3, rx_desc, 1, NULL);
}

int am_zlg600_piccb_anticoll(am_zlg600_handle_t  handle,
                             uint8_t             slot_flag,
                             uint8_t            *p_uid)
{
    am_zlg600_info_buf_desc_t tx_desc[1];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if (handle == NULL || p_uid == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &slot_flag;
    tx_desc[0].len   = 1;

    rx_desc[0].p_buf = p_uid;
    rx_desc[0].len   = 12;

    return __zlg600_frame_proc(handle, 6, 'Q', tx_desc, 1, rx_desc, 1, NULL);
}

/******************************************************************************/

/* B型卡修改传输属性 */
int am_zlg600_piccb_attrib_set (am_zlg600_handle_t handle,
                                uint8_t           *p_pupi,
                                uint8_t            cid,
                                uint8_t            protype)
{
    am_zlg600_info_buf_desc_t tx_desc[3];

    if (handle == NULL || p_pupi == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = p_pupi;
    tx_desc[0].len   = 4;

    tx_desc[1].p_buf = &cid;
    tx_desc[1].len   = 1;

    tx_desc[2].p_buf = &protype;
    tx_desc[2].len   = 1;

    return __zlg600_frame_proc(handle, 6, 'R', tx_desc, 3, NULL, 0, NULL);
}

/* B型卡 HALT */
int am_zlg600_piccb_halt (am_zlg600_handle_t handle,
                          uint8_t           *p_pupi)
{
    am_zlg600_info_buf_desc_t tx_desc[1];

    if (handle == NULL || p_pupi == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = p_pupi;
    tx_desc[0].len   = 4;

    return __zlg600_frame_proc(handle, 6, 'S', tx_desc, 1, NULL, 0, NULL);
}

/*******************************************************************************
  PLUS CPU 卡
*******************************************************************************/

/* PLUS CPU 卡命令-个人化更新数据 */
int am_zlg600_pluscpu_persotcl_write (am_zlg600_handle_t  handle,
                                      uint16_t            sector_addr,
                                      uint8_t            *p_buf)
{
    am_zlg600_info_buf_desc_t tx_desc[2];

    if ((handle == NULL) || (p_buf == NULL)) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = (uint8_t *)&sector_addr;
    tx_desc[0].len   = 2;

    tx_desc[1].p_buf = p_buf;
    tx_desc[1].len   = 16;

    return __zlg600_frame_proc(handle, 7, 'B', tx_desc, 2, NULL, 0, NULL);
}

/* PLUS CPU 卡命令-提交个人化 */
int am_zlg600_pluscpu_persotcl_commit (am_zlg600_handle_t  handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_frame_proc(handle, 7, 'C', NULL, 0, NULL, 0, NULL);
}

/******************************************************************************/

/* PLUS CPU 卡命令-SL3首次直接验证 */
int am_zlg600_pluscpu_sl3_first_direct_authent (am_zlg600_handle_t  handle,
                                                uint16_t            authent_addr,
                                                uint8_t            *p_key)
{
    am_zlg600_info_buf_desc_t tx_desc[2];

    if ((handle == NULL) || (p_key == NULL)) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = (uint8_t *)&authent_addr;
    tx_desc[0].len   = 2;

    tx_desc[1].p_buf = p_key;
    tx_desc[1].len   = 16;

    return __zlg600_frame_proc(handle, 7, 'J', tx_desc, 2, NULL, 0, NULL);
}

/*  PLUS CPU 卡命令-SL3首次E2验证 */
int am_zlg600_pluscpu_sl3_first_e2_authent (am_zlg600_handle_t  handle,
                                            uint16_t            authent_addr,
                                            uint8_t             key_sector)
{
    am_zlg600_info_buf_desc_t tx_desc[2];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = (uint8_t *)&authent_addr;
    tx_desc[0].len   = 2;

    tx_desc[1].p_buf = &key_sector;
    tx_desc[1].len   = 1;

    return __zlg600_frame_proc(handle, 7, 'K', tx_desc, 2, NULL, 0, NULL);
}

/******************************************************************************/

/* PLUS CPU 卡命令-SL3直接跟随验证 */
int am_zlg600_pluscpu_sl3_follow_direct_authent (am_zlg600_handle_t  handle,
                                                 uint16_t            authent_addr,
                                                 uint8_t            *p_key)
{    am_zlg600_info_buf_desc_t tx_desc[2];

    if ((handle == NULL) || (p_key == NULL)) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = (uint8_t *)&authent_addr;
    tx_desc[0].len   = 2;

    tx_desc[1].p_buf = p_key;
    tx_desc[1].len   = 16;

    return __zlg600_frame_proc(handle, 7, 'L', tx_desc, 2, NULL, 0, NULL);
}

/* PLUS CPU 卡命令-SL3首次E2验证 */
int am_zlg600_pluscpu_sl3_follow_e2_authent (am_zlg600_handle_t  handle,
                                             uint16_t            authent_addr,
                                             uint8_t             key_sector)
{
    am_zlg600_info_buf_desc_t tx_desc[2];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = (uint8_t *)&authent_addr;
    tx_desc[0].len   = 2;

    tx_desc[1].p_buf = &key_sector;
    tx_desc[1].len   = 1;

    return __zlg600_frame_proc(handle, 7, 'M', tx_desc, 2, NULL, 0, NULL);
}

/******************************************************************************/

/* PLUS CPU 卡命令-SL3复位验证  */
int am_zlg600_pluscpu_sl3_reset_authent (am_zlg600_handle_t  handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg600_frame_proc(handle, 7, 'N', NULL, 0, NULL, 0, NULL);
}

/******************************************************************************/

/* PLUS CPU 卡命令-SL3读  */
int am_zlg600_pluscpu_sl3_read (am_zlg600_handle_t  handle,
                                uint8_t             read_mode,
                                uint16_t            start_block,
                                uint8_t             block_num,
                                uint8_t            *p_buf)
{
    am_zlg600_info_buf_desc_t tx_desc[3];
    am_zlg600_info_buf_desc_t rx_desc[1];

    if ((handle == NULL) || (p_buf == NULL)) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &read_mode;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = (uint8_t *)&start_block;
    tx_desc[1].len   = 2;

    tx_desc[2].p_buf = &block_num;
    tx_desc[2].len   = 1;

    rx_desc[0].p_buf = p_buf;
    rx_desc[0].len   = block_num * 16;

    return __zlg600_frame_proc(handle, 7, 'O', tx_desc, 3, rx_desc, 1, NULL);
}

/* PLUS CPU 卡命令-SL3写 */
int am_zlg600_pluscpu_sl3_write (am_zlg600_handle_t  handle,
                                 uint8_t             write_mode,
                                 uint16_t            start_block,
                                 uint8_t             block_num,
                                 uint8_t            *p_buf)
{
    am_zlg600_info_buf_desc_t tx_desc[4];

    if ((handle == NULL) || (p_buf == NULL)) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &write_mode;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = (uint8_t *)&start_block;
    tx_desc[1].len   = 2;

    tx_desc[2].p_buf = &block_num;
    tx_desc[2].len   = 1;

    tx_desc[3].p_buf = p_buf;
    tx_desc[3].len   = block_num * 16;

    return __zlg600_frame_proc(handle, 7, 'P', tx_desc, 4, NULL, 0, NULL);
}

/******************************************************************************/

/* PLUS CPU 卡命令-值块操作 */
int am_zlg600_pluscpu_val_operate (am_zlg600_handle_t  handle,
                                   uint8_t             mode,
                                   uint16_t            src_block,
                                   uint16_t            dst_block,
                                   int32_t             value)
{
    am_zlg600_info_buf_desc_t tx_desc[4];

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &mode;
    tx_desc[0].len   = 1;

    tx_desc[1].p_buf = (uint8_t *)&src_block;
    tx_desc[1].len   = 2;

    tx_desc[2].p_buf = (uint8_t *)&dst_block;
    tx_desc[2].len   = 2;

    tx_desc[3].p_buf = (uint8_t *)&value;
    tx_desc[3].len   = 4;

    return __zlg600_frame_proc(handle, 7, 'S', tx_desc, 4, NULL, 0, NULL);
}

/******************************************************************************/

/* 读取身份证信息 */
int am_zlg600_id_card_info_get (am_zlg600_handle_t handle, uint8_t req_mode, uint8_t *p_info)
{
    am_zlg600_info_buf_desc_t  tx_desc[1];
    am_zlg600_info_buf_desc_t  rx_desc[1];

    uint8_t cmd_type  = 0x06;       /* 指令类型 */
    uint8_t atq       = req_mode;   /* 请求代码 */

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    tx_desc[0].p_buf = &atq;
    tx_desc[0].len   = 1;

    rx_desc[0].p_buf = p_info;
    rx_desc[0].len   = 8;

    return __zlg600_frame_proc(handle, cmd_type, 'T', tx_desc, 1, rx_desc, 1, NULL);
}

/******************************************************************************/

/* 设置指令超时时间 */
int am_zlg600_cmd_timeout_set (am_zlg600_handle_t handle,
                               uint32_t           timeout_ms)
{
    return ((int (*)(am_zlg600_handle_t, uint32_t))
            (handle->pfn_cmd_timeout_set))(handle, timeout_ms);
}

/* end of file */
