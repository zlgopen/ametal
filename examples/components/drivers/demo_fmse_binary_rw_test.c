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
 * \brief 安全芯片FMSE-A03 例程，通过标准接口实现
 *
 * - 实验现象：
 *
 * \par 源代码
 * \snippet demo_fmse_binary_rw_test.c src_fmse_binary_rw_test
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-01  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_fmse_binary_rw_test
 * \copydoc demo_fmse_binary_rw_test.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_fmse_a03.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_fmse_binary_rw_test_entry (am_fmse_handle_t handle)
{
    uint8_t  i, j;
    uint8_t  bt_rbuf[256];
    uint8_t  bt_inbuf[256];   
    uint16_t bt_rlen   = 0;
    uint16_t bt_sw     = 0;    
    uint16_t file_index[6] ={0x0003, 0x0004, 0x0005, 0x0006, 0x0B01, 0x0B91}; 
    uint8_t  pin[4];

    /* 选择 SE APP */ 
    bt_sw = am_fmse_file_select(handle, 0xDF01, bt_rbuf, &bt_rlen);
    if(bt_sw != 0x9000) {
        printf("1. file selection failed\r\n");
    }

    /* SE芯片出厂默认PIN值 */
    pin[0] = 0x11;
    pin[1] = 0x22;
    pin[2] = 0x33;

    bt_sw = am_fmse_pin_verify(handle, 3, pin);
    if(bt_sw != 0x9000) {
        printf("2. pin verify failed\r\n");
    }

    /** 
    * \brief  0x0004文件需要进行外部认证才能写操作，
    * 否则只能读，写操作会显示权限不足的返回状态
    */

    printf("\r\n\r\n");  
    for (j = 0; j < 6; j++) {

        printf("** binary file %x test! ** \r\n",file_index[j]);

        /* 选择文件，这个文件也是默认存在的，出厂即有 */
        bt_sw = am_fmse_file_select(handle, file_index[j], bt_rbuf, &bt_rlen);                  
        if(bt_sw != 0x9000) {
            printf("3. file selection failed\r\n");
        }
        printf("select file: sw = %04x, rlen = %d\r\n", bt_sw, bt_rlen);
        for(i = 0; i < bt_rlen; i++) {
            printf("%02x,",bt_rbuf[i]);
        }
        printf("\r\n");        

        bt_sw = am_fmse_binary_read(handle, 0, 0x80, bt_rbuf, &bt_rlen);                
        if(bt_sw != 0x9000) { 
            printf("4. binary reading failed\r\n");
        }
        printf("read file: sw = %04x, rlen = %d\r\n",bt_sw,bt_rlen);          
        for(i = 0;i < bt_rlen; i++) {
            printf("%02x,",bt_rbuf[i]);
        }
        printf("\r\n");

        am_fmse_memset(bt_inbuf, 0xff, 64);
        bt_sw = am_fmse_binary_write(handle,
                                     0,
                                     0x40,
                                     bt_inbuf,
                                     bt_rbuf,
                                    &bt_rlen);
        if(bt_sw != 0x9000) { 
            printf("5. binary writing failed\r\n");
        }
        printf("write new data to file: sw = %04x, rlen = %d\r\n",bt_sw,bt_rlen);          
        for(i = 0; i < bt_rlen; i++) {
            printf("%02x,",bt_rbuf[i]);
        }
        printf("\r\n");

        bt_sw = am_fmse_binary_read(handle, 0, 0x80, bt_rbuf, &bt_rlen);                
        if(bt_sw != 0x9000) { 
            printf("6. binary reading failed\r\n");
        }
        printf("read file: sw = %04x, rlen = %d\r\n",bt_sw,bt_rlen);          
        for(i = 0; i < bt_rlen; i++) {
            printf("%02x,",bt_rbuf[i]);
        }
        printf("\r\n");        

        am_fmse_memset(bt_inbuf, 0x00, 64);
        bt_sw = am_fmse_binary_write(handle, 0, 0x40, bt_inbuf, bt_rbuf, &bt_rlen);                
        if(bt_sw != 0x9000) { 
            printf("7. binary writing failed\r\n");
        }
        printf("write default data to file: sw = %04x, rlen = %d\r\n", bt_sw, bt_rlen);          
        for(i = 0; i < bt_rlen; i++) {
            printf("%02x,",bt_rbuf[i]);
        }
        printf("\r\n\r\n\r\n");               
    } 
}

/* end of file */
