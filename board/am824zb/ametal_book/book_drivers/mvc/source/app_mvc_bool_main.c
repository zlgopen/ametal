#include "ametal.h"
#include "am_led.h"
#include "am_input.h"
#include "am_zm516x.h"
#include "model_bool.h"
#include "view_led.h"
#include "view_zigbee.h"
#include "app_mvc_bool_main.h"

static model_bool_t __g_model_bool;      			//定义一个布尔型实例
static void __input_key_proc (void *p_arg, int key_code, int key_state, int keep_time)
{
//    int       	code = (int)p_arg; 
    am_bool_t 	value;

    if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
        if (model_bool_get(&__g_model_bool, &value) == 0) {
            model_bool_set(&__g_model_bool, !value);
        }
    }
}

int app_mvc_bool_main (int led_id, int key_code,am_zm516x_handle_t zm516x_handle)
{
    view_led_t              view_led0;     	//定义一个LED视图实例
    view_zigbee_t           view_zigbee;   	//定义一个zigbee视图实例
    am_input_key_handler_t   key_handler;   //定义一个按键处理器

    //注册按键事件，将按键对应的编码通过传参传递给回调函数
    am_input_key_handler_register(&key_handler, __input_key_proc, (void *)key_code);
    //初始化模型,value的初值为AM_FALSE
    model_bool_init(&__g_model_bool, AM_FALSE);       
    //初始化视图实例
    view_led_init(&view_led0, led_id);
    view_zigbee_init(&view_zigbee, zm516x_handle);
    //添加视图
    model_attach(&(__g_model_bool.isa), &(view_led0.isa));
    model_attach(&(__g_model_bool.isa), &(view_zigbee.isa));
    while (1) {
    }
//    return 0;
}

