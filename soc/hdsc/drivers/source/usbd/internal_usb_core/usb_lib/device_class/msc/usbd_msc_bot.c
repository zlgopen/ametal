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
/** \file usbd_msc_bot.c
 **
 ** A detailed description is available at
 ** @link
        This file provides the USBD bulk only transfer functions.
    @endlink
 **
 **   - 2019-05-15  1.0  zhangxl First version for USB bulk only transfer.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ametal.h"
#include "usbd_msc_bot.h"
#include "usbd_msc_scsi.h"
#include "usbd_ioreq.h"
#include "usbd_msc_mem.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t MSC_BOT_DataLen;
uint8_t MSC_BOT_State;
uint8_t MSC_BOT_Status;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined (__ICCARM__)   /*!< IAR Compiler */
      #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN uint8_t MSC_BOT_Data[MSC_MEDIA_PACKET] __USB_ALIGN_END;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined (__ICCARM__)   /*!< IAR Compiler */
      #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN MSC_BOT_CBW_TypeDef MSC_BOT_cbw __USB_ALIGN_END;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined (__ICCARM__)   /*!< IAR Compiler */
      #pragma data_alignment=4
    #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN MSC_BOT_CSW_TypeDef MSC_BOT_csw __USB_ALIGN_END;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/* MSC BOT Private Function Prototype */
static void MSC_BOT_CBW_Decode(USB_OTG_CORE_HANDLE  *pdev);
static void MSC_BOT_SendData(USB_OTG_CORE_HANDLE  *pdev,
                             uint8_t* pbuf,
                             uint16_t len);
static void MSC_BOT_Abort(USB_OTG_CORE_HANDLE  *pdev);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Initialize the BOT Process
 ** \param pdev: device instance
 ** \retval None
 ******************************************************************************/
void MSC_BOT_Init(USB_OTG_CORE_HANDLE  *pdev)
{
    MSC_BOT_State  = BOT_IDLE;
    MSC_BOT_Status = BOT_STATE_NORMAL;
    USBD_STORAGE_fops->Init(0);

    DCD_EP_Flush(pdev, MSC_OUT_EP);
    DCD_EP_Flush(pdev, MSC_IN_EP);
    /* Prapare EP to Receive First BOT Cmd */
    DCD_EP_PrepareRx(pdev,
                     MSC_OUT_EP,
                     (uint8_t *)&MSC_BOT_cbw,
                     BOT_CBW_LENGTH);
}

/**
 *******************************************************************************
 ** \brief Reset the BOT Machine
 ** \param pdev: device instance
 ** \retval None
 ******************************************************************************/
void MSC_BOT_Reset(USB_OTG_CORE_HANDLE  *pdev)
{
    MSC_BOT_State  = BOT_IDLE;
    MSC_BOT_Status = BOT_STATE_RECOVERY;
    /* Prapare EP to Receive First BOT Cmd */
    DCD_EP_PrepareRx(pdev,
                     MSC_OUT_EP,
                     (uint8_t *)&MSC_BOT_cbw,
                     BOT_CBW_LENGTH);
}

/**
 *******************************************************************************
 ** \brief Uninitialize the BOT Machine
 ** \param pdev: device instance
 ** \retval None
 ******************************************************************************/
void MSC_BOT_DeInit(USB_OTG_CORE_HANDLE  *pdev)
{
    MSC_BOT_State = BOT_IDLE;
}

/**
 *******************************************************************************
 ** \brief Handle BOT IN data stage
 ** \param pdev: device instance
 ** \param epnum: endpoint index
 ** \retval None
 ******************************************************************************/
void MSC_BOT_DataIn(USB_OTG_CORE_HANDLE  *pdev,
                    uint8_t epnum)
{

    switch (MSC_BOT_State)
    {
        case BOT_DATA_IN:
            if (SCSI_ProcessCmd(pdev,
                                MSC_BOT_cbw.bLUN,
                                &MSC_BOT_cbw.CB[0]) < 0)
            {
                MSC_BOT_SendCSW(pdev, CSW_CMD_FAILED);
            }
            break;

        case BOT_SEND_DATA:
        case BOT_LAST_DATA_IN:
            MSC_BOT_SendCSW(pdev, CSW_CMD_PASSED);

            break;

        default:
            break;
    }
}

/**
 *******************************************************************************
 ** \brief Proccess MSC OUT data
 ** \param pdev: device instance
 ** \param epnum: endpoint index
 ** \retval None
 ******************************************************************************/
void MSC_BOT_DataOut(USB_OTG_CORE_HANDLE  *pdev,
                     uint8_t epnum)
{
    switch (MSC_BOT_State)
    {
        case BOT_IDLE:
            MSC_BOT_CBW_Decode(pdev);
            break;

        case BOT_DATA_OUT:

            if (SCSI_ProcessCmd(pdev,
                                MSC_BOT_cbw.bLUN,
                                &MSC_BOT_cbw.CB[0]) < 0)
            {
                MSC_BOT_SendCSW(pdev, CSW_CMD_FAILED);
            }

            break;

        default:
            break;
    }

}

/**
 *******************************************************************************
 ** \brief Decode the CBW command and set the BOT state machine accordingtly
 ** \param pdev: device instance
 ** \retval None
 ******************************************************************************/
