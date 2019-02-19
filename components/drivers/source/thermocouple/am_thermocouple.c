/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief  热电偶计算公式
 *
 * \internal
 * \par modification history:
 * - 1.00 19-01-17  htf, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "math.h"
#include "am_thermocouple.h"
/*******************************************************************************
  T、N、R、J、K型热偶，温度转电压，电压转温度系数
*******************************************************************************/
/**
 * \brief K型温度转电压的系数
 */
static const double __g_var_t_to_v_k[3][12] = {
    {
         0,                  3.9450128025/1e2,   2.3622373598/1e5,
        -3.2858906784/1e7,  -4.9904828777/1e9,
        -6.7509059173/1e11, -5.7410327428/1e13, -3.1088872894/1e15,
        -1.0451609365/1e17, -1.9889266878/1e20, -1.6322697486/1e23
    },
    {
        -1.7600413686/1e2,   3.8921204975/1e2,   1.8558770032/1e5,
        -9.9457592874/1e8,   3.1840945719/1e10, -5.60722844889/1e13,
         5.6075059059/1e16, -3.2020720003/1e19,  9.7151147152/1e23,
        -1.2104721275/1e26
    },
    {
         1.185976/1e1,      -1.183432/1e4
    },
};

/**
 * \brief K型电压转温度的系数
 */
static const double __g_var_v_to_t_k[3][10] = {
    {
         0,              2.5173462e1,   -1.1662878,
        -1.0833638,     -8.9773540/1e1, -3.7342377/1e1,
        -8.6632643/1e2, -1.0450598/1e2, -5.1920577/1e4
    },
    {
         0,             2.508355e1,    7.860106/1e2,
        -2.503131/1e1,  8.315270/1e2, -1.228034/1e2,
         9.804036/1e4, -4.413030/1e5,  1.057734/1e6,
        -1.052755/1e8
    },
    {
        -1.318058e2,    4.830222e1,  -1.646031,
         5.464731/1e2, -9.650715/1e4, 8.802193/1e6,
        -3.110810/1e8
    },
};

/**
 * \brief T型温度转电压的系数
 */
static const double __g_var_t_to_v_t[2][15] = {
    {
        0,                 3.8748106364*1e1,  4.4194434347/1e2,
        1.1844323105/1e4,  2.0032973554/1e5,  9.0138019559/1e7,
        2.2651156593/1e8,  3.6071154205/1e10, 3.8493939883/1e12,
        2.8213521925/1e14, 1.4251594779/1e16, 4.8768662286/1e19,
        1.0795539270/1e21, 1.3945027062/1e24, 7.9795153927/1e29
    },
    {
         0,                 3.8748106364*1e1,   3.329222788/1e2,
         2.0618243404/1e4, -2.1882556846/1e6,   1.0996880928/1e8,
        -3.0815758772/1e11, 4.5479135290/1e14, -2.7512901673/1e17
    },

};

/**
 * \brief T型电压转温度的系数
 */
static const double __g_var_v_to_t_t[2][8] = {
    {
        0,              2.5949192e1,   -2.1316967/1e1,
        7.9018692/1e1,  4.2527777/1e1,  1.3304473/1e1,
        2.02414460/1e2, 1.2668171/1e3
    },
    {
         0,               2.592800e1,   -7.602961/1e1,
         4.637791/1e2,   -2.165394/1e3,  6.048144/1e5,
        -7.293422/1e7
    },
};

/**
 * \brief N型温度转电压的系数
 */
static const double __g_var_t_to_v_n[2][11] = {
    {
         0,                  2.6159105962*1e1,   1.0957484228/1e2,
        -9.3841111554/1e5,  -4.6412039759/1e8,  -2.6303557716/1e9,
        -2.2653438003/1e11, -7.6089300791/1e14, -9.3419667835/1e17,
    },
    {
         0,                  2.5929394601*1e1,   1.571014188/1e2,
         4.3825627237/1e5,  -2.5261169794/1e7,   6.4311819339/1e10,
        -1.0063471519/1e12,  9.9745338992/1e16, -6.0863245607/1e19,
         2.0849229339/1e22  -3.0682196151/1e26
    },

};

