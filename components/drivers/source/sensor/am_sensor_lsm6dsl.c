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
 * \brief 传感器 LSM6DSL 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-20  yrz, first implementation.
 * \endinternal
 */

#include "am_sensor_lsm6dsl.h"
#include "am_gpio.h"
#include "am_vdebug.h"

/*******************************************************************************
 * 宏定义
 ******************************************************************************/
#define  __LSM6DSL_REG_FUNC_CFG_ACCESS   0x01 /**< \brief 嵌入式功能配置寄存器*/
#define  __LSM6DSL_REG_SYNC_TIME_FRAME   0x04 /**< \brief 传感器同步配置寄存器*/
#define  __LSM6DSL_REG_SYNC_RES_RATIO    0x05 
#define  __LSM6DSL_REG_FIFO_CTRL1        0x06 /**< \brief FIFO控制寄存器1     */
#define  __LSM6DSL_REG_FIFO_CTRL2        0x07 /**< \brief FIFO控制寄存器2     */
#define  __LSM6DSL_REG_FIFO_CTRL3        0x08 /**< \brief FIFO控制寄存器3     */
#define  __LSM6DSL_REG_FIFO_CTRL4        0x09 /**< \brief FIFO控制寄存器4     */
#define  __LSM6DSL_REG_FIFO_CTRL5        0x0A /**< \brief FIFO控制寄存器5     */
#define  __LSM6DSL_REG_DRDY_PULSE_CFG_G  0x0B /**< \brief 数据就绪配置寄存器  */
#define  __LSM6DSL_REG_RESERVED          0x0C /**< \brief 保留寄存器          */
#define  __LSM6DSL_REG_INT1_CTRL         0x0D /**< \brief INT1引脚控制寄存器  */
#define  __LSM6DSL_REG_INT2_CTRL         0x0E /**< \brief INT2引脚控制寄存器  */
#define  __LSM6DSL_REG_WHO_AM_I          0x0F /**< \brief ID寄存器            */
#define  __LSM6DSL_REG_CTRL1_XL          0x10 /**< \brief 控制寄存器1         */
#define  __LSM6DSL_REG_CTRL2_G           0x11 /**< \brief 控制寄存器2         */
#define  __LSM6DSL_REG_CTRL3_C           0x12 /**< \brief 控制寄存器3         */
#define  __LSM6DSL_REG_CTRL4_C           0x13 /**< \brief 控制寄存器4         */
#define  __LSM6DSL_REG_CTRL5_C           0x14 /**< \brief 控制寄存器5         */
#define  __LSM6DSL_REG_CTRL6_C           0x15 /**< \brief 控制寄存器6         */
#define  __LSM6DSL_REG_CTRL7_G           0x16 /**< \brief 控制寄存器7         */
#define  __LSM6DSL_REG_CTRL8_XL          0x17 /**< \brief 控制寄存器8         */
#define  __LSM6DSL_REG_CTRL9_XL          0x18 /**< \brief 控制寄存器9         */
#define  __LSM6DSL_REG_CTRL10_C          0x19 /**< \brief 控制寄存器10        */
#define  __LSM6DSL_REG_MASTER_CONFIG     0x1A /**< \brief 主机配置寄存器      */
#define  __LSM6DSL_REG_WAKE_UP_SRC       0x1B /**< \brief 唤醒源寄存器        */
#define  __LSM6DSL_REG_TAP_SRC           0x1C /**< \brief 点击源寄存器        */
#define  __LSM6DSL_REG_D6D_SRC           0x1D /**< \brief 方向源寄存器        */
#define  __LSM6DSL_REG_STATUS            0x1E /**< \brief 状态寄存器          */
#define  __LSM6DSL_REG_OUT_TEMP_L        0x20 /**< \brief 温度低字节数据寄存器*/
#define  __LSM6DSL_REG_OUT_TEMP_H        0x21 /**< \brief 温度高字节数据寄存器*/
#define  __LSM6DSL_REG_OUTX_L_G          0x22 /**< \brief 陀螺仪X轴数据寄存器 */
#define  __LSM6DSL_REG_OUTX_H_G          0x23 /**< \brief 陀螺仪X轴数据寄存器 */
#define  __LSM6DSL_REG_OUTY_L_G          0x24 /**< \brief 陀螺仪Y轴数据寄存器 */
#define  __LSM6DSL_REG_OUTY_H_G          0x25 /**< \brief 陀螺仪Y轴数据寄存器 */
#define  __LSM6DSL_REG_OUTZ_L_G          0x26 /**< \brief 陀螺仪Z轴数据寄存器 */
#define  __LSM6DSL_REG_OUTZ_H_G          0x27 /**< \brief 陀螺仪Z轴数据寄存器 */
#define  __LSM6DSL_REG_OUTX_L_XL         0x28 /**< \brief 加速度X轴数据寄存器 */
#define  __LSM6DSL_REG_OUTX_H_XL         0x29 /**< \brief 加速度X轴数据寄存器 */
#define  __LSM6DSL_REG_OUTY_L_XL         0x2A /**< \brief 加速度Y轴数据寄存器 */
#define  __LSM6DSL_REG_OUTY_H_XL         0x2B /**< \brief 加速度Y轴数据寄存器 */
#define  __LSM6DSL_REG_OUTZ_L_XL         0x2C /**< \brief 加速度Z轴数据寄存器 */
#define  __LSM6DSL_REG_OUTZ_H_XL         0x2D /**< \brief 加速度Z轴数据寄存器 */
#define  __LSM6DSL_REG_SENSORHUB1        0x2E /**< \brief 集线器输出寄存器1   */
#define  __LSM6DSL_REG_SENSORHUB2        0x2F /**< \brief 集线器输出寄存器2   */
#define  __LSM6DSL_REG_SENSORHUB3        0x30 /**< \brief 集线器输出寄存器3   */
#define  __LSM6DSL_REG_SENSORHUB4        0x31 /**< \brief 集线器输出寄存器4   */
#define  __LSM6DSL_REG_SENSORHUB5        0x32 /**< \brief 集线器输出寄存器5   */
#define  __LSM6DSL_REG_SENSORHUB6        0x33 /**< \brief 集线器输出寄存器6   */
#define  __LSM6DSL_REG_SENSORHUB7        0x34 /**< \brief 集线器输出寄存器7   */
#define  __LSM6DSL_REG_SENSORHUB8        0x35 /**< \brief 集线器输出寄存器8   */
#define  __LSM6DSL_REG_SENSORHUB9        0x36 /**< \brief 集线器输出寄存器9   */
#define  __LSM6DSL_REG_SENSORHUB10       0x37 /**< \brief 集线器输出寄存器10  */
#define  __LSM6DSL_REG_SENSORHUB11       0x38 /**< \brief 集线器输出寄存器11  */
#define  __LSM6DSL_REG_SENSORHUB12       0x39 /**< \brief 集线器输出寄存器12  */
#define  __LSM6DSL_REG_FIFO_STATUS1      0x3A /**< \brief FIFO状态寄存器1     */
#define  __LSM6DSL_REG_FIFO_STATUS2      0x3B /**< \brief FIFO状态寄存器2     */
#define  __LSM6DSL_REG_FIFO_STATUS3      0x3C /**< \brief FIFO状态寄存器3     */
#define  __LSM6DSL_REG_FIFO_STATUS4      0x3D /**< \brief FIFO状态寄存器4     */
#define  __LSM6DSL_REG_FIFO_DATA_OUT_L   0x3E /**< \brief FIFO数据低字节寄存器*/
#define  __LSM6DSL_REG_FIFO_DATA_OUT_H   0x3F /**< \brief FIFO数据高字节寄存器*/
#define  __LSM6DSL_REG_TIMESTAMP0        0x40 /**< \brief 时间戳输出寄存器0   */
#define  __LSM6DSL_REG_TIMESTAMP1        0x41 /**< \brief 时间戳输出寄存器1   */
#define  __LSM6DSL_REG_TIMESTAMP2        0x42 /**< \brief 时间戳输出寄存器2   */
#define  __LSM6DSL_REG_STEP_TIMESTAMP_L  0x49 /**< \brief 步进时间戳寄存器    */
#define  __LSM6DSL_REG_STEP_TIMESTAMP_H  0x4A /**< \brief 步进时间戳寄存器    */
#define  __LSM6DSL_REG_STEP_COUNTER_L    0x4B /**< \brief 步数计数器寄存器    */
#define  __LSM6DSL_REG_STEP_COUNTER_H    0x4C /**< \brief 步数计数器寄存器    */
#define  __LSM6DSL_REG_SENSORHUB13       0x4D /**< \brief 集线器输出寄存器13  */
#define  __LSM6DSL_REG_SENSORHUB14       0x4E /**< \brief 集线器输出寄存器14  */
#define  __LSM6DSL_REG_SENSORHUB15       0x4F /**< \brief 集线器输出寄存器15  */
#define  __LSM6DSL_REG_SENSORHUB16       0x50 /**< \brief 集线器输出寄存器16  */
#define  __LSM6DSL_REG_SENSORHUB17       0x51 /**< \brief 集线器输出寄存器17  */
#define  __LSM6DSL_REG_SENSORHUB18       0x52 /**< \brief 集线器输出寄存器18  */
#define  __LSM6DSL_REG_FUNC_SRC1         0x53 /**< \brief 中断源寄存器1       */
#define  __LSM6DSL_REG_FUNC_SRC2         0x54 /**< \brief 中断源寄存器2       */ 
#define  __LSM6DSL_REG_WRIST_TILT_IA     0x55 /**< \brief 手腕倾斜中断源寄存器*/
#define  __LSM6DSL_REG_TAP_CFG           0x58 /**< \brief 点击配置寄存器      */
#define  __LSM6DSL_REG_TAP_THS_6D        0x59 /**< \brief 点击功能阈值寄存器  */
#define  __LSM6DSL_REG_INT_DUR2          0x5A /**< \brief 点击功能设置寄存器  */
#define  __LSM6DSL_REG_WAKE_UP_THS       0x5B /**< \brief 单双击阈值寄存器    */
#define  __LSM6DSL_REG_WAKE_UP_DUR       0x5C /**< \brief 唤醒设置寄存器      */
#define  __LSM6DSL_REG_FREE_FALL         0x5D /**< \brief 自由落体功能寄存器  */
#define  __LSM6DSL_REG_MD1_CFG           0x5E /**< \brief 中断1寄存器         */ 
#define  __LSM6DSL_REG_MD2_CFG           0x5F /**< \brief 中断2寄存器         */ 
#define  __LSM6DSL_REG_MASTER_CMD_CODE   0x60 /**< \brief 主机命令码寄存器    */
#define  __LSM6DSL_REG_SPI_ERROR_CODE    0x61 /**< \brief SPI错误码寄存器     */
#define  __LSM6DSL_REG_OUT_MAG_RAW_X_L   0x66 /**< \brief 外磁力计数据寄存器  */
#define  __LSM6DSL_REG_OUT_MAG_RAW_X_H   0x67 /**< \brief 外磁力计数据寄存器  */
#define  __LSM6DSL_REG_OUT_MAG_RAW_Y_L   0x68 /**< \brief 外磁力计数据寄存器  */
#define  __LSM6DSL_REG_OUT_MAG_RAW_Y_H   0x69 /**< \brief 外磁力计数据寄存器  */
#define  __LSM6DSL_REG_OUT_MAG_RAW_Z_L   0x6A /**< \brief 外磁力计数据寄存器  */
#define  __LSM6DSL_REG_OUT_MAG_RAW_Z_H   0x6B /**< \brief 外磁力计数据寄存器  */
#define  __LSM6DSL_REG_X_OFS_USR         0x73 /**< \brief X轴加速度用户偏移   */
#define  __LSM6DSL_REG_Y_OFS_USR         0x74 /**< \brief Y轴加速度用户偏移   */
#define  __LSM6DSL_REG_Z_OFS_USR         0x75 /**< \brief Z轴加速度用户偏移   */
                                              