static void  MSC_BOT_CBW_Decode(USB_OTG_CORE_HANDLE  *pdev)
{

    MSC_BOT_csw.dTag         = MSC_BOT_cbw.dTag;
    MSC_BOT_csw.dDataResidue = MSC_BOT_cbw.dDataLength;

    if ((USBD_GetRxCount(pdev, MSC_OUT_EP) != BOT_CBW_LENGTH) ||
        (MSC_BOT_cbw.dSignature != BOT_CBW_SIGNATURE) ||
        (MSC_BOT_cbw.bLUN > 1u) ||
        (MSC_BOT_cbw.bCBLength < 1u) ||
        (MSC_BOT_cbw.bCBLength > 16u))
    {

        SCSI_SenseCode(MSC_BOT_cbw.bLUN,
                       ILLEGAL_REQUEST,
                       INVALID_CDB);
        MSC_BOT_Status = BOT_STATE_ERROR;
        MSC_BOT_Abort(pdev);

    }else
    {
        if (SCSI_ProcessCmd(pdev,
                            MSC_BOT_cbw.bLUN,
                            &MSC_BOT_cbw.CB[0]) < 0)
        {
            MSC_BOT_Abort(pdev);
        }
        /*Burst xfer handled internally*/
        else if ((MSC_BOT_State != BOT_DATA_IN) &&
                 (MSC_BOT_State != BOT_DATA_OUT) &&
                 (MSC_BOT_State != BOT_LAST_DATA_IN))
        {
            if (MSC_BOT_DataLen > (uint16_t)0)
            {
                MSC_BOT_SendData(pdev,
                                 MSC_BOT_Data,
                                 MSC_BOT_DataLen);
            }
            //else if (MSC_BOT_DataLen == (uint16_t)0)  /* MISRAC */
            else
            {
                MSC_BOT_SendCSW(pdev,
                                CSW_CMD_PASSED);
            }
        }
        else
        {
            //
        }
    }
}

/**
 *******************************************************************************
 ** \brief Send the requested data
 ** \param pdev: device instance
 ** \param buf: pointer to data buffer
 ** \param len: Data Length
 ** \retval None
 ******************************************************************************/
static void  MSC_BOT_SendData(USB_OTG_CORE_HANDLE  *pdev,
                              uint8_t* buf,
                              uint16_t len)
{

    len                       = (uint16_t)min(MSC_BOT_cbw.dDataLength, len);
    MSC_BOT_csw.dDataResidue -= len;
    MSC_BOT_csw.bStatus       = CSW_CMD_PASSED;
    MSC_BOT_State             = BOT_SEND_DATA;

    DCD_EP_Tx(pdev, MSC_IN_EP, buf, (uint32_t)len);
}

/**
 *******************************************************************************
 ** \brief Send the Command Status Wrapper
 ** \param pdev: device instance
 ** \param status : CSW status
 ** \retval None
 ******************************************************************************/
void  MSC_BOT_SendCSW(USB_OTG_CORE_HANDLE  *pdev,
                      uint8_t CSW_Status)
{
    MSC_BOT_csw.dSignature = BOT_CSW_SIGNATURE;
    MSC_BOT_csw.bStatus    = CSW_Status;
    MSC_BOT_State          = BOT_IDLE;

    DCD_EP_Tx(pdev,
              MSC_IN_EP,
              (uint8_t *)&MSC_BOT_csw,
              BOT_CSW_LENGTH);

    /* Prapare EP to Receive next Cmd */
    DCD_EP_PrepareRx(pdev,
                     MSC_OUT_EP,
                     (uint8_t *)&MSC_BOT_cbw,
                     BOT_CBW_LENGTH);

}

/**
 *******************************************************************************
 ** \brief Abort the current transfer
 ** \param pdev: device instance
 ** \retval status
 ******************************************************************************/
static void  MSC_BOT_Abort(USB_OTG_CORE_HANDLE  *pdev)
{

    if ((MSC_BOT_cbw.bmFlags == (uint8_t)0) &&
        (MSC_BOT_cbw.dDataLength != (uint32_t)0) &&
        (MSC_BOT_Status == BOT_STATE_NORMAL))
    {
        DCD_EP_Stall(pdev, MSC_OUT_EP);
    }
    DCD_EP_Stall(pdev, MSC_IN_EP);

    if (MSC_BOT_Status == BOT_STATE_ERROR)
    {
        DCD_EP_PrepareRx(pdev,
                         MSC_OUT_EP,
                         (uint8_t *)&MSC_BOT_cbw,
                         BOT_CBW_LENGTH);
    }
}

/**
 *******************************************************************************
 ** \brief Complete the clear feature request
 ** \param pdev: device instance
 ** \param epnum: endpoint index
 ** \retval None
 ******************************************************************************/
void  MSC_BOT_CplClrFeature(USB_OTG_CORE_HANDLE  *pdev, uint8_t epnum)
{
    if (MSC_BOT_Status == BOT_STATE_ERROR)/* Bad CBW Signature */
    {
        DCD_EP_Stall(pdev, MSC_IN_EP);
        MSC_BOT_Status = BOT_STATE_NORMAL;
    }else if (((epnum & (uint8_t)0x80) == (uint8_t)0x80) && (MSC_BOT_Status != BOT_STATE_RECOVERY))
    {
        MSC_BOT_SendCSW(pdev, CSW_CMD_FAILED);
    }
    else
    {
        //
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