/**
 * \brief N型电压转温度的系数
 */
static const double __g_var_v_to_t_n[3][10] = {
    {
        0,          3.843684e1,    1.1010485,
        5.2229312,  7.2060525,     5.8488586,
        2.7754916,  7.7075166/1e1, 1.1582665/1e1,
        7.3138868/1e3
    },
    {
         0,            3.86896e1,   -1.08267,
         4.70205/1e2, -2.12169/1e6, -1.17272/1e4,
         5.39280/1e6  -7.98156/1e8
    },
    {
        1.972485*1e1,   3.300943e1, -3.915159/1e1,
        9.855391/1e3, -1.274371/1e4, 7.767022/1e7
    },
};


/**
 * \brief R型温度转电压的系数
 */
static const double __g_var_t_to_v_r[3][10] = {
    {
         0,                   5.28961729765,       1.39165589782/1e2,
        -2.38855693017/1e5,   3.56916001063/1e8,  -4.62347662298/1e11,
         5.00777441034/1e14, -3.73105886191/1e17,  1.57716482367/1e20,
        -2.81038625251/1e24
    },
    {
         2.95157925316*1e3,  -2.52061251332,     1.59564501865/1e2,
        -7.64085947576/1e6,  2.05305291024/1e9, -2.93359668173/1e13
    },
    {
         1.5223211829*1e5,  -2.68819888509*1e2, 1.71280280471/1e1,
        -3.45895706453/1e5, -9.34633971046/1e12
    },
};

/**
 * \brief R型电压转温度系数
 */
static const double __g_var_v_to_t_r[4][11] = {
    {
         0,               1.889138/1e1,   -9.383529/1e5,
         1.3068619/1e7,  -2.270358/1e10,   3.5145659/1e13,
        -3.895390/1e16,   2.8239471/1e19, -1.2607281/1e22,
         3.1353611/1e26, -3.3187769/1e30
    },
    {
         1.334584505*1e1,  1.472644573/1e1,  -1.844024844/1e5,
         4.031129726/1e9, -6.249428360/1e13,  6.468412046/1e17,
        -4.458750426/1e21, 1.994710146/1e25, -5.313401790/1e30,
         6.481976217/1e35
    },
    {
        -8.199599416*1e1,   1.553962042/1e1, -8.342197663/1e6,
         4.279433549/1e10, -1.191577910/1e14, 1.492290091/1e19
    },
    {
         3.406177836*1e4, -7.023729171,       5.582903813/1e4,
        -1.952394635/1e8,  2.560740231/1e13
    }
};

/**
 * \brief J型温度转电压系数
 */
static const double  __g_var_t_to_v_j[2][9] = {
    {
         0,                  5.0381187815*1e1,  3.047583693/1e2,
        -8.568106572/1e5,    1.3228195295/1e7, -1.7052958337/1e10,
         2.0948090697/1e13, -1.2538395336/1e16, 1.5631725697/1e20
    },
    {
         2.9645625681*1e5, -1.4976127786*1e3,  3.1787103924,
        -3.1847686701/1e3,  1.5720819004/1e6, -3.0691369056/1e10
    }
};

/**
 * \brief J型电压转温度系数
 */
static const double __g_var_v_to_t_j[3][9] = {
    {
         0,              1.9528268e1,   -1.2286185,
        -1.0752178,     -5.9086933/1e1, -1.7256713/1e1,
        -2.8131513/1e2, -2.3963370/1e3, -8.3823321/1e5
    },
    {
         0,               1.978425e1,  -2.001204/1e1,
         1.036969/1e2,   -2.549687/1e4, 3.585153/1e6,
        -5.344285/1e8,    5.099890/1e10
    },
    {
        -3.11358187*1e3,   3.00543684e2, -9.94773230,
         1.70276630/1e1, -1.43034468/1e3, 4.73886084/1e6
    },
};


