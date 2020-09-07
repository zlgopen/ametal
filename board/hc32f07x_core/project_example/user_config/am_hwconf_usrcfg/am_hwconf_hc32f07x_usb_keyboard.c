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
 * \brief hc32f07x USB_keyboard 用户配置文件
 * \sa am_hc32f07x_hwconfig_usb_keyboard.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-19  adw, first implementation.
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
#include "am_usbd_keyboard.h"
#include "am_hc32f07x_usbd.h"

/**
 * \addtogroup am_hc32f07x_if_hwconfig_src_usb_keyboard
 * \copydoc am_hc32f07x_hwconfig_usb_keyboard.c
 * @{
 */

/*******************************************************************************
 * 用户USB描述符配置宏,用户配置描述符宏即可,无需关心USB描述符。
 ******************************************************************************/

/** \brief USB设备描述配置宏*/
#define __USBD_KEYBOARD_VENDOR_ID                 (0x1FC9) /**< \brief 厂商编号,作为产品发布必须写自己公司的厂商编号，以免侵权，此处填了一个没有在USB协会注册的编号*/
#define __USBD_KEYBOARD_PRODUCT_ID                (0x7C03) /**< \brief 产品编号*/
#define __USBD_KEYBOARD_DEVICE_ID                 (0x0002) /**< \brief 设备编号*/
#define __USBD_KEYBOARD_VENDOR_STRING_INDEX       (0x01)   /**< \brief 描述厂商的字符串索引*/
#define __USBD_KEYBOARD_PRODUCT_STRING_INDEX      (0x02)   /**< \brief 描述产品的字符串索引 */
#define __USBD_KEYBOARD_CONFIG_COUNT              (1U)     /**< \brief 配置数，一般设置只有一个配置*/

/** \brief USB配置描述符配置宏*/
#define __USBD_KEYBOARD_INTERFACE_COUNT           (1U)     /**< \brief 接口数*/
#define __USBD_KEYBOARD_CONFIGURE_INDEX           (1U)     /**< \brief 配置描述符索引*/
#define __USBD_KEYBOARD_DEVICE_POWER               AM_USBD_MAX_POWER /**< \brief 设备向主机申请的电流，单位2mA,最大100mA*/
/** \brief 设备属性，自供电，不支持远程唤醒*/
#define __USBD_KEYBOARD_DEVICE_ATTRIBUTE          \
              (AM_USBD_CONFIG_SELF_POWER | AM_USBD_CONFIG_NOT_REMOTE_WAKEUP)

#define __USBD_KEYBOARD_COUNTRYCODE                (0x21U) /**< \brief 国家\地区代码，0x21表示美国（因为键盘一般是美式键盘） */
#define __USBD_KEYBOARD_DESC_NUMBER                (1U)    /**< \brief 下级描述符数量*/


/**\brief USB端点描述符配置宏*/
#define __USBD_KEYBOARD_ENDPOINT_IN                3       /**< \brief 输入端点号*/
#define __USBD_KEYBOARD_ENDPOINT_IN_PACKSIZE       AM_USBD_MAX_EP_DATA_CNT       /**< \brief 端点包大小,设置为64*/
#define __USBD_KEYBOARD_ENDPOINT_IN_ATTRIBUTE      AM_USB_ENDPOINT_INTERRUPT     /**< \brief 设置端点属性为批量传输*/
#define __USBD_KEYBOARD_ENDPOINT_IN_QUERY_TIME    (0x0A)                         /**< \brief 设置端点查询时间为10ms,单位为1ms*/

#define __USBD_KEYBOARD_ENDPOINT_OUT               4
#define __USBD_KEYBOARD_ENDPOINT_OUT_PACKSIZE      AM_USBD_MAX_EP_DATA_CNT
#define __USBD_KEYBOARD_ENDPOINT_OUT_ATTRIBUTE     AM_USB_ENDPOINT_INTERRUPT
#define __USBD_KEYBOARD_ENDPOINT_OUT_QUERY_TIME   (0x0A)

