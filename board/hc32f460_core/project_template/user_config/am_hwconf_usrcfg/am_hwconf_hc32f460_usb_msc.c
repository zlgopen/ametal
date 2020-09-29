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
 * \brief hc32f460 USB_MSC 用户配置文件
 * \sa am_hc32f460_hwconfig_usb_msc.c
 *
 * \internal
 * \par Modification history
 *
 * - 1.10 19-01-11  adw, add configuration macro
 * - 1.00 18-10-29  enf, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_types.h"
#include "am_clk.h"
#include "am_usb.h"
#include "am_gpio.h"
#include "am_usbd.h"
#include "am_usbd_msc.h"
#include "am_hc32f460.h"
#include "am_hc32f460_usbd.h"
#include "amhw_hc32f460_usbd.h"
#include "am_usbd_msc.h"
#include "usb_core.h"
#include "usbd_desc.h"
#include "usbd_usr.h"
#include "usbd_msc_core.h"
/**
 * \addtogroup am_hc32f460_if_hwconfig_src_usb_msc
 * \copydoc am_hc32f460_hwconfig_usb_msc.c
 * @{
 */

/*******************************************************************************
 * 用户USB描述符配置宏,用户配置描述符宏即可,无需关心USB描述符。
 ******************************************************************************/
#define __USBD_MSC_SYS_WIN7         (0)
#define __USBD_MSC_SYS_WIN10        (1)


#define __USBD_MSC_FILE_CREAT_TIME1    (15U)
#define __USBD_MSC_FILE_CREAT_TIME2    (48U)
#define __USBD_MSC_FILE_CREAT_TIME3    (26U)

#define __USBD_MSC_FILE_CREAT_DATE1    (2018U)
#define __USBD_MSC_FILE_CREAT_DATE2    (10U)
#define __USBD_MSC_FILE_CREAT_DATE3    (29U)

#define __USBD_MSC_FILE_MODIFIED_TIME1 (15U)
#define __USBD_MSC_FILE_MODIFIED_TIME2 (36U)
#define __USBD_MSC_FILE_MODIFIED_TIME3 (47U)

#define __USBD_MSC_FILE_MODIFIED_DATE1 (2018U)
#define __USBD_MSC_FILE_MODIFIED_DATE2 (11U)
#define __USBD_MSC_FILE_MODIFIED_DATE3 (2U)

/** \brief USB设备描述配置宏*/
#define __USBD_MSC_VENDOR_ID                 (0x4195) /**< \brief 厂商编号,作为产品发布必须写自己公司的厂商编号，以免侵权，此处填了一个没有在USB协会注册的编号*/
#define __USBD_MSC_PRODUCT_ID                (0x6515) /**< \brief 产品编号*/
#define __USBD_MSC_DEVICE_ID                 (0x0101) /**< \brief 设备编号*/
#define __USBD_MSC_VENDOR_STRING_INDEX       (0x01)   /**< \brief 描述厂商的字符串索引*/
#define __USBD_MSC_PRODUCT_STRING_INDEX      (0x02)   /**< \brief 描述产品的字符串索引 */
#define __USBD_MSC_DEVICE_STRING_INDEX       (0x03)   /**< \brief 描述设备序列号的字符串索引*/
#define __USBD_CONFIGURATION_COUNT           (1U)     /**< \brief 配置数，一般设置只有一个配置*/

/** \brief USB配置描述符配置宏*/
#define __USBD_MSC_INTERFACE_COUNT           (1U)               /**< \brief 接口数*/
#define __USBD_MSC_CONFIGURE_INDEX           (1U)               /**< \brief 配置描述符索引*/
#define __USBD_MSC_DEVICE_POWER               AM_USBD_MAX_POWER /**< \brief 设备向主机申请的电流，单位2mA,最大100mA*/
/** \brief 设备属性，自供电，不支持远程唤醒*/
#define __USBD_MSC_DEVICE_ATTRIBUTE          \
              (AM_USBD_CONFIG_SELF_POWER | AM_USBD_CONFIG_NOT_REMOTE_WAKEUP)

