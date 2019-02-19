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
#define AM_ZML166_ADC_SYS_ADDR       (0x00) /**< \brief 系统配置寄存器地址 */
#define AM_ZML166_ADC_ADC0_ADDR      (0x01) /**< \brief ADC 配置寄存器 0 地址 */
#define AM_ZML166_ADC_ADC1_ADDR      (0x02) /**< \brief ADC 配置寄存器 1 地址 */
#define AM_ZML166_ADC_ADC2_ADDR      (0x03) /**< \brief ADC 配置寄存器 2 地址 */
#define AM_ZML166_ADC_ADC3_ADDR      (0x04) /**< \brief ADC 配置寄存器 3 地址 */
#define AM_ZML166_ADC_ADC4_ADDR      (0x05) /**< \brief ADC 配置寄存器 4 地址 */
#define AM_ZML166_ADC_ADC5_ADDR      (0x06) /**< \brief ADC 配置寄存器 5 地址 */
#define AM_ZML166_ADC_ADO_ADDR       (0x09) /**< \brief ADC 转换数据寄存器地址 */
#define AM_ZML166_ADC_ADS_ADDR       (0x0A) /**< \brief ADC 转换数据读取标准寄存器地址 */
#define AM_ZML166_ADC_VREFT_ADDR     (0x11) /**< \brief ADC VREF VBE修调寄存器 */
#define AM_ZML166_ADC_LDOT_ADDR      (0x12) /**< \brief ADC LDOT修调寄存器 */
#define AM_ZML166_ADC_OSCT_ADDR      (0x13) /**< \brief ADC OSCT修调寄存器 */
#define AM_ZML166_ADC_TMUX_ADDR      (0x16) /**< \brief ADC TMUX输出配置寄存器 */
#define AM_ZML166_ADC_CLK_ADJ_ADDR   (0x55) /**< \brief 时钟频率修调寄存器地址 */
#define AM_ZML166_ADC_TEST_MODE_ADDR (0x5A) /**< \brief 芯片测试模式设置寄存器地址 */
#define AM_ZML166_ADC_RESET_ADDR     (0xEA) /**< \brief 系统复位寄存器地址 */
/**
 * @}
 */

/*******************************************************************************
  AM_ZML166_ADC SYS 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_TMODE_MASK    (3 << 6) /**< \brief 测量模式掩码 */
#define AM_ZML166_ADC_TMODE_POWER   (2 << 6) /**< \brief 电源电压测量模式 */
#define AM_ZML166_ADC_TMODE_MANUAL  (0 << 6) /**< \brief 手动测量模式（自由配置） */

#define AM_ZML166_ADC_PMODE_MASK    (3 << 4) /**< \brief 工作模式掩码 */
#define AM_ZML166_ADC_PMODE_MANUAL  (3 << 4) /**< \brief 自由模式(ADC 自由配置) */
#define AM_ZML166_ADC_PMODE_PWM     (2 << 4) /**< \brief 占空比模式，DR = 640Hz */
#define AM_ZML166_ADC_PMODE_PER     (1 << 4) /**< \brief 性能模式 */
#define AM_ZML166_ADC_PMODE_NORMAL  (0 << 4) /**< \brief 普通模式（当前不可用） */

#define AM_ZML166_ADC_ENREF_ENABLE  (1 << 3) /**< \brief VREF 使能 */
#define AM_ZML166_ADC_ENREF_DISABLE (0 << 3) /**< \brief VREF 关闭 */

#define AM_ZML166_ADC_ENADC_ENABLE  (1 << 2) /**< \brief ADC 使能 */
#define AM_ZML166_ADC_ENADC_DISABLE (0 << 2) /**< \brief ADC 关闭 */

#define AM_ZML166_ADC_ENLDO_ENABLE  (1 << 1) /**< \brief LDO 使能 */
#define AM_ZML166_ADC_ENLDO_DISABLE (0 << 1) /**< \brief LDO 关闭 */

/*******************************************************************************
  AM_ZML166_ADC ADC0 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_IMODE_PER    (1 << 7)    /**< \brief 调制器性能模式电流(普通模式电流x2) */
#define AM_ZML166_ADC_IMODE_NORMAL (0 << 7)    /**< \brief 调制器普通模式电流 */

