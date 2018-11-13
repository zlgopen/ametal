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
 * \brief 程序清单6.1105
 *
 * \note 该历程需要用到miniport拓展板,可以用LED看效果
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-19  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_zlg72128.h"
#include "am_delay.h"
#include "am_hwconf_zlg72128.h"

struct key_event_info {                        // 按键事件信息结构体类型定义
    am_bool_t    key_event;                    // 是否有按键事件
    uint8_t      key_val;                      // 普通按键键值
    uint8_t      repeat_cnt;                   // 普通按键重复计数
    uint8_t      funkey_val;                   // 功能键键值
};
struct key_event_info g_key_event_info;        // 结构体变量，用以保存按键信息

// 自定义按键处理回调函数
static void zlg72128_key_callback(void      *p_arg,      // 用户参数，注册回调函数时指定
                                  uint8_t    key_val,    // 普通按键键值
                                  uint8_t    repeat_cnt, // 连击计数器
                                  uint8_t    funkey_val) // 功能键键值
{                                                        
    if (g_key_event_info.key_event == AM_FALSE)  {       // 无按键事件待处理，填充新的键值
        g_key_event_info.key_val     =  key_val;
        g_key_event_info.repeat_cnt  =  repeat_cnt;
        g_key_event_info.funkey_val  =  funkey_val;
        g_key_event_info.key_event   =  AM_TRUE ;
    }
}


static void digitron_test_process (am_zlg72128_handle_t handle)
{
    am_zlg72128_digitron_disp_test(handle);    // 测试命令
    am_mdelay(3000);                           // 延时3s,使能测试命令保存3s
    am_zlg72128_digitron_disp_reset(handle);   // 复位数码管显示
}

static void normal_key_test_process (am_zlg72128_handle_t   handle, // ZLG72128操作句柄
                                     uint8_t               key_val) // 普通键键值
{
    // 数码管1显示十位，十位为0时不显示（显示空格），不显示小数点，不闪烁
    if (g_key_event_info.key_val / 10 != 0) {
        am_zlg72128_digitron_disp_num(handle, 1,key_val / 10, AM_FALSE,AM_FALSE );
    } else {
        am_zlg72128_digitron_disp_char(handle, 1, ' ',AM_FALSE,AM_FALSE );
    }
    // 在数码管0显示个位，不显示小数点，不闪烁
    am_zlg72128_digitron_disp_num(handle, 0, key_val % 10, AM_FALSE,AM_FALSE );   
}

static void combination_key_process(am_zlg72128_handle_t   handle,       // ZLG72128操作句柄
                                    uint8_t                key_val,      // 普通键键值
                                    uint8_t                funckey_val)  // 功能键键值
{
    static uint16_t flash = 0x0000;               // 初始时，所有未均不闪烁
    if ((funckey_val & (1 << 0)) == 0) {          // F0 按下
        switch (key_val) {
           case 1:                // 循环左移动  
                 am_zlg72128_digitron_shift(
                     handle,
                     AM_ZLG72128_DIGITRON_SHIFT_LEFT,
                     AM_TRUE, 
                     1);
           break;
         case 2:                // 循环右移动
             am_zlg72128_digitron_shift(
                 handle, 
                 AM_ZLG72128_DIGITRON_SHIFT_RIGHT,
                 AM_TRUE, 
                 1);
             break;
         case 3:                // 打开/关闭闪烁
           flash = ~flash;      // 取反，所有为闪烁状态改变
           am_zlg72128_digitron_flash_ctrl(handle, flash);
            break;
         default:
           break;
        }
    }
}

int am_main (void)
{
    am_zlg72128_handle_t zlg72128_handle = am_zlg72128_inst_init();
    am_zlg72128_key_cb_set(zlg72128_handle, zlg72128_key_callback, NULL);// 注册按键回调函数
    digitron_test_process(zlg72128_handle);                              // 数码管显示测试
    while (1) {
        if (g_key_event_info.key_event == AM_TRUE) {
                 // 仅普通按键按下，按键测试程序
            if ((g_key_event_info.funkey_val == 0xFF) && (g_key_event_info.key_val != 0)) {
                normal_key_test_process(zlg72128_handle, g_key_event_info.key_val);
            }
              // 组合键按下
            if ((g_key_event_info.funkey_val != 0xFF) && (g_key_event_info.key_val != 0)) {
                combination_key_process(
                    zlg72128_handle, g_key_event_info.key_val, g_key_event_info.funkey_val);
            }
            g_key_event_info.key_event = AM_FALSE;
        }
    }
}



/* end of file */

