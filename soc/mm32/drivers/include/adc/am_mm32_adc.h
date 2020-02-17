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
 * \brief ADC驱动，服务ADC标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-12  ari, first implementation
 * \endinternal
 */

#ifndef  __AM_MM32_ADC_H
#define  __AM_MM32_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_adc.h"

#include "hw/amhw_mm32_adc.h"

/**
 * \addtogroup am_mm32_if_adc
 * \copydoc am_mm32_adc.h
 * @{
 */

/**
 * \brief ADC设备信息
 */
typedef struct am_mm32_adc_devinfo {

    /** \brief ADC寄存器块的基地址 */
    uint32_t   adc_reg_base;

    /** \brief ADC中断号 */
    int16_t    inum;

    /** \brief 时钟ID */
    int        clk_num;

    /**
     * \brief ADC参考电压，单位：mV
     *
     * 参考电压 Vref = (Vrefp - Vrefn)  为了在最大采样速率时
     * 稳定运行ADC，确保：VREFP = VDDA Vrefn 典型值为： Vss 或 Vssa 。
     *
     * \note 该参考电压由具体的电路决定
     *
     */
    uint32_t   vref;

    /** \brief 转换精度 */
    uint8_t    bits_mode;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_mm32_adc_devinfo_t;

/**
 * \brief ADC设备实例
 */
typedef struct am_mm32_adc_dev {

    /** \brief ADC标准服务 */
    am_adc_serv_t                  adc_serve;

    /** \brief 指向ADC设备信息的指针 */
    const am_mm32_adc_devinfo_t    *p_devinfo;

    /** \brief 存放用户启动转换后的回调函数 */
    am_adc_seq_cb_t                pfn_callback;

    /** \brief 用户启动回调函数的回调函数参数 */
    void                          *p_arg;

    /** \brief 当前转换的通道 */
    uint32_t                       chan;

    /** \brief 当前转换的序列描述符数组首地址 */
    am_adc_buf_desc_t             *p_desc;

    /** \brief 当前转换的序列描述符数量 */
    uint32_t                       desc_num;

    /** \brief 转换的次数 */
    uint32_t                       count;

    /** \brief 转换标志 */
    uint32_t                       flags;

    /** \brief 对当前序列描述符已经采样的次数 */
    uint32_t                       conv_cnt;

    /** \brief 正在执行当前序列描述符的索引 */
    uint32_t                       desc_index;

    /** \brief 对整个序列转换完成的次数 */
    uint32_t                       seq_cnt;

    /** \brief 数据右移位数 */
    uint16_t                       right_bit;

} am_mm32_adc_dev_t;

/**
 * \brief ADC初始化
 *
 * 默认初始化序列A
 *
 * \param[in] p_dev     : 指向ADC设备的指针
 * \param[in] p_devinfo : 指向ADC设备信息的指针
 *
 * \return ADC标准服务操作句柄 如果为 NULL，表明初始化失败
 */
am_adc_handle_t am_mm32_adc_init (am_mm32_adc_dev_t           *p_dev,
                                 const am_mm32_adc_devinfo_t *p_devinfo);

/**
 * \brief ADC去初始化
 *
 * 默认去初始化序列A
 *
 * \param[in] handle : am_mm32_adc_init() 初始化函数获得的ADC服务句柄
 *
 * \return 无
 */
void am_mm32_adc_deinit (am_adc_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MM32_ADC_H */

/* end of file */
