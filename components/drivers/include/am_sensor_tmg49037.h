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
 * \brief 环境光/颜色、接近/手势传感器 TMG49037 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_TMG49037_H
#define __AM_SENSOR_TMG49037_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器 TMG49037 使能设置
 */
#define AM_TMG49037_GEN                             (1u << 6)
#define AM_TMG49037_WEN                             (1u << 3)
#define AM_TMG49037_PEN                             (1u << 2)
#define AM_TMG49037_AEN                             (1u << 1)
#define AM_TMG49037_PON                             (1u << 0)

/**
 * \brief 传感器 TMG49037 ALS ADC的积分时间
    积分时间(单位:ms) = 2.78ms * (256 - ATIME)
 */
#define AM_TMG49037_ATIME_2_78MS                    (0xFF)
#define AM_TMG49037_ATIME_27_8MS                    (0xF6)
#define AM_TMG49037_ATIME_100MS                     (0xDC)
#define AM_TMG49037_ATIME_178MS                     (0xC0)
#define AM_TMG49037_ATIME_711MS                     (0x00)

/**
 * \brief 传感器 TMG49037 Prox采样时间
    Prox采样时间(单位:ms) = 2.78ms * (PTIME + 1)
 */
#define AM_TMG49037_PTIME_2_78MS                    (0x00)
#define AM_TMG49037_PTIME_5_56MS                    (0x01)
#define AM_TMG49037_PTIME_11_1MS                    (0x03)
#define AM_TMG49037_PTIME_100MS                     (0x23)
#define AM_TMG49037_PTIME_711MS                     (0xFF)

/**
 * \brief 传感器 TMG49037 ALS等待时间
    等待时间(单位:ms) ---- (WLONG = 0) : 2.78ms * (256 - WTIME)
                      ---- (WLONG = 1) : 33.3ms * (256 - WTIME)
 */
#define AM_TMG49037_WTIME_2_78MS_OR_0_03S           (0xFF)
#define AM_TMG49037_WTIME_100MS_OR_1_2S             (0xDC)
#define AM_TMG49037_WTIME_417MS_OR_5S               (0x6A)
#define AM_TMG49037_WTIME_711MS_OR_8_53S            (0x00)

/**
 * \brief 传感器 TMG49037 低功耗空闲
 */
#define AM_TMG49037_LOWPOWER_IDLE_DISABLE           (0x00)
#define AM_TMG49037_LOWPOWER_IDLE_ENABLE            (0x01)

/**
 * \brief 传感器 TMG49037 Prox脉冲长度
 */
#define AM_TMG49037_PULSE_LEN_4US                   (0x00)
#define AM_TMG49037_PULSE_LEN_8US                   (0x01)
#define AM_TMG49037_PULSE_LEN_16US                  (0x02)
#define AM_TMG49037_PULSE_LEN_32US                  (0x03)

/**
 * \brief 传感器 TMG49037 Prox IR传感器增益
 */
#define AM_TMG49037_PGGAIN_1                        (0x00)
#define AM_TMG49037_PGGAIN_2                        (0x01)
#define AM_TMG49037_PGGAIN_4                        (0x02)
#define AM_TMG49037_PGGAIN_8                        (0x03)

/**
 * \brief 传感器 TMG49037 Prox IR VCSEL电流驱动能力设置
    驱动电流(单位:mA) = 10mA + (20mA * PGLDRIVE)
 */
#define AM_TMG49037_PGLDRIVE_10MA                   (0x00)
#define AM_TMG49037_PGLDRIVE_30MA                   (0x01)
#define AM_TMG49037_PGLDRIVE_50MA                   (0x02)
#define AM_TMG49037_PGLDRIVE_290MA                  (0x0E)
#define AM_TMG49037_PGLDRIVE_310MA                  (0x0F)

/**
 * \brief 传感器 TMG49037 ALS/Color传感器增益
 */
