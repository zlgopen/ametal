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
 * \brief NXP缂備緡鍨甸褔宕归鍛窞闁哄稁鍓﹂崯搴☆渻閻熸澘鑸瑰┑顔规櫊閹啴宕熼锟介。鏌ユ煛閸繍妲风紒韬插劤缁瑧锟斤綆浜滃В鎰版煛娴ｈ绶查柛娆忔瀹曪綁鏁愰崪鍐煑闂佸搫瀚搁幏锟�
 * \sa    demo_nxp_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-08-28  adw, first implementation
 * \endinternal
 */

#ifndef __DEMO_NXP_ENTRIES_H
#define __DEMO_NXP_ENTRIES_H

#include "am_clk.h"
#include "am_timer.h"
#include "am_adc.h"
#include "am_lpc_sct.h"

#include "hw/amhw_lpc_wwdt.h"
#include "hw/amhw_lpc_crc.h"
#include "hw/amhw_lpc_i2c.h"
#include "hw/amhw_lpc_sct.h"
#include "hw/amhw_lpc_usart.h"
#include "hw/amhw_lpc_spi.h"
#include "hw/amhw_lpc_wkt.h"
#include "hw/amhw_lpc_mrt.h"
#include "hw/amhw_lpc_fmc.h"

#include "hw/amhw_lpc82x_adc.h"
#include "hw/amhw_lpc82x_acmp.h"
//#include "hw/amhw_lpc82x_fmc.h"
#include "hw/amhw_lpc82x_gpio.h"
#include "hw/amhw_lpc82x_pmu.h"
#include "hw/amhw_lpc84x_gpio.h"
#include "hw/amhw_lpc84x_dac.h"
#include "hw/amhw_lpc84x_adc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief WWDT 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍕归敐鍫熺《闁轰降鍊栫粋宥夊幢閺団�冲箑闂佸搫鍋嗛崣蹇曟濮橆厾鐟归悗锝庡厴閺屻倝鏌涜箛瀣姎鐟滃府鎷�
 *
 * \param[in] p_hw_wwdt : 闂佸湱顭堝ú銈夊箖閿燂拷 WWDT 婵犮垼鍩栭悧鐐侯敊閺囩媴鎷烽棃娑橆仼闁宦板姂瀹曟娊濡搁妷锕�浠撮梺姹囧妼鐎氼厾锟藉灚宀搁弻锕傛晸閿燂拷
 * \param[in] inum      : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剙鐭楅柨鐕傛嫹
 * \param[in] wdt_freq  : WWDT婵☆偆澧楅崹鐟邦啅閿燂拷
 *
 * \note 婵炴垶鎼幏鐑芥煠閸撴彃澧柛搴＄箻瀹曟﹢宕楅崫銉ф啴闂佹寧绋戦懟顖氾耿椤忓懏濯撮悹鎭掑妽閺嗗繘鏌￠崼顒佸 demo 闂佸憡鎸哥粙鍕濠靛洦浜ゆ慨妤嬫嫹婵炴挸鐖奸獮宥夊箻瀹曞洨锛� WWDT 濠碘槅鍨敓鍊熸珪閸嬨儵鏌ｉ埡鍐剧劸缂併劍鐓￠幆鍐礋椤掍緡妲梻浣烘櫕閸ｏ妇妲愰敓锟�
 * 闂備焦婢樼粔鍫曟偪閸℃稑瑙﹂柛顐犲劵閹风兘宕掑鍕殸WDT闂佸搫鍟悥濂稿箰閹捐揪绱ｉ柟瀵稿Т閼烘娊鏌曢崱顓熷
 */
void demo_lpc_hw_wwdt_entry (amhw_lpc_wwdt_t *p_hw_wwdt,
                             int              inum,
                             uint32_t         wdt_freq);

/**
 * \brief acmp 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍕归敐鍫熺《闁轰降鍊栫粋宥夊幢閺団�冲箑闂佸搫鍋嗛崣蹇曟濮橆厾鐟归悗锝庡厴閺屻倝鏌涜箛瀣姎鐟滃府鎷�
 *
 * \param[in] p_hw_acmp : 闂佸湱顭堝ú銈夊箖閿燂拷 acmp 婵犮垼鍩栭悧鐐侯敊閺囩媴鎷烽棃娑橆仼闁宦板姂瀹曟娊濡搁妷锕�浠撮梺姹囧妼鐎氼厾锟藉灚宀搁弻锕傛晸閿燂拷
 * \param[in] flags     : acmp闂備焦婢樼粔鍫曟偪閿燂拷
 * \param[in] inum      : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剙鐭楅柨鐕傛嫹
 * \param[in] pin       : 濠殿噯绲惧Λ浣烘濠靛洦缍囬柟鎯у暱濮ｅ锟芥鍠楀ú婵嬪礈閿燂拷
 *
 * \note 婵炴垶鎼幏鐑芥煠閸撴彃澧柛搴＄箻瀹曟﹢宕楅崫銉ф啴闂佹寧绋戦懟顖氾耿椤忓懏濯撮悹鎭掑妽閺嗗繘鏌￠崼顒佸 demo 闂佸憡鎸哥粙鍕濠靛洦浜ゆ慨妤嬫嫹婵炴挸鐖奸獮宥夊箻瀹曞洨锛� WWDT 濠碘槅鍨敓鍊熸珪閸嬨儵鏌ｉ埡鍐剧劸缂併劍鐓￠幆鍐礋椤掍緡妲梻浣烘櫕閸ｏ妇妲愰敓锟�
 * 闂備焦婢樼粔鍫曟偪閸℃稑瑙﹂柛顐犲劵閹风兘宕掑鍕殸WDT闂佸搫鍟悥濂稿箰閹捐揪绱ｉ柟瀵稿Т閼烘娊鏌曢崱顓熷
 */