#define  __LSM6DSL_MY_ID                 0X6A /**< \brief ID值                */

/** \brief 设置采样范围 */
#define  __LSM6DSL_RANGE_SET_2G  (0 << 2) /**< \brief 设置采样范围为±2G      */
#define  __LSM6DSL_RANGE_SET_4G  (2 << 2) /**< \brief 设置采样范围为±4G      */
#define  __LSM6DSL_RANGE_SET_8G  (3 << 2) /**< \brief 设置采样范围为±8G      */
#define  __LSM6DSL_RANGE_SET_16G (1 << 2) /**< \brief 设置采样范围为±16G     */

/** \brief 中断触发配置 */
#define  __LSM6DSL_TRIGGER_READ  (1 << 7) /**< \brief 使能数据可读触发        */
#define  __LSM6DSL_TRIGGER_CLOSE (0 << 7) /**< \brief 禁能数据可读触发        */
#define  __LSM6DSL_EN_INT1_DRDY  (1 << 0) /**< \brief 使能引脚中断            */

/** \brief 将两个int8转换为一个int16_t类型 */
#define  __LSM6DSL_UINT8_TO_UINT16(buff) \
                       (int16_t)(((int16_t)(buff[1]) << 8) \
                                | (int16_t)buff[0])
/** \brief 计算温度,并扩大10^6倍 */
#define  __LSM6DSL_GET_TEMP_VALUE(data) (int32_t)((25 + \
                                            ((int16_t)(data) >> 8)) * 1000000)

