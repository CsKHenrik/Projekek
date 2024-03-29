/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

/** INCLUDES *******************************************************/
#include <xc.h>
#include "system.h"

#include "usb_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_cdc.h"

//#include "RTCC.h"
#include "LCD.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

//A kommnunikaciora hasznalt lab definialasa
#define CommIrany _TRISD11
#define VoltageOut _LATD11
#define VoltageIn _RD11

#define _kimenet 0
#define _bemenet 1

//kommunikacio idozitesehez szukseges parameterek
#define ReadRelease 4       //olvasas kezdetet jelzo lehuzas hossza
#define ReadSampleDelay 11  //olvasaskor a lehuzas kezdete es a jel olvasasa kozti ido
#define SlotLength 70       //egy darab iro/olvaso szakasz hossza
#define Write1Release 8     //1-es irasakor az alacsony feszültsegszint idotartama
#define PostSlotDelay 10    //iro/olvaso szakasz utani varakozas hossza

/*********************************************************************
* Function: void APP_DeviceCDCBasicDemoInitialize(void);
*
* Overview: Initializes the code
*
* PreCondition: None
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_CDCInitialize(void);

/*********************************************************************
* Function: void APP_DeviceCDCBasicDemoTasks(void);
*
* Overview: Keeps the demo running.
*
* PreCondition: The demo should have been initialized and started via
*   the APP_DeviceCDCBasicDemoInitialize()
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_CDCTasks(void);

/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/



/** VARIABLES ******************************************************/

//USB iro/olvaso bufferei
static uint8_t readBuffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t writeBuffer[CDC_DATA_IN_EP_SIZE];

//LCD-re kiirando string
char LCD[80];


int PresenceOK = 0;             //inicializacio teljesuleset jelzo valtozo
int SlotOK = 0;                 //egy darab iro/olvaso szakasz veget jelzo valtozo
int SampleBit = 0;              //kiolvasott bit
int NegativCelsius;             //negativ homerseklet jelzesere hasznaljuk

uint64_t ROMCodes[2];           //kiolvasott ROM kodok

//ROM kod kiolvasas kozben kiolvasott bitek
uint64_t DataBit = 0;
uint64_t DataBitNeg = 0;

uint64_t ScratchpadContent = 0; //scratchpad kiolvasott tartalma
uint8_t ScratchpadCRC = 0;      //srcratchpad kiolvasott CRC kodja
uint8_t ShiftRegister = 0;      //CRC kod ellenorzesekor hasznalt shift register
uint8_t RotatingBit = 0;        //CRC kod ellenorzesekor hasznalt bit
double Temp[2];                 //kiolvasott homersekletertekek


//Timer 1 vege utan vegrehajtando feladatot azonosito enum valtozo
enum WaitStates {ResetPulse, Pullup, Release};
enum WaitStates WaitState = ResetPulse;

//kommunikacio allapotat azonosito enum valtozo
enum CommandStates {Idle, Initialization, ROMCommand, FunctionCommand};
enum CommandStates CommandState = Initialization;

//ROM parancsokat azonosito enum valtozo
enum ROMCommands {SkipROM, SearchROM, MatchROM};
enum ROMCommands ActualROMCommand = SearchROM;


//funkcioparancsokat azonosito enum valtozo
enum FunctionCommands {Convert, ReadScratchpad};
enum FunctionCommands ActualFunctionCommand = Convert;


//Timer1 beallítasa es elinditasa
void StartTimer1_us(uint32_t ido, enum WaitStates waitState){
    _T1IF = 0; // Interrupt flag törlése
    _T1IE = 1; // Interrupt engedélyezése
    PR1 = ido * 16 - 1; // Ido beállítása
    TMR1 = 0x0000; // Timer1 beállítása
    T1CONbits.TCKPS = 0; // T1 elõosztó 1:1
    WaitState = waitState; //az ido letelte utani feladat beallitasa
    T1CONbits.TON = 1; // T1 ON
}

//Timer1 beallítasa inditas nelkul
void SetTimer1_us(uint32_t ido, enum WaitStates waitState){
    _T1IF = 0; // Interrupt flag törlése
    _T1IE = 1; // Interrupt engedélyezése
    PR1 = ido * 16 - 1; // Idõ beállítása
    TMR1 = 0x0000; // Timer1 beállítása
    T1CONbits.TCKPS = 0; // T1 elõosztó 1:1
    WaitState = waitState; //az ido letelte utani feladat beallitasa
}

