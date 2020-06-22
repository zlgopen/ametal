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
/** \file usbd_msc_scsi.c
 **
 ** \brief This file provides all the USBD SCSI layer functions.
 **
 **   - 2019-05-15  1.0  Zhangxl First version for USB MSC device demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ametal.h"
#include "usbd_msc_bot.h"
#include "usbd_msc_scsi.h"
#include "usbd_msc_mem.h"
#include "usbd_msc_data.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
SCSI_Sense_TypeDef SCSI_Sense [SENSE_LIST_DEEPTH];
uint8_t SCSI_Sense_Head;
uint8_t SCSI_Sense_Tail;

uint32_t SCSI_blk_size;
uint32_t SCSI_blk_nbr;

uint64_t SCSI_blk_addr;     /* uint64_t for SD card size larger than 64GB */
uint32_t SCSI_blk_len;

USB_OTG_CORE_HANDLE *cdev;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static int8_t SCSI_TestUnitReady(uint8_t lun, uint8_t *params);
static int8_t SCSI_Inquiry(uint8_t lun, uint8_t *params);
static int8_t SCSI_ReadFormatCapacity(uint8_t lun, uint8_t *params);
static int8_t SCSI_ReadCapacity10(uint8_t lun, uint8_t *params);
static int8_t SCSI_RequestSense(uint8_t lun, uint8_t *params);
static int8_t SCSI_StartStopUnit(uint8_t lun, uint8_t *params);
static int8_t SCSI_ModeSense6(uint8_t lun, uint8_t *params);
static int8_t SCSI_ModeSense10(uint8_t lun, uint8_t *params);
static int8_t SCSI_Write10(uint8_t lun, uint8_t *params);
static int8_t SCSI_Read10(uint8_t lun, uint8_t *params);
static int8_t SCSI_Verify10(uint8_t lun, uint8_t *params);
static int8_t SCSI_CheckAddressRange(uint8_t lun,
                                     uint32_t blk_offset,
                                     uint16_t blk_nbr);
static int8_t SCSI_ProcessRead(uint8_t lun);

static int8_t SCSI_ProcessWrite(uint8_t lun);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Process SCSI commands
 ** \param pdev: device instance
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
int8_t SCSI_ProcessCmd(USB_OTG_CORE_HANDLE  *pdev,
                       uint8_t lun,
                       uint8_t *params)
{
    int8_t Res = (int8_t)0;
    cdev = pdev;

    switch (params[0])
    {
        case SCSI_TEST_UNIT_READY:
            Res = SCSI_TestUnitReady(lun, params);
            break;
        case SCSI_REQUEST_SENSE:
            Res = SCSI_RequestSense(lun, params);
            break;
        case SCSI_INQUIRY:
            Res = SCSI_Inquiry(lun, params);
            break;
        case SCSI_START_STOP_UNIT:
            Res = SCSI_StartStopUnit(lun, params);
            break;
        case SCSI_ALLOW_MEDIUM_REMOVAL:
            Res = SCSI_StartStopUnit(lun, params);
            break;
        case SCSI_MODE_SENSE6:
            Res = SCSI_ModeSense6(lun, params);
            break;
        case SCSI_MODE_SENSE10:
            Res = SCSI_ModeSense10(lun, params);
            break;
        case SCSI_READ_FORMAT_CAPACITIES:
            Res = SCSI_ReadFormatCapacity(lun, params);
            break;
        case SCSI_READ_CAPACITY10:
            Res = SCSI_ReadCapacity10(lun, params);
            break;
        case SCSI_READ10:
            Res = SCSI_Read10(lun, params);
            break;
        case SCSI_WRITE10:
            Res = SCSI_Write10(lun, params);
            break;
        case SCSI_VERIFY10:
            Res = SCSI_Verify10(lun, params);
            break;
        default:
            SCSI_SenseCode(lun,
                           ILLEGAL_REQUEST,
                           INVALID_CDB);
            Res = (int8_t)-1;
            break;
    }
    return Res;
}

