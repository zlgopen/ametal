/**
 * \file
 * \brief ZSN603组件相关底层驱动
 *
 */
#include "zsn603.h"
#include "stdio.h"
#include "string.h"

/**\brief  中断引脚回调函数*/
void zsn603_uart_data_rx_callback(void *p_arg, char data)
{
    zsn603_uart_dev_t *p_dev   = (zsn603_uart_dev_t *)p_arg;

    p_dev->buffer[p_dev->rx_count ++] = data;

    /*若接收数据大于最大缓冲区大小，则清空缓冲区*/
    if(p_dev->rx_count >= ZSN603_BUFFER_SIZE_MAX){
        p_dev->rx_count = 0;
    }

    if((p_dev->rx_count >= p_dev->is_wait) && (p_dev->is_wait != 0)){
        if(p_dev->zsn603_dev.status == ZSN603_NORMAL_MODE){
            /*若为正常命令响应模式   则 释放等待串口数据信号量 ,并将等待字节数清0*/
            p_dev->is_wait = 0;
            ZSN603_SEMB_GIVE(p_dev->uart_semb);
            return;

        }else if((p_dev->zsn603_dev.status == ZSN603_AUTO_MODE_NO_INT) ||
               (p_dev->zsn603_dev.status == ZSN603_AUTO_MODE_INT_MESSAGE)){

            if(p_dev->is_wait == ZSN603_FRAME_HEAD_LENGHT){
                if(p_dev->buffer[3] == 0x02){
                    /*如果接收当前数据为完成的自动检测数据回应帧帧头,则继续等待帧接收完成*/
                    p_dev->is_wait = ZSN603_FRAME_FRAME_MIN_LENGHT +
                                   ((p_dev->buffer[7] << 8) | p_dev->buffer[6]);
                }else{
                    /*如果接收当前数据为错误数据,则丢弃该数据*/
                    p_dev->rx_count = 0;
                    p_dev->is_wait  = 0;
                }
                return;
            }else if(p_dev->is_wait >= ZSN603_FRAME_FRAME_MIN_LENGHT){
                p_dev->is_wait = 0;
                if(p_dev->zsn603_dev.pfn_card_cb){
                    p_dev->zsn603_dev.pfn_card_cb(p_dev->zsn603_dev.p_arg);
                }
                return;
            }
        }
    }

    if(((p_dev->zsn603_dev.status == ZSN603_AUTO_MODE_NO_INT) ||
            (p_dev->zsn603_dev.status == ZSN603_AUTO_MODE_INT_MESSAGE)) &&
            (p_dev->buffer[p_dev->rx_count - 1] == 0xb3)){
        /* 若在ZSN603_AUTO_MODE_NO_INT 模式下  检测到当前数据为0xb3 则此帧信息可能为自动检测数据*/
        p_dev->rx_count = 0;
        p_dev->buffer[p_dev->rx_count ++] = data;
        p_dev->is_wait = ZSN603_FRAME_HEAD_LENGHT;
    }
}


static int __zsn603_uart_wait_data(zsn603_uart_dev_t *p_dev, uint32_t nbytes, uint32_t timeout)
{
    if(p_dev->rx_count < nbytes){
        p_dev->is_wait = nbytes;
        return ZSN603_SEMB_TAKE(p_dev->uart_semb, timeout);
    }else{
        return 0;
    }
}

/**\brief  串口命令传输函数 */
static uint8_t __zsn603_uart_cmd_tx(void      *p_drv,
                                    uint8_t   *p_cmd_data,
                                    uint32_t   cmd_lenght)
{
    uint8_t            ret   = 0;
    zsn603_uart_dev_t *p_dev = p_drv;
    ret =  zsn603_platform_uart_tx(&p_dev->platform_dev, p_cmd_data, cmd_lenght);
    p_dev->rx_count = 0;
    return ret;
}
/**\brief  I2C命令传输函数 */
static uint8_t __zsn603_i2c_cmd_tx(void      *p_drv,
                                   uint8_t   *p_cmd_data,
                                   uint32_t   cmd_lenght)
{
    int               ret         = -1;
    uint8_t           buffer[2]   = {0};
    zsn603_i2c_dev_t *p_dev       = (zsn603_i2c_dev_t *)p_drv;

    /* 往 0x0102 寄存器地址发送  总帧长度信息 */
    buffer[0] = (uint8_t)(cmd_lenght & 0xff);
    buffer[1] = (uint8_t)((cmd_lenght >> 8) & 0xff);

    ret = zsn603_platform_i2c_write(&p_dev->platform_dev,
                                    (p_dev->zsn603_dev.local_addr >> 1),
                                     0x0102,
                                     buffer,
                                     2);

    if(ret == ZSN603_EXECUTE_SUCCESS){

        /* 往 0x0104 寄存器地址发送  命令帧 */
        ret = zsn603_platform_i2c_write(&p_dev->platform_dev,
                                        (p_dev->zsn603_dev.local_addr >> 1),
                                         0x0104,
                                         p_cmd_data,
                                         cmd_lenght);

        if(ret == ZSN603_EXECUTE_SUCCESS){

            /* 往 0x0101 寄存器地址发送  0x8D  开始执行命令 */
            buffer[0] = 0x8D;
            return zsn603_platform_i2c_write(&p_dev->platform_dev,
                                             (p_dev->zsn603_dev.local_addr >> 1),
                                              0x0101,
                                              buffer,
                                              1);
        }else{
            return ZSN603_IIC_TRANS_ERR;
        }
    }
    return ZSN603_IIC_TRANS_ERR;
}

