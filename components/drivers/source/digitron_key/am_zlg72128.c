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
 * \brief ZLG72128 driver implement
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-22  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_zlg72128.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_int.h"

/*******************************************************************************
    Sub register defines
*******************************************************************************/
#define __ZLG72128_REGADDR_SYSTEM        0x00
#define __ZLG72128_REGADDR_KEY           0x01
#define __ZLG72128_REGADDR_REPEATCNT     0x02
#define __ZLG72128_REGADDR_FUNCTION_KEY  0x03

#define __ZLG72128_REGADDR_CMDBUF0       0x07
#define __ZLG72128_REGADDR_CMDBUF1       0x08

#define __ZLG72128_REGADDR_FLASH_TIME    0x0B

#define __ZLG72128_REGADDR_DISP_CTRL0    0x0C
#define __ZLG72128_REGADDR_DISP_CTRL1    0x0D

#define __ZLG72128_REGADDR_FLASH_CTRL0   0x0E
#define __ZLG72128_REGADDR_FLASH_CTRL1   0x0F

#define __ZLG72128_REGADDR_DISPBUF0      0x10
#define __ZLG72128_REGADDR_DISPBUF1      0x11
#define __ZLG72128_REGADDR_DISPBUF(n)   (0x10 + (n))

/*******************************************************************************
    cmd defines(for cmdbuf0(0x07))
*******************************************************************************/

/* seg off */
#define __ZLG72128_DIGITRON_CMD_SEGOFF           0x10

/* seg on  */
#define __ZLG72128_DIGITRON_CMD_SEGON            0x11

/* download the char and decode */
#define __ZLG72128_DIGITRON_CMD_DOWNLOAD(pos)   (0x20 + ((pos) & 0xF))

/* rest cmd */
#define __ZLG72128_DIGITRON_CMD_RESET            0x30

/* test cmd */
#define __ZLG72128_DIGITRON_CMD_TEST             0x40

/* shift cmd */
#define __ZLG72128_DIGITRON_CMD_SHIFT(is_left, is_cyclic, n) \
    (((is_left) ? 0x50 : 0x70) + ((is_cyclic) ? 0x10 : 0x00) + ((n) & 0x0F))

/*******************************************************************************
    fixed options defines
*******************************************************************************/

/* The max number of the digitron */
#define __ZLG72128_DIGITRON_NUM_MAX              12

/* The 7-bit address of the ZLG72128 */
#define __ZLG72128_I2C_ADDR                      0x30

/* The state of the ZLG72128 */
#define __ZLG72128_STATE_IDLE                    0
#define __ZLG72128_STATE_WAIT_READ_COMPLETE      1

/*******************************************************************************
    Local Functions Declare
*******************************************************************************/

static void __zlg72128_key_process (am_zlg72128_dev_t *p_dev);

/******************************************************************************/


/*******************************************************************************
    Local Functions
*******************************************************************************/

static int __zlg72128_data_write (am_zlg72128_dev_t *p_dev,
                                  uint8_t            regaddr,
                                  uint8_t           *p_buf,
                                  size_t             len)
{
    return am_i2c_write(&(p_dev->i2c_dev),
                        regaddr,
                        p_buf,
                        len);
}

/******************************************************************************/
static void __zlg72128_data_read (am_zlg72128_dev_t *p_dev)
{
    if (p_dev->state == __ZLG72128_STATE_IDLE) {
        p_dev->state = __ZLG72128_STATE_WAIT_READ_COMPLETE;
        am_i2c_msg_start(p_dev->handle, &(p_dev->msg));
    }
}

/******************************************************************************/
static void __zlg72128_trans_complete (void *p_arg)
{
    am_zlg72128_dev_t *p_dev = (am_zlg72128_dev_t *)p_arg;
    
    if (p_dev->msg.status == AM_OK) {
        __zlg72128_key_process(p_dev);
    }
    
    p_dev->state = __ZLG72128_STATE_IDLE;
    
    if (p_dev->p_devinfo->use_int_pin) {
        /* enable the interrupt */
        am_gpio_trigger_on(p_dev->p_devinfo->int_pin);

        /*若中断引脚已经是低电平，则表明已有按键按下*/
        if (am_gpio_get(p_dev->p_devinfo->int_pin) == 0) {

           /*读取键值*/
            __zlg72128_data_read(p_dev);
        }
    }
}

/******************************************************************************/
static void __zlg72128_int_callback (void *p_arg)
{
    am_zlg72128_dev_t *p_dev = (am_zlg72128_dev_t *)p_arg;
    
    if (p_dev->p_devinfo->use_int_pin) {
        /* disable the interrupt */
        am_gpio_trigger_off(p_dev->p_devinfo->int_pin);
    }
    
    __zlg72128_data_read(p_dev);
}

