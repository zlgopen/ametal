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
 * \brief I2C驱动，服务I2C标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 
 * \endinternal
 */

/*******************************************************************************
  头文件
*******************************************************************************/
#include "ametal.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_hc32f460_i2c.h"
#include "hw/amhw_hc32f460_i2c.h"
#include "hc32f460_irq_handle.h"
#include "hc32f460_intctrl.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define __I2C_ST_IDLE             (0x10u)          /**< \brief 空闲状态 */
#define __I2C_ST_MSG_START        (0x11u)          /**< \brief 消息传输开始状态 */
#define __I2C_ST_TRANS_START      (0x12u)          /**< \brief 单个传输开始状态 */
#define __I2C_ST_SEND_SLA_ADDR    (0x13u)          /**< \brief 发送从机地址状态 */
#define __I2C_ST_M_SEND_DATA      (0x14u)          /**< \brief 发送数据状态 */
#define __I2C_ST_M_RECV_DATA      (0x15u)          /**< \brief 接收数据状态 */
#define __I2C_ST_ARBI_LOST        (0x16u)          /**< \brief 仲裁丢失状态         */
#define __I2C_ST_TIMEOUT          (0x17u)          /**< \brief 超时状态            */


#define __I2C_EVT_NONE            (0xFFu)          /**< \brief 无事件 */
#define __I2C_EVT_MSG_LAUNCH      (0xFEu)          /**< \brief 开始处理一个新的消息 */
#define __I2C_EVT_TRANS_LAUNCH    (0xFDu)          /**< \brief 开始处理一个新的传输 */

#define __I2C_EVT_MST_IDLE         AM_SBF(0x0, 1)  /**< \brief 主机空闲事件 */
#define __I2C_EVT_MST_RX           AM_SBF(0x1, 1)  /**< \brief 可以接收数据 */
#define __I2C_EVT_MST_TX           AM_SBF(0x2, 1)  /**< \brief 可以发送数据 */

#define __I2C_EVT_MST_ADDR_NO_ACK  AM_SBF(0x3, 1)  /**< \brief 地址无应答           */
#define __I2C_EVT_MST_DATA_NO_ACK  AM_SBF(0x4, 1)  /**< \brief 数据无应答           */
#define __I2C_EVT_MST_BERR         AM_SBF(0x5, 1)  /**< \brief 总线错误             */

#define __I2C_EVT_MST_TIMEOUT      AM_SBF(0x6, 1)  /**< \brief 超时错误             */

/*******************************************************************************
  函数声明
*******************************************************************************/

/** \brief I2C 硬件初始化 */
static int __i2c_hard_init (am_hc32f460_i2c_dev_t *p_dev);

/** \brief I2C 状态机函数 */
static int __i2c_mst_sm_event (am_hc32f460_i2c_dev_t *p_dev, uint32_t event);

/** \brief I2C 消息处理函数 */
static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_trans);

/** \brief 从控制器传输列表表头取出一条 message */
am_static_inline
struct am_i2c_message *__i2c_msg_out (am_hc32f460_i2c_dev_t *p_dev);

/*******************************************************************************
  全局变量
*******************************************************************************/

/**
 * \brief I2C 驱动函数定义
 */
static am_const struct am_i2c_drv_funcs __g_i2c_drv_funcs = {
    __i2c_msg_start
};


/* check if transfers empty */
#define __I2C_TRANS_EMPTY(p_dev) \
    ((p_dev)->p_cur_trans >= (p_dev)->p_cur_msg->p_transfers \
                             + (p_dev)->p_cur_msg->trans_num)

#if 0
static int __i2c_slv_timeout_set (am_hc32f460_i2c_dev_t *p_dev,
                                  uint32_t             timeout_ms)
{
    amhw_hc32f460_i2c_t *p_hw_i2c = NULL;    
    stc_clock_timeout_init_t stcTimoutInit;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_hc32f460_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    /* 不需要超时 */
    if (timeout_ms == 0) {    
        stcTimoutInit.enClkTimeOutSwitch = TimeoutFunOff;
        stcTimoutInit.u16TimeOutHigh = (0xffff);
        stcTimoutInit.u16TimeOutLow  = (0xffff);
        I2C_ClkTimeOutConfig(p_hw_i2c, &stcTimoutInit);
        
        amhw_hc32f460_i2c_intr_disable(p_hw_i2c, I2C_CR2_TMOURIE);        
        return AM_OK;
    }

    stcTimoutInit.enClkTimeOutSwitch = BothTimeOutOn;
    stcTimoutInit.u16TimeOutHigh = (0xffff);
    stcTimoutInit.u16TimeOutLow  = (0xffff);
    I2C_ClkTimeOutConfig(p_hw_i2c, &stcTimoutInit);
    
    amhw_hc32f460_i2c_intr_enable(p_hw_i2c, I2C_CR2_TMOURIE);        
    return AM_OK;
}
#endif