void demo_lpc824_hw_acmp_int_entry (amhw_lpc82x_acmp_t *p_hw_acmp, 
                                    uint32_t            flags, 
                                    int                 inum, 
                                    int                 pin);

/**
 * \brief acmp 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍕煟閵忋倖娑ч悽顖氭喘濮婂ジ鎳滃▓鎸庢祰闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_acmp : 闂佸湱顭堝ú銈夊箖閿燂拷 acmp 婵犮垼鍩栭悧鐐侯敊閺囩媴鎷烽棃娑橆仼闁宦板姂瀹曟娊濡搁妷锕�浠撮梺姹囧妼鐎氼厾锟藉灚宀搁弻锕傛晸閿燂拷
 * \param[in] pin       : 闁哄鐗婇幐鎼佸吹椤撶倫鎺楀籍閸モ晝鐛╅悗娈垮枟濞叉繈宕滈敓锟�
 */
void demo_lpc824_hw_acmp_lad_entry (amhw_lpc82x_acmp_t *p_hw_acmp, 
                                    int                 pin);
                             
/**
 * \brief acmp 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍕级閻戝棗鏋︽い鏇燁殜閺佸秵寰勬径灞肩矗缂備礁顑呴鍛村矗閸℃稑鐭楅柨鐕傛嫹
 *
 * \param[in] p_hw_acmp : 闂佸湱顭堝ú銈夊箖閿燂拷 acmp 婵犮垼鍩栭悧鐐侯敊閺囩媴鎷烽棃娑橆仼闁宦板姂瀹曟娊濡搁妷锕�浠撮梺姹囧妼鐎氼厾锟藉灚宀搁弻锕傛晸閿燂拷
 * \param[in] p_acmp    : 濠碘槅鍨崜婵堬拷姘懃琚欓柡鍐ㄦ川缁愭盯鎮楅棃娑橆仼闁宦板姂瀹曟娊濡搁妷锕�浠�
 */
void demo_lpc824_hw_acmp_poll_entry (amhw_lpc82x_acmp_t *p_hw_acmp,
                                     int                 pin);

/**
 * \brief adc 闂佸搫绉村ú銈夊闯閻ユ吀c闂佹寧绋戝鎭梐闂佸搫鍊婚幊鎾舵閿熺姵鏅璺侯槺娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 *
 * \param[in] adc_handle  : adc 闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] chan        : adc 闂備緡鍋呭畝鍛婄閿燂拷
 * \param[in] vref_mv     : adc 闂佸憡鐟ラ崐浠嬫晸閽樺鏆熼柡浣规倐瀹曘垽鏁撻敓锟�
 */
void demo_lpc824_hw_adc_dma_entry (amhw_lpc82x_adc_t *p_hw_adc,
                                   int                ch,
                                   uint32_t           vref_mv);

/**
 * \brief adc 闂佸搫绉村ú銈夊闯閻ユ吀c闂佹寧绋戝鎭梐闂佸搫鍊婚幊鎾舵閿熺姵鏅璺侯槺娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 *
 * \param[in] adc_handle  : adc 闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] inum        : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剙鐭楅柨鐕傛嫹
 * \param[in] vref_mv     : adc 闂佸憡鐟ラ崐浠嬫晸閽樺鏆熼柡浣规倐瀹曘垽鏁撻敓锟�
 */
void demo_lpc824_hw_adc_thcmp_entry (amhw_lpc82x_adc_t  *p_hw_adc, 
                                     int                 inum,
                                     uint32_t            vref_mv);

/**
 * \brief acmp 闂佸搫绉村ú銈夊闯閻ユ吀c闂佹寧绋戝鎭梐 int闂佸搫鍊婚幊鎾舵閿熺姵鏅璺侯槺娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 *
 * \param[in] adc_handle  : adc 闂佸憡鐟ｉ崕鏌ユ倿閿燂拷
 * \param[in] chan        : adc 闂備緡鍋呭畝鍛婄閿燂拷
 * \param[in] rate        : adc 闂備焦褰冨ú锕傛偋闁秵鐒婚柣鏂挎惈閼猴拷
 */
void demo_lpc_std_adc_dma_int_entry (am_adc_handle_t adc_handle,
                                     int             chan,
                                     uint32_t        rate);

/**
 * \brief acmp 闂佸搫绉村ú銈夊闯閻ユ吀c闂佹寧绋戝鎭梐 poll闂佸搫鍊婚幊鎾舵閿熺姵鏅璺侯槺娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 *
 * \param[in] adc_handle  : adc闂佸憡鐟ｉ崕鏌ユ倿閿燂拷
 * \param[in] chan        : adc 闂備緡鍋呭畝鍛婄閿燂拷
 * \param[in] rate        : adc 闂備焦褰冨ú锕傛偋闁秵鐒婚柣鏂挎惈閼猴拷
 */
