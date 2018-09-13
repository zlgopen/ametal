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
 * \brief CAN hardware operation interface
 *
 * -note:在对同一寄存器进行读写时其所对应的功能不同
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-05  zcb, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZLG217_CAN_H
#define __AMHW_ZLG217_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_zlg217_if_can
 * \copydoc amhw_zlg217_can.h
 * @{
 */

/**
 * \brief ZLG217 CAN寄存器块结构体
 */
typedef struct amhw_zlg217_can {

    union {
        __IO  uint32_t cr;              /**< \brief 0x00 basic 控制寄存器 */
        __IO  uint32_t mod;             /**< \brief 0x00 peli 模式寄存器 */
    }mode;

    __IO  uint32_t cmr;                 /**< \brief 0x04 命令寄存器 */
    __IO  uint32_t sr;                  /**< \brief 0x08 状态寄存器 */
    __I   uint32_t ir;                  /**< \brief 0x0c 中断寄存器 */

    union {

        struct {
            __IO  uint32_t acr;         /**< \brief 0x10 验收代码寄存器 */
            __IO  uint32_t amr;         /**< \brief 0x14 验收屏蔽寄存器 */
        }basic;

        struct {
            __IO  uint32_t ier;         /**< \brief 0x10 中断使能寄存器 */
                  uint32_t reserve0;
        }peli;

    }accept;

    __IO  uint32_t btr0;                /**< \brief 0x18 总线定时器0 */
    __IO  uint32_t btr1;                /**< \brief 0x1c 总线定时器1 */

    union {

        struct {
                  uint32_t reserve1[2]; /**< \brief 保留 */
            __IO  uint32_t txid0;       /**< \brief 0x28 id10 ~ 3 */
            __IO  uint32_t txid1;       /**< \brief 0x2c id2 ~ 0/rtr/dlc0-4 */
            __IO  uint32_t txdr[8];     /**< \brief 0x30 DATA1 - 8 */
            __IO  uint32_t rxid0;       /**< \brief 0x50 id10 ~ 3 */
            __IO  uint32_t rxid1;       /**< \brief 0x54 id2 ~ 0/rtr/dlc0-4 */
            __IO  uint32_t rxdr[8];     /**< \brief 0x58 DATA1 - 8 */
                  uint32_t reserve2;    /**< \brief 0x78 保留 */
        }basic;

        struct {
                  uint32_t reserve1[3]; /**< \brief 保留 */
            __IO  uint32_t alc;         /**< \brief 0x2c 总裁丢失捕捉寄存器 */
            __IO  uint32_t ecc;         /**< \brief 0x30 错误代码捕捉寄存器 */
            __IO  uint32_t ewlr;        /**< \brief 0x34 错误报警限制寄存器 */
            __IO  uint32_t rxerr;       /**< \brief 0x38 RX错误计数寄存器 */
            __IO  uint32_t txerr;       /**< \brief 0x3c TX错误计数寄存器 */
            __IO  uint32_t sff;         /**< \brief 0x40 RX/TX帧信息寄存器 */
            __IO  uint32_t id0;         /**< \brief 0x44 RX/TX识别码1 */
            __IO  uint32_t id1;         /**< \brief 0x48 RX/TX识别码2 */
            __IO  uint32_t data[10];    /**< \brief 0x4c RX/TX数据 */
            __IO  uint32_t rmc;         /**< \brief 0x74 RX信息计数器 */
            __IO  uint32_t rbsa;        /**< \brief 0x78 RX缓冲期起始地址 */
        }peli;

    }info;

    __IO  uint32_t cdr;                 /**< \brief 0x7c 时钟分频器 */

}amhw_zlg217_can_t;

/**
 * \brief CAN模式定义
 */
typedef enum amhw_zlg217_can_type {
    AMHW_ZLG217_CAN_BASIC_CAN = 0,        /**< \brief basic can */
    AMHW_ZLG217_CAN_PELI_CAN,             /**< \brief peli can */
}amhw_zlg217_can_type_t;

/**
 * \brief CAN功能定义
 *
 * \note basic can只知有复位模式、工作模式、睡眠模式
 *
 */
typedef enum amhw_zlg217_can_mode_func {
    AMHW_ZLG217_CAN_MODE_RESET = 0,       /**< \brief 复位模式 */
    AMHW_ZLG217_CAN_MODE_ONLY_LISTEN,     /**< \brief 只听模式 */
    AMHW_ZLG217_CAN_MODE_SELF_CHECK,      /**< \brief 自检模式 */
    AMHW_ZLG217_CAN_MODE_SLEEP = 4,       /**< \brief 睡眠模式 */
    AMHW_ZLG217_CAN_MODE_RUN,             /**< \brief 工作模式 */
}amhw_zlg217_can_mode_t;

/**
 * \brief CAN数据接收方式
 */
typedef enum amhw_zlg217_can_recv_mode {
    AMHW_ZLG217_CAN_RECV_POLL = 0,        /**< \brief 查询接收模式 */
    AMHW_ZLG217_CAN_RECV_INT,             /**< \brief 中断接收模式 */
}amhw_zlg217_can_recv_mode_t;

/**
 * \brief peli CAN滤波方式
 */
typedef enum amhw_zlg217_can_peli_filter_mode {
    AMHW_ZLG217_CAN_PELI_SINGLE_FILTER,   /**< \brief peli can 单滤波器模式 */
    AMHW_ZLG217_CAN_PELI_DOUBLE_FLITER,   /**< \brief peli can 双滤波器模式 */
}amhw_zlg217_can_peli_filter_mode_t;

/******************************************************************************/

/**
 * \brief CAN中断
 *
 * \note basic can只支持前5种中断，其中唤醒中断没有使能和禁能位，只能使能或者禁能前4种中断
 *
 * @{
 */

#define AMHW_ZLG217_CAN_INT_NUM              (8ul)

#define AMHW_ZLG217_CAN_INT_BUS_ERR          (0x1 << 7ul)  /**< \brief 总线错误中断 */
#define AMHW_ZLG217_CAN_INT_ARBITRATION_LOST (0x1 << 6ul)  /**< \brief 仲裁丢失中断 */
#define AMHW_ZLG217_CAN_INT_ERR_PASSIVE      (0x1 << 5ul)  /**< \brief 错误消极中断 */
#define AMHW_ZLG217_CAN_INT_WAKEUP           (0x1 << 4ul)  /**< \brief 唤醒中断 */
#define AMHW_ZLG217_CAN_INT_OVER             (0x1 << 3ul)  /**< \brief 数据溢出中断 */
#define AMHW_ZLG217_CAN_INT_ERR              (0x1 << 2ul)  /**< \brief 错误中断 */
#define AMHW_ZLG217_CAN_INT_TRAN             (0x1 << 1ul)  /**< \brief 发送中断 */
#define AMHW_ZLG217_CAN_INT_RECV             (0x1 << 0ul)  /**< \brief 接收中断 */

/**
 * @}
 */

/**
 * \brief can中断使能
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] type       : can类型，枚举amhw_zlg217_can_mode_t定义
 * \param[in] flag       : 中断标志，参考宏定义AMHW_ZLG217_CAN_INT_
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_can_int_enable (amhw_zlg217_can_t      *p_hw_can,
                                 amhw_zlg217_can_type_t  type,
                                 uint32_t                flag)
{
    if (type == AMHW_ZLG217_CAN_BASIC_CAN) {
        p_hw_can->mode.cr |= (flag << 1);
    } else if (type == AMHW_ZLG217_CAN_PELI_CAN) {
        p_hw_can->accept.peli.ier |= (flag);
    }
}

/**
 * \brief can中断禁能
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] type       : can类型，枚举amhw_zlg217_can_mode_t定义
 * \param[in] flag       : 中断标志，参考宏定义AMHW_ZLG217_CAN_INT_
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_can_int_disable (amhw_zlg217_can_t      *p_hw_can,
                                  amhw_zlg217_can_type_t  type,
                                  uint32_t                flag)
{
    if (type == AMHW_ZLG217_CAN_BASIC_CAN) {
        p_hw_can->mode.cr &= ~(flag << 1);
    } else if (type == AMHW_ZLG217_CAN_PELI_CAN) {
        p_hw_can->accept.peli.ier &= ~(flag);
    }
}

/**
 * \brief can中断标志读取，判断中断触发源
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 触发中断的中断源
 */
am_static_inline
uint8_t amhw_zlg217_can_int_status_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->ir & 0xff);
}

