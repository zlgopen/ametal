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
 *   6. 若要修改 配置文件注意上位机发送的帧格式和类型  配置文件在 amh_hwconf_hk32f103rbt6_can.c
 *    默认设置在 CAN_ID_EXT
 *               AMHW_HK32F103RBT6_CAN_FILTER_MODE_IDMASK 
 *               AMHW_HK32F103RBT6_CAN_FILTER_SCALE_32BIT
 *      修改帧格式和类型后 ， 需要在注意table的设置 格式变化
 *      详情见 下方注释 和 HK32F103RBT6用户手册17.6.4章  标识符过滤
 * - 实验现象：
 *   1. 上位机发送数据后，收到发送的数据。
 *   2. 串口打印相关调试信息。
 *
 *
 * \par 源代码
 * \snippet demo_hk32f103rbt6_can.c src_std_can
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
#include "amhw_hk32f103rbt6_can.h"
#include "am_hk32f103rbt6_can.h"

/**
 *  \brief 2个16位过滤器  标识符屏蔽位模式
 *  需在am_hwconf_hk32f103rbt6_can.c中的__g_can_filter设置为
 *  AMHW_HK32F103RBT6_CAN_FILTER_MODE_IDMASK  
 *  AMHW_HK32F103RBT6_CAN_FILTER_SCALE_16BIT
 *
 *  滤波表table说明：
 *    在此模式下  table中包含2组 (ID + ID_MASK)
 *    此时ID为     EXID[17:15] STID[10:3] STID[2:0] (EXT_ID)
 *        或者     STID[10:3] STID[2:0](STD_ID)
 *    
 *    帧类型                                       EXT_ID                           STD_ID
 *    table[0] ID_H:      EXID[17:15] STID[10:8]           STID[10:8]
 *    table[1] ID_L:      STID[7:0]                        STID[7:0]
 *
 *    此时MASK为： STID[10:3] STID[2:0] RTR IDR EXID[17:15]
 *    帧类型                                       EXT_ID                            STD_ID
 *    table[2] MASK_H:    STID[10:3]                        STID[10:3]
 *    table[3] MASK_L:    STID[2:0] RTR IDE EXID[17:15]    (STID[2:0] 
 *                                                          RTR IDE EXID[17:15])
 *                                               (STD_ID 模式下 EXID[17:15] 全为 0)
 *
 *            table[4]-table[7] 为下一组
 *  注意 ：
 *      在设置 扩展标识符模式时，其屏蔽为设置只能设置在EXID 
        不能设置在STID的位上 否者数据服无法接收
 *
 *      在设置为标准标识符模式时，其屏蔽位只能设置在STID位上
 *      不能设置在EXID上  否者数据无法接收
 *
 *
 * 在配置文件中修改帧类型 为  CAN_ID_EXT
 * static uint8_t table[8] = {0x28, 0x00, 0x00,  0x07,  0x28, 0x00, 0x00, 0x07};
 *      即 ID_H 0X28 ID_L  0X00  其ID的实际值为 0x28000
 *      即在上位机测试时，需设置ID(0x28,0x00)为   0x28000 (此为18位的扩展ID)
 *      其中0x7 为 扩展标识符掩码 STID[17:15] 每一位必须匹配
 *      上位机发送ID位 28xxx 标识符为扩展  远程请求无要求
 *
 * 在配置文件中修改帧类型为 CAN_IS_STD
 * static uint8_t table[8] = {0x00, 0x01, 0x00,  0x20,  0x00, 0x02, 0x00, 0x40};
 *       此滤波表的  设置为 必须关心ID的第0位 或第 1 位 为 1 即可存入FIFO中
 *       上位机发送设置为   标识符为标准帧 远程请求无要求
 *    STID[10:3] STID[2:0] RTR IDR EXID[17:15]
 * 
 */

/**
 * \brief 4个16位 标识符列表滤波器
 * 需在am_hwconf_hk32f103rbt6_can.c中的__g_can_filter设置为
 * AMHW_HK32F103RBT6_CAN_FILTER_MODE_IDLIST  
 * AMHW_HK32F103RBT6_CAN_FILTER_SCALE_16BIT
 *
 *  滤波表table说明：
 *    在此模式下  table中包4组 (ID)
 *    此时ID为     EXID[17:15] STID[10:3] STID[2:0] (EXT_ID)
 *        或者     STID[10:3] STID[2:0](STD_ID)
 *    
 *    帧类型                                       EXT_ID                           STD_ID
 *    table[0] ID_H:      EXID[17:15] STID[10:8]           STID[10:8]
 *    table[1] ID_L:      STID[7:0]                        STID[7:0]
 * 
 *    每两组元素为一组ID依次类推
 * 
 * 注意: 在扩展模式下只能对扩展ID的EID[17:15] 进行设置 其他位不能设置
 *                                         （即不能设置标准ID的值）
 *       在标准模式下只能对标准ID的进行设置  不能设置扩展ID的值
 *
 * 在配置文件中修改帧类型 为  CAN_ID_EXT
 * static uint8_t table[8] = {0x08, 0x00, 0x18, 0x00, 0x20, 0x00, 0x30, 0x00};
 *  此table可过滤的ID为       0x8xxx      0x18xxx     0x2xxxx     0x3xxxx
 *  帧模式为  扩展帧  远程帧请求需要与配置文件中设置的匹配
 *
 * 在扩展帧模式下必须在扩展ID为设有ID值 即EXID[17:15] 至少有一位为 1
 *
 * 在配置文件中修改帧类型为 CAN_IS_STD
 *                             
 * static uint8_t table[16] = {0x00, 0x01, 0x00, 0X02, 0x00, 0x03, 0x00, 0x04,
 *                             0x00, 0x05, 0x00, 0X06, 0x00, 0x07, 0x00, 0x08};
 *  此table过滤ID为           0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8
 *  帧模式为  标准帧  远程帧请求需要与配置文件中设置的匹配
 *
 *
 */

