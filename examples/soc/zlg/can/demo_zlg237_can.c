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
 *      修改帧格式和类型后 ， 需要在注意table的设置 格式变化 详情见注释
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

/**
 *  2个16位过滤器  标识符屏蔽位模式
 *  需在am_hwconf_zlg237_can.c中的__g_can_filter设置为
 *  AMHW_ZLG237_CAN_FILTER_MODE_IDMASK  AMHW_ZLG237_CAN_FILTER_SCALE_16BIT
 *
 *
 *  注意 ：
 *      在设置 扩展标识符模式时， 其屏蔽为设置只能设置在EXID 不能设置在STID的位上 否者数据服无法接收
 *
 *      在设置为标准标识符模式时，其屏蔽位只能设置在STID位上，不能设置在EXID上  否者数据无法接收
 *      16位寄存器映像如下 ： STID[10:3] STID[2:0] RTR IDR EXID[17:15]
 *
 *      table中传入的ID为 标准标识符模式  STID[10:3] STID[2:0]
 *                     扩展标识符模式 EXID[17:15]  STID[10:3] STID[2:0]
 *
 *  在配置文件中修改帧类型 为  CAN_ID_EXT
 *  static uint8_t table[8] = {0x28, 0x00, 0x00,  0x07,  0x28, 0x00, 0x00, 0x07};
 *                       即 ID_H 0X28 ID_L  0X00  其ID的实际值为 0x28000
 *                       即在上位机测试时，需设置ID(0x28,0x00)为   0x28000 (此为18位的扩展ID)
 *                       其中0x7 为 扩展标识符掩码 STID[17:15] 每一位必须匹配
 *                       上位机发送ID位 28xxx 标识符为扩展  远程请求无要求
 *
 *  在配置文件中修改帧类型为 CAN_IS_STD
 *  static uint8_t table[8] = {0x00, 0x01, 0x00,  0x20,  0x00, 0x02, 0x00, 0x40};
 *       此滤波表的  设置为 必须关心ID的第0位 或第 1 位 为 1 即可存入FIFO中
 *       上位机发送设置为   标识符为标准帧 远程请求无要求
 *    STID[10:3] STID[2:0] RTR IDR EXID[17:15]
 *
 *    其中远程请求的掩码  同上进行设置
 *
 *      table中传入的MASK设置
 *      table[0] 0x28   0b101000 EXID[17:15]  0b101  STID[10:8] 0b000
 *      table[1] 0x00   STID[7:0] 0
 *      table[2] 0x00   MASK_STD[10:3]
 *      table[3] 0x07   0b00000111  MASK_STD[2:0] MASK_RTR MASK_IDE MASK_EXID
 *      table[4] - table[7] 下一组
 *                            id_h  id_l  mask_h mask_l
 * static uint8_t table[8] = {0x28, 0x00, 0x00,  0x07,  0x28, 0x00, 0x00, 0x07};
 */

/**
 * 4个16位 标识符列表滤波器
 * 需在am_hwconf_zlg237_can.c中的__g_can_filter设置为
 * AMHW_ZLG237_CAN_FILTER_MODE_IDLIST  AMHW_ZLG237_CAN_FILTER_SCALE_16BIT
 *
 *注意:  在扩展模式下 只能对 扩展ID的EID[17:15] 进行设置 其他位不能设置（即不能设置标准ID的值）
 *      在标准模式下只能对标准ID的进行设置  不能设置扩展ID的值
 *
 *      table中传入的ID为 标准标识符模式  STID[10:3]   STID[2:0]
 *                     扩展标识符模式 EXID[17:15]  STID[10:3] STID[2:0]
 *
 *                     扩展标识符下其ID高位实际值参照  2个16位过滤器  标识符屏蔽位模式
 *      table[0]  ID高位
 *      table[1]  ID低位
 *      每两个元素为一组ID
 *
 * CAN_IS_STD
 *                             ID_H  ID_L  ID_H ID_L ....
 * static uint8_t table[16] = {0x00, 0x01, 0x00, 0X02, 0x00, 0x03, 0x00, 0x04,
 *                             0x00, 0x05, 0x00, 0X06, 0x00, 0x07, 0x00, 0x08};
 *  此table过滤ID为           0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8
 *  帧模式为  标准帧  远程帧请求需要与配置文件中设置的匹配
 *
 * CAN_ID_EXT
 * static uint8_t table[8] = {0x08, 0x00, 0x18, 0x00, 0x20, 0x00, 0x30, 0x00};
 *  此table可过滤的ID为                            0x8xxx     0x18xxx     0x2xxxx     0x3xxxx
 *  帧模式为  扩展帧  远程帧请求需要与配置文件中设置的匹配
 *
 * 在扩展帧模式下必须在扩展ID为设有ID值 即EXID[17:15] 至少有一位为 1
 *
 */

