#include <xc.h>
#include "LCD.h"

//magyar ékezetes karakterek
const unsigned char hu_char[] = {
0x02,0x04,0x0E,0x01,0x0F,0x11,0x0F,0x00, //á
0x02,0x04,0x0E,0x11,0x1F,0x10,0x0E,0x00, //é
0x02,0x04,0x0C,0x04,0x04,0x04,0x0E,0x00, //í
0x02,0x04,0x0E,0x11,0x11,0x11,0x0E,0x00, //ó
0x02,0x04,0x11,0x11,0x11,0x13,0x0D,0x00, //ú
0x0A,0x00,0x11,0x11,0x11,0x13,0x0D,0x00, //ü
0x05,0x0A,0x11,0x11,0x11,0x13,0x0D,0x00, //û
0x05,0x0A,0x0E,0x11,0x11,0x11,0x0E,0x00, //õ
};

/**
 * <b>LCD inicializálása</b><br>
 * lábak beállítása<br>
 * indítás 8 bites üzemmódban<br>
 */
void lcdInit(){
    
    // az LCD vezérlõ vonalainak beállítása
    TRISE &= 0xff00; // D0-D7 kimenet
    _TRISG0 = 0;     // RS kimenet
    _TRISG1 = 0;     // RW kimenet
    _TRISF1 = 0;     // E kimenet
    LCD_RS = 0;      // RS alacsony
    LCD_RW = 0;      // RW alacsony
    LCD_E = 0;       // E alacsony
    
    DELAY_MS(50);    // vár az eszköz beállására
    
    lcdPutCmd(0x38); // 2 soros display, 5x8 karakter
    lcdPutCmd(0x08); // display off
    lcdPutCmd(0x01); // képernyõ törlése, kurzor alaphelyzetbe állítás
    lcdPutCmd(0x06); // automatikus inkrementálás, nem lépteti a kijelzõt
    lcdPutCmd(0x0C); // a display bekapcsolása; kurzor és villogás kikapcsolva
    
    DELAY_MS(3);
}


/**
 * adat vagy utasítás küldése
 * @param c adat/karakter
 * @param rs 0 - utasítás, 1 - adat
 */
void lcdWrite(uint8_t c, uint8_t rs){
    
    uint8_t BF;      // BusyFlag (foglaltság) figyelése
    TRISE |= 0x00ff; // lábak bemenetek
    LCD_RW = 1;      // beolvasás
    LCD_RS = 0;      // utasítás
    LCD_DATA &= 0xff00;
    do {
        LCD_E = 1;   // E magas
        Nop(); Nop(); Nop(); BF = LCD_BF;
        LCD_E = 0;   // E alacsony
    } while (BF);
    
    TRISE &= 0xff00; // lábak kimenetek
    LCD_RW = 0;      // írás
    LCD_RS = rs ? 1 : 0;      // adat vagy parancs
    LCD_DATA = (LCD_DATA & 0xff00) | c;    // 8 bit küldése
    LCD_E = 1;       // E magas
    Nop(); Nop(); Nop();
    LCD_E = 0;       // E alacsony
}


/**
 * kiír egy karakterfüzért az LCD-re
 * @param s Karaktertömb
 */
void lcdPutStr(char *s){
  while (*s) {
    char c = *s;
    switch(c) {           // magyar karakterek cseréje a feltöltöttre
      case 'á':  c = 0x00; break;
      case 'é':  c = 0x01; break;
      case 'í':  c = 0x02; break;
      case 'ó':  c = 0x03; break;
      case 'ú':  c = 0x04; break;
      case 'ü':  c = 0x05; break;
      case 'û':  c = 0x06; break;
      case 'õ':  c = 0x07; break;
      case 'ö':  c = 0xEF; break;
    }
    if (c == '\n')
      lcdGo2Row();        // kurzor mozgatása a második sor elejére
    else
      lcdPutChar(c);      // karakter kiíratása
    s++;
  }
}


// magyar ékezetes karakterek feltöltése a CGRAM-ba
void lcdLoadHuChars(void) {
    int i;
    lcdPutCmd(0x40);             // kurzor a CGRAM elejére (0. char)
    for(i=0; i<64; i++) {
      lcdPutChar(hu_char[i]);    // definiálható karakterek feltöltése
    }                            // ékezetes karakterekkel
    lcdPutCmd(0x80);             // kurzor vissza, a DDRAM elejére
}