/******************************************************************************/

/**
 * \brief can 模式设置
 *
 * \param[in] p_hw_can 指向CAN寄存器块的指针
 * \param[in] type     can模式，枚举amhw_zlg217_can_type_t定义
 * \param[in] p_now    当前模式
 * \param[in] update   待设置模式
 *
 * \return 无
 *
 * \note basic can模式只支持复位、正常运行、睡眠三种模式
 *
 */
void amhw_zlg217_can_mode_set (amhw_zlg217_can_t      *p_hw_can,
                               amhw_zlg217_can_type_t  type,
                               amhw_zlg217_can_mode_t *p_now,
                               amhw_zlg217_can_mode_t  update);

/******************************************************************************/

/**
 * \brief 命令寄存器相关位定义
 *
 * \note 命令寄存器在basic can中读出全为0xff,在peli can中读出全为0x00
 *
 * @{
 */

#define AMHW_ZLG217_CAN_CMR_MASK     (0x1F)

#define AMHW_ZLG217_CAN_CMR_GTS      (4ul)  /**< \brief 睡眠(basic) */
#define AMHW_ZLG217_CAN_CMR_SRR      (4ul)  /**< \brief 自接收请求(peli) */
#define AMHW_ZLG217_CAN_CMR_CDO      (3ul)  /**< \brief 清除数据溢出 */
#define AMHW_ZLG217_CAN_CMR_RRB      (2ul)  /**< \brief 释放接收缓冲器 */
#define AMHW_ZLG217_CAN_CMR_AT       (1ul)  /**< \brief 终止传输 */
#define AMHW_ZLG217_CAN_CMR_TR       (0ul)  /**< \brief 发送请求 */