/**
 *  1个32位过滤器  标识符屏蔽
 *  需在am_hwconf_zlg237_can.c中的__g_can_filter设置为
 *  AMHW_ZLG237_CAN_FILTER_MODE_IDMASK  AMHW_ZLG237_CAN_FILTER_SCALE_32BIT
 *
 *      table中传入的ID为 标准标识符模式  STID[10:0]
 *                     扩展标识符模式 EXID[28：0]
 *
 *  必须关注帧格式的掩码设置和ID的掩码设置是否匹配
 *
 *  其中ID 为 0x70                         0b1 1100 00
 *   其掩码设置关心时 为 table[7]   0x01 0xc0  0b1 1100 0000 (最后两位为 IDE RTR)
 *   MASK :    EXIT[28:0] IDE RTR
 *         或者  STID[10:0] IDE RTR
 *
 * CAN_ID_EXT
 *                            ID_H  ID_H  ID_L  ID_L  MASK_H  MASK_H MASK_L MASK_L
 * static uint8_t table[8] = {0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x01, 0xc0};
 * 上位机设置ID为 0x x7x  帧格式为 扩展帧 远程请求无要求
 *
 * CAN_IS_STD
 *                            ID_H  ID_H  ID_L  ID_L  MASK_H  MASK_H MASK_L MASK_L
 * static uint8_t table[8] = {0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x01, 0xc0};
 * 上位机设置ID为 0x x7x  帧格式为 扩展帧 远程请求无要求
 *
 * table中传入的MASK设置    STID[10:3] STID[2:0] EXID[17:13] EXID[12:5] EXID[4:0] IDR RTR
 *
 *
 *
 */


/**
 *  2个32位过滤器  标识符列表
 *  需在am_hwconf_zlg237_can.c中的__g_can_filter设置为
 *  AMHW_ZLG237_CAN_FILTER_MODE_IDLIST  AMHW_ZLG237_CAN_FILTER_SCALE_32BIT
 *
 *      table中传入的ID为 标准标识符模式  STID[10:0]
 *                     扩展标识符模式 EXID[28：0]
 *

 *
 * table中传入的MASK: STID[10:3] STID[2:0] EXID[17:13] EXID[12:5] EXID[4:0] IDR RTR
 *
 * static uint8_t table[16] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02,
 *                             0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04};
 *
 * 此table过滤ID为  0x1 0x2 0x3 0x4  帧类型和帧格式必须和设置的进行匹配
 *
 *
 *
 */

/**\brief 滤波表 */
 static uint8_t table[8] = {0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x01, 0xc0};

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
    ret = am_can_filter_tab_set(can_handle,table,
                                sizeof(table)/sizeof(uint8_t));

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

        if (can_rcv_msg.msglen  || can_rcv_msg.flags || can_rcv_msg.id) {
            am_kprintf("can recv id: 0x%x\r\n",can_rcv_msg.id);
            for (i = 0; i < can_rcv_msg.msglen; i++) {
                am_kprintf("data: 0x%x \r\n",can_rcv_msg.msgdata[i]);
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
