/*
 * File:   pwm.c
 * Author: Miswired
 *
 */

#include "pwm.h"
#include <xc.h>     // include processor files - each processor file is guarded. 
#include <stdint.h> //include C basic type definitions
#include "tools.h"

void pwm_init(void) 
{
    // Alternate Pin Function Control Register [PG110]
    APFCONbits.P2SEL = 0b1;       //Use PWM2 on pin RA4
    
    //--PWM Control Register [PG216]
    PWM2CONbits.EN = 0b1;       //PWM2 Enabled
    PWM2CONbits.OE = 0b1;       //PWM2 Output Pin Enabled
    PWM2CONbits.POL = 0b0;      //PMW2 Polarity Control active high
    PWM2CONbits.MODE = 0b00;    //PWM2 Standard Mode
    
    //--PWM Interrupt Enable Register [PG217]
    PWM2INTEbits.OFIE = 0b0;    //PWM2 Offset Interrupt Disabled
    PWM2INTEbits.PHIE = 0b0;    //PWM2 Phase Interrupt Disabled
    PWM2INTEbits.DCIE = 0b0;    //PWM2 Duty Cycle Interrupt Disabled
    PWM2INTEbits.PRIE = 0b0;    //PWM2 Period Interrupt Disabled
    
    //--PWM Interrupt Request Register [PG 218]
    //PWM2INTF Register to check if interrupts occurred [PG 218]
    
    //--PWM Clock Control Register [PG219]
    PWM2CLKCONbits.PS = 0b000;  //PWM2 clock source prescaler = none
    PWM2CLKCONbits.CS = 0b00;   //PWM2 clock source = FOSC 
    
    //--PWM Reload Trigger source Select Register [PG220]
    PWM2LDCONbits.LDT = 0b0;    //PWM2 Loads buffers end of period when LDA is set
    
    //--PWM offset trigger source Register [PG 221]
    PWM2OFCONbits.OFM = 00;     //PWM2 Independent Run mode
    
    //--PWM Phase Count Register [PG 222]
    PWM2PHH = 0x00;             //PWM2 Phase Offset High byte = 0
    PWM2PHL = 0x00;             //PWM2 Phase Offset Low byte = 0
    
    //--PWM Duty Cycle Count Register [PG 223]
    //--Note, no effect in Toggle on match mode
    PWM2DCH = 0x00;             //PWM2 Duty Cycle High byte = 0
    PWM2DCL = 0x00;             //PWM2 Duty Cycle Low byte = 0
    
    //--PWM Period Register [PG 224]
    PWM2PRH = 0x00;             //PWM2 Period Count High byte 
    PWM2PRL = 0xFF;             //PMW2 Period Count Low byte 
    
    //--PWM Offset Count Register [PG225]
    PWM2OFH = 0x00;             //PMW2 Offset Count High byte = 0
    PWM2OFL = 0x00;             //PWM2 Offset Count Low byte = 0
    
    //--PWM Timer Register [PG226]
    PWM2TMRH = 0x00;            //PWM2 Timer High Byte = 0
    PWM2TMRL = 0x00;            //PWM2 Timer Low Byte = 0
    
    //--PWM load value
    PWM2LDCONbits.LDA = 0b1;    //Load configuration
}


uint8_t pwm_set_duty_cycle_register(uint16_t dc)
{
    PWM2DCH = GET_HI_BYTE(dc);          //PWM2 Duty Cycle High byte
    PWM2DCL = GET_LOW_BYTE(dc);         //PWM2 Duty Cycle Low byte
    
    PWM2LDCONbits.LDA = 0b1;            //Load configuration
    
    return 0;
}
