/*******************************************************************************
*                                 AMetal
*                       ---------------------------
*                       innovating embedded systems
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
 * \brief zlg126 USB_printer 用户配置文件
 * \sa am_zlg126_hwconfig_usb_printer.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-12  adw, first implementation.
 * \endinternal
 */

#include "hw/amhw_hc32f07x_rcc.h"
#include "ametal.h"
#include "am_hc32f07x.h"
#include "am_delay.h"
#include "am_types.h"
#include "am_clk.h"
#include "am_usb.h"
#include "am_gpio.h"
#include "am_usbd.h"
#include "am_usbd_printer.h"
#include "am_hc32f07x_usbd.h"
/**
 * \addtogroup am_zlg116_if_hwconfig_src_usb_printer
 * \copydoc am_zlg116_hwconfig_usb_printer.c
 * @{
 */

/*******************************************************************************
 * 用户USB描述符配置宏,用户配置描述符宏即可,无需关心USB描述符。
 ******************************************************************************/

/** \brief USB设备描述配置宏*/
#define __USBD_PRINTER_VENDOR_ID                 (0x1F22) /**< \brief 厂商编号,作为产品发布必须写自己公司的厂商编号，以免侵权，此处填了一个没有在USB协会注册的编号*/
#define __USBD_PRINTER_PRODUCT_ID                (0x029B) /**< \brief 产品编号*/
#define __USBD_PRINTER_DEVICE_ID                 (0x0101) /**< \brief 设备编号*/
#define __USBD_PRINTER_VENDOR_STRING_INDEX       (0x01)   /**< \brief 描述厂商的字符串索引*/
#define __USBD_PRINTER_PRODUCT_STRING_INDEX      (0x02)   /**< \brief 描述产品的字符串索引 */
#define __USBD_PRINTER_CONFIG_COUNT              (1U)     /**< \brief 配置数，一般设置只有一个配置*/

/** \brief USB配置描述符配置宏*/
#define __USBD_PRINTER_INTERFACE_COUNT           (1U)     /**< \brief 接口数*/
#define __USBD_PRINTER_CONFIGURE_INDEX           (1U)     /**< \brief 配置描述符索引*/
#define __USBD_PRINTER_DEVICE_POWER               AM_USBD_MAX_POWER /**< \brief 设备向主机申请的电流，单位2mA,最大100mA*/
/** \brief 设备属性，自供电，不支持远程唤醒*/
#define __USBD_PRINTER_DEVICE_ATTRIBUTE          \
              (AM_USBD_CONFIG_SELF_POWER | AM_USBD_CONFIG_NOT_REMOTE_WAKEUP)

/**\brief USB端点描述符配置宏*/
#define __USBD_PRINTER_ENDPOINT_IN                1  /**< \brief 输入端点号*/
#define __USBD_PRINTER_ENDPOINT_IN_PACKSIZE       AM_USBD_MAX_EP_DATA_CNT       /**< \brief 端点包大小,设置为64*/
#define __USBD_PRINTER_ENDPOINT_IN_ATTRIBUTE      AM_USB_ENDPOINT_BULK          /**< \brief 设置端点属性为批量传输*/
#define __USBD_PRINTER_ENDPOINT_IN_QUERY_TIME    (0x06)                         /**< \brief 设置端点查询时间为10ms,单位为1ms*/

#define __USBD_PRINTER_ENDPOINT_OUT               2
#define __USBD_PRINTER_ENDPOINT_OUT_PACKSIZE      AM_USBD_MAX_EP_DATA_CNT
#define __USBD_PRINTER_ENDPOINT_OUT_ATTRIBUTE     AM_USB_ENDPOINT_BULK
#define __USBD_PRINTER_ENDPOINT_OUT_QUERY_TIME   (0x06)

/**\brief USB接口描述符配置宏*/
#define __USBD_PRINTER_ENDPOINT_COUNT            (2U)     /**< \brief 端点个数,输入端点与输出端点(不包括控制端点)*/


/*****************************************************************************
 * 全局变量和常量
 *****************************************************************************/

/**
 * \brief 打印机设备id 字符串定义
 * 前两个字节为字符串数据长度，大端对齐.所以保留前两个字符串字符数据，该id号是作为主机检索驱动使用的标识。
 */
static uint8_t __g_printer_id[] = "xxMFG:ZLG;MDL: usb printer;CMD:POSTSCRIPT";

/**
 * \brief 打印机数据传输buff定义
 * \note 该buff大小必须大于端点最大包大小，并且buff大小应该大于打印机id(__g_printer_id)字符串长度,以免数据溢出
 */
