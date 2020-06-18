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
/** \file usbd_ioreq.c
 **
 ** A detailed description is available at
 ** @link
    This file provides the IO requests APIs for control endpoints.
    @endlink
 **
 **   - 2018-12-26  1.0  wangmin First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbd_ioreq.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

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
 ** \brief  USBD_CtlSendData
 **         send data on the ctl pipe
 ** \param  pdev: device instance
 ** \param  buff: pointer to data buffer
 ** \param  len: length of data to be sent
 ** \retval status
 ******************************************************************************/
USBD_Status  USBD_CtlSendData (USB_OTG_CORE_HANDLE  *pdev,
                               uint8_t *pbuf,
                               uint16_t len)
{
    USBD_Status ret = USBD_OK;

    pdev->dev.in_ep[0].total_data_len = len;
    pdev->dev.in_ep[0].rem_data_len   = len;
    pdev->dev.device_state = USB_OTG_EP0_DATA_IN;

    //printf("ctrl in %d\n",len);

    DCD_EP_Tx (pdev, 0u, pbuf, (uint32_t)len);

    return ret;
}

/**
 *******************************************************************************
 ** \brief  USBD_CtlContinueSendData
 **         continue sending data on the ctl pipe
 ** \param  pdev: device instance
 ** \param  buff: pointer to data buffer
 ** \param  len: length of data to be sent
 ** \retval status
 ******************************************************************************/
USBD_Status  USBD_CtlContinueSendData (USB_OTG_CORE_HANDLE  *pdev,
                                       uint8_t *pbuf,
                                       uint16_t len)
{
    USBD_Status ret = USBD_OK;

    DCD_EP_Tx (pdev, 0u, pbuf, (uint32_t)len);
    return ret;
}

/**
 *******************************************************************************
 ** \brief  USBD_CtlPrepareRx
 **         receive data on the ctl pipe
 ** \param  pdev: USB OTG device instance
 ** \param  buff: pointer to data buffer
 ** \param  len: length of data to be received
 ** \retval status
 ******************************************************************************/
USBD_Status  USBD_CtlPrepareRx (USB_OTG_CORE_HANDLE  *pdev,
                                  uint8_t *pbuf,
                                  uint16_t len)
{
    USBD_Status ret = USBD_OK;

    pdev->dev.out_ep[0].total_data_len = len;
    pdev->dev.out_ep[0].rem_data_len   = len;
    pdev->dev.device_state = USB_OTG_EP0_DATA_OUT;

    DCD_EP_PrepareRx (pdev,
                        0u,
                        pbuf,
                        len);
    return ret;
}

/**
 *******************************************************************************
 ** \brief  USBD_CtlContinueRx
 **         continue receive data on the ctl pipe
 ** \param  pdev: USB OTG device instance
 ** \param  buff: pointer to data buffer
 ** \param  len: length of data to be received
 ** \retval status
 ******************************************************************************/
USBD_Status  USBD_CtlContinueRx (USB_OTG_CORE_HANDLE  *pdev,
                                          uint8_t *pbuf,
                                          uint16_t len)
{
    USBD_Status ret = USBD_OK;

    DCD_EP_PrepareRx (pdev,
                        0u,
                        pbuf,
                        len);
    return ret;
}
/**
 *******************************************************************************
 ** \brief  USBD_CtlSendStatus
 **         send zero lzngth packet on the ctl pipe
 ** \param  pdev: USB OTG device instance
 ** \retval status
 ******************************************************************************/
USBD_Status  USBD_CtlSendStatus (USB_OTG_CORE_HANDLE  *pdev)
{
    USBD_Status ret = USBD_OK;
    pdev->dev.device_state = USB_OTG_EP0_STATUS_IN;
    DCD_EP_Tx (pdev,
                0u,
                pdev->dev.setup_packet,//NULL,
                0u);
    //printf("ctrl send status\n");
    //  USB_OTG_EP0_OutStart(pdev);

    return ret;
}

/**
 *******************************************************************************
 ** \brief  USBD_CtlReceiveStatus
 **         receive zero lzngth packet on the ctl pipe
 ** \param  pdev: USB OTG device instance
 ** \retval status
 ******************************************************************************/
USBD_Status  USBD_CtlReceiveStatus (USB_OTG_CORE_HANDLE  *pdev)
{
    USBD_Status ret = USBD_OK;
    pdev->dev.device_state = USB_OTG_EP0_STATUS_OUT;
    DCD_EP_PrepareRx ( pdev,
                        0u,
                        pdev->dev.setup_packet,//NULL,
                        0u);
    //  USB_OTG_EP0_OutStart(pdev);
    return ret;
}


/**
 *******************************************************************************
 ** \brief  USBD_GetRxCount
 **         returns the received data length
 ** \param  pdev: USB OTG device instance
 **         epnum: endpoint index
 ** \retval Rx Data blength
 ******************************************************************************/
uint16_t  USBD_GetRxCount (USB_OTG_CORE_HANDLE  *pdev , uint8_t epnum)
{
    return (uint16_t)pdev->dev.out_ep[epnum].xfer_count;
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
