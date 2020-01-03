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
 * \brief GS200 driver implement
 *
 * \internal
 * \par Modification History
 * - 1.00 19-12-16  fzb, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_gs200.h"
#include "am_int.h"
#include "am_uart.h"
#include "am_uart_rngbuf.h"
#include "am_vdebug.h"
#include "am_wait.h"
#include "string.h"

/*
 *  1.GS200手势检测模块默认采用9600的波特率与主机进行通讯，同时支持4800、115200的波特率
 *         进行通讯；在进行波特率修改时，若指令无误，模块将会在1ms内返回相同的指令，若
 *         模块修改新设置的波特率参数成功，模块将在10ms后以新设置的波特率再次返回相同的
 *         设置指令。
 *
 *  注意：模块掉电后不保存已经设置的波特率，重新上电后默认波特率为9600。
 *
 *  2.用户在实际使用时，可以根据具体的需求对需要的手势进行禁能和使能的配置。上电后
 *      默认上、下、左、右手势动作使能，悬停手势默认禁能。
 *
 *  3.悬停手势指令输出默认采用保持悬停动作500ms，间隔100ms，当采用悬停定时输出功能时，
 *      可以更改定时输出时间间隔；采用单次触发输出模式时，可以更改单次触发的时间间隔
        （软件默认最小时间为200ms）
 */

/* \brief   命令接收解析状态机  */
#define __GS200_CMD_PROC_STAT_NONE              0       /* 无命令流程       */
#define __GS200_CMD_PROC_STAT_START             1       /* 命令流程启动     */
#define __GS200_CMD_PROC_STAT_SEND              2       /* 命令发送         */
#define __GS200_CMD_PROC_STAT_RECV_HEAD         3       /* 接收到帧头       */
#define __GS200_CMD_PROC_STAT_RECV_FRAMEID      4       /* 接收到帧标识     */
#define __GS200_CMD_PROC_STAT_RECV_DATA1        5       /* 接收到数据1      */
#define __GS200_CMD_PROC_STAT_RECV_DATA2        6       /* 接收到数据2      */
#define __GS200_CMD_PROC_STAT_RECV_END          7       /* 命令接收完成     */
#define __GS200_CMD_PROC_STAT_RECV_OVERFLOW     8       /* 接收命令溢出     */
#define __GS200_CMD_PROC_STAT_RECV_ERR          9       /* 接收命令错误     */

/*
 * \brief   通信协议帧格式  
 * 
 *      帧头            帧标识          帧数据1         帧数据2         帧校验CRC-8
 *    1Byte(0x1A)       1Byte           1Byte           1Byte           1Byte
 *
 */

#define __GS200_COM_FRAME_HEAD                  (0x1A)  /* 帧头 */

/* 指令(帧标识) */
#define __GS200_CMD_GESTURE_REPORTING           (0x00)  /* 手势主动上报功能指令             */
#define __GS200_CMD_SET_BAUDRATE                (0x01)  /* 设置模块通讯接口波特率指令       */
#define __GS200_CMD_SET_GESTURE_STATE           (0x02)  /* 设置模块手势功能使能与禁能指令   */
#define __GS200_CMD_SET_WORKING_CONDITION       (0x03)  /* 设置工作状态指令                 */
#define __GS200_CMD_WORKING_STATUS_INDICATOR    (0x04)  /* 模块工作状态指示指令             */
#define __GS200_CMD_SET_HOVERING_GESTURE        (0x0A)  /* 设置悬停手势输出指令             */

/* 设置悬停手势输出使能 */
#define __GS200_SET_OUTPUT_TIMING_ENABLE        (0x02)  /* 设置GS200悬停指令定时输出使能 */
#define __GS200_SET_OUTPUT_SINGLE_ENABLE        (0x03)  /* 设置GS200悬停指令单次输出使能 */

/*******************************************************************************
* 函数声明
*******************************************************************************/

/** 
 * \brief GS200 帧数据校验
 */
am_local uint8_t __gs200_frame_check(uint8_t *ptr, uint32_t len);

/** 
 * \brief GS200 命令发送
 */
am_local int __gs200_cmd_send (am_gs200_dev_t   *p_this,
                               uint8_t           frame_id,      //帧标识
                               uint8_t           frame_data1,   //帧数据位1
                               uint8_t           frame_data2,   //帧数据位2
                               am_bool_t         need_check);