//Timer2 beallítasa inditas nelkul
void SetTimer2_us_SampleDelay(uint32_t ido){
    _T2IF = 0; // Interrupt flag törlése
    _T2IE = 1; // Interrupt engedélyezése
    PR2 = ido * 16 - 1; // Ido beállítása
    TMR2 = 0x0000; // Timer2 beállítása
    T2CONbits.TCKPS = 0; // T2 elõosztó 1:1
}

//Timer1 beallítasa inditas nelkul
void SetTimer3_us_ReadSlot(uint32_t ido){
    _T3IF = 0; // Interrupt flag törlése
    _T3IE = 1; // Interrupt engedélyezése
    PR3 = ido * 16 - 1; // Ido beállítása
    TMR3 = 0x0000; // Timer3 beállítása
    T3CONbits.TCKPS = 0; // T3 elõosztó 1:1
}

//
//Reset pulse kiadasa, Presence pulse megvarasa
//
void PresenceCheck()
{
    PresenceOK = 0; //incializacio nem tortent meg
    lcdClear();
    lcdPutStr("No Signal"); //segedinfomracio az LCD-n
    CommIrany = _bemenet;   //vezetek lebeg
    if(VoltageIn)           //ha a pullup resistor felhuzza
    {
        VoltageOut = 0;         //a kimenet alacsony feszultsegszint
        CommIrany = _kimenet;   //a vezetek nem lebeg, ekkor megjelenik rajta a kimenet
        StartTimer1_us(500, ResetPulse); //vezetek 0 logikai szinten tartasa a megadott ideig, tovabbi mukodes a timer1 interruptban
    }
    while (!PresenceOK) Nop();  //varakozas az incializacio teljesuleseig
}

//
//1-es erteku bit irasa
//
void SingleWireWriteZero()
{
    SlotOK = 0;                 //kommunikacios szakasznak nincs vege
    VoltageOut = 0;             //a kimenet alacsony feszultsegszint
    CommIrany = _kimenet;       //a vezetek nem lebeg, ekkor megjelenik rajta a kimenet
    T3CONbits.TON = 1;          //Timer3 inditasa, ez a kommunikacios szakasz veget fogja jelezni
    while (!SlotOK) Nop();      //amig a kommunikacios szakasz tart, varakozunk
    CommIrany = _bemenet;       //vezetek lebeg
    DELAY_US(PostSlotDelay);    //kommunikacios szakasz utani varakozas
}

//
//0-as erteku bit irasa
//
void SingleWireWriteOne()
{
    SlotOK = 0;                 //kommunikacios szakasznak nincs vege
    SetTimer1_us(Write1Release, Release);  //Timer1 beallitasa
    VoltageOut = 0;             //a kimenet alacsony feszultsegszint
    CommIrany = _kimenet;       //a vezetek nem lebeg, ekkor megjelenik rajta a kimenet
    T1CONbits.TON = 1;          //Timer1 inditasa, tovabbi mukodes a timer1 interruptban
    T3CONbits.TON = 1;          //Timer3 inditasa, ez a kommunikacios szakasz veget fogja jelezni
    while (!SlotOK) Nop();      //amig a kommunikacios szakasz tart, varakozunk
    DELAY_US(PostSlotDelay);    //kommunikacios szakasz utani varakozas
}

//
//bit olvasasa
//
uint8_t SingleWireReadBit()
{
    SlotOK = 0;                 //kommunikacios szakasznak nincs vege
    SetTimer1_us(ReadRelease, Release);   //Timer1 beallitasa
    VoltageOut = 0;             //a kimenet alacsony feszultsegszint
    CommIrany = _kimenet;       //a vezetek nem lebeg, ekkor megjelenik rajta a kimenet
    T1CONbits.TON = 1;          //Timer1 inditasa, tovabbi mukodes a timer1 interruptban
    T2CONbits.TON = 1;          //Timer2 inditasa, tovabbi mukodes a timer2 interruptban
    T3CONbits.TON = 1;          //Timer3 inditasa, ez a kommunikacios szakasz veget fogja jelezni
    while (!SlotOK) Nop();      //amig a kommunikacios szakasz tart, varakozunk
    DELAY_US(PostSlotDelay);    //kommunikacios szakasz utani varakozas
    return SampleBit;           //visszateres a kiolvasott ertekkel
}

//
//byte irasa
//
void SingeWireWriteByte(uint8_t command)
{
    int i = 0;
    for (i = 0; i < 8; i++)  //vegigmegyunk a 8 biten LSB-tol MSB-ig
    {
        //ha az aktualis bit 1, 1-et irunk
        if((command >> i) % 2)
            SingleWireWriteOne();
        //ha az aktualis bit 0, 0-at irunk
        else
            SingleWireWriteZero();
    }
}