/** \brief 计算加速度(单位为g),并扩大10^6倍 */
#define  __LSM6DSL_GET_ACCEL_VALUE_2G(data)  ((int32_t)(data * 61))
#define  __LSM6DSL_GET_ACCEL_VALUE_4G(data)  ((int32_t)(data * 122))
#define  __LSM6DSL_GET_ACCEL_VALUE_8G(data)  ((int32_t)(data * 244))
#define  __LSM6DSL_GET_ACCEL_VALUE_16G(data) ((int32_t)(data * 488))

/*******************************************************************************
 * 本地函数声明
 ******************************************************************************/
/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id);

/** \brief 获取传感器通道采样数据 */
am_local am_err_t __pfn_data_get (void            *p_drv,
                                  const int       *p_ids,
                                  int              num,
                                  am_sensor_val_t *p_buf);

/** \brief 使能传感器通道 */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result);

/** \brief 禁能传感器通道 */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result);

/** \brief 配置传感器通道属性 */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val);

/** \brief 获取传感器通道属性 */
am_local am_err_t __pfn_attr_get (void            *p_drv,
                                  int              id,
                                  int              attr,
                                  am_sensor_val_t *p_val);

/** \brief 设置触发，一个通道仅能设置一个触发回调函数 */
am_local am_err_t __pfn_trigger_cfg (void                   *p_drv,
                                     int                     id,
                                     uint32_t                flags,
                                     am_sensor_trigger_cb_t  pfn_cb,
                                     void                   *p_arg);

