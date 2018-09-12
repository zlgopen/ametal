/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief SCT驱动
 *
 * 1. SCT（状态可配置定时器）是一个以计数器为基础，以事件为中心，状态状态可配置的定时器
 * 2. SCT计数器
 *    - 一个最大32位计数的计数器；
 *    - 可实现单向递增计数或者双向计数；
 *    - 可被SCT事件硬件上自动限制（清零或反向计数）、停止、启动、终止；
 *    - 可被SCT事件硬件上自动跟踪设定的匹配值、判断是否匹配；
 *    - 可被SCT事件硬件上自动获取计数值，以便实现捕获逻辑。
 * 3. SCT事件
 *    - 事件相互独立，事件数目高达13个；
 *    - 可软件配置事件产生的条件，条件可能是IO条件、匹配条件、捕获条件或者IO跟匹配条件组合；
 *    - 可软件配置事件在哪些SCT状态下可以产生的；
 *    - 可软件配置事件产生时，对计数器的操作，如限制（清零或反向计数）、停止、启动、终止；
 *    - 可软件配置事件产生时，是否触发DMA传输请求，支持两个DMA触发通道；
 *    - 可软件配置事件产生时，对输出IO通道进行控制；
 *    - 可软件配置事件产生时，产生SCT事件中断；
 *    - 可软件配置事件产生后，对SCT状态的影响。
 * 4. SCT状态
 *    - SCT提供了13个状态
 *    - 可以配置SCT处于哪个状态；
 *    - 状态之间可通过SCT事件进行自动切换。
 * 5. SCT 输入IO通道
 *    - 提供了8个输入IO通道；
 *    - 每个通道均可用于作为SCT事件触发的IO条件或者捕获引脚状态的输入；
 * 6. SCT 输出IO通道
 *    - 提供了8个输入IO通道；
 *    - 每个通道均可用于作为SCT事件触发的IO条件或者事件产生时，被控制的输出IO通道；
 *    - 输出IO通道3，可用于触发ADC转换，而其他通道无此功能；
 *    - 当多个事件同时操作同一个输出通道时，且发生了电平冲突，则可以配置这种冲突的解决方法。
 *
 * \internal
 * \par Modification history
 * - 1.00 15-11-30  hgo, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC_SCT_H
#define __AM_LPC_SCT_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_lpc_if_sct
 * \copydoc am_lpc_sct.h
 * @{
 */

/** \brief SCT事件中断信息 */
struct am_lpc_sct_evt_isr_info {

    /** \brief 触发回调函数 */
    am_pfnvoid_t  pfn_isr;

    /** \brief 回调函数的参数 */
    void         *p_arg;
};


/**
 * \brief SCT定时功能相关的设备信息
 */
typedef struct am_lpc_sct_devinfo {

    /** \brief SCT寄存器块的基地址*/
    uint32_t                           sct_regbase;

    /** \brief SCT中断号 */
    uint8_t                            inum;

    /** \brief SCT时钟编号 */
    uint32_t                           clk_id;

    /** \brief 总事件中断数量 */
    int                                evt_isr_cnt;

    /** \brief ISR信息映射(大小与 evt_isr_cnt 一致) */
    uint8_t                           *p_isrmap;

    /** \brief ISR信息数量 */
    int                                isrinfo_cnt;


    /** \brief ISR信息映射内存(大小与 isrinfo_cnt 一致) */
    struct am_lpc_sct_evt_isr_info *p_isrinfo;

    /** \brief 输出通道数量 */
    int                                 sct_out_num;

    /** \brief DMA请求通道数量  */
    int                                 sct_dma_req_num;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);
} am_lpc_sct_devinfo_t;

/**
 * \brief SCT定时功能设备
 */
