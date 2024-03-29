#ifndef RTCC_HEADER
#define	RTCC_HEADER

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//Naptár struktúra
typedef struct _RTCTime     
{
    uint8_t year;     // 0x00-0x99
    uint8_t month;    // 0x01-0x12
    uint8_t day;      // 0x01-0x31
    uint8_t weekday;  // 0x00-0x06 Hétfõ 0, Vasárnap 6
    uint8_t hour;     // 0x00-0x23
    uint8_t minute;   // 0x00-0x59
    uint8_t second;   // 0x00-0x59
} RTCTime;

//Alarm maszkolása
#define RTCC_MASK_EVERY_HALF_SECOND  0  // fél másodpercenként
#define RTCC_MASK_EVERY_SECOND       1  // másodpercenként
#define RTCC_MASK_EVERY_10_SECOND    2  // 10 másodpercenként
#define RTCC_MASK_EVERY_MINUTE       3  // percenként
#define RTCC_MASK_EVERY_10_MINUTE    4  // 10 percenként 
#define RTCC_MASK_EVERY_HOUR         5  // óránként
#define RTCC_MASK_EVERY_DAY          6  // naponta
#define RTCC_MASK_EVERY_WEEK         7  // hetente
#define RTCC_MASK_EVERY_MONTH        8  // havonta
#define RTCC_MASK_EVERY_YEAR         9  // évente

    
//Naptár modul beállítása
void setRTCTime(RTCTime ido);
//Idõ lekérdezése
RTCTime getRTCTime();
//Alarm beállítása
void setAlarm(RTCTime ido, uint8_t mask);


#endif	/* RTCC_HEADER */