/******************************************************************************/
static void __zlg72128_key_process (am_zlg72128_dev_t *p_dev)
{
    uint8_t *p_buf = p_dev->key_buf;
    
    if ((p_buf[0] & 0x01) != 0) {                  /* 存在按键按下   */
        
        if (p_dev->pfn_key_cb) {
            
            p_dev->pfn_key_cb(p_dev->pfn_key_arg, p_buf[1], p_buf[2], p_buf[3]);
        }
    }
}

/******************************************************************************/
am_zlg72128_handle_t am_zlg72128_init (am_zlg72128_dev_t           *p_dev,
                                       const am_zlg72128_devinfo_t *p_devinfo,
                                       am_i2c_handle_t              i2c_handle)
{
    static const uint8_t reg_addr = 0x00;    /* 始终为0 */ 
    
    if ((p_dev == NULL) || (p_devinfo == NULL) || (i2c_handle == NULL)) {
        return NULL;
    }
 
    p_dev->pfn_key_arg    = NULL;
    p_dev->pfn_key_cb     = NULL;
    
    p_dev->p_devinfo      = p_devinfo;
    p_dev->state          = __ZLG72128_STATE_IDLE;
    p_dev->handle         = i2c_handle;

    if (p_devinfo->rst_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->rst_pin, AM_GPIO_OUTPUT_INIT_LOW);
        am_mdelay(1);
        am_gpio_set(p_devinfo->rst_pin, 1);
        am_mdelay(5);
    }

    am_i2c_mkdev(&(p_dev->i2c_dev), 
                   i2c_handle, 
                   __ZLG72128_I2C_ADDR, 
                   AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* 传输寄存器地址 */
    am_i2c_mktrans(&(p_dev->trans[0]),
                     __ZLG72128_I2C_ADDR,
                     AM_I2C_ADDR_7BIT | AM_I2C_M_WR,
                     (uint8_t *)&reg_addr,
                     1);
    
    /* 读取连续3个键值寄存器的值 */
    am_i2c_mktrans(&(p_dev->trans[1]),
                     __ZLG72128_I2C_ADDR,
                     AM_I2C_ADDR_7BIT | AM_I2C_M_RD,
                     p_dev->key_buf,
                     4);
                     
    /* 制作好msg,后续直接传输即可，避免重复设置msg */
    am_i2c_mkmsg(&(p_dev->msg),
                 &(p_dev->trans[0]),
                 2,
                 __zlg72128_trans_complete,
                 p_dev);
    
    
    if (p_devinfo->use_int_pin == AM_TRUE) {
        
        am_gpio_pin_cfg(p_devinfo->int_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        
        /* 连接GPIO引脚中断 */
        if (am_gpio_trigger_connect(p_devinfo->int_pin, 
                                    __zlg72128_int_callback, 
                                    p_dev) != AM_OK) {
            return NULL;
        }
        
        /* 设置中断引脚低电平触发 */
        if (am_gpio_trigger_cfg(p_devinfo->int_pin,
                                AM_GPIO_TRIGGER_LOW) != AM_OK) {

            /*设置低电平触发失败，则设置为下降沿触发*/

            /*设置为下降沿触发*/
            if (am_gpio_trigger_cfg(p_devinfo->int_pin,
                                    AM_GPIO_TRIGGER_FALL) != AM_OK) {
                return NULL;
            }
            /*设置为下降沿触发成功，开启触发*/
            am_gpio_trigger_on(p_devinfo->int_pin);

            /*若中断引脚已经是低电平，则表明已有按键按下*/
            if (am_gpio_get(p_devinfo->int_pin) == 0) {

                /*读取键值*/
                __zlg72128_data_read(p_dev);
            }
        } else {
            /*设置低电平触发成功，开启触发*/
            am_gpio_trigger_on(p_devinfo->int_pin);
        }
 
    } else {                          /* 使用软件定时器以一定的时间间隔扫描 */
        
        /* 中断处理函数与引脚中断是一样的 */
        am_softimer_init(&(p_dev->timer), __zlg72128_int_callback, p_dev);
        am_softimer_start(&(p_dev->timer), p_devinfo->interval_ms);
    }

    return p_dev;
}

/******************************************************************************/
int am_zlg72128_key_cb_set (am_zlg72128_handle_t  handle, 
                            am_zlg72128_key_cb_t  pfn_key_cb,
                            void                 *p_arg)
{
    uint32_t key;
    
    if (pfn_key_cb != NULL && handle != NULL) {
        
        key = am_int_cpu_lock();
        
        handle->pfn_key_cb  = pfn_key_cb;
        handle->pfn_key_arg = p_arg;
        
        am_int_cpu_unlock(key);
        
        return AM_OK;
    }
 
    return -AM_EINVAL;
}

/******************************************************************************/
int am_zlg72128_digitron_flash_time_cfg (am_zlg72128_handle_t  handle,
                                         uint8_t               on_ms,
                                         uint8_t               off_ms)
{
    uint8_t on_n, off_n;
    uint8_t ctrl;
    
    if (handle == NULL) {
        return -AM_EINVAL;
    }
    
    if (on_ms <= 150) {
        on_n = 0;
    } else {
        on_n = ((on_ms - 150) + (50 - 1)) / 50;
    }

    if (off_ms <= 150) {
        off_n = 0;
    } else {
        off_n = ((off_ms - 150) + (50 - 1)) / 50;
    }
    
    on_n  &= 0xF;
    off_n &= 0xF;
    
    ctrl = (on_n << 4) | off_n;
    
    return __zlg72128_data_write(handle,
                                __ZLG72128_REGADDR_FLASH_TIME,
                                &ctrl,
                                1);
}

/******************************************************************************/
int am_zlg72128_digitron_flash_ctrl (am_zlg72128_handle_t  handle,
                                     uint16_t              ctrl_val)
{
    uint8_t ctrl[2];
    
    if (handle == NULL) {
        return -AM_EINVAL;
    }
    
    /* 低字节控制的高8位，高字节控制的低8位 */
    ctrl[0] = (ctrl_val >> 8) & 0xF;      /* 高8位后发送 */
    ctrl[1] =  ctrl_val & 0xFF;           /* 低8位后发送 */ 
    
    return __zlg72128_data_write(handle,
                                 __ZLG72128_REGADDR_FLASH_CTRL0,
                                 ctrl,
                                 2);
}

/******************************************************************************/
int am_zlg72128_digitron_disp_ctrl (am_zlg72128_handle_t  handle, 
                                    uint16_t              ctrl_val)
{
    uint8_t ctrl[2];
    
    if (handle == NULL) {
        return -AM_EINVAL;
    }
    
    /* 低字节控制的高8位，高字节控制的低8位 */
    ctrl[0] = (ctrl_val >> 8) & 0xF;      /* 高8位后发送 */
    ctrl[1] =  ctrl_val & 0xFF;           /* 低8位后发送 */ 
    
    return __zlg72128_data_write(handle,
                                 __ZLG72128_REGADDR_DISP_CTRL0,
                                 ctrl,
                                 2);
}

/******************************************************************************/
int am_zlg72128_digitron_dispbuf_set (am_zlg72128_handle_t  handle, 
                                      uint8_t               start_pos,
                                      uint8_t              *p_buf,
                                      uint8_t               num)
{
    if (start_pos >= __ZLG72128_DIGITRON_NUM_MAX || handle == NULL) {
        return -AM_EINVAL;
    }
    
    if ((start_pos + num) > 12) {
        num = (start_pos + num) - 12;
    }
    
    return __zlg72128_data_write(handle,
                                 __ZLG72128_REGADDR_DISPBUF(start_pos),
                                 p_buf,
                                 num);
}

/******************************************************************************/
int am_zlg72128_digitron_dispbuf_get (am_zlg72128_handle_t  handle,
                                      uint8_t               start_pos,
                                      uint8_t              *p_buf,
                                      uint8_t               num)
{
    if (start_pos >= __ZLG72128_DIGITRON_NUM_MAX || handle == NULL) {
        return -AM_EINVAL;
    }

    if ((start_pos + num) > 12) {
        num = (start_pos + num) - 12;
    }

    return am_i2c_read(&(handle->i2c_dev),
                        __ZLG72128_REGADDR_DISPBUF(start_pos),
                        p_buf,
                        num);
}

/******************************************************************************/
int am_zlg72128_digitron_disp_char (am_zlg72128_handle_t  handle, 
                                    uint8_t               pos,
                                    char                  ch,
                                    am_bool_t             is_dp_disp,
                                    am_bool_t             is_flash)
                                    
{
    static const char code[32] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'b', 'C', 'd', 'E', 'F',
        'G', 'H', 'i', 'J', 'L', 'o', 'p', 'q',
        'r', 't', 'U', 'y', 'c', 'h', 'T', ' ',
    };

    uint8_t i;
    uint8_t cmd[2];
    
    if (handle == NULL || pos >= __ZLG72128_DIGITRON_NUM_MAX) {
        return -AM_EINVAL;
    }
    
    for (i = 0; i < sizeof(code); i++) {
        
        if (code[i] == ch) {
            break;
        }
    }
    
    if (i == sizeof(code)) {
        return -AM_ENOTSUP;
    }
    
    cmd[0] = __ZLG72128_DIGITRON_CMD_DOWNLOAD(pos);
    cmd[1] = i;
    
    if (is_dp_disp == AM_TRUE) {
        cmd[1] |= (1 << 7);
    }
    
    if (is_flash == AM_TRUE) {
        cmd[1] |= (1 << 6);
    }
 
    return __zlg72128_data_write(handle,
                                __ZLG72128_REGADDR_CMDBUF0,
                                cmd,
                                2);
}