/**
 *******************************************************************************
 ** \brief Process SCSI Test Unit Ready Command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_TestUnitReady(uint8_t lun, uint8_t *params)
{
    int8_t i8Ret = (int8_t)0;
    /* case 9 : Hi > D0 */
    if (MSC_BOT_cbw.dDataLength != 0u)
    {
        SCSI_SenseCode(MSC_BOT_cbw.bLUN,
                       ILLEGAL_REQUEST,
                       INVALID_CDB);
        i8Ret = (int8_t)-1;
    }
    else
    {
        if (USBD_STORAGE_fops->IsReady(lun) != 0u)
        {
            SCSI_SenseCode(lun,
                           NOT_READY,
                           MEDIUM_NOT_PRESENT);
            i8Ret = (int8_t)-1;
        }
        else
        {
            MSC_BOT_DataLen = 0u;
        }
    }
    return i8Ret;
}

/**
 *******************************************************************************
 ** \brief Process Inquiry command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t  SCSI_Inquiry(uint8_t lun, uint8_t *params)
{
    const uint8_t* pPage;
    uint16_t len;

    if (params[1] & 0x01u)/*Evpd is set*/
    {
        pPage = (const uint8_t *)MSC_Page00_Inquiry_Data;
        len   = LENGTH_INQUIRY_PAGE00;
    }else
    {

        pPage = (uint8_t *)&USBD_STORAGE_fops->pInquiry[lun * USBD_STD_INQUIRY_LENGTH];
        len   = (uint16_t)pPage[4] + (uint16_t)5;

        if (params[4] <= len)
        {
            len = params[4];
        }
    }
    MSC_BOT_DataLen = len;

    while (len)
    {
        len--;
        MSC_BOT_Data[len] = pPage[len];
    }
    return (int8_t)0;
}

/**
 *******************************************************************************
 ** \brief Process Read Capacity 10 command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_ReadCapacity10(uint8_t lun, uint8_t *params)
{
    int8_t i8Ret = (int8_t)0;
    if (USBD_STORAGE_fops->GetCapacity(lun, &SCSI_blk_nbr, &SCSI_blk_size) != 0u)
    {
        SCSI_SenseCode(lun,
                       NOT_READY,
                       MEDIUM_NOT_PRESENT);
        i8Ret = (int8_t)-1;
    }else
    {

        MSC_BOT_Data[0] = (uint8_t)((SCSI_blk_nbr - 1u) >> 24u);
        MSC_BOT_Data[1] = (uint8_t)((SCSI_blk_nbr - 1u) >> 16u);
        MSC_BOT_Data[2] = (uint8_t)((SCSI_blk_nbr - 1u) >> 8u);
        MSC_BOT_Data[3] = (uint8_t)(SCSI_blk_nbr - 1u);

        MSC_BOT_Data[4] = (uint8_t)(SCSI_blk_size >> 24u);
        MSC_BOT_Data[5] = (uint8_t)(SCSI_blk_size >> 16u);
        MSC_BOT_Data[6] = (uint8_t)(SCSI_blk_size >> 8u);
        MSC_BOT_Data[7] = (uint8_t)(SCSI_blk_size);

        MSC_BOT_DataLen = 8u;
    }
    return i8Ret;
}

/**
 *******************************************************************************
 ** \brief Process Read Format Capacity command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_ReadFormatCapacity(uint8_t lun, uint8_t *params)
{
    int8_t i8Ret = (int8_t)0;
    uint32_t blk_size;
    uint32_t blk_nbr;
    uint16_t i;

    for (i = 0u; i < 12u; i++)
    {
        MSC_BOT_Data[i] = 0u;
    }

    if (USBD_STORAGE_fops->GetCapacity(lun, &blk_nbr, &blk_size) != 0u)
    {
        SCSI_SenseCode(lun,
                       NOT_READY,
                       MEDIUM_NOT_PRESENT);
        i8Ret = (int8_t)-1;
    }else
    {
        MSC_BOT_Data[3]  = 0x08u;
        MSC_BOT_Data[4]  = (uint8_t)((blk_nbr - 1u) >> 24u);
        MSC_BOT_Data[5]  = (uint8_t)((blk_nbr - 1u) >> 16u);
        MSC_BOT_Data[6]  = (uint8_t)((blk_nbr - 1u) >> 8u);
        MSC_BOT_Data[7]  = (uint8_t)(blk_nbr - 1u);

        MSC_BOT_Data[8]  = 0x02u;
        MSC_BOT_Data[9]  = (uint8_t)(blk_size >> 16u);
        MSC_BOT_Data[10] = (uint8_t)(blk_size >> 8u);
        MSC_BOT_Data[11] = (uint8_t)(blk_size);

        MSC_BOT_DataLen  = 12u;
    }
    return i8Ret;
}

/**
 *******************************************************************************
 ** \brief Process Mode Sense6 command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_ModeSense6(uint8_t lun, uint8_t *params)
{

    uint16_t len = 8u;
    MSC_BOT_DataLen = len;

    while (len)
    {
        len--;
        MSC_BOT_Data[len] = MSC_Mode_Sense6_data[len];
    }
    return (int8_t)0;
}

/**
 *******************************************************************************
 ** \brief Process Mode Sense10 command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_ModeSense10(uint8_t lun, uint8_t *params)
{
    uint16_t len = 8u;

    MSC_BOT_DataLen = len;

    while (len)
    {
        len--;
        MSC_BOT_Data[len] = MSC_Mode_Sense10_data[len];
    }
    return (int8_t)0;
}

/**
 *******************************************************************************
 ** \brief Process Request Sense command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_RequestSense(uint8_t lun, uint8_t *params)
{
    uint8_t i;

    for (i = 0u; i < REQUEST_SENSE_DATA_LEN; i++)
    {
        MSC_BOT_Data[i] = 0u;
    }

    MSC_BOT_Data[0] = 0x70u;
    MSC_BOT_Data[7] = REQUEST_SENSE_DATA_LEN - 6u;

    if ((SCSI_Sense_Head != SCSI_Sense_Tail))
    {

        MSC_BOT_Data[2]  = SCSI_Sense[SCSI_Sense_Head].Skey;
        MSC_BOT_Data[12] = SCSI_Sense[SCSI_Sense_Head].w.b.ASCQ;
        MSC_BOT_Data[13] = SCSI_Sense[SCSI_Sense_Head].w.b.ASC;
        SCSI_Sense_Head++;

        if (SCSI_Sense_Head == SENSE_LIST_DEEPTH)
        {
            SCSI_Sense_Head = 0u;
        }
    }
    MSC_BOT_DataLen = REQUEST_SENSE_DATA_LEN;

    if (params[4] <= REQUEST_SENSE_DATA_LEN)
    {
        MSC_BOT_DataLen = params[4];
    }
    return (int8_t)0;
}

/**
 *******************************************************************************
 ** \brief Load the last error code in the error list
 ** \param lun: Logical unit number
 ** \param sKey: Sense Key
 ** \param ASC: Additional Sense Key
 ** \retval none
 ******************************************************************************/