/**\brief USB接口描述符配置宏*/
#define __USBD_KEYBOARD_ENDPOINT_COUNT            (2U)     /**< \brief 端点个数,输入端点与输出端点(不包括控制端点)*/

/*****************************************************************************
 * USB描述符(设备描述符,配置描述符，接口描述符,端点描述符),该部分用户只需配置上述宏即可
 *****************************************************************************/

/* USB 设备描述符 */
static const uint8_t __g_am_usbd_keyboard_desc_dev[AM_USB_DESC_LENGTH_DEVICE]  = {
    AM_USB_DESC_LENGTH_DEVICE,        /* 设备描述符的字节数 */
    AM_USB_DESC_TYPE_DEVICE,          /* 设备描述符类型编号，固定为0x01 */

    /* USB版本  */
    AM_USB_SHORT_GET_LOW(AM_USB_VERSION), AM_USB_SHORT_GET_HIGH(AM_USB_VERSION),
    AM_USBD_CLASS,                          /* 通信类 */
    AM_USBD_SUBCLASS,                       /* 设备子类 */
    AM_USBD_PROTOCOL,                       /* 协议码 */
    AM_USBD_MAX_EP_DATA_CNT,                /* 端点0的最大包大小 */

    /**
     * 厂商编号。需要向USB协会申请，如果作为学习使用可以随便选一个已经注册过的，
     * 但是作为产品发布的话就必须写自己公司的厂商编号，以免侵权，此处填了一个没有在USB协会注册的编号
     */
    AM_USB_SHORT_GET_LOW(__USBD_KEYBOARD_VENDOR_ID), AM_USB_SHORT_GET_HIGH(__USBD_KEYBOARD_VENDOR_ID),

    /* 产品编号 */
    AM_USB_SHORT_GET_LOW(__USBD_KEYBOARD_PRODUCT_ID), AM_USB_SHORT_GET_HIGH(__USBD_KEYBOARD_PRODUCT_ID),

    /* 设备出厂编号 */
    AM_USB_SHORT_GET_LOW(__USBD_KEYBOARD_DEVICE_ID), AM_USB_SHORT_GET_HIGH(__USBD_KEYBOARD_DEVICE_ID),
    __USBD_KEYBOARD_VENDOR_STRING_INDEX,     /* 描述厂商的字符串索引 */
    __USBD_KEYBOARD_PRODUCT_STRING_INDEX,    /* 描述产品的字符串索引 */
    0x00,                                   /* 描述设备序列号的字符串索引 */
    __USBD_KEYBOARD_CONFIG_COUNT,            /* 配置的数量（只能有一个） */
};