static uint8_t __hc32f460_master_start_or_restart(amhw_hc32f460_i2c_t *p_hw_i2c, uint8_t u8Start)
{
    uint32_t u32TimeOut = TIMEOUT;
    en_i2c_flag_status_t enFlagBusy = I2C_RESET;
    en_i2c_flag_status_t enFlagStartf = I2C_RESET;

    /* generate start or restart signal */
    if(!u8Start)
    {
        /* Wait I2C bus idle */
        while(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_BUSY))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }

        amhw_hc32f460_i2c_generate_start(p_hw_i2c , I2C_ENABLE);
    }
    else
    {
        /* Clear start status flag */
        amhw_hc32f460_i2c_clear_status(p_hw_i2c, I2C_CLR_STARTFCLR);
        /* Send restart condition */
        amhw_hc32f460_i2c_generate_restart(p_hw_i2c , I2C_ENABLE);
    }

    /* Judge if start success*/
    u32TimeOut = TIMEOUT;
    while(1)
    {
        enFlagBusy = amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_BUSY);
        enFlagStartf = amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_STARTF);
        if(enFlagBusy && enFlagStartf)
        {
            break;
        }
        if(0ul == (u32TimeOut--))
        {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

uint8_t __hc32f460_master_stop(amhw_hc32f460_i2c_t *p_hw_i2c)
{
    uint32_t u32TimeOut;

    /* Wait I2C bus busy */
    u32TimeOut = TIMEOUT;
    while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_BUSY))
    {
        if(0ul == (u32TimeOut--))
        {
            return AM_FALSE;
        }
    }

    amhw_hc32f460_i2c_generate_stop(p_hw_i2c, I2C_ENABLE);

    /* Wait STOPF */
    u32TimeOut = TIMEOUT;
    while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_STOPF))
    {
        if(0ul == (u32TimeOut--))
        {
            return AM_FALSE;
        }
    }
    return AM_TRUE;
}

static uint8_t __hc32f460_master_send_addr(amhw_hc32f460_i2c_t *p_hw_i2c, uint8_t u8Adr)
{
    uint32_t u32TimeOut = TIMEOUT;

    /* Wait tx buffer empty */
    while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_TEMPTYF))
    {
        if(0ul == (u32TimeOut--))
        {
            return AM_FALSE;
        }
    }

    /* Send I2C address */
    amhw_hc32f460_i2c_send_data(p_hw_i2c, u8Adr);

    if(!(u8Adr & 0x01u))     
    {
        /* If in master transfer process, Need wait transfer end*/
        uint32_t u32TimeOut = TIMEOUT;
        while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_TENDF))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }
    }

    /* Check ACK */
    u32TimeOut = TIMEOUT;
    while(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_NACKDETECTF))
    {
        if(0ul == (u32TimeOut--))
        {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

static uint8_t __hc32f460_master_read_data(amhw_hc32f460_i2c_t *p_hw_i2c,
                                           uint8_t             *pRxData,
                                           uint32_t             u32Size)
{
    uint32_t u32TimeOut = TIMEOUT;

    for(uint32_t i=0ul; i<u32Size; i++)
    {
        /* if the last byte receive, need config NACK*/
        if(i == (u32Size - 1ul))
        {
            amhw_hc32f460_i2c_nack_config(p_hw_i2c, I2C_ENABLE);
        }

        /* Wait receive full flag*/
        u32TimeOut = TIMEOUT;
        while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_RFULLF))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }

        /* read data from register*/
        *pRxData++ = amhw_hc32f460_i2c_dat_read(p_hw_i2c);
    }

    return AM_OK;
}