/**
 * @}
 */

/**
 * \brief 写入can命令
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] cmr        : 命令相关宏定义，参考AMHW_ZLG217_CAN_CMR_
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_can_cmr_set (amhw_zlg217_can_t *p_hw_can, uint32_t cmr)
{
    p_hw_can->cmr |= (0x1 << cmr) & AMHW_ZLG217_CAN_CMR_MASK;
}

/**
 * \brief basic CAN进入睡眠模式
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_basic_can_go_to_sleep (amhw_zlg217_can_t *p_hw_can)
{
    p_hw_can->cmr |= (0x1 << AMHW_ZLG217_CAN_CMR_GTS);
}

/**
 * \brief basic CAN唤醒
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_basic_can_wake_up (amhw_zlg217_can_t *p_hw_can)
{
    p_hw_can->cmr &= ~(0x1 << AMHW_ZLG217_CAN_CMR_GTS);
}
/******************************************************************************/

/**
 * \brief 状态寄存器相关位定义
 * @{
 */

#define AMHW_ZLG217_CAN_SR_MASK         (0xFF)

#define AMHW_ZLG217_CAN_SR_BS           (0x1 << 7ul)  /**< \brief 总线状态位 */
#define AMHW_ZLG217_CAN_BUS_CLOSE       (1ul)         /**< \brief 总线关闭 */
#define AMHW_ZLG217_CAN_BUS_OPEN        (0ul)         /**< \brief 总线开启 */

#define AMHW_ZLG217_CAN_SR_ES           (0x1 << 6ul)  /**< \brief 出错状态位 */
#define AMHW_ZLG217_CAN_ERROR           (1ul)         /**< \brief 出错 */
#define AMHW_ZLG217_CAN_OK              (0ul)         /**< \brief OK */

#define AMHW_ZLG217_CAN_SR_TS           (0x1 << 5ul)  /**< \brief 发送状态位 */
#define AMHW_ZLG217_CAN_TRANS_RUN       (1ul)         /**< \brief 发送 */
#define AMHW_ZLG217_CAN_TRANS_IDLE      (0ul)         /**< \brief 空闲 */

#define AMHW_ZLG217_CAN_SR_RS           (0x1 << 4ul)  /**< \brief 接收状态位 */
#define AMHW_ZLG217_CAN_RECV_RUN        (1ul)         /**< \brief 接收 */
#define AMHW_ZLG217_CAN_RECV_IDLE       (0ul)         /**< \brief 空闲 */

#define AMHW_ZLG217_CAN_SR_TCS          (0x1 << 3ul)  /**< \brief 发送完毕状态位 */
#define AMHW_ZLG217_CAN_TRANS_FINISH    (1ul)         /**< \brief 发送完成 */
#define AMHW_ZLG217_CAN_TRANS_FINISH_NO (0ul)         /**< \brief 发送未完成 */

#define AMHW_ZLG217_CAN_SR_TBS          (0x1 << 2ul)  /**< \brief 发送缓冲器状态位 */
#define AMHW_ZLG217_CAN_TX_BUF_UNLOCK   (1ul)         /**< \brief 发送缓冲器释放 */
#define AMHW_ZLG217_CAN_TX_BUF_LOCK     (0ul)         /**< \brief 发送缓冲器锁定 */

#define AMHW_ZLG217_CAN_SR_DOS          (0x1 << 1ul)  /**< \brief 数据溢出状态位 */
#define AMHW_ZLG217_CAN_DATA_OVER       (1ul)         /**< \brief 数据溢出 */
#define AMHW_ZLG217_CAN_DATA_OVER_NO    (0ul)         /**< \brief 数据溢出空缺 */

#define AMHW_ZLG217_CAN_SR_RBS          (0x1 << 0ul)  /**< \brief 接收缓冲器状态 */
#define AMHW_ZLG217_CAN_RX_BUF_FULL     (1ul)         /**< \brief 满 */
#define AMHW_ZLG217_CAN_RX_BUF_EMPTY    (0ul)         /**< \brief 空 */

/**
 * @}
 */

/**
 * \brief 获取CAN的状态
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return CAN的相关状态
 */
