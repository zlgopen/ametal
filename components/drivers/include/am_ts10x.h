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
 * \brief ts10x（指纹仪模组）接口函数
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-12  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_TS10X_H
#define __AM_TS10X_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_i2c.h"
#include "am_softimer.h"
#include "am_uart_rngbuf.h"

/**
 * @addtogroup am_if_ts10x
 * @copydoc am_ts10x.h
 * @{
 */

/**
 * \name 执行结果的状态
 *
 *     这些值为事务处理回调函数的status参数可能的值， 仅仅当结果为  AM_TS10X_SUCCESS 时，
 * 才表明执行成功。
 * @{
 */

#define AM_TS10X_SUCCESS       0x00   /**< \brief 执行成功               */
#define AM_TS10X_FAIL          0x01   /**< \brief 执行失败               */
#define AM_TS10X_FULL          0x04   /**< \brief 数据库满               */
#define AM_TS10X_NOUSER        0x05   /**< \brief 没有这个用户     */
#define AM_TS10X_USER_EXIST    0x07   /**< \brief 用户已存在           */
#define AM_TS10X_TIMEOUT       0x08   /**< \brief 图像采集超时      */
#define AM_TS10X_BREAK         0x18   /**< \brief 当前操作被终止 */
#define AM_TS10X_IMAGEFAIL     0x84   /**< \brief 图像不合格           */

/** @} */

/**
 * \brief TS10X操作回调函数
 *
 * \param[in] p_arg    : 用户自定义参数
 * \param[in] status   : 操作执行的状态
 *
 *
 * \note status的值若为0，表示操作成功执行完成。若为正数，为指纹仪自定义错误信息，
 * 如  AM_TS10X_USER_EXIST。若为负数，为AMetal平台自定义的错误信息，如  -AM_ENOMEM。
 * 一般来讲，调用回调函数时，都表示操作完成，可以继续执行下一次操作。特殊地，对于
 * 用户注册操作（am_ts10x_user_register_*），由于需要多次按压，每次按压后，需要手
 * 指抬起后再次按压。这种情况下，每次按压结束后都会调用回调函数，若还需要继续按压
 * 以便注册，此时, status的值为 -AM_EINPROGRESS，表示操作还在继续执行中。只要当整个
 * 注册过程结束时，status的值才不为 -AM_EINPROGRESS。
 */
typedef void (*am_ts10x_process_cb_t) (void *p_arg, int status);

/**
 * \brief ts10x设备结构体定义
 */
