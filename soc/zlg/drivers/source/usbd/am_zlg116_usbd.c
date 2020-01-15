/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief USBD 驱动层实现函数
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-29  sdq, first implementation.
 * \endinternal
 */

#include "am_zlg116_usbd.h"
#include "amhw_zlg116_usbd.h"
#include "am_usb.h"
#include "am_int.h"
#include "am_usb_spec.h"
#include "am_usbd_ch9.h"

/* 需要调试USB的时候去除注释，将会在串口中打印出触发USB中断后程序的流程 */
//#define USB_DEBUG

/**
 * \brief 通过wValue的值寻找指定的描述符
 *
 * \retval 成功返回描述符指针，失败返回NULL
 *
 * \note 配置描述符的下级描述符不能通过此法获得
 */
static uint8_t * __find_desc_by_wValue1 (const am_zlg126_usbd_dev_t *p_dev,
                                         uint16_t                 w_value)
{
    int i;

    for (i = 0; i < p_dev->p_info->p_devinfo->descriptor_num; i++) {
        if (w_value == p_dev->p_info->p_devinfo->p_descriptor[i].desc_wValue) {
            return (uint8_t *)(p_dev->p_info->p_devinfo->p_descriptor[i].p_desc);
        }
    }
    return NULL;
}

/**
 * \brief 初始化指定的端点
 *
 * \param[in] p_dev  : USB设备
 * \param[in] epInit : 端点初始化信息
 *
 * \note 该函数并不是随意指定一个硬件有的端点，初始化的端点必须是在描述符中指定过的，并且地址和传输方向
 *       必须一致，也就是说，p_dev->device.endpoint_info[i].endpoint_address和
 *       epInit->endpoint_address的值必须相等，才能进行初始化。
 */
static am_usb_status_t __usb_device_endpoint_init (am_zlg126_usbd_dev_t       *p_dev,
                                                   am_usbd_endpoint_init_t    *epinit)
{
    amhw_zlg116_usbd_t *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    uint16_t max_packet_size = epinit->max_packet_size;
    uint8_t  endpoint = (epinit->endpoint_address & AM_USBD_ENDPOINT_NUMBER_MASK);
    int i;

    for (i = 0; i < AM_USBD_MAX_EP_CNT; i++) {
        /* 端点描述符中必须指定了这个端点 */
        if (p_dev->device.endpoint_info[i].inuse == AM_TRUE &&
            p_dev->device.endpoint_info[i].ep_address == epinit->endpoint_address) {
            break;
        }
    }
    if (i == AM_USBD_MAX_EP_CNT)
        return AM_USB_STATUS_INVALID_REQUEST;

    if (endpoint >= AM_USBD_MAX_EP_CNT)
        return AM_USB_STATUS_INVALID_PARAMETER;
    if (epinit->transfer_type > AM_USB_ENDPOINT_INTERRUPT)
        return AM_USB_STATUS_INVALID_PARAMETER;

    /* 设置端点能传输的最大字节个数 */
    if (max_packet_size > AM_USBD_MAX_EP_DATA_CNT)
        max_packet_size = AM_USBD_MAX_EP_DATA_CNT;

    amhw_zlg116_usbd_ep_halt_reset(p_usb, 1 << endpoint);     /* 清除端点暂停 */
    amhw_zlg116_usbd_ep_enable(p_usb, 1 << endpoint);         /* 使能端点 */

    p_dev->device.endpoint_info[endpoint].stalled = 0;
    p_dev->device.endpoint_info[endpoint].max_packet_size = max_packet_size;
    p_dev->device.endpoint_info[endpoint].transfer_type = epinit->transfer_type;

    return AM_USB_STATUS_SUCCESS;
}

/**
 * \brief 对某个端点去初始化
 *
 *  \param[in] p_dev       : USB设备
 *  \param[in] endpoint    : 指定的端点
 */
static am_usb_status_t __usb_device_endpoint_deinit (am_zlg126_usbd_dev_t *p_dev,
                                                    uint8_t            endpoint)
{
    amhw_zlg116_usbd_t *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    endpoint &= AM_USBD_ENDPOINT_NUMBER_MASK;

    amhw_zlg116_usbd_ep_disable(p_usb, 1 << endpoint);  /* 禁能端点 */

    return AM_USB_STATUS_SUCCESS;
}

/**
 * \brief 设置端点阻塞
 *
 * \param[in] p_dev         : USB设备
 * \param[in] endpoint_addr : 端点地址
 */
static am_usb_status_t __usb_device_endpoint_stall (am_zlg126_usbd_dev_t *p_dev,
                                                   uint8_t            endpoint_addr)
{
    uint8_t endpoint = endpoint_addr & AM_USBD_ENDPOINT_NUMBER_MASK;
    amhw_zlg116_usbd_t *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    p_dev->device.endpoint_info[endpoint].stalled = 1;    /* 设置阻塞标志 */

    amhw_zlg116_usbd_ep_halt_set(p_usb, 1 << endpoint);

    return AM_USB_STATUS_SUCCESS;
}

/**
 * \brief 设置端点不阻塞
 *
 * \param[in] p_dev         : USB设备
 * \param[in] endpoint_addr : 端点地址  D7： 1：USB_IN  0：USB_OUT
 */
