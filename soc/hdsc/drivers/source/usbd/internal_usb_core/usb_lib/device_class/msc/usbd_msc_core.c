/******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file usbd_msc_core.c
 **
 ** \brief USB MSC device example.
 **
 **   - 2019-05-15  1.0  Zhangxl First version for USB MSC device demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbd_msc_mem.h"
#include "usbd_msc_core.h"
#include "usbd_msc_bot.h"
#include "usbd_req.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/* USB MSC Core callback functions */
USBD_Class_cb_TypeDef USBD_MSC_cb =
{
//    &USBD_MSC_Init,
//    &USBD_MSC_DeInit,
//    &USBD_MSC_Setup,
//    NULL,                        /*EP0_TxSent*/
//    NULL,                        /*EP0_RxReady*/
//    &USBD_MSC_DataIn,
//    &USBD_MSC_DataOut,
//    NULL,                        /*SOF */
//    NULL,
//    NULL,
//    &USBD_MSC_GetCfgDesc,
//#ifdef USB_OTG_HS_CORE
//    &USBD_MSC_GetOtherCfgDesc,
//#endif
    &USBD_MSC_Init,
    &USBD_MSC_DeInit,
    NULL,
    NULL,                        /*EP0_TxSent*/
    NULL,                        /*EP0_RxReady*/
    NULL,
    NULL,
    NULL,                        /*SOF */
    NULL,
    NULL,
    &USBD_MSC_GetCfgDesc,
#ifdef USB_OTG_HS_CORE
    NULL,
#endif

};

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined (__ICCARM__)       /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Mass storage device Configuration Descriptor */
/* All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
__USB_ALIGN_BEGIN uint8_t USBD_MSC_CfgDesc[USB_MSC_CONFIG_DESC_SIZ] __USB_ALIGN_END =
{

    0x09,                        /* bLength: Configuation Descriptor size */
    USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
    USB_MSC_CONFIG_DESC_SIZ,

    0x00,
    0x01,                        /* bNumInterfaces: 1 interface */
    0x01,                        /* bConfigurationValue: */
    0x04,                        /* iConfiguration: */
    0xC0,                        /* bmAttributes: */
    0x32,                        /* MaxPower 100 mA */

    /********************  Mass Storage interface ********************/
    0x09,                        /* bLength: Interface Descriptor size */
    0x04,                        /* bDescriptorType: */
    0x00,                        /* bInterfaceNumber: Number of Interface */
    0x00,                        /* bAlternateSetting: Alternate setting */
    0x02,                        /* bNumEndpoints*/
    0x08,                        /* bInterfaceClass: MSC Class */
    0x06,                        /* bInterfaceSubClass : SCSI transparent*/
    0x50,                        /* nInterfaceProtocol */
    0x05,                        /* iInterface: */
    /********************  Mass Storage Endpoints ********************/
    0x07,                        /*Endpoint descriptor length = 7*/
    0x05,                        /*Endpoint descriptor type */
    MSC_IN_EP,                   /*Endpoint address (IN, address 1) */
    0x02,                        /*Bulk endpoint type */
    LOBYTE(MSC_MAX_PACKET),
    HIBYTE(MSC_MAX_PACKET),
    0x00,                        /*Polling interval in milliseconds */

    0x07,                        /*Endpoint descriptor length = 7 */
    0x05,                        /*Endpoint descriptor type */
    MSC_OUT_EP,                  /*Endpoint address (OUT, address 1) */
    0x02,                        /*Bulk endpoint type */
    LOBYTE(MSC_MAX_PACKET),
    HIBYTE(MSC_MAX_PACKET),
    0x00                         /*Polling interval in milliseconds*/
};

