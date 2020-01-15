///*******************************************************************************
//*                                 AMetal
//*                       ----------------------------
//*                       innovating embedded platform
//*
//* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
//* All rights reserved.
//*
//* Contact information:
//* web site:    http://www.zlg.cn/
//*******************************************************************************/
//
///**
// * \file
// * \brief USB interfaces for operating hardware.
// *
// * \internal
// * \par Modification history
// * - 1.00 17-09-18  sdq, first implementation
// * \endinternal
// */
//#ifndef __AMHW_ZLG217_USB_H
//#define __AMHW_ZLG217_USB_H
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//#include "am_common.h"
//#include "am_types.h"
//#include "amhw_zlg217_periph_map.h"
//
///**
// * \addtogroup amhw_zlg217_if_usbd
// * \copydoc amhw_zlg217_usbd.h
// * @{
// */
//
///**
// *  \brief USB 寄存器结构体定义
// */
//typedef struct amhe_zlg217_usb{
//    __IO  uint16_t top;              /* 中断状态寄存器，偏移地址0x00 */
//          uint16_t reserve0;         /* 保留位 */
//    __IO  uint16_t int_state;        /* 中断状态寄存器，偏移地址0x04 */
//          uint16_t reserve1;         /* 保留位 */
//    __IO  uint16_t ep_int_state;     /* 端点中断状态寄存器，偏移地址0x08 */
//          uint16_t reserve2;         /* 保留位 */
//    __IO  uint16_t ep0_int_state;    /* 端点0中断状态寄存器，偏移地址0xc0 */
//          uint16_t reserve3;         /* 保留位 */
//    __IO  uint16_t int_en;           /* 中断使能寄存器，偏移地址0x10 */
//          uint16_t reserve4;         /* 保留位 */
//    __IO  uint16_t ep_int_en;        /* 端点中断使能寄存器，偏移地址0x14 */
//          uint16_t reserve5;         /* 保留位 */
//    __IO  uint16_t ep0_int_en;       /* 端点0中断使能寄存器，偏移地址0x18 */
//          uint16_t reserve6[3];      /* 保留位 */
//    struct {
//    __IO  uint16_t epx_int_state;
//          uint16_t reserved6;
//    }epx_int_state_arr[4];                 /* 端点x中断状态寄存器，偏移地址0x20-0x2c */
//          uint16_t reserve7[8];            /* 保留位 */
//    struct {
//    __IO  uint16_t epx_int_en;
//          uint16_t reserved8;
//    } epx_int_en_arr[4];                   /* 端点x中断使能寄存器，偏移地址0x40-0x4c */
//          uint16_t  reserve9[8];           /* 保留位 */
//    __IO  uint16_t addr;                   /* usb地址寄存器，偏移地址0x60 */
//          uint16_t reserve10;              /* 保留位 */
//    __IO  uint16_t ep_en;                  /* usb端点使能寄存器，偏移地址0x64 */
//          uint16_t reserve11[9];           /* 保留位 */
//    __IO  uint16_t tog_crl1_4;             /* 数据翻转控制寄存器，偏移地址0x78 */
//          uint16_t  reserve12[3];          /* 保留位 */
//    struct {
//    __IO  uint16_t stupx;
//          uint16_t reserved13;
//    }stupx_arr[8];                         /* 设置包数据寄存器，偏移地址0x80-0x9c */
//    __IO  uint16_t packet_size;            /* 传输包大小寄存器，偏移地址0xa0 */
//          uint16_t reserve14[47];          /* 保留位 */
//    struct {
//    __IO  uint16_t epx_avai;
//          uint16_t reserved15;
//    }epx_avail_arr[5];                     /* 端点x有效数据寄存器，偏移地址0x100-0x110 */
//          uint16_t  reserve16[22];         /* 保留位 */
//    struct {
//    __IO  uint16_t epx_ctrl;
//          uint16_t reserved17;
//    }epx_ctrl_arr[5];                      /* 端点x控制寄存器，偏移地址0x140-0x150 */
//          uint16_t reserve18[6];           /* 保留位 */
//    struct {
//    __IO  uint16_t epx_fifo;
//          uint16_t reserved19;
//    }epx_fifo_arr[5];                      /* 端点x fifo寄存器，偏移地址0x160-0x170 */
//          uint16_t reserve20[8];           /* 保留位 */
//    __IO  uint16_t ep_dma;                 /* 端点dma使能寄存器，偏移地址0x184 */
//          uint16_t reserve21;              /* 保留位 */
//    __IO  uint16_t ep_halt;                /* 端点暂存寄存器，偏移地址0x188 */
//          uint16_t reserve22[27];          /* 保留位 */
//    __IO  uint16_t power;                  /* 功耗寄存器，偏移地址0x1c0 */
//}amhw_zlg217_usb_t;
//
///*******************  Bit definition for USB_TOP register  *******************/
//#define  __USB_TOP_SPEED        0
//#define  __USB_TOP_CONNECT      1
//#define  __USB_TOP_RESET        3
//#define  __USB_TOP_SUSPEND      4
//#define  __USB_TOP_DP_DM_STATE  5
//#define  __USB_TOP_ACTIVE       7
//
///**************************  USB_TOP Configuration  ***************************/
//#define  __USB_SPEED            (1UL << __USB_TOP_SPEED)
//#define  __USB_CONNECT          (1UL << __USB_TOP_CONNECT)
//#define  __USB_RESET            (1UL << __USB_TOP_RESET)
//#define  __USB_SUSPEND          (1UL << __USB_TOP_SUSPEND)
//#define  __USB_DP_DM_STATE      (3UL << __USB_TOP_DP_DM_STATE)
//#define  __USB_ACTIVE           (1UL << __USB_TOP_ACTIVE)
//
///**
// * \brief USB速率枚举
// */
//typedef enum {
//    AMHW_ZLG217_USB_SPEED_FULL,   /* 全速传输 */
//    AMHW_ZLG217_USB_SPEED_LOW     /* 低速传输 */
//}amhw_zlg217_usb_speed_t;
//
///**
// *  \brief 设置usb速率函数
// *
// *  \param[in] speed  usb速率
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_speed_set (amhw_zlg217_usb_speed_t speed)
//{
//    if (speed == AMHW_ZLG217_USB_SPEED_FULL)
//        AMHW_ZLG217_USB->top &= (~__USB_SPEED);
//    else if (speed == AMHW_ZLG217_USB_SPEED_LOW)
//        AMHW_ZLG217_USB->top |= (__USB_SPEED);
//}
//
///**
// *  \brief 获得usb速率函数
// *
// *  \return usb速率
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_speed_get (void)
//{
//    return ((AMHW_ZLG217_USB->top & __USB_SPEED) >> __USB_TOP_SPEED);
//}
//
///**
// * \brief USB连接状态枚举
// */
//typedef enum {
//    AMHW_ZLG217_USB_DISCONNECT,   /* 断开连接 */
//    AMHW_ZLG217_USB_CONNECT       /* 连接 */
//}amhw_zlg217_usb_connect_t;
//
///**
// *  \brief 设置usb连接状态函数
// *
// *  \param[in] connect  usb连接状态
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_connect_set (amhw_zlg217_usb_connect_t connect)
//{
//    if (connect == AMHW_ZLG217_USB_DISCONNECT)
//        AMHW_ZLG217_USB->top &= (~__USB_CONNECT);
//    else if (connect == AMHW_ZLG217_USB_CONNECT)
//        AMHW_ZLG217_USB->top |= (__USB_CONNECT);
//}
//
///**
// *  \brief 获得usb连接状态函数
// *
// *  \return usb连接状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_connect_get (void)
//{
//    return ((AMHW_ZLG217_USB->top & __USB_CONNECT) >> __USB_TOP_CONNECT);
//}
//
///**
// * \brief USB复位状态枚举
// */
//typedef enum {
//    AMHW_ZLG217_USB_NORESET,   /* 不复位 */
//    AMHW_ZLG217_USB_RESET      /* 复位 */
//}amhw_zlg217_usb_reset_t;
//
///**
// *  \brief 设置usb复位函数
// *
// *  \param[in] reset  usb复位状态
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_reset_set (amhw_zlg217_usb_reset_t reset)
//{
//    if(reset == AMHW_ZLG217_USB_NORESET)
//        AMHW_ZLG217_USB->top &= (~__USB_RESET);
//    else if(reset == AMHW_ZLG217_USB_RESET)
//        AMHW_ZLG217_USB->top |= (__USB_RESET);
//}
//
///**
// *  \brief 获得usb复位函数
// *
// *  \return usb复位状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_reset_get (void)
//{
//    return ((AMHW_ZLG217_USB->top & __USB_RESET) >> __USB_TOP_RESET);
//}
//
///**
// * \brief USB suspend状态枚举
// */
//typedef enum {
//    AMHW_ZLG217_USB_SUSPEND_WORK,     /* 工作状态 */
//    AMHW_ZLG217_USB_SUSPEND_PENDING   /* 挂起状态 */
//}amhw_zlg217_usb_suspend_t;
//
///**
// *  \brief 获得usb suspend状态函数
// *
// *  \return usb suspend状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_suspend_get (void)
//{
//    return ((AMHW_ZLG217_USB->top & __USB_SUSPEND) >> __USB_TOP_SUSPEND);
//}
//
///**
// *  \brief 获得usb dp/dm线状态函数
// *
// *  \return usb dp/dm状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_dp_dm_get (void)
//{
//    return ((AMHW_ZLG217_USB->top & __USB_DP_DM_STATE) >> __USB_TOP_DP_DM_STATE);
//}
//
///**
// * \brief USB 总线活跃状态
// */
//typedef enum {
//    AMHW_ZLG217_USB_INACTIVE,     /* 总线不活跃 */
//    AMHW_ZLG217_USB_ACTIVE        /* 总线活跃 */
//}amhw_zlg217_usb_active_t;
//
///**
// *  \brief 设置usb总线活跃状态函数
// *
// *  \param[in] active  usb 总线活跃状态
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_active_set(amhw_zlg217_usb_active_t active)
//{
//    if (active == AMHW_ZLG217_USB_INACTIVE)
//        AMHW_ZLG217_USB->top &= (~__USB_ACTIVE);
//    else if (active == AMHW_ZLG217_USB_ACTIVE)
//        AMHW_ZLG217_USB->top |= (__USB_ACTIVE);
//}
//
///**
// *  \brief 获得usb总线活跃状态函数
// *
// *  \return usb 总线活跃状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_active_get (void)
//{
//    return ((AMHW_ZLG217_USB->top & __USB_ACTIVE) >> __USB_TOP_ACTIVE);
//}
//
///***********************  USB_INT_STATE Configuration  ************************/
//#define  AMHW_ZLG217_USB_INT_STATE_RSTF      (1UL << 0)   /* 总线复位请求 */
//#define  AMHW_ZLG217_USB_INT_STATE_SUSPENDF  (1UL << 1)   /* 总线挂起 */
//#define  AMHW_ZLG217_USB_INT_STATE_RESUMF    (1UL << 2)   /* 总线唤醒 */
//#define  AMHW_ZLG217_USB_INT_STATE_SOFF      (1UL << 3)   /* 检测到SOF */
//#define  AMHW_ZLG217_USB_INT_STATE_EPINTF    (1UL << 4)   /* 端点中断 */
//#define  AMHW_ZLG217_USB_INT_STATE_ALL        0X0F
///**
// *  \brief usb中断状态寄存器清除函数
// *
// *  \param[in] state  usb 中断状态寄存器状态位枚举
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_int_state_clear (uint16_t state)
//{
//    AMHW_ZLG217_USB->int_state |= (state & AMHW_ZLG217_USB_INT_STATE_ALL);
//}
//
///**
// *  \brief 获得usb中断状态寄存器状态函数
// *
// *  \return usb 中断状态寄存器状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_int_state_get (void)
//{
//    return AMHW_ZLG217_USB->int_state;
//}
//
///************************  EP_INT_STATE Configuration  ************************/
//#define  AMHW_ZLG217_EP_INT_STATE_EP0F        (1UL << 0)
//#define  AMHW_ZLG217_EP_INT_STATE_EP1F        (1UL << 1)
//#define  AMHW_ZLG217_EP_INT_STATE_EP2F        (1UL << 2)
//#define  AMHW_ZLG217_EP_INT_STATE_EP3F        (1UL << 3)
//#define  AMHW_ZLG217_EP_INT_STATE_EP4F        (1UL << 4)
//#define  AMHW_ZLG217_EP_INT_STATE_EP_ALL       0X1F
//
///**
// *  \brief 获得usb端点中断状态寄存器状态函数
// *
// *  \return usb 中断状态寄存器状态
// */
//am_static_inline
//uint16_t amhw_zlg217_ep_int_state_get (void)
//{
//    return AMHW_ZLG217_USB->ep_int_state;
//}
//
///**
// *  \brief 清除usb端点中断状态
// *
// *  \param[in] ep 要清除的端点
// */
//am_static_inline
//uint16_t amhw_zlg217_ep_int_state_clear (uint16_t ep)
//{
//    return AMHW_ZLG217_USB->ep_int_state |= ep;
//}
//
///************************  EP0_INT_STATE Configuration  ***********************/
//#define  AMHW_ZLG217_EP0_INT_STATE_SETUP          (1UL << 0)  /* 接收到SETUP包 */
//#define  AMHW_ZLG217_EP0_INT_STATE_END            (1UL << 1)  /* 端点传输完成 */
//#define  AMHW_ZLG217_EP0_INT_STATE_IN_NACK        (1UL << 2)
//#define  AMHW_ZLG217_EP0_INT_STATE_IN_ACK         (1UL << 3)
//#define  AMHW_ZLG217_EP0_INT_STATE_IN_STALL       (1UL << 4)
//#define  AMHW_ZLG217_EP0_INT_STATE_OUT_NACK       (1UL << 5)
//#define  AMHW_ZLG217_EP0_INT_STATE_OUT_ACK        (1UL << 6)
//#define  AMHW_ZLG217_EP0_INT_STATE_OUT_STALL      (1UL << 7)
//#define  AMHW_ZLG217_EP0_INT_STATE_ALL             0XFF
//
///**
// *  \brief 获得usb端点0中断状态寄存器状态函数
// *
// *  \return usb 中断状态寄存器状态
// */
//am_static_inline
//uint16_t amhw_zlg217_ep0_int_state_get (void)
//{
//    return AMHW_ZLG217_USB->ep0_int_state;
//}
//
///**
// *  \brief usb端点0中断状态寄存器清除函数
// *
// *  \param[in] state  usb 中断状态寄存器状态位
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_ep0_int_state_clear (uint16_t state)
//{
//    AMHW_ZLG217_USB->ep0_int_state |= state;
//}
//
///************************  USB_INT_EN Configuration  ***********************/
//#define AMHW_ZLG217_USB_INT_EN_RSTIE       (1ul << 0)  /* USB 总线复位中断使能位 */
//#define AMHW_ZLG217_USB_INT_EN_SUSPENDIE   (1ul << 1)  /* USB 总线挂起中断使能位 */
//#define AMHW_ZLG217_USB_INT_EN_RESUMIE     (1ul << 2)  /* 唤醒中断使能位 */
//#define AMHW_ZLG217_USB_INT_EN_SOFIE       (1ul << 3)  /* SOF 检测中断使能位 */
//#define AMHW_ZLG217_USB_INT_EN_EPIE        (1ul << 4)  /* 端点中断使能位 */
//#define AMHW_ZLG217_USB_INT_EN_ALL          0X1F
//
///**
// *  \brief 使能USB中断
// *
// *  \param[in] int_en               要使能的USB中断
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_int_enable (uint16_t int_en)
//{
//    AMHW_ZLG217_USB->int_en |= int_en;
//}
//
///**
// *  \brief 禁能USB中断
// *
// *  \param[in] int_dis              要禁能的USB中断
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_int_disable (uint16_t int_dis)
//{
//    AMHW_ZLG217_USB->int_en &= ~int_dis;
//}
//
///**
// *  \brief 获得usb端点0中断状态寄存器状态函数
// *
// *  \return usb 中断状态寄存器状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_int_en_get (void)
//{
//    return AMHW_ZLG217_USB->int_en;
//}
//
///**************************  EP_INT_EN Configuration  ************************/
//#define  AMHW_ZLG217_EP_INT_EN_EP0IE          (1UL << 0)
//#define  AMHW_ZLG217_EP_INT_EN_EP1IE          (1UL << 1)
//#define  AMHW_ZLG217_EP_INT_EN_EP2IE          (1UL << 2)
//#define  AMHW_ZLG217_EP_INT_EN_EP3IE          (1UL << 3)
//#define  AMHW_ZLG217_EP_INT_EN_EP4IE          (1UL << 4)
//#define  AMHW_ZLG217_EP_INT_EN_ALL             0X1F
//
///**
// *  \brief 获得usb端点中断使能寄存器状态
// *
// *  \return usb 端点中断使能寄存器状态
// */
//am_static_inline
//uint16_t amhw_zlg217_ep_int_en_get (void)
//{
//    return AMHW_ZLG217_USB->ep_int_en;
//}
//
///**
// *  \brief usb端点中断使能
// *
// *  \brief epx  要使能的端点
// */
//am_static_inline
//void amhw_zlg217_ep_int_enable (uint16_t epx)
//{
//    AMHW_ZLG217_USB->ep_int_en |= epx;
//}
//
///**
// *  \brief usb端点中断禁能
// *
// *  \brief epx  要禁能的端点
// */
//am_static_inline
//void amhw_zlg217_ep_int_disable (uint16_t epx)
//{
//    AMHW_ZLG217_USB->ep_int_en &= ~epx;
//}
//
///**************************  EP0_INT_EN Configuration  ************************/
//#define  AMHW_ZLG217_EP0_INT_EN_SETUPIE        (1UL << 0) /* 接收到 SETUP 包 */
//#define  AMHW_ZLG217_EP0_INT_EN_ENDIE          (1UL << 1) /* 传输完成 */
//#define  AMHW_ZLG217_EP0_INT_EN_IN_NACKIE      (1UL << 2) /* IN 包应答 NACK */
//#define  AMHW_ZLG217_EP0_INT_EN_IN_ACKIE       (1UL << 3) /* IN 包应答 ACK */
//#define  AMHW_ZLG217_EP0_INT_EN_IN_STALLIE     (1UL << 4) /* IN 包应答 STALL */
//#define  AMHW_ZLG217_EP0_INT_EN_OUT_NACKIE     (1UL << 5) /* OUT 包应答 NACK */
//#define  AMHW_ZLG217_EP0_INT_EN_OUT_ACKIE      (1UL << 6) /* OUT 包应答 ACK */
//#define  AMHW_ZLG217_EP0_INT_EN_OUT_STALLIE    (1UL << 7) /* OUT 包应答 STALL */
//#define  AMHW_ZLG217_EP0_INT_EN_ALL            0XFF
//
///**
// *  \brief 获得usb端点0中断使能寄存器状态
// *
// *  \return usb 端点0中断使能寄存器状态
// */
//am_static_inline
//uint16_t amhw_zlg217_ep0_int_en_get (void)
//{
//    return AMHW_ZLG217_USB->ep0_int_en;
//}
//
///**
// *  \brief 获得usb端点0中断使能
// *
// *  \param   enable  要使能的中断类型
// */
//am_static_inline
//void amhw_zlg217_ep0_int_enable (uint16_t enable)
//{
//    AMHW_ZLG217_USB->ep0_int_en |= enable;
//}
//
///**
// *  \brief 获得usb端点0中断禁能
// *
// *  \param   disable  要禁能的中断类型
// */
//am_static_inline
//void amhw_zlg217_ep0_int_disable (uint16_t disable)
//{
//    AMHW_ZLG217_USB->ep0_int_en &= ~disable;
//}
//
///*************************  EPX_INT_STATE Configuration  **********************/
//#define  AMHW_ZLG217_EPX_INT_STATE_ENDIE       (1UL << 1) /* 传输完成 */
//#define  AMHW_ZLG217_EPX_INT_STATE_IN_NACKIE   (1UL << 2) /* IN 包应答 NACK */
//#define  AMHW_ZLG217_EPX_INT_STATE_IN_ACKIE    (1UL << 3) /* IN 包应答 ACK */
//#define  AMHW_ZLG217_EPX_INT_STATE_IN_STALLIE  (1UL << 4) /* IN 包应答 STALL */
//#define  AMHW_ZLG217_EPX_INT_STATE_OUT_NACKIE  (1UL << 5) /* OUT 包应答 NACK */
//#define  AMHW_ZLG217_EPX_INT_STATE_OUT_ACKIE   (1UL << 6) /* OUT 包应答 ACK */
//#define  AMHW_ZLG217_EPX_INT_STATE_OUT_STALLIE (1UL << 7) /* OUT 包应答 STALL */
//#define  AMHW_ZLG217_EPX_INT_STATE_ALL          0XFE
//
///**
// * \brief USB 端点
// */
//typedef enum {
//    AMHW_ZLG217_USB_EP1,
//    AMHW_ZLG217_USB_EP2,
//    AMHW_ZLG217_USB_EP3,
//    AMHW_ZLG217_USB_EP4
//}amhw_zlg217_usb_epx_t;
//
///**
// *  \brief 获得usb端点X中断状态
// *
// *  \param[in] epx                  要获取的端点
// *
// *  \return usb 端点X中断使能寄存器状态
// */
//am_static_inline
//uint16_t amhw_zlg217_epx_int_state_get (amhw_zlg217_usb_epx_t epx)
//{
//    return AMHW_ZLG217_USB->epx_int_state_arr[epx].epx_int_state;
//}
//
///**
// *  \brief usb端点X中断状态清除
// *
// *  \param[in] epx                  要清除的端点
// *  \param[in] clear                要清除的位（状态）
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_epx_int_state_clear (amhw_zlg217_usb_epx_t epx,
//                                      uint16_t clear)
//{
//    AMHW_ZLG217_USB->epx_int_state_arr[epx].epx_int_state |= clear;
//}
//
///**************************  EPX_INT_EN Configuration  ************************/
//#define  AMHW_ZLG217_EPX_INT_EN_ENDIE       (1UL << 1) /* 传输完成 */
//#define  AMHW_ZLG217_EPX_INT_EN_IN_NACKIE   (1UL << 2) /* IN 包应答 NACK */
//#define  AMHW_ZLG217_EPX_INT_EN_IN_ACKIE    (1UL << 3) /* IN 包应答 ACK */
//#define  AMHW_ZLG217_EPX_INT_EN_IN_STALLIE  (1UL << 4) /* IN 包应答 STALL */
//#define  AMHW_ZLG217_EPX_INT_EN_OUT_NACKIE  (1UL << 5) /* OUT 包应答 NACK */
//#define  AMHW_ZLG217_EPX_INT_EN_OUT_ACKIE   (1UL << 6) /* OUT 包应答 ACK */
//#define  AMHW_ZLG217_EPX_INT_EN_OUT_STALLIE (1UL << 7) /* OUT 包应答 STALL */
//#define  AMHW_ZLG217_EPX_INT_EN_ALL          0XFE
//
///**
// *  \brief 获得usb端点X中断使能寄存器状态
// *
// *  \param[in] epx                  要获取的端点
// *
// *  \return usb 端点X中断使能寄存器状态
// */
//am_static_inline
//uint16_t amhw_zlg217_epx_int_en_get (amhw_zlg217_usb_epx_t epx)
//{
//    return AMHW_ZLG217_USB->epx_int_en_arr[epx].epx_int_en;
//}
//
///**
// *  \brief usb端点X中断使能
// *
// *  \param[in] epx                  端点
// *  \param[in] enable               要使能的位
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_epx_int_enable (amhw_zlg217_usb_epx_t epx,
//                                 uint16_t enable)
//{
//    AMHW_ZLG217_USB->epx_int_en_arr[epx].epx_int_en |= enable;
//}
//
///**
// *  \brief usb端点X中断禁能
// *
// *  \param[in] epx                  端点
// *  \param[in] disable              要禁能的位
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_epx_int_disable (amhw_zlg217_usb_epx_t epx,
//                                  uint16_t disable)
//{
//    AMHW_ZLG217_USB->epx_int_en_arr[epx].epx_int_en &= ~disable;
//}
//
///***************************  USB_ADDR Configuration  *************************/
//#define __USB_ADDR_MASK     0X7F
///**
// *  \brief 设置USB的地址
// *
// *  \param[in] addr                 要设置的地址
// *
// *  \return usb 端点X中断使能寄存器状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_addr_set (uint8_t addr)
//{
//    return AMHW_ZLG217_USB->addr = (addr & __USB_ADDR_MASK);
//}
//
///**
// *  \brief 获得USB的地址
// *
// *  \return usb USB地址
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_addr_get (void)
//{
//    return AMHW_ZLG217_USB->addr;
//}
//
///******************************  EP_EN Configuration  *************************/
//#define  AMHW_ZLG217_USB_EP0EN       (1UL << 0)
//#define  AMHW_ZLG217_USB_EP1EN       (1UL << 1)
//#define  AMHW_ZLG217_USB_EP2EN       (1UL << 2)
//#define  AMHW_ZLG217_USB_EP3EN       (1UL << 3)
//#define  AMHW_ZLG217_USB_EP4EN       (1UL << 4)
//#define  AMHW_ZLG217_USB_EP_ALL       0X1F
//
///**
// *  \brief 获得USB端点使能寄存器状态
// *
// *  \return usb USB端点使能寄存器状态
// */
//am_static_inline
//uint16_t amhw_zlg217_ep_en_get (void)
//{
//    return AMHW_ZLG217_USB->ep_en;
//}
//
///**
// *  \brief usb端点使能
// *
// *  \param[in] enable               要使能的端点
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_ep_enable (uint16_t enable)
//{
//    AMHW_ZLG217_USB->ep_en |= enable;
//}
//
///**
// *  \brief usb端点禁能
// *
// *  \param[in] disable              要禁能的端点
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_ep_disable (uint16_t disable)
//{
//    AMHW_ZLG217_USB->ep_en &= ~disable;
//}
//
///****************************  TOG_CTRL Configuration  ************************/
///**< \brief 端点数据翻转 */
//#define  AMHW_ZLG217_USB_DTOG_MASK    0X55
//#define  AMHW_ZLG217_USB_DTOG1       (1UL << 0)
//#define  AMHW_ZLG217_USB_DTOG2       (1UL << 2)
//#define  AMHW_ZLG217_USB_DTOG3       (1UL << 4)
//#define  AMHW_ZLG217_USB_DTOG4       (1UL << 6)
//
///**< \brief 端点数据翻转使能位 */
//#define  AMHW_ZLG217_USB_DTOGEN_MASK  0XAA
//#define  AMHW_ZLG217_USB_DTOG1EN     (1UL << 1)
//#define  AMHW_ZLG217_USB_DTOG2EN     (1UL << 3)
//#define  AMHW_ZLG217_USB_DTOG3EN     (1UL << 5)
//#define  AMHW_ZLG217_USB_DTOG4EN     (1UL << 7)
//
///**
// *  \brief 设置USB端点数据设为DATA0
// *
// *  \param[in] ep  要设置的端点号
// *
// *  \note 端点号不可按位或，端点号范围为1~4代表端点1~4
// */
//am_static_inline
//void amhw_zlg217_usb_dtog_data0 (uint16_t ep)
//{
//    if (ep >= 1 && ep <= 4) {
//        AMHW_ZLG217_USB->tog_crl1_4 &=  ~(1 << ((ep - 1) * 2));
//    }
//}
//
///**
// *  \brief 设置USB端点数据设为DATA1
// *
// *  \param[in] ep   要设置的端点号（不可按位或）
// *
// *  \note 端点号不可按位或，端点号范围为1~4代表端点1~4
// */
//am_static_inline
//void amhw_zlg217_usb_dtog_data1 (uint16_t ep)
//{
//    if (ep >= 1 && ep <= 4) {
//        AMHW_ZLG217_USB->tog_crl1_4 |=  (1 << ((ep - 1) * 2));
//    }
//}
//
///**
// *  \brief 获得USB端点数据翻转状态
// *
// *  \return usb USB端点数据翻转使能状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_dtog_get (void)
//{
//    return AMHW_ZLG217_USB->tog_crl1_4 & AMHW_ZLG217_USB_DTOG_MASK;
//}
//
///**
// *  \brief usb端点数据翻转使能
// *
// *  \param[in] ep     要使能翻转的端点
// *
// *  \note 端点号不可按位或，端点号范围为1~4代表端点1~4
// */
//am_static_inline
//void amhw_zlg217_usb_dtog_enable (uint16_t ep)
//{
//    if (ep >= 1 && ep <= 4) {
//        AMHW_ZLG217_USB->tog_crl1_4 |=  (1 << ((ep - 1) * 2 + 1));
//    }
//}
//
///**
// *  \brief usb端点数据翻转禁能
// *
// *  \param[in] ep   要禁能翻转的端点
// *
// *  \note 端点号不可按位或，端点号范围为1~4代表端点1~4
// */
//am_static_inline
//void amhw_zlg217_usb_dtog_disable (uint16_t ep)
//{
//    if (ep >= 1 && ep <= 4) {
//        AMHW_ZLG217_USB->tog_crl1_4 &=  ~(1 << ((ep - 1) * 2 + 1));
//    }
//}
//
///*****************************  SETUPX Configuration  *************************/
///**
// *  \brief 获取USB数据包数据（请求命令）
// *
// *  \param[out] p_data              USB数据包中的数据
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_setupdata_get (uint8_t *p_data)
//{
//    int i = 0;
//
//    for (i = 0; i < 8; i++) {
//        p_data[i] = AMHW_ZLG217_USB->stupx_arr[i].stupx;
//    }
//}
//
///**************************  PACKET_SIZE Configuration  ***********************/
//
///**
// *  \brief 设置USB传输包的大小
// *
// *  \param[in] size                 USB传输包的大小
// *
// *  \return 无
// *
// *  \note 最大可以设置64字节
// */
//am_static_inline
//void amhw_zlg217_usb_packet_size_set (uint8_t size)
//{
//    if (size > 64)
//        size = 64;
//
//    AMHW_ZLG217_USB->packet_size = size;
//}
//
///**
// *  \brief 获取USB传输包的大小
// *
// *  \return USB传输包的大小
// */
//am_static_inline
//uint8_t amhw_zlg217_usb_packet_size_get (void)
//{
//    return AMHW_ZLG217_USB->packet_size;
//}
//
///***************************  EPX_AVAIL Configuration  ************************/
///**
// * \brief USB 端点
// */
//typedef enum {
//    AMHW_ZLG217_USB_EPX0,
//    AMHW_ZLG217_USB_EPX1,
//    AMHW_ZLG217_USB_EPX2,
//    AMHW_ZLG217_USB_EPX3,
//    AMHW_ZLG217_USB_EPX4
//}amhw_zlg217_usb_epx2_t;  /* 和amhw_zlg217_usb_epx_t 区分 */
//
///**
// *  \brief 获得USB端点X FIFO有效数据个数
// *
// *  \return USB端点X FIFO有效数据个数
// */
//am_static_inline
//uint8_t amhw_zlg217_usb_epx_avail_size_get (amhw_zlg217_usb_epx2_t epx)
//{
//    return AMHW_ZLG217_USB->epx_avail_arr[epx].epx_avai;
//}
//
///****************************  EPX_CTRL Configuration  ************************/
//#define AMHW_ZLG217_USB_EPX_TRANCOUNT_MASK    0X7F
//
//#define AMHW_ZLG217_USB_EPX_TRANEN    (1 << 7)    /* 使能传输 */
//
///**
// *  \brief 设置端点X的数据传输个数并启动传输
// *
// *  \param[in] epx                  要进行传输的端点
// *  \param[in] count                要传输的数据个数
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_epx_transfer (amhw_zlg217_usb_epx2_t epx,
//                                   uint8_t count)
//{
//    AMHW_ZLG217_USB->epx_ctrl_arr[epx].epx_ctrl = AMHW_ZLG217_USB_EPX_TRANEN | count;
//}
//
///**
// *  \brief 判断端点X的USB数据是否传输完成
// *
// *  \param[in] epx                  要进行传输的端点
// *
// *  \return 传输完成返回1，未完成返回0
// */
//am_static_inline
//uint8_t amhw_zlg217_usb_transfer_end(amhw_zlg217_usb_epx2_t epx)
//{
//    if (AMHW_ZLG217_USB->epx_ctrl_arr[epx].epx_ctrl & AMHW_ZLG217_USB_EPX_TRANEN)
//        return 0;
//    return 1;
//}
//
///****************************  EPX_FIFO Configuration  ************************/
///**
// *  \brief 向USB端点X的fifo中写入数据
// *
// *  \param[in] epx                  要设置的端点
// *  \param[in] data                 要写入的数据
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_epx_fifo_write (amhw_zlg217_usb_epx2_t epx,
//                                     uint8_t data)
//{
//    AMHW_ZLG217_USB->epx_fifo_arr[epx].epx_fifo = data;
//}
//
///**
// *  \brief 从USB端点X的fifo中读出数据
// *
// *  \param[in] epx                  要设置的端点
// *
// *  \return fifo中的数据
// */
//am_static_inline
//uint8_t amhw_zlg217_usb_epx_fifo_read (amhw_zlg217_usb_epx2_t epx)
//{
//    return AMHW_ZLG217_USB->epx_fifo_arr[epx].epx_fifo;
//}
//
///*****************************  EP_DMA Configuration  *************************/
///**< \brief 端点DMA使能位（只支持端点1和端点2） */
//#define  AMHW_ZLG217_USB_DMA1EN     (1UL << 0)
//#define  AMHW_ZLG217_USB_DMA2EN     (1UL << 1)
//
///**
// *  \brief 使能端点DMA
// *
// *  \param[in] dma_epx              要使能DMA的端点
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_ep_dma_enable (uint16_t dma_epx)
//{
//    AMHW_ZLG217_USB->ep_dma |= dma_epx;
//}
//
///**
// *  \brief 禁能端点DMA
// *
// *  \param[in] dma_epx              要禁能DMA的端点
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_ep_dma_disable (uint16_t dma_epx)
//{
//    AMHW_ZLG217_USB->ep_dma &= ~dma_epx;
//}
//
///**
// *  \brief 获取端点DMA的使能状态
// *
// *  \return 无
// */
//am_static_inline
//uint8_t amhw_zlg217_usb_ep_dma_get (void)
//{
//    return AMHW_ZLG217_USB->ep_dma;
//}
//
///*****************************  EP_HALT Configuration  ************************/
///**< \brief 端点暂停位 */
///**< \brief 当该位设为‘1’，设备会在 IN/OUT 传输后自动响应 STALL。当接收到令牌包时该位会被硬件自动清零 */
//#define  AMHW_ZLG217_USB_EP_HALT0     (1UL << 0)
//#define  AMHW_ZLG217_USB_EP_HALT1     (1UL << 1)
//#define  AMHW_ZLG217_USB_EP_HALT2     (1UL << 2)
//#define  AMHW_ZLG217_USB_EP_HALT3     (1UL << 3)
//#define  AMHW_ZLG217_USB_EP_HALT4     (1UL << 4)
//#define  AMHW_ZLG217_USB_EP_HALT_ALL   0X1F
//
///**
// *  \brief 设置端点暂停
// *
// *  \param[in] halt                 要暂停的位
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_ep_halt_set (uint16_t halt)
//{
//    AMHW_ZLG217_USB->ep_halt |= halt;
//}
//
///**
// *  \brief 清除端点暂停（不暂停）
// *
// *  \param[in] halt   要清除的暂停位
// *
// *  \return 无
// */
//am_static_inline
//void amhw_zlg217_usb_ep_halt_reset (uint16_t halt)
//{
//    AMHW_ZLG217_USB->ep_halt &= ~halt;
//}
//
///**
// *  \brief 获取端点暂停状态
// *
// *  \return 端点暂停状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_ep_halt_get (void)
//{
//    return AMHW_ZLG217_USB->ep_halt;
//}
//
///*****************************  USB_POWER Configuration  **********************/
//#define  AMHW_ZLG217_USB_POWER_SUSPEN     (1UL << 0)
//#define  AMHW_ZLG217_USB_POWER_SUSP       (1UL << 1)
//#define  AMHW_ZLG217_USB_POWER_WKUP       (1UL << 3)
//
///**< \brief 挂起控制宏 */
///**< \brief 总线使能挂起宏 */
//#define  AMHW_ZLG217_USB_SUSPDIS    0         /**< \brief 由控制器控制是否挂起 */
//#define  AMHW_ZLG217_USB_SUSPEN    (1 << 0)   /**< \brief 根据挂起模式（位挂起宏）控制是否挂起 */
//
///**< \brief 位挂起宏 */
//#define  AMHW_ZLG217_USB_SUSP       0         /**< \brief 挂起模式 */
//#define  AMHW_ZLG217_USB_NORMAL    (1 << 1)   /**< \brief 正常工作模式 */
//
///**
// *  \brief 设置USB挂起状态
// *
// *  \param[in] susp 传入的值应该是一个 位挂起宏 和 一个总线使能挂起宏相或
// *
// *  \retval 返回1表示控制器处于正常工作模式，返回0表示控制器处于挂起模式
// */
//am_static_inline
//void amhw_zlg217_usb_susp_set (uint16_t susp)
//{
//    AMHW_ZLG217_USB->power |= (susp & (~AMHW_ZLG217_USB_POWER_WKUP));
//}
//
///**
// *  \brief 获取USB挂起状态
// *
// *  \retval 返回的内容包含挂起是否使能以及挂起状态
// */
//am_static_inline
//uint16_t amhw_zlg217_usb_susp_get (void)
//{
//    return (AMHW_ZLG217_USB->power & ~AMHW_ZLG217_USB_POWER_WKUP);
//}
//
///**
// *  \brief 将控制器从挂起状态唤醒
// */
//am_static_inline
//void amhw_zlg217_usb_wakeup (void)
//{
//    AMHW_ZLG217_USB->power |= AMHW_ZLG217_USB_POWER_WKUP;
//}
//
///**
// * @}if_amhw_zlg217_usbd
// */
//
//#ifdef __cplusplus
//}
//#endif
//
//#endif /* __AMHW_ZLG217_USBD_H */
//
///* end of file */
