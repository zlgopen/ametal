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
 * \brief CAN中断演示例程
 *
 * - 操作步骤：
 *   1. 观察串口打印的调试信息，需要将 PIOA_3 引脚连接 PC 串口的 TXD，
 *      PIOA_2 引脚连接 PC 串口的 RXD；
 *   2. 连接CAN1的相关引脚到CAN收发器，再将CAN收发器接口与USBCAN-II设备相连。
 *   3. 打开CAN上位机后，设置波特率为宏定义CAN_BAUD的值。
 *   4. 打开串口调试终端，波特率115200-8-N-1.
 *
 * - 实验现象：
 *   1. 触发相关中断后，打印提示信息。
 *
 *      唤醒中断：串口打印等待唤醒提示信息，上位机发送一组数据后，成功唤醒，并打印提示信息。
 *      接收中断：每接收到一组数据，串口打印ID号，字节数据。
 *      溢出中断：FIFO(容量3)溢出后，串口打印FIFO收到的第一组数据，ID号，字节数据。
 *      发送中断：每500ms在while循环中发送一次数据，数据发送成功，进入发送中断，串口打印提示信息。
 *      错误警告中断：串口打印提示信息，且打印发送错误计数值，接收错误计数值。
 *      错误被动中断：串口打印提示信息，且打印发送错误计数值，接收错误计数值。
 *
 *      报文结构体can_rcv_msg在demo_stm32f103rbt6_can_int_entry()中使用的是全局变量，正常工作(已唤醒)，
 *      开启了接收或溢出中断后，上位机发送的数据，本测试工程会每500ms返回给上位机。
 *
 *   2. 串口打印相关调试信息。
 *
 * \par 源代码
 * \snippet src_stm32f103rbt6_can_int.c
 *
 * \internal
 * \par Modification History
 * - 19-08-08, zp, first implementation.
 * - 19-12-18, zc, add filter table extern 
 * \endinternal
 */

/**
 * \addtogroup demo_stm32f103rbt6_can_int
 * \copydoc demo_stm32f103rbt6_can_int.c
 */

/** [src_std_can] */
#include "ametal.h"
#include "am_can.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "amhw_stm32f103rbt6_can.h"
#include "am_stm32f103rbt6_can.h"

am_bool_t int_err_flag   = AM_FALSE;
am_bool_t int_sleep_flag = AM_FALSE;

am_can_message_t can_rcv_msg;

