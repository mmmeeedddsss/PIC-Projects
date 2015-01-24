/*
 * File:   newmain.c
 * Author: Mert
 *
 * Created on 12 Aral?k 2014 Cuma, 15:51
 */

#include <p18f4550.h>
#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

#define _XTAL_FREQ 20000000 //This is the speed the controller is running at
#pragma config PLLDIV = 5 , CPUDIV = OSC1_PLL2 , USBDIV = 2 , FOSC = INTOSCIO_EC	// You can write this way

// OR
#pragma config FCMEN = OFF		  // OR this way
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config BORV = 3
#pragma config VREGEN = OFF
#pragma config WDT = OFF
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF

void Delay1Second(void);
void DelayN10MS( int n );
void init();
void Out_D( int pin, int o );
void Out_B( int pin, int o );


void main(int argc, char** argv) {

    LATB0 = 1;
    LATB1 = 1;
    LATB2 = 1;
    LATB3 = 1;
    LATB4 = 1;
    LATB5 = 1;
    LATB6 = 1;
    LATB7 = 1;
    LATD0 = 0;
    LATD1 = 0;
    LATD2 = 0;
    LATD3 = 0;
    LATD4 = 0;
    LATD5 = 0;
    LATD6 = 0;
    LATD7 = 0;

    while(1)
    {
        init();
    }
}

void Out_D( int pin, int o )
{
    switch( pin )
    {
        case 0: LATD0 = o; break;
        case 1: LATD1 = o; break;
        case 2: LATD2 = o; break;
        case 3: LATD3 = o; break;
        case 4: LATD4 = o; break;
        case 5: LATD5 = o; break;
        case 6: LATD6 = o; break;
        case 7: LATD7 = o; break;
    }
}

void Out_B( int pin, int o )
{
    switch( pin )
    {
        case 0: LATB0 = o; break;
        case 1: LATB1 = o; break;
        case 2: LATB2 = o; break;
        case 3: LATB3 = o; break;
        case 4: LATB4 = o; break;
        case 5: LATB5 = o; break;
        case 6: LATB6 = o; break;
        case 7: LATB7 = o; break;
    }
}

void init()
{
    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    TRISBbits.RB5 = 0;
    TRISBbits.RB6 = 0;
    TRISBbits.RB7 = 0;
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    TRISDbits.RD3 = 0;
    TRISDbits.RD4 = 0;
    TRISDbits.RD5 = 0;
    TRISDbits.RD6 = 0;
    TRISDbits.RD7 = 0;

    LATB0 = 0;
    LATB1 = 0;
    LATB2 = 0;
    LATB3 = 0;
    LATB4 = 0;
    LATB5 = 0;
    LATB6 = 0;
    LATB7 = 0;
    LATD0 = 1;
    LATD1 = 1;
    LATD2 = 1;
    LATD3 = 1;
    LATD4 = 1;
    LATD5 = 1;
    LATD6 = 1;
    LATD7 = 1;

    Delay1Second();
}

void Delay1Second()
{
    int i=0;
    for(i=0;i<100;i++)
        __delay_ms(10);
}

void DelayN10MS( int n )
{
    int i=0;
    for(i=0;i<n;i++)
        __delay_ms(10);
}