#define AM_ZML166_ADC_FS_662_22    (1 << 6)    /**< \brief 采样频率为 662.22KHz */
#define AM_ZML166_ADC_FS_331_11    (0 << 6)    /**< \brief 采样频率为 331.11KHz */

#define AM_ZML166_ADC_INNS_MASK    (7 << 3)    /**< \brief PGA 负端输入信号掩码 */
#define AM_ZML166_ADC_INNS_TSN     (7 << 3)    /**< \brief PGA 负端输入信号：TS 负端 */
#define AM_ZML166_ADC_INNS_1_2_VS  (5 << 3)    /**< \brief PGA 负端输入信号：1/2 VS（共模电压） */
#define AM_ZML166_ADC_INNS_GND     (4 << 3)    /**< \brief PGA 负端输入信号：GND（仅在 TMODE=10 有效） */
#define AM_ZML166_ADC_INNS_AIN4    (3 << 3)    /**< \brief PGA 负端输入信号：AIN4 */
#define AM_ZML166_ADC_INNS_AIN3    (2 << 3)    /**< \brief PGA 负端输入信号：AIN3 */
#define AM_ZML166_ADC_INNS_AIN2    (1 << 3)    /**< \brief PGA 负端输入信号：AIN2 */
#define AM_ZML166_ADC_INNS_AIN1    (0 << 3)    /**< \brief PGA 负端输入信号：AIN1 */

#define AM_ZML166_ADC_INNS_AIN(ch) ((ch) << 3) /**< \brief PGA 负端输入信号：AIN4~AIN1 */
#define AM_ZML166_ADC_INPS_AIN(ch) ((ch) << 0) /**< \brief PGA 正端输入信号：AIN3~AIN0 */
#define AM_ZML166_ADC_INPS_MASK    (7 << 0)    /**< \brief PGA 正端输入信号掩码 */
#define AM_ZML166_ADC_INPS_TSP     (7 << 0)    /**< \brief PGA 正端输入信号：TS 正端 */
#define AM_ZML166_ADC_INPS_1_2_VS  (5 << 0)    /**< \brief PGA 正端输入信号：1/2 VS（共模电压） */
#define AM_ZML166_ADC_INPS_1_8_VDD (4 << 0)    /**< \brief PGA 正端输入信号：1/8 VDD（仅在 TMODE=10 有效） */
#define AM_ZML166_ADC_INPS_AIN3    (3 << 0)    /**< \brief PGA 正端输入信号：AIN3 */
#define AM_ZML166_ADC_INPS_AIN2    (2 << 0)    /**< \brief PGA 正端输入信号：AIN2 */
#define AM_ZML166_ADC_INPS_AIN1    (1 << 0)    /**< \brief PGA 正端输入信号：AIN1 */
#define AM_ZML166_ADC_INPS_AIN0    (0 << 0)    /**< \brief PGA 正端输入信号：AIN0 */
/*******************************************************************************
  AM_ZML166_ADC ADC1 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_DR_MASK       (7 << 5)   /**< \brief ADC 输出速率掩码 */
#define AM_ZML166_ADC_DR_1280       (7 << 5)   /**< \brief ADC 输出速率为 1280 Hz */
#define AM_ZML166_ADC_DR_640        (6 << 5)   /**< \brief ADC 输出速率为 640 Hz */
#define AM_ZML166_ADC_DR_320        (5 << 5)   /**< \brief ADC 输出速率为 320 Hz */
#define AM_ZML166_ADC_DR_160        (4 << 5)   /**< \brief ADC 输出速率为 160 Hz */
#define AM_ZML166_ADC_DR_80         (3 << 5)   /**< \brief ADC 输出速率为 80 Hz */
#define AM_ZML166_ADC_DR_40         (2 << 5)   /**< \brief ADC 输出速率为 40 Hz */
#define AM_ZML166_ADC_DR_20         (1 << 5)   /**< \brief ADC 输出速率为 20 Hz */
#define AM_ZML166_ADC_DR_10         (0 << 5)   /**< \brief ADC 输出速率为 10 Hz */

#define AM_ZML166_ADC_BUFBP_DISABLE (1 << 4)   /**< \brief Buffer 关闭（TBD，当前不可用） */
#define AM_ZML166_ADC_BUFBP_ENABLE  (0 << 4)   /**< \brief Buffer 开启 */

