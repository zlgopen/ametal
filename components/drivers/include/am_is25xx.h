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
 * \brief ISSI��˾��IS25ϵ��IS25XXоƬ���� 
 * 
 * ��֪����оƬ�� 
 *   - IS25WP032
 *   - IS25WP064 
 *   - IS25WP0128
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-03  yrz, first implementation.
 * \endinternal
 */
 
#ifndef __AM_IS25XX_H
#define __AM_IS25XX_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "ametal.h"
#include "am_types.h"
#include "am_spi.h"
#include "am_gpio.h"
#include "am_mtd.h"
    
/**
 * \addtogroup am_if_is25xx
 * \copydoc am_is25xx.h
 * @{
 */

/**
 * \brief ����һ��оƬ�ͺ�
 *
 * \param[in] page_size        : ҳ��С��ʹ��2���ݱ�ʾ����ҳ��СΪ256�����ֵΪ8
 * \param[in] pages_in_sector  : ���������а�����ҳ��Ŀ��ʹ��2���ݱ�ʾ
 * \param[in] sectors_in_block : �������а�����������Ŀ��ʹ��2���ݱ�ʾ
 * \param[in] nblocks          : оƬ���ܿ���
 * \param[in] id               : оƬID
 *
 * \return оƬ�ͺţ�����ֱ����Ϊʵ����Ϣ�� type ��Ա��ֵ
 */
#define AM_IS25XX_TYPE_DEF(page_size,         \
                           pages_in_sector,   \
                           sectors_in_block,  \
                           nblocks,           \
                           id)                \
        {page_size, pages_in_sector, sectors_in_block, nblocks, id},

/**
 * \name ��֪��һЩоƬ�ͺŶ���
 *
 *     ��ʹ�õ�оƬ�ͺŴ˴�δ���壬����ʹ�� \sa AM_is25XX_TYPE_DEF() �궨��һ
 * ���µ�оƬ�� ��Щ�����ֱ����Ϊʵ����Ϣ�� type ��Ա��ֵ��
 * 
 * @{
 */
#define AM_IS25XX_IS25WP032     AM_IS25XX_TYPE_DEF(8, 4, 4, 6, 0x16609D)
#define AM_IS25XX_IS25WP064     AM_IS25XX_TYPE_DEF(8, 4, 4, 7, 0x17609D)
#define AM_IS25XX_IS25WP128     AM_IS25XX_TYPE_DEF(8, 4, 4, 8, 0x18609D)
        
/** @} */

/**
 * \brief �����ͺţ�����ҳ��С����Ϣ��
 */
typedef struct am_is25xx_type {

    uint8_t   page_size;        /**< \brief ҳ��С��ʹ��2���ݱ�ʾ  */
    uint8_t   pages_in_sector;  /**< \brief ��������������ҳ����ʹ��2���ݱ�ʾ */
    uint8_t   sectors_in_block; /**< \brief ��������������ҳ����ʹ��2���ݱ�ʾ */
    uint8_t   nblocks;          /**< \brief оƬ�ܿ�����ʹ��2���ݱ�ʾ  */

    /**
     * ��ID�ɣ�manufacture_id + memory type + memory density����
     * �磺IS25LP064 manufacture_id�� memory type�� memory density�ֱ�Ϊ
     * 0x9D��0x60��0x17�����ֵΪ��0x17609D
     */
    uint32_t  id;
 
} am_is25xx_type_t;
 
/**
 * \brief ISSI25XX ʵ����Ϣ
 */
typedef struct am_is25xx_devinfo {
 
    uint16_t          spi_mode;      /**< \brief ����ʹ�õ�SPIģʽ */
    int               spi_cs_pin;    /**< \brief SPIƬѡ���� */
    uint32_t          spi_speed;     /**< \brief ʹ�õ�SPI���� */
    am_is25xx_type_t  type;          /**< \brief �����ͺ� */
 
} am_is25xx_devinfo_t;
     
/**
 * \brief ISSI25XX ʵ��
 */
typedef struct am_is25xx_dev {
    am_spi_device_t            spi_dev;        /**< \brief SPI�豸 */
    uint32_t                   addr_offset;    /**< \brief ������ַ�ռ� */
    const am_is25xx_devinfo_t *p_devinfo;      /**< \brief ʵ����Ϣ */
} am_is25xx_dev_t;

