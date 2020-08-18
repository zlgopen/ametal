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
/** \file usb_dcd_int.c
 **
 ** A detailed description is available at
 ** @link
        Peripheral Device interrupt subroutines.
    @endlink
 **
 **   - 2018-12-26  1.0  wangmin First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdio.h>
#include "usb_dcd_int.h"
#include "usb_bsp.h"
#include "am_hc32f460_usbd.h"

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
extern uint32_t DCD_WriteEmptyTxFifo(USB_OTG_CORE_HANDLE *pdev, uint32_t epnum);
extern uint32_t DCD_ReadDevInEP (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
uint8_t data_update_flag = 0;
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
char const *state_str[7] =
{
    "idle",
    "setup",
    "data_in",
    "data_out",
    "status_in",
    "status_out",
    "stall"
};
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
/**
 *******************************************************************************
 **\brief  USBD_OTG_EP1OUT_ISR_Handler
 **        handles all USB Interrupts
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DOEPINTn_TypeDef  doepint;
    USB_OTG_DEPXFRSIZ_TypeDef  deptsiz;

    doepint.d32 = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[1]->DOEPINT);
    doepint.d32&= USB_OTG_READ_REG32(&pdev->regs.DREGS->DOUTEP1MSK);

    /* Transfer complete */
    if ( doepint.b.xfercompl )
    {
        /* Clear the bit in DOEPINTn for this interrupt */
        CLEAR_OUT_EP_INTR(1, out_xfercompl);
        if (pdev->cfg.dma_enable == 1)
        {
            deptsiz.d32 = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[1]->DOEPTSIZ));
            /*ToDo : handle more than one single MPS size packet */
            pdev->dev.out_ep[1].xfer_count = pdev->dev.out_ep[1].maxpacket - \
            deptsiz.b.xfersize;
        }
        /* Inform upper layer: data ready */
        /* RX COMPLETE */
        USBD_DCD_INT_fops->DataOutStage(pdev , 1);
    }

    /* Endpoint disable  */
    if ( doepint.b.epdisabled )
    {
        /* Clear the bit in DOEPINTn for this interrupt */
        CLEAR_OUT_EP_INTR(1, out_epdisabled);
    }

    return 1;
}

/**
 *******************************************************************************
 **\brief  USBD_OTG_EP1IN_ISR_Handler
 **        handles all USB Interrupts
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DIEPINTn_TypeDef  diepint;
    uint32_t fifoemptymsk, msk, emp;

    msk = USB_OTG_READ_REG32(&pdev->regs.DREGS->DINEP1MSK);
    emp = USB_OTG_READ_REG32(&pdev->regs.DREGS->DIEPEMPMSK);
    msk |= ((emp >> 1 ) & 0x1) << 7;
    diepint.d32  = USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[1]->DIEPINT) & msk;

    if ( diepint.b.xfercompl )
    {
        fifoemptymsk = 0x1 << 1;
        USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, fifoemptymsk, 0);
        CLEAR_IN_EP_INTR(1, in_xfercompl);
        /* TX COMPLETE */
        USBD_DCD_INT_fops->DataInStage(pdev , 1);
    }
    if ( diepint.b.epdisabled )
    {
        CLEAR_IN_EP_INTR(1, in_epdisabled);
    }
    if ( diepint.b.timeout )
    {
        CLEAR_IN_EP_INTR(1, in_timeout);
    }
    if (diepint.b.intktxfemp)
    {
        CLEAR_IN_EP_INTR(1, in_intktxfemp);
    }
    if (diepint.b.inepnakeff)
    {
        CLEAR_IN_EP_INTR(1, in_inepnakeff);
    }
    if (diepint.b.emptyintr)
    {
        DCD_WriteEmptyTxFifo(pdev , 1);
        CLEAR_IN_EP_INTR(1, in_emptyintr);
    }
    return 1;
}
#endif