/** \brief HID设备报告描述符,用USB_HID 工具选择生成 */
/** \brief 把该描述符写在配置描述符之前，是因为在配置描述符中需要知道报告描述符中的长度 */
static const uint8_t __g_keyboard_hid_report[] = {
    /* 每行开始第一个字节为该条目的前缀，[D7:D4]：bTag [D3:D2]：bType [D1:D0]：bSize */

    /* 全局条目，用途页选择为普通桌面页 */
    0x05, 0x01, /* Usage Page (Generic Desktop) */

    /* 局部条目，用途选择为键盘 */
    0x09, 0x06, /* Usage (Keyboard) */

    /* 主条目，开集合，表示为应用集合，它的性质由用途页和用途定义为了普通桌面的键盘 */
    0xa1, 0x01, /* Collection (Application) */

    /* 全局条目，选择用途页为键盘 */
    0x05, 0x07, /* Usage Page (Keyboard/Keypad) */

    /* 局部条目，用途的最小值为0xe0，其实是键盘上左ctrl键。具体的用途值可在HID用途表中查看 */
    0x19, 0xe0, /* Usage Minimum (Keyboard LeftControl) */

    /* 局部条目，用途的最大值为0xe7，其实是键盘右GUI(windows)键 */
    0x29, 0xe7, /* Usage Maximum (Keyboard Right GUI) */

    /* 全局条目，说明返回数据的逻辑值（就是我们返回数据域中的值）的最小值 */
    0x15, 0x00, /* Logical Mimimum (0) */

    /* 全局条目，说明返回数据的逻辑值的最大值 */
    0x25, 0x01, /* Logical Maximum (1) */

    /* 全局条目，说明数据域的数量为8个 */
    0x95, 0x08, /* Report Count (8) */

    /* 全局条目，说明每个数据域的长度为一个位 */
    0x75, 0x01, /* Report Size (1) */

    /* 主条目，说明有8个长度为1位的数据域（数量和长度由前两个全局条目所定义）用来作为输入 */
    0x81, 0x02, /* Input (Data,Var,Abs) ，Data表示这些数据可以变动；Var表示这些数据域是独立的，每个域表示一个意思；Abs表示绝对值。*/
    /* 这样定义的结果就是，当某个域的值为1时，就表示对应的按键被按下，位0对应着用途最小值0xe0，位7对应着用途最大值0xe7 */

    /* 全局条目，说明数据域的数量为1个 */
    0x95, 0x01, /* Report Count (1)*/

    /* 全局条目，说明每个数据域的长度为8位 */
    0x75, 0x08, /* Report Size (8) */

    /* 主条目，输入用。由前面两个全局条目可知，长度为8位，数量为1个 */
    /* 它的属性为常量，返回的数据一直是0（保留给OEM使用） */
    0x81, 0x03, /* INPUT (Cnst,Var,Abs) */

    /* 全局条目，说明数据域的数量为6个 */
    0x95, 0x06, /* Report Count (6)*/

    /* 全局条目，说明每个数据域的长度为8位 */
    /* 其实这个条目不要也是可以的，因为前面已经有一个定义长度为8我的全局条目了 */
    0x75, 0x08, /* Report Size (8) */

    /* 全局条目，定义逻辑最小值为0 */
    /* 这个条目也可以不要，因为前面已经有一个定义逻辑最小值为0的条目了 */
    0x15, 0x00, /* Logical Mimimum (0) */

    /* 全局条目，定义逻辑最大值为255 */
    0x25, 0xff, /* Logical Maximum (255) */

    /* 全局条目，定义用途页为键盘 */
    /* 前面已经有了，因此该条目不要也可以 */
    0x05, 0x07, /* Usage Page (Keyboard/Keypad) */

    /* 局部条目，定义用途最小值为0，0表示没有键按下 */
    0x19, 0x00, /* Usage Mimimum (Reserved(no event indicated)) */

    /* 局部条目，定义用途最大值为0x65 */
    0x29, 0x65, /* Usage Maximum */

    /* 主条目，说明这6个8位的数据是输入用的，属性为Data,Ary,Abs */
    /* 如果按下的键太多（例如超过这里定义的长度，或者键盘本身无法扫描出按键的情况时），则这些数据全部返回全0xff，表示按键无效 */
    0x81, 0x00, /* Input(Data,Ary,Abs) */

    /* 以下为输出报告的描述 */

    /* 逻辑最小值前面已经定义过了，这里省略 */
    /* 全局条目，逻辑最大值为1 */
    0x25, 0x01, /* Logical Maximum (1) */

    /* 全局条目，说明数据域的数量有5个 */
    0x95, 0x05, /* Report Count (5) */

    /* 全局条目，说明数据域的长度为1位 */
    0x75, 0x01, /* Report Size (1) */

    /* 全局条目，说明使用的用途页为LED灯 */
    0x05, 0x08, /* Usage Page (LEDs) */

    /* 局部条目，说明用途最小值为数字键盘灯 */
    0x19, 0x01, /* Usage Minimum (Num Lock) */

    /* 局部条目，说明用途最大值为Kana灯 */
    0x29, 0x05, /* Usage Maximum (Kana) */

    /* 主条目，定义输出数据 */
    0x91, 0x02, /* Output (Data,Var,Abs) */

    /* 全局条目，定义位域数量为1个 */
    0x95, 0x01, /* Report Count (1) */

    /* 全局条目，定义位域长度为3位 */
    0x75, 0x03, /* Report Size (3) */

    /* 主条目，定义输出常量，前面用了5个位，所以这里要用三个位凑成1个字节 */
    0x91, 0x03, /* Output (Cnst,Var,Abs) */

    /* 主条目，用来关闭前面的集合，bSize为0，表示后面没有数据 */
    0xc0        /* END Collection */
};

