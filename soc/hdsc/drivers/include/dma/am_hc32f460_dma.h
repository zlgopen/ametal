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
 * \brief DMA驱动，服务DMA接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-09-17  cds, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_DMA_H
#define __AM_HC32F460_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "hw/amhw_hc32f460_dma.h"
#include "hc32f460_inum.h"

/**
 * \addtogroup am_hc32f460_if_dma
 * \copydoc am_hc32f460_dma.h
 * @{
 */

/**
 * \name 通道传输中断标志
 * \anchor am_hc32f460_dma_chan_cfg_flags
 * @{
 */
#define AM_HC32F460_DMA_INT_DEFAULE           (0 << 0)  /** \brief 无中断*/
#define AM_HC32F460_DMA_INT_ERR_REQERR        (1 << 0)  /** \brief 传输请求溢出中断标识 */
#define AM_HC32F460_DMA_INT_ERR_TRNERR        (1 << 1)  /** \brief 传输错误中断标识 */
#define AM_HC32F460_DMA_INT_BLK_COMPLETE      (1 << 2)  /** \brief 块传输完成中断标识 */
#define AM_HC32F460_DMA_INT_COMPLETE          (1 << 3)  /** \brief 完成中断标识 */

/* 剩余传输次数计数器重置方式 */
typedef enum am_hc32f460_dma_rcfg_cntmd {
    AM_HC32F460_DMA_RFG_CNTMD_KEEP            = 0, /* 保持不变 */
	AM_HC32F460_DMA_RFG_CNTMD_BY_SRCADDR      = 1, /* 按源地址方式 */
	AM_HC32F460_DMA_RFG_CNTMD_KEEP_BY_DSRADDR = 2  /* 按目标地址方式 */
} am_hc32f460_dma_rfg_cntmd_t;

/* 目标地址重置方式 */
typedef enum am_hc32f460_dma_rcfg_darmd {
    AM_HC32F460_DMA_RFG_DARMD_KEEP        = 0, /* 保持不变 */
	AM_HC32F460_DMA_RFG_DARMD_NOSEQ       = 1, /* 不连续式重置 */
	AM_HC32F460_DMA_RFG_DARMD_KEEP_REPEAT = 2  /* 重复式重置 */
} am_hc32f460_dma_rcfg_darmd_t;

/* 源地址重置方式 */
typedef enum am_hc32f460_dma_rcfg_sarmd {
    AM_HC32F460_DMA_RFG_SARMD_KEEP        = 0, /* 保持不变 */
	AM_HC32F460_DMA_RFG_SARMD_NOSEQ       = 1, /* 不连续式重置 */
	AM_HC32F460_DMA_RFG_SARMD_KEEP_REPEAT = 2  /* 重复式重置 */
} am_hc32f460_dma_rcfg_sarmd_t;


/** @} */

/** \brief DMA中断回调函数类型 */
typedef void (*am_hc32f460_pfn_dma_isr_t)(void *p_arg, uint32_t flag);

/** \brief DMA中断回调函数信息 */
struct am_hc32f460_dma_int_info {

    /** \brief DMA触发回调函数 */
    am_hc32f460_pfn_dma_isr_t  pfn_isr;

    /** \brief 回调函数的第一个入口参数 */
    void                    *p_arg;
};

