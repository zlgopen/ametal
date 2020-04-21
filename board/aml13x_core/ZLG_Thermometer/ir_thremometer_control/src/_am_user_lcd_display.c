/**
 * \file
 * \brief LCD驱动层实现
 *
 */

#include "ametal.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "hc32x3x_pin.h"
#include "am_hc32l13x_inst_init.h"
#include "am_user_lcd_display.h"

/* 特殊显示宏定义 */
#define AMHW_USER_BT_DISP         (0x00000008)//0
#define AMHW_USER_BZ_DISP         (0x00000004)//0
#define AMHW_USER_0F_DISP         (0x00000002)//0
#define AMHW_USER_0C_DISP         (0x00000001)//0
#define AMHW_USER_P1_DISP         (0x00080000)//1
#define AMHW_USER_BT3_DISP        (0x00000008)//2
#define AMHW_USER_LOG_DISP        (0x00080000)//0
#define AMHW_USER_SUR_DISP        (0x00000800)//1
#define AMHW_USER_KU_DISP         (0x00080000)//2
#define AMHW_USER_XIAO_DISP       (0x00000800)//3
#define AMHW_USER_BLUE_DISP       (0x00080000)//3
#define AMHW_USER_BODY_DISP       (0x00040000)//3
#define AMHW_USER_BT2_DISP        (0x00020000)//3
#define AMHW_USER_BT1_DISP        (0x00010000)//3

/* 定义LCDRAM */
uint32_t __g_lcdram[4] = {0};

/**
 * \brief 显示数据
 *
 * \note : LCD屏一共四个数字显示单元，LCD每个数字显示由两个字节的各低四位控制,
 *         数字0-9 数据码：
 *                      0-------0x0a0f
 *                      1-------0x0006
 *                      2-------0x060d
 *                      3-------0x040f
 *                      4-------0x0c06
 *                      5-------0x0c0b
 *                      6-------0x0e0b
 *                      7-------0x000e
 *                      8-------0x0e0f
 *                      9-------0x0c0f
 *                      .-------0x0100
 *                      x-------0x0000 (x 表示无显示)
 *
 */
static uint32_t lcd_datacode[14] = {
    0x050F, 0x0006, 0x060B, 0x020F,
    0x0306, 0x030D, 0x070D, 0x0007,
    0x070F, 0x030F, 0x0008, 0x0000,
    0x0709, 0x0707
};

/**
 * \brief LCD COM-SEG端口初始化配置信息结构体
 *
 * \note : 用户使用LCD屏时，需要自己定义一个结构体并填充
 *         （用户在控制端口时1表示打开0表示关闭）
 */
static am_hc32_lcd_com_seg_t __g_lcd_com_seg_info =
{
    0x00006fff,     /**< \brief seg0-31端口 */
    0xf,            /**< \brief seg32-35端口 */
    0x0,            /**< \brief 管脚功能选择由CR0.DUTY决定*/
    0x10,           /**< \brief seg40-51端口 */
    0x1,            /**< \brief SEG32~SEG35端口功能选择 */
    0xf,            /**< \brief COM0-3端口功能选择 */
};

/**
 * \brief 用户 LCD 初始化
 */
am_lcd_handle_t am_user_lcd_disp_init (void)
{
    am_lcd_handle_t handle = am_hc32_lcd_inst_init();

    /* LCD COM SEG 端口配置 */
    am_hc32_lcd_com_seg_init(handle, &__g_lcd_com_seg_info);

    /* LCD 选择显示模式0，并使能 */
    am_hc32_lcd_enable(handle, AM_HC32_LCD_MODE_0);

    return handle;
}

/**
 * \brief 用户 LCD 清屏
 */
int am_user_lcd_disp_clr (am_lcd_handle_t handle)
{
    int i = 0;

    amhw_hc32_lcd_t *p_hw_lcd;

    if (handle == NULL){

        return AM_ERROR;
    }

    p_hw_lcd = (amhw_hc32_lcd_t *)handle->p_devinfo->reg_base;

    /* 清除显存数据 */
    for (i = 0; i < 4; i++){

        amhw_hc32_lcd_ram_write (p_hw_lcd, 0x0, i);
    }

    return AM_OK;
}

/**
 * \brief 用户 LCD 写浮点数
 */