am_static_inline
uint32_t amhw_zlg217_can_status_get (amhw_zlg217_can_t *p_hw_can)
{
    return (p_hw_can->sr & AMHW_ZLG217_CAN_SR_MASK);
}

/******************************************************************************/

/**
 * \brief 波特率预设值设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] brp        : 波特率预设值，6bit宽度
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_can_brp_set (amhw_zlg217_can_t *p_hw_can, uint8_t brp)
{
    p_hw_can->btr0 = (p_hw_can->btr0 & (~0x3f)) | (brp & 0x3f);
}

/**
 * \brief 获取波特率预设值
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 波特率预设值，6bit
 */
am_static_inline
uint8_t amhw_zlg217_can_brp_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->btr0 & 0x3f);
}

/**
 * \brief 同步跳转宽度设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] sjw        : 同步跳转宽度值，2bit宽度
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_can_sjw_set (amhw_zlg217_can_t *p_hw_can, uint8_t sjw)
{
    p_hw_can->btr0 = (p_hw_can->btr0 & (~(0x3 << 6))) | ((sjw & 0x3) << 6);
}

/**
 * \brief 获取同步跳转宽度
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 同步跳转宽度，2bit
 */
am_static_inline
uint8_t amhw_zlg217_can_sjw_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)((p_hw_can->btr0 >> 6) & 0x3);
}

/**
 * \brief 采样模式设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] sam        : 采样模式值，1bit宽度，1 总线采样三次
 *                                         0 总线采样一次
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_can_sam_set (amhw_zlg217_can_t *p_hw_can, uint8_t sam)
{
    p_hw_can->btr1 = (p_hw_can->btr1 & (~(0x1 << 7))) | ((sam & 0x1) << 7);
}

/**
 * \brief 获取采样模式
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \retval    0 总线采样一次
 *            1 总线采样三次
 */
am_static_inline
uint8_t amhw_zlg217_can_sam_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)((p_hw_can->btr1 >> 7) & 0x1);
}

/**
 * \brief 时间段1设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] tseg1        : 采样模式值，4bit宽度
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_can_tseg1_set (amhw_zlg217_can_t *p_hw_can, uint8_t tseg1)
{
    p_hw_can->btr1 = (p_hw_can->btr1 & (~0xf)) | (tseg1 & 0xf);
}

/**
 * \brief 获取时间段1
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 时间段1，4bit
 */
am_static_inline
uint8_t amhw_zlg217_can_tseg1_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->btr1 & 0xf);
}

/**
 * \brief 时间段2设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] tseg2        : 采样模式值，3bit宽度
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_can_tseg2_set (amhw_zlg217_can_t *p_hw_can, uint8_t tseg2)
{
    p_hw_can->btr1 = (p_hw_can->btr1 & (~(0x7 << 4))) | ((tseg2 & 0x7) << 4);
}

/**
 * \brief 时间段2设置
 *
 * \param[in] p_hw_can 指向CAN寄存器块的指针
 *
 * \return 无
 */
am_static_inline
uint8_t amhw_zlg217_can_tseg2_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)((p_hw_can->btr1 >> 4) & 0x7);
}

/******************************************************************************/

/**
 * \brief basicCAN复位请求
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_basic_can_reset_set (amhw_zlg217_can_t *p_hw_can)
{
    p_hw_can->mode.cr |= 0x1;
}

/**
 * \brief basicCAN复位请求清除
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_basic_can_reset_clr (amhw_zlg217_can_t *p_hw_can)
{
    p_hw_can->mode.cr &= ~0x1;
}

/**
 * \brief 获取basicCAN复位标志
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 无
 */
am_static_inline
am_bool_t amhw_zlg217_basic_can_reset_get (amhw_zlg217_can_t *p_hw_can)
{
    return ((p_hw_can->mode.cr & 0x1) ? AM_TRUE : AM_FALSE);
}

/**
 * \brief basicCAN验收代码设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] ac         : 验收代码
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_basic_can_ac_set (amhw_zlg217_can_t *p_hw_can, uint8_t ac)
{
    p_hw_can->accept.basic.acr = ac;
}

/**
 * \brief 获取basicCAN验收代码
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 验收代码
 */
am_static_inline
uint8_t amhw_zlg217_basic_can_ac_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->accept.basic.acr & 0xff);
}

/**
 * \brief basicCAN验收屏蔽值设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] am         : 验收屏蔽值
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_basic_can_am_set (amhw_zlg217_can_t *p_hw_can, uint8_t am)
{
    p_hw_can->accept.basic.amr = am;
}

/**
 * \brief 获取basicCAN验收屏蔽值
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 验收屏蔽值
 */
am_static_inline
uint8_t amhw_zlg217_basic_can_am_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->accept.basic.amr & 0xff);
}

