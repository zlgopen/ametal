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
 * \brief LCD显示软件包实现
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-18  vir, first implementation.
 * \endinternal
 */

#include "am_ili9341.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_font.h"

/**
 * \brief 常用的指令
 */
#define __ili9341_SET_X_CMD             0X2A      /**< \brief 设置X坐标命令  */
#define __ili9341_SET_Y_CMD             0X2B      /**< \brief 设置Y坐标命令  */
#define __ili9341_WRITE_GRAM_CMD        0X2C      /**< \brief 写GRAM命令  */


/**
 * \brief 向ili9341 LCD发送命令
 */
am_static_inline
void __ili9341_cmd_sent(am_ili9341_dev_t *p_dev, uint8_t cmd)
{
    if ( p_dev->p_serv->p_funcs->pfn_ili9341_cmd_write) {
        p_dev->p_serv->p_funcs->pfn_ili9341_cmd_write(p_dev->p_serv->p_drv, cmd);
    }
}

/**
 * \brief 向ili9341 LCD发送一字节的数据
 */
am_static_inline
void __ili9341_data_u8_sent(am_ili9341_dev_t *p_dev, uint8_t data)
{
    if ( p_dev->p_serv->p_funcs->pfn_ili9341_data_u8_write ) {
        p_dev->p_serv->p_funcs->pfn_ili9341_data_u8_write(p_dev->p_serv->p_drv, data);
    }
}

/**
 * \brief 向ili9341 LCD发送两字节的数据
 */
static void __ili9341_data_u16_sent(am_ili9341_dev_t *p_dev, uint16_t data)
{
    if ( p_dev->p_serv->p_funcs->pfn_ili9341_data_u16_write) {
        p_dev->p_serv->p_funcs->pfn_ili9341_data_u16_write(p_dev->p_serv->p_drv, data);
    }
}



/**
 * 设置光标位置
 */
static void __ili9341_cursor_set( am_ili9341_dev_t *p_dev, uint16_t x_pos, uint16_t y_pos )
{
    /*　设置Ｘ */
    __ili9341_cmd_sent( p_dev,__ili9341_SET_X_CMD);
    __ili9341_data_u16_sent( p_dev, x_pos);

    /*　设置Y */
    __ili9341_cmd_sent( p_dev,__ili9341_SET_Y_CMD);
    __ili9341_data_u16_sent( p_dev, y_pos);
}

/**
 * \brief 设置窗口
 *
 * \note (x,y)为窗口的起始坐标点
 *        w 为窗口的宽
 *        h 为窗口的高
 */
static void __ili9341_window_set(am_ili9341_dev_t *p_dev,
                     uint32_t x, uint32_t y, uint32_t w ,uint32_t h)
{
    __ili9341_cmd_sent( p_dev,__ili9341_SET_X_CMD);
    __ili9341_data_u16_sent(p_dev, x);
    __ili9341_data_u16_sent(p_dev, x + w - 1);

    __ili9341_cmd_sent( p_dev,__ili9341_SET_Y_CMD);
    __ili9341_data_u16_sent(p_dev, y);
    __ili9341_data_u16_sent(p_dev, y + h - 1);
}

/**
 * \biref 画点
 */
static void __ili9341_draw_point (am_ili9341_handle_t handle, uint16_t x, uint16_t y)
{
    __ili9341_cursor_set( handle,x,y);
    __ili9341_cmd_sent( handle,__ili9341_WRITE_GRAM_CMD);
    __ili9341_data_u16_sent(handle,handle->point_color);

}
/**
 * \brief m的n次方
 */
static uint32_t __ili9341_power(uint8_t m, uint8_t n)
{
    uint32_t  result=1;
    while(n--) result *= m;
    return result;
}

/**
 * \biref ili9341的初始化
 */
