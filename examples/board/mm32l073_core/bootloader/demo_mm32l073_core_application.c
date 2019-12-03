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
 * \brief bootloader 例程，本demo是作为应用程序。
 *
 * - 操作步骤：
 *   参考AMmetal-AM116-Core-bootloader操作手册
 *
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_hw_adc_int.c src_mm32l073_core_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-18  yrh, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_application
 * \copydoc demo_mm32l073_core_application.c
 */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_boot.h"
#include "am_uart.h"
#include "am_boot_firmware.h"
#include "am_boot.h"
#include "am_bootconf_mm32l073.h"
#include "am_mm32l073_inst_init.h"
#include <string.h>


/**
 * \name 数据接收状态
 * @{
 */
/** \brief 接收命令数据阶段 */
#define  COMMAND_PHASE                   0

/** \brief 接收固件数据阶段 */
#define  FIRMWARE_PHASE                  1

/** \brief 接收固件头部数据阶段 */
#define  FIRMWARE_HEAD                   2
/** @} */

/** \brief 固件接受超时时间 */
#define  FIRMWARE_RECEIVE_TIMEOUT_VALUE  15000

/** \brief 串口接收缓冲区大小 */
#define UART_CALLBACK_BUF_SIZE           64

/** \brief 用户命令长度 */
#define USER_COMMAND_LEN                 5

/** \brief 用户可以定义的命令, 命令以0x5a,0xa6作为帧头，命令以0x0d结尾，可以自定义中间的两个个字符  */
static char user_command[5] = {0x5a, 0xa6, 0x11, 0x66, 0x0d};

/** \brief 用来存放接收的命令 */
static char command_rec[6] = {0};

/** \brief 数据接收状态 */
static uint8_t int_state = COMMAND_PHASE;
/** \brief 串口数据接收缓冲区  */
static uint8_t uart_callback_buffer[UART_CALLBACK_BUF_SIZE] = {0};

static uint8_t index = 0, command_error = 0, firmware_timeout = 0;

/** \brief 固件存放操作句柄  */
static am_boot_firmware_handle_t firmware_handle;

/** \brief flash操作句柄  */
static am_boot_flash_handle_t    flash_handle;

/** \brief led软件定时器结构体  */
static am_softimer_t             led_timer;

/** \brief 数据接收超时软件定时器结构体  */
static am_softimer_t             receive_callback_timer;

static uint32_t  write_offset = 0, read_offset = 0;

/**
 * \brief 串口接收回调函数
 */
void ___pfn_uart_rec_callback(void *p_arg, char inchar)
{
    if(int_state == COMMAND_PHASE) {
        if(index < sizeof(command_rec) ) {
            command_rec[index++] = inchar;
            am_softimer_start(&receive_callback_timer, 500);
        }
    } else {
        uart_callback_buffer[write_offset++] = (uint8_t)inchar;
        write_offset &= UART_CALLBACK_BUF_SIZE - 1;
    }
}
static void __delay(void)
{
    int i = 1000000;
    while(i--);
}
/**
 * \brief  从串口接收缓冲区读数据
 */
static int __read_data(uint8_t *p_buffer, uint32_t byte_count)
{
    uint32_t current_bytes_read = 0;
    am_softimer_start(&receive_callback_timer, FIRMWARE_RECEIVE_TIMEOUT_VALUE);
    while (current_bytes_read != byte_count) {
        if(firmware_timeout == 1) {
            firmware_timeout = 0;
            am_softimer_stop(&receive_callback_timer);
            return AM_ERROR;
        }

        if(read_offset != write_offset) {
            p_buffer[current_bytes_read++] = uart_callback_buffer[read_offset++];
            read_offset &= UART_CALLBACK_BUF_SIZE - 1;
        }
    }
    return AM_OK;
}
/**
 * \brief led软件定时器回调函数
 * */
static void __led_timer_handle (void *p_arg)
{
    am_led_toggle(LED0);
}
/**
 * \brief 串口接收数据软件定时器回调函数
 */
static void __receive_callback_timer_handle(void *p_arg)
{
    int i;
    if(int_state == COMMAND_PHASE) {
        /* \brief 接收的命令字符个数出错  */
        if(index != USER_COMMAND_LEN) {
            command_error = 1;
        } else {
            for(i = 0; i < USER_COMMAND_LEN; i++) {
                if(user_command[i] != command_rec[i]) {
                    command_error = 1;
                    break;
                }
            }
            /* 接收到用户的命令正确 ，则进入固件接收状态 */
            if(i == USER_COMMAND_LEN){
                int_state = FIRMWARE_PHASE;
            }
        }
        memset(command_rec, 0, sizeof(command_rec));
        am_softimer_stop(&receive_callback_timer);
        index = 0;
    } else {
        firmware_timeout = 1;
    }
}
/**
 * \brief 检测是否有固件发送过来
 */
