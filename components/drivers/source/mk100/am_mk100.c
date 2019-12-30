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
#include "am_mk100.h"
#include <string.h>
#include "am_vdebug.h"

/*******************************************************************************
    宏定义
*******************************************************************************/
#define __MK100_FRAME_HEADER_INDEX  0            //帧头是每帧的第一个字节
#define __MK100_FRAME_FLAG_INDEX    1            //帧标识是每帧的第二个字节
#define __MK100_FRAME_DATE_1_INDEX  2            //帧数据1是每帧的第三个字节
#define __MK100_FRAME_DATE_2_INDEX  3            //帧数据2是每帧的第四个字节
#define __MK100_FRAME_CRC_INDEX     4            //帧校验是每帧的第五个字节
#define __MK100_FRAME_CRC_LEN       3            //帧校验为帧标识、帧数据1、帧数据2一共3个字节的数据的CRC-8校验值

#define __MK100_NO_PARAMETER        0x0000       //表示_mk100_ctrl不需要参数

#define __MK100_SYS_ERROR_FLAG   0x31            //表示系统错误返回帧
#define __MK100_FRAME_ERROR_FLAG 0x32            //表示接收帧错误

#define __MK100_SET_QUERY_MODE 0x8000             //设置查询模式的帧的数据1+数据2
#define __MK100_SET_REPORT_CHANGE_MODE 0x4000    //设置主动上报变化模式的帧的数据1+数据2
/*******************************************************************************
    类型定义
*******************************************************************************/
typedef enum _mk100_ctrl_type {
    _mk100_ctrl_type_get_angle_gear = 0,
    _mk100_ctrl_type_get_knob_pos,
    _mk100_ctrl_type_get_strength_threshold,
    _mk100_ctrl_type_get_rotation_direction,
    _mk100_ctrl_type_get_strength_threshold_status,
    _mk100_ctrl_type_get_direction_data,
    _mk100_ctrl_type_get_infrared_data,
    _mk100_ctrl_type_set_angle,
    _mk100_ctrl_type_set_strength_upper_limit,
    _mk100_ctrl_type_set_strength_lower_limit,
    _mk100_ctrl_type_set_rotation_direction,
    _mk100_ctrl_type_direction_cal,
    _mk100_ctrl_type_set_direction_parameter,
    _mk100_ctrl_type_set_infrared_parameter,
    _mk100_ctrl_type_set_baudrate,
    _mk100_ctrl_type_set_system_func,
    _mk100_ctrl_type_set_system_output_mode,
    _mk100_ctrl_type_set_knob_sensitivity,
    _mk100_ctrl_type_set_delay_window,
    _mk100_ctrl_type_set_enter_low_power_time,
    _mk100_ctrl_type_max
}_mk100_ctrl_type_t;
/*******************************************************************************
  全局变量函数
*******************************************************************************/
/** \brief 帧标识 */
static const uint8_t __g_mk100_frame_flasg[] = {
    0x2F, 0x2E, 0x22, 0x23, 0x24, 0x25, 0x26,
    0x91, 0x92, 0x93, 0x94, 0x97, 0x95, 0x96, 
    0xC1, 0xC2, 0xC3, 0xC4, 0xC6, 0xC7
};
    
/** \brief 串口缓冲区设备 */
static am_uart_rngbuf_dev_t __g_uart_ringbuf_dev;

/*******************************************************************************
  私有函数
*******************************************************************************/
/** \brief 发送命令 */
am_local int __mk100_cmd_send(am_mk100_handle_t handle, uint8_t *cmd, uint32_t cmd_len)
{    
    if (cmd_len != AM_MK100_FRAME_LEN) {
        return -AM_EINVAL;
    }
    
    am_uart_rngbuf_send(handle->uart_rngbuf_handle, cmd, cmd_len);
    
    return AM_OK;
}

/** \brief 接收命令 */
am_local int __mk100_cmd_receive(am_mk100_handle_t handle, uint8_t *cmd, uint32_t cmd_len)
{    
    if (cmd_len < AM_MK100_FRAME_LEN) {
        return -AM_EINVAL;
    }
    
    am_uart_rngbuf_receive(handle->uart_rngbuf_handle, cmd, cmd_len);
    
    return AM_OK;
}

/** \brief 计算CRC */
am_local uint8_t __mk100_xCal_crc(uint8_t *ptr, uint32_t len)
{
     uint8_t crc;
     uint8_t i;
     crc = 0xFF;
     while(len--) {
        crc ^= *ptr++;
        for(i = 0; i < 8; i++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x0131;
            } else {
                crc = (crc << 1);
            }
        }
     }
     return crc;
}

