/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief book`s keyn driver for AM824
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "keyn.h"
#include "am_lpc82x.h"
#include "am_softimer.h"
typedef  struct  key_info {
    int              pin;                                    // 按键对应的管脚
    unsigned char    active_level;                           // 有键按下时对应的电平
} key_info_t;

static const  key_info_t g_key_info[] = {{ PIO0_1, 0}};
static const int key_num = sizeof(g_key_info)/sizeof(g_key_info[0]);    // 按键个数
static uint32_t key_last_value;                              // 上次键值
static uint32_t key_final_value;                             // 最终键值
                                                             
static uint32_t keyn_read (void)                             // 获取键值函数
{                                                            
    uint32_t key_current_value = 0;                          // 初始化键值为0
    int      i;

    for (i = 0; i < key_num; i++) {
        key_current_value |= am_gpio_get(g_key_info[i].pin) << i; // 保存状态在第i位
    }
    return key_current_value;                                // 返回当前键值
}

void keyn_init (void)
{
    int i;

    for (i = 0; i < key_num; i++) {
        am_gpio_pin_cfg(g_key_info[i].pin, AM_GPIO_INPUT);  // 保存状态在第i位
    }
    key_final_value = key_last_value = keyn_read();
}


static uint8_t keyn_val_process (uint32_t key_current_value)
{
    uint8_t    ret_flag  =   0xFF;
    uint32_t   change     =   0; 
    int       i         =   0;    
    
    if (key_last_value == key_current_value) {
        // 由于两次扫描得到的值相等，因此按键行为有效
        if (key_current_value != key_final_value) {
            //按键状态发生改变
            change = key_final_value ^ key_current_value; // 通过异或找出变化的位
            key_final_value = key_current_value;          //保存两次扫描后相等的键值
            for (i = 0; i < key_num; i++) {
                if (change & (1 << i)) {
                    // 该位有变化，状态未变化的位不做处理
                    if (((key_current_value & (1 << i)) >> i) == g_key_info[i].active_level){
                      // 有键按下
                      ret_flag = i;
                    } else {
                      ret_flag = (1 << 7) | i;         // 按键释放，最高位置1
                    }
                }
            }
        }
    }
  key_last_value = key_current_value;                   // 保存上次键值
  return ret_flag;                  // 返回按键状态标志ret_flag
}



uint8_t keyn_scan (void)                                    //按键扫描函数
{
    uint32_t  key_current_value = keyn_read();              // 读取键值
    return keyn_val_process(key_current_value);             // 处理键值，返回结果
}

static pfn_keyn_callback_t     g_keyn_callback = NULL;
static void                   *g_keyn_arg   = NULL;
static am_softimer_t           g_keyn_timer;              // 按键扫描软件定时器

static void keyn_softimer_callback(void *p_arg)
{
    uint8_t key_return = keyn_scan();
    if (key_return != 0xFF) {
        g_keyn_callback(g_keyn_arg, key_return);
    }
}

static void keyn_softimer_set (void)  
{
    am_softimer_init(&g_keyn_timer, keyn_softimer_callback, NULL);
    am_softimer_start(&g_keyn_timer,10);
}

void keyn_init_with_softimer(pfn_keyn_callback_t p_func, void *p_arg)
{
   keyn_init();
   g_keyn_callback  = p_func;
   g_keyn_arg       = p_arg;
   keyn_softimer_set();
}


/* end of file */