static uint8_t __g_printer_buff[AM_USBD_MAX_EP_DATA_CNT + 1] = {0};

/*****************************************************************************
 * USB描述符(设备描述符,配置描述符，接口描述符,端点描述符),该部分用户只需配置上述宏即可
 *****************************************************************************/

/* USB 设备描述符 */
static const uint8_t __g_am_usbd_printer_desc_dev[AM_USB_DESC_LENGTH_DEVICE]  = {
    AM_USB_DESC_LENGTH_DEVICE,        /* 设备描述符的字节数 */
    AM_USB_DESC_TYPE_DEVICE,          /* 设备描述符类型编号，固定为0x01 */

    /* USB版本 USB2.0 */
    AM_USB_SHORT_GET_LOW(AM_USB_VERSION), AM_USB_SHORT_GET_HIGH(AM_USB_VERSION),
    AM_USBD_CLASS,                          /* 通信类 */
    AM_USBD_SUBCLASS,                       /* 设备子类 */
    AM_USBD_PROTOCOL,                       /* 协议码 */
    AM_USBD_MAX_EP_DATA_CNT,                /* 端点0的最大包大小 */

    /**
     * 厂商编号。需要向USB协会申请，如果作为学习使用可以随便选一个已经注册过的，
     * 但是作为产品发布的话就必须写自己公司的厂商编号，以免侵权，此处填了一个没有在USB协会注册的编号
     */
    AM_USB_SHORT_GET_LOW(__USBD_PRINTER_VENDOR_ID), AM_USB_SHORT_GET_HIGH(__USBD_PRINTER_VENDOR_ID),

    /* 产品编号 */
    AM_USB_SHORT_GET_LOW(__USBD_PRINTER_PRODUCT_ID), AM_USB_SHORT_GET_HIGH(__USBD_PRINTER_PRODUCT_ID),

    /* 设备出厂编号 */
    AM_USB_SHORT_GET_LOW(__USBD_PRINTER_DEVICE_ID), AM_USB_SHORT_GET_HIGH(__USBD_PRINTER_DEVICE_ID),
    __USBD_PRINTER_VENDOR_STRING_INDEX,     /* 描述厂商的字符串索引 */
    __USBD_PRINTER_PRODUCT_STRING_INDEX,    /* 描述产品的字符串索引 */
    0x00,                                   /* 描述设备序列号的字符串索引 */
    __USBD_PRINTER_CONFIG_COUNT,            /* 配置的数量（只能有一个） */
};

