#ifndef _DAI_TONE_H_
#define _DAI_TONE_H_

#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

#include "Xu_schedule.h"

extern MD_MAX72XX mx;

extern uint8_t mode_flag;
enum
{
    MICROSOUND = 0,
    FREE
};

//   记得pinMode(tonepin,OUTPUT);
/* ---------------------------------- 调用方式 ---------------------------------- */
void tone_shang_chun_shan(void);
void tone_da_yu(void);
void tone_yuan_yu_chou(void);
void Dai_tone_init(void);
void displaySoundLevel(int level);

#endif //_DAI_TONE_H_