typedef struct am_lpc_sct_dev {

    /** \brief 指向SCT(定时功能)设备信息常量的指针 */
    const am_lpc_sct_devinfo_t  *p_devinfo;

    /** \brief 所有事件的状态
     *  bit0 = event0, bit1 = event1...
     *  当bit0 = 1时，事件0已被使用
     *  当bit0 = 0时，事件0未使用
     */
    uint8_t                         evt_stat;

    /** \brief 参数有效标志 */
    am_bool_t                       valid_flg;
} am_lpc_sct_dev_t;

/** \brief SCT服务操作句柄类型定义 */
typedef am_lpc_sct_dev_t *am_lpc_sct_handle_t;

/**
 * \brief SCT事件
 */
typedef struct am_lpc_sct_evt {
    uint64_t    cond_flags;          /**< \brief 事件发生的条件配置项        */
    uint8_t     stat_en_flags;       /**< \brief 事件在哪些SCT状态下生效配置项     */
    uint8_t     out_stat_flag;       /**< \brief 事件发生时SCT状态控制输出配置项 */
    uint8_t     out_cnt_flags;      /**< \brief 事件发生时控制计数器的配置项   */
    uint8_t     out_dma_flags;       /**< \brief 事件发生时控制DMA请求配置项   */
    uint16_t    out_io_flags;        /**< \brief 事件发生时控制IO的配置项        */
    uint64_t    out_int_flag;        /**< \brief 事件发生时中断配置项        */
} am_lpc_sct_evt_t;


/**
 * \name 事件配置cond_flags bit[63:0]功能码位域定义
 * @{
 */
#define AM_LPC_SCT_ISOUT_BITS_START        0   /**< \brief IO方向选择起始位置  */
#define AM_LPC_SCT_IOSEL_CH_BITS_START     1   /**< \brief IO通道起始位置  */
#define AM_LPC_SCT_IOCOND_BITS_START       5   /**< \brief IO条件起始位置  */
#define AM_LPC_SCT_COND_OPR_BITS_START     7   /**< \brief 条件运算起始位置  */
#define AM_LPC_SCT_MATCAP_CH_BITS_START    16  /**< \brief 匹配通道起始位置  */
#define AM_LPC_SCT_MATCAP_ISCAP_BITS_START 24  /**< \brief 是否设置匹配   */
#define AM_LPC_SCT_MATCAP_ISSET_BITS_START 25  /**< \brief 是否匹配功能   */
#define AM_LPC_SCT_MAT_VAL_BITS_START      32  /**< \brief 匹配值位域起始位置*/
#define AM_LPC_SCT_MATCAP_CH_BITS_LEN      4   /**< \brief 匹配通道位域长度   */
/** @} */

/**
 * \name 事件控制状态变化配置out_stat_flag bit[7:0]功能码位域定义
 * @{
 */
#define AM_LPC_SCT_ISRELOAD_BITS_START    0  /**< \brief 是否重装状态起始位置  */
#define AM_LPC_SCT_STAT_VAL_BITS_START    1  /**< \brief 状态值起始位置  */
/** @} */

/**
 * \name 事件控制计数器配置out_cnt_flags bit[8:0]功能码位域定义
 * @{
 */
#define AM_LPC_SCT_LIMIT_BITS_START   0   /**< \brief 限制定时器起始位置  */
#define AM_LPC_SCT_STOP_BITS_START    1   /**< \brief 停止定时器起始位置  */
#define AM_LPC_SCT_START_BITS_START   2   /**< \brief 启动定时器起始位置  */
#define AM_LPC_SCT_HALT_BITS_START    3   /**< \brief 终止定时器起始位置  */
/** @} */

/**
 * \name 事件控制IO输出配置out_io_flags bit[15:0]功能码位域定义
 * @{
 */
#define AM_LPC_SCT_IOSET_CH_BITS_START    0   /**< \brief 置位IO通道起始位置  */
#define AM_LPC_SCT_IOCLR_CH_BITS_START    8   /**< \brief 清零IO通道起始位置  */
/** @} */

