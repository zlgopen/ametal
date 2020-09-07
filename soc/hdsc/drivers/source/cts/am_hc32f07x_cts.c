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
 * \brief CTS驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-30
 * \endinternal
 */
 
#include "am_hc32f07x_cts.h"
#include "ametal.h"
#include "hc32f07x_periph_map.h"
#include "hc32f07x_inum.h"
#include "am_int.h"
#include "hc32f07x_clk.h"
#include "am_vdebug.h"

/* 中断服务函数 */
am_local void __cts_irq_handler(void *parg)
{
    amhw_hc32f07x_cts_t *p_hw_cts = HC32_CTS;	
			
    if(amhw_hc32f07x_cts_flag_get (p_hw_cts, AMHW_HC32F07X_CTS_FLAG_ERRF)){
        *(int *)parg = 2;
    }
		
    if(amhw_hc32f07x_cts_flag_get (p_hw_cts, AMHW_HC32F07X_CTS_FLAG_OKF)){
        *(int *)parg = 1;
    }
		
    amhw_hc32f07x_cts_flag_clr (p_hw_cts,
                                AMHW_HC32F07X_CTS_CLR_CAPC |
                                AMHW_HC32F07X_CTS_CLR_UDFC |
                                AMHW_HC32F07X_CTS_CLR_ERRC |
                                AMHW_HC32F07X_CTS_CLR_WARNC |
                                AMHW_HC32F07X_CTS_CLR_OKC  );
}

/* 时钟校准 */
uint8_t am_hc32f07x_cts_calibrate( uint32_t                    fclk,
	                                 uint32_t                    fref,
	                                 amhw_hc32f07x_cts_clksrc_t clksrc,
	                                 amhw_hc32f07x_cts_refsrc_t refsrc)
{
    int ret = AM_ERROR;
    int break_flag = 0;
    int arr = 0;
    int felim = 0;
    int prs = 0;	
    amhw_hc32f07x_cts_t *p_hw_cts = HC32_CTS;
	
    if(fref > (1000 * 256)) {
        prs = 7;
        fref /= 256;
    } else if(fref > (1000 * 64)) {
        prs = 6;
        fref /= 64; 
    } else if(fref > (1000 * 32)) {
        prs = 5;
        fref /= 32; 
    } else if(fref > (1000 * 16)) {
        prs = 4;
        fref /= 16; 
    } else if(fref > (1000 * 8)) {
        prs = 3;
        fref /= 8; 
    } else if(fref > (1000 * 4)) {
        prs = 2;
        fref /= 1; 
    } else if(fref > (1000 * 2)) {
        prs = 1;
        fref /= 2; 
    }

    arr = fclk/fref-1;
    felim = (fclk/fref)*2/100/2;

    amhw_hc32f07x_cts_clksrc_set (p_hw_cts, clksrc);
    amhw_hc32f07x_cts_refsrc_set (p_hw_cts, refsrc);
    amhw_hc32f07x_cts_prs_set (p_hw_cts,
                               (amhw_hc32f07x_cts_prs_t)(AMHW_HC32F07X_CTS_PRS_0DIV + prs));
    amhw_hc32f07x_cts_felim_set (p_hw_cts, felim);
    amhw_hc32f07x_cts_arr_set (p_hw_cts,arr);
//		AM_DBG_INFO("felim: %d\r\n",felim);
//		AM_DBG_INFO("arr: %d\r\n",arr);
//		AM_DBG_INFO("prs: %d\r\n",prs);

    amhw_hc32f07x_cts_trim_set (p_hw_cts, 0X00);
    amhw_hc32f07x_cts_flag_clr (p_hw_cts,
                                AMHW_HC32F07X_CTS_CLR_CAPC |
                                AMHW_HC32F07X_CTS_CLR_UDFC |
                                AMHW_HC32F07X_CTS_CLR_ERRC |
                                AMHW_HC32F07X_CTS_CLR_WARNC |
                                AMHW_HC32F07X_CTS_CLR_OKC  );
																
    /* 连接中断 */
    am_int_connect(INUM_CLKTRIM_CTS,
                   __cts_irq_handler,
                  (void *)&break_flag);
	
    /* 使能中断 */
    amhw_hc32f07x_cts_int_enable (p_hw_cts,
	                                AMHW_HC32F07X_CTS_INT_ERRIE );
    amhw_hc32f07x_cts_int_enable (p_hw_cts,
                                  AMHW_HC32F07X_CTS_INT_WARNIE  );
    amhw_hc32f07x_cts_int_enable (p_hw_cts,
                                  AMHW_HC32F07X_CTS_INT_OKIE  );													
    am_int_enable(INUM_CLKTRIM_CTS);	
		
		amhw_hc32f07x_cts_auto_trim_enable (p_hw_cts);
    amhw_hc32f07x_cts_cen_enable (p_hw_cts);
				
	  while(0 == break_flag){
        ;
    }
    amhw_hc32f07x_cts_int_disable (p_hw_cts,
                                   AMHW_HC32F07X_CTS_INT_ERRIE );
    amhw_hc32f07x_cts_int_disable (p_hw_cts,
	                                AMHW_HC32F07X_CTS_INT_WARNIE  );
    amhw_hc32f07x_cts_int_disable (p_hw_cts,
	                                AMHW_HC32F07X_CTS_INT_OKIE  );
    am_int_disable(INUM_CLKTRIM_CTS);
		
		if(1 == break_flag){
        ret = AM_OK;
		}
		
		return ret;
}


/* end of file */