static int __ili9341_init (am_ili9341_dev_t *p_dev)
{
    if( NULL == p_dev) {
        return -AM_EINVAL;
    }

    __ili9341_cmd_sent( p_dev, 0x28); /* 关闭显示 */

    /* Power control A */
    __ili9341_cmd_sent( p_dev,    0xCB);
    __ili9341_data_u8_sent(p_dev, 0x39);
    __ili9341_data_u8_sent(p_dev, 0x2C);
    __ili9341_data_u8_sent(p_dev, 0x00);
    __ili9341_data_u8_sent(p_dev, 0x34);
    __ili9341_data_u8_sent(p_dev, 0x02);

    /* Power control B */
    __ili9341_cmd_sent( p_dev,    0xCF);
    __ili9341_data_u8_sent(p_dev, 0x00);
    __ili9341_data_u8_sent(p_dev, 0xC1);
    __ili9341_data_u8_sent(p_dev, 0X30);

    /* Power on sequence control */
    __ili9341_cmd_sent( p_dev,    0xED);
    __ili9341_data_u8_sent(p_dev, 0x64);
    __ili9341_data_u8_sent(p_dev, 0x03);
    __ili9341_data_u8_sent(p_dev, 0X12);
    __ili9341_data_u8_sent(p_dev, 0X81);

    /* Driver timing control A */
    __ili9341_cmd_sent( p_dev,    0xE8);
    __ili9341_data_u8_sent(p_dev, 0x85);
    __ili9341_data_u8_sent(p_dev, 0x10);
    __ili9341_data_u8_sent(p_dev, 0x7A);

    /* Pump ratio control */
    __ili9341_cmd_sent( p_dev,    0xF7);
    __ili9341_data_u8_sent(p_dev, 0x20);

    /* Driver timing control B */
    __ili9341_cmd_sent( p_dev,    0xEA);
    __ili9341_data_u8_sent(p_dev, 0x00);
    __ili9341_data_u8_sent(p_dev, 0x00);

    /* Power control */
    __ili9341_cmd_sent( p_dev,    0xC0);
    __ili9341_data_u8_sent(p_dev, 0x1B);

    __ili9341_cmd_sent( p_dev,    0xC1);
    __ili9341_data_u8_sent(p_dev, 0x01);

    /* VCM control */
    __ili9341_cmd_sent( p_dev,    0xC5);
    __ili9341_data_u8_sent(p_dev, 0x45);
    __ili9341_data_u8_sent(p_dev, 0x25);

    /* VCM control2 */
    __ili9341_cmd_sent( p_dev,    0xC7);
    __ili9341_data_u8_sent(p_dev, 0XB7);

    /* Memory Access Control */
    __ili9341_cmd_sent( p_dev,    0x36);
    __ili9341_data_u8_sent(p_dev, 0x28);

    /* Pixel Format Set */
    __ili9341_cmd_sent( p_dev,    0x3A);
    __ili9341_data_u8_sent(p_dev, 0x55);

    /* Frame Rate Control */
    __ili9341_cmd_sent( p_dev,    0xB1);
    __ili9341_data_u8_sent(p_dev, 0x00);
    __ili9341_data_u8_sent(p_dev, 0x1A);

    /* Display Function Control */
    __ili9341_cmd_sent( p_dev,    0xB6);
    __ili9341_data_u8_sent(p_dev, 0x0A);
    __ili9341_data_u8_sent(p_dev, 0x82);

    /* 3Gamma Function Disable */
    __ili9341_cmd_sent( p_dev,    0xF2);
    __ili9341_data_u8_sent(p_dev, 0x00);

    /* Gamma curve selected */
    __ili9341_cmd_sent( p_dev,    0x26);
    __ili9341_data_u8_sent(p_dev, 0x01);

    /* Set Gamma */
    __ili9341_cmd_sent( p_dev,    0xE0);
    __ili9341_data_u8_sent(p_dev, 0x0F);
    __ili9341_data_u8_sent(p_dev, 0x2A);
    __ili9341_data_u8_sent(p_dev, 0x28);
    __ili9341_data_u8_sent(p_dev, 0x08);
    __ili9341_data_u8_sent(p_dev, 0x0E);
    __ili9341_data_u8_sent(p_dev, 0x08);
    __ili9341_data_u8_sent(p_dev, 0x54);
    __ili9341_data_u8_sent(p_dev, 0XA9);
    __ili9341_data_u8_sent(p_dev, 0x43);
    __ili9341_data_u8_sent(p_dev, 0x0A);
    __ili9341_data_u8_sent(p_dev, 0x0F);
    __ili9341_data_u8_sent(p_dev, 0x00);
    __ili9341_data_u8_sent(p_dev, 0x00);
    __ili9341_data_u8_sent(p_dev, 0x00);
    __ili9341_data_u8_sent(p_dev, 0x00);

    __ili9341_cmd_sent( p_dev,    0xE1);
    __ili9341_data_u8_sent(p_dev, 0x00);
    __ili9341_data_u8_sent(p_dev, 0x15);
    __ili9341_data_u8_sent(p_dev, 0x17);
    __ili9341_data_u8_sent(p_dev, 0x07);
    __ili9341_data_u8_sent(p_dev, 0x11);
    __ili9341_data_u8_sent(p_dev, 0x06);
    __ili9341_data_u8_sent(p_dev, 0x2B);
    __ili9341_data_u8_sent(p_dev, 0x56);
    __ili9341_data_u8_sent(p_dev, 0x3C);
    __ili9341_data_u8_sent(p_dev, 0x05);
    __ili9341_data_u8_sent(p_dev, 0x10);
    __ili9341_data_u8_sent(p_dev, 0x0F);
    __ili9341_data_u8_sent(p_dev, 0x3F);
    __ili9341_data_u8_sent(p_dev, 0x3F);
    __ili9341_data_u8_sent(p_dev, 0x0F);

    /* Exit Sleep */
    __ili9341_cmd_sent( p_dev,    0x11);
    am_mdelay(120);

    /* display on */
    __ili9341_cmd_sent( p_dev,    0x29);

    return AM_OK;
}