/* 中断服务函数，进入中断后需要手动清除相应标志位，否则会一直触发中断*/
void can_int_handle(void *p_arg)
{

    am_stm32f103rbt6_can_dev_t *p_dev    = (am_stm32f103rbt6_can_dev_t *)p_arg;
    amhw_stm32f103rbt6_can_t   *p_hw_can = NULL;

    am_can_int_type_t p_int_type     = 0;
    am_can_bus_err_t  p_bus_err      = 0;
    uint8_t           i              = 0;

    p_hw_can = (amhw_stm32f103rbt6_can_t *)p_dev->p_devinfo->regbase;

    am_can_status_get(p_arg, &p_int_type , &p_bus_err);

    /* FIFO溢出（容量为3，>3），溢出标志有效 */
    if(p_int_type & AM_CAN_INT_DATAOVER) {

        am_kprintf("\r\nCAN int data over!\r\n");

        /* 尝试读取报文 */
        if(am_can_msg_recv(p_arg, &can_rcv_msg) == AM_CAN_NOERROR){

            am_kprintf("can recv id: 0x%x data:",can_rcv_msg.id);
            for (i = 0; i < can_rcv_msg.msglen; i++) {
                am_kprintf("0x%x  ",can_rcv_msg.msgdata[i]);
            }
            am_kprintf("\r\n");
        }

        /* 根据FIFO特性。读取一次之后，数据减少一组。
         * 所以上面执行一次读取操作后可不再执行清除操作（实际为释放FIFO一次）。
         * 读取或释放最好只存在一种操作，否则会造成数据丢失。
         */

//        /* 释放一次FIFO 0，才能清除该标志 */
//        amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can, AMHW_STM32F103RBT6_CAN_INT_FOVIE0);
//
//        /* 释放一次FIFO 1，才能清除该标志 */
//        amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can, AMHW_STM32F103RBT6_CAN_INT_FOVIE1);
    }

    /* FIFO非空（容量为3，>0），接收标志有效 */
    if(p_int_type & AM_CAN_INT_RX) {

        am_kprintf("\r\nCAN int rx!\r\n");

        /* 尝试读取报文 */
        if(am_can_msg_recv(p_arg, &can_rcv_msg) == AM_CAN_NOERROR){

            am_kprintf("can recv id: 0x%x\r\ndata:",can_rcv_msg.id);
            for (i = 0; i < can_rcv_msg.msglen; i++) {
                am_kprintf("0x%x  ",can_rcv_msg.msgdata[i]);
            }
            am_kprintf("\r\n");
        }

        /* 根据FIFO特性。读取一次之后，数据减少一组。
         * 所以上面执行一次读取操作后可不再执行清除操作（实际为释放FIFO一次）。
         * 读取或释放最好只存在一种操作，否则会造成数据丢失。
         */

//        /* 释放一次FIFO 0，FIFO为空时，才能清除该标志*/
//        amhw_stm32f103rbt6_can_clear_itstatus(p_hw_can, AMHW_STM32F103RBT6_CAN_INT_FMPIE0);
//
//        /* 释放一次FIFO 1，FIFO为空时，才能清除该标志*/
//        amhw_stm32f103rbt6_can_clear_itstatus(p_hw_can, AMHW_STM32F103RBT6_CAN_INT_FMPIE1);
    }

    /* 发送结束，发送标志有效 */
    if(p_int_type & AM_CAN_INT_TX) {

        am_kprintf("\r\nCAN int tx!\r\n");

        /* 释放一次FIFO，FIFO为空时，才能清除该标志*/
        amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can, AMHW_STM32F103RBT6_CAN_INT_TMEIE);
    }

    /* 在睡眠模式下，接收到帧的SOF位，唤醒标志有效 */
    if(p_int_type & AM_CAN_INT_WAKE_UP) {
        am_kprintf("\r\nCAN int wake up!\r\n");

        int_sleep_flag = AM_FALSE;

        /* 清除唤醒标志 */
        amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can, AMHW_STM32F103RBT6_CAN_INT_WKUIE);
    }

    /* 错误计数器超过阈值（固定，不可更改），产生不同错误标志。当接收到正确数据，计数值大小减1 */
    if(p_int_type & AM_CAN_INT_ERROR) {

        int_err_flag = AM_TRUE;

        am_kprintf("\r\nCAN int error  ");

        /* 错误计数值>96,错误警告标志有效 */
        if(p_int_type & AM_CAN_INT_WARN) {
            am_kprintf("warning!");
        }

        /* 错误计数值>127,错误被动标志有效 */
        if(p_int_type & AM_CAN_INT_ERROR_PASSIVE) {
            am_kprintf("passive!");
        }

        /* 错误计数值>255,离线标志有效 */
        if(p_int_type & AM_CAN_INT_BUS_OFF) {
            am_kprintf("off!");
        }

        /* 清除错误标志*/
        amhw_stm32f103rbt6_can_clear_itstatus (p_hw_can, AMHW_STM32F103RBT6_CAN_INT_ERRIE);
    }
}