/** \brief 构造命令数据1和数据2部分 */
am_local void __mk100_mkcmd_data(uint8_t* cmd_buff, _mk100_ctrl_type_t ctrl_type, uint16_t parameter)
{
    switch (ctrl_type) {
    case _mk100_ctrl_type_get_angle_gear:
    case _mk100_ctrl_type_get_direction_data:
        cmd_buff[__MK100_FRAME_DATE_1_INDEX] = parameter & 0xFF;
        cmd_buff[__MK100_FRAME_DATE_2_INDEX] = 0x55;
        break;
    case _mk100_ctrl_type_get_knob_pos:
    case _mk100_ctrl_type_get_strength_threshold:
    case _mk100_ctrl_type_get_rotation_direction:
    case _mk100_ctrl_type_get_strength_threshold_status:
    case _mk100_ctrl_type_get_infrared_data:
    case _mk100_ctrl_type_direction_cal:
        cmd_buff[__MK100_FRAME_DATE_1_INDEX] = 0x00;
        cmd_buff[__MK100_FRAME_DATE_2_INDEX] = 0x55;
        break;
    case _mk100_ctrl_type_set_strength_upper_limit:
    case _mk100_ctrl_type_set_strength_lower_limit:
    case _mk100_ctrl_type_set_rotation_direction:
    case _mk100_ctrl_type_set_baudrate:
    case _mk100_ctrl_type_set_system_func:
    case _mk100_ctrl_type_set_knob_sensitivity:
    case _mk100_ctrl_type_set_delay_window:
    case _mk100_ctrl_type_set_enter_low_power_time:
        cmd_buff[__MK100_FRAME_DATE_1_INDEX] = 0x00;
        cmd_buff[__MK100_FRAME_DATE_2_INDEX] = parameter;
        break;
    case _mk100_ctrl_type_set_angle:
    case _mk100_ctrl_type_set_direction_parameter:
    case _mk100_ctrl_type_set_infrared_parameter:
    case _mk100_ctrl_type_set_system_output_mode:
        cmd_buff[__MK100_FRAME_DATE_1_INDEX] = (parameter >> 8) & 0xFF;
        cmd_buff[__MK100_FRAME_DATE_2_INDEX] = parameter & 0xFF;
        break;
    default:
        return; 
    }
}

/** \brief 构造命令 */
am_local int __mk100_mkcmd(uint8_t* cmd_buff, uint32_t cmd_len, _mk100_ctrl_type_t ctrl_type, uint16_t parameter)
{
    if (cmd_len != AM_MK100_FRAME_LEN || 
        ctrl_type >=_mk100_ctrl_type_max ) {
        return -AM_EINVAL; 
    }
    cmd_buff[__MK100_FRAME_HEADER_INDEX] = 0x2A;
    cmd_buff[__MK100_FRAME_FLAG_INDEX] = __g_mk100_frame_flasg[ctrl_type];
    __mk100_mkcmd_data(cmd_buff, ctrl_type, parameter);
    cmd_buff[__MK100_FRAME_CRC_INDEX] = __mk100_xCal_crc(cmd_buff + __MK100_FRAME_FLAG_INDEX, __MK100_FRAME_CRC_LEN);
    
    return AM_OK;
}

/** \brief 对接收到的命令进行检查 */
am_local int __mk100_received_cmd_check(uint8_t* cmd_buff, uint32_t cmd_len, _mk100_ctrl_type_t ctrl_type)
{
    if (cmd_buff[__MK100_FRAME_HEADER_INDEX] != 0x2A ||
        cmd_buff[__MK100_FRAME_FLAG_INDEX] == __MK100_SYS_ERROR_FLAG ||
        cmd_buff[__MK100_FRAME_FLAG_INDEX] == __MK100_FRAME_ERROR_FLAG ||
        cmd_buff[__MK100_FRAME_FLAG_INDEX] != __g_mk100_frame_flasg[ctrl_type] ||
        cmd_buff[__MK100_FRAME_CRC_INDEX] != __mk100_xCal_crc(cmd_buff + __MK100_FRAME_FLAG_INDEX, __MK100_FRAME_CRC_LEN)) {
        return -AM_EINVAL; 
    } else {
        return AM_OK;
    }
}

