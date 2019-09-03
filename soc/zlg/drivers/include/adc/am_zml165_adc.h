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
 * \brief  ZML165_ADC 应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 17-9-15  lqy, first implementation.
 * \endinternal
 */

#ifndef __AM_ZML165_ADC_H
#define __AM_ZML165_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup am_if_zml165_adc
 * @copydoc am_zml165_adc.h
 * @{
 */

#include "ametal.h"
#include "am_adc.h"


/**
 * \name ZML165_ADC参考电压选择
 * @{
 */
#define AM_ZML165_ADC_VOUT_ENABLE       0      /**< \brief ADC VOUT使能 */
#define AM_ZML165_ADC_VOUT_DISABLE      1      /**< \brief ADC VOUT禁能  */

/**
 * \name ZML165_ADC通道选择
 * @{
 */

#define AM_ZML165_ADC_CHANNEL_A         0      /**< \brief 通道 A */
#define AM_ZML165_ADC_CHANNEL_B         1      /**< \brief 通道 B */
#define AM_ZML165_ADC_CHANNEL_TEMP      2      /**< \brief 温度 */
#define AM_ZML165_ADC_CHANNEL_SHORT     3      /**< \brief 内短 */

/** @} */

/**
 * \name ZML165_ADC PGA选择
 * @{
 */
#define AM_ZML165_ADC_PGA_1             0       /**< \brief 增益倍数 1*/
#define AM_ZML165_ADC_PGA_2             1       /**< \brief 增益倍数 2*/
#define AM_ZML165_ADC_PGA_64            2       /**< \brief 增益倍数 64*/
#define AM_ZML165_ADC_PGA_128           3       /**< \brief 增益倍数 128*/

/** @} */

/**
 * \name ZML165_ADC 输出速率选择
 * @{
 */
#define AM_ZML165_ADC_SPEED_10HZ        0       /**< \brief ADC输出速率10HZ */
#define AM_ZML165_ADC_SPEED_40HZ        1       /**< \brief ADC输出速率40HZ  */
#define AM_ZML165_ADC_SPEED_640HZ       2       /**< \brief ADC输出速率640HZ */
#define AM_ZML165_ADC_SPEED_1280HZ      3       /**< \brief ADC输出速率1280HZ*/

/** @} */


typedef struct am_zml165_adc_config{
    int8_t      pga;        /** \brief 输出增益倍数设置,若采用默认值则填入  -1 */
    int8_t      speed;      /** \brief 输出速率设置,若采用默认值则填入  -1 */
    int8_t      channel;    /** \brief 输出速率通道设置,若采用默认值则填入  -1 */
    int8_t      refo_off;   /** \brief REF基准源状态设置,若采用默认值则填入  -1 */
}am_zml165_adc_config_t;


/**
 * \brief ZML165_ADC 设备信息结构体
 */
typedef struct am_zml165_adc_devinfo{

    /*\brief  ADC初始配置项 */
    am_zml165_adc_config_t   config;

    /**
     * \brief ADC参考电压，单位：mV
     * \note 该参考电压由具体的电路决定
     */
    uint32_t                 vref;

}am_zml165_adc_devinfo_t;

/** \brief 触发回调函数 */
typedef void (*am_zml165_adc_code_read_cb_t) (void *p_arg,uint32_t code);

/**
 * \brief ZML165_ADC 设备结构体
 */
typedef struct am_zml165_adc_dev{

    /** \brief 放大倍数 */
    uint8_t                        pga;
    /** \brief 输出速率 */
    uint8_t                        out_speed;
    /** \brief 通道 */
    uint8_t                        ch;
    /** \brief REF使能 */
    uint8_t                        ref;
    /** \brief 是否开启中断模式 */
    am_bool_t                      is_int;

    /** \brief INT引脚的触发信息类型 */
    struct am_zml165_adc_trigger_info {

        /** \brief 触发回调函数 */
        am_zml165_adc_code_read_cb_t   pfn_callback;
        /** \brief 回调函数的参数 */
        void                      *p_arg;

    } triginfo;/**< \brief INT引脚的触发信息 */

    /** \brief 设备信息 */
    const am_zml165_adc_devinfo_t *p_devinfo;

    /** \brief ADC标准服务 */
    am_adc_serv_t                  adc_serve;

    /** \brief 存放用户启动转换后的回调函数 */
    am_adc_seq_cb_t                pfn_callback;

    /** \brief 用户启动回调函数的回调函数参数 */
    void                          *p_arg;

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

}am_zml165_adc_dev_t;

typedef am_zml165_adc_dev_t * am_zml165_adc_handle_t; /**< \brief 句柄定义 */

/** @} */

/**
 * \brief ZML165_ADC 设备初始化
 *
 * \param[in] p_dev      :指向ZML165_ADC设备结构体的指针
 * \param[in] p_devinfo  :指向ZML165_ADC设备信息结构体的指针
 *
 * \return ZML165_ADC服务操作句柄,如果为 NULL，表明初始化失败
 */
am_zml165_adc_handle_t am_zml165_adc_init(am_zml165_adc_dev_t            *p_dev,
                                          const am_zml165_adc_devinfo_t  *p_devinfo);

/**
 * \brief ZML165_ADC 设备解初始化
 *
 * \param[in] handle : ZML165_ADC操作句柄
 *
 * \return 无
 */
void am_zml165_adc_deinit (am_zml165_adc_handle_t handle);

/**
 * \brief ZML165_ADC 配置寄存器获取
 *
 * \param[in] p_dev : ZML165_ADC操作句柄
 *
 * \return adc配置寄存器值
 */
uint8_t am_zml165_adc_config_reg_read(am_zml165_adc_dev_t  *p_dev);

/**
 * \brief ZML165 ADC 写入配置
 *
 * \param[in] p_dev : ZML165_ADC操作句柄
 * \param[in] speed : p_config 指向配置结构体的指针
 *
 * \retval  AM_OK     : 设置成功
 *          AM_ERROR  : 设置失败，ADC未准备好
 */
uint8_t am_zml165_adc_config_load(am_zml165_adc_dev_t     *p_dev,
                                  am_zml165_adc_config_t  *p_config);


/**
 * \brief ZML165_ADC pga放大倍数获取
 *
 * \param[in] p_dev : ZML165_ADC操作句柄
 *
 * \return pga放大倍数
 */
uint32_t am_zml165_adc_pga_get(am_zml165_adc_dev_t  *p_dev);

/**
 * \brief ZML165 ADC 通道号获取
 *
 * \param[in] p_dev : ZML165_ADC操作句柄
 *
 * \return ch通道号
 */
uint8_t am_zml165_adc_ch_get(am_zml165_adc_dev_t  *p_dev);

/**
 * \brief ZML165_ADC adc输出速率读
 *
 * \param[in] p_dev : ZML165_ADC操作句柄
 *
 * \return adc输出速率
 */
uint32_t am_zml165_adc_out_speed_get(am_zml165_adc_dev_t  *p_dev);

/**
 * \brief ZML165_ADC 允许读
 *
 * \param[in] p_dev   : ZML165_ADC操作句柄
 *
 * \retval  AM_OK     : 操作成功
 */
int am_zml165_adc_read_int_enable(am_zml165_adc_dev_t  *p_dev);

/**
 * \brief ZML165_ADC 禁止读
 *
 * \param[in] p_dev   : ZML165_ADC操作句柄
 *
 * \retval  AM_OK     : 操作成功
 */
int am_zml165_adc_read_int_disable(am_zml165_adc_dev_t  *p_dev);

/**
 * \brief ZML165_ADC 轮循读adc采集值
 *
 * \param[in]  p_dev   : ZML165_ADC操作句柄
 * \param[out] val     : 采样数据
 *
 * \retval  AM_OK 采集成功
 */
int am_zml165_adc_read_polling(am_zml165_adc_dev_t *p_dev, uint32_t *val);

/**
 * \brief ZML165_ADC 进入低功耗模式
 *
 * \param[in] p_dev   : ZML165_ADC操作句柄
 *
 * \retval  无
 */
void am_zml165_adc_power_down_enter(am_zml165_adc_dev_t  *p_dev);

/**
 * \brief ZML165_ADC 退出低功耗模式
 *
 * \param[in] p_dev   : ZML165_ADC操作句柄
 *
 * \retval  无
 */
void am_zml165_adc_power_down_out(am_zml165_adc_dev_t  *p_dev);

/**
 * \brief ZML165_ADC 连接中断回调函数
 *
 * \param[in] p_dev   : ZML165_ADC操作句柄
 * \param[in] p_fun   : ZML165_ADC中断回调函数
 * \param[in] p_arg   : ZML165_ADC中断回调函数参数
 *
 * \retval  无
 */
void am_zml165_adc_int_connect(am_zml165_adc_dev_t      *p_dev ,
                               am_zml165_adc_code_read_cb_t  p_fun,
                               void                     *p_arg);

/**
 * \brief ZML165_ADC 删除中断回调函数
 *
 * \param[in] p_dev   : ZML165_ADC操作句柄
 * \param[in] p_fun   : ZML165_ADC中断回调函数
 * \param[in] p_arg   : ZML165_ADC中断回调函数参数
 *
 * \retval  无
 */
void am_zml165_adc_int_disconnect(am_zml165_adc_dev_t      *p_dev ,
                                  am_zml165_adc_code_read_cb_t  p_fun,
                                  void                         *p_arg);

/**
 * \brief ZML165_ADC 获得标准adc句柄
 * \param[in] p_dev : ZML165_ADC操作句柄
 *
 * \retval 标准adc操作句柄
 */
am_adc_handle_t am_zml165_adc_standard_adc_handle_get(am_zml165_adc_dev_t *p_dev);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SPI_H */

/*end of file */