typedef struct am_ts10x_dev {

    am_softimer_t    timer;       /**< \brief The Timer for timeout */
    am_uart_handle_t uart_handle; /**< \brief The UART handle       */

    am_bool_t        cmd_ack_complete;   /**< \brief cmd/ack is complete?     */

    uint8_t   data[4];            /**< \brief The data for first packet       */
    uint8_t   cmd_ack_type;       /**< \brief The type of this cmd            */
    uint8_t   is_send_sec;        /**< \brief there is second packet to send? */
    uint8_t   send_sec_num;       /**< \brief the buffer num for the second   */
    uint8_t   is_recv_sec;        /**< \brief there is second packet to recv? */
    uint8_t   recv_sec_num;       /**< \brief the buffer num for the second   */
    uint8_t   chk_value;          /**< \brief the check value for send/recv   */

    int       int_pin;            /**< \brief The int pin                     */
    int       complete_status;    /**< \brief The status for complete         */

    uint32_t  timeout_val;        /**< \brief timeout val for recv now        */
    uint32_t  timeout_expect_val; /**< \brief timeout expect value            */

    size_t    max_length;         /**< \brief The max length of sec data      */
    size_t    data_index;         /**< \brief The data index for now          */
    size_t    sec_buf_index;      /**< \brief The buffer index for sec pack   */


    /** \brief The data buffer info for send second packet */
    struct {
        const uint8_t  *p_buf;
        size_t          expect_len;
    } send_sec_buf_info[3];

    /** \brief The data buffer info for receive second packet */
    struct {
        uint8_t  *p_buf;
        size_t    expect_len;
    } recv_sec_buf_info[3];

    /** \brief private callback   */
     void                       (*pfn_private_cb) (struct am_ts10x_dev *, int);

    /** \brief user callback  */
     am_ts10x_process_cb_t        pfn_process_cb;

    /** \brief The argument for user callback */
    void                         *p_process_arg;

    /** \brief user callback  */
    am_pfnvoid_t                  pfn_int_pin_cb;

    /** \brief The argument for user callback */
    void                         *p_int_pin_arg;

    /** \brief 仅仅供内部使用                 */
    const void                   *p_tx_state;

    /** \brief 仅仅供内部使用                 */
    const void                   *p_rx_state;

    /** \brief 命令开始后，至接收到ACK第一个字符的超时时间  */
    uint32_t                      timeout_cmd_start;

    /** \brief The data use internel only */
    union {
        struct {
            volatile  size_t   *p_info_len;
        } version_info_get;

        struct {
            volatile uint16_t *p_user_count;
        } user_count_get;

        struct {
            volatile uint8_t  *p_user_role;
        } user_role_get;

        struct {
            volatile uint16_t  *p_user_id;
              volatile uint8_t   *p_user_role;
        } compare_1_n;

        struct {
            volatile uint16_t  *p_user_id;
        } unused_user_id_get;

        struct {
            volatile uint32_t   baudrate;
        } baudrate_set;

        struct {
            volatile size_t   *p_actual_len;
        } image_data_get;

        struct {
            volatile uint8_t   *p_old_level;
        } compare_level_set;

        struct {
              uint8_t   user_number[2];
              uint16_t *p_user_count;
        } user_all_id_role_get;

        struct {
              am_bool_t *p_allow_same;
        } register_mode_get;

        struct {
              uint8_t  user_id_role[3];
        } add_user_with_feature;

        struct {
              uint8_t  user_id_role[3];
        } compare_feature_with_one_user;

        struct {
              uint16_t  *p_user_id;
        } compare_feature_with_all_user;

        struct {
              uint8_t  data[3];
        } zero_data;

        struct {
               uint16_t  *p_user_id;
               uint8_t    user_role;
               void     (*pfn_continue) (struct am_ts10x_dev *);
        } register_data;

    } private_data;

} am_ts10x_dev_t;


/** \brief 定义ts10x操作句柄  */
typedef struct am_ts10x_dev *am_ts10x_handle_t;

/**
 * \brief ts10x初始化函数
 *
 * 使用ts10x前，应该首先调用该初始化函数，以获取操作ts10x的handle。
 *
 * \param[in] p_dev       : 指向ts10x实例的指针
 * \param[in] int_pin     : 与TS10X中断引脚相连接的MCU引脚号
 * \param[in] uart_handle : UART标准服务操作句柄
 *
 * \return 操作ts10x的handle，若初始化失败，则返回值为NULL。
 *
 * \note 设备指针p_dev指向的设备只需要定义，不需要在调用本函数前初始化。
 */
am_ts10x_handle_t am_ts10x_init (am_ts10x_dev_t           *p_dev,
                                 int                       int_pin,
                                 am_uart_handle_t          uart_handle);

/**
 * \brief ts10x 解初始化
 * \param[in] handle  : TS10X的实例句柄
 * \return AM_OK，解初始化成功；其它值，解初始化失败。
 */
int am_ts10x_deinit (am_ts10x_handle_t handle);

/**
 * \brief 设置 ts10x 的事务处理回调函数
 *
 * 后续所有功能相关的接口函数（除初始化函数和解初始化函数外），都是异步执行的，函数
 * 会立即返回，当操作完成一个阶段时，将调用此处设置的事务处理回调函数。
 *
 * \param[in] handle         : TS10X的实例句柄
 * \param[in] pfn_process_cb : 事务处理回调函数
 * \param[in] p_arg          : 回调函数参数
 *
 * \return 操作ts10x的handle，若初始化失败，则返回值为NULL。
 */
int am_ts10x_process_cb_set (am_ts10x_handle_t        handle,
                             am_ts10x_process_cb_t    pfn_process_cb,
                             void                    *p_arg);