/** \brief 从接收到的命令中解析出数据 */
am_local int __mk100_received_cmd_parse(uint8_t* cmd_buff, uint32_t cmd_len, _mk100_ctrl_type_t ctrl_type)
{
    int ret = 0;
    
    if (__mk100_received_cmd_check(cmd_buff, cmd_len, ctrl_type) < 0 ) {
        return -AM_EINVAL; 
    }
    
    switch (ctrl_type) {
    case _mk100_ctrl_type_get_angle_gear:
    case _mk100_ctrl_type_set_baudrate:
    case _mk100_ctrl_type_set_system_func:
    case _mk100_ctrl_type_set_knob_sensitivity:
    case _mk100_ctrl_type_set_delay_window:
    case _mk100_ctrl_type_set_enter_low_power_time:
        if (cmd_buff[__MK100_FRAME_DATE_1_INDEX] == 0xFF && 
            cmd_buff[__MK100_FRAME_DATE_2_INDEX] == 0x00) {
            ret = -AM_EINVAL;
        } else {
            ret =  cmd_buff[__MK100_FRAME_DATE_2_INDEX];
        }
        break;
    case _mk100_ctrl_type_get_knob_pos:
    case _mk100_ctrl_type_get_rotation_direction:
    case _mk100_ctrl_type_set_strength_upper_limit:
    case _mk100_ctrl_type_set_strength_lower_limit:
    case _mk100_ctrl_type_set_rotation_direction:
        if (cmd_buff[__MK100_FRAME_DATE_1_INDEX] == 0xFF) {
            ret = -AM_EINVAL;
        } else {
            ret =  cmd_buff[__MK100_FRAME_DATE_2_INDEX];
        }
        break;
    case _mk100_ctrl_type_get_strength_threshold:
    case _mk100_ctrl_type_get_strength_threshold_status:
    case _mk100_ctrl_type_get_direction_data:
    case _mk100_ctrl_type_get_infrared_data:
    case _mk100_ctrl_type_set_angle:
        if (cmd_buff[__MK100_FRAME_DATE_1_INDEX] == 0xFF) {
            ret = -AM_EINVAL;
        } else {
            ret =  (cmd_buff[__MK100_FRAME_DATE_1_INDEX] << 8) | cmd_buff[__MK100_FRAME_DATE_2_INDEX];
        }
        break;
    case _mk100_ctrl_type_direction_cal:
        if (cmd_buff[__MK100_FRAME_DATE_1_INDEX] == 0x00 && 
            cmd_buff[__MK100_FRAME_DATE_2_INDEX] == 0x55) {
            ret = AM_OK; 
        } else {
            return -AM_EINVAL; 
        }
        break;
    case _mk100_ctrl_type_set_direction_parameter:
    case _mk100_ctrl_type_set_infrared_parameter:
    case _mk100_ctrl_type_set_system_output_mode:
        if (cmd_buff[__MK100_FRAME_DATE_1_INDEX] == 0xFF && 
            cmd_buff[__MK100_FRAME_DATE_2_INDEX] == 0x00) {
            ret = -AM_EINVAL;
        } else {
            ret = (cmd_buff[__MK100_FRAME_DATE_1_INDEX] << 8) | cmd_buff[__MK100_FRAME_DATE_2_INDEX];
        }
        break;
    default:
        return -AM_EINVAL; 
    }
            
    return ret;
}

/** \brief 控制发送命令、接收命令、解析数据的流程。当解析到的数据有误，
 *  返回负数，错误的接收或者系统错误返回保存在error_frame
 */
am_local int _mk100_ctrl(am_mk100_handle_t handle, _mk100_ctrl_type_t ctrl_type, uint16_t parameter)
{
    uint8_t send_cmd_buff[AM_MK100_FRAME_LEN] = {0};
    uint8_t receive_cmd_buff[AM_MK100_FRAME_LEN] = {0};
    int ret = 0;
    
    __mk100_mkcmd(send_cmd_buff, AM_MK100_FRAME_LEN, ctrl_type, parameter);
    
    __mk100_cmd_send(handle, send_cmd_buff, AM_MK100_FRAME_LEN);
    
    __mk100_cmd_receive(handle, receive_cmd_buff, AM_MK100_FRAME_LEN);
    
    ret = __mk100_received_cmd_parse(receive_cmd_buff, AM_MK100_FRAME_LEN, ctrl_type);
    if (ret < 0) {
        memcpy(handle->error_frame, receive_cmd_buff, AM_MK100_FRAME_LEN);
    } else {
        memset(handle->error_frame, 0, AM_MK100_FRAME_LEN);
    }
    return ret;
}
/*******************************************************************************
  公共函数
*******************************************************************************/
/**
 * \brief 获取自动上报模式时MK100返回的帧
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] p_type                  : 指向存放返回数据的类型，分别为角度数据或磁旋钮位置数据
 * \param[in] p_data                  : 指向存放数据
 * \return    -AM_EINVAL              : 错误
 *            AM_OK                   : 成功
 */