/*****************************************************************************
 * 外部函数实现
 ****************************************************************************/

/**
 * \brief ili9341软件复位
 */
int am_ili9341_reset(am_ili9341_handle_t handle)
{
    int ret = AM_OK;
    ret = am_gpio_set(handle->p_devinfo->rst_pin, AM_GPIO_LEVEL_LOW);
    am_mdelay(10);
    ret = am_gpio_set(handle->p_devinfo->rst_pin, AM_GPIO_LEVEL_HIGH);
    am_mdelay(120);
    if ( AM_OK != ret) {
        return ret;
    }
    return AM_OK;
}
/**
 * \brief ili9341初始化
 */
am_ili9341_handle_t am_ili9341_init(am_ili9341_dev_t                   *p_dev,
                                    const am_ili9341_devinfo_t         *p_devinfo,
                                    am_ili9341_serv_t                  *p_serv)
{
    int ret = AM_OK;
    uint8_t reg_val = 0;

    /* 验证参数有效性 */
    if ( NULL == p_dev || NULL == p_devinfo || NULL == p_serv) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;
    p_dev->p_serv    = p_serv;

    p_dev->back_color  = AM_ILI9341_COLOR_WHITE;
    p_dev->point_color = AM_ILI9341_COLOR_BLACK;


    /* 初始化GPIO口 */
    am_gpio_pin_cfg(p_devinfo->bl_pin,
            AM_GPIO_OUTPUT_INIT_LOW | AM_GPIO_PUSH_PULL);   /* 配置bl为推挽输出 */
    am_gpio_pin_cfg(p_devinfo->rst_pin,
            AM_GPIO_OUTPUT_INIT_HIGH | AM_GPIO_PUSH_PULL);  /* 配置rst为推挽输出 */


    /* 复位ili9341设备 */
    ret = am_ili9341_reset(p_dev);
    if(AM_OK != ret) {
        return NULL;
    }
    ret = __ili9341_init(p_dev);
    if (AM_OK != ret) {
        return NULL;
    }

    if( 0 == p_devinfo->lcd_dir ) {
        /* 竖屏处理 */
        p_dev->heigh = 320;
        p_dev->width =240;

        reg_val =(0<<7)|(1<<6)|(0<<5);
        reg_val |= 0X08;

        __ili9341_cmd_sent(p_dev,0x36);
        __ili9341_data_u8_sent(p_dev, reg_val);
    } else {
        /* 横屏处理 */
        p_dev->heigh = 240;
        p_dev->width =320;
        reg_val =(1<<7)|(1<<6)|(1<<5);
        reg_val |= 0X08;
        __ili9341_cmd_sent(p_dev,0x36);
        __ili9341_data_u8_sent(p_dev, reg_val);
    }

    am_gpio_set(p_devinfo->bl_pin, AM_GPIO_LEVEL_HIGH); /* 开启背光 */
    return p_dev;
}