/* 错误信息打印函数 */
void can_err_printf(am_can_handle_t  handle)
{
    int_err_flag = AM_FALSE;

    am_can_int_type_t p_int_type     = 0;
    am_can_bus_err_t  p_bus_err      = 0;
    am_can_err_cnt_t  am_can_err_cnt = {0, 0};

    am_can_status_get(handle, &p_int_type , &p_bus_err);

    /* 格式错误 */
    if (AM_CAN_BUS_ERR_FORM & p_bus_err) {
        am_kprintf("  lasterror_code form!");
    }

    /* 填充错误 */
    if (AM_CAN_BUS_ERR_STUFF & p_bus_err) {
        am_kprintf("  lasterror_code stuff!");
    }

    /* 未知错误     显性错误   隐性错误  由软件设置错误*/
    if (AM_CAN_BUS_ERR_UNKNOWN & p_bus_err) {
        am_kprintf("  lasterror_code unknown!");
    }

    /* ACK错误 */
    if (AM_CAN_BUS_ERR_ACK & p_bus_err) {
        am_kprintf("  lasterror_code ack!");
    }

    /* CRC错误 */
    if (AM_CAN_BUS_ERR_CRC & p_bus_err) {

    }

    /* 获取错误计数值 */
    am_can_err_cnt_get (handle, &am_can_err_cnt);

    /* 打印错误计数*/
    am_kprintf("   err_rx:%d , err_tx:%d\r\n",
               am_can_err_cnt.rx_error_cnt,
               am_can_err_cnt.tx_error_cnt);
}

/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_can_int_entry (am_can_handle_t     can_handle,
                                am_can_bps_param_t *can_btr_baud,
                                am_can_int_type_t   int_type,
                                am_can_filter_t    *p_filterbuff,
                                size_t              lenth)
{
    am_can_err_t ret;

    can_rcv_msg.id         = 0x54;
    can_rcv_msg.msglen     = 3;
    can_rcv_msg.msgdata[0] = 0x78;
    can_rcv_msg.msgdata[0] = 0xfd;
    can_rcv_msg.msgdata[0] = 0xa8;

    /* 配置波特率 */
    ret = am_can_baudrate_set (can_handle, can_btr_baud);

    if (ret == AM_CAN_NOERROR) {
        am_kprintf("\r\nCAN: controller baudrate set ok. \r\n");
    } else {
        am_kprintf("\r\nCAN: controller baudrate set error! %d \r\n", ret);
    }

    /* 配置滤波表 */
    ret = am_can_filter_tab_ext_set(can_handle,p_filterbuff, lenth);

    if (ret == AM_CAN_NOERROR) {
        am_kprintf("\r\nCAN: controller filter tab set ok. \r\n");
    } else {
        am_kprintf("\r\nCAN: controller filter tab set error! %d \r\n", ret);
    }

    /* 连接中断服务函数  */
    /* 第二个参数传入为NULL时，连接内部默认中断服务函数， __can_stm32f103rbt6_int_handle
     * 可调用am_can_intcb_connect函数设置用户中断回调函数
     */
    am_can_connect(can_handle,can_int_handle,(void *)can_handle->p_drv);

    /* CAN中断使能函数 */
    am_can_int_enable(can_handle,int_type);

    /* 启动can */
    ret = am_can_start (can_handle);

    if (ret == AM_CAN_NOERROR) {
        am_kprintf("\r\nCAN: controller start \r\n");
    } else {
        am_kprintf("\r\nCAN: controller start error! %d \r\n", ret);
    }

    if(int_type & AM_CAN_INT_WAKE_UP)
    {
        if(am_can_sleep(can_handle) == AM_CAN_NOERROR) {
            int_sleep_flag = AM_TRUE;
        }
    }

    AM_FOREVER {

        /* 不处于等待唤醒状态时，发送数据 */
        if(int_sleep_flag == AM_FALSE) {

            /* 发送报文 */
            am_can_msg_send(can_handle, &can_rcv_msg);
        } else {

            /* 处于等待唤醒状态，打印提示信息。CAN需要接受报文，或者调用唤醒函数*/
            am_kprintf("\r\nCAN: sleep mode! waiting for wakeup!\r\n");
        }

        /* 错误标志有效，调用错误打印函数 */
        if(AM_TRUE == int_err_flag) {
            can_err_printf(can_handle);
        } else {
            am_mdelay(500);
        }

    }
}
/** [src_stm32f103rbt6_can_int] */

/* end of file */