int am_mk100_get_report(am_mk100_handle_t handle, uint8_t* p_type, uint8_t *p_data)
{
    int ret = 0;
    uint8_t receive_cmd_buff[AM_MK100_FRAME_LEN] = {0};
    ret = __mk100_cmd_receive(handle, receive_cmd_buff, AM_MK100_FRAME_LEN);

    if (ret < 0 ||
        receive_cmd_buff[__MK100_FRAME_HEADER_INDEX] != 0x2A ||
        (receive_cmd_buff[__MK100_FRAME_FLAG_INDEX] != AM_MK100_GET_ANGLE &&
        receive_cmd_buff[__MK100_FRAME_FLAG_INDEX] != AM_MK100_GET_KNOB_POS)) {
        memcpy(handle->error_frame, receive_cmd_buff, AM_MK100_FRAME_LEN);
        return -AM_EINVAL;
    } else {
        memset(handle->error_frame, 0, AM_MK100_FRAME_LEN);
        *p_type = receive_cmd_buff[__MK100_FRAME_FLAG_INDEX];
        *p_data = receive_cmd_buff[__MK100_FRAME_DATE_2_INDEX];
        return AM_OK;
    }
}
/**
 * \brief 获取角度值
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] gear                    : 等级
 * \return    -AM_EINVAL              : 错误
 *            >=0                     : 角度值
 */
int am_mk100_get_angle(am_mk100_handle_t handle, uint8_t gear)
{
    return _mk100_ctrl(handle, _mk100_ctrl_type_get_angle_gear, gear);
}
/**
 * \brief 获取磁旋钮位置
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            >=0                     : 磁旋钮位置
 */
int am_mk100_get_knob(am_mk100_handle_t handle)
{
    return _mk100_ctrl(handle, _mk100_ctrl_type_get_knob_pos, __MK100_NO_PARAMETER);
}
/**
 * \brief 获取当前磁场强度阈值，保存在upper_limit和lower_limit指向的内存中
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] upper_limit             : 上限
 * \param[in] lower_limit             : 下限
 * \return    -AM_EINVAL              : 错误
 *            AM_OK                   : 成功
 */
int am_mk100_get_strength_threshold(am_mk100_handle_t handle, uint8_t* upper_limit, uint8_t* lower_limit)
{
    int ret = 0;
    ret = _mk100_ctrl(handle, _mk100_ctrl_type_get_strength_threshold, __MK100_NO_PARAMETER);
    if (ret < 0) {
        return ret;
    } else {
        *upper_limit = (ret >> 8) & 0xFF;
        *lower_limit = ret & 0xFF;
        return AM_OK;
    }        
}
/**
 * \brief 获取当前旋转方向
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            >=0                     : 当前旋转方向
 */
int am_mk100_get_rotation_direction(am_mk100_handle_t handle)
{
    return _mk100_ctrl(handle, _mk100_ctrl_type_get_rotation_direction, __MK100_NO_PARAMETER);
}
/**
 * \brief 获取当前磁场强度阈值状态，保存在upper_limit_status和lower_limit_status指向的内存中
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] upper_limit_status      : 0x01 表示当前磁强度已达到上限， 0x00 表示当前磁强度未达到上限
 * \param[in] lower_limit_status      : 0x01 表示当前磁强度已达到下限， 0x00 表示当前磁强度未达到下限
 * \return    -AM_EINVAL              : 错误
 *            AM_OK                   : 成功
 */
int am_mk100_get_strength_threshold_status(am_mk100_handle_t handle, am_bool_t* upper_limit_status, am_bool_t* lower_limit_status)
{
    int ret = 0;
    uint8_t data1 = 0;
    uint8_t data2 = 0;
    ret = _mk100_ctrl(handle, _mk100_ctrl_type_get_strength_threshold_status, __MK100_NO_PARAMETER);
    if (ret < 0) {
        return ret;
    } else {
        data1 = (ret >> 8) & 0xFF;
        data2 = ret & 0xFF;
        if (data1 == 0x01) {
            *upper_limit_status = AM_TRUE;
        } else if (data1 == 0x00) {
            *upper_limit_status = AM_FALSE; 
        } else {
            return -AM_EINVAL; 
        }
        if (data2 == 0x01) {
            *lower_limit_status = AM_TRUE;
        } else if (data1 == 0x00) {
            *lower_limit_status = AM_FALSE; 
        } else {
            return -AM_EINVAL; 
        }
        return AM_OK;
    }        
}
/**
 * \brief 获取方向传感器数据
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] direction               : 方向，上下左右分别为AM_MK100_DIRECTION_UP、AM_MK100_DIRECTION_DOWN、AM_MK100_DIRECTION_LEFT、AM_MK100_DIRECTION_RIGHT    
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 该方向的数据
 */
