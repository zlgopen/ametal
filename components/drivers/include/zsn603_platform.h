#ifndef __ZSN603_PLATFORM_H
#define __ZSN603_PLATFORM_H
#include "am_wait.h"
#include "am_uart.h"
#include "am_uart_rngbuf.h"
#include "am_i2c.h"
typedef  unsigned char   uint8_t;
typedef  unsigned short  uint16_t;
//typedef  unsigned int    uint32_t;

/**************************************************************
 * UART模式下通信平台相关结构体
 **************************************************************/
/** \brief zsn603 串口平台初始化中断回调函数结构体
 *   注意，用户在使适配过程中不得修改此结构体
 *  该函数结构体用于串口平台初始化，该结构体有2个成员，该两个成员都是函数指针
 *
 * zsn603_uart_rx 为串口接收函数，用户可需置为串口接收函数回调或者在串口接收中断中直接调用
 *   该函数第一个参数为用户自定义的参数，该参数为串口初始化传输的p_arg参数，用户直接进行传递即可。
 *   该函数第二个参数为串口接收到的数据。
 * zsn603_int_pin_cb 为中断引脚回调函数，用户需设置为引脚中断回调函数或在引脚中断函数中直接调用该函数即可。
 *   需要值得注意的是，在串口模式下，只有进入自动检测模式，并且配置由中断产生是才会产生中断，且引脚需配置为下降沿中断
 *   此函数只有一个参数，为串口平台初始化中传递的p_arg参数，用户直接进行传递即可。
 */
typedef struct zsn603_uart_funcs {
    /** \brief 串口接收数据回调函数*/
    void (*zsn603_uart_rx)    (void *p_arg, char data);
    /** \brief 引脚中断引脚回调函数*/
    void (*zsn603_int_pin_cb) (void *p_arg);
}zsn603_uart_funcs_t;


/** \brief 用户UART通信模式下 平台设备信息结构体定义
 *   该结构体的作用为保存用户初始化UART通信模式时需要用到的参数。
 *   该结构体会在UART通信模式平台初始化函数(见line186)中进行调用，作为UART初始化参数类型传递
 *   该结构体中可以保存初始化UART会用到的信息，例如硬件设备号、初始波特率、缓冲区大小等信息。
 *
 *   当然，用户也可以不对该结构体进行适配，直接将该结构体的定义注释即可。
 *   这样设计的目的是用户可以在使用中直接修改该结构体的参数即可完成UART的通信属性的修改。
 */
typedef struct am_zsn603_platform_uart_devinfo{
    int8_t                 rst_pin;
    uint8_t                int_pin;
    am_uart_handle_t     (*pfn_handle_get)(void);
    void                 (*pfn_handle_deinit)(am_uart_handle_t);
    uint32_t               baud_rate;
}am_zsn603_platform_uart_devinfo_t;


/** \brief 用户UART通信模式下 平台设备相关结构体定义
 *   该结构体的作用为保存用户平台实现UART模式通信时所需要的成员
 *   例如服务句柄、串口接收发送缓冲区等。
 *
 *   该结构体类型同时作为用户适配的UART平台初始化函数(见line-183)以及传输函数(见line-168)的第一个参数。
 *
 *   用户可以自定义该结构体类型名字以及结构体中成员类型。
 *
 *   当然，用户也可以不对该结构体进行适配(将结构体定义注释即可)。
 *   但更推荐使用此方式,使用这种方式将会实现更好的对UART通信设备进行管理，同时也便于跨平台服用。
 */
typedef struct am_zsn603_platform_uart_dev{
    am_uart_handle_t                          handle;
    const am_zsn603_platform_uart_devinfo_t  *p_devinfo;
}am_zsn603_platform_uart_dev_t;


/**\brief 用户平台相关设备信息结构体类型别名定义
 *  用户在适配过程中不需要使用平台相关设备信息结构体进行适配，则可以对平台设备进行如下定义
 *   typedef  void *  zsn603_platform_uart_devinfo_t;
 *  也就是将平台设备类型定义为(void *)类型即可。
 */
typedef  am_zsn603_platform_uart_dev_t        zsn603_platform_uart_t;

