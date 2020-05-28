
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_board.h"
#include "am_gpio.h"
#include "am_vdebug.h"
#include "am_clk.h"
#include "demo_hc32f460_entries.h"
#include "am_hc32f460_inst_init.h"



void demo_hc32f460_core_drv_qspi_entry (void)
{

    AM_DBG_INFO("demo hc32f460_core drv qspi !\r\n");

    demo_hc32f460_drv_qspi_entry((am_hc32f460_qspi_dev_t *)am_hc32f460_qspi_inst_init());
}