/**
 *******************************************************************************
 **\brief  USBF_OTG_ISR_Handler
 **        handles all USB Interrupts
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
 #if 0
uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintr_status;
    uint32_t retval = 0ul;
    uint32_t ReadCoreItrTemp = 0ul;   /* C-STAT */

    if (USB_OTG_IsDeviceMode(pdev)) /* ensure that we are in device mode */
    {
        ReadCoreItrTemp = USB_OTG_ReadCoreItr(pdev);      /* C-STAT */
        gintr_status.b = *(stc_bUSB_OTG_GINTSTS_t*)&ReadCoreItrTemp;   /* C-STAT */
        /* avoid spurious interrupt */
        if (!*(uint32_t*)&gintr_status.b) /* C-STAT */
        {
            //return 0ul;   /* MISRAC 2004*/
            retval = 0ul;
        }
        /* Out endpoint interrupt */
        else if (gintr_status.b.outepintr)
        {
            printf("wyy  DCD_HandleOutEP_ISR\r\n");
            retval |= DCD_HandleOutEP_ISR(pdev);
        }
        /* In endpoint interrupt */
        else if (gintr_status.b.inepint)
        {        
            printf("wyy  DCD_HandleInEP_ISR\r\n");
            retval |= DCD_HandleInEP_ISR(pdev);
        }
        /* Mode mismatch interrupt */
        else if (gintr_status.b.modemismatch)
        {
            printf("wyy  modemismatch\r\n");
        
            USB_OTG_GINTSTS_TypeDef  gintsts;

            /* Clear interrupt */
            gintsts.d32 = 0ul;
            gintsts.b.modemismatch = 1u;
            USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);
        }
        /* Resume/remote wakeup detected interrupt */
        else if (gintr_status.b.wkupintr)
        {
            printf("wyy  DCD_HandleResume_ISR\r\n");        
            retval |= DCD_HandleResume_ISR(pdev);
        }
        /* USB suspend interrupt */
        else if (gintr_status.b.usbsuspend)
        {
            printf("wyy  DCD_HandleUSBSuspend_ISR\r\n");                
            retval |= DCD_HandleUSBSuspend_ISR(pdev);
        }
        /* Start of frame interrupt */
        else if (gintr_status.b.sofintr)
        {
            printf("wyy  DCD_HandleSof_ISR\r\n");                        
            retval |= DCD_HandleSof_ISR(pdev);
        }
        /* RxFIFO non-empty interrupt */
        else if (gintr_status.b.rxstsqlvl)
        {
            printf("wyy  DCD_HandleRxStatusQueueLevel_ISR\r\n");                        
        
            retval |= DCD_HandleRxStatusQueueLevel_ISR(pdev);
        }
        /* USB reset interrupt */
        else if (gintr_status.b.usbreset)
        {
            printf("wyy  DCD_HandleUsbReset_ISR\r\n");                        
        
            retval |= DCD_HandleUsbReset_ISR(pdev);
        }
        /* Enumeration done interrupt */
        else if (gintr_status.b.enumdone)
        {
            printf("wyy  DCD_HandleEnumDone_ISR\r\n");                        
            retval |= DCD_HandleEnumDone_ISR(pdev);
        }
        /* Incomplete periodic transfer */
        else if (gintr_status.b.incomplisoin)
        {
            printf("wyy  DCD_IsoINIncomplete_ISR\r\n");                        
            retval |= DCD_IsoINIncomplete_ISR(pdev);
        }
        /* Incomplete isochronous IN transfer */
        else if (gintr_status.b.incomplisoout)
        {
            printf("wyy  DCD_IsoOUTIncomplete_ISR\r\n");                        
            retval |= DCD_IsoOUTIncomplete_ISR(pdev);
        }

#ifdef VBUS_SENSING_ENABLED
        /* VBUS valid interrupt */
        else if (gintr_status.b.vbusvint)
        {
            printf("vbus\n");
            printf("wyy  DCD_SessionRequest_ISR\r\n");                        
            retval |= DCD_SessionRequest_ISR(pdev);
        }

    //    else if (gintr_status.b.otgintr)
    //    {
    //      retval |= DCD_OTG_ISR(pdev);
    //    }
        else
        {
            //
        }
#endif
    }
    return retval;
}
#endif
#ifdef VBUS_SENSING_ENABLED
/**
 *******************************************************************************
 **\brief  DCD_SessionRequest_ISR
 **        Indicates that the USB_OTG controller has detected a connection
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_SessionRequest_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintsts;
    USB_OTG_PCGCCTL_TypeDef  power;
    USBD_DCD_INT_fops->DevConnected (pdev);

//    printf("SessionRequest !!\r\n");
    /* Clear interrupt */
    gintsts.d32 = 0ul;
    gintsts.b.vbusvint = 1u;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);
    if(pdev->cfg.low_power)
    {
        /* un-gate USB Core clock */
        power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
        power.b.gatehclk = 0u;
        power.b.stoppclk = 0u;
        USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
    }

    return 1ul;
}