/**\brief  传输命令帧构建函数*/
static uint8_t __zsn603_cmd_execute(zsn603_dev_t  *p_dev,
                                    uint8_t        local_addr,
                                    uint8_t        cmd_class,
                                    uint8_t        cmd_code,
                                    uint8_t       *p_tx_data,
                                    uint32_t       tx_data_count,
                                    uint32_t       buffer_len,
                                    uint8_t       *p_rx_data,
                                    uint32_t      *p_rx_data_count)
{
    uint32_t  ret         =  0;
    uint32_t  i           =  0;
    uint32_t  check_sum   =  0;
    uint8_t   temp[290]   = {0};
    uint8_t   org_status  =  0;
    uint32_t  length      =  0;

    if(p_dev == NULL){
        return ZSN603_DEV_SERVER_ERR;
    }

    org_status    = p_dev->status;
    p_dev->status = ZSN603_NORMAL_MODE;

    /* 构建需要进行发送的命令帧  */
    temp[length++] = local_addr;
    temp[length++] = 0x00;
    temp[length++] = 0x00;
    temp[length++] = cmd_class;
    temp[length++] = (uint8_t)(cmd_code & 0xff);
    temp[length++] = (uint8_t)((cmd_code >> 8) & 0xff);
    temp[length++] = (uint8_t)(tx_data_count & 0xff);
    temp[length++] = (uint8_t)((tx_data_count >> 8) & 0xff);

    memcpy((void *)&temp[length], p_tx_data, tx_data_count);
    length += tx_data_count;

    for(i = 0; i < length; i++){
        check_sum += temp[i];
    }
    check_sum = (~check_sum);

    temp[length++] = (uint8_t)(check_sum & 0xff);
    temp[length++] = (uint8_t)((check_sum >> 8) & 0xff);

    /* 调用命令发送传输函数 */
    ret = p_dev->pfn_cmd_tx((void *)p_dev, temp, length);

    if(ret != ZSN603_EXECUTE_SUCCESS){
        return ret;
    }else{
        /* 调用接收解析数据函数 */
        ret =  p_dev->pfn_cmd_rx_analysis((void *)p_dev,
                                           buffer_len,
                                           p_rx_data,
                                           p_rx_data_count);

        if((tx_data_count < 3) && (org_status != ZSN603_NORMAL_MODE) && ret == 0){
            if((cmd_class == 0x01 && cmd_code == 0x4B) ||
               (cmd_class == 0x02 && cmd_code == 0x4F)){
                p_dev->status = org_status;
            }else{
                p_dev->status = ZSN603_NORMAL_MODE;
            }
        }
        return ret;
    }
}

/**\brief  I2C等待接收回应帧*/
uint8_t __zsn603_iic_message_recv(void      *p_drv,
                                  uint8_t   *p_data,
                                  uint32_t  *p_lenght)
{
    int           ret          = -1;
    uint32_t      frame_lenght =  0;
    uint8_t       buffer[2]    = {0};
    zsn603_i2c_dev_t *p_dev = (zsn603_i2c_dev_t *)p_drv;
    if(p_dev == NULL){
        return ZSN603_DEV_SERVER_ERR;
    }

    ret = ZSN603_SEMB_TAKE(p_dev->int_semb, 800);
    if(ret != ZSN603_EXECUTE_SUCCESS){
        return ZSN603_I2C_SEMB_TIMEOUT;
    }

    /* 读 0x0102 寄存器    获取总帧长度信息 */
    ret = zsn603_platform_i2c_read(&p_dev->platform_dev,
                                   (p_dev->zsn603_dev.local_addr >> 1),
                                    0x0102,
                                    buffer,
                                    2);

    if(ret == ZSN603_EXECUTE_SUCCESS){

        /* 读 0x0104 寄存器    获取帧信息 */
        frame_lenght = buffer[0] + buffer[1] * 256;
        ret = zsn603_platform_i2c_read(&p_dev->platform_dev,
                                       (p_dev->zsn603_dev.local_addr >> 1),
                                        0x0104,
                                        p_data,
                                        frame_lenght);

        if(ret == ZSN603_EXECUTE_SUCCESS){
            *p_lenght = frame_lenght;
            return ret;
        }else{
            return ZSN603_FRAME_INFO_RX_ERR;
        }
    }else{
        return ZSN603_FRAME_LENGHT_RX_ERR;
    }
}

/**\brief  I2C接收解析回应帧*/
static int __zsn603_i2c_message_rx_analysis(void           *p_drv,
                                            uint32_t        buffer_len,
                                            uint8_t        *p_data,
                                            uint32_t       *p_data_lenght)
{
    uint8_t ret              =  0;
    uint32_t  count            =  0;
    uint32_t  check_sum        =  0;
    uint32_t  check_sum_rx     =  0;
    uint32_t  frame_length     =  0;
    uint8_t   rx_data[282]     = {0};
    uint32_t  rx_data_length   =  0;
    uint32_t  status;                      /** \brief 执行状态 */
    uint32_t  info_lenght;                 /** \brief 信息长度 */
    zsn603_dev_t *p_dev = (zsn603_dev_t *)p_drv;
    if(p_dev == NULL){
        return ZSN603_DEV_SERVER_ERR;
    }
    /* IIC 接收回应帧数据 */
    ret = __zsn603_iic_message_recv((void *)p_dev, rx_data, &rx_data_length);

    if(ret != ZSN603_EXECUTE_SUCCESS){
        return ret;
    }

    /* 解析回应帧数据 */
    if(rx_data_length < 10){
        return  ZSN603_FRAME_LENGHT_ERR;
    }

    while(count < rx_data_length){
        if(rx_data[count] == (p_dev->local_addr + 1)){
            break;
        }
        count ++;
    }

    if(count == rx_data_length){
        return ZSN603_FRAME_NO_SLV_ADDRS_ERR;
    }
    status        = ((rx_data[count + 4] << 8) | rx_data[count + 5]);
    info_lenght   = ((rx_data[count + 7] << 8) | rx_data[count + 6]);

    frame_length = 8 + info_lenght;

    if(rx_data_length < (frame_length + 2)){
        return  ZSN603_FRAME_LENGHT_ERR;
    }

    for(count = 0; count < frame_length; count ++){
        check_sum += rx_data[count];
    }
    check_sum = (~check_sum) & 0xffff;

    check_sum_rx = (rx_data[frame_length + 1] << 8) | (rx_data[frame_length] & 0xff);

    if(check_sum_rx == check_sum){
        if(status == 0){
            if(p_data_lenght != NULL){
                *p_data_lenght = info_lenght;
            }
            if((p_data != NULL) && (info_lenght !=0)){
                if(buffer_len >= info_lenght){
                    memcpy((void *)p_data, (const void *)&rx_data[8], info_lenght);
                }else{
                    if(p_data_lenght != NULL){
                        *p_data_lenght = buffer_len;
                    }
                    memcpy((void *)p_data, (const void *)&rx_data[8], buffer_len);
                }
            }
        }
        return status;
    }else{
        return ZSN603_FRAME_CHECK_SUM_ERR;
    }
}