void demo_lpc_std_adc_dma_poll_entry (am_adc_handle_t adc_handle,
                                      int             chan,
                                      uint32_t        rate);

/**
 * \brief acmp 闂佸搫绉村ú銈夊闯閻ユ吀c闂佹寧绋戝鐒爐闂佸搫鍊婚幊鎾舵閿熺姵鏅璺侯槺娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 *
 * \param[in] adc_handle  : adc闂佸憡鐟ｉ崕鏌ユ倿閿燂拷
 * \param[in] chan        : adc 闂備緡鍋呭畝鍛婄閿燂拷
 * \param[in] rate        : adc 闂備焦褰冨ú锕傛偋闁秵鐒婚柣鏂挎惈閼猴拷
 */
void demo_lpc_std_adc_int_entry (am_adc_handle_t adc_handle,
                                 int             chan,
                                 uint32_t        rate);

/**
 * \brief acmp 闂佸搫绉村ú銈夊闯閻ユ吀c闂佹寧绋戝绌檒l 闂佸搫鍊婚幊鎾舵閿熺姵鏅璺侯槺娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 *
 * \param[in] adc_handle  : adc闂佸憡鐟ｉ崕鏌ユ倿閿燂拷
 * \param[in] ch          : 闂備緡鍋呭畝鍛婄閿燂拷
 * \param[in] rate        : 闂備緡鍋嗛崰鎾愁啅閿燂拷
 */
void demo_lpc_std_adc_poll_entry (am_adc_handle_t handle, 
                                  int             ch, 
                                  uint32_t        rate);


/**
 * \brief acmp 缂佺虎鍙庨崗娑樷枎閵忣澀娌柨鐕傛嫹 crc婵炴挻鑹鹃鍥煝婵傜绀傞柕澶堝劚缂嶏拷
 *
 * \param[in] p_hw_crc  : crc 闂佸憡鐟ｉ崕鏌ユ倿閿燂拷
 * \param[in] p_data    : 濠电偞娼欓鍫ユ儊椤栫偛鏋侀柣妤�鐗嗙粊锟�
 * \param[in] rate      : 濠电偞娼欓鍫ユ儊椤栫偛鏋侀柣妤�鐗嗙粊锕傛⒒閿熶粙宕归纰卞敽
 */
void demo_lpc_hw_crc_entry (amhw_lpc_crc_t  *p_hw_crc,
                            am_const char   *p_data, 
                            uint32_t         nbytes);


/**
 * \brief dma 缂佺虎鍙庨崗娑樷枎閵忋垺鍠嗛柨鏇嫹鐟滄澘鍊圭粭鐔猴拷锝庡厴閺屻倝鏌涜箛瀣姎鐟滃府鎷�
 *
 * \param[in] chan : dma 闂備緡鍋呭畝鍛婄閿燂拷
 * \param[in] pin  : 闁荤喐鐟辩粻鎴ｃ亹閸屾凹鍤曢柡鍥ｏ拷鍐差棝
 */
void demo_lpc824_drv_dma_hwtrigger_burst_entry (uint8_t  chan,
                                                int      pin);


/**
 * \brief dma 闂佸憡鍔曢幊搴ㄦ偤閵娾晛绀嗛柡澶婄仢閺佸爼鎮楀☉娅偐娆㈤妷褏鐭欓悗锝庝簻瀵娊鏌涘▎娆愬
 *
 * \param[in] p_src : 濠电偞娼欓鍫ユ儊椤栫偛鏋侀柣妤�鐗嗙粊锔界節瑜庨崝鏇烇耿閹绢喖閿ら柨鐕傛嫹
 * \param[in] len   : 濠电偞娼欓鍫ユ儊椤栫偛鏋侀柣妤�鐗嗙粊锕傛煟閵娿儱顏柡鍡氶哺閹棃鏁撻敓锟�
 */
void demo_lpc824_drv_dma_m2m_entry (uint8_t *p_src,
                                    int      len);



/**
 * \brief DMA ping-pong 婵炴挻鑹鹃鍥煝婵傜绀傞柕澶堝劚缂嶏拷
 *
 * \param[in] chan  : DMA 闂備緡鍋呭畝鍛婄閿燂拷
 * \param[in] p_src : 濠电偞娼欓鍫ユ儊椤栫偛鏋侀柣妤�鐗嗙粊锔界節瑜庨崝鏇烇耿閹绢喖閿ら柨鐕傛嫹
 * \param[in] len   : 濠电偞娼欓鍫ユ儊椤栫偛鏋侀柣妤�鐗嗙粊锕傛煟閵娿儱顏柡鍡氶哺閹棃鏁撻敓锟�
 */
void demo_lpc824_drv_dma_ping_pong_entry (uint8_t  chan, 
                                          uint8_t *p_src, 
                                          int      len);