#define AM_ZML166_ADC_PGA_MASK      (3 << 2)   /**< \brief PGA 增益掩码 */
#define AM_ZML166_ADC_PGA_32        (3 << 2)   /**< \brief PGA 增益为 32 */
#define AM_ZML166_ADC_PGA_16        (2 << 2)   /**< \brief PGA 增益为 16 */
#define AM_ZML166_ADC_PGA_1         (0 << 2)   /**< \brief PGA 增益为 1 */

#define AM_ZML166_ADC_ADGN_MASK     (3 << 0)   /**< \brief 调制器增益掩码 */
#define AM_ZML166_ADC_ADGN_8        (3 << 0)   /**< \brief 调制器增益为 8，DR 下降为 1/4 */
#define AM_ZML166_ADC_ADGN_4        (2 << 0)   /**< \brief 调制器增益为 4，DR 下降为 1/2 */
#define AM_ZML166_ADC_ADGN_2        (1 << 0)   /**< \brief 调制器增益为 2 */
#define AM_ZML166_ADC_ADGN_1        (0 << 0)   /**< \brief 调制器增益为 1 */

/*******************************************************************************
  AM_ZML166_ADC ADC3 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_GTCSL_COARSE    (1 << 7) /**< \brief 增益温漂补偿粗调，为精调x6，用于补偿传感器温漂 */
#define AM_ZML166_ADC_GTCSL_FINE      (0 << 7) /**< \brief 精调，用于调整芯片自身温漂 */

#define AM_ZML166_ADC_GTC_MASK        (7 << 4) /**< \brief 增益温漂补偿掩码 */
#define AM_ZML166_ADC_GTC_P_15PPM     (7 << 4) /**< \brief 增益温漂补偿 15PPM/℃(CTCSL=0) */
#define AM_ZML166_ADC_GTC_P_10PPM     (6 << 4) /**< \brief 增益温漂补偿 10PPM/℃(CTCSL=0) */
#define AM_ZML166_ADC_GTC_P_5PPM      (5 << 4) /**< \brief 增益温漂补偿 5PPM/℃(CTCSL=0) */
#define AM_ZML166_ADC_GTC_0PPM        (0 << 4) /**< \brief 增益温漂补偿 0PPM/℃(CTCSL=0) */
#define AM_ZML166_ADC_GTC_N_15PPM     (3 << 4) /**< \brief 增益温漂补偿 -15PPM/℃(CTCSL=0) */
#define AM_ZML166_ADC_GTC_N_10PPM     (2 << 4) /**< \brief 增益温漂补偿 -10PPM/℃(CTCSL=0) */
#define AM_ZML166_ADC_GTC_N_5PPM      (1 << 4) /**< \brief 增益温漂补偿 -5PPM/℃(CTCSL=0) */

#define AM_ZML166_ADC_LVSCP_ENABLE    (1 << 3) /**< \brief 电平移位模块斩波使能，斩波频率 Fs/128 */
#define AM_ZML166_ADC_LVSCP_DISABLE   (0 << 3) /**< \brief 电平移位模块斩波不使能 */

#define AM_ZML166_ADC_LVSHIFT_ENABLE  (1 << 2) /**< \brief 电平移位使能 */
#define AM_ZML166_ADC_LVSHIFT_DISABLE (0 << 2) /**< \brief 电平移位不使能 */

/*******************************************************************************
  AM_ZML166_ADC ADC4 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_CHOPM_MASK       (3 << 6) /**< \brief 斩波频率掩码 */
#define AM_ZML166_ADC_CHOPM_64_128     (3 << 6) /**< \brief 仪放斩波频率为 fs_clk/64，调制器斩波频率为 fs_clk/128 */
#define AM_ZML166_ADC_CHOPM_32_128     (2 << 6) /**< \brief 仪放斩波频率为 fs_clk/32，调制器斩波频率为 fs_clk/128 */
#define AM_ZML166_ADC_CHOPM_32_256     (1 << 6) /**< \brief 仪放斩波频率为 fs_clk/32，调制器斩波频率为 fs_clk/256 */
#define AM_ZML166_ADC_CHOPM_DISABLE    (0 << 6) /**< \brief 不开斩波 fs_clk 为 MOD 采样频率 */