/**\brief  UART接收解析回应帧*/
static int __zsn603_uart_message_rx_analysis(void      *p_drv,
                                             uint32_t   buffer_len,
                                             uint8_t   *p_data,
                                             uint32_t  *p_data_lenght)
{
    int       ret              = -1;
    uint8_t   frame_head_index =  0;
    uint8_t  *p_rx_data        =  NULL;
    uint32_t  i                =  0;
    uint32_t  check_sum        =  0;
    uint32_t  check_sum_rx     =  0;
    uint32_t  status           =  0;
    uint32_t  info_lenght      =  0;

    zsn603_uart_dev_t     *p_dev       = (zsn603_uart_dev_t *)p_drv;\

    p_rx_data = p_dev->buffer;
    if(p_dev == NULL){
        return ZSN603_DEV_SERVER_ERR;
    }

    do{
        ret  = __zsn603_uart_wait_data(p_dev, frame_head_index + 1, 2000);
        frame_head_index ++;
    }while(ret == 0 && (p_rx_data[frame_head_index - 1] != (p_dev->zsn603_dev.local_addr + 1)));

    frame_head_index -= 1;

    if(ret != 0){
        p_dev->rx_count = 0;
        return ZSN603_UART_RX_TIMEOUT;
    }

    ret = __zsn603_uart_wait_data(p_dev, frame_head_index + 8, 500);

    if(ret != 0){
        p_dev->rx_count = 0;
        return ZSN603_FRAME_LENGHT_RX_ERR;
    }
    status       = ((p_rx_data[frame_head_index + 4] << 8) |
                     p_rx_data[frame_head_index + 5]);
    info_lenght  = ((p_rx_data[frame_head_index + 7] << 8) |
                     p_rx_data[frame_head_index + 6]);

    ret = __zsn603_uart_wait_data(p_dev, frame_head_index + info_lenght + 10, 500);

    if(ret != 0){
        p_dev->rx_count = 0;
        return ZSN603_FRAME_LENGHT_RX_ERR;
    }

    for(i = 0; i < (info_lenght + 8); i ++){
        check_sum += p_rx_data[frame_head_index + i];
    }
    check_sum = (~check_sum) & 0xffff;
    check_sum_rx = (p_rx_data[frame_head_index + info_lenght + 9] << 8) |
                   (p_rx_data[frame_head_index + info_lenght + 8] & 0xff);

    if(check_sum == check_sum_rx){
        if(status == 0){
            if(p_data_lenght != NULL){
                *p_data_lenght = info_lenght;
            }
            if((p_data != NULL) && (info_lenght != 0)){

                if(buffer_len >= info_lenght){
                    memcpy((void *)p_data,
                           (const void *)&p_rx_data[frame_head_index + 8],
                           info_lenght);
                }else{
                    if(p_data_lenght != NULL){
                        *p_data_lenght = buffer_len;
                    }
                    memcpy((void *)p_data,
                           (const void *)&p_rx_data[frame_head_index + 8],
                            buffer_len);
                }
            }
        }
        p_dev->rx_count = 0;
        return status;
    }else{
        p_dev->rx_count = 0;
        return ZSN603_FRAME_CHECK_SUM_ERR;
    }
}

/**\brief I2C模式  中断引脚回调函数*/
void zsn603_i2c_int_pin_hander(void *p_arg)
{
    zsn603_i2c_dev_t *p_dev   = (zsn603_i2c_dev_t *)p_arg;
    if(p_dev->zsn603_dev.status == ZSN603_NORMAL_MODE){
        ZSN603_SEMB_GIVE(p_dev->int_semb);
    }else{
        if(p_dev->zsn603_dev.pfn_card_cb){
            p_dev->zsn603_dev.pfn_card_cb(p_dev->zsn603_dev.p_arg);
        }
    }
}

/**\brief UART模式  中断引脚回调函数*/
void zsn603_uart_int_pin_hander(void *p_arg)
{
    zsn603_uart_dev_t *p_dev   = (zsn603_uart_dev_t *)p_arg;
   if(p_dev->zsn603_dev.status == ZSN603_AUTO_MODE_INT){
       if(p_dev->zsn603_dev.pfn_card_cb){
           p_dev->zsn603_dev.pfn_card_cb(p_dev->zsn603_dev.p_arg);
       }
   }
}

/**\brief  串口回调函数结构体 */
zsn603_uart_funcs_t __g_uart_func = {
    zsn603_uart_data_rx_callback,
    zsn603_uart_int_pin_hander
};

/**\brief  ZSN603设备初始化 UART模式*/
zsn603_handle_t  zsn603_uart_init(zsn603_uart_dev_t            *p_dev,
                                  const zsn603_uart_devinfo_t  *p_devinfo)
{
    if(p_dev == NULL || p_devinfo == NULL){
        return NULL;
    }

    p_dev->rx_count                       = 0;
    p_dev->zsn603_dev.work_mode           = ZSN603_WORK_MODE_UART;
    p_dev->zsn603_dev.status              = ZSN603_NORMAL_MODE;
    p_dev->zsn603_dev.local_addr          = p_devinfo->local_addr;

    p_dev->zsn603_dev.pfn_cmd_tx          = __zsn603_uart_cmd_tx;
    p_dev->zsn603_dev.pfn_cmd_rx_analysis = __zsn603_uart_message_rx_analysis;

    ZSN603_SEMB_INIT(p_dev->uart_semb);

    if(0 == zsn603_platform_uart_init(
            &p_dev->platform_dev,
            (const zsn603_platform_uart_devinfo_t *)&p_devinfo->platform_info,
            &__g_uart_func,
            (void *)p_dev)){

        return (zsn603_handle_t)&p_dev->zsn603_dev;

    }else {
        return NULL;
    }
}

/**\brief  ZSN603解初始化 UART模式*/
int  zsn603_uart_deinit(zsn603_handle_t handle)
{
    int ret = 0;
    zsn603_uart_dev_t *p_dev;
    if(handle == NULL){
        return 1;
    }
    p_dev = (zsn603_uart_dev_t *)handle;

    ret = zsn603_platform_uart_deinit(&p_dev->platform_dev);

    return ret;
}

/**\brief  ZSN603设备初始化I2C模式*/
zsn603_handle_t  zsn603_i2c_init(zsn603_i2c_dev_t           *p_dev,
                                 const zsn603_i2c_devinfo_t *p_devinfo)
{
    if(p_dev == NULL){
        return NULL;
    }
    p_dev->zsn603_dev.work_mode           = ZSN603_WORK_MODE_IIC;
    p_dev->zsn603_dev.status              = ZSN603_NORMAL_MODE;
    p_dev->zsn603_dev.local_addr          = p_devinfo->local_addr;
    p_dev->zsn603_dev.pfn_cmd_tx          = __zsn603_i2c_cmd_tx;
    p_dev->zsn603_dev.pfn_cmd_rx_analysis = __zsn603_i2c_message_rx_analysis;

    ZSN603_SEMB_INIT(p_dev->int_semb);

    if(0 == zsn603_platform_i2c_init(
            &p_dev->platform_dev,
            (const zsn603_platform_i2c_devinfo_t *)&p_devinfo->platform_info,
             zsn603_i2c_int_pin_hander,
            (void *)p_dev)){

        return &p_dev->zsn603_dev;

    }else{
        return NULL;
    }
}

