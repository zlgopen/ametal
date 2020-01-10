#include "ametal.h"
#include "am_board.h"
#include "am_clk.h"
#include "hc32f460_clk.h"
#include "am_hc32f460_lpuart.h"

/* USART channel definition */
#define USART_CH                        (M4_USART3)

#define USART_TX_PORT                   (PortE)
#define USART_TX_PIN                    (Pin05)
#define USART_TX_FUNC                   (Func_Usart3_Tx)

/* USART baudrate definition */
#define USART_BAUDRATE                  (115200ul)

void uart_init(void) {
    en_result_t enRet = Ok;

    uint32_t u32Fcg1Periph = PWC_FCG1_PERIPH_USART1 | PWC_FCG1_PERIPH_USART2 | \
                                 PWC_FCG1_PERIPH_USART3 | PWC_FCG1_PERIPH_USART4;
    am_clk_enable(CLK_USART3);
    uint32_t clk_freq = am_clk_rate_get(CLK_USART3);

    const stc_usart_uart_init_t stcInitCfg = {
        UsartIntClkCkNoOutput,
        UsartClkDiv_1,
        UsartDataBits8,
        UsartDataLsbFirst,
        UsartOneStopBit,
        UsartParityNone,
        UsartSamleBit8,
        UsartStartBitFallEdge,
        UsartRtsEnable,
    };


	enRet = USART_UART_Init(USART_CH, &stcInitCfg);
	PORT_SetFunc(USART_TX_PORT, USART_TX_PIN, USART_TX_FUNC, Disable);

	/* Set baudrate */
	enRet = USART_SetBaudrate(USART_CH, USART_BAUDRATE);

    if (enRet != Ok)
    {
        while (1)
        {
        }
    }

    /*Enable RX && TX function*/
    USART_FuncCmd(USART_CH, UsartRx, Enable);
    USART_FuncCmd(USART_CH, UsartTx, Enable);

}

void debug_test(){
	uint16_t u16RxData;
	u16RxData = 0x12;

	uart_init();

	while(1) {
		while (Reset == USART_GetStatus(USART_CH, UsartTxEmpty))  /* Warit Tx data register empty */
	        {
	        }

	        USART_SendData(USART_CH, u16RxData);
	}
}

void __chars_printf(char* str)
{
	uint32_t i = 0;

    for ( i = 0; str[i] != '\0' ;i++) {
		while (Reset == USART_GetStatus(USART_CH, UsartTxEmpty))  /* Warit Tx data register empty */
	        {
	        }

	   USART_SendData(USART_CH, str[i]);
    }
}

void __clk_freq_printf(am_clk_id_t clk_id) {

	uint32_t clk_freq = am_clk_rate_get(clk_id);
    uint32_t bytes = 0;
    uint8_t data[9] = {0};
    uint32_t i = 0;
    uint32_t freq_temp = clk_freq;

    while (freq_temp > 0) {
    	freq_temp = freq_temp / 10;
    	bytes++;
    }

    freq_temp = clk_freq;


    for ( i = 0; freq_temp > 0; i++) {
    	data[i] = freq_temp % 10;
    	freq_temp /= 10;
    }

    for ( i = 0; i < bytes ;i++) {
		while (Reset == USART_GetStatus(USART_CH, UsartTxEmpty))  /* Warit Tx data register empty */
	        {
	        }

	   USART_SendData(USART_CH, data[bytes - 1 - i] + '0');
    }

    while (Reset == USART_GetStatus(USART_CH, UsartTxEmpty)){}
    __chars_printf("\r\n");
}




void clk_printf(char *str, am_clk_id_t clk_id)
{
	__chars_printf(str);
	while (Reset == USART_GetStatus(USART_CH, UsartTxEmpty)){}
    USART_SendData(USART_CH, ':');
    while (Reset == USART_GetStatus(USART_CH, UsartTxEmpty)){}
    USART_SendData(USART_CH, ' ');
	__clk_freq_printf(clk_id);
}