/**
 * \brief flash 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛灞剧懅娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 *
 * \param[in] chan  : DMA 闂備緡鍋呭畝鍛婄閿燂拷
 * \param[in] p_src : 濠电偞娼欓鍫ユ儊椤栫偛鏋侀柣妤�鐗嗙粊锔界節瑜庨崝鏇烇耿閹绢喖閿ら柨鐕傛嫹
 * \param[in] len   : 濠电偞娼欓鍫ユ儊椤栫偛鏋侀柣妤�鐗嗙粊锕傛煟閵娿儱顏柡鍡氶哺閹棃鏁撻敓锟�
 */
void demo_lpc824_hw_flash_ctrl_entry (amhw_lpc_fmc_time_t time);


/**
 * \brief gpio 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛灞剧懅娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 *
 * \param[in] p_hw_gpio  : GPIO 闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑銉╂煙缁嬫寧鍠橀柟鍑ゆ嫹
 * \param[in] pin        : 閻庢鍠楀ú婵嬪礈閿燂拷
 */
void demo_lpc824_hw_gpio_entry (amhw_lpc82x_gpio_t *p_hw_gpio, 
                                int                 pin);

/**
 * \brief gpio 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛灞剧懆閸橆剟鏌￠崒銈呭箳缂佽翰鍎崇划娆戯拷锝庝簻瀵娊鏌涘▎娆愬
 *
 * \param[in] key_pin  : 闂佸湱顭堥ˇ鐢稿极椤撶儐鍤曢柡鍥ｏ拷鍐差棝
 * \param[in] led_pin  : led閻庢鍠楀ú婵嬪礈閿燂拷
 * \param[in] pint_sel : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剚鐒绘慨妯绘构婢规洟鏌涘▎娆愬
 * \param[in] inum     : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剙鐭楅柨鐕傛嫹
 */
void demo_lpc824_hw_gpio_int_entry (int                 key_pin,
                                    int                 led_pin,
                                    int                 pint_sel,
                                    int                 inum);

/**
 * \brief gpio 闂佸憡鐗曠紞濠囧储閵堝憘鐔煎灳瀹曞洨顢呮繛鎾磋壘椤戝洭鍩㈡總绋跨闁靛鍎辩紞锟�
 *
 * \param[in] pin  : led閻庢鍠楀ú婵嬪礈閿燂拷
 */
void demo_lpc824_hw_gpio_pmatch_entry (int pin);

/**
 * \brief gpio 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍕煙缁嬫妯�闁轰緡鍙冮弫宥嗗緞婢跺奔绱ｇ紓浣割儏椤戝懘宕ｉ崱娑樼煑闁跨噦鎷�
 *
 * \param[in] pin  : 闂佸湱顭堥ˇ鐢稿极椤撶儐鍤曢柡鍥ｏ拷鍐差棝
 */
void demo_lpc_std_gpio_key_entry (int pin);

/**
 * \brief i2c master 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒a闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_i2c  : i2c闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] clkdiv    : 闂佸憡甯掑Λ婵嬨�傞崼鏇炵煑闁稿本绋掑▓锟�
 * \param[in] addr      : 闂侀潻绲婚崝瀣枎閵忋倕鎹堕柡澶嬪缁诧拷
 * \param[in] sub_addr  : 婵炲濮寸�涒晛鈹冮敓浠嬫煕閿旇姤绶叉繛纭锋嫹
 */
void demo_lpc824_hw_i2c_master_dma_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                          uint32_t        clkdiv,
                                          uint8_t         addr,
                                          uint8_t         sub_add);


/**
 * \brief i2c master 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒a闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_i2c  : i2c闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] clkdiv    : 闂佸憡甯掑Λ婵嬨�傞崼鏇炵煑闁稿本绋掑▓锟�
 * \param[in] addr      : 闂侀潻绲婚崝瀣枎閵忋倕鎹堕柡澶嬪缁诧拷
 */
void demo_lpc_hw_i2c_master_poll_entey (amhw_lpc_i2c_t *p_hw_i2c,
                                        uint32_t        clkdiv,
                                        uint8_t         addr);

/**
 * \brief i2c master 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒a闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_i2c : i2c闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] chan     : dma闂備緡鍋呭畝鍛婄妤ｅ啫鐭楅柨鐕傛嫹
 */
void demo_lpc824_hw_i2c_slave_dma_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                         uint8_t         chan);

/**
 * \brief i2c slave 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒琻t闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_i2c  : i2c闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] inum      : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剙鐭楅柨鐕傛嫹
 */
void demo_lpc_hw_i2c_slave_int_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                      int             inum);


/**
 * \brief i2c slave 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒緊ll闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_i2c  : i2c闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 */
void demo_lpc_hw_i2c_slave_poll_entry (amhw_lpc_i2c_t *p_hw_i2c);
/**
 * \brief iap 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛灞剧懅娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 */
void demo_lpc824_hw_iap_entry (void);

/**
 * \brief i2c master 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒a闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_mrt  : mrt闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] inum      : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剙鐭楅柨鐕傛嫹
 */
void demo_lpc824_hw_mrt_entry (amhw_lpc_mrt_t *p_hw_mrt, int inum);

/**
 * \brief pll 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛灞剧懅娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 */
void demo_lpc824_hw_pll_entry(void);

/**
 * \brief pmu 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒eppowerdown闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_mrt  : pmu闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 */
void demo_lpc824_hw_pmu_deeppowerdown_entry (amhw_lpc82x_pmu_t  *p_hw_pmu);