int am_mk100_get_direction_data(am_mk100_handle_t handle, uint8_t direction)
{
    if (direction !=  AM_MK100_DIRECTION_UP &&
        direction !=  AM_MK100_DIRECTION_DOWN &&
        direction !=  AM_MK100_DIRECTION_LEFT &&
        direction !=  AM_MK100_DIRECTION_RIGHT) {
         return -AM_EINVAL;    
    }
    return _mk100_ctrl(handle, _mk100_ctrl_type_get_direction_data, direction);
}
/**
 * \brief 获取红外传感器数据
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的角度值
 */
int am_mk100_get_infrared_data(am_mk100_handle_t handle)
{
    return _mk100_ctrl(handle, _mk100_ctrl_type_get_infrared_data, __MK100_NO_PARAMETER);
}
/**
 * \brief 设置当前角度值
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] angle                   : 角度值，0~360
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的角度值
 */
int am_mk100_set_angle(am_mk100_handle_t handle, uint16_t angle)
{
    if (angle > 360) {
        return -AM_EINVAL; 
    }
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_angle, angle);
}
/**
 * \brief 设置当前磁场强度阈值上限
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] limit                   : 阈值上限，最大值为0x07
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的阈值上限
 */
int am_mk100_set_strength_upper_limit(am_mk100_handle_t handle, uint8_t limit)
{
    if (limit > 0x07) {
        return -AM_EINVAL; 
    }
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_strength_upper_limit, limit);
}
/**
 * \brief 设置当前磁场强度阈值下限
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] limit                   : 阈值下限，最大值为0x07
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的阈值下限
 */
int am_mk100_set_strength_lower_limit(am_mk100_handle_t handle, uint8_t limit)
{
    if (limit > 0x07) {
        return -AM_EINVAL; 
    }
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_strength_lower_limit, limit);
}
/**
 * \brief 设置当前旋转方向
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] direction               : 方向，AM_MK100_DIR_CW: 顺时针、AM_MK100_DIR_CCW :逆时针
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的方向
 */
int am_mk100_set_rotation_direction(am_mk100_handle_t handle, uint8_t direction)
{
    if (direction != AM_MK100_DIR_CW && direction != AM_MK100_DIR_CCW) {
        return -AM_EINVAL; 
    }
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_rotation_direction, direction);
}
/**
 * \brief 方向传感器初始值校准
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            AM_OK                   : 成功
 */
int am_mk100_direction_cal(am_mk100_handle_t handle)
{
    return _mk100_ctrl(handle, _mk100_ctrl_type_direction_cal, __MK100_NO_PARAMETER);
}
/**
 * \brief 设置方向传感器参数
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] data                    : 方向传感器参数
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的方向传感器参数
 */
int am_mk100_set_direction_parameter(am_mk100_handle_t handle, uint8_t direction, uint16_t threshold)
{
    uint16_t parameter;
    if (threshold > 3300) {
        return -AM_EINVAL;
    }
    parameter = ((direction << 12) & (threshold & 0xFFF)) & 0xFFFF;
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_direction_parameter, parameter);
}
/**
 * \brief 设置红外传感器参数
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] data                    : 红外传感器参数
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的红外传感器参数
 */
int am_mk100_set_infrared_parameter(am_mk100_handle_t handle, uint16_t data)
{
    if (data > 1023) {
        return -AM_EINVAL; 
    }
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_infrared_parameter, data);
}
/**
 * \brief 设置系统波特率
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] baudrate_setting        : 波特率选择，只能设置AM_MK100_BAUDRATE_***这些宏定义
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设置的波特率
 */
