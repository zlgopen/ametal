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
 * \brief CAN演示例程
 *
 * - 操作步骤：
 *   1. 本例程需在am_prj_config.h头文件里使能
 *   -  对应平台的串口设备宏(AM_DEV_XXXXX_UART1)。
 *   2. 连接CAN1的相关引脚到CAN收发器，再将CAN收发器接口与USBCAN-II设备相连。
 *   3. 打开CAN上位机后，设置波特率为50k。
 *   4. 打开串口调试终端，波特率115200-8-N-1.
 *   5. 上位机 发送帧格式为 扩展帧  远程请求无要求(数据帧和远程帧都可) ID为 0x -7- 即  第4-6 位为1
 *
 * - 实验现象：
 *   1. 上位机发送数据后，收到发送的数据。
 *   2. 串口打印相关调试信息。
 *
 * - 备注：
 *   1. 其中 CAN ID 和 波特率配置 需要根据具体硬件平台修改。
 *
 * \par 源代码
 * \snippet demo_hk32f103rbt6_core_can.c
 *
 * \internal
 * \par modification history:
 * - 19-8-12, zc, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_can.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_hk32f103rbt6_inst_init.h"
#include "demo_hk32f103rbt6_entries.h"


#define  APB1_CLK        36000000 /*CAN 的输入时钟 */
#define  CAN_BAUD        50000   /*CAN 的波特率  50k*/

/**
 * \brief 获取对应波特率配置值  CAN_BAUD = APB1/((BRP + 1) * (TEG1 + TEG2 + 3))
 */
static void __can_auto_cfg_baud(am_can_bps_param_t  *can_btr_baud,
                                uint32_t             src_clk,
                                uint32_t             baud )
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

    /* 设置重新同步跳跃宽度为2个时间单位 */
    can_btr_baud->sjw = 2;
    can_btr_baud->brp = (sum_prescaler/record) - 1;
    can_btr_baud->tesg2 = (record - 3) / 3;
    can_btr_baud->tesg1 = (record - 3) - can_btr_baud->tesg2;

}
/**
 * \brief CAN 历程入口
 *
 * \return 无
 */
void demo_hk32f103rbt6_core_can_entry (void)
{
    am_can_bps_param_t  can_btr_baud = {0};

    AM_DBG_INFO("demo hk32f103rbt6_core std can!\r\n");

    __can_auto_cfg_baud(&can_btr_baud, APB1_CLK, CAN_BAUD);

    demo_hk32f103rbt6_can_entry (am_hk32f103rbt6_can_inst_init (), &can_btr_baud);
}

/** [src_test_flexcan] */

/* end of file */
