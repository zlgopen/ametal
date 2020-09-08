/*******************************************************************************
*                                 AMorks
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2016 Guangzhou ZHIYUAN Electronics Co., Ltd.
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
 * \note 可使用‘或’操作开启多个中断。
 *
 *       注意，请不要同时开启溢出和接收中断。
 *       原因，程序设计，接收中断使能后，为避免一直触发接收中断，会对FIFO内的报文进行清除或读取操作。
 *           在这种情况下，溢出中断是不可能触发的。
 *
 *       开启唤醒中断时，需要上位机先发送一组报文以唤醒CAN。
 *       开启错误中断时，需要上位机主动将波特率设置成不匹配，导致CAN发送失败。（方法不唯一，可自行模拟错误传输）
 *
 * \par 源代码
 * \snippet demo_stm32f103rbt6_can_int.c demo_stm32f103rbt6_can_int
 *
 * \internal
 * \par modification history:
 * - 19-08-08, zp, first implementation.
 * - 19-12-18, zc, add filter table extern
 * \endinternal
 */

#include "ametal.h"
#include "am_can.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_stm32f103rbt6_inst_init.h"
#include "demo_zlg_entries.h"

#define  APB1_CLK        36000000 /*CAN 的输入时钟 */
#define  CAN_BAUD        50000    /*CAN 的波特率 */

/**\brief 滤波表数组，相关设定请参考demo_stm32f103rbt6_core_can.c例程 */
am_can_filter_t table[1] = {
        {
                AM_CAN_FRAME_TYPE_EXT,
                AM_CAN_FRAME_FORMAT_DATA,
                {0x00},
                {0x00},
        }
};

/**
 * \brief 获取对应波特率配置值  CAN_BAUD = APB1/((BRP + 1) * (TEG1 + TEG2 + 3))
 */
static void __can_auto_cfg_baud(am_can_bps_param_t *can_btr_baud,
                                uint32_t            src_clk,
                                uint32_t            baud )
{
    uint32_t i,value = baud,record = 1;
    uint32_t remain = 0,sum_prescaler = 0;
    while(( baud == 0 )||( src_clk == 0 ));
    sum_prescaler = src_clk / baud;
    for ( i = 25; i > 3; i-- ) {
        remain = sum_prescaler - ((sum_prescaler / i)*i);
        if( remain == 0 ) {
          record = i;
          break;
        } else {
          if (remain < value) {
              value = remain;
              record = i;
          }
        }
    }
    can_btr_baud->sjw = 2;
    can_btr_baud->brp = (sum_prescaler/record) - 1;
    can_btr_baud->tesg2 = (record - 3) / 3;
    can_btr_baud->tesg1 = (record - 3) - can_btr_baud->tesg2;
}
/**
 * \brief CAN 中断例程入口
 *
 * \return 无
 */
void demo_stm32f103rbt6_core_can_int_entry (void)
{
    am_can_bps_param_t  can_btr_baud = {0};
    am_can_int_type_t   int_type;

    AM_DBG_INFO("demo stm32f103rbt6_core std can!\r\n");

    __can_auto_cfg_baud(&can_btr_baud, APB1_CLK, CAN_BAUD);

    /* 可使用‘或’操作开启多个中断
     * 注意，请不要同时开启溢出和接收中断。
     * 原因，程序设计，接收中断使能后，为避免一直触发接收中断，会对FIFO内的报文进行清除或读取操作。
     *     在这种情况下，溢出中断是不可能触发的。
     *
     *     开启唤醒中断时，需要上位机先发送一组报文以唤醒CAN。
     *     开启错误中断时，需要上位机主动将波特率设置成不匹配，导致CAN发送失败。（方法不唯一，可自行模拟错误传输）
     */
    int_type = AM_CAN_INT_WARN          |
               AM_CAN_INT_ERROR_PASSIVE |
               AM_CAN_INT_WAKE_UP       |
               AM_CAN_INT_TX            |
               AM_CAN_INT_DATAOVER ;

    demo_stm32f103rbt6_can_int_entry (am_stm32f103rbt6_can_inst_init(),
                               &can_btr_baud,
                               int_type,
                               table,
                               1);
}

/** [demo_stm32f103rbt6_can_int] */

/* end of file */
