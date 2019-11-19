#include "math.h"
#include "ametal.h"
#include "string.h"
#include "am_delay.h"
#include "am_input.h"
#include "am_buzzer.h"
#include "am_zlg_flash.h"
#include "am_input_code.h"
#include "am_zml166_adc.h"
#include "am_thermocouple.h"
#include "zlg116_periph_map.h"
#include "am_aml166_inst_init.h"
#include "am_hwconf_zml166_adc.h"
#include "am_pt100_to_temperature.h"
#include "demo_zlg_entries.h"
const static uint8_t pga[8] = {1, 2, 4, 8, 16, 32, 64, 128};
/*
 * \brief  取出字符串中浮点数
 */
static float __atof(char *str_a)
{
    int     sum_int  = 0.0;
    float   temp     = 1.0;
    float   sum_f    = 0.0;
    uint8_t flag     = 0;
    uint8_t flag_neg = 0;

    if (*str_a == '-') {
        flag_neg = 1;
        str_a++;
    }
    for (; *str_a != '\n'; ) {

        if ('.' == *str_a) {
            flag = 1;
            str_a++;
            continue;
        }
        if(0 == flag && *str_a >= '0' && *str_a <= '9'){
            sum_int = sum_int * 10 + (*str_a - '0');
        } else if( *str_a >= '0' && *str_a <= '9'){
            temp /= 10.0;
            sum_f = sum_f  + (*str_a - '0') * temp;
        }
        str_a++;
    }
    sum_f += sum_int;
    sum_f += 0.0005;
    if (flag_neg == 1) {
        return -sum_f;
    } else {
        return sum_f;
    }
}
/**
 * \brief 公式打印函数
 */
static void __print_adjust_function(uint8_t pag_index,float para_1, float para_2)
{
    if(pag_index == 10){
        am_kprintf("The PT100 formula is : \r\n");
    }else if(pag_index == 11){
        am_kprintf("The Thermocouple formula is : \r\n ");
    }else {
        am_kprintf("The PGA %d formula is : \r\n ", pga[pag_index]);
    }
    if(para_2 > 0){
        am_kprintf("Y = %1d.%07d * X + %1d.%04d\r\n\r\n",
                   ((int32_t)(para_1 * 10000000) / 10000000),
                   ((int32_t)(para_1 * 10000000)) % 10000000,
                   ((int32_t)(para_2 * (10000)) / 10000),
                   ((int32_t)(para_2 * 10000)) % 10000);
    }else{
        am_kprintf("Y = %1d.%07d * X - %1d.%04d\r\n\r\n",
                   ((int32_t)((para_1 * 10000000) / 10000000)),
                   ((int32_t)(para_1 * 10000000)) % 10000000,
                   ((int32_t)(para_2 * (-10000)) / 10000),
                   ((int32_t)(para_2 * (-10000))) % 10000);
    }
}

/**
 * \brief 字符串比较
 */
static int __str_cmp(char *p_str1, char *p_str2, int count)
{
    int i = 0;
    while(*p_str1 != '\0' && *p_str2 != '\0'){
        if(*p_str1 == *p_str2){
            p_str1 ++;
            p_str2 ++;
            i ++;
            if(i == count){
                return AM_OK;
            }
            continue;
        }else{
            return 1;
        }
    }
    return 1;
}

/**
 * \brief 串口接收以'\n'为帧尾的数据
 */
void uart_str_get(am_uart_handle_t handle, char *buffer)
{
    int   uart_data_count = 0;
    while(1){
        if(am_uart_poll_getchar(handle, &buffer[uart_data_count]) == AM_OK){
            uart_data_count ++;
            if(buffer[uart_data_count - 1] == '\n'){
                break;
            }
        }
    }
}

float am_zml166_adc_thermistor_res_data_get(void *p_handle);
/**
 * \brief 测试AML166板级外接热电阻的阻值以及对应的转换温度
 */