void SCSI_SenseCode(uint8_t lun, uint8_t sKey, uint8_t ASC)
{
    SCSI_Sense[SCSI_Sense_Tail].Skey  = sKey;
    SCSI_Sense[SCSI_Sense_Tail].w.ASC = (uint32_t)ASC << 8u;
    SCSI_Sense_Tail++;
    if (SCSI_Sense_Tail == SENSE_LIST_DEEPTH)
    {
        SCSI_Sense_Tail = 0u;
    }
}

/**
 *******************************************************************************
 ** \brief Process Start Stop Unit command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_StartStopUnit(uint8_t lun, uint8_t *params)
{
    MSC_BOT_DataLen = 0u;
    return (int8_t)0;
}

/**
 *******************************************************************************
 ** \brief Process Read10 command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_Read10(uint8_t lun, uint8_t *params)
{
    int8_t i8Ret = (int8_t)0;
    if (MSC_BOT_State == BOT_IDLE) /* Idle */
    {
        /* case 10 : Ho <> Di */
        if ((MSC_BOT_cbw.bmFlags & 0x80u) != 0x80u)
        {
            SCSI_SenseCode(MSC_BOT_cbw.bLUN,
                           ILLEGAL_REQUEST,
                           INVALID_CDB);
            i8Ret = (int8_t)-1;
        }
        else if (USBD_STORAGE_fops->IsReady(lun) != 0u)
        {
            SCSI_SenseCode(lun,
                           NOT_READY,
                           MEDIUM_NOT_PRESENT);
            i8Ret = (int8_t)-1;
        }
        else
        {
            SCSI_blk_addr = ((uint64_t)params[2] << 24u) | \
                            ((uint64_t)params[3] << 16u) | \
                            ((uint64_t)params[4] << 8u) | \
                            (uint64_t)params[5];

            SCSI_blk_len = ((uint32_t)params[7] << 8u) | \
                           (uint32_t)params[8];


            if (SCSI_CheckAddressRange(lun, (uint32_t)SCSI_blk_addr, (uint16_t)SCSI_blk_len) < 0)
            {
                i8Ret = (int8_t)-1;             /* error */
            }
            else
            {
                MSC_BOT_State  = BOT_DATA_IN;
                SCSI_blk_addr *= SCSI_blk_size;
                SCSI_blk_len  *= SCSI_blk_size;

                /* cases 4,5 : Hi <> Dn */
                if (MSC_BOT_cbw.dDataLength != SCSI_blk_len)
                {
                    SCSI_SenseCode(MSC_BOT_cbw.bLUN,
                                   ILLEGAL_REQUEST,
                                   INVALID_CDB);
                    i8Ret = (int8_t)-1;
                }
            }
        }
    }

    if(((int8_t)-1) != i8Ret)
    {
        MSC_BOT_DataLen = (uint16_t)MSC_MEDIA_PACKET;
        i8Ret = SCSI_ProcessRead(lun);
    }
    return i8Ret;
}