/** \brief DMA设备信息 */
typedef struct am_hc32f460_dma_devinfo {

    /** \brief 指向DMA寄存器块的指针 */
    uint32_t   dma_reg_base;

    /** \brief DMA中断号开始 */
    uint8_t    inum;

    /** \brief DMA设备ID */
    uint8_t    id;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_hc32f460_dma_devinfo_t;

/**
 * \brief DMA设备实例
 */
typedef struct am_hc32f460_dma_dev {

    /** \brief 指向DMA设备信息的指针 */
    const am_hc32f460_dma_devinfo_t *p_devinfo;

    /** \brief DMA中断回调函数信息数组 */
    struct am_hc32f460_dma_int_info __dma_int_info[AMHW_HC32F460_DMA_CHAN_CNT];

    /** \brief DMA中断回调函数信息映射 */
    uint8_t __dma_int_map[AMHW_HC32F460_DMA_CHAN_CNT];

}am_hc32f460_dma_dev_t;



/**
 * \brief 配置DMA传输通道
 *
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 * \param[in] src  : 设定通道请求的触发源（en_event_src_t）
 *
 */
void am_hc32f460_dma_chan_src_set (am_hc32f460_dma_dev_t *p_dev, int chan, en_event_src_t src);

/**
 * \brief 配置DMA传输通道
 *
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 * \param[in] src  : 设定通道请求的触发源（en_event_src_t）
 *
 * \retval AM_OK    : 配置成功
 * \retval AM_ERROR : 配置错误，该通道之前配置过
 */
int am_hc32f460_dma_chan_start (am_hc32f460_dma_dev_t *p_dev, int chan);

/**
 * \brief 停止通道传输
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 *
 * \retval AM_OK   : 操作成功
 */
int am_hc32f460_dma_chan_stop (am_hc32f460_dma_dev_t *p_dev, int chan);

/**
 * \brief 建立传输描述符
 *
 * \param[in] p_desc   : 指向DMA传输描述符的指针
 * \param[in] src_addr : 源端首地址
 * \param[in] dst_addr : 目标端首地址
 * \param[in] nbytes   : 传输字节数
 * \param[in] flags    : 配置参数
 *
 * \retval  AM_OK      : 建立成功
 * \retval -AM_EINVAL  : 未对齐，无效参数
 */
int am_hc32f460_dma_xfer_desc_build (amhw_hc32f460_dma_xfer_desc_t *p_desc,
                                     uint32_t                       src_addr,
                                     uint32_t                       dst_addr,
                                     uint32_t                       nbytes,
                                     uint32_t                       flags);

int am_hc32f460_dma_chain_xfer_desc_build (amhw_hc32f460_dma_chain_xfer_desc_t *p_desc,   /**< \brief 描述符结构体指针地址 */
                                           uint32_t                             src_addr, /**< \brief 源地址  */
                                           uint32_t                             dst_addr, /**< \brief 目的地址  */
                                           uint32_t                             count,    /**< \brief 传输次数  */
                                           uint32_t                             blk_size, /**< \brief 数据块大小  */
                                           uint32_t                             drpt,     /**< \brief 目标地址重复区域大小  */
                                           uint32_t                             srpt,     /**< \brief 源地址重复区域大小  */
                                           uint32_t                             snscnt,   /**< \brief 源地址跳转的数据量  */
                                           uint32_t                             soffset,  /**< \brief 源地址跳转的地址偏移量  */
                                           uint32_t                             dnscnt,   /**< \brief 目标地址跳转的数据量  */
                                           uint32_t                             dsoffset, /**< \brief 目标地址跳转的地址偏移量  */
                                           uint32_t                             link,     /**< \brief 下一次传输的描述符所在地址  */
                                           uint32_t                             ch_ctl);  /**< \brief 通道控制寄存器设置值  */

/** \brief DMA连锁传输配置 */
int am_hc32f460_dma_chain_xfer_desc_chan_cfg (am_hc32f460_dma_dev_t                  *p_dev,
                                              amhw_hc32f460_dma_chain_xfer_desc_t    *p_desc,
                                              amhw_hc32f460_dma_transfer_type_t       type,
                                              uint8_t                                 chan);

/**
 * \brief 开始DMA传输
 *
 * \param[in] p_dev   : 指向DMA设备的指针
 * \param[in] p_desc  : 指向DMA传输描述符的指针
 * \param[in] type    : DMA传输模式设置 ：有对应的枚举体dma_transfer_type_t
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_EINVAL : 参数无效
 */
int am_hc32f460_dma_xfer_desc_chan_cfg (am_hc32f460_dma_dev_t            *p_dev,
                                        amhw_hc32f460_dma_xfer_desc_t    *p_desc,
                                        amhw_hc32f460_dma_transfer_type_t type,
                                        uint8_t                           chan);

/**
 * \brief 连接DMA回调函数
 *
 * \param[in] p_dev   : 指向DMA设备的指针
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 * \param[in] pfn_isr : 回调函数指针
 * \param[in] p_arg   : 回调函数的第一个入口参数 ，在连接过程中，对应通道写上对应数字，
 *                      例如DMA0通道，则该参数为(void *)0
 *
 * \retval  AM_OK     : 连接成功
 * \retval -AM_EPERM  : 参数错误
 */
int am_hc32f460_dma_isr_connect (am_hc32f460_dma_dev_t    *p_dev,
                                 int                       chan,
                                 am_hc32f460_pfn_dma_isr_t pfn_isr,
                                 void                     *p_arg);

/**
 * \brief 删除DMA回调函数的连接
 *
 * \param[in] p_dev   : 指向DMA设备的指针
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 * \param[in] pfn_isr : 回调函数指针
 * \param[in] p_arg   : 回调函数的第一个入口参数，在连接过程中，对应通道写上对应数字，
 *                      例如DMA0通道，则该参数为(void *)0
 *
 * \retval  AM_OK     : 删除成功
 * \retval -AM_EPERM  : 参数错误
 */
int am_hc32f460_dma_isr_disconnect (am_hc32f460_dma_dev_t     *p_dev,
                                    int                        chan,
                                    am_hc32f460_pfn_dma_isr_t  pfn_isr,
                                    void                      *p_arg);


/**
 * \brief 设置中断屏蔽
 *
 * \param[in] p_dev    : 指向DMA设备的指针
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 * \param[in] int_flag : 中断标志
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_EINVAL : 参数无效
 */
void am_hc32f460_dma_chan_int_mask_set (am_hc32f460_dma_dev_t *p_dev,
                                        int                    chan,
                                        uint32_t               int_flag);

/**
 * \brief 清除中断屏蔽
 *
 * \param[in] p_dev    : 指向DMA设备的指针
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 * \param[in] int_flag : 中断标志
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_EINVAL : 参数无效
 */
void am_hc32f460_dma_chan_int_mask_clr (am_hc32f460_dma_dev_t *p_dev,
                                        int                    chan,
                                        uint32_t               int_flag);

/**
 * \breif 获取  某通道当前传输剩余的字节数
 *
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 *
 * \retval  字节数
 */
uint16_t am_hc32f460_dma_tran_data_get (am_hc32f460_dma_dev_t *p_dev, int chan);

/**
 * \breif 设置 某通道当前传输剩余的字节数
 *
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 * \param[in] trans_data_byte 要传输的字节数
 *
 * \retval  无
 *
 * \note 在设置传输的字节数之前必须停止通道传输
 */
void am_hc32f460_dma_tran_data_size (am_hc32f460_dma_dev_t *p_dev,
                                     int                    chan,
                                     uint32_t               trans_data_byte);

/**
 * \breif 设置每次传输时，块大小
 *
 * \param[in] chan : DMA 通道号，值为：DMA_CHAN_*(* 为1/2/3/4)
 * \param[in] block_data_num  块大小数量
 *
 * \retval  AM_OK     : 设置成功
 *          -AM_EINVAL: 参数错误
 *
 * \note 在设置传输的字节数之前必须停止通道传输
 */
int am_hc32f460_dma_block_data_size (am_hc32f460_dma_dev_t *p_dev,
                                     int                    chan,
                                     uint16_t               block_data_num);

/**
 * \breif 设定通道重置请求的触发源
 *
 * \param[in] src : 触发源
 *
 * \retval  无
 */
void am_hc32f460_dma_chan_rcfg_src_set (en_event_src_t src);

/**
 * \breif 通道重置控制寄存器配置
 *
 * \param[in] p_dev   指向DMA设备的指针
 * \param[in] cntmd   剩余传输次数计数器重置方式
 * \param[in] darmd   目标地址重置方式
 * \param[in] sarmd   源地址重置方式
 * \param[in] chan    重置通道选择
 * \param[in] rcfgllp 链指针式通道重置
 * \param[in] rcfgen  通道重置许可
 *
 * \retval  无
 *
 * \note 在DMA使能前设置该寄存器，该寄存器必须在重置通道的第一次传输前设置。
 */
void am_hc32f460_dma_chan_rcfg_set (am_hc32f460_dma_dev_t *p_dev,
                                    uint8_t                cntmd,
                                    uint8_t                darmd,
                                    uint8_t                sarmd,
                                    uint8_t                chan,
                                    uint8_t                rcfgllp,
                                    uint8_t                rcfgen);

/**
 * \brief 设置源地址重复区域大小
 *        源设备在每传输size个数据后，源地址重载为DMA_SARx寄存器的值。
 *
 * \param[in] p_dev  指向DMA设备的指针
 * \param[in] chan : 通道号
 * \param[in] size : 源地址重复区域大小
 *
 * \retval  无
 */
int am_hc32f460_dma_srpt_size_set (am_hc32f460_dma_dev_t *p_dev,
                                   int                    chan,
                                   uint16_t               size);
/**
 * \brief 设置目标地址重复区域大小
 *        源设备在每传输size个数据后，目标地址重载为DMA_DARx寄存器的值。
 *
 * \param[in] p_dev  指向DMA设备的指针
 * \param[in] chan : 通道号
 * \param[in] size : 源地址重复区域大小
 *
 * \retval  无
 */
int am_hc32f460_dma_drpt_size_set (am_hc32f460_dma_dev_t *p_dev, int chan, uint16_t size);

/**
 * \brief 源设备不连续地址传输配置
 *
 * \param[in] p_dev   : 指向DMA设备的指针
 * \param[in] chan    : 通道号
 * \param[in] snscnt  : 源地址跳转的数据量
 * \param[in] soffset : 源地址跳转的地址偏移量
 *
 * \retval  无
 */
int am_hc32f460_dma_snseqctl_cfg (am_hc32f460_dma_dev_t *p_dev,
                                  int                    chan,
                                  uint16_t               snscnt,
                                  uint16_t               soffset);

/**
 * \brief 目标设备不连续地址传输配置
 *
 * \param[in] p_dev   : 指向DMA设备的指针
 * \param[in] chan    : 通道号
 * \param[in] dnscnt  : 目标地址跳转的数据量
 * \param[in] doffset : 目标地址跳转的地址偏移量
 *
 * \retval  无
 */
int am_hc32f460_dma_dnseqctl_cfg (am_hc32f460_dma_dev_t *p_dev,
                                  int                    chan,
                                  uint16_t               dnscnt,
                                  uint16_t               doffset);

/**
 * \brief DMA软件触发
 */
void am_hc32f460_dma_soft_trig(void);


/**
 * \brief DMA初始化
 *
 * \param[in] p_dev         : 指向DMA设备的指针
 * \param[in] p_dma_devinfo : 指向DMA设备信息的指针
 *
 * \retval  AM_OK       : 初始化成功
 */
int am_hc32f460_dma_init (am_hc32f460_dma_dev_t           *p_dev,
                          const am_hc32f460_dma_devinfo_t *p_dma_devinfo);

/**
 * \brief DMA去初始化
 *
 * \param[in] p_dev      : 指向DMA设备的指针
 *
 * \return 无
 */
void am_hc32f460_dma_deinit (am_hc32f460_dma_dev_t *p_dev);

/**
 * \brief DMA中断函数
 *
 * \param[in] p_arg : 中断函数参数
 *
 * \return 无
 */
void dma_int_handler(void *p_arg);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_DMA_H */

/* end of file */