/**
 *******************************************************************************
 **\brief  DCD_OTG_ISR
 **        Indicates that the USB_OTG controller has detected an OTG event:
 **                used to detect the end of session i.e. disconnection
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
//static uint32_t DCD_OTG_ISR(USB_OTG_CORE_HANDLE *pdev)
//{
//
//  USB_OTG_GOTGINT_TypeDef  gotgint;
//  printf("\tDCD_OTG_ISR !!\n");
//  gotgint.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GOTGINT);
//
//  if (gotgint.b.sesenddet)
//  {
//    USBD_DCD_INT_fops->DevDisconnected (pdev);
//  }
//  /* Clear OTG interrupt */
//  USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GOTGINT, gotgint.d32);
//  return 1;
//}
#endif
/**
 *******************************************************************************
 **\brief  DCD_HandleResume_ISR
 **        Indicates that the USB_OTG controller has detected a resume or
 **                remote Wake-up sequence
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_HandleResume_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintsts;
    USB_OTG_DCTL_TypeDef     devctl;
    USB_OTG_PCGCCTL_TypeDef  power;

//    printf("resume !!\n");
    if(pdev->cfg.low_power)
    {
        /* un-gate USB Core clock */
        power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
        power.b.gatehclk = 0u;
        power.b.stoppclk = 0u;
        USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
    }

    /* Clear the Remote Wake-up Signaling */
    devctl.d32 = 0ul;
    devctl.b.rmtwkupsig = 1u;
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, devctl.d32, 0ul);

    /* Inform upper layer by the Resume Event */
    USBD_DCD_INT_fops->Resume (pdev);

    /* Clear interrupt */
    gintsts.d32 = 0ul;
    gintsts.b.wkupintr = 1u;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);
    return 1ul;
}

/**
 *******************************************************************************
 **\brief  USB_OTG_HandleUSBSuspend_ISR
 **        Indicates that SUSPEND state has been detected on the USB
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_HandleUSBSuspend_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintsts;
    USB_OTG_PCGCCTL_TypeDef  power;
    USB_OTG_DSTS_TypeDef     dsts;
    __IO uint8_t prev_status = 0u;

//    printf("suspend !!\n");
    prev_status = pdev->dev.device_status;
    USBD_DCD_INT_fops->Suspend (pdev);

    dsts.b = *(__IO stc_bUSB_OTG_DSTS_t*)&USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);   /* C-STAT */

    /* Clear interrupt */
    gintsts.d32 = 0ul;
    gintsts.b.usbsuspend = 1u;
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);

    if((prev_status  == USB_OTG_CONFIGURED) &&
       (pdev->cfg.low_power) && (dsts.b.suspsts == 1u)  &&
       (pdev->dev.connection_status == 1u))
    {
        /*  switch-off the clocks */
        power.d32 = 0ul;
        power.b.stoppclk = 1u;
        USB_OTG_MODIFY_REG32(pdev->regs.PCGCCTL, 0ul, power.d32);

        power.b.gatehclk = 1u;
        USB_OTG_MODIFY_REG32(pdev->regs.PCGCCTL, 0ul, power.d32);

        /* Request to enter Sleep mode after exit from current ISR */
        // SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk);
    }
    return 1ul;
}

