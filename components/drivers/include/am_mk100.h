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
 * \brief MK100底层驱动程序
 * 
 * \internal
 * \par Modification history
 * - 1.00 19-12-20  mxy, first implementation.
 * \endinternal
 */
#ifndef __AM_MK100_H
#define __AM_MK100_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_uart_rngbuf.h"
#include "am_uart.h"

/**
 * \addtogroup am_mk100
 * \copydoc am_mk100.h
 * @{
 */
    
#define AM_MK100_DEFAULT_BAUDRATE                   9600          /**< \brief  默认波特率是9600 */
#define AM_MK100_FRAME_LEN                          5             /**< \brief  帧长度是5个字节 */
#define AM_MK100_DIR_CW                             0x00          /**< \brief  顺时针方向 */
#define AM_MK100_DIR_CCW                            0x01          /**< \brief  逆时针方向 */

#define AM_MK100_DIRECTION_UP                       0x00          /**< \brief  上 */
#define AM_MK100_DIRECTION_DOWN                     0x01          /**< \brief  下 */ 
#define AM_MK100_DIRECTION_LEFT                     0x02          /**< \brief  左 */
#define AM_MK100_DIRECTION_RIGHT                    0x03          /**< \brief  右 */

#define AM_MK100_BAUDRATE_TEST                      0x00          /**< \brief  设置测试波特率 */
#define AM_MK100_BAUDRATE_2400                      0x01          /**< \brief  设置2400波特率 */
#define AM_MK100_BAUDRATE_4800                      0x02          /**< \brief  设置4800波特率 */
#define AM_MK100_BAUDRATE_9600                      0x03          /**< \brief  设置9600波特率 */
#define AM_MK100_BAUDRATE_56000                     0x04          /**< \brief  设置56000波特率 */
#define AM_MK100_BAUDRATE_115200                    0x05          /**< \brief  设置115200波特率 */

#define AM_MK100_SET_ROTATION_FUNC                  0x00          /**< \brief  设置旋转功能 */
#define AM_MK100_SET_INFRARED_DETECT                0x10          /**< \brief  设置旋转+拿起放下检测（红外检测）*/
#define AM_MK100_SET_STATUS_DETECT                  0x20          /**< \brief  设置旋转+拿起放下检测（状态检测） */
#define AM_MK100_SET_LEFT_RIGNH_DETECT              0x33          /**< \brief  使能左右方向推动检测 */
#define AM_MK100_SET_UP_DOWN_DETECT                 0x3C          /**< \brief  使能上下方向推动检测 */
#define AM_MK100_SET_UP_DOWN_LEFT_RIGNH_DETECT      0x3F          /**< \brief  使能上下左右推动检测 */

#define AM_MK100_GET_ANGLE                          0x2F          /**< \brief  用于判断自动上报模式的数据，表示是角度值 */
#define AM_MK100_GET_KNOB_POS                       0x2E          /**< \brief  用于判断自动上报模式的数据，表示是磁旋钮位置 */
/**
 * \brief mk100 设备信息
 */
typedef struct am_mk100_devinfo {
    /** \brief 用于串口接收的缓冲区，建议大小在32以上     */
    uint8_t     *p_uart_rxbuf;

    /** \brief 用于串口发送的缓冲区，建议大小在32以上     */
    uint8_t     *p_uart_txbuf;

    /** \brief 用于串口接收的缓冲区大小 */
    uint16_t     rxbuf_size;

    /** \brief 用于串口发送的缓冲区大小 */
    uint16_t     txbuf_size;

} am_mk100_devinfo_t;

/**
 * \brief mk100设备实例
 */
typedef struct am_mk100_dev {
    /** \brief UART 环形缓冲区实例句柄                    */
    am_uart_rngbuf_handle_t uart_rngbuf_handle;
    /** \brief UART实例句柄                    */
    am_uart_handle_t uart_handle;
    /** \brief 错误接收                    */
    uint8_t error_frame[AM_MK100_FRAME_LEN];
    /** \brief 用于保存设备信息指针 */
    const am_mk100_devinfo_t *p_devinfo;
} am_mk100_dev_t;

