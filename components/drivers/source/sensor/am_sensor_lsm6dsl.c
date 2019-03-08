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
 * \brief ������ LSM6DSL �����ļ�
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
 * �궨��
 ******************************************************************************/
#define  __LSM6DSL_REG_FUNC_CFG_ACCESS   0x01 /**< \brief Ƕ��ʽ�������üĴ���*/
#define  __LSM6DSL_REG_SYNC_TIME_FRAME   0x04 /**< \brief ������ͬ�����üĴ���*/
#define  __LSM6DSL_REG_SYNC_RES_RATIO    0x05 
#define  __LSM6DSL_REG_FIFO_CTRL1        0x06 /**< \brief FIFO���ƼĴ���1     */
#define  __LSM6DSL_REG_FIFO_CTRL2        0x07 /**< \brief FIFO���ƼĴ���2     */
#define  __LSM6DSL_REG_FIFO_CTRL3        0x08 /**< \brief FIFO���ƼĴ���3     */
#define  __LSM6DSL_REG_FIFO_CTRL4        0x09 /**< \brief FIFO���ƼĴ���4     */
#define  __LSM6DSL_REG_FIFO_CTRL5        0x0A /**< \brief FIFO���ƼĴ���5     */
#define  __LSM6DSL_REG_DRDY_PULSE_CFG_G  0x0B /**< \brief ���ݾ������üĴ���  */
#define  __LSM6DSL_REG_RESERVED          0x0C /**< \brief �����Ĵ���          */
#define  __LSM6DSL_REG_INT1_CTRL         0x0D /**< \brief INT1���ſ��ƼĴ���  */
#define  __LSM6DSL_REG_INT2_CTRL         0x0E /**< \brief INT2���ſ��ƼĴ���  */
#define  __LSM6DSL_REG_WHO_AM_I          0x0F /**< \brief ID�Ĵ���            */
#define  __LSM6DSL_REG_CTRL1_XL          0x10 /**< \brief ���ƼĴ���1         */
#define  __LSM6DSL_REG_CTRL2_G           0x11 /**< \brief ���ƼĴ���2         */
#define  __LSM6DSL_REG_CTRL3_C           0x12 /**< \brief ���ƼĴ���3         */
#define  __LSM6DSL_REG_CTRL4_C           0x13 /**< \brief ���ƼĴ���4         */
#define  __LSM6DSL_REG_CTRL5_C           0x14 /**< \brief ���ƼĴ���5         */
#define  __LSM6DSL_REG_CTRL6_C           0x15 /**< \brief ���ƼĴ���6         */
#define  __LSM6DSL_REG_CTRL7_G           0x16 /**< \brief ���ƼĴ���7         */
#define  __LSM6DSL_REG_CTRL8_XL          0x17 /**< \brief ���ƼĴ���8         */
#define  __LSM6DSL_REG_CTRL9_XL          0x18 /**< \brief ���ƼĴ���9         */
#define  __LSM6DSL_REG_CTRL10_C          0x19 /**< \brief ���ƼĴ���10        */
#define  __LSM6DSL_REG_MASTER_CONFIG     0x1A /**< \brief �������üĴ���      */
#define  __LSM6DSL_REG_WAKE_UP_SRC       0x1B /**< \brief ����Դ�Ĵ���        */
#define  __LSM6DSL_REG_TAP_SRC           0x1C /**< \brief ���Դ�Ĵ���        */
#define  __LSM6DSL_REG_D6D_SRC           0x1D /**< \brief ����Դ�Ĵ���        */
#define  __LSM6DSL_REG_STATUS            0x1E /**< \brief ״̬�Ĵ���          */
#define  __LSM6DSL_REG_OUT_TEMP_L        0x20 /**< \brief �¶ȵ��ֽ����ݼĴ���*/
#define  __LSM6DSL_REG_OUT_TEMP_H        0x21 /**< \brief �¶ȸ��ֽ����ݼĴ���*/
#define  __LSM6DSL_REG_OUTX_L_G          0x22 /**< \brief ������X�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTX_H_G          0x23 /**< \brief ������X�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTY_L_G          0x24 /**< \brief ������Y�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTY_H_G          0x25 /**< \brief ������Y�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTZ_L_G          0x26 /**< \brief ������Z�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTZ_H_G          0x27 /**< \brief ������Z�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTX_L_XL         0x28 /**< \brief ���ٶ�X�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTX_H_XL         0x29 /**< \brief ���ٶ�X�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTY_L_XL         0x2A /**< \brief ���ٶ�Y�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTY_H_XL         0x2B /**< \brief ���ٶ�Y�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTZ_L_XL         0x2C /**< \brief ���ٶ�Z�����ݼĴ��� */
#define  __LSM6DSL_REG_OUTZ_H_XL         0x2D /**< \brief ���ٶ�Z�����ݼĴ��� */
#define  __LSM6DSL_REG_SENSORHUB1        0x2E /**< \brief ����������Ĵ���1   */
#define  __LSM6DSL_REG_SENSORHUB2        0x2F /**< \brief ����������Ĵ���2   */
#define  __LSM6DSL_REG_SENSORHUB3        0x30 /**< \brief ����������Ĵ���3   */
#define  __LSM6DSL_REG_SENSORHUB4        0x31 /**< \brief ����������Ĵ���4   */
#define  __LSM6DSL_REG_SENSORHUB5        0x32 /**< \brief ����������Ĵ���5   */
#define  __LSM6DSL_REG_SENSORHUB6        0x33 /**< \brief ����������Ĵ���6   */
#define  __LSM6DSL_REG_SENSORHUB7        0x34 /**< \brief ����������Ĵ���7   */
#define  __LSM6DSL_REG_SENSORHUB8        0x35 /**< \brief ����������Ĵ���8   */
#define  __LSM6DSL_REG_SENSORHUB9        0x36 /**< \brief ����������Ĵ���9   */
#define  __LSM6DSL_REG_SENSORHUB10       0x37 /**< \brief ����������Ĵ���10  */
#define  __LSM6DSL_REG_SENSORHUB11       0x38 /**< \brief ����������Ĵ���11  */
#define  __LSM6DSL_REG_SENSORHUB12       0x39 /**< \brief ����������Ĵ���12  */
#define  __LSM6DSL_REG_FIFO_STATUS1      0x3A /**< \brief FIFO״̬�Ĵ���1     */
#define  __LSM6DSL_REG_FIFO_STATUS2      0x3B /**< \brief FIFO״̬�Ĵ���2     */
#define  __LSM6DSL_REG_FIFO_STATUS3      0x3C /**< \brief FIFO״̬�Ĵ���3     */
#define  __LSM6DSL_REG_FIFO_STATUS4      0x3D /**< \brief FIFO״̬�Ĵ���4     */
#define  __LSM6DSL_REG_FIFO_DATA_OUT_L   0x3E /**< \brief FIFO���ݵ��ֽڼĴ���*/
#define  __LSM6DSL_REG_FIFO_DATA_OUT_H   0x3F /**< \brief FIFO���ݸ��ֽڼĴ���*/
#define  __LSM6DSL_REG_TIMESTAMP0        0x40 /**< \brief ʱ�������Ĵ���0   */
#define  __LSM6DSL_REG_TIMESTAMP1        0x41 /**< \brief ʱ�������Ĵ���1   */
#define  __LSM6DSL_REG_TIMESTAMP2        0x42 /**< \brief ʱ�������Ĵ���2   */
#define  __LSM6DSL_REG_STEP_TIMESTAMP_L  0x49 /**< \brief ����ʱ����Ĵ���    */
#define  __LSM6DSL_REG_STEP_TIMESTAMP_H  0x4A /**< \brief ����ʱ����Ĵ���    */
#define  __LSM6DSL_REG_STEP_COUNTER_L    0x4B /**< \brief �����������Ĵ���    */
#define  __LSM6DSL_REG_STEP_COUNTER_H    0x4C /**< \brief �����������Ĵ���    */
#define  __LSM6DSL_REG_SENSORHUB13       0x4D /**< \brief ����������Ĵ���13  */
#define  __LSM6DSL_REG_SENSORHUB14       0x4E /**< \brief ����������Ĵ���14  */
#define  __LSM6DSL_REG_SENSORHUB15       0x4F /**< \brief ����������Ĵ���15  */
#define  __LSM6DSL_REG_SENSORHUB16       0x50 /**< \brief ����������Ĵ���16  */
#define  __LSM6DSL_REG_SENSORHUB17       0x51 /**< \brief ����������Ĵ���17  */
#define  __LSM6DSL_REG_SENSORHUB18       0x52 /**< \brief ����������Ĵ���18  */
#define  __LSM6DSL_REG_FUNC_SRC1         0x53 /**< \brief �ж�Դ�Ĵ���1       */
#define  __LSM6DSL_REG_FUNC_SRC2         0x54 /**< \brief �ж�Դ�Ĵ���2       */ 
#define  __LSM6DSL_REG_WRIST_TILT_IA     0x55 /**< \brief ������б�ж�Դ�Ĵ���*/
#define  __LSM6DSL_REG_TAP_CFG           0x58 /**< \brief ������üĴ���      */
#define  __LSM6DSL_REG_TAP_THS_6D        0x59 /**< \brief ���������ֵ�Ĵ���  */
#define  __LSM6DSL_REG_INT_DUR2          0x5A /**< \brief ����������üĴ���  */
#define  __LSM6DSL_REG_WAKE_UP_THS       0x5B /**< \brief ��˫����ֵ�Ĵ���    */
#define  __LSM6DSL_REG_WAKE_UP_DUR       0x5C /**< \brief �������üĴ���      */
#define  __LSM6DSL_REG_FREE_FALL         0x5D /**< \brief �������幦�ܼĴ���  */
#define  __LSM6DSL_REG_MD1_CFG           0x5E /**< \brief �ж�1�Ĵ���         */ 
#define  __LSM6DSL_REG_MD2_CFG           0x5F /**< \brief �ж�2�Ĵ���         */ 
#define  __LSM6DSL_REG_MASTER_CMD_CODE   0x60 /**< \brief ����������Ĵ���    */
#define  __LSM6DSL_REG_SPI_ERROR_CODE    0x61 /**< \brief SPI������Ĵ���     */
#define  __LSM6DSL_REG_OUT_MAG_RAW_X_L   0x66 /**< \brief ����������ݼĴ���  */
#define  __LSM6DSL_REG_OUT_MAG_RAW_X_H   0x67 /**< \brief ����������ݼĴ���  */
#define  __LSM6DSL_REG_OUT_MAG_RAW_Y_L   0x68 /**< \brief ����������ݼĴ���  */
#define  __LSM6DSL_REG_OUT_MAG_RAW_Y_H   0x69 /**< \brief ����������ݼĴ���  */
#define  __LSM6DSL_REG_OUT_MAG_RAW_Z_L   0x6A /**< \brief ����������ݼĴ���  */
#define  __LSM6DSL_REG_OUT_MAG_RAW_Z_H   0x6B /**< \brief ����������ݼĴ���  */
#define  __LSM6DSL_REG_X_OFS_USR         0x73 /**< \brief X����ٶ��û�ƫ��   */
#define  __LSM6DSL_REG_Y_OFS_USR         0x74 /**< \brief Y����ٶ��û�ƫ��   */
#define  __LSM6DSL_REG_Z_OFS_USR         0x75 /**< \brief Z����ٶ��û�ƫ��   */
                                              