/**
 *******************************************************************************
 **\brief  DCD_HandleInEP_ISR
 **        Indicates that an IN EP has a pending Interrupt
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_HandleInEP_ISR(am_hc32f460_usbd_dev_t *p_dev, USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DIEPINTn_TypeDef  diepint;

    uint32_t ep_intr;
    uint32_t epnum = 0ul;
    uint32_t fifoemptymsk;
    uint32_t u32ReadInEpTmp = 0ul; /* C-STAT */

    *(uint32_t*)&diepint.b = 0ul;    /* C-STAT */
    ep_intr = USB_OTG_ReadDevAllInEPItr(pdev);

    while ( ep_intr )
    {
        if (ep_intr&0x1u) /* In ITR */
        {
            //diepint.d32 = DCD_ReadDevInEP(pdev , epnum); /* Get In ITR status */  /* C-STAT */
            u32ReadInEpTmp = DCD_ReadDevInEP(pdev , (uint8_t)epnum);
            diepint.b = *(stc_bUSB_OTG_DIEPINTn_t*)&u32ReadInEpTmp;
//            printf("****************[DCD_HandleInEP_ISR] epnum = %d**************\r\n", epnum);
            if ( diepint.b.xfercompl )
            {            
//                printf("11111111111111111111111111111\r\n", epnum);            
                fifoemptymsk = (uint32_t)0x1 << epnum;
                USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, fifoemptymsk, 0ul);
                CLEAR_IN_EP_INTR(epnum, in_xfercompl);
                /* TX COMPLETE */
                USBD_DCD_INT_fops->DataInStage(pdev , epnum);

                if (pdev->cfg.dma_enable == 1u)
                {
                    if((epnum == 0u) && (pdev->dev.device_state == USB_OTG_EP0_STATUS_IN))
                    {
                        /* prepare to rx more setup packets */
                        ///printf("status in\n");
                        USB_OTG_EP0_OutStart(pdev);
                        pdev->dev.device_state = USB_OTG_EP0_IDLE;
                    }
                } 
                if(epnum != 0) {            
//                    printf("************[DCD_HandleInEP_ISR] pfn_callback = %d *********\r\n",epnum);
                    /* 端点回调函数  */
                    if (p_dev->device.endpoint_info[epnum].pfn_callback != NULL) {
//                         printf("6666666666666\r\n", epnum);                                                
                        (p_dev->device.endpoint_info[epnum].pfn_callback)(p_dev->device.endpoint_info[epnum].p_arg);
                    }
                }                
                ///printf("ctrl-in %x\n",USB_OTG_READ_REG32(&(pdev->regs.INEP_REGS[epnum]->DIEPTSIZ)));
            }
            if ( diepint.b.timeout )
            {
//                printf("22222222222222222\r\n", epnum);            
                CLEAR_IN_EP_INTR(epnum, in_timeout);
            }
            if (diepint.b.intktxfemp)
            {
//                printf("3333333333333333\r\n", epnum);                        
                CLEAR_IN_EP_INTR(epnum, in_intktxfemp);
            }
            if (diepint.b.inepnakeff)
            {
//                printf("44444444444444444\r\n", epnum);                                    
                CLEAR_IN_EP_INTR(epnum, in_inepnakeff);
            }
            if ( diepint.b.epdisabled )
            {
//                printf("55555555555555555\r\n", epnum);                                                
                CLEAR_IN_EP_INTR(epnum, in_epdisabled);
            }
            if (diepint.b.emptyintr)
            {
//                printf("************[tx fifo empty] epnum = %d *********\r\n",epnum);
                DCD_WriteEmptyTxFifo(pdev , epnum);
                fifoemptymsk = (uint32_t)0x1 << epnum;
                USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, fifoemptymsk, 0ul);
                CLEAR_IN_EP_INTR(epnum, in_emptyintr);                                
            }
        }
        epnum++;
        ep_intr >>= 1u;
    }

    return 1ul;
}

