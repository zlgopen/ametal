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
 * \brief I2S驱动
 * \internal
 * \par Modification history
 * - 1.00 17-4-21  nwt, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F07X_I2S_H
#define __AM_HC32F07X_I2S_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "amhw_hc32f07x_i2s.h"

/**
 * \addtogroup am_hc32f07x_if_i2s
 * \copydoc am_hc32f07x_i2s.h
 * @{
 */


/**
 * \brief i2s设备
 */
typedef struct am_hc32f07x_i2s_dev {
    amhw_hc32f07x_i2s_clk_t     clk_sel;       /**< \brief I2S 的输入时钟 */
    uint32_t                    fre_mclk;      /**< \brief I2S 的输入时钟频率 */
    uint32_t                    fshz;          /**< \brief I2S 采样频率 */
    am_bool_t                   mclk_en;       /**< \brief I2S 主时钟输出使能 */
    amhw_hc32f07x_i2s_mode_t    mode;          /**< \brief I2S 模式  */
    amhw_hc32f07x_i2s_data_t    data_len;      /**< \brief I2S 通道长度  */
    amhw_hc32f07x_i2s_std_t     std;           /**< \brief I2S 标准  */
} am_hc32f07x_i2s_dev_t;

/**
 * \brief i2s初始化
 *
 * \param[in] p_hw_i2s : 指向I2S寄存器结构体的指针
 * \param[in] p_dev    : i2s设备信息
 *
 * \return 
 */
void am_hc32f07x_i2s_init (amhw_hc32f07x_i2s_t *p_hw_i2s,
                           am_hc32f07x_i2s_dev_t *p_dev);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F07X_I2S_H */

/* end of file */
