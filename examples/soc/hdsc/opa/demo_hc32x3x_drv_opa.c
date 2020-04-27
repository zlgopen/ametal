/**
 * \file
 * \brief OPA不同模式下输入输出关系例程，通过 std 层接口实现
 *
 * - 实验现象：
 *
 *    OPA_UintMode_Test
 *    此时通过示波器观察PC06和PC07信号，PC07输出信号是PC06信号通信幅值是一致的。
 *    OPA_ForWardMode_Test
 *    此时通过示波器观察PC06和PC07信号，PC07输出信号是PC06信号通信幅值是其两倍。
 *    OPA_GpMode_Test
 *    PC06输入VCOR1.5V，PB15和PC07接电阻22K，PB15接电阻10K对地，
 *    此时通过示波器观察PC06和PC07信号，PC07输出信号是PC06信号通信幅值是其两倍。
 *
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 * * \par 源代码
 * \snippet demo_hc32_std_opa.c src_hc32_std_opa
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-17  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32_std_opa
 * \copydoc demo_hc32_std_opa.c
 */

/** [src_hc32_std_opa] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_board.h"
#include "am_hc32x3x_opa.h"

/**
 * \brief OPA例程，通过异步标准接口实现
 */
void demo_hc32x3x_drv_opa_entry (void *p_handle, uint8_t ch)
{

   if (p_handle ==NULL){
       return ;
   }

    /* 设置模式   需要底层配置IO端口与内部连接为AWHW_HC32_OPA_PO_EN */
    am_hc32_opa_mode_set(p_handle, ch);

    /* 使能 */
    am_hc32_opa_enable(p_handle);

    while(1){
      ;
    }
}

/* end of file */
