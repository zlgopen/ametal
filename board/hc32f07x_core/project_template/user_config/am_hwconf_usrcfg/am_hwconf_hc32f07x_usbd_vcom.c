

#include "hw/amhw_hc32f07x_rcc.h"
#include "ametal.h"
#include "am_hc32f07x.h"
#include "am_delay.h"
#include "am_types.h"
#include "am_clk.h"
#include "am_usb.h"
#include "am_gpio.h"
#include "am_usbd.h"
#include "am_usbd_cdc_vcom.h"
#include "am_hc32f07x_usbd_cdc_vcom.h"
#include "am_hc32f07x_usbd.h"

#define VIRTUAL_COM_PORT_SIZ_DEVICE_DESC        18
#define VIRTUAL_COM_PORT_SIZ_CONFIG_DESC        67
#define VIRTUAL_COM_PORT_SIZ_STRING_LANGID      4
#define VIRTUAL_COM_PORT_SIZ_STRING_VENDOR      38
#define VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT     60
#define VIRTUAL_COM_PORT_SIZ_STRING_SERIAL      26

/* USB Standard Device Descriptor */
static const uint8_t Virtual_Com_Port_DeviceDescriptor[] =
  {
    0x12,   /* bLength */
    AM_USB_DESC_TYPE_DEVICE,     /* bDescriptorType */
    0x10,
    0x01,   /* bcdUSB = 2.00 */
    0x02,   /* bDeviceClass: CDC */
    0x00,   /* bDeviceSubClass */
    0x00,   /* bDeviceProtocol */
    0x40,   /* bMaxPacketSize0 */
    0x81,
    0x2F,   /* idVendor = 0x2F81 */
    0x09,
    0x72,   /* idProduct = 0x7209 */
    0x00,
    0x02,   /* bcdDevice = 2.00 */
    1,              /* Index of string descriptor describing manufacturer */
    2,              /* Index of string descriptor describing product */
    3,              /* Index of string descriptor describing the device's serial number */
    0x01    /* bNumConfigurations */
  };

static uint8_t Virtual_Com_Port_ConfigDescriptor[] =
  {
    /*Configuation Descriptor*/
    AM_USB_DESC_LENGTH_CONFIGURE,   /* bLength: Configuation Descriptor size */
    AM_USB_DESC_TYPE_CONFIGURE,      /* bDescriptorType: Configuration */
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC,       /* wTotalLength:no of returned bytes */
    0x00,
    0x02,   /* bNumInterfaces: 2 interface */
    0x01,   /* bConfigurationValue: Configuration value */
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0,   /* bmAttributes: self powered */
    0x32,   /* MaxPower 0 mA */
    /*Interface Descriptor*/
    0x09,   /* bLength: Interface Descriptor size */
    AM_USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x01,   /* bNumEndpoints: One endpoints used */
    0x02,   /* bInterfaceClass: Communication Interface Class */
    0x02,   /* bInterfaceSubClass: Abstract Control Model */
    0x01,   /* bInterfaceProtocol: Common AT commands */
    0x00,   /* iInterface: */
    /*Header Functional Descriptor*/
    0x05,   /* bLength: Endpoint Descriptor size */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x00,   /* bDescriptorSubtype: Header Func Desc */
    0x10,   /* bcdCDC: spec release number */
    0x01,
    /*Call Managment Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
    0x00,   /* bmCapabilities: D0+D1 */
    0x00,   /* bDataInterface: 1 */
    /*ACM Functional Descriptor*/
    0x04,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,   /* bmCapabilities */
    /*Union Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x06,   /* bDescriptorSubtype: Union func desc */
    0x00,   /* bMasterInterface: Communication class interface */
    0x01,   /* bSlaveInterface0: Data Class Interface */
    /*Endpoint 2 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    AM_USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
    0x83,   /* bEndpointAddress: (IN2) */
    0x03
		,   /* bmAttributes: Interrupt */
    0x40,   /* wMaxPacketSize: 64*/
    0x00,
    0xFF,   /* bInterval: */
    /*Data class interface descriptor*/
    0x09,   /* bLength: Endpoint Descriptor size */
    AM_USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: */
    0x01,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: Two endpoints used */
    0x0A,   /* bInterfaceClass: CDC */
    0x00,   /* bInterfaceSubClass: */
    0x00,   /* bInterfaceProtocol: */
    0x00,   /* iInterface: */
    /*Endpoint 3 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    AM_USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
    0x02,   /* bEndpointAddress: (OUT3) */
    0x02,   /* bmAttributes: Bulk */
    0x40,             /* wMaxPacketSize: 64*/
    0x00,
    0x00,   /* bInterval: ignore for Bulk transfer */
    /*Endpoint 1 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    AM_USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
    0x81,   /* bEndpointAddress: (IN1) */
    0x02,   /* bmAttributes: Bulk */
    0x40,             /* wMaxPacketSize: 64*/
    0x00,
    0x00    /* bInterval */
  };

/* USB String Descriptors */
static const uint8_t Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_LANGID,
    AM_USB_DESC_TYPE_STRING,
    0x09,
    0x04 /* LangID = 0x0409: U.S. English */
  };

