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
 * \brief  CAN标准接口实现
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-06-02  anu, first implementation.
 * - 1.01 19-12-18  zc , add filter table extern api
 * \endinternal
 */
 

#ifndef __AM_CAN_H
#define __AM_CAN_H

#ifdef __cplusplus
extern "C" {
#endif
    

#include "am_types.h"

/** 
 * \addtogroup am_if_can
 * \copydoc am_can.h
 * @{
 */

/**
 * \name CAN标志的定义
 * \anchor grp_am_can_flag
 * @{
 */

/** \brief flags 报文信息的标志
 * -----------------------------------------------------------
 * | 15:10                             | 9:8 controller type |
 * -----------------------------------------------------------
 * -----------------------------------------------------------
 * | 7:reseverd |6:reseverd | 5:SEF | 4:SDF | 3 : 0 sendtype |
 * -----------------------------------------------------------
 * bit7:reseverd 保留
 * bit6:reseverd 保留
 * bit5:SEF 帧格式 (1:extended扩展帧 0:std 标准帧)
 * bit4:SDF 帧类型 (1:remote远程帧   0:data数据帧)
 * bit0~3:发送类型  (sendtype:见宏定义)
 */
typedef uint16_t am_can_flag_t;            /**< \brief CAN标志             */
#define AM_CAN_NORMAL_SEND_FLAG     0X00   /**< \brief 正常发送           */
#define AM_CAN_ONCE_SEND_FLAG       0X01   /**< \brief 单次发送           */
#define AM_CAN_SELF_SEND_FLAG       0X02   /**< \brief 自发自收           */
#define AM_CAN_SELFONCE_SEND_FLAG   0X03   /**< \brief 单次自发自收 */
#define AM_CAN_REMOTE_FLAG		    0X10   /**< \brief 远程帧标志位 */
#define AM_CAN_EXTERN_FLAG		    0X20   /**< \brief 扩展帧标志位 */
#define AM_CAN_FD_CTRL_FLAG         0x0100 /**< \brief CANFD标志位   */

/** @} */

/** \brief CAN 接收发送 信息  */
typedef struct am_can_message {
	uint32_t	     id;                    /**< \brief id 帧ID              */
    am_can_flag_t    flags;                 /**< \brief flags      标志位     	 */
	uint16_t	     msglen;                /**< \brief msglen     报文长度  	 */
	uint8_t		     msgdata[8];            /**< \brief msgdata    报文缓冲区  */
} am_can_message_t;


/** \brief 接收信息  */
typedef struct am_canfd_message {
	am_can_message_t can_msg;               /**< \brief CAN信息                                   */
    uint8_t msgdata[64-8];                  /**< \brief CANFD 数据长度为64B	*/
} am_canfd_message_t;


/**
 * \name CAN错误类型
 * \anchor grp_am_can_err_t
 * @{
 */

typedef uint32_t am_can_err_t;                 /**< \brief 错误类型   */
#define AM_CAN_NOERROR                 	0x00   /**< \brief 无错误   */
#define AM_CAN_NOTINITIALIZED          	0x01   /**< \brief 未初始化   */
#define AM_CAN_ILLEGAL_CHANNEL_NO      	0x02   /**< \brief 非法通道号   */
#define AM_CAN_ILLEGAL_CONFIG          	0x03   /**< \brief 非法配置   */
#define AM_CAN_ILLEGAL_DATA_LENGTH     	0x04   /**< \brief 非法数据长度   */
#define AM_CAN_ILLEGAL_MASK_VALUE      	0x05   /**< \brief 非法屏蔽码   */
#define AM_CAN_NO_AVAIL_CHANNELS       	0x06   /**< \brief 通道不可用   */
#define AM_CAN_INVALID_PARAMETER       	0x07   /**< \brief 无效参数  */
#define AM_CAN_ILLEGAL_OFFSET          	0x08   /**< \brief 无效的偏置   */
#define AM_CAN_CANNOT_SET_ERRINT       	0x09   /**< \brief 未使能错误中断   */
#define AM_CAN_CANNOT_SET_BOFFINT      	0x10   /**< \brief 未使能总线关闭中断   */
#define AM_CAN_CANNOT_SET_WAKEUPINT    	0x11   /**< \brief 未使能唤醒中断   */
#define AM_CAN_INCOMPATIBLE_TYPE       	0x12   /**< \brief 类型不兼容   */
#define AM_CAN_BAUDRATE_ERROR          	0x13   /**< \brief 波特率错误   */

/** @} */

/**
 * \name CAN总线错误类型 
 * \anchor grp_am_can_bus_err
 * @{
 */
 
typedef uint32_t am_can_bus_err_t;              /**< \brief CAN总线错误类型    */
#define AM_CAN_BUS_ERR_NONE    			0x00	/**< \brief 无错误   */
#define AM_CAN_BUS_ERR_BIT     			0x01	/**< \brief 位错误   */
#define AM_CAN_BUS_ERR_ACK     			0x02	/**< \brief 应答错误 */
#define AM_CAN_BUS_ERR_CRC     			0x04	/**< \brief CRC错误  */
#define AM_CAN_BUS_ERR_FORM    			0x08	/**< \brief 格式错误 */
#define AM_CAN_BUS_ERR_STUFF  	 		0x10	/**< \brief 填充错误 */
#define AM_CAN_BUS_ERR_UNKNOWN 			0x20	/**< \brief 未知错误 */

/** @} */

/**
 * \name CAN中断类型
 * \anchor grp_am_can_int_type
 * @{
 */

typedef uint32_t am_can_int_type_t;                /**< \brief 中断类型             */
#define AM_CAN_INT_NONE     			0x00       /**< \brief 无类型                  */
#define AM_CAN_INT_ERROR    			0x01       /**< \brief 错误中断             */
#define AM_CAN_INT_BUS_OFF  			0x02       /**< \brief 总线关闭中断   */
#define AM_CAN_INT_WAKE_UP  			0x04       /**< \brief 唤醒中断             */
#define AM_CAN_INT_TX       			0x08       /**< \brief 发送中断             */
#define AM_CAN_INT_RX       			0x10       /**< \brief 接收中断             */
#define AM_CAN_INT_DATAOVER             0x20       /**< \brief 总线超载中断   */
#define AM_CAN_INT_WARN    			    0x40       /**< \brief 警告中断             */
#define AM_CAN_INT_ERROR_PASSIVE        0x80       /**< \brief 错误被动中断   */
#define AM_CAN_INT_ALL      			0xffffffff /**< \brief 所有中断             */

/** @} */

/**
 * \name CAN模式类型 
 * \anchor grp_am_can_mode_type
 * @{
 */
 
typedef uint8_t am_can_mode_type_t;             /**< \brief CAN模式类型  */
#define AM_CAN_MODE_NROMAL				0x00	/**< \brief 正常工作模式 */
#define AM_CAN_MODE_LISTEN_ONLY		    0x01	/**< \brief 只听工作模式 */


/**
 * \name CAN帧类型
 * \anchor grp_am_can_frame_type
 * @{
 */
typedef uint8_t am_can_frame_type_t;             /**< \brief CAN帧类型  */
#define AM_CAN_FRAME_TYPE_STD            0x01    /**< \brief 标准帧  */
#define AM_CAN_FRAME_TYPE_EXT            0x02    /**< \brief 扩展帧  */

/**
 * \name CAN帧格式
 * \anchor grp——am_can_frame_format_t
 * @{
 */

typedef uint8_t am_can_frame_format_t;             /**< \brief CAN帧类型  */
#define AM_CAN_FRAME_FORMAT_NOCARE       0x00    /**< \brief 数据帧和远程帧都可接收  */
#define AM_CAN_FRAME_FORMAT_DATA         0x01    /**< \brief 数据帧  */
#define AM_CAN_FRAME_FORMAT_REMOTE       0x02    /**< \brief 远程帧  */




/** @} */

/** \brief 错误计数 */
typedef struct am_can_err_cnt {
	uint8_t rx_error_cnt; 	                    /**< \brief 接收错误计数器 */
	uint8_t tx_error_cnt; 	                    /**< \brief 发送错误计数器 */
} am_can_err_cnt_t;

/** \brief 波特率参数 */
typedef struct am_can_bps_param {
	uint8_t   	tesg1;  		                /**< \brief tseg1 相位段1    */
	uint8_t   	tesg2;  		                /**< \brief tseg2 相位段2    */   
	uint8_t   	sjw;    		                /**< \brief sjw 同步跳转宽度  */
	uint8_t     smp;                            /**< \brief smp 采样模式            */
	uint16_t    brp;                            /**< \brief brp 分频值                 */
} am_can_bps_param_t;

/** \brief 滤波表配置 */
typedef struct am_can_filter {
    am_can_frame_type_t   ide;                     /**< \brief 滤波器帧类型   */
    am_can_frame_format_t rtr;                     /**< \brief 滤波器帧格式   */

    uint32_t id[4];                                /**< \brief ID验收码    */
    uint32_t mask[4];                              /**< \brief ID掩码    */

} am_can_filter_t;

/**
 * \brief CAN reconfigure information .
 */
typedef struct am_can_cfg_info {
    am_can_mode_type_t        can_mode;         /**< \brief CAN 模式    */
} am_can_cfg_info_t;

/**
 * \brief CAN drive functions.
 */
struct am_can_drv_funcs {

	/** \brief CAN启动 */
	am_can_err_t (*pfn_can_start)(void *p_drv);
    
    /** \brief CAN复位（停止） */
	am_can_err_t (*pfn_can_reset)(void *p_drv);
    
    /** \brief CAN休眠  */
	am_can_err_t (*pfn_can_sleep)(void *p_drv);

    /** \brief CAN唤醒 */
	am_can_err_t (*pfn_can_wakeup)(void *p_drv);

    /** \brief 中断使能 */
	am_can_err_t (*pfn_can_int_enable)(void *p_drv, am_can_int_type_t int_mask);

    /** \brief 中断禁能*/
	am_can_err_t (*pfn_can_int_disable)(void *p_drv, am_can_int_type_t int_mask);

	/** \brief 模式设置 */
	am_can_err_t (*pfn_can_mode_set)(void *p_drv, am_can_mode_type_t mode);

    /** \brief 波特率设置 */
	am_can_err_t (*pfn_can_baudrate_set)(void               *p_drv,
										 am_can_bps_param_t *p_can_baudrate);

    /** \brief 波特率获取 */
    am_can_err_t (*pfn_can_baudrate_get)(void               *p_drv,
                                         am_can_bps_param_t *p_can_baudrate);
   
    /** \brief 获取错误计数 */
	am_can_err_t (*pfn_can_err_cnt_get)(void             *p_drv,
										am_can_err_cnt_t *p_can_err_reg);
    
    /** \brief 清除错误计数 */
	am_can_err_t (*pfn_can_err_cnt_clr)(void *p_drv);

    /** \brief 发送信息   */
	am_can_err_t (*pfn_can_msg_send)(void *p_drv, am_can_message_t *p_txmsg);

    /** \brief 接收信息   */
	am_can_err_t (*pfn_can_msg_recv)(void *p_drv, am_can_message_t *p_rxmsg);

    /** \brief 停止发送信息   */
	am_can_err_t (*pfn_can_stop_msg_snd)(void *p_drv);

    /** \brief 设置滤波函数 */
	am_can_err_t (*pfn_can_filter_tab_set)(void    *p_drv,
	                                       uint8_t *p_filterbuff,
	                                       size_t   lenth);
    /** \brief 获取滤波函数*/
	am_can_err_t (*pfn_can_filter_tab_get)(void    *p_drv,
	                                       uint8_t *p_filterbuff,
	                                       size_t  *p_lenth);

    /** \brief CAN 状态 */
	am_can_err_t (*pfn_can_status_get)(void              *p_drv,
                                       am_can_int_type_t *p_int_type,
                                       am_can_bus_err_t  *p_bus_err);

    /** \brief 中断连接 */
    am_can_err_t (*pfn_can_connect)(void 		 *p_drv,
    								am_pfnvoid_t  pfn_isr,
    								void         *p_arg);

    /** \brief 删除中断连接 */
    am_can_err_t (*pfn_can_disconnect)(void         *p_drv,
                                       am_pfnvoid_t  pfn_isr,
                                       void         *p_arg);
		
    /** \brief 中断回调函数的连接 */
    am_can_err_t (*pfn_can_intcb_connect)(void              *p_drv,
    									  am_can_int_type_t  inttype,
    									  am_pfnvoid_t       pfn_callback,
    									  void              *p_arg);
    /** \brief 删除中断回调函数的连接 */
    am_can_err_t (*pfn_can_intcb_disconnect)(void              *p_drv,
    										 am_can_int_type_t  inttype);

    /** \brief 设置滤波函数(扩展) */
    am_can_err_t (*pfn_can_filter_tab_ext_set)(void            *p_drv,
                                               am_can_filter_t *p_filterbuff,
                                               size_t           lenth);

    /** \brief 获取滤波函数(扩展)*/
    am_can_err_t (*pfn_can_filter_tab_ext_get)(void             *p_drv,
                                               am_can_filter_t  *p_filterbuff,
                                               size_t           *p_lenth);
};

/**
 * \brief CAN 设备.
 */
typedef struct am_can_serv {

    struct am_can_drv_funcs   *p_drv_funcs;    /**< \brief  驱动函数 */
    void                      *p_drv;          /**< \brief  驱动设备 */

} am_can_serv_t;

/** \brief handle 标准服务 */
typedef am_can_serv_t *am_can_handle_t;

/**
 * \brief CAN 启动.
 *
 * \param[in] handle : CAN 标准服务 handle.
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_start (am_can_handle_t handle)
{
    return handle->p_drv_funcs->pfn_can_start(handle->p_drv);
}

/**
 * \brief CAN 复位.
 *
 * \param[in] handle : CAN 标准服务 handle.
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_reset (am_can_handle_t handle)
{
    return handle->p_drv_funcs->pfn_can_reset(handle->p_drv);
}

/**
 * \brief CAN 睡眠.
 *
 * \param[in] handle : CAN 标准服务 handle.
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_sleep (am_can_handle_t handle)
{
    return handle->p_drv_funcs->pfn_can_sleep(handle->p_drv);
}

/**
 * \brief CAN 唤醒.
 *
 * \param[in] handle : CAN 标准服务 handle.
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_wakeup (am_can_handle_t handle)
{
    return handle->p_drv_funcs->pfn_can_wakeup(handle->p_drv);
}

/**
 * \brief CAN 中断使能.
 *
 * \param[in] handle : CAN 标准服务 handle.
 * \param[in] mask   : 中断类型
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_int_enable (am_can_handle_t handle, am_can_int_type_t mask)
{
    return handle->p_drv_funcs->pfn_can_int_enable(handle->p_drv, mask);
}

/**
 * \brief CAN 中断失能.
 *
 * \param[in] handle : CAN 标准服务 handle.
 * \param[in] mask   : 中断类型
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_int_disable (am_can_handle_t handle, am_can_int_type_t mask)
{
    return handle->p_drv_funcs->pfn_can_int_disable(handle->p_drv, mask);
}

/**
 * \brief CAN 设置模式.
 *
 * \param[in] handle : CAN 标准服务 handle.
 * \param[in] mode   : 工作模式
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_mode_set (am_can_handle_t    handle,
                              am_can_mode_type_t mode)
{
    return handle->p_drv_funcs->pfn_can_mode_set(handle->p_drv, mode);
}

/**
 * \brief CAN 设置波特率.
 *
 * \param[in] handle           : CAN 标准服务 handle.
 * \param[in] p_can_baudrate   : 波特率
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_baudrate_set (am_can_handle_t          handle,
                                  struct am_can_bps_param *p_can_baudrate)
{
    return handle->p_drv_funcs->pfn_can_baudrate_set(handle->p_drv, 
                                                     p_can_baudrate);
} 

/**
 * \brief CAN 获取波特率.
 *
 * \param[in] handle           : CAN 标准服务 handle.
 * \param[out] p_can_baudrate   : 波特率
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_baudrate_get (am_can_handle_t          handle,
                                  struct am_can_bps_param *p_can_baudrate)
{
    return handle->p_drv_funcs->pfn_can_baudrate_get(handle->p_drv,
                                                     p_can_baudrate);
}

/**
 * \brief CAN 获取错误计数
 *
 * \param[in] handle         : CAN 标准服务 handle.
 * \param[out] p_can_err_cnt : 错误计数
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_err_cnt_get (am_can_handle_t        handle,
                                 struct am_can_err_cnt *p_can_err_cnt)
{
    return handle->p_drv_funcs->pfn_can_err_cnt_get(handle->p_drv,
                                                    p_can_err_cnt);
} 

/**
 * \brief CAN 清除错误计数
 *
 * \param[in] handle : CAN 标准服务 handle.
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_err_cnt_clr (am_can_handle_t handle)
{
    return handle->p_drv_funcs->pfn_can_err_cnt_clr(handle->p_drv);
} 

/**
 * \brief CAN 发送信息.
 *
 * \param[in] handle  : CAN 标准服务 handle.
 * \param[in] p_txmsg : 发送的消息
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_msg_send (am_can_handle_t handle, am_can_message_t *p_txmsg)
{
    return handle->p_drv_funcs->pfn_can_msg_send(handle->p_drv, p_txmsg);
}

/**
 * \brief CAN 接收消息
 *
 * \param[in] handle   : CAN 标准服务 handle.
 * \param[out] p_rxmsg : 接收的消息
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_msg_recv(am_can_handle_t handle, am_can_message_t *p_rxmsg)
{
    return handle->p_drv_funcs->pfn_can_msg_recv(handle->p_drv, p_rxmsg);
}

/**
 * \brief CAN 停止发送
 *
 * \param[in] handle   : CAN 标准服务 handle.
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_msg_stop_send(am_can_handle_t handle)
{
    return handle->p_drv_funcs->pfn_can_stop_msg_snd(handle->p_drv);
}

/**
 * \brief CAN 设置滤波
 *
 * \param[in] handle  : CAN 标准服务 handle.
 * \param[in] p_filterbuff : 滤波表首地址
 * \param[in] lenth        : 滤波表长度
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_filter_tab_set (am_can_handle_t  handle,
                                    uint8_t         *p_filterbuff,
                                    size_t           lenth)
{
    return handle->p_drv_funcs->pfn_can_filter_tab_set(handle->p_drv,
                                                       p_filterbuff,
                                                       lenth);
}

/**
 * \brief CAN 设置滤波(扩展)
 *
 * \param[in] handle  : CAN 标准服务 handle.
 * \param[in] p_filterbuff : 滤波表 详见 am_can_filter_t
 * \param[in] lenth        : 传输的am_can_filter_t的个数
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_filter_tab_ext_set (am_can_handle_t  handle,
                                        am_can_filter_t *p_filterbuff,
                                        size_t           lenth)
{
    return handle->p_drv_funcs->pfn_can_filter_tab_ext_set(handle->p_drv,
                                                           p_filterbuff,
                                                           lenth);
}

/**
 * \brief CAN 获取滤波表设置
 *
 * \param[in]  handle          : CAN 标准服务 handle.
 * \param[out] p_filterbuff    : 滤波表首地址
 * \param[out] p_lenth         : 滤波表长度
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_filter_tab_get (am_can_handle_t  handle,
                                    uint8_t         *p_filterbuff,
                                    size_t          *p_lenth)
{
    return handle->p_drv_funcs->pfn_can_filter_tab_get(handle->p_drv,
                                                       p_filterbuff,
                                                       p_lenth);
}

/**
 * \brief CAN 获取滤波表设置(扩展)
 *
 * \param[in]  handle          : CAN 标准服务 handle.
 * \param[out] p_filterbuff    : 滤波表 详见 am_can_filter_t
 * \param[out] p_lenth         : 设置的am_can_filter_t个数
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_filter_tab_ext_get (am_can_handle_t  handle,
                                        am_can_filter_t *p_filterbuff,
                                         size_t         *p_lenth)
{
    return handle->p_drv_funcs->pfn_can_filter_tab_ext_get(handle->p_drv,
                                                           p_filterbuff,
                                                           p_lenth);
}

/**
 * \brief CAN 获取状态
 *
 * \param[in] handle      : CAN 标准服务 handle.
 * \param[out] p_int_type : 中断类型.
 * \param[out] p_bus_err  : 总线错误类型.
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_status_get (am_can_handle_t    handle,
                                am_can_int_type_t *p_int_type,
                                am_can_bus_err_t  *p_bus_err)
{
    return handle->p_drv_funcs->pfn_can_status_get(handle->p_drv,
                                                   p_int_type,
                                                   p_bus_err);
}

/**
 * \brief CAN 中断连接
 *
 * \param[in] handle  : CAN 标准服务 handle.
 * \param[in] pfn_isr : CAN 中断服务函数（NULL时则使用内部中断服务）.
 * \param[in] p_arg   : CAN 中断服务函数参数.
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_connect (am_can_handle_t  handle,
							 am_pfnvoid_t     pfn_isr,
							 void            *p_arg)
{
    return handle->p_drv_funcs->pfn_can_connect(handle->p_drv, pfn_isr, p_arg);
}

/**
 * \brief CAN 删除中断连接
 *
 * \param[in] handle  : CAN 标准服务 handle.
 * \param[in] pfn_isr : CAN 中断服务函数.
 * \param[in] p_arg   : CAN 中断服务函数参数.
 *
 * \retval  AM_CAN_NOERROR            :  成功.
 * \retval  -AM_CAN_INVALID_PARAMETER : 参数错误
 */
am_static_inline
am_can_err_t am_can_disconnect (am_can_handle_t  handle,
                                am_pfnvoid_t     pfn_isr,
                                void            *p_arg)
{
    return handle->p_drv_funcs->pfn_can_disconnect(handle->p_drv,
                                                   pfn_isr,
                                                   p_arg);
}

/**
 * \brief CAN 注册中断回调函数
 *
 * \param[in] handle			 : CAN 标准服务 handle.
 * \param[in] inttype			 : 中断类型
 * \param[in] pfn_callback       : 中断回调函数
 * \param[in] p_arg 			 : 回调函数参数
 *
 * \retval  AM_CAN_NOERROR              : 成功.
 * \retval  -AM_CAN_INVALID_PARAMETER   : 参数错误
 * \retval  -AM_CAN_ILLEGAL_MASK_VALUE  : 中断类型不合法
 */
am_static_inline
am_can_err_t am_can_intcb_connect (am_can_handle_t    handle,
						           am_can_int_type_t  inttype,
                                   am_pfnvoid_t       pfn_callback,
                                   void              *p_arg)
{
	return handle->p_drv_funcs->pfn_can_intcb_connect(handle->p_drv, 
	                                                  inttype,
	                                                  pfn_callback, p_arg);
}

/**
 * \brief CAN 解除中断回调函数的注册
 *
 * \param[in] handle			 : CAN 标准服务 handle.
 * \param[in] inttype			 : 中断类型
 *
 * \retval  AM_CAN_NOERROR                : 成功.
 * \retval  -AM_CAN_INVALID_PARAMETER     : 参数错误
 * \retval  -AM_CAN_ILLEGAL_MASK_VALUE    : 中断类型不合法
 */
am_static_inline
am_can_err_t am_can_intcb_disconnect (am_can_handle_t    handle,
						              am_can_int_type_t  inttype)
{
	return handle->p_drv_funcs->pfn_can_intcb_disconnect(handle->p_drv,
	                                                     inttype);
}

/** 
 * @} am_if_can
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_CAN_H */

/*end of file */