/**\brief USB端点描述符配置宏*/
#define __USBD_MSC_ENDPOINT_IN               (1U)                     /**< \brief 输入端点号*/
#define __USBD_MSC_ENDPOINT_IN_PACKSIZE      AM_USBD_MAX_EP_DATA_CNT  /**< \brief 端点包大小,设置为64*/
#define __USBD_MSC_ENDPOINT_IN_ATTRIBUTE     AM_USB_ENDPOINT_BULK     /**< \brief 设置端点属性为批量传输*/
#define __USBD_MSC_ENDPOINT_IN_QUERY_TIME    (0x01)                   /**< \brief 设置端点查询时间为10ms,单位为1ms*/

#define __USBD_MSC_ENDPOINT_OUT              (2)
#define __USBD_MSC_ENDPOINT_OUT_PACKSIZE     AM_USBD_MAX_EP_DATA_CNT
#define __USBD_MSC_ENDPOINT_OUT_ATTRIBUTE    AM_USB_ENDPOINT_BULK
#define __USBD_MSC_ENDPOINT_OUT_QUERY_TIME   (0x01)

/**\brief USB接口描述符配置宏*/
#define __USBD_MSC_ENDPOINT_COUNT            (2U)     /**< \brief 端点个数,输入端点与输出端点(不包括控制端点)*/

/*****************************************************************************
 * USB描述符(设备描述符,配置描述符，接口描述符,端点描述符),该部分用户只需配置上述宏即可
 *****************************************************************************/
/**\brief 设备描述符 */
static const uint8_t __g_usb_msc_desc_dev[AM_USB_DESC_LENGTH_DEVICE]  = {
    AM_USB_DESC_LENGTH_DEVICE,       /**< \brief 设备描述符的字节数 */
    AM_USB_DESC_TYPE_DEVICE,         /**< \brief 设备描述符类型编号，固定为0x01 */

    /**\brief USB版本 USB2.0 */
    AM_USB_SHORT_GET_LOW(AM_USB_VERSION), AM_USB_SHORT_GET_HIGH(AM_USB_VERSION),

    AM_USBD_CLASS,                   /**< \brief 通信类 */
    AM_USBD_SUBCLASS,                /**< \brief 设备子类 */
    AM_USBD_PROTOCOL,                /**< \brief 协议码 */
    AM_USBD_MAX_EP_DATA_CNT,         /**< \brief 端点0的最大包大小 */

    /**
     * 厂商编号。需要向USB协会申请，如果作为学习使用可以随便选一个已经注册过的，
     * 但是作为产品发布的话就必须写自己公司的厂商编号，以免侵权，此处填了一个没有在USB协会注册的编号
     */
    AM_USB_SHORT_GET_LOW(__USBD_MSC_VENDOR_ID), AM_USB_SHORT_GET_HIGH(__USBD_MSC_VENDOR_ID),

    /**\brief 产品编号 */
    AM_USB_SHORT_GET_LOW(__USBD_MSC_PRODUCT_ID), AM_USB_SHORT_GET_HIGH(__USBD_MSC_PRODUCT_ID),

    /**\brief 设备出厂编号 */
    AM_USB_SHORT_GET_LOW(__USBD_MSC_DEVICE_ID), AM_USB_SHORT_GET_HIGH(__USBD_MSC_DEVICE_ID),

    __USBD_MSC_VENDOR_STRING_INDEX,    /**< \brief 描述厂商的字符串索引 */
    __USBD_MSC_PRODUCT_STRING_INDEX,   /**< \brief 描述产品的字符串索引 */
    __USBD_MSC_DEVICE_STRING_INDEX,    /**< \brief 描述设备序列号的字符串索引 */
    __USBD_CONFIGURATION_COUNT,        /**< \brief 配置的数量（只能有一个） */
};

