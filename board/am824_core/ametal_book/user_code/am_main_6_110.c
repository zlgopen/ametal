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
 * \brief 程序清单6.110
 *
 * \note 该历程需要用到第三方库，hash table
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-25  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_zlg600.h"
#include "am_hwconf_zlg600.h"
#include "am_led_dev.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "string.h"

static void __zlg600_callback (void *p_arg)
{
	// 在设置回调函数时，虽然p_arg的值设置为 detect_flag 的地址，而实际上p_arg是指向
	// detect_flag的指针，因此需要将其转换为uint8_t *类型的指针，并修改detect_flag的值为1
    	*(uint8_t *)p_arg = 1;
}

static int __zlg600_rw_test(
	am_zlg600_handle_t           	zlg600_handle,
	am_zlg600_mifare_card_info_t   *p_card_info)
{
    int      	ret;
    uint8_t 	*p_uid;
    int      	i;
    uint8_t  	key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t  	buf[16];

    for (i = 0; i < 16; i++)		buf[i] = i;
    p_uid = &( p_card_info->uid[p_card_info->uid_len - 4]);
    ret = am_zlg600_mifare_card_direct_authent(
		zlg600_handle,
          AM_ZLG600_IC_KEY_TYPE_A,
          p_uid,
          key,
          6,
          1);
    if (ret == AM_OK) {
        AM_DBG_INFO("验证成功！\r\n");
    } else {
        AM_DBG_INFO("验证失败！\r\n");
        return ret;
    }
    am_zlg600_mifare_card_write(zlg600_handle, 1, &buf[0]);
  	for (i = 0; i < 16; i++) 		buf[i] = 0;
    am_zlg600_mifare_card_read(zlg600_handle, 1, &buf[0]);
    for (i = 0; i < 16; i++) {
        if (buf[i] != i) {
            return AM_ERROR;
        }
    }
    return AM_OK;
}

const am_zlg600_auto_detect_info_t g_zlg600_auto_detect_info = {
    AM_ZLG600_MIFARE_CARD_AD_HALT,     	// 挂起
    AM_ZLG600_MIFARE_CARD_TX_ALL,		// 同时使用TX1和TX2天线
    AM_ZLG600_MIFARE_CARD_REQ_IDLE,		// 仅对空间的卡检测
    AM_ZLG600_MIFARE_CARD_AUTH_NO,     	// 无需验证
    AM_ZLG600_IC_KEY_TYPE_A,				// 验证类型，无需设置
    {0},									// 密钥，无需设置
    0,									// 密钥长度，无需设置
    0									// 验证块号，无需设置
};

int am_main(void)
{
    int 		ret, i;
    uint8_t	detect_flag	= 0;
    am_zlg600_mifare_card_info_t        card_info;
    am_zlg600_handle_t 	zlg600_handle 	= am_zlg600_uart_inst_init();// 获取ZLG600实例句柄
	// 设置自动检测回调函数，p_arg的值设置为detect_flag的地址
    am_zlg600_mifare_card_auto_detect_cb_set(zlg600_handle, __zlg600_callback, 	&detect_flag);
    // 启动自动检测
    am_zlg600_mifare_card_auto_detect_start(zlg600_handle, &g_zlg600_auto_detect_info);
    while (1) {
		if (detect_flag == 1) {					// 检测到卡片
			detect_flag = 0;					// 清0标志，以便后续继续检测卡片
           	ret = am_zlg600_mifare_card_auto_detect_read(zlg600_handle, &card_info);
            	if (ret == AM_OK) {
                	AM_DBG_INFO("检测到卡，卡ID为 %d bytes ： \r\n", card_info.uid_len);
                	for (i = 0; i < card_info.uid_len; i++)
					AM_DBG_INFO("%02x   ", card_info.uid[i]);
                	if (__zlg600_rw_test(zlg600_handle, &card_info) != AM_OK)
					AM_DBG_INFO("读写测试出错！");
                	AM_DBG_INFO ("\r\n");
            		am_led_toggle(1);
            	}
            	// 再次启动自动检测
            	am_zlg600_mifare_card_auto_detect_start(zlg600_handle, &g_zlg600_auto_detect_info);
        }
        am_led_toggle(0);		am_mdelay(200);
    }
}
/* end of file */