/** \brief 打开触发 */
am_local am_err_t __pfn_trigger_on (void *p_drv, int id);

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id);
                                  
/** \brief 传感器标准服务 */
am_local am_const struct am_sensor_drv_funcs __g_sensor_lsm6dsl_funcs = {
        __pfn_type_get,
        __pfn_data_get,
        __pfn_enable,
        __pfn_disable,
        __pfn_attr_set,
        __pfn_attr_get,
        __pfn_trigger_cfg,
        __pfn_trigger_on,
        __pfn_trigger_off
};
/*******************************************************************************
  Local functions
*******************************************************************************/
/**
 * \brief LSM6DSL 写数据
 */
am_local am_err_t __lsm6dsl_write (am_sensor_lsm6dsl_dev_t *p_this,
                                   uint32_t                subaddr,
                                   uint8_t                *p_buf,
                                   uint32_t                nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief LSM6DSL 读数据
 */
am_local am_err_t __lsm6dsl_read (am_sensor_lsm6dsl_dev_t *p_this,
                                  uint32_t                subaddr,
                                  uint8_t                *p_buf,
                                  uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief 根据量程将结果转换成加速度实际值
 */
am_local int32_t __lsm6dsl_get_accel_value (am_sensor_lsm6dsl_dev_t* p_this,
                                            uint32_t                 data)
{
    int32_t real_data = 0;
    
    if (p_this == NULL) {
        return NULL;
    }
    real_data = __LSM6DSL_GET_ACCEL_VALUE_2G(data);
    switch (p_this->sam_range.val) {
    case 2:
        real_data = __LSM6DSL_GET_ACCEL_VALUE_2G(data);
        break;
    case 4:
        real_data = __LSM6DSL_GET_ACCEL_VALUE_4G(data);
        break;
    case 8:
        real_data = __LSM6DSL_GET_ACCEL_VALUE_8G(data);
        break;
    case 16:
        real_data = __LSM6DSL_GET_ACCEL_VALUE_16G(data);
        break;
    default:
        real_data = -AM_EINVAL;
        break;
    }
    return real_data;
}

/** \breif 中断回调函数 */
am_local void __lsm6dsl_alarm_callback (void *p_arg)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief 添加中断延迟处理任务 */
}

/** \breif 中断延迟函数 */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_arg;

    uint8_t reg_data[2];
    uint8_t i = 0;
    int32_t tem_data   = 0;

    /** \brief 获取X轴加速度 */
    __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTX_L_XL, reg_data, 2);
    
    tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);

    /** \brief X轴加速度 */
    p_this->data[0].val = __lsm6dsl_get_accel_value(p_this, tem_data); 
    p_this->data[0].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6) */

    /** \brief 获取Y轴加速度 */
    __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTY_L_XL, reg_data, 2);

    tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
   
    /** \brief Y轴加速度 */
    p_this->data[1].val = __lsm6dsl_get_accel_value(p_this, tem_data);  
    p_this->data[1].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6) */

    /** \brief 获取Z轴加速度 */
    __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTZ_L_XL, reg_data, 2);
    
    tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
   
    /** \brief Z轴加速度 */
    p_this->data[2].val = __lsm6dsl_get_accel_value(p_this, tem_data);
    p_this->data[2].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6) */

    /** \brief 获取温度 */
    __lsm6dsl_read(p_this, __LSM6DSL_REG_OUT_TEMP_L, reg_data, 2);
    
    tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
    tem_data = __LSM6DSL_GET_TEMP_VALUE(tem_data);

    /** \brief 温度 */
    p_this->data[3].val = tem_data; 
    p_this->data[3].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/    
    
    for (i = 0; i < 4; i++) {
        if (p_this->pfn_trigger_cb[i] &&
                (p_this->flags[i] & AM_SENSOR_TRIGGER_DATA_READY)) {
            p_this->pfn_trigger_cb[i](p_this->p_arg[i],
                                      AM_SENSOR_TRIGGER_DATA_READY);
        }
    }
}