//
//CRC kod ellenorzesehez hasznalt polinomgenerator egy lepese az adatlap alapjan
//
void PolinomGeneratorStep(uint8_t input)        //az input valtozonak a legkiosebb bitje lesz a polinomgenerator bemenete
{
    RotatingBit = (ShiftRegister ^ input) & 1;  //"beforgo" bit generalasa a shift register legkisebb bitjebol es a bemenetbol
    ShiftRegister >>= 1;                        //shift register eltolasa lefele

    //shift register megfelelo bitjeinek frissitese
    ShiftRegister |= RotatingBit << 7;
    ShiftRegister ^= RotatingBit << 3;
    ShiftRegister ^= RotatingBit << 2;
}

MAIN_RETURN main(void)
{
    //Orajel forras beallitasa
    // PLL kimenete
    CLKDIVbits.CPDIV = 0;    // 1:1 32MHz
    // megvarjuk amig a PLL modul vegez
    while(!OSCCONbits.LOCK) Nop();
	
    // szoftveres WDT kikapcsolasa
    RCONbits.SWDTEN = 0;
	
	// Orakvarc engedelyezese
	__builtin_write_OSCCONL(OSCCON | 0x02);

	// Periferia - lab osszerendeles PPS (pp.135)
	//__builtin_write_OSCCONL(OSCCON & 0xbf);  //PPSUnLock;
	//
	//__builtin_write_OSCCONL(OSCCON | 0x40); //PPSLock;
    
    // USB kapcsolat inicializálása
    SYSTEM_Initialize(SYSTEM_STATE_USB_START);
    USBDeviceInit();
    USBDeviceAttach();

    // Labak iranya	
	_TRISG6 = 0;	// LED1 kimenet
	_TRISG7 = 0;	// LED2 kimenet
	_TRISG8 = 0;	// LED3 kimenet
    _TRISG9 = 0;	// LED4 kimenet
    _TRISD15 = 0;	// LEDR kimenet
    _TRISF4 = 0;	// LEDG kimenet
    _TRISF5 = 0;	// LEDB kimenet
	_TRISC1 = 1;	// SW1 bemenet
	_TRISC3 = 1;	// SW2 bemenet 
	_TRISE8 = 1;	// SW3 bemenet
    _TRISE9 = 1;	// SW4 bemenet
    CommIrany = _bemenet; //vezetek lebeg
    
    
    lcdInit();                  //LCD modul inicializálása
    lcdLoadHuChars();           //Magyar karakterek betöltése
    
    //Timer2 beallitasa a kiolvasas kesleltetesenek meresere
    SetTimer2_us_SampleDelay(ReadSampleDelay);
    //Timer3 beallitasa a kommunikacios szakasz idejenek meresere
    SetTimer3_us_ReadSlot(SlotLength);
    //innentol csak inditani kell ezeket a timereket

    //allapotok kezdeti erteke, inditaskor ezekbe az allapotokba lep
    enum CommandStates CommandState = Initialization;   //kommunikacio inicializalasa
    enum ROMCommands ActualROMCommand = SearchROM;      //ROM kod azonositasa

    lcdPutStr("Starting");   //Bekapcsolast jelzo uzenet
    
    DELAY_MS(500);           //rovid varakozas

    while(1)
    {
        
        //Application specific tasks
        APP_CDCTasks();

    }//end while
}//end main

// ISR

//
//TIMER1 INTERRUPT
//
void __attribute__((interrupt(auto_psv))) _T1Interrupt()
{
    T1CONbits.TON = 0; // T1 Off
    
    //az inditaskor megjelolt feladat vegrehajtasa
    switch(WaitState)
    {
    case ResetPulse:
        CommIrany = _bemenet;       //vezetek lebeg
        StartTimer1_us(20, Pullup); //varakozas a pullup resistor altali felhuzasra
        break;
    case Pullup:
        while(VoltageIn) Nop();     //amig a homero nem huzta le a feszultseget, varunk
        if (!VoltageIn)             //ha a Presence pulse megerkezik
        {
            DELAY_US(480);          //kivarjuk a Presence pulse veget
            PresenceOK = 1;         //az inicializacio megtortent
        }
        break;
    case Release:
        CommIrany = _bemenet;       //vezetek lebeg
        break;
    }
    IFS0bits.T1IF = 0; // T1 interrupt flag törlése
}

//
//TIMER3 INTERRUPT - a jel kiolvasasat jelzi
//
void __attribute__((interrupt(auto_psv))) _T2Interrupt()
{    
    T2CONbits.TON = 0; // T2 Off
    SampleBit = VoltageIn;  //vezetek feszultsegszintjenek kiolvasasa

    IFS0bits.T2IF = 0; // T2 interrupt flag törlése
}