/** 
 * \brief GS200 命令字节发送
 */
am_local int __gs200_cmd_char_send(am_gs200_dev_t   *p_this,
                                   char              ch);

/** 
 * \brief GS200 数据发送
 */
am_local int __gs200_data_send (am_gs200_dev_t      *p_this,
                                const uint8_t       *p_buf,
                                int                  len);

/** 
 * \brief GS200 发送回调函数
 */
am_local int __gs200_uart_txchar_get (void *p_arg, char *p_outchar);

/** 
 * \brief GS200 发送结果 (等待发送完成)
 */
am_local int __gs200_cmd_result (am_gs200_dev_t *p_this,
                                 uint8_t         frame_id,      //帧标识
                                 am_bool_t       need_check);

/** 
 * \brief GS200 接收回调函数
 */
am_local int __gs200_uart_rxchar_put (void *p_arg, char inchar);

/** 
 * \brief GS200 主机发送完成后,从机回复
 */
am_local int __gs200_cmd_ack_recv_proc (am_gs200_dev_t *p_this, char inchar);

/** 
 * \brief GS200 发送结果信息获取 (包含校验)
 */
am_local int __gs200_cmd_result_info_get (am_gs200_dev_t    *p_this,
                                          uint8_t            frame_id,      //帧标识
                                          am_bool_t          need_check);

/** 
 * \brief GS200 数据接收
 */
am_local int __gs200_data_recv (am_gs200_dev_t      *p_this,
                                uint8_t             *p_buf,
                                int                  len);


/******************************************************************************/

/** 
 * \brief GS200 帧数据校验
 */