int am_user_lcd_disp_float (am_lcd_handle_t handle,
                            float           data)
{
    amhw_hc32_lcd_t *p_hw_lcd;

    if (handle == NULL){
        return AM_ERROR;
    }

    p_hw_lcd = (amhw_hc32_lcd_t *)handle->p_devinfo->reg_base;

    uint8_t         i = 0;
    int             data_ten_times = 0;
    uint32_t        data_bit[4] = {0};
    uint32_t        data_div = 0;

    data_ten_times = (int)(data * 10);  /* 将 data 放大 10 倍，取整 */
    data_div = data_ten_times;

    /* 人体温度范围：32.9 ~ 42.0，若不在范围内，数码管显示 ERR */
    if ( data_ten_times > 9999 ) {
        
        data_bit[0] = 11;
        data_bit[1] = 13;
        data_bit[2] = 13;
        data_bit[3] = 12;

    } else {
        /* 当data不为0时分离各个位，data为0时跳过while */
        while (data_div != 0) {
            *(data_bit + i) = data_div % 10;
            i++;
            data_div /= 10;
        }

        /*  
            此段作用：当高位为0时不显示（当 data = 0 时，只显示个位的0）
        例：
            四位数：全显示                                  1357    显示 1357
            三位数：最高位0不显示                           103     显示 x103
            二位数：最高位和次高位的0不显示                 78      显示 xx78
            一位数：最高三个位的0不显示，只显示最低位       5       显示 xxx5
                    （当数据为0时，显示xxx0）
        */
        for (i = 3; i > 1; i--) {        /* 从最高位往次低位判断 */
            if (data_bit[i] != 0) {
                break;
            } else {
                data_bit[i] = 11;
            }
        }
    }

    __g_lcdram[0] = ( (lcd_datacode[data_bit[0]] & 0xff ) << 24);

    __g_lcdram[1] = ( (lcd_datacode[data_bit[1]] & 0xff ) << 24)    |
                    ( (lcd_datacode[data_bit[0]] & 0xff00 ) << 8);

    __g_lcdram[2] = ( (lcd_datacode[data_bit[1]] & 0xff00 ) >> 8)    |
                    ( (lcd_datacode[data_bit[2]]) << 8 )            |
                    ( (lcd_datacode[data_bit[3]] & 0xff ) << 24);

    __g_lcdram[3] = ( (lcd_datacode[data_bit[3]] & 0xff00 ) << 8);

    __g_lcdram[1] |= 0x00080000;

    for (i = 0; i < 4; i++) {
        amhw_hc32_lcd_ram_write (p_hw_lcd, __g_lcdram[i], i);
    }

    return AM_OK;
}

/**
 * \brief 用户 LCD 写记忆数（取整）
 */
int am_user_lcd_disp_mem(am_lcd_handle_t    handle,
                         uint32_t           data)
{
    amhw_hc32_lcd_t *p_hw_lcd;

    if (handle == NULL){
        return AM_ERROR;
    }

    p_hw_lcd = (amhw_hc32_lcd_t *)handle->p_devinfo->reg_base;

    uint8_t         i = 0;
    uint32_t        data_bit[2] = {0};
    uint32_t        data_div = data;
    
    if (data > 99) {
        
    } else {
        /* 当data不为0时分离各个位，data为0时跳过while */
        while (data_div != 0) {
            *(data_bit + i) = data_div % 10;
            i++;
            data_div /= 10;
        }
        
        __g_lcdram[0] = ( __g_lcdram[0] & 0xfffff80f ) |
                        ( (lcd_datacode[data_bit[0]]) << 8 );
        
        __g_lcdram[1] = ( __g_lcdram[1] & 0xffffff80 ) |
                        ( (lcd_datacode[data_bit[1]]) );
        
    }

    for (i = 0; i < 4; i++) {
        amhw_hc32_lcd_ram_write (p_hw_lcd, __g_lcdram[i], i);
    }
    
    return AM_OK;
}

/**
 * \brief 用户 LCD 显示报错
 */
int am_user_lcd_disp_err(am_lcd_handle_t    handle)
{
    amhw_hc32_lcd_t *p_hw_lcd;

    if (handle == NULL){
        return AM_ERROR;
    }

    p_hw_lcd = (amhw_hc32_lcd_t *)handle->p_devinfo->reg_base;

    uint8_t         i = 0;    
    uint32_t        data_bit[4] = {0};
    
    data_bit[0] = 13;
    data_bit[1] = 13;
    data_bit[2] = 12;
    data_bit[3] = 11;

    __g_lcdram[0] = ( (lcd_datacode[data_bit[0]] & 0xff ) << 24);

    __g_lcdram[1] = ( (lcd_datacode[data_bit[1]] & 0xff ) << 24)    |
                    ( (lcd_datacode[data_bit[0]] & 0xff00 ) << 8);

    __g_lcdram[2] = ( (lcd_datacode[data_bit[1]] & 0xff00 ) >> 8)    |
                    ( (lcd_datacode[data_bit[2]]) << 8 )            |
                    ( (lcd_datacode[data_bit[3]] & 0xff ) << 24);

    __g_lcdram[3] = ( (lcd_datacode[data_bit[3]] & 0xff00 ) << 8);

    for (i = 0; i < 4; i++) {
        amhw_hc32_lcd_ram_write (p_hw_lcd, __g_lcdram[i], i);
    }
    
    return AM_OK;
}

/**
 * \brief 用户 LCD 特殊显示使能
 */