/******************************************************************************/
int am_zlg72128_digitron_disp_str (am_zlg72128_handle_t  handle, 
                                   uint8_t               start_pos,
                                   const char           *p_str)
{
    int   ret     = AM_OK;

    if (handle == NULL || p_str == NULL) {
        return -AM_EINVAL;
    }
    
    if (start_pos >= __ZLG72128_DIGITRON_NUM_MAX) {
        return -AM_EINVAL;
    }        
    
    while (*p_str != '\0' && start_pos < __ZLG72128_DIGITRON_NUM_MAX) {
        
        /* independent '.' */
        if (*p_str == '.') {
            
            /* dsiplay '.' */
            ret = am_zlg72128_digitron_disp_char(handle,
                                                 start_pos, 
                                                 ' ',    
                                                 AM_TRUE,
                                                 AM_FALSE);
        } else {
            
            /* The next char is '.', just display at here */
            if (*(p_str + 1) == '.') {
            
                /* The '.' display at here */
                ret = am_zlg72128_digitron_disp_char(handle,
                                                     start_pos, 
                                                    *p_str, 
                                                     AM_TRUE,
                                                     AM_FALSE);
                
                /* skip the '.', the pos needn't increase */
                p_str++;
                
            } else {
                
                ret = am_zlg72128_digitron_disp_char(handle,
                                                     start_pos, 
                                                    *p_str, 
                                                     AM_FALSE,
                                                     AM_FALSE);
            }
            
        }
 
        if (ret != AM_OK) {
            return ret;
        }
        
        start_pos++;
        p_str++;
    }
    
    return ret;
}