void demo_aml166_thermistor_measure_entry(am_uart_handle_t uart_handle)
{
    volatile float  r_data = 0, temperature = 0;
    volatile float pt100_para[2] = {1 , 0};
    am_zml166_adc_handle_t  handle = am_zml166_adc_inst_init();

    am_zlg_flash_init(ZLG116_FLASH);
    memcpy((void *)pt100_para, (uint32_t *)PT100_PARA_SAVE_ADDRESS, 4 * 2);

    /* 若flash中无保存系数 */
    if(!((pt100_para[0] < 1.1) && (pt100_para[0] > 0.9))) {
        pt100_para[0] = 1;
    }
    if(!((pt100_para[1] < 0.15) &&(pt100_para[1] > -0.15))){
        pt100_para[1] = 0;
    }

    while(1){
        char str;
        /* 设置PT100增益倍数 */
        am_zml166_adc_gain_set(handle, 1);

        r_data = am_zml166_adc_thermistor_res_data_get((void *)handle);

        /* 电阻校准系数  */
        r_data = pt100_para[0]*r_data + pt100_para[1];

//        am_kprintf("r_data = %d\r\n", (int) r_data);

        /* PT100电阻转温度  */
        temperature   = pt100_to_temperature(r_data);

        if(temperature < 0){
            temperature *= -1;
            am_kprintf("Tem = -%d.%03d°\r\n",
               ((int32_t)(temperature * 1000) / 1000) ,
               ((int32_t)(temperature * 1000) % 1000));
        }else{
            am_kprintf("Tem = %d.%03d°\r\n\r\n",
               ((int32_t)(temperature * 1000) / 1000) ,
               ((int32_t)(temperature * 1000) % 1000));
        }

        if(am_uart_poll_getchar(uart_handle, &str) == AM_OK){
            if(str == 'e'){
                break;
            }
        }
    }
}

void demo_aml166_thermocouple_measrue_entry(am_uart_handle_t uart_handle)
{

    char str;
    char  uart_data[20] ;
    int32_t  adc_val[1];
    int8_t   type = -1;
    float    para[16];
    double   temp;
    am_ther_conversion_t    ther_dev;
    am_ther_formula_t       ther_handle;
    am_zml166_adc_handle_t  handle = am_zml166_adc_inst_init();
    am_adc_handle_t         adc_handle  = &handle->adc_serve;

    /* 热电偶计算以及电压校准参数初始化 */
    am_zlg_flash_init(ZLG116_FLASH);
    memcpy((void *)para, (uint32_t *)((FLASH_BLOCK_NUM * 1024)), 4 * 16);

    if(!((para[0] < 1.1) && (para[0] > 0.9))) {
        para[0] = 1;
    }
    if(!((para[1] < 0.15) &&(para[1] > -0.15))){
        para[1] = 0;
    }
    if(!((para[8] < 1.1) && (para[8] > 0.9))) {
        para[8] = 1;
    }
    if(!((para[9] < 0.15) &&(para[9] > -0.15))){
        para[9] = 0;
    }

    while(type < 0 || type > 3){
        am_kprintf("Please select thermocouple type. eg: ‘1\\n’ \r\n");
        am_kprintf("1. J   \r\n");
        am_kprintf("2. K   \r\n");
        am_kprintf("3. T   \r\n");
        uart_str_get(uart_handle, uart_data);
        type = uart_data[0] - '1';
    }
    if(type == 0){
        ther_handle = am_thermocouple_j_init(&ther_dev);
    }else if(type == 1){
        ther_handle = am_thermocouple_k_init(&ther_dev);
    }else{
        ther_handle = am_thermocouple_t_init(&ther_dev);
    }

    while(1){
        double temperature, v1;
        /* 测量冷端温度 */
        am_zml166_adc_gain_set(handle, 1);
        am_zml166_adc_reg_set(handle,
                              AM_ZML166_ADC_ADC3_ADDR,
                              AM_ZML166_ADC_LVSHIFT_ENABLE|AM_ZML166_ADC_LVSCP_ENABLE);
        am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN2 | AM_ZML166_ADC_INNS_GND);
        am_adc_read(adc_handle, 0, (uint32_t *)adc_val, AM_NELEMENTS(adc_val));

        temp  = (double)((double)adc_val[0] / 8388607 * 2500 );
        temp  = temp * para[0] + para[1];
        temp = (double)((temp * 8388607 / 2500 ) / (double)((1<<23)-adc_val[0]));
        temperature = 1 / (log(temp)/3435 + 1 / 298.15)-273.15;

        am_ther_tem_to_vol(ther_handle, temperature, &v1);

        am_zml166_adc_reg_set(handle,
                              AM_ZML166_ADC_ADC3_ADDR,
                              AM_ZML166_ADC_LVSHIFT_DISABLE|AM_ZML166_ADC_LVSCP_ENABLE);
        am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN3 | AM_ZML166_ADC_INNS_AIN4);

        am_zml166_adc_gain_set(handle, 16);
        am_adc_read(adc_handle, 0, (uint32_t *)adc_val, AM_NELEMENTS(adc_val));
        temp  = (double)((double)adc_val[0] / 16.0 * 2500 / 8388607  );
        temp  = temp * para[8] + para[9];

        if(am_ther_vol_to_tem(ther_handle, temp + v1, &temperature) == AM_OK){

            temperature *= 1000;

            if(temperature < 0){
                temperature *= -1;
                am_kprintf("Tem = -%d.%03d°\r\n", (uint32_t)temperature/1000 , (uint32_t)temperature%1000);
            }else{
                am_kprintf("Tem = %d.%03d°\r\n\r\n", (uint32_t)temperature/1000 , (uint32_t)temperature%1000);
            }

        }else{
            am_kprintf("Thermocouple  error.....\r\n");
        }
        am_mdelay(300);
        if(am_uart_poll_getchar(uart_handle, &str) == AM_OK){
            if(str == 'e'){
                break;
            }
        }
    }
}