/*******************************************************************************
 热电偶温度转电压，电压转温度函数数组
*******************************************************************************/
/**
 * \brief j型热电偶 温度转热电偶电压
 */
am_err_t __j_temperature_to_v (double             temperature,
                               double            *p_v )
{
    uint8_t  i     = 0;
    double    value = 0;
    if ((temperature < -210) || (temperature > 1200.0)){
        return AM_ERROR;
    } else if (temperature > -210 && temperature < 760) {
        value = __g_var_t_to_v_j[0][8];
        for (i = 8; i > 0; i--) {
            value = temperature * value + __g_var_t_to_v_j[0][i-1];
        }
    } else {
        value = __g_var_t_to_v_j[0][5];
        for(i = 5; i > 0; i--) {
            value = temperature * value + __g_var_t_to_v_j[1][i-1];
        }
    }
    *p_v = value / 1000.0;
     return AM_OK;
}

/**
 * \brief j型热电偶 电压转温度
 */
am_err_t __j_v_to_temperature (double              v,
                               double             *p_temperature)
{
    uint8_t  i     = 0;
    double   value = 0;

    if (v >= -8.105 && v < 0) {
        value = __g_var_v_to_t_j[0][8];
        for (i = 8; i > 0; i--) {
            value = v * value + __g_var_v_to_t_j[0][i-1];
        }
    } else if (v >= 0 && v < 42.919) {
        value = __g_var_v_to_t_j[1][7];
        for (i = 7; i > 0; i--) {
            value = v * value + __g_var_v_to_t_j[1][i-1];
        }
    } else if (v >= 42.919 && v <= 69.553) {
        value = __g_var_v_to_t_j[2][5];
        for (i = 5; i > 0; i--) {
            value = v * value + __g_var_v_to_t_j[2][i-1];
        }
    }
    *p_temperature = value ;
     return AM_OK;
}

/**
 * \brief r型热电偶 温度转热电偶电压(冷端温度为0)
 */
am_err_t __r_temperature_to_v (double             temperature,
                               double            *p_v )
{
    uint8_t   i     = 0;
    double    value = 0;
    if ((temperature < -50) && (temperature > 1760.0)){
        return AM_ERROR;
    } else if (temperature > -50 && temperature < 1064.18) {
        value = __g_var_t_to_v_r[0][9];
        for (i = 9; i > 0; i--) {
            value = temperature * value + __g_var_t_to_v_r[0][i-1];
        }
    } else if (temperature > 1064.18 && temperature < 1664.5){
        value = __g_var_t_to_v_r[1][5];
        for(i = 5; i > 0; i--) {
            value = temperature * value + __g_var_t_to_v_r[1][i-1];
        }
    } else {
        value = __g_var_t_to_v_r[2][4];
        for(i = 4; i > 0; i--) {
            value = temperature * value + __g_var_t_to_v_r[2][i-1];
        }
    }

    *p_v = value / 1000.0;
     return AM_OK;
}

/**
 * \brief R型热电偶 电压转温度(冷端温度为0)
 */
am_err_t __r_v_to_temperature (double              v,
                               double             *p_temperature)
{
    uint8_t  i     = 0;
    double   value = 0;

    if (v >= -0.24 && v < 1.923) {
        value = __g_var_v_to_t_r[0][10];
        for (i = 10; i > 0; i--) {
            value = v * value + __g_var_v_to_t_r[0][i-1];
        }
    } else if (v >= 1.923 && v < 13.228) {
        value = __g_var_v_to_t_r[1][9];
        for (i = 9; i > 0; i--) {
            value = v * value + __g_var_v_to_t_r[1][i-1];
        }
    } else if (v >= 11.361 && v < 19.739) {
        value = __g_var_v_to_t_r[2][5];
        for (i = 5; i > 0; i--) {
            value = v * value + __g_var_v_to_t_r[2][i-1];
        }
    }else {
        value = __g_var_v_to_t_r[2][5];
        for (i = 4; i > 0; i--) {
            value = v * value + __g_var_v_to_t_r[3][i-1];
        }
    }
    *p_temperature = value * 1000000.0;
     return AM_OK;
}