/**
 * \brief pmu 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒epsleep闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_mrt  : pmu闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] pin       : 閻庢鍠楀ú婵嬪礈閿燂拷
 */
void demo_lpc824_hw_pmu_deepsleep_entry (amhw_lpc82x_pmu_t  *p_hw_pmu,
                                         int                 pin);

/**
 * \brief pmu 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒緊werdown闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_mrt  : pmu闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] pin       : 閻庢鍠楀ú婵嬪礈閿燂拷
 */
void demo_lpc824_hw_pmu_powerdown_entry (amhw_lpc82x_pmu_t  *p_hw_pmu,
                                         int                 pin);

/**
 * \brief pmu 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍓唋eep闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_mrt  : pmu闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] pin       : 閻庢鍠楀ú婵嬪礈閿燂拷
 */
void demo_lpc824_hw_pmu_sleep_entry (amhw_lpc82x_pmu_t  *p_hw_pmu,
                                     int                 pin);

/**
 * \brief sct 婵＄偟鎳撳畷顒佹叏閳哄啩娌柛宀嬪缁�鍒緒m闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] sct_handle  : sct闂佸搫鐗嗙粔瀛樻叏閻旂厧绠肩�广儱瀚粙濠囨煕濞嗗秴鍔滈柣鎺炴嫹
 */
void demo_lpc_drv_sct_timing_pwm_entry (am_lpc_sct_handle_t sct_handle);

/**
 * \brief sct 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒緒m闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_sct  : sct闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] frq       : 缂備緡鍨靛畷鐢靛垝濞差亜绫嶉柛顐ｆ礃鐎殿厼螞閻楀牆鐏╃�归潻鎷�
 */
void demo_lpc_hw_sct_1_32bit_pwm_entry (amhw_lpc_sct_t *p_hw_sct, 
                                           uint32_t        frq);

/**
 * \brief sct 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍓噄ming_32bit闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_sct  : sct闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] inum      : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剙鐭楅柨鐕傛嫹
 * \param[in] frq       : 缂備緡鍨靛畷鐢靛垝濞差亜绫嶉柛顐ｆ礃鐎殿厼螞閻楀牆鐏╃�归潻鎷�
 */
void demo_lpc_hw_sct_1_32bit_timing_entry (amhw_lpc_sct_t *p_hw_sct,
                                           int             inum,
                                           uint32_t        frq);


/**
 * \brief sct 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍓噄ming_16bit闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_sct  : sct闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] inum      : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剙鐭楅柨鐕傛嫹
 * \param[in] frq       : 缂備緡鍨靛畷鐢靛垝濞差亜绫嶉柛顐ｆ礃鐎殿厼螞閻楀牆鐏╃�归潻鎷�
 * \param[in] pin       : 闂佺偓鑹鹃崐璺ㄧ矉韫囨稑闂柕濞垮劤缁屽潡鏌ゆ潏鐐
 */
void demo_lpc_hw_sct_2_16bit_timing_entry (amhw_lpc_sct_t *p_hw_sct, 
                                           int             inum,
                                           uint32_t        frq,
                                           int             pin);


/**
 * \brief sct 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒緒m_16bit闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_sct  : sct闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] frq       : 缂備緡鍨靛畷鐢靛垝濞差亜绫嶉柛顐ｆ礃鐎殿厼螞閻楀牆鐏╃�归潻鎷�
 */
void demo_lpc_hw_sct_2_16bit_pwm_entry(amhw_lpc_sct_t *p_hw_sct,
                                       int             frq);

/**
 * \brief sct 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒緒m_16bit闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_sct  : sct闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] inum      : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剙鐭楅柨鐕傛嫹
 * \param[in] frq       : 缂備緡鍨靛畷鐢靛垝濞差亜绫嶉柛顐ｆ礃鐎殿厼螞閻楀牆鐏╃�归潻鎷�
 */
void demo_lpc_hw_sct_cap_entry (amhw_lpc_sct_t *p_hw_sct, 
                                int             inum, 
                                uint32_t        frq);

/**
 * \brief sct 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒緒m_16bit闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_sct  : sct闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] frq       : 缂備緡鍨靛畷鐢靛垝濞差亜绫嶉柛顐ｆ礃鐎殿厼螞閻楀牆鐏╃�归潻鎷�
 */
void demo_lpc_hw_sct_multi_states_entry(amhw_lpc_sct_t *p_hw_sct,
                                        uint32_t        frq);
/**
 * \brief bod 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍕煙閸濆嫷妲洪柡浣侯焾闇夐悗锝庡幘濡叉悂鏌ㄥ☉姗嗘Х缂佽翰鍎崇划娆戯拷锝庝簻瀵娊鏌涘▎娆愬
 */
void demo_lpc824_hw_bod_entry(void);


/**
 * \brief clk 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍕煛閸愩劎鍩ｉ柟鍛婃尰閿涙劙骞嬮悩鑼跺У闁哄鐗婇幐鎼佸吹椤撱垺鏅璺侯槺娴兼劗绱掔�ｎ亶鍎忛柛娆忔瀹曪綁鏁撻敓锟�
 */
void demo_lpc824_hw_clkout_entry (void);