/**
 *******************************************************************************
 **\brief  DCD_HandleOutEP_ISR
 **        Indicates that an OUT EP has a pending Interrupt
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
#if 0
static uint32_t DCD_HandleOutEP_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    uint32_t ep_intr;
    USB_OTG_DOEPINTn_TypeDef  doepint;
    USB_OTG_DEPXFRSIZ_TypeDef  deptsiz;
    uint32_t epnum = 0ul;

    doepint.d32 = 0ul;

    /* Read in the device interrupt bits */
    ep_intr = USB_OTG_ReadDevAllOutEp_itr(pdev);

    while ( ep_intr )
    {
        if (ep_intr&0x1)
        {
            doepint.d32 = USB_OTG_ReadDevOutEP_itr(pdev, epnum);

            /* Transfer complete */
            if ( doepint.b.xfercompl )
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                CLEAR_OUT_EP_INTR(epnum, out_xfercompl);
                if (pdev->cfg.dma_enable == 1)
                {
                    deptsiz.d32 = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[epnum]->DOEPTSIZ));
                    /*ToDo : handle more than one single MPS size packet */
                    pdev->dev.out_ep[epnum].xfer_count = pdev->dev.out_ep[epnum].maxpacket - \
                    deptsiz.b.xfersize;
                    printf("ep%d xfer_count %d t %d  s %d\n",epnum, pdev->dev.out_ep[epnum].xfer_count,
                    deptsiz.b.xfersize, pdev->dev.device_state);
                }
                /* Inform upper layer: data ready */
                /* RX COMPLETE */
                USBD_DCD_INT_fops->DataOutStage(pdev , epnum);

                if (pdev->cfg.dma_enable == 1u)
                {
                    if((epnum == 0u) && (pdev->dev.device_state == USB_OTG_EP0_STATUS_OUT))
                    {
                        /* prepare to rx more setup packets */
                         // printf("prepare to rx more setup..\n");
                        USB_OTG_EP0_OutStart(pdev);
                    }
                }
                if (epnum == 0u)
                {
                    if (pdev->dev.ep0_state == USB_OTG_EP0_IDLE /*&&
                    (pdev->dev.out_ep[epnum].xfer_count == 8)*/)
                    {
                        USB_OTG_BSP_uDelay(100);
                        doepint.d32 = USB_OTG_ReadDevOutEP_itr(pdev, epnum);
                        //if ( doepint.b.setup )
                        if ( (1 << 15)  & USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[0]->DOEPINT))
                        {
                            CLEAR_OUT_EP_INTR(epnum, out_sr);
                            if (pdev->dev.ep0_state == USB_OTG_EP0_IDLE)
                            {
                                pdev->dev.ep0_state = USB_OTG_EP0_SETUP;
                                /* inform the upper layer that a setup packet is available */
                                /* SETUP COMPLETE */
                                USBD_DCD_INT_fops->SetupStage(pdev);
                                printf("setup7 %d\n",pdev->dev.device_state);
                                CLEAR_OUT_EP_INTR(epnum, out_setup);
                            }
                        }


                    }
                    else if (pdev->dev.ep0_state == USB_OTG_EP0_STATUS_OUT)
                    {
                        pdev->dev.ep0_state = USB_OTG_EP0_IDLE;
                    }
                }
            }
            /* Endpoint disable  */
            if ( doepint.b.epdisabled )
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                CLEAR_OUT_EP_INTR(epnum, out_epdisabled);
            }
            /* Setup Phase Done (control EPs) */
            doepint.d32 = USB_OTG_ReadDevOutEP_itr(pdev, epnum);
            if ( doepint.b.setup )
            {
                /* inform the upper layer that a setup packet is available */
                /* SETUP COMPLETE */
                if (pdev->dev.ep0_state == USB_OTG_EP0_IDLE)
                {
                    pdev->dev.ep0_state = USB_OTG_EP0_SETUP;
                    USBD_DCD_INT_fops->SetupStage(pdev);
                    printf("setup %d\n",pdev->dev.device_state);
                }
                CLEAR_OUT_EP_INTR(epnum, out_setup);
            }

        }
        epnum++;
        ep_intr >>= 1;
    }
    return 1ul;
}
#else
uint32_t DCD_HandleOutEP_ISR(am_hc32f460_usbd_dev_t *p_dev, USB_OTG_CORE_HANDLE *pdev)
{
    uint32_t ep_intr;
    USB_OTG_DOEPINTn_TypeDef  doepint;
    USB_OTG_DEPXFRSIZ_TypeDef  deptsiz;
    uint32_t epnum = 0ul;
    uint32_t xfer;   /* MISRAC*/
    uint32_t u32ReadOutEpTmp = 0ul; /* C-STAT */
    uint32_t u32ReadEpSizeTmp = 0ul; /* C-STAT */

    *(__IO uint32_t*)&doepint.b = 0ul;  /* C-STAT */

    /* Read in the device interrupt bits */
    ep_intr = USB_OTG_ReadDevAllOutEp_itr(pdev);

    while ( ep_intr )
    {
        if (ep_intr&0x1ul)
        {
            //doepint.d32 = USB_OTG_ReadDevOutEP_itr(pdev, epnum);  /* C-STAT */
            u32ReadOutEpTmp = USB_OTG_ReadDevOutEP_itr(pdev, (uint8_t)epnum);
            doepint.b = *(__IO stc_bUSB_OTG_DOEPINTn_t*)&u32ReadOutEpTmp;

            /* Transfer complete */
            if ( doepint.b.xfercompl )
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                CLEAR_OUT_EP_INTR(epnum, out_xfercompl);
                if (pdev->cfg.dma_enable == 1u)
                {
                    //deptsiz.d32 = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[epnum]->DOEPTSIZ));  /* C-STAT */
                    u32ReadEpSizeTmp = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[epnum]->DOEPTSIZ));
                    deptsiz.b = *(stc_bUSB_OTG_DEPXFRSIZ_t*)&u32ReadEpSizeTmp;

                    /*ToDo : handle more than one single MPS size packet */
                    xfer = __MIN(pdev->dev.out_ep[epnum].maxpacket,pdev->dev.out_ep[epnum].xfer_len);
                    pdev->dev.out_ep[epnum].xfer_count = xfer - deptsiz.b.xfersize;
                    if (epnum != 0u)
                    {
                        pdev->dev.out_ep[epnum].xfer_count = pdev->dev.out_ep[epnum].xfer_len - deptsiz.b.xfersize;
                    }
                }
                /* Inform upper layer: data ready */
                /* RX COMPLETE */
                USBD_DCD_INT_fops->DataOutStage(pdev , epnum);

                if (pdev->cfg.dma_enable == 1u)
                {
                    if((epnum == 0u) && (pdev->dev.device_state == USB_OTG_EP0_STATUS_OUT))
                    {
                        /* prepare to rx more setup packets */
                        ///printf("status out\n");
                        USB_OTG_EP0_OutStart(pdev);
                        pdev->dev.device_state = USB_OTG_EP0_IDLE;
                    }
                }

               if(epnum != 0) {            
//                   printf("************[DCD_HandleOutEP_ISR] pfn_callback = %d *********\r\n",epnum);
                   /* 端点回调函数  */
                   if (p_dev->device.endpoint_info[epnum].pfn_callback != NULL) {
//                        printf("777777777\r\n", epnum);                                                
                       (p_dev->device.endpoint_info[epnum].pfn_callback)(p_dev->device.endpoint_info[epnum].p_arg);
                   }
               } 
            }
            /* Endpoint disable  */
            if ( doepint.b.epdisabled )
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                CLEAR_OUT_EP_INTR(epnum, out_epdisabled);
            }

            //doepint.d32 = USB_OTG_ReadDevOutEP_itr(pdev, epnum);  /* C-STAT */
            u32ReadOutEpTmp = USB_OTG_ReadDevOutEP_itr(pdev, (uint8_t)epnum);
            doepint.b = *(stc_bUSB_OTG_DOEPINTn_t*)&u32ReadOutEpTmp;

            /* Setup Phase Done (control EPs) */
            if ( doepint.b.setup )
            {
                /* inform the upper layer that a setup packet is available */
                /* SETUP COMPLETE */
                USBD_DCD_INT_fops->SetupStage(pdev);
//                __usb_setup_handle(p_dev, pdev);
                CLEAR_OUT_EP_INTR(epnum, out_setup);
            }

