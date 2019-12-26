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
 *   1. 本例程需在am_prj_config.h头文件里使能
 *   -  对应平台的串口设备宏(AM_DEV_XXXXX_UART1)。
 *   2. 连接CAN1的相关引脚到CAN收发器，再将CAN收发器接口与USBCAN-II设备相连。
 *   3. 打开CAN上位机后，设置波特率为50k。
 *   4. 打开串口调试终端，波特率115200-8-N-1.
 *   5. 上位机 发送帧格式为 扩展帧  远程请求无要求(数据帧和远程帧都可) ID为 0x -7- 即  第4-6 位为1
 *   6. 若要修改 配置文件注意上位机发送的帧格式和类型  配置文件在 amh_hwconf_zlg237_can.c
 *    默认设置在 CAN_ID_EXT
 *               AMHW_ZLG237_CAN_FILTER_MODE_IDMASK 
 *               AMHW_ZLG237_CAN_FILTER_SCALE_32BIT
 *      修改帧格式和类型后 ， 需要在注意table的设置 格式变化
 *      详情见 下方注释 和 ZLG237用户手册17.6.4章  标识符过滤
 * - 实验现象：
 *   1. 上位机发送数据后，收到发送的数据。
 *   2. 串口打印相关调试信息。
 *
 *
 * \par 源代码
 * \snippet demo_zlg237_can.c src_std_can
 *
 * \internal
 * \par Modification History
 * - 19-8-12, zc, first implementation.
 * - 19-12-18     add filter table extern 
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
#include "amhw_zlg237_can.h"
#include "am_zlg237_can.h"


/**\brief 滤波表 */

am_can_filter_t tab[4] = {
        {
                AM_CAN_FRAME_TYPE_EXT,
                AM_CAN_FRAME_FORMAT_DATA,
                {0x10000000},
                {0x10000000},

        },
        {
                AM_CAN_FRAME_TYPE_EXT,
                AM_CAN_FRAME_FORMAT_REMOTE,
                {0x10000001},
                {0x10000001},

        },
        {
                AM_CAN_FRAME_TYPE_STD,
                AM_CAN_FRAME_FORMAT_DATA,
                {0x01},
                {0x01},

        },
        {
                AM_CAN_FRAME_TYPE_STD,
                AM_CAN_FRAME_FORMAT_REMOTE,
                {0x02},
                {0x02},

        },
};

am_can_filter_t get_tab[4]={0};

/**
 * \brief 错误判断
 */
static void __can_err_sta( am_can_bus_err_t err)
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
void demo_zlg237_can_entry (am_can_handle_t      can_handle,
                            am_can_bps_param_t  *can_btr_baud)
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
    ret = am_can_filter_tab_ext_set(can_handle,tab,
                                    sizeof(tab)/sizeof(am_can_filter_t));

    if (ret == AM_CAN_NOERROR) {
        am_kprintf("\r\nCAN: controller filter table set ok. \r\n");
    } else {
        am_kprintf("\r\nCAN: controller filter table set error! %d \r\n", ret);
    }

    size_t len = 0;
    ret = am_can_filter_tab_ext_get(can_handle,get_tab,&len);

    if (ret == AM_CAN_NOERROR) {
        am_kprintf("\r\nCAN: controller filter table get ok. \r\n");
    } else {
        am_kprintf("\r\nCAN: controller filter table get error! %d \r\n", ret);
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

        ret = am_can_status_get (can_handle,
                                &can_int_status,
                                &can_bus_err_status);

        am_mdelay(10);
        if (can_bus_err_status != AM_CAN_BUS_ERR_NONE) {
            __can_err_sta(can_bus_err_status);
        }
    }
}
/** [src_zlg237_can] */

/* end of file */