static am_usb_status_t __usb_device_endpoint_unstall (am_zlg126_usbd_dev_t *p_dev,
                                                      uint8_t               endpoint_addr)
{
    amhw_zlg116_usbd_t *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    uint8_t endpoint = endpoint_addr & AM_USBD_ENDPOINT_NUMBER_MASK;

    p_dev->device.endpoint_info[endpoint].stalled = 0;

    amhw_zlg116_usbd_dtog_data0(p_usb, endpoint);

    amhw_zlg116_usbd_ep_halt_reset(p_usb, 1 << endpoint);

    return AM_USB_STATUS_SUCCESS;
}

/**
 * \brief 初始化USB设备
 */
static am_usb_status_t __usb_device_init (am_usbd_handle_t handle)
{
    int i = 0;
    am_usbd_endpoint_init_t endpoint;
    am_zlg126_usbd_dev_t *p_dev = (am_zlg126_usbd_dev_t *)handle;
    amhw_zlg116_usbd_t   *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    /* 复位USB控制器的端点和FIFO */
    amhw_zlg116_usbd_connect_set(p_usb, ZLG116_USB_DISCONNECT);
    amhw_zlg116_usbd_reset_set(p_usb, ZLG116_USB_RESET);
    amhw_zlg116_usbd_reset_set(p_usb, ZLG116_USB_NORESET);
    amhw_zlg116_usbd_connect_set(p_usb, ZLG116_USB_DISCONNECT);

    /* 清中断状态 */
    amhw_zlg116_usbd_int_state_clear(p_usb, AMHW_ZLG116_USB_INT_STATE_ALL);

    /* 清端点中断状态 */
    amhw_zlg116_usbd_ep_int_state_clear(p_usb, AMHW_ZLG116_EP_INT_STATE_EP_ALL);
    p_dev->int_ep_union.int_ep_flag = 0;

    amhw_zlg116_usbd_ep0_int_state_clear(p_usb, AMHW_ZLG116_EP0_INT_STATE_ALL);
    p_dev->ep_int_type_union.ep_int_type[0] = 0;

    for (i = 0; i < AM_USBD_MAX_EP_CNT - 1; i++) {
        amhw_zlg116_usbd_epx_int_state_clear(p_usb, (zlg116_usb_epx_t)i, AMHW_ZLG116_EPX_INT_STATE_ALL);
        p_dev->ep_int_type_union.ep_int_type[i + 1] = 0;
    }

    /* 使能USB中断（未使能SOF检测中断） */
    /* 如果使能了SOF中断的话那每隔1ms就会进入一次中断，如果不使能SOF中断，中断标志也会置位，但是不会
                 进入中断     */
    amhw_zlg116_usbd_int_enable(p_usb,
                                AMHW_ZLG116_USB_INT_EN_RSTIE |
                                AMHW_ZLG116_USB_INT_EN_SUSPENDIE |
                                AMHW_ZLG116_USB_INT_EN_RESUMIE |
                                AMHW_ZLG116_USB_INT_EN_EPIE);

    /* 使能端点中断 */
    for (i = 0; i < AM_USBD_MAX_EP_CNT; i++) {
        if (p_dev->device.endpoint_info[i].inuse == AM_TRUE)
            amhw_zlg116_usbd_ep_int_enable(p_usb, 1 << i);
    }

    /* 使能端点0的所有中断 */
    amhw_zlg116_usbd_ep0_int_enable(p_usb, AMHW_ZLG116_EP0_INT_EN_ALL);
    /* 使能其他端点的所有中断 */
    for (i = 0; i < AM_USBD_MAX_EP_CNT - 1; i++) {
        if (p_dev->device.endpoint_info[i + 1].inuse == AM_TRUE)
            amhw_zlg116_usbd_epx_int_enable(p_usb, (zlg116_usb_epx_t)i, AMHW_ZLG116_EPX_INT_EN_ALL);
    }

    /* 清除SETUP数据 */
    p_dev->device.setup_data.bm_request_type = 0;
    p_dev->device.setup_data.b_request = 0;
    p_dev->device.setup_data.w_value = 0;
    p_dev->device.setup_data.w_index = 0;
    p_dev->device.setup_data.w_length = 0;


    /**< \brief 初始化端点 */
    for (i = 0; i < AM_USBD_MAX_EP_CNT; i++) {
        if (p_dev->device.endpoint_info[i].inuse == AM_TRUE) {
            endpoint.endpoint_address = p_dev->device.endpoint_info[i].ep_address;
            endpoint.max_packet_size = p_dev->device.endpoint_info[i].max_packet_size;
            endpoint.transfer_type = p_dev->device.endpoint_info[i].transfer_type;
            __usb_device_endpoint_init(p_dev, &endpoint);
        }
    }


    /* 设置地址为0 */
    amhw_zlg116_usbd_addr_set(p_usb, 0);
    p_dev->device.device_address = 0;

    amhw_zlg116_usbd_active_set(p_usb, ZLG116_USB_ACTIVE);      /* 活跃 */
    amhw_zlg116_usbd_speed_set(p_usb, ZLG116_USB_SPEED_FULL);   /* 全速 */
    amhw_zlg116_usbd_connect_set(p_usb, ZLG116_USB_CONNECT);    /* 连接 */

    return AM_USB_STATUS_SUCCESS;
}

/**
 * \bruef USB去初始化
 *
 * \param[in] USB操作句柄
 *
 * \retval USB错误码
 */