#define AM_TMG49037_AGAIN_1                         (0x00)
#define AM_TMG49037_AGAIN_4                         (0x01)
#define AM_TMG49037_AGAIN_16                        (0x02)
#define AM_TMG49037_AGAIN_64                        (0x03)

/**
 * \brief 传感器 TMG49037 STATUS位
 */
#define AM_TMG49037_STATUS_ASAT                     (1u << 7)
#define AM_TMG49037_STATUS_PGSAT                    (1u << 6)
#define AM_TMG49037_STATUS_PINT                     (1u << 5)
#define AM_TMG49037_STATUS_AINT                     (1u << 4)
#define AM_TMG49037_STATUS_GINT                     (1u << 2)
#define AM_TMG49037_STATUS_CINT                     (1u << 1)

/**
 * \brief 传感器 TMG49037 STATUS2位
 */
#define AM_TMG49037_STATUS2_PVALID                  (1u << 7)
#define AM_TMG49037_STATUS2_AVALID                  (1u << 6)
#define AM_TMG49037_STATUS2_ASAT_DIGITAL            (1u << 4)
#define AM_TMG49037_STATUS2_ASAT_ANALOG             (1u << 3)
#define AM_TMG49037_STATUS2_PGSAT_ADC               (1u << 2)
#define AM_TMG49037_STATUS2_PGSAT_REFLECTIVE        (1u << 1)
#define AM_TMG49037_STATUS2_PGSAT_AMBIENT           (1u << 0)

/**
 * \brief 传感器 TMG49037 方向光电二极管
    写 1 禁用x方向光电二极管
 */
#define AM_TMG49037_PMASK_E                         (1u << 7)
#define AM_TMG49037_PMASK_W                         (1u << 6)
#define AM_TMG49037_PMASK_S                         (1u << 5)
#define AM_TMG49037_PMASK_N                         (1u << 4)
#define AM_TMG49037_AMASK                           (1u << 3)   /* ALS Mask */

/**
 * \brief 传感器 TMG49037 CFG4位
 */
#define AM_TMG49037_ALS_INT_DIRECT                  (1u << 7)
#define AM_TMG49037_ALS_INT_DIRECT_GPIO             (1u << 6)
#define AM_TMG49037_PROX_INT_DIRECT                 (1u << 5)
#define AM_TMG49037_PROX_INT_DIRECT_GPIO            (1u << 4)

/**
 * \brief 传感器 TMG49037 CFG5位
 */
#define AM_TMG49037_LONG_LTFSTOP_DISCARD_ALS        (1u << 5)
#define AM_TMG49037_DISABLE_IR_CORRECTION           (1u << 3)
#define AM_TMG49037_PROX_FILTER_DOWNSAMPLE          (1u << 2)
#define AM_TMG49037_PROX_FILTER_SIZE                (1u << 1)
#define AM_TMG49037_PROX_FILTER                     (1u << 0)

/**
 * \brief 传感器 TMG49037 手势退出掩码
 */
#define AM_TMG49037_GTHR_MASK_E                     (1u << 5)
#define AM_TMG49037_GTHR_MASK_W                     (1u << 4)
#define AM_TMG49037_GTHR_MASK_S                     (1u << 3)
#define AM_TMG49037_GTHR_MASK_N                     (1u << 2)

/**
 * \brief 传感器 TMG49037 手势退出连续故障数
 */
#define AM_TMG49037_GTHR_OUT_NR_1                   (0x00)
#define AM_TMG49037_GTHR_OUT_NR_2                   (0x01)
#define AM_TMG49037_GTHR_OUT_NR_4                   (0x02)
#define AM_TMG49037_GTHR_OUT_NR_8                   (0x03)

/**
 * \brief 传感器 TMG49037 定义手势FIFO级别，NSWE数据集的数量
 */