/** \brief 获取该传感器某一通道的类型 */
am_local am_err_t __pfn_type_get (void *p_drv, int id)
{
    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (id == AM_LSM6DSL_CHAN_1 || id == AM_LSM6DSL_CHAN_2 ||
        id == AM_LSM6DSL_CHAN_3) {
        return AM_SENSOR_TYPE_ACCELEROMETER;
    } else if (id == AM_LSM6DSL_CHAN_4) {
        return AM_SENSOR_TYPE_TEMPERATURE;
    } else {
        return -AM_ENODEV;
    }
}

/** \brief 获取传感器通道采样数据 */
am_local am_err_t __pfn_data_get (void            *p_drv,
                                  const int       *p_ids,
                                  int              num,
                                  am_sensor_val_t *p_buf)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_drv;
    
    am_err_t ret        = AM_OK;
    uint8_t reg_data[2] = {0};
    uint32_t tem_data   = 0;

    int cur_id = 0;
    int i = 0;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (AM_BIT_GET(p_this->trigger, 15) != 1) {
        return -AM_EPERM;
    }

    for (i = 0; i < num; i++) {
        p_buf[i].unit = AM_SENSOR_UNIT_INVALID;
    }

    /* 若为1，则可能在数据准备就绪触发回调函数中使用 */
    if (num == 1) {
        cur_id = p_ids[0];
        /* 若打开数据准备就绪触发方式，则直接赋值 */
        if ((AM_BIT_GET(p_this->trigger, 4)) &&
                ((p_this->flags[0] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->flags[1] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->flags[2] & AM_SENSOR_TRIGGER_DATA_READY) ||
                 (p_this->flags[3] & AM_SENSOR_TRIGGER_DATA_READY))) {
            p_buf[0].val = p_this->data[cur_id].val;
            p_buf[0].unit= p_this->data[cur_id].unit;
            return AM_OK;
        }
    } 
    
    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0) {

            /** \brief 获取X轴加速度 */
            ret = __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTX_L_XL, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);

            /** \brief X轴加速度 */
            p_buf[i].val = __lsm6dsl_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 1) {

            /** \brief 获取Y轴加速度 */
            ret = __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTY_L_XL, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
           
            /** \brief Y轴加速度 */
            p_buf[i].val = __lsm6dsl_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 2) {

            /** \brief 获取Z轴加速度 */
            ret = __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTZ_L_XL, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
           
            /** \brief Z轴加速度 */
            p_buf[i].val = __lsm6dsl_get_accel_value(p_this, tem_data);  
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/

        } else if (cur_id == 3) {

            /** \brief 获取温度 */
            ret = __lsm6dsl_read(p_this, __LSM6DSL_REG_OUT_TEMP_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
            tem_data = __LSM6DSL_GET_TEMP_VALUE(tem_data);
       
            /** \brief 温度 */
            p_buf[i].val = tem_data; 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief 单位默认为0:10^(-6)*/

        } else {
            return -AM_ENODEV;  /*< \brief 若此次通道不属于该传感器，直接返回 */
        }
    }
    
    return ret;
}