/* 配置描述符及其下级描述符（不能越过上级描述符直接得到下级描述符） */
static uint8_t __g_am_usbd_printer_desc_conf[AM_USB_DESC_LENGTH_ALL(__USBD_PRINTER_ENDPOINT_COUNT)] = {
    /* 配置描述符 */
    AM_USB_DESC_LENGTH_CONFIGURE,     /* 配置描述符字节数 */
    AM_USB_DESC_TYPE_CONFIGURE,       /* 配置描述符类型编号，固定为0x02 */

    /* 配置描述符及下属描述符的总长度(配置描述符，接口描述符号，和两个端点描述符) */
    AM_USB_SHORT_GET_LOW(AM_USB_DESC_LENGTH_ALL(__USBD_PRINTER_ENDPOINT_COUNT)),
    AM_USB_SHORT_GET_HIGH(AM_USB_DESC_LENGTH_ALL(__USBD_PRINTER_ENDPOINT_COUNT)),
    __USBD_PRINTER_INTERFACE_COUNT,         /* 接口描述符个数 */
    __USBD_PRINTER_CONFIGURE_INDEX,         /* 配置值 */
    0x00,                                   /* 描述该配置的字符串索引 */

    /* 设备属性：总线供电，不支持远程唤醒 */
    __USBD_PRINTER_DEVICE_ATTRIBUTE,
    __USBD_PRINTER_DEVICE_POWER,            /* 从总线获取的最大电流：100mA， 2mA一个单位 */

    /* 接口描述符 */
    AM_USB_DESC_LENGTH_INTERFACE,     /* 接口描述符字节数 */
    AM_USB_DESC_TYPE_INTERFACE,       /* 接口描述符类型编号，固定为0x04 */
    0x00,                                   /* 该接口编号 */
    0x00,                                   /* 可选设置的索引值（该接口的备用编号） */
    __USBD_PRINTER_ENDPOINT_COUNT,          /* 该接口使用的端点数（不包括端点0） */
    AM_USBD_CONFIG_PRINTER_CLASS_CODE,      /* printer_CLASS类 */
    AM_USBD_PRINTER_SUBCLASS,               /* printer子类型 */
    AM_USBD_PRINTER_PROTOCOL,               /* printer协议类型 */
    0x00,                                   /* 描述该接口的字符串索引 */

    /* 输入端点描述符 */
    AM_USB_DESC_LENGTH_ENDPOINT,            /* 端点描述符字节数 */
    AM_USB_DESC_TYPE_ENDPOINT,        /* 端点描述符类型编号，固定为0x05 */

    /* D7 1:USB_IN  0:USB_OUT D3:D0 端点号 */
    (__USBD_PRINTER_ENDPOINT_IN | (AM_USB_IN << AM_USB_REQ_TYPE_DIR_SHIFT)),
    __USBD_PRINTER_ENDPOINT_IN_ATTRIBUTE,   /* 端点属性 02表示批量  */

    AM_USB_SHORT_GET_LOW(__USBD_PRINTER_ENDPOINT_IN_PACKSIZE),
    AM_USB_SHORT_GET_HIGH(__USBD_PRINTER_ENDPOINT_IN_PACKSIZE), /* 端点一次性收发的最大包大小 */

    __USBD_PRINTER_ENDPOINT_IN_QUERY_TIME,  /* 主机查询端点时的时间间隔：10ms  */

    /* 输出端点描述符 */
    AM_USB_DESC_LENGTH_ENDPOINT,            /* 端点描述符字节数 */
    AM_USB_DESC_TYPE_ENDPOINT,        /* 端点描述符类型编号，固定为0x05 */

    /* 端点地址及输出属性 */
    (__USBD_PRINTER_ENDPOINT_OUT | (AM_USB_OUT << AM_USB_REQ_TYPE_DIR_SHIFT)),

    __USBD_PRINTER_ENDPOINT_OUT_ATTRIBUTE,  /* 端点属性 */

    AM_USB_SHORT_GET_LOW(__USBD_PRINTER_ENDPOINT_OUT_PACKSIZE),
    AM_USB_SHORT_GET_HIGH(__USBD_PRINTER_ENDPOINT_OUT_PACKSIZE), /* 端点一次性收发的最大包大小 */

    __USBD_PRINTER_ENDPOINT_OUT_QUERY_TIME  /* 主机查询端点时的时间间隔 10ms */
};

/*******************************************************************************
 * 字符串描述符,如果用户需要修改描述符信息，注意使用的是UINCODE码(注意大小端)
 ******************************************************************************/
/**< \brief 描述产品的字符串描述符 */
static const uint8_t __g_am_usbd_printer_desc_str_iproduct[18] = {
    sizeof(__g_am_usbd_printer_desc_str_iproduct),       /* 字符串描述符字节数 */
    AM_USB_DESC_TYPE_STRING,          /* 字符串描述符类型编号，固定为0x03 */

     0x55, 0x00, /* U */
     0x42, 0x00, /* S */
     0x53, 0x00, /* B */
     0x21, 0x6a, /* 模 */
     0xdf, 0x62, /* 拟 */
     0x53, 0x62, /* 打 */
     0x70, 0x53, /* 印 */
     0x73, 0x67, /* 机 */
};

/**< \brief 语言ID字符串描述符 */
/**< \brief 这里使用美式英语，不使用简体中文的原因是如果使用简体中文，则主机不会向从机要字符串描述符 */
/**< \brief 美式英语的语言ID为0x0409，简体中文的语言ID为0x0804，注意大小端。 */
static const uint8_t __g_am_usbd_printer_desc_str_language_id[4] = {
    sizeof(__g_am_usbd_printer_desc_str_language_id),       /* 字符串描述符字节数 */
    AM_USB_DESC_TYPE_STRING,          /* 字符串描述符类型编号，固定为0x03 */
    0x04,
    0x08,       /* 简体中文 */
};

/**< \brief 描述厂商的字符串描述符 */
static uint8_t __g_am_usbd_printer_desc_str_imanufacturer[22] = {
    sizeof(__g_am_usbd_printer_desc_str_imanufacturer),       /* 字符串描述符字节数 */
    AM_USB_DESC_TYPE_STRING,          /* 字符串描述符类型编号，固定为0x03 */
    0x7f, 0x5e, /* 广 */
    0xde, 0x5d, /* 州 */
    0xf4, 0x81, /* 致 */
    0xdc, 0x8f, /* 远 */
    0x35, 0x75, /* 电 */
    0x50, 0x5b, /* 子 */
    0x09, 0x67, /* 有 */
    0x50, 0x96, /* 限 */
    0x6c, 0x51, /* 公 */
    0xf8, 0x53, /* 司 */
};