/**
 * \brief 设置 ts10x 中断引脚回调函数
 *
 * 当有手指按压指纹仪时，中断引脚将输出高电平，此时，将调用此处设置的中断引脚回调
 * 函数
 *
 * \param[in] handle         : 指向ts10x实例的指针
 * \param[in] pfn_int_pin_cb : 中断引脚回调函数
 * \param[in] p_arg          : 回调函数参数
 *
 * \return 操作ts10x的handle，若初始化失败，则返回值为NULL。
 */
int am_ts10x_int_pin_cb_set (am_ts10x_handle_t  handle,
                             am_pfnvoid_t       pfn_int_pin_cb,
                             void              *p_arg);

/**
 * \name 注册用户使用的注册方式
 *
 * 用户注册共有4种主流的注册方式，即1C3R、 2C2R、 3C3R、 MOFN(默认的标准版本是1C3R的
 * 模式)。aCbR的意思是主机端发送a个Command，模块端返回b个Return，即发送a次命令b次
 * 返回；其中2C2R也兼容1C3R。 MOFN的意思是M-N次按压注册，最少M次，最多N次，N次还注
 * 册不成功返回错误。
 *
 * 特定的指纹仪一般只支持特定的一种注册方法，使用  am_ts10x_user_register() 函数进行
 * 用户注册时，应该确保使用正确的注册方式。
 *
 * @{
 */

#define AM_TS10X_USER_REG_METHOD_1C3R      1    /**< \brief 1C3R 注册方式   */
#define AM_TS10X_USER_REG_METHOD_2C2R      2    /**< \brief 2C2R 注册方式   */
#define AM_TS10X_USER_REG_METHOD_3C3R      3    /**< \brief 3C3R 注册方式   */
#define AM_TS10X_USER_REG_METHOD_MOFN      4    /**< \brief MOFN 注册方式   */

/** @} */

/**
 * \brief 用户注册
 *
 * \param[in]     handle     : ts10x实例句柄
 * \param[in,out] p_user_id  : 输入：本次注册用户的ID （有效值：1 ~ 10000），为0时
 *                                   将自动分配一个未被使用的ID
 *                             输出：实际注册用户的ID，当输入值为0时，该值即为自动
 *                                   分配的用户ID。
 *
 * \param[in]     user_role  : 用户权限，有效值1 ~ 3，具体各值的含义用户自定义
 * \param[in]     reg_method : 注册方式，如：#AM_TS10X_USER_REG_METHOD_1C3R
 *
 * \retval AM_OK            : 用户注册成功
 * \retval -AM_EINPROGRESS  : 正在注册中，注册完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_user_register (am_ts10x_handle_t  handle,
                            uint16_t          *p_user_id,
                            uint8_t            user_role,
                            uint8_t            reg_method);

/**
 * \brief 删除一个特定用户
 *
 * \param[in]  handle       : ts10x实例句柄
 * \param[in]  user_id      : 待删除的用户 ID
 *
 * \retval AM_OK            : 删除用户成功
 * \retval -AM_EINPROGRESS  : 正在删除中，删除完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_user_one_del (am_ts10x_handle_t  handle, uint16_t user_id);

/**
 * \brief 删除所有用户
 *
 * \param[in]  handle       : ts10x实例句柄
 *
 * \retval AM_OK            : 删除用户成功
 * \retval -AM_EINPROGRESS  : 正在删除中，删除完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_user_all_del (am_ts10x_handle_t  handle);


/**
 * \brief 获取当前用户总数
 *
 * \param[in]  handle       : ts10x实例句柄
 * \param[out] p_user_count : 获取用户总数的指针
 *
 * \retval AM_OK            : 获取用户总数成功
 * \retval -AM_EINPROGRESS  : 正在获取中，获取完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_user_count_get (am_ts10x_handle_t handle, uint16_t *p_user_count);

/**
 * \brief 获取指定用户的权限
 *
 * \param[in]  handle      : ts10x实例句柄
 * \param[in]  user_id     : 用户ID
 * \param[out] p_user_role : 获取用户权限的指针
 *
 * \retval AM_OK            : 获取用户权限成功
 * \retval -AM_EINPROGRESS  : 正在获取中，获取完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_user_role_get (am_ts10x_handle_t handle,
                            uint16_t          user_id,
                            uint8_t          *p_user_role);

/**
 * \brief 获取一个未被使用的用户ID
 *
 * \param[in]  handle      : ts10x实例句柄
 * \param[out] p_user_id   : 用于获取未使用的用户ID的指针
 *
 * \retval AM_OK            : 获取成功
 * \retval -AM_EINPROGRESS  : 正在获取中，获取完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_user_id_unused_get (am_ts10x_handle_t  handle,
                                 uint16_t          *p_user_id);

/**
 * \brief 设置波特率（每次复位都会恢复115200）
 *
 * \param[in]  handle        : ts10x实例句柄
 * \param[out] baudrate      : 使用的波特率。
 *                             指纹仪仅支持：9600、19200、38400、57600 和 115200
 *
 * \retval AM_OK            : 获取成功
 * \retval -AM_EINPROGRESS  : 正在获取中，获取完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 *
 * \note 使用该函数设置波特率时，需要确保UART本身支持该波特率
 */
