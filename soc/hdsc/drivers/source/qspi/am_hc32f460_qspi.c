/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief QSPI驱动
 *
 * \internal
 * \par Modification History
 * - 1.00
 * \endinternal
 */

/*******************************************************************************
  头文件
*******************************************************************************/

#include "am_common.h"
#include "am_types.h"
#include "am_hc32f460_qspi.h"

/**
 * \brief qspi读函数
 */
int am_hc32f460_qspi_read (am_hc32f460_qspi_dev_t *p_dev,
                           uint32_t                addr,
                           uint8_t                *p_buf,
                           uint32_t                nbytes)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    uint32_t i =0;
    uint8_t         *p_temp = p_buf;

    if ((addr >= 0x98000000) && (addr < 0x9FFFFFFF)) {
        for(i = 0; i < nbytes; i++) {
            p_temp[i] = *(uint8_t *)(addr + i);
        }
    } else {
        return -AM_EINVAL;
    }

    return AM_OK;
}


/**
 * \brief 进入直接通信模式
 */
int am_hc32f460_qspi_enter_directcom (am_hc32f460_qspi_dev_t *p_dev)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    amhw_hc32f460_qspi_t *p_hw_qspi =
         (amhw_hc32f460_qspi_t *)p_dev->p_devinfo->regbase;

    amhw_hc32f460_qspi_dcome_set(p_hw_qspi, AMHW_HC32F460_QSPI_DIRECT_ACCESS);

    return AM_OK;
}

/**
 * \brief 退出直接通信模式
 */
int am_hc32f460_qspi_exit_directcom (am_hc32f460_qspi_dev_t *p_dev)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    amhw_hc32f460_qspi_t *p_hw_qspi =
         (amhw_hc32f460_qspi_t *)p_dev->p_devinfo->regbase;

    amhw_hc32f460_qspi_dcome_set(p_hw_qspi, AMHW_HC32F460_QSPI_ROM_ACCESS);

    return AM_OK;
}

/**
 * \brief 直接读数据
 */
uint8_t am_hc32f460_qspi_directcom_read (am_hc32f460_qspi_dev_t *p_dev)
{
    amhw_hc32f460_qspi_t *p_hw_qspi =
         (amhw_hc32f460_qspi_t *)p_dev->p_devinfo->regbase;

    return amhw_hc32f460_qspi_directcom_read(p_hw_qspi);
}

/**
 * \brief 直接写数据
 */
int am_hc32f460_qspi_directcom_write (am_hc32f460_qspi_dev_t *p_dev, uint8_t data)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    amhw_hc32f460_qspi_t *p_hw_qspi =
         (amhw_hc32f460_qspi_t *)p_dev->p_devinfo->regbase;

    amhw_hc32f460_qspi_directcom_write(p_hw_qspi, data);

    return AM_OK;
}

/**
 * \brief qspi初始化函数
 */
am_qspi_handle_t am_hc32f460_qspi_init (am_hc32f460_qspi_dev_t            *p_dev,
                                        const am_hc32f460_qspi_devinfo_t  *p_devinfo)
{
    amhw_hc32f460_qspi_t *p_hw_qspi;

    if (p_devinfo == NULL) {
        return NULL;
    }

    /* 获取配置参数 */
    p_hw_qspi = (amhw_hc32f460_qspi_t *)p_devinfo->regbase;
    p_dev->p_devinfo = p_devinfo;


    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    amhw_hc32f460_qspi_clk_div_set(p_hw_qspi, p_devinfo->clk_div);
    amhw_hc32f460_qspi_mode_set(p_hw_qspi, p_devinfo->spi_mode);
    amhw_hc32f460_qspi_pfe_set(p_hw_qspi, AM_FALSE);
    amhw_hc32f460_qspi_pfsae_set(p_hw_qspi, AM_FALSE);
    amhw_hc32f460_qspi_readmode_set(p_hw_qspi,p_devinfo->read_mode);
    amhw_hc32f460_qspi_dprsl_set(p_hw_qspi, p_devinfo->protocol_data);
    amhw_hc32f460_qspi_aprsl_set(p_hw_qspi, p_devinfo->protocol_addr);
    amhw_hc32f460_qspi_iprsl_set(p_hw_qspi, p_devinfo->protocol_cmd);

    amhw_hc32f460_qspi_cscr_ssnw_set(p_hw_qspi, AMHW_HC32F460_QSPI_QSSN_VALID_EXTEND_NOT);
    amhw_hc32f460_qspi_cscr_sshw_set(p_hw_qspi, AMHW_HC32F460_QSPI_QSSN_INTERVAL_QSCK8);

    amhw_hc32f460_qspi_qsfcr_duty_enable(p_hw_qspi, AMHW_HC32F460_QSPI_QSCK_DUTY_CORR_NOT);


    amhw_hc32f460_qspi_qsfcr_virtual_cycle_set(p_hw_qspi, AMHW_QSPI_VIRTUAL_PRTIOD_QSCK8);

    amhw_hc32f460_qspi_qsfcr_wp_pinlevel_set(p_hw_qspi, AMHW_HC32F460_QSPI_WP_PINOUTPUT_LOW);
    amhw_hc32f460_qspi_qsfcr_ssnld_set(p_hw_qspi, AMHW_HC32F460_QSPI_QSSN_SETUP_DELAY_HALFQSCK);
    amhw_hc32f460_qspi_qsfcr_ssnhd_set(p_hw_qspi, AMHW_HC32F460_QSPI_QSSN_HOLD_DELAY_HALFQSCK);
    amhw_hc32f460_qspi_qsfcr_4byte_addr_set(p_hw_qspi, AM_FALSE);
    amhw_hc32f460_qspi_qsfcr_addr_width_set(p_hw_qspi, AMHW_HC32F460_QSPI_ADDRESS_BYTE_THREE);
    amhw_hc32f460_qspi_dcome_set(p_hw_qspi, AMHW_HC32F460_QSPI_ROM_ACCESS);

    amhw_hc32f460_qspi_romaccess_cmd_set(p_hw_qspi, AMHW_HC32F460_QSPI_3BINSTR_STANDARD_READ);


    return p_dev;
}


/**
 * \brief QSPI 去初始化
 */
void am_hc32f460_qspi_deinit (am_qspi_handle_t handle)
{
    am_hc32f460_qspi_dev_t *p_dev  = (am_hc32f460_qspi_dev_t *)handle;

    if (handle == NULL){
        return ;
    }

    /* 平台去初始化 */
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

}

/* end of file */