#define AM_TMG49037_GFIFOTHR_1                      (0x00)
#define AM_TMG49037_GFIFOTHR_4                      (0x01)
#define AM_TMG49037_GFIFOTHR_8                      (0x02)
#define AM_TMG49037_GFIFOTHR_16                     (0x03)

/**
 * \brief 传感器 TMG49037 手势过滤器采样数
 */
#define AM_TMG49037_GEST_FILTER_SIZE_2              (0x00)
#define AM_TMG49037_GEST_FILTER_SIZE_4              (0x01)

/**
 * \brief 传感器 TMG49037 手势过滤器 使能/禁能
 */
#define AM_TMG49037_GEST_FILTER_DISABLE             (0x00)
#define AM_TMG49037_GEST_FILTER_ENABLE              (0x01)


/**
 * \brief 传感器 TMG49037 手势采样时间
    采样时间(单位:us) = 88us * (GTIME + 1)
 */
#define AM_TMG49037_GTIME_88US                      (0x00)
#define AM_TMG49037_GTIME_176US                     (0x01)
#define AM_TMG49037_GTIME_968US                     (0x0A)
#define AM_TMG49037_GTIME_9_9MS                     (0x70)
#define AM_TMG49037_GTIME_22_5MS                    (0xFF)

/**
 * \brief 传感器 TMG49037 进入/退出 手势模式
 */
#define AM_TMG49037_EXIT_GESTURE_MODE               (0x00)
#define AM_TMG49037_ENTER_GESTURE_MODE              (0x01)


/**
 * \brief 传感器 TMG49037 Prox校准类型选择
 */
#define AM_TMG49037_ELECTRICAL_OPTICAL_CROSSTALK    (0x00)
#define AM_TMG49037_ELECTRICAL_CROSSTALK_ONLY       (0x01)

/**
 * \brief 传感器 TMG49037 校准是/否跳过二进制搜索步骤
 */
#define AM_TMG49037_BINSRCH_SKIP_DISABLE            (0x00)
#define AM_TMG49037_BINSRCH_SKIP_ENABLE             (0x01)

/**
 * \brief 传感器 TMG49037 DCAVG_ITERATIONS
 */
#define AM_TMG49037_DCAVG_ITERATIONS_SKIP           (0x00)
#define AM_TMG49037_DCAVG_ITERATIONS_2              (0x01)
#define AM_TMG49037_DCAVG_ITERATIONS_4              (0x02)
#define AM_TMG49037_DCAVG_ITERATIONS_8              (0x03)
#define AM_TMG49037_DCAVG_ITERATIONS_16             (0x04)
#define AM_TMG49037_DCAVG_ITERATIONS_32             (0x05)
#define AM_TMG49037_DCAVG_ITERATIONS_64             (0x06)
#define AM_TMG49037_DCAVG_ITERATIONS_128            (0x07)

/**
 * \brief 传感器 TMG49037 PXDCAVG_BASELINE_WINDOW
 */
#define AM_TMG49037_PXDCAVG_BASELINE_WINDOW_SKIP    (0x00)
#define AM_TMG49037_PXDCAVG_BASELINE_WINDOW_2       (0x01)
#define AM_TMG49037_PXDCAVG_BASELINE_WINDOW_4       (0x02)
#define AM_TMG49037_PXDCAVG_BASELINE_WINDOW_8       (0x03)
#define AM_TMG49037_PXDCAVG_BASELINE_WINDOW_16      (0x04)
#define AM_TMG49037_PXDCAVG_BASELINE_WINDOW_32      (0x05)
#define AM_TMG49037_PXDCAVG_BASELINE_WINDOW_64      (0x06)
#define AM_TMG49037_PXDCAVG_BASELINE_WINDOW_128     (0x07)

/**
 * \brief 传感器 TMG49037 二进制搜索目标
 */