int am_ts10x_baudrate_set (am_ts10x_handle_t  handle,
                           uint32_t           baudrate);

/**
 * \brief 采集指纹图像,提取指纹图像的特征值（494字节）
 *
 * \param[in]  handle     : ts10x实例句柄
 * \param[out] p_data_buf : 存放提取的特征值的缓冲区
 * \param[in]  buf_len    : 缓冲区长度，由于特征值长度为494字节，因此该值为 494
 *
 * \retval AM_OK            : 提取成功
 * \retval -AM_EINPROGRESS  : 正在提取中，提取完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 *
 * \note 执行该函数后，指纹仪进入采集指纹状态，需要在8s内按压指纹，以便采集。
 */
int am_ts10x_collect_image_feature_get (am_ts10x_handle_t  handle,
                                        uint8_t           *p_data_buf,
                                        size_t             buf_len);

/**
 * \brief 采集指纹图像,返回指纹图像的位图数据
 *
 * \param[in]  handle       : ts10x实例句柄
 * \param[out] p_data_buf   : 存放图像值的缓冲区
 * \param[in]  buf_len      : 缓冲区长度，应该与实际的图像数据长度一致，实际
 *                            图像数据的长度与传感器分辨率相关，
 *                            为：width * 64 + Height / 4
 *
 * \param[out] p_actual_len : 实际位图数据的长度
 *
 * \retval AM_OK            : 提取成功
 * \retval -AM_EINPROGRESS  : 正在提取中，提取完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 *
 * \note 执行该函数后，指纹仪进入采集指纹状态，需要在8s内按压指纹，以便采集。
 * 一般地，我们使用的指纹仪传感器的分辨率为 176 * 176，此时，图像数据的大小为：
 * 176 * 64 + 176 / 4 = 11308 字节。大约 11K，使用该函数时，就需要一个11K的
 * 缓冲区。在系统RAM较小的场合，很少使用该函数直接获取图像数据，只需要获取特征值
 * 就可以了。
 */
int am_ts10x_collect_image_data_get (am_ts10x_handle_t  handle,
                                     uint8_t           *p_data_buf,
                                     size_t             buf_len,
                                     size_t            *p_actual_len);

