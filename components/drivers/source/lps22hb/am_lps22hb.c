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
 * \brief LPS22HB�����ļ�
 * \sa am_lsp22hb.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-11  yrz, first implementation.
 * \endinternal
 */
 
#include "am_lps22hb.h" 
 
 
/**
 * \brief LPS22HB�ӻ���ַ
 */    
#define LPS22HB_DEVICE_ADDR        0x5C  
 
/**
 * \brief LPS22HB�Ĵ�����ַ
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
 * \brief LPS22HBѹ�����¶�״̬��־λ
 */ 
#define   STATUS_P_DA     (1 << 0)   
#define   STATUS_T_DA     (1 << 1)

/**
 * \brief LPS22HB��λָ��
 */ 
#define   LPS22HB_RESET            0x04


/**
 * \brief ��ʼ��LPS22HB
 */
am_lps22hb_handle_t am_lsp22hb_init (am_lps22hb_dev_t *p_dev, 
                                     const am_lps22hb_devinfo_t *p_devinfo, 
                                     am_i2c_handle_t i2c_handle)  
{   
    uint8_t       lps22hb_id = 0; 
    uint8_t       ctrl_reg1  = 0x00; 
    const uint8_t ctrl_reg2  = LPS22HB_RESET;

    /* ��֤������Ч�� */
    if (NULL == p_dev || NULL == p_devinfo || NULL == i2c_handle) {
        return NULL;
    }    

    p_dev->p_devinfo = p_devinfo;
    p_dev->i2c_dev.handle = i2c_handle; 
    
    /* ��ʼ����LPS22HB����Ϣ */
    am_i2c_mkdev(&(p_dev->i2c_dev),
                i2c_handle,
                LPS22HB_DEVICE_ADDR,
                AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
    /* ��λLPS22HB */  
    am_i2c_write(&(p_dev->i2c_dev), LPS22HB_CTRL_REG2, &ctrl_reg2, 1);
    am_mdelay(10); 

    /* ��ȡLPS22HB ID */ 
    am_i2c_read(&(p_dev->i2c_dev), LPS22HB_WHO_AM_I, &lps22hb_id, 1);
    AM_DBG_INFO("LPS22HB_ID = 0x%x \r\n",lps22hb_id); 

    /* ������ѹ���¶��������Ƶ�ʡ���ͨ�˲� */
    ctrl_reg1 = (p_devinfo->output_data_rate 
              | p_devinfo->enable_lpfp 
              | p_devinfo->lpfp_bw);
    am_i2c_write(&(p_dev->i2c_dev), LPS22HB_CTRL_REG1, &ctrl_reg1, 1);
    
    return p_dev;
}

/**
 * \brief ��ȡ�¶ȼĴ���
 */
int am_lsp22hb_read_temp (am_lps22hb_handle_t handle, int16_t *temp)     
{
    uint8_t status    = 0;
    uint8_t temp_low  = 0;
    uint8_t temp_high = 0;
    int     ret       = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev);  
    
    /* ��ȡ״̬�Ĵ��� */ 
    ret = am_i2c_read(p_i2c_dev, LPS22HB_STATUS, &status, 1);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    }  
    
    if (status & STATUS_T_DA) {
        
        ret = am_i2c_read(p_i2c_dev, LPS22HB_TEMP_OUT_L, &temp_low, 1);
        /* ��鷢���Ƿ�ʧ�� */
        if (ret != AM_OK) {
            return ret;
        }
        
        ret = am_i2c_read(p_i2c_dev, LPS22HB_TEMP_OUT_H, &temp_high, 1);
        /* ��鷢���Ƿ�ʧ�� */
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
 * \brief ��ȡѹ���Ĵ���
 */
int am_lsp22hb_read_press (am_lps22hb_handle_t handle, uint32_t *press)    
{
    uint8_t   status    = 0;
    uint8_t   press_xl  = 0;
    uint8_t   press_l   = 0;
    uint8_t   press_h   = 0;
    int       ret       = AM_OK;
    
    /* I2C�豸ָ�� */
    am_i2c_device_t *p_i2c_dev = &(handle->i2c_dev); 
    
    /* ��ȡ״̬�Ĵ��� */ 
    ret = am_i2c_read(p_i2c_dev, LPS22HB_STATUS, &status, 1);
    
    /* ��鷢���Ƿ�ʧ�� */
    if (ret != AM_OK) {
        return ret;
    } 
    
    /* ѹ��ת����� */
    if (status & STATUS_P_DA) {
        am_i2c_read(p_i2c_dev, LPS22HB_PRESS_OUT_XL, &press_xl, 1);
        am_i2c_read(p_i2c_dev, LPS22HB_PRESS_OUT_L, &press_l, 1);  
        am_i2c_read(p_i2c_dev, LPS22HB_PRESS_OUT_H, &press_h, 1);  
    }
    /* ת����ʵ��ѹ��ֵ */
    *press = (uint32_t)(((uint32_t)(press_h) << 16) 
           | ((uint32_t)(press_l) << 8) | ((uint32_t)(press_xl))); 
    *press = (double)(*press) / 4096.0; 
    
    return  ret;
}


/* end of file */ 