#define  __LSM6DSL_MY_ID                 0X6A /**< \brief IDֵ                */

/** \brief ���ò�����Χ */
#define  __LSM6DSL_RANGE_SET_2G  (0 << 2) /**< \brief ���ò�����ΧΪ��2G      */
#define  __LSM6DSL_RANGE_SET_4G  (2 << 2) /**< \brief ���ò�����ΧΪ��4G      */
#define  __LSM6DSL_RANGE_SET_8G  (3 << 2) /**< \brief ���ò�����ΧΪ��8G      */
#define  __LSM6DSL_RANGE_SET_16G (1 << 2) /**< \brief ���ò�����ΧΪ��16G     */

/** \brief �жϴ������� */
#define  __LSM6DSL_TRIGGER_READ  (1 << 7) /**< \brief ʹ�����ݿɶ�����        */
#define  __LSM6DSL_TRIGGER_CLOSE (0 << 7) /**< \brief �������ݿɶ�����        */
#define  __LSM6DSL_EN_INT1_DRDY  (1 << 0) /**< \brief ʹ�������ж�            */

/** \brief ������int8ת��Ϊһ��int16_t���� */
#define  __LSM6DSL_UINT8_TO_UINT16(buff) \
                       (int16_t)(((int16_t)(buff[1]) << 8) \
                                | (int16_t)buff[0])