/**
 *  \brief 1个32位过滤器  标识符屏蔽
 *  需在am_hwconf_hk32f103rbt6_can.c中的__g_can_filter设置为
 *  AMHW_HK32F103RBT6_CAN_FILTER_MODE_IDMASK 
 *  AMHW_HK32F103RBT6_CAN_FILTER_SCALE_32BIT
 * 
 *  必须关注帧格式的掩码设置和ID的掩码设置是否匹配
 *
 *   滤波表table说明
 *   table中包含 1 组(ID + ID_MASK)
 *   此时ID为：STID[10:3] STID[2:0] EXID[17:13] EXID[12:5] EXID[4:0] (EXID[28:0])
 *   或者      STID[10:3] STID[2:0]
 *     帧类型                                                   EXT_ID                     STD_ID
 *   table[0] ID_H :            EXID[28:24]                     
 *   table[1] ID_H :            EXID[23:16]
 *   table[2] ID_L :            EXID[15:8]                 STD_ID[10:8]
 *   table[3] ID_L :            EXID[7:0]                  STD_ID[7:0]
 *
 *   此时ID掩码为：STID[10:3] STID[2:0] EXID[17:13] EXID[12:5] EXID[4:0] IDE RTR  
 *   或者          STID[10:3] STID[2:0] IDE RTR
 *     帧类型                                                   EXT_ID                     STD_ID
 *    table[4] MASK_H :         EXID[28:22]                     
 *    table[5] MASK_H :         EXID[21:14]
 *    table[6] MASK_L :         EXID[13:6]                 STD_ID[10:6]
 *    table[7] MASK_L :         EXID[5:0]  IDE RTR         STD_ID[5:0]  IDE RTR
 * 
 *    IDE(1bit) 帧类型屏蔽位 
 *             0 : 不关心帧类型   
 *             1 ：帧类型匹配 与配置文件(am_hwconf_hk32f103rbt6_can.c)中一致才接收数据 
 *
 *    RTR(1bit) 帧格式屏蔽位
 *             0 : 不关心帧格式
 *             1 ：帧格式匹配 与配置文件(am_hwconf_hk32f103rbt6_can.c)中一致才接收数据 
 *    注意： 当RTR设置为关心， ID掩码设置为关心时，则上位机设置的帧类型也必须与
 *           配置文件中的一致才接收
 *
 *  其中ID 为 0x70                         0b1 1100 00
 *   其掩码设置关心时 为 table[7]   0x01 0xc0  0b1 1100 0000 (最后两位为 IDE RTR)
 *   MASK :    EXIT[28:0] IDE RTR
 *         或者  STID[10:0] IDE RTR
 *
 * 在配置文件中修改为 CAN_ID_EXT
 *                            ID_H  ID_H  ID_L  ID_L MASK_H MASK_H MASK_L MASK_L
 * static uint8_t table[8] = {0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x01, 0xc0};
 * 上位机设置ID为 0x x7x  帧格式为 扩展帧 远程请求无要求
 *
 * 在配置文件中修改为 CAN_IS_STD
 *                            ID_H  ID_H  ID_L  ID_L MASK_H MASK_H MASK_L MASK_L
 * static uint8_t table[8] = {0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x01, 0xc0};
 * 上位机设置ID为 0x x7x  帧格式为 标准帧  远程请求无要求
 *
 *
 */


/**
 *  \brief 2个32位过滤器  标识符列表
 *  需在am_hwconf_hk32f103rbt6_can.c中的__g_can_filter设置为
 *  AMHW_HK32F103RBT6_CAN_FILTER_MODE_IDLIST
 *  AMHW_HK32F103RBT6_CAN_FILTER_SCALE_32BIT
 *   
 *  滤波表table说明：
 *   table中包含 2 组 (ID):
 *   此时ID为： STID[10:3] STID[2:0] EXID[17:13] EXID[12:5] EXID[4:0] 
 *   或者       STID[10:3] STID[2:0]
 *      帧类型                                           EXT_ID                       STD_ID
 *  table[0] ID_H :           EXID[28:24]                     
 *  table[1] ID_H :           EXID[23:16]
 *  table[2] ID_L :           EXID[15:8]                   STD_ID[10:8]
 *  table[3] ID_L :           EXID[7:0]                    STD_ID[7:0]
 *
 *  table[4] - table[7] 依次类推
 *
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
void demo_hk32f103rbt6_can_entry (am_can_handle_t      can_handle,
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
/** [src_hk32f103rbt6_can] */

/* end of file */
