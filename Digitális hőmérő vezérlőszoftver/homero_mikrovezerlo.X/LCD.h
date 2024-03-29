#ifndef LCD_H
#define	LCD_H

#include <xc.h>
#include "system.h"

// LCD lábak
#define LCD_DATA    LATE // bit 0..7
#define LCD_RS      _LATG0
#define LCD_RW      _LATG1
#define LCD_E       _LATF1
#define LCD_BF      _RE7

// LCD makrók
#define lcdPutChar(c) lcdWrite(c, 1)    // karakter küldése
#define lcdPutCmd(d)  lcdWrite(d, 0)    // utasítás küldése
#define lcdClear()    lcdWrite(0x01,0)  // LCD törlése
#define lcdGoHome()   lcdWrite(0x02,0)  // LCD küldése az 1. sorba
#define lcdGo2Row()   lcdWrite(0xC0,0)  // LCD küldése a 2. sorba

void lcdInit();
void lcdWrite(uint8_t c, uint8_t rs);
void lcdPutStr(char *s);
void lcdLoadHuChars(void);


#endif	/* LCD_H */