/** \brief �����¶�,������10^6�� */
#define  __LSM6DSL_GET_TEMP_VALUE(data) (int32_t)((25 + \
                                            ((int16_t)(data) >> 8)) * 1000000)

/** \brief ������ٶ�,������10^6�� */
#define  __LSM6DSL_GET_ACCEL_VALUE_2G(data)  ((int32_t)(data * 610))
#define  __LSM6DSL_GET_ACCEL_VALUE_4G(data)  ((int32_t)(data * 1220))
#define  __LSM6DSL_GET_ACCEL_VALUE_8G(data)  ((int32_t)(data * 2440))
#define  __LSM6DSL_GET_ACCEL_VALUE_16G(data) ((int32_t)(data * 4880))

/*******************************************************************************
 * ���غ�������
 ******************************************************************************/
/** \brief ��ȡ�ô�����ĳһͨ�������� */
am_local am_err_t __pfn_type_get (void *p_drv, int id);

/** \brief ��ȡ������ͨ���������� */
am_local am_err_t __pfn_data_get (void            *p_drv,
                                  const int       *p_ids,
                                  int              num,
                                  am_sensor_val_t *p_buf);

/** \brief ʹ�ܴ�����ͨ�� */
am_local am_err_t __pfn_enable (void            *p_drv,
                                const int       *p_ids,
                                int              num,
                                am_sensor_val_t *p_result);

