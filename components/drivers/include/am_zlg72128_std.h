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
 * \brief ZLG72128通用接口（包括数码管接口和按键接口）驱动
 *
 * \internal
 * \par Modification History
 * - 1.00 17-05-22  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_ZLG72128_STD_H
#define __AM_ZLG72128_STD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
#include "am_zlg72128.h"
#include "am_digitron_dev.h"

/**
 * @addtogroup am_if_zlg72128_std
 * @copydoc am_zlg72128_std.h
 * @{
 */

#define AM_ZLG72128_STD_KEY_ROW_0  (1 << 0) /**< \brief 使用第0行      */
#define AM_ZLG72128_STD_KEY_ROW_1  (1 << 1) /**< \brief 使用第1行      */
#define AM_ZLG72128_STD_KEY_ROW_2  (1 << 2) /**< \brief 使用第2行      */
#define AM_ZLG72128_STD_KEY_ROW_3  (1 << 3) /**< \brief 使用第3行（功能按键行）*/
                                                 
#define AM_ZLG72128_STD_KEY_COL_0  (1 << 0) /**< \brief 使用第0列      */
#define AM_ZLG72128_STD_KEY_COL_1  (1 << 1) /**< \brief 使用第1列      */
#define AM_ZLG72128_STD_KEY_COL_2  (1 << 2) /**< \brief 使用第2列      */
#define AM_ZLG72128_STD_KEY_COL_3  (1 << 3) /**< \brief 使用第3列      */
#define AM_ZLG72128_STD_KEY_COL_4  (1 << 4) /**< \brief 使用第4列      */
#define AM_ZLG72128_STD_KEY_COL_5  (1 << 5) /**< \brief 使用第5列      */
#define AM_ZLG72128_STD_KEY_COL_6  (1 << 6) /**< \brief 使用第6列      */
#define AM_ZLG72128_STD_KEY_COL_7  (1 << 7) /**< \brief 使用第7列      */

/**
 * \brief ZLG72128用作通用数码管和按键的设备信息
 */
typedef struct am_zlg72128_std_devinfo {
    am_zlg72128_devinfo_t  base_info;         /**< \brief 基础设备信息         */
    am_digitron_devinfo_t  id_info;           /**< \brief 数码管显示器ID         */
    uint16_t               blink_on_time;     /**< \brief 一个闪烁周期内，点亮的时间，如 500ms */
    uint16_t               blink_off_time;    /**< \brief 一个闪烁周期内，熄灭的时间，如 500ms */
    uint8_t                key_use_row_flags; /**< \brief 实际使用的行标志  */
    uint8_t                key_use_col_flags; /**< \brief 实际使用的列标志 */
    const int             *p_key_codes;    /**< \brief 按键编码， 其大小为 key_rows * key_cols */
    uint8_t                num_digitron;   /**< \brief 实际使用的数码管个数 */
} am_zlg72128_std_devinfo_t;

/**
 * \brief ZLG72128用作通用数码管和按键的设备
 */
typedef struct am_zlg72128_std_digitron_key_dev {

    /** 使用底层的ZLG72128功能    */
    am_zlg72128_dev_t     zlg72128_dev;

    /** \brief 数码管设备      */
    am_digitron_dev_t     digitron_dev;

    /** \brief ZLG72128句柄      */
    am_zlg72128_handle_t  handle;

    /** \brief 功能键值，将功能键转换为通用按键使用      */
    uint8_t               f_key;
    
    /** \brief 实际使用的按键行数  */
    uint8_t               num_rows;

    /** \brief 实际使用按键的列数 */
    uint8_t               num_cols;

    /** \brief 闪烁标记  */
    uint16_t              blink_flags;

    /** \brief 自定义解码函数  */
    uint16_t  (*pfn_decode)(uint16_t code);

    /** \brief 用于保存设备信息指针 */
    const am_zlg72128_std_devinfo_t  *p_info;

} am_zlg72128_std_dev_t;

/**
 * \brief ZLG72128初始化函数
 *
 * 使用ZLG72128前，应该首先调用该初始化函数，以获取操作ZLG72128的handle。
 *
 * \param[in] p_dev      : 指向ZLG72128设备的指针
 * \param[in] p_info     : 指向ZLG72128设备信息的指针
 * \param[in] i2c_handle : I2C标准服务操作句柄（使用该I2C句柄与ZLG72128通信）
 *
 * \return AM_OK，初始化成功；否则，初始化失败
 */
int am_zlg72128_std_init (am_zlg72128_std_dev_t           *p_dev,
                          const am_zlg72128_std_devinfo_t *p_info,
                          am_i2c_handle_t                  i2c_handle);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG72128_STD_H */

/* end of file */