#ifdef USB_OTG_HS_CORE
 #ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
   #if defined (__ICCARM__)      /*!< IAR Compiler */
     #pragma data_alignment=4
   #endif
 #endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint8_t USBD_MSC_OtherCfgDesc[USB_MSC_CONFIG_DESC_SIZ] __USB_ALIGN_END =
{

    0x09,                        /* bLength: Configuation Descriptor size */
    USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,
    USB_MSC_CONFIG_DESC_SIZ,

    0x00,
    0x01,                        /* bNumInterfaces: 1 interface */
    0x01,                        /* bConfigurationValue: */
    0x04,                        /* iConfiguration: */
    0xC0,                        /* bmAttributes: */
    0x32,                        /* MaxPower 100 mA */

    /********************  Mass Storage interface ********************/
    0x09,                        /* bLength: Interface Descriptor size */
    0x04,                        /* bDescriptorType: */
    0x00,                        /* bInterfaceNumber: Number of Interface */
    0x00,                        /* bAlternateSetting: Alternate setting */
    0x02,                        /* bNumEndpoints*/
    0x08,                        /* bInterfaceClass: MSC Class */
    0x06,                        /* bInterfaceSubClass : SCSI transparent command set*/
    0x50,                        /* nInterfaceProtocol */
    0x05,                        /* iInterface: */
    /********************  Mass Storage Endpoints ********************/
    0x07,                        /*Endpoint descriptor length = 7*/
    0x05,                        /*Endpoint descriptor type */
    MSC_IN_EP,                   /*Endpoint address (IN, address 1) */
    0x02,                        /*Bulk endpoint type */
    0x40,
    0x00,
    0x00,                        /*Polling interval in milliseconds */

    0x07,                        /*Endpoint descriptor length = 7 */
    0x05,                        /*Endpoint descriptor type */
    MSC_OUT_EP,                  /*Endpoint address (OUT, address 1) */
    0x02,                        /*Bulk endpoint type */
    0x40,
    0x00,
    0x00                         /*Polling interval in milliseconds*/
};
#endif

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined (__ICCARM__)       /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN static uint8_t USBD_MSC_MaxLun __USB_ALIGN_END = 0u;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined (__ICCARM__)       /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN static uint8_t USBD_MSC_AltSet __USB_ALIGN_END = 0u;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  Initialize  the mass storage configuration
 ** \param pdev : device instance
 ** \param cfgidx : configuration index
 ** \retval : status
 ******************************************************************************/
uint8_t  USBD_MSC_Init(void  *pdev,
                       uint8_t cfgidx)
{
    USBD_MSC_DeInit(pdev, cfgidx);

//    /* Open EP IN */
    DCD_EP_Open(pdev,
                MSC_IN_EP,
                MSC_EPIN_SIZE,
                USB_OTG_EP_BULK);

    /* Open EP OUT */
    DCD_EP_Open(pdev,
                MSC_OUT_EP,
                MSC_EPOUT_SIZE,
                USB_OTG_EP_BULK);

//    /* Init the BOT  layer */
    MSC_BOT_Init(pdev);

    return USBD_OK;
}

/**
 *******************************************************************************
 ** \brief  DeInitilaize  the mass storage configuration
 ** \param pdev : device instance
 ** \param cfgidx : configuration index
 ** \retval : status
 ******************************************************************************/
uint8_t  USBD_MSC_DeInit(void  *pdev,
                         uint8_t cfgidx)
{
    /* Close MSC EPs */
    DCD_EP_Close(pdev, MSC_IN_EP);
    DCD_EP_Close(pdev, MSC_OUT_EP);

    /* Un-Init the BOT layer */
    MSC_BOT_DeInit(pdev);
    return USBD_OK;
}

/**
 *******************************************************************************
 ** \brief  Handle the MSC specific requests
 ** \param pdev : device instance
 ** \param req : USB request
 ** \retval : status
 ******************************************************************************/