/**
 * \name 事件控制中断配置out_int_flag bit[63:0]功能码位域定义
 * @{
 */
#define AM_LPC_SCT_CALLBACK_BITS_START    0  /**< \brief 回调函数起始位置  */
#define AM_LPC_SCT_CBPARAM_BITS_START     32 /**< \brief 回调函数参数起始位置  */
#define AM_LPC_SCT_CALLBACK_BITS_LEN      32  /**< \brief 回调函数长度  */
#define AM_LPC_SCT_CBPARAM_BITS_LEN       32  /**< \brief 回调函数参数长度  */
/** @} */

/**
 * \name 事件控制DMA触发配置out_dma_flags bit[7:0]功能码位域定义
 * @{
 */
#define AM_LPC_SCT_DMA_REQ_CH_BITS_START    0 /**< \brief DMA触发通道起始位置  */
/** @} */

/**
 * \name SCT所有的输出IO通道编号
 * \anchor grp_am_lpc_sct_io_chan_out
 * @{
 */
#define    AM_LPC_SCT_IO_CHAN_OUT0   0   /**< \brief SCT输出通道0     */
#define    AM_LPC_SCT_IO_CHAN_OUT1   1   /**< \brief SCT输出通道1     */
#define    AM_LPC_SCT_IO_CHAN_OUT2   2   /**< \brief SCT输出通道2     */
#define    AM_LPC_SCT_IO_CHAN_OUT3   3   /**< \brief SCT输出通道3     */
#define    AM_LPC_SCT_IO_CHAN_OUT4   4   /**< \brief SCT输出通道4     */
#define    AM_LPC_SCT_IO_CHAN_OUT5   5   /**< \brief SCT输出通道5     */
#define    AM_LPC_SCT_IO_CHAN_OUT6   6   /**< \brief SCT输出通道6     */
#define    AM_LPC_SCT_IO_CHAN_OUT7   7   /**< \brief SCT输出通道7     */

/** @} */

/**
 * \name SCT所有的输入IO通道编号
 * \anchor grp_am_lpc_sct_io_chan_in
 * @{
 */
#define    AM_LPC_SCT_IO_CHAN_IN0    0    /**< \brief SCT输入通道0 */
#define    AM_LPC_SCT_IO_CHAN_IN1    1    /**< \brief SCT输入通道1 */
#define    AM_LPC_SCT_IO_CHAN_IN2    2    /**< \brief SCT输入通道2 */
#define    AM_LPC_SCT_IO_CHAN_IN3    3    /**< \brief SCT输入通道3 */
#define    AM_LPC_SCT_IO_CHAN_IN4    4    /**< \brief SCT输入通道4 */
#define    AM_LPC_SCT_IO_CHAN_IN5    5    /**< \brief SCT输入通道5 */
#define    AM_LPC_SCT_IO_CHAN_IN6    6    /**< \brief SCT输入通道6 */
#define    AM_LPC_SCT_IO_CHAN_IN7    7    /**< \brief SCT输入通道7 */

/** @} */

/**
 * \name SCT所有可供检测的IO条件
 * \anchor grp_am_lpc_sct_io_cond
 * @{
 */
#define    AM_LPC_SCT_IO_COND_LOW     0    /**< \brief 低电平检测     */
#define    AM_LPC_SCT_IO_COND_RISE    1    /**< \brief 上升沿检测     */
#define    AM_LPC_SCT_IO_COND_FALL    2    /**< \brief 下降沿检测     */
#define    AM_LPC_SCT_IO_COND_HIGH    3    /**< \brief 高电平检测     */
/** @} */


