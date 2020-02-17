/*
 * File:   main.c
 * Author: Miswired
 *
 */

//Notes
    //Set A0 to output and turn high
    //TRISA0 = 0;
    //RA0 = 1;
    // or
    //TRISAbits.TRISA0 = 0;
    //PORTAbits.RA0 = 1;
    //Blocking Delays
    //__delay_ms(1000);

// PIC12F1572 Configuration Bit Settings

// CONFIG1
#pragma config FOSC = INTOSC    //  (INTOSC oscillator; I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)(LVP must be OFF)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOREN = OFF    // Low Power Brown-out Reset enable bit (LPBOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)


// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>     // include processor files - each processor file is guarded. 
#include <stdint.h> //include C basic type definitions
#include "pwm.h"
#include "portal_7813hz8.h" //include sound


//Common State Names
#define HIGH                1u
#define LOW                 0u
#define INPUT               1u
#define OUTPUT              0u
#define PULLUP_ENABLED      1u
#define PULLUP_DISABLED     0u
#define TRUE                1u
#define FALSE               0u
#define DIGITAL             0u
#define ANALOG              1u

#define MAX_REPLAYS         3u

#define TRIGGER_1_PIN       PORTAbits.RA2   
#define MODE_SELECTION_PIN  PORTAbits.RA3

void shift_in_sound(void);

// Global Variables
uint32_t g_sound_timer = 0;
uint32_t g_sample = 0;
uint8_t  g_play_counter = 0;
uint8_t  g_replay_counter = 0;
uint8_t  g_target_replays = 1;

void setup(void)
{
    //Set clock speed
    OSCCONbits.SPLLEN = 0b1;    //PLL enabled
    OSCCONbits.IRCF = 0b1111;   //16 MHz
    OSCCONbits.SCS = 0b10;      //Internal clock source
    
    //Define FOSC frequency for macros
    #define _XTAL_FREQ 16000000
    
    //Set Initial Port Configs
    //--Pullups
    WPUAbits.WPUA1 = PULLUP_DISABLED;
    
    //--Directions
    TRISAbits.TRISA0 = OUTPUT;
    TRISAbits.TRISA1 = OUTPUT;
    TRISAbits.TRISA2 = INPUT;   //Configuration pin
    TRISAbits.TRISA3 = INPUT;   //MCLR I/O Input Only
    TRISAbits.TRISA4 = OUTPUT;
    TRISAbits.TRISA5 = OUTPUT;
    
    ANSELAbits.ANSA2 = DIGITAL; //Set input mode to digital

    
    //--Initial States
    PORTAbits.RA0 = LOW;
    PORTAbits.RA1 = LOW;
    //PORTAbits.RA2 = LOW;
    PORTAbits.RA4 = LOW;
    PORTAbits.RA5 = LOW;
    
    //PWM Setup
    pwm_init();
    
    
    
    //Interrupts
    
    //--Timer 0
    OPTION_REGbits.TMR0CS = 0u;     //Timer0 Internal Clock Source
    OPTION_REGbits.PSA = 0u;        //Set Timer0 prescaler Enable
    OPTION_REGbits.PS = 0b111;      //Need timer of 7813Hz to match g_sample rate
                                    //Div clock by 128 ~= 2 ticks
    INTCONbits.TMR0IE = 1u;         //Turn off Timer0 Interrupt to start with
    
    //--Global
    INTCONbits.GIE = 1;             //Turn on global Interrupt
    
    if(MODE_SELECTION_PIN == HIGH)
    {
        g_target_replays = MAX_REPLAYS;
    }
    else
    {
        g_target_replays = 1;
    }
}

void main(void) 
{
    setup();
    
    while(1)
    {
        if(TRIGGER_1_PIN == HIGH)
        {
            INTCONbits.TMR0IE = 1u;
        }
        
    }
    
    return;
}


//Interrupt (Single vector on this chip)
void __interrupt ()  tc_int  (void)
{
    if(INTCONbits.TMR0IF && INTCONbits.TMR0IE) 
    {                                       // if timer flag is set & interrupt enabled
        TMR0 -= 2u;                       // reload the timer = 7813 hz
        INTCONbits.TMR0IF = 0;              // clear the interrupt flag 
        
        // Manage Sound
        if(g_sound_timer > 0)
        {
            //If we are still waiting to play the clip, decrement
            g_sound_timer--;
        }
        else
        {
            //Ready to play clip, shift in the next PWM value
            shift_in_sound();
        }
    }

}

//Really don't like doing so much in an interrupt, but since it's ALL the chip
//does, and it is quick, it should be fine.
void shift_in_sound(void)
{
    if(g_sample < SOUND_SIZE)
    {
        //Write duty cycle to register
        pwm_set_duty_cycle_register(portal[g_sample]);
        g_sample++;
    }
    else
    {
        //If we reached the end of the sample
        //Increment play counter, turn output off, and reset sample to 0
        g_play_counter++;
        pwm_set_duty_cycle_register(0x0000);
        g_sample=0;
        //If we haven't played three times yet, short delay
        //If we have, long delay
        if (g_play_counter < 3)
        {
            //Pause between short clip ~80ms
            g_sound_timer = 625u;
        }
        else
        {
            //Pause for full sound
            g_sound_timer = 7813u;
            g_play_counter = 0;
            g_replay_counter++;
        }

    }
    
    //If we replayed the whole audio enough times, stop
    if(g_replay_counter >= g_target_replays)
    {
        g_replay_counter = 0;
        INTCONbits.TMR0IE = 0u;
    }
}