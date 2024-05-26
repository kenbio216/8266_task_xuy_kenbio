#ifndef _DAI_TONE_H_
#define _DAI_TONE_H_

#include <Arduino.h>

/* --------------------------------- 每一曲的长度 --------------------------------- */

const int tonepin = 12;

//   记得pinMode(tonepin,OUTPUT);
/* ---------------------------------- 调用方式 ---------------------------------- */
void tone_shang_chun_shan(void);
void tone_da_yu(void);
void tone_yuan_yu_chou(void);

#endif _DAI_TONE_H_