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
 * \brief USB模拟鼠标例程，通过driver层的接口实现
 *
 * - 操作步骤：
 *   1. 将USB连接上电脑后下载程序；
 *
 * - 实验现象：
 *   1. 按下按键，鼠标移动
 *
 * \par 源代码
 * \snippet demo_hc32f07x_core_usbd_mouse.c src_hc32f07x_core_usbd_mouse
 *
 * \internal
 * \par Modification History
 * - 1.00 19-05-28  htf, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_demo_hc32f07x_core_usbd_mouse
 * \copydoc demo_hc32f07x_core_usbd_mouse.c
 */

/** [src_demo_hc32f07x_core_usbd_mouse] */

#include "ametal.h"
#include "am_board.h"
#include "am_gpio.h"
#include "am_hc32f07x.h"
#include "am_hc32f07x_inst_init.h"
#include "demo_std_entries.h"
#include "demo_hc32_entries.h"
#include "demo_amf07x_core_entries.h"
#include "hw/amhw_hc32_gpio.h"

#define LEFT_PIN  PIOC_12  /**< \brief 左移引脚 */
#define UP_PIN    PIOD_3   /**< \brief 上移引脚 */
#define DOWN_PIN  PIOD_4   /**< \brief 下移引脚 */
#define RIGHT_PIN PIOD_2   /**< \brief 右移引脚 */
#define PRESS_PIN PIOD_6   /**< \brief 右移引脚 */

#define CURSOR_STEP     2   //鼠标移动最小单位

static uint8_t Mouse_Buffer[4] = {0, 0, 0, 0};

void key_init (void    *p_hw_gpio,
               int32_t  input_pin)
{
    amhw_hc32_gpio_t *p_gpio = (amhw_hc32_gpio_t *)p_hw_gpio;

    am_clk_enable(CLK_GPIO);

    /* 取消引脚的复用功能 */
    amhw_hc32_gpio_afio_set(p_gpio, AMHW_HC32_AFIO_NO, input_pin);

    /* 数字端口设置*/
    amhw_hc32_gpio_pin_digital_set(p_gpio, input_pin);

    /* 引脚输入 */
    amhw_hc32_gpio_pin_dir_input(p_gpio, input_pin);

    /* 使能引脚的上拉电阻 */
    amhw_hc32_gpio_pin_pu_enable(p_gpio, input_pin);   
}

void key_get (void    *p_hw_gpio)
{
    amhw_hc32_gpio_t *p_gpio = (amhw_hc32_gpio_t *)p_hw_gpio;

    Mouse_Buffer[0]=0;
    Mouse_Buffer[1]=0;
    Mouse_Buffer[2]=0;
    Mouse_Buffer[3]=0;

  if(amhw_hc32_gpio_pin_input_get(p_gpio, LEFT_PIN)==0){
        Mouse_Buffer[1]-=CURSOR_STEP;
    }
    if(amhw_hc32_gpio_pin_input_get(p_gpio, RIGHT_PIN)==0){
        Mouse_Buffer[1]+=CURSOR_STEP;
    }
    if(amhw_hc32_gpio_pin_input_get(p_gpio, UP_PIN)==0){
        Mouse_Buffer[2]-=CURSOR_STEP;
    }
    if(amhw_hc32_gpio_pin_input_get(p_gpio, DOWN_PIN)==0){
        Mouse_Buffer[2]+=CURSOR_STEP;
    }
    if(amhw_hc32_gpio_pin_input_get(p_gpio, PRESS_PIN)==0){
        Mouse_Buffer[0]=1;
    }
}

void demo_hc32f07x_usbd_mouse_entry (void)
{
    AM_DBG_INFO("demo hc32f07x usbd mouse!\r\n");

    /* 开启系统配置的时钟 */
    am_clk_enable(CLK_GPIO);

    key_init ((void *)HC32_GPIO0, LEFT_PIN);
    key_init ((void *)HC32_GPIO0, UP_PIN);
    key_init ((void *)HC32_GPIO0, DOWN_PIN);
    key_init ((void *)HC32_GPIO0, RIGHT_PIN);
    key_init ((void *)HC32_GPIO0, PRESS_PIN);

     /* usb 鼠标实例初始化*/
     am_usbd_dev_t handle = am_hc32f07x_usbd_mouse_inst_init();

     /* 等待初始化完成 */	
     am_mdelay(3000);

    while (1) {
        key_get ((void *)HC32_GPIO0);
        am_usbd_send(&handle, 1, Mouse_Buffer, 4);
        am_udelay(100);
    }
}

/** [src_demo_hc32f07x_core_usbd_mouse] */

/* end of file */