/**
 * \brief ili9341解初始化
 */
void am_ili9341_deinit (am_ili9341_handle_t handle)
{

}

/**
 * \brief 设置画笔的颜色
 */
int am_ili9341_point_color_set(am_ili9341_handle_t handle, uint16_t color)
{
    if ( NULL == handle ) {
        return -AM_EINVAL;
    }
    handle->point_color = color;
    return AM_OK;
}

/**
 * \brief 设置背景颜色
 */
int am_ili9341_back_color_set(am_ili9341_handle_t handle, uint16_t color)
{
    if ( NULL == handle ) {
        return -AM_EINVAL;
    }
    handle->back_color = color;
    return AM_OK;
}

/**
 * \brief LCD颜色填充
 */
int am_ili9341_clear (am_ili9341_handle_t handle)
{
    if ( NULL == handle) {
        return -AM_EINVAL;
    }
    uint16_t i     = 0;
    uint16_t count = 0;
    __ili9341_window_set( handle, 0, 0, handle->width,  handle->heigh);
    __ili9341_cmd_sent( handle,0x2c);

    count = (handle->width  *  handle->heigh) / 8;

    for(i = 0; i < count; i++) {
        __ili9341_data_u16_sent(handle,handle->back_color);
        __ili9341_data_u16_sent(handle,handle->back_color);
        __ili9341_data_u16_sent(handle,handle->back_color);
        __ili9341_data_u16_sent(handle,handle->back_color);

        __ili9341_data_u16_sent(handle,handle->back_color);
        __ili9341_data_u16_sent(handle,handle->back_color);
        __ili9341_data_u16_sent(handle,handle->back_color);
        __ili9341_data_u16_sent(handle,handle->back_color);
    }
    return AM_OK;
}

/**
 * \brief 画直线
 *
 * \note (x1,y1) 起始坐标
 *       (x2,y2) 终点坐标
 */
int am_ili9341_draw_line(am_ili9341_handle_t handle,
        uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if ( NULL == handle        ||  x1 > handle->width  ||
         y1 > handle->heigh ||  x2 > handle->width ||
         y2 > handle->heigh ) {
        return -AM_EINVAL;
    }

    uint16_t  t ;
    int  xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int  incx, incy, uRow, uCol;

    delta_x  = x2 - x1;
    delta_y  = y2 - y1;
    uRow     = x1;
    uCol     = y1;

    if( delta_x > 0) {
        incx = 1;
    } else if ( delta_x == 0) {
        incx = 0;
    }  else {
        incx = -1;
        delta_x = -delta_x;
    }

    if( delta_y > 0) {
        incy = 1;
    } else if( delta_y == 0) {
        incy = 0;
    } else {
        incy = -1;
        delta_y = -delta_y;
    }

    if( delta_x > delta_y) {
        distance = delta_x;
    } else {
        distance = delta_y;
    }

    for( t=0; t <= distance + 1; t++ ) {

        __ili9341_draw_point( handle, uRow, uCol);  /* 画点 */

        xerr += delta_x;
        yerr += delta_y;
        if( xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }
        if(yerr>distance) {
            yerr -= distance;
            uCol += incy;
        }
    }

   return AM_OK;
}