/**\brief 配置描述符及其下级描述符（不能越过上级描述符直接得到下级描述符） */
static uint8_t __g_usb_msc_desc_conf[AM_USB_DESC_LENGTH_ALL(__USBD_MSC_ENDPOINT_COUNT)] = {
    /**\brief 配置描述符 */
    AM_USB_DESC_LENGTH_CONFIGURE,     /**< \brief 配置描述符字节数 */
    AM_USB_DESC_TYPE_CONFIGURE,       /**< \brief 配置描述符类型编号，固定为0x02 */

    /**\brief 配置描述符及下属描述符的总长度 */
    AM_USB_SHORT_GET_LOW(AM_USB_DESC_LENGTH_ALL(__USBD_MSC_ENDPOINT_COUNT)),
    AM_USB_SHORT_GET_HIGH(AM_USB_DESC_LENGTH_ALL(__USBD_MSC_ENDPOINT_COUNT)),
    __USBD_MSC_INTERFACE_COUNT,       /**< \brief 接口描述符个数 */
    __USBD_MSC_CONFIGURE_INDEX,       /**< \brief 配置值 */
    0x00,                             /**< \brief 描述该配置的字符串索引,0x00表示没有 */

    /**\brief 设备属性：总线供电，不支持远程唤醒 */
    __USBD_MSC_DEVICE_ATTRIBUTE,
    __USBD_MSC_DEVICE_POWER,          /**< \brief 从总线获取的最大电流：100mA， 2mA一个单位 */

    /**\brief 接口描述符 */
    AM_USB_DESC_LENGTH_INTERFACE,     /**< \brief 接口描述符字节数 */
    AM_USB_DESC_TYPE_INTERFACE,       /**< \brief 接口描述符类型编号，固定为0x04 */
    0x00,                             /**< \brief 该接口编号,接口描述符必须从0开始,以此类推 */
    0x00,                             /**< \brief 可选设置的索引值（该接口的备用编号） */
    __USBD_MSC_ENDPOINT_COUNT,        /**< \brief 该接口使用的端点数（不包括端点0） */
    AM_USBD_CONFIG_MSC_CLASS_CODE,    /**< \brief MSC_CLASS类 */
    AM_USBD_MSC_SUBCLASS,             /**< \brief msc子类型 */
    AM_USBD_MSC_PROTOCOL,             /**< \brief msc协议类型 */
    0x00,                             /**< \brief 描述该接口的字符串索引, 0x00表示没有 */

    /**\brief 输入端点描述符 */
    AM_USB_DESC_LENGTH_ENDPOINT,      /**< \brief 端点描述符字节数 */
    AM_USB_DESC_TYPE_ENDPOINT,        /**< \brief 端点描述符类型编号，固定为0x05 */
    (__USBD_MSC_ENDPOINT_IN | (AM_USB_IN << AM_USB_REQ_TYPE_DIR_SHIFT)),
    __USBD_MSC_ENDPOINT_IN_ATTRIBUTE, /**< \brief 端点属性 02表示批量  */

    AM_USB_SHORT_GET_LOW(__USBD_MSC_ENDPOINT_IN_PACKSIZE),
    AM_USB_SHORT_GET_HIGH(__USBD_MSC_ENDPOINT_IN_PACKSIZE), /**< \brief 端点一次性收发的最大包大小 */

    __USBD_MSC_ENDPOINT_IN_QUERY_TIME,   /* 主机查询端点时的时间间隔：10ms  */

    /**\brief 输出端点描述符 */
    AM_USB_DESC_LENGTH_ENDPOINT,         /**< \brief 端点描述符字节数 */
    AM_USB_DESC_TYPE_ENDPOINT,           /**< \brief 端点描述符类型编号，固定为0x05 */

    /**\brief 端点地址及输出属性 */
    (__USBD_MSC_ENDPOINT_OUT | (AM_USB_OUT << AM_USB_REQ_TYPE_DIR_SHIFT)),

    __USBD_MSC_ENDPOINT_OUT_ATTRIBUTE,   /**< \brief 端点属性 */

    AM_USB_SHORT_GET_LOW(__USBD_MSC_ENDPOINT_OUT_PACKSIZE),
    AM_USB_SHORT_GET_HIGH(__USBD_MSC_ENDPOINT_OUT_PACKSIZE), /**< \brief 端点一次性收发的最大包大小 */

    __USBD_MSC_ENDPOINT_OUT_QUERY_TIME,     /**< \brief 主机查询端点时的时间间隔 10ms */
};

/*******************************************************************************
 * 字符串描述符,如果用户需要修改描述符信息，注意使用的是UINCODE码(注意大小端)
 ******************************************************************************/