/**
 * \brief 获取指纹模组的版本信息（字符串）
 *
 *  获取模组版本信息，其中包含了程序版本、传感器类型、注册方式、编译时间等信息。
 *
 *  例如，编写本驱动程序时，获取到的信息为下面几行的字符串（长度为140字符）：
 *   Version:  7V33.35_stdrl_024
 *   Sensor:TS1060
 *   RegMode:1C3R
 *   MaxUser:394
 *   Time:Jul  4-15:50:02
 *   Tooan Co.ltd. All rights reserved.
 *
 * \param[in]  handle       : ts10x实例句柄
 * \param[in]  p_buf        : 存放版本信息的缓冲区
 * \param[in]  buf_size     : 缓冲区长度
 * \param[out] p_info_size  : 实际信息的长度
 *
 * \retval AM_OK            : 获取版本信息成功
 * \retval -AM_EINPROGRESS  : 正在处理中，处理完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_version_info_get (am_ts10x_handle_t  handle,
                               uint8_t           *p_buf,
                               size_t             buf_size,
                               size_t            *p_info_size);
/**
 * \brief 设置比对等级
 *
 *  比对等级可以设置为 0-3，默认的是光学/TS10X1的是1，半导体1060的是3，比对等级越高，
 *  限制越严格。模组重启后会恢复到默认值。
 *
 * \param[in]  handle       : ts10x实例句柄
 * \param[in]  new_level    : 新设置的比对等级
 * \param[out] p_old_level  : 获取之前使用的比对等级
 *
 * \retval AM_OK            : 设置比对等级成功
 * \retval -AM_EINPROGRESS  : 正在处理中，处理完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_compare_level_set (am_ts10x_handle_t  handle,
                                uint8_t            new_level,
                                uint8_t           *p_old_level);
/**
 * \brief 获取所有注册用户的ID和权限信息
 *
 * \param[in]  handle       : ts10x实例句柄
 * \param[out] p_user_count : 当前实际用户数目
 * \param[out] p_buf        : 存放用户ID和权限信息的缓冲区，每个用于占用三个字节：
 *                             user_id(高8位) 、 user_id (低8位)、role
 *
 * \param[in]  buf_len      : 缓冲区长度，应该为：用户数目 * 3 （字节数）
 *
 * \retval AM_OK            : 获取成功
 * \retval -AM_EINPROGRESS  : 正在获取中，获取完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_user_all_id_role_get (am_ts10x_handle_t   handle,
                                   uint16_t           *p_user_count,
                                   uint8_t            *p_buf,
                                   size_t              buf_len);

/**
 * \brief 设置注册模式（是否允许重复注册已经存在的用户）
 *
 * \param[in] handle      : ts10x实例句柄
 * \param[in] allow_same  : 是否允许重复注册已经存在的用户，AM_TRUE，允许，AM_FALSE，不允许
 *
 * \retval AM_OK            : 设置成功
 * \retval -AM_EINPROGRESS  : 正在设置中，设置完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 *
 * \note 默认为允许重复注册已经存在的用户，每次上电均会恢复至默认值
 */
int am_ts10x_register_mode_set (am_ts10x_handle_t   handle,
                                am_bool_t           allow_same);

/**
 * \brief 获取当前的注册模式（是否允许重复注册已经存在的用户）
 *
 * \param[in] handle        : ts10x实例句柄
 * \param[out] p_allow_same : 获取当前的注册模式，AM_TRUE，允许，AM_FALSE，不允许
 *
 * \retval AM_OK            : 获取成功
 * \retval -AM_EINPROGRESS  : 正在 获取中，设置完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_register_mode_get (am_ts10x_handle_t   handle,
                                am_bool_t          *p_allow_same);

/**
 * \brief 获取指定用户的指纹特征值
 *
 * \param[in]  handle      : ts10x实例句柄
 * \param[in]  user_id     : 用户ID
 * \param[out] p_user_role : 获取用户权限的指针
 * \param[out] p_buf       : 存放提取的特征值的缓冲区
 * \param[in]  buf_len     : 缓冲区长度，由于特征值长度为494字节，因此该值为 494
 *
 * \retval AM_OK            : 获取成功
 * \retval -AM_EINPROGRESS  : 正在 获取中，设置完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_user_feature_get (am_ts10x_handle_t   handle,
                               uint16_t            user_id,
                               uint8_t            *p_user_role,
                               uint8_t            *p_buf,
                               size_t              buf_len);

/**
 * \brief 下传用户指纹特征值，存储在指纹模组中（添加一个用户）
 *
 * \param[in] handle      : ts10x实例句柄
 * \param[in] user_id     : 分配给该指纹特征值的用户 ID
 * \param[in] user_role   : 该用户的权限信息，有效值 1 ~ 3，各值含义用户自定义
 * \param[in] p_feature   : 存储指纹特征值空间的首地址
 * \param[in] feature_len : 指纹特征值的长度（字节数），必须为 494
 *
 * \retval AM_OK            : 删除用户成功
 * \retval -AM_EINPROGRESS  : 正在传输中，传输完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 */