static am_usb_status_t __usb_device_deinit (am_usbd_handle_t handle)
{
    am_zlg126_usbd_dev_t *p_dev = (am_zlg126_usbd_dev_t *)handle;
    amhw_zlg116_usbd_t   *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    if (handle == NULL){
        return AM_USB_STATUS_INVALID_HANDLE;
    }

    amhw_zlg116_usbd_int_disable(p_usb, AMHW_ZLG116_USB_INT_EN_ALL); /* 禁能USB中断 */
    amhw_zlg116_usbd_ep_int_disable(p_usb, AMHW_ZLG116_EP_INT_EN_ALL);   /* 禁能端点中断 */
    amhw_zlg116_usbd_ep0_int_disable(p_usb, AMHW_ZLG116_EP0_INT_EN_ALL); /* 禁能端点0的中断 */
    amhw_zlg116_usbd_ep_disable(p_usb, AMHW_ZLG116_USB_EP_ALL);      /* 禁能所有端点 */

    /* 禁能端点的中断 */
    amhw_zlg116_usbd_epx_int_disable(p_usb, ZLG116_USB_EP1,
                                  AMHW_ZLG116_EPX_INT_EN_ALL);
    amhw_zlg116_usbd_epx_int_disable(p_usb, ZLG116_USB_EP2,
                                  AMHW_ZLG116_EPX_INT_EN_ALL);
    amhw_zlg116_usbd_epx_int_disable(p_usb, ZLG116_USB_EP3,
                                  AMHW_ZLG116_EPX_INT_EN_ALL);
    amhw_zlg116_usbd_epx_int_disable(p_usb, ZLG116_USB_EP4,
                                  AMHW_ZLG116_EPX_INT_EN_ALL);

    if (p_dev->p_info->pfn_plfm_deinit)
        p_dev->p_info->pfn_plfm_deinit();

    return AM_USB_STATUS_SUCCESS;
}

/**
 * \brief USB设备发送数据
 *
 * \param[in] handle           : USB控制句柄
 * \param[in] endpoint_address : 指定的端点地址
 * \param[in] buffer           : 指向要发送的数据
 * \param[in] length           : 要发送的数据长度
 *
 * \note 如果传入的length为0，则发送空包，如果传入的buffer为NULL，但length不为0，则发送length
 *       个0
 *
 * \retval USB错误码
 */
static am_usb_status_t __usb_device_send (am_usbd_handle_t handle,
                                          uint8_t          endpoint_address,
                                          uint8_t         *buffer,
                                          uint32_t         length)
{
    am_zlg126_usbd_dev_t *p_dev = (am_zlg126_usbd_dev_t *)handle;
    uint32_t send_once_size = 0;
    uint8_t endpoint = endpoint_address & AM_USBD_ENDPOINT_NUMBER_MASK;
    amhw_zlg116_usbd_t *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    int i = 0;

    if (endpoint >= AM_USBD_MAX_EP_CNT)
        return AM_USB_STATUS_INVALID_PARAMETER;

    if (length == 0) {  /* 发送空包 */
        while (!amhw_zlg116_usbd_transfer_end(p_usb, (zlg116_usb_epx2_t)endpoint));
        amhw_zlg116_usbd_epx_transfer(p_usb, (zlg116_usb_epx2_t)endpoint, 0);
    } else {
        while (length) {        /* 如果一次不能完全传输则分多次传输 */
            if (length > AM_USBD_MAX_EP_DATA_CNT) {
                send_once_size = AM_USBD_MAX_EP_DATA_CNT;
            } else {
                send_once_size = length;
            }
            length -= send_once_size;
            while (!amhw_zlg116_usbd_transfer_end(p_usb, (zlg116_usb_epx2_t)endpoint));
            if (buffer) {
                for (i = 0; i < send_once_size; i++) {
                    amhw_zlg116_usbd_epx_fifo_write(p_usb, (zlg116_usb_epx2_t)endpoint, *buffer++);
                }
            } else {
                for (i = 0; i < send_once_size; i++) {
                    amhw_zlg116_usbd_epx_fifo_write(p_usb, (zlg116_usb_epx2_t)endpoint, 0);
                }
            }
            amhw_zlg116_usbd_epx_transfer(p_usb, (zlg116_usb_epx2_t)endpoint, send_once_size);
        }
    }

    return AM_USB_STATUS_SUCCESS;
}

/**
 * \brief 端点0发送空包
 */
static void __ep0_send_empty_packet (am_usbd_handle_t handle)
{
    am_zlg126_usbd_dev_t *p_dev = (am_zlg126_usbd_dev_t *)handle;
    amhw_zlg116_usbd_t   *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    switch (p_dev->device.state) {
    case AM_USBD_CTRL_SETUP:
        p_dev->device.state = AM_USBD_CTRL_IN;
        break;

    case AM_USBD_CTRL_IN:
        while (!amhw_zlg116_usbd_transfer_end(p_usb, ZLG116_USB_EPX0));
        amhw_zlg116_usbd_epx_transfer(p_usb, ZLG116_USB_EPX0, 0);
        p_dev->device.state = AM_USBD_CTRL_IDLE;
        break;

    default:
        p_dev->device.state = AM_USBD_CTRL_IDLE;
    }
}

/**
 * \brief USB设备接收数据
 *
 * \param[in] handle           : USB控制句柄
 * \param[in] endpoint_address : 指定的端点地址
 * \param[in] buffer           : 指向要发送的数据
 * \param[in] length           : 要接收的数据长度
 *
 * \retval USB错误码
 *
 * \note 该函数从fifo中取出数据.
 *       如果fifo中没有数据，则返回AM_USB_STATUS_ERROR.
 *       如果fifo中有数据但是数据个数不够指定的数据长度，则返回AM_USB_STATUS_ALLOC_FAIL并填充缓冲区。
 *       如果fifo中有数据并且数据个数等于指定的数据长度，则返回AM_USB_STATUS_SUCCESS并填充缓冲区。
 *       如果fifo中有数据并且数据个数大于指定的数据长度，则返回AM_USB_STATUS_SUCCESS并填充指定的数据长度。
 */