/**
 * \name SCT事件条件配置宏
 *
 * \note 特定的IO边沿或电平，指定的捕获电平或者某个匹配值均可触发SCT事件发生；
 * - #AM_LPC_SCT_EVT_COND_MAT 可以跟 #AM_LPC_SCT_EVT_COND_IO_OUT 或
 *   #AM_LPC_SCT_EVT_COND_IO_IN 其中一个进行‘或’或‘与’运算，组成的值作为条件配置项；
 * - #AM_LPC_SCT_EVT_COND_IO_OUT 和 #AM_LPC_SCT_EVT_COND_IO_IN
 *   不能同时用于配置条件；
 * - 使用 #AM_LPC_SCT_EVT_COND_CAP 做捕获条件时，与其他条件宏互斥，
 *   其他宏不能与该宏进行或运算组成条件配置项。
 * \anchor grp_am_lpc_sct_evt_cond_flags
 * @{
 */

/**
 *  \brief 匹配条件宏(当SCT计数器等于该匹配值时，事件发生)
 *         mat_ch为指定的匹配寄存器的序号，范围为0~7
 *         mat_val为匹配值，取值范围为0~0xFFFFFFFF
 */
#define AM_LPC_SCT_EVT_COND_MAT(mat_ch, mat_val)  \
                  (AM_BIT(AM_LPC_SCT_MATCAP_ISSET_BITS_START) | \
                   AM_SBF(mat_ch, AM_LPC_SCT_MATCAP_CH_BITS_START) | \
                   AM_SBF((uint64_t)mat_val, AM_LPC_SCT_MAT_VAL_BITS_START))

/**
 * \brief 输出IO条件宏（当指定IO通道的特定动作被检测到时，事件发生）
 *        io_ch_out为输出IO通道序号，参见\ref grp_am_lpc_sct_io_chan_out,
 *        io_cond为指定检测的IO动作，参见\ref grp_am_lpc_sct_io_cond
 */
#define AM_LPC_SCT_EVT_COND_IO_OUT(io_ch_out, io_cond)  \
                    (AM_BIT(AM_LPC_SCT_ISOUT_BITS_START) | \
                     AM_SBF(io_ch_output, AM_LPC_SCT_IOSEL_CH_BITS_START) | \
                     AM_SBF(io_cond, AM_LPC_SCT_IOCOND_BITS_START))

/**
 * \brief 输入IO条件宏（当指定IO通道的特定动作被检测到时，事件发生）
 *        io_ch_in为输入IO通道序号，参见\ref grp_am_lpc_sct_io_chan_in
 *        io_cond为指定检测的IO动作，参见\ref grp_am_lpc_sct_io_cond
 */
#define AM_LPC_SCT_EVT_COND_IO_IN(io_ch_in, io_cond) \
                      (AM_SBF(io_ch_input, AM_LPC_SCT_IOSEL_CH_BITS_START) |\
                       AM_SBF(io_cond, AM_LPC_SCT_IOCOND_BITS_START))
/**
 * \brief 捕获条件宏（当指定输入IO通道的特定动作被检测到时，事件发生）
 *        cap_ch为捕获通道序号，范围为0~7
 *        io_ch_in为输入IO通道序号，参见\ref grp_am_lpc_sct_io_chan_in
 *        io_cond为指定捕获的IO动作，参见\ref grp_am_lpc_sct_io_cond
 */
#define AM_LPC_SCT_EVT_COND_CAP(cap_ch, io_ch_in, io_cond) \
                      (AM_BIT(AM_LPC_SCT_MATCAP_ISSET_BITS_START) | \
                       AM_BIT(AM_LPC_SCT_MATCAP_ISCAP_BITS_START) | \
                       AM_SBF(mat_ch, AM_LPC_SCT_MATCAP_CH_BITS_START) |  \
                       AM_LPC_SCT_COND_IO_IN(io_ch_input, io_cond))


/** \brief 条件或运算，指定匹配条件、IO检测条件任意一个发生时，事件发生 */
#define AM_LPC_SCT_EVT_COND_OPR_OR      \
                                    AM_SBF(0, AM_LPC_SCT_COND_OPR_BITS_START)