/**
 * \brief mk100设备句柄
 */
typedef struct am_mk100_dev *am_mk100_handle_t;
/**
 * \brief mk100初始化
 *
 * \param[in] p_dev              : mk100设备指针
 * \param[in] p_devinfo          : mk100设备信息指针
 * \param[in] uart_handle        : uart句柄
 *
 * \return    am_mk100_handle_t  ：mk100句柄
 */
am_mk100_handle_t am_mk100_init (am_mk100_dev_t           *p_dev,
                                 const am_mk100_devinfo_t *p_devinfo,
                                 am_uart_handle_t          uart_handle);
/**
 * \brief 获取角度值
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] gear                    : 等级
 * \return    -AM_EINVAL              : 错误
 *            >=0                     : 角度值
 */
int am_mk100_get_angle(am_mk100_handle_t handle, uint8_t gear);
/**
 * \brief 获取磁旋钮位置
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            >=0                     : 磁旋钮位置
 */             
int am_mk100_get_knob(am_mk100_handle_t handle);
/**
 * \brief 获取当前磁场强度阈值，保存在upper_limit和lower_limit指向的内存中
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] upper_limit             : 上限
 * \param[in] lower_limit             : 下限
 * \return    -AM_EINVAL              : 错误
 *            AM_OK                   : 成功
 */                                 
int am_mk100_get_strength_threshold(am_mk100_handle_t handle, uint8_t* upper_limit, uint8_t* lower_limit);
/**
 * \brief 获取当前旋转方向
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            >=0                     : 当前旋转方向
 */
int am_mk100_get_rotation_direction(am_mk100_handle_t handle);
/**
 * \brief 获取当前磁场强度阈值状态，保存在upper_limit_status和lower_limit_status指向的内存中
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] upper_limit_status      : 0x01 表示当前磁强度已达到上限， 0x00 表示当前磁强度未达到上限
 * \param[in] lower_limit_status      : 0x01 表示当前磁强度已达到下限， 0x00 表示当前磁强度未达到下限
 * \return    -AM_EINVAL              : 错误
 *            AM_OK                   : 成功
 */
int am_mk100_get_strength_threshold_status(am_mk100_handle_t handle, am_bool_t* upper_limit_status, am_bool_t* lower_limit_status);
/**
 * \brief 获取方向传感器数据
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] direction               : 方向，上下左右分别为AM_MK100_DIRECTION_UP、AM_MK100_DIRECTION_DOWN、AM_MK100_DIRECTION_LEFT、AM_MK100_DIRECTION_RIGHT    
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 该方向的数据
 */
int am_mk100_get_direction_data(am_mk100_handle_t handle, uint8_t direction);
/**
 * \brief 获取红外传感器数据
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 红外传感器数据
 */
int am_mk100_get_infrared_data(am_mk100_handle_t handle);
/**
 * \brief 设置当前角度值
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] angle                   : 角度值，0~360
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的角度值
 */
int am_mk100_set_angle(am_mk100_handle_t handle, uint16_t angle);
/**
 * \brief 设置当前磁场强度阈值上限
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] limit                   : 阈值上限，最大值为0x07
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的阈值上限
 */
int am_mk100_set_strength_upper_limit(am_mk100_handle_t handle, uint8_t limit);
/**
 * \brief 设置当前磁场强度阈值下限
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] limit                   : 阈值下限，最大值为0x07
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的阈值下限
 */
int am_mk100_set_strength_lower_limit(am_mk100_handle_t handle, uint8_t limit);
/**
 * \brief 设置当前旋转方向
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] direction               : 方向，AM_MK100_DIR_CW: 顺时针、AM_MK100_DIR_CCW :逆时针
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的方向
 */