/**< \brief 描述产品的字符串描述符 */
static const uint8_t __g_usb_msc_desc_str_iproduct[16] = {
    sizeof(__g_usb_msc_desc_str_iproduct), /**< \brief 字符串描述符字节数 */
    AM_USB_DESC_TYPE_STRING,               /**< \brief 字符串描述符类型编号，固定为0x03 */

     0x55, 0x00, /**< \brief U */
     0x42, 0x00, /**< \brief S */
     0x53, 0x00, /**< \brief B */
     0x21, 0x6a, /**< \brief 模 */
     0xdf, 0x62, /**< \brief 拟 */
     0x55, 0x00, /**< \brief U */
     0xd8, 0x76  /**< \brief 盘 */
};
/*******************************************************************************
 * 语言ID字符串描述符
 * 这里使用美式英语，不使用简体中文的原因是如果使用简体中文，则主机不会向从机要字符串描述符
 * 美式英语的语言ID为0x0409，简体中文的语言ID为0x0804，注意大小端。
******************************************************************************/
static const uint8_t __g_usb_msc_desc_str_language_id[4] = {
    sizeof(__g_usb_msc_desc_str_language_id),  /**< \brief 字符串描述符字节数 */
    AM_USB_DESC_TYPE_STRING,                   /**< \brief 字符串描述符类型编号，固定为0x03 */
    0x04,
    0x08,                                      /**< \brief 简体中文 */
};

/**< \brief 描述厂商的字符串描述符 */
static const uint8_t __g_usb_msc_desc_str_imanufacturer[22] = {
    sizeof(__g_usb_msc_desc_str_imanufacturer),       /**< \brief 字符串描述符字节数 */
    AM_USB_DESC_TYPE_STRING,       /**< \brief 字符串描述符类型编号，固定为0x03 */
    0x7f, 0x5e, /**< \brief 广 */
    0xde, 0x5d, /**< \brief 州 */
    0xf4, 0x81, /**< \brief 致 */
    0xdc, 0x8f, /**< \brief 远 */
    0x35, 0x75, /**< \brief 电 */
    0x50, 0x5b, /**< \brief 子 */
    0x09, 0x67, /**< \brief 有 */
    0x50, 0x96, /**< \brief 限 */
    0x6c, 0x51, /**< \brief 公 */
    0xf8, 0x53, /**< \brief 司 */
};

/**< \brief 描述设备序列号的字符串描述符 */
static const uint8_t __g_usb_msc_desc_str_iserialnumber[22] = {
    sizeof(__g_usb_msc_desc_str_iserialnumber),  /**< \brief 字符串描述符字节数 */
    AM_USB_DESC_TYPE_STRING,                     /**< \brief 字符串描述符类型编号，固定为0x03 */
    0x32, 0x00, /**< \brief 2 */
    0x30, 0x00, /**< \brief 0 */
    0x31, 0x00, /**< \brief 1 */
    0x38, 0x00, /**< \brief 8 */
    0x2d, 0x00, /**< \brief - */
    0x31, 0x00, /**< \brief 1 */
    0x30, 0x00, /**< \brief 0 */
    0x2d, 0x00, /**< \brief - */
    0x32, 0x00, /**< \brief 2 */
    0x39, 0x00, /**< \brief 9 */
};


/******************************************************************************
 * 各描述符信息
 *****************************************************************************/
static const am_usbd_descriptor_t __g_usb_msc_descriptor[] = {
    /**\brief 设备描述符 */
    {
        (AM_USB_DESC_TYPE_DEVICE << 8) | (0x00),
        sizeof(__g_usb_msc_desc_dev),
        __g_usb_msc_desc_dev
    },

    /**\brief 配置描述符及其下级描述符 */
    {
        (AM_USB_DESC_TYPE_CONFIGURE << 8) | (0x00),
        sizeof(__g_usb_msc_desc_conf),
        __g_usb_msc_desc_conf
    },

    /**\brief 字符串描述符，描述语言id */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | (0x00),
        sizeof(__g_usb_msc_desc_str_language_id),
        __g_usb_msc_desc_str_language_id
    },

    /**\brief 字符串描述符，描述厂商 */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | __USBD_MSC_VENDOR_STRING_INDEX,
        sizeof(__g_usb_msc_desc_str_imanufacturer),
        __g_usb_msc_desc_str_imanufacturer
    },

    /**\brief 字符串描述符，描述产品 */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | __USBD_MSC_PRODUCT_STRING_INDEX,
        sizeof(__g_usb_msc_desc_str_iproduct),
        __g_usb_msc_desc_str_iproduct
    },

    /**\brief 字符串描述符，描述设备序列号 */
    {
        (AM_USB_DESC_TYPE_STRING << 8)| __USBD_MSC_DEVICE_STRING_INDEX,
        sizeof(__g_usb_msc_desc_str_iserialnumber),
        __g_usb_msc_desc_str_iserialnumber
    },
};

