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
 * \brief USB DEVICE驱动
 *
 * \internal
 * \par Modification History
 * - 1.00 17-09-19  sdq, first implementation
 * \endinternal
 */
#ifndef __AMHW_ZLG217_USBD_H
#define __AMHW_ZLG217_USBD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amhw_zlg217_usbd.h"
#include "am_usb_dci.h"

/**
 * \addtogroup am_zlg217_if_usbd
 * \copydoc am_zlg217_usbd.h
 * @{
 */

/**< \brief zlg217支持的最大端点个数 */
#define     AM_ZLG217_USBD_MAX_EP_CNT         5

/**< \brief zlg217一次性收发的最大字节个数 */
#define     AM_ZLG217_USBD_MAX_EP_DATA_CNT    64

/**< \brief 定义事务类型 */
#define     AM_ZLG217_USB_CTRL_SETUP          0x00
#define     AM_ZLG217_USB_CTRL_IN             0x01
#define     AM_ZLG217_USB_CTRL_OUT            0x02
#define     AM_ZLG217_USB_CTRL_IDLE           0xFF

/**
 * \brief 定义描述符
 *
 *  HOST会根据描述符类型和描述符编号要求从机发送描述符
 *  配置描述符的下级描述符不能越过上级描述符直接获取，可以按级别顺序接在配置描述符之后
 */
typedef struct am_zlg217_usb_descriptor {
    uint16_t  desc_wValue;     /* 高8位为描述符类型，低8位为描述符编号 */
    uint16_t  desc_size;       /* 描述符大小 */
    uint8_t  *p_desc;          /* 描述符内容 */
}am_zlg217_usb_descriptor_t;

/**
 * \brief ZLG217 USB Device initialize parameter structure
 */
typedef struct am_zlg217_usbd_devinfo{
    uint32_t usb_regbase;          /**< \brief usb 基地址 */

    int inum;                      /**< \brief 中断号 */

    am_zlg217_usb_descriptor_t *p_descriptor;    /**< \brief 描述符首地址 */

    uint8_t descriptor_num;        /**< \brief 描述符个数 */

    void (*pfn_plfm_init)(void);   /**< \brief 平台初始化函数 */

    void (*pfn_plfm_deinit)(void); /**< \brief 平台去初始化函数 */
}am_zlg217_usbd_devinfo_t;

/** \brief 端点状态结构 */
typedef struct am_zlg217_usbd_ep_stat
{
    uint8_t inuse;         /**< \brief 端点是否会被使用（端点描述符中是否有该端点） */
    uint8_t stalled;       /**< \brief 端点是否阻塞 */
    //uint8_t transferring;  /**< \brief 是否正在传输 */

    /**< \brief 端点地址
     * [D7]    : 端点的传输方向  1:IN 0:OUT
     * [D6:D4] : 保留，值为0
     * [D3:D0] : 端点号 */
    uint8_t endpoint_address;

    /**< \brief 端点一次性传输的最大包大小 */
    uint16_t max_packet_size;

    /**< \brief 端点传输类型
     *  [D7:D2] 保留
     *  [D1:D0] 00：控制传输
     *          01：同步传输
     *          10：批量传输
     *          11：中断传输 */
    uint8_t  transfer_type;
} am_zlg217_usbd_ep_stat_t;

/**
 * \brief ZLG217 USBD device structure
 */
typedef struct am_zlg217_usbd_dev {
    /**< \brief Device handle used to identify the device object belongs to */
    am_usbd_dev_t  device;

    /**< \brief 中断状态
     *          D4：端点中断
     *          D3：SOF
     *          D2：唤醒
     *          D1：总线挂起
     *          D0：总线复位请求
     */
    uint8_t int_stat;

    /**< \brief 发生中断的端点，为1表示发生中断，为0表示没有发生中断
     *   D7:D5  保留
     *   D4~D0  对应发生中断的端点4~端点0。
     */
    union {
        uint8_t int_ep_flag;
        struct {
            uint8_t ep0 : 1;
            uint8_t ep1 : 1;
            uint8_t ep2 : 1;
            uint8_t ep3 : 1;
            uint8_t ep4 : 1;
        }int_ep_flag_field;
    }int_ep_union;

    /**< \brief 端点中断的类型，为1表示发生中断，为0表示没有发生中断
     *   D7:OUT_STALL
     *   D6:OUT_ACK
     *   D5:OUT_NACK
     *   D4:IN_STALL
     *   D3:IN_ACK
     *   D2:IN_NACK
     *   D1:END       端点传输完成
     *   D0:SETUP     接收到SETUP包
     */
    union {
        uint8_t ep_int_type[AM_ZLG217_USBD_MAX_EP_CNT];
        struct {
            uint8_t setup     : 1;
            uint8_t end       : 1;
            uint8_t in_nack   : 1;
            uint8_t in_ack    : 1;
            uint8_t in_stall  : 1;
            uint8_t out_nack  : 1;
            uint8_t out_ack   : 1;
            uint8_t out_stall : 1;
        }ep_int_type_field[AM_ZLG217_USBD_MAX_EP_CNT];
    }ep_int_type_union;

    /**< \brief 接收到的setup包中的数据 */
    am_usb_setup_struct_t setup_data;

    /**< \brief USB当前正在处理的事务类型
     *     AM_ZLG217_USB_CTRL_SETUP      0x00
     *     AM_ZLG217_USB_CTRL_IN         0x01
     *     AM_ZLG217_USB_CTRL_OUT        0x02
     *     AM_ZLG217_USB_CTRL_IDLE       0xFF
     */
    uint8_t running_ctrl_state;

    //uint8_t ep_num;     /**< \brief 使用的端点个数 */

    /**< \brief 端点状态 */
    am_zlg217_usbd_ep_stat_t endpoint_state[AM_ZLG217_USBD_MAX_EP_CNT];

    /**< \brief 设备信息结构指针 */
    const am_zlg217_usbd_devinfo_t *p_info;
}am_zlg217_usbd_dev_t;

/**< \brief 定义端点中断处理函数指针类型 */
typedef void (*USB_DeviceInterruptEndpoint_t)(am_zlg217_usbd_dev_t *);

/**< \brief 定义具体的端点中断类型 的处理函数类型 */
typedef void (*USB_DeviceInterruptEndpointType_t)(am_zlg217_usbd_dev_t *);

extern am_usbd_handle_t am_zlg217_usbd_init (am_zlg217_usbd_dev_t *p_dev,
                                   const am_zlg217_usbd_devinfo_t *p_devinfo);

extern void am_zlg217_usbd_deinit (const am_zlg217_usbd_devinfo_t *p_info);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AM_ZLG217_USBD_H */

/* end of file */