static const uint8_t Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_VENDOR,     /* Size of Vendor string */
    AM_USB_DESC_TYPE_STRING,             /* bDescriptorType*/
    /* Manufacturer: "MindMotion" */

    'M', 0, 
    'i', 0, 
    'n', 0,
    'd', 0,
    'M', 0, 
    'o', 0,
    't', 0,
    'i', 0,
    'o', 0,
    'n', 0,
    
  };

static const uint8_t Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT,          /* bLength */
    AM_USB_DESC_TYPE_STRING,        /* bDescriptorType */
    /* Product name: ""MindMotion  Virtual COM Port" */

    'M', 0, 
    'y', 0,
    ' ', 0, 
    'V', 0,
    'i', 0,
    'r', 0,
    't', 0, 
    'u', 0,
    'a', 0,
    'l', 0, 
    ' ', 0, 
    'C', 0,
    'O', 0,
    'M', 0, 
    ' ', 0, 
    'P', 0,
    'o', 0,
    'r', 0,
    't', 0, 
    ' ', 0, 
    ' ', 0
    
  };

static const uint8_t Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_SERIAL,           /* bLength */
    AM_USB_DESC_TYPE_STRING,                   /* bDescriptorType */
    

    'Z', 0,
    'M', 0,
    'F', 0,
    '1', 0,
    '5', 0,
    '9', 0,
     0,  0,
  };





/******************************************************************************
 * 各描述符信息
 *****************************************************************************/
static const am_usbd_descriptor_t __g_am_usbd_vcom_descriptor[] = {
    /* 设备描述符 */
    {
        (AM_USB_DESC_TYPE_DEVICE << 8) | (0x00),
        sizeof(Virtual_Com_Port_DeviceDescriptor),
        Virtual_Com_Port_DeviceDescriptor
    },

    /* 配置描述符及其下级描述符 */
    {
        (AM_USB_DESC_TYPE_CONFIGURE << 8) | (0x00),
        sizeof(Virtual_Com_Port_ConfigDescriptor),
        Virtual_Com_Port_ConfigDescriptor
    },

    /* 字符串描述符0，描述语言id */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | (0x00),
        sizeof(Virtual_Com_Port_StringLangID),
        Virtual_Com_Port_StringLangID
    },

    /* 字符串描述符1，描述厂商 */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | 0x01,
        sizeof(Virtual_Com_Port_StringVendor),
        Virtual_Com_Port_StringVendor
    },

    /* 字符串描述符2，描述产品 */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | 0x02,
        sizeof(Virtual_Com_Port_StringProduct),
        Virtual_Com_Port_StringProduct
    },

    /* 字符串描述符3，描述设备 */
    {
        (AM_USB_DESC_TYPE_STRING << 8) | 0x03,
        sizeof(Virtual_Com_Port_StringSerial),
        Virtual_Com_Port_StringSerial
    },
};


/**
 * \brief 平台初始化
 */
static void __am_usbd_vcom_init (void) {
	
    /* 使能时钟 */
    amhw_hc32f07x_rcc_set_start(0x5A5A);
    amhw_hc32f07x_rcc_set_start(0xA5A5);
    amhw_hc32f07x_rcc_usbclk_adjust_set (AMHW_HC32F07X_USBCLK_ADJUST_PLL);
    am_clk_enable(CLK_USB);
}

/**
 * \brief 平台去初始化
 */
static void __am_usbd_vcom_deinit (void) {
    am_clk_disable(CLK_USB);
    amhw_hc32f07x_rcc_peripheral_disable (AMHW_HC32F07X_RCC_USB);       /* 禁能USB时钟 */                             /* 禁能USB时钟 */
}

static const am_usbd_devinfo_t __g_usbd_info = {
        __g_am_usbd_vcom_descriptor,                                                         /* 描述符地址 */
        sizeof(__g_am_usbd_vcom_descriptor) / sizeof(__g_am_usbd_vcom_descriptor[0]),     /* 描述符个数 */
};

/**< \brief 定义USB设备信息 */
static const am_hc32f07x_usbd_devinfo_t  __g_hc32f07x_usbd_vcom_info = {
    HC32F07X_USB_BASE,                           /* 寄存器基地址 */
    INUM_USB,                                  /* 中断号 */
    __am_usbd_vcom_init,              /**< \brief 平台初始化 */
    __am_usbd_vcom_deinit,            /**< \brief 平台去初始化 */
    &__g_usbd_info,
};

static uint8_t __buffer_out[64 + 1] = {0};

const am_usbd_cdc_vcom_info_t __g_usbd_vcom_info = {
    3,
    1,
    2,
    __buffer_out,
};


am_hc32f07x_usbd_dev_t  __g_hc32f07x_dev;
am_usbd_cdc_vcom_t    __g_vcom_dev;


/** \brief usb_vcom实例初始化，获得usb_vcom标准服务句柄 */
am_usbd_cdc_vcom_handle am_hc32f07x_usbd_vcom_inst_init (void)
{
    return am_hc32f07x_usbd_cdc_vcom_init(&__g_vcom_dev,
                                 &__g_usbd_vcom_info,
                                  am_hc32f07x_usbd_init(&__g_hc32f07x_dev, &__g_hc32f07x_usbd_vcom_info));
}


/** \brief usb_printer解初始化，获得usb_printer标准服务句柄 */
void am_hc32f07x_usbd_vcom_inst_deinit (void)
{
	am_hc32f07x_usbd_cdc_vcom_deinit(&__g_vcom_dev);
}


/* end of file */

