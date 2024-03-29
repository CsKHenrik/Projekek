#ifndef RTCC_HEADER
#define	RTCC_HEADER

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//Napt�r strukt�ra
typedef struct _RTCTime     
{
    uint8_t year;     // 0x00-0x99
    uint8_t month;    // 0x01-0x12
    uint8_t day;      // 0x01-0x31
    uint8_t weekday;  // 0x00-0x06 H�tf� 0, Vas�rnap 6
    uint8_t hour;     // 0x00-0x23
    uint8_t minute;   // 0x00-0x59
    uint8_t second;   // 0x00-0x59
} RTCTime;

//Alarm maszkol�sa
#define RTCC_MASK_EVERY_HALF_SECOND  0  // f�l m�sodpercenk�nt
#define RTCC_MASK_EVERY_SECOND       1  // m�sodpercenk�nt
#define RTCC_MASK_EVERY_10_SECOND    2  // 10 m�sodpercenk�nt
#define RTCC_MASK_EVERY_MINUTE       3  // percenk�nt
#define RTCC_MASK_EVERY_10_MINUTE    4  // 10 percenk�nt 
#define RTCC_MASK_EVERY_HOUR         5  // �r�nk�nt
#define RTCC_MASK_EVERY_DAY          6  // naponta
#define RTCC_MASK_EVERY_WEEK         7  // hetente
#define RTCC_MASK_EVERY_MONTH        8  // havonta
#define RTCC_MASK_EVERY_YEAR         9  // �vente

    
//Napt�r modul be�ll�t�sa
void setRTCTime(RTCTime ido);
//Id� lek�rdez�se
RTCTime getRTCTime();
//Alarm be�ll�t�sa
void setAlarm(RTCTime ido, uint8_t mask);


#endif	/* RTCC_HEADER */