/**\brief  ZSN603设备解初始化I2C模式*/
int  zsn603_i2c_deinit(zsn603_handle_t handle)
{
    int ret = 0;
    zsn603_i2c_dev_t *p_dev;
    if(handle == NULL){
        return 1;
    }
    p_dev = (zsn603_i2c_dev_t *)handle;

    ret = zsn603_platform_i2c_deinit(&p_dev->platform_dev);

    return ret;

}


/** \brief 获取设备信息 */
uint8_t zsn603_get_device_info(zsn603_handle_t      handle,
                               uint32_t             buffer_len,
                               uint8_t             *p_rx_data,
                               uint32_t            *p_rx_data_count)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_GET_DEVICE_INFO,
                                NULL, 0,
                                buffer_len, p_rx_data, p_rx_data_count);
}
/** \brief 配置IC卡接口 */
uint8_t zsn603_config_icc_interface(zsn603_handle_t  handle)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_CONFIG_ICC_INTERFACE,
                                NULL, 0,
                                0, NULL, NULL);
}
/** \brief 关闭IC卡接口 */
uint8_t zsn603_close_icc_interface(zsn603_handle_t  handle)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_CLOSE_ICC_INTERFACE,
                                NULL, 0,
                                0, NULL, NULL);
}
/** \brief 设置协议类型 */
uint8_t zsn603_set_ios_type(zsn603_handle_t  handle,
                            uint8_t          isotype)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_SET_ICC_INTERFACE_PROTOCOL,
                                &isotype, 1,
                                0, NULL, NULL);
}
/** \brief 装载IC卡密钥 */
uint8_t zsn603_load_icc_key(zsn603_handle_t  handle,
                            uint8_t          key_type,
                            uint8_t          key_block,
                            uint8_t         *p_key,
                            uint32_t         key_length)
{
    uint8_t temp[18] = {0};
    temp[0] = key_type;
    temp[1] = key_block;
    memcpy(&temp[2], p_key, key_length);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_LOAD_ICC_KEY,
                                temp, key_length + 2,
                                0, NULL, NULL);
}
/** \brief 设置IC卡接口的寄存器值 */
uint8_t zsn603_set_icc_reg(zsn603_handle_t  handle,
                           uint8_t          reg_addr,
                           uint8_t          reg_val)
{
    uint8_t temp[2] = {0};
    temp[0] = reg_addr;
    temp[1] = reg_val;
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_SET_ICC_INTERFACE_REG,
                                temp, 2,
                                0, NULL, NULL);
}
/** \brief 获取IC卡接口的寄存器值 */
uint8_t zsn603_get_icc_reg(zsn603_handle_t  handle,
                           uint8_t          reg_addr,
                           uint8_t         *p_val)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_GET_ICC_INTERFACE_REG,
                                &reg_addr, 1,
                                1, p_val, NULL);
}
/** \brief 设置波特率 */
uint8_t zsn603_set_baud_rate(zsn603_handle_t  handle,
                             uint8_t          baudrate_flag)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_SET_BAUD_RATE,
                                &baudrate_flag, 1,
                                0, NULL, NULL);
}
/** \brief 设置天线模式 */
uint8_t zsn603_set_ant_mode(zsn603_handle_t  handle,
                            uint8_t          antmode_flag)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_SET_ICC_INTERFACE_OUTPUT,
                                &antmode_flag, 1,
                                0, NULL, NULL);
}
/** \brief 设置天线通道 */
uint8_t zsn603_set_ant_channel(zsn603_handle_t  handle,
                               uint8_t          ant_channel)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_SET_ANTENNA_CHANNAL,
                                &ant_channel, 1,
                                0, NULL, NULL);
}
/** \brief 设置local address */
uint8_t zsn603_set_local_addr(zsn603_handle_t  handle,
                              uint8_t          local_addr)
{
    uint8_t ret = 0;
    ret = __zsn603_cmd_execute(handle,
                               handle->local_addr,
                               ZSN603_DEVICE_CONTROL_CMD,
                               ZSN603_SET_LOCAL_ADDRESS,
                              &local_addr, 1,
                               0, NULL, NULL);

     if(ret == 0){
         handle->local_addr = local_addr;
     }

     return ret;
}
/** \brief LED灯控制 */
uint8_t zsn603_control_led(zsn603_handle_t  handle,
                           uint8_t          control_led)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_LED_CONTROL,
                               &control_led, 1,
                                0, NULL, NULL);
}
/** \brief 蜂鸣器灯控制 */
uint8_t zsn603_control_buzzer(zsn603_handle_t  handle,
                              uint8_t           control_byte)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_BUZZER_CONTROL,
                               &control_byte, 1,
                                0, NULL, NULL);
}
/** \brief 读EEPROM */
uint8_t zsn603_read_eeprom(zsn603_handle_t  handle,
                           uint8_t          eeprom_addr,
                           uint8_t          nbytes,
                           uint32_t         buffer_len,
                           uint8_t         *p_buf)
{
    uint8_t temp[2] = {0};
    temp[0] = eeprom_addr;
    temp[1] = nbytes;
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_READ_EEPROM,
                                temp, 2,
                                buffer_len, p_buf, NULL);
}
/** \brief 写EEPROM */
uint8_t zsn603_write_eeprom(zsn603_handle_t  handle,
                            uint8_t          eeprom_addr,
                            uint8_t          nbytes,
                            uint8_t         *p_buf)
{
    uint8_t temp[260] = {0};
    temp[0] = eeprom_addr;
    temp[1] = nbytes;
    memcpy(&temp[2], p_buf, nbytes);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_DEVICE_CONTROL_CMD,
                                ZSN603_WRITE_EEPROM,
                                temp, nbytes + 2,
                                0, NULL, NULL);
}
/** \brief Mifare卡的请求 */
uint8_t zsn603_mifare_request(zsn603_handle_t  handle,
                              uint8_t          req_mode,
                              uint16_t        *p_atq)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_REQUEST,
                               &req_mode, 1,
                                1, (uint8_t *)p_atq, NULL);
}
/** \brief Mifare卡的防碰撞 */
uint8_t zsn603_mifare_anticoll(zsn603_handle_t  handle,
                               uint8_t          anticoll_level,
                               uint8_t         *p_know_uid,
                               uint8_t          nbit_cnt,
                               uint8_t         *p_uid,
                               uint32_t        *p_uid_cnt)
{
    uint8_t temp[10];
    temp[0] = anticoll_level;
    temp[1] = nbit_cnt;
    memcpy(&temp[2], p_know_uid, nbit_cnt);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_ANTICOLL,
                                temp, nbit_cnt + 2,
                                10, p_uid, p_uid_cnt);
}
/** \brief Mifare卡的选择*/
uint8_t zsn603_mifare_select(zsn603_handle_t  handle,
                             uint8_t          anticoll_level,
                             uint8_t         *p_uid,
                             uint8_t         *p_sak)
{
    uint8_t temp[11];
    temp[0] = anticoll_level;
    memcpy(&temp[1], p_uid, 4);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_SELECT,
                                temp, 5,
                                1, p_sak, NULL);
}
/** \brief Mifare卡挂起 */
uint8_t zsn603_mifare_halt(zsn603_handle_t     handle)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_HALT,
                                NULL, 0,
                                0, NULL, NULL);
}
/** \brief EEPROM密钥验证 */
uint8_t zsn603_eeprom_auth(zsn603_handle_t  handle,
                           uint8_t          key_type,
                           uint8_t         *p_uid,
                           uint8_t          key_sec,
                           uint8_t          nblock)
{
    uint8_t temp[7] = {0};
    temp[0] = key_type;
    memcpy( &temp[1], p_uid, 4);
    temp[5] = key_sec;
    temp[6] = nblock;

    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_AUTH_E2,
                                temp, 7,
                                0, NULL, NULL);
}
/** \brief 直接密钥验证 */
uint8_t zsn603_key_auth(zsn603_handle_t   handle,
                        uint8_t           key_type,
                        uint8_t          *p_uid,
                        uint8_t          *p_key,
                        uint8_t           key_len,
                        uint8_t           nblock)
{
    uint8_t temp[22] = {0};
    temp[0] = key_type;
    memcpy(&temp[1], p_uid, 4);
    memcpy(&temp[5], p_key, key_len);
    temp[5 + key_len] = nblock;

    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_AUTH_KEY,
                                temp, 6 + key_len,
                                0, NULL, NULL);
}