/** \brief ���� ISSI25XX ��ʵ��������� */
typedef struct am_is25xx_dev *am_is25xx_handle_t;


/** 
 * \brief ��ʼ�� IS25XX����ȡ���� IS25XX �� handle
 * 
 * \param[in] p_dev      : ָ�� IS25XX ʵ����ָ��
 * \param[in] p_devinfo  : ָ�� IS25XX ʵ����Ϣ��ָ��
 * \param[in] handle     : SPI��׼����handle,ʹ�ø� handle �� IS25XX ����ͨ��
 *
 * \return IS25XX ��������������ʼ��ʧ�ܣ��򷵻�ֵΪNULL
 */
am_is25xx_handle_t am_is25xx_init(am_is25xx_dev_t            *p_dev,
                                  const am_is25xx_devinfo_t  *p_devinfo,
                                  am_spi_handle_t             handle);

/**
 * \brief IS25xx�� MTD ���ܳ�ʼ��
 *
 * ��ʼ���󣬼���ʹ��MTD��׼�ӿڶ� IS25xx���в���
 *
 * \param[in] handle         : IS25XX �������
 * \param[in] p_mtd          : ָ�� MTD ʵ����ָ��
 * \param[in] reserved_nblks : �����Ŀ�������ʼ�� reserved_nblks �鲻�ᱻMTDʹ�á�
 *
 * \return MTDʵ�����
 *
 */
am_mtd_handle_t am_is25xx_mtd_init(am_is25xx_handle_t  handle,
                                   am_mtd_serv_t      *p_mtd,
                                   uint32_t            reserved_nblks);


/**
 * \brief ����
 *
 *    ��������Ҫ�����ǽ�����ȫ������Ϊ0xFF�������д洢��Ԫ��λ����Ϊ1������������
 * ������ֱ�Ӳ���ĳ����һ��ַ��Ԫ����������С��ԪΪ��������ÿ��ֻ�ܲ�����������
 * ���������ڲ�������С��ԪΪ��������ˣ������������ʼ��ַ����Ϊĳ��������ʼ��ַ��
 * �������ȱ���Ϊ������С����������
 *
 * \param[in] handle : IS25XX �������
 * \param[in] addr   : ����������׵�ַ������Ϊĳ��������ʼ��ַ
 * \param[in] len    : ��������ĳ��ȣ�����Ϊ������С��������
 *
 * \retval  AM_OK     : �����ɹ�
 * \retval -AM_EINVAL : ����ʧ��, ��������
 * \retval -AM_EIO    : ����ʧ��, SPIͨ�ų���
 */
int am_is25xx_erase(am_is25xx_handle_t  handle,
                    uint32_t            addr,
                    uint32_t            len);


/**
 * \brief ��ȡ����
 *
 * \param[in] handle : IS25XX �������
 * \param[in] addr   : ��ȡ���ݵ��׵�ַ
 * \param[in] p_buf  : ��ȡ���ݴ�ŵĻ�����
 * \param[in] len    : ���ݶ�ȡ�ĳ���
 *
 * \retval  AM_OK     : ��ȡ���ݳɹ�
 * \retval -AM_EINVAL : ��ȡ����ʧ��, ��������
 * \retval -AM_EIO    : ��ȡ����ʧ��, SPIͨ�ų���
 */
int am_is25xx_read(am_is25xx_handle_t  handle,
                   uint32_t            addr,
                   uint8_t            *p_buf,
                   uint32_t            len);


/** 
 * \brief д������
 *
 * \param[in] handle : IS25XX �������
 * \param[in] addr   : д�����ݵ��׵�ַ
 * \param[in] p_buf  : д�����ݴ�ŵĻ�����
 * \param[in] len    : ���ݶ�ȡ�ĳ���
 *
 * \retval  AM_OK     : ��ȡ���ݳɹ�
 * \retval -AM_EINVAL : ��ȡ����ʧ��, ��������
 * \retval -AM_EIO    : ��ȡ����ʧ��, SPIͨ�ų���
 */