static uint8_t __hc32f460_master_write_data(amhw_hc32f460_i2c_t *p_hw_i2c,
                                            uint8_t             *pTxData,
                                            uint32_t             u32Size)
{
    uint32_t u32TimeOut = TIMEOUT;

    while(u32Size--)
    {
        /* Wait tx buffer empty */
        u32TimeOut = TIMEOUT;
        while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_TEMPTYF))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }

        /* Send one byte data */
        amhw_hc32f460_i2c_send_data(p_hw_i2c, *pTxData++);

        /* Wait transfer end*/
        u32TimeOut = TIMEOUT;
        while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_TENDF))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }

        /* Check ACK */
        u32TimeOut = TIMEOUT;
        while(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_NACKDETECTF))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }
    }

    return AM_OK;
}


/******************************************************************************/

/** \brief I2C 硬件初始化 */
am_local int __i2c_hard_init (am_hc32f460_i2c_dev_t *p_dev)
{
    amhw_hc32f460_i2c_t *p_hw_i2c = NULL;

    stc_i2c_init_t stcI2cInit;
    uint32_t pclk3Freq;


    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_hc32f460_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }
    
    amhw_hc32f460_i2c_deinit(p_hw_i2c);
    
    /* Get system clock frequency */
    pclk3Freq =  am_clk_rate_get(p_dev->p_devinfo->clk_num);
    
    stcI2cInit.enI2cMode = I2cMaster;
    stcI2cInit.u32Pclk3 = pclk3Freq;
    stcI2cInit.u32Baudrate = p_dev->p_devinfo->speed;
    stcI2cInit.u32SclTime = 0ul;
    amhw_hc32f460_i2c_init(p_hw_i2c, &stcI2cInit);

    amhw_hc32f460_i2c_enable(p_hw_i2c);

    return AM_OK;
}

/** \brief I2C 硬件重新初始化 */
am_local int __i2c_hard_re_init (am_hc32f460_i2c_dev_t *p_dev)
{
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }
    return __i2c_hard_init(p_dev);
}

/**
 * \brief 重新初始化 I2C
 *
 * \param[in] p_arg 指向I2C设备结构体的指针
 *
 * \return 无
 */
am_local void __i2c_re_init (void *p_arg)
{
    am_hc32f460_i2c_dev_t *p_dev = (am_hc32f460_i2c_dev_t *)p_arg;
    uint32_t             key   = 0;

    if (p_arg == NULL) {
        return;
    }

    key = am_int_cpu_lock();

    p_dev->is_abort = AM_TRUE;

    do {

        /* 回调消息完成函数 */
        if ((p_dev->p_cur_msg != NULL) &&
            (p_dev->p_cur_msg->pfn_complete != NULL)) {
            p_dev->p_cur_msg->status = -AM_ETIMEDOUT;
            p_dev->p_cur_msg->pfn_complete(p_dev->p_cur_msg->p_arg);
        }

        p_dev->p_cur_msg = __i2c_msg_out(p_dev);
        if (NULL == p_dev->p_cur_msg) {
            break;
        }
    } while (1);

    p_dev->p_cur_trans = NULL;
    p_dev->p_cur_msg   = NULL;
    p_dev->is_complete = AM_FALSE;
    p_dev->data_ptr    = 0;
    p_dev->trans_state = 0;
    p_dev->busy        = AM_FALSE;
    p_dev->state       = __I2C_ST_IDLE; /* 初始化为空闲状态 */
    p_dev->slave_addr  = 0;

    am_list_head_init(&(p_dev->msg_list));

    __i2c_hard_re_init(p_dev);

    if (p_dev->p_devinfo->pfn_bus_clean) {
        p_dev->p_devinfo->pfn_bus_clean();
    }
    am_int_cpu_unlock(key);
}

/**
 * \brief 使能软件定时器
 *
 * \param[in] p_arg 指向I2C设备结构体的指针
 *
 * \return 无
 */
am_local void __softimer_start (am_hc32f460_i2c_dev_t *p_dev)
{
    p_dev->is_abort = AM_FALSE;

    if (p_dev->p_devinfo->timeout_ms == 0) {
        return;
    }

    if (p_dev->softimer_state == AM_FALSE) {
        p_dev->softimer_state = AM_TRUE;
        am_softimer_start(&p_dev->softimer,
                           p_dev->p_devinfo->timeout_ms);
    } else {
        am_softimer_stop(&p_dev->softimer);
        am_softimer_start(&p_dev->softimer,
                           p_dev->p_devinfo->timeout_ms);
    }
}

/**
 * \brief 停止软件定时器
 *
 * \param[in] p_arg 指向I2C设备结构体的指针
 *
 * \return 无
 */