/** \brief Mifare卡读操作 */
uint8_t zsn603_mifare_read(zsn603_handle_t  handle,
                           uint8_t          nblock,
                           uint32_t         buffer_len,
                           uint8_t         *p_buf)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_READ,
                               &nblock, 1,
                                buffer_len, p_buf, NULL);
}
/** \brief Mifare卡写操作 */
uint8_t zsn603_mifare_write(zsn603_handle_t   handle,
                            uint8_t           nblock,
                            uint8_t          *p_buf)
{
    uint8_t temp[17];
    temp[0] = nblock;
    memcpy(&temp[1], p_buf, 16);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_WRITE,
                                temp, 17,
                                0, NULL, NULL);
}
/** \brief UltraLight卡写操作 */
uint8_t zsn603_ultralight_write(zsn603_handle_t   handle,
                                uint8_t           nblock,
                                uint8_t          *p_buf)
{
    uint8_t temp[5];
    temp[0] = nblock;
    memcpy(&temp[1], p_buf, 4);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE0_WRITE,
                                temp, 5,
                                0, NULL, NULL);
}

/** \brief Mifare卡的值块操作 */
uint8_t zsn603_mifare_value(zsn603_handle_t  handle,
                            uint8_t          mode,
                            uint8_t          nblock,
                            uint8_t          ntransblk,
                            uint32_t         value)
{
    uint8_t temp[7];
    temp[0] = mode;
    temp[1] = nblock;
    memcpy(&temp[2], (const void *)&value, 4);
    temp[6] = ntransblk;
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_VALUE,
                                temp, 7,
                                0, NULL, NULL);
}
/** \brief 卡片复位 */
uint8_t zsn603_card_reset(zsn603_handle_t  handle,
                          uint8_t          time_ms)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_RESET,
                               &time_ms, 1,
                                0, NULL, NULL);
}
/** \brief Mifare卡激活操作 */
uint8_t zsn603_mifare_card_active(zsn603_handle_t   handle,
                                  uint8_t           req_mode,
                                  uint16_t         *p_atq,
                                  uint8_t          *p_saq,
                                  uint8_t          *p_len,
                                  uint8_t          *p_uid)
{
    uint8_t ret = 0;
    uint8_t temp[2];
    uint8_t data[14];
    uint32_t  data_lenght;
    temp[0] = 0x00;
    temp[1] = req_mode;
    ret = __zsn603_cmd_execute(handle,
                               handle->local_addr,
                               ZSN603_MIFARE_CARD_CMD,
                               ZSN603_MIFARE_ACTIVE,
                               temp, 2,
                               14, data, &data_lenght);
    if(ret == 0){
        *p_saq  = data[2];
        *p_len  = data[3];
         memcpy(p_atq, data, 2);
         memcpy(p_uid, &data[4], *p_len);
    }
    return ret;
}
/** \brief 进入自动检测模式 */
uint8_t zsn603_auto_detect(zsn603_handle_t              handle,
                           zsn603_auto_detect_ctrl_t   *p_ctrl)
{
    uint8_t ret         = 0;
    uint8_t temp[22]    = {0};
    uint8_t temp_lenght =  0;
    temp[0] = p_ctrl->ad_mode;
    temp[1] = p_ctrl->tx_mode;
    temp[2] = p_ctrl->req_code;
    temp[3] = p_ctrl->auth_mode;
    if(p_ctrl->auth_mode == ZSN603_AUTO_DETECT_NO_AUTH){
        temp_lenght = 4;
    }else if(p_ctrl->auth_mode == ZSN603_AUTO_DETECT_E2_AUTH ||
            p_ctrl->auth_mode == ZSN603_AUTO_DETECT_KEY_AUTH){
        temp[4] = p_ctrl->key_type;
        if(p_ctrl->auth_mode == ZSN603_AUTO_DETECT_E2_AUTH){
             temp[5] = p_ctrl->key_num;
             temp[6] = p_ctrl->block;
             temp_lenght = 7;
        }else if(p_ctrl->auth_mode == ZSN603_AUTO_DETECT_KEY_AUTH){
            memcpy(&temp[5], p_ctrl->p_key, p_ctrl->key_len);
            temp[5 + p_ctrl->key_len] = p_ctrl->block;
            temp_lenght = 6 + p_ctrl->key_len;
        }
    }else{
        return ZSN603_INPUT_PARA_ERROR;
    }

    ret =  __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_AUTO_DETECT,
                                temp, temp_lenght,
                                0, NULL, NULL);

    if(ret == 0){
        handle->pfn_card_cb = p_ctrl->pfn_card_input;
        handle->p_arg       = p_ctrl->p_arg;

        if(handle->work_mode == ZSN603_WORK_MODE_IIC){
            handle->status = ZSN603_AUTO_MODE_INT;
        }else{
            if(p_ctrl->ad_mode & ZSN603_AUTO_DETECT_SEND){
                if(p_ctrl->ad_mode & ZSN603_AUTO_DETECT_INTERRUPT){
                    handle->status = ZSN603_AUTO_MODE_INT_MESSAGE;
                }else{
                    handle->status = ZSN603_AUTO_MODE_NO_INT;
                }
            }else if(p_ctrl->ad_mode & ZSN603_AUTO_DETECT_INTERRUPT){
                handle->status = ZSN603_AUTO_MODE_INT;
            }
        }
        handle->is_continue = (p_ctrl->ad_mode & ZSN603_AUTO_DETECT_CONTINUE) >> 2;
    }
    return ret;
}
/** \brief 获取自动检测数据模式 */
uint8_t zsn603_get_auto_detect(zsn603_handle_t             handle,
                               uint8_t                     ctrl_mode,
                               zsn603_auto_detect_data_t  *p_data)
{
    int8_t   ret        = -1;
    uint8_t  data[31]   = {0};
    uint32_t data_count =  0;

    if(ctrl_mode == handle->is_continue &&(
       handle->status == ZSN603_AUTO_MODE_INT_MESSAGE ||
       handle->status == ZSN603_AUTO_MODE_NO_INT)){

       ret = handle->pfn_cmd_rx_analysis((void *)handle, 31, data, &data_count);

    }else{
        handle->is_continue = ctrl_mode;
        ret = __zsn603_cmd_execute(handle,
                                   handle->local_addr,
                                   ZSN603_MIFARE_CARD_CMD,
                                   ZSN603_MIFARE_GET_AUTO_DETECT,
                                  &ctrl_mode, 1,
                                   31, data, &data_count);
    }

    if(ret == 0){
        p_data->ant_mode = data[0];
        p_data->sak      = data[3];
        p_data->id_len   = data[4];
        memcpy(&p_data->atq, &data[1], 2);
        memcpy(p_data->uid, &data[5], data[4]);
        p_data->data_len = data_count - 5 - data[4];
        memcpy(p_data->data, &data[5 + data[4]], p_data->data_len);
    }

    if(handle->is_continue == 0x00){
        handle->status = ZSN603_NORMAL_MODE;
    }

    return ret;
}
/** \brief 设置Mifare卡值块的值 */
uint8_t zsn603_mifare_set_value(zsn603_handle_t  handle,
                                uint8_t          block,
                                int              data)
{
    uint8_t temp[5] = {0};
    temp[0] = block;
    memcpy(&temp[1], (const void *)&data, 4);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_SET_VALUE,
                                temp, 5,
                                0, NULL, NULL);
}
/** \brief 获取Mifare卡值块的值 */
uint8_t zsn603_mifare_get_value(zsn603_handle_t  handle,
                                uint8_t          block,
                                int             *p_value)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_GET_VALUE,
                               &block, 1,
                               4, (uint8_t *)p_value, NULL);
}
/** \brief 获取Mifare卡数据交换 */
uint8_t zsn603_mifare_exchange_block(zsn603_dev_t  *handle,
                                     uint8_t       *p_data_buf,
                                     uint8_t        len,
                                     uint8_t        wtxm_crc,
                                     uint8_t        fwi,
                                     uint32_t       buffer_len,
                                     uint8_t       *p_rx_buf,
                                     uint32_t      *p_len)
{
    uint8_t temp[50] = {0};
    temp[0] = wtxm_crc;
    temp[1] = fwi;
    memcpy(p_data_buf, p_data_buf, len);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_EXCHANGE_BLOCK1,
                                temp ,len + 2 ,
                                buffer_len, p_rx_buf, p_len);
}
/** \brief 获取Mifare卡命令传输 */
uint8_t zsn603_mifare_cmd_trans(zsn603_handle_t   handle,
                                uint8_t          *p_tx_buf,
                                uint8_t           tx_nbytes,
                                uint32_t          buffer_len,
                                uint8_t          *p_rx_buf,
                                uint32_t         *p_rx_nbytes)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_MIFARE_CARD_CMD,
                                ZSN603_MIFARE_MF0_CMD_TRANS,
                                p_tx_buf,tx_nbytes,
                                buffer_len, p_rx_buf, p_rx_nbytes);
}
/** \brief 传输APDU数据流 */
uint8_t zsn603_cicc_tpdu(zsn603_handle_t   handle,
                         uint8_t          *p_tx_buf,
                         uint32_t          tx_bufsize,
                         uint32_t          buffer_len,
                         uint8_t          *p_rx_buf,
                         uint32_t         *p_rx_len)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO7816_PROTOCOL_CMD,
                                ZSN603_CICC_TPDU,
                                p_tx_buf, tx_bufsize,
                                buffer_len, p_rx_buf, p_rx_len);
}
/** \brief 冷复位 */
uint8_t zsn603_cicc_cold_reset(zsn603_handle_t   handle,
                               uint32_t          buffer_len,
                               uint8_t          *p_rx_buf,
                               uint32_t         *p_rx_len)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO7816_PROTOCOL_CMD,
                                ZSN603_CICC_COLD_RESET,
                                NULL, 0,
                                buffer_len, p_rx_buf, p_rx_len);
}
/** \brief 热复位 */
uint8_t zsn603_cicc_warm_reset(zsn603_handle_t    handle,
                               uint32_t           buffer_len,
                               uint8_t           *p_rx_buf,
                               uint32_t          *p_rx_len)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO7816_PROTOCOL_CMD,
                                ZSN603_CICC_WARM_RESET,
                                NULL, 0,
                                buffer_len, p_rx_buf, p_rx_len);
}
/** \brief 关闭接触式IC卡的电源和时钟 */
uint8_t zsn603_cicc_deactivation(zsn603_handle_t      handle)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO7816_PROTOCOL_CMD,
                                ZSN603_CICC_DEACTIVATION,
                                NULL, 0,
                                0, NULL, NULL);
}