/**
 * \brief N型热电偶 温度转热电偶电压(冷端温度为0)
 */
am_err_t __n_temperature_to_v (double             temperature,
                               double            *p_v )
{
    uint8_t   i     = 0;
    double    value = 0;
    if ((temperature < -270) && (temperature > 1300.0)){
        return AM_ERROR;
    } else if (temperature > -270 && temperature < 0) {
        value = __g_var_t_to_v_n[0][8];
        for (i = 8; i > 0; i--) {
            value = temperature * value + __g_var_t_to_v_n[0][i-1];
        }
    } else {
        value = __g_var_t_to_v_n[1][10];
        for(i = 10; i > 0; i--) {
            value = temperature * value + __g_var_t_to_v_n[1][i-1];
        }
    }

    *p_v = value / 1000000.0;
     return AM_OK;
}

/**
 * \brief N型热电偶 电压转温度(冷端温度为0)
 */
am_err_t __n_v_to_temperature (double              v,
                               double              *p_temperature)
{
    uint8_t  i     = 0;
    double   value = 0;

    if (v >= -4.365 && v < 0) {
        value = __g_var_v_to_t_n[0][9];
        for (i = 9; i > 0; i--) {
            value = v * value + __g_var_v_to_t_n[0][i-1];
        }
    } else if (v >= 0 && v < 20.613) {
        value = __g_var_v_to_t_n[1][7];
        for (i = 7; i > 0; i--) {
            value = v * value + __g_var_v_to_t_n[1][i-1];
        }
    } else if (v >= 20.613 && v < 47.533) {
        value = __g_var_v_to_t_n[2][5];
        for (i = 5; i > 0; i--) {
            value = v * value + __g_var_v_to_t_n[2][i-1];
        }
    }
    *p_temperature = value;
     return AM_OK;
}

/**
 * \brief T型热电偶 温度转热电偶电压(冷端温度为0)
 */
am_err_t __t_temperature_to_v (double             temperature,
                               double            *p_v )
{
    uint8_t   i     = 0;
    double    value = 0;
    if ((temperature < -270) && (temperature > 400.0)){
        return AM_ERROR;
    } else if (temperature > -270 && temperature < 0) {
        value = __g_var_t_to_v_t[0][14];
        for (i = 14; i > 0; i--) {
            value = temperature * value + __g_var_t_to_v_t[0][i-1];
        }
    } else {
        value = __g_var_t_to_v_t[1][8];
        for(i = 8; i > 0; i--) {
            value = temperature * value + __g_var_t_to_v_t[1][i-1];
        }
    }

    *p_v = value / 1000.0;
     return AM_OK;
}

/**
 * \brief T型热电偶 电压转温度(冷端温度为0)
 */
am_err_t __t_v_to_temperature (double              v,
                             double             *p_temperature)
{
    uint8_t  i     = 0;
    double   value = 0;

    if (v >= -6.268 && v < 0) {
        value = __g_var_v_to_t_t[0][7];
        for (i = 7; i > 0; i--) {
            value = v * value + __g_var_v_to_t_t[0][i-1];
        }
    } else if (v >= 0 && v < 20.882) {
        value = __g_var_v_to_t_t[1][6];
        for (i = 6; i > 0; i--) {
            value = v * value + __g_var_v_to_t_t[1][i-1];
        }
    }
    *p_temperature = value;
     return AM_OK;
}

/**
 * \brief K型热电偶 温度转热电偶电压(冷端温度为0)
 */