int am_mk100_set_rotation_direction(am_mk100_handle_t handle, uint8_t direction);
/**
 * \brief 方向传感器初始值校准
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            AM_OK                   : 成功
 */
int am_mk100_direction_cal(am_mk100_handle_t handle);
/**
 * \brief 设置方向传感器参数
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] threshold               : 方向传感器参数
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的方向传感器参数
 */
int am_mk100_set_direction_parameter(am_mk100_handle_t handle, uint8_t direction, uint16_t threshold);
/**
 * \brief 设置红外传感器参数
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] data                    : 红外传感器参数
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的红外传感器参数
 */
int am_mk100_set_infrared_parameter(am_mk100_handle_t handle, uint16_t data);
/**
 * \brief 设置系统波特率
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] baudrate_setting        : 波特率选择，只能设置AM_MK100_BAUDRATE_***这些宏定义
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的波特率
 */
int am_mk100_set_baudrate(am_mk100_handle_t handle, uint8_t baudrate_setting);
/**
 * \brief 设置系统功能
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] mode                    : 功能选择，只有以下有效的输入：
 *                                      AM_MK100_SET_ROTATION_FUNC、
 *                                      AM_MK100_SET_INFRARED_DETECT、
 *                                      AM_MK100_SET_STATUS_DETECT、
 *                                      AM_MK100_SET_LEFT_RIGNH_DETECT、
 *                                      AM_MK100_SET_UP_DOWN_DETECT、
 *                                      AM_MK100_SET_UP_DOWN_LEFT_RIGNH_DETECT
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置
 */
int am_mk100_set_system_func(am_mk100_handle_t handle, uint8_t mode);
/**
 * \brief 设置系统为查询模式
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前模式
 */
int am_mk100_set_query_mode(am_mk100_handle_t handle);
/**
 * \brief 设置系统为主动上报模式, 数据有改变时输出，设置完成后需要用am_mk100_get_report来获取数据，不能设置延时，否则会数据读取错误
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前模式
 */
int am_mk100_set_report_change_mode(am_mk100_handle_t handle);
/**
 * \brief 设置系统为主动上报模式, 固定频率输出，设置完成后需要用am_mk100_get_report来获取数据，不能设置延时，否则会数据读取错误
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] time                    : 固定间隔时间,单位为 ms，范围为 50 - 2000
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前模式
 */
int am_mk100_set_report_periodically_mode(am_mk100_handle_t handle, uint16_t time);
/**
 * \brief 设置旋钮灵敏度
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] sensitivity             : 灵敏度等级选项，值的范围为 2-50
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前灵敏度等级
 */
int am_mk100_set_knob_sensitivity(am_mk100_handle_t handle, uint8_t sensitivity);
/**
 * \brief 设置延时窗口宽度
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] delay_window            : 延时窗口宽度，单位为采样时间 10ms，值的范围为 10-100。例如配置的值为 50，则过滤窗口总时间为 500ms
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前延时窗口宽度
 */
int am_mk100_set_delay_window(am_mk100_handle_t handle, uint8_t delay_window);
/**
 * \brief 设置进入低功耗时间
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] enter_low_power_time    : 进入低功耗时间，单位为秒, 值的范围为 0-255
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设定的时间
 */
int am_mk100_set_enter_low_power_time(am_mk100_handle_t handle, uint8_t enter_low_power_time);
/**
 * \brief 获取自动上报模式时MK100返回的帧
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] p_type                  : 指向存放返回数据的类型，分别为角度数据或磁旋钮位置数据
 * \param[in] p_data                  : 指向存放数据
 * \return    -AM_EINVAL              : 错误
 *            AM_OK                   : 成功
 */
int am_mk100_get_report(am_mk100_handle_t handle, uint8_t* p_type, uint8_t *p_data);
/**
 * \brief 打印错误接收
 *
 * \param[in] handle              : mk100句柄
 * \return    无 
 */
void am_mk100_display_error_recevice(am_mk100_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_MK100_H */

/* end of file */
