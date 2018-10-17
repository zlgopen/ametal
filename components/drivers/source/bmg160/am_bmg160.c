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
 * \brief  BMG160三轴陀螺仪传感器应用接口实现
 *
 * \internal
 * \par Modification History
 * - 1.00 2018-09-11  wk, first implementation.
 * \endinternal
 */

#include "am_bmg160.h"


#define BMG160_ADDR          0x68   /**< \brief i2c地址 */

#define BMG160_CHIP_ID       0x00   /**< \brief 包含芯片编码 */
                              
#define BMG160_RATE_X_LSB    0x02   /**< \brief 包含X通道角速率读数值的最低有效位 */
#define BMG160_RATE_X_MSB    0x03   /**< \brief 包含X通道角速率读数值的最高有效位 */
#define BMG160_RATE_Y_LSB    0x04   /**< \brief 包含Y通道角速率读数值的最低有效位 */
#define BMG160_RATE_Y_MSB    0x05   /**< \brief 包含Y通道角速率读数值的最高有效位 */
#define BMG160_RATE_Z_LSB    0x06   /**< \brief 包含Z通道角速率读数值的最低有效位 */
#define BMG160_RATE_Z_MSB    0x07   /**< \brief 包含Z通道角速率读数值的最高有效位 */
#define BMG160_TEMP          0x08   /**< \brief 包含当前芯片温度 */
#define BMG160_INT_STATUS_0  0x09   /**< \brief 包含中断状态位 */
#define BMG160_INT_STATUS_1  0x0A   /**< \brief 包含中断状态位 */
#define BMG160_INT_STATUS_2  0x0B   /**< \brief 包含任何运动中断状态位 */
#define BMG160_INT_STATUS_3  0x0C   /**< \brief 包含高速率中断状态位 */
                             
#define BMG160_FIFO_STATUS   0x0E   /**< \brief 包含FIFO状态标志 */
#define BMG160_RANGE         0x0F   /**< \brief 包含BMG160支持四种不同的角速率测量范围 */
#define BMG160_BW            0x10   /**< \brief 该寄存器允许选择速率数据滤波器带宽 */
#define BMG160_LPM1          0x11   /**< \brief 选择主电源模式 */
#define BMG160_LPM2          0x12   /**< \brief 快速上电和外部触发的配置设置 */
#define BMG160_RATE_HBW      0x13   /**< \brief 角速率数据采集和数据输出格式 */
#define BMG160_BGW_SOFTRESET 0x14	  /**< \brief 控制用户触发的传感器复位 */ 
#define BMG160_INT_EN_0      0x15   /**< \brief 控制启用哪些中断 */
#define BMG160_INT_EN_1      0x16   /**< \brief 包含中断引脚配置 */
#define BMG160_INT_MAP_0     0x17   /**< \brief 控制哪些中断信号映射到INT1引脚 */
#define BMG160_INT_MAP_1     0x18   /**< \brief 控制哪些中断信号映射到INT1引脚和INT2引脚 */
#define BMG160_INT_MAP_2     0x19   /**< \brief 控制哪些中断信号映射到INT2引脚 */
                              
#define BMG160_INT_RST_LATCH 0x21   /**< \brief 包含中断复位位和中断模式选择 */
#define BMG160_High_Th_x     0x22   /**< \brief 包含x轴的高速率阈值和高速率滞后设置 */
#define BMG160_High_Dur_x    0x23   /**< \brief 包含x轴的高速率持续时间设置 */
#define BMG160_High_Th_y     0x24   /**< \brief 包含y轴的高速率阈值和高速率滞后设置 */
#define BMG160_High_Dur_y    0x25   /**< \brief 包含y轴的高速率持续时间设置 */
#define BMG160_High_Th_z     0x26   /**< \brief 包含z轴的高速率阈值和高速率滞后设置 */
#define BMG160_High_Dur_z    0x27   /**< \brief 包含z轴的高速率持续时间设置 */
                              
#define BMG160_SoC           0x31   /**< \brief 包含慢速偏移取消设置 */
#define BMG160_A_FOC         0x32   /**< \brief 包含快速偏移取消设置 */
#define BMG160_TRIM_NVM_CTRL 0x33   /**< \brief 包含几次可编程非易失性存储器（NVM）的控制设置 */
#define BMG160_BGW_SPI3_WDT  0x34   /**< \brief 包含数字接口的设置 */
                              