/**
 *******************************************************************************
 ** \brief Process Write10 command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_Write10(uint8_t lun, uint8_t *params)
{
    int8_t i8Ret = (int8_t)0;
    if (MSC_BOT_State == BOT_IDLE) /* Idle */
    {
        /* case 8 : Hi <> Do */
        if ((MSC_BOT_cbw.bmFlags & 0x80u) == 0x80u)
        {
            SCSI_SenseCode(MSC_BOT_cbw.bLUN,
                           ILLEGAL_REQUEST,
                           INVALID_CDB);
            i8Ret = (int8_t)-1;
        }
        else
        {
            /* Check whether Media is ready */
            if (USBD_STORAGE_fops->IsReady(lun) != 0u)
            {
                SCSI_SenseCode(lun,
                               NOT_READY,
                               MEDIUM_NOT_PRESENT);
                i8Ret = (int8_t)-1;
            }
            /* Check If media is write-protected */
            else if (USBD_STORAGE_fops->IsWriteProtected(lun) != 0u)
            {
                SCSI_SenseCode(lun,
                               NOT_READY,
                               WRITE_PROTECTED);
                i8Ret = (int8_t)-1;
            }
            else
            {
                SCSI_blk_addr = ((uint64_t)params[2] << 24u) | \
                                ((uint64_t)params[3] << 16u) | \
                                ((uint64_t)params[4] << 8u) | \
                                (uint64_t)params[5];
                SCSI_blk_len  = ((uint32_t)params[7] << 8u) | \
                                (uint32_t)params[8];

                /* check if LBA address is in the right range */
                if (SCSI_CheckAddressRange(lun, (uint32_t)SCSI_blk_addr, (uint16_t)SCSI_blk_len) < 0)
                {
                    i8Ret = (int8_t)-1;             /* error */
                }
                else
                {
                    SCSI_blk_addr *= SCSI_blk_size;
                    SCSI_blk_len  *= SCSI_blk_size;

                    /* cases 3,11,13 : Hn,Ho <> D0 */
                    if (MSC_BOT_cbw.dDataLength != SCSI_blk_len)
                    {
                        SCSI_SenseCode(MSC_BOT_cbw.bLUN,
                                       ILLEGAL_REQUEST,
                                       INVALID_CDB);
                        i8Ret = (int8_t)-1;
                    }
                    else
                    {
                        /* Prepare EP to receive first data packet */
                        MSC_BOT_State = BOT_DATA_OUT;
                        DCD_EP_PrepareRx(cdev,
                                         MSC_OUT_EP,
                                         MSC_BOT_Data,
                                         (uint16_t)min(SCSI_blk_len, MSC_MEDIA_PACKET));
                    }
                }
            }
        }
    }
    else /* Write Process ongoing */
    {
        i8Ret = SCSI_ProcessWrite(lun);
    }
    return i8Ret;
}

