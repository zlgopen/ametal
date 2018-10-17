/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief LPS22HB驱动文件
 * \sa am_lsp22hb.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */
 
#include "am_lps22hb.h" 
 
 
/**
 * \brief LPS22HB从机地址
 */    
#define LPS22HB_DEVICE_ADDR        0x5C  
 
/**
 * \brief LPS22HB寄存器地址
 */
#define   LPS22HB_INTERRUPT_CFG    0X0B     
#define   LPS22HB_THS_P_L          0X0C
#define   LPS22HB_THS_P_H          0X0D 
#define   LPS22HB_WHO_AM_I         0X0F
#define   LPS22HB_CTRL_REG1        0X10
#define   LPS22HB_CTRL_REG2        0X11
#define   LPS22HB_CTRL_REG3        0X12
#define   LPS22HB_FIFO_CTRL        0X14
#define   LPS22HB_REF_P_XL         0X15
#define   LPS22HB_REF_P_L          0X16
#define   LPS22HB_REF_P_H          0X17
#define   LPS22HB_RPDS_L           0X18
#define   LPS22HB_RPDS_H           0X19                                         
#define   LPS22HB_RES_CONF         0X1A
#define   LPS22HB_INT_SOURCE       0X25
#define   LPS22HB_FIFO_STATUS      0X26
#define   LPS22HB_STATUS           0X27
#define   LPS22HB_PRESS_OUT_XL     0X28
#define   LPS22HB_PRESS_OUT_L      0X29
#define   LPS22HB_PRESS_OUT_H      0X2A
#define   LPS22HB_TEMP_OUT_L       0X2B
#define   LPS22HB_TEMP_OUT_H       0X2C
#define   LPS22HB_LPFP_RES         0X33

/**
 * \brief LPS22HB压力、温度状态标志位
 */ 
#define   STATUS_P_DA     (1 << 0)   
#define   STATUS_T_DA     (1 << 1)

/**
 * \brief LPS22HB复位指令
 */ 
#define   LPS22HB_RESET            0x04


/**
 * \brief 初始化LPS22HB
 */
am_lps22hb_handle_t am_lsp22hb_init (am_lps22hb_dev_t *p_dev, 
                                     const am_lps22hb_devinfo_t *p_devinfo, 
                                     am_i2c_handle_t i2c_handle)  
{   
    uint8_t       lps22hb_id = 0; 
    uint8_t       ctrl_reg1  = 0x00; 
    const uint8_t ctrl_reg2  = LPS22HB_RESET;

    /* 验证参数有效性 */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }    

    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* 初始配置LPS22HB备信息 */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                i2c_handle,
                LPS22HB_DEVICE_ADDR,
                AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
    /* 复位LPS22HB */  
    am_i2c_write(&(p_dev->i2c_dev), LPS22HB_CTRL_REG2, &ctrl_reg2, 1);
    am_mdelay(10); 

    /* 读取LPS22HB ID */ 
    am_i2c_read(&(p_dev->i2c_dev), LPS22HB_WHO_AM_I, &lps22hb_id, 1);
    AM_DBG_INFO("LPS22HB_ID = 0x%x \r\n",lps22hb_id); 

    /* 配置气压、温度数据输出频率、低通滤波 */
    ctrl_reg1 = (p_devinfo->output_data_rate 
              | p_devinfo->enable_lpfp 
              | p_devinfo->lpfp_bw);
    am_i2c_write(&(p_dev->i2c_dev), LPS22HB_CTRL_REG1, &ctrl_reg1, 1);
    
    return p_dev;
}

/**
 * \brief 读取温度寄存器
 */
int am_lsp22hb_read_temp (am_lps22hb_handle_t handle, int16_t *temp)     
{
    uint8_t status    = 0;
    uint8_t temp_low  = 0;
    uint8_t temp_high = 0;
    int     ret       = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);  
    
    /* 读取状态寄存器 */ 
    ret = am_i2c_read(p_i2c_dev, LPS22HB_STATUS, &status, 1);
    
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  
    
    if (status & STATUS_T_DA) {
        
        ret = am_i2c_read(p_i2c_dev, LPS22HB_TEMP_OUT_L, &temp_low, 1);
        /* 检查发送是否失败 */
        if (ret != AM_OK) {
            return ret;
        }
        
        ret = am_i2c_read(p_i2c_dev, LPS22HB_TEMP_OUT_H, &temp_high, 1);
        /* 检查发送是否失败 */
        if (ret != AM_OK) {
            return ret;
        }   
    }
    
    *temp = (int16_t)(((uint16_t)(temp_high) << 8) 
          | ((uint16_t)(temp_low))); 
    
    *temp = (double)(*temp) / 100.0;
    
    return  ret;
}

/**
 * \brief 读取压力寄存器
 */
int am_lsp22hb_read_press (am_lps22hb_handle_t handle, uint32_t *press)    
{
    uint8_t   status    = 0;
    uint8_t   press_xl  = 0;
    uint8_t   press_l   = 0;
    uint8_t   press_h   = 0;
    int       ret       = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev); 
    
    /* 读取状态寄存器 */ 
    ret = am_i2c_read(p_i2c_dev, LPS22HB_STATUS, &status, 1);
    
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* 压力转换完成 */
    if (status & STATUS_P_DA) {
        am_i2c_read(p_i2c_dev, LPS22HB_PRESS_OUT_XL, &press_xl, 1);
        am_i2c_read(p_i2c_dev, LPS22HB_PRESS_OUT_L, &press_l, 1);  
        am_i2c_read(p_i2c_dev, LPS22HB_PRESS_OUT_H, &press_h, 1);  
    }
    /* 转换成实际压力值 */
    *press = (uint32_t)(((uint32_t)(press_h) << 16) 
           | ((uint32_t)(press_l) << 8) | ((uint32_t)(press_xl))); 
    *press = (double)(*press) / 4096.0; 
    
    return  ret;
}

/* end of file */ 

