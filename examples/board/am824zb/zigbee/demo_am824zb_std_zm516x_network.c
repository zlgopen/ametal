/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ZM516X 模块组网例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 测试本 Demo 最少需要使用两块 AM824ZB 板，主机只能有 1 个，从机可以有多个，
 *      主机和从机需要下载不同的程序，
 *      作为主机的开发板打开宏 MASTER_BORD_USE，关闭宏 SLAVE_BORD_USE，编译并下载，
 *      作为从机的开发板打开宏 SLAVE_BORD_USE，关闭宏 MASTER_BORD_USE，编译并下载；
 *   2. 将从机的 JOIN 跳线帽短接 3 秒以下，使从机进入入网申请阶段；
 *   3. 按下主机的 KEY/RES 按键，使主机进入 __JOIN_TIME 秒的组网模式；
 *   4. 等待主机退出组网模式后，按下从机的 KEY/RES 按键。
 *
 * - 实验现象：
 *   1. 如果 ZM516X 模块初始化并配置失败，LED0 闪烁；
 *   2. 串口打印读取到的本地模块配置信息；
 *   3. 串口打印提示信息；
 *   4. 主机打印从机列表与从机信息；
 *   5. 主机向从机发送 "I'am Master\r\n";
 *   5. 从机向主机发送 "I'am Slave x\r\n"，其中 x 为从机网络地址;
 *   3. 主机和从机分别通过调试串口打印接收到的数据。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_8 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. ZigBee 模块内连接关系如下：
 * <pre>
 *           PIO0_26  <-->  ZigBee_TX
 *           PIO0_27  <-->  ZigBee_RX
 *           PIO0_28  <-->  ZigBee_RST
 * </pre>
 *        如果需要使用 ZigBee，这些 IO 口不能用作其它用途。
 *
 * \par 源代码
 * \snippet demo_am824zb_std_zm516x_network.c src_am824zb_std_zm516x_network
 *
 * \internal
 * \par Modification History
 * - 1.00 18-01-15  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824zb_std_zm516x_network
 * \copydoc demo_am824zb_std_zm516x_network.c
 */

/** [src_am824zb_std_zm516x_network] */
#include "ametal.h"
#include "am_zm516x.h"
#include "am_vdebug.h"
#include "demo_components_entries.h"
#include "am_lpc82x_inst_init.h"

/**
 * \brief 例程入口
 */
void demo_am824zb_std_zm516x_network_entry (void)
{

    am_zm516x_handle_t zm516x_handle  = am_zm516x_inst_init();

    demo_zm516x_network_entry(zm516x_handle);
}
/** [src_am824zb_std_zm516x_network] */

/* end of file */