/** \brief 只检测匹配条件，忽略IO检测条件，指定匹配条件发生时，事件发生*/
#define AM_LPC_SCT_EVT_COND_OPR_MATCH   \
                                    AM_SBF(1, AM_LPC_SCT_COND_OPR_BITS_START)

/** \brief 只检测匹配条件，忽略IO检测条件，指定IO条件发生时，事件发生*/
#define AM_LPC_SCT_EVT_COND_OPR_IO      \
                                    AM_SBF(2, AM_LPC_SCT_COND_OPR_BITS_START)

/** \brief 条件与运算，指定匹配条件和IO检测条件同时发生时，事件发生*/
#define AM_LPC_SCT_EVT_COND_OPR_AND     \
                                    AM_SBF(3, AM_LPC_SCT_COND_OPR_BITS_START)
/** @} */

/**
 * \name SCT事件状态使能配置宏
 *
 * \note 一个SCT状态可以产生多个SCT事件，一个事件可以在不同SCT状态下产生
 *
 * \anchor grp_am_lpc_sct_evt_stat_en_flags
 * @{
 */

/** \brief 事件在指定状态可以产生，stat_num取值为0~7 */
#define AM_LPC_SCT_EVT_STAT_EN(stat_num)         AM_SBF(1, stat_num)

/** \brief 事件在指定状态范围下可以产生
 *         stat_mask的bit0代表状态0，bit0=1时，事件在状态0使能，
 *                                 bit1=1时，事件在状态1使能，其他位可类推
 */
#define AM_LPC_SCT_EVT_STAT_EN_MASK(stat_mask)   (stat_mask)
/** @} */


/**
 * \name SCT事件控制状态配置宏
 *
 * \note 事件发生后，配置SCT的状态变化，
 *       但是仅当该事件是当前状态下允许发生的事件中事件号最高时，才按此配置改变SCT状态
 * \anchor grp_am_lpc_sct_evt_out_stat_flags
 * @{
 */

/** \brief 事件发生后，重装状态值，SCT状态变为stat，stat取值为0~7 */
#define AM_LPC_SCT_EVT_OUT_STAT_RELOAD(stat)   \
                              (AM_BIT(AM_LPC_SCT_ISRELOAD_BITS_START) |\
                               AM_SBF(stat, AM_LPC_SCT_STAT_VAL_BITS_START))

/** \brief 事件发生后，叠加状态值，SCT状态变为(old_state + stat)，stat取值为0~7 */
#define AM_LPC_SCT_EVT_OUT_STAT_ADD(stat)      \
                               AM_SBF(stat, AM_LPC_SCT_STAT_VAL_BITS_START)
/** @} */

/**
 * \name SCT事件控制计数器配置宏
 *
 * \note 一个SCT事件可以同时控制计数器限制、停止、启动、终止；
 *       - 限制计数器是指计数器清零或者反向计数，取决于SCT计数器的配置；
 *       - 停止计数器是指计数器停止计数，但是与IO相关的输入或输出仍有效，只是匹配和捕获失效，
 *                                 可以硬件重新启动；
 *       - 启动计数器是指计数器开始计数，将处在停止状态下的计数器，重新启动计数；
 *       - 终止计数器是关闭SCT所有时序，与停止计数器的区别是此时IO相关动作也失效，
 *                                 并且硬件无法唤醒，必须使用软件清除以重新启动SCT。
 * \anchor grp_am_lpc_sct_evt_out_cnt_flags
 * @{
 */
/** \brief 事件发生时，限制计数器计数   */
#define AM_LPC_SCT_EVT_OUT_CNT_LIMIT  AM_BIT(AM_LPC_SCT_LIMIT_BITS_START)

/** \brief 事件发生时，停止计数器计数   */
#define AM_LPC_SCT_EVT_OUT_CNT_STOP   AM_BIT(AM_LPC_SCT_STOP_BITS_START)

/** \brief 事件发生时，启动计数器计数   */
#define AM_LPC_SCT_EVT_OUT_CNT_START  AM_BIT(AM_LPC_SCT_START_BITS_START)