static am_usb_status_t __usb_device_recv (am_usbd_handle_t handle,
                                          uint8_t                   endpoint_address,
                                          uint8_t                  *buffer,
                                          uint32_t                  length)
{
    am_zlg126_usbd_dev_t *p_dev = (am_zlg126_usbd_dev_t *)handle;
    amhw_zlg116_usbd_t   *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    am_usb_status_t error = AM_USB_STATUS_ERROR;
    uint8_t endpoint = endpoint_address & AM_USBD_ENDPOINT_NUMBER_MASK;
    uint8_t avali_data_cnt = 0;     /* fifo中有效数据个数 */

    /* 接收数据的端点方向必须是OUT_OUT */
    if ((endpoint_address & AM_USB_DESC_ENDPOINT_ADDR_DIR_MASK) !=
            AM_USB_DESC_ENDPOINT_ADDR_DIR_OUT)
        return AM_USB_STATUS_INVALID_REQUEST;

    if (endpoint >= AM_USBD_MAX_EP_CNT)
        return AM_USB_STATUS_INVALID_PARAMETER;

    avali_data_cnt = amhw_zlg116_usbd_epx_avail_size_get(p_usb, (zlg116_usb_epx2_t)endpoint);
    p_dev->device.endpoint_info[endpoint].val_length = avali_data_cnt;

    if (avali_data_cnt == 0)
        return AM_USB_STATUS_ERROR;
    else if (avali_data_cnt < length)
        error = AM_USB_STATUS_ALLOC_FAIL;
    else if (avali_data_cnt == length)
        error = AM_USB_STATUS_SUCCESS;
    else if (avali_data_cnt > length) {
        error = AM_USB_STATUS_SUCCESS;
        avali_data_cnt = length;
    }

    while (avali_data_cnt--) {
        *buffer++ = amhw_zlg116_usbd_epx_fifo_read(p_usb, (zlg116_usb_epx2_t)endpoint);
    }

    return error;
}

/**
 * \brief 终止某个端点正在进行的传输
 *
 * \param[in] handle        : 设备句柄
 * \param[in] endpoint_addr : 端点地址
 *
 * \retval USB错误码
 */
static am_usb_status_t __usb_device_cancel (am_usbd_handle_t handle,
                                            uint8_t          endpoint_addr)
{

    return AM_USB_STATUS_ERROR;
}

/**
 * \brief 设置设备为默认状态
 *
 * \param[in] p_dev:设备实例（操作句柄）
 *
 * \note 默认状态为使能所有中断并使能所有端点，将设备地址置0
 */
static void __usb_device_setdefault_state(am_zlg126_usbd_dev_t *p_dev)
{
    /* 清除地址 */
    amhw_zlg116_usbd_t   *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    amhw_zlg116_usbd_addr_set(p_usb, 0);

    amhw_zlg116_usbd_ep_enable(p_usb, AMHW_ZLG116_USB_EP_ALL);  /* 使能端点 */

    /* 清中断状态 */
    amhw_zlg116_usbd_int_state_clear(p_usb, AMHW_ZLG116_USB_INT_STATE_ALL);
    amhw_zlg116_usbd_ep0_int_state_clear(p_usb, AMHW_ZLG116_EP0_INT_STATE_ALL);
    amhw_zlg116_usbd_epx_int_state_clear(p_usb, ZLG116_USB_EP1,
                                         AMHW_ZLG116_EPX_INT_STATE_ALL);
    amhw_zlg116_usbd_epx_int_state_clear(p_usb, ZLG116_USB_EP2,
                                         AMHW_ZLG116_EPX_INT_STATE_ALL);
    amhw_zlg116_usbd_epx_int_state_clear(p_usb, ZLG116_USB_EP3,
                                         AMHW_ZLG116_EPX_INT_STATE_ALL);
    amhw_zlg116_usbd_epx_int_state_clear(p_usb, ZLG116_USB_EP4,
                                         AMHW_ZLG116_EPX_INT_STATE_ALL);

    /* 使能USB中断 */
    amhw_zlg116_usbd_int_enable(p_usb, AMHW_ZLG116_USB_INT_EN_ALL);

    /* 使能所有端点中断 */
    amhw_zlg116_usbd_ep_int_enable(p_usb, AMHW_ZLG116_EP_INT_EN_ALL);

    /* 使能端点0的所有中断 */
    amhw_zlg116_usbd_ep0_int_enable(p_usb, AMHW_ZLG116_EP0_INT_EN_ALL);

    /* 使能其他端点的所有中断 */
    amhw_zlg116_usbd_epx_int_enable(p_usb, ZLG116_USB_EP1,
                                 AMHW_ZLG116_EPX_INT_EN_ALL);
    amhw_zlg116_usbd_epx_int_enable(p_usb, ZLG116_USB_EP2,
                                 AMHW_ZLG116_EPX_INT_EN_ALL);
    amhw_zlg116_usbd_epx_int_enable(p_usb, ZLG116_USB_EP3,
                                 AMHW_ZLG116_EPX_INT_EN_ALL);
    amhw_zlg116_usbd_epx_int_enable(p_usb, ZLG116_USB_EP4,
                                 AMHW_ZLG116_EPX_INT_EN_ALL);
}