/** \brief T = 0传输协议 */
uint8_t zsn603_cicc_tp0(zsn603_handle_t    handle,
                        uint8_t           *p_tx_buf,
                        uint32_t           tx_bufsize,
                        uint32_t           buffer_len,
                        uint8_t           *p_rx_buf,
                        uint32_t          *p_rx_len)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO7816_PROTOCOL_CMD,
                                ZSN603_CICC_TP0,
                                p_tx_buf, tx_bufsize,
                                buffer_len, p_rx_buf, p_rx_len);
}
/** \brief T = 1传输协议 */
uint8_t zsn603_cicc_tp1(zsn603_handle_t   handle,
                        uint8_t          *p_tx_buf,
                        uint32_t          tx_bufsize,
                        uint32_t          buffer_len,
                        uint8_t          *p_rx_buf,
                        uint32_t         *p_rx_len)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO7816_PROTOCOL_CMD,
                                ZSN603_CICC_TP1,
                                p_tx_buf, tx_bufsize,
                                buffer_len, p_rx_buf, p_rx_len);
}
/** \brief A型卡请求 */
uint8_t zsn603_picca_request(zsn603_handle_t   handle,
                             uint8_t           req_mode,
                             uint16_t         *p_atq)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCA_REQUEST,
                               &req_mode, 1,
                                2, (uint8_t *) p_atq, NULL);
}

