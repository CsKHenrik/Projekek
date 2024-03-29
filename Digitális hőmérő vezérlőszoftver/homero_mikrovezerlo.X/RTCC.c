#include "xc.h"
#include "RTCC.h"

//Naptár modul beállítása
void setRTCTime(RTCTime ido)
{
    // Orakvarc engedelyezese
    __builtin_write_OSCCONL(OSCCON | 0x02);
    
    //A modul írhatóságának az engedélyezése
    __builtin_write_RTCWEN();    //feloldjuk az írásvédelmet
    RCFGCALbits.RTCEN = 0;       //kikapcsoljuk a modult

    // Beállítjuk az id?t
    RCFGCALbits.RTCPTR = 3;                  //ráállunk az évre
    RTCVAL = ido.year;                       //év (#0x00YY)
    RTCVAL = (ido.month << 8) + ido.day;     //hónap és nap (#0xMMDD)
    RTCVAL = (ido.weekday << 8) + ido.hour;  //hétköznap és óra (#0x0WHH)
    RTCVAL = (ido.minute << 8) + ido.second; //perc és másodperc (#0xMMSS)

    RCFGCALbits.RTCEN = 1;      //bekapcsoljuk a modult
    RCFGCALbits.RTCWREN = 0;    //visszaállítjuk az írásvédelmet
}

//Id? lekérdezése
RTCTime getRTCTime()
{
    RTCTime ido;
    while(RCFGCALbits.RTCSYNC);  //várakozás az RTCSYNC 0-ra

    RCFGCALbits.RTCPTR=3;           //ráállunk az évre
    ido.year = RTCVAL;              //év kiolvasása

    uint16_t month_date=RTCVAL; //hónap és nap
    ido.month = month_date >> 8;
    ido.day = month_date & 0xFF;

    uint16_t wday_hour=RTCVAL;  //hétköznap és óra
    ido.weekday = wday_hour >> 8;
    ido.hour = wday_hour & 0xFF;

    uint16_t min_sec=RTCVAL;    //perc és másodperc
    ido.minute = min_sec >> 8;
    ido.second = min_sec & 0xFF;

    return ido;
}

//Alarm beállítása
void setAlarm(RTCTime ido, uint8_t mask)
{
    ALCFGRPTbits.ALRMEN=0;       //kikapcsoljuk a modult

    // Beállítjuk az id?t
    ALCFGRPTbits.ALRMPTR = 3;	  //ráállunk az évre
    ALRMVAL = ido.year; 			      //év (#0x00YY)
    ALRMVAL = (ido.month << 8) + ido.day;      //hónap és nap (#0xMMDD)
    ALRMVAL = (ido.weekday << 8) + ido.hour;   //hétköznap és óra (#0x0WHH)
    ALRMVAL = (ido.minute << 8) + ido.second;  //perc és másodperc (#0xMMSS)
    
    ALCFGRPTbits.AMASK=mask;    //összehasonlítás maszkolása
    
    ALCFGRPTbits.CHIME=1;       //végtelen ismétlés
    ALCFGRPTbits.ALRMEN=1;      //bekapcsoljuk a modult
}