/**
 * \brief USB控制函数，用于设置USB为指定的状态
 *
 * \param[in] handle : USB操作句柄
 * \param[in] type   : 控制类型
 * \param[in] param  : 控制参数
 *
 * \retval USB错误码
 */
static am_usb_status_t __usb_device_control(am_usbd_handle_t          handle,
                                            am_usbd_control_type_t    type,
                                            void                     *param)
{
    am_zlg126_usbd_dev_t *p_dev = (am_zlg126_usbd_dev_t *)handle;
    amhw_zlg116_usbd_t   *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    am_usb_status_t error = AM_USB_STATUS_ERROR;
    uint8_t *temp8;
    am_usbd_ep_status_t *endpointStatus = NULL;
    uint8_t endpoint = 0;

    if (!handle)
        return AM_USB_STATUS_INVALID_HANDLE;

    switch ((int)type) {
        case AM_USBD_CONTROL_RUN:
            amhw_zlg116_usbd_connect_set(p_usb, ZLG116_USB_CONNECT);    /* 连接 */
            error = AM_USB_STATUS_SUCCESS;
            break;

        case AM_USBD_CONTROL_STOP:
            amhw_zlg116_usbd_connect_set(p_usb, ZLG116_USB_DISCONNECT); /* 断开连接 */
            error = AM_USB_STATUS_SUCCESS;
            break;

        case AM_USBD_CONTROL_ENDPOINT_INIT:     /* 对某个端点初始化 */
            if (param) {
                error = __usb_device_endpoint_init(p_dev,
                                                  (am_usbd_endpoint_init_t *)param);
            }
            break;

        case AM_USBD_CONTROL_ENDPOINT_DEINIT:   /* 对某个端点去初始化 */
            if (param) {
                temp8 = (uint8_t *)param;
                error = __usb_device_endpoint_deinit(p_dev, *temp8);
            }
            break;

        case AM_USBD_CONTROL_ENDPOINT_STALL:    /* 控制端点阻塞 */
            if (param) {
                temp8 = (uint8_t *)param;
                error = __usb_device_endpoint_stall(p_dev, *temp8);
            }
            break;

        case AM_USBD_CONTROL_ENDPOINT_UNSTALL:  /* 控制端点不阻塞 */
            if (param) {
                temp8 = (uint8_t *)param;
                error = __usb_device_endpoint_unstall(p_dev, *temp8);
            }
            break;

        case AM_USBD_CONTROL_GET_DEVICE_STATUS: /* 获取设备状态 */

            break;

        case AM_USBD_CONTROL_GET_ENDPOINT_STATUS:   /* 通过端点地址获取端点状态 */
            if (param) {
                endpointStatus = (am_usbd_ep_status_t *)param;

                endpoint = (endpointStatus->endpoint_address) & AM_USBD_ENDPOINT_NUMBER_MASK;

                if (endpoint < AM_USBD_MAX_EP_CNT) {
                    endpointStatus->endpoint_status = p_dev->device.endpoint_info[endpoint].stalled;
                    error = AM_USB_STATUS_SUCCESS;
                } else {
                    error = AM_USB_STATUS_INVALID_PARAMETER;
                }
            }
            break;

        case AM_USBD_CONTROL_SET_DEVICE_ADDRESS:    /* 设置USB设备地址 */
            if (param) {
                error = AM_USB_STATUS_SUCCESS;
            }
            break;

        case AM_USBD_CONTROL_GET_SYNCH_FRAME:
            break;

        case AM_USBD_CONTROL_RESUME:
            amhw_zlg116_usbd_wakeup(p_usb);
            break;

            /* 设置默认状态 */
        case AM_USBD_CONTROL_SET_DEFAULT_STATUS:
            __usb_device_setdefault_state(p_dev);
            error = AM_USB_STATUS_SUCCESS;
            break;

        case AM_USBD_CONTROL_GET_SPEED:
            if (param) {
                temp8 = (uint8_t *)param;
                *temp8 = AM_USB_SPEED_FULL;
                error = AM_USB_STATUS_SUCCESS;
            }
            break;

        case AM_USBD_CONTROL_GET_OTG_STATUS:
            break;
        case AM_USBD_CONTROL_SET_OTG_STATUS:
            break;
        case AM_USBD_CONTROL_SET_TEST_MODE:
            break;
        default:
            break;
    }
    return error;
}

static const am_usbd_interface_t __g_usb_device_interface = {
    __usb_device_init,
    __usb_device_deinit,
    __usb_device_send,
    __usb_device_recv,
    __usb_device_cancel,
    __usb_device_control
};

/**
 * \brief 事务处理
 */
static void __ctrl_deal_handle (am_zlg126_usbd_dev_t *p_dev)
{
    am_usbd_dev_t   *p_usbd_dev = &p_dev->device;
    am_data_info_t  *p_ctrl     = &p_usbd_dev->ctrl_info;

    if(p_ctrl->p_buf != NULL){
        if(p_ctrl->length > p_usbd_dev->setup_data.w_length){
            p_ctrl->length = p_usbd_dev->setup_data.w_length;
        }
        am_usbd_send(p_usbd_dev, 0, p_ctrl->p_buf, p_ctrl->length);
    }else{
        __ep0_send_empty_packet(p_dev);
    }
    p_usbd_dev->state = AM_USBD_CTRL_IDLE;
}

