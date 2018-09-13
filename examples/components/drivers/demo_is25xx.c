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
 * \brief IS25XX ����
 *
 * - ʵ������
 *   1. ���ڴ�ӡ�����Խ����
 *
 * \par Դ����
 * \snippet demo_is25xx.c src_is25xx
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-03  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_is25xx
 * \copydoc demo_is25xx.c
 */

/** [src_is25xx] */
#include "ametal.h"
#include "am_delay.h"
#include "am_is25xx.h"
#include "am_vdebug.h"

#define __IS25_BUF_SIZE 512 /**< \brief ��������С */

/**
 * \brief �������
 */
void demo_is25xx_entry (am_is25xx_handle_t is25xx_handle, int32_t test_lenth)
{
    int     ret;
    uint16_t i;
    uint8_t wr_buf[__IS25_BUF_SIZE] = {0}; /* д���ݻ��涨�� */
    uint8_t rd_buf[__IS25_BUF_SIZE] = {0}; /* �����ݻ��涨�� */

    if (__IS25_BUF_SIZE < test_lenth) {
        test_lenth = __IS25_BUF_SIZE;
    }

    /* ��䷢�ͻ����� */
    for (i = 0;i < test_lenth; i++) {
        wr_buf[i] = i;
        rd_buf[i] = 0;
    }

    /* �������� */
    ret = am_is25xx_erase(is25xx_handle, 0x001000, test_lenth);
    if (ret != AM_OK) {
        AM_DBG_INFO("am_is25xx_erase error(id: %d).\r\n", ret);
        return;
    }
    
    /* д���� */
    ret = am_is25xx_write(is25xx_handle, 0x001000, &wr_buf[0], test_lenth);

    if (ret != AM_OK) {
        AM_DBG_INFO("am_is25xx_write error(id: %d).\r\n", ret);
        return;
    }
    am_mdelay(5);
    
    /* ������ */
    ret = am_is25xx_read(is25xx_handle, 0x001000, &rd_buf[0], test_lenth);

    if (ret != AM_OK) {
        AM_DBG_INFO("am_is25xx_read error(id: %d).\r\n", ret);
        return;
    }

    /* У��д��Ͷ�ȡ�������Ƿ�һ�� */
    for (i = 0; i < test_lenth; i++) {
        AM_DBG_INFO("Read FLASH the %2dth data is %2x\r\n", i ,wr_buf[i]);

        /* У��ʧ�� */
        if(wr_buf[i] != rd_buf[i]) {
            AM_DBG_INFO("verify failed at index %d.\r\n", i);
            break;
        }
    }

    if (test_lenth == i) {
        AM_DBG_INFO("verify success!\r\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_is25xx] */

/* end of file */