#define AM_TMG49037_BINSRCH_TARGET_0                (0x00)
#define AM_TMG49037_BINSRCH_TARGET_1                (0x01)
#define AM_TMG49037_BINSRCH_TARGET_3                (0x02)
#define AM_TMG49037_BINSRCH_TARGET_7                (0x03)
#define AM_TMG49037_BINSRCH_TARGET_15               (0x04)
#define AM_TMG49037_BINSRCH_TARGET_31               (0x05)
#define AM_TMG49037_BINSRCH_TARGET_63               (0x06)
#define AM_TMG49037_BINSRCH_TARGET_127              (0x07)

/**
 * \brief 传感器 TMG49037 AUTO_GTHR_IN_MULT
    factor = (AUTO_GTHR_IN_MULT + 4) / 4
 */
#define AM_TMG49037_AUTO_GTHR_IN_MULT_1_00          (0x00)
#define AM_TMG49037_AUTO_GTHR_IN_MULT_1_25          (0x01)
#define AM_TMG49037_AUTO_GTHR_IN_MULT_1_50          (0x02)
#define AM_TMG49037_AUTO_GTHR_IN_MULT_4_50          (0x0E)
#define AM_TMG49037_AUTO_GTHR_IN_MULT_4_75          (0x0F)

/**
 * \brief 传感器 TMG49037 中断使能
 */
#define AM_TMG49037_ASIEN                           (1u << 7)
#define AM_TMG49037_PGSIEN                          (1u << 6)
#define AM_TMG49037_PIEN                            (1u << 5)
#define AM_TMG49037_AIEN                            (1u << 4)
#define AM_TMG49037_GIEN                            (1u << 2)
#define AM_TMG49037_CIEN                            (1u << 1)

/**
 * \brief 传感器 TMG49037 中断清除
 */
#define AM_TMG49037_INTCLEAR_ASAT                   (1u << 7)
#define AM_TMG49037_INTCLEAR_PGSAT                  (1u << 6)
#define AM_TMG49037_INTCLEAR_PINT                   (1u << 5)
#define AM_TMG49037_INTCLEAR_AINT                   (1u << 4)
#define AM_TMG49037_INTCLEAR_GINT                   (1u << 2)
#define AM_TMG49037_INTCLEAR_CINT                   (1u << 1)

/**
 * \brief 传感器 TMG49037 控制指令定义
 */