/** \brief ���ܴ�����ͨ�� */
am_local am_err_t __pfn_disable (void            *p_drv,
                                 const int       *p_ids,
                                 int              num,
                                 am_sensor_val_t *p_result);

/** \brief ���ô�����ͨ������ */
am_local am_err_t __pfn_attr_set (void                  *p_drv,
                                  int                    id,
                                  int                    attr,
                                  const am_sensor_val_t *p_val);

/** \brief ��ȡ������ͨ������ */
am_local am_err_t __pfn_attr_get (void            *p_drv,
                                  int              id,
                                  int              attr,
                                  am_sensor_val_t *p_val);

/** \brief ���ô�����һ��ͨ����������һ�������ص����� */
am_local am_err_t __pfn_trigger_cfg (void                   *p_drv,
                                     int                     id,
                                     uint32_t                flags,
                                     am_sensor_trigger_cb_t  pfn_cb,
                                     void                   *p_arg);

/** \brief �򿪴��� */
am_local am_err_t __pfn_trigger_on (void *p_drv, int id);

/** \brief �رմ��� */
am_local am_err_t __pfn_trigger_off (void *p_drv, int id);
                                  
/** \brief ��������׼���� */
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
 * \brief LSM6DSL д����
 */
am_local am_err_t __lsm6dsl_write (am_sensor_lsm6dsl_dev_t *p_this,
                                   uint32_t                subaddr,
                                   uint8_t                *p_buf,
                                   uint32_t                nbytes)
{
    return am_i2c_write(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief LSM6DSL ������
 */
am_local am_err_t __lsm6dsl_read (am_sensor_lsm6dsl_dev_t *p_this,
                                  uint32_t                subaddr,
                                  uint8_t                *p_buf,
                                  uint32_t                nbytes)
{
    return am_i2c_read(&p_this->i2c_dev, subaddr, p_buf, nbytes);
}

/**
 * \brief �������̽����ת���ɼ��ٶ�ʵ��ֵ
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

/** \breif �жϻص����� */
am_local void __lsm6dsl_alarm_callback (void *p_arg)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_arg;

    am_isr_defer_job_add(&p_this->g_myjob);   /*< \brief ����ж��ӳٴ������� */
}

/** \breif �ж��ӳٺ��� */
am_local void __am_pfnvoid_t (void *p_arg)
{
    am_sensor_lsm6dsl_dev_t* p_this = (am_sensor_lsm6dsl_dev_t*)p_arg;

    uint8_t reg_data[2];
    uint8_t i = 0;
    int32_t tem_data   = 0;

    /** \brief ��ȡX����ٶ� */
    __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTX_L_XL, reg_data, 2);
    
    tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);

    /** \brief X����ٶ� */
    p_this->data[0].val = __lsm6dsl_get_accel_value(p_this, tem_data); 
    p_this->data[0].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6) */

    /** \brief ��ȡY����ٶ� */
    __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTY_L_XL, reg_data, 2);

    tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
   
    /** \brief Y����ٶ� */
    p_this->data[1].val = __lsm6dsl_get_accel_value(p_this, tem_data);  
    p_this->data[1].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6) */

    /** \brief ��ȡZ����ٶ� */
    __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTZ_L_XL, reg_data, 2);
    
    tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
   
    /** \brief Z����ٶ� */
    p_this->data[2].val = __lsm6dsl_get_accel_value(p_this, tem_data);
    p_this->data[2].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6) */

    /** \brief ��ȡ�¶� */
    __lsm6dsl_read(p_this, __LSM6DSL_REG_OUT_TEMP_L, reg_data, 2);
    
    tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
    tem_data = __LSM6DSL_GET_TEMP_VALUE(tem_data);

    /** \brief �¶� */
    p_this->data[3].val = tem_data; 
    p_this->data[3].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/    
    
    for (i = 0; i < 4; i++) {
        if (p_this->pfn_trigger_cb[i] &&
                (p_this->flags[i] & AM_SENSOR_TRIGGER_DATA_READY)) {
            p_this->pfn_trigger_cb[i](p_this->p_arg[i],
                                      AM_SENSOR_TRIGGER_DATA_READY);
        }
    }
}