int am_is25xx_write(am_is25xx_handle_t  handle,
                    uint32_t            addr,
                    uint8_t            *p_buf,
                    uint32_t            len);

/**
 * \brief ��ȡ JEDEC ID
 *
 * ��ID�ɣ�manufacture_id + memory type + memory density����
 * 
 * \param[in]  handle   : IS25XX �������
 * \param[out] p_id     : JEDEC ID
 *
 * \retval  AM_OK     : ��ȡ�ɹ�
 * \retval -AM_EINVAL : ��ȡʧ��, ��������
 * \retval -AM_EIO    : ��ȡʧ��, SPIͨ�ų���
 */ 
int am_is25xx_id_read(am_is25xx_handle_t handle, uint32_t *p_id);
                                        
/** 
 * \brief ʹ��REMS�����ȡ ����ID��1�ֽڣ�������ID��1�ֽڣ�
 * 
 * \param[in]  handle           : IS25XX �������
 * \param[out] p_manufacture_id : ����ID��1�ֽڣ�
 * \param[out] p_device_id      : �豸ID��1�ֽڣ�
 *
 * \retval  AM_OK     : ��ȡ�ɹ�
 * \retval -AM_EINVAL : ��ȡʧ��, ��������
 * \retval -AM_EIO    : ��ȡʧ��, SPIͨ�ų���
 */                                        
int am_is25xx_id_rems_read(am_is25xx_handle_t  handle, 
                           uint8_t            *p_manufacture_id,
                           uint8_t            *p_device_id);
                            
/** 
 * \brief ��ȡIS25XX״̬�Ĵ�����ֵ
 * 
 * \param[in]  handle : IS25XX �������
 * \param[out] p_stat : ��ȡ����״̬�Ĵ�����ֵ
 *
 * \retval  AM_OK     : ��ȡ״̬�Ĵ����ɹ�
 * \retval -AM_EINVAL : ��ȡʧ��, ��������
 * \retval -AM_EIO    : ��ȡʧ��, SPIͨ�ų���
 */
int am_is25xx_status_read(am_is25xx_handle_t  handle,
                          uint8_t            *p_stat);

/** 
 * \brief ����IS25XX״̬�Ĵ�����ֵ
 * 
 * \param[in] handle : IS25XX �������
 * \param[in] val    : д��״̬�Ĵ�����ֵ
 *
 * \retval  AM_OK     : д��״̬�Ĵ����ɹ�
 * \retval -AM_EINVAL : д��ʧ��, ��������
 * \retval -AM_EIO    : д��ʧ��, SPIͨ�ų���
 *
 * \note ��������Ӧ�ã����򲻽���ʹ�øú���ֱ��д״̬�Ĵ�����ֵ
 */
int am_is25xx_status_write(am_is25xx_handle_t  handle,
                           uint8_t             val);

/** 
 * \brief �������ģʽ���͹��ģ�
 * 
 * \param[in] handle : IS25XX �������
 *
 * \retval  AM_OK     : �������ģʽ�ɹ�
 * \retval -AM_EINVAL : �������ģʽʧ��, ��������
 * \retval -AM_EIO    : �������ģʽʧ��, SPIͨ�ų���
 */                              
int am_is25xx_deep_power_down_enter(am_is25xx_handle_t  handle);

/** 
 * \brief �˳�����ģʽ
 * 
 * \param[in] handle : IS25XX �������
 *
 * \retval  AM_OK     : �������ģʽ�ɹ�
 * \retval -AM_EINVAL : �������ģʽʧ��, ��������
 * \retval -AM_EIO    : �������ģʽʧ��, SPIͨ�ų���
 */
int am_is25xx_deep_power_down_exit(am_is25xx_handle_t  handle);

/** 
 * \brief IS25XX���ʼ�����ͷ������Դ
 * 
 *     ���ʼ����IS25XX��غ���������ʹ�ã�����ʹ��IS25XX��ȡ
 * ����NVRAM��׼����handle��
 * 
 * \param[in] p_dev : ָ�� IS25XX �豸��ָ��
 *
 * \return ��
 */
void am_is25xx_deinit(am_is25xx_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
 
#endif /* __AM_IS25XX_H */

/* end of file */