/**
 * \brief basicCAN写11位发送标识符
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] id         : 标识符11bit
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_basic_can_tx_id_write (amhw_zlg217_can_t *p_hw_can,
                                        uint16_t           id)
{
    p_hw_can->info.basic.txid0 = (id >> 3) & 0xff;
    p_hw_can->info.basic.txid1 = (p_hw_can->info.basic.txid1 &
                                              (~(0x7 << 5))) |
                                              ((id & 0x7) << 5);
}

/**
 * \brief basicCAN远程标志位RTR写入
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] rtr        : rtr值，0/1
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_basic_can_tx_rtr_write (amhw_zlg217_can_t *p_hw_can,
                                         uint8_t            rtr)
{
    p_hw_can->info.basic.txid1 = (p_hw_can->info.basic.txid1 &
                                              (~(0x1 << 4))) |
                                              ((rtr & 0x1) << 4);
}

/**
 * \brief basicCAN写发送数据长度
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] len        : 数据长度0-8bytes
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_basic_can_tx_data_len_set (amhw_zlg217_can_t *p_hw_can,
                                            uint8_t            len)
{
    p_hw_can->info.basic.txid1 = (p_hw_can->info.basic.txid1 & (~0xf)) |
                                                           (len & 0xf) ;
}

/**
 * \brief basicCAN写发送数据
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] offset     : 数据偏移0-8
 * \param[in] val        : 数据值
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_basic_can_tx_data_write (amhw_zlg217_can_t *p_hw_can,
                                          uint8_t            offset,
                                          uint8_t            val)
{
    p_hw_can->info.basic.txdr[offset] = val & 0xff;
}

/**
 * \brief basicCAN读11位接收标识符
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 标识符
 */
am_static_inline
uint16_t amhw_zlg217_basic_can_rx_id_read (amhw_zlg217_can_t *p_hw_can)
{
    return (uint16_t)((p_hw_can->info.basic.rxid0 & 0xff) << 3) |
                     ((p_hw_can->info.basic.rxid1 & 0xff) >> 5) ;
}

/**
 * \brief basicCAN读远程标志位RTR
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return rtr值
 */
am_static_inline
uint8_t amhw_zlg217_basic_can_rx_rtr_read (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)((p_hw_can->info.basic.rxid1 >> 4) & 0x1);
}

/**
 * \brief basicCAN读接收数据长度
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 数据长度0-8bytes
 */
am_static_inline
uint8_t amhw_zlg217_basic_can_rx_data_len_read (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->info.basic.rxid1 & 0xf);
}

/**
 * \brief basicCAN读接收的数据
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] offset     : 数据偏移0-8
 *
 * \return 数据值
 */
am_static_inline
uint8_t amhw_zlg217_basic_can_rx_data_read (amhw_zlg217_can_t *p_hw_can,
                                            uint8_t            offset)
{
    return (uint8_t)(p_hw_can->info.basic.rxdr[offset] & 0xff);
}

/******************************************************************************/

/**
 * \brief 模式寄存器相关位定义
 * @{
 */

#define AMHW_ZLG217_CAN_PELI_MODE_SM       (4ul)  /**< \brief 睡眠模式 */
#define AMHW_ZLG217_CAN_PELI_MODE_AFM      (3ul)  /**< \brief 验收滤波器模式 */
#define AMHW_ZLG217_CAN_PELI_MODE_STM      (2ul)  /**< \brief 自检测模式 */
#define AMHW_ZLG217_CAN_PELI_MODE_LOM      (1ul)  /**< \brief 只听模式 */
#define AMHW_ZLG217_CAN_PELI_MODE_RM       (0ul)  /**< \brief 复位模式 */

/**
 * @}
 */

/**
 * \brief peli CAN模式设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] flag       : 参考AMHW_ZLG217_CAN_PELI_MODE_
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_peli_can_mod_set (amhw_zlg217_can_t *p_hw_can,
                                   uint32_t           flag)
{
    p_hw_can->mode.mod |= (0x1 << flag);
}

/**
 * \brief peli CAN模式清除
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] flag       : 参考AMHW_ZLG217_CAN_PELI_MODE_
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_peli_can_mod_clr (amhw_zlg217_can_t *p_hw_can,
                                   uint32_t           flag)
{
    p_hw_can->mode.mod &= ~(0x1 << flag);
}

/**
 * \brief peli CAN模式获取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 模式
 */
am_static_inline
uint8_t amhw_zlg217_peli_can_mod_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)((p_hw_can->mode.mod & 0x1f) &
                            ~(0x1 << AMHW_ZLG217_CAN_PELI_MODE_AFM));
}