/** \brief A型卡防碰撞 */
uint8_t zsn603_picca_anticoll(zsn603_handle_t   handle,
                              uint8_t           anticoll_level,
                              uint8_t          *p_know_uid,
                              uint8_t           nbit_cnt,
                              uint32_t          buffer_len,
                              uint8_t          *p_uid,
                              uint32_t         *p_uid_cnt)
{
    uint8_t temp[10];
    temp[0] = anticoll_level;
    temp[1] = nbit_cnt;
    memcpy(&temp[2], p_know_uid, nbit_cnt);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCA_ANTICOLL,
                                temp, nbit_cnt + 2,
                                buffer_len, p_uid, p_uid_cnt);
}
/** \brief A型卡选择 */
uint8_t zsn603_picca_select(zsn603_handle_t   handle,
                            uint8_t           anticoll_level,
                            uint8_t          *p_uid,
                            uint8_t           uid_cnt,
                            uint8_t          *p_sak)
{
    uint8_t temp[11];
    temp[0] = anticoll_level;
    memcpy(&temp[1], p_uid, uid_cnt);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCA_SELECT,
                                temp,
                                uid_cnt + 1,
                                1, p_sak, NULL);
}
/** \brief A型卡挂起 */
uint8_t zsn603_picca_halt(zsn603_handle_t      handle)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCA_HALT,
                                NULL, 0,
                                0, NULL, NULL);
}
/** \brief A型卡 RATS */
uint8_t zsn603_picca_rats(zsn603_handle_t   handle,
                          uint8_t           cid,
                          uint32_t          buffer_len,
                          uint8_t          *p_ats_buf,
                          uint32_t         *p_rx_len)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCA_RATS,
                               &cid, 1,
                                buffer_len, p_ats_buf, p_rx_len);
}
/** \brief A型卡 PPS */
uint8_t zsn603_picca_pps(zsn603_handle_t  handle,
                         uint8_t          dsi_dri)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCA_PPS,
                               &dsi_dri, 1,
                                0, NULL, NULL);
}
/** \brief A型卡取消选择 */
uint8_t zsn603_picca_deselect(zsn603_handle_t     handle)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCA_DESELECT,
                                NULL, 0,
                                0, NULL, NULL);
}
/** \brief A型卡T=CL */
uint8_t zsn603_picca_tpcl(zsn603_handle_t   handle,
                          uint8_t          *p_cos_buf,
                          uint8_t           cos_bufsize,
                          uint32_t          buffer_len,
                          uint8_t          *p_res_buf,
                          uint32_t         *p_rx_len)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCA_TPCL,
                                p_cos_buf,
                                cos_bufsize,
                                buffer_len,
                                p_res_buf,
                                p_rx_len);
}
/** \brief A型卡交换数据 */
uint8_t zsn603_picca_exchange_block(zsn603_handle_t   handle,
                                    uint8_t          *p_data_buf,
                                    uint8_t           len,
                                    uint8_t           wtxm_crc,
                                    uint8_t           fwi,
                                    uint32_t          buffer_len,
                                    uint8_t          *p_rx_buf,
                                    uint32_t         *p_rx_len)
{
    uint8_t temp[50];
    temp[0] = wtxm_crc;
    temp[1] = fwi;
    memcpy(&temp[2], p_data_buf, len);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCA_EXCHANGE_BLOCK2,
                                temp,
                                len + 2,
                                buffer_len, p_rx_buf, p_rx_len);
}
/** \brief A型卡复位 */
uint8_t zsn603_picca_reset(zsn603_handle_t  handle,
                           uint8_t          time_ms)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCA_RESET,
                               &time_ms, 1,
                                0, NULL, NULL);
}
/** \brief A型卡激活 */
uint8_t zsn603_picca_active(zsn603_handle_t   handle,
                            uint8_t           req_mode,
                            uint16_t         *p_atq,
                            uint8_t          *p_saq,
                            uint8_t          *p_len,
                            uint8_t          *p_uid)
{

    uint8_t ret = 0;
    uint8_t temp[2];
    uint8_t data[14];
    uint32_t  data_lenght;
    temp[0] = 0x00;
    temp[1] = req_mode;
    ret = __zsn603_cmd_execute(handle,
                               handle->local_addr,
                               ZSN603_ISO14443_PROTOCOL_CMD,
                               ZSN603_PICCA_ACTIVE,
                               temp, 2,
                               14, data, &data_lenght);
    if(ret == 0){
        *p_saq  = data[2];
        *p_len  = data[3];
         memcpy(p_atq, data, 2);
         memcpy(p_uid, &data[4], *p_len);
    }
    return ret;
}
/** \brief B型卡激活 */
uint8_t zsn603_piccb_active(zsn603_handle_t   handle,
                            uint8_t           req_mode,
                            uint8_t          *p_info)
{
    uint8_t ret        = 0;
    uint8_t tx_data[2] = {0};
    
    tx_data[0] = req_mode;
    tx_data[1] = 0x00;
    
    ret = zsn603_set_icc_reg(handle, 0x0d, 0x00);
    if(ret){
        return ret;
    }

    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCB_ACTIVE,
                                tx_data, 2,
                                12, p_info, NULL);
}
/** \brief B型卡复位 */
uint8_t zsn603_piccb_reset(zsn603_handle_t handle,
                           uint8_t         time_ms)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCB_RESET,
                               &time_ms, 1,
                                0, NULL, NULL);
}
/** \brief B型卡请求 */
uint8_t zsn603_piccb_request(zsn603_handle_t   handle,
                             uint8_t           req_mode,
                             uint8_t           slot_time,
                             uint32_t          buffer_len,
                             uint8_t          *p_uid)
{
    uint8_t temp[3];
    temp[0] = req_mode;
    temp[1] = 0x00;
    temp[2] = slot_time;
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCB_REQUEST,
                                temp,
                                3,
                                buffer_len, p_uid, NULL);
}
/** \brief B型卡修改传输属性(卡选择) */
uint8_t zsn603_piccb_attrib(zsn603_handle_t   handle,
                            uint8_t          *p_pupi,
                            uint8_t           cid,
                            uint8_t           protype)
{
    uint8_t temp[6];
    memcpy(&temp[0], p_pupi, 4);
    temp[4] = cid;
    temp[5] = protype;
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCB_ATTRIB,
                                temp, 6,
                                0, NULL, NULL);
}
/** \brief B型卡挂起 */
uint8_t zsn603_piccb_halt(zsn603_handle_t  handle,
                          uint8_t         *p_pupi)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCB_HALT,
                                p_pupi, 4,
                                0, NULL, NULL);
}
/** \brief B型卡获取ID */
uint8_t zsn603_piccb_getid(zsn603_handle_t   handle,
                           uint8_t           req_mode,
                           uint8_t          *p_uid)
{
    uint8_t temp[2];
    temp[0] = req_mode;
    temp[1] = 0x00;
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_ISO14443_PROTOCOL_CMD,
                                ZSN603_PICCB_GETUID,
                                temp, 2,
                                0, NULL, NULL);
}