int am_ts10x_user_add_with_feature (am_ts10x_handle_t  handle,
                                    uint16_t           user_id,
                                    uint8_t            user_role,
                                    const uint8_t     *p_feature,
                                    size_t             feature_len);

/**
 * \brief 采集指纹图像，进行 1 比 1 比对（与指纹库指定ID用户进行比较）
 *
 * \param[in] handle  : ts10x实例句柄
 * \param[in] user_id : 用户ID，用以指定用于比较的用户
 *
 * \retval AM_OK            : 比对成功
 * \retval -AM_EINPROGRESS  : 正在比对中，比对完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 *
 * \note 执行该函数后，指纹仪进入采集指纹状态，需要在8s内按压指纹，以便采集。
 */
int am_ts10x_compare_collect_with_one_user (am_ts10x_handle_t handle,
                                            uint16_t          user_id);

/**
 * \brief 采集指纹图像，进行 1 比  N 比对（与指纹库中所有用户进行比较）
 *
 * \param[in]  handle      : ts10x实例句柄
 * \param[out] p_user_id   : 若比对成功，该指针可获取到比对成功的用户ID
 * \param[out] p_user_role : 若比对成功，该指针可获取到比对成功的用户Role
 *
 *
 * \retval AM_OK            : 比对成功
 * \retval -AM_EINPROGRESS  : 正在比对中，比对完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 *
 * \note 执行该函数后，指纹仪进入采集指纹状态，需要在8s内按压指纹，以便采集。
 */
int am_ts10x_compare_collect_with_all_user (am_ts10x_handle_t  handle,
                                            uint16_t          *p_user_id,
                                            uint8_t           *p_user_role);


/**
 * \brief 采集指纹图像，与指定的指纹特征值比对
 *
 * \param[in] handle      : ts10x实例句柄
 * \param[in] p_feature   : 存储指纹特征值空间的首地址
 * \param[in] feature_len : 指纹特征值的长度（字节数），必须为 494
 *
 * \retval AM_OK            : 比对成功
 * \retval -AM_EINPROGRESS  : 正在比对中，比对完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 *
 * \note 执行该函数后，指纹仪进入采集指纹状态，需要在8s内按压指纹，以便采集。
 */
int am_ts10x_compare_collect_with_feature (am_ts10x_handle_t  handle,
                                           const uint8_t     *p_feature,
                                           size_t             feature_len);


/**
 * \brief 将指定的特征值与指定的用户特征值进行比对
 *
 * \param[in] handle      : ts10x实例句柄
 * \param[in] p_feature   : 存储指纹特征值空间的首地址
 * \param[in] feature_len : 指纹特征值的长度（字节数），必须为 494
 * \param[in] user_id     : 用户ID，用以指定用于比对的用户
 * \param[in] user_role   : 指定用于比对的用户的权限值
 *
 * \retval AM_OK            : 比对成功
 * \retval -AM_EINPROGRESS  : 正在比对中，比对完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 *
 * \note 执行该函数后，指纹仪进入采集指纹状态，需要在8s内按压指纹，以便采集。
 */
int am_ts10x_compare_feature_with_one_user (am_ts10x_handle_t  handle,
                                            const uint8_t     *p_feature,
                                            size_t             feature_len,
                                            uint16_t           user_id,
                                            uint8_t            user_role);

/**
 * \brief 将指定的特征值与所有用户特征值进行比较
 *
 * \param[in] handle      : ts10x实例句柄
 * \param[in] p_feature   : 存储指纹特征值空间的首地址
 * \param[in] feature_len : 指纹特征值的长度（字节数），必须为 494
 * \param[in] p_user_id   : 用户ID的指针
 *
 * \retval AM_OK            : 比对成功
 * \retval -AM_EINPROGRESS  : 正在比对中，比对完成后将调用回调函数
 * \retval -AM_EBUSY        : 设备忙
 *
 * \note 执行该函数后，指纹仪进入采集指纹状态，需要在8s内按压指纹，以便采集。
 */
int am_ts10x_compare_feature_with_all_user (am_ts10x_handle_t  handle,
                                            const uint8_t     *p_feature,
                                            size_t             feature_len,
                                            uint16_t          *p_user_id);

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_TS10X_H */

/* end of file */