/**\brief 用户平台相关设备结构体类型别名定义
 *  若用户在适配过程中不需要使用平台相关设备结构体进行适配，则可以对平台设备进行如下定义
 *   typedef  void *   zsn603_platform_uart_t;
 *  将平台设备类型定义为(void *)类型即可。
 */
typedef  am_zsn603_platform_uart_devinfo_t    zsn603_platform_uart_devinfo_t;


/**************************************************************
 * I2C模式下通信平台相关结构体
 **************************************************************/
/** \brief 用户I2C通信模式下 平台设备信息结构体定义
 *   该结构体的作用为保存用户初始化I2C通信模式时需要用到的参数。
 *   该结构体会在I2C平台初始化函数(见line-241)中进行调用，作为I2C初始化参数传递。
 *   该结构体中可以保存初始化I2C会用到的信息，例如硬件I2C的设备号、初始从机地址、中断引脚等。
 *
 *   当然，用户也可以不对该结构体进行适配(注释该结构体定义即可)，直接在初始化函数中进行用户所使用的I2C总线的初始化。
 */
typedef struct am_zsn603_platform_i2c_devinfo{
    int8_t                 rst_pin;
    uint8_t                int_pin;
    am_i2c_handle_t      (*pfn_handle_get)(void);
    void                 (*pfn_handle_deinit)(am_i2c_handle_t);
}am_zsn603_platform_i2c_devinfo_t;


/** \brief 用户I2C通信模式下 平台设备相关结构体定义
 *   该结构体的作用为保存用户平台实现I2C模式通信时所需要的成员
 *   例如服务句柄、I2C的当前速率等。
 *   该结构体类型同时作为用户适配的I2C平台初始化函数(见line254)以及读写函数(见line217与line236)的第一个参数,
 *   用户可以自定义该结构体类型名字以及结构体成员。
 *
 *   当然，用户也可以不对该结构体进行适配,直接注释该结构体定义即可。
 */
typedef struct am_zsn603_platform_i2c_dev{
    am_i2c_handle_t                          handle;
    const am_zsn603_platform_i2c_devinfo_t  *p_devinfo;
}am_zsn603_platform_i2c_dev_t;


/**\brief 用户平台相关设备信息结构体类型别名定义
 *
 *  若用户在适配过程中、不需要使用平台相关设备信息结构体进行适配，则可以对平台设备进行如下定义:
 *        typedef  void *  zsn603_platform_i2c_devinfo_t;
 *  也就是将平台设备类型定义为(void *)类型即可。
 */
typedef    am_zsn603_platform_i2c_dev_t        zsn603_platform_i2c_t;

/**\brief 用户平台相关设备结构体类型别名定义
 *  用户在适配过程中、不需要使用平台相关设备结构体进行适配，则可以对平台设备进行如下定义
 *   typedef  void * zsn603_platform_i2c_t;
 *  即将平台设备类型定义为(void *)类型。
 */
typedef    am_zsn603_platform_i2c_devinfo_t    zsn603_platform_i2c_devinfo_t;


/******************************************************************************
 *   ZSN603通用驱动需要用户适配的一些类似于二进制信号量的宏定义
 *   这些宏定义用于I2C模式下  驱动获取中断信号产生以便获取回应帧信息，需要用户根据所使用平台进行适配
 ******************************************************************************/
/*
 * \brief 二进制信号量获取成功标识定义
 */
#define ZSN603_SEMB_TAKE_SUCCESS          0
/*
 * \brief 二进制信号量获取超时标识定义
 */
#define ZSN603_SEMB_TAKE_TIMEOUT          1

/*
 * \brief 二进制信号量(中断标志)定义
 *    在ZSN603设备结构体中进行使用，用于驱动获取中断信号
 */
#define ZSN603_SEMB_DECL(semb)            am_wait_t semb

/*
 * \brief 二进制信号量(中断标志)获取
 *    将会在ZSN603设备初始化中进行调用，用作出示话二进制信号量(中断标志)
 */
#define ZSN603_SEMB_INIT(semb)            am_wait_init(&semb)