typedef enum am_sensor_tmg49037_control_cmd {
    AM_SENSOR_TMG49037_FUNC_ENABLE = 1,
    AM_SENSOR_TMG49037_FUNC_DISABLE,
    AM_SENSOR_TMG49037_ATIME_SET,
    AM_SENSOR_TMG49037_PTIME_SET,
    AM_SENSOR_TMG49037_WTIME_SET,
    AM_SENSOR_TMG49037_LOW_ALS_THRESHOLD_SET,
    AM_SENSOR_TMG49037_HIGH_ALS_THRESHOLD_SET,
    AM_SENSOR_TMG49037_LOW_PROX_THRESHOLD_SET,
    AM_SENSOR_TMG49037_HIGH_PROX_THRESHOLD_SET,
    AM_SENSOR_TMG49037_LOW_ALS_THRESHOLD_GET,
    AM_SENSOR_TMG49037_HIGH_ALS_THRESHOLD_GET,
    AM_SENSOR_TMG49037_LOW_PROX_THRESHOLD_GET,
    AM_SENSOR_TMG49037_HIGH_PROX_THRESHOLD_GET,
    AM_SENSOR_TMG49037_PPERS_SET,
    AM_SENSOR_TMG49037_APERS_SET,
    AM_SENSOR_TMG49037_LOWPOWER_IDLE_SET,
    AM_SENSOR_TMG49037_WLONG_SET,
    AM_SENSOR_TMG49037_PGPULSE_LEN_SET,
    AM_SENSOR_TMG49037_PPULSE_SET,
    AM_SENSOR_TMG49037_PGGAIN_SET,
    AM_SENSOR_TMG49037_PGLDRIVE_SET,
    AM_SENSOR_TMG49037_AGAIN_SET,
    AM_SENSOR_TMG49037_REVID_GET,
    AM_SENSOR_TMG49037_ID_GET,
    AM_SENSOR_TMG49037_CFG2_ENABLE,
    AM_SENSOR_TMG49037_CFG2_DISABLE,
    AM_SENSOR_TMG49037_LTF_USEPROX_SET,
    AM_SENSOR_TMG49037_SAI_SET,
    AM_SENSOR_TMG49037_CFG4_SET_BIT,
    AM_SENSOR_TMG49037_CFG4_RESET_BIT,
    AM_SENSOR_TMG49037_CFG5_SET_BIT,
    AM_SENSOR_TMG49037_CFG5_RESET_BIT,
    AM_SENSOR_TMG49037_GTHR_MASK_SET,
    AM_SENSOR_TMG49037_GTHR_OUT_NR_SET,
    AM_SENSOR_TMG49037_GFIFOTHR_SET,
    AM_SENSOR_TMG49037_GPULSE_SET,
    AM_SENSOR_TMG49037_GEST_FILTER_SIZE_SET,
    AM_SENSOR_TMG49037_GEST_FILTER_SET,
    AM_SENSOR_TMG49037_GTIME_SET,
    AM_SENSOR_TMG49037_GMODE_SET,
    AM_SENSOR_TMG49037_GESTURE_ENTRY_THRESHOLD_SET,
    AM_SENSOR_TMG49037_GESTURE_EXIT_THRESHOLD_SET,
    AM_SENSOR_TMG49037_SAI_ACTIVE_CLEAR,
    AM_SENSOR_TMG49037_AUXID_GET,
    AM_SENSOR_TMG49037_AZ_NTH_ITERATION_SET,
    AM_SENSOR_TMG49037_START_OFFSET_CAL_SET,
    AM_SENSOR_TMG49037_DCAVG_AUTO_OFFSET_ADJUST_SET,
    AM_SENSOR_TMG49037_PROX_CAL_TYPE_SEL,
    AM_SENSOR_TMG49037_BINSRCH_SKIP_SET,
    AM_SENSOR_TMG49037_DCAVG_ITERATIONS_SET,
    AM_SENSOR_TMG49037_PXDCAVG_AUTO_GTHR_SET,
    AM_SENSOR_TMG49037_PROX_AUTO_OFFSET_ADJUST_SET,
    AM_SENSOR_TMG49037_PXDCAVG_AUTO_BASELINE_SET,
    AM_SENSOR_TMG49037_PXDCAVG_BASELINE_WINDOW_SET,
    AM_SENSOR_TMG49037_DCAVG_AUTO_GTHR_SET,
    AM_SENSOR_TMG49037_BINSRCH_TARGET_SET,
    AM_SENSOR_TMG49037_AUTO_GTHR_IN_MULT_SET,
    AM_SENSOR_TMG49037_ZERO_WEIGHT_SET,
    AM_SENSOR_TMG49037_INT_ENABLE,
    AM_SENSOR_TMG49037_INT_DISABLE,
    AM_SENSOR_TMG49037_INT_CLEAR,
} am_sensor_tmg49037_control_cmd_t;

/*
 * \brief 传感器TMG49037提供的所有通道ID定义
 */
#define AM_TMG49037_CHAN_1      0              /* TMG49037 的通道1，Prox */
#define AM_TMG49037_CHAN_2      1              /* TMG49037 的通道2，ALS */
#define AM_TMG49037_CHAN_3      2              /* TMG49037 的通道3，GESTURE */

/**
 * \brief TMG49037参数配置初始化结构体定义 
 */