/*
 * \brief 配置选择量程
 */
am_local am_err_t  __reg_attr_range_set (am_sensor_lsm6dsl_dev_t *p_this, 
                                         uint8_t                 range)
{
    uint8_t range_cfg = 0;

    __lsm6dsl_read(p_this, __LSM6DSL_REG_CTRL1_XL, &range_cfg, 1);
    
    switch (range) {
    case 2:
        range_cfg |= __LSM6DSL_RANGE_SET_2G;
        break;
    case 4:
        range_cfg |= __LSM6DSL_RANGE_SET_4G;
        break;
    case 8:
        range_cfg |= __LSM6DSL_RANGE_SET_8G;
        break;
    case 16:
        range_cfg |= __LSM6DSL_RANGE_SET_16G;
        break;
    default:
        break;
    }
    return __lsm6dsl_write(p_this, __LSM6DSL_REG_CTRL1_XL, &range_cfg, 1);
}

/** \brief 使能传感器通道 */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;
    uint8_t ctrl1_xl = (4 << 4);

    am_err_t ret = -AM_ENODEV;
    am_err_t curent_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1 || cur_id == 2 || cur_id == 3) {
            if (p_result != NULL) {
                p_result[i].val = AM_OK;
            }
            
            AM_BIT_SET(p_this->trigger, 14 - cur_id);

            ret = AM_OK;
        } else {
            if (p_result != NULL) {
                p_result[i].val = -AM_ENODEV;
            }
            curent_ret = -AM_ENODEV;
        }
    }

    if (ret != AM_OK) {    /**< \breif 如果本次没有该传感器的通道传入，则退出 */
        return curent_ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 15);
    }    
    
    /* 配置为工作模式 */
    ret = __lsm6dsl_write (p_this, __LSM6DSL_REG_CTRL1_XL, &ctrl1_xl, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    } 
    if (p_this->sam_range.val == 0) {
        /* 默认量程为±2G */
        p_this->sam_range.val  = 2;    
        p_this->sam_range.unit = AM_SENSOR_UNIT_BASE;
    }
    
    /* 配置量程 */ 
    ret = __reg_attr_range_set(p_this, p_this->sam_range.val);
    if (ret != AM_OK) {
        curent_ret = ret;
    }    
    
    return curent_ret;
}

/** \brief 禁能传感器通道 */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_drv;

    int i = 0;
    int cur_id = 0;
    uint8_t ctrl1_xl = 0;
    
    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        cur_id = p_ids[i];

        if (cur_id == 0 || cur_id == 1 || cur_id == 2 || cur_id == 3) {

            AM_BIT_CLR(p_this->trigger, 14 - cur_id);
            ret = AM_OK;

        } else {
            ret = -AM_ENODEV;
        }

        if (p_result != NULL) {
            p_result[i].val = ret;
        }
    }

    if ((AM_BIT_GET(p_this->trigger, 14) == 0)
            && (AM_BIT_GET(p_this->trigger, 13) == 0)
            && (AM_BIT_GET(p_this->trigger, 12) == 0)
            && (AM_BIT_GET(p_this->trigger, 11) == 0)) {

        /* 配置为掉电模式 */
        ret = __lsm6dsl_write (p_this, __LSM6DSL_REG_CTRL1_XL, &ctrl1_xl, 1);      
        if (ret != AM_OK){
            cur_ret = ret;
        }   
        
        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->trigger, 15);
        }
    }

    return cur_ret;
}