/*
 * \brief 二进制信号量(中断标志)获取
 *  在指定超时时间timeout内获取二进制信号量(中断标志)，用于I2C发送命令帧之后，等待命令帧完成。
 *  正常情况下，若从机接收并执行命令帧成功，则会在指定超时时间内，在引脚中断回调函数中释放该信号量(中断标志)
 *    (1)若获取成功则返回  ZSN603_SEMB_TAKE_SUCCESS
 *    (2)若获取超时则返回  ZSN603_SEMB_TAKE_TIMEOUT
 */
#define ZSN603_SEMB_TAKE(semb, timeout)   am_wait_on_timeout(&semb, timeout)

/*
 * \brief 二进制信号量(中断标志)释放
 *   在外部中断引脚回调函数中调用，用于释放命令帧发送完毕后的等待的二进制信号量(中断标志)
 */
#define ZSN603_SEMB_GIVE(semb)            am_wait_done(&semb)

/*
 * \brief 二进制信号量(中断标志)接初始化
 *   用于二进制信号量内存释放,若用户无此功能直接忽略此宏定义适配即可
 */
#define ZSN603_SEMB_DEINIT(sem)

/******************************************************************************
 *   ZSN603通用驱动需要用户适配的串口信息函数
 *   这些函数在使用ZSN603的串口模式时被调用，若用I2C模式则以下相关函数可以不需要适配
 ******************************************************************************/
/* \brief
 *  用户需根据自己平台进行适配,函数需实现功能为:
 *           使用串口发送  保存在p_data中的nbytes个数据
 *
 * \param[in] p_dev           : ZSN603平台相关设备(UART模式)
 * \param[in] p_data          : 保存通用驱动需发送数据的地址
 * \param[in] nbytes          : 待发送的字节数
 * \retval     返回值有以下两点需要注意
 *                  (1)若全部发送成功则返回 0
 *                  (2)若发送失败或者发送字节数小于nbytes时则返回任意非0值即可,
 *                     该返回值会在执行失败的时候进行返回，用户可自定义相关宏，用于提示串口通信相关错误。
 */
int zsn603_platform_uart_tx(zsn603_platform_uart_t *p_dev,
                            uint8_t                *p_data,
                            uint32_t                nbytes);


/* \brief ZSN603平台相关设备(UART模式)初始化
 *  该函数将会在zsn603_uart_init()函数中进行调用。
 *  用户需根据自己平台进行适配,该函数内可进行UART通信初始化、串口工作时所需的相关内存的申请以及用户自定义添加的初始化工作
 * \param[in] p_dev       : ZSN603平台相关设备(UART模式)
 * \param[in] p_info      : ZSN603平台相关设备信息(UART模式)
 * \param[in] p_funcs     : ZSN603需要在用户串口中断以及引脚中断中调用的回调函数
 *                           具体介绍 见Line-9
 * \param[in] p_arg       : ZSN603需要在用户串口中断以及引脚中断中调用的回调函数参数
 * \retval     若初始化成功,则直接返回 0 即可，若初始化不成功返回任意非零值即可。
 */
uint8_t  zsn603_platform_uart_init(zsn603_platform_uart_t                *p_dev,
                                   const zsn603_platform_uart_devinfo_t  *p_info,
                                   zsn603_uart_funcs_t                   *p_funcs,
                                   void                                  *p_arg);


/* \brief ZSN603平台相关设备(UART模式)解初始化
 *  用户需根据自己使用的平台进行适配,若不使用ZSN603设备,在平台设备初始化函数中进行初始化过的设备可以在此函数
 *  中进行解初始化,也可以在此函数中完成对ZSN603平台相关设备的内存释放等工作。
 *
 * \param[in] p_dev  : ZSN603平台相关设备(UART模式)
 * \retval     若解初始化成功,则直接返回0即可，若初始化不成功返回任意非零值即可。
 */
uint8_t  zsn603_platform_uart_deinit(zsn603_platform_uart_t  *p_dev);


/******************************************************************************
 *   ZSN603通用驱动需要用户适配的串口信息函数
 *   这些函数在使用ZSN603的串口模式时被调用，若用I2C模式则以下相关函数可以不需要适配
 ******************************************************************************/