/**
 * \brief 处理IN事务
 */
static void __usb_in_handle(am_zlg126_usbd_dev_t *p_dev)
{
    if ((p_dev->device.setup_data.bm_request_type &
            AM_USB_REQ_TYPE_DIR_MASK) == AM_USB_REQ_TYPE_DIR_IN &&
            p_dev->device.state == AM_USBD_CTRL_SETUP) {
        __ctrl_deal_handle(p_dev);
    }else{
        __ep0_send_empty_packet(p_dev);
    }
}

/**
 * \brief setup事务处理
 */
static void __usb_setup_handle (am_zlg126_usbd_dev_t *p_dev)
{
    uint8_t ret = AM_USB_STATUS_NOT_SUPPORTED;
    uint8_t code = 0, type = 0;
    amhw_zlg116_usbd_t *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    am_usbd_dev_t      *p_usb_dev = &(p_dev->device);
    am_data_info_t     *p_data_info = &(p_usb_dev->ctrl_info);

    p_data_info->offset = 0;
    p_data_info->p_buf  = NULL;

    if(p_usb_dev->state == AM_USBD_CTRL_IDLE) {  // 如果状态为空闲态
        amhw_zlg116_usbd_setupdata_get(p_usb, (uint8_t *)&(p_usb_dev->setup_data)); // 请求数据包
        p_usb_dev->state = AM_USBD_CTRL_SETUP;   // 更新状态
        type = p_usb_dev->setup_data.bm_request_type;
        code = p_usb_dev->setup_data.b_request;
        //am_kprintf("bm_request_type = %02x\r\n", p_dev->device.setup_data.bm_request_type);
        //am_kprintf("b_request = %02x\r\n", p_dev->device.setup_data.b_request);
        //am_kprintf("w_value = %02x\r\n", p_dev->device.setup_data.w_value);
        //am_kprintf("w_index = %02x\r\n", p_dev->device.setup_data.w_index);
        //am_kprintf("w_length = %02x\r\n", p_dev->device.setup_data.w_length);
    }


    /* 判断请求类型的类型 */
    switch (type & AM_USB_REQ_TYPE_MASK) {
    /* 标准请求类型 */
    case AM_USB_REQ_TYPE_STANDARD:
        /* 合法请求判断 */
        switch (code) {
            case AM_USB_REQ_STANDARD_GET_STATUS:
                ret = p_usb_dev->p_funcs->pfn_status_get(
                        p_usb_dev, &p_data_info->p_buf, &p_data_info->length);
                break;
            case AM_USB_REQ_STANDARD_CLEAR_FEATURE:
                ret = p_usb_dev->p_funcs->pfn_feature_clr(p_usb_dev);
                break;
            case AM_USB_REQ_STANDARD_SET_FEATURE:
                ret = p_usb_dev->p_funcs->pfn_feature_set(p_usb_dev);
                break;
            case AM_USB_REQ_STANDARD_SET_ADDRESS:
                p_usb_dev->device_address = p_usb_dev->setup_data.w_value;
                ret = AM_USB_STATUS_SUCCESS;
                break;
            case AM_USB_REQ_STANDARD_GET_DESCRIPTOR:
                ret = p_usb_dev->p_funcs->pfn_desc_get(
                        p_usb_dev, &p_data_info->p_buf, &p_data_info->length);
                break;
            case AM_USB_REQ_STANDARD_SET_DESCRIPTOR:
//                ret = p_usb_dev->p_funcs->pfn_feature_set(p_usb_dev);
                break;
            case AM_USB_REQ_STANDARD_GET_CONFIGURATION:
                ret = p_usb_dev->p_funcs->pfn_config_get(
                        p_usb_dev, &p_data_info->p_buf, &p_data_info->length);
                break;
            case AM_USB_REQ_STANDARD_SET_CONFIGURATION:
                ret = p_usb_dev->p_funcs->pfn_config_set(p_usb_dev);
                break;
            case AM_USB_REQ_STANDARD_GET_INTERFACE:
                ret = p_usb_dev->p_funcs->pfn_interface_get(
                        p_usb_dev, &p_data_info->p_buf, &p_data_info->length);
                break;
            case AM_USB_REQ_STANDARD_SET_INTERFACE:
                ret = p_usb_dev->p_funcs->pfn_interface_set(p_usb_dev);
                break;
            case AM_USB_REQ_STANDARD_SYNCH_FRAME:

                break;
            default:
                break;
        }
        break;
    /* 类请求 */
    case AM_USB_REQ_TYPE_CLASS:
        if(p_usb_dev->class_req.pfn_class) {
            ret = p_usb_dev->class_req.pfn_class(p_usb_dev->class_req.p_arg, code);
        }
        break;
    /* 厂商请求 */
    case AM_USB_REQ_TYPE_VENDOR:
        if(p_usb_dev->vendor_req.pfn_vendor) {
            ret = p_usb_dev->vendor_req.pfn_vendor(p_usb_dev->vendor_req.p_arg, code);
        }
        break;
    default:
        break;
    }

    if((type & 0x80) && (p_data_info->length != 0)){
        am_usbd_recv(p_usb_dev, 0, p_data_info->p_buf, p_data_info->length);
    }

    if(ret == AM_USB_STATUS_SUCCESS){
        p_usb_dev->state = AM_USBD_CTRL_SETUP;
    }
}

/**
 * \brief 端点中断
 */