/* 配置描述符及其下级描述符（不能越过上级描述符直接得到下级描述符） */
static uint8_t __g_am_usbd_keyboard_desc_conf[AM_USB_DESC_LENGTH_ALL(__USBD_KEYBOARD_ENDPOINT_COUNT)
                                             + sizeof(am_usb_hid_descriptor_t)] = {
    /* 配置描述符 */
    AM_USB_DESC_LENGTH_CONFIGURE,     /* 配置描述符字节数 */
    AM_USB_DESC_TYPE_CONFIGURE,       /* 配置描述符类型编号，固定为0x02 */

    /* 配置描述符及下属描述符的总长度(配置描述符，接口描述符号，和两个端点描述符) */
    AM_USB_SHORT_GET_LOW(sizeof(__g_am_usbd_keyboard_desc_conf)),
    AM_USB_SHORT_GET_HIGH(sizeof(__g_am_usbd_keyboard_desc_conf)),
    __USBD_KEYBOARD_INTERFACE_COUNT,        /* 接口描述符个数 */
    __USBD_KEYBOARD_CONFIGURE_INDEX,        /* 配置值 */
    0x00,                                   /* 描述该配置的字符串索引 */

    /* 设备属性：总线供电，不支持远程唤醒 */
    __USBD_KEYBOARD_DEVICE_ATTRIBUTE,
    __USBD_KEYBOARD_DEVICE_POWER,           /* 从总线获取的最大电流：100mA， 2mA一个单位 */

    /* 接口描述符 */
    AM_USB_DESC_LENGTH_INTERFACE,           /* 接口描述符字节数 */
    AM_USB_DESC_TYPE_INTERFACE,             /* 接口描述符类型编号，固定为0x04 */
    0x00,                                   /* 该接口编号 */
    0x00,                                   /* 可选设置的索引值（该接口的备用编号） */
    __USBD_KEYBOARD_ENDPOINT_COUNT,         /* 该接口使用的端点数（不包括端点0） */
    AM_USBD_CONFIG_KEYBOARD_CLASS_CODE,     /* HID设备的类 */
    AM_USBD_KEYBOARD_SUBCLASS,              /* HID子类型, 1：键盘 2：鼠标*/
    AM_USBD_KEYBOARD_PROTOCOL,              /* 1表示键盘 */
    0x00,                                   /* 描述该接口的字符串索引 */

    /* HID描述符 */
    AM_USB_DESC_LENGTH_HID,                 /* HID描述符字节数 */
    AM_USB_DESC_TYPE_HID,       /* HID描述符类型编号，固定为0x21 */
    0x10, 0x01,  /* HID协议版本号 */
    __USBD_KEYBOARD_COUNTRYCODE,       /* 国家\地区代码 */
    __USBD_KEYBOARD_DESC_NUMBER,       /* 下级描述符的数量 */
    AM_USB_DESC_TYPE_HID_REPORT,       /* 下级描述符的类型：报告描述符为0x22 */
    AM_USB_SHORT_GET_LOW(sizeof(__g_keyboard_hid_report)), /* 下级描述符的长度 */
    AM_USB_SHORT_GET_HIGH(sizeof(__g_keyboard_hid_report)),

    /* 输入端点描述符 */
    AM_USB_DESC_LENGTH_ENDPOINT,      /* 端点描述符字节数 */
    AM_USB_DESC_TYPE_ENDPOINT,        /* 端点描述符类型编号，固定为0x05 */

    /* D7 1:USB_IN  0:USB_OUT D3:D0 端点号 */
    (__USBD_KEYBOARD_ENDPOINT_IN | (AM_USB_IN << AM_USB_REQ_TYPE_DIR_SHIFT)),
    __USBD_KEYBOARD_ENDPOINT_IN_ATTRIBUTE,   /* 端点属性 02表示批量  */

    AM_USB_SHORT_GET_LOW(__USBD_KEYBOARD_ENDPOINT_IN_PACKSIZE),
    AM_USB_SHORT_GET_HIGH(__USBD_KEYBOARD_ENDPOINT_IN_PACKSIZE), /* 端点一次性收发的最大包大小 */

    __USBD_KEYBOARD_ENDPOINT_IN_QUERY_TIME,  /* 主机查询端点时的时间间隔：10ms  */

    /* 输出端点描述符 */
    AM_USB_DESC_LENGTH_ENDPOINT,            /* 端点描述符字节数 */
    AM_USB_DESC_TYPE_ENDPOINT,        /* 端点描述符类型编号，固定为0x05 */

    /* 端点地址及输出属性 */
    (__USBD_KEYBOARD_ENDPOINT_OUT | (AM_USB_OUT << AM_USB_REQ_TYPE_DIR_SHIFT)),

    __USBD_KEYBOARD_ENDPOINT_OUT_ATTRIBUTE,  /* 端点属性 */

    AM_USB_SHORT_GET_LOW(__USBD_KEYBOARD_ENDPOINT_OUT_PACKSIZE),
    AM_USB_SHORT_GET_HIGH(__USBD_KEYBOARD_ENDPOINT_OUT_PACKSIZE), /* 端点一次性收发的最大包大小 */

    __USBD_KEYBOARD_ENDPOINT_OUT_QUERY_TIME  /* 主机查询端点时的时间间隔 10ms */
};