/******************************************************************************
 * 各描述符信息
 *****************************************************************************/
static const am_usbd_descriptor_t __g_am_usbd_printer_descriptor[] = {
    /* 设备描述符 */
    {
        (AM_USB_DESC_TYPE_DEVICE << 8) | (0x00),
        sizeof(__g_am_usbd_printer_desc_dev),
        __g_am_usbd_printer_desc_dev
    },

    /* 配置描述符及其下级描述符 */
    {
        (AM_USB_DESC_TYPE_CONFIGURE << 8) | (0x00),
        sizeof(__g_am_usbd_printer_desc_conf),
        __g_am_usbd_printer_desc_conf
    },

    /* 字符串描述符0，描述语言id */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | (0x00),
        sizeof(__g_am_usbd_printer_desc_str_language_id),
        __g_am_usbd_printer_desc_str_language_id
    },

    /* 字符串描述符1，描述厂商 */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | __USBD_PRINTER_VENDOR_STRING_INDEX,
        sizeof(__g_am_usbd_printer_desc_str_imanufacturer),
        __g_am_usbd_printer_desc_str_imanufacturer
    },

    /* 字符串描述符2，描述产品 */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | __USBD_PRINTER_PRODUCT_STRING_INDEX,
        sizeof(__g_am_usbd_printer_desc_str_iproduct),
        __g_am_usbd_printer_desc_str_iproduct
    },
};

/******************************************************************************
 * 平台初始化函数、解初始化函数已经设备信息
 ******************************************************************************/
/**
 * \brief 平台初始化
 */
static void __am_usbd_printer_init (void) {
	
    /* 使能时钟 */
    amhw_hc32f07x_rcc_set_start(0x5A5A);
    amhw_hc32f07x_rcc_set_start(0xA5A5);
    amhw_hc32f07x_rcc_usbclk_adjust_set (AMHW_HC32F07X_USBCLK_ADJUST_PLL);
    am_clk_enable(CLK_USB);
}

/**
 * \brief 平台去初始化
 */
static void __am_usbd_printer_deinit (void) {
    am_clk_disable(CLK_USB);
    amhw_hc32f07x_rcc_peripheral_disable (AMHW_HC32F07X_RCC_USB);       /* 禁能USB时钟 */                            /* 禁能USB时钟 */
}

static const am_usbd_devinfo_t __g_usbd_info = {
        __g_am_usbd_printer_descriptor,                                                         /* 描述符地址 */
        sizeof(__g_am_usbd_printer_descriptor) / sizeof(__g_am_usbd_printer_descriptor[0]),     /* 描述符个数 */
};

/**< \brief 定义USB设备信息 */
static const am_hc32f07x_usbd_devinfo_t  __g_hc32f07x_usbd_printer_info = {
    HC32F07X_USB_BASE,                           /* 寄存器基地址 */
    INUM_USB,                                  /* 中断号 */
    __am_usbd_printer_init,           /**< \brief 平台初始化 */
    __am_usbd_printer_deinit,         /**< \brief 平台去初始化 */
    &__g_usbd_info,
};

/** \brief USB打印机信息结构体*/
static am_usbd_printer_info_t __g_usbd_printer_info = {
        __g_printer_id,               /**< \brief 打印机id */
        sizeof(__g_printer_id) - 1,   /**< \brief 打印机id长度,(减一为了消除‘\0’)*/
        __g_printer_buff,             /**< \brief 打印机使用buff */
};

/** \brief 打印机使用handle(USB设备类) */
static am_usbd_printer_t     __g_usb_printer_dev;

/** \brief 用于hc32f07x 的USB handle*/
static am_hc32f07x_usbd_dev_t  __g_hc32f07x_usb_printer_dev;

/** \brief usb_printer实例初始化，获得usb_printer标准服务句柄 */
am_usbd_printer_handle am_hc32f07x_usbd_printer_inst_init (void)
{
    return am_usbd_printer_init(&__g_usb_printer_dev,
                                &__g_usbd_printer_info,
                                 am_hc32f07x_usbd_init(&__g_hc32f07x_usb_printer_dev, &__g_hc32f07x_usbd_printer_info));
}

/** \brief usb_printer解初始化，获得usb_printer标准服务句柄 */
void am_hc32f07x_usbd_printer_inst_deinit (void)
{
    am_usbd_printer_deinit(&__g_usb_printer_dev);
}

/**
 * @}
 */

/* end of file */