/*
 * \brief 按键回调函数
 */
static void __input_key_proc (void *p_arg, int key_code, int key_state, int keep_time)
{
    if (key_code == KEY_KP0) {
        if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
            am_kprintf("Key pressed.\r\n\r\n");
            am_buzzer_beep(500);
        }else if(key_state == AM_INPUT_KEY_STATE_RELEASED){
            am_kprintf("Key released.\r\n\r\n");
        }
    }
}


/*
 * \brief 按键蜂鸣器演示例程
 */
static void demo_key_buzzer_entry(am_uart_handle_t uart_handle)
{
    static am_input_key_handler_t key_handler;

    am_kprintf("Key and Buzzer test start.\n");

    am_input_key_handler_register(&key_handler, __input_key_proc, NULL);

    while(1){
        char str;
        if(am_uart_poll_getchar(uart_handle, &str) == AM_OK){
            if(str == 'e'){
                break;
            }
        }
        am_mdelay(10);
    }
}

/**
 * \brief 测试ZML166 ADC 校准后电压的准确度
 */
void demo_aml166_vol_measure_entry(am_uart_handle_t uart_handle,
                                   int              flag)
{	
    char    uart_data[20] ;
    uint8_t pga_index = 0, i = 0;
    float   para[16];
    am_zml166_adc_handle_t  handle = am_zml166_adc_inst_init();

    am_zlg_flash_init(ZLG116_FLASH);
    memcpy((void *)para, (uint32_t *)((FLASH_BLOCK_NUM * 1024)), 4 * 16);

    for(i = 0; i < 8; i ++){
        if(!((para[i * 2] < 1.1) && (para[i * 2] > 0.9))) {
            para[i * 2] = 1;
        }
        if(!((para[i * 2 + 1] < 0.15) &&(para[i * 2 + 1] > -0.15))){
            para[i * 2 + 1] = 0;
        }
    }

    am_kprintf("Please chose your option. eg: ‘1\\n’ \n");
    am_kprintf("1. GAIN = 1  \r\n");
    am_kprintf("2. GAIN = 2  \r\n");
    am_kprintf("3. GAIN = 4  \r\n");
    am_kprintf("4. GAIN = 8  \r\n");
    am_kprintf("5. GAIN = 16  \r\n");
    am_kprintf("6. GAIN = 32  \r\n");
    am_kprintf("7. GAIN = 64  \r\n");

    uart_str_get(uart_handle, uart_data);
    pga_index = uart_data[0] - '1';
    am_zml166_adc_gain_set(handle, pga[pga_index]);

    if(flag == 1) {
        am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN0 | AM_ZML166_ADC_INNS_AIN1);
    }else if(flag == 2){
        am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN2 | AM_ZML166_ADC_INNS_AIN1);
    }else if(flag == 3){
        am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN3 | AM_ZML166_ADC_INNS_AIN2);
    }else if(flag == 4){
        am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN3 | AM_ZML166_ADC_INNS_AIN4);
    }if(flag == 5){
        am_zml166_adc_mux_set(handle, AM_ZML166_ADC_INPS_AIN2 | AM_ZML166_ADC_INNS_AIN2);
    }

    while(1){
        int32_t adc_val;
        char  str;
        volatile double vol;
        am_adc_read(&handle->adc_serve, 0, (void *)&adc_val, 1);
        vol  = (double)((double)(adc_val/ pga[pga_index] / 8388607.0) * handle->p_devinfo->vref);
        vol  = para[pga_index * 2] * vol + para[pga_index * 2 + 1];
        vol *= 10000;
        if(vol > 0){
            am_kprintf("%3d.%04dmV\r\n", (int32_t)vol/10000, (int32_t)vol%10000);
        }else {
            vol *= -1;
            am_kprintf("-%3d.%04dmV\r\n", (int32_t)vol/10000, (int32_t)vol%10000);
        }
        am_mdelay(300);
        if(am_uart_poll_getchar(uart_handle, &str) == AM_OK){
            if(str == 'e'){
                break;
            }
            if(str >= '1'  && str <= '7') {
                pga_index = str - '1';
                am_kprintf("GAIN =  %d\r\n", pga[pga_index]);
                am_zml166_adc_gain_set(handle, pga[pga_index]);
            }
        }
    }
}

