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
 * \brief LCD驱动，服务LCD标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-13
 * \endinternal
 */
#ifndef __AM_ZSN700_LCD_H
#define __AM_ZSN700_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

#define AM_ZSN700_LCD_MODE_0    0  /**< \brief LCD 模式0 */
#define AM_ZSN700_LCD_MODE_1    1  /**< \brief LCD 模式1 */

/**
 * \addtogroup am_zsn700_if_lcd
 * \copydoc am_zsn700_lcd.h
 * @{
 */

/**
 * \brief LCD COM-SEG端口初始化配置信息结构体
 *
 * \note : 用户再使用LCD屏时，需要自己定义一个结构体并填充
 *         （用户在控制端口时1表示打开0表示关闭）
 */
typedef struct am_zsn700_lcd_com_seg {
    uint32_t seg0_31;          /**< \brief seg0-31端口 */
    uint8_t  seg32_35;         /**< \brief seg32-35端口 */
    uint8_t  seg36com7_39com4; /**< \brief seg36-3 com4-7管脚功能选择由CR0.DUTY决定*/
    uint16_t seg40_51;         /**< \brief seg40-51端口 */
    uint8_t  mux;              /**< \brief SEG32~SEG35端口功能选择 */
    uint8_t  com0_3;           /**< \brief COM0-3端口功能选择 */
}am_zsn700_lcd_com_seg_t;

/**
 * \brief LCD设备信息结构体
 */
typedef struct am_zsn700_lcd_info {

    uint32_t    reg_base;  /**< \brief LCD寄存器块基地址 */

    uint8_t     inum;      /**< \brief LCD中断编号 */

    uint8_t     bias_src;  /**< \brief LCD偏置源 */

    uint8_t     duty;      /**< \brief LCD占空比 */

    uint8_t     bias;      /**< \brief LCD偏置电压 */

    uint8_t     cpclk;     /**< \brief LCD电压泵时钟频率*/

    uint8_t     scanclk;   /**< \brief LCD扫描频率*/

    uint8_t     clk_src;   /**< \brief LCD时钟源*/

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

}am_zsn700_lcd_devinfo_t;

/**
 * \brief LCD设备实例
 */
typedef struct am_zsn700_lcd_dev {

    /** \brief 指向LCD设备信息的指针 */
    const am_zsn700_lcd_devinfo_t *p_devinfo;

    /** \brief 中断触发回调 函数*/
    void (*pfn_trigger_cb)(void *);

    /** \brief 用户回调参数 */
    void *p_arg;

    /** \brief LCD显示模式 */
    uint8_t mode;

} am_zsn700_lcd_dev_t;

/** \brief LCD标准服务操作句柄类型定义 */
typedef am_zsn700_lcd_dev_t *am_lcd_handle_t;

/**
 * \brief LCD 初始化
 *
 * \param[in] p_dev     : LCD 设备实例
 * \param[in] p_devinfo : LCD 设备信息实例
 *
 * \retval : 标准LCD服务句柄
 *
 */
am_lcd_handle_t  am_zsn700_lcd_init (am_zsn700_lcd_dev_t           *p_dev,
                                     const am_zsn700_lcd_devinfo_t *p_devinfo);

/**
 * \brief LCD 去初始化
 *
 * \param[in] handle : 标准LCD服务句柄
 *
 * \retval : 无
 *
 */
void am_zsn700_lcd_deinit (am_lcd_handle_t handle);


/**
 * \brief LCD 清屏
 *
 * \param[in] handle: 标准LCD服务句柄
 *
 * \retval AM_OK    : 清屏成功
 *         AM_ERROR : 清屏失败
 *
 */
int am_zsn700_lcd_disp_clr (am_lcd_handle_t handle);

/**
 * \brief LCD 画屏函数
 *
 * \param[in] handle: 标准LCD服务句柄
 * \param[in] addr  : 显存地址索引
 * \param[in] data  : 写入到显存的数据
 *
 * \retval AM_OK    : 画屏成功
 *         AM_ERROR : 画屏失败
 *
 */
int am_zsn700_lcd_disp_write (am_lcd_handle_t handle,
                              uint32_t addr,
                              uint32_t data);

/**
 * \brief LCD DMA模式设置 （中断触发）
 *
 * \param[in] handle   : 标准LCD服务句柄
 * \param[in] p_lcd_cb : 用户回调函数
 * \param[in] p_arg    : 用户回调参数
 *
 * \retval AM_OK    : 成功
 *         AM_ERROR : 失败
 *
 */
int am_zsn700_lcd_dma_int_set (am_lcd_handle_t handle,
                               void (*p_lcd_cb) (void *),
                               void *p_arg);

/**
 * \brief LCD 使能
 *
 * \param[in] handle   : 标准LCD服务句柄
 * \param[in] lcd_mode : LCD显示模式
 *
 * \retval AM_OK    : 成功
 *         AM_ERROR : 失败
 *
 */
int am_zsn700_lcd_enable (am_lcd_handle_t handle, uint8_t lcd_mode);

/**
 * \brief LCD COM SEG 端口配置
 *
 * \param[in] handle : 标准LCD服务句柄
 * \param[in] p_port : LCD COM-SEG端口设置结构体指针
 *
 * \retval AM_OK    : 成功
 *         AM_ERROR : 失败
 *
 */
int am_zsn700_lcd_com_seg_init (am_lcd_handle_t         handle,
                                am_zsn700_lcd_com_seg_t *p_port);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSN700_LCD_H */

/* end of file */