/******************************************************************************
 * 平台初始化函数、解初始化函数已经设备信息
 ******************************************************************************/
extern USB_OTG_CORE_HANDLE USB_OTG_dev;
/**
 * \brief 平台初始化
 */
static void __usb_msc_init (void) {
    /**\brief 使能时钟 */
    am_clk_enable(CLK_USBFS);

    am_gpio_pin_cfg(PIOA_9, GPIO_AFIO(AMHW_HC32F460_AFIO_USBF));/**< \brief VBUS */
    /**\brief 配置PIOA_11 PIOA_12为USB功能   */
    am_gpio_pin_cfg(PIOA_11, GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) |
                             GPIO_AFIO(AMHW_HC32F460_AFIO_USBF));/**< \brief USBDM */
    am_gpio_pin_cfg(PIOA_12, GPIO_MODE(AMHW_HC32F460_GPIO_MODE_AIN) |
                             GPIO_AFIO(AMHW_HC32F460_AFIO_USBF));/**< \brief USBDP */
    USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_MSC_cb,
            &USR_cb);
}


static  uint8_t __g_readme_data[]= {
    "**USB设备类  ZLG模拟U盘**\r\n"
     "1.先判断使用PC机的系统，如果使用WIN10,记得修改am_hwconf_xxxx_usb_msc.c文件中系统宏配置项;\r\n"
     "2.接上串口，将文件拖动到U盘中，会打印拖动文件的信息;\r\n"
};


#define __USBD_MSC_FILE_CREAT_TIME1    (15U)
#define __USBD_MSC_FILE_CREAT_TIME2    (48U)
#define __USBD_MSC_FILE_CREAT_TIME3    (26U)

#define __USBD_MSC_FILE_CREAT_DATE1    (2018U)
#define __USBD_MSC_FILE_CREAT_DATE2    (10U)
#define __USBD_MSC_FILE_CREAT_DATE3    (29U)

#define __USBD_MSC_FILE_MODIFIED_TIME1 (15U)
#define __USBD_MSC_FILE_MODIFIED_TIME2 (36U)
#define __USBD_MSC_FILE_MODIFIED_TIME3 (47U)

#define __USBD_MSC_FILE_MODIFIED_DATE1 (2018U)
#define __USBD_MSC_FILE_MODIFIED_DATE2 (11U)
#define __USBD_MSC_FILE_MODIFIED_DATE3 (2U)