/*******************************************************************************
 * 字符串描述符,如果用户需要修改描述符信息，注意使用的是UINCODE码(注意大小端)
 ******************************************************************************/
/**< \brief 描述产品的字符串描述符 */
static const uint8_t __g_am_usbd_keyboard_desc_str_iproduct[16] = {
    sizeof(__g_am_usbd_keyboard_desc_str_iproduct),       /* 字符串描述符字节数 */
    AM_USB_DESC_TYPE_STRING,          /* 字符串描述符类型编号，固定为0x03 */

     0x55, 0x00, /* U */
     0x42, 0x00, /* S */
     0x53, 0x00, /* B */
     0x21, 0x6a, /* 模 */
     0xdf, 0x62, /* 拟 */
     0x2e, 0x95, /* 键 */
     0xd8, 0x76, /* 盘 */
};

/**< \brief 语言ID字符串描述符 */
/**< \brief 这里使用美式英语，不使用简体中文的原因是如果使用简体中文，则主机不会向从机要字符串描述符 */
/**< \brief 美式英语的语言ID为0x0409，简体中文的语言ID为0x0804，注意大小端。 */
static const uint8_t __g_am_usbd_keyboard_desc_str_language_id[4] = {
    sizeof(__g_am_usbd_keyboard_desc_str_language_id),       /* 字符串描述符字节数 */
    AM_USB_DESC_TYPE_STRING,          /* 字符串描述符类型编号，固定为0x03 */
    0x04,
    0x08,       /* 简体中文 */
};

