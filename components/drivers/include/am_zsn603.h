#ifndef __AM_ZSN603_H
#define __AM_ZSN603_H

#include "ametal.h"
#include "am_i2c.h"
#include "zsn603.h"
#include "am_gpio.h"
#include "am_uart_rngbuf.h"

typedef struct am_zsn603_dev_info{
    int8_t   rst_pin;           /** \brief 复位引脚    若用户不需要使用此引脚   填-1即可*/
    int8_t   mode_det_pin;     /** \brief 模式选择引脚    若用户不需要使用此引脚   填-1即可  */
    uint8_t  local_address;     /** \brief 初始化local address  */
    /* \brief 在IIC模式下  此成员为中断引脚号
     *        在UART模式下  此成员为波特率标识
     */
    uint8_t other_para;

}am_zsn603_dev_info_t;

/**
 * \brief ZSN600x 设备初始化函数(UART通信模式)
 *
 * \param[in]  p_dev           : ZSN603设备
 * \param[in]  p_dev_info      : ZSN603设备信心
 * \param[in]  p_arg           : 传输函数指针参数
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
zsn603_handle_t am_zsn603_uart_init(zsn603_dev_t                *p_dev,
                                    const am_zsn603_dev_info_t  *p_dev_info,
                                    void                        *p_arg);


/**
 * \brief ZSN600x 设备初始化函数(I2C通信模式)
 *
 * \param[in]  p_dev           : ZSN603设备
 * \param[in]  p_dev_info      : ZSN603设备信心
 * \param[in]  p_arg           : 传输函数指针参数
 *
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
zsn603_handle_t am_zsn603_i2c_init(zsn603_dev_t                *p_dev,
                                   const am_zsn603_dev_info_t  *p_dev_info,
                                   void                        *p_arg);
/**
 * \brief 设置控制ZSN603的串口的波特率  此函数用于设置波特率指令之后重设主机波特率
 *
 * \param[in]  p_dev           : ZSN603设备
 * \param[in]  baud_flag       : 波特率标识  可选参数为
 *                        ZSN603_BAUD_172800     ZSN603_BAUD_115200
 *                        ZSN603_BAUD_57600      ZSN603_BAUD_38400
 *                        ZSN603_BAUD_28800      ZSN603_BAUD_19200
 *                        ZSN603_BAUD_230400     ZSN603_BAUD_9600
 * \retval 其他 : 对应错误码
 * \retval  0 : 执行成功
 */
int am_zsn603_set_uart_baud(zsn603_dev_t  *p_dev,
                            uint8_t        baud_flag);
#endif


