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
 * \brief 8段数字型数码管ASCII字形解码
 *
 * \internal
 * \par modification history:
 * - 1.00 13-02-25  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_digitron_disp.h"

/** \brief 8 segment ASCII code segment data */
struct seg8_ascii_code {
    char    ascii_char; /**< \brief the ASCII char */
    uint8_t seg_data;   /**< \brief the segment data of the ASCII char */
};

/******************************************************************************/

/** \brief default 8 segment ASCII code segment data table */

static const struct seg8_ascii_code __g_seg8_ascii_code[] = {
    {' ', 0x00},

    {'0', 0xFC}, {'1', 0x60}, {'2', 0xDA}, {'3', 0xF2}, {'4', 0x66},
    {'5', 0xB6}, {'6', 0xBE}, {'7', 0xE0}, {'8', 0xFE}, {'9', 0xF6},

    {'A', 0xEE}, {'B', 0x3E}, {'C', 0x9C}, {'D', 0x7A}, {'E', 0x9E},
    {'F', 0x8E}, {'G', 0xBC}, {'H', 0x6E}, {'I', 0x08}, {'J', 0x78},
    {'K', 0x5E}, {'L', 0x1C}, {'M', 0xAA}, {'N', 0x2A}, {'O', 0x3A},
    {'P', 0xCE}, {'Q', 0xE6}, {'R', 0x0A}, {'S', 0xB6}, {'T', 0x1E},
    {'U', 0x38}, {'V', 0x7E}, {'W', 0x56}, {'X', 0x6C}, {'Y', 0x76},
    {'Z', 0x92},

    {'h', 0x2E}, {'c', 0x1A}, {'.', 0x01},

    {'+', 0x0E}, {'-', 0x02}, {'^', 0xC4}, {'*', 0x28}, {'/', 0x90}
};


/******************************************************************************/

/**
 * \brief decode ASCII char to segment data of a 8 segment digitron
 * \param ascii_char the ASCII char to decode
 * \return the segment data of <ascii_char>, If can't decode, return blank data
 */
uint16_t am_digitron_seg8_ascii_decode (uint16_t ascii_char)
{
    int i;

    for (i = 0; i < AM_NELEMENTS(__g_seg8_ascii_code); i++) {
        if (__g_seg8_ascii_code[i].ascii_char == ascii_char) {
            return __g_seg8_ascii_code[i].seg_data;
        }
    }
    return __g_seg8_ascii_code[0].seg_data;
}

/* end of file */