typedef struct tmg49037_param_config
{
    uint8_t     atime_val;                  /**< \brief ALS ADC的积分时间 */
    uint8_t     ptime_val;                  /**< \brief Prox采样时间 */
    uint8_t     wtime_val;                  /**< \brief ALS与Prox之间的等待时间 */
    uint8_t     ppers_val;                  /**< \brief Prox中断过滤器 */
    uint8_t     apers_val;                  /**< \brief ALS中断过滤器 */
    uint8_t     lowpower_idle;              /**< \brief 低功耗空闲 */
    am_bool_t   wlong_en;                   /**< \brief WLONG使能设置，使能后WTIME值将扩大12倍 */
    uint8_t     pgpulse_len;                /**< \brief Prox脉冲长度 */
    uint8_t     pulse;                      /**< \brief 一个Prox周期的最大脉冲数 */
    uint8_t     pggain_val;                 /**< \brief Prox IR传感器增益 */
    uint8_t     pgldrive_val;               /**< \brief Prox IR VCSEL电流 */
    uint8_t     again_val;                  /**< \brief ALS/Color传感器增益 */
    uint8_t     ltf_useprox;                /**< \brief LTF_USEPROX */
    am_bool_t   sleep_after_interrupt;      /**< \brief SAI */
    uint8_t     cfg4_set_bit;               /**< \brief CFG4位置位 */
    uint8_t     cfg4_reset_bit;             /**< \brief CFG4位清除 */
    uint8_t     cfg5_set_bit;               /**< \brief CFG5位置位 */
    uint8_t     cfg5_reset_bit;             /**< \brief CFG5位清除 */
    uint8_t     gthr_mask;                  /**< \brief GTHR_MASK */
    uint8_t     gthr_out_nr;                /**< \brief GTHR_OUT_NR */
    uint8_t     gfifothr;                   /**< \brief GFIFOTHR */
    uint8_t     gpulse;                     /**< \brief GPULSE */
    uint8_t     gest_filter_size;           /**< \brief GEST_FILTER_SIZE */
    uint8_t     gest_filter;                /**< \brief GEST_FILTER */
    uint8_t     gtime_val;                  /**< \brief GTIME */
    uint8_t     az_nth_iteration;           /**< \brief AZ_NTH_ITERATION */
    am_bool_t   dcavg_auto_offset_adjust;   /**< \brief DCAVG_AUTO_OFFSET_ADJUST */
    uint8_t     prox_cal_type;              /**< \brief Prox校准类型 */
    uint8_t     binsrch_skip;               /**< \brief BINSRCH_SKIP */
    uint8_t     dcavg_iterations;           /**< \brief DCAVG_ITERATIONS */
    uint8_t     pxdcavg_auto_gthr;          /**< \brief PXDCAVG_AUTO_GTHR */
    uint8_t     prox_auto_offset_adjust;    /**< \brief PROX_AUTO_OFFSET_ADJUST */
    uint8_t     pxdcavg_auto_baseline;      /**< \brief PXDCAVG_AUTO_BASELINE */
    uint8_t     pxdcavg_baseline_window;    /**< \brief PXDCAVG_BASELINE_WINDOW */
    uint8_t     dcavg_auto_gthr;            /**< \brief DCAVG_AUTO_GTHR */
    uint8_t     binsrch_target;             /**< \brief BINSRCH_TARGET */
    uint8_t     auto_gthr_in_mult;          /**< \brief AUTO_GTHR_IN_MULT */
    uint8_t     zero_weight;                /**< \brief ZERO_WEIGHT */
} tmg49037_param_config_t;

/**
 * \brief 传感器 TMG49037 设备信息结构体
 */
typedef struct am_sensor_tmg49037_devinfo {

    const   tmg49037_param_config_t     *p_param_default;   /**< \brief 参数初始化 */

    int                                  trigger_pin;       /**< \brief 报警输出引脚 */

    uint8_t                              i2c_addr;          /**< \brief I2C 7位 设备地址 */

} am_sensor_tmg49037_devinfo_t;

/**
 * \brief TMG49037 Prox校准值保存
 */
