/*******************************************************************************
*                                AMetal
*                      ----------------------------
*                      innovating embedded platform
*
*Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
*All rights reserved.
*
*Contact information:
*web site:    http://www.zlg.cn/
*e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ADC drivers for standard interface 
 *
 * \internal
 * \par Modification History
 * - 1.02 18-04-24  pea, update standard interface
 * - 1.01 15-01-15  hbt, delete device name in amh_adc_dev
 * - 1.00 15-01-10  asd, first implementation
 * \endinternal
 */
 
#ifndef  __AM_LPC_ADC_INT_H
#define  __AM_LPC_ADC_INT_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"
#include "am_adc.h"
#include "hw/amhw_lpc_adc.h"

/**
 * \addtogroup am_lpc_if_adc_int
 * \copydoc am_lpc_adc_int.h
 * @{
 */


/**
 * \brief LPC5410x ADC device information.
 */
typedef struct am_lpc_adc_devinfo {
    
    /** \brief Pointer to the ADC register base. */
    uint32_t    adc_regbase;
    
    /** \brief  ADC clk id */
    uint32_t    clk_id;

    /** \brief ADC interrupt number array. */
    uint8_t    *intnums;

    /** \brief ADC interrupt number count. */
    uint8_t     intcnts;

    /** \brief ADC sample bits. */
    uint8_t     bits;

    /** 
     * \brief The ADC reference voltage, units: mV.
     *
     * For LPC5410x, ADC's Vref = (Vrefp - Vrefn), To operate the ADC within 
     * specifications at the maximum sampling rate, ensure that VREFP = VDDA.
     * The voltage level should typically be equal Vss and Vssa.
     * 
     * \note this decided by realistic circuits.
     *
     */
    uint32_t    vref;
    
    /** \brief ADC sample clock divide */
    uint32_t    clk_div;

    void (*pfn_plfm_init)(void);   /**< \brief plfm init  fuc  */

    void (*pfn_plfm_deinit)(void); /**< \brief plfm deinit fuc */

} am_lpc_adc_devinfo_t;



/**
 * \brief LPC5410x ADC device instance.
 */
typedef struct am_lpc_adc_dev {

    /** \brief ADC serve */
    am_adc_serv_t               adc_serve;

    /** \brief Pointer to the ADC device information */
    const am_lpc_adc_devinfo_t *p_devinfo;

    /** \brief acd seq complete callback */
    am_adc_seq_cb_t             pfn_callback;

    /** \brief the parm of acd seq complete callback */
    void                       *p_arg;

    /** \brief the head addr of current sed desc buf */
    am_adc_buf_desc_t          *p_desc;

    /** \brief the num of current sed desc buf */
    uint32_t                    desc_num;

    /** \brief adc transform count */
    uint32_t                    count;

    /** \brief adc transform flags */
    uint32_t                    flags;

    /** \brief the chan of adc transform */
    uint32_t                    chan;

    /** \brief desc buf index */
    uint32_t                    desc_pos;

    /** \brief the adc val buf index */
    uint32_t                    index;

    /** \brief the count of adc transform complete */
    uint32_t                    count_pos;

} am_lpc_adc_dev_t;

/**
 * \brief LPC5410x ADC device instance(dma).
 */
typedef struct am_lpc_adc_dma_dev {

    /** \brief ADC serve */
    am_adc_serv_t               adc_serve;

    /** \brief Pointer to the ADC device information */
    const am_lpc_adc_devinfo_t *p_devinfo;

    /** \brief acd seq complete callback  */
    am_adc_seq_cb_t             pfn_callback;

    /** \brief the parm of acd seq complete callback  */
    void                       *p_arg;

    /** \brief the head addr of current sed desc buf  */
    am_adc_buf_desc_t          *p_desc;

    /** \brief the num of current sed desc buf */
    uint32_t                    desc_num;

    /** \brief adc transform count */
    uint32_t                    count;

    /** \brief adc transform flags   */
    uint32_t                    flags;

    /** \brief the chan of adc transform */
    uint32_t                    chan;

    /** \brief desc buf index   */
    uint32_t                    desc_pos;

    /** \brief the adc val buf index */
    uint32_t                    index;

    /** \brief the count of adc transform complete */
    uint32_t                    count_pos;

} am_lpc_adc_dam_dev_t;

/**
 * \brief Initialize the ADC0. 
 *
 * Default use sequence-A.
 *
 * \param[in] p_dev     : Pointer to the ADC device.
 * \param[in] p_devinfo : Pointer to the ADC device information.
 *
 * \return The handle for standard ADC port. If NULL, indicate that Initialize failed.
 */
am_adc_handle_t am_lpc_adc_int_init(am_lpc_adc_dev_t           *p_dev,
                                    const am_lpc_adc_devinfo_t *p_devinfo);

/**
 * \brief De-initialize the ADC0. 
 *
 * Default use sequence-A.
 *
 * \param[in] handle : The handle for standard ADC
 *
 * \return None.
 */
void am_lpc_adc_int_deinit(am_adc_handle_t handle);
                              
/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /*__AM_LPC_ADC_INT_H  */

/* end of file */
