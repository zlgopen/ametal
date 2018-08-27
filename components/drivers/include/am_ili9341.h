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
 * \brief ili9341驱动接口文件
 *
 * \ note   ILI9341液晶屏 大小为  320 *240
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-18  vir, first implementation.
 * \endinternal
 */

#ifndef __AM_ILI9341_H
#define __AM_ILI9341_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup am_if_ili9341
 * @copydoc am_ili9341.h
 * @{
 */
 
#include "ametal.h"

/**
 * \brief ili9341 驱动函数
 *
 * \note 写命令、写8位和16位数据驱动函数必须有
 */
struct am_ili9341_drv_funcs {

    /** \brief 发送命令 */
    int (*pfn_ili9341_cmd_write)(void *p_drv, uint8_t cmd);

    /** \brief 发送8位数据 */
    int (*pfn_ili9341_data_u8_write)(void *p_drv, uint8_t data);

    /** \brief 发送16位数据 */
    int (*pfn_ili9341_data_u16_write)(void *p_drv, uint16_t data);

    /** \brief读取8位数据 */
    int (*pfn_ili9341_data_u8_read)(void *p_drv, uint8_t *p_data);

    /** \brief读取16位数据 */
    int (*pfn_ili9341_data_u16_read)(void *p_drv, uint16_t *p_data);

};

/**
 * \brief ili9341 标准服务
 */
typedef struct am_ili9341_serv {

    struct am_ili9341_drv_funcs *p_funcs;

    void                        *p_drv;
} am_ili9341_serv_t;

/**
 * \brief ILI9341 设备信息结构体
 *
 * \note
 */
typedef struct am_ili9341_devinfo {

    /** \brief ILI9341的复位引脚 所连接到的GPIO引脚编号 */
    int rst_pin;

    /** \brief ILI9341的背光引脚 所连接到的GPIO引脚编号 */
    int bl_pin;

    /** 设置屏幕的方向，横竖屏  0:竖屏 ， 1：横屏 */
    uint8_t lcd_dir;

}am_ili9341_devinfo_t;

/**
 * \brief ILI9341设备结构体
 */
typedef struct am_ili9341_dev {

    /** 设置画笔颜色  ,值参考grp_color*/
    uint16_t point_color;

    /* 设置屏幕的背景颜色 , 值参考grp_color*/
    uint16_t back_color;


    uint16_t width;    /* 屏宽 (in pixels) */
    uint16_t heigh;    /* 屏高 (in pixels) */

    /**< \brief 接口驱动   */
    am_ili9341_serv_t      *p_serv;

    const am_ili9341_devinfo_t *p_devinfo;  /* 指向设备信息常量的指针 */

} am_ili9341_dev_t;

typedef am_ili9341_dev_t *am_ili9341_handle_t;

/**
 * \brief ILI9341 设备初始化
 *
 * \param[in] p_dev      :指向ILI9341设备结构体的指针
 * \param[in] p_devinfo  :指向ILI9341设备信息结构体的指针
 * \param[in] p_serv     :指向ILI9341服务结构体的指针
 *
 * \return ILI9341服务操作句柄,如果为 NULL，表明初始化失败
 */
am_ili9341_handle_t am_ili9341_init(am_ili9341_dev_t                   *p_dev,
                                    const am_ili9341_devinfo_t         *p_devinfo,
                                    am_ili9341_serv_t                  *p_serv);

/**
 * \brief ILI9341 设备解初始化
 * \param[in] handle : ILI9341操作句柄
 * \return 无
 */
void am_ili9341_deinit (am_ili9341_handle_t handle);

/**
 * \brief ili9341软件复位
 *
 * \param[in] p_dev      :指向ILI9341设备结构体的指针
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_ili9341_reset(am_ili9341_handle_t handle);

/**
 * \name 颜色
 * \anchor grp_color
 * @{
 */
#define AM_ILI9341_COLOR_BLACK       0X0000    /* 黑色 */
#define AM_ILI9341_COLOR_WHITE       0XFFFF    /* 白色 */
#define AM_ILI9341_COLOR_BLUE        0X001F    /* 蓝色 */
#define AM_ILI9341_COLOR_DARKGREEN   0X03E0    /* 暗绿 */
#define AM_ILI9341_COLOR_GREEN       0x07E0    /* 绿色 */
#define AM_ILI9341_COLOR_RED         0xF800    /* 红色 */
#define AM_ILI9341_COLOR_CYAN        0x7FFF    /* 蓝绿 */
#define AM_ILI9341_COLOR_YELLOW      0xFFE0    /* 黄色 */
#define AM_ILI9341_COLOR_GRAY        0X8430    /* 灰色 */
#define AM_ILI9341_COLOR_BROWN       0XBC40    /* 棕色 */
#define AM_ILI9341_COLOR_MAGENTA     0xF81F    /* 品红 */
#define AM_ILI9341_COLOR_DARKBLUE    0X01CF    /* 深蓝色 */
#define AM_ILI9341_COLOR_LIGHTBLUE   0X7D7C    /* 浅蓝色 */
#define AM_ILI9341_COLOR_GRAYBLUE    0X5458    /* 灰蓝色 */
/**
 * @}
 */

/**
 * \brief 设置画笔的颜色
 * \param[in] handle    :指向ILI9341的句柄
 * \param[in] color     :要填充的颜色 ，参考  \ref grp_color
 *
 * \retval AM_OK     : 操作成功
 */
int am_ili9341_point_color_set(am_ili9341_handle_t handle, uint16_t color);


/**
 * \brief 设置背景颜色
 * \param[in] handle    :指向ILI9341的句柄
 * \param[in] color     :要填充的颜色 ，参考  \ref grp_color
 *
 * \retval AM_OK     : 操作成功
 */
int am_ili9341_back_color_set(am_ili9341_handle_t handle, uint16_t color);


/**
 * \brief 清屏
 *
 * \param[in] handle 指向ILI9341设备结构体的指针
 *
 * \retval AM_OK     : 操作成功
 *         AM_EINVAL : 无效参数
 * \note 清屏的颜色为用户设置的背景颜色
 */
int am_ili9341_clear (am_ili9341_handle_t handle);


/**
 * \brief 画直线
 *
 * \param[in] handle    :指向ILI9341设备结构体的指针
 *
 * \retval AM_OK     : 操作成功
 *         AM_EINVAL : 无效参数
 *
 * \note (x1,y1) 起始坐标
 *       (x2,y2) 终点坐标
 */
int am_ili9341_draw_line(am_ili9341_handle_t handle,
        uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * \brief 画矩形
 * \param[in] handle    :指向ILI9341设备结构体的指针
 *
 * \retval AM_OK     : 操作成功
 *         AM_EINVAL : 无效参数
 *
 * \note (x1,y1) 和  (x2,y2)为矩形的两个对角坐标
 */
int am_ili9341_draw_rectangle(am_ili9341_handle_t handle,
        uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * \brief 画圆
 * \param[in] handle    :指向ILI9341设备结构体的指针
 *
 * \retval AM_OK     : 操作成功
 *         AM_EINVAL : 无效参数
 *
 * \note (x0,y0)为圆心的坐标  ,r 为半径
 */
int am_ili9341_draw_circle(am_ili9341_handle_t handle,
        uint16_t x0, uint16_t y0, uint8_t r);

/**
 * \brief 显示一个字符
 *
 * \param[in] handle    :指向ILI9341设备结构体的指针
 * \param[in] x        : x坐标
 * \param[in] y        : y坐标
 * \param[in] num      : 要显示的字符
 * \param[in] size     : 字符大小   值可取为 12、16、24
 * \param[in] mode     : 叠加方式 mode =1
 *                       非叠加方式 mode =0
 *
 * \retval AM_OK     : 操作成功
 *         AM_EINVAL : 无效参数
 */
int am_ili9341_show_char(am_ili9341_handle_t handle,
        uint16_t x, uint16_t y, uint8_t num, uint8_t size,uint8_t mode);


/**
 * \brief 显示数字，
 *
 * \param[in] handle    :指向ILI9341设备结构体的指针
 * \param[in] x        : x坐标
 * \param[in] y        : y坐标
 * \param[in] num      : 要显示的数字
 * \param[in] len      : 数字的位数
 * \param[in] size     : 数字字体大小   值可取为 12、16、24
 *
 * \retval AM_OK     : 操作成功
 *         AM_EINVAL : 无效参数
 */
int am_ili9341_show_num(am_ili9341_handle_t handle,
       uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size);

/**
 * \brief 显示字符串
 * \param[in] handle    :指向ILI9341设备结构体的指针
 * \param[in] x        : x坐标
 * \param[in] y        : y坐标
 * \param[in] size     : 数字字体大小   值可取为 12、16、24
 * \param[in] width    : 显示区域的宽
 * \param[in] height   : 显示区域的高
 * \param[in] p_str    : 指向要显示的字符串指针*
 *
 * \retval AM_OK     : 操作成功
 *         AM_EINVAL : 无效参数
 */
int am_ili9341_show_string (am_ili9341_handle_t handle,
                            uint16_t x,        uint16_t y,
                            uint8_t size,      uint16_t width,
                            uint16_t height,   char *p_str);



/**
 * \brief 画一个点的颜色
 *
 * \param[in] handle    :指向ILI9341的句柄
 * \param[in] color     :要填充的颜色 ，参考  \ref grp_color
 *
 * \note （x，y）要填充点的坐标
 */
void am_ili9341_draw_point (am_ili9341_handle_t handle,
                            uint16_t            x,
                            uint16_t            y,
                            uint16_t            color);

/**
 * \brief ili9341 LCD整屏颜色填充
 *
 * \param[in] handle    :指向ILI9341设备结构体的指针
 * \param[in] color    :要填充的颜色 ，参考  \ref grp_color
 *
 * \note (x1,y1)、(x2,y2)为要填充区域的 为对角坐标
 *       只能填充长方形区域
 *
 */
void am_ili9341_color_fill(am_ili9341_handle_t handle,
                           uint16_t x1, uint16_t y1,
                           uint16_t x2, uint16_t y2,
                           uint16_t color);

/**
 * \brief 给一片区域填充特定的颜色
 *
 * \param[in] handle    :指向ILI9341的句柄
 * \param[in] x0, x1   : X区域坐标
 * \param[in] y0, y1   : Y区域坐标
 * \param[in] buf      : 像素颜色值
 * \param[in] data_len : 像素颜色格式的长度 (单位：字节）
 * \param[in] reverse  : 反转标志（表明X1(Y1)的值比X0(Y0)的小
 *
 */
void am_ili9341_area_draw_color(am_ili9341_handle_t handle,
                                  uint16_t x0, uint16_t y0,
                                  uint16_t x1, uint16_t y1,
                                  uint8_t *buf,uint8_t data_len,
                                  uint8_t reverse );

/**
 * \brief 开启屏幕
 */
void am_ili9341_on(am_ili9341_handle_t handle);

/**
 * \brief 关闭屏幕
 */
void am_ili9341_off(am_ili9341_handle_t handle);

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _AM_ILI9341_H */

/* end of file */