static void __usb_device_interrupt_endpoint (am_zlg126_usbd_dev_t *p_dev)
{
    int ep_index = 0;         /* 发生中断的端点 */
    int ep_int_type = 0;      /* 具体的端点中断类型 */
    int i = 0;
    amhw_zlg116_usbd_t   *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;

    ep_index = amhw_zlg116_usbd_ep_int_state_get(p_usb);     /* 获得发生中断的端点 */
    amhw_zlg116_usbd_ep_int_state_clear(p_usb, ep_index);    /* 清端点中断 */
    p_dev->int_ep_union.int_ep_flag = ep_index;

    /* 端点0中断 */
    if (p_dev->int_ep_union.int_ep_flag_field.ep0) {

        /* 获得端点0发生的具体中断类型 */
        ep_int_type = amhw_zlg116_usbd_ep0_int_state_get(p_usb);
        amhw_zlg116_usbd_ep0_int_state_clear(p_usb, ep_int_type);
        p_dev->ep_int_type_union.ep_int_type[0] = ep_int_type;

        /* 处理端点0中断 */
        if (p_dev->ep_int_type_union.ep_int_type_field[0].setup) {
            __usb_setup_handle(p_dev);
        }
        /* 输入无应答，那么把之前发送过的数据再发送一遍 */
        if (p_dev->ep_int_type_union.ep_int_type_field[0].in_nack) {
            __usb_in_handle(p_dev);
        }
        if (p_dev->ep_int_type_union.ep_int_type_field[0].out_ack){

        }
        if (p_dev->device.endpoint_info[0].pfn_callback != NULL) {
            (p_dev->device.endpoint_info[0].pfn_callback)(p_dev->device.endpoint_info[0].p_arg);
        }

        p_dev->ep_int_type_union.ep_int_type[0] = 0;
    }

    /* 端点x */
    for (i = 1; i < AM_USBD_MAX_EP_CNT; i++) {
        if (p_dev->device.endpoint_info[i].inuse == AM_TRUE) {
            if ((p_dev->int_ep_union.int_ep_flag >> i) & 1) {

                ep_int_type = amhw_zlg116_usbd_epx_int_state_get(p_usb, (zlg116_usb_epx_t)(i - 1));
                amhw_zlg116_usbd_epx_int_state_clear(p_usb, (zlg116_usb_epx_t)(i - 1), ep_int_type);
                p_dev->ep_int_type_union.ep_int_type[i] = ep_int_type;

                if (p_dev->device.endpoint_info[i].pfn_callback != NULL) {
                    (p_dev->device.endpoint_info[i].pfn_callback)(p_dev->device.endpoint_info[i].p_arg);
                }
                p_dev->ep_int_type_union.ep_int_type[i] = 0;
            }
        }
    }
    p_dev->ep_int_type_union.ep_int_type[0] = 0;
    p_dev->int_ep_union.int_ep_flag = 0;
}

/**
 * \brief USB中断服务函数
 */
static void __usbd_isr_function(void *p_device)
{
    am_zlg126_usbd_dev_t *p_dev =(am_zlg126_usbd_dev_t *) p_device;
    amhw_zlg116_usbd_t   *p_usb = (amhw_zlg116_usbd_t *)p_dev->p_info->usb_regbase;
    uint8_t int_status;

    if (NULL == p_dev)
        return;

    int_status = amhw_zlg116_usbd_int_state_get(p_usb);     /* 获得中断状态 */
    amhw_zlg116_usbd_int_state_clear(p_usb, int_status);    /* 清除中断状态 */

    /* 端点中断 */
    if (int_status & AMHW_ZLG116_USB_INT_STATE_EPINTF) {
        __usb_device_interrupt_endpoint(p_dev);
    }

    /* 总线复位0中断 */
    if (int_status & AMHW_ZLG116_USB_INT_STATE_RSTF) {
        am_usbd_init(&(p_dev->device));
    }

    /* 总线挂起 */
    if (int_status & AMHW_ZLG116_USB_INT_STATE_SUSPENDF) {
        amhw_zlg116_usbd_int_state_clear(p_usb, AMHW_ZLG116_USB_INT_STATE_SUSPENDF);
    }

    /* 总线唤醒 */
    if (int_status & AMHW_ZLG116_USB_INT_STATE_RESUMF) {
        amhw_zlg116_usbd_int_state_clear(p_usb, AMHW_ZLG116_USB_INT_STATE_RESUMF);
    }

    /* 检测到SOF */
    if (int_status & AMHW_ZLG116_USB_INT_STATE_SOFF) {

    }
}

/**
 * \brief （通过配置描述符）初始化端点信息（端点个数，输入输出属性，支持的最大包大小）
 *
 * \retval 成功返回该使用的端点个数，失败返回-1
 */