uint8_t  USBD_MSC_Setup(void  *pdev, USB_SETUP_REQ *req)
{
    uint8_t u8Res = USBD_OK;
    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
        /* Class request */
        case USB_REQ_TYPE_CLASS:
            switch (req->bRequest)
            {
                case BOT_GET_MAX_LUN:
                    if ((req->wValue == (uint16_t)0) &&
                        (req->wLength == (uint16_t)1) &&
                        ((req->bmRequest & 0x80u) == (uint8_t)0x80))
                    {
                        USBD_MSC_MaxLun = USBD_STORAGE_fops->GetMaxLun();
                        if (USBD_MSC_MaxLun > 0u)
                        {
                            USBD_CtlSendData(pdev,
                                             &USBD_MSC_MaxLun,
                                             1u);
                        }else
                        {
                            USBD_CtlError(pdev);
                            u8Res = USBD_FAIL;
                        }
                    }else
                    {
                        USBD_CtlError(pdev);
                        u8Res = USBD_FAIL;
                    }
                    break;

                case BOT_RESET:
                    if ((req->wValue == (uint16_t)0) &&
                        (req->wLength == (uint16_t)0) &&
                        ((req->bmRequest & 0x80u) != (uint8_t)0x80))
                    {
                        MSC_BOT_Reset(pdev);
                    }else
                    {
                        USBD_CtlError(pdev);
                        u8Res = USBD_FAIL;
                    }
                    break;

                default:
                    USBD_CtlError(pdev);
                    u8Res = USBD_FAIL;
                    break;
            }
            break;
        /* Interface & Endpoint request */
        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest)
            {
                case USB_REQ_GET_INTERFACE:
                    USBD_CtlSendData(pdev,
                                     &USBD_MSC_AltSet,
                                     1u);
                    break;

                case USB_REQ_SET_INTERFACE:
                    USBD_MSC_AltSet = (uint8_t)(req->wValue);
                    break;

                case USB_REQ_CLEAR_FEATURE:

                    /* Flush the FIFO and Clear the stall status */
                    DCD_EP_Flush(pdev, (uint8_t)req->wIndex);

                    /* Re-activate the EP */
                    DCD_EP_Close(pdev, (uint8_t)req->wIndex);
                    if ((((uint8_t)req->wIndex) & (uint16_t)0x80u) == (uint16_t)0x80)
                    {
                        DCD_EP_Open(pdev,
                                    ((uint8_t)req->wIndex),
                                    MSC_EPIN_SIZE,
                                    USB_OTG_EP_BULK);
                    }else
                    {
                        DCD_EP_Open(pdev,
                                    ((uint8_t)req->wIndex),
                                    MSC_EPOUT_SIZE,
                                    USB_OTG_EP_BULK);
                    }

                    /* Handle BOT error */
                    MSC_BOT_CplClrFeature(pdev, (uint8_t)req->wIndex);
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
    return u8Res;
}

/**
 *******************************************************************************
 ** \brief handle data IN Stage
 ** \param pdev : device instance
 ** \param epnum : endpoint index
 ** \retval : status
 ******************************************************************************/
uint8_t  USBD_MSC_DataIn(void  *pdev,
                         uint8_t epnum)
{
    MSC_BOT_DataIn(pdev, epnum);
    return USBD_OK;
}

/**
 *******************************************************************************
 ** \brief handle data OUT Stage
 ** \param pdev : device instance
 ** \param epnum : endpoint index
 ** \retval : status
 ******************************************************************************/
uint8_t  USBD_MSC_DataOut(void  *pdev,
                          uint8_t epnum)
{
    MSC_BOT_DataOut(pdev, epnum);
    return USBD_OK;
}

/**
 *******************************************************************************
 ** \brief return configuration descriptor
 ** \param speed : current device speed
 ** \param length : pointer data length
 ** \retval : pointer to descriptor buffer
 ******************************************************************************/
uint8_t  *USBD_MSC_GetCfgDesc(uint8_t speed, uint16_t *length)
{
    *length = (uint16_t)sizeof(USBD_MSC_CfgDesc);
    return USBD_MSC_CfgDesc;
}

/**
 *******************************************************************************
 ** \brief return other speed configuration descriptor
 ** \param speed : current device speed
 ** \param length : pointer data length
 ** \retval : pointer to descriptor buffer
 ******************************************************************************/
#ifdef USB_OTG_HS_CORE
uint8_t  *USBD_MSC_GetOtherCfgDesc(uint8_t speed,
                                   uint16_t *length)
{
    *length = sizeof(USBD_MSC_OtherCfgDesc);
    return USBD_MSC_OtherCfgDesc;
}
#endif

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