static const uint8_t __g_fat_root_dir[] = {

    /**\brief 磁盘标卷：zlgmcu 的假U盘 */
     'Z', 'L', 'G', 'M', 'C', 'U', 0x20, 0x20, 0x55, 0xC5, 0xCC,
    0x08,                  /**< \brief 文件属性，表示磁盘标卷 */
    0x00,                  /**< \brief 保留 */
    0x00,                  /**< \brief 创建时间毫秒时间戳 */

    /**\brief 文件创建时间，15点48分26秒 */
    (uint8_t)AM_TIME_LB(__USBD_MSC_FILE_CREAT_TIME1, __USBD_MSC_FILE_CREAT_TIME2, __USBD_MSC_FILE_CREAT_TIME3),
    (uint8_t)AM_TIME_HB(__USBD_MSC_FILE_CREAT_TIME1, __USBD_MSC_FILE_CREAT_TIME2, __USBD_MSC_FILE_CREAT_TIME3),

    /**\brief 文件创建日期，2018年10月29日 */
    (uint8_t)AM_DATE_LB(__USBD_MSC_FILE_CREAT_DATE1, __USBD_MSC_FILE_CREAT_DATE2, __USBD_MSC_FILE_CREAT_DATE3),
    (uint8_t)AM_DATE_HB(__USBD_MSC_FILE_CREAT_DATE1, __USBD_MSC_FILE_CREAT_DATE2, __USBD_MSC_FILE_CREAT_DATE3),

    /**\brief 最后访问日期，2018年10月29日 */
    (uint8_t)AM_DATE_LB(__USBD_MSC_FILE_CREAT_DATE1, __USBD_MSC_FILE_CREAT_DATE2, __USBD_MSC_FILE_CREAT_DATE3),
    (uint8_t)AM_DATE_HB(__USBD_MSC_FILE_CREAT_DATE1, __USBD_MSC_FILE_CREAT_DATE2, __USBD_MSC_FILE_CREAT_DATE3),

    0x00, 0x00,            /**< \brief 起始簇号高位字节，FAT12/16必须为0 */

    /**\brief 最后修改时间，15点36分47秒 */
    (uint8_t)AM_TIME_LB(__USBD_MSC_FILE_MODIFIED_TIME1, __USBD_MSC_FILE_MODIFIED_TIME2, __USBD_MSC_FILE_MODIFIED_TIME3),
    (uint8_t)AM_TIME_HB(__USBD_MSC_FILE_MODIFIED_TIME1, __USBD_MSC_FILE_MODIFIED_TIME2, __USBD_MSC_FILE_MODIFIED_TIME3),

    /**\brief 最后修改日期，2018年11月2日 */
    (uint8_t)AM_DATE_LB(__USBD_MSC_FILE_MODIFIED_DATE1,__USBD_MSC_FILE_MODIFIED_DATE2, __USBD_MSC_FILE_MODIFIED_DATE3),
    (uint8_t)AM_DATE_HB(__USBD_MSC_FILE_MODIFIED_DATE1,__USBD_MSC_FILE_MODIFIED_DATE2, __USBD_MSC_FILE_MODIFIED_DATE3),

    0x00, 0x00,               /**< \brief 起始簇低字 */
    0x00, 0x00, 0x00, 0x00,   /**< \brief 文件长度 */
    /**\brief 根目录下的测试文件 */
    /**\brief 文件名“README.TXT” */
    'R',  'E',   'A',  'D', 'M', 'E', ' ', ' ',  'T', 'X', 'T',
    0x01,                  /**< \brief 文件属性，表示只读文件  */
    0x00,                  /**< \brief 保留 */
    0x00,                  /**< \brief 创建时间毫秒时间戳 */

    /**\brief 文件创建时间，15点48分26秒 */
    (uint8_t)AM_TIME_LB(__USBD_MSC_FILE_CREAT_TIME1, __USBD_MSC_FILE_CREAT_TIME2, __USBD_MSC_FILE_CREAT_TIME3),
    (uint8_t)AM_TIME_HB(__USBD_MSC_FILE_CREAT_TIME1, __USBD_MSC_FILE_CREAT_TIME2, __USBD_MSC_FILE_CREAT_TIME3),

    /**\brief 文件创建日期,2018年10月29日 */
    (uint8_t)AM_DATE_LB(__USBD_MSC_FILE_CREAT_DATE1, __USBD_MSC_FILE_CREAT_DATE2, __USBD_MSC_FILE_CREAT_DATE3),
    (uint8_t)AM_DATE_HB(__USBD_MSC_FILE_CREAT_DATE1, __USBD_MSC_FILE_CREAT_DATE2, __USBD_MSC_FILE_CREAT_DATE3),

    /**\brief 最后访问日期 ,15点36分47秒 */
    (uint8_t)AM_DATE_LB(__USBD_MSC_FILE_MODIFIED_TIME1, __USBD_MSC_FILE_MODIFIED_TIME2, __USBD_MSC_FILE_MODIFIED_TIME3),
    (uint8_t)AM_DATE_HB(__USBD_MSC_FILE_MODIFIED_TIME1, __USBD_MSC_FILE_MODIFIED_TIME2, __USBD_MSC_FILE_MODIFIED_TIME3),

    0x00, 0x00,            /**< \brief 起始簇号高位字节，FAT12/16必须为0 */

    /**\brief 最后修改时间,15点36分47秒 */
    (uint8_t)AM_TIME_LB(__USBD_MSC_FILE_MODIFIED_TIME1, __USBD_MSC_FILE_MODIFIED_TIME2, __USBD_MSC_FILE_MODIFIED_TIME3),
    (uint8_t)AM_TIME_HB(__USBD_MSC_FILE_MODIFIED_TIME1, __USBD_MSC_FILE_MODIFIED_TIME2, __USBD_MSC_FILE_MODIFIED_TIME3),

    /**\brief 最后修改日期，2018年11月2日 */
    (uint8_t)AM_DATE_LB(__USBD_MSC_FILE_MODIFIED_DATE1,__USBD_MSC_FILE_MODIFIED_DATE2, __USBD_MSC_FILE_MODIFIED_DATE3),
    (uint8_t)AM_DATE_HB(__USBD_MSC_FILE_MODIFIED_DATE1,__USBD_MSC_FILE_MODIFIED_DATE2, __USBD_MSC_FILE_MODIFIED_DATE3),

    0x02, 0x00,            /**< \brief 起始簇低字，簇2。(数据区) */

    /**\brief 文件长度 */
    (sizeof(__g_readme_data) - 1) & 0xff, ((sizeof(__g_readme_data) - 1) >> 8) & 0xff, 0x00, 0x00,
};