/** \brief ��ȡ�ô�����ĳһͨ�������� */
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

/** \brief ��ȡ������ͨ���������� */
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

    /* ��Ϊ1�������������׼�����������ص�������ʹ�� */
    if (num == 1) {
        cur_id = p_ids[0];
        /* ��������׼������������ʽ����ֱ�Ӹ�ֵ */
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

            /** \brief ��ȡX����ٶ� */
            ret = __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTX_L_XL, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);

            /** \brief X����ٶ� */
            p_buf[i].val = __lsm6dsl_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else if (cur_id == 1) {

            /** \brief ��ȡY����ٶ� */
            ret = __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTY_L_XL, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
           
            /** \brief Y����ٶ� */
            p_buf[i].val = __lsm6dsl_get_accel_value(p_this, tem_data); 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else if (cur_id == 2) {

            /** \brief ��ȡZ����ٶ� */
            ret = __lsm6dsl_read(p_this, __LSM6DSL_REG_OUTZ_L_XL, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
           
            /** \brief Z����ٶ� */
            p_buf[i].val = __lsm6dsl_get_accel_value(p_this, tem_data);  
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else if (cur_id == 3) {

            /** \brief ��ȡ�¶� */
            ret = __lsm6dsl_read(p_this, __LSM6DSL_REG_OUT_TEMP_L, reg_data, 2);
            if (ret != AM_OK) {
                return ret;
            }
            tem_data = __LSM6DSL_UINT8_TO_UINT16(reg_data);
            tem_data = __LSM6DSL_GET_TEMP_VALUE(tem_data);
       
            /** \brief �¶� */
            p_buf[i].val = tem_data; 
            p_buf[i].unit = AM_SENSOR_UNIT_MICRO;/*< \brief ��λĬ��Ϊ0:10^(-6)*/

        } else {
            return -AM_ENODEV;  /*< \brief ���˴�ͨ�������ڸô�������ֱ�ӷ��� */
        }
    }
    
    return ret;
}

/*
 * \brief ����ѡ������
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

/** \brief ʹ�ܴ�����ͨ�� */
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

    if (ret != AM_OK) {    /**< \breif �������û�иô�������ͨ�����룬���˳� */
        return curent_ret;
    }

    if (ret == AM_OK) {
        AM_BIT_SET(p_this->trigger, 15);
    }    
    
    /* ����Ϊ����ģʽ */
    ret = __lsm6dsl_write (p_this, __LSM6DSL_REG_CTRL1_XL, &ctrl1_xl, 1);
    if (ret != AM_OK) {
        curent_ret = ret;
    } 
    if (p_this->sam_range.val == 0) {
        /* Ĭ������Ϊ��2G */
        p_this->sam_range.val  = 2;    
        p_this->sam_range.unit = AM_SENSOR_UNIT_BASE;
    }
    
    /* �������� */ 
    ret = __reg_attr_range_set(p_this, p_this->sam_range.val);
    if (ret != AM_OK) {
        curent_ret = ret;
    }    
    
    return curent_ret;
}