/**
 * \brief peli CAN滤波器模式设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] mode       : 参考枚举amhw_zlg217_can_peli_filter_mode_t
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_peli_filter_mod_set (amhw_zlg217_can_t *p_hw_can,
                           amhw_zlg217_can_peli_filter_mode_t mode)
{
    if (AMHW_ZLG217_CAN_PELI_SINGLE_FILTER == mode) {
        p_hw_can->mode.mod |= 0x1 << AMHW_ZLG217_CAN_PELI_MODE_AFM;
    } else if (AMHW_ZLG217_CAN_PELI_DOUBLE_FLITER == mode) {
        p_hw_can->mode.mod &= ~(0x1 << AMHW_ZLG217_CAN_PELI_MODE_AFM);
    }
}

/**
 * \brief 获取peli CAN滤波器模式设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 参考枚举amhw_zlg217_can_peli_filter_mode_t
 */
am_static_inline amhw_zlg217_can_peli_filter_mode_t
amhw_zlg217_peli_filter_mod_get (amhw_zlg217_can_t *p_hw_can)
{
    return (p_hw_can->mode.mod & (0x1 << AMHW_ZLG217_CAN_PELI_MODE_AFM) ?
                                     AMHW_ZLG217_CAN_PELI_SINGLE_FILTER :
                                     AMHW_ZLG217_CAN_PELI_DOUBLE_FLITER);
}

/**
 * \brief peli CAN 仲裁丢失捕捉寄存器值获取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 寄存器值,值对应如下
 *
 * \note 1. 0 - 11表示仲裁丢失在识别码bit1 - bit11
 *       2. 11表示仲裁丢失在SRTR位
 *       3. 12表示仲裁丢失在IDE位
 *       4. 13 - 30表示仲裁丢失在识别码bit12 - bit29
 *       5. 31表示仲裁丢失在RTR位
 */
am_static_inline
uint8_t amhw_zlg217_peli_can_alc_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->info.peli.alc & 0x1f);
}

/**
 * \brief peli CAN错误代号
 * @{
 */

#define AMHW_ZLG217_CAN_PELI_ECC_ERRC_MASK    (0x3)  /**< \brief 错误代码掩码 */
#define AMHW_ZLG217_CAN_PELI_ECC_ERRC_SHIFT   (6ul)  /**< \brief 错误代码起始位 */
#define AMHW_ZLG217_CAN_PELI_ECC_ERRC_BIT     (0ul)  /**< \brief 位错误 */
#define AMHW_ZLG217_CAN_PELI_ECC_ERRC_MODE    (1ul)  /**< \brief 格式错误 */
#define AMHW_ZLG217_CAN_PELI_ECC_ERRC_FILL    (2ul)  /**< \brief 填充错误 */
#define AMHW_ZLG217_CAN_PELI_ECC_ERRC_OTHER   (3ul)  /**< \brief 其他错误 */

/** \brief 1:RX;0:TX */
#define AMHW_ZLG217_CAN_PELI_ECC_DIR_RX             (0x1 << 3ul)

#define AMHW_ZLG217_CAN_PELI_ECC_CODE_MASK          (0x1f)
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_SHIFT         (0ul)

/** \brief ID.28-ID.21 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_ID28_21       (2ul)
/** \brief 帧开始 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_FRAME_STRAT   (3ul)
/** \brief SRTR位 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_SRTR          (4ul)
/** \brief IDE位 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_IDE           (5ul)
/** \brief ID.20-ID.18 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_ID20_18       (6ul)
/** \brief ID.17-ID.13 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_ID17_13       (7ul)
/** \brief CRC序列 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_CRC_ARRAY     (8ul)
/** \brief 保留位0 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_RESERVE0      (9ul)
/** \brief 数据区 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_DATA          (10ul)
/** \brief 数据长度代码 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_DATA_LEN      (11ul)
/** \brief RTR位 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_RTR           (12ul)
/** \brief 保留位1 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_RESERVR1      (13ul)
/** \brief ID.4-ID.0 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_ID4_0         (14ul)
/** \brief ID.12-ID.5 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_ID12_5        (15ul)
/** \brief 活动错误标志 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_ACTIVITY_ERR  (17ul)
/** \brief 中止 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_BREAK_OFF     (18ul)
/** \brief 支配（控制）位误差 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_CR_BIT        (19ul)
/** \brief 消极错误标志 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_PASSIVE       (22ul)
/** \brief 错误定义符 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_ERR           (23ul)
/** \brief CRC定义符 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_CRC           (24ul)
/** \brief 应答通道 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_ACK_CH        (25ul)
/** \brief 帧结束 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_FRAME_END     (26ul)
/** \brief 应答定义符 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_ACK           (27ul)
/** \brief 溢出标志 */
#define AMHW_ZLG217_CAN_PELI_ECC_CODE_OVERFLOW      (28ul)

/**
 * @}
 */