#define AM_ZML166_ADC_ACCU_MASK        (3 << 4) /**< \brief 占空比模式下 COMB 数据累加个数掩码 */
#define AM_ZML166_ADC_ACCU_NUM_64      (3 << 4) /**< \brief 占空比模式下 COMB 数据累加个数为 64 */
#define AM_ZML166_ADC_ACCU_NUM_32      (2 << 4) /**< \brief 占空比模式下 COMB 数据累加个数为 32 */
#define AM_ZML166_ADC_ACCU_NUM_18      (1 << 4) /**< \brief 占空比模式下 COMB 数据累加个数为 16 */
#define AM_ZML166_ADC_ACCU_NUM_8       (0 << 4) /**< \brief 占空比模式下 COMB 数据累加个数为 8 */

#define AM_ZML166_ADC_ADREFS_MASK      (3 << 2) /**< \brief ADC 参考电压掩码 */
#define AM_ZML166_ADC_ADREFS_VREF_GND  (3 << 2) /**< \brief 正参考=内部 VREF，负参考=GND */
#define AM_ZML166_ADC_ADREFS_REFP_REFN (0 << 2) /**< \brief 正参考=外部 REFP，负参考=外部 REFN */

#define AM_ZML166_ADC_LDOS_MASK        (3 << 0) /**< \brief 内部 LDO 输出 VS 电压掩码 */
#define AM_ZML166_ADC_LDOS_3_0         (3 << 0) /**< \brief 内部 LDO 输出 VS 电压为 3.0V */
#define AM_ZML166_ADC_LDOS_2_8         (2 << 0) /**< \brief 内部 LDO 输出 VS 电压为 2.8V */
#define AM_ZML166_ADC_LDOS_2_45        (1 << 0) /**< \brief 内部 LDO 输出 VS 电压为 2.45V */
#define AM_ZML166_ADC_LDOS_2_35        (0 << 0) /**< \brief 内部 LDO 输出 VS 电压为 2.35V */

/*******************************************************************************
  AM_ZML166_ADC ADC5 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_EN_OPA_ENABLE    (1 << 5) /**< \brief 使能 OPA_FIL，且 AIN2~AIN4 复用为 OPA_FIL 端口 */
#define AM_ZML166_ADC_EN_OPA_DISABLE   (0 << 5) /**< \brief 关闭 OPA_FIL，AIN2~AIN4 做模拟输入 */

#define AM_ZML166_ADC_EXFIL_EN_ENABLE  (1 << 4) /**< \brief PGA 输入信号使能 RC 滤波 */
#define AM_ZML166_ADC_EXFIL_EN_DISABLE (0 << 4) /**< \brief PGA 输入信号不使能 RC 滤波 */

#define AM_ZML166_ADC_FIL_EN_ENABLE    (1 << 2) /**< \brief Comb 之后的低通滤波器使能 */
#define AM_ZML166_ADC_FIL_EN_DISABLE   (0 << 2) /**< \brief Comb 之后的低通滤波器失能 */

#define AM_ZML166_ADC_FIL_CON1_DISABLE (1 << 1) /**< \brief 滤波器不使用级联结构 */
#define AM_ZML166_ADC_FIL_CON1_ENABLE  (0 << 1) /**< \brief 滤波器使用级联结构 */

#define AM_ZML166_ADC_FIL_CON2_2       (1 << 0) /**< \brief 使用滤波器系数 2 */
#define AM_ZML166_ADC_FIL_CON2_1       (0 << 0) /**< \brief 使用滤波器系数 1 */

/*******************************************************************************
  AM_ZML166_ADC ADS 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_ADS_ADO_BIT      (1 << 7) /**< \brief ADO 中数据已经被读取 */

#define AM_ZML166_ADC_ADS_RST_BIT      (1 << 6) /**< \brief 芯片上电复位完成 */

/*******************************************************************************
  AM_ZML166_ADC VREFT 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_VREFTRIM_MASK     0xF0    /**< \brief 内部参考 VREF 修调值掩码 */

#define AM_ZML166_ADC_BGTRIM_MASK       0x0F    /**< \brief 内部参考 VBE 修调值掩码 */

/*******************************************************************************
  AM_ZML166_ADC LDOT 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_LDOTRIM_MASK      0x0F    /**< \brief 内部参考 LDO 修调值掩码 */