/** \brief 事件发生时，终止计数器计数   */
#define AM_LPC_SCT_EVT_OUT_CNT_HALT   AM_BIT(AM_LPC_SCT_HALT_BITS_START)
/** @} */

/**
 * \name SCT事件控制DMA触发配置宏
 *
 * \note SCT事件可同时触发DMA输出请求0或DMA输出请求1
 * \anchor grp_am_lpc_sct_evt_out_dma_flags
 * @{
 */
/**
 * \brief 事件发生时，触发DMA请求
 *
 *        dma_req_out为DMA请求输出通道编号，值为0或1
 */
#define AM_LPC_SCT_EVT_OUT_DMA(dma_req_out) AM_BIT(dma_req_out)
/** @} */

/**
 * \name SCT事件控制IO输出配置宏
 *
 * \note SCT事件可以同时控制多个IO输出通道置位或清零
 * \anchor grp_am_lpc_sct_evt_out_io_flags
 * @{
 */
/**
 * \brief 事件发生时，将某输出IO通道引脚置位
 *        io_ch_out为输出IO通道序号，参见\ref grp_am_lpc_sct_io_chan_out,
 */
#define AM_LPC_SCT_EVT_OUT_IO_SET(io_ch_out) AM_BIT(io_ch_out)

/**
 * \brief 事件发生时，将某输出IO通道引脚清零
 *        io_ch_out为输出IO通道序号，参见\ref grp_am_lpc_sct_io_chan_out,
 */
#define AM_LPC_SCT_EVT_OUT_IO_CLR(io_ch_out)  \
                           AM_BIT(io_ch_out + AM_LPC_SCT_IOCLR_CH_BITS_START)
/** @} */


/**
 * \name SCT事件中断配置宏
 *
 * \note SCT事件最多只可以配置一个中断回调函数
 * \anchor grp_am_lpc_sct_evt_out_int_flags
 * @{
 */
/**
 * \brief 事件发生时，产生中断
 *        callback为回调函数，p_arg为回调函数的参数
 */
#define AM_LPC_SCT_EVT_OUT_INT(callback, p_arg)    \
                       (AM_SBF((uint64_t)((uint32_t)p_arg), \
                               AM_LPC_SCT_CBPARAM_BITS_START) |\
                              ((uint32_t)callback))
/** @} */


/**
 * \brief 初始化SCT
 *
 * \param[in] p_dev     : 指向SCT设备的指针
 * \param[in] p_devinfo : 指向SCT设备信息常量的指针
 *
 * \return SCT服务操作句柄，值为NULL时表明初始化失败
 */
am_lpc_sct_handle_t am_lpc_sct_init (am_lpc_sct_dev_t      *p_dev,
                                     const am_lpc_sct_devinfo_t  *p_devinfo);

/**
 * \brief 不使用SCT，解初始化SCT，释放相关资源
 *
 * \param[in] handle : am_lpc_sct_init() 初始化函数获得的SCT服务句柄
 *
 * \return 无
 */
void am_lpc_sct_deinit (am_lpc_sct_handle_t handle);

/**
 * \brief 初始化SCT事件配置
 *
 * \param[in] p_evt : 指向要初始化的事件
 *
 */
am_static_inline
void am_lpc_sct_evt_init (am_lpc_sct_evt_t *p_evt)
{

    p_evt->cond_flags     = 0;
    p_evt->stat_en_flags  = 0;
    p_evt->out_stat_flag  = 0;
    p_evt->out_cnt_flags  = 0;
    p_evt->out_dma_flags  = 0;
    p_evt->out_io_flags   = 0;
    p_evt->out_int_flag   = 0;
}

/** \brief 某SCT事件功能不使用配置宏 */
#define AM_LPC_SCT_EVT_FUNCS_NOT_USE   0

