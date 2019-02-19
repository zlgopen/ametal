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
 * \brief ADC24 应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 18-05-14  pea, first implementation
 * \endinternal
 */

#ifndef __AM_ADC24_H
#define __AM_ADC24_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup am_if_ADC24
 * @copydoc am_ADC24.h
 * @{
 */

#include "ametal.h"
#include "am_adc.h"
#include "am_wait.h"

#define  ADC24_CFG_NUM 7 /**< \brief 系统配置寄存器数量 */
/** \brief 电压校准系数保存地址块  */
#define  FLASH_BLOCK_NUM  63
/** \brief PT100校准系数保存地址  */
#define PT100_PARA_SAVE_ADDRESS  (((FLASH_BLOCK_NUM * 1024)) + 4 * 16)
/*******************************************************************************
  ADC24 寄存器地址
*******************************************************************************/

/**
 * \name ADC24 寄存器地址
 * @{
 */
#define ADC24_SYS_ADDR       (0x00) /**< \brief 系统配置寄存器地址 */
#define ADC24_ADC0_ADDR      (0x01) /**< \brief ADC 配置寄存器 0 地址 */
#define ADC24_ADC1_ADDR      (0x02) /**< \brief ADC 配置寄存器 1 地址 */
#define ADC24_ADC2_ADDR      (0x03) /**< \brief ADC 配置寄存器 2 地址 */
#define ADC24_ADC3_ADDR      (0x04) /**< \brief ADC 配置寄存器 3 地址 */
#define ADC24_ADC4_ADDR      (0x05) /**< \brief ADC 配置寄存器 4 地址 */
#define ADC24_ADC5_ADDR      (0x06) /**< \brief ADC 配置寄存器 5 地址 */
#define ADC24_ADO_ADDR       (0x09) /**< \brief ADC 转换数据寄存器地址 */
#define ADC24_ADS_ADDR       (0x0A) /**< \brief ADC 转换数据读取标准寄存器地址 */
#define ADC24_VREFT_ADDR     (0x11) /**< \brief ADC VREF VBE修调寄存器 */
#define ADC24_LDOT_ADDR      (0x12) /**< \brief ADC LDOT修调寄存器 */
#define ADC24_OSCT_ADDR      (0x13) /**< \brief ADC OSCT修调寄存器 */
#define ADC24_TMUX_ADDR      (0x16) /**< \brief ADC TMUX输出配置寄存器 */
#define ADC24_CLK_ADJ_ADDR   (0x55) /**< \brief 时钟频率修调寄存器地址 */
#define ADC24_TEST_MODE_ADDR (0x5A) /**< \brief 芯片测试模式设置寄存器地址 */
#define ADC24_RESET_ADDR     (0xEA) /**< \brief 系统复位寄存器地址 */
/**
 * @}
 */

/*******************************************************************************
  ADC24 SYS 寄存器
*******************************************************************************/

#define ADC24_TMODE_MASK    (3 << 6) /**< \brief 测量模式掩码 */
#define ADC24_TMODE_POWER   (2 << 6) /**< \brief 电源电压测量模式 */
#define ADC24_TMODE_MANUAL  (0 << 6) /**< \brief 手动测量模式（自由配置） */

#define ADC24_PMODE_MASK    (3 << 4) /**< \brief 工作模式掩码 */
#define ADC24_PMODE_MANUAL  (3 << 4) /**< \brief 自由模式(ADC 自由配置) */
#define ADC24_PMODE_PWM     (2 << 4) /**< \brief 占空比模式，DR = 640Hz */
#define ADC24_PMODE_PER     (1 << 4) /**< \brief 性能模式 */
#define ADC24_PMODE_NORMAL  (0 << 4) /**< \brief 普通模式（当前不可用） */

#define ADC24_ENREF_ENABLE  (1 << 3) /**< \brief VREF 使能 */
#define ADC24_ENREF_DISABLE (0 << 3) /**< \brief VREF 关闭 */

#define ADC24_ENADC_ENABLE  (1 << 2) /**< \brief ADC 使能 */
#define ADC24_ENADC_DISABLE (0 << 2) /**< \brief ADC 关闭 */

#define ADC24_ENLDO_ENABLE  (1 << 1) /**< \brief LDO 使能 */
#define ADC24_ENLDO_DISABLE (0 << 1) /**< \brief LDO 关闭 */

/*******************************************************************************
  ADC24 ADC0 寄存器
*******************************************************************************/

#define ADC24_IMODE_PER    (1 << 7)    /**< \brief 调制器性能模式电流(普通模式电流x2) */
#define ADC24_IMODE_NORMAL (0 << 7)    /**< \brief 调制器普通模式电流 */

#define ADC24_FS_662_22    (1 << 6)    /**< \brief 采样频率为 662.22KHz */
#define ADC24_FS_331_11    (0 << 6)    /**< \brief 采样频率为 331.11KHz */

#define ADC24_INNS_MASK    (7 << 3)    /**< \brief PGA 负端输入信号掩码 */
#define ADC24_INNS_TSN     (7 << 3)    /**< \brief PGA 负端输入信号：TS 负端 */
#define ADC24_INNS_1_2_VS  (5 << 3)    /**< \brief PGA 负端输入信号：1/2 VS（共模电压） */
#define ADC24_INNS_GND     (4 << 3)    /**< \brief PGA 负端输入信号：GND（仅在 TMODE=10 有效） */
#define ADC24_INNS_AIN4    (3 << 3)    /**< \brief PGA 负端输入信号：AIN4 */
#define ADC24_INNS_AIN3    (2 << 3)    /**< \brief PGA 负端输入信号：AIN3 */
#define ADC24_INNS_AIN2    (1 << 3)    /**< \brief PGA 负端输入信号：AIN2 */
#define ADC24_INNS_AIN1    (0 << 3)    /**< \brief PGA 负端输入信号：AIN1 */

#define ADC24_INNS_AIN(ch) ((ch) << 3) /**< \brief PGA 负端输入信号：AIN4~AIN1 */
#define ADC24_INPS_AIN(ch) ((ch) << 0) /**< \brief PGA 正端输入信号：AIN3~AIN0 */
#define ADC24_INPS_MASK    (7 << 0)    /**< \brief PGA 正端输入信号掩码 */
#define ADC24_INPS_TSP     (7 << 0)    /**< \brief PGA 正端输入信号：TS 正端 */
#define ADC24_INPS_1_2_VS  (5 << 0)    /**< \brief PGA 正端输入信号：1/2 VS（共模电压） */
#define ADC24_INPS_1_8_VDD (4 << 0)    /**< \brief PGA 正端输入信号：1/8 VDD（仅在 TMODE=10 有效） */
#define ADC24_INPS_AIN3    (3 << 0)    /**< \brief PGA 正端输入信号：AIN3 */
#define ADC24_INPS_AIN2    (2 << 0)    /**< \brief PGA 正端输入信号：AIN2 */
#define ADC24_INPS_AIN1    (1 << 0)    /**< \brief PGA 正端输入信号：AIN1 */
#define ADC24_INPS_AIN0    (0 << 0)    /**< \brief PGA 正端输入信号：AIN0 */
/*******************************************************************************
  ADC24 ADC1 寄存器
*******************************************************************************/

#define ADC24_DR_MASK       (7 << 5) /**< \brief ADC 输出速率掩码 */
#define ADC24_DR_1280       (7 << 5) /**< \brief ADC 输出速率为 1280 Hz */
#define ADC24_DR_640        (6 << 5) /**< \brief ADC 输出速率为 640 Hz */
#define ADC24_DR_320        (5 << 5) /**< \brief ADC 输出速率为 320 Hz */
#define ADC24_DR_160        (4 << 5) /**< \brief ADC 输出速率为 160 Hz */
#define ADC24_DR_80         (3 << 5) /**< \brief ADC 输出速率为 80 Hz */
#define ADC24_DR_40         (2 << 5) /**< \brief ADC 输出速率为 40 Hz */
#define ADC24_DR_20         (1 << 5) /**< \brief ADC 输出速率为 20 Hz */
#define ADC24_DR_10         (0 << 5) /**< \brief ADC 输出速率为 10 Hz */

#define ADC24_BUFBP_DISABLE (1 << 4) /**< \brief Buffer 关闭（TBD，当前不可用） */
#define ADC24_BUFBP_ENABLE  (0 << 4) /**< \brief Buffer 开启 */

#define ADC24_PGA_MASK      (3 << 2) /**< \brief PGA 增益掩码 */
#define ADC24_PGA_32        (3 << 2) /**< \brief PGA 增益为 32 */
#define ADC24_PGA_16        (2 << 2) /**< \brief PGA 增益为 16 */
#define ADC24_PGA_1         (0 << 2) /**< \brief PGA 增益为 1 */

#define ADC24_ADGN_MASK     (3 << 0) /**< \brief 调制器增益掩码 */
#define ADC24_ADGN_8        (3 << 0) /**< \brief 调制器增益为 8，DR 下降为 1/4 */
#define ADC24_ADGN_4        (2 << 0) /**< \brief 调制器增益为 4，DR 下降为 1/2 */
#define ADC24_ADGN_2        (1 << 0) /**< \brief 调制器增益为 2 */
#define ADC24_ADGN_1        (0 << 0) /**< \brief 调制器增益为 1 */

/*******************************************************************************
  ADC24 ADC3 寄存器
*******************************************************************************/

#define ADC24_GTCSL_COARSE    (1 << 7) /**< \brief 增益温漂补偿粗调，为精调x6，用于补偿传感器温漂 */
#define ADC24_GTCSL_FINE      (0 << 7) /**< \brief 精调，用于调整芯片自身温漂 */

#define ADC24_GTC_MASK        (7 << 4) /**< \brief 增益温漂补偿掩码 */
#define ADC24_GTC_P_15PPM     (7 << 4) /**< \brief 增益温漂补偿 15PPM/℃(CTCSL=0) */
#define ADC24_GTC_P_10PPM     (6 << 4) /**< \brief 增益温漂补偿 10PPM/℃(CTCSL=0) */
#define ADC24_GTC_P_5PPM      (5 << 4) /**< \brief 增益温漂补偿 5PPM/℃(CTCSL=0) */
#define ADC24_GTC_0PPM        (0 << 4) /**< \brief 增益温漂补偿 0PPM/℃(CTCSL=0) */
#define ADC24_GTC_N_15PPM     (3 << 4) /**< \brief 增益温漂补偿 -15PPM/℃(CTCSL=0) */
#define ADC24_GTC_N_10PPM     (2 << 4) /**< \brief 增益温漂补偿 -10PPM/℃(CTCSL=0) */
#define ADC24_GTC_N_5PPM      (1 << 4) /**< \brief 增益温漂补偿 -5PPM/℃(CTCSL=0) */

#define ADC24_LVSCP_ENABLE    (1 << 3) /**< \brief 电平移位模块斩波使能，斩波频率 Fs/128 */
#define ADC24_LVSCP_DISABLE   (0 << 3) /**< \brief 电平移位模块斩波不使能 */

#define ADC24_LVSHIFT_ENABLE  (1 << 2) /**< \brief 电平移位使能 */
#define ADC24_LVSHIFT_DISABLE (0 << 2) /**< \brief 电平移位不使能 */

/*******************************************************************************
  ADC24 ADC4 寄存器
*******************************************************************************/

#define ADC24_CHOPM_MASK       (3 << 6) /**< \brief 斩波频率掩码 */
#define ADC24_CHOPM_64_128     (3 << 6) /**< \brief 仪放斩波频率为 fs_clk/64，调制器斩波频率为 fs_clk/128 */
#define ADC24_CHOPM_32_128     (2 << 6) /**< \brief 仪放斩波频率为 fs_clk/32，调制器斩波频率为 fs_clk/128 */
#define ADC24_CHOPM_32_256     (1 << 6) /**< \brief 仪放斩波频率为 fs_clk/32，调制器斩波频率为 fs_clk/256 */
#define ADC24_CHOPM_DISABLE    (0 << 6) /**< \brief 不开斩波 fs_clk 为 MOD 采样频率 */

#define ADC24_ACCU_MASK        (3 << 4) /**< \brief 占空比模式下 COMB 数据累加个数掩码 */
#define ADC24_ACCU_NUM_64      (3 << 4) /**< \brief 占空比模式下 COMB 数据累加个数为 64 */
#define ADC24_ACCU_NUM_32      (2 << 4) /**< \brief 占空比模式下 COMB 数据累加个数为 32 */
#define ADC24_ACCU_NUM_18      (1 << 4) /**< \brief 占空比模式下 COMB 数据累加个数为 16 */
#define ADC24_ACCU_NUM_8       (0 << 4) /**< \brief 占空比模式下 COMB 数据累加个数为 8 */

#define ADC24_ADREFS_MASK      (3 << 2) /**< \brief ADC 参考电压掩码 */
#define ADC24_ADREFS_VREF_GND  (3 << 2) /**< \brief 正参考=内部 VREF，负参考=GND */
#define ADC24_ADREFS_REFP_REFN (0 << 2) /**< \brief 正参考=外部 REFP，负参考=外部 REFN */

#define ADC24_LDOS_MASK        (3 << 0) /**< \brief 内部 LDO 输出 VS 电压掩码 */
#define ADC24_LDOS_3_0         (3 << 0) /**< \brief 内部 LDO 输出 VS 电压为 3.0V */
#define ADC24_LDOS_2_8         (2 << 0) /**< \brief 内部 LDO 输出 VS 电压为 2.8V */
#define ADC24_LDOS_2_45        (1 << 0) /**< \brief 内部 LDO 输出 VS 电压为 2.45V */
#define ADC24_LDOS_2_35        (0 << 0) /**< \brief 内部 LDO 输出 VS 电压为 2.35V */

/*******************************************************************************
  ADC24 ADC5 寄存器
*******************************************************************************/

#define ADC24_EN_OPA_ENABLE    (1 << 5) /**< \brief 使能 OPA_FIL，且 AIN2~AIN4 复用为 OPA_FIL 端口 */
#define ADC24_EN_OPA_DISABLE   (0 << 5) /**< \brief 关闭 OPA_FIL，AIN2~AIN4 做模拟输入 */

#define ADC24_EXFIL_EN_ENABLE  (1 << 4) /**< \brief PGA 输入信号使能 RC 滤波 */
#define ADC24_EXFIL_EN_DISABLE (0 << 4) /**< \brief PGA 输入信号不使能 RC 滤波 */

#define ADC24_FIL_EN_ENABLE    (1 << 2) /**< \brief Comb 之后的低通滤波器使能 */
#define ADC24_FIL_EN_DISABLE   (0 << 2) /**< \brief Comb 之后的低通滤波器失能 */

#define ADC24_FIL_CON1_DISABLE (1 << 1) /**< \brief 滤波器不使用级联结构 */
#define ADC24_FIL_CON1_ENABLE  (0 << 1) /**< \brief 滤波器使用级联结构 */

#define ADC24_FIL_CON2_2       (1 << 0) /**< \brief 使用滤波器系数 2 */
#define ADC24_FIL_CON2_1       (0 << 0) /**< \brief 使用滤波器系数 1 */

/*******************************************************************************
  ADC24 ADS 寄存器
*******************************************************************************/

#define ADC24_ADS_ADO_BIT (1 << 7)     /**< \brief ADO 中数据已经被读取 */

#define ADC24_ADS_RST_BIT (1 << 6)     /**< \brief 芯片上电复位完成 */

/*******************************************************************************
  ADC24 VREFT 寄存器
*******************************************************************************/

#define ADC24_VREFTRIM_MASK  0xF0      /**< \brief 内部参考 VREF 修调值掩码 */

#define ADC24_BGTRIM_MASK    0x0F      /**< \brief 内部参考 VBE 修调值掩码 */

/*******************************************************************************
  ADC24 LDOT 寄存器
*******************************************************************************/

#define ADC24_LDOTRIM_MASK    0x0F     /**< \brief 内部参考 LDO 修调值掩码 */

/*******************************************************************************
  ADC24 OSCT 寄存器
*******************************************************************************/

#define ADC24_OSCTRIM_MASK    0xFF     /**< \brief 内部参考 OSC 修调值掩码 */

/*******************************************************************************
  ADC24 TMUX 寄存器
*******************************************************************************/

#define ADC24_TMUX_ENABLE       0<<7  /**< \brief 使能TMUX输出 */
#define ADC24_TMUX_DISABLE      1<<7  /**< \brief 禁能TMUX输出 */

#define ADC24_TMUX_CLK_16       0x07  /**< \brief TMUX输出内部时钟 16 分频 */
#define ADC24_TMUX_AGND         0x06  /**< \brief TMUX输出内部 AGND */
#define ADC24_TMUX_NEG_VOL      0x05  /**< \brief TMUX输出电平移位负端电压 */
#define ADC24_TMUX_PSO_VOL      0x04  /**< \brief TMUX输出电平移位正端电压 */
#define ADC24_TMUX_IDAC1        0x03  /**< \brief TMUX输出内部 IDAC1 电流 */
#define ADC24_TMUX_VS           0x02  /**< \brief TMUX输出内部 VS 电压 */
#define ADC24_TMUX_VREF         0x01  /**< \brief TMUX输出VREF 电压 */
#define ADC24_TMUX_VBE          0x00  /**< \brief TMUX输出VBE 电压*/
/*******************************************************************************
  ADC24 时钟频率修调寄存器
*******************************************************************************/

#define ADC24_CLK_ADJ_ENABLE   0xA5  /**< \brief 使能时钟频率修调 */
#define ADC24_CLK_ADJ_DISABLE  0     /**< \brief 失能时钟频率修调 */

/*******************************************************************************
  ADC24 系统测试模式设置寄存器
*******************************************************************************/

#define ADC24_TEST_MODE_ENABLE 0x5A  /**< \brief 进入测试模式 */

/*******************************************************************************
  ADC24 系统复位寄存器
******************************************************************************/
#define ADC24_RESET_ENABLE     0x96  /**< \brief 使能ADC复位 */


/*******************************************************************************
  ADC24 修调参数控制
******************************************************************************/
#define ADJUST_PARA_ENABLE         0            /**< \brief 修调参数写入控制标志 */
#define ADJUST_PARA_SAVE_ADDRESS   0x1FFFF810   /**< \brief 修调参数保存地址*/

/*******************************************************************************
  ADC24 增益参数
******************************************************************************/
#define AM_ADC24_PGA_1             0
#define AM_ADC24_PGA_2             1
#define AM_ADC24_PGA_4             2
#define AM_ADC24_PGA_8             3
#define AM_ADC24_PGA_16            4
#define AM_ADC24_PGA_32            5
#define AM_ADC24_PGA_64            6
#define AM_ADC24_PGA_128           7

/** \brief ADC转换结果值 */
typedef uint32_t am_adc_val_t;

/**
 * \brief ADC24 设备信息结构体
 */
typedef struct am_adc24_devinfo {
    uint32_t  vref;             /**< \brief ADC 参考电压，单位：mV */
    uint32_t  timeout;          /**< \brief 超时时间，单位为毫秒 */
} am_adc24_devinfo_t;

/**
 * \brief adc24 设备结构体
 */
typedef struct am_adc24_dev {

    /** \brief ADC 标准服务 */
    am_adc_serv_t                  adc_serve;
    
    /** \brief ADC 配置 */
    uint8_t                        dr;

    /** \brief 中断是否使能 */
    am_bool_t                      irq_en;
    
    /** \brief 当前 ADC 时钟频率 */
    uint32_t                       clk_freq;

    /** \brief 数据转换完成等待 */
    am_wait_t                      ready_wait;

    /** \brief 寄存器缓冲区 */
    uint8_t                        reg[ADC24_CFG_NUM];

    /** \brief ADC 状态 0: 空闲 1:正在转换序列中 */
    uint8_t                        adc_state;

    /** \brief 通道 */
    uint8_t                        chan;

    /** \brief 当前转换的序列描述符数组首地址 */
    am_adc_buf_desc_t             *p_desc;

    /** \brief 当前转换的序列描述符数量 */
    uint32_t                       desc_num;

    /** \brief 转换的次数 */
    uint32_t                       count;

    /** \brief 转换标志 */
    uint32_t                       flags;

    /** \brief 存放用户启动转换后的回调函数 */
    am_adc_seq_cb_t                pfn_callback;

    /** \brief 用户启动回调函数的回调函数参数 */
    void                          *p_arg;

    /** \brief 对当前序列描述符已经采样的次数 */
    uint32_t                       conv_cnt;

    /** \brief 正在执行当前序列描述符的索引 */
    uint32_t                       desc_index;

    /** \brief 对整个序列转换完成的次数 */
    uint32_t                       seq_cnt;

    /** \brief 设备信息 */
    const am_adc24_devinfo_t      *p_devinfo;
    /**< \brief 校验ADS寄存器使能开关*/
    int       verify_flag;
} am_adc24_dev_t;

/**
 * \brief ADC24 句柄定义
 */
typedef am_adc24_dev_t *am_adc24_handle_t;

/**
 * \brief ADC24 寄存器设置
 *
 * \param[in] p_dev 指向 ADC24 设备结构体的指针
 * \param[in] addr  寄存器地址，只有 SYS 和 ADC 寄存器可写
 * \param[in] data  待写入的数据
 *
 * \retval  AM_OK     写入成功
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EBUSY  设备忙
 */
am_err_t am_adc24_reg_set (am_adc24_dev_t *p_dev,
                           uint8_t         addr,
                           uint32_t        data);

/**
 * \brief ADC24 寄存器读取
 *
 * \param[in]  p_dev  指向 ADC24 设备结构体的指针
 * \param[in]  addr   待读取的寄存器地址
 * \param[out] p_data 指向接收缓冲区的指针，大小不能小于被读取的寄存器大小
 *
 * \retval  AM_OK     读取成功
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EBUSY  设备忙
 */
am_err_t am_adc24_reg_get (am_adc24_dev_t *p_dev,
                           uint8_t         addr,
                           void           *p_data);

/**
 * \brief ADC24 前置放大器增益设置
 *
 * \param[in] p_dev 指向 ADC24 设备结构体的指针
 * \param[in] gain  增益，可设置为 1、2、4、8、16、32、64、128、256
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EBUSY  设备忙
 *
 * \note 当增益配置为 4 和 32 时，ADC 输出速率降低为 1 / 2，
 *       当增益配置为 8、64、128 和 256 时，ADC 输出速率降低为 1 / 4
 */
am_err_t am_adc24_gain_set (am_adc24_dev_t *p_dev,
                            uint16_t        gain);

/**
 * \brief ADC24 前置放大器增益获取
 *
 * \param[in]  p_dev  指向 ADC24 设备结构体的指针
 * \param[out] p_gain 指向保存获取到的增益的指针
 *
 * \retval  AM_OK     获取成功
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_adc24_gain_get (am_adc24_dev_t *p_dev,
                            uint8_t        *p_gain);

/**
 * \brief ADC24 MUX 通道获取
 *
 * \param[in]  p_dev  指向 ADC24 设备结构体的指针
 * \param[out] p_chan 指向保存获取到的 MUX 通道的指针
 *
 * \retval  AM_OK     获取成功
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_adc24_mux_get (am_adc24_dev_t *p_dev,
                           uint8_t        *p_chan);

/**
 * \brief ADC24 MUX 通道设置
 *
 * \param[in] p_dev 指向 ADC24 设备结构体的指针
 * \param[in] chan  MUX 通道，0~2 位为 MUXP，3~5 位为 MUXN
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EBUSY  设备忙
 */
am_err_t am_adc24_mux_set (am_adc24_dev_t *p_dev,
                           uint8_t         chan);

/**
 * \brief ADC24 初始化寄存器函数
 *
 * \param[in] p_dev     指向 ADC24 设备结构体的指针
 *
 * \retval  AM_OK     初始化成功
 * \retval  AM_ERROR  初始化失败
 */
am_err_t am_adc24_reg_init(am_adc24_dev_t           *p_dev);

/**
 * \brief ADC24 初始化函数
 *
 * \param[in] p_dev     指向 ADC24 设备结构体的指针
 * \param[in] p_devinfo 指向 ADC24 设备信息结构体的指针
 *
 * \return ADC24 句柄，若为 NULL，表明初始化失败
 */
am_adc24_handle_t am_adc24_init (am_adc24_dev_t           *p_dev,
                                 const am_adc24_devinfo_t *p_devinfo);

/**
 * \brief ADC24 解初始化函数
 *
 * \param[in] handle ADC24 句柄
 *
 * \return 无
 */
void am_adc24_deinit (am_adc24_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ADC24_H */

/* end of file */