am_err_t __k_temperature_to_v (double             temperature,
                               double            *p_v )
{
    uint8_t   i     = 0;
    double    value = 0;
    if ((temperature < -270) && (temperature > 1370.0)){
        return AM_ERROR;
    } else if (temperature > -270 && temperature < 0) {
        value = __g_var_t_to_v_k[0][10];
        for (i = 10; i > 0; i--)
            value = temperature * value + __g_var_t_to_v_k[0][i-1];
        value  += __g_var_t_to_v_k[2][0]
                * exp(__g_var_t_to_v_k[2][1]
                * (temperature - 126.9686)
                * (temperature - 126.9686));
    } else {
        value = __g_var_t_to_v_k[1][9];
        for (i = 9; i > 0; i--)
            value = temperature * value + __g_var_t_to_v_k[1][i-1];
            value  += __g_var_t_to_v_k[2][0]
                * exp(__g_var_t_to_v_k[2][1]
                * (temperature - 126.9686)
                * (temperature - 126.9686));
    }
    *p_v = value / 1000.0;
     return AM_OK;
}

/**
 * \brief K型热电偶 电压转温度(冷端温度为0)
 */
am_err_t __k_v_to_temperature (double              v,
                               double             *p_temperature)
{
    uint8_t  i     = 0;
    double   value = 0;

    if (v >= -6.478 && v < 0) {
        value = __g_var_v_to_t_k[0][8];
        for (i = 8; i > 0; i--) {
            value = v * value + __g_var_v_to_t_k[0][i-1];
        }
    } else if (v >= 0 && v < 20.644) {
        value = __g_var_v_to_t_k[1][9];
        for (i = 9; i > 0; i--) {
            value = v * value + __g_var_v_to_t_k[1][i-1];
        }
    } else if (v >= 20.644 && v <= 54.900){
        value = __g_var_v_to_t_k[2][6];
        for (i = 6; i > 0; i--) {
            value = v * value + __g_var_v_to_t_k[2][i-1];
        }
    }
    *p_temperature = value;
     return AM_OK;
}

/**< \brief 热电偶电压值转温度函数数组 */
static pf__n_v_to_temperature_t __v_to_temperature[10];
/**< \brief 热电偶温度转电压值函数数组 */
static pf__n_temperature_to_v_t __temperature_to_v[10];
/*
 * 初始化热电偶转化算法
 */
void am_thermocouplie_init(void)
{
    /* 把热电偶的电压转温度添加到指针数组中 */
    __v_to_temperature[AM_THERMOCOUPLIE_J] = __j_v_to_temperature;
    __v_to_temperature[AM_THERMOCOUPLIE_K] = __k_v_to_temperature;
    __v_to_temperature[AM_THERMOCOUPLIE_T] = __t_v_to_temperature;
    //    __g_v_to_temperature[AM_THERMOCOUPLIE_N] = __n_v_to_temperature;
    //    __g_v_to_temperature[AM_THERMOCOUPLIE_R] = __r_v_to_temperature;

    /* 把热电偶的温度转电压添加到指针数组中 */
    __temperature_to_v[AM_THERMOCOUPLIE_J] = __j_temperature_to_v;
    __temperature_to_v[AM_THERMOCOUPLIE_K] = __k_temperature_to_v;
    __temperature_to_v[AM_THERMOCOUPLIE_T] = __t_temperature_to_v;
    //    __g_temperature_to_v[AM_THERMOCOUPLIE_N] = __n_temperature_to_v;
    //    __g_temperature_to_v[AM_THERMOCOUPLIE_R] = __r_temperature_to_v;
}


void am_thermocouplie_v2t(uint8_t            type,
                          double             voltage,
                          double            *p_temperature)
{
    __v_to_temperature[type](voltage, p_temperature);
}

void am_thermocouplie_t2v(uint8_t            type,
                          double             temperature,
                          double            *p_voltage )
{
    __temperature_to_v[type](temperature, p_voltage);
}
