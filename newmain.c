/*
 * File:   newmain.c
 * Author: Viki
 * reads the voltage value from onboard ldr and displays it in lcd
 * and controls the motor 
 * Adc steps:www.microcontrollerboard.com/analog-to-digital-converter.html 
 */
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <pic18.h>
#include <pic18f452.h>
#include <htc.h>
#include "lcd.h"
#include "delay.h"
//Chip Settings
__PROG_CONFIG(1, 0x0200);
__PROG_CONFIG(2, 0X1E1F);
__PROG_CONFIG(3, 0X8100);
__PROG_CONFIG(4, 0X00C1);
__PROG_CONFIG(5, 0XC00F);

void WaitinMicroseconds(unsigned int delay) {
    for (; delay; delay--);
    DelayUs(100);
}

void WaitinMilliseconds(unsigned int delay) {
    for (; delay; delay--);
    DelayMs(100);
}

void ApproxDelay(unsigned long lValue) {
    unsigned long lIter;
    for (lIter = 0; lIter < 50000; lIter++)
        for (; lValue > 0; lValue--);
}

void MotorDriver_Settings() {
    //motor driver settings
    //www.best-microcontroller-projects.com/pwm-pic.html
    //pic mcu's have builtin pwm generators.setting up relevant control registers is enough.
    //for E1 pin of l293d
    TRISC1 = 0; //set rc1 as output(ccp2)
    TRISC2 = 0; //set rc2 as output(ccp1)
    PR2 = 0x01; //pwm period is set by settig value for PR2,clock oscillation period    //and prescaler value of timer2
    TMR2 = 0x01;
    CCPR2L = 0xFF; //duty cycle(pulse width) is specified by the ccpr1l register,clock oscillation    //period and prescaler value of timer2
    CCPR1L = 0xFF;
    CCP2X = 0;
    CCP2Y = 1;
    CCP1X = 0;
    CCP1Y = 1;
    T2CON = 0x04; //selects the prescaler option
    CCP2CON = CCP2CON | 0x0c; //ccp2con register controls the operation of ccp2
    CCP1CON = CCP1CON | 0x0c; //ccp1con register controls the operation of ccp1
    //Set rb4 and rb5 as output
    TRISB4 = 0; //rb4
    TRISB5 = 0; //rb5
    //set rb6 and rb7 as output
    TRISB6 = 0; //rb6
    TRISB7 = 0; //rb7
}

void LcdInitialize_Settings() {
    //Let the Module start up & Initialize the LCD Module
    WaitinMicroseconds(100);
    lcd_init(0x0);
    lcd_clear(); //Clear the Module
}

void Analog2Digital_Settings() {
    TRISA0 = 1;//an0 and an1 are input        
    TRISA1 = 1;
    ADCON1bits.PCFG0 = 1; //an0 and an1 are analog//c/r=6/0
    ADCON1bits.PCFG1 = 0;//
    ADCON1bits.PCFG2 = 0;
    ADCON1bits.PCFG3 = 1;
    ADCON0bits.ADCS0 = 0; //fosc/32
    ADCON0bits.ADCS1 = 1;
}

void main() {
    unsigned int iLeftVolt = 0x00, iRightVolt = 0x00;
    MotorDriver_Settings();
    Analog2Digital_Settings();
    LcdInitialize_Settings();
    for (;;) {
        ADCON0bits.CHS0 = 0; //an0 is selected channel
        ADCON0bits.CHS1 = 0;
        ADCON0bits.CHS2 = 0;
        ADCON0bits.ADON = 1; //a/d converter module is powered up
        ADCON0bits.GO = 1; //start the a/d conversion
        while (ADCON0bits.GO); //wait until the conversion is complete
        ADCON1bits.ADFM = 0; //left justified
        iLeftVolt = ADRESH; //read value from ldr
        //Write voltage
        lcd_putsTwoint(iLeftVolt, iRightVolt);
        WaitinMilliseconds(100000);
        lcd_clear();
        if (iLeftVolt > 200) {//left ldr controls right wheel
            RB4 = 0; //front rotation
            RB5 = 1;          
        } else {        
            RB4 = 0; //stop rotation
            RB5 = 0;       
        }
        ADCON0bits.CHS0 = 1; //an1 is selected channel
        ADCON0bits.CHS1 = 0;
        ADCON0bits.CHS2 = 0;
        ADCON0bits.ADON = 1; //a/d converter module is powered up
        ADCON0bits.GO = 1; //start the a/d conversion
        while (ADCON0bits.GO); //wait until the conversion is complete
        ADCON1bits.ADFM = 0; //left justified
        iRightVolt = ADRESH; //read value from ldr
        //Write voltage
        lcd_putsTwoint(iLeftVolt, iRightVolt);
        WaitinMilliseconds(100000);
        lcd_clear();
        if (iRightVolt > 200) {//right ldr controls left wheel
            RB6 = 1; //front rotation
            RB7 = 0;
        } else {
            RB6 = 0; //stop rotation
            RB7 = 0;
        }
    }             
}