/** \brief ���ܴ�����ͨ�� */
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

        /* ����Ϊ����ģʽ */
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

/** \brief ���ô�����ͨ������ */
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

    /* �޸����� */
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

        /* ����������Ϣ */
        p_this->sam_range.val  = __sampling_range.val;
        p_this->sam_range.unit = __sampling_range.unit;

        break;

    default:
        cur_ret = -AM_ENOTSUP;
        break;
    }

    return cur_ret;
}

/** \brief ��ȡ������ͨ������ */
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

    /* ��ȡ���� */
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

/** \brief ���ô�����һ��ͨ����������һ�������ص����� */
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

/** \brief �򿪴��� */
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

    /** \brief ʹ�����ݿɶ����� */
    reg_data = __LSM6DSL_TRIGGER_READ;
    ret = __lsm6dsl_write(p_this, __LSM6DSL_REG_DRDY_PULSE_CFG_G, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
    
    /** \brief ʹ�ܴ������� */
    reg_data = __LSM6DSL_EN_INT1_DRDY;
    ret = __lsm6dsl_write(p_this, __LSM6DSL_REG_INT1_CTRL, &reg_data, 1);
    if ( ret != AM_OK) {
        cur_ret = ret;
    }
  
    if (p_this->dev_info->trigger_pin != -1) {
        
        /* �򿪴������� */
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

/** \brief �رմ��� */
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

        /** \brief �������ݿɶ����� */
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
 * \brief ������ LSM6DSL ��ʼ��
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
    
    /* �ⲿ�ж����� */
    if (p_devinfo->trigger_pin != -1) {
        am_gpio_pin_cfg(p_devinfo->trigger_pin, AM_GPIO_INPUT);
        am_gpio_trigger_connect(p_devinfo->trigger_pin,
                                __lsm6dsl_alarm_callback,
                                (void*)p_dev);
        am_gpio_trigger_cfg(p_devinfo->trigger_pin, AM_GPIO_TRIGGER_RISE);
    }

    am_isr_defer_job_init(&p_dev->g_myjob, __am_pfnvoid_t, p_dev, 1);
   
    /* ��ȡID */
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
 * \brief ������ LSM6DSL ȥ��ʼ��
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
