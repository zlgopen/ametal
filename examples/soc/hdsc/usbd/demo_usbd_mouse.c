/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief USB模拟U盘例程，通过driver层的接口实现
 *
 * - 操作步骤：
 *   1. 将USB连接上电脑后下载程序；
 *   2. 在电脑上会显示出一个盘符；
 *
 * - 实验现象：
 *   1. 打开盘符，可以看到里面有一个README.TXT文件。
 *   2. 可以往U盘中里面拖动文件,串口会显示你拖动文件的信息。
 *
 * \note
 *
 *
 *
 * \par 源代码
 * \snippet demo_usbd_msc.c src_usbd_msc
 *
 * \internal
 * \par Modification History
 * - 1.00 19-01-15  adw, first implementation
 * \endinternal
 */
/**
 * \brief 例程入口
 */
#include "demo_hc32f460_entries.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_core_entries.h"
#include "am_usbd_msc.h"
#include "am_vdebug.h"
#include "hc32f460_pin.h"
#include "hc32f460_periph_map.h"
#include "am_usb_dci.h"
#include "usb_core.h"

extern am_usbd_dev_t am_hc32f460_usb_mouse_inst_init (void);

#define CURSOR_STEP     10u

#define LEFT_PIN  PIOD_5  /**< \brief 左移引脚 */
#define UP_PIN    PIOD_4   /**< \brief 上移引脚 */
#define DOWN_PIN  PIOD_6   /**< \brief 下移引脚 */
#define RIGHT_PIN PIOD_3   /**< \brief 右移引脚 */

static uint8_t hid_buf[4] = {0, 0, 0, 0};

void usbd_hid_key_init (void *p_hw_gpio,
               int32_t  input_pin)
{
    amhw_hc32f460_gpio_t *p_gpio = (amhw_hc32f460_gpio_t *)p_hw_gpio;

    /* 取消引脚的复用功能 */
    amhw_hc32f460_gpio_pin_afr_set(p_gpio, AMHW_HC32F460_AFIO_GPIO, input_pin);

    /* 数字端口设置*/
    amhw_hc32f460_gpio_pin_digital_set(p_gpio, input_pin);

    /* 引脚输入 */
    amhw_hc32f460_gpio_pin_dir_input(p_gpio, input_pin);

    /* 使能引脚的上拉电阻 */
    amhw_hc32f460_gpio_pin_pu_enable(p_gpio, input_pin);   
}

am_local uint8_t* usbd_hid_get_pos(void    *p_hw_gpio)
{
    amhw_hc32f460_gpio_t *p_gpio = (amhw_hc32f460_gpio_t *)p_hw_gpio;

    hid_buf[0]=0;
    hid_buf[1]=0;
    hid_buf[2]=0;
    hid_buf[3]=0;

    if(amhw_hc32f460_gpio_pin_get(p_gpio, UP_PIN)==0){
        hid_buf[1] -= CURSOR_STEP;
    }
    if(amhw_hc32f460_gpio_pin_get(p_gpio, DOWN_PIN)==0){
        hid_buf[1] += CURSOR_STEP;
    }   
    if(amhw_hc32f460_gpio_pin_get(p_gpio, LEFT_PIN)==0){
        hid_buf[2] += CURSOR_STEP;
    }
    if(amhw_hc32f460_gpio_pin_get(p_gpio, RIGHT_PIN)==0){
        hid_buf[2] -= CURSOR_STEP;
    }
    return hid_buf;
}


void usbd_mouse_hanle(am_usbd_dev_t *p_handle)
{
    uint8_t *buf;

    buf = usbd_hid_get_pos((void *)HC32F460_GPIO);
    if((buf[1] != 0u) ||(buf[2] != 0u))
    {
        am_usbd_send(p_handle, 0x81u, buf, 4);
    }    
    am_mdelay(20);
}

/**
 * \brief 例程入口
 */
void demo_usbd_mouse_entry (void* p_handle)
{
    /* usb 鼠标实例初始化*/    
    usbd_hid_key_init ((void *)HC32F460_GPIO, LEFT_PIN);
    usbd_hid_key_init ((void *)HC32F460_GPIO, UP_PIN);
    usbd_hid_key_init ((void *)HC32F460_GPIO, DOWN_PIN);
    usbd_hid_key_init ((void *)HC32F460_GPIO, RIGHT_PIN);    
    while (1) {
        usbd_mouse_hanle(p_handle);
    }  
}
/**
 * \addtogroup demo_if_usbd_msc
 * \copydoc demo_usbd_msc.c
 */

/** [src_usbd_msc] */

/* end of file */