/**
 * \brief usart 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍕煠婵傚绨诲┑顔规櫅閳绘棃濮�閵忕媭妫楅梺缁樼矊濞村嫮妲愬顓犵懝閻庯綆鍏橀弻銈夋煕韫囧鍔氱憸甯嫹
 *
 * \param[in] p_hw_usart  : usart闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] u_clk       : 婵炴垶鎸昏ぐ鍐亹濞戙垹鏄ラ柣鎴炆戦幏杈╋拷瑙勬偠閸庢壆绱為弮锟介敍鎰板箣閻樿尪濮�
 */
void demo_lpc824_hw_usart_autobaud_entry(amhw_lpc_usart_t *p_hw_usart,
                                         uint32_t          u_clk);

/**
 * \brief usart 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒緊ll闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_usart  : usart闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] u_clk       : 婵炴垶鎸昏ぐ鍐亹濞戙垹鏄ラ柣鎴炆戦幏杈╋拷瑙勬偠閸庢壆绱為弮锟介敍鎰板箣閻樿尪濮�
 * \param[in] baudrate    : 濠电偛顦崹缁樼珶閹烘鍋濋柨鐕傛嫹
 */
void demo_lpc_hw_usart_poll_entry(amhw_lpc_usart_t *p_hw_usart,
                                  uint32_t          u_clk,
                                  uint32_t          baudrate);

/**
 * \brief usart 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍓厁 dma闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_usart  : usart闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] u_clk       : 婵炴垶鎸昏ぐ鍐亹濞戙垹鏄ラ柣鎴炆戦幏杈╋拷瑙勬偠閸庢壆绱為弮锟介敍鎰板箣閻樿尪濮�
 * \param[in] baudrate    : 濠电偛顦崹缁樼珶閹烘鍋濋柨鐕傛嫹
 * \param[in] chan        : 婵犮垼鍩栭悧鐐侯敊閺勭幀a闁荤姴娲弨閬嶆儑娴煎瓨鐒绘慨妯绘构婢癸拷
 */
void demo_lpc824_hw_usart_rx_dma_entry (amhw_lpc_usart_t *p_hw_usart,
                                        uint32_t          uclk,
                                        uint32_t          baudrate,
                                        int               chan);
/**
 * \brief usart 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍓噚 dma闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_usart  : usart闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] u_clk       : 婵炴垶鎸昏ぐ鍐亹濞戙垹鏄ラ柣鎴炆戦幏杈╋拷瑙勬偠閸庢壆绱為弮锟介敍鎰板箣閻樿尪濮�
 * \param[in] baudrate    : 濠电偛顦崹缁樼珶閹烘鍋濋柨鐕傛嫹
 * \param[in] chan        : 婵犮垼鍩栭悧鐐侯敊閺勭幀a闁荤姴娲弨閬嶆儑娴煎瓨鐒绘慨妯绘构婢癸拷
 */
void demo_lpc824_hw_usart_tx_dma_entry (amhw_lpc_usart_t *p_hw_usart,
                                        uint32_t          uclk,
                                        uint32_t          baudrate,
                                        int               chan);

/**
 * \brief usart 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒緊ll闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
* \param[in] uart_handle  : 闂佸搫鐗嗙粔瀛樻叏閻旂厧鐭楅柕澶涘閸旓拷
 */
void demo_lpc_std_usart_flowctrl_entry (am_uart_handle_t  uart_handle);

/**
 * \brief wkt 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒eppowerdown-wakeup闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_pmu  : pmu闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] p_hw_wkt  : wkt闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] inum      : 婵炴垶鎼╅崢浠嬪蓟閸ヮ剙鐭楅柨鐕傛嫹
 * \param[in] delay_inms: 閻庣偣鍊涢崺鏍ь渻閸岀偛绫嶉柛顐ｆ礃閿涚喖鏌ㄥ☉妯煎闁轰焦绮撻弻瀣箳閻愮數顦�
 */
void demo_lpc824_hw_wkt_deeppowerdown_wakeup_entry(amhw_lpc82x_pmu_t *p_hw_pmu,
                                                   amhw_lpc_wkt_t    *p_hw_wkt,
                                                   int                inum,
                                                   uint32_t           delay_inms);
/**
 * \brief wkt 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒eppowerdown-wakeup闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_wkt   : wkt闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] wkt_mdelay : 閻庣偣鍊涢崺鏍ь渻閸岀偛绫嶉柛顐ｆ礃閿涳拷
 */
void demo_lpc_hw_wkt_timer(amhw_lpc_wkt_t *p_hw_wkt,
                           uint32_t        wkt_mdelay);

/**
 * \brief wkt 闂佸搫绉村ú銈夊闯椤栨粈娌柛宀嬪缁�鍓噄mer闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] timer_handle   : 闂佺懓鐏濈粔宕囩礊閺冨牆鐭楅柕澶涘閸旓拷
 * \param[in] chan           : 闂備緡鍋呭畝鍛婄閿燂拷
 * \param[in] clkin          : 闁诲氦顫夌喊宥咁渻閸岀偛闂柕濠忛檮椤ρ囨⒑閺傘倖瀚�
 */
void demo_lpc_std_wkt_timer_entry (am_timer_handle_t timer_handle,
                                   uint8_t           chan,
                                   uint32_t          clkin);