/**
 * \brief 画矩形
 *
 */
int am_ili9341_draw_rectangle(am_ili9341_handle_t handle,
        uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{

    if ( NULL == handle || x2 > handle->width ||
         y2 > handle->heigh  || x1 > x2 || y1 > y2) {
        return -AM_EINVAL;
    }
    am_ili9341_draw_line(handle,x1,y1,x2,y1);
    am_ili9341_draw_line(handle,x1,y1,x1,y2);
    am_ili9341_draw_line(handle,x1,y2,x2,y2);
    am_ili9341_draw_line(handle,x2,y1,x2,y2);

    return AM_OK;
}
/**
 * \brief 画圆
 *
 */
int am_ili9341_draw_circle(am_ili9341_handle_t handle,
        uint16_t x0, uint16_t y0, uint8_t r)
{
    if ( NULL == handle ||
         x0 > handle->width ||
         y0 > handle->heigh ) {
        return -AM_EINVAL;
    }
    int a, b;
    int di;

    a=0;b=r;
    di=3-(r<<1);

    while(a<=b) {
        __ili9341_draw_point(handle,x0+a,y0-b);             //5
        __ili9341_draw_point(handle,x0+b,y0-a);             //0
        __ili9341_draw_point(handle,x0+b,y0+a);             //4
        __ili9341_draw_point(handle,x0+a,y0+b);             //6
        __ili9341_draw_point(handle,x0-a,y0+b);             //1
        __ili9341_draw_point(handle,x0-b,y0+a);
        __ili9341_draw_point(handle,x0-a,y0-b);             //2
        __ili9341_draw_point(handle,x0-b,y0-a);             //7
        a++;

        if( di < 0) {
            di += 4 * a + 6;
        } else {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
    return AM_OK;
}

/**
 * \brief 显示一个字符
 *
 * \note  num ：要显示的字符
 *        size：字体大小 12/ 16 /24
 *        mode: 叠加方式 mode =1
 *              非叠加方式 mode =0
 */
int am_ili9341_show_char(am_ili9341_handle_t handle,
        uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode)
{
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint8_t csize= (size / 8 + (( size % 8) ? 1 : 0 )) * (size / 2);
    num=num-' ';

    if ( NULL ==handle || x > handle->width  ||
         y > handle->heigh  ) {
        return -AM_EINVAL;
    }

    for(t = 0; t < csize; t++){

        /* 判断字体大小 */
        if( size == 12) {
            temp = asc2_1206[num][t];
        } else if (size == 16) {
            temp=asc2_1608[num][t];
        } else if (size == 24) {
            temp=asc2_2412[num][t];
        }  else return -AM_EINVAL;

        for(t1=0;t1<8;t1++) {
            if(temp & 0x80) {
                __ili9341_draw_point( handle, x, y);
            } else if(mode == 0) {
             //   am_ili9341_point_draw_color(handle, x, y,handle->p_devinfo->back_color);
            }

            temp <<= 1;
            y++;
            if( y>= handle->heigh) {
                 return -AM_EINVAL;
            }

            if((y - y0) == size) {
                y = y0;
                x++;
                if( x>= handle->width ) {
                    return -AM_EINVAL;
                }
                break;
            }
        }

    }
    return AM_OK;
}
/**
 * \brief 显示数字，
 *
 * \note num 要显示的数字
 *       len为数字的位数
 *       size为数字字体大小  12/ 16 /24
 */
int am_ili9341_show_num(am_ili9341_handle_t handle,
       uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t  t = 0;
    uint8_t  temp = 0;
    uint8_t  enshow = 0;

    if ( NULL ==handle || x > handle->width ||
         y > handle->heigh ) {
        return -AM_EINVAL;
    }
    for(t=0;t<len;t++) {
        temp= ( num/ __ili9341_power(10,len-t-1 ))%10;
        if( enshow == 0 && t< (len-1))
        {
            if(temp == 0)
            {
                am_ili9341_show_char(handle,x+(size/2)*t,y,' ',size,0);
                continue;
            }else enshow=1;

        }
        am_ili9341_show_char(handle, x+(size/2)*t,y,temp+'0',size,0);
    }
    return AM_OK;
}

/**
 * \brief 显示字符串
 *
 * \note  width,height 区域的大小
 */
int am_ili9341_show_string (am_ili9341_handle_t handle,
        uint16_t x, uint16_t y, uint8_t size, uint16_t width, uint16_t height,char *p_str)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    if ( NULL ==handle || x > handle->width ||
         y > handle->heigh ) {
        return -AM_EINVAL;
    }

    while((*p_str <= '~') && (*p_str >= ' ' ))
    {
        if(x >= width ) {
            x = x0;
            y += size;
        }
        if( y >= height) {
            break;
        }
        am_ili9341_show_char( handle, x, y, *p_str, size, 0);
        x += size/2;
        p_str++;
    }
    return AM_OK;
}


/******************************** 移植EMWIN所需 **********************************************/
/**
 * \brief 画点
 */

void am_ili9341_draw_point (am_ili9341_handle_t handle, uint16_t x, uint16_t y, uint16_t color)
{

    __ili9341_cursor_set( handle,x,y);
    __ili9341_cmd_sent( handle,__ili9341_WRITE_GRAM_CMD);
    __ili9341_data_u16_sent(handle,color);
}

/**
 * \brief 指定的区域填充颜色
 */
void am_ili9341_color_fill(am_ili9341_handle_t handle,
        uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{

    uint16_t i,j;
    uint16_t xlen=0;
    xlen = x2 - x1 + 1;

    for( i = y1; i <= y2; i++) {
        __ili9341_cursor_set(handle, x1,i);
        __ili9341_cmd_sent( handle,__ili9341_WRITE_GRAM_CMD);
        for( j=0; j < xlen;j++) {
            __ili9341_data_u16_sent(handle,color);
        }
    }
}

/**
 * \brief 给一片区域填充特定的颜色
 */

void am_ili9341_area_draw_color(am_ili9341_handle_t handle,
                                  uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                                  uint8_t *buf,uint8_t data_len, uint8_t reverse )
{
    int8_t addr_step = 0;
    int val = 0;

    /*　设置Ｘ */
    __ili9341_cmd_sent( handle,__ili9341_SET_X_CMD);
    __ili9341_data_u16_sent( handle, x0);
    __ili9341_data_u16_sent( handle, x1);

    /*　设置Y */
    __ili9341_cmd_sent( handle,__ili9341_SET_Y_CMD);
    __ili9341_data_u16_sent( handle, y0);
    __ili9341_data_u16_sent( handle, y1);

    __ili9341_cmd_sent( handle,__ili9341_WRITE_GRAM_CMD);

    /*
     * addr_step 代表地址的建立的方式，像素的数据在内存
     * 中保存方式为向上递增，不为0就向下递减
     */
    if (reverse) {
        addr_step = -data_len;
    } else {
        addr_step = data_len;
    }

    for (int i = 0; i < (x1 - x0 + 1) * (y1 - y0 + 1); i++) {

        switch(data_len) {
        case 1:
            val = *buf;
            break;
        case 2:
            val = *((uint16_t *)buf);
            break;
        case 4:
            val = *((uint32_t *)buf);
            break;

        default:
            val = *((uint16_t *)buf);
            break;
        }

        buf += addr_step;
        __ili9341_data_u16_sent(handle, val); /* 将颜色的数据写进去 */
    }
}

/**
 * \brief 开启屏幕
 */
void am_ili9341_on(am_ili9341_handle_t handle)
{
    am_gpio_set(handle->p_devinfo->bl_pin, AM_GPIO_LEVEL_HIGH); /* 开启背光 */
}
/**
 * \brief 关闭屏幕
 */
void am_ili9341_off(am_ili9341_handle_t handle)
{
    am_gpio_set(handle->p_devinfo->bl_pin, AM_GPIO_LEVEL_LOW); /* 开启背光 */
}


/* end of file */