/*******************************************************************************
  AM_ZML166_ADC OSCT 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_OSCTRIM_MASK      0xFF    /**< \brief 内部参考 OSC 修调值掩码 */

/*******************************************************************************
  AM_ZML166_ADC TMUX 寄存器
*******************************************************************************/

#define AM_ZML166_ADC_TMUX_ENABLE       0<<7    /**< \brief 使能TMUX输出 */
#define AM_ZML166_ADC_TMUX_DISABLE      1<<7    /**< \brief 禁能TMUX输出 */

#define AM_ZML166_ADC_TMUX_CLK_16       0x07    /**< \brief TMUX输出内部时钟 16 分频 */
#define AM_ZML166_ADC_TMUX_AGND         0x06    /**< \brief TMUX输出内部 AGND */
#define AM_ZML166_ADC_TMUX_NEG_VOL      0x05    /**< \brief TMUX输出电平移位负端电压 */
#define AM_ZML166_ADC_TMUX_PSO_VOL      0x04    /**< \brief TMUX输出电平移位正端电压 */
#define AM_ZML166_ADC_TMUX_IDAC1        0x03    /**< \brief TMUX输出内部 IDAC1 电流 */
#define AM_ZML166_ADC_TMUX_VS           0x02    /**< \brief TMUX输出内部 VS 电压 */
#define AM_ZML166_ADC_TMUX_VREF         0x01    /**< \brief TMUX输出VREF 电压 */
#define AM_ZML166_ADC_TMUX_VBE          0x00    /**< \brief TMUX输出VBE 电压*/
/*******************************************************************************
  AM_ZML166_ADC 时钟频率修调寄存器
*******************************************************************************/

#define AM_ZML166_ADC_CLK_ADJ_ENABLE    0xA5    /**< \brief 使能时钟频率修调 */
#define AM_ZML166_ADC_CLK_ADJ_DISABLE   0       /**< \brief 失能时钟频率修调 */

/*******************************************************************************
  AM_ZML166_ADC 系统测试模式设置寄存器
*******************************************************************************/

#define AM_ZML166_ADC_TEST_MODE_ENABLE  0x5A    /**< \brief 进入测试模式 */

/*******************************************************************************
  AM_ZML166_ADC 系统复位寄存器
******************************************************************************/
#define AM_ZML166_ADC_RESET_ENABLE      0x96    /**< \brief 使能ADC复位 */

/*******************************************************************************
  AM_ZML166_ADC 修调参数控制
******************************************************************************/
#define AM_ZML166_ADC_ADJUST_PARA_ENABLE        0          /**< \brief 修调参数写入控制标志 */
#define AM_ZML166_ADC_ADJUST_PARA_SAVE_ADDRESS  0x1FFFF810 /**< \brief 修调参数保存地址*/

/*******************************************************************************
  AM_ZML166_ADC 增益参数
******************************************************************************/
#define AM_ZML166_ADC_PGA_SET_1          0
#define AM_ZML166_ADC_PGA_SET_2          1
#define AM_ZML166_ADC_PGA_SET_4          2
#define AM_ZML166_ADC_PGA_SET_8          3
#define AM_ZML166_ADC_PGA_SET_16         4
#define AM_ZML166_ADC_PGA_SET_32         5
#define AM_ZML166_ADC_PGA_SET_64         6
#define AM_ZML166_ADC_PGA_SET_128        7

/** \brief ADC转换结果值 */
typedef uint32_t am_adc_val_t;
/**

 * \brief AM_ZML166_ADC 设备信息结构体
 */
typedef struct am_zml166_adc_devinfo {
    uint32_t  vref;             /**< \brief ADC 参考电压，单位：mV */
    uint32_t  timeout;          /**< \brief 超时时间，单位为毫秒 */
} am_zml166_adc_devinfo_t;

/**
 * \brief zml166_adc 设备结构体
 */
typedef struct am_zml166_adc_dev {

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
    uint8_t                        reg[AM_ZML166_ADC_CFG_NUM];

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
    const am_zml166_adc_devinfo_t      *p_devinfo;
    /**< \brief 校验ADS寄存器使能开关*/
    int       verify_flag;
} am_zml166_adc_dev_t;

/**
 * \brief AM_ZML166_ADC 句柄定义
 */
typedef am_zml166_adc_dev_t *am_zml166_adc_handle_t;