am_local uint8_t __gs200_frame_check(uint8_t *ptr, uint32_t len)
{
    uint8_t crc, i;

    crc = 0xFF;
    while(len--) {
        crc ^= *ptr++;
        for(i = 0; i < 8; i++) {
            if (crc &0x80) {
                crc = (crc << 1) ^ 0x131;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

/******************************************************************************/

/** 
 * \brief GS200 命令发送
 */
am_local int __gs200_cmd_send (am_gs200_dev_t   *p_this,
                               uint8_t           frame_id,      //帧标识
                               uint8_t           frame_data1,   //帧数据位1
                               uint8_t           frame_data2,   //帧数据位2
                               am_bool_t         need_check)
{
    uint8_t i;

    p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_START;

    /* The tx buffer temp for receive cmd ack, only used after send complete */
    p_this->p_cmd_rxbuf     = p_this->p_devinfo->p_uart_txbuf;
    p_this->cmd_rxbuf_len   = p_this->p_devinfo->txbuf_size;
    p_this->cmd_rx_len      = 0;

    /* 命令通信帧配置 */
    p_this->p_devinfo->p_cmd_txbuf[0]  = __GS200_COM_FRAME_HEAD;
    p_this->p_devinfo->p_cmd_txbuf[1]  = frame_id;
    p_this->p_devinfo->p_cmd_txbuf[2]  = frame_data1;
    p_this->p_devinfo->p_cmd_txbuf[3]  = frame_data2;

    if (need_check == AM_TRUE) {    //帧校验字节配置
        p_this->p_devinfo->p_cmd_txbuf[4] = __gs200_frame_check(&p_this->p_devinfo->p_cmd_txbuf[1], 3);//帧校验字节配置
    }

    /* 此处选择命令字节发送是因为发送两个字节之间的信号要有200us以上的时间间隔 */
    for (i = 0; i < p_this->p_devinfo->cmd_txbuf_len; i++) {
        __gs200_cmd_char_send(p_this, p_this->p_devinfo->p_cmd_txbuf[i]);
    }

    return AM_OK;
}

/******************************************************************************/

/** 
 * \brief GS200 命令字节发送
 */
am_local int __gs200_cmd_char_send(am_gs200_dev_t   *p_this,
                                   char              ch)
{
    return __gs200_data_send(p_this, (const uint8_t *)(&ch), 1);
}

/******************************************************************************/

/** 
 * \brief GS200 数据发送
 */
am_local int __gs200_data_send (am_gs200_dev_t      *p_this,
                                const uint8_t       *p_buf,
                                int                  len)
{
    am_rngbuf_t rb     = &(p_this->tx_rngbuf);
    uint32_t    nbytes = len;

    uint32_t rb_ct, write_ct;

    if (am_rngbuf_isfull(rb) == AM_TRUE) {      //环形缓冲区是否填满
        return -AM_ENOSPC;                      //设备剩余空间不足
    }

    rb_ct = am_rngbuf_freebytes(rb);            //获取环形缓冲区空闲空间大小
    write_ct = (rb_ct > len) ? len : rb_ct;

    am_rngbuf_put(rb, (const char *)p_buf, write_ct);//存放若干字节到环形缓冲区

    p_buf += write_ct;
    len   -= write_ct;

    am_uart_tx_startup(p_this->uart_handle);    //启动UART中断模式数据传输

    return nbytes - len;
}

/******************************************************************************/

/** 
 * \brief GS200 发送回调函数
 */
am_local int __gs200_uart_txchar_get (void *p_arg, char *p_outchar)
{
    am_gs200_dev_t  *p_dev      =   (am_gs200_dev_t *)p_arg;
    am_rngbuf_t     rb          =   &(p_dev->tx_rngbuf);
    uint32_t        key         =   am_int_cpu_lock();

    /* Success pop one data from ring buffer */
    if (am_rngbuf_getchar(rb, p_outchar) == 1) {    //从环形缓冲区取出一个字节数据
        am_int_cpu_unlock(key);     //1:数据成功取出    0:数据取出失败，缓冲区为空
        return AM_OK;
    }

    if (p_dev->cmd_proc_state == __GS200_CMD_PROC_STAT_START) {

        p_dev->cmd_proc_state = __GS200_CMD_PROC_STAT_SEND;

        /* send complete  */
        am_wait_done(&p_dev->ack_wait);
    }

    am_int_cpu_unlock(key);

    return -AM_EEMPTY;     /* No data to transmit,return -AM_EEMPTY */
}

/******************************************************************************/

/** 
 * \brief GS200 发送结果 (等待发送完成)
 */
am_local int __gs200_cmd_result (am_gs200_dev_t *p_this,
                                 uint8_t         frame_id,      //帧标识
                                 am_bool_t       need_check)
{
    int          key;

    /* 等待发送完成 */
    am_wait_on(&p_this->ack_wait);

    /* 一帧数据要在10ms内接收完毕，否则判断为帧数据接收超时 */
    am_softimer_start(&p_this->timer, 10);

    am_rngbuf_init(&(p_this->tx_rngbuf),
                    (char *)p_this->p_devinfo->p_uart_txbuf,
                    p_this->p_devinfo->txbuf_size);

    while (1) {

        /* wait 20ms */
        if (am_wait_on_timeout(&p_this->ack_wait, 20) == AM_OK) {

            key = am_int_cpu_lock();

            /* success process */
            if (p_this->cmd_proc_state == __GS200_CMD_PROC_STAT_RECV_END) {

                p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_NONE;
                am_int_cpu_unlock(key);

                return __gs200_cmd_result_info_get(p_this,
                                                   frame_id,
                                                   need_check);
            }

            if (p_this->cmd_proc_state != __GS200_CMD_PROC_STAT_RECV_ERR) {

                am_int_cpu_unlock(key);
                continue;
            }

            am_int_cpu_unlock(key);
        }

        /* run to here, some error happen */
        key = am_int_cpu_lock();

        /* move the data to rx rngbuf */
        if (p_this->cmd_rx_len != 0) {
            am_rngbuf_put(&(p_this->rx_rngbuf),
                          (const char *)(p_this->p_cmd_rxbuf),
                           p_this->cmd_rx_len);

            am_wait_done(&(p_this->rx_wait));
        }

        p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_NONE;

        am_int_cpu_unlock(key);

        return AM_ERROR;
    }
}

/******************************************************************************/

/** 
 * \brief GS200 接收回调函数
 */
am_local int __gs200_uart_rxchar_put (void *p_arg, char inchar)
{
    am_gs200_dev_t *p_dev  = (am_gs200_dev_t *)p_arg;
    am_rngbuf_t     rx_rb  = &(p_dev->rx_rngbuf);

    if (p_dev->cmd_proc_state >= __GS200_CMD_PROC_STAT_SEND) {
        /* success process by cmd ack */
        if (__gs200_cmd_ack_recv_proc(p_dev, inchar) == AM_OK) {
             return AM_OK;
        }
    }

    /* wait done */
    am_wait_done(&p_dev->rx_wait);      //结束之前开启的接收数据的等待

    if (am_rngbuf_putchar(rx_rb, inchar) == 1) {        /* 存放一个字节到环形缓冲区 */
        return AM_OK;                                   //1:数据成功存放    0:数据存放失败，缓冲区满
    }

    return -AM_EFULL;          /* No data to receive,return -AM_EFULL */
}

/******************************************************************************/

/** 
 * \brief GS200 主机发送完成后,从机回复
 */
am_local int __gs200_cmd_ack_recv_proc (am_gs200_dev_t *p_this, char inchar)
{
    int ret = AM_ERROR;

    switch (p_this->cmd_proc_state) {

    case __GS200_CMD_PROC_STAT_SEND:  /* Cmd send complete       */
        memset(p_this->p_cmd_rxbuf, 0 , 10);
        ret = AM_OK;
        p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
        p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_RECV_HEAD;           //接收到帧头
        break;

    case __GS200_CMD_PROC_STAT_RECV_HEAD:
        ret = AM_OK;
        p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
        p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_RECV_FRAMEID;        //接收到帧标识
        break;

    case __GS200_CMD_PROC_STAT_RECV_FRAMEID:
        ret = AM_OK;
        p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
        p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_RECV_DATA1;          //接收到数据1
        break;

    case __GS200_CMD_PROC_STAT_RECV_DATA1:
        ret = AM_OK;
        p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
        p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_RECV_DATA2;          //接收到数据2
        break;

    case __GS200_CMD_PROC_STAT_RECV_DATA2:
        ret = AM_OK;
        p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
        p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_RECV_END;            //命令接收完成
        am_softimer_stop(&p_this->timer);       //关闭接收超时检测定时器
        break;

    default:
        ret = -AM_ESRCH;
        p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_RECV_ERR;            //接收命令错误
        break;
    }

    if (ret != AM_OK) {

        if (p_this->cmd_rx_len != 0) {  /* not empty, error happen */
            p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_RECV_ERR;        //接收命令错误
        }
    } else {

        /* full */
        if ((p_this->cmd_rx_len     == p_this->cmd_rxbuf_len) &&
            (p_this->cmd_proc_state != __GS200_CMD_PROC_STAT_RECV_END)) {

            p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_RECV_ERR;        //接收命令错误
        }
    }

    /* receive one ack data */
    am_wait_done(&p_this->ack_wait);

    return ret;
}

/******************************************************************************/

/** 
 * \brief GS200 发送结果信息获取 (包含校验)
 */
am_local int __gs200_cmd_result_info_get (am_gs200_dev_t    *p_this,
                                          uint8_t            frame_id,      //帧标识
                                          am_bool_t          need_check)
{
    uint8_t i;

    if (p_this->p_cmd_rxbuf[0] != __GS200_COM_FRAME_HEAD) {
        return AM_ERROR;            //帧头错误
    }

    if (need_check == AM_TRUE) {    //进行帧校验判断
        if (__gs200_frame_check(&(p_this->p_cmd_rxbuf[1]), 4) == 0) {
            if (frame_id == __GS200_CMD_SET_WORKING_CONDITION) {    //帧标识为设置工作状态指令
                //此处还可添加模块切换状态的指令
                return AM_OK;       //接收到的帧数据无误，校验通过
            }
        } else {
            return AM_ERROR;        //校验失败，返回错误
        }
    }

    for (i = 0; i < p_this->cmd_rx_len; i++) {
        if (p_this->p_cmd_rxbuf[i] != p_this->p_devinfo->p_cmd_txbuf[i]) {
            break;
        }
    }
    if (i == p_this->cmd_rx_len) {
        return AM_OK;       //发送数据与从机回复数据一致
    } else {
        return AM_ERROR;    //发送数据与从机回复数据不一致
    }

}

/******************************************************************************/

/** 
 * \brief GS200 数据接收
 */
am_local int __gs200_data_recv (am_gs200_dev_t      *p_this,
                                uint8_t             *p_buf,
                                int                  len)
{
    am_rngbuf_t rb = &(p_this->rx_rngbuf);

    uint32_t rb_ct, read_ct;
    uint32_t nbytes = 0;

    while (len > 0) {

        if (am_rngbuf_isempty(rb) == AM_TRUE) {

            if (p_this->timeout_ms == (uint32_t)AM_WAIT_FOREVER) {

                am_wait_on(&p_this->rx_wait);       //等待接收数据

            } else if (p_this->timeout_ms != AM_NO_WAIT) {

                if (am_wait_on_timeout(&p_this->rx_wait,
                                       p_this->timeout_ms) != AM_OK) {
                    return nbytes;
                }

            } else {
                return nbytes;
            }
        }

        rb_ct   = am_rngbuf_nbytes(rb); //获取环形缓冲区中已填充的数据字节个数

        read_ct = (rb_ct > len) ? len : rb_ct;

        am_rngbuf_get(rb, (char *)p_buf, read_ct);  //从环形缓冲区获取数据

        p_buf   += read_ct;
        len     -= read_ct;
        nbytes  += read_ct;
    }

    return nbytes;
}

/******************************************************************************/

/** 
 * \brief GS200 接收超时回调函数
 */
am_local void __gs200_timeout_cb (void *p_arg)
{
    am_gs200_dev_t  *p_this = (am_gs200_dev_t *)p_arg;
    int              key    = am_int_cpu_lock();

    am_softimer_stop(&(p_this->timer));         //关闭接收超时检测定时器
    p_this->cmd_proc_state = __GS200_CMD_PROC_STAT_RECV_END;
    am_wait_done(&(p_this->rx_wait));
    am_int_cpu_unlock(key);
}

/******************************************************************************/

/** 
 * \brief GS200 初始化
 */
am_gs200_handle_t am_gs200_init (am_gs200_dev_t             *p_dev,
                                 const am_gs200_devinfo_t   *p_devinfo,
                                 am_uart_handle_t            uart_handle)
{

    if ((p_dev                   == NULL) ||
        (p_devinfo               == NULL) ||
        (p_devinfo->p_uart_rxbuf == NULL) ||
        (p_devinfo->p_uart_txbuf == NULL)) {

        return NULL;
    }

    p_dev->uart_handle    = uart_handle;
    p_dev->p_devinfo      = p_devinfo;
    p_dev->timeout_ms     = (uint32_t)AM_WAIT_FOREVER;
    p_dev->cmd_proc_state = __GS200_CMD_PROC_STAT_NONE;

    am_wait_init(&(p_dev->rx_wait));
    am_wait_init(&(p_dev->ack_wait));

    am_softimer_init(&p_dev->timer, __gs200_timeout_cb , (void *)p_dev);

    am_uart_ioctl(uart_handle, AM_UART_BAUD_SET, (void *)(p_devinfo->baudrate));

    /* Initialize the ring-buffer */
    am_rngbuf_init(&(p_dev->rx_rngbuf),
                   (char *)p_devinfo->p_uart_rxbuf,
                   p_devinfo->rxbuf_size);

    am_rngbuf_init(&(p_dev->tx_rngbuf),
                   (char *)p_devinfo->p_uart_txbuf,
                   p_devinfo->txbuf_size);

    am_uart_ioctl(uart_handle, AM_UART_MODE_SET, (void *)AM_UART_MODE_INT);

    am_uart_callback_set(uart_handle,
                         AM_UART_CALLBACK_TXCHAR_GET,
        (int (*)(void *))__gs200_uart_txchar_get,
                         (void *)(p_dev));

    am_uart_callback_set(uart_handle,
                         AM_UART_CALLBACK_RXCHAR_PUT,
        (int (*)(void *))__gs200_uart_rxchar_put,
                         (void *)(p_dev));

    return p_dev;
}

/******************************************************************************/

/** 
 * \brief GS200 控制函数
 */
int am_gs200_ioctl (am_gs200_handle_t   handle,
                    int                 cmd,
                    void               *p_arg)
{
    int ret = -AM_EIO;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    switch (cmd) {
        case AM_GS200_SET_BAUDRATE:                 //设置波特率指令
            ret = __gs200_cmd_send(handle,
                                   __GS200_CMD_SET_BAUDRATE,
                                   (uint8_t)(int)p_arg,
                                   0x00,
                                   AM_TRUE);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __gs200_cmd_result(handle,
                                     __GS200_CMD_SET_BAUDRATE,
                                     AM_TRUE);
            break;

        case AM_GS200_SET_GESTURE_STATE:            //设置模块手势功能使能与禁能指令
            ret = __gs200_cmd_send(handle,
                                   __GS200_CMD_SET_GESTURE_STATE,
                                   (uint8_t)(int)p_arg,
                                   0x00,
                                   AM_TRUE);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __gs200_cmd_result(handle,
                                     __GS200_CMD_SET_GESTURE_STATE,
                                     AM_TRUE);
            break;

        case AM_GS200_SET_WORKING_CONDITION:        //设置工作状态指令
            ret = __gs200_cmd_send(handle,
                                   __GS200_CMD_SET_WORKING_CONDITION,
                                   (uint8_t)(int)p_arg,
                                   0x00,
                                   AM_TRUE);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __gs200_cmd_result(handle,
                                     __GS200_CMD_SET_WORKING_CONDITION,
                                     AM_TRUE);
            break;

        case AM_GS200_SET_HOVERING_GESTURE_MODE:    //设置悬停手势(定时/单次)输出指令
            ret = __gs200_cmd_send(handle,
                                   __GS200_CMD_SET_HOVERING_GESTURE,
                                   (uint8_t)(int)p_arg,
                                   0x00,
                                   AM_TRUE);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __gs200_cmd_result(handle,
                                     __GS200_CMD_SET_HOVERING_GESTURE,
                                     AM_TRUE);
            break;

        case AM_GS200_SET_HOVERING_TIMING_TIME:    //设置悬停手势定时输出时间间隔
            ret = __gs200_cmd_send(handle,
                                   __GS200_CMD_SET_HOVERING_GESTURE,
                                   __GS200_SET_OUTPUT_TIMING_ENABLE,
                                   (uint8_t)(((int)p_arg) / 50),
                                   AM_TRUE);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __gs200_cmd_result(handle,
                                     __GS200_CMD_SET_HOVERING_GESTURE,
                                     AM_TRUE);
            break;

        case AM_GS200_SET_HOVERING_SINGLE_TIME:    //设置悬停手势单次触发时间
            ret = __gs200_cmd_send(handle,
                                   __GS200_CMD_SET_HOVERING_GESTURE,
                                   __GS200_SET_OUTPUT_SINGLE_ENABLE,
                                   (uint8_t)((((int)p_arg) - 200) / 50),
                                   AM_TRUE);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __gs200_cmd_result(handle,
                                     __GS200_CMD_SET_HOVERING_GESTURE,
                                     AM_TRUE);
            break;

        case AM_GS200_NREAD:
            *(int *)p_arg = am_rngbuf_nbytes(&handle->rx_rngbuf);
            ret = AM_OK;
            break;

        case AM_GS200_NWRITE:
            *(int *)p_arg = am_rngbuf_nbytes(&handle->tx_rngbuf);
            ret = AM_OK;
            break;

        case AM_GS200_FLUSH:
        {
            int key = am_int_cpu_lock();

            am_rngbuf_flush(&handle->rx_rngbuf);
            am_rngbuf_flush(&handle->tx_rngbuf);
            am_int_cpu_unlock(key);
            ret = AM_OK;
            break;
        }

        case AM_GS200_WFLUSH :
        {
            int key = am_int_cpu_lock();

            am_rngbuf_flush(&handle->tx_rngbuf);
            am_int_cpu_unlock(key);
            ret = AM_OK;
            break;
        }

        case AM_GS200_RFLUSH :
        {
            int key = am_int_cpu_lock();

            am_rngbuf_flush(&handle->rx_rngbuf);
            am_int_cpu_unlock(key);
            ret = AM_OK;
            break;
        }

        case AM_GS200_TIMEOUT:
            handle->timeout_ms = (uint32_t)p_arg;
            ret = AM_OK;
            break;

        default:
            ret = -AM_ENOTSUP;
            break;
    }

    return ret;
}

/******************************************************************************/

/** 
 * \brief GS200 发送
 */
int am_gs200_send (am_gs200_handle_t    handle,
                   const uint8_t        *p_buf,
                   int                  len)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    /* in cmd process, can't send data now */
    if (handle->cmd_proc_state != __GS200_CMD_PROC_STAT_NONE) {
        return -AM_EPERM;
    }

    return __gs200_data_send(handle, p_buf, len);
}

/******************************************************************************/

/** 
 * \brief GS200 接收
 */
int am_gs200_recv (am_gs200_handle_t    handle,
                   uint8_t              *p_buf,
                   int                  len)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __gs200_data_recv(handle, p_buf, len);
}

