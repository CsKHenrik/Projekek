#include "xc.h"
#include "RTCC.h"

//Napt�r modul be�ll�t�sa
void setRTCTime(RTCTime ido)
{
    // Orakvarc engedelyezese
    __builtin_write_OSCCONL(OSCCON | 0x02);
    
    //A modul �rhat�s�g�nak az enged�lyez�se
    __builtin_write_RTCWEN();    //feloldjuk az �r�sv�delmet
    RCFGCALbits.RTCEN = 0;       //kikapcsoljuk a modult

    // Be�ll�tjuk az id?t
    RCFGCALbits.RTCPTR = 3;                  //r��llunk az �vre
    RTCVAL = ido.year;                       //�v (#0x00YY)
    RTCVAL = (ido.month << 8) + ido.day;     //h�nap �s nap (#0xMMDD)
    RTCVAL = (ido.weekday << 8) + ido.hour;  //h�tk�znap �s �ra (#0x0WHH)
    RTCVAL = (ido.minute << 8) + ido.second; //perc �s m�sodperc (#0xMMSS)

    RCFGCALbits.RTCEN = 1;      //bekapcsoljuk a modult
    RCFGCALbits.RTCWREN = 0;    //vissza�ll�tjuk az �r�sv�delmet
}

//Id? lek�rdez�se
RTCTime getRTCTime()
{
    RTCTime ido;
    while(RCFGCALbits.RTCSYNC);  //v�rakoz�s az RTCSYNC 0-ra

    RCFGCALbits.RTCPTR=3;           //r��llunk az �vre
    ido.year = RTCVAL;              //�v kiolvas�sa

    uint16_t month_date=RTCVAL; //h�nap �s nap
    ido.month = month_date >> 8;
    ido.day = month_date & 0xFF;

    uint16_t wday_hour=RTCVAL;  //h�tk�znap �s �ra
    ido.weekday = wday_hour >> 8;
    ido.hour = wday_hour & 0xFF;

    uint16_t min_sec=RTCVAL;    //perc �s m�sodperc
    ido.minute = min_sec >> 8;
    ido.second = min_sec & 0xFF;

    return ido;
}

//Alarm be�ll�t�sa
void setAlarm(RTCTime ido, uint8_t mask)
{
    ALCFGRPTbits.ALRMEN=0;       //kikapcsoljuk a modult

    // Be�ll�tjuk az id?t
    ALCFGRPTbits.ALRMPTR = 3;	  //r��llunk az �vre
    ALRMVAL = ido.year; 			      //�v (#0x00YY)
    ALRMVAL = (ido.month << 8) + ido.day;      //h�nap �s nap (#0xMMDD)
    ALRMVAL = (ido.weekday << 8) + ido.hour;   //h�tk�znap �s �ra (#0x0WHH)
    ALRMVAL = (ido.minute << 8) + ido.second;  //perc �s m�sodperc (#0xMMSS)
    
    ALCFGRPTbits.AMASK=mask;    //�sszehasonl�t�s maszkol�sa
    
    ALCFGRPTbits.CHIME=1;       //v�gtelen ism�tl�s
    ALCFGRPTbits.ALRMEN=1;      //bekapcsoljuk a modult
}