/**
 * \brief spi 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍒碼ster闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_spi  : spi闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] clk       : 闂佸搫鍟悥濂稿箰閿燂拷
 */
void demo_lpc_hw_spi_master_entry(amhw_lpc_spi_t *p_hw_spi,
                                  uint32_t        clk);
/**
 * \brief spi 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍓唋ave闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_spi  : spi闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] clk       : 闂佸搫鍟悥濂稿箰閿燂拷
 */
void demo_lpc_hw_spi_slave_entry (amhw_lpc_spi_t *p_hw_spi,
                                  uint32_t        clk);

/**
 * \brief spi 缂佺虎鍙庨崗娑樷枎閵忣澀娌柛宀嬪缁�鍓唋ave闂佹寧绋戦ˇ顔炬閵壯呯煓閻庯綆浜滃鎶芥煕濞嗘瑦瀚�
 *
 * \param[in] p_hw_spi  : spi闁诲酣娼х�氼剟鎮洪妸鈺侀棷闁靛鍎查崑锟�
 * \param[in] chan      : dma闂備緡鍋呭畝鍛婄閿燂拷
 */
void demo_lpc824_hw_spi_slave_dma_entry(amhw_lpc_spi_t *p_hw_spi, 
                                        uint8_t         chan);

/**
 * \brief gpio 闂傚偆浜為崕濠囧箳瑜嶉崺姊漞d濞撴艾顑囬埢濂稿礂閵夈儱缍�
 *
 * \param[in] p_hw_gpio  : gpio閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] pin        : 缂佺妳銈呭闁告瑱鎷�
 */
void demo_lpc845_hw_gpio_entry (amhw_lpc84x_gpio_t *p_hw_gpio,
                                int                 pin);
/**
 * \brief gpio pmatch濞撴艾顑囬埢濂稿礂閵夈儱缍�
 *
 * \param[in] pin : 缂佺妳銈呭闁告瑱鎷�
 */
void demo_lpc845_hw_gpio_pmatch_entry (int pin);

/**
 * \brief gpio 濞戞搩鍘介弻鍥ㄧ瑹鐎ｎ兘鏌ら柛蹇嬪劚瑜帮拷
 *
 * \param[in] key_pin   : 闁圭顦甸弫顓犵不闄囬崜濂稿矗閿燂拷
 * \param[in] led_pin   : led鐎殿喗娲濋崜濂稿矗閿燂拷
 * \param[in] pint_sel  : 濞戞搩鍘介弻鍥棘閻熸壆纭�闂侇偄顦扮�氾拷
 * \param[in] inum      : 濞戞搩鍘介弻鍥矗閿燂拷
 */
void demo_lpc845_hw_gpio_int_entry(int    key_pin,
                                   int    led_pin,
                                   int    pint_sel,
                                   int    inum);

/**
 * \brief MRT 濞撴艾顑囬埢濂稿礂閵夈儱缍�
 *
 * \param[in] p_hw_mrt  : mrt閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] inum      : 濞戞搩鍘介弻鍥矗閿燂拷
 */
void demo_lpc845_hw_mrt_entry (amhw_lpc_mrt_t *p_hw_mrt, int inum);

/**
 * \brief 缂侇垵宕电划鍝勎涢敓钘壝圭�ｎ厽绁伴柡澶婄仢閵堜粙宕搁敓锟� 濞撴艾顑囬埢濂稿礂閵夈儱缍�
 */
void demo_lpc845_hw_bod_entry (void);

/**
 * \brief 闁哄啫鐖奸幐鎾存綇閹惧啿姣夊〒姘儑閳诲ジ宕楅妷銉ョ稉
 * \param[in] 闁哄喛鎷�
 */
void demo_lpc845_hw_clkout_entry (void);

/**
 * \brief I2C闂侇偅淇虹换鍍丮A閻庡湱鍋熼獮鍥ㄧ鎼淬垺绨氬〒姘儑閳诲ジ宕楅妷銉ョ稉
 *
 * \param[in] p_hw_i2c  : i2c閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] chan      : DMA闂侇偅宀告禍楣冨矗閿燂拷
 */
void demo_lpc845_hw_i2c_slave_dma_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                         uint8_t         chan);

/**
 * \brief 婵烇絽宕�规娊骞掓径灞炬毄婵☆垪锟藉磭纭�闁哥偑鍊濋崯瀣瑹鐎ｎ兘鏌ら柛蹇嬪劚瑜帮拷
 *
 * \param[in] p_hw_pmu  : pmu閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] p_hw_wkt  : WKT閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] inum      : 濞戞搩鍘介弻鍥矗閿燂拷
 * \param[in] delay_inms闁挎冻鎷� 闁哥偑鍊濋崯瀣籍閸洘锛�
 */
void demo_lpc845_hw_wkt_deeppowerdown_wakeup_entry(amhw_lpc82x_pmu_t *p_hw_pmu,
                                                   amhw_lpc_wkt_t    *p_hw_wkt,
                                                   int                inum,
                                                   uint32_t           delay_inms);
/**
 * \brief flash闁硅矇鍐ㄧ厬闁革絻鍔嬬欢銉х矙鐎ｎ亜寮抽柛娆欐嫹
 *
 * \param[in] time  : 闂傚偆浜滈悺銊ф媼閸ф锛栭柡鍐ㄧ埣濡潡鏌婂鍥╂瀭
 */