/**
 * \brief 建立SCT事件
 *
 * \param[in] p_evt          : 指向要建立的事件
 *
 * \param[in] cond_flags     : 产生事件的条件配置
 *                             参见\ref grp_am_lpc_sct_evt_cond_flags
 *
 * \param[in] stat_en_flags  : 事件在哪些SCT状态下生效配置
 *                             参见\ref grp_am_lpc_sct_evt_stat_en_flags
 *
 * \param[in] out_stat_flag  : 事件发生时SCT状态控制输出配置
 *                             参见\ref grp_am_lpc_sct_evt_out_stat_flags
 *
 * \param[in] out_cnt_flags  : 事件发生时控制计数器的配置
 *                             参见\ref grp_am_lpc_sct_evt_out_cnt_flags
 *
 * \param[in] out_dma_flags  : 事件发生时DMA控制输出配置
 *                             参见\ref grp_am_lpc_sct_evt_out_dma_flags
 *
 * \param[in] out_io_flags   : 事件发生时控制IO的配置
 *                             参见\ref grp_am_lpc_sct_evt_out_io_flags
 *
 * \param[in] out_int_flag   : 事件发生时中断配置
 *                             参见\ref grp_am_lpc_sct_evt_out_int_flags
 * \return 无
 *
 * \note 若不使用某项功能配置时，均可用宏 #AM_LPC_SCT_EVT_FUNCS_NOT_USE 指定。
 */
am_static_inline
void am_lpc_sct_evt_build (am_lpc_sct_evt_t *p_evt,
                              uint64_t             cond_flags,
                              uint8_t              stat_en_flags,
                              uint16_t             out_stat_flag,
                              uint8_t              out_cnt_flags,
                              uint8_t              out_dma_flags,
                              uint16_t             out_io_flags,
                              uint64_t             out_int_flag)
{
    p_evt->cond_flags     = cond_flags;
    p_evt->stat_en_flags  = stat_en_flags;
    p_evt->out_stat_flag  = out_stat_flag;
    p_evt->out_cnt_flags  = out_cnt_flags;
    p_evt->out_dma_flags  = out_dma_flags;
    p_evt->out_io_flags   = out_io_flags;
    p_evt->out_int_flag   = out_int_flag;
}

/**
 * \brief 添加SCT事件
 *
 * \param[in] sct_handle : SCT服务操作句柄
 * \param[in] evt_num    : 事件号，取值范围0~7
 * \param[in] p_evt      : 指向要添加的事件
 *
 * \retval  AM_OK        : 添加成功
 * \retval -AM_EINVAL    : 添加失败, 参数错误
 * \retval -AM_EPERM     : 没有空的内存映射，回调函数超过设定的限制
 *
 */
int am_lpc_sct_evt_add (am_lpc_sct_handle_t   sct_handle,
                           uint8_t                  evt_num,
                           am_lpc_sct_evt_t     *p_evt);

/**
 * \brief 删除SCT事件
 *
 * \param[in] sct_handle : SCT服务操作句柄
 * \param[in] evt_num    : 事件号，取值范围0~7
 *
 * \retval  AM_OK        : 删除成功
 * \retval -AM_EINVAL    : 删除失败, 参数错误
 *
 */
int am_lpc_sct_evt_del (am_lpc_sct_handle_t   sct_handle,
                           uint8_t                  evt_num);


/**
 * \brief 启动SCT
 *
 * \param[in] sct_handle : SCT服务操作句柄
 *
 * \retval  AM_OK        : 启动成功
 * \retval -AM_EINVAL    : 启动失败, 参数错误
 *
 */
int am_lpc_sct_start (am_lpc_sct_handle_t   sct_handle);

/**
 * \brief 停止SCT
 *
 * \param[in] sct_handle : SCT服务操作句柄
 *
 * \retval  AM_OK        : 停止成功
 * \retval -AM_EINVAL    : 停止失败, 参数错误
 *
 */
int am_lpc_sct_stop (am_lpc_sct_handle_t   sct_handle);

