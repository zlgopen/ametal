void IRQ128_Handler(void *parg);
void IRQ129_Handler(void *parg);
void IRQ136_Handler(void *parg);
void IRQ137_Handler(void *parg);
void IRQ141_Handler(void *parg);

typedef struct
{
    volatile uint32_t INTSEL                : 9;
    uint32_t RESERVED9                      :23;
} stc_intc_sel_field_t;