/**
 * \brief peli CAN 错误代码捕捉寄存器值获取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 寄存器值
 */
am_static_inline
uint8_t amhw_zlg217_peli_can_ecc_get (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->info.peli.ecc & 0xff);
}

/**
 * \brief peli CAN 错误报警限制寄存器写入
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] val        : 限制值
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_peli_can_ewlr_write (amhw_zlg217_can_t *p_hw_can,
                                      uint8_t            val)
{
    p_hw_can->info.peli.ewlr = val & 0xff;
}

/**
 * \brief peli CAN 错误报警限制寄存器读取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 限制值
 */
am_static_inline
uint8_t  amhw_zlg217_peli_can_ewlr_read (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->info.peli.ewlr & 0xff);
}

/**
 * \brief peli CAN RX错误计数寄存器写入
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] init       : 初始值
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_peli_can_rxerr_write (amhw_zlg217_can_t *p_hw_can,
                                       uint8_t            init)
{
    p_hw_can->info.peli.rxerr = init & 0xff;
}

/**
 * \brief peli CAN RX错误计数寄存器读取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 错误计数器的值
 */
am_static_inline
uint8_t  amhw_zlg217_peli_can_rxerr_read (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->info.peli.rxerr & 0xff);
}

/**
 * \brief peli CAN TX错误计数寄存器写入
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] init       : 初始值
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_peli_can_txerr_write (amhw_zlg217_can_t *p_hw_can,
                                       uint8_t            init)
{
    p_hw_can->info.peli.txerr = init & 0xff;
}

/**
 * \brief peli CAN TX错误计数寄存器读取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 错误计数器的值
 */
am_static_inline
uint8_t  amhw_zlg217_peli_can_txerr_read (amhw_zlg217_can_t *p_hw_can)
{
    return (uint8_t)(p_hw_can->info.peli.txerr & 0xff);
}

/**
 * \brief ff寄存器相关位定义
 *
 * \note ff寄存器只能一次全部写入，不可一位一位写入
 *
 * @{
 */

#define AMHW_ZLG217_CAN_PELI_SFF_FF         (0x1 << 7)  /**< \brief 远程标志位 */
#define AMHW_ZLG217_CAN_PELI_SFF_RTR        (0x1 << 6)  /**< \brief RTR标志 */

#define AMHW_ZLG217_CAN_PELI_SFF_DLC_SHIFT  (0ul)       /**< \brief DLC移动位数 */
#define AMHW_ZLG217_CAN_PELI_SFF_DLC_MASK   (0xf)       /**< \brief DLC掩码 */

/**
 * @}
 */

/**
 * \brief peli CAN SFF设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] val        : 待设置值
 *
 * \return 无
 */
am_static_inline
void amhw_zlg217_peli_can_sff_set (amhw_zlg217_can_t *p_hw_can,
                                   uint8_t            val)
{
    p_hw_can->info.peli.sff = val;
}

/**
 * \brief peli CAN SFF获取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return 寄存器值
 */
am_static_inline
uint8_t amhw_zlg217_peli_can_sff_get (amhw_zlg217_can_t *p_hw_can)
{
    return (p_hw_can->info.peli.sff & 0xff);
}

/**
 * \brief peli CAN ID设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] is_extend  : AM_TRUE 扩展帧；AM_FALSE 标准帧
 * \param[in] id         : id
 *
 * \return 无
 */
am_static_inline
void  amhw_zlg217_peli_can_id_set (amhw_zlg217_can_t *p_hw_can,
                                   am_bool_t          is_extend,
                                   uint32_t           id)
{
    if (AM_TRUE == is_extend) {
        p_hw_can->info.peli.data[1] = (id & 0x1f) << 3;
        p_hw_can->info.peli.data[0] = (id >> 5)  & 0xff;
        p_hw_can->info.peli.id1     = (id >> 13) & 0xff;
        p_hw_can->info.peli.id0     = (id >> 21) & 0xff;
    } else {
        p_hw_can->info.peli.id0 = (id >> 3) & 0xff;
        p_hw_can->info.peli.id1 = (id & 0x7) << 5;
    }
}

/**
 * \brief peli CAN ID获取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] is_extend  : AM_TRUE 扩展帧；AM_FALSE 标准帧
 *
 * \return id
 */
am_static_inline
uint32_t amhw_zlg217_peli_can_id_get (amhw_zlg217_can_t *p_hw_can,
                                      am_bool_t          is_extend)
{
    uint32_t temp = 0;

    if (AM_TRUE == is_extend) {
        temp |= (p_hw_can->info.peli.id0 & 0xff) << 21;
        temp |= (p_hw_can->info.peli.id1 & 0xff) << 13;
        temp |= (p_hw_can->info.peli.data[0] & 0xff) << 5;
        temp |= (p_hw_can->info.peli.data[1] >> 3) & 0x1f;
    } else {
        temp |= (p_hw_can->info.peli.id0 & 0xff) << 3;
        temp |= (p_hw_can->info.peli.id1 >> 5) & 0x7;
    }

    return temp;
}