//
//TIMER3 INTERRUPT - kommunikacios szakasz veget jelzi
//
void __attribute__((interrupt(auto_psv))) _T3Interrupt()
{
    T3CONbits.TON = 0; // T3 Off
    SlotOK = 1;        //kommunikacios szakasz vege
    IFS0bits.T3IF = 0; // T3 interrupt flag törlése
}


/*********************************************************************
* Function: void APP_CDCInitialize(void);
*
* Overview: Initializes the code
*
* PreCondition: None
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_CDCInitialize()
{   
    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 9600;
}

/*********************************************************************
* Function: void APP_CDCTasks(void);
*
* Overview: Keeps the demo running.
*
* PreCondition: The demo should have been initialized and started via
*   the APP_CDCInitialize()
*
* Input: None
*
* Output: None
*
********************************************************************/
int i = 0;
void APP_CDCTasks()
{

    //
    //a kommunikacio lepeseit megvalosito allapotgep
    //
    switch(CommandState)
    {
        //idle allapot - varakozas az USB-n erkezo keresre
        case Idle:
            lcdGoHome();
            sprintf(LCD, "%016llx\n%.4lf C",ROMCodes[0],Temp[0]);
            lcdPutStr(LCD); //ellenorzes celjabol kiirjuk a kiolvasott ROM kodot es homersekletet
            DELAY_MS(10);
            
            if (SW1) //ellenorzes celjabol SW1 gombbal is megmerhetjuk a homersekletet
            {
                DELAY_MS(20);
                while (SW1) Nop();
                DELAY_MS(20);
                CommandState = Initialization;
                ActualROMCommand = SkipROM;
                ActualFunctionCommand = Convert;
            }
        
            break;

        //kommunikacio inicializalasa
        case Initialization:
            PresenceCheck();            //inicializalast vegrehajto függveny hivasa
            CommandState = ROMCommand;  //inicializalas utan mindig ROM parancs kovetkezik
            break;

        //ROM parancs vegrehajtasa
        case ROMCommand:
            switch (ActualROMCommand)
            {
                //ROM parancs atugrasa
                case SkipROM:
                    SingeWireWriteByte(0xcc);       //Skip ROM parancs kiirasa
                    CommandState = FunctionCommand; //lepes funkcioparancs allapotba
                    break;

                //ROM kod kiolvasasa
                case SearchROM:
                    SingeWireWriteByte(0xf0);       //Search ROM parancs kiirasa
                    
                    //tarolt ROM kod torlese
                    ROMCodes[0] = 0;
                    ROMCodes[1] = 0;
                    
                    //polinomgenerator valtozoinak torlese
                    ShiftRegister = 0;
                    RotatingBit = 0;
                    
                    int i = 0;
                    //64 bites ROM kod kiolvasasa
                    for (i = 0; i < 64; i++)
                    {
                        //ROM kod bitjeinek, es negaltjainak kiolvasasa
                        DataBit = SingleWireReadBit();
                        DataBitNeg = SingleWireReadBit();

                        //kiolvasott bit visszakuldese
                        if (DataBit) SingleWireWriteOne();
                        else SingleWireWriteZero();

                        ROMCodes[0] |= (DataBit << i);  //kiolvasott bit beirasa a tarolt ROM kodba

                        PolinomGeneratorStep(DataBit);  //kiolvasott bit beirasa a polinomgeneratorba
                    }

                    //ha az egesz adatsor betaplalasa utan 0 van a shift registerben, nem volt hiba az adatkuldesben, idle allapotba lephetunk
                    if(!ShiftRegister)
                        CommandState = Idle;

                    //ha hiba keletkezett a kommunikacio soran, megismeteljuk a ROM kod kiolvasasat
                    else
                    {
                        CommandState = Initialization;  //kommunikacio inicializalasa
                        ActualROMCommand = SearchROM;   //Search ROM parancs
                    }
                    break;

                //homero megszolitasa ROM kod alapjan
                case MatchROM:
                    SingeWireWriteByte(0x55);       //Match ROM parancs kiirasa
                    i = 0;
                    for (i = 0; i < 64; i++)        //64 bites ROM kod kiirasa
                    {
                        //ha az aktualis bit 1, 1-et irunk
                        if((ROMCodes[0] >> i) % 2)
                            SingleWireWriteOne();
                        //ha az aktualis bit 0, 0-at irunk
                        else
                            SingleWireWriteZero();
                    }
                    CommandState = FunctionCommand; //funkcioparancs allapotba lepes
                    break;
            }
            break;

        //Funkcioparancs vegrehajtasa
        case FunctionCommand:
            switch (ActualFunctionCommand)
            {
                //homerseklet konverzioja
                case Convert:
                    SingeWireWriteByte(0x44);                   //Convert parancs kiirasa
                    lcdClear();
                    lcdPutStr("Converting\ntemperature");       //segedinfomracio az LCD-n

                    //megvarjuk, a konverzio veget
                    while (!SingleWireReadBit())
                        DELAY_MS(20);

                    //allapotok frissitese
                    CommandState = Initialization;              //kommunikcaio inicializalasa
                    ActualROMCommand = MatchROM;                //homero megszolitasa ROM kod alapjan
                    ActualFunctionCommand = ReadScratchpad;     //homero memoriajanak kiolvasasa
                    break;

                //homero memoriajanak kiolvasasa
                case ReadScratchpad:
                    SingeWireWriteByte(0xbe);                   //Read Scrtchpad parancs kiirasa
                    ScratchpadContent = 0;                      //memoria tarolt tartalmanak torlese
                    NegativCelsius = 0;

                    //polinomgenerator valtozoinak torlese
                    ShiftRegister = 0;
                    RotatingBit = 0;

                    //segedinfomracio az LCD-n
                    lcdClear();
                    lcdPutStr("Reading data");

                    //64 bit kiolvasasa, eltarolasa es beirasa a polinomgeneratorba
                    for (i = 0; i < 64; i++)
                    {
                        DataBit = SingleWireReadBit();
                        ScratchpadContent |= (DataBit << i);
                        PolinomGeneratorStep((uint8_t)DataBit);
                    }

                    //a maradek 8 bit kiolvasasa, eltarolasa es beirasa a polinomgeneratorba
                    for (i = 0; i < 8; i++)
                    {
                        DataBit = SingleWireReadBit();
                        ScratchpadCRC |= (DataBit << i);
                        PolinomGeneratorStep((uint8_t)DataBit);
                    }

                    //ha hiba keletkezett a kommunikacio kozben inicializacio allapotba lepunk es ugyanez le fog futni
                    if (ShiftRegister)
                    {
                        CommandState = Initialization;
                        break;
                    }
                    //ha nem keletkezett hiba, tovabblepunk

                    //tarolt homerseklet torlese
                    Temp[0] = 0;

                    //ha a homerseklet negativ, 2-es komplemenst kepzunk a memoria tartalmabol
                    if ((ScratchpadContent >> 12) % 2)
                    {
                        NegativCelsius = 1;
                        ScratchpadContent = (~ScratchpadContent) + 1;
                    }

                    //a memoriabol kiolvassuk a mert homersekletet
                    for (i = 0; i < 11; i++)
                    {
                        if ((ScratchpadContent >> i) % 2)
                            Temp[0] += pow(2, i - 4);
                    }

                    //ha a homerseklet negativ volt, negaljuk a tarolt homersekletet
                    if (NegativCelsius)
                        Temp[0] = -Temp[0];

                    //a mert homerseklet kikuldese USB-n
                    sprintf(writeBuffer,"%.4lf\r\n",Temp[0]);
                    putUSBUSART(writeBuffer,strlen((char*)writeBuffer));

                    //idle allapotba lepes
                    CommandState = Idle;
                    break;
            }
            break;
            
    }

    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if( USBIsDeviceSuspended()== true )
    {
        return;
    }
        

    /* Check to see if there is a transmission in progress, if there isn't, then
     * we can see about performing an echo response to data received.
     */
     if( USBUSARTIsTxTrfReady() == true) // ha a számítógép küld valami utasítást
    {
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer)); // readBuffer 64 bitet (byteot??) tud átvinni
        if (numBytesRead>0)
        {
            memset(writeBuffer,0,sizeof(writeBuffer));
     
            switch(readBuffer[0])
            {
                case 0x0A:
                    break;
                case 0x0D:
                    break;
                case '?':
                    strcpy((char *)writeBuffer,"uMogi2\r\n");
                    break;
                //a PC 't' betut kuld, ha szuksege van az aktualis homersekletre
                case 't':
                    CommandState = Initialization;      //kommunikacio inicializasa
                    ActualROMCommand = SkipROM;         //ROM parancsot atugorjuk
                    ActualFunctionCommand = Convert;    //elinditjuk a konverziot
                    break;
                default:
                    strcpy((char *)writeBuffer,"?\r\n");
                    break;
            }
            putsUSBUSART(writeBuffer); // nem küld ki ebben a pillanatban, azt egy transmission protocol küldi majd ki
        }
    } 

    CDCTxService(); // ekkor küldi ki
}

/*******************************************************************************
 End of File
*/
