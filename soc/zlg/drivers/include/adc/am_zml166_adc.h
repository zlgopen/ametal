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
 * \brief AM_ZML166_ADC 应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 18-05-14  pea, first implementation
 * \endinternal
 */

#ifndef __AM_ZML166_ADC_H
#define __AM_ZML166_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup am_if_AM_ZML166_ADC
 * @copydoc am_AM_ZML166_ADC.h
 * @{
 */

#include "ametal.h"
#include "am_adc.h"
#include "am_wait.h"

#define  AM_ZML166_ADC_CFG_NUM   7 /**< \brief 系统配置寄存器数量 */
/** \brief 电压校准系数保存地址块  */
#define  FLASH_BLOCK_NUM         63
/** \brief PT100校准系数保存地址  */
#define  PT100_PARA_SAVE_ADDRESS  (((FLASH_BLOCK_NUM * 1024)) + 4 * 16)
/*******************************************************************************
  AM_ZML166_ADC 寄存器地址
*******************************************************************************/
/**
 * \name AM_ZML166_ADC 寄存器地址
 * @{
 */
#define AM_ZML166_ADC_SYS_ADDR         (0x00) /**< \brief 系统配置寄存器地址 */
#define AM_ZML166_ADC_ADC0_ADDR        (0x01) /**< \brief ADC 配置寄存器 0 地址 */
#define AM_ZML166_ADC_ADC1_ADDR        (0x02) /**< \brief ADC 配置寄存器 1 地址 */
#define AM_ZML166_ADC_ADC2_ADDR        (0x03) /**< \brief ADC 配置寄存器 2 地址 */
#define AM_ZML166_ADC_ADC3_ADDR        (0x04) /**< \brief ADC 配置寄存器 3 地址 */
#define AM_ZML166_ADC_ADC4_ADDR        (0x05) /**< \brief ADC 配置寄存器 4 地址 */
#define AM_ZML166_ADC_ADC5_ADDR        (0x06) /**< \brief ADC 配置寄存器 5 地址 */
#define AM_ZML166_ADC_ADO_ADDR         (0x09) /**< \brief ADC 转换数据寄存器地址 */
#define AM_ZML166_ADC_ADS_ADDR         (0x0A) /**< \brief ADC 转换数据读取标准寄存器地址 */
/**
 * @}
 */

/*******************************************************************************
  AM_ZML166_ADC SYS 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_TMODE_MASK       (3 << 6) /**< \brief 测量模式掩码 */
#define AM_ZML166_ADC_TMODE_MANUAL     (0 << 6) /**< \brief 手动测量模式（自由配置） */

#define AM_ZML166_ADC_PMODE_MASK       (3 << 4) /**< \brief 工作模式掩码 */
#define AM_ZML166_ADC_PMODE_MANUAL     (3 << 4) /**< \brief 自由模式(ADC 自由配置) */

#define AM_ZML166_ADC_ENREF_ENABLE     (1 << 3) /**< \brief VREF 使能 */
#define AM_ZML166_ADC_ENREF_DISABLE    (0 << 3) /**< \brief VREF 关闭 */

#define AM_ZML166_ADC_ENADC_ENABLE     (1 << 2) /**< \brief ADC 使能 */
#define AM_ZML166_ADC_ENADC_DISABLE    (0 << 2) /**< \brief ADC 关闭 */

#define AM_ZML166_ADC_ENLDO_ENABLE     (1 << 1) /**< \brief LDO 使能 */
#define AM_ZML166_ADC_ENLDO_DISABLE    (0 << 1) /**< \brief LDO 关闭 */

/*******************************************************************************
  AM_ZML166_ADC ADC0 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_IMODE_PER        (1 << 7)    /**< \brief 调制器性能模式电流(普通模式电流x2) */
#define AM_ZML166_ADC_IMODE_NORMAL     (0 << 7)    /**< \brief 调制器普通模式电流 */

#define AM_ZML166_ADC_FS_662_22        (1 << 6)    /**< \brief 采样频率为 662.22KHz */
#define AM_ZML166_ADC_FS_331_11        (0 << 6)    /**< \brief 采样频率为 331.11KHz */

#define AM_ZML166_ADC_INNS_AIN(ch)     ((ch) << 3) /**< \brief PGA 负端输入信号：AIN4~AIN1 */
#define AM_ZML166_ADC_INNS_MASK        (7 << 3)    /**< \brief PGA 负端输入信号掩码 */
#define AM_ZML166_ADC_INNS_1_2_VS      (5 << 3)    /**< \brief PGA 负端输入信号：1/2 VS（共模电压） */
#define AM_ZML166_ADC_INNS_GND         (4 << 3)    /**< \brief PGA 负端输入信号：GND（仅在 TMODE=10 有效） */
#define AM_ZML166_ADC_INNS_AIN4        (3 << 3)    /**< \brief PGA 负端输入信号：AIN4 */
#define AM_ZML166_ADC_INNS_AIN3        (2 << 3)    /**< \brief PGA 负端输入信号：AIN3 */
#define AM_ZML166_ADC_INNS_AIN2        (1 << 3)    /**< \brief PGA 负端输入信号：AIN2 */
#define AM_ZML166_ADC_INNS_AIN1        (0 << 3)    /**< \brief PGA 负端输入信号：AIN1 */

#define AM_ZML166_ADC_INPS_AIN(ch)     ((ch) << 0) /**< \brief PGA 正端输入信号：AIN3~AIN0 */
#define AM_ZML166_ADC_INPS_MASK        (7 << 0)    /**< \brief PGA 正端输入信号掩码 */
#define AM_ZML166_ADC_INPS_1_2_VS      (5 << 0)    /**< \brief PGA 正端输入信号：1/2 VS（共模电压） */
#define AM_ZML166_ADC_INPS_AIN3        (3 << 0)    /**< \brief PGA 正端输入信号：AIN3 */
#define AM_ZML166_ADC_INPS_AIN2        (2 << 0)    /**< \brief PGA 正端输入信号：AIN2 */
#define AM_ZML166_ADC_INPS_AIN1        (1 << 0)    /**< \brief PGA 正端输入信号：AIN1 */
#define AM_ZML166_ADC_INPS_AIN0        (0 << 0)    /**< \brief PGA 正端输入信号：AIN0 */
/*******************************************************************************
  AM_ZML166_ADC ADC1 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_DR_MASK          (7 << 5)   /**< \brief ADC 输出速率掩码 */
#define AM_ZML166_ADC_DR_200           (4 << 5)   /**< \brief ADC 输出速率为 200 Hz */
#define AM_ZML166_ADC_DR_100           (3 << 5)   /**< \brief ADC 输出速率为 100 Hz */
#define AM_ZML166_ADC_DR_50            (2 << 5)   /**< \brief ADC 输出速率为 50 Hz */
#define AM_ZML166_ADC_DR_25            (1 << 5)   /**< \brief ADC 输出速率为 25 Hz */
#define AM_ZML166_ADC_DR_12_5          (0 << 5)   /**< \brief ADC 输出速率为 12.5 Hz */

#define AM_ZML166_ADC_BUFBP_MASK       (1 << 4)   /**< \brief Buffer 控制位掩码 */
#define AM_ZML166_ADC_BUFBP_DISABLE    (1 << 4)   /**< \brief Buffer 开启 */
#define AM_ZML166_ADC_BUFBP_ENABLE     (0 << 4)   /**< \brief Buffer 开启 */

#define AM_ZML166_ADC_PGA_MASK         (3 << 2)   /**< \brief PGA 增益掩码 */
#define AM_ZML166_ADC_PGA_32           (3 << 2)   /**< \brief PGA 增益为 32 */
#define AM_ZML166_ADC_PGA_16           (2 << 2)   /**< \brief PGA 增益为 16 */
#define AM_ZML166_ADC_PGA_1            (0 << 2)   /**< \brief PGA 增益为 1 */

#define AM_ZML166_ADC_ADGN_MASK        (3 << 0)   /**< \brief 调制器增益掩码 */
#define AM_ZML166_ADC_ADGN_8           (3 << 0)   /**< \brief 调制器增益为 8，DR 下降为 1/4 */
#define AM_ZML166_ADC_ADGN_4           (2 << 0)   /**< \brief 调制器增益为 4，DR 下降为 1/2 */
#define AM_ZML166_ADC_ADGN_2           (1 << 0)   /**< \brief 调制器增益为 2 */
#define AM_ZML166_ADC_ADGN_1           (0 << 0)   /**< \brief 调制器增益为 1 */

/*******************************************************************************
  AM_ZML166_ADC ADC3 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_GTCSL_COARSE     (1 << 7) /**< \brief 增益温漂补偿粗调，为精调x6，用于补偿传感器温漂 */
#define AM_ZML166_ADC_GTCSL_FINE       (0 << 7) /**< \brief 精调，用于调整芯片自身温漂 */

#define AM_ZML166_ADC_LVSCP_ENABLE     (1 << 3) /**< \brief 电平移位模块斩波使能，斩波频率 Fs/128 */
#define AM_ZML166_ADC_LVSCP_DISABLE    (0 << 3) /**< \brief 电平移位模块斩波不使能 */

#define AM_ZML166_ADC_LVSHIFT_ENABLE   (1 << 2) /**< \brief 电平移位使能 */
#define AM_ZML166_ADC_LVSHIFT_DISABLE  (0 << 2) /**< \brief 电平移位不使能 */

/*******************************************************************************
  AM_ZML166_ADC ADC4 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_CHOPM_MASK       (3 << 6) /**< \brief 斩波频率掩码 */

#define AM_ZML166_ADC_CHOPM_32_128     (2 << 6) /**< \brief 仪放斩波频率为 fs_clk/32，调制器斩波频率为 fs_clk/128 */
#define AM_ZML166_ADC_CHOPM_DISABLE    (0 << 6) /**< \brief 不开斩波 fs_clk 为 MOD 采样频率 */

#define AM_ZML166_ADC_ADREFS_MASK      (3 << 2) /**< \brief ADC 参考电压掩码 */
#define AM_ZML166_ADC_ADREFS_VREF_GND  (3 << 2) /**< \brief 正参考=内部 VREF，负参考=GND */
#define AM_ZML166_ADC_ADREFS_REFP_REFN (0 << 2) /**< \brief 正参考=外部 REFP，负参考=外部 REFN */

#define AM_ZML166_ADC_LDOS_MASK        (3 << 0) /**< \brief 内部 LDO 输出 VS 电压掩码 */
#define AM_ZML166_ADC_LDOS_3_0         (3 << 0) /**< \brief 内部 LDO 输出 VS 电压为 3.0V */

/*******************************************************************************
  AM_ZML166_ADC ADC5 寄存器
*******************************************************************************/
#define AM_ZML166_ADC_FIL_EN_ENABLE    (1 << 2) /**< \brief Comb 之后的低通滤波器使能 */
#define AM_ZML166_ADC_FIL_EN_DISABLE   (0 << 2) /**< \brief Comb 之后的低通滤波器失能 */

#define AM_ZML166_ADC_FIL_CON1_DISABLE (1 << 1) /**< \brief 滤波器不使用级联结构 */
#define AM_ZML166_ADC_FIL_CON1_ENABLE  (0 << 1) /**< \brief 滤波器使用级联结构 */

#define AM_ZML166_ADC_FIL_CON2_2       (1 << 0) /**< \brief 使用滤波器系数 2 */
#define AM_ZML166_ADC_FIL_CON2_1       (0 << 0) /**< \brief 使用滤波器系数 1 */


/** \brief ADC转换结果值 */
typedef uint32_t am_adc_val_t;
/**

 * \brief AM_ZML166_ADC 设备信息结构体
 */
typedef struct am_zml166_adc_devinfo {

    uint32_t  vref;                             /**< \brief ADC 参考电压，单位：mV */
    uint32_t  timeout;                          /**< \brief 超时时间，单位为毫秒 */

} am_zml166_adc_devinfo_t;

/**
 * \brief zml166_adc 设备结构体
 */
typedef struct am_zml166_adc_dev {

    /** \brief ADC 标准服务 */
    am_adc_serv_t                   adc_serve;
    
    /** \brief ADC 输出速率 */
    uint8_t                         speed;

    /** \brief ADC 增益倍数 */
    uint16_t                        gain;

    /** \brief 中断是否使能 */
    am_bool_t                       irq_en;
    
    /** \brief 数据转换完成等待 */
    am_wait_t                       ready_wait;

    /** \brief ADC 状态 0: 空闲 1:正在转换序列中 */
    uint8_t                         adc_state;

    /** \brief 通道 */
    uint8_t                         chan;

    /** \brief 当前转换的序列描述符数组首地址 */
    am_adc_buf_desc_t              *p_desc;

    /** \brief 当前转换的序列描述符数量 */
    uint32_t                        desc_num;

    /** \brief 转换的次数 */
    uint32_t                        count;

    /** \brief 转换标志 */
    uint32_t                        flags;

    /** \brief 存放用户启动转换后的回调函数 */
    am_adc_seq_cb_t                 pfn_callback;

    /** \brief 用户启动回调函数的回调函数参数 */
    void                           *p_arg;

    /** \brief 对当前序列描述符已经采样的次数 */
    uint32_t                        conv_cnt;

    /** \brief 正在执行当前序列描述符的索引 */
    uint32_t                        desc_index;

    /** \brief 对整个序列转换完成的次数 */
    uint32_t                        seq_cnt;

    /** \brief 设备信息 */
    const am_zml166_adc_devinfo_t  *p_devinfo;

    /**< \brief 校验ADS寄存器使能开关*/
    int                             verify_flag;
} am_zml166_adc_dev_t;

/**
 * \brief AM_ZML166_ADC 句柄定义
 */
typedef am_zml166_adc_dev_t *am_zml166_adc_handle_t;

/**
 * \brief AM_ZML166_ADC 寄存器设置
 *
 * \param[in] p_dev   :指向 AM_ZML166_ADC 设备结构体的指针
 * \param[in] addr    :寄存器地址，只有 SYS 和 ADC 寄存器可写
 * \param[in] data    :待写入的数据
 *
 * \retval  AM_OK     :写入成功
 * \retval -AM_EINVAL :无效参数
 * \retval -AM_EBUSY  :设备忙
 */
am_err_t am_zml166_adc_reg_set (am_zml166_adc_dev_t *p_dev,
                                uint8_t              addr,
                                uint32_t             data);

/**
 * \brief ZML166 ADC 寄存器读取
 *
 * \param[in]  p_dev  :指向 AM_ZML166_ADC 设备结构体的指针
 * \param[in]  addr   :待读取的寄存器地址
 * \param[out] p_data :指向接收缓冲区的指针，大小不能小于被读取的寄存器大小
 *
 * \retval  AM_OK     :读取成功
 * \retval -AM_EINVAL :无效参数
 * \retval -AM_EBUSY  :设备忙
 */
am_err_t am_zml166_adc_reg_get (am_zml166_adc_dev_t *p_dev,
                                uint8_t              addr,
                                void                *p_data);

/**
 * \brief ZML166 ADC 前置放大器增益设置
 *
 * \param[in] p_dev   :指向 AM_ZML166_ADC 设备结构体的指针
 * \param[in] gain    :增益，可设置为 1、2、4、8、16、32、64、128
 *
 * \retval  AM_OK     :设置成功
 * \retval -AM_EINVAL :无效参数
 * \retval -AM_EBUSY  :设备忙
 */
am_err_t am_zml166_adc_gain_set (am_zml166_adc_dev_t *p_dev,
                                 uint16_t             gain);

/**
 * \brief ZML166 ADC 前置放大器增益获取
 *
 * \param[in]  p_dev  :指向 AM_ZML166_ADC 设备结构体的指针
 * \param[out] p_gain :指向保存获取到的增益的指针
 *
 * \retval  AM_OK     :获取成功
 * \retval -AM_EINVAL :无效参数
 */
am_err_t am_zml166_adc_gain_get (am_zml166_adc_dev_t *p_dev,
                                 uint16_t            *p_gain);

/**
 * \brief ZML166 ADC 数据输出速率设置
 *
 * \param[in]  p_dev  :指向 AM_ZML166_ADC 设备结构体的指针
 * \param[out] speed  :设置速率值    参数可选 : AM_ZML166_ADC_DR_200
 *                                     AM_ZML166_ADC_DR_100
 *                                     AM_ZML166_ADC_DR_50
 *                                     AM_ZML166_ADC_DR_25
 *                                     AM_ZML166_ADC_DR_12_5
 *
 * \retval  AM_OK     :获取成功
 * \retval -AM_EINVAL :无效参数
 */
am_err_t am_zml166_adc_speed_set(am_zml166_adc_dev_t *p_dev,
                                 uint8_t              speed);

/**
 * \brief ZML166 ADC 数据输出速率设置
 *
 * \param[in]  p_dev    :指向 AM_ZML166_ADC 设备结构体的指针
 * \param[out] p_speed  :返回的速率设置值，具体意义见该头文件
 * \retval  AM_OK     :获取成功
 */
am_err_t am_zml166_adc_speed_get(am_zml166_adc_dev_t *p_dev,
                                 uint8_t             *p_speed);
/**
 * \brief zml166_adc  进入powerdown模式
 * \param[in]  p_dev  :指向 AM_ZML166_ADC 设备结构体的指针
 *
 * \retval  AM_OK     :进入成功
 */
am_err_t am_zml166_adc_power_down_entry(am_zml166_adc_dev_t *p_dev);

/**
 * \brief ZML166 ADC 退出powerdown模式
 * \param[in]  p_dev  :指向 AM_ZML166_ADC 设备结构体的指针
 * \retval  AM_OK     :退出成功
 */
am_err_t am_zml166_adc_power_down_exit(am_zml166_adc_dev_t *p_dev);

/**
 * \brief ZML166 ADC 使能buffer
 * \param[in]  p_dev  :指向 AM_ZML166_ADC 设备结构体的指针
 *
 * \retval  AM_OK     :使能成功
 */
am_err_t am_zml166_adc_buffer_enable(am_zml166_adc_dev_t *p_dev);

/**
 * \brief ZML166 ADC 禁能Buffer
 * \param[in]  p_dev  :指向 AM_ZML166_ADC 设备结构体的指针
 *
 * \retval  AM_OK     :禁能成功
 */
am_err_t am_zml166_adc_buffer_disable(am_zml166_adc_dev_t *p_dev);

/**
 * \brief ZML166 ADC MUX 通道获取
 *
 * \param[in]  p_dev  :指向 AM_ZML166_ADC 设备结构体的指针
 * \param[out] p_chan :指向保存获取到的 MUX 通道的指针
 * \retval  AM_OK     :获取成功
 * \retval -AM_EINVAL :无效参数
 */
am_err_t am_zml166_adc_mux_get (am_zml166_adc_dev_t *p_dev,
                                uint8_t             *p_chan);

/**
 * \brief AM_ZML166_ADC MUX 通道设置
 *
 * \param[in] p_dev :指向 AM_ZML166_ADC 设备结构体的指针
 * \param[in] chan  :MUX 通道， 0~2 位为 MUXP   可选参数为   AM_ZML166_ADC_INPS_1_2_VS
 *                                                AM_ZML166_ADC_INPS_AIN3
 *                                                AM_ZML166_ADC_INPS_AIN2
 *                                                AM_ZML166_ADC_INPS_AIN1
 *                                                AM_ZML166_ADC_INPS_AIN0
 *                          3~5 位为 MUXN   可选参数为    AM_ZML166_ADC_INNS_1_2_VS
 *                                                AM_ZML166_ADC_INNS_GND
 *                                                AM_ZML166_ADC_INNS_AIN4
 *                                                AM_ZML166_ADC_INNS_AIN3
 *                                                AM_ZML166_ADC_INNS_AIN2
 *                                                AM_ZML166_ADC_INNS_AIN1
 *
 *ex:  am_zml166_adc_mux_set(p_dev, AM_ZML166_ADC_INPS_AIN3 | AM_ZML166_ADC_INNS_AIN4);
 *
 * \retval  AM_OK     :设置成功
 * \retval -AM_EINVAL :无效参数
 * \retval -AM_EBUSY  :设备忙
 */
am_err_t am_zml166_adc_mux_set (am_zml166_adc_dev_t *p_dev,
                                uint8_t              chan);


/**
 * \brief ZML166 ADC  初始化寄存器函数
 *
 * \param[in] p_dev   :指向 AM_ZML166_ADC 设备结构体的指针
 *
 * \retval  AM_OK     :初始化成功
 * \retval  AM_ERROR  :初始化失败
 */
am_err_t am_zml166_adc_reg_init(am_zml166_adc_dev_t           *p_dev);

/**
 * \brief ZML166 ADC 初始化函数
 *
 * \param[in] p_dev     :指向 AM_ZML166_ADC 设备结构体的指针
 * \param[in] p_devinfo :指向 AM_ZML166_ADC 设备信息结构体的指针
 *
 * \return AM_ZML166_ADC 句柄，若为 NULL，表明初始化失败
 */
am_zml166_adc_handle_t am_zml166_adc_init (am_zml166_adc_dev_t           *p_dev,
                                           const am_zml166_adc_devinfo_t *p_devinfo);

/**
 * \brief ZML166 ADC 解初始化函数
 *
 * \param[in] handle :ZML166 ADC  句柄
 *
 * \return 无
 */
void am_zml166_adc_deinit (am_zml166_adc_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZML166_ADC_H */

/* end of file */