/** \brief SL0更新个人化数据 */
uint8_t zsn603_plus_cpu_write_perso(zsn603_handle_t   handle,
                                    uint16_t          addr,
                                    uint8_t          *p_data)
{
    uint8_t temp[18];
    temp[0] = addr & 0xff;
    temp[1] = (addr >> 8) & 0xff;
    memcpy(&temp[2], p_data, 16);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_CPU_CARD_CMD,
                                ZSN603_SL0_WRITE_PERSO,
                                temp, 18,
                                0, NULL, NULL);
}

/** \brief SL0的PLUS CPU卡提交个人化数据 */
uint8_t zsn603_plus_cpu_commit_perso(zsn603_handle_t      handle)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_CPU_CARD_CMD,
                                ZSN603_SL0_COMMIT_PERSO,
                                NULL, 0,
                                0, NULL, NULL);
}
/** \brief SL3的PLUS CPU卡的首次密钥验证*/
uint8_t zsn603_plus_cpu_first_auth(zsn603_handle_t   handle,
                                   uint16_t          addr,
                                   uint8_t          *p_data)
{
    uint8_t temp[18];
    temp[0] = addr & 0xff;
    temp[1] = (addr >> 8) & 0xff;
    memcpy(&temp[2], p_data, 16);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_CPU_CARD_CMD,
                                ZSN603_SL3_FIRST_AUTH,
                                NULL, 0,
                                0, NULL, NULL);
}
/** \brief SL3 PLUS CPU卡的首次密钥验证   密钥来自芯片EEPROM*/
uint8_t zsn603_plus_cpu_first_auth_e2(zsn603_handle_t  handle,
                                      uint16_t         addr,
                                      uint8_t          key_block)
{
    uint8_t temp[3];
    temp[0] = addr & 0xff;
    temp[1] = (addr >> 8) & 0xff;
    temp[2] = key_block;
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_CPU_CARD_CMD,
                                ZSN603_SL3_FIRST_AUTH_E2,
                                temp, 3,
                                0, NULL, NULL);
}
/** \brief SL3 PLUS CPU卡的跟随密钥验证*/
uint8_t zsn603_plus_cpu_follow_auth(zsn603_handle_t   handle,
                                    uint16_t          addr,
                                    uint8_t          *p_data)
{
    uint8_t temp[18];
    temp[0] = addr & 0xff;
    temp[1] = (addr >> 8) & 0xff;
    memcpy(&temp[2], p_data, 16);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_CPU_CARD_CMD,
                                ZSN603_SL3_FOLLOWING_AUTH,
                                NULL, 0,
                                0, NULL, NULL);
}
/** \brief SL3 PLUS CPU卡的跟随密钥验证    密钥来自芯片EEPROM*/
uint8_t zsn603_plus_cpu_follow_auth_e2(zsn603_handle_t  handle,
                                       uint16_t         addr,
                                       uint8_t          key_block)
{
    uint8_t temp[3];
    temp[0] = addr & 0xff;
    temp[1] = (addr >> 8) & 0xff;
    temp[2] = key_block;
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_CPU_CARD_CMD,
                                ZSN603_SL3_FOLLOWING_AUTH_E2,
                                temp, 3,
                                0, NULL, NULL);
}

/** \brief SL3 PLUS CPU卡的复位验证*/
uint8_t zsn603_plus_cpu_sl3_reset_auth(zsn603_handle_t  handle)
{
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_CPU_CARD_CMD,
                                ZSN603_SL3_RESET_AUTH,
                                NULL, 0,
                                0, NULL, NULL);
}
/** \brief SL3 PLUS CPU卡 读取数据块*/
uint8_t zsn603_plus_cpu_sl3_read(zsn603_handle_t   handle,
                                 uint8_t           read_mode,
                                 uint16_t          start_addr,
                                 uint8_t           block_num,
                                 uint32_t          buffer_len,
                                 uint8_t          *p_rx_data,
                                 uint32_t         *p_rx_lenght)
{
    uint8_t temp[4];
    temp[0] = read_mode;
    temp[1] = start_addr & 0xff;
    temp[2] = (start_addr >> 8) & 0xff;
    temp[3] = block_num;
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_CPU_CARD_CMD,
                                ZSN603_SL3_READ,
                                temp, 4,
                                buffer_len, p_rx_data, p_rx_lenght);
}

/** \brief SL3 PLUS CPU卡 写数据块*/
uint8_t zsn603_plus_cpu_sl3_write(zsn603_handle_t   handle,
                                  uint8_t           write_mode,
                                  uint16_t          start_addr,
                                  uint8_t           block_num,
                                  uint8_t          *p_tx_data,
                                  uint8_t           tx_lenght)

{
    uint8_t temp[52];
    temp[0] = write_mode;
    temp[1] = start_addr & 0xff;
    temp[2] = (start_addr >> 8) & 0xff;
    temp[3] = block_num;
    memcpy(&temp[4], p_tx_data, tx_lenght);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_CPU_CARD_CMD,
                                ZSN603_SL3_WRITE,
                                temp, 16 * block_num + 4,
                                0, NULL, NULL);

}

/** \brief SL3 PLUS CPU卡 值操作*/
uint8_t zsn603_plus_cpu_sl3_value_opr(zsn603_handle_t  handle,
                                      uint8_t          write_mode,
                                      uint16_t         src_addr,
                                      uint16_t         dst_addr,
                                      int              data)
{
    uint8_t temp[9];

    temp[0] = write_mode;
    temp[1] = src_addr & 0xff;
    temp[2] = (src_addr >> 8) & 0xff;
    temp[3] = dst_addr & 0xff;
    temp[4] = (dst_addr >> 8) & 0xff;
    memcpy(&temp[5], &data, 4);
    return __zsn603_cmd_execute(handle,
                                handle->local_addr,
                                ZSN603_CPU_CARD_CMD,
                                ZSN603_SL3_VALUE,
                                temp, 9,
                                0, NULL, NULL);

}
