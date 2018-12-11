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
 * \brief fm175xx对CPU卡进行测试，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口；
 *   2. 正确连接好天线；
 *   3. 将CPU卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 将卡片放置天线感应区后串口打印出卡片的ID；
 *   2. 如果放置的卡片是CPU卡，将打印ATS和COS指令的操作结果。
 *
 * \par 源代码
 * \snippet demo_amks16rfid8_fm175xx_cpu_card.c src_amks16rfid8_fm175xx_cpu_card
 *
 * \internal
 * \par Modification history
 * - 1.00 18-02-06  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_fm175xx_cpu_card
 * \copydoc demo_amks16rfid8_fm175xx_cpu_card.c
 */

/** [src_amks16rfid8_fm175xx_cpu_card] */
#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_vdebug.h"
#include "kl26_pin.h"
#include "am_kl26_inst_init.h"
#include "am_delay.h"
#include "am_led.h"
#include "am_fm175xx_ant.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_amks16rfid8_entries.h"

/**
 * \name 定义CD4051的引脚
 * @{
 */
#define __CD4051_PIN_EN PIOB_19
#define __CD4051_PIN_S2 PIOB_18
#define __CD4051_PIN_S1 PIOB_17
#define __CD4051_PIN_S0 PIOB_16

/* 定义 fm175xx 天线切换信息 */
static am_antenna_info_t  __g_antenna_info = {
    {
        __CD4051_PIN_EN,
        __CD4051_PIN_S0,
        __CD4051_PIN_S1,
        __CD4051_PIN_S2
    },
    AM_FM175XX_ANTENNA_EIGHT
};

/**
 * \brief CPU卡测试例程
 */
void demo_amks16rfid8_fm175xx_cpu_card (void)
{
    uint8_t tag_type[2]  = { 0 };      /* ATQA */
    uint8_t uid[10]      = { 0 };      /* UID */
    uint8_t uid_real_len = 0;          /* 接收到的UID的长度 */
    uint8_t sak[3]       = { 0 };      /* SAK */
    uint8_t i, j;
    
    /* cpu卡操作时用到以下参数 */    
    uint8_t  cid         = 0x00;       /* FSDI/CID */     
    uint8_t  ats[20]     = { 0 };      /* 存放RATS指令的ATS回应 */    
    uint8_t  sellect_MF[7]    = {0x00 ,0xA4 ,0x00 ,0x00 ,0x02 ,0x3F ,0x00};  /* 选择主文件命令 */ 
    uint32_t ats_nbytes  = 0;          /* ATS回应数据的长度 */  
    uint8_t  rbuf[200]   = {0};        /* 文件操作时，用作数据接收缓冲区 */  
    uint32_t r_nbyte     = 0;          /* 每次接收文件时，接收到的数据长度 */ 

    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    am_cd4051_pin_init(&__g_antenna_info);

    while (1) {

        /* 选择天线1 */
        am_cd4051_channel_selected(&__g_antenna_info, 0);
        
        if (AM_FM175XX_STATUS_SUCCESS == am_fm175xx_picca_active\
                                         (handle,
                                          AM_FM175XX_PICCA_REQ_IDLE,
                                          tag_type,
                                          uid,
                                          &uid_real_len,
                                          sak)) {

            am_kprintf("antenna 1 working !\r\n");
            am_kprintf("UID : ");
            for (i = 0; i < uid_real_len; i++) {
                am_kprintf("%02x ", uid[i]);
            }
            am_kprintf("\r\n");

            if(AM_FM175XX_STATUS_SUCCESS == am_fm175xx_picca_rats_get(
                                            handle,
                                            cid,
                                            ats,
                                            20,
                                            &ats_nbytes
                                                )) {
                 am_kprintf("ATS data: ");                                          
                 for (j = 0; j < ats_nbytes; j++) {
                     am_kprintf(" %02x ",ats[j]);
                 }   
                 am_kprintf("\r\n");
                 
                 if(AM_FM175XX_STATUS_SUCCESS == am_fm175xx_picca_transfer(
                                                 handle,
                                                 sellect_MF,
                                                 7,          /* 长度为COS指令的长度，此处为“选择主文件00 A4 00 00 02 3F 00“命令的长度*/            
                                                 rbuf,
                                                 200,       
                                                 &r_nbyte )) {
                                                     
                     am_kprintf("send data : "); 
                     for (j = 0; j < 7; j++) {
                         am_kprintf(" %02x ",sellect_MF[j]);
                     } 
                     am_kprintf("\r\n");  
                     
                     am_kprintf("return data : ");                                                                    
                     for (j = 0; j < r_nbyte; j++) {
                         am_kprintf(" %02x ",rbuf[j]);
                     }
                     am_kprintf("\r\n");        
                 }
                 else {
                     am_kprintf("sellect_MF err!"); 
                 }
             }
             else {
                 am_kprintf("RATS err!\r\n");  
             }                                               
             
//            am_fm175xx_picca_halt(&fm175xx_dev);  
              /* 选择天线2，实际作用对天线1起到关闭作用，对卡片重新上电复位 */
              am_cd4051_channel_selected(&__g_antenna_info, 1);
              am_kprintf("\r\n\r\n\r\n\r\n");              
        } else {
            am_led_off(j);
        }        
        am_mdelay(500);  
      
    }
}

/** [src_amks16rfid8_fm175xx_cpu_card] */

/* end of file */