#define BMG160_OFC1          0x36   /**< \brief 包含偏移补偿值 */
#define BMG160_OFC2          0x37   /**< \brief 包含X通道的偏移补偿值 */
#define BMG160_OFC3          0x38   /**< \brief 包含Y通道的偏移补偿值 */
#define BMG160_OFC4          0x39   /**< \brief 包含Z通道的偏移补偿值 */
#define BMG160_TRIM_GP0      0x3A   /**< \brief 包含具有NVM备份的通用数据寄存器 */
#define BMG160_TRIM_GP1      0x3B   /**< \brief 包含具有NVM备份的通用数据寄存器 */
#define BMG160_BIST          0x3C   /**< \brief 包含内置自测（BIST）功能 */
#define BMG160_FIFO_CONFIG_0 0x3D   /**< \brief 包含FIFOwatermark级别 */
#define BMG160_FIFO_CONFIG_1 0x3E   /**< \brief 包含FIFO配置设置 */
#define BMG160_FIFO_DATA     0x3F   /**< \brief FIFO数据读出寄存器 */


/**
 * \brief 读取BMG160三轴陀螺仪寄存器
 * \return AM_OK:成功    OTHER：失败
 */
static int __bmg160_read_rate_reg(am_bmg160_handle_t handle, 
                                  uint8_t            reg_addr,
                                  int16_t            *p_rate)
{
    uint8_t rate_temp[2];
    int     ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);
    
    ret =  am_i2c_read(p_i2c_dev, reg_addr, rate_temp, 2);
    
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    
    /* 数据处理 */
    if (rate_temp[0] & 0x01) {
        *p_rate = (((uint16_t)(rate_temp[1] << 8)
		          | (uint16_t)rate_temp[0])*360/65535);			
    }
		
    return ret;
}
 
 /**
 * \brief 读取bmg160三轴陀螺仪
 * \return AM_OK:成功    OTHER：失败
 */
int am_bmg160_read_rate(am_bmg160_handle_t handle, int16_t* p_rate)
{
    int ret = AM_OK;

    /* 读取X轴角速率 */
    ret = __bmg160_read_rate_reg(handle, BMG160_RATE_X_LSB, &p_rate[0]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }  

    /* 读取Y轴加速度 */    
    ret = __bmg160_read_rate_reg(handle, BMG160_RATE_Y_LSB, &p_rate[1]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
		
    /* 读取Z轴加速度 */    
    ret = __bmg160_read_rate_reg(handle, BMG160_RATE_Z_LSB, &p_rate[2]);
    /* 检查读取是否失败 */
    if (ret != AM_OK) {
        return ret;
    }
    return  ret;  
}

/**
 * \brief 读取bmg160温度
 * \return AM_OK:成功    OTHER：失败
 */
int am_bmg160_read_temp (am_bmg160_handle_t handle, int8_t* p_temp)
{
    int ret = AM_OK;
    
    /* I2C设备指针 */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);  
    
    ret = am_i2c_read(p_i2c_dev,BMG160_TEMP,(uint8_t*)(p_temp), 1);
    
    /* 检查发送是否失败 */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* 计算温度 */
    *p_temp = (int8_t)(24 + *p_temp * 1.0 / 2);
    
    return  ret;     
}

/**
 * \brief BMG160传感器初始化
 */
am_bmg160_handle_t am_bmg160_init(am_bmg160_dev_t           *p_dev, 
                                  const am_bmg160_devinfo_t *p_devinfo,
                                  am_i2c_handle_t           i2c_handle)
{
    uint8_t bmg160_id =    0; 
  
    /* 验证参数有效性 */
    if (NULL == p_dev ||  NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }
		
    p_dev->i2c_dev.handle = i2c_handle; 
		
    /* 初始配置好BMG160设备信息 */
    am_i2c_mkdev(&(p_dev->i2c_dev),
	              i2c_handle,
                  BMG160_ADDR,
				  AM_I2C_ADDR_7BIT 
		        | AM_I2C_SUBADDR_1BYTE);

    /* 读取BMG160 ID */
    am_i2c_read(&(p_dev->i2c_dev),BMG160_CHIP_ID, &bmg160_id, 1);
    AM_DBG_INFO("bmg160_ID = 0x%x\r\n", bmg160_id);
    
    /* 配置陀螺仪范围为±125°/s */
    am_i2c_write(&(p_dev->i2c_dev), BMG160_RANGE, &(p_devinfo->range), 1);

    /* 配置滤波带宽为230Hz */
    am_i2c_write(&(p_dev->i2c_dev), BMG160_BW, &(p_devinfo->bw), 1);
       
    /* 配置电源为正常模式、睡眠周期2ms */
    am_i2c_write(&(p_dev->i2c_dev), BMG160_LPM1, &(p_devinfo->sleep_period), 1);    
        
    return p_dev;
}

/**
 * \brief BMG160 设备解初始化
 */
void am_bmg160_deinit (am_bmg160_handle_t handle)
{

}

/* end of file */