void demo_lpc845_hw_flash_ctrl_entry(amhw_lpc_fmc_time_t time);

/**
 * \brief DAC 閺夊牊鎸搁崵顓烆潰閿濆懘娴繛澶堝灩閹风増绋夋径搴健婵炲婢�缁躲儳绮欓敓锟�
 *
 * \param[in] p_hw_dac  : dac閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] inum      : 濞戞搩鍘介弻鍥矗閿燂拷
 */
void demo_lpc_hw_dac_buf_int_entry (amhw_lpc_dac_t *p_hw_dac,
                                    int             inum);
/**
 * \brief ADC 濞撴艾顑囬埢濂稿礂閵夈儱缍�
 *
 * \param[in] p_hw_adc  : adc閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] ch        : 闂侇偅宀告禍楣冨矗閿燂拷
 * \param[in] vref_mv   : 闁糕晛鎼崳顖炴偨闂堟稑绔�
 */
void demo_lpc845_hw_adc_dma_entry(amhw_lpc84x_adc_t *p_hw_adc,
                                  int                ch,
                                  uint32_t        vref_mv);
/**
 * \brief ADC 濞撴艾顑囬埢濂稿礂閵夈儱缍�
 *
 * \param[in] p_hw_adc  : adc閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] ch        : 闂侇偅宀告禍楣冨矗閿燂拷
 * \param[in] vref_mv   : 闁糕晛鎼崳顖炴偨闂堟稑绔�
 */
void demo_lpc845_hw_adc_thcmp_entry(amhw_lpc84x_adc_t  *p_hw_adc,
                                    int                 inum,
                                    uint32_t            vref_mv);


void demo_lpc845_drv_dma_hwtrigger_burst_entry (uint8_t  chan,
                                                int      pin);

/**
 * \brief PMU 闁瑰搫顦遍弫绋课熼垾宕囩闁告帗绻傞～鎰板礌閿燂拷
 */
void demo_lpc845_hw_pmu_powerdown_entry (amhw_lpc82x_pmu_t  *p_hw_pmu,
                                         int                 pin);

/**
 * \brief ADC 濞撴艾顑囬埢濂稿礂閵夈儱缍�
 *
 * \param[in] p_src  : adc閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] len        : 闂侇偅宀告禍楣冨矗閿燂拷
 */
void demo_lpc845_drv_dma_m2m_entry(uint8_t *p_src, int len);

/**
 * \brief DMA 濞磋偐濮剧欢顓㈡晬鐏炶偐顔婂☉鏃�鎸昏啯鐎殿噯鎷�
 */
void demo_lpc845_drv_dma_ping_pong_entry (uint8_t  chan,
                                          uint8_t *p_src,
                                          int      len);
/**
 * \brief ADC 濞撴艾顑囬埢濂稿礂閵夈儱缍�
 *
 * \param[in] p_hw_acmp  : adc閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] pin        : 闂侇偅宀告禍楣冨矗閿燂拷
 */
void demo_lpc845_hw_acmp_poll_entry(amhw_lpc82x_acmp_t *p_hw_acmp,
                                    int                 pin);

/**
 * \brief ADC 濞撴艾顑囬埢濂稿礂閵夈儱缍�
 *
 * \param[in] p_hw_i2c  : adc閻庨潧瀚悺銊╁闯閵娿儲鍋�
 * \param[in] clkdiv    : 闂侇偅宀告禍楣冨矗閿燂拷
 * \param[in] addr      : 闂侇偅宀告禍楣冨矗閿燂拷
 * \param[in] sub_addr  : 闂侇偅宀告禍楣冨矗閿燂拷
 */
void demo_lpc845_hw_i2c_master_dma_entry (amhw_lpc_i2c_t *p_hw_i2c,
                                          uint32_t        clkdiv,
                                          uint8_t         addr,
                                          uint8_t         sub_addr);

/**
 * \brief PMU 深度睡眠模式 初始化
 */
void demo_lpc845_hw_pmu_deepsleep_entry (amhw_lpc82x_pmu_t  *p_hw_pmu,
                                         int                 pin);

/**
 * \brief USART 闁告帗绻傞～鎰板礌閿燂拷
 */
void demo_lpc845_hw_usart_rx_dma_entry (amhw_lpc_usart_t *p_hw_usart,
                                        uint32_t          uclk,
                                        uint32_t          baudrate,
                                        int               chan);

/**
 * \brief PMU 睡眠模式 初始化
 */
void demo_lpc845_hw_pmu_sleep_entry (amhw_lpc82x_pmu_t  *p_hw_pmu,
                                     int                 pin);

/**
 * \brief USART 闁告帗绻傞～鎰板礌閿燂拷
 */
void demo_lpc845_hw_usart_tx_dma_entry (amhw_lpc_usart_t *p_hw_usart,
                                        uint32_t          uclk,
                                        uint32_t          baudrate,
                                        int               chan);

/**
 * \brief iap 例程入口
 */
void demo_lpc845_hw_iap_entry (void);
#ifdef __cplusplus
}
#endif

#endif /* __DEMO_NXP_ENTRIES_H */

/* end of file */