/**
 * \brief 终止SCT
 *
 * \param[in] sct_handle : SCT服务操作句柄
 *
 * \retval  AM_OK        : 终止成功
 * \retval -AM_EINVAL    : 终止失败, 参数错误
 *
 */
int am_lpc_sct_halt (am_lpc_sct_handle_t   sct_handle);


/**
 * \name SCT输出通道电平冲突的解决方法配置宏
 * \anchor grp_am_lpc_sct_res_flags
 * @{
 */
#define AM_LPC_SCT_RES_OUT_NOCHANGE     (0)    /**< \brief 冲突时无变化     */
#define AM_LPC_SCT_RES_OUT_SET          (1)    /**< \brief 冲突时置位输出   */
#define AM_LPC_SCT_RES_OUT_CLR          (2)    /**< \brief 冲突时清零输出   */
#define AM_LPC_SCT_RES_OUT_TOG          (3)    /**< \brief 冲突时翻转输出   */
/**
 * @}
 */

/**
 * \brief 配置输出通道电平冲突时的解决方法
 *
 * \param[in] sct_handle : SCT服务操作句柄
 * \param[in] output_num : 输出通道号，0 ~ 5
 * \param[in] value      : 冲突解决方式：\ref grp_am_lpc_sct_res_flags
 *                          - #AM_LPC_SCT_RES_OUT_NOCHANGE : 输出无变化
 *                          - #AM_LPC_SCT_RES_OUT_SET      : 输出高电平
 *                          - #AM_LPC_SCT_RES_OUT_CLR      : 输出低电平
 *                          - #AM_LPC_SCT_RES_OUT_TOG      : 翻转输出电平
 *
 * \retval  AM_OK        : 配置成功
 * \retval -AM_EINVAL    : 配置失败, 参数错误
 */
int am_lpc_sct_conflict_res_config (am_lpc_sct_handle_t sct_handle,
                                       uint8_t                output_num,
                                       uint8_t                value);

/**
 * \brief 设置SCT状态值
 *
 * \param[in] sct_handle : SCT服务操作句柄
 * \param[in] state    : 状态值
 *
 * \note 设置状态值时，必须确保相应的计数器处于终止(HALT)状态，否则设定失败
 *
 * \retval  AM_OK        : 设置成功
 * \retval -AM_EINVAL    : 设置失败, 参数错误
 * \retval -AM_EPERM     : 未处于HALT状态，设置失败
 */
int am_lpc_sct_state_set (am_lpc_sct_handle_t sct_handle,
                             uint16_t               state);

/**
 * \brief 通过时间获取需要设定的匹配值
 *
 * \param[in] sct_handle : SCT服务操作句柄
 * \param[in] period_ns  : 周期，单位ns
 *
 * \return 匹配值
 *
 * \note 如果达到可配置的极限，即匹配周期过小或过大，那么输出自动限制为1（周期过小）
 *       或4294967295（周期过大）。
 */
uint32_t am_lpc_sct_time_to_match(am_lpc_sct_handle_t sct_handle,
                                     uint32_t               period_ns);

/**
 * \brief 对指定的输出通道设置输出电平
 *
 * \param[in] sct_handle : SCT服务操作句柄
 * \param[in] output_num : 输出通道号，0 ~ 5
 * \param[in] value      : 引脚状态，参见 \ref grp_am_gpio_pin_level
 *
 * \note 写SCT输出寄存器必须确保所有的计数器处于终止(HALT)状态，否则设定失败
 *
 * \retval  AM_OK        : 设置成功
 * \retval -AM_EINVAL    : 设置失败, 参数错误
 * \retval -AM_EPERM     : 未处于HALT状态，设置失败
 */

int am_lpc_sct_out_state_set (am_lpc_sct_handle_t sct_handle,
                              uint8_t             output_num,
                              uint8_t             value);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC_SCT_H */

/* end of file */
