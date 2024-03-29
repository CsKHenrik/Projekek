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

#include "system.h"

// PLL utani orajel
#define SYS_FREQ		32000000L
#define FCY				SYS_FREQ/2

// Masodlagos orajel forras - orakvarc
#define F_SEC_OSC		32768

// Delay fuggvenyek
#define DELAY_MS(ms)	__delay_ms(ms);
#define DELAY_US(us)	__delay_us(us);
#include <libpic30.h>   // __delay_ms

// Nyomogombok
#define SW1			!_RC1
#define SW2			!_RC3
#define SW3			!_RE8
#define SW4			!_RE9

// LED-ek
#define LED1		_LATG6
#define LED2		_LATG7
#define LED3		_LATG8
#define LED4		_LATG9
#define LEDR		_LATD15
#define LEDG		_LATF4
#define LEDB		_LATF5
#define R_LED()		{LEDR=1; LEDG=0; LEDB=0;}
#define G_LED()		{LEDR=0; LEDG=1; LEDB=0;}
#define B_LED()		{LEDR=0; LEDG=0; LEDB=1;}