/** \brief 配置传感器通道属性 */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_drv;

    am_err_t cur_ret = AM_OK;

    am_sensor_val_t sensor_val = {0, 0};

    am_sensor_val_t __sampling_range = {0, 0};

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if ((id != AM_LSM6DSL_CHAN_1) && (id != AM_LSM6DSL_CHAN_2) &&
        (id != AM_LSM6DSL_CHAN_3)) {
        return -AM_ENODEV;
    }

    switch (attr) {

    /* 修改量程 */
    case AM_SENSOR_ATTR_FULL_SCALE:

        sensor_val.unit = p_val->unit;
        sensor_val.val  = p_val->val;    
    
        if (sensor_val.val < 2 || sensor_val.val > 16) {
            return -AM_ENOTSUP;
        }

        am_sensor_val_unit_convert(&sensor_val, 1, AM_SENSOR_UNIT_BASE);

        if (sensor_val.val == 2) {
            __sampling_range.val = 2;
        } else if (sensor_val.val == 4) {
            __sampling_range.val = 4;
        } else if (sensor_val.val == 8) {
            __sampling_range.val = 8;
        } else if (sensor_val.val == 16) {
            __sampling_range.val = 16;
        }

        /* 保存配置信息 */
        p_this->sam_range.val  = __sampling_range.val;
        p_this->sam_range.unit = __sampling_range.unit;

        break;

    default:
        cur_ret = -AM_ENOTSUP;
        break;
    }

    return cur_ret;
}

/** \brief 获取传感器通道属性 */
am_local am_err_t __pfn_attr_get (void            *p_drv,
                                  int              id,
                                  int              attr,
                                  am_sensor_val_t *p_val)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_drv;

    am_err_t ret = AM_OK;

    if ((id != AM_LSM6DSL_CHAN_1) && (id != AM_LSM6DSL_CHAN_2)&&
        (id != AM_LSM6DSL_CHAN_3)) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    switch (attr) {

    /* 获取量程 */
    case AM_SENSOR_ATTR_FULL_SCALE:

        p_val->val  = p_this->sam_range.val;
        p_val->unit = p_this->sam_range.unit;
        break;

    default:
        ret = -AM_ENOTSUP;
        break;

    }

    return ret;
}

/** \brief 设置触发，一个通道仅能设置一个触发回调函数 */
am_local am_err_t __pfn_trigger_cfg (void                   *p_drv,
                                     int                     id,
                                     uint32_t                flags,
                                     am_sensor_trigger_cb_t  pfn_cb,
                                     void                   *p_arg)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_drv;

    if (id != 0 && id != 1 && id != 2 && id != 3) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    switch (flags) {
    case AM_SENSOR_TRIGGER_DATA_READY:
        break;
    default:
        return -AM_ENOTSUP;
    }

    p_this->pfn_trigger_cb[id] = pfn_cb;
    p_this->p_arg[id]          = p_arg;
    p_this->flags[id]          = flags;

    return AM_OK;
}

