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
 * \brief CAN演示例程
 *
 * - 操作步骤：
 *   1. 本例程需在am_prj_params.h头文件里使能
 *   - 对应平台的CAN设备宏(AM_DEV_XXXXX_CAN1);
 *   - AM_COM_CONSOLE;
 *   -  对应平台的串口设备宏(AM_DEV_XXXXX_UART1)。
 *   2. 连接CAN1的相关引脚到CAN收发器，再将CAN收发器接口与USBCAN-II设备相连。
 *   3. 打开CAN上位机后，设置波特率为250k。
 *   4. 打开串口调试终端，波特率115200-8-N-1.
 *
 * - 实验现象：
 *   1. 上位机发送数据后，收到发送的数据。
 *   2. 串口打印相关调试信息。
 *
 * - 备注：
 *   1. 其中 CAN ID 和 波特率配置 需要根据具体硬件平台修改。
 *   2. 在basic模式下不支持扩展帧。
 *   3. zmf159 滤波器在设置掩码时，在特定情况下会出现标准帧和扩展帧 都可接受
 *      （由滤波器寄存器实际存储方式决定的）
 *   4. 掩码设置中 1 ： 关心
 *              0 ： 不关心
 *
 * \par 源代码
 * \snippet demo_std_crc.c src_std_can
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-09  bzq, first implementation
 * - 1.00 19-12-18  zc,  adapt filter extern api
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_can
 * \copydoc demo_std_can.c
 */

/** [src_std_can] */
#include "ametal.h"
#include "am_can.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**\brief 滤波表 */
static am_can_filter_t table[1] = {
        {
                AM_CAN_FRAME_TYPE_EXT,
                AM_CAN_FRAME_FORMAT_NOCARE,
                {0x00},
                {0x00},
        }
};

/**
 * \brief 错误判断
 */
void __can_err_sta( am_can_bus_err_t err)
{

    if (err & AM_CAN_BUS_ERR_BIT) {    /**< \brief 位错误 */
        am_kprintf(("AM_CAN_BUS_ERR_BIT\n"));
    }
    if (err &AM_CAN_BUS_ERR_ACK) {     /**< \brief 应答错误 */
        am_kprintf(("AM_CAN_BUS_ERR_ACK\n"));
    }
    if (err &AM_CAN_BUS_ERR_CRC) {     /**< \brief CRC错误 */
        am_kprintf(("AM_CAN_BUS_ERR_CRC\n"));
    }
    if (err &AM_CAN_BUS_ERR_FORM) {    /**< \brief 格式错误 */
        am_kprintf(("AM_CAN_BUS_ERR_FORM\n"));
    }
    if (err &AM_CAN_BUS_ERR_STUFF) {   /**< \brief 填充错误 */
        am_kprintf(("AM_CAN_BUS_ERR_STUFF\n"));
    }
}

/**
 * \brief 例程入口
 */
void demo_std_can_entry (am_can_handle_t can_handle, am_can_bps_param_t  *can_btr_baud)
{
    am_can_err_t ret;
    uint8_t      i = 0;
    am_can_message_t    can_rcv_msg = {0};
    am_can_bus_err_t    can_bus_err_status;
    am_can_int_type_t   can_int_status;

    /* 配置波特率 */
    ret = am_can_baudrate_set (can_handle, can_btr_baud);

    if (ret == AM_CAN_NOERROR) {
        am_kprintf("\r\nCAN: controller baudrate set ok. \r\n");
    } else {
        am_kprintf("\r\nCAN: controller baudrate set error! %d \r\n", ret);
    }

    /* 配置滤波表 */
    ret = am_can_filter_tab_ext_set(can_handle,
                                    table,
                                    1);

    if (ret == AM_CAN_NOERROR) {
        am_kprintf("\r\nCAN: controller filter tab set ok. \r\n");
    } else {
        am_kprintf("\r\nCAN: controller filter tab set error! %d \r\n", ret);
    }

    /* 启动can */
    ret = am_can_start (can_handle);

    if (ret == AM_CAN_NOERROR) {
        am_kprintf("\r\nCAN: controller start \r\n");
    } else {
        am_kprintf("\r\nCAN: controller start error! %d \r\n", ret);
    }

    AM_FOREVER {

        ret = am_can_msg_recv (can_handle, &can_rcv_msg);

        if (can_rcv_msg.msglen) {
            am_kprintf("can recv id: 0x%x\r\n",can_rcv_msg.id);
            /* 不打印远程帧时的数据缓存 */
            if ((can_rcv_msg.flags & AM_CAN_REMOTE_FLAG) != AM_CAN_REMOTE_FLAG) {
                for (i = 0; i < can_rcv_msg.msglen; i++) {
                    am_kprintf("data: 0x%x \r\n",can_rcv_msg.msgdata[i]);
                }
            }
                ret = am_can_msg_send (can_handle, &can_rcv_msg);

                if (ret == AM_CAN_NOERROR) {
                    am_kprintf(("\r\nCAN: controller rcv data ok. \r\n"));
                } else {
                    am_kprintf("\r\nCAN: controller no rcv data! \r\n");
                }

        }

        ret = am_can_status_get (can_handle, &can_int_status, &can_bus_err_status);

        am_mdelay(10);
        if (can_bus_err_status != AM_CAN_BUS_ERR_NONE) {
            __can_err_sta(can_bus_err_status);
        }
    }
}
/** [src_std_can] */

/* end of file */