/* \brief I2C总线写函数
 *
 *  用户需根据自己平台进行适配,函数需实现功能为:
 *           往从机地址为slv_addr且器件子地址为sub_addr(2字节)的设备写入p_data内的nbytes个数据
 *
 * \param[in] p_dev           : ZSN603平台相关设备(I2C模式)
 * \param[in] slv_addr        : 从机地址(低7位有效)
 * \param[in] sub_addr        : 器件子地址(2字节)
 * \param[in] p_data          : 保存待写的数据的地址
 * \param[in] nbytes          : 需要写入数据字节数
 * \retval     返回值有以下两点需要注意
 *                (1)若I2C写成功 返回 0即可
 *                (2)若I2C写失败 则返回任意非0值即可,该返回值会在执行失败的时候进行返回，用户可自定义相关宏，用于提示I2C通信相关错误
 */
int zsn603_platform_i2c_write(zsn603_platform_i2c_t *p_dev,
                              uint8_t                slv_addr,
                              uint16_t               sub_addr,
                              uint8_t               *p_data,
                              uint32_t               nbytes);


/* \brief I2C总线读函数
 *
 *  用户需根据自己平台进行适配,函数需实现功能为:
 *           从从机地址为slv_addr且器件子地址为sub_addr(2字节)的设备读出nbytes个数据存放至p_data内
 * \param[in] p_dev           : ZSN603平台相关设备(I2C模式)
 * \param[in] slv_addr        : 从机地址(低7位有效)
 * \param[in] sub_addr        : 器件子地址(2字节)
 * \param[in] p_data          : 保存i2c读取数据的起始地址
 * \param[in] nbytes          : 需要读取的字节数
 * \retval     返回值有以下两点需要注意
 *               (1)若IIC读成功 返回 0即可
 *               (2)若IIC读失败 则返回任意非0值即可,该返回值会在执行失败的时候进行返回，用户可自定义相关宏，用于提示串口通信相关错误
 */
int zsn603_platform_i2c_read(zsn603_platform_i2c_t  *p_dev,
                             uint8_t                 slv_addr,
                             uint16_t                sub_addr,
                             uint8_t                *p_data,
                             uint32_t                nbytes);


/* \brief ZSN603平台相关设备(I2C模式)初始化
 *  该函数将会在zsn603_i2c_init()函数中进行调用。
 *    用户需根据自己平台进行适配,该函数内可进行I2C通信初始化、完成需要的内存申请、中断引脚初始化以及用户自定义添加的初始化工作
 *    需要值得注意的是 : 第三个参数为一个函数指针，指向中断引脚的回调函数，中断引脚需配置为下降沿触发，用户可以设置该函数为中断回调函数或者在中断函数中调用该函数。
 *               该函数的作用为,ZSN603接受到命令帧之后，执行完对应的命令之后，会产生一个下降沿，用以通知通用软件包I2C命令执行完毕，可以进行回应帧读取
 *               第四个参数为函数指针的参数,该参数为(void *)类型，用户无需关心该参数，只需要将其传递给参数三的函数指针即可
 * \param[in] p_dev                      : ZSN603平台相关设备(I2C模式)
 * \param[in] p_info                     : ZSN603平台相关设备信息(I2C模式)
 * \param[in] int_pin_callback_function  : 中断引脚回调函数
 * \param[in] p_arg                      : 回调函数参数
 * \retval     若初始化成功,则直接返回0即可，若初始化不成功返回任意非零值即可。
 */
uint8_t  zsn603_platform_i2c_init(zsn603_platform_i2c_t                *p_dev,
                                  const zsn603_platform_i2c_devinfo_t  *p_info,
                                  void(*int_pin_callback_function)     (void *),
                                  void                                 *p_arg);


/* \brief ZSN603平台相关设备(I2C模式)解初始化
 *  该函数将会在
 *    用户需根据自己使用的平台进行适配,若不使用ZSN603设备,在平台设备初始化函数中进行初始化过的设备可以在此函数
 *    中进行解初始化,也可以在此函数中完成对ZSN603平台相关设备的内存释放等工作。
 *
 * \param[in] p_dev  : ZSN603平台相关设备(I2C模式)
 * \retval     若解初始化成功,则直接返回0即可，若初始化不成功返回任意非零值即可。
 */
uint8_t  zsn603_platform_i2c_deinit(zsn603_platform_i2c_t    *p_dev);
#endif