//           if(epnum != 0) {            
//               printf("************[DCD_HandleOutEP_ISR] pfn_callback = %d *********\r\n",epnum);
//               /* 端点回调函数  */
//               if (p_dev->device.endpoint_info[epnum].pfn_callback != NULL) {
//                    printf("777777777\r\n", epnum);                                                
//                   (p_dev->device.endpoint_info[epnum].pfn_callback)(p_dev->device.endpoint_info[epnum].p_arg);
//               }
//           }             
        }
        epnum++;
        ep_intr >>= 1u;
    }
    return 1ul;
}
#endif
/**
 *******************************************************************************
 **\brief  DCD_HandleSof_ISR
 **        Handles the SOF Interrupts
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_HandleSof_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  GINTSTS;


    USBD_DCD_INT_fops->SOF(pdev);

    /* Clear interrupt */
    GINTSTS.d32 = 0ul;
    GINTSTS.b.sofintr = 1u;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, GINTSTS.d32);

    return 1ul;
}

/**
 *******************************************************************************
 **\brief  DCD_HandleRxStatusQueueLevel_ISR
 **        Handles the Rx Status Queue Level Interrupt
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_HandleRxStatusQueueLevel_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTMSK_TypeDef  int_mask;
    USB_OTG_DRXSTS_TypeDef   status;
    USB_OTG_EP *ep;
    uint32_t u32Tmp;

    /* Disable the Rx Status Queue Level interrupt */
    int_mask.d32 = 0ul;
    int_mask.b.rxstsqlvl = 1u;
    USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, int_mask.d32, 0ul);

    /* Get the Status from the top of the FIFO */
    //status.b = *(__IO stc_bUSB_OTG_DRXSTS_t*)&USB_OTG_READ_REG32( &pdev->regs.GREGS->GRXSTSP ); /* C-STAT */
    u32Tmp = USB_OTG_READ_REG32( &pdev->regs.GREGS->GRXSTSP );
    status.b = *(__IO stc_bUSB_OTG_DRXSTS_t*)&u32Tmp; /* C-STAT */
    ep = &pdev->dev.out_ep[status.b.epnum];