/**
 * \brief peli CAN 数据写入
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] is_extend  : AM_TRUE 扩展帧；AM_FALSE 标准帧
 * \param[in] offset     : 偏移
 * \param[in] val        : 数据
 *
 * \return 无
 */
am_static_inline
void  amhw_zlg217_peli_can_data_write (amhw_zlg217_can_t *p_hw_can,
                                       am_bool_t          is_extend,
                                       uint8_t            offset,
                                       uint8_t            val)
{
    if (AM_TRUE == is_extend) {
        p_hw_can->info.peli.data[offset + 2] = val;
    } else {
        p_hw_can->info.peli.data[offset] = val;
    }
}

/**
 * \brief peli CAN 数据读取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] is_extend  : AM_TRUE 扩展帧；AM_FALSE 标准帧
 * \param[in] offset     : 偏移
 *
 * \return 数据
 */
am_static_inline
uint8_t amhw_zlg217_peli_can_data_read (amhw_zlg217_can_t *p_hw_can,
                                        am_bool_t          is_extend,
                                        uint8_t            offset)
{
    if (AM_TRUE == is_extend) {
        return (uint8_t)(p_hw_can->info.peli.data[offset + 2] & 0xff);
    } else {
        return (uint8_t)(p_hw_can->info.peli.data[offset] & 0xff);
    }
}

/**
 * \brief peli CAN 验收代码设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] ac         : 滤波值
 *
 * \return 数据
 */
am_static_inline
void amhw_zlg217_peli_can_ac_set (amhw_zlg217_can_t *p_hw_can,
                                  uint32_t           ac)
{
    p_hw_can->info.peli.data[0] = ac & 0xff;
    p_hw_can->info.peli.id1     = (ac >> 8) & 0xff;
    p_hw_can->info.peli.id0     = (ac >> 16) & 0xff;
    p_hw_can->info.peli.sff     = (ac >> 24) & 0xff;
}

/**
 * \brief peli CAN 验收代码获取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return ac
 */
am_static_inline
uint32_t amhw_zlg217_peli_can_ac_get (amhw_zlg217_can_t *p_hw_can)
{
    uint32_t temp = 0;

    temp |= (p_hw_can->info.peli.sff     & 0xff) << 24;
    temp |= (p_hw_can->info.peli.id0     & 0xff) << 16;
    temp |= (p_hw_can->info.peli.id1     & 0xff) << 8;
    temp |= (p_hw_can->info.peli.data[0] & 0xff);

    return temp;
}

/**
 * \brief peli CAN 屏蔽寄存器设置
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] am         : 屏蔽值
 *
 * \return 数据
 */
am_static_inline
void amhw_zlg217_peli_can_am_set (amhw_zlg217_can_t *p_hw_can,
                                  uint32_t           am)
{
    p_hw_can->info.peli.data[4] = am & 0xff;
    p_hw_can->info.peli.data[3] = (am >> 8) & 0xff;
    p_hw_can->info.peli.data[2] = (am >> 16) & 0xff;
    p_hw_can->info.peli.data[1] = (am >> 24) & 0xff;
}

/**
 * \brief peli CAN 屏蔽寄存器获取
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 *
 * \return am
 */
am_static_inline
uint32_t amhw_zlg217_peli_can_am_get (amhw_zlg217_can_t *p_hw_can)
{
    uint32_t temp = 0;

    temp |= (p_hw_can->info.peli.data[1] & 0xff) << 24;
    temp |= (p_hw_can->info.peli.data[2] & 0xff) << 16;
    temp |= (p_hw_can->info.peli.data[3] & 0xff) << 8;
    temp |= (p_hw_can->info.peli.data[4] & 0xff);

    return temp;
}

/******************************************************************************/

/**
 * \brief CAN模式设置，必须在复位功能下写
 *
 * \param[in] p_hw_can   : 指向CAN寄存器块的指针
 * \param[in] type       : 枚举amhw_zlg217_can_mode_t定义
 *
 * \return wu
 */
am_static_inline
void amhw_zlg217_can_type_set (amhw_zlg217_can_t      *p_hw_can,
                               amhw_zlg217_can_type_t  type)
{
    if (type == AMHW_ZLG217_CAN_BASIC_CAN) {
        p_hw_can->cdr &= ~(0x1 << 7);
    } else if (type == AMHW_ZLG217_CAN_PELI_CAN) {
        p_hw_can->cdr |= (0x1 << 7);
    }
}

/**
 * @} amhw_zlg217_if_can
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZLG217_CAN_H */

/* end of file */