int am_user_lcd_disp_flag_enable(am_lcd_handle_t    handle,
                                 uint32_t           flags)
{
    
    amhw_hc32_lcd_t *p_hw_lcd;

    if (handle == NULL){
        return AM_ERROR;
    }

    p_hw_lcd = (amhw_hc32_lcd_t *)handle->p_devinfo->reg_base;

    uint8_t         i = 0;

    if (flags & AM_USER_BT_DISP) {
        __g_lcdram[0] |= AMHW_USER_BT_DISP;
    }

    if (flags & AM_USER_BZ_DISP) {
        __g_lcdram[0] |= AMHW_USER_BZ_DISP;
    }

    if (flags & AM_USER_0F_DISP) {
        __g_lcdram[0] |= AMHW_USER_0F_DISP;
    }

    if (flags & AM_USER_0C_DISP) {
        __g_lcdram[0] |= AMHW_USER_0C_DISP;
    }

    if (flags & AM_USER_P1_DISP) {
        __g_lcdram[1] |= AMHW_USER_P1_DISP;
    }

    if (flags & AM_USER_BT3_DISP) {
        __g_lcdram[2] |= AMHW_USER_BT3_DISP;
    }

    if (flags & AM_USER_LOG_DISP) {
        __g_lcdram[0] |= AMHW_USER_LOG_DISP;
    }

    if (flags & AM_USER_SUR_DISP) {
        __g_lcdram[1] |= AMHW_USER_SUR_DISP;
    }

    if (flags & AM_USER_KU_DISP) {
        __g_lcdram[2] |= AMHW_USER_KU_DISP;
    }

    if (flags & AM_USER_XIAO_DISP) {
        __g_lcdram[3] |= AMHW_USER_XIAO_DISP;
    }

    if (flags & AM_USER_BLUE_DISP) {
        __g_lcdram[3] |= AMHW_USER_BLUE_DISP;
    }

    if (flags & AM_USER_BODY_DISP) {
        __g_lcdram[3] |= AMHW_USER_BODY_DISP;
    }

    if (flags & AM_USER_BT2_DISP) {
        __g_lcdram[3] |= AMHW_USER_BT2_DISP;
    }

    if (flags & AM_USER_BT1_DISP) {
        __g_lcdram[3] |= AMHW_USER_BT1_DISP;
    }

    for (i = 0; i < 4; i++) {
        amhw_hc32_lcd_ram_write (p_hw_lcd, __g_lcdram[i], i);
    }

    return AM_OK;
}

/**
 * \brief 用户 LCD 特殊显示禁能
 */
int am_user_lcd_disp_flag_disable(am_lcd_handle_t    handle,
                                  uint32_t           flags)
{
    
    amhw_hc32_lcd_t *p_hw_lcd;

    if (handle == NULL){
        return AM_ERROR;
    }

    p_hw_lcd = (amhw_hc32_lcd_t *)handle->p_devinfo->reg_base;

    uint8_t         i = 0;

    if (flags & AM_USER_BT_DISP) {
        __g_lcdram[0] &= (~AMHW_USER_BT_DISP);
    }

    if (flags & AM_USER_BZ_DISP) {
        __g_lcdram[0] &= (~AMHW_USER_BZ_DISP);
    }

    if (flags & AM_USER_0F_DISP) {
        __g_lcdram[0] &= (~AMHW_USER_0F_DISP);
    }

    if (flags & AM_USER_0C_DISP) {
        __g_lcdram[0] &= (~AMHW_USER_0C_DISP);
    }

    if (flags & AM_USER_P1_DISP) {
        __g_lcdram[1] &= (~AMHW_USER_P1_DISP);
    }

    if (flags & AM_USER_BT3_DISP) {
        __g_lcdram[2] &= (~AMHW_USER_BT3_DISP);
    }

    if (flags & AM_USER_LOG_DISP) {
        __g_lcdram[0] &= (~AMHW_USER_LOG_DISP);
    }

    if (flags & AM_USER_SUR_DISP) {
        __g_lcdram[1] &= (~AMHW_USER_SUR_DISP);
    }

    if (flags & AM_USER_KU_DISP) {
        __g_lcdram[2] &= (~AMHW_USER_KU_DISP);
    }

    if (flags & AM_USER_XIAO_DISP) {
        __g_lcdram[3] &= (~AMHW_USER_XIAO_DISP);
    }

    if (flags & AM_USER_BLUE_DISP) {
        __g_lcdram[3] &= (~AMHW_USER_BLUE_DISP);
    }

    if (flags & AM_USER_BODY_DISP) {
        __g_lcdram[3] &= (~AMHW_USER_BODY_DISP);
    }

    if (flags & AM_USER_BT2_DISP) {
        __g_lcdram[3] &= (~AMHW_USER_BT2_DISP);
    }

    if (flags & AM_USER_BT1_DISP) {
        __g_lcdram[3] &= (~AMHW_USER_BT1_DISP);
    }

    for (i = 0; i < 4; i++) {
        amhw_hc32_lcd_ram_write (p_hw_lcd, __g_lcdram[i], i);
    }

    return AM_OK;
}

/* end of file */