//    printf("[DCD_HandleRxStatusQueueLevel_ISR] ep = %d, cnt = %d\r\n",status.b.epnum,status.b.bcnt);

    switch (status.b.pktsts)
    {
        case STS_GOUT_NAK:
            break;
        case STS_DATA_UPDT:
            if (status.b.bcnt)
            {
//                if (status.b.epnum == 2)
//                    printf("ep%d cnt %d\n",status.b.epnum,status.b.bcnt);
//                printf("****************[DCD_HandleRxStatusQueueLevel_ISR][STS_DATA_UPDT]  USB_OTG_ReadPacket]**************\r\n");
                USB_OTG_ReadPacket(pdev,ep->xfer_buff, (uint16_t)status.b.bcnt);
                ep->xfer_buff += status.b.bcnt;
                ep->xfer_count += status.b.bcnt;
                data_update_flag = 1;                
            } else
            {
                ;//printf("ep%d cnt %d\n",status.b.epnum,status.b.bcnt);
            }
            break;
        case STS_XFER_COMP:
            break;
        case STS_SETUP_COMP:
            break;
        case STS_SETUP_UPDT:
            /* Copy the setup packet received in FIFO into the setup buffer in RAM */
            //printf("ep%d setup %d\n",status.b.epnum,status.b.bcnt);
//            printf("****************[DCD_HandleRxStatusQueueLevel_ISR][STS_SETUP_UPDT] USB_OTG_ReadPacket]**************\r\n");
            USB_OTG_ReadPacket(pdev , pdev->dev.setup_packet, 8u);
            ep->xfer_count += status.b.bcnt;
            break;
        default:
            break;
    }
    /* Enable the Rx Status Queue Level interrupt */
    USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, 0ul, int_mask.d32);
    return 1ul;
}

/**
 *******************************************************************************
 **\brief  DCD_WriteEmptyTxFifo
 **        check FIFO for the next packet to be loaded
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_WriteEmptyTxFifo(USB_OTG_CORE_HANDLE *pdev, uint32_t epnum)
{
    USB_OTG_DTXFSTSn_TypeDef  txstatus;
    USB_OTG_EP *ep;
    uint32_t len = 0ul;
    uint32_t len32b;

    *(uint32_t*)&txstatus.b = 0ul;  /* C-STAT */

    ep = &pdev->dev.in_ep[epnum];
    len = ep->xfer_len - ep->xfer_count;

    if (len > ep->maxpacket)
    {
        len = ep->maxpacket;
    }

    len32b = (len + 3u) / 4u;
    txstatus.b = *(__IO stc_bUSB_OTG_DTXFSTSn_t*)&USB_OTG_READ_REG32( &pdev->regs.INEP_REGS[epnum]->DTXFSTS);  /* C-STAT */
    //printf("ep%d tx len %d\n",epnum,len);

    while ((txstatus.b.txfspcavail > len32b) && (ep->xfer_count < ep->xfer_len) && (ep->xfer_len != 0u))
    {
        /* Write the FIFO */
        len = ep->xfer_len - ep->xfer_count;

        if (len > ep->maxpacket)
        {
            len = ep->maxpacket;
        }
        len32b = (len + 3u) / 4u;

//        printf("****************[DCD_WriteEmptyTxFifo USB_OTG_WritePacket]**************\r\n");
        USB_OTG_WritePacket (pdev , ep->xfer_buff, (uint8_t)epnum, (uint16_t)len);

        ep->xfer_buff  += len;
        ep->xfer_count += len;

        txstatus.b = *(__IO stc_bUSB_OTG_DTXFSTSn_t*)&USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[epnum]->DTXFSTS);  /* C-STAT */
    }
    return 1ul;
}

