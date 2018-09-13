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
 * \brief MicroPort FLASH ���̣�ͨ����׼�ӿ�ʵ��
 *
 * - �������裺
 *   1. �� MicroPort FLASH ������ӵ� AM116-Core �� MicroPort �ӿڡ�
 *
 * - ʵ������
 *   1. ����д���ݵ� FLASH��
 *   2. ������ FLASH ��ȡ���ݣ���ͨ�����ڴ�ӡ����
 *   3. ���ڴ�ӡ�����Խ����
 *
 * \note
 *    ����۲촮�ڴ�ӡ�ĵ�����Ϣ����Ҫ�� PIOA_9 �������� PC ���ڵ� RXD��
 *
 * \par Դ����
 * \snippet demo_am116_core_microport_flash.c src_am116_core_microport_flash
 *
 * \internal
 * \par Modification history
 * - 1.00  17-11-13  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am116_core_microport_flash
 * \copydoc demo_am116_core_microport_flash.c
 */

/** [src_am116_core_microport_flash] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg116_inst_init.h"
#include "demo_components_entries.h"

/**
 * \brief �������
 */
void demo_am116_core_microport_flash_entry (void)
{
    AM_DBG_INFO("demo am116_core microport flash!\r\n");
    
    /* MX25xx flash���� */
//    demo_mx25xx_entry(am_microport_flash_inst_init(), 8);

    /* IS25xx flash���� */
    demo_is25xx_entry (am_microport_flash_is25xx_inst_init(), 512);     
}
/** [src_am116_core_microport_flash] */

/* end of file */