am_local void __softimer_stop (am_hc32f460_i2c_dev_t *p_dev)
{
    if (p_dev->p_devinfo->timeout_ms == 0) {
        return;
    }

    am_softimer_stop(&p_dev->softimer);

    p_dev->softimer_state = AM_FALSE;
}

/******************************************************************************/

/**
 * \brief 添加一条 message 到控制器传输列表末尾
 *
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
void __i2c_msg_in (am_hc32f460_i2c_dev_t *p_dev, struct am_i2c_message *p_msg)
{
    am_list_add_tail((struct am_list_head *)(&p_msg->ctlrdata),
                    &(p_dev->msg_list));
}

/**
 * \brief 从控制器传输列表表头取出一条 message
 *
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
struct am_i2c_message *__i2c_msg_out (am_hc32f460_i2c_dev_t *p_dev)
{
    if (am_list_empty_careful(&(p_dev->msg_list))) {
        return NULL;
    } else {
        struct am_list_head *p_node = p_dev->msg_list.next;
        am_list_del(p_node);
        return am_list_entry(p_node, struct am_i2c_message, ctlrdata);
    }
}

/******************************************************************************/

/**
 * \brief 超时软件定时器回调函数
 *
 * \param[in] p_arg 指向I2C设备结构体的指针
 *
 * \return 无
 */
am_local void __softimer_callback (void *p_arg)
{
    am_hc32f460_i2c_dev_t *p_dev = (am_hc32f460_i2c_dev_t *)p_arg;

    if (p_arg == NULL) {
        return;
    }

    __softimer_stop(p_dev);
    __i2c_re_init(p_arg);
    p_dev->busy = AM_FALSE;
}
/******************************************************************************/

static int __i2c_msg_start (void *p_drv, am_i2c_message_t *p_msg)
{
    am_hc32f460_i2c_dev_t *p_dev    = (am_hc32f460_i2c_dev_t *)p_drv;    

    int key;

    if ( (p_dev              == NULL) ||
         (p_msg              == NULL) ||
         (p_msg->p_transfers == NULL) ||
         (p_msg->trans_num   == 0)) {

        return -AM_EINVAL;
    }

    key = am_int_cpu_lock();

    /* 当前正在处理消息，只需要将新的消息加入链表即可 */
    if (p_dev->busy == AM_TRUE) {

        __i2c_msg_in(p_dev, p_msg);

        am_int_cpu_unlock(key);

        return AM_OK;

    } else {
        p_dev->busy = AM_TRUE;
        __i2c_msg_in(p_dev, p_msg);
        p_msg->status = -AM_EISCONN; /* 正在排队中 */
        am_int_cpu_unlock(key);
        
        return __i2c_mst_sm_event(p_dev, __I2C_EVT_MSG_LAUNCH);

    }
}

/**
 * \brief I2C初始化
 */
am_i2c_handle_t am_hc32f460_i2c_init (am_hc32f460_i2c_dev_t           *p_dev,
                                      const am_hc32f460_i2c_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->i2c_serv.p_funcs = (struct am_i2c_drv_funcs *)&__g_i2c_drv_funcs;
    p_dev->i2c_serv.p_drv   = p_dev;

    p_dev->p_devinfo = p_devinfo;

    p_dev->p_cur_trans = NULL;
    p_dev->p_cur_msg   = NULL;
    p_dev->is_complete = AM_FALSE;
    p_dev->data_ptr    = 0;
    p_dev->trans_state = 0;
    p_dev->busy        = AM_FALSE;
    p_dev->state       = __I2C_ST_IDLE; /* 初始化为空闲状态 */
    p_dev->slave_addr  = 0;

    if (p_devinfo->timeout_ms != 0) {
        p_dev->softimer_state = AM_FALSE;
        if (am_softimer_init(&p_dev->softimer,
                              __softimer_callback,
                              p_dev) != AM_OK) {
            return NULL;
        }
    }

    p_dev->is_abort = AM_FALSE;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    am_list_head_init(&(p_dev->msg_list));

    __i2c_hard_init(p_dev);

    if (p_dev->p_devinfo->pfn_bus_clean) {
        p_dev->p_devinfo->pfn_bus_clean();
    }
    
    return &(p_dev->i2c_serv);
}

/**
 * \brief I2C解初始化
 */