static am_err_t __init_ep_info (am_zlg126_usbd_dev_t *p_dev)
{
   uint8_t   ret     = 0;       /* 定义返回值 */
    uint8_t  *p_tmp  = NULL;    /* 数据缓存指针 */
    uint8_t   ep_num = 0;       /* 端点号 */
    uint8_t  tmp     = 0;       /* 临时变量*/
    uint8_t  offset  = 0;       /* 偏移量*/
    uint8_t  len     = 0;       /* 描述符总长度 */
    am_usb_descriptor_endpoint_t  *p_desc_ep   = NULL;
    /* 获取配置描述符 */
    p_tmp = __find_desc_by_wValue1(p_dev, (AM_USB_DESC_TYPE_CONFIGURE << 8) | 0);
    if (p_tmp == NULL)
        return AM_ERROR;
    /*配置描述符及其下属描述符的总长度*/
    len = (uint8_t)( ( *(p_tmp + 2) ) | ( ( *(p_tmp + 3)) >> 8 ) );
    /* 找端点描述符*/
    while (tmp < len) {
        offset  =  *p_tmp;    /* 该描述符的字节数,第一次为配置描述符*/
        p_tmp  +=   offset;   /* 做偏移 */
        tmp    +=   offset;
        /* 如果是端点描述符*/
        if ((*(p_tmp + 1) == AM_USB_DESC_TYPE_ENDPOINT) && (*p_tmp == AM_USB_DESC_LENGTH_ENDPOINT)) {
            p_desc_ep = (am_usb_descriptor_endpoint_t *)p_tmp;  // 端点描述符
            // 端点号
            ep_num = p_desc_ep->b_endpoint_address & AM_USB_DESC_ENDPOINT_ADDR_NUMBER_MASK;
            p_dev->device.endpoint_info[ep_num].inuse = 1;  // 表示端点被使用
            p_dev->device.endpoint_info[ep_num].ep_address = p_desc_ep->b_endpoint_address;
            p_dev->device.endpoint_info[ep_num].max_packet_size = (p_desc_ep->wmax_packet_size[1] << 8) |
                                                                   p_desc_ep->wmax_packet_size[0];
            p_dev->device.endpoint_info[ep_num].transfer_type = p_desc_ep->bm_attributes;
            p_dev->device.endpoint_info[ep_num].stalled = 0;
            ret++;
        }
    }
    return ret;
}

/**
 * \brief 初始化USB
 *
 * \param[in] p_dev     : 指向USB设备
 * \param[in] p_info    : 指向USB设备信息
 *
 * \return USB标准服务操作句柄。如果为 NULL，表明初始化失败。
 */
am_usbd_dev_t *am_zlg126_usbd_init (am_zlg126_usbd_dev_t           *p_dev,
                                    const am_zlg126_usbd_devinfo_t *p_info)
{
    int i = 0;

    if (NULL == p_dev  || NULL == p_info) {
        return NULL;
    }

    p_dev->p_info = p_info;

    p_dev->device.p_info = p_info->p_devinfo;

    p_dev->device.ctrl_handle = p_dev;
    p_dev->device.p_interface = &__g_usb_device_interface;

    for (i = 0; i < AM_USBD_MAX_EP_CNT; i++) {
        p_dev->device.endpoint_info[i].pfn_callback = NULL;
        p_dev->device.endpoint_info[i].p_arg = NULL;
    }

    /* 初始化厂商请求*/
    p_dev->device.vendor_req.pfn_vendor = NULL;
    p_dev->device.vendor_req.p_arg      = NULL;

    p_dev->device.class_req.pfn_class   = NULL;
    p_dev->device.class_req.p_arg       = NULL;

    p_dev->device.device_address = 0;
    p_dev->device.state          = AM_USBD_STATE_DEFAULT;

    p_dev->int_ep_union.int_ep_flag = 0;

    for (i = 0; i < AM_USBD_MAX_EP_CNT; i++)
        p_dev->ep_int_type_union.ep_int_type[i] = 0;

    p_dev->device.setup_data.bm_request_type    = 0;
    p_dev->device.setup_data.b_request          = 0;
    p_dev->device.setup_data.w_value            = 0;
    p_dev->device.setup_data.w_index            = 0;
    p_dev->device.setup_data.w_length           = 0;


    /**< \brief 设置端点0的默认配置 */
    p_dev->device.endpoint_info[0].stalled          = 0;
    p_dev->device.endpoint_info[0].ep_address       = 0;
    p_dev->device.endpoint_info[0].max_packet_size  = AM_USBD_MAX_EP_DATA_CNT;
    p_dev->device.endpoint_info[0].transfer_type    = AM_USB_ENDPOINT_CONTROL;
    p_dev->device.endpoint_info[0].inuse = 1;
    p_dev->device.endpoint_info[0].val_length       = 0;

    for (i = 1 ;i < AM_USBD_MAX_EP_CNT; i++) {
        p_dev->device.endpoint_info[i].stalled          = 0;
        p_dev->device.endpoint_info[i].ep_address       = 0;
        p_dev->device.endpoint_info[i].max_packet_size  = 0;
        p_dev->device.endpoint_info[i].transfer_type    = 0;
        p_dev->device.endpoint_info[i].inuse            = 0;
        p_dev->device.endpoint_info[i].val_length       = 0;
    }
    if (__init_ep_info(p_dev) == -1) {
#ifdef USB_DEBUG
        am_kprintf("fail to init endpoint\n");
#endif
    }

    p_dev->device.state = AM_USBD_CTRL_IDLE;

    am_usbd_ch9_std_request_init(&(p_dev->device));

    if (p_info->pfn_plfm_init) {
        p_info->pfn_plfm_init();
    }

    am_int_connect(p_info->inum, __usbd_isr_function, (void *)p_dev);
    am_int_enable(p_info->inum);
    am_usbd_init(&p_dev->device);

    return &(p_dev->device);
}

/**
 * \brief USB Device 去初始化
 *
 * \param[in] p_info : 指向USB设备信息
 */
void am_zlg126_usbd_deinit (const am_zlg126_usbd_devinfo_t *p_info)
{
    if (p_info != NULL && p_info->pfn_plfm_deinit != NULL) {
        p_info->pfn_plfm_deinit();
    }
}