int am_mk100_set_baudrate(am_mk100_handle_t handle, uint8_t baudrate_setting)
{
    if(baudrate_setting > AM_MK100_BAUDRATE_115200) {
        return -AM_EINVAL; 
    }
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_baudrate, baudrate_setting);
}
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
int am_mk100_set_system_func(am_mk100_handle_t handle, uint8_t mode)
{
    if (mode != AM_MK100_SET_ROTATION_FUNC &&
        mode != AM_MK100_SET_INFRARED_DETECT && 
        mode != AM_MK100_SET_STATUS_DETECT &&
        mode != AM_MK100_SET_LEFT_RIGNH_DETECT &&
        mode != AM_MK100_SET_UP_DOWN_DETECT &&
        mode != AM_MK100_SET_UP_DOWN_LEFT_RIGNH_DETECT) {
         return -AM_EINVAL;    
    }
        
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_system_func, mode);
}

/**
 * \brief 设置系统为查询模式
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前模式
 */
int am_mk100_set_query_mode(am_mk100_handle_t handle)
{
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_system_output_mode, __MK100_SET_QUERY_MODE);
}
/**
 * \brief 设置系统为主动上报模式, 数据有改变时输出，设置完成后需要用am_mk100_get_report来获取数据，不能设置延时，否则会数据读取错误
 *
 * \param[in] handle                  : mk100句柄
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前模式
 */
int am_mk100_set_report_change_mode(am_mk100_handle_t handle)
{
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_system_output_mode, __MK100_SET_REPORT_CHANGE_MODE);
}
/**
 * \brief 设置系统为主动上报模式, 固定频率输出，设置完成后需要用am_mk100_get_report来获取数据，不能设置延时，否则会数据读取错误
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] time                    : 固定间隔时间,单位为 ms，范围为 50 - 2000
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前模式
 */
int am_mk100_set_report_periodically_mode(am_mk100_handle_t handle, uint16_t time)
{
    if (time < 50 || time > 2000) {
        return -AM_EINVAL; 
    }
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_system_output_mode, time);
}
/**
 * \brief 设置旋钮灵敏度
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] sensitivity             : 灵敏度等级选项，值的范围为 2-50
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前灵敏度等级
 */
int am_mk100_set_knob_sensitivity(am_mk100_handle_t handle, uint8_t sensitivity)
{
    if(sensitivity < 2 || sensitivity > 50) {
        return -AM_EINVAL;
    }
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_knob_sensitivity, sensitivity);
}
/**
 * \brief 设置延时窗口宽度
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] delay_window            : 延时窗口宽度，单位为采样时间 10ms，值的范围为 10-100。例如配置的值为 50，则过滤窗口总时间为 500ms
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前延时窗口宽度
 */
int am_mk100_set_delay_window(am_mk100_handle_t handle, uint8_t delay_window)
{
    if(delay_window < 10 || delay_window > 100) {
        return -AM_EINVAL;
    }
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_delay_window, delay_window);
}
/**
 * \brief 设置进入低功耗时间
 *
 * \param[in] handle                  : mk100句柄
 * \param[in] enter_low_power_time    : 进入低功耗时间，单位为秒, 值的范围为 0-255
 * \return    -AM_EINVAL              : 错误
 *            >= 0                    : 当前设定的时间
 */
int am_mk100_set_enter_low_power_time(am_mk100_handle_t handle, uint8_t enter_low_power_time)
{
    return _mk100_ctrl(handle, _mk100_ctrl_type_set_enter_low_power_time, enter_low_power_time);
}
/**
 * \brief 打印错误接收
 *
 * \param[in] handle              : mk100句柄
 * \return    无 
 */
void am_mk100_display_error_recevice(am_mk100_handle_t handle)
{
    uint8_t *cmd = handle->error_frame;
    AM_DBG_INFO("error recevice: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\r\n",
                cmd[0], cmd[1], cmd[2], cmd[3], cmd[4]);
}
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
                                 am_uart_handle_t          uart_handle)
{
    if ((p_dev                   == NULL) ||
        (p_devinfo               == NULL) ||
        (p_devinfo->p_uart_rxbuf == NULL) ||
        (p_devinfo->p_uart_txbuf == NULL)) {

        return NULL;
    }
    p_dev->p_devinfo = p_devinfo;
    p_dev->uart_rngbuf_handle = am_uart_rngbuf_init(&__g_uart_ringbuf_dev,
                                      uart_handle,
                                      p_dev->p_devinfo->p_uart_rxbuf,
                                      p_dev->p_devinfo->rxbuf_size,
                                      p_dev->p_devinfo->p_uart_txbuf,
                                      p_dev->p_devinfo->txbuf_size);
    memset(p_dev->error_frame, 0, AM_MK100_FRAME_LEN);
    return p_dev;
}
/* end of file */