/**
 *******************************************************************************
 ** \brief Process Verify10 command
 ** \param lun: Logical unit number
 ** \param params: Command parameters
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_Verify10(uint8_t lun, uint8_t *params)
{
    int8_t i8Ret = (int8_t)0;
    if ((params[1] & 0x02u) == 0x02u)
    {
        SCSI_SenseCode(lun, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
        i8Ret = (int8_t)-1;                 /* Error, Verify Mode Not supported*/
    }
    else if (SCSI_CheckAddressRange(lun, (uint32_t)SCSI_blk_addr, (uint16_t)SCSI_blk_len) < (int8_t)0)
    {
        i8Ret = (int8_t)-1;                 /* error */
    }
    else
    {
        MSC_BOT_DataLen = 0u;
    }
    return i8Ret;
}

/**
 *******************************************************************************
 ** \brief Check address range
 ** \param lun: Logical unit number
 ** \param blk_offset: first block address
 ** \param blk_nbr: number of block to be processed
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_CheckAddressRange(uint8_t lun, uint32_t blk_offset, uint16_t blk_nbr)
{
    int8_t i8Ret = (int8_t)0;
    if ((blk_offset + blk_nbr) > SCSI_blk_nbr)
    {
        SCSI_SenseCode(lun, ILLEGAL_REQUEST, ADDRESS_OUT_OF_RANGE);
        i8Ret = (int8_t)-1;
    }
    return i8Ret;
}

/**
 *******************************************************************************
 ** \brief Handle Read Process
 ** \param lun: Logical unit number
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_ProcessRead(uint8_t lun)
{
    uint32_t len;
    int8_t i8Ret = (int8_t)0;

    len = min(SCSI_blk_len, MSC_MEDIA_PACKET);

    if(0u == SCSI_blk_size)    /* C-STAT */
    {
        i8Ret = (int8_t)-1;
    }
    else if (USBD_STORAGE_fops->Read(lun,
                                MSC_BOT_Data,
                                SCSI_blk_addr / SCSI_blk_size,
                                len / SCSI_blk_size) < 0)
    {

        SCSI_SenseCode(lun, HARDWARE_ERROR, UNRECOVERED_READ_ERROR);
        i8Ret = (int8_t)-1;
    }
    else
    {
        DCD_EP_Tx(cdev,
                  MSC_IN_EP,
                  MSC_BOT_Data,
                  len);


        SCSI_blk_addr            += len;
        SCSI_blk_len             -= len;

        /* case 6 : Hi = Di */
        MSC_BOT_csw.dDataResidue -= len;

        if (SCSI_blk_len == 0u)
        {
            MSC_BOT_State = BOT_LAST_DATA_IN;
        }
    }
    return i8Ret;
}

/**
 *******************************************************************************
 ** \brief Handle Write Process
 ** \param lun: Logical unit number
 ** \retval status
 ******************************************************************************/
static int8_t SCSI_ProcessWrite(uint8_t lun)
{
    uint32_t len;
    int8_t i8Ret = (int8_t)0;

    len = min(SCSI_blk_len, MSC_MEDIA_PACKET);

    if(0u == SCSI_blk_size)/* C-STAT */
    {
        i8Ret = (int8_t)-1;
    }
    else if (USBD_STORAGE_fops->Write(lun,
                                 MSC_BOT_Data,
                                 SCSI_blk_addr / SCSI_blk_size,
                                 len / SCSI_blk_size) < 0)
    {
        SCSI_SenseCode(lun, HARDWARE_ERROR, WRITE_FAULT);
        i8Ret = (int8_t)-1;
    }
    else
    {
        SCSI_blk_addr            += len;
        SCSI_blk_len             -= len;

        /* case 12 : Ho = Do */
        MSC_BOT_csw.dDataResidue -= len;

        if (SCSI_blk_len == 0u)
        {
            MSC_BOT_SendCSW(cdev, CSW_CMD_PASSED);
        }else
        {
            /* Prapare EP to Receive next packet */
            DCD_EP_PrepareRx(cdev,
                             MSC_OUT_EP,
                             MSC_BOT_Data,
                             (uint16_t)min(SCSI_blk_len, MSC_MEDIA_PACKET));
        }
    }
    return i8Ret;
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