/**< \brief 描述厂商的字符串描述符 */
static uint8_t __g_am_usbd_keyboard_desc_str_imanufacturer[22] = {
    sizeof(__g_am_usbd_keyboard_desc_str_imanufacturer),       /* 字符串描述符字节数 */
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
static const am_usbd_descriptor_t __g_am_usbd_keyboard_descriptor[] = {
    /* 设备描述符 */
    {
        (AM_USB_DESC_TYPE_DEVICE << 8) | (0x00),
        sizeof(__g_am_usbd_keyboard_desc_dev),
        __g_am_usbd_keyboard_desc_dev
    },

    /* 配置描述符及其下级描述符 */
    {
        (AM_USB_DESC_TYPE_CONFIGURE << 8) | (0x00),
        sizeof(__g_am_usbd_keyboard_desc_conf),
        __g_am_usbd_keyboard_desc_conf
    },

    /* 字符串描述符0，描述语言id */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | (0x00),
        sizeof(__g_am_usbd_keyboard_desc_str_language_id),
        __g_am_usbd_keyboard_desc_str_language_id
    },

    /* 字符串描述符1，描述厂商 */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | __USBD_KEYBOARD_VENDOR_STRING_INDEX,
        sizeof(__g_am_usbd_keyboard_desc_str_imanufacturer),
        __g_am_usbd_keyboard_desc_str_imanufacturer
    },

    /* 字符串描述符2，描述产品 */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | __USBD_KEYBOARD_PRODUCT_STRING_INDEX,
        sizeof(__g_am_usbd_keyboard_desc_str_iproduct),
        __g_am_usbd_keyboard_desc_str_iproduct
    },

    /* HID 报表描述符 */
    {
        (AM_USB_DESC_TYPE_HID_REPORT << 8) | (0x00),
        sizeof(__g_keyboard_hid_report),
        __g_keyboard_hid_report
    },
};

/******************************************************************************
 * 平台初始化函数、解初始化函数已经设备信息
 ******************************************************************************/
/**
 * \brief 平台初始化
 */
static void __am_usbd_keyboard_init (void) {
	
    /* 使能时钟 */
    amhw_hc32f07x_rcc_set_start(0x5A5A);
    amhw_hc32f07x_rcc_set_start(0xA5A5);
    amhw_hc32f07x_rcc_usbclk_adjust_set (AMHW_HC32F07X_USBCLK_ADJUST_PLL);
    am_clk_enable(CLK_USB);
}

/**
 * \brief 平台去初始化
 */
static void __am_usbd_keyboard_deinit (void) {
    am_clk_disable(CLK_USB);
    amhw_hc32f07x_rcc_peripheral_disable (AMHW_HC32F07X_RCC_USB);       /* 禁能USB时钟 */         
}

static const am_usbd_devinfo_t __g_usbd_info = {
        __g_am_usbd_keyboard_descriptor,                                                         /* 描述符地址 */
        sizeof(__g_am_usbd_keyboard_descriptor) / sizeof(__g_am_usbd_keyboard_descriptor[0]),     /* 描述符个数 */
};

/**< \brief 定义USB设备信息 */
static const am_hc32f07x_usbd_devinfo_t  __g_hc32f07x_usbd_keyboard_info = {
    HC32F07X_USB_BASE,                           /* 寄存器基地址 */
    INUM_USB,                                  /* 中断号 */
    __am_usbd_keyboard_init,          /**< \brief 平台初始化 */
    __am_usbd_keyboard_deinit,        /**< \brief 平台去初始化 */
    &__g_usbd_info,
};

/** \brief USB打印机信息结构体*/
static am_usbd_keyboard_info_t __g_usbd_keyboard_info = {
	__USBD_KEYBOARD_ENDPOINT_IN,
	__USBD_KEYBOARD_ENDPOINT_OUT
};

/** \brief USB keyboard 设备实例 */
static am_usbd_keyboard_t __g_usbd_keyboard_dev;

/** \brief hc32f07x USB设备实例 */
static am_hc32f07x_usbd_dev_t  __g_hc32f07x_usbd_keyboard;


/** \brief usb_keyboard实例初始化，获得usb_keyboard标准服务句柄 */
am_usbd_keyboard_handle am_hc32f07x_usbd_keyboard_inst_init (void)
{
    return am_usbd_keyboard_init (&__g_usbd_keyboard_dev,
                                  &__g_usbd_keyboard_info,
								                  am_hc32f07x_usbd_init(&__g_hc32f07x_usbd_keyboard, &__g_hc32f07x_usbd_keyboard_info));
}

/** \brief usb_keyboard解初始化，获得usb_keyboard标准服务句柄 */
void am_hc32f07x_usbd_keyboard_inst_deinit (void)
{
    am_usbd_keyboard_deinit (&__g_usbd_keyboard_dev);
}

/**
 * @}
 */

/* end of file */
