void IRQ128_Handler(void *parg);
void IRQ129_Handler(void *parg);
void IRQ130_Handler(void *parg);
void IRQ131_Handler(void *parg);
void IRQ132_Handler(void *parg);
void IRQ136_Handler(void *parg);
void IRQ137_Handler(void *parg);
void IRQ138_Handler(void *parg);
void IRQ139_Handler(void *parg);
void IRQ141_Handler(void *parg);
void IRQ142_Handler(void *parg);
void IRQ143_Handler(void *parg);

typedef struct
{
    volatile uint32_t INTSEL                : 9;
    uint32_t RESERVED9                      :23;
} stc_intc_sel_field_t;

/**
 * \brief 自由中断注册函数
 *
 * \param[in] inum :    中断向量编号
 * \param[in] src_num : 中断源编号
 *
 * \return none
 */
am_static_inline
void amhw_hc32f460_int_sel0_31(uint8_t inum, uint16_t src_num)
{
    stc_intc_sel_field_t *stcIntSel;
    stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * inum));
    stcIntSel->INTSEL = src_num;
}
