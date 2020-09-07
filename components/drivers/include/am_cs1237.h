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
 * \brief  CS1237 应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 17-9-15  lqy, first implementation.
 * \endinternal
 */

#ifndef __AM_CS1237_H
#define __AM_CS1237_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup am_if_cs1237
 * @copydoc am_cs1237.h
 * @{
 */

#include "ametal.h"
#include "am_adc.h"
/**
 * \brief CS1237 设备信息结构体
 */
typedef struct am_cs1237_adc_devinfo{

    /** \brief IO模拟SPI SCLK引脚选择 */
    uint8_t      clk;

    /**
     * \brief cs1237_out引脚选择
     *
     * \note 如果硬件没有对DRDY/DOUT引脚分离，该引脚应该和in_pin相同
     * */
    uint8_t      out_pin;

    /**
     * \brief cs1237_in引脚选择
     *
     * \note 如果硬件没有对DRDY/DOUT引脚分离，该引脚应该和out_pin相同
     * */
    uint8_t      in_pin;

    /** \brief REF基准源状态设置,0使用内部基准源，1使用外部基准源 */
    uint8_t      refo_off;

    /**
     * \brief ADC参考电压，单位：mV
     *
     * \note 该参考电压由具体的电路决定
     *
     */
    uint32_t     vref;

    /** \brief CLK时序高电平延时时间 ，微妙级别*/
    uint8_t      high_clk_delay;

    /** \brief CLK时序低电平延时时间 ，微妙级别*/
    uint8_t      low_clk_delay;

    /** \brief 进入低功耗模式clk高电平持续时间 ，微妙级别，理论值100us*/
    uint8_t      powerdown_delay;

}am_cs1237_adc_devinfo_t;

/** \brief 触发回调函数 */
typedef void (*am_cs1237_code_read_cb_t) (void *p_arg,uint32_t code);

/**
 * \brief CS1237 设备结构体
 */
typedef struct am_cs1237_adc_dev{

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
    struct am_cs1237_trigger_info {

        /** \brief 触发回调函数 */
        am_cs1237_code_read_cb_t   pfn_callback;
        /** \brief 回调函数的参数 */
        void                      *p_arg;

    } triginfo;/**< \brief INT引脚的触发信息 */

    /** \brief 设备信息 */
    const am_cs1237_adc_devinfo_t *p_devinfo;

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

}am_cs1237_adc_dev_t;

typedef am_cs1237_adc_dev_t * am_cs1237_adc_handle_t; /**< \brief 句柄定义 */

/**
 * \name CS1237通道选择
 * @{
 */

#define AM_CS1237_CHANNEL_A         0      /**< \brief 通道 A,默认 */
#define AM_CS1237_CHANNEL_B         1      /**< \brief 芯片保留使用位 */
#define AM_CS1237_CHANNEL_TEMP      2      /**< \brief 温度 */
#define AM_CS1237_CHANNEL_SHORT     3      /**< \brief 内短 */

/** @} */

/**
 * \name CS1237 PGA选择
 * @{
 */

#define AM_CS1237_PGA_1             0       /**< \brief 放大倍数 1*/
#define AM_CS1237_PGA_2             1       /**< \brief 放大倍数 2*/
#define AM_CS1237_PGA_64            2       /**< \brief 放大倍数 64*/
#define AM_CS1237_PGA_128           3       /**< \brief 放大倍数 128，默认*/

/** @} */

/**
 * \name CS1237 输出速率选择
 * @{
 */

#define AM_CS1237_SPEED_10HZ        0       /**< \brief ADC输出速率10HZ，默认  */
#define AM_CS1237_SPEED_40HZ        1       /**< \brief ADC输出速率40HZ     */
#define AM_CS1237_SPEED_640HZ       2       /**< \brief ADC输出速率640HZ    */
#define AM_CS1237_SPEED_1280HZ      3       /**< \brief ADC输出速率1280HZ   */

/** @} */

/**
 * \brief CS1237 设备初始化
 *
 * \param[in] p_dev      :指向CS1237设备结构体的指针
 * \param[in] p_devinfo  :指向CS1237设备信息结构体的指针
 *
 * \return CS1237服务操作句柄,如果为 NULL，表明初始化失败
 */
am_cs1237_adc_handle_t am_cs1237_init(am_cs1237_adc_dev_t            *p_dev,
                                  const am_cs1237_adc_devinfo_t  *p_devinfo);

/**
 * \brief CS1237 设备解初始化
 *
 * \param[in] handle : CS1237操作句柄
 *
 * \return 无
 */
void am_cs1237_deinit (am_cs1237_adc_handle_t handle);

/**
 * \brief CS1237 配置寄存器读
 *
 * \param[in] p_dev : CS1237操作句柄
 *
 * \return adc配置寄存器值
 */
uint8_t am_cs1237_config_reg_read(am_cs1237_adc_dev_t  *p_dev);

/**
 * \brief CS1237 配置寄存器pga写
 *
 * \param[in] p_dev : CS1237操作句柄
 * \param[in] pga   : pga相关宏
 *
 * \retval  AM_OK     : 设置成功
 *          AM_ERROR  : 设置失败，ADC未准备好
 */
int am_cs1237_pga_set(am_cs1237_adc_dev_t  *p_dev, uint32_t pga);

/**
 * \brief CS1237 配置寄存器ch写
 *
 * \param[in] p_dev : CS1237操作句柄
 * \param[in] ch    : ch相关宏
 *
 * \retval  AM_OK     : 设置成功
 *          AM_ERROR  : 设置失败，ADC未准备好
 */
int am_cs1237_ch_set(am_cs1237_adc_dev_t  *p_dev, uint32_t ch);

/**
 * \brief CS1237 配置寄存器speed写
 *
 * \param[in] p_dev : CS1237操作句柄
 * \param[in] speed : speed相关宏
 *
 * \retval  AM_OK     : 设置成功
 *          AM_ERROR  : 设置失败，ADC未准备好
 */
int am_cs1237_out_speed_set(am_cs1237_adc_dev_t  *p_dev, uint32_t speed);

/**
 * \brief CS1237 配置寄存器ref
 *
 * \param[in] p_dev : CS1237操作句柄
 * \param[in] speed : 0 使用内部基准源     1使用外部基准源
 *
 * \retval  AM_OK     : 设置成功
 *          AM_ERROR  : 设置失败，ADC未准备好
 */
int am_cs1237_ref_set(am_cs1237_adc_dev_t  *p_dev, uint8_t flag);

/**
 * \brief CS1237 pga放大倍数读
 *
 * \param[in] p_dev : CS1237操作句柄
 *
 * \return pga放大倍数
 */
uint32_t am_cs1237_pga_get(am_cs1237_adc_dev_t  *p_dev);

/**
 * \brief CS1237 通道号读
 *
 * \param[in] p_dev : CS1237操作句柄
 *
 * \return ch通道号
 */
uint8_t am_cs1237_ch_get(am_cs1237_adc_dev_t  *p_dev);

/**
 * \brief CS1237 adc输出速率读
 *
 * \param[in] p_dev : CS1237操作句柄
 *
 * \return adc输出速率
 */
uint32_t am_cs1237_out_speed_get(am_cs1237_adc_dev_t  *p_dev);

/**
 * \brief CS1237 允许读
 *
 * \param[in] p_dev   : CS1237操作句柄
 *
 * \retval  AM_OK     : 操作成功
 */
int am_cs1237_read_int_enable(am_cs1237_adc_dev_t  *p_dev);

/**
 * \brief CS1237 禁止读
 *
 * \param[in] p_dev   : CS1237操作句柄
 *
 * \retval  AM_OK     : 操作成功
 */
int am_cs1237_read_int_disable(am_cs1237_adc_dev_t  *p_dev);

/**
 * \brief CS1237 轮循读adc采集值
 *
 * \param[in]  p_dev   : CS1237操作句柄
 * \param[out] val     : 采样数据
 *
 * \retval  AM_OK 采集成功
 */
int am_cs1237_read_polling(am_cs1237_adc_dev_t *p_dev, uint32_t *val);

/**
 * \brief CS1237 进入低功耗模式
 *
 * \param[in] p_dev   : CS1237操作句柄
 *
 * \retval  无
 */
void am_cs1237_power_down_enter(am_cs1237_adc_dev_t  *p_dev);

/**
 * \brief CS1237 退出低功耗模式
 *
 * \param[in] p_dev   : CS1237操作句柄
 *
 * \retval  无
 */
void am_cs1237_power_down_out(am_cs1237_adc_dev_t  *p_dev);

/**
 * \brief CS1237 连接中断回调函数
 *
 * \param[in] p_dev   : CS1237操作句柄
 * \param[in] p_fun   : CS1237中断回调函数
 * \param[in] p_arg   : CS1237中断回调函数参数
 *
 * \retval  无
 */
void am_cs1237_int_connect(am_cs1237_adc_dev_t      *p_dev ,
                           am_cs1237_code_read_cb_t  p_fun,
                           void                     *p_arg);

/**
 * \brief CS1237 删除中断回调函数
 *
 * \param[in] p_dev   : CS1237操作句柄
 * \param[in] p_fun   : CS1237中断回调函数
 * \param[in] p_arg   : CS1237中断回调函数参数
 *
 * \retval  无
 */
void am_cs1237_int_disconnect(am_cs1237_adc_dev_t      *p_dev ,
                              am_cs1237_code_read_cb_t  p_fun,
                              void                     *p_arg);

/**
 * \brief CS1237 获得标准adc句柄
 * \param[in] p_dev : CS1237操作句柄
 *
 * \retval 标准adc操作句柄
 */
am_adc_handle_t am_cs1237_standard_adc_handle_get(am_cs1237_adc_dev_t *p_dev);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SPI_H */

/*end of file */


