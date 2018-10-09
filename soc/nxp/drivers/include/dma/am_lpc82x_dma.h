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
 * \brief LPC82x DMA 操作接口
 *
 * DMA 传输的数据通过两种结构描述：
 * - #am_lpc82x_dma_transfer_t  可以进行简单的内存到内存，内存到外设，外设到内存传输；
 * - #am_lpc82x_dma_xfer_desc_t 可以灵活的配置链式传输，如pingpong模式传输。
 * 
 * \internal
 * \par Modification History
 * - 1.01 15-11-30  mem,modified.
 * - 1.00 15-07-12  win, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC82X_DMA_H
#define __AM_LPC82X_DMA_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_lpc82x_if_dma
 * \copydoc am_lpc82x_dma.h
 * @{
 */

/**
 * \name DMA transfer结构体传输控制参数
 * \anchor grp_dma_trans_flags
 *
 *  用于配置结构体 #am_lpc82x_dma_transfer_t 中flags参数
 * @{
 */

/** \brief 传输数据宽度为8 bit */
#define AM_LPC82X_DMA_TRANS_WIDTH_8BIT   AM_SBF(0, 0)

/** \brief 传输数据宽度为16 bit */
#define AM_LPC82X_DMA_TRANS_WIDTH_16BIT  AM_SBF(1, 0)

/** \brief 传输数据宽度为32 bit */
#define AM_LPC82X_DMA_TRANS_WIDTH_32BIT  AM_SBF(2, 0)

/** \brief 源地址不增加 */
#define AM_LPC82X_DMA_TRANS_SRCINC_NOINC AM_SBF(0, 2)

/** \brief 源地址增量为1个数据宽度 */
#define AM_LPC82X_DMA_TRANS_SRCINC_1X    AM_SBF(1, 2)

/** \brief 源地址增量为2个数据宽度*/
#define AM_LPC82X_DMA_TRANS_SRCINC_2X    AM_SBF(2, 2)

/** \brief 源地址增量为4个数据宽度 */
#define AM_LPC82X_DMA_TRANS_SRCINC_4X    AM_SBF(3, 2)

/** \brief 目标地址不变 */
#define AM_LPC82X_DMA_TRANS_DSTINC_NOINC AM_SBF(0, 4)

/** \brief 目标地址增量为1个数据宽度 */
#define AM_LPC82X_DMA_TRANS_DSTINC_1X    AM_SBF(1, 4)

/** \brief 目标地址增量为2个数据宽度 */
#define AM_LPC82X_DMA_TRANS_DSTINC_2X    AM_SBF(2, 4)

/** \brief 目标地址增量为4个数据宽度 */
#define AM_LPC82X_DMA_TRANS_DSTINC_4X    AM_SBF(3, 4)

/** \brief 传输完成后重复传输 */
#define AM_LPC82X_DMA_TRANS_REPEAT       AM_BIT( 6 )

/** @} */

/** \brief DMA transfer结构体 */
typedef struct am_lpc82x_dma_transfer {
    uint32_t src_addr;            /**< \brief 源地址              */
    uint32_t dst_addr;            /**< \brief 目标地址            */
    uint16_t nbytes;              /**< \brief 总共传输的字节数    */
    uint16_t flags;               /**< \brief 传输控制标志        */
} am_lpc82x_dma_transfer_t;

/**
 * \name DMA xfer_desc结构体传输控制参数
 * \anchor grp_am_lpc82x_dma_xfer_flags
 *
 * 用于配置 #am_lpc82x_dma_xfer_desc_t 中的xfercfg参数
 *
 * @{
 */

/** \brief 传输描述符有效 */
#define AM_LPC82X_DMA_XFER_VALID           AM_BIT(0)

/** \brief 描述符传输完成时，自动重载新的通道描述符 */
#define AM_LPC82X_DMA_XFER_RELOAD           AM_BIT(1)

/** \brief 立即触发DMA传输启动 */
#define AM_LPC82X_DMA_XFER_SWTRIG           AM_BIT(2)

/** \brief 描述符传输完成时，清除触发标志 */
#define AM_LPC82X_DMA_XFER_CLRTRIG          AM_BIT(3)

/** \brief 描述符传输完成时，产生A中断 */
#define AM_LPC82X_DMA_XFER_SETINTA          AM_BIT(4)

/** \brief 描述符传输完成时，产生B中断 */
#define AM_LPC82X_DMA_XFER_SETINTB          AM_BIT(5)

/** \brief 源地址不增加 */
#define AM_LPC82X_DMA_XFER_WIDTH_8BIT       AM_SBF(0, 8)

/** \brief 传输数据宽度为16 bit */
#define AM_LPC82X_DMA_XFER_WIDTH_16BIT      AM_SBF(1, 8)

/** \brief 传输数据宽度为32 bit */
#define AM_LPC82X_DMA_XFER_WIDTH_32BIT      AM_SBF(2, 8)

/** \brief 每次传输源地址不增加  */
#define AM_LPC82X_DMA_XFER_SRCINC_NOINC    AM_SBF(0, 12)

/** \brief 源地址增量为1个数据宽度 */
#define AM_LPC82X_DMA_XFER_SRCINC_1X        AM_SBF(1, 12)

/** \brief 源地址增量为2个数据宽度 */
#define AM_LPC82X_DMA_XFER_SRCINC_2X        AM_SBF(2, 12)

/** \brief 源地址增量为4个数据宽度 */
#define AM_LPC82X_DMA_XFER_SRCINC_4X        AM_SBF(3, 12)

/** \brief 目标地址不变 */
#define AM_LPC82X_DMA_XFER_DSTINC_NOINC     AM_SBF(0, 14)

/** \brief 目标地址增量为1个数据宽度 */
#define AM_LPC82X_DMA_XFER_DSTINC_1X        AM_SBF(1, 14)

/** \brief 目标地址增量为2个数据宽度 */
#define AM_LPC82X_DMA_XFER_DSTINC_2X        AM_SBF(2, 14)

/** \brief 目标地址增量为3个数据宽度 */
#define AM_LPC82X_DMA_XFER_DSTINC_4X        AM_SBF(3, 14)

/**
 * \brief DMA xfer_desc结构体
 * \attention xfer_desc的地址必须是16字节对齐
 */
typedef struct am_lpc82x_dma_xfer_desc {
    volatile uint32_t xfercfg;           /**< \brief 传输配置         */
    volatile uint32_t src_end_addr;      /**< \brief 源数据末地址     */
    volatile uint32_t dst_end_addr;      /**< \brief 目标数据末地址   */
    volatile uint32_t link;              /**< \brief 指向下一个描述符 */
} am_lpc82x_dma_xfer_desc_t;

/**
 * \name DMA 传输结束状态标志
 * \anchor am_lpc82x_dma_complete_cb_stat
 *
 *  用于DMA传输结束时，传入回调函数中的stat参数 
 * @{
 */

#define AM_LPC82X_DMA_STAT_INTA   AM_BIT(0) /**< \brief 中断A发生     */
#define AM_LPC82X_DMA_STAT_INTB   AM_BIT(1) /**< \brief 中断B发生     */
#define AM_LPC82X_DMA_STAT_INTERR AM_BIT(2) /**< \brief 错误中断发生  */

/** @} */

/**
 * \brief DMA传输结束回调函数定义
 *
 * \param[in] p_arg : 传输结构体传入的回调函数参数
 * \param[in] stat  : 传输结束的状态
 *      \arg AM_LPC82X_DMA_STAT_INTA   : 中断A发生
 *      \arg AM_LPC82X_DMA_STAT_INTB   : 中断B发生
 *      \arg AM_LPC82X_DMA_STAT_INTERR : 错误中断发生
 * 
 * \return 无
 *
 * \note #am_lpc82x_dma_transfer() 传输数据不会产生AM_LPC82X_DMA_STAT_INTB中断
 */
typedef void (*am_lpc82x_dma_complete_cb_t)(void *p_arg, int stat);


/** \brief DMA通道控制器
 *
 * 所有传输基于控制器 \n
 * 通过 #am_lpc82x_dma_controller_get()     初始化通道和获取控制器
 * 通过 #am_lpc82x_dma_controller_abort()   终止控制器数据传输
 * 通过 #am_lpc82x_dma_controller_release() 释放控制器
 */
typedef struct am_lpc82x_dma_controller {
    uint8_t                      chan;         /**< \brief 通道               */
    uint8_t                      state;        /**< \brief 控制器状态         */
    uint16_t                     ext_info;     /**< \brief 其他信息，内部使用 */
    am_lpc82x_dma_complete_cb_t  pfn_callback; /**< \brief 回调函数           */
    void                        *p_arg;        /**< \brief 回调函参数         */
} am_lpc82x_dma_controller_t;

/** \brief DMA设备信息 */
typedef struct am_lpc82x_dma_devinfo {
    
    /** \brief DMA寄存器块基址 */
    uint32_t    dma_regbase;
    
    /** \brief INMUX寄存器块基址 */
    uint32_t    inmux_regbase;
    
    /** \brief 指向控制器内存映射的指针 */
    uint8_t     *p_controller_map;

    /** \brief 指向控制器内存的指针 */
    am_lpc82x_dma_controller_t *p_controller;
    
    /** \brief 指向传输描述符基地址的指针，必须是512字节对齐 */
    am_lpc82x_dma_xfer_desc_t  *p_xfer_base;
    
    /** \brief 使用的通道数量   */
    uint8_t     chan_count;
    
    /** \brief DMA中断号 */
    uint8_t     inum;
    
    /** \brief 平台初始化函数 */
    void      (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void      (*pfn_plfm_deinit)(void);
    
} am_lpc82x_dma_devinfo_t;

/**
 * \brief DMA设备
 */
typedef struct am_lpc82x_dma_dev {
    
    /** \brief 指向DMA设备信息的指针 */
    const am_lpc82x_dma_devinfo_t *p_devinfo;
    
} am_lpc82x_dma_dev_t;


/**
 * \brief 获取通道控制器 
 *
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_* (#DMA_CHAN_0)
 * \param[in] opts : 配置参数，DMA_CHAN_OPT_*宏或多个 \n   
*                    \ref grp_dma_chan_opt_prio               : 优先级 \n
 *                   \ref grp_dma_chan_opt_periph_req_en      : 外设请求使能 \n
 *                   \ref grp_dma_chan_opt_hwtrig_mode        : 硬件触发条件 \n
 *                   \ref grp_dma_chan_opt_hwtrig_src         : 硬件触发源 \n
 *                   \ref grp_dma_chan_opt_hwtrig_burst_en    : 突发模式使能 \n
 *                   \ref grp_dma_chan_opt_hwtrig_burst_wrap  : 突发模式地址环绕 \n
 *                   \ref grp_dma_chan_opt_hwtrig_burst_count : 突发传输大小 \n
 *                   \ref grp_dma_chan_opt_trig_out           : 触发输出 \n
 *                   \see lpc82x_dma_chan.h
 * 
 * \return 指向通道控制器的指针
 *
 * \note 如果通道已经被使用且没有释放，或没有足够的资源使用，函数返回NULL
 */
am_lpc82x_dma_controller_t* am_lpc82x_dma_controller_get (uint8_t  chan,
                                                          uint32_t opts);

/**
 * \brief 终止控制器数据传输 
 *
 * \param[in] p_ctr : 指向通道控制器的指针
 *
 * \retval AM_OK      : 操作成功
 * \retval -AM_EINVAL : 参数无效
 * \retval -AM_ENXIO  : 设备不存在
 */
int am_lpc82x_dma_controller_abort (am_lpc82x_dma_controller_t *p_ctr);

/**
 * \brief 释放控制器 
 *
 * \param[in] p_ctr   : 指向通道控制器的指针
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_EINVAL : 参数无效
 * \retval -AM_EPERM  : 操作不允许(正在传输)
 * \retval -AM_ENXIO  : 设备不存在
 */
int am_lpc82x_dma_controller_release (am_lpc82x_dma_controller_t *p_ctr);

/**
 * \brief 建立transfer结构体 
 * 
 * \param[in] p_trans  : 指向传输结构体的指针 
 * \param[in] src_addr : 源端首地址
 * \param[in] dst_addr : 目标端首地址
 * \param[in] nbytes   : 传输字节数，注意要与“flags”的宽度对应
 * \param[in] flags    : 传输参数 参见：\ref grp_dma_trans_flags
 *
 * \retval  AM_OK      : 建立成功 
 * \retval -AM_EINVAL  : 参数无效
 */
int am_lpc82x_dma_transfer_build (am_lpc82x_dma_transfer_t *p_trans,
                                  uint32_t                  src_addr,
                                  uint32_t                  dst_addr,
                                  uint16_t                  nbytes,
                                  uint16_t                  flags);

/**
 * \brief 使用transfer方式传输数据
 *
 * 该函数会启动一次DMA传输，发送p_trans指向结构体描述的数据
 *
 * \param[in] p_ctr        : 指向通道控制器的指针
 * \param[in] p_trans      : 指向传输结构体的指针
 * \param[in] pfn_callback : 传输结束回调函数
 * \param[in] p_arg        : 回调函数参数
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_EINVAL : 参数无效
 * \retval -AM_ENXIO  : 设备不存在
 * \retval -AM_RBUSY  : 设备忙
 */
int am_lpc82x_dma_transfer (am_lpc82x_dma_controller_t  *p_ctr,
                            am_lpc82x_dma_transfer_t    *p_trans,
                            am_lpc82x_dma_complete_cb_t  pfn_callback,
                            void                        *p_arg);

/**
 * \brief 建立xfer_desc结构体 
 *
 * \attention 如使描述符需要自动重载，重载描述符的地址应该16字节对齐  
 * 
 * \param[in] p_desc   : 指向DMA传输描述符的指针 
 * \param[in] src_addr : 源端首地址 
 * \param[in] dst_addr : 目标端首地址 
 * \param[in] nbytes   : 传输字节数，注意要与“flags”的宽度对应 
 * \param[in] flags    : 配置参数，参见
 *                       \ref grp_am_lpc82x_dma_xfer_flags
 *
 * \retval  AM_OK      : 建立成功 
 * \retval -AM_EINVAL  : 参数无效
 */
int am_lpc82x_dma_xfer_desc_build (am_lpc82x_dma_xfer_desc_t *p_desc,
                                   uint32_t                   src_addr,
                                   uint32_t                   dst_addr,
                                   uint32_t                   nbytes,
                                   uint32_t                   flags);
                                   
/**
 * \brief 连接两个xfer_desc传输描述符 
 *
 * \param[in] p_desc : 指向当前传输描述符的指针
 * \param[in] p_next : 指向下一个传输描述符的指针，必须16字节对齐 
 *
 * \retval  AM_OK    : 连接成功 
 * \retval -AM_EINVL : 参数无效 
 */
int am_lpc82x_dma_xfer_desc_link (am_lpc82x_dma_xfer_desc_t *p_desc,
                                  am_lpc82x_dma_xfer_desc_t *p_next);

/**
 * \brief 使用xfer_describe方式传输数据
 *
 * 该函数会启动一次DMA传输，发送p_desc指向结构体描述的数据，如果p_desc指向一个链表，
 * DMA会自动重载链表中的传输描述符
 *
 * \param[in] p_ctr        : 指向通道控制器的指针
 * \param[in] p_desc       : 指向第一个传输描述符的指针
 * \param[in] pfn_callback : 传输结束回调函数
 * \param[in] p_arg        : 回调函数参数
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_EINVAL : 参数无效
 * \retval -AM_ENXIO  : 设备不存在
 * \retval -AM_RBUSY  : 设备忙
 */
int am_lpc82x_dma_xfer_desc_startup (am_lpc82x_dma_controller_t  *p_ctr,
                                     am_lpc82x_dma_xfer_desc_t   *p_desc,
                                     am_lpc82x_dma_complete_cb_t  pfn_callback,
                                     void                        *p_arg);

/**
 * \brief DMA初始化
 *
 * \param[in] p_dev     : 指向DMA设备的指针 
 * \param[in] p_devinfo : 指向DMA设备信息的指针 
 *
 * \retval  AM_OK        : 初始化成功 
 * \retval -AM_EINVLA    : 参数无效
 * \retval -AM_EPERM     : 操作不允许(设备已经初始化) 
 */
int am_lpc82x_dma_init (am_lpc82x_dma_dev_t           *p_dev,
                        const am_lpc82x_dma_devinfo_t *p_devinfo);


/**
 * \brief DMA去初始化 
 * \return  无
 */
void am_lpc82x_dma_deinit (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* __AM_LPC82X_DMA_H */

/* end of file */