/**
 *******************************************************************************
 **\brief  DCD_HandleUsbReset_ISR
 **        This interrupt occurs when a USB Reset is detected
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_HandleUsbReset_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_DAINT_TypeDef    daintmsk;
    USB_OTG_DOEPMSK_TypeDef  doepmsk;
    USB_OTG_DIEPMSK_TypeDef  diepmsk;
    USB_OTG_DCFG_TypeDef     dcfg;
    USB_OTG_DCTL_TypeDef     dctl;
    USB_OTG_GINTSTS_TypeDef  gintsts;
    uint32_t i;

    dctl.d32 = 0ul;
    daintmsk.d32 = 0ul;
    doepmsk.d32 = 0ul;
    diepmsk.d32 = 0ul;
    dcfg.d32 = 0ul;
    gintsts.d32 = 0ul;

//    printf("%s\n",__FUNCTION__);
    /* Clear the Remote Wake-up Signaling */
    dctl.b.rmtwkupsig = 1u;
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, dctl.d32, 0ul );

    /* Flush the Tx FIFO */
    USB_OTG_FlushTxFifo(pdev, (uint32_t)0);

    for (i = 0ul; i < pdev->cfg.dev_endpoints ; i++)
    {
        USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPINT, 0xFFul);
        USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPINT, 0xFFul);
    }
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINT, 0xFFFFFFFFul );

    daintmsk.ep.in = 1u;
    daintmsk.ep.out = 1u;
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINTMSK, daintmsk.d32 );

    doepmsk.b.setup = 1u;
    doepmsk.b.xfercompl = 1u;
    doepmsk.b.epdisabled = 1u;
    doepmsk.b.stsphsercvd = 1u;
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOEPMSK, doepmsk.d32 );
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOUTEP1MSK, doepmsk.d32 );
#endif
    diepmsk.b.xfercompl = 1u;
    diepmsk.b.timeout = 1u;
    diepmsk.b.epdisabled = 1u;

    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DIEPMSK, diepmsk.d32 );
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DINEP1MSK, diepmsk.d32 );
#endif
    /* Reset Device Address */
    dcfg.d32 = USB_OTG_READ_REG32( &pdev->regs.DREGS->DCFG);
    dcfg.b.devaddr = 0u;
    USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DCFG, dcfg.d32);

    /* setup EP0 to receive SETUP packets */
//    printf("reset\n");
    USB_OTG_EP0_OutStart(pdev);

    /* Clear interrupt */
    gintsts.d32 = 0ul;
    gintsts.b.usbreset = 1ul;
    USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);

    /*Reset internal state machine */
    USBD_DCD_INT_fops->Reset(pdev);
    return 1ul;
}

/**
 *******************************************************************************
 **\brief  DCD_HandleEnumDone_ISR
 **        Read the device status register and set the device speed
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_HandleEnumDone_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef  gintsts;
    USB_OTG_GUSBCFG_TypeDef  gusbcfg;

    USB_OTG_EP0Activate(pdev);

    /* Set USB turn-around time based on device speed and PHY interface. */
    gusbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);

    /* Full or High speed */
    if ( USB_OTG_GetDeviceSpeed(pdev) == USB_SPEED_HIGH)
    {
        pdev->cfg.speed            = USB_OTG_SPEED_HIGH;
        pdev->cfg.mps              = USB_OTG_HS_MAX_PACKET_SIZE ;
        gusbcfg.b.usbtrdtim = 9u;
    }
    else
    {
        pdev->cfg.speed            = USB_OTG_SPEED_FULL;
        pdev->cfg.mps              = USB_OTG_FS_MAX_PACKET_SIZE ;
        gusbcfg.b.usbtrdtim = 9u;
    }

    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GUSBCFG, gusbcfg.d32);

    /* Clear interrupt */
    gintsts.d32 = 0ul;
    gintsts.b.enumdone = 1u;
    USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTSTS, gintsts.d32 );
    return 1ul;
}


/**
 *******************************************************************************
 **\brief  DCD_IsoINIncomplete_ISR
 **        handle the ISO IN incomplete interrupt
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_IsoINIncomplete_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef gintsts;

    gintsts.d32 = 0ul;

    USBD_DCD_INT_fops->IsoINIncomplete (pdev);

    /* Clear interrupt */
    gintsts.b.incomplisoin = 1u;
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);

    return 1ul;
}

/**
 *******************************************************************************
 **\brief  DCD_IsoOUTIncomplete_ISR
 **        handle the ISO OUT incomplete interrupt
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_IsoOUTIncomplete_ISR(USB_OTG_CORE_HANDLE *pdev)
{
    USB_OTG_GINTSTS_TypeDef gintsts;

    gintsts.d32 = 0ul;

    USBD_DCD_INT_fops->IsoOUTIncomplete (pdev);

    /* Clear interrupt */
    gintsts.b.incomplisoout = 1u;
    USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);
    return 1ul;
}
/**
 *******************************************************************************
 **\brief  DCD_ReadDevInEP
 **        Reads ep flags
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
uint32_t DCD_ReadDevInEP (USB_OTG_CORE_HANDLE *pdev, uint8_t epnum)
{
    uint32_t v, msk, emp;
    msk = USB_OTG_READ_REG32(&pdev->regs.DREGS->DIEPMSK);
    emp = USB_OTG_READ_REG32(&pdev->regs.DREGS->DIEPEMPMSK);
    msk |= ((emp >> epnum) & 0x1ul) << 7u;
    v = USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT) & msk;
    return v;
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