static void __detect_firmware_receive(void)
{
    am_boot_firmware_verify_info_t firmware_head;
    uint32_t rev, count, i;
    int ret;
    /* 命令错误处理 */
    if(command_error == 1) {
        AM_DBG_INFO("application : input command error! still execute previous application\r\n");
        command_error = 0;
        return;
    }
    /* 固件接收  */
    if(int_state == FIRMWARE_PHASE) {

       am_kprintf("application : update init...\r\n");
       /* 固件存放标准接口初始化 */
       firmware_handle = am_mm32l073_boot_firmware_flash(flash_handle);
       /* 开始固件存放  */
       am_boot_firmware_store_start(firmware_handle, 24 * 1024);
       am_softimer_start(&led_timer, 100);
       memset(uart_callback_buffer, 0, sizeof(uart_callback_buffer));
       am_kprintf("application : firmware transmission is ready\r\n");

       /* 读取固件头部  */
       if(AM_OK != __read_data((uint8_t *)&firmware_head, sizeof(firmware_head))) {
           int_state = COMMAND_PHASE;
           am_softimer_start(&led_timer, 1000);
           am_kprintf("application : firmware transmission is timeout, still execute previous application\r\n");
           return;
       }

       count = firmware_head.len / 4;
       for(i = 0; i < count; i++) {
           ret = __read_data((uint8_t *)&rev, sizeof(rev));
           if(ret != AM_OK) {
               int_state = COMMAND_PHASE;
               am_softimer_start(&led_timer, 1000);
               am_kprintf("application : firmware transmission is timeout, still execute previous application\r\n");
               return;
           }
           /* 固件存放，每次存放四字节 */
           ret = am_boot_firmware_store_bytes(firmware_handle,
                                             (uint8_t *)&rev,
                                              sizeof(rev));
           if(ret != AM_OK) {
               int_state = COMMAND_PHASE;
               am_softimer_start(&led_timer, 1000);
               am_kprintf("application : firmware transmission is error,still execute previous application\r\n");
               return;
           }
       }

       if (firmware_head.len & 0x3) {

           rev = 0xffffffff;
           __read_data((uint8_t *)&rev, firmware_head.len & 0x3);
           am_boot_firmware_store_bytes (firmware_handle,
                                        (uint8_t *)&rev,
                                         firmware_head.len & 0x3);
       }
       /* 固件存放结束  */
       am_boot_firmware_store_final(firmware_handle);
       am_kprintf("application : firmware receive successful\r\n");
       /* 固件校验  */
       ret = am_boot_firmware_verify(firmware_handle, &firmware_head);
       if(ret != AM_OK) {
           am_kprintf("application : firmware verify error\r\n");
		   int_state = COMMAND_PHASE;
		   return;
       } else {
           am_kprintf("application : firmware verify successful\r\n");
       }
       /* 设置升级标志为升级区有效  */
       am_boot_update_flag_set(AM_BOOTLOADER_FLAG_UPDATE);
       am_kprintf("application : device will restart...\r\n");

       /*在设备重启之前最好进行一小段的延时，让串口有时间将缓冲区数据处理完*/
       __delay();
       /* 重启设备  */
       am_boot_reset();
   }
}

void demo_mm32l073_core_application_entry (void)
{
    am_kprintf("application : mm32l073_core_bootloader_application start up successful!\r\n");

    am_softimer_init(&led_timer, __led_timer_handle, NULL);
    am_softimer_start(&led_timer, 1000);
    /* 用于固件接收的超时的定时器初始化 */
    am_softimer_init(&receive_callback_timer, __receive_callback_timer_handle, NULL);
    /* flash初始化  */
    flash_handle = am_mm32l073_boot_flash_inst_init();
    /* bootloader 标准接口初始化  */
    am_mm32l073_std_boot_inst_init(flash_handle);

    am_uart_handle_t uart_handle = am_mm32l073_uart1_inst_init();
    /* 使能串口中断模式 */
    am_uart_ioctl(uart_handle, AM_UART_MODE_SET, (void *)AM_UART_MODE_INT);
    /* 注册发送回调函数 */
    am_uart_callback_set(uart_handle, AM_UART_CALLBACK_RXCHAR_PUT, ___pfn_uart_rec_callback, NULL);

    while (1) {
        /* 检测是否有发送固件过来的命令 */
        __detect_firmware_receive();
    }
}
/* end of file */