/**
 * \brief AM_ZML166_ADC 寄存器设置
 *
 * \param[in] p_dev 指向 AM_ZML166_ADC 设备结构体的指针
 * \param[in] addr  寄存器地址，只有 SYS 和 ADC 寄存器可写
 * \param[in] data  待写入的数据
 *
 * \retval  AM_OK     写入成功
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EBUSY  设备忙
 */
am_err_t am_zml166_adc_reg_set (am_zml166_adc_dev_t *p_dev,
                                uint8_t              addr,
                                uint32_t             data);

/**
 * \brief AM_ZML166_ADC 寄存器读取
 *
 * \param[in]  p_dev  指向 AM_ZML166_ADC 设备结构体的指针
 * \param[in]  addr   待读取的寄存器地址
 * \param[out] p_data 指向接收缓冲区的指针，大小不能小于被读取的寄存器大小
 *
 * \retval  AM_OK     读取成功
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EBUSY  设备忙
 */
am_err_t am_zml166_adc_reg_get (am_zml166_adc_dev_t *p_dev,
                                uint8_t              addr,
                                void                *p_data);

/**
 * \brief AM_ZML166_ADC 前置放大器增益设置
 *
 * \param[in] p_dev 指向 AM_ZML166_ADC 设备结构体的指针
 * \param[in] gain  增益，可设置为 1、2、4、8、16、32、64、128、256
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EBUSY  设备忙
 *
 * \note 当增益配置为 4 和 32 时，ADC 输出速率降低为 1 / 2，
 *       当增益配置为 8、64、128 和 256 时，ADC 输出速率降低为 1 / 4
 */
am_err_t am_zml166_adc_gain_set (am_zml166_adc_dev_t *p_dev,
                                 uint16_t             gain);

/**
 * \brief AM_ZML166_ADC 前置放大器增益获取
 *
 * \param[in]  p_dev  指向 AM_ZML166_ADC 设备结构体的指针
 * \param[out] p_gain 指向保存获取到的增益的指针
 *
 * \retval  AM_OK     获取成功
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_zml166_adc_gain_get (am_zml166_adc_dev_t *p_dev,
                                 uint8_t             *p_gain);

/**
 * \brief AM_ZML166_ADC MUX 通道获取
 *
 * \param[in]  p_dev  指向 AM_ZML166_ADC 设备结构体的指针
 * \param[out] p_chan 指向保存获取到的 MUX 通道的指针
 *
 * \retval  AM_OK     获取成功
 * \retval -AM_EINVAL 无效参数
 */
am_err_t am_zml166_adc_mux_get (am_zml166_adc_dev_t *p_dev,
                                uint8_t             *p_chan);

/**
 * \brief AM_ZML166_ADC MUX 通道设置
 *
 * \param[in] p_dev 指向 AM_ZML166_ADC 设备结构体的指针
 * \param[in] chan  MUX 通道，0~2 位为 MUXP，3~5 位为 MUXN
 *
 * \retval  AM_OK     设置成功
 * \retval -AM_EINVAL 无效参数
 * \retval -AM_EBUSY  设备忙
 */
am_err_t am_zml166_adc_mux_set (am_zml166_adc_dev_t *p_dev,
                                uint8_t              chan);

/**
 * \brief AM_ZML166_ADC 初始化寄存器函数
 *
 * \param[in] p_dev     指向 AM_ZML166_ADC 设备结构体的指针
 *
 * \retval  AM_OK     初始化成功
 * \retval  AM_ERROR  初始化失败
 */
am_err_t am_zml166_adc_reg_init(am_zml166_adc_dev_t           *p_dev);

/**
 * \brief AM_ZML166_ADC 初始化函数
 *
 * \param[in] p_dev     指向 AM_ZML166_ADC 设备结构体的指针
 * \param[in] p_devinfo 指向 AM_ZML166_ADC 设备信息结构体的指针
 *
 * \return AM_ZML166_ADC 句柄，若为 NULL，表明初始化失败
 */
am_zml166_adc_handle_t am_zml166_adc_init (am_zml166_adc_dev_t           *p_dev,
                                           const am_zml166_adc_devinfo_t *p_devinfo);

/**
 * \brief AM_ZML166_ADC 解初始化函数
 *
 * \param[in] handle AM_ZML166_ADC 句柄
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
