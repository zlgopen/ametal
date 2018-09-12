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
 * \brief LPC82x 在应用中编程(IAP)硬件操作接口
 *
 * IAP - 通过ROM的函数实现，提供对FLASH进行操作的一系列接口函数。
 * 接口函数主要包括以下几种：
 *     - 扇区写操作前的准备工作接口函数；
 *     - 从RAM复制内容到FLASH接口函数；
 *     - 扇区擦除接口函数；
 *     - 扇区空白检查接口函数；
 *     - 读取PartID接口函数；
 *     - 读取BOOT_VERSION接口函数；
 *     - 数据比较接口函数；
 *     - 重新进入ISP模式接口函数；
 *     - 读取UID接口函数；
 *     - 页擦除接口函数。
 *
 *
 * \internal
 * \par Modification history
 * - 1.02 15-12-05  cyl, modify the namespace. 
 * - 1.01 15-09-22  aii, check.
 * - 1.00 15-07-02  oce, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC82X_IAP_H
#define __AMHW_LPC82X_IAP_H

#include "ametal.h"
#include "am_lpc82x.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amhw_lpc82x_if_iap
 * \copydoc amhw_lpc82x_iap.h
 * @{
 */

/** IAP ROM API函数 */
typedef void (*amhw_lpc82x_iap_entry_t)(uint32_t[], uint32_t[]);

/**
 * \name IAP指令宏
 * @{
 */

/** \brief 扇区准备工作         */    
#define AMHW_LPC82X_IAP_CMD_PREPARE             50  
/** \brief 从RAM复制内容到FLASH */
#define AMHW_LPC82X_IAP_CMD_COPY_RAM_TO_FLASH   51  
/** \brief 扇区擦出命令         */
#define AMHW_LPC82X_IAP_CMD_ERASE_SECTOR        52  
/** \brief 扇区查空命令         */
#define AMHW_LPC82X_IAP_CMD_BLANK_CHECK         53  
/** \brief 读取PartID命令       */
#define AMHW_LPC82X_IAP_CMD_PARTID_READ         54  
/** \brief 读取BOOT_VERSION     */
#define AMHW_LPC82X_IAP_CMD_BOOT_VER_READ       55  
/** \brief 数据比较指令         */
#define AMHW_LPC82X_IAP_CMD_COMPARE             56  
/** \brief 重新进入ISP模式      */
#define AMHW_LPC82X_IAP_CMD_REINVOKE_ISP        57  
/** \brief 读UID                */
#define AMHW_LPC82X_IAP_CMD_READ_UID            58  
/** \brief 页擦除指令           */
#define AMHW_LPC82X_IAP_CMD_ERASE_PAGE          59  

/** @} */

/**
 * \brief IAP状态
 */
typedef enum amhw_lpc82x_iap_stat {
    
    /** \brief 指令执行成功          */
    AMHW_LPC82X_IAP_STAT_SUCCESS             = 0,  
    /** \brief 无效指令              */
    AMHW_LPC82X_IAP_STAT_INVALID_CMD         = 1,  
    /** \brief 源地址未按字对齐      */
    AMHW_LPC82X_IAP_STAT_SRC_ADDR_ERROR      = 2,  
    /** \brief 目标地址对齐错误      */
    AMHW_LPC82X_IAP_STAT_DST_ADDR_ERROR      = 3,  
    /** \brief 源地址不在内存映射中  */
    AMHW_LPC82X_IAP_STAT_SRC_ADDR_NOT_MAPPED = 4,  
    /** \brief 目标地址未在内存映射  */
    AMHW_LPC82X_IAP_STAT_DST_ADDR_NOT_MAPPED = 5,  
    /** \brief 字节个数值错误        */
    AMHW_LPC82X_IAP_STAT_COUNT_ERROR         = 6,  
    /** \brief 扇区值错误            */
    AMHW_LPC82X_IAP_STAT_INVALID_SECTOR      = 7,  
    /** \brief 扇区非空              */
    AMHW_LPC82X_IAP_STAT_SECTOR_NOT_BLANK    = 8,  
    /** \brief 扇区准备指令没有执行  */
    AMHW_LPC82X_IAP_STAT_SECTOR_NOT_PREPARED = 9,  
    /** \brief 源和目标地址数据不同  */
    AMHW_LPC82X_IAP_STAT_COMPARE_ERROR       = 10, 
    /** \brief FLASH忙状态           */
    AMHW_LPC82X_IAP_STAT_BUSY                = 11
    
} amhw_lpc82x_iap_stat_t;


/**
 * \brief 该函数用于直接调用ROM API函数
 *
 * \param[in]  cmd_param     : 指令相关数据
 * \param[out] status_result : 指令执行结果的相关数据
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_iap_entry (uint32_t cmd_param[], uint32_t status_result[])
{
    ((amhw_lpc82x_iap_entry_t) LPC82X_IAP_ENTRY_LOCATION)(cmd_param, 
                                                          status_result);
}

/**
 * \brief 写操作前的扇区准备函数
 *
 * \param[in] start_sec : 起始扇区号
 * \param[in] end_sec   : 结束扇区号
 *
 * \return IAP指令执行的状态
 *
 * \note 结束扇区号必须大于或等于起始扇区号
 *
 */
amhw_lpc82x_iap_stat_t amhw_lpc82x_iap_prepare (uint32_t start_sec, 
                                                uint32_t end_sec);

/**
 * \brief 复制RAM中数据至FLASH中，相当于FLASH写操作
 *
 * \param[in] dst_addr : 目标FLASH地址，应该以 256 字节对齐
 * \param[in] src_addr : 数据源RAM的地址，应该以字对齐
 * \param[in] nbytes   : 复制的数据个数, 应该是 256、512 、1024 或者 4096
 *
 * \return IAP指令执行的状态
 *
 * \note 该函数操作到的FLASH扇区必须在调用该函数前调用
 *        amhw_lpc82x_iap_prepare()函数进行写入准备操作
 */
amhw_lpc82x_iap_stat_t amhw_lpc82x_iap_copy (uint32_t dst_addr,
                                             uint32_t src_addr,
                                             uint32_t nbytes);

/**
 * \brief 扇区擦除
 *
 * \param[in] start_sec : 起始扇区号
 * \param[in] end_sec   : 结束扇区号
 *
 * \return IAP指令执行的状态
 *
 * \note 结束扇区号必须大于或等于起始扇区号，该函数操作到的FLASH扇区必须在调用该
 *       函数前调用amhw_lpc82x_iap_prepare()函数进行准备操作
 */
amhw_lpc82x_iap_stat_t amhw_lpc82x_iap_erase_sector (uint32_t start_sec,
                                                     uint32_t end_sec);

/**
 * \brief 页擦除，单页或多页擦除
 *
 * \param[in] start_page : 起始页号
 * \param[in] end_page   : 结束页号
 *
 * \return IAP指令执行的状态
 *
 * \note 结束页号必须大于或等于起始页号，该函数操作到的FLASH扇区必须在调用该
 *       函数前调用amhw_lpc82x_iap_prepare()函数进行准备操作
 */
amhw_lpc82x_iap_stat_t amhw_lpc82x_iap_erase_page (uint32_t start_page,
                                                   uint32_t end_page);

/**
 * \brief 单个扇区或多个扇区查空
 *
 * \param[in] start_sec : 起始扇区号
 * \param[in] end_sec   : 结束扇区号
 *
 * \return IAP指令执行的状态，执行如果成功(AMHW_LPC82X_IAP_STAT_SUCCESS)表明扇区为空。
 *
 * \note 结束扇区号必须大于或等于起始扇区号
 */
amhw_lpc82x_iap_stat_t amhw_lpc82x_iap_blank_check (uint32_t start_sec, 
                                                    uint32_t end_sec);

/**
 * \brief 读Part ID值
 * \return Part ID值
 */
uint32_t amhw_lpc82x_iap_part_id_read (void);

/**
 * \brief 读boot code版本号
 * \return boot code版本号, 版本号格式：<字节1(主版本号)>.<字节0(次版本号)>
 */
uint16_t amhw_lpc82x_iap_boot_ver_read (void);

/**
 * \brief 比较两处内存中的数据
 *
 * \param[in] dst_addr : 数据比较的数据目标地址，可以是FLASH地址或者RAM地址
 * \param[in] src_addr : 数据比较的数据源地址，可以是FLASH地址或者RAM地址
 * \param[in] nbytes   : 数据比较的个数
 *
 * \return IAP指令执行的状态
 *
 * \note 地址应该以字对齐，字节个数应该是4的整数倍
 */
amhw_lpc82x_iap_stat_t amhw_lpc82x_iap_compare (uint32_t dst_addr,
                                                uint32_t src_addr,
                                                uint32_t nbytes);

/**
 * \brief 通过IAP将 boot loader重新切换到ISP模式
 * \return 无
 */
void amhw_lpc82x_iap_reinvoke_isp (void);

/**
 * \brief 读器件的UID(共128位)
 * \param[out] uid :
 *              - uid[0] : 第一个32位数据 (最低地址，最低位0~31）
 *              - uid[1] : 第二个32位数据
 *              - uid[2] : 第三个32位数据
 *              - uid[3] : 第四个32位数据
 * \return IAP指令执行的状态
 */
amhw_lpc82x_iap_stat_t amhw_lpc82x_iap_unique_id_read (uint32_t uid[4]);

/**
 * @} amhw_if_iap
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AMHW_LPC82X_IAP_H */

/* end of files */