void am_hc32f460_i2c_deinit (am_i2c_handle_t handle)
{
    amhw_hc32f460_i2c_t   *p_hw_i2c = NULL;
    am_hc32f460_i2c_dev_t *p_dev    = NULL;

    if (NULL == handle) {
        return ;
    }

    p_dev    = (am_hc32f460_i2c_dev_t *)handle;
    p_hw_i2c = (amhw_hc32f460_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    p_dev->i2c_serv.p_funcs = NULL;
    p_dev->i2c_serv.p_drv   = NULL;

    amhw_hc32f460_i2c_disable (p_hw_i2c);
    
    __softimer_stop(p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/******************************************************************************/
/*  状态机内部状态切换 */
#define __I2C_NEXT_STATE(s, e) \
    do { \
        p_dev->state = (s); \
        new_event = (e); \
    } while(0)

/**
 * \brief I2C 状态机函数
 */
static int __i2c_mst_sm_event (am_hc32f460_i2c_dev_t *p_dev, uint32_t event)
{
    int                     key;
    amhw_hc32f460_i2c_t    *p_hw_i2c    = NULL;
    struct am_i2c_message  *p_cur_msg   = NULL;
    struct am_i2c_transfer *p_cur_trans = NULL;
    uint8_t *pRxData = NULL;    
    uint8_t *pTxData = NULL;
    uint8_t  ret = AM_FALSE;
    uint32_t u32Size = 0;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }
    
    key = am_int_cpu_lock();

    p_cur_msg        = __i2c_msg_out(p_dev);
    p_dev->p_cur_msg = p_cur_msg;

    if (p_cur_msg) {
        p_cur_msg->status = -AM_EINPROGRESS;
    } else {
        p_dev->busy = AM_FALSE;
    }
    
    p_dev->state = __I2C_ST_IDLE;

    am_int_cpu_unlock(key);

    p_dev->p_cur_trans  = p_cur_msg->p_transfers;
    p_cur_trans = p_dev->p_cur_trans;
    p_hw_i2c = (amhw_hc32f460_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    __softimer_start(p_dev);

    p_dev->data_ptr = 0;

    while(1) {
        if(p_cur_trans[p_cur_msg->done_num].flags & AM_I2C_M_RD) {  /* 读操作 */
            p_dev->state = __I2C_ST_MSG_START;
            __hc32f460_master_start_or_restart(p_hw_i2c, GENERATE_RESTART);
            __hc32f460_master_send_addr(p_hw_i2c, (uint8_t)(p_cur_trans->addr << 1u) | ADDRESS_R);
            
            /* 接收数据 */
            pRxData = p_cur_trans[p_cur_msg->done_num].p_buf;
            u32Size = p_cur_trans[p_cur_msg->done_num].nbytes;
            ret = __hc32f460_master_read_data(p_hw_i2c, pRxData, u32Size);
            if (AM_OK == ret) {
                p_dev->state = __I2C_ST_IDLE;
            }
                                   
        } else {    /* 写操作 */
            p_dev->state = __I2C_ST_MSG_START;
            if (0 == p_cur_msg->done_num) { 
                __hc32f460_master_start_or_restart(p_hw_i2c, GENERATE_START);
                __hc32f460_master_send_addr(p_hw_i2c, (uint8_t)(p_cur_trans->addr << 1u) | ADDRESS_W);
            }
            /* 传输目标地址 & 写数据 */            
            pTxData = p_cur_trans[p_cur_msg->done_num].p_buf;
            u32Size = p_cur_trans[p_cur_msg->done_num].nbytes;
            ret = __hc32f460_master_write_data(p_hw_i2c, pTxData, u32Size);           
            if (AM_OK == ret) {
                p_dev->state = __I2C_ST_IDLE;
            }
        }
        /* 开始下一个trans传输 */
        p_cur_msg->done_num++;

        if(p_cur_msg->done_num >= p_cur_msg->trans_num) {
            /* 设置停止停止标志 */
            __hc32f460_master_stop(p_hw_i2c);
            break;
        }
    }

    p_dev->busy = AM_FALSE;
    p_dev->state = __I2C_ST_IDLE;
    p_cur_msg->status = AM_OK;

    __softimer_stop(p_dev);

    /* 回调消息完成函数 */
    if (p_cur_msg->pfn_complete != NULL) {
        p_cur_msg->pfn_complete(p_dev->p_cur_msg->p_arg);
    }

    return AM_OK;
}

/* end of file */