/** \brief LM75传感器I2C设备 */
static am_i2c_device_t __g_lm75_dev;

/**
 * \brief LM75设备初始化
 */
static void lm75_init (am_i2c_handle_t handle)
{

    /* 初始配置好LM75设备信息 */
    am_i2c_mkdev(&__g_lm75_dev,
                  handle,
                  0x48,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
}

/**
 * \brief 从LM75传感器读取温度值
 *
 * \note 返回的数值为读取的温度数值，
 *       高字节为整数部分，低字节为小数部分，实际温度应该除以256.0
 */
static int16_t __lm75_read (void)
{

    /* 用于存放读取的值 */
    uint8_t temp_value[2] = {0 , 0};

    /* 16位有符号数，用于保存温度值 */
    int16_t temp;

    /* 从0地址读取两个字节，即温度 */
    am_i2c_read(&__g_lm75_dev,
                0x00,
                &temp_value[0],
                2);

    /* 0xE0 = 1110 0000，小数部分仅高3位有效，低5位清零 */
    temp_value[1] &= 0xE0;

    temp = temp_value[0] << 8 | temp_value[1];

    return temp;
}
void demo_aml166_std_i2c_lm75_entry (am_uart_handle_t uart_handle)
{
    char            str;
    int16_t         temperature = 0;
    am_i2c_handle_t handle;

    handle = am_zlg116_i2c1_inst_init();

    lm75_init(handle);

    while (1) {
        temperature = __lm75_read();

        AM_DBG_INFO("Current temperature is %d.%01d°C\r\n",
                     temperature / 256,
                    (temperature * 10 / 256)  % 10);
        am_mdelay(300);
        if(am_uart_poll_getchar(uart_handle, &str) == AM_OK){
            if(str == 'e'){
                break;
            }
        }
    }
}

/**
 * \brief 热电阻校准系数获取函数
 */
void demo_aml166_thermistor_para_adjust_entry(am_uart_handle_t uart_handle)
{
    char  uart_data[20];
    float mem_data[16];
    float pt100_para[2] = {1, 0};
    float r1_std, r2_std, r1_mea, r2_mea;
    am_zml166_adc_handle_t  handle = am_zml166_adc_inst_init();

    am_kprintf("Enter PT100 ratio adjust mode...\n");
   /* 获取R1标准值 */
    while(AM_OK != __str_cmp(uart_data, "r1:", 3)){
        am_kprintf("Please input R1 resistance(unit:Ω). eg:  r1:30.112\\n\n ");
        uart_str_get(uart_handle, uart_data);
    }
    r1_std = __atof(&uart_data[3]);
    am_kprintf("You intput R1 is %d.%03d Ω\r\n\r\n",
               (uint32_t)(r1_std*1000)/1000,
               (uint32_t)(r1_std*1000)%1000);
    /* 获取R2标准值 */
    while(AM_OK != __str_cmp(uart_data, "r2:", 3)){
        am_kprintf("Please input R2 resistance(unit:Ω). eg:  r2:30.112\\n\n ");
        uart_str_get(uart_handle, uart_data);
    }
    r2_std = __atof(&uart_data[3]);
    am_kprintf("You intput R2 is %d.%03d Ω\r\n\r\n",
               (uint32_t)(r2_std * 1000) / 1000,
               (uint32_t)(r2_std * 1000) % 1000);

    /* 设置PT100增益倍数 */
    am_zml166_adc_gain_set(handle, 1);

    am_kprintf("Please config R1 measure.\n");
    while(AM_OK != __str_cmp(uart_data, "Y", 1)){
        am_kprintf("Are you do it? (Intput ‘Y\\n’ to ensure you have do it) \r\n");
        uart_str_get(uart_handle, uart_data);
    }
    am_kprintf("Y\r\n\r\n");

    r1_mea = am_zml166_adc_thermistor_res_data_get(handle);

    memset(uart_data, 0 , 20);
    am_kprintf("Please config R2 measure.\n");
    while(AM_OK != __str_cmp(uart_data, "Y", 1)){
        am_kprintf("Are you do it? (Intput ‘Y\\n’ to ensure you have do it) \r\n");
        uart_str_get(uart_handle, uart_data);
    }
    am_kprintf("Y\r\n\r\n");

    r2_mea = am_zml166_adc_thermistor_res_data_get(handle);

    if((r2_std > r1_std && r2_mea > r1_mea) || (r2_std < r1_std && r2_mea < r1_mea)){
        pt100_para[0] = (r2_std - r1_std) / (r2_mea - r1_mea);
        pt100_para[1] = r2_std - pt100_para[0] * r2_mea;
    }else{
        r2_std = r2_std + r1_std;
        r1_std = r2_std - r1_std;
        r2_std = r2_std - r1_std;
        pt100_para[0] = (r2_std - r1_std) / (r2_mea - r1_mea);
        pt100_para[1] = r2_std - pt100_para[0] * r2_mea;
    }

    __print_adjust_function(10, pt100_para[0], pt100_para[1]);

    //判断温度系数修调是否成功
    if((pt100_para[0] < 1.02 && pt100_para[0] > 0.98) &&
            (pt100_para[1] < 0.3 && pt100_para[1] > -0.3)){
        am_kprintf("Demo-a Success!\r\n\r\n");
    }else{
        am_kprintf("Demo-a Fail!\r\n\r\n");
        pt100_para[0] = 1;
        pt100_para[1] = 0;
    }

    am_zlg_flash_init(ZLG116_FLASH);

    memcpy((void *)mem_data, (uint32_t *)((FLASH_BLOCK_NUM * 1024)), 4 * 16);

    am_zlg_flash_sector_erase (ZLG116_FLASH, (1024 * FLASH_BLOCK_NUM));

    am_zlg_flash_sector_program(ZLG116_FLASH,
                               (1024 * FLASH_BLOCK_NUM),
                               (uint32_t *)mem_data, 16);

    am_zlg_flash_sector_program(ZLG116_FLASH,
                                PT100_PARA_SAVE_ADDRESS,
                               (uint32_t *)pt100_para,
                                2);
}

/*
 * \brief ADC系数相关操作函数
 */
void demo_aml166_vol_adjust_entry(am_uart_handle_t uart_handle)
{
    float   para[18];

    am_zml166_adc_handle_t  handle = am_zml166_adc_inst_init();

    am_zlg_flash_init(ZLG116_FLASH);

    demo_zml166_adc_vol_para_adjuet_entry(handle, uart_handle,(float *) para);
}


/*
 * AML166 所有板级资源演示程序
 */
void  am_aml166_all_demos(void)
{
    char  buffer[20];
    am_uart_handle_t uart_handle = am_zlg116_uart1_inst_init();
    while(1){
        while(AM_OK != __str_cmp(buffer, "demo-", 5)){
            am_kprintf("Please select which demo you want to do:(eg: ‘demo-1\\n’) \r\n\r\n");
            am_kprintf("1. PT100 temperature measure  Demo\r\n");
            am_kprintf("2. Thermocouple temperature measure Demo\r\n");
            am_kprintf("3. Res bridge measure Demo\r\n");
            am_kprintf("4. LM75 temperature measure Demo\r\n");
            am_kprintf("5. Key and Buzzer Demo\r\n");
            am_kprintf("6. Calibration Demo\r\n");
            uart_str_get(uart_handle, buffer);
        }
        switch (buffer[5]) {
            case '1':
                demo_aml166_thermistor_measure_entry(uart_handle);
                break;
            case '2':
                demo_aml166_thermocouple_measrue_entry(uart_handle);
                break;
            case '3':
                demo_aml166_vol_measure_entry(uart_handle, 1);
                break;
            case '4':
                demo_aml166_std_i2c_lm75_entry(uart_handle);
                break;
            case '5':
                demo_key_buzzer_entry(uart_handle);
                break;
            case '6':
                demo_aml166_vol_adjust_entry(uart_handle);
                break;
            case 'a':
                demo_aml166_thermistor_para_adjust_entry(uart_handle);
                break;
            case 'b':
                demo_aml166_vol_measure_entry(uart_handle, 4);
                break;
            default:
                break;
        }
        memset(buffer,0,20);
    }
}