typedef struct am_tmg49037_calibration_data {
    int16_t offset_n;
    int16_t offset_s;
    int16_t offset_w;
    int16_t offset_e;
} am_tmg49037_calibration_data_t;

/**
 * \brief TMG49037 结果结构体定义
 */
typedef struct tmg49037_result_info
{
    uint16_t clear_channel_data;        /**< \brief clear */
    uint16_t red_channel_data;          /**< \brief red */
    uint16_t green_channel_data;        /**< \brief green */
    uint16_t blue_channel_data;         /**< \brief blue */
    uint16_t raw_prox_value;            /**< \brief Prox AD值 */
    uint16_t raw_gfifo_n;               /**< \brief GFIFON AD值 */
    uint16_t raw_gfifo_s;               /**< \brief GFIFOS AD值 */
    uint16_t raw_gfifo_w;               /**< \brief GFIFOW AD值 */
    uint16_t raw_gfifo_e;               /**< \brief GFIFOE AD值 */
    int16_t  prox_offset_adjust_value;  /**< \brief Prox校准值 */
    int16_t  offset_n_adjust_value;     /**< \brief OFFSETN 校准值 */
    int16_t  offset_s_adjust_value;     /**< \brief OFFSETS 校准值 */
    int16_t  offset_w_adjust_value;     /**< \brief OFFSETW 校准值 */
    int16_t  offset_e_adjust_value;     /**< \brief OFFSETE 校准值 */
    uint16_t prox_value;                /**< \brief Prox值 */
    uint16_t gfifo_n;                   /**< \brief GFIFON 值 */
    uint16_t gfifo_s;                   /**< \brief GFIFOS 值 */
    uint16_t gfifo_w;                   /**< \brief GFIFOW 值 */
    uint16_t gfifo_e;                   /**< \brief GFIFOE 值 */
} tmg49037_result_info_t;

/**
 * \breif 传感器 TMG49037 设备结构体定义
 */
typedef struct am_sensor_tmg49037_dev {
    am_sensor_serv_t                    tmg49037_dev;   /*< \breif 传感器标准服务 */
    am_i2c_device_t                     i2c_dev;        /*< \brief i2c设备实例 */
    am_tmg49037_calibration_data_t      cal_val[1];     /*< \brief 保存Prox校准值 */
    void                                *p_arg[3];      /*< \brief 触发回调函数参数 */
    uint8_t                             flags[3];       /*< \brief 触发标志位 */
    am_isr_defer_job_t                  g_myjob;        /*< \brief 中断延时处理任务 */
    uint8_t                             trigger;        /*< \brief 记录标志位 */
    am_sensor_val_t                     data[9];        /*< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t              pfn_trigger_cb[3];

    /** \brief 设备信息 */
    const am_sensor_tmg49037_devinfo_t *dev_info;

} am_sensor_tmg49037_dev_t;

/**
 * \brief 传感器 TMG49037 初始化
 *
 * \param[in] p_dev     : 指向传感器TMG49037设备的指针
 * \param[in] p_devinfo : 指向传感器TMG49037设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_tmg49037_init (
        am_sensor_tmg49037_dev_t            *p_dev,
        const am_sensor_tmg49037_devinfo_t  *p_devinfo,
        am_i2c_handle_t                      handle);

/**
 * \brief 传感器 TMG49037 去初始化
 *
 * \param[in] handle : am_sensor_tmg49037_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_tmg49037_deinit (am_sensor_handle_t handle);

/**
 * \brief 传感器 TMG49037 控制函数
 *
 * \param[in] handle    : am_sensor_tmg49037_init()初始化函数获得的传感器标准服务句柄
 * \param[in] cmd       : 控制指令
 *
 * \param[in,out] p_arg : 该指令对应的参数
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_ENOTSUP : 指令不支持
 */
am_err_t am_sensor_tmg49037_ioctl (am_sensor_handle_t    handle,
                                   int                   cmd,
                                   void                 *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_TMG49037_H */

/* end of file */