/** \brief 打开触发 */
am_local am_err_t __pfn_trigger_on (void *p_drv, int id)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_drv;

    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;
    uint8_t reg_data = 0;

    if (id != 0 && id != 1 && id != 2 && id != 3) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->trigger & 0x10) {
        return AM_OK;
    }

    /** \brief 使能数据可读触发 */
    reg_data = __LSM6DSL_TRIGGER_READ;
    ret = __lsm6dsl_write(p_this, __LSM6DSL_REG_DRDY_PULSE_CFG_G, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
    
    /** \brief 使能触发引脚 */
    reg_data = __LSM6DSL_EN_INT1_DRDY;
    ret = __lsm6dsl_write(p_this, __LSM6DSL_REG_INT1_CTRL, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
  
    if (p_this->dev_info->trigger_pin != -1) {
        
        /* 打开触发引脚 */
        ret = am_gpio_trigger_on(p_this->dev_info->trigger_pin);
        if (ret != AM_OK) {
            cur_ret = ret;
        }
    }
    
    if (cur_ret == AM_OK && id == 0) {
        AM_BIT_SET(p_this->trigger, 0);
        AM_BIT_SET(p_this->trigger, 4);
    }
    if (cur_ret == AM_OK && id == 1) {
        AM_BIT_SET(p_this->trigger, 1);
        AM_BIT_SET(p_this->trigger, 4);
    }
    if (cur_ret == AM_OK && id == 2) {
        AM_BIT_SET(p_this->trigger, 2);
        AM_BIT_SET(p_this->trigger, 4);
    }
    if (cur_ret == AM_OK && id == 3) {
        AM_BIT_SET(p_this->trigger, 3);
        AM_BIT_SET(p_this->trigger, 4);
    }
    
    return cur_ret;
}

/** \brief 关闭触发 */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_drv;

    am_err_t cur_ret = AM_OK;
    am_err_t ret     = AM_OK;
    uint8_t reg_data = 0;
    
    if (id != 0 && id != 1 && id != 2 && id != 3) {
        return -AM_ENODEV;
    }

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_this->pfn_trigger_cb[id] = NULL;
    p_this->p_arg[id]          = NULL;
    p_this->flags[id]          = 0;

    AM_BIT_CLR(p_this->trigger, id);

    if ((p_this->trigger & 0xF) == 0) {

        /** \brief 禁能数据可读触发 */
        reg_data = __LSM6DSL_TRIGGER_CLOSE;
        ret = __lsm6dsl_write(p_this, __LSM6DSL_REG_DRDY_PULSE_CFG_G, &reg_data, 1);
        if (ret != AM_OK) {
            cur_ret = ret;
        }

        if ((p_this->dev_info->trigger_pin != -1) && (cur_ret == AM_OK)) {
            ret = am_gpio_trigger_off(p_this->dev_info->trigger_pin);
            if (ret != AM_OK) {
                cur_ret = ret;
            }
        }

        if (cur_ret == AM_OK) {
            AM_BIT_CLR(p_this->trigger, 4);
        }
    }    
    
    return cur_ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/
/**
 * \brief 传感器 LSM6DSL 初始化
 */
am_sensor_handle_t am_sensor_lsm6dsl_init (
        am_sensor_lsm6dsl_dev_t           *p_dev,
        const am_sensor_lsm6dsl_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle)
{
    am_err_t ret = AM_OK;
    am_err_t cur_ret = AM_OK;

    uint8_t lsm6dsl_id = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    am_i2c_mkdev(&p_dev->i2c_dev,
                 handle,
                 p_devinfo->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_dev->lsm6dsl_dev.p_drv   = p_dev;
    p_dev->lsm6dsl_dev.p_funcs = &__g_sensor_lsm6dsl_funcs;
    p_dev->dev_info            = p_devinfo;
    p_dev->pfn_trigger_cb[0]   = NULL;
    p_dev->pfn_trigger_cb[1]   = NULL;
    p_dev->pfn_trigger_cb[2]   = NULL;
    p_dev->pfn_trigger_cb[3]   = NULL;
    p_dev->p_arg[0]            = NULL;
    p_dev->p_arg[1]            = NULL;
    p_dev->p_arg[2]            = NULL;
    p_dev->p_arg[3]            = NULL;
    p_dev->flags[0]            = 0;
    p_dev->flags[1]            = 0;
    p_dev->flags[2]            = 0;
    p_dev->flags[3]            = 0;
    p_dev->trigger             = 0;
    p_dev->data[0].val         = 0;
    p_dev->data[0].unit        = AM_SENSOR_UNIT_INVALID;
    p_dev->data[1].val         = 0;
    p_dev->data[1].unit        = AM_SENSOR_UNIT_INVALID;
    p_dev->data[2].val         = 0;
    p_dev->data[2].unit        = AM_SENSOR_UNIT_INVALID;
    p_dev->data[3].val         = 0;
    p_dev->data[3].unit        = AM_SENSOR_UNIT_INVALID;
    p_dev->sam_range.val       = 0;
    p_dev->sam_range.unit      = AM_SENSOR_UNIT_INVALID;
    
    /* 外部中断连接 */
    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __lsm6dsl_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_RISE);
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);
   
    /* 读取ID */
    ret = __lsm6dsl_read(p_dev, __LSM6DSL_REG_WHO_AM_I, &lsm6dsl_id, 1);
    if (ret != AM_OK || lsm6dsl_id != __LSM6DSL_MY_ID) {
        cur_ret = ret;
    }

    if (cur_ret != AM_OK) {
        am_kprintf("\r\nSensor LSM6DSL Init is ERROR! \r\n");
        return NULL;
    }

    return &(p_dev->lsm6dsl_dev);
}

/**
 * \brief 传感器 LSM6DSL 去初始化
 */
am_err_t am_sensor_lsm6dsl_deinit (am_sensor_handle_t handle)
{
    am_sensor_lsm6dsl_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->lsm6dsl_dev.p_drv   = NULL;
    p_dev->lsm6dsl_dev.p_funcs = NULL;
    p_dev->dev_info            = NULL;

    return AM_OK;
}


/* end of file */