/**
 * \brief 平台去初始化
 */
static void __usb_msc_deinit (void) {
    am_clk_disable(CLK_USBFS);                 /**< \brief 禁能USB时钟 */
}

/**\brief 设备信息*/
static const am_usbd_devinfo_t __g_usbd_msc_info = {
        __g_usb_msc_descriptor,                                                 /**< \brief 描述符地址 */
        sizeof(__g_usb_msc_descriptor) / sizeof(__g_usb_msc_descriptor[0]),     /**< \brief 描述符个数 */
};

/**\brief 定义USB设备信息 */
static const am_hc32f460_usbd_devinfo_t  __g_hc32f460_usbd_msc_info = {
    HC32F460_USB_OTG_FS_BASE,                  /**< \brief 寄存器基地址 */
    INUM_USBFS_GLB,                            /**< \brief 中断号 */
    __usb_msc_init,                            /**< \brief 平台初始化 */
    __usb_msc_deinit,                          /**< \brief 平台去初始化 */
    &__g_usbd_msc_info,
};

/** \brief USB MSC设备实例 */
am_usbd_msc_t      __g_usb_msc_dev;

/** \brief USB设备实例 */
am_hc32f460_usbd_dev_t  __g_hc32f460_usbd_msc;

/**< \brief 接收SCSI命令缓冲区 */
static uint8_t __g_sici_cmd_buff[AM_USBD_MAX_EP_DATA_CNT] = {0};

const am_usbd_msc_diskinfo_t __g_usbd_msc_disk_info = {
    __USBD_MSC_SYS_WIN7,
    0,

    AM_USBD_MSC_DISD_SIZE,
    AM_USBD_MSC_SECTOR_SIZE,
    AM_USBD_MSC_DISD_SIZE / AM_USBD_MSC_SECTOR_SIZE,
    (AM_USBD_MSC_DISD_SIZE / 256 / 1024 + 1) * 512,
    (AM_USBD_MSC_DISD_SIZE / 256 / 1024 * 2 + 1) * 512,
    (AM_USBD_MSC_DISD_SIZE / 256 / 1024 * 2 + 17) * 512,

    /**\brief 指令缓冲区  */
    __g_sici_cmd_buff,

    /**\brief FAT根目录 */
    __g_fat_root_dir,
    sizeof(__g_fat_root_dir),

    /**\brief README文件数据 */
    __g_readme_data,
    sizeof(__g_readme_data),
};


/** \brief usb_msc实例初始化，获得usb_msc标准服务句柄 */
am_usbd_msc_handle am_hc32f460_usb_msc_inst_init (void)
{
    return am_usbd_msc_init(&__g_usb_msc_dev,
                            &__g_usbd_msc_disk_info,
                            am_hc32f460_usbd_init(&__g_hc32f460_usbd_msc, &__g_hc32f460_usbd_msc_info));
}

/** \brief usb_msc解初始化 */
void am_hc32f460_usb_msc_inst_deinit (void)
{
    am_usb_msc_deinit(&__g_usb_msc_dev);
}

/**
 * @}
 */

/* end of file */