/******************************************************************************/
int am_zlg72128_digitron_disp_num (am_zlg72128_handle_t  handle, 
                                   uint8_t               pos,
                                   uint8_t               num,
                                   am_bool_t             is_dp_disp,
                                   am_bool_t             is_flash)
{
    uint8_t cmd[2];
    
    if (num >= 10 || handle == NULL || pos >= __ZLG72128_DIGITRON_NUM_MAX) {
        return -AM_EINVAL;
    }
    
    cmd[0] = __ZLG72128_DIGITRON_CMD_DOWNLOAD(pos);
    cmd[1] = num;
    
    if (is_dp_disp == AM_TRUE) {
        cmd[1] |= (1 << 7);
    }
    
    if (is_flash == AM_TRUE) {
        cmd[1] |= (1 << 6);
    }
 
    return __zlg72128_data_write(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 cmd,
                                 2);
}

/******************************************************************************/
int am_zlg72128_digitron_seg_ctrl (am_zlg72128_handle_t  handle, 
                                   uint8_t               pos,
                                   char                  seg,
                                   am_bool_t             is_on)
{
    uint8_t cmd[2];
    
    if (handle == NULL || pos >= __ZLG72128_DIGITRON_NUM_MAX) {
        return -AM_EINVAL;
    }
    
    if (is_on == AM_TRUE) {
        cmd[0] = __ZLG72128_DIGITRON_CMD_SEGON;
    } else {
        cmd[0] = __ZLG72128_DIGITRON_CMD_SEGOFF;
    }
    
    cmd[1] = ((pos & 0x0F) << 4) | (seg);
    
    return __zlg72128_data_write(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 cmd,
                                 2);
}

/******************************************************************************/
int am_zlg72128_digitron_shift (am_zlg72128_handle_t  handle, 
                                uint8_t               dir,
                                am_bool_t             is_cyclic,
                                uint8_t               num)
{
    uint8_t cmd;
    
    if (handle == NULL || num >= __ZLG72128_DIGITRON_NUM_MAX) {
        return -AM_EINVAL;
    }
    
    /* 低4位为移位的位数, dir (0-right，1-left) */
    cmd = __ZLG72128_DIGITRON_CMD_SHIFT(dir, is_cyclic, num);
 
    return __zlg72128_data_write(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 &cmd,
                                 1);
    
}

/******************************************************************************/
int am_zlg72128_digitron_disp_reset (am_zlg72128_handle_t  handle)
{
    uint8_t cmd;
    
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    cmd = __ZLG72128_DIGITRON_CMD_RESET;
    
    return __zlg72128_data_write(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 &cmd,
                                 1);
}

/******************************************************************************/
int am_zlg72128_digitron_disp_test (am_zlg72128_handle_t handle)
{
    uint8_t cmd;
    
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    cmd = __ZLG72128_DIGITRON_CMD_TEST;
    
    return __zlg72128_data_write(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 &cmd,
                                 1);
}

/* end of file */
